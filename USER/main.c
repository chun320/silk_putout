#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "gpio.h"
#include "steep.h"
#include "w25qxx.h"
#include "u8g2_user.h"
#include "input_key.h"


int main(void)
{
	HAL_Init();                    	//初始化HAL库    
	Stm32_Clock_Init(336,8,2,7);  	//设置时钟,168Mhz
	delay_init(168);               	//初始化延时函数
	uart_init(115200);              //初始化串口1
	LED_Init();                     //初始化LED端口
	LCD12864_Init();                //初始化LCD12864显示端口
	Input_Encoder_Init();           //初始化编码按钮端口
	EndStop_Init();                 //初始化移动电机B限位端口
	Motor_Intt();                   //初始化电机控制端口
	W25QXX_Init();                  //初始化FLASH存储器端口
	Read_Motor_Speed();             //开机读取电机速度
	
	//设置电机初始失能
	Motor_A_EN = 1;
  Motor_B_EN = 1;
	Motor_C_EN = 1;
	
	//设置电机初始方向
	Motor_A_Dir = 0;
	Motor_B_Dir = 0;
	Motor_C_Dir = 0;
	
	delay_ms(500);

	while(1)
	{
		//更新LCD12864显示
		if(menu_update_flage)
		{
			LCD12864_UpDate();
		}
		
		//正面移动电机限位开关被触发
		if(END_STOP_L == 1)
		{
			//消抖延时
			delay_ms(10);
			if(END_STOP_L == 1)
				Motor_C_Dir = 1;
		}
		//正面移动电机限位开关被触发
		if(END_STOP_R == 1)	
		{
			//消抖延时
			delay_ms(10);
			if(END_STOP_R == 1)
				Motor_C_Dir = 0;
		}
		
		
		//蜂鸣器运行
		if(Beep_run)
		{
			BEEP_ON;
			delay_ms(40);
			BEEP_OFF;
			Beep_run = 0;
		}
	}                                      	
}	
