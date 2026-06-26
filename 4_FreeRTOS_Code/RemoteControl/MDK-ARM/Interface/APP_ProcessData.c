#include "APP_ProcessData.h"

Joystick_Stuct Joystick_Data = {0}; // 定义一个遥感结构体变量，用于存储遥感数据
Remote_Data remote_Data = {0};      // 定义一个遥感结构体变量，用于存储遥感数据


/**
 * @brief 按下按键偏移值
 *
 */
int16_t key_offset_pitch = 0;
int16_t key_offset_roll = 0;


/**
 * @brief 遥感偏移量
 *
 */
int16_t remote_offset_throttle = 0;
int16_t remote_offset_pitch = 0;
int16_t remote_offset_yaw = 0;
int16_t remote_offset_roll = 0;


/**
 * @brief 处理按键数据
 *
 *
 */
void APP_ProcessKeyData(void)
{
    Key_type key = Key_Scan(); // 扫描按键状态
    if (key == KEY_UP)
    {
        key_offset_pitch += 10; // 按下上键，增加俯仰角偏移值
    }
    else if (key == KEY_DOWN)
    {
        key_offset_pitch -= 10; // 按下下键，减少俯仰角偏移值
    }
    else if (key == KEY_LEFT)
    {
        key_offset_roll -= 10; // 按下左键，减少横滚角偏移值
    }
    else if (key == KEY_RIGHT)
    {
        key_offset_roll += 10; // 按下右键，增加横滚角偏移值
    }
    else if (key == KEY_LEFT_X)
    {
        remote_Data.shutdown = 1; // 按下左键，设置遥感关机标志位为1
    }
    else if (key == KEY_RIGHT_X)
    {
        remote_Data.fix_high = 1; // 按下右键，设置遥感定高标志位为1
    }
    else if (key == KEY_RIGHT_X_LONG)
    {
        APP_CalibrateJoystickData(); // 校准摇杆数据
    }
}


/**
 * @brief 校准摇杆数据
 *
 */
void APP_CalibrateJoystickData(void)
{
    key_offset_pitch = 0;
    key_offset_roll = 0;   
    int16_t throttle_sum = 0;
    int16_t yaw_sum = 0;
    int16_t pitch_sum = 0;
    int16_t roll_sum = 0;
    for (uint8_t i = 0; i < 10; i++)
    {
        APP_ProcessJoystickData(); // 处理摇杆数据
        throttle_sum += Joystick_Data.throttle - 0;
        yaw_sum += Joystick_Data.yaw - 500;
        pitch_sum += Joystick_Data.pitch - 500;
        roll_sum += Joystick_Data.roll - 500;
        vTaskDelay(10); // 延时10ms，等待遥感数据稳定
    }
    remote_offset_throttle += throttle_sum / 10;
    remote_offset_yaw += yaw_sum / 10;
    remote_offset_pitch += pitch_sum / 10;
    remote_offset_roll += roll_sum / 10;
}


/**
 * @brief 处理摇杆数据
 *
 *
 * @note 12位ADC采样值范围为0~4095，遥感的油门、偏航角、俯仰角、横滚角的范围为0~1000，遥感的中立位置为500。
 *
 */
void APP_ProcessJoystickData(void)
{
    taskENTER_CRITICAL(); // 进入临界区，防止中断打断遥感数据处理
    Joystick_GetValue(&Joystick_Data);                                      // 获取遥感值
    Joystick_Data.pitch = 1000 - (Joystick_Data.pitch * 1000 / 4095);       // 将ADC采样值转换为遥感的俯仰角值
    Joystick_Data.yaw = 1000 - (Joystick_Data.yaw * 1000 / 4095);           // 将ADC采样值转换为遥感的偏航角值
    Joystick_Data.roll = 1000 - (Joystick_Data.roll * 1000 / 4095);         // 将ADC采样值转换为遥感的横滚角值
    Joystick_Data.throttle = 1000 - (Joystick_Data.throttle * 1000 / 4095); // 将ADC采样值转换为遥感的油门值

    Joystick_Data.throttle -= remote_offset_throttle; // 油门值减去偏移量
    Joystick_Data.yaw -= remote_offset_yaw;           // 偏航角值减去偏移量
    Joystick_Data.pitch -= remote_offset_pitch;       // 俯仰角值减去偏移量
    Joystick_Data.roll -= remote_offset_roll;         // 横滚角值减去偏移量


    //遥感偏移量加上按键偏移量
    Joystick_Data.pitch += key_offset_pitch; // 俯仰角值加
    Joystick_Data.roll += key_offset_roll;   // 横滚角值加

    Joystick_Data.throttle = Com_Limit(Joystick_Data.throttle, 0, 1000); // 限制油门值在0~1000之间
    Joystick_Data.yaw = Com_Limit(Joystick_Data.yaw, 0, 1000);     // 限制偏航角值在0~1000之间
    Joystick_Data.pitch = Com_Limit(Joystick_Data.pitch, 0, 1000);   // 限制俯仰角值在0~1000之间
    Joystick_Data.roll = Com_Limit(Joystick_Data.roll, 0, 1000);    // 限制横滚角值在0~1000之间

    remote_Data.throttle = Joystick_Data.throttle;
    remote_Data.yaw = Joystick_Data.yaw;
    remote_Data.pitch = Joystick_Data.pitch;
    remote_Data.roll = Joystick_Data.roll;

    taskEXIT_CRITICAL(); // 退出临界区
    debug_printf(": %d, %d, %d, %d\n", remote_Data.throttle, remote_Data.yaw, remote_Data.pitch, remote_Data.roll); // 打印遥感值
}
