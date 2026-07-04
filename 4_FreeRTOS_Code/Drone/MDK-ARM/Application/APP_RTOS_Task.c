#include "APP_RTOS_Task.h"

// 任务优先级定义
#define Power_Task_PRIORITY 4  // 电源管理任务优先级
#define Flight_TASK_PRIORITY 3 // 飞行控制任务优先级
#define Com_Task_PRIORITY 2    // 通讯任务优先级
#define LED_Task_PRIORITY 1    // LED闪烁任务优先级

// 任务栈大小定义
#define Power_Task_STACK_SIZE 128  // 电源管理任务栈大小，单位为字节
#define Flight_TASK_STACK_SIZE 128 // 飞行控制任务栈大小，单位为字节
#define LED_Task_STACK_SIZE 128    // LED闪烁任务栈大小，单位为字节
#define Com_Task_STACK_SIZE 128    // 通讯任务栈大小，单位为字节

// 任务周期定义
#define POWER_TASK_PERIOD 10000 // 电源管理任务周期，单位为ms
#define FLIGHT_TASK_PERIOD 6    // 飞行控制任务周期，单位为ms
#define LED_TASK_PERIOD 100     // LED闪烁任务周期，单位为ms
#define COM_TASK_PERIOD 6       // 通讯任务周期，单位为ms

// 任务句柄定义
TaskHandle_t Power_Task_Handler;  // 电源管理任务句柄
TaskHandle_t Flight_Task_Handler; // 飞行控制任务句柄
TaskHandle_t LED_Task_Handler;    // LED闪烁任务句柄
TaskHandle_t Com_Task_Handler;    // 通讯任务句柄

// 内存管理，C语言当中的结构体通常保存在堆中，不会自动释放，始终使用同一个结构体来保存电机状态，避免频繁创建和销毁结构体导致内存碎片化

// 电机结构体定义
Motor_Struct left_top_motor = {.tim = &htim3, .channel = TIM_CHANNEL_1, .speed = 200};     // 电机1
Motor_Struct left_bottom_motor = {.tim = &htim4, .channel = TIM_CHANNEL_4, .speed = 200};  // 电机2
Motor_Struct right_top_motor = {.tim = &htim2, .channel = TIM_CHANNEL_2, .speed = 200};    // 电机3
Motor_Struct right_bottom_motor = {.tim = &htim1, .channel = TIM_CHANNEL_3, .speed = 200}; // 电机4

// LED结构体定义
LED_Struct left_top_led = {.Port = LED1_GPIO_Port, .Pin = LED1_Pin};
LED_Struct right_top_led = {.Port = LED2_GPIO_Port, .Pin = LED2_Pin};
LED_Struct right_bottom_led = {.Port = LED3_GPIO_Port, .Pin = LED3_Pin};
LED_Struct left_bottom_led = {.Port = LED4_GPIO_Port, .Pin = LED4_Pin};

// 当前连接状态
Remote_State remote_state = REMOTE_DISCONNECTED;  // 遥控器已连接状态
Remote_Data remote_data = {0};                    // 遥控器数据结构体
Throttle_State throttle_state = Free;             // 油门状态
uint8_t ReceiveData_buffer[TX_PLOAD_WIDTH] = {0}; // 静态接收缓冲区

// 表示当前飞行状态
Flight_State flight_state = NORMAL; // 初始飞行状态为NORMAL

// 电源管理任务
void Power_Task(void *args)
{
    // 获取当前基准时间
    TickType_t xLastWakeTime = xTaskGetTickCount();
    while (1)
    {
        // xTaskDelayUntil(&xLastWakeTime, POWER_TASK_PERIOD); // 每隔10秒执行一次
        // 任务通知接收，等待遥控器关机信号,每隔10秒check一次是否收到遥控器关机信号
        // ulTaskNotifyTake()函数会阻塞当前任务，直到收到通知或者超时，ulNotifyValue为收到的通知值，如果超时则为0
        uint32_t res = ulTaskNotifyTake(pdTRUE, POWER_TASK_PERIOD); // 等待任务通知，阻塞直到收到通知
        if (res != 0)
        {
            // 收到遥控器关机信号，执行关机操作
            Power_Shutdown();
        }
        else
        {
            // 超时未收到遥控器关机信号，正常运行
            Power_Start();
        }
    }
}

