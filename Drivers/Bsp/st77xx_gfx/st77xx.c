/**
 * @file st77xx.c
 * @author PickingChip
 * @brief 
 * @version 0.2
 * @date 2025-11-09
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include "st77xx.h"
#include <stdint.h>

lcd_dev_t lcd_dev;
volatile uint16_t tearingEffectCount = 0;

void ST77xx_Reset(void) {
    HAL_GPIO_WritePin(LCD_RST_GPIO_Port, LCD_RST_Pin, GPIO_PIN_RESET);
    HAL_Delay(100);
    HAL_GPIO_WritePin(LCD_RST_GPIO_Port, LCD_RST_Pin, GPIO_PIN_SET);
    HAL_Delay(100);
}

void ST77xx_WriteCommand(uint8_t cmd) {
    HAL_GPIO_WritePin(LCD_DC_GPIO_Port, LCD_DC_Pin, GPIO_PIN_RESET);
    HAL_SPI_Transmit(&ST77xx_SPI_INSTANCE, &cmd, 1, HAL_MAX_DELAY);
}

void ST77xx_WriteByte(uint8_t data) {
    HAL_GPIO_WritePin(LCD_DC_GPIO_Port, LCD_DC_Pin, GPIO_PIN_SET);
    HAL_SPI_Transmit(&ST77xx_SPI_INSTANCE, &data, 1, HAL_MAX_DELAY);
}

void ST77xx_WriteData(uint8_t *data, size_t data_size) {

    HAL_GPIO_WritePin(LCD_DC_GPIO_Port, LCD_DC_Pin, GPIO_PIN_SET);
    HAL_SPI_Transmit(&ST77xx_SPI_INSTANCE, (uint8_t *)data, (uint16_t)data_size,
                     HAL_MAX_DELAY);
}

/**
 * @brief 设置显示窗口
 * 
 * @param x0 
 * @param y0 
 * @param x1 
 * @param y1 
 */
void ST77xx_SetAddressWindow(uint16_t x0, uint16_t y0, uint16_t x1,
                             uint16_t y1) {
    x0 += ST77xx_XSTART;
    y0 += ST77xx_YSTART;

    x1 += ST77xx_XSTART;
    y1 += ST77xx_YSTART;

    ST77xx_WriteCommand(ST7735_CASET);
    uint8_t data[] = {x0 >> 8, x0 & 0x00FF, x1 >> 8, x1 & 0x00FF};
    ST77xx_WriteData(data, sizeof(data));

    ST77xx_WriteCommand(ST7735_RASET);
    data[0] = y0 >> 8;
    data[1] = y0 & 0x00FF;
    data[2] = y1 >> 8;
    data[3] = y1 & 0x00FF;
    ST77xx_WriteData(data, sizeof(data));
}

volatile uint8_t IsTransmittingBlock_;
void ST77xx_Bitmap(const uint16_t *bitmap, uint16_t posx, uint16_t posy,
                   uint16_t sizex, uint16_t sizey) {
    IsTransmittingBlock_ = 1;
    ST77xx_SetAddressWindow(posx, posy, posx + sizex - 1, posy + sizey - 1);
    ST77xx_WriteCommand(ST7735_RAMWR);
    /* 将spi发送长度配置为16bit */
    while (((ST77xx_SPI_INSTANCE.Instance->SR) & SPI_FLAG_BSY) != RESET) {}
    __HAL_SPI_DISABLE(&ST77xx_SPI_INSTANCE);
    ST77xx_SPI_INSTANCE.Instance->CR1 |= SPI_DATASIZE_16BIT;
    __HAL_SPI_ENABLE(&ST77xx_SPI_INSTANCE);

    HAL_GPIO_WritePin(LCD_DC_GPIO_Port, LCD_DC_Pin, GPIO_PIN_SET);
    if (HAL_SPI_Transmit_DMA(&ST77xx_SPI_INSTANCE, (uint8_t *)bitmap,
                             (uint16_t)(sizex * sizey)) != HAL_OK) {
        IsTransmittingBlock_ = 0;
        __HAL_SPI_DISABLE(&ST77xx_SPI_INSTANCE);
        ST77xx_SPI_INSTANCE.Instance->CR1 &= ~SPI_DATASIZE_16BIT;
        __HAL_SPI_ENABLE(&ST77xx_SPI_INSTANCE);
    }
}

