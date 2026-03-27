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

#include "app_touchgfx.h"

#include <stdio.h>
#include <string.h>

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

/**
 * @brief 上报给UI的消息类型
 * 
 */
typedef enum {
    UI_REMOTE_CTRL = 0x00U, /*!< 遥控器控制消息 */
    UI_R1_STATE,            /*!< R1 状态消息 */
    UI_R2_STATE,            /*!< R2 状态消息 */
    MSG_TYPE_NUM             /*!< 保留长度 */
} ui_msg_type_t;

/**
 * @brief 遥控器键盘回调函数
 * 
 * @param key 按键
 * @param event 事件
 */
typedef void (*remote_key_callback_t)(uint8_t key, remote_key_event_t event);

/**
 * @brief R1 状态数据
 * 
 */
typedef struct __packed {
    int16_t x_speed; /*!< x 坐标 */
    int16_t y_speed; /*!< y 坐标 */
    int16_t angle;   /*!< yaw 坐标 */

    /*!< R1 状态 
     * bit[7:2] 保留
     * bit[1]   是否在自瞄状态
     * bit[0]   是否世界坐标系
     */
    uint8_t r1_status;
} r1_data_t;

/* 遥控器发送数据结构 */
typedef struct __packed {
    int8_t key;   /*!< 按键值 */
    int8_t rs[4]; /*!< 摇杆, 左 x, 左 y, 右 x, 右 y */
} remote_send_data_t;

/* 遥控器控制消息结构 */
typedef struct {
    uint8_t voltage;          /*!< 电压值 */
    remote_send_data_t *data; /*!< 遥控器发送数据指针 */
} remote_ctrl_msg_t;

/* R1 状态消息结构 */
typedef struct{
    r1_data_t *data;
} r1_state_msg_t;

typedef struct {
    ui_msg_type_t type;          /*!< 消息类型 */
    void *data;              /*!< 消息数据指针 */
} ui_msg_t;



extern QueueHandle_t ui_msg_queue;
extern QueueHandle_t speed_data_queue;

/* 遥控器数据发送任务API */
void remote_send_init(UART_HandleTypeDef *send_uart);
void remote_recv_msg_callback(uint32_t msg_length, uint8_t msg_id_type,
                              uint8_t *msg_data);
void remote_register_key_callback(uint8_t key, remote_key_event_t event,
                                  remote_key_callback_t callback);
void remote_unregister_key_callback(uint8_t key, remote_key_event_t event);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __INCLUDES_H */
