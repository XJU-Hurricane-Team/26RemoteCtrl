/**
 * @file    recv_data.c
 * @author  Deadline039
 * @brief   主板数据接收
 * @version 0.1
 * @date    2025-04-22
 */

#include "includes.h"
#include "string.h"

static r1_data_t recv_r1_data;
QueueHandle_t speed_data_queue;

/**
 * @brief 接收数据内容
 */
typedef enum __packed {
    REMOTE_REPORT_SPEED, /*!< 上报速度信息 */
} remote_report_data_t;

/**
 * @brief 主板接收数据回调函数
 * 
 * @param msg_length 消息帧长度
 * @param msg_id_type 消息 ID 和数据类型 (高四位为 ID, 低四位为数据类型)
 * @param[in] msg_data 消息数据接收区
 */
void remote_recv_msg_callback(uint32_t msg_length, uint8_t msg_id_type,
                              uint8_t *msg_data) {
    UNUSED(msg_length);
    static uint32_t led_time = 0; /*!< 控制 LED 闪烁频率 */
    ui_msg_t ui_msg = {0};
    r1_state_msg_t r1_state_msg = {0};

    if ((msg_id_type >> 4) != MSG_TO_REMOTE) {
        return;
    }

    switch (msg_data[0]) {
        case UI_R1_STATE: {
            memcpy(&recv_r1_data, &msg_data[1], sizeof(recv_r1_data));

            r1_state_msg.data = &recv_r1_data;

            ui_msg.type = UI_R1_STATE;
            ui_msg.data = &r1_state_msg;
            /* 这里将数据入队列 */
            xQueueOverwrite(ui_msg_queue, &ui_msg);
        } break;

        default:
            return;
    }

    /* LED3 闪烁判断消息接收是否正常 */
    if (HAL_GetTick() - led_time > 200) {
        led_time = HAL_GetTick();
        LED3_TOGGLE();
    }
}

// /**
//  * @brief 主板接收数据回调函数
//  *
//  * @param msg_length 消息帧长度
//  * @param msg_id_type 消息 ID 和数据类型 (高四位为 ID, 低四位为数据类型)
//  * @param[in] msg_data 消息数据接收区
//  */
// void remote_recv_msg_callback(uint32_t msg_length, uint8_t msg_id_type,
//                                 uint8_t *msg_data) {
//     UNUSED(msg_length);
//     if ((msg_id_type >> 4) != MSG_TO_MASTER) {
//         return;
//     }

//     switch (msg_data[0]) {
//         case REMOTE_REPORT_SPEED: {
//             memcpy(&recv_speed_data, &msg_data[1],
//                    sizeof(recv_speed_data));
//             LED3_TOGGLE();
//             /* 这里将数据入队列 */
//             xQueueOverwrite(speed_data_queue, &recv_speed_data);
//         } break;

//         default:
//             return;
//     }
// }