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
    int8_t keyValue; // latest sampled key value
    int8_t voltage;
    int8_t rsL_x;
    int8_t rsL_y;
    int8_t rsR_x;
    int8_t rsR_y;

    int16_t r1_x_speed;
    int16_t r1_y_speed;
    int16_t r1_angle;
    uint8_t r1_status;

    int16_t r2_x_speed;
    int16_t r2_y_speed;
    int16_t r2_angle;
    uint8_t r2_status;
};

#endif // MODEL_HPP
