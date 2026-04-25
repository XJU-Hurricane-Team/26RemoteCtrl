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
TIM_HandleTypeDef htim3;

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
/* TIM3 init function */
void MX_TIM3_Init(void)
{

  /* USER CODE BEGIN TIM3_Init 0 */

  /* USER CODE END TIM3_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 90-1;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 65535;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM3_Init 2 */

  /* USER CODE END TIM3_Init 2 */
  HAL_TIM_MspPostInit(&htim3);

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
  else if(tim_baseHandle->Instance==TIM3)
  {
  /* USER CODE BEGIN TIM3_MspInit 0 */

  /* USER CODE END TIM3_MspInit 0 */
    /* TIM3 clock enable */
    __HAL_RCC_TIM3_CLK_ENABLE();
  /* USER CODE BEGIN TIM3_MspInit 1 */

  /* USER CODE END TIM3_MspInit 1 */
  }
}
void HAL_TIM_MspPostInit(TIM_HandleTypeDef* timHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(timHandle->Instance==TIM3)
  {
  /* USER CODE BEGIN TIM3_MspPostInit 0 */

  /* USER CODE END TIM3_MspPostInit 0 */

    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**TIM3 GPIO Configuration
    PB5     ------> TIM3_CH2
    */
    GPIO_InitStruct.Pin = GPIO_PIN_5;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF2_TIM3;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* USER CODE BEGIN TIM3_MspPostInit 1 */

  /* USER CODE END TIM3_MspPostInit 1 */
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
  else if(tim_baseHandle->Instance==TIM3)
  {
  /* USER CODE BEGIN TIM3_MspDeInit 0 */

  /* USER CODE END TIM3_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_TIM3_CLK_DISABLE();
  /* USER CODE BEGIN TIM3_MspDeInit 1 */

  /* USER CODE END TIM3_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */
const Note_t Sailor_Song[] = {
    // --- 在受人欺负的时候 --- (0_ 6,_ | 2_ 2_ 2_ 3_ 2_ 2_ 2)
    {REST, EIGHTH}, {L6_D, EIGHTH}, {M2_D, EIGHTH}, {M2_D, EIGHTH}, {M2_D, EIGHTH}, {M3_D, EIGHTH}, {M2_D, EIGHTH}, {M2_D, EIGHTH}, {M2_D, QUARTER},
    // --- 总是听见水手说 --- (1_ 2_ 3_ 3_ 2_ 1_ 3)
    {M1_D, EIGHTH}, {M2_D, EIGHTH}, {M3_D, EIGHTH}, {M3_D, EIGHTH}, {M2_D, EIGHTH}, {M1_D, EIGHTH}, {M3_D, QUARTER}, 
    // --- 他说 --- (3_ 5_)
    {M3_D, EIGHTH}, {M5_D, EIGHTH}, 
    // --- 风雨中这点 --- (6 5_ 3_ 2_ 1_)
    {M6_D, QUARTER}, {M5_D, EIGHTH}, {M3_D, EIGHTH}, {M2_D, EIGHTH}, {M1_D, EIGHTH}, 
    // --- 痛算什么，擦干 --- (2= 3= 2_ 1_ 7,_ 6,_ 6,_ 7,)
    {M2_D, SIXTEENTH}, {M3_D, SIXTEENTH}, {M2_D, EIGHTH},{M1_D, EIGHTH}, {L7_D, EIGHTH}, {L6_D, EIGHTH}, {L6_D, EIGHTH},{L7_D, QUARTER},
    // --- 泪不要怕，至少 --- (1 1= 2= 3_ 2_ 7,_ 5,)
    {M1_D, QUARTER},{M1_D, SIXTEENTH}, {M2_D, SIXTEENTH}, {M3_D, EIGHTH}, {M2_D, EIGHTH}, {L7_D, EIGHTH}, {L5_D, QUARTER}, {L7_D, EIGHTH}, {M1_D, EIGHTH},{M2_D, EIGHTH}, {M3_D, DOTTED_Q}                      // 有梦
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
            htim3.Instance->CNT = 0;
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
        __HAL_TIM_SET_COMPARE(BUZZER_TIM, BUZZER_CH, 32768); // 50% 占空比
    } else {
        __HAL_TIM_SET_COMPARE(BUZZER_TIM, BUZZER_CH, 0); // 0% 占空比，关闭蜂鸣器
    }
}
/* USER CODE END 1 */

