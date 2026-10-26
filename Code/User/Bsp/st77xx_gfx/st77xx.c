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

extern void touchgfxNotifyBlockTransferDone(void);
extern void touchgfxNotifyBlockTransferDoneFromISR(void);

static inline uint8_t st77xx_in_isr_context(void) {
    return (__get_IPSR() != 0U) ? 1U : 0U;
}

static inline void st77xx_notify_transfer_done(void) {
    if (st77xx_in_isr_context()) {
        touchgfxNotifyBlockTransferDoneFromISR();
    } else {
        touchgfxNotifyBlockTransferDone();
    }
}

lcd_dev_t lcd_dev;
volatile uint16_t tearingEffectCount = 0;

#define ST77XX_CS_LOW()  HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_RESET)
#define ST77XX_CS_HIGH() HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_SET)

void ST77xx_WriteCommand(uint8_t cmd) {
    ST77XX_CS_LOW();
    HAL_GPIO_WritePin(LCD_DC_GPIO_Port, LCD_DC_Pin, GPIO_PIN_RESET);
    HAL_SPI_Transmit(&ST77xx_SPI_INSTANCE, &cmd, 1, HAL_MAX_DELAY);
    ST77XX_CS_HIGH();
}

void ST77xx_WriteByte(uint8_t data) {
    ST77XX_CS_LOW();
    HAL_GPIO_WritePin(LCD_DC_GPIO_Port, LCD_DC_Pin, GPIO_PIN_SET);
    HAL_SPI_Transmit(&ST77xx_SPI_INSTANCE, &data, 1, HAL_MAX_DELAY);
    ST77XX_CS_HIGH();
}

