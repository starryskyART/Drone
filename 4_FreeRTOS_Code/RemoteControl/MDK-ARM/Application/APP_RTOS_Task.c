#include "APP_RTOS_Task.h"



/*
////STM32F103C8T6的SRAM大小为20K，也就是20 * 1024 = 20480字节，
////FreeRTOS的堆大小配置为17K，剩余的内存用于任务栈和其他用途。
*/

//分配128字节的堆栈空间给Power_Task任务
#define Power_Task_STACK_SIZE 128 
//设置Power_Task任务的优先级为1，数值越大优先级越高，最大4，不推荐使用最小优先级0，影响CPU睡眠时一直占用CPU资源
#define Power_Task_PRIORITY   4   
TaskHandle_t Power_Task_Handler; //定义一个任务句柄，用于管理Power_Task任务
void Power_Task( void *args )
{
    //获取当前基准时间
    TickType_t xLastWakeTime = xTaskGetTickCount();
    while(1)
    {
        //每十秒执行一次，避免电源自动关机,比xTaskDelay更准确，适合周期性任务
        vTaskDelayUntil(&xLastWakeTime, 10000);
        //启动电源逻辑
        Init_Power();
    }
}



void App_RTOS_Start(void)
{
    //创建任务
    xTaskCreate(Power_Task, "Power_Task", Power_Task_STACK_SIZE, NULL, Power_Task_PRIORITY, &Power_Task_Handler);

    //启动调度器
    vTaskStartScheduler();
}
