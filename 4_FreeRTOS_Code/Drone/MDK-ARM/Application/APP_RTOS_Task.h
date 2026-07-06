#ifndef __APP_RTOS_TASK_H__
#define __APP_RTOS_TASK_H__


#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "Com_debug.h"
#include "Com_config.h"
#include "Power.h"
#include "Motor.h"
#include "LED.h"
#include "SI24R1.h"
#include "MPU6050.h"
#include "APP_ReceiveData.h"
#include "APP_Flight.h"






void APP_RTOS_Start(void);


#endif
