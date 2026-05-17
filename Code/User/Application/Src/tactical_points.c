/**
 * @file    tactical_points.c
 * @brief   战术点位表定义 + 右波轮选择函数
 * @version 0.1
 * @date    2026-05-17
 */

#include "tactical_points.h"
#include "gpio.h"
#include "my_math/my_math.h"

const char *const kTacticalNames[TACTICAL_POINT_TOTAL + 1] = {
    "NONE",     /* idx=0, 未选 */
    "POINT_A",
    "POINT_B",
    "TACT_C",
    "TACT_D",
    "BUFF_E",
    "GUARD_F",
    "SNIPE_G",
    "BACK_H"
};

static int8_t tactical_num = 1;

uint8_t get_tactical_point(uint8_t ctrl_key) {
    switch (ctrl_key) {
        case WHE_R_TURNUP:
            tactical_num -= 1;
            my_limit(tactical_num, 1, TACTICAL_POINT_TOTAL);
            break;
        case WHE_R_TURNDO:
            tactical_num += 1;
            my_limit(tactical_num, 1, TACTICAL_POINT_TOTAL);
            break;
        default:
            break;
    }
    return (uint8_t)tactical_num;
}
