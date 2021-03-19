#ifndef __HUANYU_MOTO_H
#define __HUANYU_MOTO_H 	

#include <string.h> 
#include <stdbool.h>
#include <math.h>
#include "Huanyu_sys.h"


#define ENCODER_TTL_COUNT_VALUE	   	933.0f
#define ROBOT_INITIATIVE_DIAMETER	0.067f	//Ö÷¶¯ÂÖÖ±¾¶	
#define CONTROL_TIMER_CYCLE			0.1f	
#define Pi_v						3.1415f
#define Base_Width					0.175f


#define MOTO_LEFT		2
#define MOTO_RIGHT		3

typedef struct _Moto_
{
	int Encoder_Value;
	float Current_Speed;
	float Target_Speed;
	short ESC_Output_PWM;
	float L_Error;
	float LL_Error;
}_Moto_Str;


extern _Moto_Str Left_moto;
extern _Moto_Str Right_moto;


void HuanyuMoto_PWM_Init(unsigned int arr, unsigned int pcs);
void Huanyu_PWM_Output(signed short Moto1_Left,signed short Moto_Right);


#endif
