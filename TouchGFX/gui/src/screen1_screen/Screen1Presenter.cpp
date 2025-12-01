#include <gui/screen1_screen/Screen1View.hpp>
#include <gui/screen1_screen/Screen1Presenter.hpp>

Screen1Presenter::Screen1Presenter(Screen1View& v)
    : view(v)
{

}

void Screen1Presenter::activate()
{

}

void Screen1Presenter::deactivate()
{

}

void Screen1Presenter::onKeyValueChanged(int8_t key){
    view.keyState = key;
}

void Screen1Presenter::onVoltageChanged(int8_t voltage){
    view.voltage = voltage;
}

void Screen1Presenter::onJoystickChanged(int8_t rsL_x, int8_t rsL_y, int8_t rsR_x, int8_t rsR_y){
    view.rsL_x = rsL_x;
    view.rsL_y = rsL_y;
    view.rsR_x = rsR_x;
    view.rsR_y = rsR_y;
}