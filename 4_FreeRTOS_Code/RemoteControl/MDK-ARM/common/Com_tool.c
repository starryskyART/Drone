#include "Com_tool.h"




/**
 * @brief 限幅函数，将输入值限制在指定范围内
 * @param value 输入值
 * @param min 最小值
 * @param max 最大值
 * @return 限幅后的值
 */
int16_t Com_Limit(int16_t value, int16_t min, int16_t max)
{
    int16_t result = value;
    if (value < min)
    {
        result = min;
    }
    else if (value > max)
    {
        result = max;
    } 
    return result;
}
