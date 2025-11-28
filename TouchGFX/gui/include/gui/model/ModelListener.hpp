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
    virtual void onKeyValueChanged(uint8_t /*key*/) {}
protected:
    Model* model;
};

#endif // MODELLISTENER_HPP
