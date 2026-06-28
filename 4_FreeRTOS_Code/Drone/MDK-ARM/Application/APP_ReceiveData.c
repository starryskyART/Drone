#include "APP_ReceiveData.h"

uint8_t Remote_ReceiveData[TX_PLOAD_WIDTH] = {0}; // 定义一个静态接收缓冲区
Remote_Data remote_data = {0};                    // 定义一个静态遥控器数据结构体

/**
 * @brief 接收遥控器数据
 *
 * @return uint8_t 返回1表示接收失败，返回0表示接收成功
 */
uint8_t ReceiveData(void)
{
    memset(Remote_ReceiveData, 0, TX_PLOAD_WIDTH); // 清空接收缓冲区
    SI24R1_RxPacket(Remote_ReceiveData);           // 接收遥控器数据
    // 判断是否接收到数据
    if (strlen((char *)Remote_ReceiveData) == 0)
    {
        return 1;
    }
    // 帧头校验
    if (Remote_ReceiveData[0] != FRAME_HEADER1 && Remote_ReceiveData[1] != FRAME_HEADER2 && Remote_ReceiveData[2] != FRAME_HEADER3)
    {
        return 1;
    }

    // 帧尾校验
    uint32_t sum = 0;
    uint32_t sum_Receive = 0;
    for (uint8_t i = 0; i < 13; i++)
    {
        sum += Remote_ReceiveData[i];
    }
    // 计算接收到的数据的和，高位在前
    sum_Receive = Remote_ReceiveData[13] << 24 | Remote_ReceiveData[14] << 16 | Remote_ReceiveData[15] << 8 | Remote_ReceiveData[16];
    if (sum != sum_Receive)
    {
        return 1;
    }

    // 将接收到的数据存入遥控器数据结构体
    remote_data.throttle = (Remote_ReceiveData[3] << 8) | Remote_ReceiveData[4];
    remote_data.yaw = (Remote_ReceiveData[5] << 8) | Remote_ReceiveData[6];
    remote_data.pitch = (Remote_ReceiveData[7] << 8) | Remote_ReceiveData[8];
    remote_data.roll = (Remote_ReceiveData[9] << 8) | Remote_ReceiveData[10];
    remote_data.shutdown = Remote_ReceiveData[11];
    remote_data.fix_high = Remote_ReceiveData[12];
    // debug_printf(": %d, %d, %d, %d, %d, %d\n", remote_data.throttle, remote_data.yaw, remote_data.pitch, remote_data.roll, remote_data.shutdown, remote_data.fix_high);
    return 0; // 接收成功
}
