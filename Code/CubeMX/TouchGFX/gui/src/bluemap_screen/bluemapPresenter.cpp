/**
 * @file    bluemapPresenter.cpp
 * @brief   蓝方地图Presenter实现
 */
#include <gui/bluemap_screen/bluemapView.hpp>
#include <gui/bluemap_screen/bluemapPresenter.hpp>

bluemapPresenter::bluemapPresenter(bluemapView& v)
    : view(v)
{

}

void bluemapPresenter::activate()
{

}

void bluemapPresenter::deactivate()
{

}

void bluemapPresenter::onCtrlKeyValueChanged(int8_t ctrl_Key, int8_t choose){
    view.choosekey = choose;
    view.handleKeyEvent(ctrl_Key);
    view.colortoggleEvent(choose);
}

void bluemapPresenter::R1StateChanged(uint8_t status ,uint8_t state ,float accel,uint8_t source){
    view.r1_halt = status;
    view.r1_accel_xy = accel;
    view.r1_state = state;
    view.r1_yaw_source = source;
}

void bluemapPresenter::onMapSubModeChanged(bool in_sub, uint8_t tactical_idx){
    view.in_sub = in_sub;
    view.cur_tactical_idx = tactical_idx;
}
