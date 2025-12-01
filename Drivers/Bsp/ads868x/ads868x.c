/**
 * @file    ads868x.c
 * @author  KkarinL15
 * @brief   
 * @version 1.0
 * @date    2025-11-12
 */

#include "ads868x.h"
#include "../core/core_delay.h"

uint16_t g_My_AD[8]={0};//存储通道读取到的数值
volatile float g_real_AD[8]={0.0};//存储通道的真实电压值 mV

void ads868x_gpio_init(void){

    __HAL_RCC_GPIOA_CLK_ENABLE();

    GPIO_InitTypeDef gpio_initstruct = { .Pull = GPIO_NOPULL,
                                         .Speed = GPIO_SPEED_FREQ_HIGH};

    gpio_initstruct.Pin = ads868x_NCS_PIN | ads868x_SDI_PIN | ads868x_SCLK_PIN;
    gpio_initstruct.Mode = GPIO_MODE_OUTPUT_PP;
    HAL_GPIO_Init(ads868x_NCS_GPIO_Port, &gpio_initstruct);

    gpio_initstruct.Pin = ads868x_SDO_PIN;
    gpio_initstruct.Mode = GPIO_MODE_INPUT;
    HAL_GPIO_Init(ads868x_SDO_GPIO_Port, &gpio_initstruct);

    HAL_GPIO_WritePin(ads868x_NCS_GPIO_Port, ads868x_SDO_PIN | ads868x_NCS_PIN | 
                        ads868x_SDI_PIN | ads868x_SCLK_PIN, GPIO_PIN_RESET);

}

void ads868x_spi_writeBit(uint8_t bit)
{
    uint8_t bit_temp = bit;
    uint8_t s;
    ads868x_NCS_L;
    for (s = 0; s < 8; s++)
    {
        if(bit_temp & 0x80)
        {
            ads868x_SDI_H;
        }else{
            ads868x_SDI_L;
        }
        ads868x_SCLK_H;
        bit_temp <<= 1;
        ads868x_SCLK_L;
    }
}

uint8_t ads868x_spi_readBit(void)
{
    uint8_t rxdata = 0;
    uint8_t s;
    ads868x_NCS_L;
    for(s=0;s<8;s++) 
	{ 
		rxdata <<= 1;
		ads868x_SCLK_H;
		if(ads868x_SDO) 
		{ 
			rxdata |= 0x01;
		}
		else 
		{ 
			rxdata &= 0xFE;
		}
	ads868x_SCLK_L; 
	}
	return rxdata;
}

/**
 * @brief 写ads868x命令寄存器
 *
 */
void ads868x_Write_Command_Reg(uint16_t command)
{
    ads868x_NCS_L;
    ads868x_spi_writeBit(command >> 8 & 0xFF);
    ads868x_spi_writeBit(command & 0xFF);
    ads868x_NCS_H;
}

/**
 * @brief 写ads868x程序寄存器
 * @param addr 写入的对应寄存器地址
 * @param data 写入的数据
 */
void ads868x_Write_Program_Reg(uint8_t addr,uint8_t data)
{
    ads868x_NCS_L;
    ads868x_spi_writeBit(addr << 1 | 0x01);
    ads868x_spi_writeBit(data);
    ads868x_NCS_H;
}

/**
 * @brief 读ads868x程序寄存器
 * @param addr 读取的对应寄存器地址
 */
uint8_t ads868x_Read_Program_Reg(uint8_t addr)
{
    uint8_t data = 0;
    ads868x_NCS_L;
    ads868x_spi_writeBit(addr << 1);
    data = ads868x_spi_readBit();
    data = ads868x_spi_readBit();
    ads868x_NCS_H;
    return data;
}

/**
 * @brief 软件复位模式
 * 
 */
void Enter_reset_mode(void)
{
    ads868x_Write_Command_Reg(RST);
    delay_us(100);
}

/**
 * @brief 进入自动扫描模式
 * 
 */
void Auto_rst_mode(void)
{
    ads868x_Write_Command_Reg(AUTO_RST);
}

/**
 * @brief 手动扫描模式
 * 
 */
void Man_ch_n_mode(uint16_t ch)
{
    ads868x_Write_Command_Reg(ch);
}

/**
 * @brief 设置通道的范围
 * 
 */
void Set_ch_range(uint8_t ch, uint8_t range)
{
    ads868x_Write_Program_Reg(ch, range);
    
}

/**
 * @brief 获取自动扫描模式各通道的AD转换数据
 * 
 */
