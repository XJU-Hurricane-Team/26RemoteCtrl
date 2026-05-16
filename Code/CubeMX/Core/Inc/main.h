/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define LCD_SCK_Pin GPIO_PIN_2
#define LCD_SCK_GPIO_Port GPIOE
#define KEY_LZ_Pin GPIO_PIN_3
#define KEY_LZ_GPIO_Port GPIOE
#define LCD_CS_Pin GPIO_PIN_4
#define LCD_CS_GPIO_Port GPIOE
#define LCD_MISO_Pin GPIO_PIN_5
#define LCD_MISO_GPIO_Port GPIOE
#define LCD_MOSI_Pin GPIO_PIN_6
#define LCD_MOSI_GPIO_Port GPIOE
#define KeyBoard0_Pin GPIO_PIN_0
#define KeyBoard0_GPIO_Port GPIOC
#define KeyBoard1_Pin GPIO_PIN_1
#define KeyBoard1_GPIO_Port GPIOC
#define KeyBoard2_Pin GPIO_PIN_2
#define KeyBoard2_GPIO_Port GPIOC
#define KeyBoard3_Pin GPIO_PIN_3
#define KeyBoard3_GPIO_Port GPIOC
#define ads868x_NCS_Pin GPIO_PIN_4
#define ads868x_NCS_GPIO_Port GPIOA
#define ads868x_SCLK_Pin GPIO_PIN_5
#define ads868x_SCLK_GPIO_Port GPIOA
#define ads868x_SDO_Pin GPIO_PIN_6
#define ads868x_SDO_GPIO_Port GPIOA
#define ads868x_SDI_Pin GPIO_PIN_7
#define ads868x_SDI_GPIO_Port GPIOA
#define KeyBoard4_Pin GPIO_PIN_4
#define KeyBoard4_GPIO_Port GPIOC
#define KeyBoard5_Pin GPIO_PIN_5
#define KeyBoard5_GPIO_Port GPIOC
#define WHE_R_DO_Pin GPIO_PIN_7
#define WHE_R_DO_GPIO_Port GPIOE
#define WHE_R_KEY_Pin GPIO_PIN_8
#define WHE_R_KEY_GPIO_Port GPIOE
#define WHE_R_UP_Pin GPIO_PIN_9
#define WHE_R_UP_GPIO_Port GPIOE
#define KEY_TR_Pin GPIO_PIN_10
#define KEY_TR_GPIO_Port GPIOB
#define KEY_TL_Pin GPIO_PIN_11
#define KEY_TL_GPIO_Port GPIOB
#define KeyBoard6_Pin GPIO_PIN_6
#define KeyBoard6_GPIO_Port GPIOC
#define KeyBoard7_Pin GPIO_PIN_7
#define KeyBoard7_GPIO_Port GPIOC
#define WHE_L_DO_Pin GPIO_PIN_8
#define WHE_L_DO_GPIO_Port GPIOC
#define WHE_L_KEY_Pin GPIO_PIN_9
#define WHE_L_KEY_GPIO_Port GPIOC
#define WHE_L_UP_Pin GPIO_PIN_8
#define WHE_L_UP_GPIO_Port GPIOA
#define KEY_RZ_Pin GPIO_PIN_15
#define KEY_RZ_GPIO_Port GPIOA
#define LED0_Pin GPIO_PIN_0
#define LED0_GPIO_Port GPIOD
#define LED1_Pin GPIO_PIN_1
#define LED1_GPIO_Port GPIOD
#define LED2_Pin GPIO_PIN_2
#define LED2_GPIO_Port GPIOD
#define LED3_Pin GPIO_PIN_3
#define LED3_GPIO_Port GPIOD
#define LCD_DC_Pin GPIO_PIN_0
#define LCD_DC_GPIO_Port GPIOE
#define LCD_BLK_Pin GPIO_PIN_1
#define LCD_BLK_GPIO_Port GPIOE

/* USER CODE BEGIN Private defines */



/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
