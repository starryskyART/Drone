#ifndef __COM_PID_H__
#define __COM_PID_H__

#define PERIOD 0.006 // 控制周期

/**
 * @brief PID控制结构体
 *
 */
typedef struct
{
    float Kp;       /**< 比例系数 值越大响应速度越快*/
    float Ki;       /**< 积分系数 解决稳态误差 无人机控制中积分项一般不用*/
    float Kd;       /**< 微分系数 值越大 抑制效果越好 解决过调震荡*/
    float err;      /**< 当前误差*/
    float err_last; /**< 上一次误差*/
    float desire;   /**< 期望值*/
    float measure;  /**< 测量值*/
    float integral; /**< 积分累积*/
    float output;   /**< 输出值*/
} PID_TypeDef;

void Com_PID_Calculate(PID_TypeDef *pid);
void Com_PID_Calculate_Chain(PID_TypeDef *out_pid, PID_TypeDef *in_pid);

#endif
