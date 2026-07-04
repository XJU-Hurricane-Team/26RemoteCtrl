/**
 * @file    Model.cpp
 * @brief   应用数据模型实现
 */
#include "includes.h"
#include <gui/model/Model.hpp>
#include <gui/model/ModelListener.hpp>

const char *const Model::kPresetPointNames[Model::kPresetPointCount] = {
    "NONE",
    "CATCH_1",
    "CATCH_2",
    "CATCH_3",
    "CATCH_4",
    "ASSEMBLE",
    "GUARD_F",
    "SNIPE_G",
    "BACK_H"
};
const char Model::kCtrlLabels[2]   = {'M', 'A'};
const char Model::kSourceLabels[2] = {'S', 'W'};

Model::Model()
    : modelListener(nullptr),
      /* 遥控器数据 */
      ctrl_keyValue(0), choosepoint(0),
      keyValue(0), voltage(0),
      rsL_x(0), rsL_y(0), rsR_x(0), rsR_y(0),
      irda_msgnum(0),
      /* R1 底盘数据 */
      r1_pose_x(0), r1_pose_y(0), r1_pose_yaw(0),
      r1_chassis_status(1), r1_chassis_state(0), r1_yaw_source(0),
      r1_accel_xy(0.0f),
      r1_left_pos(0.0f), r1_right_pos(0.0f),
      r1_left_adsorbed(0), r1_right_adsorbed(0),
      r1_send_msg(0), r1_rec_msg(0)
    //   r2_pose_x(0), r2_pose_y(0), r2_angle(0), r2_status(0)
      {}

void Model::tick() {
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
                    if (ctrl_msg->ctrl_key != ctrl_keyValue) {
                        ctrl_keyValue = ctrl_msg->ctrl_key;
                        modelListener->onCtrlKeyChanged(ctrl_keyValue);
                    }
                    if (ctrl_msg->data.point != choosepoint) {
                        choosepoint = ctrl_msg->data.point;
                        modelListener->onChoosepointChanged(choosepoint);
                    }

                    modelListener->onIrdaMsgNumChanged(irda_msgnum);

                    remote_changed = true;
                } break;

                case UI_R1_STATE: {
                    const r1_data_t *r1_msg = &msg.payload.r1_state;
                    r1_pose_x = r1_msg->pose_x;
                    r1_pose_y = r1_msg->pose_y;
                    r1_pose_yaw = r1_msg->pose_yaw;
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
                    r1_irda_send_mode = r1_msg->send_mode;
                    r1_changed = true;
                } break;

                case UI_SCREEN_STATE: {
                    const ui_screen_state_t *s = &msg.payload.screen_state;
                    bool in_sub = (s->screen == SCREEN_RED_SUB) || (s->screen == SCREEN_BLUE_SUB);
                    modelListener->onMapSubModeChanged(in_sub, s->preset_idx);
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
                r1_pose_x, r1_pose_y, r1_pose_yaw, r1_chassis_status, r1_chassis_state, r1_accel_xy,
                r1_left_pos, r1_right_pos, r1_left_adsorbed, r1_right_adsorbed,
                r1_send_msg, r1_rec_msg, r1_yaw_source, r1_irda_send_mode);
            r1_changed = false;
        }
    }
}

/**
 * @brief 将按键的值更改到UI，并通知监听器更新UI显示
 */
void Model::setKeyValue(int8_t k) {
    keyValue = k;
    if (modelListener) {
        modelListener->onKeyValueChanged(keyValue);
    }
}
