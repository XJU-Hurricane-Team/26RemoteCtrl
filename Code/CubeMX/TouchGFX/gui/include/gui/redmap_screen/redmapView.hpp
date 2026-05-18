/**
 * @file    redmapView.hpp
 * @brief   红方地图视图
 */
#ifndef REDMAPVIEW_HPP
#define REDMAPVIEW_HPP

#include <gui_generated/redmap_screen/redmapViewBase.hpp>
#include <gui/redmap_screen/redmapPresenter.hpp>

class redmapView : public redmapViewBase
{
public:
    redmapView();
    virtual ~redmapView() {}
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
    uint8_t cur_preset_idx;

protected:
};

#endif // REDMAPVIEW_HPP
