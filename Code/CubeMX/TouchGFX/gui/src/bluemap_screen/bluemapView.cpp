#include <gui/bluemap_screen/bluemapView.hpp>
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

bluemapView::bluemapView():choosekey(0),r1state(1),r1_state(0),r1_accel_xy(0.0f),r1_yaw_source(0)
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
        /* 0  无效 */ {nullptr,  0},
        /* 1  深蓝 */ {&box95,   kBlueDeep},
        /* 2  浅蓝 */ {&box94,   kBlueLight},
        /* 3  浅蓝 */ {&box93,   kBlueLight},
        /* 4  浅蓝 */ {&box92,   kBlueLight},
        /* 5  浅蓝 */ {&box91,   kBlueLight},
        /* 6  浅蓝 */ {&box85,   kBlueLight},
        /* 7  浅蓝 */ {&box84,   kBlueLight},
        /* 8  浅蓝 */ {&box83,   kBlueLight},
        /* 9  浅蓝 */ {&box82,   kBlueLight},
        /* 10 浅蓝 */ {&box81,   kBlueLight},
        /* 11 青色 */ {&box75,   kCyan},
        /* 12 青色 */ {&box74,   kCyan},
        /* 13 青色 */ {&box73,   kCyan},
        /* 14 青色 */ {&box72,   kCyan},
        /* 15 青色 */ {&box71,   kCyan},
        /* 16 青色 */ {&box65,   kCyan},
        /* 17 无效 */ {nullptr,  0},
        /* 18 无效 */ {nullptr,  0},
        /* 19 无效 */ {nullptr,  0},
        /* 20 青色 */ {&box61,   kCyan},
        /* 21 青色 */ {&box55,   kCyan},
        /* 22 无效 */ {nullptr,  0},
        /* 23 无效 */ {nullptr,  0},
        /* 24 无效 */ {nullptr,  0},
        /* 25 青色 */ {&box51,   kCyan},
        /* 26 青色 */ {&box45,   kCyan},
        /* 27 无效 */ {nullptr,  0},
        /* 28 无效 */ {nullptr,  0},
        /* 29 无效 */ {nullptr,  0},
        /* 30 青色 */ {&box41,   kCyan},
        /* 31 青色 */ {&box35,   kCyan},
        /* 32 无效 */ {nullptr,  0},
        /* 33 无效 */ {nullptr,  0},
        /* 34 无效 */ {nullptr,  0},
        /* 35 青色 */ {&box31,   kCyan},
        /* 36 青色 */ {&box25,   kCyan},
        /* 37 青色 */ {&box24,   kCyan},
        /* 38 青色 */ {&box23,   kCyan},
        /* 39 青色 */ {&box22,   kCyan},
        /* 40 青色 */ {&box21,   kCyan},
        /* 41 浅蓝 */ {&box15,   kBlueLight},
        /* 42 天蓝 */ {&box14,   kBlueSky},
        /* 43 天蓝 */ {&box13,   kBlueSky},
        /* 44 天蓝 */ {&box12,   kBlueSky},
        /* 45 天蓝 */ {&box11,   kBlueSky},
        /* 46 天蓝 */ {&box05,   kBlueSky},
        /* 47 天蓝 */ {&box04,   kBlueSky},
        /* 48 天蓝 */ {&box03,   kBlueSky},
        /* 49 天蓝 */ {&box02,   kBlueSky},
        /* 50 深蓝 */ {&box01,   kBlueDeep},
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
    Unicode::snprintf(bluechoosepointBuffer, BLUECHOOSEPOINT_SIZE, "%d", choosekey);
    Unicode::snprintf(stateBuffer, STATE_SIZE, "%d", static_cast<unsigned int>(r1state));

    /* 使用数组查找表替代switch-case，索引对应r1_state值，越界时回退到默认值"M" */
    const char *kCtrlLabels[] = {"M", "A"};
    const char *ctrl = (r1_state < 2) ? kCtrlLabels[r1_state] : "M";
    CTRLBuffer[0] = ctrl[0];
    CTRLBuffer[1] = 0;

    /* 索引对应r1_yaw_source值，越界时回退到默认值"S" */
    const char *kSourceLabels[] = {"S", "W"};
    const char *source = (r1_yaw_source < 2) ? kSourceLabels[r1_yaw_source] : "S";
    SOURCEBuffer[0] = source[0];
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
