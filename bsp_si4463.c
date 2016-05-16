#include "bsp_si4463.h"
#include "hal_spi1.h"
#include "radio_config_Si4463.h"
/*
需要移植的代码：
*/
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*!<
	是否适用GPIO0 1
	0-不使用
	1-使用
*/
#define USE_GPIO		0

//接口函数-毫秒级延时函数
#define BSP_SI4463_DelayMs_Port(ms)		CoTickDelay(ms)

//接口函数-SI4463 SPI读写一个字节
#define BSP_SI4463_ReadWrite_Port(data)		HAL_SPI1_ReadWriteByte(data)

//接口函数-SI4463 片选使能
#define BSP_SI4463_NSS_EN		GPIO_ResetBits(GPIOC, GPIO_Pin_5)
//接口函数-SI4463 片选失能
#define BSP_SI4463_NSS_DN		GPIO_SetBits(GPIOC, GPIO_Pin_5)

//接口函数-SI4463 读取IRQ引脚状态
#define BSP_SI4463_IRQ_R		GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_11)

#if USE_GPIO == 1
	#define BSP_SI4463_GPIO0_R	1
	#define BSP_SI4463_GPIO1_R	1
#endif

//接口函数-SI4463 IO口初始化
void BSP_SI4463_Init_Port(void)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	
	//NSS引脚 PC5
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;		//输出
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	//推挽
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;		//上拉
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
//	BSP_SI4463_NSS_DN;
	
	//IRQ引脚 PE11
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;		//输入
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;	//开漏
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;		//上拉
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	
#if USE_GPIO == 1
	__NOP();	//初始化GPIO0 1 连接的IO口
#endif
	
	HAL_SPI1_Init();	//SCLK稳定为低电平且上升沿所存数据
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//bool BSP_SI4463_CheckCTS(void)
//{
//	BSP_SI4463_NSS_EN;
//	BSP_SI4463_ReadWrite_Port(0x00);	//写入指令字

//	if(0xFF == BSP_SI4463_ReadWrite_Port(0x00))	//读取寄存器数值
//	{
//		BSP_SI4463_NSS_DN;
//		return true;
//	}
//	else 
//	{
//		BSP_SI4463_NSS_DN;
//		return false;
//	}
//}

/*
 ************************************************************
 *  名称：	BSP_SI4463_WriteCMD()
 *  功能：	向芯片中写入指令
 *	输入：  cmd    指令字
						plen   写入参数数量(写入属性时最多为12个参数)
						*p     写入参数存放地址
						loopcts 0=忽略CTS检测; 1=检测CTS
 *	输出：  无
 ************************************************************
*/
void BSP_SI4463_WriteCMD(u8 cmd, u16 plen, u8 *p, u8 loopcts)
{	
	if(loopcts == 1)
	{
#if USE_GPIO == 1
		//等待CTS为高(执行完成)
		while(BSP_SI4463_GPIO1_R == 0)
		{
			BSP_SI4463_DelayMs_Port(10);
		}
#else
		BSP_SI4463_DelayMs_Port(10);
#endif
	}    

	BSP_SI4463_NSS_EN;								//nSEL置低,开始数据传输过程
	BSP_SI4463_ReadWrite_Port(cmd);		//写入指令字
	//写入后续数据
	for(u8 i=0; i<plen; i++)
	{
		BSP_SI4463_ReadWrite_Port(p[i]);    
	}
	BSP_SI4463_NSS_DN;	//nSEL置高,结束数据传输过程
}

/*
 ************************************************************
 *  名称：	BSP_SI4463_ReadCMD()
 *  功能：	从芯片中读取指令响应
 *	输入：  cmd    指令字
						plen   读取响应数量(读取属性时最多为16个参数)
						*p     写入参数存放地址
						loopcts 0=忽略CTS检测; 1=检测CTS
 *	输出：  CTS响应值
 ************************************************************
*/
u8 BSP_SI4463_ReadCMD(u8 cmd, u8 len, u8 *p, u8 loopcts)
{
	if(loopcts == 1)
	{
#if USE_GPIO == 1
		//等待CTS为高(执行完成)
		while(BSP_SI4463_GPIO1_R == 0)
		{
			BSP_SI4463_DelayMs_Port(10);
		}
#else
		BSP_SI4463_DelayMs_Port(10);
#endif
	}

	BSP_SI4463_NSS_EN;                               
	BSP_SI4463_ReadWrite_Port(cmd);	//写入指令字
	for(u8 i=0; i<=len; i++)
	{
		p[i] = BSP_SI4463_ReadWrite_Port(0);	//读取寄存器数值
	}
	BSP_SI4463_NSS_DN;
	
	return(p[0]);		//返回CTS
}


