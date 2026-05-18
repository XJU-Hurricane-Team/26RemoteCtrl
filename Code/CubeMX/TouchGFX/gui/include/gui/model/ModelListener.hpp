/**
 * @file    ModelListener.hpp
 * @brief   Model-View-Presenter 监听接口
 */
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
    virtual void onIrdaMsgNumChanged(uint8_t){}
    virtual void onCtrlKeyChanged(int8_t) {}
    virtual void onChoosepointChanged(int8_t) {}
    virtual void onVoltageChanged(int8_t) {}
    virtual void onJoystickChanged(int8_t, int8_t, int8_t, int8_t) {}
    virtual void onR1StateChanged(int16_t, int16_t, int16_t,
                                  uint8_t, uint8_t, float,
                                  float, float,
                                  uint8_t, uint8_t,
                                  uint8_t, uint8_t,
                                  uint8_t /*yaw_source*/) {}
    virtual void onMapSubModeChanged(bool /*in_sub*/, uint8_t /*preset_idx*/) {}
protected:
    Model* model;
};

#endif // MODELLISTENER_HPP
