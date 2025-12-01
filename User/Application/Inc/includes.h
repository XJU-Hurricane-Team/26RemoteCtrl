/**
 * @file    includes.h
 * @author  Deadline039
 * @brief   Include files
 * @version 1.0
 * @date    2024-04-03
 */

#ifndef __INCLUDES_H
#define __INCLUDES_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include <bsp.h>

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "queue.h"

#include "./Modules/msg_protocol.h"
#include <math.h>

#include <stdio.h>

/**
 * @brief 遥控器键盘事件
 * @note 大部分回调注册使用按键按下普通事件
 */
typedef enum {
    REMOTE_KEY_PRESS_DOWN, /*!< 按键按下 */
    REMOTE_KEY_PRESSING,   /*!< 按键长按 */
    REMOTE_KEY_PRESS_UP,   /*!< 按键抬起 */
    REMOTE_KEY_EVENT_NUM   /*!< 保留长度 */
} remote_key_event_t;

typedef enum {
    ORIGIN_SET_INT = 1, /*!< 进入原点设置 */
    ORIGIN_SET_LAST,
    ORIGIN_SET_NEXT, /*!< 下一个原点 */
    ORIGIN_SET_DIRECTION,
    ORIGIN_SET_OUT   /*!< 退出原点设置  */
} origin_key_t;

/**
 * @brief 遥控器键盘回调函数
 * 
 * @param key 按键
 * @param event 事件
 */
typedef void (*remote_key_callback_t)(uint8_t key, remote_key_event_t event);

/**
 * @brief 底盘速度队列数据
 * 
 */
typedef struct __packed {
    int16_t x_speed;     /*!< x 坐标 */
    int16_t y_speed;     /*!< y 坐标 */
    int16_t angle; /*!< yaw 坐标 */

    /*!< 底盘状态 
     * bit[7:2] 保留
     * bit[1]   是否在自瞄状态
     * bit[0]   是否世界坐标系
     */
    uint8_t chassis_status;
} speed_data_t;

extern QueueHandle_t SampleQueue;
extern QueueHandle_t speed_data_queue;

/* 遥控器数据发送任务API */
void remote_send_init(UART_HandleTypeDef *send_uart);
void remote_report_msg_callback(uint32_t msg_length, uint8_t msg_id_type,
                                uint8_t *msg_data);
void remote_register_key_callback(uint8_t key, remote_key_event_t event,
                                  remote_key_callback_t callback);
void remote_unregister_key_callback(uint8_t key, remote_key_event_t event);


void freertos_start(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __INCLUDES_H */
