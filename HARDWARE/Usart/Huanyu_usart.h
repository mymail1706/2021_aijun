#ifndef __HUANYU_USART_H
#define __HUANYU_USART_H


#include "stdio.h"	
#include "stm32f4xx_conf.h"
#include "Huanyu_sys.h" 
#include <stdbool.h>

#define PROTOCOL_HEADER		0XFEFEFEFE
#define PROTOCOL_END		0XEE

#define PROTOCL_DATA_SIZE 33

#pragma pack(1)

typedef struct __Mpu6050_Str_
{
	short X_data;
	short Y_data;
	short Z_data;
}Mpu6050_Str;

typedef union _Upload_Data_   
{
	unsigned char buffer[PROTOCL_DATA_SIZE];
	struct _Sensor_Str_
	{
		unsigned int Header;
		float X_speed;			
		float Y_speed;
		float Z_speed;
		float Source_Voltage;
		
		Mpu6050_Str Link_Accelerometer;
		Mpu6050_Str Link_Gyroscope;
		
		unsigned char End_flag;
	}Sensor_Str;
}Upload_Data;

#pragma pack(4)
	
extern Upload_Data Send_Data, Recive_Data;
	
extern float send_data[4];	

void Huanyu_Usart1_Init(u32 bound);
void USART1_SendChar(unsigned char b);
void Huanyu_SendTo_UbuntuPC(void);
void Kinematics_Positive(float vx,float vz);
void shanwai_send_data1(uint8_t *value, uint32_t size);

#endif


