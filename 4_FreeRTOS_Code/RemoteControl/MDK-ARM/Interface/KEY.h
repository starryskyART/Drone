#ifndef __KEY_H
#define __KEY_H

#include "main.h"
#include "FreeRTOS.h"
#include "task.h"

/**
 * @brief 按键类型枚举
 * 
 */
typedef enum
{
    KEY_NONE = 0,
    KEY_UP,
    KEY_DOWN,
    KEY_LEFT,
    KEY_RIGHT,
    KEY_LEFT_X,
    KEY_RIGHT_X,
    KEY_RIGHT_X_LONG,
}Key_type;


Key_type Key_Scan(void);

#endif
