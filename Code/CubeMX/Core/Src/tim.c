/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    tim.c
  * @brief   This file provides code for the configuration
  *          of the TIM instances.
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
/* Includes ------------------------------------------------------------------*/
#include "tim.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim5;

/* TIM2 init function */
void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 8999;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 199;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */

}
/* TIM5 init function */
void MX_TIM5_Init(void)
{

  /* USER CODE BEGIN TIM5_Init 0 */

  /* USER CODE END TIM5_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM5_Init 1 */

  /* USER CODE END TIM5_Init 1 */
  htim5.Instance = TIM5;
  htim5.Init.Prescaler = 90-1;
  htim5.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim5.Init.Period = 65535;
  htim5.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim5.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_Base_Init(&htim5) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim5, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim5) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim5, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim5, &sConfigOC, TIM_CHANNEL_4) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM5_Init 2 */

  /* USER CODE END TIM5_Init 2 */
  HAL_TIM_MspPostInit(&htim5);

}

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* tim_baseHandle)
{

  if(tim_baseHandle->Instance==TIM2)
  {
  /* USER CODE BEGIN TIM2_MspInit 0 */

  /* USER CODE END TIM2_MspInit 0 */
    /* TIM2 clock enable */
    __HAL_RCC_TIM2_CLK_ENABLE();

    /* TIM2 interrupt Init */
    HAL_NVIC_SetPriority(TIM2_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(TIM2_IRQn);
  /* USER CODE BEGIN TIM2_MspInit 1 */

  /* USER CODE END TIM2_MspInit 1 */
  }
  else if(tim_baseHandle->Instance==TIM5)
  {
  /* USER CODE BEGIN TIM5_MspInit 0 */

  /* USER CODE END TIM5_MspInit 0 */
    /* TIM5 clock enable */
    __HAL_RCC_TIM5_CLK_ENABLE();
  /* USER CODE BEGIN TIM5_MspInit 1 */

  /* USER CODE END TIM5_MspInit 1 */
  }
}
void HAL_TIM_MspPostInit(TIM_HandleTypeDef* timHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(timHandle->Instance==TIM5)
  {
  /* USER CODE BEGIN TIM5_MspPostInit 0 */

  /* USER CODE END TIM5_MspPostInit 0 */

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**TIM5 GPIO Configuration
    PA3     ------> TIM5_CH4
    */
    GPIO_InitStruct.Pin = GPIO_PIN_3;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF2_TIM5;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* USER CODE BEGIN TIM5_MspPostInit 1 */

  /* USER CODE END TIM5_MspPostInit 1 */
  }

}

void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef* tim_baseHandle)
{

  if(tim_baseHandle->Instance==TIM2)
  {
  /* USER CODE BEGIN TIM2_MspDeInit 0 */

  /* USER CODE END TIM2_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_TIM2_CLK_DISABLE();

    /* TIM2 interrupt Deinit */
    HAL_NVIC_DisableIRQ(TIM2_IRQn);
  /* USER CODE BEGIN TIM2_MspDeInit 1 */

  /* USER CODE END TIM2_MspDeInit 1 */
  }
  else if(tim_baseHandle->Instance==TIM5)
  {
  /* USER CODE BEGIN TIM5_MspDeInit 0 */

  /* USER CODE END TIM5_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_TIM5_CLK_DISABLE();
  /* USER CODE BEGIN TIM5_MspDeInit 1 */

  /* USER CODE END TIM5_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */

const Note_t Sailor_Song[] = {
    // /*诗*/
    // {M3_D,QUARTER}, {M2_D,QUARTER}, {M1_D,QUARTER}, {L6_D,QUARTER}, {REST,EIGHTH},
    // {M5_D,QUARTER}, {M3_D,QUARTER}, {L1_D,QUARTER}, {REST,QUARTER},
    // {M2_D,QUARTER}, {M3_D,QUARTER}, {M5_D,QUARTER}, {M6_D,QUARTER}, {REST,EIGHTH},
    // {H1_D,QUARTER}, {M6_D,EIGHTH}, {M3_D,EIGHTH}, {M6_D,QUARTER}, {REST,QUARTER}

    // /*兰花草*/
    {L6_D,EIGHTH}, {M3_D,EIGHTH}, {M3_D,EIGHTH}, {M3_D,EIGHTH}, {M3_D,QUARTER}, {M2_D,QUARTER}, {M1_D,EIGHTH}, {M2_D,SIXTEENTH}, {M1_D,EIGHTH}, {L7_D,EIGHTH}, {L6_D,QUARTER}, {REST,QUARTER},
    {M6_D,EIGHTH}, {M6_D,EIGHTH}, {M6_D,EIGHTH}, {M6_D,EIGHTH}, {M6_D,QUARTER}, {M5_D,EIGHTH}, {M3_D,EIGHTH}, {M5_D,EIGHTH}, {M5_D,EIGHTH}, {M4_D,EIGHTH}, {L3_D,QUARTER}, {REST,QUARTER},

    /*好运来*/
    // {M6_D,QUARTER}, {H3_D,QUARTER}, {H2_D,QUARTER}, {H1_D,EIGHTH}, {M6_D,EIGHTH},{M5_D,QUARTER}, {H1_D,EIGHTH}, {H2_D,EIGHTH},{M6_D,QUARTER},{REST,QUARTER},
    // {M6_D,QUARTER}, {H2_D,QUARTER}, {H1_D,QUARTER}, {M6_D,EIGHTH}, {M5_D,EIGHTH},{M2_D,QUARTER}, {M5_D,EIGHTH}, {M6_D,EIGHTH},{M3_D,QUARTER}

    // /*星星点灯*/
    // {M6_D,QUARTER}, {M6_D,QUARTER}, {M3_D,EIGHTH}, {M6_D,QUARTER},
    // {M6_D,SIXTEENTH}, {M6_D,SIXTEENTH}, {M5_D,EIGHTH}, {M6_D,EIGHTH},{M7_D,EIGHTH},{M5_D,EIGHTH},
    // {M5_D,QUARTER},

    // /*水手*/
    // {M3_D, EIGHTH}, {M5_D, EIGHTH}, 
    // {M6_D, QUARTER}, {M5_D, QUARTER}, {M3_D, QUARTER}, {M2_D, EIGHTH}, {M1_D, EIGHTH}, 
    // {M2_D, EIGHTH}, {M3_D, SIXTEENTH}, {M2_D, SIXTEENTH},{M1_D, EIGHTH}, {L7_D, EIGHTH}, {L6_D, QUARTER},
};

#define SONG_LENGTH (sizeof(Sailor_Song) / sizeof(Sailor_Song[0])) // 计算数组长度的宏
/**
 * @brief Sing the song.新增
 */
void Play_Music(void) {
    HAL_TIM_PWM_Start(BUZZER_TIM, BUZZER_CH);
    for (int i = 0; i < SONG_LENGTH; i++) {
        uint16_t current_freq = Sailor_Song[i].freq;
        uint16_t current_dur = Sailor_Song[i].duration;
        if (current_freq == REST) {
            __HAL_TIM_SET_COMPARE(BUZZER_TIM, BUZZER_CH, 0);// 休止符：占空比设为 0，停止发声
        } else {
            uint32_t arr_value = (1000000 / current_freq) - 1;
            __HAL_TIM_SET_AUTORELOAD(BUZZER_TIM, arr_value);
            __HAL_TIM_SET_COMPARE(BUZZER_TIM, BUZZER_CH, arr_value / 2);
            htim5.Instance->CNT = 0;
        }
        HAL_Delay(current_dur); 
        __HAL_TIM_SET_COMPARE(BUZZER_TIM, BUZZER_CH, 0); 
        HAL_Delay(20); // 20ms 的间隔，可根据听感微调
    }
    // 播放完毕，关闭 PWM
    HAL_TIM_PWM_Stop(BUZZER_TIM, BUZZER_CH);
}

/**
 * @brief Beep switch.新增
 * @param on: 1 to turn on the beep, 0 to turn off the beep
 */
void BEEP_SWITCH(uint8_t on) {
    if (on) {
        __HAL_TIM_SET_COMPARE(BUZZER_TIM, BUZZER_CH, 65535);
    } else {
        __HAL_TIM_SET_COMPARE(BUZZER_TIM, BUZZER_CH, 0); // 0% 占空比，关闭蜂鸣器
    }
}
/* USER CODE END 1 */

