/******************** (C) COPYRIGHT  ���׵��ӹ����� ****************************
 * �� �� ��   ��LT8920.c
 * �� ��      ��LT8920��������
 * ʵ �� ƽ̨ ��STM8������
 * �Ĵ����汾 ��V1.0.0
 * ����       ��rocfan        
 * �Ա�       ��https://shop64537643.taobao.com/
 *�޸�ʱ��    ��2016-11-1
*******************************************************************************/
#include "LT8920.h"
#include "spi.h"
//#include "delay.h"
unsigned char RegH;
unsigned char RegL;  //Used to store the Registor Value which is read.

void SPI_WriteReg(unsigned char addr, unsigned char H, unsigned char L);
void SPI_ReadReg(unsigned char addr);
/*******************************************************************************
*�� �� ��: SPI_FLASH_Init
*����˵��: ʹ��LT8920оƬ��Ƭѡ����CS--PC4  RST--PA3 PKT--PC3
*��    �Σ���
*�� �� ֵ: ��
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
  PC_DDR &= 0xf7;          /* ����PKT���ͽ���״̬�Ĵ���Ϊ�������� */
  PC_CR1 |= (1<<3);        /* ������������ */  
  PC_DDR |= (1<<4);        /* ����CSΪ������� */
  PC_CR1 |= (1<<4);        /* �����������--�鿴STM8�Ĵ���.pdf P88 */
  PA_DDR |= (1<<3);        /* ����ΪRST������� */
  PA_CR1 |= (1<<3);        /* �����������--�鿴STM8�Ĵ���.pdf P88 */
  LT8920_CS_HIGH();        /* ��ʼ��LT8920оƬƬѡ�������ӵ�I/O���ţ�������ߵ�ƽ */
#endif
}
/*******************************************************************************
*�� �� ��: LT8900_Init
*����˵��: LT8920оƬ��ʼ������
*��    �Σ���
*�� �� ֵ: ��
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
	SPI_WriteReg(32, 0x48, 0x00 );      //8920��62.5kbps��ͬ��ͷֻ����32��16bit
	SPI_WriteReg(33, 0x3f, 0xc7 );
	SPI_WriteReg(34, 0x20, 0x00 );
	SPI_WriteReg(35, 0x05, 0x00 );			//�ط�����4��
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
	SPI_WriteReg(42, 0xfd, 0xb0 );		/*�ȴ�RX_ACKʱ�� 176us*/
	SPI_WriteReg(43, 0x00, 0x0f );					
	SPI_WriteReg(44, 0x10, 0x00 );
	SPI_WriteReg(45, 0x05, 0x52 );		 //62.5k
	SPI_WriteReg(50, 0x00, 0x00 );
  HAL_Delay(100);
}
/*******************************************************************************
*�� �� ��: SPI_WriteReg
*����˵��: SPIд16λ�Ĵ���
*��    �Σ�addr��ַ H���ֽ� L���ֽ�
*�� �� ֵ: ��
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
*�� �� ��: SPI_ReadReg
*����˵��: SPI��8λ�Ĵ���
*��    �Σ�addrҪ��ȡ�ļĴ�����ֵַ
*�� �� ֵ: ��
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
/********************** (C) COPYRIGHT ���׵��ӹ����ҹ����� *****END OF FILE****/