/**
 * @file    bluemapView.hpp
 * @brief   蓝方地图视图
 */
#ifndef BLUEMAPVIEW_HPP
#define BLUEMAPVIEW_HPP

#include <gui_generated/bluemap_screen/bluemapViewBase.hpp>
#include <gui/bluemap_screen/bluemapPresenter.hpp>

class bluemapView : public bluemapViewBase
{
public:
    bluemapView();
    virtual ~bluemapView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
    virtual void update2();
    virtual void handleKeyEvent(uint8_t key);
    virtual void colortoggleEvent(uint8_t point);
    int8_t choosekey;
    uint8_t r1_halt;
    uint8_t r1_state;
    float r1_accel_xy;
    uint8_t r1_yaw_source;
    bool in_sub;
    bool last_in_sub;
    uint8_t cur_preset_idx;
protected:
};

#endif // BLUEMAPVIEW_HPP
