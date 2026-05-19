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
    int8_t keyValue; // latest sampled key value
    float  voltage;
    int8_t rsL_x;
    int8_t rsL_y;
    int8_t rsR_x;
    int8_t rsR_y;
};

#endif // MODEL_HPP
