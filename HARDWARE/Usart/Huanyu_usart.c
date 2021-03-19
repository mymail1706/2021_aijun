#include "Huanyu_sys.h"
#include "Huanyu_usart.h"
#include "Huanyu_moto.h"
#include "Huanyu_led.h"
#include "Huanyu_charge.h"
#include <math.h>

Upload_Data Send_Data, Recive_Data;

#if 1           
struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;       
void _sys_exit(int x) 
{ 
	x = x; 
} 
int fputc(int ch, FILE *f)
{ 	
	while((USART1->SR&0X40)==0);
	USART1->DR = (u8) ch;      
	return ch;
}
#endif


/*
 @ describetion: USART1 initialization function
 @ param: bound->115200
 @ return: none
 @ author: Xuewei Zhou
 @ date : 2019-4-17
 @ note: 
 @ function: void Huanyu_Usart1_Init(u32 bound)
*/
void Huanyu_Usart1_Init(u32 bound)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
 
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1); 
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9 ,GPIO_AF_USART1); 
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_9; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOA,&GPIO_InitStructure);

	USART_InitStructure.USART_BaudRate = bound;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	
	USART_Init(USART1, &USART_InitStructure);
	
	USART_Cmd(USART1, ENABLE);  
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);

	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;		
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;		
	NVIC_Init(&NVIC_InitStructure);	
}

/*
 @ describetion:usart send a char data
 @ param: b:data
 @ return: none
 @ author: Xuewei Zhou
 @ date : 2019-4-17
 @ note: 
 @ function: void USART1_SendChar(unsigned char b)
*/
void USART1_SendChar(unsigned char b)
{
    while (USART_GetFlagStatus(USART1,USART_FLAG_TC) == RESET);
			USART_SendData(USART1,b);
}


/*
 @ describetion: Chassis kinematics Positive solution function
 @ param: float vx,float vz
 @ return: none
 @ author: Xuewei Zhou
 @ date : 2019-4-17
 @ note: 
 @ function: void Kinematics_Positive(float vx,float vz)
*/
void Kinematics_Positive(float vx,float vz)
{
	if(vx == 0.0f){			//原地旋转或静止
		Right_moto.Target_Speed = vz * Base_Width / 2.0f;
		Left_moto.Target_Speed  = (-1) * Right_moto.Target_Speed;
	}
	else if(vz == 0.0f){	//静止或者前后运动
		Right_moto.Target_Speed = Left_moto.Target_Speed = vx;
	}	
	else{					//在前进或者后退过程中转弯
		Left_moto.Target_Speed  = vx - vz * Base_Width / 2.0f;
		Right_moto.Target_Speed = vx + vz * Base_Width / 2.0f;
	}
}

/*
 @ describetion:Send Data To ubuntu pc
 @ param: b:data
 @ return: none
 @ author: Xuewei Zhou
 @ date : 2019-4-17
 @ note: 
 @ function: void Huanyu_SendTo_UbuntuPC()
*/
void Huanyu_SendTo_UbuntuPC()
{
	unsigned char i = 0;
	Send_Data.Sensor_Str.Header   = PROTOCOL_HEADER;
	Send_Data.Sensor_Str.End_flag = PROTOCOL_END;
	
	Send_Data.Sensor_Str.X_speed = (Left_moto.Current_Speed + Right_moto.Current_Speed)/2.0f;
	Send_Data.Sensor_Str.Y_speed = 0.0;
	Send_Data.Sensor_Str.Z_speed = (Left_moto.Current_Speed - Right_moto.Current_Speed)/Base_Width;
	
	Send_Data.Sensor_Str.Source_Voltage = Source_Valtage;
	
	for(i=0; i<PROTOCL_DATA_SIZE; i++)
	{
		USART1_SendChar(Send_Data.buffer[i]);
	}
}


/*
 @ describetion: USART1 interrupt process function
 @ param: void
 @ return: none
 @ author: Xuewei Zhou
 @ date : 2019-4-17
 @ note: 
 @ function: void USART1_IRQHandler(void)
*/
unsigned char Rcount = 0;
void USART1_IRQHandler(void)                	
{
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  
	{
		Recive_Data.buffer[Rcount] = USART_ReceiveData(USART1);
		(Recive_Data.buffer[0] == 0xFe)?(Rcount++):(Rcount = 0);
		if (Rcount == PROTOCL_DATA_SIZE)	//验证数据包的长度
		{
			if(Recive_Data.Sensor_Str.Header == PROTOCOL_HEADER)	//验证数据包的头部校验信息
			{
				if(Recive_Data.Sensor_Str.End_flag == PROTOCOL_END)	//验证数据包的尾部校验信息
				{
					//接收上位机控制命令，使机器人产生相应的运动
					Kinematics_Positive(Recive_Data.Sensor_Str.X_speed, Recive_Data.Sensor_Str.Z_speed);
				}
			}
			Rcount = 0;
		}
	} 
}

/*
 @ describetion: hanwai Usart1 send function
 @ param: uint8_t *value,uint32_t size
 @ return: none
 @ author: Xuewei Zhou
 @ date : 2019-4-17
 @ note: 
 @ function: void shanwai_send_data1(uint8_t *value,uint32_t size )
*/
float send_data[4];
void shanwai_send_data1(uint8_t *value,uint32_t size )
{
	USART1_SendChar(0x03);
	USART1_SendChar(0xfc);
	while(size)
	{
		USART1_SendChar(*value);
		value++;
		size--;
	}
	USART1_SendChar(0xfc);   
	USART1_SendChar(0x03);
}


