#ifndef MODEL_HPP
#define MODEL_HPP

class ModelListener;

class Model
{
public:
    Model();

    void bind(ModelListener* listener)
    {
        modelListener = listener;
    }

    void tick();
    // Optional API: allow external code to set key value (thread-safety not handled here)
    void setKeyValue(int8_t k);
protected:
    ModelListener* modelListener;
    int8_t keyValue; // latest sampled key value
    int8_t voltage;
    int8_t rsL_x;
    int8_t rsL_y;
    int8_t rsR_x;
    int8_t rsR_y;
};

#endif // MODEL_HPP
