/**
 * @file    redmapView.cpp
 * @brief   红方地图视图实现
 */
#include <gui/redmap_screen/redmapView.hpp>
#include <gui/model/Model.hpp>
#include <touchgfx/Color.hpp>

/* 格式化浮点数为 xx.x 格式，如 "12.3" */
static void formatFixed2(touchgfx::Unicode::UnicodeChar *buffer, uint16_t bufferSize,
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

redmapView::redmapView():choosekey(0),r1_halt(1),r1_state(0),r1_accel_xy(0.0f),r1_yaw_source(0),in_sub(false),cur_preset_idx(0)
{

}

void redmapView::setupScreen()
{
    redmapViewBase::setupScreen();
}

void redmapView::tearDownScreen()
{
    redmapViewBase::tearDownScreen();
}

void redmapView::handleKeyEvent(uint8_t key){
    redmapViewBase::handleKeyEvent(key);
}

void redmapView::colortoggleEvent(uint8_t point) {
    static uint8_t last_point = 0;

    static const touchgfx::colortype kActiveColor = 0xBB00FF;   // 高亮: 紫色
    static const touchgfx::colortype kDefaultColor = 0xFCFBEB;  // 默认: 浅粉

    static const touchgfx::colortype kRedDeep   = 0xD60000;  // 深红
    static const touchgfx::colortype kPink      = 0xFAA5A5;  // 粉红
    static const touchgfx::colortype kPinkLight = 0xFCFBEB;  // 浅粉
    static const touchgfx::colortype kRedBright = 0xFF8C8C;  // 亮红
    static const touchgfx::colortype kGray      = 0xA3A2A2;  // 灰色

    struct PointConfig {
        touchgfx::BoxWithBorder* box;
        touchgfx::colortype color;
    };

    static const PointConfig kPoints[51] = {
         {nullptr,  0},
         {&box01,   kRedDeep},
         {&box02,   kPinkLight},
         {&box03,   kPinkLight},
         {&box04,   kPinkLight},
         {&box05,   kPinkLight},
         {&box11,   kPinkLight},
         {&box12,   kPinkLight},
         {&box13,   kPinkLight},
         {&box14,   kPinkLight},
         {&box15,   kPinkLight},
         {&box21,   kPink},
         {&box22,   kPink},
         {&box23,   kPink},
         {&box24,   kPink},
         {&box25,   kPink},
         {&box31,   kPink},
         {nullptr,  0},
         {nullptr,  0},
         {nullptr,  0},
         {&box35,   kPink},
         {&box41,   kPink},
         {nullptr,  0},
         {nullptr,  0},
         {nullptr,  0},
         {&box45,   kPink},
         {&box51,   kPink},
         {nullptr,  0},
         {nullptr,  0},
         {nullptr,  0},
         {&box55,   kPink},
         {&box61,   kPink},
         {nullptr,  0},
         {nullptr,  0},
         {nullptr,  0},
         {&box65,   kPink},
         {&box71,   kPink},
         {&box72,   kPink},
         {&box73,   kPink},
         {&box74,   kPink},
         {&box75,   kPink},
         {&box81,   kGray},
         {&box82,   kRedBright},
         {&box83,   kRedBright},
         {&box84,   kRedBright},
         {&box85,   kRedBright},
         {&box91,   kRedDeep},
         {&box92,   kRedBright},
         {&box93,   kRedBright},
         {&box94,   kRedBright},
         {&box95,   kRedBright},
    };

    // 恢复上次选中的默认颜色，设置新的高亮色
    if (last_point != 0 && last_point <= 50 && kPoints[last_point].box) {
        kPoints[last_point].box->setColor(
            kPoints[last_point].color ? kPoints[last_point].color : kDefaultColor);
        kPoints[last_point].box->invalidate();
    }

    if (point != 0 && point <= 50 && kPoints[point].box) {
        kPoints[point].box->setColor(kActiveColor);
        kPoints[point].box->invalidate();
    }

    last_point = point;
}


void redmapView::update2() {
    static bool last_in_sub = false;
    if (in_sub != last_in_sub) {
        container4.setVisible(!in_sub);
        containerTact.setVisible(in_sub);
        container4.invalidate();
        containerTact.invalidate();
        last_in_sub = in_sub;
    }
    if (in_sub) {
        uint8_t idx = (cur_preset_idx < Model::kPresetPointCount) ? cur_preset_idx : 0;
        Unicode::strncpy(RunPointBuffer, Model::kPresetPointNames[idx], RUNPOINT_SIZE);
        RunPoint.invalidate();
        return;
    }

    Unicode::snprintf(redchoosepointBuffer, REDCHOOSEPOINT_SIZE, "%d", choosekey);
    Unicode::snprintf(stateBuffer, STATE_SIZE, "%d", static_cast<unsigned int>(r1_halt));

    CTRLBuffer[0]   = (r1_state < 2)       ? Model::kCtrlLabels[r1_state]     : Model::kCtrlLabels[0];
    CTRLBuffer[1]   = 0;
    SOURCEBuffer[0] = (r1_yaw_source < 2)  ? Model::kSourceLabels[r1_yaw_source] : Model::kSourceLabels[0];
    SOURCEBuffer[1] = 0;

    // FIXME: 第2行覆盖第1行，ACCEL始终显示为整数，后续可能换为模式，暂时不解决
    formatFixed2(ACCELBuffer, ACCEL_SIZE, r1_accel_xy);
    Unicode::snprintf(ACCELBuffer, ACCEL_SIZE, "%u", static_cast<unsigned int>(r1_accel_xy));

    redchoosepoint.invalidate();
    state.invalidate();
    CTRL.invalidate();
    ACCEL.invalidate();
    SOURCE.invalidate();
}
