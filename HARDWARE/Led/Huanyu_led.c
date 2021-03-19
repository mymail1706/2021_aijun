#include "Huanyu_led.h"
#include "Huanyu_charge.h"

/**
 @ LED1 = PC0
 @ LED2 = PC1
 @ LED3 = PC2
 @
 @ RUN_LED	= PA8
 @ BEEP = PA11
**/

/*
 @ describetion: led output gpio initializes configuration
 @ param: none
 @ return: none
 @ author: Xuewei Zhou
 @ date : 2019-4-17
 @ note: 
 @ function: void Huanyu_LED_init(void);
*/
void Huanyu_LED_init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOA , ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	LED1 = LED2 = LED3 = 1;
}
/*
 @ describetion: Board valtage LED show
 @ param: float valtage
 @ return: none
 @ author: Xuewei Zhou
 @ date : 2019-4-17
 @ note: 
 @ function void Huanyu_BAT_Show(float valtage)
*/

void Huanyu_BAT_Show(float valtage)
{
	(CHARGE_IN)?(BEEP = BEEP):(BEEP = 0);	//由于充电器插入电池电压升高，所以需要在条件外结束beep报警
	
	if(valtage <= BAT_LIMIT_10V)	//小于10.2f
	{
		LED1 = !LED1;
		LED2 = !LED2;
		LED3 = !LED3;
		(CHARGE_IN)?(BEEP = ~BEEP):(BEEP = 0);
		return;
	}
	if(valtage >= BAT_LIMIT_MAX)	//大于12.0v
	{
		LED1 = LED2 = LED3 = 0;
		return;
	}
	if((valtage >= BAT_LIMIT_MIDDLE) && (valtage <= BAT_LIMIT_MAX))		//12.0v---11.0v
	{
		LED1 = 1;
		LED2 = LED3 = 0;
		return;
	}
	if((valtage >= BAT_LIMIT_MIN) && (valtage <= BAT_LIMIT_MIDDLE))		//10.5---11.0
	{
		LED1 = LED2 = 1;
		LED3 = 0;
		return;
	}
	if(valtage <= BAT_LIMIT_MIN)	//小于10.5v
	{
		LED1 = LED2 = LED3 = 1;
	}

}
/*
 @ describetion: Beep output gpio initializes configuration
 @ param: none
 @ return: none
 @ author: Xuewei Zhou
 @ date : 2019-4-17
 @ note: 
 @ function void Huanyu_BEEP_init(void);
*/
void Huanyu_BEEP_init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA , ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}


