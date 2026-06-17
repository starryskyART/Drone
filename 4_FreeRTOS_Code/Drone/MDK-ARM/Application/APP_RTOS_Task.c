#include "APP_RTOS_Task.h"




///电源管理任务
#define Power_Task_STACK_SIZE 128
#define Power_Task_PRIORITY 4
TaskHandle_t Power_Task_Handler;
void Power_Task(void *args)
{
    //获取当前基准时间
    TickType_t xLastWakeTime = xTaskGetTickCount();
    while(1)
    {
        xTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(1000)); //每隔1秒执行一次
        Power_Init();
    }
}






void APP_RTOS_Start(void)
{
    xTaskCreate(Power_Task, "Power_Task", Power_Task_STACK_SIZE, NULL, Power_Task_PRIORITY, &Power_Task_Handler);
    vTaskStartScheduler();
}
