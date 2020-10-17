#ifndef __SERIAL_BOOT_H
#define __SERIAL_BOOT_H
#include <stdint.h>
//未初始化
#define FDILink_Status_Uninitialized 		0
//运行
#define FDILink_Status_Running 				2
//正忙
#define FDILink_Status_Busy 				3

#define FDILink_Frame_Start					0
#define FDILink_Frame_CMD					1
#define FDILink_Frame_Length				2
#define FDILink_Frame_SerialNumber			3
#define FDILink_Frame_CRC8					4
#define FDILink_Frame_CRC16H				5
#define FDILink_Frame_CRC16L				6
#define FDILink_Frame_Data					7
#define FDILink_Frame_End					8
typedef struct
{
	uint8_t Start;
	uint8_t CMD;
	uint8_t Length;
	uint8_t SerialNumber;
	uint8_t CRC8;
	union
	{
		uint16_t CRC16;
		struct
		{
			uint8_t CRC16L;
			uint8_t CRC16H;
		};
	};
	uint8_t Data[0];
}__attribute__((packed)) FDILink_Frame_t;

#define Storage_Type_Only_Code		0x0800
#define Storage_Type_Only_Data		0x0400
#define Storage_Type_Both			0x0000

#define Storage_Type_NONE			0x0000
#define Storage_Type_NOR_FLASH		0x0010
#define Storage_Type_NAND_FLASH		0x0020
#define Storage_Type_SRAM			0x0030
#define Storage_Type_DRAM			0x0040
#define Storage_Type_EEPROM			0x0050
#define Storage_Type_FRAM			0x0060
#define Storage_Type_ROM			0x0070
#define Storage_Type_UNKOWN			0x0080

#define Storage_Type_ExNOR_FLASH	0x8010
#define Storage_Type_ExNAND_FLASH	0x8020
#define Storage_Type_ExSRAM			0x8030
#define Storage_Type_ExDRAM			0x8040
#define Storage_Type_ExEEPROM		0x8050
#define Storage_Type_ExFRAM			0x8060
#define Storage_Type_ExUNKOWN		0x8080



typedef struct{
	uint32_t		Address;
	uint32_t		Length;
	uint32_t		Type;
}Storage_Info_t;



typedef struct{
	uint32_t		Address;
	uint32_t		Length;
	int32_t			Checked;
	uint32_t		Version;
}Firmware_Info_t;


typedef struct
{
	uint32_t 		Version_List_ID;			//列表版本ID
	uint32_t		Board_ID;					//PCB的ID
	uint32_t		Boot_Address;				//单位字节
	uint32_t		Boot_Length;				//单位字节
	uint32_t		APP_Entry;					//应用程序入口
	uint32_t		Chip_ID;					//芯片缩略ID
	uint32_t		Full_ID[4];
	Firmware_Info_t	Firmware[3];				//固件区
	Storage_Info_t	Storage[4];					//存储器
}__attribute__((packed)) FDILink_Version_t;
/***************************************************************
 *	下行指令
 **************************************************************/
#define FDILink_FrameCMD_Version			0x0c	//	获得版本
#define FDILink_FrameCMD_Keep 				0x0d	//	保持连接
#define FDILink_FrameCMD_Unlock 			0x0e	//	解锁
#define FDILink_FrameCMD_Erasure 			0x0f	//	擦除
#define FDILink_FrameCMD_Download 			0x10	//	下载
#define FDILink_FrameCMD_Upload 			0x11	//	上传
#define FDILink_FrameCMD_Verify 			0x12	//	存储校验
#define FDILink_FrameCMD_Reset 				0x13	//	跳转
#define FDILink_FrameCMD_Firmware			0x14	//	获得固件信息
#define FDILink_FrameCMD_Download_Message 	0x16	//	下载

/***************************************************************
 *	上行指令
 **************************************************************/
#define FDILink_FrameCMD_Catalog 			0x40	//	目录
#define FDILink_FrameCMD_Status 			0x41	//	当前状态
#define FDILink_FrameCMD_Error 				0x42	//	错误

#define FDILink_StatusType_Reconnecting		0x01
#define FDILink_StatusType_Processing		0x02
#define FDILink_StatusType_Idle				0x03
#define FDILink_StatusType_Break			0x04

#define FDILink_Return_OK					0
#define FDILink_Error_InvalidStart			-10	//无效的Start符
#define FDILink_Error_InvalidStatus			-11	//无效的状态
#define FDILink_Error_CRC8					-12	//错误的CRC8
#define FDILink_Error_CRC16					-13	//错误的CRC16

/***************************************************************
*	帧符号
***************************************************************/

#define FDILink_STX_Flag 0xFC
#define FDILink_EDX_Flag 0xFD
#define FDILink_Connect_Flag 0xFD

typedef struct FDILink_Status
{
	int 				BootStatus;
	int					RxStatus;
	int 				RxType;
	int 				RxDataLeft;
	int		 			RxNumber;
	int		 			TxNumber;
	int 				CRC8_Verify;
	int 				CRC16_Verify;
	uint32_t 			BufferIndex;
	uint8_t 			FDILink_Frame_Buffer[12];
	uint8_t 			Buffer[256];
}FDILink_Status_t;


int FDILink_Init(FDILink_Status_t* FDILink);
int FDILink_Send(FDILink_Status_t* FDILink, uint8_t type, uint8_t * buf, int len);
int FDILink_Recv(FDILink_Status_t* FDILink, uint8_t * buf, int len);

//定义串口发送函数
//格式 int Serial_Send(void* buffer,int length)
#define Serial_Send(...) 0

#endif
