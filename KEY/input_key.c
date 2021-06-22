#include "input_key.h"
#include "u8g2_user.h"
#include "usart.h"
#include "map_pin.h"
#include "delay.h"
#include "steep.h"
#include "w25qxx.h"


TIM_HandleTypeDef TIM7_Handler;      //定时器7句柄

uint8_t menu_show;     			    //0: 显示主菜单画面地址寄存器
uint8_t control_menu_show;      //1: 显示控制画面地址寄存器

int8_t menu_current = 0;        //设置当前菜单光标位置
uint8_t menu_update_flage;      //屏幕刷新标志位

u8 Encoder_KEY_nu = 0;          //编码器按键相消抖计数
u8 Encoder_KEY_flage = 0;       //编码器按键相释放抬起标记位
u8 Encoder_AB_nu = 0;           //编码器旋转相消抖计数
u8 Encoder_AB_flage = 0;        //编码器旋转相消抖标记位

u8 Beep_run = 0;

//按键消抖用基本定时器7中断初始化
//2ms产生一次中断,用以检测按键按下的时间
void TIM7_Init(void)
{
	TIM7_Handler.Instance=TIM7;             //基本定时器7
	TIM7_Handler.Init.Prescaler=840-1;      //分频系数
	TIM7_Handler.Init.Period=200-1;         //自动装载值
	HAL_TIM_Base_Init(&TIM7_Handler);
    
	HAL_TIM_Base_Start_IT(&TIM7_Handler); //使能定时器7和定时器7更新中断：TIM_IT_UPDATE
}


void Input_Encoder_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  
  ENABLE_KEY_RCC_CLOCK;               //开启控制按键时钟
	
	//初始化旋转编码器A相脉冲引脚
	GPIO_InitStruct.Pin=KEY_A_Pin;                			
	GPIO_InitStruct.Mode=GPIO_MODE_IT_RISING_FALLING;      //下降沿触发外部中断
	GPIO_InitStruct.Pull=GPIO_PULLUP;
	HAL_GPIO_Init(KEY_A_Prot,&GPIO_InitStruct);
	
	//初始化旋转编码器B相脉冲引脚
	GPIO_InitStruct.Pin=KEY_B_Pin;                			
	GPIO_InitStruct.Mode=GPIO_MODE_INPUT;                  //引脚输入模式
	GPIO_InitStruct.Pull=GPIO_PULLUP;
	HAL_GPIO_Init(KEY_B_Prot,&GPIO_InitStruct);
	
	//初始化蜂鸣器控制引脚
	GPIO_InitStruct.Pin = KEY_BEEP_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(KEY_BEEP_Prot, &GPIO_InitStruct);	
	
	//设置中断线15_10
	HAL_NVIC_SetPriority(EXTI15_10_IRQn,2,0);       //抢占优先级为2，子优先级为0
	HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);             //使能中断线	
	
	//初始化按键检测的定时器7
	TIM7_Init();
}

