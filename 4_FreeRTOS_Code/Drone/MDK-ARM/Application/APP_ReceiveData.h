#ifndef __APP_RECEIVEDATA_H__
#define __APP_RECEIVEDATA_H__

#include "SI24R1.h"
#include "Com_debug.h"
#include "Com_config.h"

/**
 * @校验帧
 *
 */
#define FRAME_HEADER1 'l'
#define FRAME_HEADER2 'h'
#define FRAME_HEADER3 'y'

uint8_t ReceiveData(void);

#endif
