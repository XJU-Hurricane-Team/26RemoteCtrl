/**
 * @file    rc_msg_process.c
 * @author  PickingChip,KkarinL15
 * @brief   遥控器数据处理并发送
 * @version 0.1
 * @date    2026-03-27
 */

#include "includes.h"
#include "my_math/my_math.h"
#include "tactical_points.h"
/* 发送时间间隔, 单位: ms */
#define REMOTE_SEND_PERIOD 10

/* UI 消息队列深度 */
#define UI_MSG_QUEUE_LEN   8

/* 遥控器触发回调按键数量 */
#define REMOTE_KEY_NUM     18

/* 屏幕总数量 */
#define SCREEN_TOTALNUM    2

#define KEY_EVENT_CB(key, event)                                               \
    do {                                                                       \
        if ((key >= 1) && (key <= REMOTE_KEY_NUM) &&                           \
            key_callback[key - 1][event]) {                                    \
            key_callback[key - 1][event](key, event);                          \
        }                                                                      \
    } while (0)

/* 发送任务句柄 */
static TaskHandle_t remote_send_task_handle;
/* 传递给屏幕的消息队列 */
QueueHandle_t ui_msg_queue = NULL;
/* 按键回调函数数组 */
static remote_key_callback_t key_callback[REMOTE_KEY_NUM][REMOTE_KEY_EVENT_NUM];
/* UI 消息序列号 */
static uint32_t ui_msg_seq = 0;
static int8_t screen = 1;
static uint8_t sub_mode = SUB_MODE_OFF; /*!< 红/蓝区子模式状态 */
static uint8_t MSG_MODES = 0; /*!< 消息模式, 0: 普通模式, 1: 跑点模式 */

/**
 * @brief UI 消息发布函数
 *
 * @param msg
 */
static void ui_publish_msg(const ui_msg_t *msg) {
    if ((ui_msg_queue == NULL) || (msg == NULL)) {
        return;
    }

    if (xQueueSend(ui_msg_queue, msg, 0U) != pdPASS) {
        ui_msg_t dropped = {0};
        /* 满了就读取最旧的存入最新的 */
        (void)xQueueReceive(ui_msg_queue, &dropped, 0U);
        (void)xQueueSend(ui_msg_queue, msg, 0U);
    }
}

/**
 * @brief 遥感值修正函数
 *
 * @param adc_value ADC 读取值
 * @return int8_t
 */
static int8_t joystick_set_value(uint32_t adc_value) {
    int32_t value = 20 - (int32_t)adc_value;

    my_limit(value, -20, 20);

    return (int8_t)value;
}

/**
 * @brief 负责将数据发送到主控和UI
 *
 * @param pvParameters
 */
