#include "APP_TransmitData.h"

extern Remote_Data remote_Data;              // 外部声明遥感数据结构体变量
uint8_t transmit_buffer[TX_ADR_WIDTH] = {0}; // 定义一个发送缓冲区，用于存储遥感数据的字节流

/**
 * @brief 发送数据函数
 *
 *
 */
void APP_TransmitData(void)
{
    SI24R1_TX_Mode(); // 设置SI24R1为发送模式
    // 将遥感数据结构体变量转换为字节流，存储到发送缓冲区中
    // 数据结构体大小为17字节，按照顺序存储遥感数据的各个字段
    // 帧头校验指定发给对应的设备：唯一性
    // 发送数据的结尾 => 添加校验和（将发送的数据累加到的值添加到数据的末尾）：可靠性
    int32_t checksum = 0;               // 定义一个校验和变量，用于计算遥感数据的校验和
    transmit_buffer[0] = FRAME_HEADER1; // 帧头1
    transmit_buffer[1] = FRAME_HEADER2; // 帧头2
    transmit_buffer[2] = FRAME_HEADER3; // 帧头3

    // 高八位在前
    transmit_buffer[3] = (remote_Data.throttle >> 8) & 0xFF; // 油门高八位
    transmit_buffer[4] = remote_Data.throttle & 0xFF;        // 油门低八位

    transmit_buffer[5] = (remote_Data.yaw >> 8) & 0xFF; // 偏航角高八位
    transmit_buffer[6] = remote_Data.yaw & 0xFF;        // 偏航角低八位

    transmit_buffer[7] = (remote_Data.pitch >> 8) & 0xFF; // 俯仰角高八位
    transmit_buffer[8] = remote_Data.pitch & 0xFF;        // 俯仰角低八位

    transmit_buffer[9] = (remote_Data.roll >> 8) & 0xFF; // 横滚角高八位
    transmit_buffer[10] = remote_Data.roll & 0xFF;       // 横滚角低八位

    // 理论上系统运行到这里可能出现未刚进行到准备数据这里任务切换的时候。
    // 上一行代码没有被记录下来，所以必须等到清除关机标志位之后才能切换任务，避免重复发送关机信号。
    // 使用临界区的方式确保在发送数据的过程中不会被中断打断，保证数据的完整性和一致性。
    taskENTER_CRITICAL();                       // 进入临界区，防止中断打断
    transmit_buffer[11] = remote_Data.shutdown; // 遥感关机标志位
    remote_Data.shutdown = 0;                   // 发送完毕后，清除遥感关机标志位，避免重复发送
    transmit_buffer[12] = remote_Data.fix_high; // 遥感定高标志位
    remote_Data.fix_high = 0;                   // 发送完毕后，清除遥感定高标志位，避免重复发送
    taskEXIT_CRITICAL();                        // 退出临界区，允许中断打断
    for (uint8_t i = 0; i < 13 - 1; i++)
    {
        checksum += transmit_buffer[i]; // 计算校验和
    }
    transmit_buffer[13] = (checksum >> 24) & 0xFF;
    transmit_buffer[14] = (checksum >> 16) & 0xFF;
    transmit_buffer[15] = (checksum >> 8) & 0xFF;
    transmit_buffer[16] = checksum & 0xFF; // 校验和低八位
	debug_printf("Transmit Data: %d, %d, %d, %d, %d, %d\n", remote_Data.throttle, remote_Data.yaw, remote_Data.pitch, remote_Data.roll, remote_Data.shutdown, remote_Data.fix_high);
    SI24R1_TxPacket(transmit_buffer);      // 发送数据
    SI24R1_RX_Mode();                      // 切换到接收模式，等待接收数据
}
