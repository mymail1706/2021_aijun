#ifndef _HUANYU_CHARGE_H_
#define _HUANYU_CHARGE_H_

#include "Huanyu_sys.h"

void Huanyu_Charge_configure(void);
unsigned short Huanyu_Get_Adc(unsigned char ch) ;

#define CHARGE_IN	PBin(15)



#define Source_Valtage ((float)Huanyu_Get_Adc(ADC_Channel_15)*3.3f/4096.0f/0.16393f)

#endif


