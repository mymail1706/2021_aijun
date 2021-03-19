#include "Huanyu_pid.h"
#include "Huanyu_moto.h"
#include "Huanyu_usart.h"
#include "Huanyu_led.h"


#define KP   225.9f 
#define KI   165.0f 
#define KD   110.0f		


/*
 @ describetion: Moto speed PID control function
 @ param: float current_speed,float target_speed, unsigned char Moto_ID
 @ return: none
 @ author: Xuewei Zhou
 @ date : 2019-4-14
 @ function : void Huanyu_moto_Control_speed(float current_speed,float target_speed, unsigned char Moto_ID)
*/
//#define _Debug_LineShow_
void Huanyu_moto_Control_speed(float current_speed,float target_speed, unsigned char Moto_ID)
{
	float Error   = 0;
	float P_Error = 0;
	float I_Error = 0;
	float D_Error = 0;
	float add     = 0;
	
	if(Moto_ID == MOTO_LEFT)
	{
		Error = target_speed - current_speed;
		//Update the current one-two third-order error for the current proportional integral and differential calculation
		P_Error = Error;
		I_Error = Error - Left_moto.L_Error;
		D_Error = Error - 2*Left_moto.L_Error + Left_moto.LL_Error;
		
		//calculation current proportional integral and differential 
		add = KP * P_Error + KI * I_Error + KD * D_Error;
		Left_moto.ESC_Output_PWM += add;
		
		Left_moto.LL_Error = Left_moto.L_Error;
		Left_moto.L_Error = Error;
		
		if(Left_moto.ESC_Output_PWM > ESC_output_PWM_LIMT)	Left_moto.ESC_Output_PWM = ESC_output_PWM_LIMT;		
		else if(Left_moto.ESC_Output_PWM < -ESC_output_PWM_LIMT)	Left_moto.ESC_Output_PWM = -ESC_output_PWM_LIMT;
	}
	else if(Moto_ID == MOTO_RIGHT)
	{
		Error = target_speed - current_speed;
		//Update the current one-two third-order error for the current proportional integral and differential calculation
		P_Error = Error;
		I_Error = Error - Right_moto.L_Error;
		D_Error = Error - 2*Right_moto.L_Error + Right_moto.LL_Error;

		//calculation current proportional integral and differential 
		add = KP * P_Error + KI * I_Error + KD * D_Error;
		Right_moto.ESC_Output_PWM += add;
		
		Right_moto.LL_Error = Right_moto.L_Error;
		Right_moto.L_Error = Error;
		
		if(Right_moto.ESC_Output_PWM > ESC_output_PWM_LIMT)	Right_moto.ESC_Output_PWM = ESC_output_PWM_LIMT;		
		else if(Right_moto.ESC_Output_PWM < -ESC_output_PWM_LIMT) Right_moto.ESC_Output_PWM = -ESC_output_PWM_LIMT;
		
		#ifdef _Debug_LineShow_
		{	
			send_data[0] = current_speed*1000;
			send_data[1] = target_speed*1000;
			send_data[2] = add;
			send_data[3] = Right_moto.ESC_Output_PWM;
			
			shanwai_send_data1((uint8_t*)&send_data,sizeof(send_data));
		}
		#endif
	}
	// moto control function
	Huanyu_PWM_Output(Left_moto.ESC_Output_PWM, Right_moto.ESC_Output_PWM);
	//Huanyu_PWM_Output(500, 500);
}
