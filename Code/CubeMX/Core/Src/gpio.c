/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    gpio.c
  * @brief   This file provides code for the configuration
  *          of all used GPIO pins.
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
#include "gpio.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/*----------------------------------------------------------------------------*/
/* Configure GPIO                                                             */
/*----------------------------------------------------------------------------*/
/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/** Configure pins as
        * Analog
        * Input
        * Output
        * EVENT_OUT
        * EXTI
        * Free pins are configured automatically as Analog (this feature is enabled through
        * the Code Generation settings)
*/
void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOE, LCD_CS_Pin|LCD_DC_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, KeyBoard0_Pin|KeyBoard1_Pin|KeyBoard2_Pin|KeyBoard3_Pin
                          |KeyBoard4_Pin|KeyBoard5_Pin|KeyBoard6_Pin|KeyBoard7_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, ads868x_NCS_Pin|ads868x_SCLK_Pin|ads868x_SDI_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, LED0_Pin|LED1_Pin|LED2_Pin|LED3_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LCD_BLK_GPIO_Port, LCD_BLK_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin : KEY_LZ_Pin */
  GPIO_InitStruct.Pin = KEY_LZ_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(KEY_LZ_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : LCD_CS_Pin */
  GPIO_InitStruct.Pin = LCD_CS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(LCD_CS_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : PC13 PC10 PC11 PC12 */
  GPIO_InitStruct.Pin = GPIO_PIN_13|GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : KeyBoard0_Pin KeyBoard1_Pin KeyBoard2_Pin KeyBoard3_Pin
                           KeyBoard4_Pin KeyBoard5_Pin KeyBoard6_Pin KeyBoard7_Pin */
  GPIO_InitStruct.Pin = KeyBoard0_Pin|KeyBoard1_Pin|KeyBoard2_Pin|KeyBoard3_Pin
                          |KeyBoard4_Pin|KeyBoard5_Pin|KeyBoard6_Pin|KeyBoard7_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : PA0 PA1 PA2 PA3
                           PA11 PA12 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3
                          |GPIO_PIN_11|GPIO_PIN_12;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : ads868x_NCS_Pin ads868x_SCLK_Pin ads868x_SDI_Pin */
  GPIO_InitStruct.Pin = ads868x_NCS_Pin|ads868x_SCLK_Pin|ads868x_SDI_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : ads868x_SDO_Pin */
  GPIO_InitStruct.Pin = ads868x_SDO_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(ads868x_SDO_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : PB0 PB1 PB2 PB12
                           PB13 PB14 PB15 PB4
                           PB6 PB7 PB8 PB9 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_12
                          |GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15|GPIO_PIN_4
                          |GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : WHE_R_UP_Pin WHE_R_KEY_Pin WHE_R_DO_Pin */
  GPIO_InitStruct.Pin = WHE_R_UP_Pin|WHE_R_KEY_Pin|WHE_R_DO_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pins : PE10 PE11 PE12 PE13
                           PE14 PE15 */
  GPIO_InitStruct.Pin = GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13
                          |GPIO_PIN_14|GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pins : KEY_TR_Pin KEY_TL_Pin */
  GPIO_InitStruct.Pin = KEY_TR_Pin|KEY_TL_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : PD8 PD9 PD10 PD11
                           PD12 PD13 PD14 PD15
                           PD4 PD5 PD6 PD7 */
  GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11
                          |GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15
                          |GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pins : WHE_L_DO_Pin WHE_L_KEY_Pin */
  GPIO_InitStruct.Pin = WHE_L_DO_Pin|WHE_L_KEY_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : WHE_L_UP_Pin */
  GPIO_InitStruct.Pin = WHE_L_UP_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(WHE_L_UP_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : KEY_RZ_Pin */
  GPIO_InitStruct.Pin = KEY_RZ_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(KEY_RZ_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : LED0_Pin LED1_Pin LED2_Pin LED3_Pin */
  GPIO_InitStruct.Pin = LED0_Pin|LED1_Pin|LED2_Pin|LED3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pins : LCD_DC_Pin LCD_BLK_Pin */
  GPIO_InitStruct.Pin = LCD_DC_Pin|LCD_BLK_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

}

/* USER CODE BEGIN 2 */

/**
 * @brief  Reads the specified GPIO input data port.
 * @param  GPIOx: where x can be (A..I) to select the GPIO peripheral.
 * @retval GPIO input data port value.
 */
static inline uint16_t GPIO_ReadInputData(GPIO_TypeDef *GPIOx) {
    return ((uint16_t)GPIOx->IDR);
}

/**
 * @brief 获取1所在的二进制位, 例如0100, 返回3
 *
 * @param number 二进制数
 * @return 1所在的位
 */
static uint8_t get_bits(uint8_t number) {
    uint8_t bit_number = 1;

    while ((number & 0x01) == 0) {
        ++bit_number;
        number >>= 1;
    }
    return bit_number;
}

/**
 * @brief 矩阵键盘扫描
 *
 * @return uint8_t 按下的按键, 1-16
 */
uint8_t keyboard_scan(void) {
    uint8_t col_bits = 0x00, row_bits = 0x00;

    /* 列置高电平, 行置低电平 */
    HAL_GPIO_WritePin(KEYBOARD_COL_GPIO_PORT, COL_KEY_IO, GPIO_PIN_SET);
    HAL_GPIO_WritePin(KEYBOARD_ROW_GPIO_PORT, ROW_KEY_IO, GPIO_PIN_RESET);
    HAL_Delay(15); /* 消抖 */
    /* 读行 */
    row_bits = (uint8_t)(GPIO_ReadInputData(KEYBOARD_ROW_GPIO_PORT) & 0x00f0);

    if (row_bits == 0x00) {
        return 0;
    }
    /* 多按键按下, 为避免鬼影, 产生误操作, 不做处理 */
    if (!(row_bits == 0x10 || row_bits == 0x20 || row_bits == 0x40 ||
          row_bits == 0x80)) {
        return 0;
    }

    /* 行置高电平, 列置低电平 */
    HAL_GPIO_WritePin(KEYBOARD_COL_GPIO_PORT, COL_KEY_IO, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(KEYBOARD_ROW_GPIO_PORT, ROW_KEY_IO, GPIO_PIN_SET);
    HAL_Delay(15); /* 消抖 */
    /* 读取列 */
    col_bits = (uint8_t)(GPIO_ReadInputData(KEYBOARD_COL_GPIO_PORT) & 0x000f);
    /* 多按键按下, 为避免鬼影, 产生误操作, 不做处理 */
    if (!(col_bits == 0x01 || col_bits == 0x02 || col_bits == 0x04 ||
          col_bits == 0x08)) {
        return 0;
    }

    /* 获取行, 列的位置 */
    uint8_t row_num = get_bits(row_bits) - 4;
    uint8_t col_num = get_bits(col_bits);

    return (4 * (row_num - 1) + col_num);
}

#define KEY_LZ HAL_GPIO_ReadPin(KEY_LZ_GPIO_Port, KEY_LZ_Pin)
#define KEY_TL HAL_GPIO_ReadPin(KEY_TL_GPIO_Port, KEY_TL_Pin)
#define KEY_RZ HAL_GPIO_ReadPin(KEY_RZ_GPIO_Port, KEY_RZ_Pin)
#define KEY_TR HAL_GPIO_ReadPin(KEY_TR_GPIO_Port, KEY_TR_Pin)
#define WHE_L_UP HAL_GPIO_ReadPin(WHE_L_UP_GPIO_Port, WHE_L_UP_Pin)
#define WHE_L_PS HAL_GPIO_ReadPin(WHE_L_KEY_GPIO_Port, WHE_L_KEY_Pin)
#define WHE_L_DO HAL_GPIO_ReadPin(WHE_L_DO_GPIO_Port, WHE_L_DO_Pin)
#define WHE_R_UP HAL_GPIO_ReadPin(WHE_R_UP_GPIO_Port, WHE_R_UP_Pin)
#define WHE_R_PS HAL_GPIO_ReadPin(WHE_R_KEY_GPIO_Port, WHE_R_KEY_Pin)
#define WHE_R_DO HAL_GPIO_ReadPin(WHE_R_DO_GPIO_Port, WHE_R_DO_Pin)


/**
 * @brief Scan the key.
 *
 * @param scan_continous Detecting consecutive presses.
 * @return The key which be pressed.
 * @note Note that keys have priority,`KEY_LZ > KEY_TL > KEY_RZ > KEY_TR`
 */
key_press_t key_scan(uint8_t scan_continous) {
    static uint8_t key_up = 1; /* The flag of key released. */
    if (scan_continous == 1) {
        key_up = 1; /* Detect consecutive presses. */
    }
    if (key_up && (KEY_LZ == 0 || KEY_RZ == 0 || KEY_TL == 0 || KEY_TR == 0 || WHE_L_UP == 1 ||
        WHE_L_PS == 1 || WHE_L_DO == 1 || WHE_R_UP == 1 || WHE_R_PS == 1 || WHE_R_DO == 1)) {
        HAL_Delay(10);
        key_up = 0;
        if (KEY_LZ == 0) {
            return KEY_LZ_PRESS;
        } else if (KEY_RZ == 0) {
            return KEY_RZ_PRESS;
        } else if (KEY_TL == 0) {
            return KEY_TL_PRESS;
        } else if (KEY_TR == 0) {
            return KEY_TR_PRESS;
        }else if (WHE_L_UP == 1) {
            return WHE_L_TURNUP;
        }else if (WHE_L_PS == 1) {
            return WHE_L_PRESS;
        }else if (WHE_L_DO == 1) {
            return WHE_L_TURNDO;
        }else if (WHE_R_UP == 1) {
            return WHE_R_TURNUP;
        }else if (WHE_R_PS == 1) {
            return WHE_R_PRESS;
        }else if (WHE_R_DO == 1) {
            return WHE_R_TURNDO;
        }
    } else if (KEY_LZ == 1 && KEY_RZ == 1 && KEY_TL == 1 && KEY_TR == 1 && WHE_L_UP == 0 &&
        WHE_L_PS == 0 && WHE_L_DO == 0 && WHE_R_UP == 0 && WHE_R_PS == 0 && WHE_R_DO == 0) {
        key_up = 1;
    }
    return KEY_NO_PRESS;
}

/* USER CODE END 2 */
