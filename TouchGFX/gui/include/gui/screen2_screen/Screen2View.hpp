#ifndef SCREEN2VIEW_HPP
#define SCREEN2VIEW_HPP

#include <gui_generated/screen2_screen/Screen2ViewBase.hpp>
#include <gui/screen2_screen/Screen2Presenter.hpp>

class Screen2View : public Screen2ViewBase
{
public:
    Screen2View();
    virtual ~Screen2View() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
    virtual void handleTickEvent();
protected:
    int tickCounter;
    int digitalHours; /* 毫秒时间戳，用于基于系统毫秒计时（HAL_GetTick）实现稳定的秒钟 */ 
    int digitalMinutes;
    int digitalSeconds;
    float graphValue;
};

#endif // SCREEN2VIEW_HPP
