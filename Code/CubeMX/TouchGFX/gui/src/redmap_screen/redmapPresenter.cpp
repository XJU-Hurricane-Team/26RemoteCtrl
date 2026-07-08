/**
 * @file    redmapPresenter.cpp
 * @brief   红方地图Presenter实现
 */
#include <gui/redmap_screen/redmapView.hpp>
#include <gui/redmap_screen/redmapPresenter.hpp>

redmapPresenter::redmapPresenter(redmapView& v)
    : view(v)
{

}

void redmapPresenter::activate()
{
    view.choosekey = model->choosepoint;
    view.colortoggleEvent(view.choosekey);
}

void redmapPresenter::deactivate()
{

}

void redmapPresenter::onCtrlKeyChanged(int8_t ctrl_Key){
    view.handleKeyEvent(ctrl_Key);
}

void redmapPresenter::onChoosepointChanged(int8_t choose){
    view.choosekey = choose;
    view.colortoggleEvent(choose);
}

void redmapPresenter::onR1StateChanged(
    int16_t /*Pos_x*/, int16_t /*Pos_y*/, int16_t /*Pos_yaw*/,
    uint8_t status, uint8_t state, float accel,
    float /*leftPos*/, float /*rightPos*/,
    uint8_t /*leftAdsorbed*/, uint8_t /*rightAdsorbed*/,
    uint8_t /*sendMsg*/, uint8_t /*recMsg*/,
    uint8_t source, bool sendflag) {
    view.r1_halt = status;
    view.r1_accel_xy = accel;
    view.r1_state = state;
    view.r1_yaw_source = source;
}

void redmapPresenter::onMapSubModeChanged(bool in_sub, uint8_t preset_idx){
    view.in_sub = in_sub;
    view.cur_preset_idx = preset_idx;
}
