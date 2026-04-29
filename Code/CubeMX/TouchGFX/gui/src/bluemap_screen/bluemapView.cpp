#include <gui/bluemap_screen/bluemapView.hpp>
#include <touchgfx/Color.hpp>

bluemapView::bluemapView():choosekey(0),point(0)
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

void bluemapView::colortoggleEvent(uint8_t point){
    static uint8_t last_point;
    switch (last_point)
    {
    case 34:
        //toggle01
        box01.setColor(touchgfx::Color::getColorFromRGB(0, 192, 250));
        box01.invalidate();
        break;
    case 35:
        //toggle02
        box02.setColor(touchgfx::Color::getColorFromRGB(0, 192, 250));
        box02.invalidate();
        break;
    case 36:
        //toggle03
        box03.setColor(touchgfx::Color::getColorFromRGB(0, 192, 250));
        box03.invalidate();
        break;
    case 37:
        //toggle04
        box04.setColor(touchgfx::Color::getColorFromRGB(0, 192, 250));
        box04.invalidate();
        break;
    case 38:
        //toggle05
        box05.setColor(touchgfx::Color::getColorFromRGB(0, 192, 250));
        box05.invalidate();
        break;
    case 29:
        //toggle11
        box11.setColor(touchgfx::Color::getColorFromRGB(0, 192, 250));
        box11.invalidate();
        break;
    case 30:
        //toggle12
        box12.setColor(touchgfx::Color::getColorFromRGB(0, 192, 250));
        box12.invalidate();
        break;
    case 31:
        //toggle13
        box13.setColor(touchgfx::Color::getColorFromRGB(0, 192, 250));
        box13.invalidate();
        break;
    case 32:
        //toggle14
        box14.setColor(touchgfx::Color::getColorFromRGB(0, 192, 250));
        box14.invalidate();
        break;
    case 33:
        //toggle15
        box15.setColor(touchgfx::Color::getColorFromRGB(163, 162, 162));
        box15.invalidate();
        break;
    case 24:
        //toggle21
        box21.setColor(touchgfx::Color::getColorFromRGB(0, 238, 255));
        box21.invalidate();
        break;
    case 25:
        //toggle22
        box22.setColor(touchgfx::Color::getColorFromRGB(0, 238, 255));
        box22.invalidate();
        break;
    case 26:
        //toggle23
        box23.setColor(touchgfx::Color::getColorFromRGB(0, 238, 255));
        box23.invalidate();
        break;
    case 27:
        //toggle24
        box24.setColor(touchgfx::Color::getColorFromRGB(0, 238, 255));
        box24.invalidate();
        break;
    case 28:
        //toggle25
        box25.setColor(touchgfx::Color::getColorFromRGB(0, 238, 255));
        box25.invalidate();
        break;
    case 22:
        //toggle31
        box31.setColor(touchgfx::Color::getColorFromRGB(0, 238, 255));
        box31.invalidate();
        break;
    case 23:
        //toggle35
        box35.setColor(touchgfx::Color::getColorFromRGB(0, 238, 255));
        box35.invalidate();
        break;
    case 20:
        //toggle41
        box41.setColor(touchgfx::Color::getColorFromRGB(0, 238, 255));
        box41.invalidate();
        break;
    case 21:
        //toggle45
        box45.setColor(touchgfx::Color::getColorFromRGB(0, 238, 255));
        box45.invalidate();
        break;
    case 18:
        //toggle51
        box51.setColor(touchgfx::Color::getColorFromRGB(0, 238, 255));
        box51.invalidate();
        break;
    case 19:
        //toggle55
        box55.setColor(touchgfx::Color::getColorFromRGB(0, 238, 255));
        box55.invalidate();
        break;
    case 16:
        //toggle61
        box61.setColor(touchgfx::Color::getColorFromRGB(0, 238, 255));
        box61.invalidate();
        break;
    case 17:
        //toggle65
        box65.setColor(touchgfx::Color::getColorFromRGB(0, 238, 255));
        box65.invalidate();
        break;
    case 11:
        //toggle71
        box71.setColor(touchgfx::Color::getColorFromRGB(0, 238, 255));
        box71.invalidate();
        break;
    case 12:
        //toggle72
        box72.setColor(touchgfx::Color::getColorFromRGB(0, 238, 255));
        box72.invalidate();
        break;
    case 13:
        //toggle73
        box73.setColor(touchgfx::Color::getColorFromRGB(0, 238, 255));
        box73.invalidate();
        break;
    case 14:
        //toggle74
        box74.setColor(touchgfx::Color::getColorFromRGB(0, 238, 255));
        box74.invalidate();
        break;
    case 15:
        //toggle75
        box75.setColor(touchgfx::Color::getColorFromRGB(0, 238, 255));
        box75.invalidate();
        break;
    case 6:
        //toggle81
        box81.setColor(touchgfx::Color::getColorFromRGB(212, 252, 255));
        box81.invalidate();
        break;
    case 7:
        //toggle82
        box82.setColor(touchgfx::Color::getColorFromRGB(212, 252, 255));
        box82.invalidate();
        break;
    case 8:
        //toggle83
        box83.setColor(touchgfx::Color::getColorFromRGB(212, 252, 255));
        box83.invalidate();
        break;
    case 9:
        //toggle84
        box84.setColor(touchgfx::Color::getColorFromRGB(212, 252, 255));
        box84.invalidate();
        break;
    case 10:
        //toggle85
        box85.setColor(touchgfx::Color::getColorFromRGB(212, 252, 255));
        box85.invalidate();
        break;
    case 1:
        //toggle91
        box91.setColor(touchgfx::Color::getColorFromRGB(212, 252, 255));
        box91.invalidate();
        break;
    case 2:
        //toggle92
        box92.setColor(touchgfx::Color::getColorFromRGB(212, 252, 255));
        box92.invalidate();
        break;
    case 3:
        //toggle93
        box93.setColor(touchgfx::Color::getColorFromRGB(212, 252, 255));
        box93.invalidate();
        break;
    case 4:
        //toggle94
        box94.setColor(touchgfx::Color::getColorFromRGB(212, 252, 255));
        box94.invalidate();
        break;
    case 5:
        //toggle95
        box95.setColor(touchgfx::Color::getColorFromRGB(212, 252, 255));
        box95.invalidate();
        break;
    default:
        break;
    }

    switch (point)
    {
    case 34:
        //toggle01
        box01.setColor(touchgfx::Color::getColorFromRGB(187, 0, 255));
        box01.invalidate();
        break;
    case 35:
        //toggle02
        box02.setColor(touchgfx::Color::getColorFromRGB(187, 0, 255));
        box02.invalidate();
        break;
    case 36:
        //toggle03
        box03.setColor(touchgfx::Color::getColorFromRGB(187, 0, 255));
        box03.invalidate();
        break;
    case 37:
        //toggle04
        box04.setColor(touchgfx::Color::getColorFromRGB(187, 0, 255));
        box04.invalidate();
        break;
    case 38:
        //toggle05
        box05.setColor(touchgfx::Color::getColorFromRGB(187, 0, 255));
        box05.invalidate();
        break;
    case 29:
        //toggle11
        box11.setColor(touchgfx::Color::getColorFromRGB(187, 0, 255));
        box11.invalidate();
        break;
    case 30:
        //toggle12
        box12.setColor(touchgfx::Color::getColorFromRGB(187, 0, 255));
        box12.invalidate();
        break;
    case 31:
        //toggle13
        box13.setColor(touchgfx::Color::getColorFromRGB(187, 0, 255));
        box13.invalidate();
        break;
    case 32:
        //toggle14
        box14.setColor(touchgfx::Color::getColorFromRGB(187, 0, 255));
        box14.invalidate();
        break;
    case 33:
        //toggle15
        box15.setColor(touchgfx::Color::getColorFromRGB(187, 0, 255));
        box15.invalidate();
        break;
    case 24:
        //toggle21
        box21.setColor(touchgfx::Color::getColorFromRGB(187, 0, 255));
        box21.invalidate();
        break;
    case 25:
        //toggle22
        box22.setColor(touchgfx::Color::getColorFromRGB(187, 0, 255));
        box22.invalidate();
        break;
    case 26:
        //toggle23
        box23.setColor(touchgfx::Color::getColorFromRGB(187, 0, 255));
        box23.invalidate();
        break;
    case 27:
        //toggle24
        box24.setColor(touchgfx::Color::getColorFromRGB(187, 0, 255));
        box24.invalidate();
        break;
    case 28:
        //toggle25
        box25.setColor(touchgfx::Color::getColorFromRGB(187, 0, 255));
        box25.invalidate();
        break;
    case 22:
        //toggle31
        box31.setColor(touchgfx::Color::getColorFromRGB(187, 0, 255));
        box31.invalidate();
        break;
    case 23:
        //toggle35
        box35.setColor(touchgfx::Color::getColorFromRGB(187, 0, 255));
        box35.invalidate();
        break;
    case 20:
        //toggle41
        box41.setColor(touchgfx::Color::getColorFromRGB(187, 0, 255));
        box41.invalidate();
        break;
    case 21:
        //toggle45
        box45.setColor(touchgfx::Color::getColorFromRGB(187, 0, 255));
        box45.invalidate();
        break;
    case 18:
        //toggle51
        box51.setColor(touchgfx::Color::getColorFromRGB(187, 0, 255));
        box51.invalidate();
        break;
    case 19:
        //toggle55
        box55.setColor(touchgfx::Color::getColorFromRGB(187, 0, 255));
        box55.invalidate();
        break;
    case 16:
        //toggle61
        box61.setColor(touchgfx::Color::getColorFromRGB(187, 0, 255));
        box61.invalidate();
        break;
    case 17:
        //toggle65
        box65.setColor(touchgfx::Color::getColorFromRGB(187, 0, 255));
        box65.invalidate();
        break;
    case 11:
        //toggle71
        box71.setColor(touchgfx::Color::getColorFromRGB(187, 0, 255));
        box71.invalidate();
        break;
    case 12:
        //toggle72
        box72.setColor(touchgfx::Color::getColorFromRGB(187, 0, 255));
        box72.invalidate();
        break;
    case 13:
        //toggle73
        box73.setColor(touchgfx::Color::getColorFromRGB(187, 0, 255));
        box73.invalidate();
        break;
    case 14:
        //toggle74
        box74.setColor(touchgfx::Color::getColorFromRGB(187, 0, 255));
        box74.invalidate();
        break;
    case 15:
        //toggle75
        box75.setColor(touchgfx::Color::getColorFromRGB(187, 0, 255));
        box75.invalidate();
        break;
    case 6:
        //toggle81
        box81.setColor(touchgfx::Color::getColorFromRGB(187, 0, 255));
        box81.invalidate();
        break;
    case 7:
        //toggle82
        box82.setColor(touchgfx::Color::getColorFromRGB(187, 0, 255));
        box82.invalidate();
        break;
    case 8:
        //toggle83
        box83.setColor(touchgfx::Color::getColorFromRGB(187, 0, 255));
        box83.invalidate();
        break;
    case 9:
        //toggle84
        box84.setColor(touchgfx::Color::getColorFromRGB(187, 0, 255));
        box84.invalidate();
        break;
    case 10:
        //toggle85
        box85.setColor(touchgfx::Color::getColorFromRGB(187, 0, 255));
        box85.invalidate();
        break;
    case 1:
        //toggle91
        box91.setColor(touchgfx::Color::getColorFromRGB(187, 0, 255));
        box91.invalidate();
        break;
    case 2:
        //toggle92
        box92.setColor(touchgfx::Color::getColorFromRGB(187, 0, 255));
        box92.invalidate();
        break;
    case 3:
        //toggle93
        box93.setColor(touchgfx::Color::getColorFromRGB(187, 0, 255));
        box93.invalidate();
        break;
    case 4:
        //toggle94
        box94.setColor(touchgfx::Color::getColorFromRGB(187, 0, 255));
        box94.invalidate();
        break;
    case 5:
        //toggle95
        box95.setColor(touchgfx::Color::getColorFromRGB(187, 0, 255));
        box95.invalidate();
        break;
    default:
        break;
    }
    last_point = point;
}

void bluemapView::update2(){
    Unicode::snprintf(bluechoosepointBuffer, BLUECHOOSEPOINT_SIZE, "%d", choosekey);
    Unicode::snprintf(bluesendpointBuffer, BLUESENDPOINT_SIZE, "%d", point);
    bluechoosepoint.invalidate();
    bluesendpoint.invalidate();
}
