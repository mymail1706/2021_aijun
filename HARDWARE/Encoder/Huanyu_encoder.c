#include "Huanyu_encoder.h"
#include "Huanyu_moto.h"
#include "Huanyu_usart.h"


/*
 @ describetion:left moto encoder input TIM4 configure 
 @ param: float Dacvalue
 @ return: none
 @ author: Xuewei Zhou
 @ date : 2019-3-14
 @ function : void RightMoto_Encoder_Input_init(void)
*/
void RightMoto_Encoder_Input_init(void)
{
	GPIO_InitTypeDef gpio;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3 ,ENABLE);

	gpio.GPIO_Pin 	= GPIO_Pin_6 | GPIO_Pin_7;
	gpio.GPIO_Mode 	= GPIO_Mode_AF;
	gpio.GPIO_OType = GPIO_OType_PP;
	gpio.GPIO_PuPd 	= GPIO_PuPd_UP;
	gpio.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOA,&gpio);

	GPIO_PinAFConfig(GPIOA, GPIO_PinSource6,  GPIO_AF_TIM3);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource7,  GPIO_AF_TIM3);

	TIM_EncoderInterfaceConfig(TIM3, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);

	TIM_Cmd(TIM3, ENABLE);
}

/*
 @ describetion:Right moto encoder input TIM3 configure 
 @ param: float Dacvalue
 @ return: none
 @ author: Xuewei Zhou
 @ date : 2019-3-14
 @ function : void LeftMoto_Encoder_Input_init(void)
*/
void LeftMoto_Encoder_Input_init(void)
{
	GPIO_InitTypeDef gpio;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4 ,ENABLE);

	gpio.GPIO_Pin 	= GPIO_Pin_6 | GPIO_Pin_7;
	gpio.GPIO_Mode 	= GPIO_Mode_AF;
	gpio.GPIO_OType = GPIO_OType_PP;
	gpio.GPIO_PuPd 	= GPIO_PuPd_UP;
	gpio.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOB,&gpio);

	GPIO_PinAFConfig(GPIOB, GPIO_PinSource6,  GPIO_AF_TIM4);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource7,  GPIO_AF_TIM4);

	TIM_EncoderInterfaceConfig(TIM4, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);

	TIM_Cmd(TIM4, ENABLE);
}

/*
 @ describetion: Encoder count start
 @ param:  none
 @ return: none
 @ author: Xuewei Zhou
 @ date : 2019-3-14
 @ function : void Robot_Encoder_Start(void)
*/
void Robot_Encoder_Start(void)
{
    TIM3->CNT = 0x7fff;
	TIM4->CNT = 0x7fff;
}

/*
 @ describetion: Get encoder value
 @ param:  NONE
 @ return: none
 @ author: Xuewei Zhou
 @ date : 2019-3-14
 @ function : void  Robot_Encoder_Get_CNT(void)
*/
void  Robot_Encoder_Get_CNT(void)
{
	Left_moto.Encoder_Value   = (TIM3->CNT)-0x7fff;		//读取左右轮子的脉冲累计数
	Right_moto.Encoder_Value  = -((TIM4->CNT)-0x7fff);
	
	//计算左右轮子的线性速度，速度 =（（轮子的直径 * 3.14 * （编码器脉冲数 / 轮子一圈积累的脉冲数））/ 采样周期）
	Left_moto.Current_Speed \
		= -((ROBOT_INITIATIVE_DIAMETER *Pi_v * (Left_moto.Encoder_Value  / ENCODER_TTL_COUNT_VALUE))/CONTROL_TIMER_CYCLE);
	Right_moto.Current_Speed\
		= ((ROBOT_INITIATIVE_DIAMETER  *Pi_v * (Right_moto.Encoder_Value / ENCODER_TTL_COUNT_VALUE))/CONTROL_TIMER_CYCLE);
	
	TIM3->CNT = 0x7fff;		//清除左右轮子的脉冲数
	TIM4->CNT = 0x7fff;
}


