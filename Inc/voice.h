#ifndef _VOICE_H
#define _VOICE_H
#include "stm32f0xx_hal.h"
//#include "stdint.h"

#define VOICE_CLK_GPIO_PIN                      GPIO_PIN_9
#define VOICE_CLK_GPIO_PORT                     GPIOA
#define VOICE_DATA_GPIO_PIN                     GPIO_PIN_10
#define VOICE_DATA_GPIO_PORT                    GPIOA
#define VOICE_GPIO_CLK_ENABLE()           			__HAL_RCC_GPIOA_CLK_ENABLE()

#define VOICE_CLK_0 HAL_GPIO_WritePin(GPIOA,VOICE_CLK_GPIO_PIN,GPIO_PIN_RESET)
#define VOICE_CLK_1 HAL_GPIO_WritePin(GPIOA,VOICE_CLK_GPIO_PIN,GPIO_PIN_SET)
#define VOICE_DATA_0 HAL_GPIO_WritePin(GPIOA,VOICE_DATA_GPIO_PIN,GPIO_PIN_RESET)
#define VOICE_DATA_1 HAL_GPIO_WritePin(GPIOA,VOICE_DATA_GPIO_PIN,GPIO_PIN_SET)

void voice_init();
void voice_play(uint8_t);


#endif