/*
 ************************************************************
 *  名称：	BSP_SI4463_PowerOnReset()
 *  功能：	芯片上电后的复位
 *	输入：  无
 *	输出：  无
 ************************************************************
*/
void BSP_SI4463_PowerOnReset(void)
{
	BSP_SI4463_Init_Port();
	//...//拉高CTS
//	BSP_SI4463_NSS_DN;	//SDN=1=进入掉电模式
//	BSP_SI4463_DelayMs_Port(10);	//等待1ms
//	BSP_SI4463_NSS_EN;	//SDN=0=进入上电模式
	
	BSP_SI4463_DelayMs_Port(100);
}

bool BSP_SI4463_GetINFO(void)
{
	u8 buff[9];
	
	BSP_SI4463_ReadCMD(s46CMD_PART_INFO_B0_CMD, 9, buff, 1);    //读取CTS和指令响应
	
	if(buff[0] == 0xFF)
	{
		return true;
	}
	else
	{
		return false;
	}
}
/*
 ************************************************************
 *  名称：	BSP_SI4463_LoadRadioConfig()
 *  功能：	将WDS生成的配置列表写入芯片中
 *	输入：  无
 *	输出：  无
 ************************************************************
*/
u8 si4463_set[] = RADIO_CONFIGURATION_DATA_ARRAY;
void BSP_SI4463_LoadRadioConfig(void)
{
  u16 i = 0;

	do
	{
		BSP_SI4463_WriteCMD(si4463_set[i+1], si4463_set[i]-1, &si4463_set[i+2], 1);	//写入配置列表中的数据
		i = i + si4463_set[i] + 1;		//指向下一组数据
	}
	while (si4463_set[i] != 0x00);
}


/*
 ************************************************************
 *  名称：	BSP_SI4463_ClearInterruptStatus()
 *  功能：	清除全部中断,并返回数据包处理器中断状态
 *	输入：  无
 *	输出：  数据包处理器中断状态
 ************************************************************
*/
u8 BSP_SI4463_ClearInterruptStatus(void)
{
	u8 buff[9];

	buff[0] = 0;
	buff[1] = 0;
	buff[2] = 0;
	
	BSP_SI4463_WriteCMD(s46CMD_GET_INT_STATUS_B0_CMD, 3, buff, 1);	//写入返回中断状态指令
	BSP_SI4463_ReadCMD(s46CMD_READ_CMD_BUFF_B0_CMD, 9, buff, 1);    //读取CTS和指令响应
	
	return	(buff[s46RIN_GET_INT_STATUS_B3_PH_PEND]);	//返回数据包处理器中断状态
}



/*
 ************************************************************
 *  名称：	BSP_SI4463_StartRX()
 *  功能：	进入接收状态
 *	输入：  无
 *	输出：  无
 ************************************************************
*/
void BSP_SI4463_StartRX(void)
{
	u8 buff[7];
    
	BSP_SI4463_ClearInterruptStatus();															//清除全部中断

	buff[0] = s46CFP_CHANGE_STATE_B1_READY;                         // 进入READY模式
	BSP_SI4463_WriteCMD(s46CMD_CHANGE_STATE_B0_CMD, 1, buff, 1);    // 发送指令

	buff[0] = s46PRT_INT_CTL_PH_ENABLE_GROUP;                       // 属性分组=包处理器中断使能
	buff[1] = 1;                                                    // 属性数量=1
	buff[2] = s46PRT_INT_CTL_PH_ENABLE_INDEX;                       // 起始地址=PH_ENABLE
	buff[3] = s46PFS_INT_CTL_PH_ENABLE_PACKET_RX_EN                 // 使能PACKET_RX中断
					| s46PFS_INT_CTL_PH_ENABLE_CRC_ERROR_EN;                // 使能CRC_ERROR中断
	BSP_SI4463_WriteCMD(s46CMD_SET_PROPERTY_B0_CMD, 4, buff, 1);    // 写入包处理器中断使能属性

	buff[0] = RADIO_CONFIGURATION_DATA_CHANNEL_NUMBER;              // CHANNEL
	buff[1] = s46RFP_START_RX_B2_USE                                // 更新参数并进入RX模式
					| s46CFP_START_RX_B2_IMMEDIATE;                         // 立即接收
	buff[2] = (u8)(BSP_SI4463_BUFF_LEN >> 8);                       // RX_LEN bit12:8
	buff[3] = (u8)(BSP_SI4463_BUFF_LEN);                            // TX_LEN bit7:0
	buff[4] = s46CFP_START_RX_B5B7_NO_CHANGE;                       // 前导检测超时时自动进入的状态                            
	buff[5] = s46CFP_START_RX_B5B7_READY;                           // 接收到有效数据包时自动进入的状态
	buff[6] = s46CFP_START_RX_B5B7_READY;                           // 接收到无效数据包时自动进入的状态
	BSP_SI4463_WriteCMD(s46CMD_START_RX_B0_CMD, 7, buff, 1);        // 启动接收
}


