#include "Huanyu_moto.h"
#include "Huanyu_led.h"
#include "Huanyu_encoder.h"

_Moto_Str Left_moto;
_Moto_Str Right_moto;

/*
 @ describetion: Timer PWM output configure
 @ param: none
 @ return: none
 @ author: Xuewei Zhou
 @ date : 2019-3-14
 @ function : void HuanyuMoto_PWM_Init(unsigned int arr, unsigned int pcs)
*/
void HuanyuMoto_PWM_Init(unsigned int arr, unsigned int pcs)
{		 					 
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
    TIM_BDTRInitTypeDef TIM_BDTRInitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8,ENABLE);  	  
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE); 	

	GPIO_PinAFConfig(GPIOC,GPIO_PinSource6,GPIO_AF_TIM8);
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource7,GPIO_AF_TIM8);
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource8,GPIO_AF_TIM8); 
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource9,GPIO_AF_TIM8);
   

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;           
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        
	GPIO_Init(GPIOC,&GPIO_InitStructure);  
	
	TIM_TimeBaseStructure.TIM_Prescaler=pcs;  
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; 
	TIM_TimeBaseStructure.TIM_Period=arr;   
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV2;	
	TIM_TimeBaseInit(TIM8,&TIM_TimeBaseStructure);

	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 0;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
    
    TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;   
    TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_Low;   
    TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;

	TIM_OC1Init(TIM8, &TIM_OCInitStructure);
	TIM_OC2Init(TIM8, &TIM_OCInitStructure);
	TIM_OC3Init(TIM8, &TIM_OCInitStructure);
	TIM_OC4Init(TIM8, &TIM_OCInitStructure);
    
    TIM_BDTRInitStructure.TIM_OSSRState = TIM_OSSRState_Enable;               
    TIM_BDTRInitStructure.TIM_OSSIState = TIM_OSSIState_Enable;               
    TIM_BDTRInitStructure.TIM_LOCKLevel = TIM_LOCKLevel_1;                    
    TIM_BDTRInitStructure.TIM_DeadTime = 0X94;
    TIM_BDTRInitStructure.TIM_Break = TIM_Break_Disable;                    
    TIM_BDTRInitStructure.TIM_BreakPolarity = TIM_BreakPolarity_Low;        
    TIM_BDTRInitStructure.TIM_AutomaticOutput = TIM_AutomaticOutput_Enable;    
    TIM_BDTRConfig(TIM8, &TIM_BDTRInitStructure);

	TIM_OC1PreloadConfig(TIM8, TIM_OCPreload_Enable);
	TIM_OC2PreloadConfig(TIM8, TIM_OCPreload_Enable);
	TIM_OC3PreloadConfig(TIM8, TIM_OCPreload_Enable);
	TIM_OC4PreloadConfig(TIM8, TIM_OCPreload_Enable);

	TIM_ARRPreloadConfig(TIM8,ENABLE);

	TIM_Cmd(TIM8, ENABLE); 
	TIM_CtrlPWMOutputs(TIM8,ENABLE); 
}

/*
 @ describetion: Pwm output function
 @ param: none
 @ return: none
 @ author: Xuewei Zhou
 @ date : 2019-4-17
 @ note: 
 @ function void Huanyu_PWM_Output(signed short Moto1_Left,signed short Moto_Right)
*/
 void Huanyu_PWM_Output(signed short Moto_Left,signed short Moto_Right)
{
	if(Moto_Left >= 0){
    LEFT_DIR = 1;
		TIM_SetCompare1(TIM8,0);
		TIM_SetCompare2(TIM8,fabs(Moto_Left));
	}
	else{
    LEFT_DIR = 0;
		TIM_SetCompare1(TIM8,fabs(Moto_Left));
		TIM_SetCompare2(TIM8,0);
	}
	if(Moto_Right >= 0){
    RIGHT_DIR = 1;
		TIM_SetCompare3(TIM8,fabs(Moto_Right));
		TIM_SetCompare4(TIM8,0);
	}
	else{
    RIGHT_DIR = 0;
		TIM_SetCompare3(TIM8,0);
		TIM_SetCompare4(TIM8,fabs(Moto_Right));
		
	}
}


