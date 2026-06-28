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
#define FRAME_HEADER1 'l'
#define FRAME_HEADER2 'h'
#define FRAME_HEADER3 'y'




void APP_TransmitData(void);

#endif
