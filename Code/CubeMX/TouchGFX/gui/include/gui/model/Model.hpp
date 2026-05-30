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

    enum : uint8_t { kPresetPointCount = 9 };
    static const char *const kPresetPointNames[kPresetPointCount];
    static const char        kCtrlLabels[2];         /* 自动/手动 {'M', 'A'} */
    static const char        kSourceLabels[2];       /* 车体坐标系 {'S', 'W'} */
    int8_t choosepoint;

  protected:
    ModelListener *modelListener;
    int8_t ctrl_keyValue;
    int8_t keyValue;
    double voltage;
    int8_t rsL_x;
    int8_t rsL_y;
    int8_t rsR_x;
    int8_t rsR_y;
    uint8_t irda_msgnum;

    int16_t r1_x_speed;
    int16_t r1_y_speed;
    int16_t r1_w_speed;
    uint8_t r1_chassis_status;
    uint8_t r1_chassis_state;
    float r1_accel_xy;
    float r1_left_pos;
    float r1_right_pos;
    uint8_t r1_left_adsorbed;
    uint8_t r1_right_adsorbed;
    uint8_t r1_send_msg;
    uint8_t r1_rec_msg;
    uint8_t r1_yaw_source;

};

#endif // MODEL_HPP
