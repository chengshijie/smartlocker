/**
 * @section LICENSE
 *
 * Copyright (c) 2013 @tosihisa, MIT License
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software 
 * and associated documentation files (the "Software"), to deal in the Software without restriction, 
 * including without limitation the rights to use, copy, modify, merge, publish, distribute, 
 * sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is 
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or 
 * substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING 
 * BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND 
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, 
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, 
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * @section DESCRIPTION
 *
 * Fingerprint reader module "GT-511C3" class.
 *
 * http://www.adh-tech.com.tw/?22,gt-511c3-gt-511c31
 * http://www.adh-tech.com.tw/files/GT-511C3_datasheet_V1%201_20131127[1].pdf
 * https://www.sparkfun.com/products/11792
 * https://github.com/sparkfun/Fingerprint_Scanner-TTL/ 
 */
#include "commondef.h"
#include "ts1013m.h"
#include "usart.h"
#include "main.h"
#include "voice.h"
 
#define SET_AND_SUMADD(idx,val) sendbuf[idx]=((uint8_t)(val))

//UART_HandleTypeDef UartFpHandle;
extern uint32_t timeout_ms;

int TS1013M_Init(void)
{
	//POWER CTRL
	
	GPIO_InitTypeDef GPIO_InitStructure;
	TS1013_PWR_GPIO_CLK_ENABLE();
	GPIO_InitStructure.Pin = GPIO_PIN_1;
	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_LOW ;
	HAL_GPIO_Init(GPIOA,&GPIO_InitStructure);

	/*
	UartFpHandle.Instance        = USARTy;

  UartFpHandle.Init.BaudRate   = 115200;
  UartFpHandle.Init.WordLength = UART_WORDLENGTH_8B;
  UartFpHandle.Init.StopBits   = UART_STOPBITS_1;
  UartFpHandle.Init.Parity     = UART_PARITY_NONE;
  UartFpHandle.Init.HwFlowCtl  = UART_HWCONTROL_NONE;
  UartFpHandle.Init.Mode       = UART_MODE_TX_RX;
  UartFpHandle.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if(HAL_UARTy_DeInit(&UartFpHandle) != HAL_OK)
  {
    //Error_Handler();
		return -1;
  }  
  if(HAL_UARTy_Init(&UartFpHandle) != HAL_OK)
  {
   // Error_Handler();
		return -1;
  }*/
	MX_USART1_UART_Init();
  return 0;
}

void TS1013M_DeInit(void)
{
	HAL_UART_DeInit(&huart1);
	HAL_Delay(100);
	TS1013M_Poweroff();
}

void TS1013M_clear_buff()
{
	uint8_t ch;
//	while ((HAL_UART_GetState(&UartFpHandle) & HAL_UART_STATE_BUSY_RX) == HAL_UART_STATE_BUSY_RX)
	 if(__HAL_UART_GET_FLAG(&huart1, UART_FLAG_ORE)) {
        __HAL_UART_CLEAR_OREFLAG(&huart1);
    }
    // Check if data is received
   while ( (__HAL_UART_GET_FLAG(&huart1, UART_FLAG_RXNE) != RESET) ? 1 : 0)
		HAL_UART_Receive(&huart1, &ch, 1, 0);
}

bool TSM1013M_shakehand(uint16_t timeov)
{
	int16_t  time;
	bool result = FALSE;
	uint8_t data_buf[8];
	time=timeov;
	while(time>0) 
	{

		TS1013M_SendCommand(0,0xfe);
		if(HAL_UART_Receive(&huart1, (uint8_t *)data_buf, 8, 10) == HAL_OK)
			break;
		time-=10;
	}
	if(time <= 0)
		return FALSE;
	TS1013M_clear_buff();
	TS1013M_SendCommand(0,0xfd);
	time=30;
	while(time--)
	{
		if ((HAL_UART_Receive(&huart1, (uint8_t *)data_buf, 8, 1) == HAL_OK) && (data_buf[1]==0xfd) && (data_buf[4]==0x01))
			break;
	}
	if(time)
		return TRUE; 
	else
		return FALSE; 
}

