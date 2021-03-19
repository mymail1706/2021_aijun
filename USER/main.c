#include <math.h>
#include <stdbool.h>
#include "Huanyu_sys.h"
#include "Huanyu_delay.h"
#include "Huanyu_time.h"
#include "Huanyu_led.h"
#include "Huanyu_usart.h"
#include "Huanyu_encoder.h"
#include "Huanyu_moto.h"
#include "Huanyu_pid.h"
#include "Huanyu_iic.h"
#include "Huanyu_mpu6050.h"
#include "Huanyu_charge.h"
#include "Huanyu_iwdg.h"

int main(void)
{ 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); // The system interrupts the packet
	Huanyu_delay_init(168);               			// The tick timer  is initialized with a clock frequency of 168M
	BaseBoard_TIM7_Init(999,8400);           		// PID cycle control, timer interrupt initial, the overflow time is 100ms(999)  
	Huanyu_Usart1_Init(115200);						// Serial port 1 initialization, baud rate is 115200, PC communication interface
	Huanyu_LED_init();
	Huanyu_BEEP_init();
	RightMoto_Encoder_Input_init();					// collter encoder data calculate moto speed ,
	LeftMoto_Encoder_Input_init();
	Huanyu_Charge_configure();
	Huanyu_IIC_Init();								// IIC bus configure use to Mpu6050 commnication
	Huanyu_MPU_Init();								// MPU6050 register configure function
	HuanyuMoto_PWM_Init(1000, 10);					// Motor PWM output initialization, the frequency is 10.42Khz, PWM duty is 1-1000
	Huanyu_IWDG_Init(4, 200);						// IWDG feed init function timer=100ms
	memset(&Left_moto,  0, sizeof(Left_moto));
	memset(&Right_moto, 0, sizeof(Right_moto));
	
	memset(&Send_Data,   0, sizeof(Send_Data));
	memset(&Recive_Data, 0, sizeof(Recive_Data));
	
	Robot_Encoder_Start();
	
	BEEP = ON;
	delay_ms(200);
	BEEP = OFF;
	
	while(1)
	{	
		delay_ms(46);
		
		Huanyu_moto_Control_speed(Right_moto.Current_Speed, Right_moto.Target_Speed ,MOTO_RIGHT);
		Huanyu_moto_Control_speed(Left_moto.Current_Speed,  Left_moto.Target_Speed  ,MOTO_LEFT );
		
		MPU_Get_Accelerometer(&Send_Data.Sensor_Str.Link_Accelerometer);	//通过IIC读取加速度信息
		MPU_Get_Gyroscope(&Send_Data.Sensor_Str.Link_Gyroscope);			//通过IIC读取角速度信息
		
		Huanyu_SendTo_UbuntuPC();											//向树莓派透传数据
		
		Huanyu_IWDG_Feed();
	}
}

