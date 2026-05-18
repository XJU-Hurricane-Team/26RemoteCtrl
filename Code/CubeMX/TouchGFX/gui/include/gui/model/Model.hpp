/**
 * @file    Model.hpp
 * @brief   应用数据模型
 */
#ifndef MODEL_HPP
#define MODEL_HPP

#include <stdint.h>

class ModelListener;

class Model {
  public:
    Model();

    void bind(ModelListener *listener) {
        modelListener = listener;
    }

    void tick();

    void setKeyValue(int8_t k);

    /* UI 共享文案表 — 跨 Screen 复用, 住在 Model 而非 Application */
    enum : uint8_t { kTacticalCount = 9 }; /* idx=0..8 */
    static const char *const kTacticalNames[kTacticalCount];
    static const char        kCtrlLabels[2];     /* {'M', 'A'} */
    static const char        kSourceLabels[2];   /* {'S', 'W'} */

  protected:
    ModelListener *modelListener;
    int8_t ctrl_keyValue;    // 面板按键值(CTRL/SET 等)
    int8_t choosepoint;      // 选中的地图点
    int8_t keyValue;         // 最新按键采样值
    int8_t voltage;          // 电池电压
    int8_t rsL_x;            // 左摇杆 X
    int8_t rsL_y;            // 左摇杆 Y
    int8_t rsR_x;            // 右摇杆 X
    int8_t rsR_y;            // 右摇杆 Y
    uint8_t irda_msgnum;     // IRDA 消息号

    int16_t r1_x_speed;      // R1 X 速度
    int16_t r1_y_speed;      // R1 Y 速度
    int16_t r1_w_speed;      // R1 角速度
    uint8_t r1_chassis_status;  // R1 底盘连接状态
    uint8_t r1_chassis_state;   // R1 底盘控制模式
    float r1_accel_xy;         // R1 加速度
    float r1_left_pos;         // R1 左臂位置
    float r1_right_pos;        // R1 右臂位置
    uint8_t r1_left_adsorbed;  // R1 左吸附状态
    uint8_t r1_right_adsorbed; // R1 右吸附状态
    uint8_t r1_send_msg;       // R1 已发送消息数
    uint8_t r1_rec_msg;        // R1 已接收消息数
    uint8_t r1_yaw_source;     // R1 坐标系: 0-自身, 1-世界坐标系

};

#endif // MODEL_HPP
