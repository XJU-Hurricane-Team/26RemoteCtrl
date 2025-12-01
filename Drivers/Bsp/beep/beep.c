/**
 * @file    beep.c
 * @author  KkarinL15
 * @brief   
 * @version 1.0
 * @date    2025-11-20
 */

#include "beep.h"

void beep_init(void){
    GPIO_InitTypeDef gpio_initure = {0};

    CSP_GPIO_CLK_ENABLE(BEEP_GPIO_PORT);
    gpio_initure.Pin = BEEP_GPIO_PIN;
    gpio_initure.Mode = GPIO_MODE_OUTPUT_PP;
    gpio_initure.Pull = GPIO_PULLUP;
    gpio_initure.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(CSP_GPIO_PORT(BEEP_GPIO_PORT), &gpio_initure);

    BEEP_OFF();
}