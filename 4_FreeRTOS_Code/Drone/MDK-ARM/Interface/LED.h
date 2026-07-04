#ifndef __LED_H__
#define __LED_H__

#include "main.h"


typedef struct
{
    GPIO_TypeDef *Port;
    uint16_t Pin;
} LED_Struct;


/**
 * @brief 打开LED灯
 * 
 * 
 * @param LED
 */
void LED_Turn_On(LED_Struct *led);


/**
 * @brief 关闭LED灯
 * 
 * 
 * @param LED
 */
void LED_Turn_Off(LED_Struct *led);


/**
 * @brief 翻转LED灯
 * 
 * 
 * @param LED
 */
void LED_Toggle(LED_Struct *led);


#endif
