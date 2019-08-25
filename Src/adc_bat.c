#include "adc_bat.h"
#include "adc.h"

extern ADC_HandleTypeDef hadc;

uint32_t adc_get_level()
{
	uint32_t ResultVolt = 0;
	uint8_t i;
	
	MX_ADC_Init();
	//HAL_Delay(200);
	
	HAL_ADC_Start(&hadc);
	
	for (i=0;i<8;i++)
	{
		if (HAL_ADC_PollForConversion(&hadc,200) == HAL_OK)		
			ResultVolt += HAL_ADC_GetValue(&hadc);
		else 
			i--;
	}
	ResultVolt = ResultVolt >> 3;
	ResultVolt =(ResultVolt * 3300) >> 12;
	//printf("bat adc: %d\r\n",ResultVolt);
	return ResultVolt;
	
}