static void remote_send_task(void *pvParameters) {
    UNUSED(pvParameters);

    uint8_t last_key = 0;
    uint32_t rs_adc_buf[5] = {0};
    remote_send_data_t remote_send_data = {0};
    TickType_t last_wake_time = xTaskGetTickCount();
    TickType_t led_time = xTaskGetTickCount();

    ui_msg_queue = xQueueCreate(UI_MSG_QUEUE_LEN, sizeof(ui_msg_t));
    if (ui_msg_queue == NULL) {
        vTaskDelete(NULL);
    }

    while (1) {
        uint8_t keyboard = keyboard_scan();
        uint8_t add_key = add_key_scan(1);
        uint8_t ctrl_key = ctrl_key_scan(0);
        uint8_t ctrl_key_for_ui = ctrl_key;

        /* screen / 子模式 切换 */
        if (ctrl_key == WHE_L_TURNUP) {
            if (sub_mode == SUB_MODE_BLUE) {
                sub_mode = SUB_MODE_OFF;        /* 蓝区子模式: 反向退出 */
                ctrl_key_for_ui = KEY_NO_PRESS;
            } else if (sub_mode == SUB_MODE_RED) {
                ctrl_key_for_ui = KEY_NO_PRESS; /* 红区子模式同方向: 吞掉, 不下钻 */
            } else if (screen == 0) {
                sub_mode = SUB_MODE_RED;        /* 红区边界上越界: 进子模式 */
                ctrl_key_for_ui = KEY_NO_PRESS;
            } else {
                screen--;
            }
        } else if (ctrl_key == WHE_L_TURNDO) {
            if (sub_mode == SUB_MODE_RED) {
                sub_mode = SUB_MODE_OFF;        /* 红区子模式: 反向退出 */
                ctrl_key_for_ui = KEY_NO_PRESS;
            } else if (sub_mode == SUB_MODE_BLUE) {
                ctrl_key_for_ui = KEY_NO_PRESS; /* 蓝区子模式同方向: 吞掉 */
            } else if (screen == 2) {
                sub_mode = SUB_MODE_BLUE;       /* 蓝区边界下越界: 进子模式 */
                ctrl_key_for_ui = KEY_NO_PRESS;
            } else {
                screen++;
            }
        }
        my_limit(screen, 0, SCREEN_TOTALNUM);
        if (screen == 1) {
            sub_mode = SUB_MODE_OFF;            /* 跨进 info 强制清子模式 */
        }

        /* 点数/消息选择: irdamsg 仅在 info 屏由右波轮维护, 其他屏强制清零 */
        if (screen == 1) {
            remote_send_data.point        = 0;
            remote_send_data.irdamsg      = get_irda_msg(ctrl_key);
            remote_send_data.tactical_idx = 0;
        } else if (sub_mode != SUB_MODE_OFF) {
            remote_send_data.point        = 0;
            remote_send_data.irdamsg      = 0;
            remote_send_data.tactical_idx = get_tactical_point(ctrl_key);
        } else {
            remote_send_data.point        = get_point_value(ctrl_key);
            remote_send_data.irdamsg      = 0;
            remote_send_data.tactical_idx = 0;
        }
        remote_send_data.sub_mode = sub_mode;

        rs_get_value(rs_adc_buf, 10, 40);

        uint8_t mV = (uint8_t)(rs_adc_buf[4] & 0xFF); /* 电压 */
        if (mV <= 35) {
            LED1_ON();
        } else {
            LED1_OFF();
        }

        /* 模式选择 (模式1优先级高于模式2) */
        MSG_MODES = NORMAL_MODE;
        switch (add_key) {
            case KEY_LZ_PRESS: keyboard = 49; break;
            case KEY_RZ_PRESS: keyboard = 50; break;
            case KEY_TL_PRESS:
                MSG_MODES = CHANGE_TO_MODE1;
                if (keyboard) keyboard += 16;
                break;
            case KEY_TR_PRESS:
                MSG_MODES = CHANGE_TO_MODE2;
                if (keyboard) keyboard += 32;
                break;
        }

        /* 填充发送数据 */
        remote_send_data.key = keyboard;
        remote_send_data.rs[0] = joystick_set_value(rs_adc_buf[2]); /* 左 x */
        remote_send_data.rs[1] = joystick_set_value(rs_adc_buf[3]); /* 左 y */
        remote_send_data.rs[2] = joystick_set_value(rs_adc_buf[0]); /* 右 x */
        remote_send_data.rs[3] = joystick_set_value(rs_adc_buf[1]); /* 右 y */

        message_send_data(MSG_RC_TO_MASTER, MSG_MODES, (uint8_t *)&remote_send_data,
                          sizeof(remote_send_data));

        /* UI 消息 */
        ui_msg_t ui_msg = {
            .type = UI_REMOTE_CTRL,
            .seq = ++ui_msg_seq,
            .payload.remote_ctrl =
                {.ctrl_key = ctrl_key_for_ui, .voltage = mV, .data = remote_send_data}};
        ui_publish_msg(&ui_msg);

        /* 判断按键状态并触发按键回调 */
        uint8_t current_key = remote_send_data.key;
        if (current_key != last_key) {
            if (last_key != 0) {
                /* 上一个按键抬起 */
                KEY_EVENT_CB(last_key, REMOTE_KEY_PRESS_UP);
            }
            if (current_key != 0) {
                /* 当前按键按下 */
                KEY_EVENT_CB(current_key, REMOTE_KEY_PRESS_DOWN);
            }
        } else {
            if (current_key != 0) {
                /* 当前按键长按 */
                KEY_EVENT_CB(current_key, REMOTE_KEY_PRESSING);
            }
        }
        last_key = current_key;

        /* LED2 绿色灯闪烁判断消息发送是否正常 */
        if (xTaskGetTickCount() - led_time > 200) {
            led_time = xTaskGetTickCount();
            LED2_TOGGLE();
        }

        vTaskDelayUntil(&last_wake_time, REMOTE_SEND_PERIOD);
    }
}

/**
 * @brief 遥控器发送初始化
 *
 * @param send_uart 发送数据的串口
 */
void remote_send_init(UART_HandleTypeDef *send_uart) {
    if (send_uart == NULL) {
        return;
    }

    message_register_send_uart(MSG_RC_TO_MASTER, send_uart, 20);
    xTaskCreate(remote_send_task, "send task", 512, NULL, 3,
                &remote_send_task_handle);
}

/**
 * @brief 遥控器键盘注册回调函数
 *
 * @param key 按键
 * @param event 事件
 * @param callback 事件回调函数
 */
void remote_register_key_callback(uint8_t key, remote_key_event_t event,
                                  remote_key_callback_t callback) {
    if (key > 18) {
        return;
    }

    if (event >= REMOTE_KEY_EVENT_NUM) {
        return;
    }

    key_callback[key - 1][event] = callback;
}

/**
 * @brief 遥控器键盘取消注册回调函数
 *
 * @param key 按键
 * @param event 事件
 * @param callback 事件回调函数
 */
void remote_unregister_key_callback(uint8_t key, remote_key_event_t event) {
    if (key > 18) {
        return;
    }

    if (event >= REMOTE_KEY_EVENT_NUM) {
        return;
    }

    key_callback[key - 1][event] = NULL;
}

/**
 * @brief 主板接收数据回调函数
 *
 * @param msg_length 消息帧长度
 * @param msg_id_type 消息 ID 和数据类型 (高四位为 ID, 低四位为数据类型)
 * @param[in] msg_data 消息数据接收区
 */
void remote_recv_msg_callback(uint32_t msg_length, uint8_t msg_id_type,
                              uint8_t *msg_data) {
    static uint32_t led_time = 0; /*!< 控制 LED 闪烁频率 */
    report_data_t report_data = {0};
    ui_msg_t ui_msg = {0};

    if ((msg_data == NULL) || ((msg_id_type >> 4) != MSG_MASTER_TO_RC)) {
        return;
    }

    if ((msg_id_type & 0x0F) != NORMAL_MODE) {
        return;
    }

    if (msg_length != sizeof(report_data_t)) {
        return;
    }

    memcpy(&report_data, msg_data, sizeof(report_data_t));

    ui_msg.type = UI_R1_STATE;
    ui_msg.seq = ++ui_msg_seq;
    memcpy(&ui_msg.payload.r1_state, &report_data.r1_state, sizeof(r1_data_t));
    ui_publish_msg(&ui_msg);

    /* LED3 闪烁判断消息接收是否正常 */
    if (HAL_GetTick() - led_time > 200) {
        led_time = HAL_GetTick();
        LED3_TOGGLE();
    }
}
