#include "Huanyu_iic.h"
#include "Huanyu_delay.h"

/*
 @ describetion: IIC protocol configure
 @ param: none
 @ return: none
 @ author: Xuewei Zhou
 @ date : 2019-4-17
 @ note: 
 @ function void Huanyu_IIC_Init(void)
*/
void Huanyu_IIC_Init(void)
{			
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	IIC_SCL=1;
	IIC_SDA=1;
	//MPU6050_AD0 = 0;
}

/*
 @ describetion: IIC start signal
 @ param: none
 @ return: none
 @ author: Xuewei Zhou
 @ date : 2019-4-17
 @ note: 
 @ function void IIC_Start(void)
*/
void IIC_Start(void)
{
	SDA_OUT();    
	IIC_SDA=1;	  	  
	IIC_SCL=1;
	delay_us(4);
	//START:when CLK is high,DATA change form high to low 
 	IIC_SDA=0;
	delay_us(4);
	IIC_SCL=0;
}	  

/*
 @ describetion: IIC stop signal
 @ param: none
 @ return: none
 @ author: Xuewei Zhou
 @ date : 2019-4-17
 @ note: 
 @ function void IIC_Stop(void)
*/
void IIC_Stop(void)
{
	SDA_OUT();
	IIC_SCL=0;
	//STOP:when CLK is high DATA change form low to high
	IIC_SDA=0;
 	delay_us(4);
	IIC_SCL=1; 
	IIC_SDA=1;
	delay_us(4);							   	
}
/*
 @ describetion: IIC wait ack signal function
 @ param: none
 @ return:  1-> Ack failed
 @			0-> Ack succeed
 @ author: Xuewei Zhou
 @ date : 2019-4-17
 @ note: 
 @ function: unsigned char IIC_Wait_Ack(void)
*/
unsigned char IIC_Wait_Ack(void)
{
	u8 ucErrTime=0;
	SDA_IN();      
	IIC_SDA=1;delay_us(1);	   
	IIC_SCL=1;delay_us(1);	 
	while(READ_SDA)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			IIC_Stop();
			return 1;
		}
	}
	IIC_SCL=0;  
	return 0;  
} 
/*
 @ describetion: produce ack signal function
 @ param: none
 @ return:  none
 @ author: Xuewei Zhou
 @ date : 2019-4-17
 @ note: 
 @ function: void IIC_Ack(void)
*/
void IIC_Ack(void)
{
	IIC_SCL=0;
	SDA_OUT();
	IIC_SDA=0;
	delay_us(2);
	IIC_SCL=1;
	delay_us(2);
	IIC_SCL=0;
}
/*
 @ describetion: not produce ack signal function
 @ param: none
 @ return:  none
 @ author: Xuewei Zhou
 @ date : 2019-4-17
 @ note: 
 @ function: void IIC_NAck(void)
*/	    
void IIC_NAck(void)
{
	IIC_SCL=0;
	SDA_OUT();
	IIC_SDA=1;
	delay_us(2);
	IIC_SCL=1;
	delay_us(2);
	IIC_SCL=0;
}					 				     

/*
 @ describetion: IIC send char data 
 @ param: none
 @ return:  none
 @ author: Xuewei Zhou
 @ date : 2019-4-17
 @ note: 
 @ function: void IIC_NAck(void)
*/
void IIC_Send_Byte(u8 txd)
{                        
    u8 t;   
	SDA_OUT(); 	    
    IIC_SCL=0;
    for(t=0;t<8;t++)
    {              
        IIC_SDA=(txd&0x80)>>7;
        txd<<=1; 	  
		delay_us(2);   
		IIC_SCL=1;
		delay_us(2); 
		IIC_SCL=0;	
		delay_us(2);
    }	 
} 	    
/*
 @ describetion: IIC read char data 
 @ param: ack 1-> send ACK
 @			  0->send not ACK
 @ return:  none
 @ author: Xuewei Zhou
 @ date : 2019-4-17
 @ note: 
 @ function: unsigned char IIC_Read_Byte(unsigned char ack)
*/
unsigned char IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	SDA_IN();
    for(i=0;i<8;i++ )
	{
        IIC_SCL=0; 
        delay_us(2);
		IIC_SCL=1;
        receive<<=1;
        if(READ_SDA)receive++;   
		delay_us(1); 
    }					 
    if (!ack)
        IIC_NAck();
    else
        IIC_Ack();
    return receive;
}



























