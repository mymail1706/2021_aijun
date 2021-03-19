#ifndef _HUANYU_GPIO_H_
#define _HUANYU_GPIO_H_

#include "Huanyu_sys.h"

#define POWER12V_Enable 			GPIOE->BSRRL=GPIO_Pin_0			
#define POWER12V_Disable			GPIOE->BSRRH=GPIO_Pin_0				
#define POWER5V_Enable 				GPIOE->BSRRL=GPIO_Pin_1				
#define POWER5V_Disable				GPIOE->BSRRH=GPIO_Pin_1
#define POWER19V_Enable 			GPIOE->BSRRL=GPIO_Pin_15				
#define POWER19V_Disable			GPIOE->BSRRH=GPIO_Pin_15	

#define JDQ_Bush_On					GPIOC->BSRRL=GPIO_Pin_13			
#define JDQ_Bush_Off				GPIOC->BSRRH=GPIO_Pin_13		
#define JDQ_XISHUI_On				GPIOC->BSRRL=GPIO_Pin_12		
#define JDQ_XISHUI_Off				GPIOC->BSRRH=GPIO_Pin_12

#define DCF_CHUISHUI_On				GPIOB->BSRRL=GPIO_Pin_5		
#define DCF_CHUISHUI_Off			GPIOB->BSRRH=GPIO_Pin_5

#define LEASER_POWER_ENABLE			GPIOB->BSRRL=GPIO_Pin_3
#define LEASER_POWER_DISABLE		GPIOB->BSRRH=GPIO_Pin_3

#define XISHUI_TUIGAN_UP			(GPIOC->BSRRH=GPIO_Pin_8, GPIOC->BSRRL=GPIO_Pin_9)
#define XISHUI_TUIGAN_DOWN			(GPIOC->BSRRL=GPIO_Pin_8, GPIOC->BSRRH=GPIO_Pin_9)

#define SHUAPAN_TUIGAN_UP			(GPIOC->BSRRL=GPIO_Pin_10, GPIOC->BSRRH=GPIO_Pin_11)
#define SHUAPAN_TUIGAN_DOWN			(GPIOC->BSRRH=GPIO_Pin_10, GPIOC->BSRRL=GPIO_Pin_11)
			

void Xiaoyuan_GPIO_Init(void);


#endif