void Get_Auto_rst_mode_data(uint16_t *output_data, uint8_t ch_num)
{
    uint8_t i=0, datah=0, datal=0;
    uint16_t data = 0;
    for ( i = 0; i < ch_num; i++)
    {
        ads868x_NCS_L;
        ads868x_spi_writeBit(0x00);
        ads868x_spi_writeBit(0x00);
        datah = ads868x_spi_readBit();
        datal = ads868x_spi_readBit();
        ads868x_NCS_H;
        data = datah << 8 | datal;
        output_data[i] = data;
    }
}

/**
 * @brief 获取手动扫描模式各通道的AD转换数据
 * 
 */
uint16_t Get_Man_ch_n_mode_data(void)
{
    uint8_t data=0,datah=0, datal=0;
    ads868x_NCS_L;
    ads868x_spi_writeBit(0x00);
    ads868x_spi_writeBit(0x00);
    datah = ads868x_spi_readBit();
    datal = ads868x_spi_readBit();
    ads868x_NCS_H;
    data = (datah << 8) | datal;
    return data;
}

/**
 * @brief 初始化单个通道转换数据
 * @param range
 * 参数 -> 范围
 * 0x00 -> +-2.5*ref (+-10.24V)
 * 0x01 -> +-1.25*ref (+-5.12V)
 * 0x02 -> +-0.625*ref (+-2.56V)
 * 0x05 -> +2.5*ref
 * 0x06 -> +1.25*ref
 */
void ads868x_Single_ch_Init(uint8_t ch,uint8_t range)
{
    Enter_reset_mode();
    ads868x_Write_Program_Reg(AUTO_SC, 0xff); //自动扫描所有通道
    ads868x_Read_Program_Reg(AUTO_SC);
    delay_us(2);
    ads868x_Write_Program_Reg(CH_PD,0x00); //所有通道上电
    ads868x_Write_Program_Reg(AUTO_SC,0xff);
    Set_ch_range(ch, range);
    /*! 方便使用在初始化之后再调用*/
    // Man_ch_n_mode(MAN_Ch_1);//CHx对应的MAN_Ch_x
}

/**
 * @brief 初始化多个通道转换数据
 * @param range
 * 参数 -> 范围
 * 0x00 -> +-2.5*ref (+-10.24V)
* 0x01 -> +-1.25*ref (+-5.122V)
 * 0x02 -> +-0.625*ref (+-2.56V)
 * 0x05 -> +2.5*ref
 * 0x06 -> +1.25*ref
 */
void ads868x_Mult_ch_Init(uint8_t range)
{
    Enter_reset_mode();
    ads868x_Write_Program_Reg(0x00, 0x00); //核心配置寄存器,恢复默认值
    delay_us(10);
    ads868x_Write_Program_Reg(AUTO_SC, 0xff); 
    delay_us(10);
    ads868x_Write_Program_Reg(CH_PD, 0xe0); 
    delay_us(10);
    ads868x_Write_Program_Reg(Fea_Sel, 0x03); 
    delay_us(10);

    // Set_ch_range(All_ch,range);
    Set_ch_range(CH0, range);
    delay_us(10);
    Set_ch_range(CH1, range);
    delay_us(10);
    Set_ch_range(CH2, range);
    delay_us(10);
    Set_ch_range(CH3, range);
    delay_us(10);
    Set_ch_range(CH4, range);
    delay_us(10);
    // Set_ch_range(CH5, range);
    // Set_ch_range(CH6, range);
    // Set_ch_range(CH7, range);
    Auto_rst_mode();//进入自动扫描模式

    /*！有触发时设置*/
    // ads868x_Write_Program_Reg(0xa0, 0x01);//写核心配置寄存器，0x03外部触发模式|连续转换模式
    // delay_us(10);
    // Auto_rst_mode();
}

/**
 * @brief 获取手动扫描模式各通道的AD转换数据
 *        单极性t = 1 双极性t = 0
 * @param 真实电压(mV) = (原始值 - 零点偏移) × 满量程电压(V) / ADC分辨率 × 1000(V→mV)
 */
double get_real_data(uint16_t data, uint8_t t)
{
    double real_data;
    if (!t)
    {
        real_data = (double)data * 51.2 / 65535;
    }else{
        real_data = (int16_t)(((double)data * 10 / (double)65535) * 1000);
    }
    return real_data;
}

/**
 * @brief 各通道的AD转换真实电压数据存至全局数组
 * 
 */
void get_real_AD(int ch_sum)
{
    for (int i = 0; i < ch_sum; i++)
    {
        g_real_AD[i] = (float)get_real_data(g_My_AD[i], 1);
    }
}

/**
 * @brief 自动扫描各通道的AD数据
 * 
 */
void get_ads(int ch_sum)
{
    Get_Auto_rst_mode_data(g_My_AD, ch_sum);
    // get_real_AD(ch_sum);
}
