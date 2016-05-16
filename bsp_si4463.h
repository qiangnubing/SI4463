#ifndef __BSP_SI4463_H__
#define __BSP_SI4463_H__

#include "user_config.h"

//SI4463传输包长
#define BSP_SI4463_BUFF_LEN		48

void BSP_SI4463_PowerOnReset(void);
bool BSP_SI4463_GetINFO(void);
void BSP_SI4463_LoadRadioConfig(void);
void BSP_SI4463_StartRX(void);

void BSP_SI4463_SendPacket(u8 *p);
u8 BSP_SI4463_RXCheck(u8 *p);
u8 BSP_SI4463_GetRSSI(u8 latch);


//========================== API Commands 指令 ==========================
#define s46CMD_POWER_UP_B0_CMD                              0x02    // 设备上电和模式选择
#define s46CMD_NOP_CMD                                      0x00    // 空操作
#define s46CMD_PART_INFO_B0_CMD                             0x01    // 获取设备的基本信息
#define s46CMD_FUNC_INFO_B0_CMD                             0x10    // 获取设备的功能版本信息
#define s46CMD_SET_PROPERTY_B0_CMD                          0x11    // 设置属性
#define s46CMD_GPIO_PIN_CFG_B0_CMD                          0x13    // GPIO引脚配置
#define s46CMD_GET_ADC_READING_B0_CMD                       0x14    // 获取ADC转换结果
#define s46CMD_FIFO_INFO_B0_CMD                             0x15    // 获取FIFO计数和复位
#define s46CMD_PACKET_INFO_B0_CMD                           0x16    // 返回最后接收的数据包信息和覆盖字段长度
#define s46CMD_IRCAL_B0_CMD                                 0x17    // 镜像抑制校准
#define s46CMD_PROTOCOL_CFG_B0_CMD                          0x18    // 设置芯片到指定协议
#define s46CMD_IRCAL_MANUAL_B0_CMD                          0x17    // 镜像抑制校准
#define s46CMD_GET_INT_STATUS_B0_CMD                        0x20    // 获取中断状态
#define s46CMD_GET_PH_STATUS_B0_CMD                         0x21    // 获取包处理器状态
#define s46CMD_GET_MODEM_STATUS_B0_CMD                      0x22    // 获取调制解调器状态
#define s46CMD_GET_CHIP_STATUS_B0_CMD                       0x23    // 获取芯片状态
#define s46CMD_START_TX_B0_CMD                              0x31    // 切换到TX状态并发送数据包
#define s46CMD_START_RX_B0_CMD                              0x32    // 切换到RX状态
#define s46CMD_REQUEST_DEVICE_STATE_B0_CMD                  0x33    // 获取当前设备状态通道
#define s46CMD_CHANGE_STATE_B0_CMD                          0x34    // 改变当前设备状态
#define s46CMD_RX_HOP_B0_CMD                                0x36    // RX跳频
#define s46CMD_TX_HOP_B0_CMD                                0x37    // TX跳频
#define s46CMD_READ_CMD_BUFF_B0_CMD                         0x44    // 读取CTS和指令响应
#define s46CMD_FRR_A_READ_B0_CMD                            0x50    // 读取从A开始的快速响应寄存器
#define s46CMD_FRR_B_READ_B0_CMD                            0x51    // 读取从B开始的快速响应寄存器
#define s46CMD_FRR_C_READ_B0_CMD                            0x53    // 读取从C开始的快速响应寄存器
#define s46CMD_FRR_D_READ_B0_CMD                            0x57    // 读取从C开始的快速响应寄存器
#define s46CMD_WRITE_TX_FIFO_B0_CMD                         0x66    // 写入TX_FIFO
#define s46CMD_READ_RX_FIFO_B0_CMD                          0x77    // 读取RX_FIFO

#define s46CFP_FIFO_INFO_B1_RX_FIFO_RESET                   1<<1    // 复位RX_FIFO
#define s46CFP_FIFO_INFO_B1_TX_FIFO_RESET                   1       // 复位TX_FIFO

