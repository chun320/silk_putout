#include "steep.h"
#include "w25qxx.h"
#include "string.h"
#include "usart.h"
#include "stdlib.h"

TIM_HandleTypeDef TIM9_Handler;      	//定时器9句柄
TIM_HandleTypeDef TIM10_Handler;      //定时器10句柄
TIM_HandleTypeDef TIM11_Handler;      //定时器11句柄

//当前抽丝电机A的转速
float Motor_A_Speed_Now = 0.0;
//当前挤丝电机B的转速
float Motor_B_Speed_Now = 0.0;
//当前移动电机C的转速
float Motor_C_Speed_Now = 0.0;

//各轴控制电机定时器自动重装载值寄存器
u16 Motor_A_Step_ARR = 0;
u16 Motor_B_Step_ARR = 0;
u16 Motor_C_Step_ARR = 0;

//各轴电机速度数据（字符串格式）
u8 Motor_A_Speed_Str[4];
u8 Motor_B_Speed_Str[4];
u8 Motor_C_Speed_Str[4];

//各轴电机初始化时速度数据（字符串格式）
u8 Motor_Init_Speed_Str[4] = {"0.0"};

void Steep_Init(void)
{ 
	TIM9_Handler.Instance=TIM9;                        //基本定时器9
	TIM9_Handler.Init.Prescaler=84-1;                 //分频系数
	TIM9_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;  //计数器实数模式
	TIM9_Handler.Init.Period=60000-1;                    //自动装载值
	HAL_TIM_Base_Init(&TIM9_Handler);
  
	__HAL_TIM_CLEAR_IT(&TIM9_Handler, TIM_IT_UPDATE);  //清理TIM开启时的中断标识
	
	
	TIM10_Handler.Instance=TIM10;                        //基本定时器10
	TIM10_Handler.Init.Prescaler=840-1;                 //分频系数
	TIM10_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;  //计数器实数模式
	TIM10_Handler.Init.Period=6000-1;                    //自动装载值
	HAL_TIM_Base_Init(&TIM10_Handler);
  
	__HAL_TIM_CLEAR_IT(&TIM10_Handler, TIM_IT_UPDATE);  //清理TIM开启时的中断标识
	
	TIM11_Handler.Instance=TIM11;                        //基本定时器11
	TIM11_Handler.Init.Prescaler=840-1;                 //分频系数
	TIM11_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;  //计数器实数模式
	TIM11_Handler.Init.Period=6000-1;                    //自动装载值
	HAL_TIM_Base_Init(&TIM11_Handler);
  
	__HAL_TIM_CLEAR_IT(&TIM11_Handler, TIM_IT_UPDATE);  //清理TIM开启时的中断标识
}

//定时器9中断服务函数
void TIM1_BRK_TIM9_IRQHandler(void)
{
	HAL_TIM_IRQHandler(&TIM9_Handler);       //调用定时器中断处理公用函数
}

//定时器10中断服务函数
void TIM1_UP_TIM10_IRQHandler(void)
{
	HAL_TIM_IRQHandler(&TIM10_Handler);       //调用定时器中断处理公用函数
}

//定时器11中断服务函数
void TIM1_TRG_COM_TIM11_IRQHandler(void)
{
	HAL_TIM_IRQHandler(&TIM11_Handler);       //调用定时器中断处理公用函数
}

//读取W25Q的FLASH中各电机的速度数据
void Read_Motor_Speed(void)
{
	W25QXX_Read(Motor_A_Speed_Str,W25Q_Motor_A_Speed_Add,4);
	W25QXX_Read(Motor_B_Speed_Str,W25Q_Motor_B_Speed_Add,4);
	W25QXX_Read(Motor_C_Speed_Str,W25Q_Motor_C_Speed_Add,4);
	
	//获取抽丝电机A的运行速度
	if(strcmp((char*)Motor_A_Speed_Str, "0.0") == 0)
		Motor_A_Speed_Now = MOTOR_A_SPEED;
	else
		Motor_A_Speed_Now = atof((char*)Motor_A_Speed_Str);
	
	
	//获取挤丝电机B的运行速度
	if(strcmp((char*)Motor_B_Speed_Str, "0.0") == 0)
		Motor_B_Speed_Now = MOTOR_B_SPEED;
	else
		Motor_B_Speed_Now = atof((char*)Motor_B_Speed_Str);
	
	
	//获取移动电机C的运行速度
	if(strcmp((char*)Motor_C_Speed_Str, "0.0") == 0)
		Motor_C_Speed_Now = MOTOR_C_SPEED;
	else
		Motor_C_Speed_Now = atof((char*)Motor_C_Speed_Str);
	
	
	//计算出各轴控制定时器的自动重装载值；将其转换为整数(10*f + 5)/10;
	Motor_A_Step_ARR = ((10*1000*1000*(1/((Motor_A_Speed_Now*MOTOR_A_STEP)/60)))+5)/10;
	Motor_B_Step_ARR = ((10*100*1000*(1/((Motor_B_Speed_Now*MOTOR_B_STEP)/60)))+5)/10;
	Motor_C_Step_ARR = ((10*100*1000*(1/((Motor_C_Speed_Now*MOTOR_C_STEP)/60)))+5)/10;
	
	//printf("Motor_A_Speed_Now->%.1f\r\n", Motor_A_Speed_Now);
	//printf("Motor_B_Speed_Now->%.1f\r\n", Motor_B_Speed_Now);
	//printf("Motor_C_Speed_Now->%.1f\r\n", Motor_C_Speed_Now);
	
	//为定时器更新自动重装载值
	__HAL_TIM_SET_AUTORELOAD(&TIM9_Handler, Motor_A_Step_ARR-1);
	__HAL_TIM_SET_AUTORELOAD(&TIM10_Handler, Motor_B_Step_ARR-1);
	__HAL_TIM_SET_AUTORELOAD(&TIM11_Handler, Motor_C_Step_ARR-1);
}


