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

    int8_t choosekey;      // 当前选中的点位(1-50)
    uint8_t r1_halt;       // R1 自锁状态
    uint8_t r1_state;      // R1 控制模式: 0-手动, 1-自动
    float r1_accel_xy;     // R1 加速度
    uint8_t r1_yaw_source; // R1 坐标系: 0-自身, 1-世界坐标系
    uint8_t sub_mode;        /*!< sub_mode_t */
    uint8_t tactical_idx;    /*!< 战术点索引 */

protected:
};

#endif // REDMAPVIEW_HPP
