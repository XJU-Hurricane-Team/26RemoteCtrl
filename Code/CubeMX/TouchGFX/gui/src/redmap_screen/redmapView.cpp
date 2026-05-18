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

redmapView::redmapView():choosekey(0),r1_halt(1),r1_state(0),r1_accel_xy(0.0f),r1_yaw_source(0),in_sub(false),cur_tactical_idx(0)
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
        /* 0  无效 */ {nullptr,  0},
        /* 1  深红 */ {&box01,   kRedDeep},
        /* 2  浅粉 */ {&box02,   kPinkLight},
        /* 3  浅粉 */ {&box03,   kPinkLight},
        /* 4  浅粉 */ {&box04,   kPinkLight},
        /* 5  浅粉 */ {&box05,   kPinkLight},
        /* 6  浅粉 */ {&box11,   kPinkLight},
        /* 7  浅粉 */ {&box12,   kPinkLight},
        /* 8  浅粉 */ {&box13,   kPinkLight},
        /* 9  浅粉 */ {&box14,   kPinkLight},
        /* 10 浅粉 */ {&box15,   kPinkLight},
        /* 11 粉红 */ {&box21,   kPink},
        /* 12 粉红 */ {&box22,   kPink},
        /* 13 粉红 */ {&box23,   kPink},
        /* 14 粉红 */ {&box24,   kPink},
        /* 15 粉红 */ {&box25,   kPink},
        /* 16 粉红 */ {&box31,   kPink},
        /* 17 无效 */ {nullptr,  0},
        /* 18 无效 */ {nullptr,  0},
        /* 19 无效 */ {nullptr,  0},
        /* 20 粉红 */ {&box35,   kPink},
        /* 21 粉红 */ {&box41,   kPink},
        /* 22 无效 */ {nullptr,  0},
        /* 23 无效 */ {nullptr,  0},
        /* 24 无效 */ {nullptr,  0},
        /* 25 粉红 */ {&box45,   kPink},
        /* 26 粉红 */ {&box51,   kPink},
        /* 27 无效 */ {nullptr,  0},
        /* 28 无效 */ {nullptr,  0},
        /* 29 无效 */ {nullptr,  0},
        /* 30 粉红 */ {&box55,   kPink},
        /* 31 粉红 */ {&box61,   kPink},
        /* 32 无效 */ {nullptr,  0},
        /* 33 无效 */ {nullptr,  0},
        /* 34 无效 */ {nullptr,  0},
        /* 35 粉红 */ {&box65,   kPink},
        /* 36 粉红 */ {&box71,   kPink},
        /* 37 粉红 */ {&box72,   kPink},
        /* 38 粉红 */ {&box73,   kPink},
        /* 39 粉红 */ {&box74,   kPink},
        /* 40 粉红 */ {&box75,   kPink},
        /* 41 灰色 */ {&box81,   kGray},
        /* 42 亮红 */ {&box82,   kRedBright},
        /* 43 亮红 */ {&box83,   kRedBright},
        /* 44 亮红 */ {&box84,   kRedBright},
        /* 45 亮红 */ {&box85,   kRedBright},
        /* 46 深红 */ {&box91,   kRedDeep},
        /* 47 亮红 */ {&box92,   kRedBright},
        /* 48 亮红 */ {&box93,   kRedBright},
        /* 49 亮红 */ {&box94,   kRedBright},
        /* 50 亮红 */ {&box95,   kRedBright},
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
        uint8_t idx = (cur_tactical_idx < Model::kTacticalCount) ? cur_tactical_idx : 0;
        Unicode::strncpy(RunPointBuffer, Model::kTacticalNames[idx], RUNPOINT_SIZE);
        RunPoint.invalidate();
        return;
    }

    Unicode::snprintf(redchoosepointBuffer, REDCHOOSEPOINT_SIZE, "%d", choosekey);
    Unicode::snprintf(stateBuffer, STATE_SIZE, "%d", static_cast<unsigned int>(r1_halt));

    /* 控制模式 / 坐标系来源: 用 Model 共享文案表查表 */
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
