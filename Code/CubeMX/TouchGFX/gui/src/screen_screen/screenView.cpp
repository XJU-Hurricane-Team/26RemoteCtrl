#include <gui/screen_screen/screenView.hpp>
#include <touchgfx/Unicode.hpp>
#include "stm32f4xx_hal.h"
#include <stdlib.h>

namespace {
    /* 格式化固定小数点 */
void formatFixed2(touchgfx::Unicode::UnicodeChar *buffer, uint16_t bufferSize,
                  float value) {
    int32_t scaled = static_cast<int32_t>(value * 10.0f);
    int32_t absScaled = (scaled < 0) ? -scaled : scaled;
    int32_t intPart = absScaled / 10;
    int32_t fracPart = absScaled % 10;

    if (scaled < 0) {
        touchgfx::Unicode::snprintf(buffer, bufferSize, "-%d.%01d", intPart,
                                    fracPart);
    } else {
        touchgfx::Unicode::snprintf(buffer, bufferSize, "%d.%01d", intPart,
                                    fracPart);
    }
}
} // namespace

screenView::screenView()
    : keyState(0), voltage(0.0f), pointvalue(0), rsL_x(0), rsL_y(0), rsR_x(0), rsR_y(0),
      graphValue(0.0f), tickCounter(0), digitalHours(0),
      digitalMinutes(0), digitalSeconds(0) {}

void screenView::setupScreen() {
    screenViewBase::setupScreen();
}

void screenView::tearDownScreen() {
    screenViewBase::tearDownScreen();
}

/**
 * @brief Handles the tick event for the screen
 */
void screenView::handleTickEvent() {
    screenViewBase::handleTickEvent();

    // tickCounter++;

    uint32_t now = HAL_GetTick();
    uint32_t totalSeconds = now / 1000u;
    totalSeconds %= (24u * 3600u);

    digitalHours = totalSeconds / 3600u;
    digitalMinutes = (totalSeconds % 3600u) / 60u;
    digitalSeconds = totalSeconds % 60u;

    // Update the clock display
    RunningTime.setTime24Hour(digitalHours, digitalMinutes, digitalSeconds);
}

void screenView::handleKeyEvent(uint8_t key){
    screenViewBase::handleKeyEvent(key);
}

/**
 * @brief Updates the information displayed on the screen
 * 
 */
void screenView::InfoUpdate1() {
    // 使用每个 wildcard 对应的缓冲区大小常量，避免写越界
    // 更新按键显示并刷新所有被修改的 text areas
    touchgfx::Unicode::snprintf(RockLBuffer1, ROCKLBUFFER1_SIZE, "%d", rsL_x);
    touchgfx::Unicode::snprintf(RockLBuffer2, ROCKLBUFFER2_SIZE, "%d", rsL_y);
    touchgfx::Unicode::snprintf(RockRBuffer1, ROCKRBUFFER1_SIZE, "%d", rsR_x);
    touchgfx::Unicode::snprintf(RockRBuffer2, ROCKRBUFFER2_SIZE, "%d", rsR_y);
    touchgfx::Unicode::snprintf(KeyNumBuffer, KEYNUM_SIZE, "%d", keyState);
    formatFixed2(BatteryBuffer, BATTERY_SIZE, voltage);
    touchgfx::Unicode::snprintf(pointBuffer, POINT_SIZE, "%d", pointvalue);

    point.invalidate();
    RockL.invalidate();
    RockR.invalidate();
    KeyNum.invalidate();
    Battery.invalidate();
}
