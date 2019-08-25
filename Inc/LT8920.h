/******************** (C) COPYRIGHT  欣易电子工作室 ****************************
 * 文 件 名   ：LT8920.h
 * 描 述      ：LT8920驱动函数
 * 实 验 平台 ：STM8开发板
 * 寄存器版本 ：V1.0.0
 * 作者       ：rocfan        
 * 淘宝       ：https://shop64537643.taobao.com/
 *修改时间    ：2016-11-1
*******************************************************************************/
#ifndef __LT8920_H
#define __LT8920_H


/* 选中LT8920芯片 */
#define LT8920_CS_LOW()       HAL_GPIO_WritePin(GPIOA,GPIO_PIN_4,GPIO_PIN_RESET)
#define LT8920_RST_LOW()      HAL_GPIO_WritePin(GPIOF,GPIO_PIN_0,GPIO_PIN_RESET)
/* 不选中LT8920芯片 */
#define LT8920_CS_HIGH()      HAL_GPIO_WritePin(GPIOA,GPIO_PIN_4,GPIO_PIN_SET)
#define LT8920_RST_HIGH()     HAL_GPIO_WritePin(GPIOF,GPIO_PIN_0,GPIO_PIN_SET)
#define LT8920_PKT_IN         HAL_GPIO_ReadPin(GPIOF,GPIO_PIN_1)
//#define LT8920_PKT_OUT        PC_ODR_ODR3
//#define LED                   PB_ODR_ODR5
#define Dummy_Byte 0x00       /* 写入空字节操作 */ 

//#define RESET          0
//#define SET            1
extern unsigned char RegH;
extern unsigned char RegL;              //Used to store the Registor Value which is read.
void LT8900_Init(void);
void SPI_LT8920_Init();
void SPI_ReadReg(unsigned char addr);
void SPI_WriteReg(unsigned char addr, unsigned char H, unsigned char L);

#endif

/********************** (C) COPYRIGHT 欣易电子工作室工作室 *****END OF FILE****/