#include "includes.h"
#include <gui/model/Model.hpp>
#include <gui/model/ModelListener.hpp>

Model::Model()
    : modelListener(nullptr), ctrl_keyValue(0), choosepoint(0),
      keyValue(0), voltage(0), rsL_x(0), rsL_y(0), rsR_x(0), rsR_y(0){}

void Model::tick() {
    /* UI在这里进行数据采集，为了不阻塞UI渲染通过消息队列进行通信 */
    if ((ui_msg_queue != NULL) && (modelListener != nullptr)) {
        ui_msg_t msg = {};
        bool remote_changed = false;

        while (xQueueReceive(ui_msg_queue, &msg, 0U) == pdPASS) {
        if(msg.type == UI_REMOTE_CTRL){
            const remote_ctrl_msg_t *ctrl_msg = &msg.payload.remote_ctrl;
            voltage = ctrl_msg->voltage;
            rsR_x = ctrl_msg->data.rs[2];
            rsR_y = ctrl_msg->data.rs[3];
            rsL_x = ctrl_msg->data.rs[0];
            rsL_y = ctrl_msg->data.rs[1];

            if (ctrl_msg->data.key != keyValue) {
                keyValue = ctrl_msg->data.key;
                modelListener->onKeyValueChanged(keyValue);
            }
        remote_changed = true;
        }
        if (remote_changed) {
            modelListener->onVoltageChanged(voltage);
            modelListener->onJoystickChanged(rsL_x, rsL_y, rsR_x, rsR_y);
            remote_changed = false;
            }
        }
    }
}

/* 将按键的值更改到UI */
void Model::setKeyValue(int8_t k) {
    keyValue = k;
    if (modelListener) {
        modelListener->onKeyValueChanged(keyValue);
    }
}
