#ifndef __APP_TRANSMITDATA_H
#define __APP_TRANSMITDATA_H


#include "FreeRTOS.h"
#include "task.h"
#include "APP_ProcessData.h"
#include "SI24R1.h"


/**
 * @校验帧
 * 
 */
#define FRAME_HEADER1 's'
#define FRAME_HEADER2 'g'
#define FRAME_HEADER3 'g'




void APP_TransmitData(void);

#endif
