#ifndef _COMMON_DEF_H_
#define _COMMON_DEF_H_
#include "stm32f0xx_hal.h"

typedef enum
{
  FALSE = 0U,
  TRUE
}bool;

void delay_us(uint16_t time)  
{      
   uint16_t i = 0;
	 //__disable_irq(); 
   while(time--)  
   {  
      i = 2;   //5
      while(i--);      
   } 
	 //__enable_irq();
} 

#endif

