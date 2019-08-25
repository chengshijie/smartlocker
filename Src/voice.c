#include "voice.h"


extern void delay_us(uint16_t time);

void voice_init()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	VOICE_GPIO_CLK_ENABLE();
	GPIO_InitStructure.Pin = GPIO_PIN_9|GPIO_PIN_10;
	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOA,&GPIO_InitStructure);
	VOICE_CLK_0;
	VOICE_DATA_0;
	//HAL_Delay(100);
}


void voice_play(uint8_t data)
{
	__disable_irq();
//	printf("voiceplay\r\n");
	uint8_t i;
	VOICE_CLK_0;
  VOICE_CLK_1;
	delay_us(5000);
	for(i=0; i<8; i++ )
	{
		if(data & 0x80)
		{
			VOICE_DATA_1;
		}
		else
		{
			VOICE_DATA_0;
		}
		VOICE_CLK_0;
		delay_us(500);
		VOICE_CLK_1;
		delay_us(500);
		data <<= 1;
	}
	VOICE_CLK_0;
	VOICE_DATA_0;

	__enable_irq();
}

