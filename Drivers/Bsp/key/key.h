/**
 * @file    key.h
 * @author  Deadline039
 * @brief   On board key function.
 * @version 1.1
 * @date    2024-07-29
 */

#ifndef __KEY_H
#define __KEY_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include <CSP_Config.h>

#define KEY0_GPIO_PORT C
#define KEY0_GPIO_PIN  GPIO_PIN_3

#define KEY1_GPIO_PORT C
#define KEY1_GPIO_PIN  GPIO_PIN_2

#define KEY2_GPIO_PORT C
#define KEY2_GPIO_PIN  GPIO_PIN_13

#define WKUP_GPIO_PORT A
#define WKUP_GPIO_PIN  GPIO_PIN_0

/* Detect the key Pin status. */
#define KEY0  HAL_GPIO_ReadPin(CSP_GPIO_PORT(KEY0_GPIO_PORT), KEY0_GPIO_PIN)
#define KEY1  HAL_GPIO_ReadPin(CSP_GPIO_PORT(KEY1_GPIO_PORT), KEY1_GPIO_PIN)
#define KEY2  HAL_GPIO_ReadPin(CSP_GPIO_PORT(KEY2_GPIO_PORT), KEY2_GPIO_PIN)
#define WK_UP HAL_GPIO_ReadPin(CSP_GPIO_PORT(WKUP_GPIO_PORT), WKUP_GPIO_PIN)

/**
 * @brief The key value.
 */
typedef enum {
    KEY_NO_PRESS = 0U,
    KEY0_PRESS,
    KEY1_PRESS,
    KEY2_PRESS,
    WKUP_PRESS
} key_press_t;

void key_init(void);
key_press_t key_scan(uint8_t scan_continous);
key_press_t key_scan_1_2(uint8_t scan_continous);
key_press_t key_scan_k_0(uint8_t scan_continous);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __KEY_H */
