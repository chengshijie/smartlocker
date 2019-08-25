/******************** (C) COPYRIGHT  ���׵��ӹ����� ****************************
 * �� �� ��   ��LT8920.h
 * �� ��      ��LT8920��������
 * ʵ �� ƽ̨ ��STM8������
 * �Ĵ����汾 ��V1.0.0
 * ����       ��rocfan        
 * �Ա�       ��https://shop64537643.taobao.com/
 *�޸�ʱ��    ��2016-11-1
*******************************************************************************/
#ifndef __LT8920_H
#define __LT8920_H


/* ѡ��LT8920оƬ */
#define LT8920_CS_LOW()       HAL_GPIO_WritePin(GPIOA,GPIO_PIN_4,GPIO_PIN_RESET)
#define LT8920_RST_LOW()      HAL_GPIO_WritePin(GPIOF,GPIO_PIN_0,GPIO_PIN_RESET)
/* ��ѡ��LT8920оƬ */
#define LT8920_CS_HIGH()      HAL_GPIO_WritePin(GPIOA,GPIO_PIN_4,GPIO_PIN_SET)
#define LT8920_RST_HIGH()     HAL_GPIO_WritePin(GPIOF,GPIO_PIN_0,GPIO_PIN_SET)
#define LT8920_PKT_IN         HAL_GPIO_ReadPin(GPIOF,GPIO_PIN_1)
//#define LT8920_PKT_OUT        PC_ODR_ODR3
//#define LED                   PB_ODR_ODR5
#define Dummy_Byte 0x00       /* д����ֽڲ��� */ 

//#define RESET          0
//#define SET            1
extern unsigned char RegH;
extern unsigned char RegL;              //Used to store the Registor Value which is read.
void LT8900_Init(void);
void SPI_LT8920_Init();
void SPI_ReadReg(unsigned char addr);
void SPI_WriteReg(unsigned char addr, unsigned char H, unsigned char L);

#endif

/********************** (C) COPYRIGHT ���׵��ӹ����ҹ����� *****END OF FILE****/