//此函数会被HAL_TIM_Base_Init()函数调用
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
	if(htim->Instance==TIM7)
	{
		GPIO_InitTypeDef GPIO_InitStruct;
		
		ENABLE_KEY_RCC_CLOCK;                   //开启控制按键时钟
		__HAL_RCC_TIM7_CLK_ENABLE();            //使能TIM7时钟
		
		//初始化旋转编码器按键
		GPIO_InitStruct.Pin=KEY_EN_Pin;                			
		GPIO_InitStruct.Mode=GPIO_MODE_INPUT;                  //引脚输入模式
		GPIO_InitStruct.Pull=GPIO_PULLUP;
		HAL_GPIO_Init(KEY_EN_Prot,&GPIO_InitStruct);
		
		HAL_NVIC_SetPriority(TIM7_IRQn,2,1);    //设置中断优先级，抢占优先级1，子优先级3
		HAL_NVIC_EnableIRQ(TIM7_IRQn);          //开启ITM7中断   
	}
	
	if(htim->Instance==TIM9)
	{
		__HAL_RCC_TIM9_CLK_ENABLE();
		
		HAL_NVIC_SetPriority(TIM1_BRK_TIM9_IRQn,1,0);    //设置中断优先级，抢占优先级1，子优先级0
		HAL_NVIC_EnableIRQ(TIM1_BRK_TIM9_IRQn);          //开启ITM9中断  
	}
	
	if(htim->Instance==TIM10)
	{
		__HAL_RCC_TIM10_CLK_ENABLE();
		
		HAL_NVIC_SetPriority(TIM1_UP_TIM10_IRQn,1,1);    //设置中断优先级，抢占优先级1，子优先级1
		HAL_NVIC_EnableIRQ(TIM1_UP_TIM10_IRQn);          //开启ITM9中断  
	}
	
	if(htim->Instance==TIM11)
	{
		__HAL_RCC_TIM11_CLK_ENABLE();
		
		HAL_NVIC_SetPriority(TIM1_TRG_COM_TIM11_IRQn,1,2);    //设置中断优先级，抢占优先级1，子优先级2
		HAL_NVIC_EnableIRQ(TIM1_TRG_COM_TIM11_IRQn);          //开启ITM9中断  
	}
}

//定时器7中断服务函数
void TIM7_IRQHandler(void)
{
	HAL_TIM_IRQHandler(&TIM7_Handler);       //调用定时器中断处理公用函数
}

//外部中断10-15中断服务函数
void EXTI15_10_IRQHandler(void)
{
	HAL_GPIO_EXTI_IRQHandler(KEY_A_Pin);		//调用外部中断处理公用函数
}

