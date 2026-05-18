/**
 * @file    bluemapView.cpp
 * @brief   蓝方地图视图实现
 */
#include <gui/bluemap_screen/bluemapView.hpp>
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

bluemapView::bluemapView():choosekey(0),r1_halt(1),r1_state(0),r1_accel_xy(0.0f),r1_yaw_source(0),in_sub(false),last_in_sub(false),cur_preset_idx(0)
{

}

void bluemapView::setupScreen()
{
    bluemapViewBase::setupScreen();
}

void bluemapView::tearDownScreen()
{
    bluemapViewBase::tearDownScreen();
}

void bluemapView::handleKeyEvent(uint8_t key){
    bluemapViewBase::handleKeyEvent(key);
}

void bluemapView::colortoggleEvent(uint8_t point) {
    static uint8_t last_point = 0;

    static const touchgfx::colortype kActiveColor = 0xBB00FF;   // 高亮: 紫色
    static const touchgfx::colortype kDefaultColor = 0x00EEFF;  // 默认: 青色

    static const touchgfx::colortype kBlueDeep  = 0x003CFF;   // 深蓝
    static const touchgfx::colortype kBlueLight = 0xD4FCFF;   // 浅蓝
    static const touchgfx::colortype kCyan      = 0x00EEFF;   // 青色
    static const touchgfx::colortype kBlueSky   = 0x3CC0FA;   // 天蓝

    struct PointConfig {
        touchgfx::BoxWithBorder* box;
        touchgfx::colortype color;
    };

    static const PointConfig kPoints[51] = {
         {nullptr,  0},
         {&box95,   kBlueDeep},
         {&box94,   kBlueLight},
         {&box93,   kBlueLight},
         {&box92,   kBlueLight},
         {&box91,   kBlueLight},
         {&box85,   kBlueLight},
         {&box84,   kBlueLight},
         {&box83,   kBlueLight},
         {&box82,   kBlueLight},
         {&box81,   kBlueLight},
         {&box75,   kCyan},
         {&box74,   kCyan},
         {&box73,   kCyan},
         {&box72,   kCyan},
         {&box71,   kCyan},
         {&box65,   kCyan},
         {nullptr,  0},
         {nullptr,  0},
         {nullptr,  0},
         {&box61,   kCyan},
         {&box55,   kCyan},
         {nullptr,  0},
         {nullptr,  0},
         {nullptr,  0},
         {&box51,   kCyan},
         {&box45,   kCyan},
         {nullptr,  0},
         {nullptr,  0},
         {nullptr,  0},
         {&box41,   kCyan},
         {&box35,   kCyan},
         {nullptr,  0},
         {nullptr,  0},
         {nullptr,  0},
         {&box31,   kCyan},
         {&box25,   kCyan},
         {&box24,   kCyan},
         {&box23,   kCyan},
         {&box22,   kCyan},
         {&box21,   kCyan},
         {&box15,   kBlueLight},
         {&box14,   kBlueSky},
         {&box13,   kBlueSky},
         {&box12,   kBlueSky},
         {&box11,   kBlueSky},
         {&box05,   kBlueSky},
         {&box04,   kBlueSky},
         {&box03,   kBlueSky},
         {&box02,   kBlueSky},
         {&box01,   kBlueDeep},
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

void bluemapView::update2() {
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

    Unicode::snprintf(bluechoosepointBuffer, BLUECHOOSEPOINT_SIZE, "%d", choosekey);
    Unicode::snprintf(stateBuffer, STATE_SIZE, "%d", static_cast<unsigned int>(r1_halt));

    CTRLBuffer[0]   = (r1_state < 2)       ? Model::kCtrlLabels[r1_state]     : Model::kCtrlLabels[0];
    CTRLBuffer[1]   = 0;
    SOURCEBuffer[0] = (r1_yaw_source < 2)  ? Model::kSourceLabels[r1_yaw_source] : Model::kSourceLabels[0];
    SOURCEBuffer[1] = 0;

    // FIXME: 第2行覆盖第1行，ACCEL始终显示为整数，后续可能换为模式，暂时不解决
    formatFixed2(ACCELBuffer, ACCEL_SIZE, r1_accel_xy);
    Unicode::snprintf(ACCELBuffer, ACCEL_SIZE, "%u", static_cast<unsigned int>(r1_accel_xy));

    bluechoosepoint.invalidate();
    state.invalidate();
    CTRL.invalidate();
    ACCEL.invalidate();
    SOURCE.invalidate();
}
