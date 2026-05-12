#include "includes.h"
#include <gui/model/Model.hpp>
#include <gui/model/ModelListener.hpp>

Model::Model()
    : modelListener(nullptr), ctrl_keyValue(0), choosepoint(0),
      keyValue(0), voltage(0), rsL_x(0), rsL_y(0), rsR_x(0), rsR_y(0), irda_msgnum(0),
      r1_x_speed(0), r1_y_speed(0), r1_w_speed(0), r1_chassis_status(1), r1_chassis_state(0), r1_yaw_source(0),
      r1_accel_xy(0.0f), r1_left_pos(0.0f), r1_right_pos(0.0f), r1_left_adsorbed(0), r1_right_adsorbed(0),
      r1_send_msg(0), r1_rec_msg(0)
    //   r2_x_speed(0), r2_y_speed(0), r2_angle(0), r2_status(0) 
      {}

void Model::tick() {
    /* UI在这里进行数据采集，为了不阻塞UI渲染通过消息队列进行通信 */
    if ((ui_msg_queue != NULL) && (modelListener != nullptr)) {
        ui_msg_t msg = {};
        bool remote_changed = false;
        bool r1_changed = false;

        while (xQueueReceive(ui_msg_queue, &msg, 0U) == pdPASS) {
            switch (msg.type) {
                case UI_REMOTE_CTRL: {
                    const remote_ctrl_msg_t *ctrl_msg =
                        &msg.payload.remote_ctrl;
                    voltage = ctrl_msg->voltage;
                    rsR_x = ctrl_msg->data.rs[2];
                    rsR_y = ctrl_msg->data.rs[3];
                    rsL_x = ctrl_msg->data.rs[0];
                    rsL_y = ctrl_msg->data.rs[1];
                    irda_msgnum = ctrl_msg->data.irdamsg;

                    if (ctrl_msg->data.key != keyValue) {
                        keyValue = ctrl_msg->data.key;
                        modelListener->onKeyValueChanged(keyValue);
                    }
                    if (ctrl_msg->ctrl_key!= ctrl_keyValue || ctrl_msg->data.point != choosepoint ) {
                        ctrl_keyValue = ctrl_msg->ctrl_key;
                        choosepoint = ctrl_msg->data.point;
                        modelListener->onCtrlKeyValueChanged(ctrl_keyValue, choosepoint);
                    }

                    modelListener->onIrdaMsgNumChanged(irda_msgnum);

                    remote_changed = true;
                } break;

                case UI_R1_STATE: {
                    const r1_data_t *r1_msg = &msg.payload.r1_state;
                    r1_x_speed = r1_msg->x_speed;
                    r1_y_speed = r1_msg->y_speed;
                    r1_w_speed = r1_msg->w_speed;
                    r1_left_pos = r1_msg->left_pos;
                    r1_right_pos = r1_msg->right_pos;
                    r1_left_adsorbed = r1_msg->left_adsorbed;
                    r1_right_adsorbed = r1_msg->right_adsorbed;
                    r1_chassis_status = r1_msg->r1_chassis_status;
                    r1_chassis_state = r1_msg->r1_chassis_state;
                    r1_accel_xy = r1_msg->accel_xy;
                    r1_send_msg = r1_msg->send_msg;
                    r1_rec_msg = r1_msg->rec_msg;
                    r1_yaw_source = r1_msg->yaw_source;
                    r1_changed = true;
                } break;

                default:
                    break;
            }
        }

        if (remote_changed) {
            modelListener->onVoltageChanged(voltage);
            modelListener->onJoystickChanged(rsL_x, rsL_y, rsR_x, rsR_y);
            remote_changed = false;
        }
        if (r1_changed) {
            modelListener->onR1StateChanged(
                r1_x_speed, r1_y_speed, r1_w_speed, r1_chassis_status, r1_chassis_state, r1_accel_xy,
                r1_left_pos, r1_right_pos, r1_left_adsorbed, r1_right_adsorbed,
                r1_send_msg, r1_rec_msg);
            modelListener->R1StateChanged(r1_chassis_status, r1_chassis_state, r1_accel_xy, r1_yaw_source);
            r1_changed = false;
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
