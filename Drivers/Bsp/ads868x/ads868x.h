/**
 * @file    ads868x.h
 * @author  KkarinL15
 * @brief   ADS868x驱动
 * @version 1.0
 * @date    2025-11-12
 * @note    Fea_Sel SDO[2:0]的 4 种组合（000~011）
 *          +---------+------------------+
 *          | SDO[2:0]|       输出功能格式|
 *          +---------+------------------+
 *          |      000|     仅输出转换结果|
 *          +---------+------------------+
 *          |      001|      结果+通道地址|
 *          +---------+------------------+
 *          |      010| 结果+通道+设备地址|
 *          +---------+------------------+
 *          |      011| 结果+地址+输入量程|
 *          +---------+------------------+
 */

#ifndef __ADS868X_H
#define __ADS868X_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include <CSP_Config.h>
#include "Drivers/CSP/SPI_STM32F4xx.h"

//Command Register
#define NO_OP       0X0000 
#define STDBY       0X8200 
#define PWR_DN      0X8300 
#define RST         0X8500 
#define AUTO_RST    0XA000 
#define MAN_Ch_1    0XC000 
#define MAN_Ch_2    0XC400 
#define MAN_Ch_3    0XC800 
#define MAN_Ch_4    0XCC00 
#define MAN_Ch_5    0XD000 
#define MAN_Ch_6    0XD400 
#define MAN_Ch_7    0XD800 
#define MAN_Ch_8    0XDC00 
#define MAN_AUX     0XE000 

//Program Register
#define AUTO_SC     0x01       //CH7-CH0对应bit写1是被选择为自动扫描模式
#define CH_PD       0x02       //BIT7-BIT0对应CH7_PD-CH0_PD，0是上电使能
#define Fea_Sel     0x03       //控制SDO引脚的数据输出格式（如位数、同步模式）
#define CH0         0x05       //R_X
#define CH1         0x06       //R_Y
#define CH2         0x07       //L_X
#define CH3         0x08       //L_Y
#define CH4         0x09 
#define CH5         0x0a 
#define CH6         0x0b 
#define CH7         0x0c
#define All_CH   CH0 | CH1 | CH2 | CH3 | CH4 | CH5 | CH6 | CH7 

#define ads868x_NCS_GPIO_Port         CSP_GPIO_PORT(A)
#define ads868x_NCS_PIN               GPIO_PIN_4
#define ads868x_SDO_GPIO_Port         CSP_GPIO_PORT(A)
#define ads868x_SDO_PIN               GPIO_PIN_6
#define ads868x_SDI_GPIO_Port         CSP_GPIO_PORT(A)
#define ads868x_SDI_PIN               GPIO_PIN_7
#define ads868x_SCLK_GPIO_Port        CSP_GPIO_PORT(A)
#define ads868x_SCLK_PIN              GPIO_PIN_5

#define ads868x_SCLK_H               HAL_GPIO_WritePin(ads868x_SCLK_GPIO_Port,          \
                                        ads868x_SCLK_PIN,GPIO_PIN_SET) 
#define ads868x_SCLK_L               HAL_GPIO_WritePin(ads868x_SCLK_GPIO_Port,          \
                                        ads868x_SCLK_PIN,GPIO_PIN_RESET) 
#define ads868x_NCS_H                HAL_GPIO_WritePin(ads868x_NCS_GPIO_Port,           \
                                        ads868x_NCS_PIN,GPIO_PIN_SET)
#define ads868x_NCS_L                HAL_GPIO_WritePin(ads868x_NCS_GPIO_Port,           \
                                        ads868x_NCS_PIN,GPIO_PIN_RESET) 
#define ads868x_SDI_H                HAL_GPIO_WritePin(ads868x_SDI_GPIO_Port,           \
                                        ads868x_SDI_PIN,GPIO_PIN_SET) 
#define ads868x_SDI_L                HAL_GPIO_WritePin(ads868x_SDI_GPIO_Port,           \
                                        ads868x_SDI_PIN,GPIO_PIN_RESET) 
#define ads868x_SDO                  HAL_GPIO_ReadPin(ads868x_SDO_GPIO_Port,ads868x_SDO_PIN)

extern uint16_t g_My_AD[8];
extern volatile float g_real_AD[8];
void ads868x_gpio_init(void);
void ads868x_spi_writeBit(uint8_t bit);
uint8_t ads868x_spi_readBit(void);
void ads868x_Write_Command_Reg(uint16_t command);
void ads868x_Write_Program_Reg(uint8_t addr,uint8_t data);
uint8_t ads868x_Read_Program_Reg(uint8_t addr);
void Enter_reset_mode(void);
void Auto_rst_mode(void);
void Man_ch_n_mode(uint16_t ch);
void Set_ch_range(uint8_t ch, uint8_t range);
void Get_Auto_rst_mode_data(uint16_t *output_data, uint8_t ch_num);
uint16_t Get_Man_ch_n_mode_data(void);
void ads868x_Single_ch_Init(uint8_t ch,uint8_t range);
void ads868x_Mult_ch_Init(uint8_t range);
double get_real_data(uint16_t data, uint8_t t);
void get_real_AD(int ch_sum);
void get_ads(int ch_sum);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __ADS868X_H */