// 飞行控制任务
void Flight_Task(void *args)
{
    // 获取当前基准时间
    TickType_t xLastWakeTime = xTaskGetTickCount();
    MPU6050_Init(); // 初始化MPU6050传感器
    while (1)
    {
        // 获取三轴陀螺仪和三轴加速度计数据
        APP_Flight_Get_Euler_Angle();
        xTaskDelayUntil(&xLastWakeTime, FLIGHT_TASK_PERIOD); // 每隔6ms执行一次
    }
}

// LED闪烁任务
void LED_Task(void *args)
{
    // 获取当前基准时间
    TickType_t xLastWakeTime = xTaskGetTickCount();
    uint8_t count = 0;
    while (1)
    {
        count++;
        // 前两个灯表示连接状态
        // 判断当前连接状态
        if (remote_state == REMOTE_CONNECTED)
        {
            // 遥控器已连接，点亮前两个LED
            LED_Turn_On(&left_top_led);
            LED_Turn_On(&right_top_led);
        }
        else if (remote_state == REMOTE_DISCONNECTED)
        {
            // 遥控器已断开，关掉前两个LED
            LED_Turn_Off(&left_top_led);
            LED_Turn_Off(&right_top_led);
        }

        // 后两个灯表示飞行状态
        if (flight_state == IDLE)
        {
            // 空闲状态，后两个LED慢闪烁，500ms亮，500ms灭
            if (count % 5 == 0)
            {
                // 如果是5的公倍数，那么闪烁的速度为count自增的公倍数，count自增一次100ms
                LED_Toggle(&left_bottom_led);
                LED_Toggle(&right_bottom_led);
            }
        }
        else if (flight_state == NORMAL)
        {
            // 飞行状态，灯快闪，200ms亮，200ms灭
            if (count % 2 == 0)
            {
                // 如果是2的公倍数，那么闪烁的速度为count自增的速度乘被整除的数，count自增一次100ms
                LED_Toggle(&left_bottom_led);
                LED_Toggle(&right_bottom_led);
            }
        }
        else if (flight_state == FIX_HEIGHT)
        {
            // 定高飞行状态，后两个LED常亮
            LED_Turn_On(&left_bottom_led);
            LED_Turn_On(&right_bottom_led);
        }
        else if (flight_state == FAIL)
        {
            // 故障状态，后两个LED常灭
            LED_Turn_Off(&left_bottom_led);
            LED_Turn_Off(&right_bottom_led);
        }

        // 将count自增计数重置
        if (count == 10)
        {
            count = 0;
        }
        vTaskDelayUntil(&xLastWakeTime, LED_TASK_PERIOD); // 每隔100ms执行一次
    }
}

// 通讯任务
void Com_Task(void *args)
{
    TickType_t xLastWakeTime = xTaskGetTickCount(); // 获取当前基准时间
    while (1)
    {
        uint8_t res = ReceiveData();
        Drone_Connect_State_Check(res); // 检查遥控器连接状态
        if (remote_data.shutdown == 1)
        {
            // 使用Power_Shutdown()函数关闭电源可以实现
            //  Power_Shutdown();但是为了体现RTOS系统的优势
            //  使用RTOS直接任务通知 => 通知电源任务 => 电源任务执行关闭电源操作 => 电源任务需要接收任务通知
            //  任务通知的好处是可以实现任务间的通信和同步，避免了直接调用函数可能带来的问题，比如任务优先级不同导致的阻塞等问题
            xTaskNotifyGive(Power_Task_Handler); // 通知电源任务关闭电源
        }
        Drone_State();                                    // 检查飞行状态
        xTaskDelayUntil(&xLastWakeTime, COM_TASK_PERIOD); // 每隔COM_TASK_PERIODms执行一次
    }
}

void APP_RTOS_Start(void)
{
    // 电源控制任务
    xTaskCreate(Power_Task, "Power_Task", Power_Task_STACK_SIZE, NULL, Power_Task_PRIORITY, &Power_Task_Handler);
    // 飞行控制任务
    xTaskCreate(Flight_Task, "Flight_Task", Flight_TASK_STACK_SIZE, NULL, Flight_TASK_PRIORITY, &Flight_Task_Handler);
    // 创建LED闪烁任务
    xTaskCreate(LED_Task, "LED_Task", LED_Task_STACK_SIZE, NULL, LED_Task_PRIORITY, &LED_Task_Handler);
    // 创建通讯任务
    xTaskCreate(Com_Task, "Com_Task", Com_Task_STACK_SIZE, NULL, Com_Task_PRIORITY, &Com_Task_Handler);
    vTaskStartScheduler();
}
