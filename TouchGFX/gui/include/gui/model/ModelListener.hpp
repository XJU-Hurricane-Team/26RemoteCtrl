#ifndef MODELLISTENER_HPP
#define MODELLISTENER_HPP

#include <gui/model/Model.hpp>

class ModelListener
{
public:
    ModelListener() : model(0) {}
    
    virtual ~ModelListener() {}

    void bind(Model* m)
    {
        model = m;
    }
    
    /**
     * Called by Model when the sampled key value changes.
     * Override this in Presenter to react to key updates.
     */
    virtual void onKeyValueChanged(int8_t keyValue) {};
    virtual void onVoltageChanged(int8_t voltage) {};
    virtual void onJoystickChanged(int8_t rsL_x, int8_t rsL_y, int8_t rsR_x, int8_t rsR_y) {};
protected:
    Model* model;
};

#endif // MODELLISTENER_HPP
