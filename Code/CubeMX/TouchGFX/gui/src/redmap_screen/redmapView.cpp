#include <gui/redmap_screen/redmapView.hpp>
#include <touchgfx/Color.hpp>

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

redmapView::redmapView():choosekey(0),r1state(1),r1_state(0),r1_accel_xy(0.0f),r1_yaw_source(0)
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

void redmapView::colortoggleEvent(uint8_t point)
{
    static uint8_t last_point;
    switch (last_point)
    {
    case 1:
        //toggle01
        box01.setColor(touchgfx::Color::getColorFromRGB(214, 0, 0));
        box01.invalidate();
        break;
    case 2:
        //toggle02
        box02.setColor(touchgfx::Color::getColorFromRGB(252, 235, 235));
        box02.invalidate();
        break;
    case 3:
        //toggle03
        box03.setColor(touchgfx::Color::getColorFromRGB(252, 235, 235));
        box03.invalidate();
        break;
    case 4:
        //toggle04
        box04.setColor(touchgfx::Color::getColorFromRGB(252, 235, 235));
        box04.invalidate();
        break;
    case 5:
        //toggle05
        box05.setColor(touchgfx::Color::getColorFromRGB(252, 235, 235));
        box05.invalidate();
        break;
    case 6:
        //toggle11
        box11.setColor(touchgfx::Color::getColorFromRGB(252, 235, 235));
        box11.invalidate();
        break;
    case 7:
        //toggle12
        box12.setColor(touchgfx::Color::getColorFromRGB(252, 235, 235));
        box12.invalidate();
        break;
    case 8:
        //toggle13
        box13.setColor(touchgfx::Color::getColorFromRGB(252, 235, 235));
        box13.invalidate();
        break;
    case 9:
        //toggle14
        box14.setColor(touchgfx::Color::getColorFromRGB(252, 235, 235));
        box14.invalidate();
        break;
    case 10:
        //toggle15
        box15.setColor(touchgfx::Color::getColorFromRGB(252, 235, 235));
        box15.invalidate();
        break;
    case 11:
        //toggle21
        box21.setColor(touchgfx::Color::getColorFromRGB(250, 165, 165));
        box21.invalidate();
        break;
    case 12:
        //toggle22
        box22.setColor(touchgfx::Color::getColorFromRGB(250, 165, 165));
        box22.invalidate();
        break;
    case 13:
        //toggle23
        box23.setColor(touchgfx::Color::getColorFromRGB(250, 165, 165));
        box23.invalidate();
        break;
    case 14:
        //toggle24
        box24.setColor(touchgfx::Color::getColorFromRGB(250, 165, 165));
        box24.invalidate();
        break;
    case 15:
        //toggle25
        box25.setColor(touchgfx::Color::getColorFromRGB(250, 165, 165));
        box25.invalidate();
        break;
    case 16:
        //toggle31
        box31.setColor(touchgfx::Color::getColorFromRGB(250, 165, 165));
        box31.invalidate();
        break;
    case 20:
        //toggle35
        box35.setColor(touchgfx::Color::getColorFromRGB(250, 165, 165));
        box35.invalidate();
        break;
    case 21:
        //toggle41
        box41.setColor(touchgfx::Color::getColorFromRGB(250, 165, 165));
        box41.invalidate();
        break;
    case 25:
        //toggle45
        box45.setColor(touchgfx::Color::getColorFromRGB(250, 165, 165));
        box45.invalidate();
        break;
    case 26:
        //toggle51
        box51.setColor(touchgfx::Color::getColorFromRGB(250, 165, 165));
        box51.invalidate();
        break;
    case 30:
        //toggle55
        box55.setColor(touchgfx::Color::getColorFromRGB(250, 165, 165));
        box55.invalidate();
        break;
    case 31:
        //toggle61
        box61.setColor(touchgfx::Color::getColorFromRGB(250, 165, 165));
        box61.invalidate();
        break;
    case 35:
        //toggle65
        box65.setColor(touchgfx::Color::getColorFromRGB(250, 165, 165));
        box65.invalidate();
        break;
    case 36:
        //toggle71
        box71.setColor(touchgfx::Color::getColorFromRGB(250, 165, 165));
        box71.invalidate();
        break;
    case 37:
        //toggle72
        box72.setColor(touchgfx::Color::getColorFromRGB(250, 165, 165));
        box72.invalidate();
        break;
    case 38:
        //toggle73
        box73.setColor(touchgfx::Color::getColorFromRGB(250, 165, 165));
        box73.invalidate();
        break;
    case 39:
        //toggle74
        box74.setColor(touchgfx::Color::getColorFromRGB(250, 165, 165));
        box74.invalidate();
        break;
    case 40:
        //toggle75
        box75.setColor(touchgfx::Color::getColorFromRGB(250, 165, 165));
        box75.invalidate();
        break;
    case 41:
        //toggle81
        box81.setColor(touchgfx::Color::getColorFromRGB(163, 162, 162));
        box81.invalidate();
        break;
    case 42:
        //toggle82
        box82.setColor(touchgfx::Color::getColorFromRGB(255, 140, 140));
        box82.invalidate();
        break;
    case 43:
        //toggle83
        box83.setColor(touchgfx::Color::getColorFromRGB(255, 140, 140));
        box83.invalidate();
        break;
    case 44:
        //toggle84
        box84.setColor(touchgfx::Color::getColorFromRGB(255, 140, 140));
        box84.invalidate();
        break;
    case 45:
        //toggle85
        box85.setColor(touchgfx::Color::getColorFromRGB(255, 140, 140));
        box85.invalidate();
        break;
    case 46:
        //toggle91
        box91.setColor(touchgfx::Color::getColorFromRGB(214, 0, 0));
        box91.invalidate();
        break;
    case 47:
        //toggle92
        box92.setColor(touchgfx::Color::getColorFromRGB(255, 140, 140));
        box92.invalidate();
        break;
    case 48:
        //toggle93
        box93.setColor(touchgfx::Color::getColorFromRGB(255, 140, 140));
        box93.invalidate();
        break;
    case 49:
        //toggle94
        box94.setColor(touchgfx::Color::getColorFromRGB(255, 140, 140));
        box94.invalidate();
        break;
    case 50:
        //toggle95
        box95.setColor(touchgfx::Color::getColorFromRGB(255, 140, 140));
        box95.invalidate();
        break;
    default:
        break;
    }

    switch (point)
    {
    case 1:
        //toggle01
        box01.setColor(touchgfx::Color::getColorFromRGB(187, 0, 255));
        box01.invalidate();
        break;
    case 2:
        //toggle02
        box02.setColor(touchgfx::Color::getColorFromRGB(187, 0, 255));
        box02.invalidate();
        break;
    case 3:
        //toggle03
        box03.setColor(touchgfx::Color::getColorFromRGB(187, 0, 255));
        box03.invalidate();
        break;
    case 4:
        //toggle04
        box04.setColor(touchgfx::Color::getColorFromRGB(187, 0, 255));
        box04.invalidate();
        break;
    case 5:
        //toggle05
        box05.setColor(touchgfx::Color::getColorFromRGB(187, 0, 255));
        box05.invalidate();
        break;
    case 6:
        //toggle11
        box11.setColor(touchgfx::Color::getColorFromRGB(187, 0, 255));
        box11.invalidate();
        break;
    case 7:
        //toggle12
        box12.setColor(touchgfx::Color::getColorFromRGB(187, 0, 255));
        box12.invalidate();
        break;
    case 8:
        //toggle13
        box13.setColor(touchgfx::Color::getColorFromRGB(187, 0, 255));
        box13.invalidate();
        break;
    case 9:
        //toggle14
        box14.setColor(touchgfx::Color::getColorFromRGB(187, 0, 255));
        box14.invalidate();
        break;
    case 10:
        //toggle15
        box15.setColor(touchgfx::Color::getColorFromRGB(187, 0, 255));
        box15.invalidate();
        break;
    case 11:
        //toggle21
        box21.setColor(touchgfx::Color::getColorFromRGB(187, 0, 255));
        box21.invalidate();
        break;
    case 12:
        //toggle22
        box22.setColor(touchgfx::Color::getColorFromRGB(187, 0, 255));
        box22.invalidate();
        break;
    case 13:
        //toggle23
        box23.setColor(touchgfx::Color::getColorFromRGB(187, 0, 255));
        box23.invalidate();
        break;
    case 14:
        //toggle24
        box24.setColor(touchgfx::Color::getColorFromRGB(187, 0, 255));
        box24.invalidate();
        break;
    case 15:
        //toggle25
        box25.setColor(touchgfx::Color::getColorFromRGB(187, 0, 255));
        box25.invalidate();
        break;
    case 16:
        //toggle31
        box31.setColor(touchgfx::Color::getColorFromRGB(187, 0, 255));
        box31.invalidate();
        break;
    case 20:
        //toggle35
        box35.setColor(touchgfx::Color::getColorFromRGB(187, 0, 255));
        box35.invalidate();
        break;
    case 21:
        //toggle41
        box41.setColor(touchgfx::Color::getColorFromRGB(187, 0, 255));
        box41.invalidate();
        break;
    case 25:
        //toggle45
        box45.setColor(touchgfx::Color::getColorFromRGB(187, 0, 255));
        box45.invalidate();
        break;
    case 26:
        //toggle51
        box51.setColor(touchgfx::Color::getColorFromRGB(187, 0, 255));
        box51.invalidate();
        break;
    case 30:
        //toggle55
        box55.setColor(touchgfx::Color::getColorFromRGB(187, 0, 255));
        box55.invalidate();
        break;
    case 31:
        //toggle61
        box61.setColor(touchgfx::Color::getColorFromRGB(187, 0, 255));
        box61.invalidate();
        break;
    case 35:
        //toggle65
        box65.setColor(touchgfx::Color::getColorFromRGB(187, 0, 255));
        box65.invalidate();
        break;
    case 36:
        //toggle71
        box71.setColor(touchgfx::Color::getColorFromRGB(187, 0, 255));
        box71.invalidate();
        break;
    case 37:
        //toggle72
        box72.setColor(touchgfx::Color::getColorFromRGB(187, 0, 255));
        box72.invalidate();
        break;
    case 38:
        //toggle73
        box73.setColor(touchgfx::Color::getColorFromRGB(187, 0, 255));
        box73.invalidate();
        break;
    case 39:
        //toggle74
        box74.setColor(touchgfx::Color::getColorFromRGB(187, 0, 255));
        box74.invalidate();
        break;
    case 40:
        //toggle75
        box75.setColor(touchgfx::Color::getColorFromRGB(187, 0, 255));
        box75.invalidate();
        break;
    case 41:
        //toggle81
        box81.setColor(touchgfx::Color::getColorFromRGB(187, 0, 255));
        box81.invalidate();
        break;
    case 42:
        //toggle82
        box82.setColor(touchgfx::Color::getColorFromRGB(187, 0, 255));
        box82.invalidate();
        break;
    case 43:
        //toggle83
        box83.setColor(touchgfx::Color::getColorFromRGB(187, 0, 255));
        box83.invalidate();
        break;
    case 44:
        //toggle84
        box84.setColor(touchgfx::Color::getColorFromRGB(187, 0, 255));
        box84.invalidate();
        break;
    case 45:
        //toggle85
        box85.setColor(touchgfx::Color::getColorFromRGB(187, 0, 255));
        box85.invalidate();
        break;
    case 46:
        //toggle91
        box91.setColor(touchgfx::Color::getColorFromRGB(187, 0, 255));
        box91.invalidate();
        break;
    case 47:
        //toggle92
        box92.setColor(touchgfx::Color::getColorFromRGB(187, 0, 255));
        box92.invalidate();
        break;
    case 48:
        //toggle93
        box93.setColor(touchgfx::Color::getColorFromRGB(187, 0, 255));
        box93.invalidate();
        break;
    case 49:
        //toggle94
        box94.setColor(touchgfx::Color::getColorFromRGB(187, 0, 255));
        box94.invalidate();
        break;
    case 50:
        //toggle95
        box95.setColor(touchgfx::Color::getColorFromRGB(187, 0, 255));
        box95.invalidate();
        break;
    default:
        break;
    }
    last_point = point;
}


void redmapView::update2(){
    Unicode::snprintf(redchoosepointBuffer, REDCHOOSEPOINT_SIZE, "%d", choosekey);
    Unicode::snprintf(stateBuffer, STATE_SIZE, "%d", static_cast<unsigned int>(r1state));
    Unicode::snprintf(CTRLBuffer, CTRL_SIZE, "%d", r1_state);
    formatFixed2(ACCELBuffer, ACCEL_SIZE, r1_accel_xy);
    Unicode::snprintf(ACCELBuffer, ACCEL_SIZE, "%u", static_cast<unsigned int>(r1_accel_xy));
    Unicode::snprintf(SOURCEBuffer, SOURCE_SIZE, "%d", r1_yaw_source);
    redchoosepoint.invalidate();
    state.invalidate();
    CTRL.invalidate();
    ACCEL.invalidate();
    SOURCE.invalidate();
}
