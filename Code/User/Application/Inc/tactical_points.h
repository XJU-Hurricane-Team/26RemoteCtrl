/**
 * @file    tactical_points.h
 * @brief   红/蓝区共享的硬编码战术点位表 + 右波轮选择函数
 * @version 0.1
 * @date    2026-05-17
 */

#ifndef __TACTICAL_POINTS_H
#define __TACTICAL_POINTS_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define TACTICAL_POINT_TOTAL 8   /* 不含 idx=0 ("NONE"), 改这里同步生效 */

/* 红蓝共享的英文点位名表, idx 范围 [0, TACTICAL_POINT_TOTAL] */
extern const char *const kTacticalNames[TACTICAL_POINT_TOTAL + 1];

/* 右波轮驱动选择, 返回 1..TACTICAL_POINT_TOTAL. ctrl_key 由主循环传入 */
uint8_t get_tactical_point(uint8_t ctrl_key);

#ifdef __cplusplus
}
#endif

#endif /* __TACTICAL_POINTS_H */
