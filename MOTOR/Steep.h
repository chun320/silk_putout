#ifndef _STEEP_H
#define _STEEP_H

#include "map_pin.h"
#include "sys.h"

extern TIM_HandleTypeDef TIM9_Handler;      	//定时器9句柄
extern TIM_HandleTypeDef TIM10_Handler;      	//定时器10句柄
extern TIM_HandleTypeDef TIM11_Handler;      	//定时器11句柄
extern TIM_HandleTypeDef TIM12_Handler;      	//定时器12句柄

//当前抽丝电机A的转速
extern float Motor_A_Speed_Now;
//当前移动电机B的转速
extern float Motor_B_Speed_Now;
//当前挤丝电机C的转速
extern float Motor_C_Speed_Now;

//各轴电机初始化时速度数据（字符串格式）
extern u8 Motor_Init_Speed_Str[4];

//各轴电机速度数据（字符串格式）
extern u8 Motor_A_Speed_Str[4];
extern u8 Motor_B_Speed_Str[4];
extern u8 Motor_C_Speed_Str[4];

//各轴控制电机定时器自动重装载值寄存器
extern u16 Motor_A_Step_ARR;
extern u16 Motor_B_Step_ARR;
extern u16 Motor_C_Step_ARR;

//抽丝电机A步进值
#define MOTOR_A_STEP   10000*4.25f //抽丝电机减速比为：4.25
//移动电机B步进值
#define MOTOR_B_STEP   10000
//挤丝电机C步进值
#define MOTOR_C_STEP   10000

//抽丝电机A初始化转速(r/min)
#define MOTOR_A_SPEED   							1.0
//移动电机B初始化转速(r/min)
#define MOTOR_B_SPEED   							1.0
//挤丝电机C初始化转速(r/min)
#define MOTOR_C_SPEED   							1.0

//抽丝电机A速度值存储地址
#define W25Q_Motor_A_Speed_Add      	0
//移动电机B速度值存储地址
#define W25Q_Motor_B_Speed_Add      	4
//挤丝电机C速度值存储地址
#define W25Q_Motor_C_Speed_Add      	8


//抽丝电机A控制引脚
#define Motor_A_Pulse  PAout(2)
#define Motor_A_Dir    PCout(2)
#define Motor_A_EN     PAout(4)
//挤丝电机B控制引脚
#define Motor_B_Pulse  PAout(5)
#define Motor_B_Dir    PCout(4)
#define Motor_B_EN     PBout(0)
//移动电机C控制引脚
#define Motor_C_Pulse  PAout(1)
#define Motor_C_Dir    PCout(3)
#define Motor_C_EN     PAout(3)

//移动电机C限位开关引脚映射
#define END_STOP_L     PEin(5)
#define END_STOP_R     PEin(6)

void Steep_Init(void);
void Read_Motor_Speed(void);

#endif