int touchgfxDisplayDriverTransmitActive(void) {
    return IsTransmittingBlock_;
}

void touchgfxDisplayDriverTransmitBlock(const uint8_t *pixels, uint16_t x,
                                        uint16_t y, uint16_t w, uint16_t h) {
    ST77xx_Bitmap((uint16_t *)pixels, x, y, w, h);
}

int touchgfxDisplayDriverShouldTransferBlock(uint16_t bottom) {
    // Only allow block transfer if the display has drawn past the bottom of the requested block (plus a margin of two lines)
    // A timer is used to estimate how many lines have been drawn by setting the prescaler so the tick rate matches the line draw rate
    // tearingEffectCount > 0;

    /* 这里由于没有te信号所以无法同步所以一直返回1 */
    return 1; 
}

//Signal TE interrupt to TouchGFX
void touchgfxSignalVSync(void);

void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi) {
    if (hspi->Instance == ST77xx_SPI_INSTANCE.Instance) {
        IsTransmittingBlock_ = 0;
        while (((ST77xx_SPI_INSTANCE.Instance->SR) & SPI_FLAG_BSY) != RESET) {}
        __HAL_SPI_DISABLE(&ST77xx_SPI_INSTANCE);
        ST77xx_SPI_INSTANCE.Instance->CR1 &= ~SPI_DATASIZE_16BIT;
        __HAL_SPI_ENABLE(&ST77xx_SPI_INSTANCE);
        // Signal Transfer Complete to TouchGFX
        DisplayDriver_TransferCompleteCallback();
    }
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
    if (htim == &htim2) {
        tearingEffectCount++;
        touchgfxSignalVSync(); // 调用 TouchGFX 的信号函数 (C 函数)
    }
}

/* 设置ST7735显示方向 */
void ST77xx_SetRotation(uint8_t rotation) {
    uint8_t madctl = 0;

    switch (rotation) {
        case 0:
            madctl = ST7735_MADCTL_MX | ST7735_MADCTL_MY |
                     ST77xx_MADCTL_MODE; /* 0XC0 */
            lcd_dev.width = ST77xx_WIDTH;
            lcd_dev.height = ST77xx_HEIGHT;
            break;
        case 1:
            madctl = ST7735_MADCTL_MY | ST7735_MADCTL_MV |
                     ST77xx_MADCTL_MODE; /*!< 0XA0 */
            lcd_dev.width = ST77xx_HEIGHT;
            lcd_dev.height = ST77xx_WIDTH;
            break;
        case 2:
            madctl = ST77xx_MADCTL_MODE; /*!< 0X00 */
            lcd_dev.width = ST77xx_WIDTH;
            lcd_dev.height = ST77xx_HEIGHT;
            break;
        case 3:
            madctl = ST7735_MADCTL_MX | ST7735_MADCTL_MV |
                     ST77xx_MADCTL_MODE; /* !< 0x60 */
            lcd_dev.width = ST77xx_HEIGHT;
            lcd_dev.height = ST77xx_WIDTH;
            break;
        default:
            break;
    }

    ST77xx_WriteCommand(ST7735_MADCTL);
    ST77xx_WriteByte(madctl);
}

/**
 * @brief ST7735屏幕初始化函数
 * @note 初始化之前请先初始化使用的SPI，模式：SPI_MODE_MASTER, SPI_CLK_MODE0, SPI_DATASIZE_8BIT, SPI_FIRSTBIT_MSB
 * 
 */
