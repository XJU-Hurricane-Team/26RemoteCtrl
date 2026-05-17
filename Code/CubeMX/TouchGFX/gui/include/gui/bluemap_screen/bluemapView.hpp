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
    int8_t choosekey;      // 当前选中的点位(1-50)
    uint8_t r1_halt;       // R1 自锁状态
    uint8_t r1_state;      // R1 控制模式: 0-手动, 1-自动
    float r1_accel_xy;     // R1 加速度
    uint8_t r1_yaw_source; // R1 坐标系: 0-自身, 1-世界坐标系
protected:
};

#endif // BLUEMAPVIEW_HPP
