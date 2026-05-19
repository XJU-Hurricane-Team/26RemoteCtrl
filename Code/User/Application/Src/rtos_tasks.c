/**
 * @file    rtos_tasks.c
 * @author  Deadline039
 * @brief   RTOS tasks.
 * @version 1.0
 * @date    2024-01-31
 */

#include "includes.h"

static TaskHandle_t start_task_handle;
void start_task(void *pvParameters);

static TaskHandle_t task1_handle;
void task1(void *pvParameters);

TaskHandle_t TouchGFX_Task_handle;
void TouchGFX_Task(void *pvParameters);

static TaskHandle_t message_polling_task_handle;
void message_polling_task(void *pvParameters);

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
    BaseType_t ret;
    taskENTER_CRITICAL();

    ret = xTaskCreate(task1, "task1", 128, NULL, 2, &task1_handle);
    if (ret != pdPASS) {
        taskEXIT_CRITICAL();
        while (1) {}
    }

    ret = xTaskCreate(TouchGFX_Task, "TouchGFX_Task", 4096, NULL, 2,
                      &TouchGFX_Task_handle);
    if (ret != pdPASS) {
        taskEXIT_CRITICAL();
        while (1) {}
    }

    ret = xTaskCreate(message_polling_task, "message_polling_task", 128, NULL,
                      2, &message_polling_task_handle);
    if (ret != pdPASS) {
        taskEXIT_CRITICAL();
        while (1) {}
    }

    taskEXIT_CRITICAL();
    Play_Music();

    vTaskDelete(start_task_handle);
}

/**
 * @brief Task1: Blink.
 *
 * @param pvParameters Start parameters.
 */
void task1(void *pvParameters) {
    UNUSED(pvParameters);
    while (1) {
        LED0_TOGGLE();
        vTaskDelay(1000);
    }
}

/**
 * @brief TouchGFX_Task: TouchGFX processing task.
 *
 * @param pvParameters Start parameters.
 */
void TouchGFX_Task(void *pvParameters) {
    UNUSED(pvParameters);
    MX_TouchGFX_Process();
    vTaskDelay(10);
}

/**
 * @brief message_polling_task 指定串口轮询数据函数
 *
 * @param pvParameters
 */
void message_polling_task(void *pvParameters) {
    UNUSED(pvParameters);
    /* 创建发送任务 */
    remote_send_init(&huart1);

    /* 注册接收回调函数 */
    message_register_recv_callback(MSG_MASTER_TO_RC, remote_recv_msg_callback);
    message_register_polling_uart(MSG_MASTER_TO_RC, &huart1, 128, 128);

    while (1) {
        message_polling_data();
        IWDG_Feed();//看门狗喂狗
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
