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
    uint8_t KeyState;
    
protected:
    int myCounter;
};

#endif // SCREEN1VIEW_HPP
