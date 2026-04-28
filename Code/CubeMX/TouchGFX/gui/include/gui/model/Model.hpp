#ifndef MODEL_HPP
#define MODEL_HPP

class ModelListener;

class Model {
  public:
    Model();

    void bind(ModelListener *listener) {
        modelListener = listener;
    }

    void tick();

    void setKeyValue(int8_t k);

  protected:
    ModelListener *modelListener;
    int8_t ctrl_keyValue;
    int8_t choosepoint;
    int8_t sendpoint;
    int8_t modevalue;
    int8_t keyValue; // latest sampled key value
    int8_t voltage;
    int8_t rsL_x;
    int8_t rsL_y;
    int8_t rsR_x;
    int8_t rsR_y;

    int16_t r1_x_speed;
    int16_t r1_y_speed;
    int16_t r1_w_speed;
    uint8_t r1_chassis_status;
    float r1_left_pos;
    float r1_right_pos;
    uint8_t r1_left_adsorbed;
    uint8_t r1_right_adsorbed;
    uint8_t r1_send_msg;

    int16_t r2_x_speed;
    int16_t r2_y_speed;
    int16_t r2_angle;
    uint8_t r2_status;
    uint8_t r2_send_msg;
};

#endif // MODEL_HPP
