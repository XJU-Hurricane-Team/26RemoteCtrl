/**
 * @file    keyboard.c
 * @author  KkarinL15
 * @brief   16键矩阵键盘驱动
 * @version 1.0
 * @date    2025-11-24
 * @note    键盘IO如下
 *          +------+-------+-------+-------+-------+
 *          | IO   | PC0   | PC1   | PC2   | PC3   |
 *          +------+-------+-------+-------+-------+
 *          | PC4  |   1   |   2   |   3   |   4   |
 *          +------+-------+-------+-------+-------+
 *          | PC5  |   5   |   6   |   7   |   8   |
 *          +------+-------+-------+-------+-------+
 *          | PC6  |   9   |  10   |  11   |  12   |
 *          +------+-------+-------+-------+-------+
 *          | PC7  |  13   |  14   |  15   |  16   |
 *          +------+-------+-------+-------+-------+
 *
 */

#include "keyboard.h"
#include "../core/core_delay.h"


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
 * @brief 矩阵键盘初始化
 *
 */

void keyboard_init(void) {
    KEYBOARD_GPIO_ENABLE();

    GPIO_InitTypeDef gpio_initure = {0};
    gpio_initure.Speed = GPIO_SPEED_FREQ_HIGH;
    gpio_initure.Mode = GPIO_MODE_OUTPUT_PP;
    gpio_initure.Pull = GPIO_PULLUP;

    gpio_initure.Pin = COL_KEY_IO;
    HAL_GPIO_Init(KEYBOARD_COL_GPIO_PORT, &gpio_initure);

    gpio_initure.Pin = ROW_KEY_IO;
    HAL_GPIO_Init(KEYBOARD_ROW_GPIO_PORT, &gpio_initure);
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
    delay_ms(15); /* 消抖 */
    /* 读行 */
    row_bits =
        (uint8_t)(GPIO_ReadInputData(KEYBOARD_ROW_GPIO_PORT)  & 0x00f0);

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
    delay_ms(15); /* 消抖 */
    /* 读取列 */
    col_bits =
        (uint8_t)(GPIO_ReadInputData(KEYBOARD_COL_GPIO_PORT) & 0x000f);
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