void ST77xx_Init(uint8_t dir, ic_type_t st77xx) {
    /* IO初始化 */
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    __HAL_RCC_GPIOB_CLK_ENABLE();

    GPIO_InitStruct.Pin = LCD_CS_Pin | LCD_DC_Pin | LCD_RST_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
    HAL_GPIO_WritePin(LCD_DC_GPIO_Port, LCD_DC_Pin, GPIO_PIN_RESET);

    lcd_dev.dir = dir;
    lcd_dev.id = st77xx;
    lcd_dev.width = ST77xx_WIDTH;
    lcd_dev.height = ST77xx_HEIGHT;

    if (lcd_dev.id == ST7735) {
        /* 屏幕初始化 */
        ST77xx_Reset();
        ST77xx_WriteCommand(ST7735_SLPOUT);
        HAL_Delay(120);
        ST77xx_SetRotation(lcd_dev.dir);
        ST77xx_WriteCommand(ST7735_FRMCTR1);
        ST77xx_WriteByte(0x01);
        ST77xx_WriteByte(0x2C);
        ST77xx_WriteByte(0x2D);
        ST77xx_WriteCommand(ST7735_FRMCTR2);
        ST77xx_WriteByte(0x01);
        ST77xx_WriteByte(0x2C);
        ST77xx_WriteByte(0x2D);
        ST77xx_WriteCommand(ST7735_FRMCTR3);
        ST77xx_WriteByte(0x01);
        ST77xx_WriteByte(0x2C);
        ST77xx_WriteByte(0x2D);
        ST77xx_WriteByte(0x01);
        ST77xx_WriteByte(0x2C);
        ST77xx_WriteByte(0x2D);
        ST77xx_WriteCommand(ST7735_INVCTR);
        ST77xx_WriteByte(0x07);
        ST77xx_WriteCommand(ST7735_PWCTR1);
        ST77xx_WriteByte(0xA2);
        ST77xx_WriteByte(0x02);
        ST77xx_WriteByte(0x84);
        ST77xx_WriteCommand(ST7735_PWCTR2);
        ST77xx_WriteByte(0xC5);
        ST77xx_WriteCommand(ST7735_PWCTR3);
        ST77xx_WriteByte(0x0A);
        ST77xx_WriteByte(0x00);
        ST77xx_WriteCommand(ST7735_PWCTR4);
        ST77xx_WriteByte(0x8A);
        ST77xx_WriteByte(0x2A);
        ST77xx_WriteCommand(ST7735_PWCTR5);
        ST77xx_WriteByte(0x8A);
        ST77xx_WriteByte(0xEE);
        ST77xx_WriteCommand(ST7735_VMCTR1);
        ST77xx_WriteByte(0x0E);
        ST77xx_WriteCommand(ST7735_INVERSE ? ST7735_INVON : ST7735_INVOFF);
        ST77xx_WriteCommand(ST7735_COLMOD);
        ST77xx_WriteByte(0x05);
        ST77xx_WriteCommand(ST7735_CASET); /* 设置窗口行地址 */
        ST77xx_WriteByte(0x00);
        ST77xx_WriteByte(0x00);
        ST77xx_WriteByte(0x00);
        ST77xx_WriteByte(0x81);
        ST77xx_WriteCommand(ST7735_RASET); /* 设置窗口列地址 */
        ST77xx_WriteByte(0x00);
        ST77xx_WriteByte(0x00);
        ST77xx_WriteByte(0x00);
        ST77xx_WriteByte(0xA1);
        ST77xx_WriteCommand(ST7735_GMCTRP1);
        ST77xx_WriteByte(0x02);
        ST77xx_WriteByte(0x1C);
        ST77xx_WriteByte(0x07);
        ST77xx_WriteByte(0x12);
        ST77xx_WriteByte(0x37);
        ST77xx_WriteByte(0x32);
        ST77xx_WriteByte(0x29);
        ST77xx_WriteByte(0x2D);
        ST77xx_WriteByte(0x29);
        ST77xx_WriteByte(0x25);
        ST77xx_WriteByte(0x2B);
        ST77xx_WriteByte(0x39);
        ST77xx_WriteByte(0x00);
        ST77xx_WriteByte(0x01);
        ST77xx_WriteByte(0x03);
        ST77xx_WriteByte(0x10);
        ST77xx_WriteCommand(ST7735_GMCTRN1);
        ST77xx_WriteByte(0x03);
        ST77xx_WriteByte(0x1D);
        ST77xx_WriteByte(0x07);
        ST77xx_WriteByte(0x06);
        ST77xx_WriteByte(0x2E);
        ST77xx_WriteByte(0x2C);
        ST77xx_WriteByte(0x29);
        ST77xx_WriteByte(0x2D);
        ST77xx_WriteByte(0x2E);
        ST77xx_WriteByte(0x2E);
        ST77xx_WriteByte(0x37);
        ST77xx_WriteByte(0x3F);
        ST77xx_WriteByte(0x00);
        ST77xx_WriteByte(0x00);
        ST77xx_WriteByte(0x02);
        ST77xx_WriteByte(0x10);
        ST77xx_WriteCommand(ST7735_NORON);
        HAL_Delay(10);
        ST77xx_WriteCommand(ST7735_DISPON);
        HAL_Delay(10);

    } else if (lcd_dev.id == ST7789) {
        ST77xx_Reset();
        ST77xx_WriteCommand(ST7789_SLPOUT);
        HAL_Delay(120);
        ST77xx_SetRotation(lcd_dev.dir);
        ST77xx_WriteCommand(ST7789_COLMOD);
        ST77xx_WriteByte(0x05);
        ST77xx_WriteCommand(ST7789_CMD_PORCH);
        ST77xx_WriteByte(0x0C);
        ST77xx_WriteByte(0x0C);
        ST77xx_WriteByte(0x00);
        ST77xx_WriteByte(0x33);
        ST77xx_WriteByte(0x33);
        ST77xx_WriteCommand(ST7789_CMD_GATECTRL);
        ST77xx_WriteByte(0x35);
        ST77xx_WriteCommand(ST7789_CMD_VCOM);
        ST77xx_WriteByte(0x32);
        ST77xx_WriteCommand(ST7789_CMD_VDVVRHEN);
        ST77xx_WriteByte(0x01);
        ST77xx_WriteCommand(ST7789_CMD_VRHS_SET);
        ST77xx_WriteByte(0x15); //GVDD=4.8V  颜色深度
        ST77xx_WriteCommand(ST7789_CMD_VDV_SET);
        ST77xx_WriteByte(0x20); //VDV, 0x20:0v
        ST77xx_WriteCommand(ST7789_CMD_FRAMERATE);
        ST77xx_WriteByte(0x0F); //0x0F:60Hz
        ST77xx_WriteCommand(ST7789_CMD_PV_GM);
        ST77xx_WriteByte(0xA4);
        ST77xx_WriteByte(0xA1);
        ST77xx_WriteCommand(ST7789_GMCTRP1);
        ST77xx_WriteByte(0xD0);
        ST77xx_WriteByte(0x08);
        ST77xx_WriteByte(0x0E);
        ST77xx_WriteByte(0x09);
        ST77xx_WriteByte(0x09);
        ST77xx_WriteByte(0x05);
        ST77xx_WriteByte(0x31);
        ST77xx_WriteByte(0x33);
        ST77xx_WriteByte(0x48);
        ST77xx_WriteByte(0x17);
        ST77xx_WriteByte(0x14);
        ST77xx_WriteByte(0x15);
        ST77xx_WriteByte(0x31);
        ST77xx_WriteByte(0x34);
        ST77xx_WriteCommand(ST7789_GMCTRN1);
        ST77xx_WriteByte(0xD0);
        ST77xx_WriteByte(0x08);
        ST77xx_WriteByte(0x0E);
        ST77xx_WriteByte(0x09);
        ST77xx_WriteByte(0x09);
        ST77xx_WriteByte(0x15);
        ST77xx_WriteByte(0x31);
        ST77xx_WriteByte(0x33);
        ST77xx_WriteByte(0x48);
        ST77xx_WriteByte(0x17);
        ST77xx_WriteByte(0x14);
        ST77xx_WriteByte(0x15);
        ST77xx_WriteByte(0x31);
        ST77xx_WriteByte(0x34);
        ST77xx_WriteCommand(ST7789_INVON);
        ST77xx_WriteCommand(ST7789_DISPON);
        HAL_Delay(10);
    }
}
