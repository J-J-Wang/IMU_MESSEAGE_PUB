#include "FDILink.h"
#include "string.h"

extern uint8_t CRC8_Table(uint8_t* p, uint8_t counter);
extern uint16_t CRC16_Table(uint8_t* p, uint8_t counter);

#ifndef FDI_ASSERT
#define FDI_ASSERT(x)
#endif

void FDILink_Error(FDILink_Status_t* FDILink)
{
	FDILink->RxStatus = FDILink_Frame_Start;
}
void FDILink_Insert(FDILink_Status_t* FDILink, uint8_t value)
{
	if (FDILink->RxDataLeft <= 0)
	{
		FDILink_Error(FDILink);
		return;
	}
	if (FDILink->RxStatus != FDILink_Frame_Data)
	{
		FDILink_Error(FDILink);
		return;
	}
	FDILink->Buffer[FDILink->BufferIndex++] = value;  //将元素放入队列尾部
	if (FDILink->BufferIndex >= 256)
	{
		FDILink_Error(FDILink);
		return;
	}
	FDILink->RxDataLeft--;
}
void FDILink_Reset(FDILink_Status_t* FDILink)
{
	FDILink->RxStatus = FDILink_Frame_Start;
	FDILink->RxDataLeft = 0;
	FDILink->RxType = 0;
	FDILink->BufferIndex = 0;
	FDILink->TxNumber = 0;
}
int FDILink_RunningData(FDILink_Status_t* FDILink, uint8_t value)
{
	if (FDILink->RxStatus < FDILink_Frame_Start || FDILink->RxStatus > FDILink_Frame_End)
	{
		FDILink_Error(FDILink);
		return -3;
	}
	FDILink->FDILink_Frame_Buffer[FDILink->RxStatus] = value;
	switch (FDILink->RxStatus)
	{
		case FDILink_Frame_Start:
			FDILink_Reset(FDILink);
			if (value == FDILink_Connect_Flag)
			{
				return 0;
			}
			if (value != FDILink_STX_Flag)
			{
				FDILink_Error(FDILink);
				return -1;
			}
			FDILink->RxStatus = FDILink_Frame_CMD;
			break;
		case FDILink_Frame_CMD:
			FDILink->RxType = value;
			FDILink->RxStatus = FDILink_Frame_Length;
			break;
		case FDILink_Frame_Length:
			FDILink->RxDataLeft = value;
			FDILink->RxStatus = FDILink_Frame_SerialNumber;
			break;
		case FDILink_Frame_SerialNumber:
			FDILink->RxNumber = value;
			FDILink->RxStatus = FDILink_Frame_CRC8;
			break;
		case FDILink_Frame_CRC8:
			FDILink->CRC8_Verify = value;
			if (CRC8_Table(FDILink->FDILink_Frame_Buffer, FDILink_Frame_CRC8) != FDILink->CRC8_Verify)
			{
				FDILink_Error(FDILink);
				return -1;
			}
			if(FDILink->RxDataLeft == 0)
			{
				FDILink->RxStatus = FDILink_Frame_Start;
				return 1;
			}
			FDILink->RxStatus = FDILink_Frame_CRC16H;
			break;
		case FDILink_Frame_CRC16H:
			FDILink->CRC16_Verify = value;
			FDILink->RxStatus = FDILink_Frame_CRC16L;
			break;
		case FDILink_Frame_CRC16L:
			FDILink->CRC16_Verify = (FDILink->CRC16_Verify << 8) | value;
			FDILink->RxStatus = FDILink_Frame_Data;
			break;
		case FDILink_Frame_Data:
			if (FDILink->RxDataLeft)
			{
				FDILink_Insert(FDILink,value);
				if (FDILink->RxDataLeft == 0)
				{
					FDILink->RxStatus = FDILink_Frame_End;
				}
				break;
			}
			else
			{
				FDILink->RxStatus = FDILink_Frame_End;
			}

		case FDILink_Frame_End:
		{
			if (value != FDILink_EDX_Flag)
			{
				FDILink_Error(FDILink);
				return -1;
			}
			uint16_t CRC16 = CRC16_Table(FDILink->Buffer, FDILink->BufferIndex);
			if (CRC16 != FDILink->CRC16_Verify)
			{
				FDILink_Error(FDILink);
				return -1;
			}
			FDILink->RxStatus = FDILink_Frame_Start;
			return 1;
		}
		default:
			FDILink_Error(FDILink);
			return -1;
	}
	return 0;
}
int FDILink_CheckData(FDILink_Status_t* FDILink, uint8_t len)
{
	if (FDILink->BufferIndex != len)
	{
		FDILink_Error(FDILink);
		return -1;
	}
	return 0;
}
extern void FDILink_Decode(int type,void* buf);
// void FDILink_Effect(FDILink_Status_t* FDILink)
// {
// 	FDILink_Decode(FDILink->RxType,FDILink->Buffer);
// }
int FDILink_Send(FDILink_Status_t* FDILink, uint8_t type, uint8_t * buf, int len)
{
	uint8_t buffer[256];
	FDI_ASSERT(len < 248);
	buffer[FDILink_Frame_Start] = FDILink_STX_Flag;
	buffer[FDILink_Frame_CMD] = type;
	buffer[FDILink_Frame_Length] = len;
	buffer[FDILink_Frame_SerialNumber] = FDILink->TxNumber++;
	uint8_t CRC8 = CRC8_Table(buffer, FDILink_Frame_CRC8);
	buffer[FDILink_Frame_CRC8] = CRC8;
	if(len == 0)
	{
		//没有CRC16校验和结束符
		return Serial_Send(buffer,FDILink_Frame_CRC8 + 1);
	}
	else
	{
		uint8_t* buf_data = buffer + FDILink_Frame_Data;
		memcpy(buf_data,buf,len);
		uint16_t CRC16 = CRC16_Table(buf_data, len);
		buffer[FDILink_Frame_CRC16H] = (CRC16 >> 8);
		buffer[FDILink_Frame_CRC16L] = (CRC16 & 0xff);
		buffer[FDILink_Frame_End + len - 1] = FDILink_EDX_Flag;
		return Serial_Send(buffer,FDILink_Frame_End + len);
	}
}

static inline int FDILink_Receive(FDILink_Status_t* FDILink, uint8_t value)
{
	FDI_ASSERT(FDILink->BootStatus == FDILink_Status_Running);
	uint8_t result = FDILink_RunningData(FDILink, value);
	// if (result == 1)
	// {
	// 	FDILink_Effect(FDILink);
	// }
	return result;
}

int FDILink_Init(FDILink_Status_t* FDILink)
{
	FDILink->BufferIndex = 0;
	FDILink->BootStatus = FDILink_Status_Running;
	FDILink->RxStatus = FDILink_Frame_Start;
	for (int i = 0; i < 256; i++)
	{
		FDILink->Buffer[i] = 0;
	}
	return 0;
}


int FDILink_Recv(FDILink_Status_t* FDILink, uint8_t * buf, int len)
{
	for(int i = 0;i < len;i++)
	{
		FDILink_Receive(FDILink, buf[i]);
	}
	return 0;
}


