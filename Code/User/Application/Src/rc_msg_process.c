/**
 * @file    rc_msg_process.c
 * @author  PickingChip,KkarinL15
 * @brief   遥控器数据处理并发送
 * @version 0.1
 * @date    2026-03-27
 */

#include "includes.h"

/* 发送时间间隔, 单位: ms */
#define REMOTE_SEND_PERIOD 10

/* UI 消息队列深度 */
#define UI_MSG_QUEUE_LEN   8

/* 遥控器触发回调按键数量 */
#define REMOTE_KEY_NUM     18

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

    if (value > 20) {
        value = 20;
    } else if (value < -20) {
        value = -20;
    }

    return (int8_t)value;
}

/**
 * @brief 负责将数据发送到主控和UI
 * 
 * @param pvParameters 
 */
static void remote_send_task(void *pvParameters) {
    UNUSED(pvParameters);

    uint8_t mV = 0;
    uint8_t last_key = 0;
    uint8_t ctrl_key = 0;
    uint8_t keyboard_value = 0;
    uint32_t rs_adc_buf[5] = {0};

    remote_send_data_t remote_send_data = {0};
    ui_msg_t ui_msg = {0};

    TickType_t last_wake_time = xTaskGetTickCount(); /*!< 保证发送周期固定 */
    TickType_t led_time = xTaskGetTickCount();       /*!< 控制 LED 闪烁频率 */

    ui_msg_queue = xQueueCreate(UI_MSG_QUEUE_LEN, sizeof(ui_msg_t));
    if (ui_msg_queue == NULL) {
        vTaskDelete(NULL);
    }

    while (1) {

        keyboard_value = keyboard_scan();
        if (add_key_scan(1) != KEY_NO_PRESS)
        {
            keyboard_value = add_key_scan(1);
        }
        
        rs_get_value(rs_adc_buf, 10, 40);

        mV = (uint8_t)(rs_adc_buf[4] & 0xFF); /* 电压 */
        if (mV <= 35)
        {
            LED1_ON();
        }else{
            LED1_OFF();
        }
        
        /* 控制按键有更高的优先级 */
        remote_send_data.key = (uint8_t)keyboard_value;
        remote_send_data.rs[2] = joystick_set_value(rs_adc_buf[0]); /* 右 x */
        remote_send_data.rs[3] = joystick_set_value(rs_adc_buf[1]); /* 右 y */
        remote_send_data.rs[0] = joystick_set_value(rs_adc_buf[2]); /* 左 x */
        remote_send_data.rs[1] = joystick_set_value(rs_adc_buf[3]); /* 左 y */

        message_send_data(MSG_RC_TO_MASTER, MSG_MODES,
                          (uint8_t *)&remote_send_data,
                          sizeof(remote_send_data));

        ui_msg.type = UI_REMOTE_CTRL;
        ui_msg.seq = ++ui_msg_seq;
        ui_msg.payload.remote_ctrl.ctrl_key = ctrl_key;
        ui_msg.payload.remote_ctrl.voltage = mV;
        ui_msg.payload.remote_ctrl.data = remote_send_data;
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
// void remote_recv_msg_callback(uint32_t msg_length, uint8_t msg_id_type,
//                               uint8_t *msg_data) {
//     static uint32_t led_time = 0; /*!< 控制 LED 闪烁频率 */
//     report_data_t report_data = {0};
//     ui_msg_t ui_msg = {0};

//     if ((msg_data == NULL) || ((msg_id_type >> 4) != MSG_MASTER_TO_RC)) {
//         return;
//     }

//     if ((msg_id_type & 0x0F) != NORMAL_MODE) {
//         return;
//     }

//     if (msg_length != sizeof(report_data_t)) {
//         return;
//     }

//     memcpy(&report_data, msg_data, sizeof(report_data_t));

//     ui_msg.type = UI_R1_STATE;
//     ui_msg.seq = ++ui_msg_seq;
//     memcpy(&ui_msg.payload.r1_state, &report_data.r1_state, sizeof(r1_data_t));
//     ui_publish_msg(&ui_msg);

//     ui_msg.type = UI_R2_STATE;
//     ui_msg.seq = ++ui_msg_seq;
//     memcpy(&ui_msg.payload.r2_state, &report_data.r2_state, sizeof(r2_data_t));
//     ui_publish_msg(&ui_msg);

//     /* LED3 闪烁判断消息接收是否正常 */
//     if (HAL_GetTick() - led_time > 200) {
//         led_time = HAL_GetTick();
//         LED3_TOGGLE();
//     }
// }