#define s46RIN_GET_INT_STATUS_B0_CTS                        0       // CTS应答
#define s46RIN_GET_INT_STATUS_B1_INT_PEND                   1       // 
#define s46RIN_GET_INT_STATUS_B2_INT_STATUS                 2       // 
#define s46RIN_GET_INT_STATUS_B3_PH_PEND                    3       // 
#define s46RIN_GET_INT_STATUS_B4_PH_STATUS                  4       // 
#define s46RIN_GET_INT_STATUS_B5_MODEM_PEND                 5       // 
#define s46RIN_GET_INT_STATUS_B6_MODEM_STATUS               6       // 
#define s46RIN_GET_INT_STATUS_B7_CHIP_PEND                  7       // 
#define s46RIN_GET_INT_STATUS_B8_CHIP_STATUS                8       // 

#define s46RFP_GET_INT_STATUS_B4_FILTER_MATCH               1<<7    // FILTER_MATCH中断产生
#define s46RFP_GET_INT_STATUS_B4_FILTER_MISS                1<<6    // FILTER_MISS中断产生
#define s46RFP_GET_INT_STATUS_B4_PACKET_SENT                1<<5    // PACKET_SENT中断产生
#define s46RFP_GET_INT_STATUS_B4_PACKET_RX                  1<<4    // PACKET_RX中断产生
#define s46RFP_GET_INT_STATUS_B4_CRC_ERROR                  1<<3    // CRC_ERROR中断产生
#define s46RFP_GET_INT_STATUS_B4_ALT_CRC_ERROR              1<<2    // ALT_CRC_ERROR中断产生
#define s46RFP_GET_INT_STATUS_B4_TX_FIFO_ALMOST_EMPTY       1<<1    // TX_FIFO_ALMOST_EMPTY中断产生
#define s46RFP_GET_INT_STATUS_B4_RX_FIFO_ALMOST_FULL        1<<0    // RX_FIFO_ALMOST_FULL中断产生

#define s46RIN_GET_MODEM_STATUS_B3_CURR_RSSI                3       // 从调制解调器中获取当前RSSI值
#define s46RIN_GET_MODEM_STATUS_B4_LATCH_RSSI               4       // 从调制解调器中获取锁存RSSI值

#define s46RFP_START_TX_B2_NOCHANGE                         0<<4    // 不改变状态
#define s46RFP_START_TX_B2_SLEEP                            1<<4    // 休眠状态
#define s46RFP_START_TX_B2_SPI_ACTIVE                       2<<4    // SPI状态
#define s46RFP_START_TX_B2_READY                            3<<4    // 准备状态
#define s46RFP_START_TX_B2_READY2                           4<<4    // 准备状态2
#define s46RFP_START_TX_B2_TX_TUNE                          5<<4    // TX准备状态
#define s46RFP_START_TX_B2_RX_TUNE                          6<<4    // RX准备状态
#define s46RFP_START_TX_B2_RESERVED                         7<<4    // 保留(未使用)
#define s46RFP_START_TX_B2_RX                               8<<4    // RX状态
#define s46RFP_START_TX_B2_USE                              0<<3    // 更新参数并进入TX模式
#define s46RFP_START_TX_B2_UPDATE                           1<<3    // 更新参数但不进入TX模式
#define s46RFP_START_TX_B2_TRANSMIT_FIFO                    0<<2    // 发送FIFO中的数据(下溢时产生中断)
#define s46RFP_START_TX_B2_RETRANSMIT                       1<<2    // 重发上一次的数据包(不要向FIFO中写入新的数据)
#define s46RFP_START_TX_B2_IMMEDIATE                        0       // 立即发送
#define s46RFP_START_TX_B2_WUT                              1       // 等待唤醒定时器到期

#define s46RFP_START_RX_B2_USE                              0<<3    // 更新参数并进入RX模式
#define s46RFP_START_RX_B2_UPDATE                           1<<3    // 更新参数但不进入RX模式
#define s46CFP_START_RX_B2_IMMEDIATE                        0       // 立即接收
#define s46CFP_START_RX_B2_WUT                              1       // 等待唤醒定时器到期

