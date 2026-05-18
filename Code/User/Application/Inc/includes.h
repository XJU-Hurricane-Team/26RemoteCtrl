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
    REMOTE_KEY_EVENT_NUM
} remote_key_event_t;

/**
 * @brief 上报给UI的消息类型
 *
 */
typedef enum {
    UI_REMOTE_CTRL = 0x00U, /*!< 遥控器控制消息 */
    UI_R1_STATE,            /*!< R1 状态消息 */
    UI_R2_STATE,            /*!< R2 状态消息 */
    UI_SCREEN_STATE,        /*!< 屏变化 (screen + preset_idx) */
    MSG_TYPE_NUM
} ui_msg_type_t;

/**
 * @brief 屏索引 (中心对称: 负=红, 正=蓝, 0=info)
 * @note  物理 TouchGFX 屏只有 3 张, 子模式叠加在地图屏上无独立屏
 */
typedef enum {
    SCREEN_RED_SUB  = -2, /*!< 红区子模式 (叠加在 redmap 屏) */
    SCREEN_RED_MAP  = -1, /*!< 红图屏 */
    SCREEN_INFO     =  0, /*!< Info 屏 */
    SCREEN_BLUE_MAP =  1, /*!< 蓝图屏 */
    SCREEN_BLUE_SUB =  2  /*!< 蓝区子模式 (叠加在 bluemap 屏) */
} screen_t;

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
    int16_t x_speed;
    int16_t y_speed;
    int16_t w_speed;
    uint8_t r1_chassis_status;
    uint8_t r1_chassis_state;
    float accel_xy;
    float left_pos;
    float right_pos;
    bool left_adsorbed;
    bool right_adsorbed;
    uint8_t rec_msg;
    uint8_t send_msg;
    uint8_t yaw_source;
} r1_data_t;

/**
 * @brief 上报的数据包
 *
 */
typedef struct __packed {
    r1_data_t r1_state;
} report_data_t;

/* 遥控器发送数据结构 */
typedef struct __packed {
    int8_t key;    /*!< 1..16 (矩阵键), 49/50 = LZ/RZ, 51+ = 固定点 */
    int8_t rs[4];  /*!< [0]=左x, [1]=左y, [2]=右x, [3]=右y */
    int8_t point;  /*!< A* 跑点位置 */
    int8_t irdamsg;
} remote_send_data_t;

/* 遥控器控制消息结构 */
typedef struct {
    uint8_t voltage;
    int8_t ctrl_key;
    remote_send_data_t data;
} remote_ctrl_msg_t;

/* UI 消息载荷 */
typedef struct {
    int8_t  screen;
    uint8_t preset_idx;
} ui_screen_state_t;

typedef union {
    remote_ctrl_msg_t remote_ctrl;
    r1_data_t r1_state;
    ui_screen_state_t screen_state;
    // r2_data_t r2_state;
} ui_msg_payload_t;

typedef struct {
    ui_msg_type_t type;
    uint32_t seq;
    ui_msg_payload_t payload;
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
