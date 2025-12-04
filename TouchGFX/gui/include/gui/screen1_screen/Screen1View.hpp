#ifndef SCREEN1VIEW_HPP
#define SCREEN1VIEW_HPP

#include <cstdint>
#include <gui_generated/screen1_screen/Screen1ViewBase.hpp>
#include <gui/screen1_screen/Screen1Presenter.hpp>

class Screen1View : public Screen1ViewBase
{
public:
    Screen1View();
    virtual ~Screen1View() {}
    virtual void setupScreen();
    virtual void handleTickEvent();
    virtual void tearDownScreen();
    virtual void InfoUpdate1();
    int8_t keyState;
    int8_t voltage;
    int8_t rsL_x;
    int8_t rsL_y;
    int8_t rsR_x;
    int8_t rsR_y;
    float graphValue;
    int tickCounter;
    
protected:
};

#endif // SCREEN1VIEW_HPP
