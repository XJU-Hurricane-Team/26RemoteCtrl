/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    iwdg.c
  * @brief   This file provides code for the configuration
  *          of the IWDG instances.
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
#include "iwdg.h"

/* USER CODE BEGIN 0 */

#include "includes.h"

static const uint32_t wd_timeout_ms[IWDG_NUM] = {
  [IWDG_REMOTE] = 300,
  [IWDG_GUI]    = 800, 
  [IWDG_MSG]    = 150,
  [IWDG_BLINK]  = 1500,
};

/* USER CODE END 0 */

IWDG_HandleTypeDef hiwdg;

/* IWDG init function */
void MX_IWDG_Init(void)
{

  /* USER CODE BEGIN IWDG_Init 0 */

  /* USER CODE END IWDG_Init 0 */

  /* USER CODE BEGIN IWDG_Init 1 */

  /* USER CODE END IWDG_Init 1 */
  hiwdg.Instance = IWDG;
  hiwdg.Init.Prescaler = IWDG_PRESCALER_64;
  hiwdg.Init.Reload = 500;
  if (HAL_IWDG_Init(&hiwdg) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN IWDG_Init 2 */

  /* USER CODE END IWDG_Init 2 */

}

/* USER CODE BEGIN 1 */
static volatile uint32_t wd_heartbeat[IWDG_NUM];

void iwdg_kick(iwdg_id_t id) {
  if (id < IWDG_NUM) {
    wd_heartbeat[id] = xTaskGetTickCount();
  }
}

static void watchdog_task(void *pvParameters) {
  UNUSED(pvParameters);

  TickType_t last = xTaskGetTickCount();
  for (int i = 0; i < IWDG_NUM; i++){
    wd_heartbeat[i] = last;
  }

  while (1) {
    uint32_t now = xTaskGetTickCount();
    uint8_t stuck = 0xFF;
    for (uint8_t i = 0; i < IWDG_NUM; i++) {
      if ((now - wd_heartbeat[i]) > wd_timeout_ms[i]) {
      stuck = i;
      break;
      }
    }
    if (stuck == 0xFF) {
      HAL_IWDG_Refresh(&hiwdg);
    }
    vTaskDelayUntil(&last, pdMS_TO_TICKS(50));
  }
}

void iwdg_init(void) {
  xTaskCreate(watchdog_task, "watchdog_task", 128, NULL, 1, NULL);
}

/* USER CODE END 1 */

