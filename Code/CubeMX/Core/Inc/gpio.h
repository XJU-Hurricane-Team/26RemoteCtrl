/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    gpio.h
  * @brief   This file contains all the function prototypes for
  *          the gpio.c file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __GPIO_H__
#define __GPIO_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* USER CODE BEGIN Private defines */

/**
 * @brief The key value.
 */
typedef enum {
    KEY_NO_PRESS = 0U,
    KEY_LZ_PRESS =17U,  /* 左侧摇杆赶下*/
    KEY_RZ_PRESS,       /* 右侧摇杆按下 */
    KEY_TL_PRESS,       /* 左肩键按下 */
    KEY_TR_PRESS,       /* 右肩键按下 */
    WHE_L_TURNUP,       /* 左波轮向上 */
    WHE_L_PRESS,        /* 左波轮按下 */
    WHE_L_TURNDO,       /* 左波轮向下 */
    WHE_R_TURNUP,       /* 右波轮向上 */
    WHE_R_PRESS,        /* 右波轮按下 */
    WHE_R_TURNDO        /* 右波轮向下 */
} key_press_t;

#define KEYBOARD_COL_GPIO_PORT GPIOC
#define KEYBOARD_ROW_GPIO_PORT GPIOC

/* 列的IO, 0,1,2,3 */
#define COL_KEY_IO             KeyBoard0_Pin | KeyBoard1_Pin | KeyBoard2_Pin | KeyBoard3_Pin
/* 行的IO, 4,5,6,7 */
#define ROW_KEY_IO             KeyBoard4_Pin | KeyBoard5_Pin | KeyBoard6_Pin | KeyBoard7_Pin

/* LED0 */
#define LED0(x)                                                                \
    x ? HAL_GPIO_WritePin(LED0_GPIO_Port, LED0_Pin, GPIO_PIN_SET)              \
      : HAL_GPIO_WritePin(LED0_GPIO_Port, LED0_Pin, GPIO_PIN_RESET)
#define LED0_ON()     HAL_GPIO_WritePin(LED0_GPIO_Port, LED0_Pin, GPIO_PIN_RESET)
#define LED0_OFF()    HAL_GPIO_WritePin(LED0_GPIO_Port, LED0_Pin, GPIO_PIN_SET)
#define LED0_TOGGLE() HAL_GPIO_TogglePin(LED0_GPIO_Port, LED0_Pin)

/* LED1 */
#define LED1(x)                                                                \
    x ? HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_SET)              \
      : HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_RESET)
#define LED1_ON()     HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_RESET)
#define LED1_OFF()    HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_SET)
#define LED1_TOGGLE() HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin)

/* LED2 */
#define LED2(x)                                                                \
    x ? HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_SET)              \
      : HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_RESET)
#define LED2_ON()     HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_RESET)
#define LED2_OFF()    HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_SET)
#define LED2_TOGGLE() HAL_GPIO_TogglePin(LED2_GPIO_Port, LED2_Pin)

/* LED3 */
#define LED3(x)                                                                \
    x ? HAL_GPIO_WritePin(LED3_GPIO_Port, LED3_Pin, GPIO_PIN_SET)              \
      : HAL_GPIO_WritePin(LED3_GPIO_Port, LED3_Pin, GPIO_PIN_RESET)
#define LED3_ON()     HAL_GPIO_WritePin(LED3_GPIO_Port, LED3_Pin, GPIO_PIN_RESET)
#define LED3_OFF()    HAL_GPIO_WritePin(LED3_GPIO_Port, LED3_Pin, GPIO_PIN_SET)
#define LED3_TOGGLE() HAL_GPIO_TogglePin(LED3_GPIO_Port, LED3_Pin)

/* USER CODE END Private defines */

void MX_GPIO_Init(void);

/* USER CODE BEGIN Prototypes */

uint8_t keyboard_scan(void);

key_press_t add_key_scan(uint8_t scan_continous);
key_press_t ctrl_key_scan(uint8_t scan_continous);
uint8_t get_point_value(uint8_t ctrl_key);
uint8_t get_irda_msg(uint8_t ctrl_key);

/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif
#endif /*__ GPIO_H__ */

