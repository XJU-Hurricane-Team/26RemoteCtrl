/**
 * @file    screenPresenter.cpp
 * @brief   主遥控界面Presenter实现
 */
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

void screenPresenter::onChoosepointChanged(int8_t choose){
    view.pointvalue = choose;
}

void screenPresenter::onCtrlKeyChanged(int8_t ctrl_key){
    view.handleKeyEvent(ctrl_key);
}

void screenPresenter::onVoltageChanged(double voltage){
    view.voltage = voltage;
}

void screenPresenter::onJoystickChanged(int8_t rsL_x, int8_t rsL_y, int8_t rsR_x, int8_t rsR_y){
    view.rsL_x = rsL_x;
    view.rsL_y = rsL_y;
    view.rsR_x = rsR_x;
    view.rsR_y = rsR_y;
}

void screenPresenter::onR1StateChanged(int16_t Pos_x, int16_t Pos_y, int16_t Pos_yaw, uint8_t status, uint8_t state, float accel,
    float leftPos, float rightPos, uint8_t leftAdsorbed, uint8_t rightAdsorbed, uint8_t sendMsg, uint8_t recMsg,
    uint8_t source, bool sendflag){
    view.r1_pose_x = Pos_x;
    view.r1_pose_y = Pos_y;
    view.r1_pose_yaw = Pos_yaw;
    view.r1_status = status;
    view.r1_state = state;
    view.r1_accel_xy= accel;
    view.r1_left_pos = leftPos;
    view.r1_right_pos = rightPos;
    view.r1_left_adsorbed = leftAdsorbed;
    view.r1_right_adsorbed = rightAdsorbed;
    view.r1_send_msg = sendMsg;
    view.r1_rec_msg = recMsg;
    view.r1_yaw_source = source;
    view.r1_irda_send_flag = sendflag;
}

void screenPresenter::onIrdaMsgNumChanged(uint8_t msgnum){
    view.msgnum = msgnum;
}
