#include <gui/screen_screen/screenView.hpp>
#include <touchgfx/Unicode.hpp>
#include "stm32f4xx_hal.h"
#include <stdlib.h>

screenView::screenView()
    : keyState(0), voltage(0), pointvalue(0), rsL_x(0), rsL_y(0), rsR_x(0), rsR_y(0),
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
    touchgfx::Unicode::snprintf(BatteryBuffer, BATTERY_SIZE, "%d", voltage);
    touchgfx::Unicode::snprintf(pointBuffer, POINT_SIZE, "%d", pointvalue);

    point.invalidate();
    RockL.invalidate();
    RockR.invalidate();
    KeyNum.invalidate();
    Battery.invalidate();
}
