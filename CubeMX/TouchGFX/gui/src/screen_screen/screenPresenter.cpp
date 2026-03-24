#include <gui/screen_screen/screenView.hpp>
#include <gui/screen_screen/screenPresenter.hpp>

screenPresenter::screenPresenter(screenView& v)
    : view(v)
{

}

void screenPresenter::activate()
{

}

void screenPresenter::deactivate()
{

}

void screenPresenter::onKeyValueChanged(int8_t key){
    view.keyState = key;
}

void screenPresenter::onVoltageChanged(int8_t voltage){
    view.voltage = voltage;
}

void screenPresenter::onJoystickChanged(int8_t rsL_x, int8_t rsL_y, int8_t rsR_x, int8_t rsR_y){
    view.rsL_x = rsL_x;
    view.rsL_y = rsL_y;
    view.rsR_x = rsR_x;
    view.rsR_y = rsR_y;
}