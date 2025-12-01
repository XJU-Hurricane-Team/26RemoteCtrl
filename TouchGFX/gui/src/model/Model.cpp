#include "includes.h"
#include <gui/model/Model.hpp>
#include <gui/model/ModelListener.hpp>
// 访问在 freertos.c 中创建的消息队列
QueueHandle_t SampleQueue;

Model::Model() : modelListener(0), keyValue(0)
{

}

void Model::tick()
{
    /* UI在这里进行数据采集，为了不阻塞UI渲染通过消息队列进行通信 */
    if (SampleQueue != NULL)
    {
        int8_t msg[6] = {0};
        BaseType_t st = xQueueReceive(SampleQueue, &msg, 0U);
        if (st == pdPASS)
        {
            modelListener->onKeyValueChanged(keyValue);
            voltage = msg[0];
            rsR_x = msg[1];
            rsR_y = msg[2];
            rsL_x = msg[3];
            rsL_y = msg[4];
            int8_t k = msg[5];

            /*! 更新按键值*/
            if (k != keyValue)
            {
                keyValue = k;
                modelListener->onKeyValueChanged(keyValue);
            }

            /*! 更新电压值*/
            modelListener->onVoltageChanged(voltage);
            /*! 更新摇杆值*/
            modelListener->onJoystickChanged(rsL_x, rsL_y, rsR_x, rsR_y);
        }
    }
}

/* 将按键的值更改到UI */
void Model::setKeyValue(int8_t k)
{
    keyValue = k;
    if (modelListener)
    {
        modelListener->onKeyValueChanged(keyValue);
    }
}