/*
 ************************************************************
 *  名称：	BSP_SI4463_SendPacket()
 *  功能：	发送数据包
 *	输入：  *p     发送数据的首地址
						len    发送数据的长度
 *	输出：  无
 ************************************************************
*/
void BSP_SI4463_SendPacket(u8 *p)
{
	u8 buff[8];

	
	while(BSP_SI4463_IRQ_R != 1)				//等待nIRQ为高(中断已清除)
	{
		BSP_SI4463_ClearInterruptStatus();	//清除全部中断
		BSP_SI4463_DelayMs_Port(10);
	}
	
	BSP_SI4463_WriteCMD(s46CMD_WRITE_TX_FIFO_B0_CMD, BSP_SI4463_BUFF_LEN, p, 0);	//将待发送数据写入TX_FIFO

	buff[0] = s46PRT_INT_CTL_PH_ENABLE_GROUP;                       //属性分组=包处理器中断使能
	buff[1] = 1;                                                    //属性数量=1
	buff[2] = s46PRT_INT_CTL_PH_ENABLE_INDEX;                       //起始地址=PH_ENABLE
	buff[3] = s46PFS_INT_CTL_PH_ENABLE_PACKET_SENT_EN;              //使能PACKET_SENT中断
	BSP_SI4463_WriteCMD(s46CMD_SET_PROPERTY_B0_CMD, 4, buff, 1);		//写入包处理器中断使能属性

	buff[0] = RADIO_CONFIGURATION_DATA_CHANNEL_NUMBER;              //CHANNEL
	buff[1] = s46RFP_START_TX_B2_NOCHANGE                           //发送结束后进入READY状态
					| s46RFP_START_TX_B2_USE                                //更新参数并进入TX模式
					| s46RFP_START_TX_B2_TRANSMIT_FIFO                      //发送FIFO中的数据(下溢时产生中断)
					| s46RFP_START_TX_B2_IMMEDIATE;                         //立即发送
	buff[2] = (u8)(BSP_SI4463_BUFF_LEN >> 8);                       //TX_LEN bit12:8
	buff[3] = (u8)(BSP_SI4463_BUFF_LEN);                            //TX_LEN bit7:0
	buff[4] = 0;                                                    //数据包之间的间隔时间
	buff[5] = 0;                                                    //数据包重复次数
	BSP_SI4463_WriteCMD(s46CMD_START_TX_B0_CMD, 6, buff, 1);        //启动发送

	while(BSP_SI4463_IRQ_R != 0)																		//等待nIRQ为低(执行完成)
	{
		BSP_SI4463_DelayMs_Port(10);
	}
	BSP_SI4463_ClearInterruptStatus();															//清除全部中断
//	BSP_SI4463_StartRX();                                           //返回RX状态
}


/*
 ************************************************************
 *  名称：	BSP_SI4463_RXCheck()
 *  功能：	检测是否接收到数据
 *	输入：  *p     接收数据存放地址
						len    发送数据的长度
 *	输出：  0无接收  1接收到有效数据
 ************************************************************
*/
u8 BSP_SI4463_RXCheck(u8 *p)
{
	u8 r=0;
	u8 buff[3];
	
	if(BSP_SI4463_IRQ_R == 0)
	{
		u8 t = BSP_SI4463_ClearInterruptStatus();    //清除全部中断,并返回数据包处理器中断状态
		if(t & s46RFP_GET_INT_STATUS_B4_PACKET_RX)	 //检查是否为数据包接收完成中断
		{                        
			BSP_SI4463_ReadCMD(s46CMD_READ_RX_FIFO_B0_CMD, BSP_SI4463_BUFF_LEN, p, 0);  //读取RX_FIFO中的数据
			r = 1;                                                          						//检测到有效数据包                                          
		}
		
		buff[0] = s46CFP_FIFO_INFO_B1_RX_FIFO_RESET;                        //复位RX_FIFO
		BSP_SI4463_WriteCMD(s46CMD_FIFO_INFO_B0_CMD, 1, buff, 1);           //发送FIFO复位指令
		BSP_SI4463_StartRX();                                               //返回RX状态
	}
	
	return	r;
}

/*
 ************************************************************
 *  名称：	BSP_SI4463_GetRSSI()
 *  功能：	获取信号强度
 *	输入：  *p     接收数据存放地址
						len    发送数据的长度
 *	输出：  0无接收  1接收到有效数据
 ************************************************************
*/
u8 BSP_SI4463_GetRSSI(u8 latch)
{
	u8 buff[9];
	
	BSP_SI4463_WriteCMD(s46CMD_GET_MODEM_STATUS_B0_CMD, 0, 0, 1);        // 写入获取调制解调器状态指令
	BSP_SI4463_ReadCMD(s46CMD_READ_CMD_BUFF_B0_CMD, 9, buff, 1);         // 读取CTS和指令响应

	if(latch == 0)
	{
		return buff[s46RIN_GET_MODEM_STATUS_B3_CURR_RSSI];          // 返回CURR_RSSI
	}
	else
	{
		return buff[s46RIN_GET_MODEM_STATUS_B4_LATCH_RSSI];         // 返回LATCH_RSSI
	}
}



