#include "Com_PID.h"

/**
 * @brief PID计算函数
 *
 * @param pid PID结构体指针
 *
 */
void Com_PID_Calculate(PID_TypeDef *pid)
{
    // 1. 计算误差值：期望值 - 测量值
    pid->err = pid->desire - pid->measure;

    // 2. 计算积分累加值：累加误差值
    pid->integral += pid->err;

    // 判断是否为第一次计算PID，如果是第一次计算，则将上一次误差值设置为当前误差值，避免微分项计算错误，增加计算的稳定性
    if (pid->err_last == 0)
    {
        pid->err_last = pid->err;
    }

    // 3. 计算微分误差值： 当前误差 - 上一次误差
    float derivative = pid->err - pid->err_last;

    // 4. 计算PID输出值：比例项 + 积分项 + 微分项
    pid->output = pid->Kp * pid->err + pid->Ki * pid->integral * PERIOD + pid->Kd * derivative / PERIOD;

    // 5. 更新上一次误差值
    pid->err_last = pid->err;
}

/**
 * @brief PID计算函数（串级PID）
 *
 * @param pid PID结构体指针
 *
 */
void Com_PID_Calculate_Chain(PID_TypeDef *out_pid, PID_TypeDef *in_pid)
{
    // 1. 外环PID计算
    Com_PID_Calculate(out_pid);

    // 将外环PID的输出作为内环PID的期望值
    in_pid->desire = out_pid->output;

    // 2. 内环PID计算
    Com_PID_Calculate(in_pid);
}



int16_t Com_Limit(int16_t speed, int16_t max_speed, int16_t min_speed)
{
    if (speed > max_speed)
    {
        return max_speed;
    }
    else if (speed < min_speed)
    {
        return min_speed;
    }
    else
    {
        return speed;
    }
}

