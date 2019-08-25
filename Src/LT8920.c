/******************** (C) COPYRIGHT  欣易电子工作室 ****************************
 * 文 件 名   ：LT8920.c
 * 描 述      ：LT8920驱动函数
 * 实 验 平台 ：STM8开发板
 * 寄存器版本 ：V1.0.0
 * 作者       ：rocfan        
 * 淘宝       ：https://shop64537643.taobao.com/
 *修改时间    ：2016-11-1
*******************************************************************************/
#include "LT8920.h"
#include "spi.h"
//#include "delay.h"
unsigned char RegH;
unsigned char RegL;  //Used to store the Registor Value which is read.

void SPI_WriteReg(unsigned char addr, unsigned char H, unsigned char L);
void SPI_ReadReg(unsigned char addr);
/*******************************************************************************
*函 数 名: SPI_FLASH_Init
*功能说明: 使能LT8920芯片的片选引脚CS--PC4  RST--PA3 PKT--PC3
*形    参：无
*返 回 值: 无
*******************************************************************************/
void SPI_LT8920_Init()
{ 
	GPIO_InitTypeDef  gpioinitstruct;

  /* LCD_CS_GPIO and LCD_DC_GPIO Periph clock enable */

  __HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOF_CLK_ENABLE();
  
  
  /* Configure LCD_CS_PIN pin: LCD Card CS pin */
  gpioinitstruct.Pin    = GPIO_PIN_4;
  gpioinitstruct.Mode   = GPIO_MODE_OUTPUT_PP;
  gpioinitstruct.Pull   = GPIO_NOPULL;
  gpioinitstruct.Speed  = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOA, &gpioinitstruct);

  /* Configure LCD_DC_PIN pin: LCD Card DC pin */
  gpioinitstruct.Pin    = GPIO_PIN_0;
  HAL_GPIO_Init(GPIOF, &gpioinitstruct);
	
  LT8920_CS_HIGH();
	LT8920_RST_HIGH();
#if 0	
  PC_DDR &= 0xf7;          /* 设置PKT发送接收状态寄存器为输入引脚 */
  PC_CR1 |= (1<<3);        /* 设置上拉输入 */  
  PC_DDR |= (1<<4);        /* 设置CS为输出引脚 */
  PC_CR1 |= (1<<4);        /* 设置推挽输出--查看STM8寄存器.pdf P88 */
  PA_DDR |= (1<<3);        /* 设置为RST输出引脚 */
  PA_CR1 |= (1<<3);        /* 设置推挽输出--查看STM8寄存器.pdf P88 */
  LT8920_CS_HIGH();        /* 初始化LT8920芯片片选引脚所接的I/O引脚，并输出高电平 */
#endif
}
/*******************************************************************************
*函 数 名: LT8900_Init
*功能说明: LT8920芯片初始化函数
*形    参：无
*返 回 值: 无
*******************************************************************************/
void LT8900_Init(void)
{
//SPI_LT8920_Init();
  LT8920_RST_LOW();
	HAL_Delay(2);
	LT8920_RST_HIGH();
	HAL_Delay(5);
//  LT8920_PKT_OUT =1;
	SPI_WriteReg( 0, 0x6f, 0xef );
	SPI_WriteReg( 1, 0x56, 0x81 );
	SPI_WriteReg( 2, 0x66, 0x17 );
	SPI_WriteReg( 4, 0x9c, 0xc9 );
	SPI_WriteReg( 5, 0x66, 0x37 );
	SPI_WriteReg( 7, 0x00, 0x00 );							  //channel is 2402Mhz
	SPI_WriteReg( 8, 0x6c, 0x90 );
	SPI_WriteReg( 9, 0x48, 0x00 );			  				//PA -12.2dbm
	SPI_WriteReg(10, 0x7f, 0xfd );
	SPI_WriteReg(11, 0x00, 0x08 );
	SPI_WriteReg(12, 0x00, 0x00 );
	SPI_WriteReg(13, 0x48, 0xbd );
	SPI_WriteReg(22, 0x00, 0xff );
	SPI_WriteReg(23, 0x80, 0x05 );
	SPI_WriteReg(24, 0x00, 0x67 );
	SPI_WriteReg(25, 0x16, 0x59 );
	SPI_WriteReg(26, 0x19, 0xe0 );
	SPI_WriteReg(27, 0x13, 0x00 );
	SPI_WriteReg(28, 0x18, 0x00 );
	SPI_WriteReg(32, 0x48, 0x00 );      //8920在62.5kbps下同步头只能是32或16bit
	SPI_WriteReg(33, 0x3f, 0xc7 );
	SPI_WriteReg(34, 0x20, 0x00 );
	SPI_WriteReg(35, 0x05, 0x00 );			//重发次数4次
//#ifdef SYNC_TEST	
	SPI_WriteReg(36, 0x03, 0x80 );
	SPI_WriteReg(37, 0x03, 0x80 );
	SPI_WriteReg(38, 0x5a, 0x5a );
	SPI_WriteReg(39, 0x03, 0x80 );
//#else	
	//SPI_WriteReg(36, 0x03, 0x80 );
	//SPI_WriteReg(37, 0x03, 0x80 );
	//SPI_WriteReg(38, 0x5a, 0x5a );
	//SPI_WriteReg(39, 0x03, 0x80 );
//#endif
	SPI_WriteReg(40, 0x44, 0x02 );
	SPI_WriteReg(41, 0xb8, 0x00 );	  /*CRC is ON; scramble is OFF; AUTO_ACK is ON*/
	SPI_WriteReg(42, 0xfd, 0xb0 );		/*等待RX_ACK时间 176us*/
	SPI_WriteReg(43, 0x00, 0x0f );					
	SPI_WriteReg(44, 0x10, 0x00 );
	SPI_WriteReg(45, 0x05, 0x52 );		 //62.5k
	SPI_WriteReg(50, 0x00, 0x00 );
  HAL_Delay(100);
}
/*******************************************************************************
*函 数 名: SPI_WriteReg
*功能说明: SPI写16位寄存器
*形    参：addr地址 H高字节 L低字节
*返 回 值: 无
*******************************************************************************/
void SPI_WriteReg(unsigned char addr, unsigned char H, unsigned char L)
{
	LT8920_CS_LOW();
  delay_us(1);
  SPI_SendByte(addr);
  delay_us(1);
  SPI_SendByte(H);
  delay_us(1);
  SPI_SendByte(L);
  delay_us(1);
	LT8920_CS_HIGH();
  delay_us(1);
}
/*******************************************************************************
*函 数 名: SPI_ReadReg
*功能说明: SPI读8位寄存器
*形    参：addr要读取的寄存器地址值
*返 回 值: 无
*******************************************************************************/
void SPI_ReadReg(unsigned char addr)
{	
	LT8920_CS_LOW();
  delay_us(1);
  addr |= 0x80;                    //when reading a Register,the Address should be added with 0x80.
  SPI_SendByte(addr);
  delay_us(1);
  RegH = SPI_SendByte(Dummy_Byte);
  delay_us(1);
  RegL = SPI_SendByte(Dummy_Byte);
  delay_us(1);
	LT8920_CS_HIGH();
  delay_us(1);
}
/********************** (C) COPYRIGHT 欣易电子工作室工作室 *****END OF FILE****/