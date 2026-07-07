#ifndef __APP_FLIGHT_H__
#define __APP_FLIGHT_H__


#include "MPU6050.h"
#include "Com_config.h"
#include "Com_debug.h"
#include "Com_filter.h"
#include "Com_IMU.h"
#include "Com_PID.h"
#include "math.h"
#include "Motor.h"



void APP_Flight_Init(void);
void APP_Flight_Get_Euler_Angle(void);
void APP_Flight_PID_Process(void);
void APP_Flight_ControlMotor(void);


#endif
