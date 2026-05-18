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

}

void redmapPresenter::deactivate()
{

}

void redmapPresenter::onCtrlKeyValueChanged(int8_t ctrl_Key, int8_t choose){
    view.choosekey = choose;
    view.handleKeyEvent(ctrl_Key);
    view.colortoggleEvent(choose);
}

void redmapPresenter::R1StateChanged(uint8_t status ,uint8_t state ,float accel ,uint8_t source){
    view.r1_halt = status;
    view.r1_accel_xy = accel;
    view.r1_state = state;
    view.r1_yaw_source = source;
}

void redmapPresenter::onMapSubModeChanged(bool in_sub, uint8_t preset_idx){
    view.in_sub = in_sub;
    view.cur_preset_idx = preset_idx;
}
