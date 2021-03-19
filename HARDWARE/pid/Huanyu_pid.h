#ifndef __HUANYU_PID_H
#define __HUANYU_PID_H


#include "Huanyu_sys.h"


#define ESC_output_PWM_LIMT			 	950
#define MOTO_DEAD_TIMER_COMPENSATION 	200


void Huanyu_moto_Control_speed(float current_speed,float target_speed, unsigned char Moto_ID);

		 				    
#endif


