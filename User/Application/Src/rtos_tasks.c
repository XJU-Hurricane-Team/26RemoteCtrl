/**
 * @file    rtos_tasks.c
 * @author  Deadline039
 * @brief   RTOS tasks.
 * @version 1.0
 * @date    2024-01-31
 */

#include "includes.h"
#include "app_touchgfx.h"

static TaskHandle_t start_task_handle;
void start_task(void *pvParameters);

static TaskHandle_t UiTask_handle;
void UiTask(void *pvParameters);

static TaskHandle_t task2_handle;
void task2(void *pvParameters);

static TaskHandle_t task3_handle;
void task3(void *pvParameters);

QueueHandle_t SampleQueue;

/*****************************************************************************/

/**
 * @brief FreeRTOS start up.
 *
 */
void freertos_start(void) {
    xTaskCreate(start_task, "start_task", 128, NULL, 2, &start_task_handle);
    vTaskStartScheduler();
}

/**
 * @brief Start up task.
 *
 * @param pvParameters Start parameters.
 */
void start_task(void *pvParameters) {
    UNUSED(pvParameters);
    MX_TouchGFX_Init();
    /* Call PreOsInit function */
    MX_TouchGFX_PreOSInit();
    taskENTER_CRITICAL();

    xTaskCreate(UiTask, "UiTask", 1024 * 8, NULL, 2, &UiTask_handle);
    xTaskCreate(task2, "task2", 128, NULL, 2, &task2_handle);
    xTaskCreate(task3, "task3", 128, NULL, 2, &task3_handle);

    SampleQueue = xQueueCreate(1, sizeof(uint8_t));

    vTaskDelete(start_task_handle);
    taskEXIT_CRITICAL();
}

/**
 * @brief UiTask: UI processing task.
 *
 * @param pvParameters Start parameters.
 */
void UiTask(void *pvParameters) {
    UNUSED(pvParameters);

    while (1) {
        MX_TouchGFX_Process();
    }
}

/**
 * @brief Task2: print running time.
 *
 * @param pvParameters Start parameters.
 */
void task2(void *pvParameters) {
    UNUSED(pvParameters);
    LED0_OFF();
    while (1) {
        LED0_TOGGLE();
        printf("STM32F4xx FreeRTOS project template. Running time: %u ms. \n",
               xTaskGetTickCount());
        vTaskDelay(1000);
    }
}

/**
 * @brief Task3: 信息采集。
 *
 * @param pvParameters Start parameters.
 */
void task3(void *pvParameters) {
    UNUSED(pvParameters);

    uint8_t key = KEY_NO_PRESS;
    uint32_t prevTick = HAL_GetTick();

    LED1_ON();
    while (1) {
        key = key_scan(0);
        if (key != KEY_NO_PRESS) {
            xQueueOverwrite(SampleQueue, &key);
        }
        if (HAL_GetTick() - prevTick >= 500) {
            prevTick = HAL_GetTick();
            LED1_TOGGLE();
        }
        vTaskDelay(10);
    }
}

#ifdef configASSERT
/**
 * @brief FreeRTOS assert failed function. 
 * 
 * @param pcFile File name
 * @param ulLine File line
 */
void vAssertCalled(const char *pcFile, unsigned int ulLine) {
    fprintf(stderr, "FreeRTOS assert failed. File: %s, line: %u. \n", pcFile,
            ulLine);
}
#endif /* configASSERT */

#if configCHECK_FOR_STACK_OVERFLOW
/**
 * @brief The application stack overflow hook is called when a stack overflow is detected for a task.
 *
 * @param xTask the task that just exceeded its stack boundaries.
 * @param pcTaskName A character string containing the name of the offending task.
 */
void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName) {
    UNUSED(xTask);
    fprintf(stderr, "Stack overflow! Taskname: %s. \n", pcTaskName);
}
#endif /* configCHECK_FOR_STACK_OVERFLOW */

#if configUSE_MALLOC_FAILED_HOOK
/**
 * @brief This hook function is called when allocation failed.
 * 
 */
void vApplicationMallocFailedHook(void) {
    fprintf(stderr, "FreeRTOS malloc failed! \n");
}
#endif /* configUSE_MALLOC_FAILED_HOOK */