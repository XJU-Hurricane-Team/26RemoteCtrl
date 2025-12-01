/**
 * @file    bsp.h
 * @author  Deadline039
 * @brief   Bsp layer export interface.
 * @version 1.0
 * @date    2024-09-18
 */

#ifndef __BSP_H
#define __BSP_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include <CSP_Config.h>

#include "./core/bsp_core.h"
#include "./core/core_delay.h"
#include "./key/key.h"
#include "./led/led.h"
#include "./keyboard/keyboard.h"
#include "./ads868x/ads868x.h"
// #include "./w25qxx/w25qxx.h"
#include "./adc/adc.h"
#include "./beep/beep.h"
#include "./time/tim.h"
#include "./crc/crc.h"
#include "./dma2d/dma2d.h"
#include "./st77xx_gfx/st77xx.h"

void bsp_init(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __BSP_H */