void ST77xx_WriteData(uint8_t *data, size_t data_size) {
    ST77XX_CS_LOW();
    HAL_GPIO_WritePin(LCD_DC_GPIO_Port, LCD_DC_Pin, GPIO_PIN_SET);
    HAL_SPI_Transmit(&ST77xx_SPI_INSTANCE, (uint8_t *)data, (uint16_t)data_size,
                     HAL_MAX_DELAY);
    ST77XX_CS_HIGH();
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

enum {
    LCD_TX_IDLE, LCD_TX_ACTIVE, LCD_TX_DONE, LCD_TX_ERROR,
    LCD_TX_RECOVERING, LCD_TX_FATAL
};
static volatile uint8_t lcdTransferState;
static uint32_t lcdTransferStarted;
static uint8_t lcdRetries;
static const uint16_t *lcdPixels;
static uint16_t lcdX, lcdY, lcdW, lcdH;

/* Debugger-visible diagnostics. Failure: 1=command, 2=DMA start,
 * 3=SPI IRQ, 4=completion timeout, 5=invalid block. */
volatile uint32_t lcdLastFailure;
volatile uint32_t lcdLastSpiError;
volatile uint32_t lcdLastHalStatus;
volatile uint32_t lcdRecoveryCount;
volatile uint32_t lcdFatalCount;

/* These bounded command writes are used only from the GUI task. */
static HAL_StatusTypeDef lcd_write8(uint8_t dataMode,
                                  const uint8_t *data, uint16_t size) {
    ST77XX_CS_LOW();
    HAL_GPIO_WritePin(LCD_DC_GPIO_Port, LCD_DC_Pin,
                      dataMode ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_StatusTypeDef status = HAL_SPI_Transmit(
        &ST77xx_SPI_INSTANCE, data, size, 20U);
    ST77XX_CS_HIGH();
    return status;
}

static void lcd_restore8(void) {
    __HAL_SPI_DISABLE(&ST77xx_SPI_INSTANCE);
    ST77xx_SPI_INSTANCE.Init.DataSize = SPI_DATASIZE_8BIT;
    CLEAR_BIT(ST77xx_SPI_INSTANCE.Instance->CR1, SPI_CR1_DFF);
    ST77XX_CS_HIGH();
    __HAL_SPI_ENABLE(&ST77xx_SPI_INSTANCE);
}

static void lcd_start_attempt(void) {
    SPI_HandleTypeDef *hspi = &ST77xx_SPI_INSTANCE;
    uint16_t x0 = lcdX + ST77xx_XSTART;
    uint16_t y0 = lcdY + ST77xx_YSTART;
    uint16_t x1 = x0 + lcdW - 1U;
    uint16_t y1 = y0 + lcdH - 1U;
    uint8_t xdata[4] = {x0 >> 8, x0 & 0xffU, x1 >> 8, x1 & 0xffU};
    uint8_t ydata[4] = {y0 >> 8, y0 & 0xffU, y1 >> 8, y1 & 0xffU};
    uint8_t cmd = ST7735_CASET;
    HAL_StatusTypeDef status;

    lcdTransferStarted = HAL_GetTick();
    lcdTransferState = LCD_TX_ACTIVE;

    /* A non-idle peripheral must be recovered before touching DFF. */
    if (hspi->State != HAL_SPI_STATE_READY ||
        __HAL_SPI_GET_FLAG(hspi, SPI_FLAG_BSY) != RESET) {
        status = HAL_BUSY;
        goto command_failed;
    }
    lcd_restore8();
    status = lcd_write8(0U, &cmd, 1U);
    if (status != HAL_OK) goto command_failed;
    status = lcd_write8(1U, xdata, sizeof(xdata));
    if (status != HAL_OK) goto command_failed;
    cmd = ST7735_RASET;
    status = lcd_write8(0U, &cmd, 1U);
    if (status != HAL_OK) goto command_failed;
    status = lcd_write8(1U, ydata, sizeof(ydata));
    if (status != HAL_OK) goto command_failed;
    cmd = ST7735_RAMWR;
    status = lcd_write8(0U, &cmd, 1U);
    if (status != HAL_OK) goto command_failed;

    __HAL_SPI_DISABLE(hspi);
    hspi->Init.DataSize = SPI_DATASIZE_16BIT;
    SET_BIT(hspi->Instance->CR1, SPI_CR1_DFF);
    __HAL_SPI_ENABLE(hspi);
    ST77XX_CS_LOW();
    HAL_GPIO_WritePin(LCD_DC_GPIO_Port, LCD_DC_Pin, GPIO_PIN_SET);
    status = HAL_SPI_Transmit_DMA(hspi, (const uint8_t *)lcdPixels,
                                  (uint16_t)((uint32_t)lcdW * lcdH));
    if (status != HAL_OK) {
        lcdLastHalStatus = status;
        lcdLastSpiError = hspi->ErrorCode;
        lcdLastFailure = 2U;
        lcdTransferState = LCD_TX_ERROR;
    }
    return;

command_failed:
    lcdLastHalStatus = status;
    lcdLastSpiError = hspi->ErrorCode;
    lcdLastFailure = 1U;
    lcdTransferState = LCD_TX_ERROR;
}

void ST77xx_Bitmap(const uint16_t *bitmap, uint16_t posx, uint16_t posy,
                   uint16_t sizex, uint16_t sizey) {
    IsTransmittingBlock_ = 1U;
    lcdPixels = bitmap;
    lcdX = posx;
    lcdY = posy;
    lcdW = sizex;
    lcdH = sizey;
    lcdRetries = 0U;
    if (bitmap == NULL || sizex == 0U || sizey == 0U ||
        (uint32_t)sizex * sizey > 65535U) {
        lcdLastFailure = 5U;
        lcdFatalCount++;
        lcdTransferState = LCD_TX_FATAL;
        return;
    }
    lcd_start_attempt();
}

/* Project-specific: LCD exclusively owns SPI4 / DMA2_Stream1.
 * Call only in GUI task with scheduler and tick interrupts running. */
static uint8_t lcd_recover(void) {
    SPI_HandleTypeDef *hspi = &ST77xx_SPI_INSTANCE;
    DMA_HandleTypeDef *hdma = hspi->hdmatx;
    uint32_t dmaIrqEnabled = NVIC_GetEnableIRQ(DMA2_Stream1_IRQn);
    uint32_t spiIrqEnabled = NVIC_GetEnableIRQ(SPI4_IRQn);
    uint8_t ok = 0U;
    uint32_t started;

    NVIC_DisableIRQ(DMA2_Stream1_IRQn);
    NVIC_DisableIRQ(SPI4_IRQn);
    __DSB();
    __ISB();
    CLEAR_BIT(hspi->Instance->CR2, SPI_CR2_TXDMAEN | SPI_CR2_RXDMAEN |
              SPI_CR2_ERRIE | SPI_CR2_TXEIE | SPI_CR2_RXNEIE);

    if (hdma == NULL) goto finish;
    /* SPI_DMAError already clears TXDMAEN. Therefore abort the DMA
     * explicitly, instead of relying only on HAL_SPI_Abort[_IT]. */
    if (hdma->State == HAL_DMA_STATE_BUSY) {
        (void)HAL_DMA_Abort(hdma);
    }
    __HAL_DMA_DISABLE(hdma);
    started = HAL_GetTick();
    while ((hdma->Instance->CR & DMA_SxCR_EN) != 0U) {
        if ((uint32_t)(HAL_GetTick() - started) >= 20U) goto finish;
    }
    /* No DMA can read the retained framebuffer now. Reset only SPI4,
     * never DMA2 as a whole (other peripherals may share DMA2). */
    ST77XX_CS_HIGH();
    __HAL_SPI_DISABLE(hspi);
    __HAL_RCC_SPI4_FORCE_RESET();
    __HAL_RCC_SPI4_RELEASE_RESET();
    if (HAL_DMA_DeInit(hdma) != HAL_OK) goto finish;
    if (HAL_DMA_Init(hdma) != HAL_OK) goto finish;
    hspi->Init.DataSize = SPI_DATASIZE_8BIT;
    if (HAL_SPI_Init(hspi) != HAL_OK) goto finish;
    ok = 1U;

finish:
    NVIC_ClearPendingIRQ(DMA2_Stream1_IRQn);
    NVIC_ClearPendingIRQ(SPI4_IRQn);
    if (dmaIrqEnabled) NVIC_EnableIRQ(DMA2_Stream1_IRQn);
    if (spiIrqEnabled) NVIC_EnableIRQ(SPI4_IRQn);
    return ok;
}

/* The only driver location allowed to release a TouchGFX block. */
void ST77xx_ProcessTransfer(void) {
    if (lcdTransferState == LCD_TX_ACTIVE &&
        (uint32_t)(HAL_GetTick() - lcdTransferStarted) >= 100U) {
        /* Claim the timeout atomically against the DMA callback. */
        uint32_t mask = __get_PRIMASK();
        __disable_irq();
        if (lcdTransferState == LCD_TX_ACTIVE) {
            lcdLastFailure = 4U;
            lcdTransferState = LCD_TX_ERROR;
        }
        __set_PRIMASK(mask);
    }

    if (lcdTransferState == LCD_TX_ERROR) {
        lcdTransferState = LCD_TX_RECOVERING;
        lcdRecoveryCount++;
        if (lcdRetries >= 2U || !lcd_recover()) {
            lcdFatalCount++;
            lcdTransferState = LCD_TX_FATAL;
            /* Keep busy and retain the block. Existing GUI watchdog
             * policy can reset the board; do not fake completion. */
            return;
        }
        lcdRetries++;
        /* Retry SAME block and window; do not free or advance allocator. */
        lcd_start_attempt();
        return;
    }

    if (lcdTransferState == LCD_TX_DONE) {
        lcd_restore8();
        lcdTransferState = LCD_TX_IDLE;
        IsTransmittingBlock_ = 0U;
        /* This may start the next block, but no completion is processed
         * recursively. A later ProcessTransfer handles its result. */
        DisplayDriver_TransferCompleteCallback();
    }
}

int touchgfxDisplayDriverTransmitActive(void) {
    return IsTransmittingBlock_;
}

void touchgfxDisplayDriverTransmitBlock(const uint8_t *pixels, uint16_t x,
                                        uint16_t y, uint16_t w, uint16_t h) {
    ST77xx_Bitmap((uint16_t *)pixels, x, y, w, h);
}

int touchgfxDisplayDriverShouldTransferBlock(void) {
    // Only allow block transfer if the display has drawn past the bottom of the requested block (plus a margin of two lines)
    // A timer is used to estimate how many lines have been drawn by setting the prescaler so the tick rate matches the line draw rate
    // tearingEffectCount > 0;

    /* 这里由于没有te信号所以无法同步所以一直返回 1 */
    return 1;
}

//Signal TE interrupt to TouchGFX
void touchgfxSignalVSync(void);

/* IRQ callbacks report events only. Allocator operations run in GUI task. */
void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi) {
    if (hspi == &ST77xx_SPI_INSTANCE && lcdTransferState == LCD_TX_ACTIVE) {
        lcdTransferState = LCD_TX_DONE;
        st77xx_notify_transfer_done();
    }
}

void HAL_SPI_ErrorCallback(SPI_HandleTypeDef *hspi) {
    if (hspi == &ST77xx_SPI_INSTANCE &&
        (lcdTransferState == LCD_TX_ACTIVE || lcdTransferState == LCD_TX_DONE)) {
        lcdLastSpiError = hspi->ErrorCode;
        lcdLastFailure = 3U;
        lcdTransferState = LCD_TX_ERROR;
        st77xx_notify_transfer_done();
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

    lcd_dev.dir = dir;
    lcd_dev.id = st77xx;
    lcd_dev.width = ST77xx_WIDTH;
    lcd_dev.height = ST77xx_HEIGHT;

    if (lcd_dev.id == ST7735) {
        /* 屏幕初始化 */
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
        ST77xx_WriteCommand(ST7789_SWRESET);
        HAL_Delay(120);
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
