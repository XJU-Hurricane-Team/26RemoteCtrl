#ifndef MODELLISTENER_HPP
#define MODELLISTENER_HPP

#include <gui/model/Model.hpp>

class ModelListener
{
public:
    ModelListener() : model(nullptr) {}
    
    virtual ~ModelListener() {}

    void bind(Model* m)
    {
        model = m;
    }
    virtual void onKeyValueChanged(int8_t) {}
    virtual void onCtrlKeyValueChanged(int8_t, int8_t) {}
    virtual void onVoltageChanged(float) {}
    virtual void onJoystickChanged(int8_t, int8_t, int8_t, int8_t) {}
protected:
    Model* model;
};

#endif // MODELLISTENER_HPP
