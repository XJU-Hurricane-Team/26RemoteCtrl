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
#include <stdbool.h>
#include <stdint.h>

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
    MSG_TYPE_NUM            /*!< 保留长度 */
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
    int16_t x_speed; /*!< x 方向速度 */
    int16_t y_speed; /*!< y 方向速度 */
    int16_t w_speed; /*!< 角速度 */
    uint8_t r1_chassis_status;
    float left_pos;    /*!< 左侧抬升高度 */
    float right_pos;   /*!< 右侧抬升高度 */
    bool left_adsorbed;  /*!< 左侧是否吸住 */
    bool right_adsorbed; /*!< 右侧是否吸住 */
    uint8_t rec_msg;     /*!< 接收到R2传的数据 */
    uint8_t send_msg;    /*!< 发送给R2的数据 */
} r1_data_t;

/**
 * @brief R2 状态数据
 */
typedef struct __packed {
    int16_t x_speed; /*!< x 方向速度 */
    int16_t y_speed; /*!< y 方向速度 */
    int16_t angle;   /*!< yaw 大小 */

    /*!< R2 状态
     * bit[7:0] 保留, 后续按协议扩展
     */
    uint8_t r2_status;
} r2_data_t;

/**
 * @brief 上报的数据包
 * 
 */
typedef struct __packed {
    r1_data_t r1_state;
    r2_data_t r2_state;
} report_data_t;

/* 遥控器发送数据结构 */
typedef struct __packed {
    int8_t key;   /*!< 按键值 */
    int8_t rs[4]; /*!< 摇杆, 左 x, 左 y, 右 x, 右 y */
    int8_t point; /*!< 跑点位置 */
} remote_send_data_t;

/* 遥控器控制消息结构 */
typedef struct {
    uint8_t voltage;         /*!< 电压值 */
    int8_t ctrl_key; /*!< 控制按键值 */
    remote_send_data_t data; /*!< 遥控器发送数据 */
} remote_ctrl_msg_t;

/* UI 消息载荷 */
typedef union {
    remote_ctrl_msg_t remote_ctrl;
    r1_data_t r1_state;
    r2_data_t r2_state;
} ui_msg_payload_t;

typedef struct {
    ui_msg_type_t type;       /*!< 消息类型 */
    uint32_t seq;             /*!< 发布序号, 用于调试丢包与乱序 */
    ui_msg_payload_t payload; /*!< 消息数据 */
} ui_msg_t;

extern QueueHandle_t ui_msg_queue;

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
