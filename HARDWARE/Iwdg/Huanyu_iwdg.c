#include "Huanyu_iwdg.h"

/*
 @ describetion: iwdg function
 @ param: prer: 0~7  rlr:0~0XFFF.
 @ return: none
 @ author: Xuewei Zhou
 @ date : 2019-4-17
 @ note: Tout=((4*2^prer)*rlr)/32 (ms).
 @ function: void Huanyu_IWDG_Init(u8 prer,u16 rlr)
*/
void Huanyu_IWDG_Init(u8 prer,u16 rlr)			
{
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable); 
	IWDG_SetPrescaler(prer); 
	IWDG_SetReload(rlr);   
	IWDG_ReloadCounter(); 
	IWDG_Enable();      
}

/*
 @ describetion: feed dog function
 @ param: none
 @ return: none
 @ author: Xuewei Zhou
 @ date : 2019-4-17
 @ note: Tout=((4*2^prer)*rlr)/32 (ms).
 @ function: void Huanyu_IWDG_Init(u8 prer,u16 rlr)
*/
void Huanyu_IWDG_Feed(void)
{
	IWDG_ReloadCounter();//reload
}
