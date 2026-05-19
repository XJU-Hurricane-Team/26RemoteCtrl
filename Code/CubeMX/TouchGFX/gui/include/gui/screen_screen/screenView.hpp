#ifndef SCREENVIEW_HPP
#define SCREENVIEW_HPP

#include <gui_generated/screen_screen/screenViewBase.hpp>
#include <gui/screen_screen/screenPresenter.hpp>

class screenView : public screenViewBase {
  public:
    screenView();
    virtual ~screenView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
    virtual void handleTickEvent();
    virtual void handleKeyEvent(uint8_t key);
    virtual void InfoUpdate1();
    int8_t keyState;
    float  voltage;
    int8_t pointvalue;
    int8_t rsL_x;
    int8_t rsL_y;
    int8_t rsR_x;
    int8_t rsR_y;

    float graphValue;
    int tickCounter;

  protected:
    int digitalHours; /* 毫秒时间戳，用于基于系统毫秒计时（HAL_GetTick）实现稳定的秒钟 */
    int digitalMinutes;
    int digitalSeconds;
};

#endif // SCREENVIEW_HPP
