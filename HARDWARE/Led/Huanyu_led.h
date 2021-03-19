#ifndef _HUANYU_LED_H_
#define _HUANYU_LED_H_

#include "Huanyu_sys.h"

#define ON	1
#define OFF	0
	
#define LED1	PCout(0)
#define LED2	PCout(0)
#define LED3	PCout(0)

#define LEFT_DIR	PCout(1)
#define RIGHT_DIR	PCout(2)



#define RUN_LED	PAout(5)
#define BEEP	PAout(11)

#define BAT_LIMIT_MAX		12.0f
#define BAT_LIMIT_MIDDLE	11.0f
#define BAT_LIMIT_MIN		10.5f
#define BAT_LIMIT_10V		10.2f		//电压为10V自动断电

void Huanyu_LED_init(void);
void Huanyu_BEEP_init(void);
void Huanyu_BAT_Show(float valtage);

#endif


