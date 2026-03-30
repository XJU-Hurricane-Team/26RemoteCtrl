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

void screenPresenter::onR1StateChanged(int16_t xSpeed, int16_t ySpeed, int16_t angle, uint8_t status){
    view.r1_x_speed = xSpeed;
    view.r1_y_speed = ySpeed;
    view.r1_angle = angle;
    view.r1_status = status;
}

void screenPresenter::onR2StateChanged(int16_t xSpeed, int16_t ySpeed, int16_t angle, uint8_t status){
    view.r2_x_speed = xSpeed;
    view.r2_y_speed = ySpeed;
    view.r2_angle = angle;
    view.r2_status = status;
}