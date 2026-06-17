#include "APP_RTOS_Task.h"


//任务创建
void Task1 (void *args)
{

}


void App_freeRTOS_Start(void)
{
    //创建任务
    xTaskCreate(Task1,  //任务函数
                "Task1", //任务名称
                128,     //任务堆栈大小
                NULL,    //任务参数
                1,       //任务优先级
                NULL);   //任务句柄

    //启动调度器
    vTaskStartScheduler();
}