void TS1013M_Poweron()
{
	TS1013_PWR_0;
}

void TS1013M_Poweroff()
{
	TS1013_PWR_1;
}
 
int TS1013M_SendCommand(uint32_t Parameter,uint8_t Command)
{
    uint8_t sendbuf[8];
    uint16_t sum = 0;
    int idx = 0;
    int i;
		//HAL_UART_Transmit(&UartFpHandle, (uint8_t*)sendbuf, 8, 100);
    SET_AND_SUMADD(idx,0xF5); idx++;
		SET_AND_SUMADD(idx,Command); idx++;
		SET_AND_SUMADD(idx,(Parameter >> 8) & 0xff); idx++;
    SET_AND_SUMADD(idx,Parameter & 0xff); idx++;
    SET_AND_SUMADD(idx,(Parameter >> 16) & 0xff); idx++;
    SET_AND_SUMADD(idx,0); idx++;
    sum = Command ^ (Parameter & 0xff) ^ ((Parameter >> 8) & 0xff) ^ ((Parameter >> 16) & 0xff);
		SET_AND_SUMADD(idx,sum); idx++;
		SET_AND_SUMADD(idx,0xF5); idx++;
	  if(HAL_UART_Transmit(&huart1, (uint8_t*)sendbuf, 8, 200)!= HAL_OK)
		{
			 return -1; 
		}
	//  printf("send: ");
  //  for(i = 0;i < idx;i++){
	//		  printf("%x ",sendbuf[i]);
      
  //  }
	//	printf("\r\n");
    return 0;
}
 
int TS1013M_RecvResponse(uint32_t *Parameter,uint8_t *Response)
{
    const uint8_t fixedbuf[1] = { 0xF5 };
    uint8_t buf[8];
    uint8_t sum = 0;
    int i;
 
    *Parameter = 0;
    *Response = 0xFF;
		if(HAL_UART_Receive(&huart1, (uint8_t *)buf, 8, 2500) != HAL_OK)
		{
			 //TS1013M_SendCommand(0,0xfe);
			 return -1;  
		}
		//TS1013M_SendCommand(0,0xfe);
		/*
		if(buf[0] != fixedbuf[0])
       return -2;
        
		for (i=1;i<6;i++)
       sum = sum ^ buf[i];
		     
		if(buf[6] != sum )
       return -2;*/
  //  printf("receive: ");    
  //  for(i = 0;i < sizeof(buf);i++)
	//		printf("%x ",buf[i]);
	//	printf("\r\n");
		
    *Parameter =  buf[1];
    *Parameter = (*Parameter << 8) | buf[2];
    *Parameter = (*Parameter << 8) | buf[3];
 
    *Response = buf[4];
    return 0;
}

bool TS1013M_Exit()
{
	/*
	uint8_t data_buf[8];
	TS1013M_clear_buff();
	TS1013M_SendCommand(0,0xfe);
	uint16_t time = timeov;
	while(time--)
	{
		//printf("time:%d",time);
		if ((HAL_UART_Receive(&UartFpHandle, (uint8_t *)data_buf, 8, 1) == HAL_OK) && (data_buf[4]==0x18))
		{
			printf("receive: ");    
			for(int i = 0;i < sizeof(data_buf);i++)
				printf("%x ",data_buf[i]);
			printf("\r\n");
			return TRUE;
		}
	}
	
	return FALSE; */
		uint32_t Parameter = 0;
    uint8_t Response = 0;
    int sts = 0;
	  TS1013M_clear_buff();
	//	HAL_UART_Receive(&UartFpHandle, &ch, 1, 0);
    sts = TS1013M_SendRecv(CMD_Cancel,&Parameter,&Response);
    if((sts == 0) && (Response == ACK_BREAK))
        return TRUE;
    return FALSE;
}
/* 
int TS1013M::RecvData(unsigned char *data,unsigned long size)
{
    const unsigned char fixedbuf[4] = { 0x5A,0xA5,0x01,0x00 };
    unsigned short sum = 0;
    int i;
 
    for(i = 0;i < size;i++){
        while(!readable());
        *(data + i) = getc();
        if(i < (size-2)){
            sum += *(data + i);
        }
        if(i < 4){
            if(*(data + i) != fixedbuf[i]){
                return -1;
            }
        }
    }
    if(*(data + size - 2) != (sum & 0xff))
        return -2;
    if(*(data + size - 1) != ((sum >> 8) & 0xff))
        return -2;
    return 0;
}*/
 
