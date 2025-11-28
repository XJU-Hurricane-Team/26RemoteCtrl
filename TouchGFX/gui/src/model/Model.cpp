#include "includes.h"
#include <gui/model/Model.hpp>
#include <gui/model/ModelListener.hpp>

// access the message queue created in freertos.c
extern QueueHandle_t SampleQueue;

Model::Model() : modelListener(0), keyValue(0)
{

}

void Model::tick()
{
    // Try to read one message from the SampleData queue (non-blocking)
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

void Model::setKeyValue(uint8_t k)
{
    keyValue = k;
    if (modelListener)
    {
        modelListener->onKeyValueChanged(keyValue);
    }
}
