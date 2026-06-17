#ifndef __COM_DEBUG_H
#define __COM_DEBUG_H


#include "usart.h"
#include "stdio.h"
#include "stdarg.h"
#include "string.h"


//日志打印过于占用资源，使用开关控制是否开启调试打印功能

#define DEBUG_LOG_ENABLE        1

#ifdef DEBUG_LOG_ENABLE

//使用宏定义方式，只打印文件名称，不打印路径名称
#define __FILE_NAME__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)
#define __FILE_NAME (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)

//使用宏定义实现调试打印功能，输出文件名和行号
#define debug_printf(format,...) printf("[%s:%d]" format, __FILE_NAME, __LINE__, ##__VA_ARGS__)

#else
//如果未定义DEBUG_LOG_ENABLE，则将debug_printf定义为空
#define debug_printf(format, ...)
#endif



#endif
