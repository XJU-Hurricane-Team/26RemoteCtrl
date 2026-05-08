/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    tim.h
  * @brief   This file contains all the function prototypes for
  *          the tim.c file
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
#ifndef __TIM_H__
#define __TIM_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

extern TIM_HandleTypeDef htim2;

extern TIM_HandleTypeDef htim5;

/* USER CODE BEGIN Private defines */
#define BUZZER_TIM &htim5
#define BUZZER_CH  TIM_CHANNEL_4                //PA3
#define BEAT_TIME  500                          // 节拍基础时长
#define QUARTER     (BEAT_TIME)                 // 四分音符 (1拍)
#define EIGHTH      (BEAT_TIME / 2)             // 八分音符 (0.5拍)
#define SIXTEENTH   (BEAT_TIME / 4)             // 十六分音符 (0.25拍)
#define HALF        (BEAT_TIME * 2)             // 二分音符 (2拍)
#define DOTTED_Q    (BEAT_TIME + BEAT_TIME/2)   // 附点四分音符 (1.5拍)
#define DOTTED_E    (BEAT_TIME/2 + BEAT_TIME/4) // 附点八分音符 (0.75拍)
#define REST        0                           // 休止符
#define SHORT_REST  50
/* 低音区 */
#define L1_D 262
#define L2_D 294
#define L3_D 330
#define L4_D 349
#define L5_D 392
#define L6_D 440
#define L7_D 494
/* 中音区 */
#define M1_D 523
#define M2_D 587
#define M3_D 659
#define M4_D 698
#define M5_D 784
#define M6_D 880
#define M7_D 988
/* 高音区 */
#define H1_D 1046
#define H2_D 1175
#define H3_D 1318
typedef struct {
    uint16_t freq;      // 音符频率 (Hz)
    uint16_t duration;  // 持续时间 (ms)
} Note_t;
/* USER CODE END Private defines */

void MX_TIM2_Init(void);
void MX_TIM5_Init(void);

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* USER CODE BEGIN Prototypes */

void Play_Music(void);
void BEEP_SWITCH(uint8_t on);
/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __TIM_H__ */

