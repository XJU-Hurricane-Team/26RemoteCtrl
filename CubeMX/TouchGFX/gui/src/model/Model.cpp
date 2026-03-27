#include "includes.h"
#include <gui/model/Model.hpp>
#include <gui/model/ModelListener.hpp>

Model::Model() : modelListener(0), keyValue(0) {}

void Model::tick() {
    /* UI在这里进行数据采集，为了不阻塞UI渲染通过消息队列进行通信 */
    if ((ui_msg_queue != NULL) && (modelListener != 0)) {
        ui_msg_t msg = {};
        BaseType_t st = xQueueReceive(ui_msg_queue, &msg, 0U);
        if (st == pdPASS) {
            switch (msg.type) {
                case UI_REMOTE_CTRL: {
                    remote_ctrl_msg_t *ctrl_msg = (remote_ctrl_msg_t *)msg.data;
                    /*! 更新数据 */
                    voltage = ctrl_msg->voltage;
                    rsR_x = ctrl_msg->data->rs[2];
                    rsR_y = ctrl_msg->data->rs[3];
                    rsL_x = ctrl_msg->data->rs[0];
                    rsL_y = ctrl_msg->data->rs[1];
                    int8_t k = ctrl_msg->data->key;

                    /*! 更新按键值*/
                    if (k != keyValue) {
                        keyValue = k;
                        modelListener->onKeyValueChanged(keyValue);
                    }

                    /*! 更新电压值*/
                    modelListener->onVoltageChanged(voltage);
                    /*! 更新摇杆值*/
                    modelListener->onJoystickChanged(rsL_x, rsL_y, rsR_x,
                                                     rsR_y);
                } break;

                default:
                    break;
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
