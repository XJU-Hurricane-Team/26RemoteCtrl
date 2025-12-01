
#include "adc.h"

#include "FreeRTOS.h"
#include "semphr.h"
#include "ads868x/ads868x.h"
#include "../core/core_delay.h"

// static SemaphoreHandle_t rs_conv_cplt_semp;

void adc_key_init(void)
{
    GPIO_InitTypeDef gpio_init_struct = {.Mode = GPIO_MODE_INPUT,
                                         .Pull = GPIO_PULLUP,
                                         .Speed = GPIO_SPEED_FREQ_HIGH};

    LEFT_Z_GPIO_ENABLE();
    gpio_init_struct.Pin = LEFT_Z_GPIO_PIN;
    HAL_GPIO_Init(LEFT_Z_GPIO_PORT, &gpio_init_struct);

    RIGHT_Z_GPIO_ENABLE();
    gpio_init_struct.Pin = RIGHT_Z_GPIO_PIN;
    HAL_GPIO_Init(RIGHT_Z_GPIO_PORT, &gpio_init_struct);

    LEFT_T_GPIO_ENABLE();
    gpio_init_struct.Pin = LEFT_T_GPIO_PIN;
    HAL_GPIO_Init(LEFT_T_GPIO_PORT, &gpio_init_struct);

    RIGHT_T_GPIO_ENABLE();
    gpio_init_struct.Pin = RIGHT_T_GPIO_PIN;
    HAL_GPIO_Init(RIGHT_T_GPIO_PORT, &gpio_init_struct);
}

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
    uint16_t adc_max = 40000;
    uint16_t adc_mid = 22500;
    uint16_t adc_min = 5000;

    // uint16_t adc_max = 41000;
    // uint16_t adc_mid = 22500;
    // uint16_t adc_min = 4000;

    float dead_zone_threshold = (adc_max - adc_min) * dead_zone / 100;
    // float dead_zone_threshold = adc_max * dead_zone / 100;
    uint16_t data_piece = (uint16_t)((adc_mid - adc_min - dead_zone_threshold) * 2 /
                                     data_level); /* 计算每一份的值 */
    uint8_t data_res = 0;

    if( data >= (adc_mid - dead_zone_threshold) && 
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
        data[4] += get_real_data(g_My_AD[4], 0);

    }

    data[0] /= 100;
    data[1] /= 100;
    data[2] /= 100;
    data[3] /= 100;
    data[4] /= 100;
}

/**
 * @brief 获取摇杆 z 轴
 * 
 * @return 摇杆是否按下 z 轴
 *  @retval - 0: 未按下
 *  @retval - 1: 左摇杆按下
 *  @retval - 2: 右摇杆按下
 *  @retval - 3: 左上按键按下
 *  @retval - 4: 左上按键按下
 * @note 两个摇杆同时按下只会响应左边的摇杆
 */
uint32_t rs_get_z(void) 
{
    if ((HAL_GPIO_ReadPin(LEFT_Z_GPIO_PORT, LEFT_Z_GPIO_PIN) == GPIO_PIN_SET) &&
        (HAL_GPIO_ReadPin(RIGHT_Z_GPIO_PORT, RIGHT_Z_GPIO_PIN) == GPIO_PIN_SET) &&
        (HAL_GPIO_ReadPin(LEFT_T_GPIO_PORT, LEFT_T_GPIO_PIN) == GPIO_PIN_SET) &&
        (HAL_GPIO_ReadPin(RIGHT_T_GPIO_PORT, RIGHT_T_GPIO_PIN) == GPIO_PIN_SET)) {
        return 0;
    }

    delay_ms(10);

    if (HAL_GPIO_ReadPin(LEFT_Z_GPIO_PORT, LEFT_Z_GPIO_PIN) == 
        GPIO_PIN_RESET) {
        return 1;
    }

    if (HAL_GPIO_ReadPin(RIGHT_Z_GPIO_PORT, RIGHT_Z_GPIO_PIN) ==
        GPIO_PIN_RESET) {
        return 2;
    }

    if (HAL_GPIO_ReadPin(LEFT_T_GPIO_PORT, LEFT_T_GPIO_PIN) ==
        GPIO_PIN_RESET) {
        return 3;
    }

    if (HAL_GPIO_ReadPin(RIGHT_T_GPIO_PORT, RIGHT_T_GPIO_PIN) ==
        GPIO_PIN_RESET) {
        return 4;
    }

    return 0;
}