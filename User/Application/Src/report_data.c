/**
 * @file    report_data.c
 * @author  Deadline039
 * @brief   主板数据上报
 * @version 0.1
 * @date    2025-04-22
 */

#include "includes.h"

static speed_data_t recv_speed_data;
QueueHandle_t speed_data_queue;

/**
 * @brief 上报数据内容
 */
typedef enum __packed {
    REMOTE_REPORT_SPEED, /*!< 上报速度信息 */
} remote_report_data_t;

/**
 * @brief 主板上报数据回调函数
 * 
 * @param msg_length 消息帧长度
 * @param msg_id_type 消息 ID 和数据类型 (高四位为 ID, 低四位为数据类型)
 * @param[in] msg_data 消息数据接收区
 */
void remote_report_msg_callback(uint32_t msg_length, uint8_t msg_id_type,
                                uint8_t *msg_data) {
    UNUSED(msg_length);
    if ((msg_id_type >> 4) != MSG_TO_MASTER) {
        return;
    }

    switch (msg_data[0]) {
        case REMOTE_REPORT_SPEED: {
            memcpy(&recv_speed_data, &msg_data[1],
                   sizeof(recv_speed_data));
            LED3_TOGGLE();
            /* 这里将数据入队列 */
            remote_report_data_t recv_speed_data = REMOTE_REPORT_SPEED;
            xQueueOverwrite(speed_data_queue, &recv_speed_data);
        } break;

        default:
            return;
    }
}