#include "includes.h"
#include <gui/model/Model.hpp>
#include <gui/model/ModelListener.hpp>
// 访问在 freertos.c 中创建的消息队列
extern QueueHandle_t SampleQueue;

Model::Model() : modelListener(0), keyValue(0)
{

}

void Model::tick()
{
    /* UI在这里进行数据采集，为了不阻塞UI渲染通过消息队列进行通信 */
    if (SampleQueue != NULL)
    {
        uint16_t msg = 0;
        BaseType_t st = xQueueReceive(SampleQueue, &msg, 0U);
        if (st == pdPASS)
        {
            uint8_t k = (uint8_t)(msg & 0xFF);
            if (k != keyValue)
            {
                keyValue = k;
                modelListener->onKeyValueChanged(keyValue);
            }
        }
    }
}

/* 将按键的值更改到UI */
void Model::setKeyValue(uint8_t k)
{
    keyValue = k;
    if (modelListener)
    {
        modelListener->onKeyValueChanged(keyValue);
    }
}
