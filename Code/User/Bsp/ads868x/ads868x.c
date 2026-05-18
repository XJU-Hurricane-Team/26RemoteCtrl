/**
 * @file    ads868x.c
 * @author  KkarinL15
 * @brief   
 * @version 1.0
 * @date    2025-11-12
 */

#include "ads868x.h"
#include "../core_delay/core_delay.h"

static uint16_t g_My_AD[8]={0};//存储通道读取到的数值
static volatile float g_real_AD[8]={0.0};//存储通道的真实电压值 mV

/**
 * @brief ADC 死区限制
 * 
 * @param data 当前值
 * @param dead_zone 死区限制
 * @param data_level 采样级
 * @return 限制结果 
 */
static uint16_t rs_data_limit(uint16_t data, uint8_t dead_zone,
                              uint8_t data_level) 
{
    uint16_t adc_max = 38000;
    uint16_t adc_mid = 21000;
    uint16_t adc_min = 4000;

    uint16_t dead_zone_threshold = (adc_max - adc_min) * dead_zone / 100;
    uint16_t data_piece = (uint16_t)((adc_mid - adc_min - dead_zone_threshold) * 2 /
                                     data_level); /* 计算每一份的值 */
    uint8_t data_res = 0;

    if (data >= adc_max)
    {
        data = adc_max;
    }else if (data <= adc_min)
    {
        data = adc_min;
    }else if( data >= (adc_mid - dead_zone_threshold) && 
        data <= (adc_mid + dead_zone_threshold)){
                data = adc_mid;/*设置死区内的数值为中间值 */
    }

    if (data > adc_mid)
    {
        data_res = (data - adc_mid - dead_zone_threshold) / data_piece + 20;
        return data_res;
    }else if (data < adc_mid)
    {
        data_res = (data - adc_min) / data_piece;
        return data_res;
    }else
    {
        return data_level / 2;
    }
}

/**
 * @brief 计算 100 次采样的平均值
 * 
 * @param data 
 * @param dead_zone 死区限制 如 8 就是上下 8% 总共 16 的死区
 * @param data_level 采样级
 * @note 推荐采样级为 40, 死区为 10. 采样级不能大于 45, 否则会导致采样值无法为零，可以增加 ADC 补偿来进一步提高采样级
 */
void rs_get_value(uint32_t *data, uint8_t dead_zone, uint8_t data_level) 
{
    // xSemaphoreTake(rs_conv_cplt_semp, portMAX_DELAY);
    data[0] = 0;
    data[1] = 0;
    data[2] = 0;
    data[3] = 0;
    data[4] = 0;

    for (int i = 0; i < 100; i++) {
        get_ads(5);
        data[0] +=
            rs_data_limit(g_My_AD[0], dead_zone, data_level);
        data[1] +=
            rs_data_limit(g_My_AD[1], dead_zone, data_level);
        data[2] +=
            rs_data_limit(g_My_AD[2], dead_zone, data_level);
        data[3] +=
            rs_data_limit(g_My_AD[3], dead_zone, data_level);
        data[4] += get_real_data(g_My_AD[4], 1);

    }

    data[0] /= 100;
    data[1] /= 100;
    data[2] /= 100;
    data[3] /= 100;
    data[4] /= 100;
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
 * @param 真实电压(mV) = 值与ADC分辨率之比 × 满量程电压(V)(V→mV)
 */
double get_real_data(uint16_t data, uint8_t t)
{
    if (!t)
    {
        int32_t val = (int32_t)data - 21000;
        if (val <= 0) {
            return 0;
        }
        return (uint32_t)val * 1024 / 655350;
    }else{
        return (uint32_t)data * 512 / 655350;
    }
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