int TS1013M_SendRecv(uint8_t Command,uint32_t *Parameter,uint8_t *Response)
{
    int sts = -1;
    if((sts = TS1013M_SendCommand(*Parameter,Command)) == 0){
        *Parameter = 0;
        sts = TS1013M_RecvResponse(Parameter,Response); 
    }
    return sts;
}
 
int TS1013M_ClearLine(void)
{
   // while(serial->readable()){
    //    (void)serial->getc();
   // }
    return 0;
}
 
 /*
int TS1013M::Open(void)
{
    unsigned long Parameter = 1;
    unsigned short Response = 0;
    unsigned char buf[4+sizeof(FirmwareVersion)+sizeof(IsoAreaMaxSize)+sizeof(DeviceSerialNumber)+2];
    int sts = 0;
 
    if((sts = Init()) != 0)
        return -1;
 
    sts = SendRecv(CMD_Open,&Parameter,&Response);
    if((sts != 0) || (Response != CMD_Ack)){
        return -1;
    }
    if((sts = RecvData(buf,sizeof(buf))) == 0){
        memcpy(&FirmwareVersion,&buf[4+0],sizeof(FirmwareVersion));
        memcpy(&IsoAreaMaxSize,&buf[4+sizeof(FirmwareVersion)],sizeof(IsoAreaMaxSize));
        memcpy(DeviceSerialNumber,&buf[4+sizeof(FirmwareVersion)+sizeof(IsoAreaMaxSize)],sizeof(DeviceSerialNumber));
    }
    return sts;
}
 
int TS1013M::WaitPress(int press)
{
    while(IsPress() != press);
    return 0;
}
 
int TS1013M::CmosLed(int onoff)
{
    unsigned long Parameter = onoff & 1;
    unsigned short Response = 0;
    int sts = 0;
 
    sts = SendRecv(CMD_CmosLed,&Parameter,&Response);
    if((sts != 0) || (Response != CMD_Ack)){
        return -1;
    }
    return 0;
}
 
int TS1013M::IsPress(void)
{
    unsigned long Parameter = 0;
    unsigned short Response = 0;
    int sts = 0;
    sts = SendRecv(CMD_IsPressFinger,&Parameter,&Response);
    if((sts != 0) || (Response != CMD_Ack))
        return 0;
    if(Parameter != 0)
        return 0;
    return 1;
}
 
int TS1013M::Capture(int best)
{
    unsigned long Parameter = best;
    unsigned char Response = 0;
    int sts = 0;
 
    sts = SendRecv(CMD_CaptureFinger,&Parameter,&Response);
    if((sts != 0) || (Response != ACK_SUCCESS))
        return -1;
    return 0;
}
 
int TS1013M::Enroll_N(int N)
{
    unsigned long Parameter = 0;
    unsigned short Response = 0;
    int sts = 0;
    enum Command cmd;
 
    switch(N){
        default:
        case 1: cmd = CMD_Enroll1; break;
        case 2: cmd = CMD_Enroll2; break;
        case 3: cmd = CMD_Enroll3; break;
    }
    sts = SendRecv(cmd,&Parameter,&Response);
    if((sts != 0) || (Response != CMD_Ack))
        return -1;
    return 0;
}*/
 
int TS1013M_Identify(uint16_t* id)
{
    uint32_t Parameter = 0;
    uint8_t Response = 0;
    int sts = 0;
		TS1013M_clear_buff();
    sts = TS1013M_SendRecv(CMD_Identify,&Parameter,&Response);
    if (sts != 0) //|| ((Parameter & 0xffff) == 0))
        return -1;
		*id = Parameter & 0xffff;
    return *id;
}
 
