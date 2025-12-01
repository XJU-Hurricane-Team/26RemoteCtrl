/**
 * @file    beep.c
 * @author  KkarinL15
 * @brief   
 * @version 1.0
 * @date    2025-11-20
 */

#ifndef __BEEP_H
#define __BEEP_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include <CSP_Config.h>

#define BEEP_GPIO_PORT C
#define BEEP_GPIO_PIN  GPIO_PIN_12

#define BEEP(x)                                                                \
    x ? HAL_GPIO_WritePin(CSP_GPIO_PORT(BEEP_GPIO_PORT), BEEP_GPIO_PIN,        \
                          GPIO_PIN_SET)                                        \
      : HAL_GPIO_WritePin(CSP_GPIO_PORT(BEEP_GPIO_PORT), BEEP_GPIO_PIN,        \
                          GPIO_PIN_RESET)
#define BEEP_OFF()                                                              \
    HAL_GPIO_WritePin(CSP_GPIO_PORT(BEEP_GPIO_PORT), BEEP_GPIO_PIN,            \
                      GPIO_PIN_RESET)
#define BEEP_ON()                                                             \
    HAL_GPIO_WritePin(CSP_GPIO_PORT(BEEP_GPIO_PORT), BEEP_GPIO_PIN,            \
                      GPIO_PIN_SET)
#define BEEP_TOGGLE()                                                          \
    HAL_GPIO_TogglePin(CSP_GPIO_PORT(BEEP_GPIO_PORT), BEEP_GPIO_PIN)

void beep_init(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __BEEP_H */