#include "APP_ReceiveData.h"

extern Remote_Data remote_data;                   // 声明外部静态遥控器数据结构体
extern Remote_State remote_state;                 // 声明外部变量，表示飞行状态
extern Flight_State flight_state;                 // 声明外部变量，表示飞行状态
extern Throttle_State throttle_state;             // 声明外部变量，表示油门状态
uint8_t connect_count = 0;                        // 记录遥控器连接次数
uint8_t Remote_ReceiveData[TX_PLOAD_WIDTH] = {0}; // 定义一个静态接收缓冲区
uint32_t throttle_max_time = 0;                     // 油门达到最大值持续时间计数
uint32_t throttle_min_time = 0;                     // 油门达到最小值持续时间计数

/**
 * @brief 接收遥控器数据
 *
 * @return uint8_t 返回1表示接收失败，返回0表示接收成功
 */
uint8_t ReceiveData(void)
{
    memset(Remote_ReceiveData, 0, TX_PLOAD_WIDTH); // 清空接收缓冲区
    SI24R1_RxPacket(Remote_ReceiveData);           // 接收遥控器数据
    // 判断是否接收到数据
    if (strlen((char *)Remote_ReceiveData) == 0)
    {
        return 1;
    }
    // 帧头校验
    if (Remote_ReceiveData[0] != FRAME_HEADER1 && Remote_ReceiveData[1] != FRAME_HEADER2 && Remote_ReceiveData[2] != FRAME_HEADER3)
    {
        return 1;
    }

    // 帧尾校验
    uint32_t sum = 0;
    uint32_t sum_Receive = 0;
    for (uint8_t i = 0; i < 13; i++)
    {
        sum += Remote_ReceiveData[i];
    }
    // 计算接收到的数据的和，高位在前
    sum_Receive = Remote_ReceiveData[13] << 24 | Remote_ReceiveData[14] << 16 | Remote_ReceiveData[15] << 8 | Remote_ReceiveData[16];
    if (sum != sum_Receive)
    {
        return 1;
    }

    // 将接收到的数据存入遥控器数据结构体
    remote_data.throttle = (Remote_ReceiveData[3] << 8) | Remote_ReceiveData[4];
    remote_data.yaw = (Remote_ReceiveData[5] << 8) | Remote_ReceiveData[6];
    remote_data.pitch = (Remote_ReceiveData[7] << 8) | Remote_ReceiveData[8];
    remote_data.roll = (Remote_ReceiveData[9] << 8) | Remote_ReceiveData[10];
    remote_data.shutdown = Remote_ReceiveData[11];
    remote_data.fix_high = Remote_ReceiveData[12];
    // debug_printf(": %d, %d, %d, %d, %d, %d\n", remote_data.throttle, remote_data.yaw, remote_data.pitch, remote_data.roll, remote_data.shutdown, remote_data.fix_high);
    return 0; // 接收成功
}

/**
 * @brief 遥控器连接状态判断
 *
 */
void Drone_Connect_State_Check(uint8_t res)
{

    if (res == 0)
    {
        remote_state = REMOTE_CONNECTED; // 遥控器已连接，飞行状态为NORMAL
        connect_count = 0;               // 一次连接成功后重置连接次数
    }
    else if (res == 1)
    {
        connect_count++; // 连接失败，增加连接失败次数
        if (connect_count >= Max_Connet)
        {
            remote_state = REMOTE_DISCONNECTED; // 连接失败次数达到最大值，飞行状态为FAIL
            connect_count = 0;                  // 重置连接次数
        }
    }
}

/**
 * @brief 遥控器解锁判断
 *
 * @return uint8_t 返回1表示解锁失败，返回0表示解锁成功
 */
static uint8_t Drone_Unlock(void)
{
    // 1. 兼顾安全问题 => 油门必须为最小值才能解锁
    switch (throttle_state)
    {
    case Free:
        if (remote_data.throttle >= 900)
        {
            throttle_state = MAX; // 油门达到最大值持续时间
            throttle_max_time = xTaskGetTickCount(); // 记录油门达到最大值的时间
        }
        break;
    case MAX:
        if (remote_data.throttle < 900)
        {
            if (xTaskGetTickCount() - throttle_max_time >= 1000) // 油门达到最大值持续时间超过1秒
            {
                throttle_state = LEAVE_MAX; // 油门达到最大值后离开最大值最长时间
            }
        }
        else
        {
            throttle_state = Free; // 油门未达到最大值，回到Free状态
        }
        break;
    case LEAVE_MAX:
        if (remote_data.throttle <= 100)
        {
            throttle_state = MIN; // 油门最小持续时间
            throttle_min_time = xTaskGetTickCount(); // 记录油门达到最小值的时间
        }
        break;
    case MIN:
        if (xTaskGetTickCount() - throttle_min_time <= 1000) // 油门达到最小值持续时间1秒之内
        {
            if (remote_data.throttle > 100)
            {
                throttle_state = Free; // 油门未达到最小值，回到Free状态
            }
            else
            {
                throttle_state = UNLOCKED; // 遥控器已解锁
            }
        }
        
        break;
    case UNLOCKED:
        break;
    default:
        break;
    }
    if (throttle_state == UNLOCKED)
    {
        return 0;
    }
    else 
    {
        return 1;
    }
}

/**
 * @brief 飞行状态判断
 *
 */
void Drone_State(void)
{
    switch (flight_state)
    {
    case IDLE:
        if (Drone_Unlock() == 0)
        {
            flight_state = NORMAL; // 解锁成功，飞行状态为NORMAL
            throttle_state = Free; // 解锁成功，重置油门状态为Free
        }
        break;
    case NORMAL:
        // 遥控器定高，飞行状态为FIX_HEIGHT
        if (remote_data.fix_high == 1)
        {
            flight_state = FIX_HEIGHT;
            remote_data.fix_high = 0; // 重置定高标志
        }
        // 遥控器断开连接，飞行状态为FAIL
        if (remote_state == REMOTE_DISCONNECTED)
        {
            flight_state = FAIL;
        }
        break;
    case FIX_HEIGHT:
        // 遥控器定高解除，飞行状态为NORMAL
        if (remote_data.fix_high == 0)
        {
            flight_state = NORMAL;
        }
        // 遥控器断开连接，飞行状态为FAIL
        if (remote_state == REMOTE_DISCONNECTED)
        {
            flight_state = FAIL;
        }
        break;
    case FAIL:
        // 遥控器断开连接，飞控缓慢下降，直到降落
        vTaskDelay(1);
        flight_state = IDLE; // 降落完成，飞行状态为IDLE
        break;
    default:
        break;
    }
}
