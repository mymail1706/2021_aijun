#include "Huanyu_time.h"
#include "Huanyu_led.h"
#include "Huanyu_moto.h"
#include "Huanyu_usart.h"
#include "Huanyu_encoder.h"
#include "Huanyu_charge.h"

/*
 @ describetion: The timer overflow interrupt initializes the configuration
 @ param: arr:Automatic reload value
 @		  psc:Clock Prescale Number
 @ return: none
 @ author: Xuewei Zhou
 @ date : 2019-4-17
 @ note: Tout=((arr+1)*(psc+1))/Ft us.
 @ function: void BaseBoard_TIM7_Init(u16 arr,u16 psc)
*/
void BaseBoard_TIM7_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7,ENABLE);  
	
	TIM_TimeBaseInitStructure.TIM_Period = arr; 	
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;  
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; 
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM7,&TIM_TimeBaseInitStructure);
	
	TIM_ITConfig(TIM7,TIM_IT_Update,ENABLE); 
	TIM_Cmd(TIM7,ENABLE); 
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM7_IRQn; 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x03; 
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

/*
 @ describetion: TIM7 interrupt function
 @ param: none
 @ return: none
 @ author: Xuewei Zhou
 @ date : 2019-4-17
 @ note: 
 @ function void TIM7_IRQHandler(void)
*/
unsigned int Safeware_Count = 0;
void TIM7_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM7,TIM_IT_Update)==SET) 
	{
		//program runing time ccount add
		(Safeware_Count >= 42949672) ?(Safeware_Count=0) : (Safeware_Count++);	//时间基数常数
		
		if(Safeware_Count%10 == 0){Huanyu_BAT_Show(Source_Valtage);}	//电量计采样周期 1000ms
		
		//在pid参数整定时，打开电机运动
		//if(Safeware_Count == 50)Kinematics_Positive(0.5, 0.0);		//正向启动
		//if(Safeware_Count == 100)Kinematics_Positive(-0.5, 0.0);		//反向切换
		//if(Safeware_Count == 150)Kinematics_Positive(0.0, 0.0);		//反向停止
		
		RUN_LED = ~RUN_LED;		//LED 指示周期 100ms
		
		Robot_Encoder_Get_CNT();	//编码器采样周期
	}
	TIM_ClearITPendingBit(TIM7,TIM_IT_Update);  
}
