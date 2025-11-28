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
    void setKeyValue(uint8_t k);
protected:
    ModelListener* modelListener;
    uint8_t keyValue; // latest sampled key value
};

#endif // MODEL_HPP