//定时器回调函数，定时器中断服务函数调用
//按键按下处理函数和按键消抖处理
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim==(&TIM7_Handler))
	{
		if(Encoder_KEY != 0){
			Encoder_KEY_nu = 0;        //清除按键抖动产生的计数
			Encoder_KEY_flage = 1;     //标记按键被释放
		}
		else{
			Encoder_KEY_nu++;
			//按键连续8次计数都被按下，且上一次确认按下已被释放
			if(Encoder_KEY_nu>=8 && Encoder_KEY_flage)
			{
				Encoder_KEY_flage = 0;     //标记按键被按下
				switch(menu_show)
				{
					//主菜单光标选择确认控制
					case 0:
						switch(menu_current)
						{
							case 0:
								//检测电机的使能标志
								if(Motor_All_En_flage ==0)
								{
									//打开各电机的使能端口
									Motor_A_EN = 0;
									Motor_B_EN = 0;
									Motor_C_EN = 0;
									//设置各电机使能标志位
									Motor_A_En_flage = 1;
									Motor_B_En_flage = 1;
									Motor_C_En_flage = 1;
									Motor_All_En_flage =1;
								}
								else
								{
									//关闭各电机的使能端口
									Motor_A_EN = 1;
									Motor_B_EN = 1;
									Motor_C_EN = 1;
									//设置各电机使能标志位
									Motor_A_En_flage = 0;
									Motor_B_En_flage = 0;
									Motor_C_En_flage = 0;
									Motor_All_En_flage = 0;
								}	
							
								//检测电机的运行标志
								if(Motor_All_Run_flage ==0)
								{
									//设置各电机运行标志位
									Motor_A_Run_flage = 1;
									Motor_B_Run_flage = 1;
									Motor_C_Run_flage = 1;
									Motor_All_Run_flage =1;
									HAL_TIM_Base_Start_IT(&TIM9_Handler); //使能定时器9
									HAL_TIM_Base_Start_IT(&TIM10_Handler); //使能定时器10
									HAL_TIM_Base_Start_IT(&TIM11_Handler); //使能定时器11
								}
								else
								{
									//设置各电机运行标志位
									Motor_A_Run_flage = 0;
									Motor_B_Run_flage = 0;
									Motor_C_Run_flage = 0;
									Motor_All_Run_flage = 0;
									HAL_TIM_Base_Stop_IT(&TIM9_Handler);		//关闭定时器9
									HAL_TIM_Base_Stop_IT(&TIM10_Handler);		//关闭定时器10
									HAL_TIM_Base_Stop_IT(&TIM11_Handler);		//关闭定时器11
								}	
								menu_show = 0;      //显示主控制界面菜单
								menu_current = 0;		//设置光标为初始位置
							break;
								
							case 1:
								menu_show = 1;      //转到手动控制界面菜单
								menu_current = 0;		//设置光标为初始位置
							break;
							
							case 2:
								menu_show = 2;      //转到电机速度设置界面菜单
								menu_current = 0;		//设置光标为初始位置
							break;
							
							case 3:/**保存当前各电机的运转速度**/
								sprintf((char*)Motor_A_Speed_Str, "%.1f", Motor_A_Speed_Now); 
								sprintf((char*)Motor_B_Speed_Str, "%.1f", Motor_B_Speed_Now);
								sprintf((char*)Motor_C_Speed_Str, "%.1f", Motor_C_Speed_Now);
							
								W25QXX_Write((u8*)Motor_A_Speed_Str,W25Q_Motor_A_Speed_Add,4);
								W25QXX_Write((u8*)Motor_B_Speed_Str,W25Q_Motor_B_Speed_Add,4);
								W25QXX_Write((u8*)Motor_C_Speed_Str,W25Q_Motor_C_Speed_Add,4);
							break;
							
							case 4:/**初始化各电机的运转速度**/
								W25QXX_Write((u8*)Motor_Init_Speed_Str,W25Q_Motor_A_Speed_Add,4);
								W25QXX_Write((u8*)Motor_Init_Speed_Str,W25Q_Motor_B_Speed_Add,4);
								W25QXX_Write((u8*)Motor_Init_Speed_Str,W25Q_Motor_C_Speed_Add,4);
							break;
						}
					break;
						
					//手动控制界面光标选择确认控制
					case 1:
						switch(menu_current)
						{
							case 0:
								menu_show = 0;      //返回控制界面菜单
								menu_current = 0;		//设置光标为初始位置
							break;
							
							case 1:
								Motor_A_En_flage =! Motor_A_En_flage;
								if(Motor_A_En_flage == 1)
									Motor_A_EN = 0; //使能抽丝电机A
								else
									Motor_A_EN = 1; //关闭抽丝电机A
							break;
							
							case 2:
								Motor_B_En_flage =! Motor_B_En_flage;
								if(Motor_B_En_flage == 1)
									Motor_B_EN = 0; //使能挤丝电机B
								else
									Motor_B_EN = 1; //关闭挤丝电机B
							break;
							
							case 3:
								Motor_C_En_flage =! Motor_C_En_flage;
								if(Motor_C_En_flage == 1)
									Motor_C_EN = 0; //使能移动电机C
								else
									Motor_C_EN = 1; //关闭移动电机C
							break;
								
							case 4:
								Motor_A_Run_flage =! Motor_A_Run_flage;
								if(Motor_A_Run_flage == 1)
									HAL_TIM_Base_Start_IT(&TIM9_Handler); //使能定时器9
								else
									HAL_TIM_Base_Stop_IT(&TIM9_Handler);		//关闭定时器9
							break;
								
							case 5:
								Motor_B_Run_flage =! Motor_B_Run_flage;
								if(Motor_B_Run_flage == 1)
									HAL_TIM_Base_Start_IT(&TIM10_Handler); //使能定时器10
								else
									HAL_TIM_Base_Stop_IT(&TIM10_Handler);		//关闭定时器10
							break;
								
							case 6:
								Motor_C_Run_flage =! Motor_C_Run_flage;
								if(Motor_C_Run_flage == 1)
									HAL_TIM_Base_Start_IT(&TIM11_Handler); //使能定时器11
								else
									HAL_TIM_Base_Stop_IT(&TIM11_Handler);		//关闭定时器11
							break;	
						}	
					break;
					
					//电机速度设置界面光标选择确认控制
					case 2:
						switch(menu_current)
						{
							case 0:/*返回上一级*/
								menu_show = 0;      //返回控制界面菜单
								menu_current = 0;		//设置光标为初始位置
							break;
							
							case 1:/*转到电机A速度设置界面*/
								menu_show = 3;      //转到抽丝电机A速度设置界面
							break;
							
							case 2:/*转到电机B速度设置界面*/
								menu_show = 4;      //转到挤丝电机B速度设置界面
							break;
							
							case 3:/*转到电机C速度设置界面*/
								menu_show = 5;      //转到移动电机C速度设置界面
							break;
						}
					break;
						
					case 3:
						menu_show = 2;      //返回控制界面菜单
						menu_current = 0;		//设置光标为初始位置
					break;
					
					case 4:
						menu_show = 2;      //返回控制界面菜单
						menu_current = 0;		//设置光标为初始位置
					break;
					
					case 5:
						menu_show = 2;      //返回控制界面菜单
						menu_current = 0;		//设置光标为初始位置
					break;
					
				}	
				//蜂鸣器启动
				Beep_run = 1;
				
				//更新LCD12864显示菜单
				menu_update_flage = 1;
			}
		}
	}
	
	if(htim==(&TIM9_Handler))
	{
		Motor_A_Pulse = !Motor_A_Pulse;
	}
	
	if(htim==(&TIM10_Handler))
	{
		Motor_B_Pulse = !Motor_B_Pulse;
	}
	
	if(htim==(&TIM11_Handler))
	{
		Motor_C_Pulse = !Motor_C_Pulse;
	}
}


