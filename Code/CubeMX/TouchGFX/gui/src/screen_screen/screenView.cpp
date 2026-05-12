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
    : keyState(0), voltage(0), pointvalue(0), rsL_x(0), rsL_y(0), rsR_x(0), rsR_y(0),
      r1_x_speed(0), r1_y_speed(0), r1_w_speed(0), r1_status(1),r1_state(0), r1_accel_xy(0.0f),r1_yaw_source(0),
      r1_left_pos(0.0f), r1_right_pos(0.0f), r1_left_adsorbed(0),
      r1_right_adsorbed(0), r1_send_msg(0), r1_rec_msg(0), 
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
    touchgfx::Unicode::snprintf(KeyNumBuffer, KEYNUM_SIZE, "Key%d", keyState);
    touchgfx::Unicode::snprintf(BatteryBuffer, BATTERY_SIZE, "%d", voltage);
    touchgfx::Unicode::snprintf(pointBuffer, POINT_SIZE, "%d", pointvalue);
    touchgfx::Unicode::snprintf(R1V_xBuffer, R1V_X_SIZE, "%d", r1_x_speed);
    touchgfx::Unicode::snprintf(R1V_yBuffer, R1V_Y_SIZE, "%d", r1_y_speed);
    touchgfx::Unicode::snprintf(R1YawBuffer, R1YAW_SIZE, "%d", r1_w_speed);
    touchgfx::Unicode::snprintf(R1StateBuffer, R1STATE_SIZE, "%u",
                                static_cast<unsigned int>(r1_status));
    touchgfx::Unicode::snprintf(CTRLBuffer, CTRL_SIZE, "%d", r1_state);
    formatFixed2(ACCELBuffer, ACCEL_SIZE, r1_accel_xy);
    touchgfx::Unicode::snprintf(ACCELBuffer, ACCEL_SIZE, "%u", static_cast<unsigned int>(r1_accel_xy));
    formatFixed2(L_PBuffer, L_P_SIZE, r1_left_pos);
    formatFixed2(R_PBuffer, R_P_SIZE, r1_right_pos);
    touchgfx::Unicode::snprintf(L_ABuffer, L_A_SIZE, "%u",
                                static_cast<unsigned int>(r1_left_adsorbed));
    touchgfx::Unicode::snprintf(R_ABuffer, R_A_SIZE, "%u",
                                static_cast<unsigned int>(r1_right_adsorbed));
    touchgfx::Unicode::snprintf(SOURCEBuffer, SOURCE_SIZE, "%d", r1_yaw_source);

    switch (msgnum)
    {
    case 0:
        touchgfx::Unicode::snprintf(R1_MSGBuffer, R1_MSG_SIZE, "NULL", msgnum);
        break;

    case 1:
        touchgfx::Unicode::snprintf(R1_MSGBuffer, R1_MSG_SIZE, "Assembled_weapon", msgnum);
        break;

    case 2:
        touchgfx::Unicode::snprintf(R1_MSGBuffer, R1_MSG_SIZE, "AWAY_MARTIAL", msgnum);
        break;

    case 3:
        touchgfx::Unicode::snprintf(R1_MSGBuffer, R1_MSG_SIZE, "AWAY_MEILIN", msgnum);
        break;

    case 4:
        touchgfx::Unicode::snprintf(R1_MSGBuffer, R1_MSG_SIZE, "COMPLEX", msgnum);
        break;

    case 5:
        touchgfx::Unicode::snprintf(R1_MSGBuffer, R1_MSG_SIZE, "CAN_PUT", msgnum);
        break;

    case 6:
        touchgfx::Unicode::snprintf(R1_MSGBuffer, R1_MSG_SIZE, "pre", msgnum);
        break;
    
    default:
        break;
    }
    

    point.invalidate();
    RockL.invalidate();
    RockR.invalidate();
    KeyNum.invalidate();
    Battery.invalidate();
    SOURCE.invalidate();

    R1V_x.invalidate();
    R1V_y.invalidate();
    R1Yaw.invalidate();
    R1State.invalidate();
    R1_MSG.invalidate();
    CTRL.invalidate();
    ACCEL.invalidate();
    L_P.invalidate();
    R_P.invalidate();
    L_A.invalidate();
    R_A.invalidate();

    // R2V_x.invalidate();
    // R2V_y.invalidate();
    // R2Yaw.invalidate();
    // R2State.invalidate();
}
