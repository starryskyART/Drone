#include "APP_RTOS_Task.h"

/*
////STM32F103C8T6的SRAM大小为20K，也就是20 * 1024 = 20480字节，
////FreeRTOS的堆大小配置为17K，剩余的内存用于任务栈和其他用途。
*/

// test
uint8_t test_data[TX_PLOAD_WIDTH] = {0}; // 定义一个测试数据数组，长度为32字节

// 定义任务堆栈大小
#define Power_Task_STACK_SIZE 128 // Power_Task
#define Com_Task_STACK_SIZE 128   // Com_Task
#define KEY_Task_STACK_SIZE 128   // KEY_Task

// 定义任务优先级(数值越大优先级越高，最大4，不推荐使用最小优先级0，影响CPU睡眠时一直占用CPU资源)
#define Power_Task_PRIORITY 4 // Power_Task
#define Com_Task_PRIORITY 3   // Com_Task
#define KEY_Task_PRIORITY 2   // KEY_Task

// 定义任务句柄
TaskHandle_t Power_Task_Handler; // Power_Task
TaskHandle_t Com_Task_Handler;   // Com_Task
TaskHandle_t KEY_Task_Handler;   // KEY_Task

// 定义任每次运行的时间间隔，单位为毫秒
#define Power_Task_Period 10000 // Power_Task每10秒执行一次
#define Com_Task_Period 6       // Com_Task每6ms执行一次
#define KEY_Task_Period 20     // KEY_Task每20ms执行一次


///******************** 任务函数定义 ********************/

/**
 * @brief Power_Task任务函数
 */
void Power_Task(void *args)
{
    // 获取当前基准时间
    TickType_t xLastWakeTime = xTaskGetTickCount();
    while (1)
    {
        // 每十秒执行一次，避免电源自动关机,比xTaskDelay更准确，适合周期性任务
        vTaskDelayUntil(&xLastWakeTime, Power_Task_Period);
        // 启动电源逻辑
        Init_Power();
    }
}

/**
 * @brief 通讯任务函数
 *
 */
void Com_Task(void *args)
{
    TickType_t xLastWakeTime = xTaskGetTickCount(); // 获取当前基准时间
    while (1)
    {
        // 启动SI24R1通讯逻辑，芯片默认接收模式
        SI24R1_TX_Mode(); // 切换到发送模式
        test_data[0] = 'H';
        test_data[1] = 'E';
        test_data[2] = 'L';
        test_data[3] = 'L';
        test_data[4] = 'O';
        test_data[5] = '!';
        SI24R1_TxPacket(test_data);                       // 发送测试数据
        SI24R1_RX_Mode();                                 // 切换到接收模式
        vTaskDelayUntil(&xLastWakeTime, Com_Task_Period); // 每次执行间隔6ms，适合高频率任务
    }
}


/**
 * @brief KEY_Task任务函数
 *
 */
void KEY_Task(void *args)
{
    TickType_t xLastWakeTime = xTaskGetTickCount(); // 获取当前基准时间
    while (1)
    {
        Key_type key = Key_Scan(); // 扫描按键状态
        if (key != KEY_NONE)
        {
           debug_printf("Key Pressed: %d\n", key); // 打印按键状态
            
        }
        vTaskDelayUntil(&xLastWakeTime, KEY_Task_Period); 
    }
}

/******************** 任务函数定义结束 ********************/



void App_RTOS_Start(void)
{
    // 创建任务
    xTaskCreate(Power_Task, "Power_Task", Power_Task_STACK_SIZE, NULL, Power_Task_PRIORITY, &Power_Task_Handler);
    // 创建通讯任务
    xTaskCreate(Com_Task, "Com_Task", Com_Task_STACK_SIZE, NULL, Com_Task_PRIORITY, &Com_Task_Handler);
    // 创建按键扫描任务
    xTaskCreate(KEY_Task, "KEY_Task", KEY_Task_STACK_SIZE, NULL, KEY_Task_PRIORITY, &KEY_Task_Handler);
    // 启动调度器
    vTaskStartScheduler();
}