//外部中断服务程序
//旋转编码开关处理函数
//GPIO_Pin:中断引脚号
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	switch(GPIO_Pin)
	{
		//旋转编码器旋钮被转动
		case KEY_A_Pin:
			//第一次中断，并且A相是下降沿（编码器消抖）
			if(Encoder_A==0 && Encoder_AB_nu==0)
			{
				Encoder_AB_flage=0;
				if(Encoder_B == 1)     //根据B相，设定标志
					Encoder_AB_flage=1;
				Encoder_AB_nu=1;
			}
			
			//第二次中断，并且A相是上升沿（确认编码器转动，并判断方向）
			if(Encoder_A==1 && Encoder_AB_nu==1)
			{
				//编码器逆时针转动
				if(Encoder_B==0 && Encoder_AB_flage==1)
				{
					switch(menu_show)
					{
						//主菜单光标向下移动控制
						case 0:   //主界面菜单
							menu_current--;
							if(Motor_All_Run_flage==1 && menu_current==1)
								menu_current--;
							if(menu_current<0)
								menu_current = menu_main-1;
						break;
							
						case 1:  //控制界面菜单
							menu_current--;
							if(menu_current<0)
								menu_current = menu_control-1;
						break;
							
						case 2:  //电机速度设置界面菜单
							menu_current--;
							if(menu_current<0)
								menu_current = menu_setting-1;
						break;
							
						case 3:  //抽丝电机A速调节度界面
							Motor_A_Speed_Now += 0.1f;
							if(Motor_A_Speed_Now >5.0f)
								Motor_A_Speed_Now = 5.0f;
							Motor_A_Step_ARR = ((10*1000*1000*(1/((Motor_A_Speed_Now*MOTOR_A_STEP)/60)))+5)/10;
							//更新自动重装载值
							__HAL_TIM_SET_AUTORELOAD(&TIM9_Handler, Motor_A_Step_ARR-1);
						
							//printf("Motor_A_Step_ARR->%d\r\n", Motor_A_Step_ARR);
						break;
						
						case 4:  //挤丝电机B速调节度界面
							Motor_B_Speed_Now += 0.1f;
							if(Motor_B_Speed_Now >5.0f)
								Motor_B_Speed_Now = 5.0f;
							Motor_B_Step_ARR = ((10*100*1000*(1/((Motor_B_Speed_Now*MOTOR_B_STEP)/60)))+5)/10;
							//更新自动重装载值
							__HAL_TIM_SET_AUTORELOAD(&TIM10_Handler, Motor_B_Step_ARR-1);
						
							//printf("Motor_B_Step_ARR->%d\r\n", Motor_B_Step_ARR);
						break;
						
						case 5:  //移动电机C速调节度界面
							Motor_C_Speed_Now += 0.1f;
							if(Motor_C_Speed_Now >5.0f)
								Motor_C_Speed_Now = 5.0f;
							Motor_C_Step_ARR = ((10*100*1000*(1/((Motor_C_Speed_Now*MOTOR_C_STEP)/60)))+5)/10;
							//更新自动重装载值
							__HAL_TIM_SET_AUTORELOAD(&TIM11_Handler, Motor_C_Step_ARR-1);
						
							//printf("Motor_C_Step_ARR->%d\r\n", Motor_C_Step_ARR);
						break;
					}
					
					//更新LCD12864显示菜单
					menu_update_flage = 1;
				}
				
				//编码器顺时针转动
				if(Encoder_B==1 && Encoder_AB_flage==0)
				{
					switch(menu_show)
					{
						//主菜单光标向下移动控制
						case 0:                     //主菜单
							menu_current++;
							if(Motor_All_Run_flage==1 && menu_current==1)
								menu_current++;
							if(menu_current>=menu_main)
								menu_current = 0;
						break;
							
						case 1:  //控制界面菜单
							menu_current++;
							if(menu_current>=menu_control)
								menu_current = 0;
						break;
						
						case 2:  //电机速度设置界面菜单
							menu_current++;
							if(menu_current>=menu_setting)
								menu_current = 0;
						break;

						case 3:  //抽丝电机A速调节度界面
							Motor_A_Speed_Now -= 0.1f;
							if(Motor_A_Speed_Now <0.1f)
								Motor_A_Speed_Now = 0.1f;
							Motor_A_Step_ARR = ((10*1000*1000*(1/((Motor_A_Speed_Now*MOTOR_A_STEP)/60)))+5)/10;
							//更新自动重装载值
							__HAL_TIM_SET_AUTORELOAD(&TIM9_Handler, Motor_A_Step_ARR-1);
						
							//printf("Motor_A_Step_ARR->%d\r\n", Motor_A_Step_ARR);
						break;
						
						case 4:  //挤丝电机B速调节度界面
							Motor_B_Speed_Now -= 0.1f;
							if(Motor_B_Speed_Now <0.1f)
								Motor_B_Speed_Now = 0.1f;
							Motor_B_Step_ARR = ((10*100*1000*(1/((Motor_B_Speed_Now*MOTOR_B_STEP)/60)))+5)/10;
							//更新自动重装载值
							__HAL_TIM_SET_AUTORELOAD(&TIM10_Handler, Motor_B_Step_ARR-1);
						
							//printf("Motor_B_Step_ARR->%d\r\n", Motor_B_Step_ARR);
						break;
						
						case 5:  //移动电机C速调节度界面
							Motor_C_Speed_Now -= 0.1f;
							if(Motor_C_Speed_Now <0.1f)
								Motor_C_Speed_Now = 0.1f;
							Motor_C_Step_ARR = ((10*100*1000*(1/((Motor_C_Speed_Now*MOTOR_C_STEP)/60)))+5)/10;
							//更新自动重装载值
							__HAL_TIM_SET_AUTORELOAD(&TIM11_Handler, Motor_C_Step_ARR-1);
						
							//printf("Motor_C_Step_ARR->%d\r\n", Motor_C_Step_ARR);
						break;
					}
					
					//更新LCD12864显示菜单
					menu_update_flage = 1;
				}
				Encoder_AB_nu = 0;
			}
		break;
	}
}


