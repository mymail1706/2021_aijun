#ifndef __HUANYU_ENCODER_H
#define __HUANYU_ENCODER_H 	

#include "Huanyu_sys.h"

/* 
 @	 moto |  port  |   A   |   B   |  tim  |
 @	 left |  PBin  |  PB6  |  PB7  | TIM4  |
 @  right |  PCin  |  PC6  |  PC7  | TIM3  |
*/

void LeftMoto_Encoder_Input_init(void);
void RightMoto_Encoder_Input_init(void);

void Robot_Encoder_Start(void);
void Robot_Encoder_Get_CNT(void);

#endif