int TS1013M_Enroll(int ID)
{
  uint32_t Parameter = (0x01 << 16) | ID;
  uint8_t Response = 0;
  int sts = 0;
	voice_play(0x17);
	HAL_Delay(2000);
	TS1013M_clear_buff();
	//TS1013M_SetMode(); //deny same
  TS1013M_SendCommand(Parameter,CMD_Enroll1);
	sts = TS1013M_RecvResponse(&Parameter,&Response); 
	if ((sts != 0) || (((Parameter >> 16) & 0xff) != 0x01) || (Response != 0x00))
		goto Enroll_fail;
	voice_play(0x18);
	HAL_Delay(2000);
	voice_play(0x17);
	HAL_Delay(2000);
	timeout_ms = 0;
	
	Parameter = 0;
	TS1013M_SendCommand(Parameter,CMD_Enroll2);
	sts = TS1013M_RecvResponse(&Parameter,&Response); 
	if ((sts != 0) || (((Parameter >> 16) & 0xff) != 0x02) || (Response != 0x00))
		goto Enroll_fail;
	voice_play(0x18);
	HAL_Delay(2000);
	voice_play(0x17);
	HAL_Delay(2000);
	timeout_ms = 0;
	Parameter = 0;
	TS1013M_SendCommand(Parameter,CMD_Enroll3);
	sts = TS1013M_RecvResponse(&Parameter,&Response); 
	if ((sts != 0) || (((Parameter >> 16) & 0xff) != 0x03) || (Response != 0x00))
		goto Enroll_fail;
	voice_play(0x18);
	HAL_Delay(2000);
	timeout_ms = 0;
    return 0;
	
	Enroll_fail:
		HAL_Delay(2000);
		return -1;
}
 
int TS1013M_CheckEnrolled(int ID)
{
    uint32_t Parameter = ID;
    uint8_t Response = 0;
    int sts = 0;
		TS1013M_clear_buff();
    sts = TS1013M_SendRecv(CMD_CheckEnrolled,&Parameter,&Response);
    if((sts == 0) && (Response == 0x01))
        return 0;  //This ID is enrolled 
    return -1;
}
 
int TS1013M_DeleteID(int ID)
{
    uint32_t Parameter = ID;
    uint8_t Response = 0;
    int sts = 0;
		TS1013M_clear_buff();
    sts = TS1013M_SendRecv(CMD_DeleteID,&Parameter,&Response);
    if((sts == 0) && (Response == ACK_SUCCESS))
        return 0;
    return -1;
}

int TS1013M_DeleteALL()
{
    uint32_t Parameter = 0;
    uint8_t Response = 0;
    int sts = 0;
		TS1013M_clear_buff();
    sts = TS1013M_SendRecv(CMD_DeleteAll,&Parameter,&Response);
    if((sts == 0) && (Response == ACK_SUCCESS))
        return 0;
    return -1;
}

int TS1013M_GetEnrollCount()
{
		uint32_t Parameter = 0;
    uint8_t Response = 0;
    int sts = 0;
	  TS1013M_clear_buff();
	//	HAL_UART_Receive(&UartFpHandle, &ch, 1, 0);
    sts = TS1013M_SendRecv(CMD_GetEnrollCount,&Parameter,&Response);
    if((sts == 0) && (Response == ACK_SUCCESS))
        return (Parameter & 0xffff);
    return -1;
	
}

int TS1013M_GetEmptyId()
{
		uint32_t Parameter = 0;
    uint8_t Response = 0;
    int sts = 0;
		TS1013M_clear_buff();
	  //HAL_UART_Receive(&UartFpHandle, &ch, 1, 0);
    sts = TS1013M_SendRecv(CMD_GetEmptyId,&Parameter,&Response);
    if((sts != 0) || (Response != 0))
        return -1;
    return (Parameter & 0xffff);
	
}

int TS1013M_SetMode()
{
		uint32_t Parameter = 0x01; //deny same
    uint8_t Response = 0;
    int sts = 0;
		TS1013M_clear_buff();
    sts = TS1013M_SendRecv(CMD_SetMode,&Parameter,&Response);
    if((sts != 0) || (Response != 0))
        return -1;
    return 0;
	
}

 