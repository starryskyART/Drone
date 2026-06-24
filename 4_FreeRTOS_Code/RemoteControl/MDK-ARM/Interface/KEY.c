#include "KEY.h"




/**
 * @brief 扫描按键状态
 * 
 * 
 * @retval Key_type 按键类型
 * 
 */
Key_type Key_Scan(void)
{
    if (HAL_GPIO_ReadPin(KEY_UP_GPIO_Port, KEY_UP_Pin) == GPIO_PIN_RESET)
    {
        //消抖
        vTaskDelay(5);
        if (HAL_GPIO_ReadPin(KEY_UP_GPIO_Port, KEY_UP_Pin) == GPIO_PIN_RESET)
        {
            //等待按键释放
            while (HAL_GPIO_ReadPin(KEY_UP_GPIO_Port, KEY_UP_Pin) == GPIO_PIN_RESET)
            {
                vTaskDelay(1);
            }
            return KEY_UP;//按键KEY_UP被按下
        }
    }
    else if (HAL_GPIO_ReadPin(KEY_DOWN_GPIO_Port, KEY_DOWN_Pin) == GPIO_PIN_RESET)
    {
        //消抖
        vTaskDelay(5);
        if (HAL_GPIO_ReadPin(KEY_DOWN_GPIO_Port, KEY_DOWN_Pin) == GPIO_PIN_RESET)
        {
            //等待按键释放
            while (HAL_GPIO_ReadPin(KEY_DOWN_GPIO_Port, KEY_DOWN_Pin) == GPIO_PIN_RESET)
            {
                vTaskDelay(1);
            }
            return KEY_DOWN;//按键KEY_DOWN被按下
        }
    }
    else if (HAL_GPIO_ReadPin(KEY_LEFT_GPIO_Port, KEY_LEFT_Pin) == GPIO_PIN_RESET)
    {
        //消抖
        vTaskDelay(5);
        if (HAL_GPIO_ReadPin(KEY_LEFT_GPIO_Port, KEY_LEFT_Pin) == GPIO_PIN_RESET)
        {
            //等待按键释放
            while (HAL_GPIO_ReadPin(KEY_LEFT_GPIO_Port, KEY_LEFT_Pin) == GPIO_PIN_RESET)
            {
                vTaskDelay(1);
            }
            return KEY_LEFT;//按键KEY_LEFT被按下
        }
    }
    else if (HAL_GPIO_ReadPin(KEY_RIGHT_GPIO_Port, KEY_RIGHT_Pin) == GPIO_PIN_RESET)
    {
        //消抖
        vTaskDelay(5);
        if (HAL_GPIO_ReadPin(KEY_RIGHT_GPIO_Port, KEY_RIGHT_Pin) == GPIO_PIN_RESET)
        {
            //等待按键释放
            while (HAL_GPIO_ReadPin(KEY_RIGHT_GPIO_Port, KEY_RIGHT_Pin) == GPIO_PIN_RESET)
            {
                vTaskDelay(1);
            }
            return KEY_RIGHT;//按键KEY_RIGHT被按下
        }
    }
    else if (HAL_GPIO_ReadPin(KEY_LEFT_X_GPIO_Port, KEY_LEFT_X_Pin) == GPIO_PIN_RESET)
    {
        //消抖
        vTaskDelay(5);
        if (HAL_GPIO_ReadPin(KEY_LEFT_X_GPIO_Port, KEY_LEFT_X_Pin) == GPIO_PIN_RESET)
        {
            //等待按键释放
            while (HAL_GPIO_ReadPin(KEY_LEFT_X_GPIO_Port, KEY_LEFT_X_Pin) == GPIO_PIN_RESET)
            {
                vTaskDelay(1);
            }
            return KEY_LEFT_X;//按键KEY_LEFT_X被按下
        }
    }
    else if (HAL_GPIO_ReadPin(KEY_RIGHT_X_GPIO_Port, KEY_RIGHT_X_Pin) == GPIO_PIN_RESET)
    {
        //获取按键按下的时间
        TickType_t Count1 = xTaskGetTickCount();
        //消抖
        vTaskDelay(5);
        if (HAL_GPIO_ReadPin(KEY_RIGHT_X_GPIO_Port, KEY_RIGHT_X_Pin) == GPIO_PIN_RESET)
        {
            //等待按键释放
            while (HAL_GPIO_ReadPin(KEY_RIGHT_X_GPIO_Port, KEY_RIGHT_X_Pin) == GPIO_PIN_RESET)
            {
                vTaskDelay(1);
            }
            TickType_t Count2 = xTaskGetTickCount();
            if ((Count2 - Count1) >= 1000) //按键按下时间超过1秒
            {
                return KEY_RIGHT_X_LONG; //按键KEY_RIGHT_X被长按
            }
            else
            {
                return KEY_RIGHT_X; //按键KEY_RIGHT_X被短按
            }
        }
    }
    return KEY_NONE;
}