#define s46CFP_START_RX_B5B7_NO_CHANGE                      0       // 不改变状态
#define s46CFP_START_RX_B5B7_SLEEP                          1       // 休眠状态
#define s46CFP_START_RX_B5B7_SPI_ACTIVE                     2       // SPI状态
#define s46CFP_START_RX_B5B7_READY                          3       // 准备状态
#define s46CFP_START_RX_B5B7_READY2                         4       // 另一个准备状态
#define s46CFP_START_RX_B5B7_TX_TUNE                        5       // TX准备状态
#define s46CFP_START_RX_B5B7_RX_TUNE                        6       // RX准备状态
#define s46CFP_START_RX_B5B7_TX                             7       // TX状态
#define s46CFP_START_RX_B5B7_RX                             8       // RX状态
#define s46CFP_START_RX_B5_RX_IDLE                          9       // RX_IDLE状态

#define s46CFP_CHANGE_STATE_B1_NOCHANGE                     0       // 不改变状态
#define s46CFP_CHANGE_STATE_B1_SLEEP                        1       // 休眠状态
#define s46CFP_CHANGE_STATE_B1_SPI_ACTIVE                   2       // SPI状态
#define s46CFP_CHANGE_STATE_B1_READY                        3       // 准备状态
#define s46CFP_CHANGE_STATE_B1_TX_TUNE                      5       // TX准备状态
#define s46CFP_CHANGE_STATE_B1_RX_TUNE                      6       // RX准备状态
#define s46CFP_CHANGE_STATE_B1_TX                           7       // TX状态
#define s46CFP_CHANGE_STATE_B1_RX                           8       // RX状态

#define s46PRT_INT_CTL_PH_ENABLE_GROUP                      0x01    // 分组
#define s46PRT_INT_CTL_PH_ENABLE_INDEX                      0x01    // 编号

#define s46PFS_INT_CTL_PH_ENABLE_FILTER_MATCH_DI            0<<7    // bit7=0=禁用FILTER_MATCH中断(默认)
#define s46PFS_INT_CTL_PH_ENABLE_FILTER_MATCH_EN            1<<7    // bit7=1=使能FILTER_MATCH中断
#define s46PFS_INT_CTL_PH_ENABLE_FILTER_MISS_DI             0<<6    // bit6=0=禁用FILTER_MISS中断(默认)
#define s46PFS_INT_CTL_PH_ENABLE_FILTER_MISS_EN             1<<6    // bit6=1=使能FILTER_MISS中断
#define s46PFS_INT_CTL_PH_ENABLE_PACKET_SENT_DI             0<<5    // bit5=0=禁用PACKET_SENT中断(默认)
#define s46PFS_INT_CTL_PH_ENABLE_PACKET_SENT_EN             1<<5    // bit5=1=使能PACKET_SENT中断
#define s46PFS_INT_CTL_PH_ENABLE_PACKET_RX_DI               0<<4    // bit4=0=禁用PACKET_RX中断(默认)
#define s46PFS_INT_CTL_PH_ENABLE_PACKET_RX_EN               1<<4    // bit4=1=使能PACKET_RX中断
#define s46PFS_INT_CTL_PH_ENABLE_CRC_ERROR_DI               0<<3    // bit3=0=禁用CRC_ERROR中断(默认)
#define s46PFS_INT_CTL_PH_ENABLE_CRC_ERROR_EN               1<<3    // bit3=1=使能CRC_ERROR中断
#define s46PFS_INT_CTL_PH_ENABLE_ALT_CRC_ERROR_DI           0<<2    // bit2=0=禁用ALT_CRC_ERROR中断(默认)
#define s46PFS_INT_CTL_PH_ENABLE_ALT_CRC_ERROR_EN           1<<2    // bit2=1=使能ALT_CRC_ERROR中断
#define s46PFS_INT_CTL_PH_ENABLE_TX_FIFO_EMPTY_DI           0<<1    // bit1=0=禁用TX_FIFO_ALMOST_EMPTY_EN中断(默认)
#define s46PFS_INT_CTL_PH_ENABLE_TX_FIFO_EMPTY_EN           1<<1    // bit1=1=使能TX_FIFO_ALMOST_EMPTY_EN中断
#define s46PFS_INT_CTL_PH_ENABLE_RX_FIFO_FULL_DI            0       // bit0=0=禁用RX_FIFO_ALMOST_FULL_EN中断(默认)
#define s46PFS_INT_CTL_PH_ENABLE_RX_FIFO_FULL_EN            1       // bit0=1=使能RX_FIFO_ALMOST_FULL_EN中断




#endif
