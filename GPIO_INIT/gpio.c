#include "gpio.h"
#include "steep.h"

/***************************初始化LED指示灯***************************/
void LED_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	//开启LED引脚时钟
	ENABLE_LED_RCC_CLOCK;
	
	//初始化红色LED灯控制引脚
	GPIO_InitStruct.Pin = LED_RED_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(LED_RED_Prot, &GPIO_InitStruct);
	
	//初始化绿色LED灯控制引脚
	GPIO_InitStruct.Pin = LED_GRE_Pin;
	HAL_GPIO_Init(LED_GRE_Prot, &GPIO_InitStruct);
	
	HAL_GPIO_WritePin(LED_RED_Prot, LED_RED_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(LED_GRE_Prot, LED_GRE_Pin, GPIO_PIN_RESET);
}
	
	

/***************************初始化LCD12864控制引脚**************************/
void LCD12864_Pin_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;

  /* 开启LCD12864引脚时钟 */
  ENABLE_LCD12864_RCC_CLOCK;
	
	//初始化LCD12864引脚
	GPIO_InitStruct.Pin    = LCD_CLK_Pin;
	GPIO_InitStruct.Mode   = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull   = GPIO_NOPULL;
	GPIO_InitStruct.Speed  = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(LCD_CLK_Prot, &GPIO_InitStruct);

	GPIO_InitStruct.Pin    = LCD_SID_Pin;
	GPIO_InitStruct.Mode   = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull   = GPIO_NOPULL;
	GPIO_InitStruct.Speed  = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(LCD_SID_Prot, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin    = LCD_CS_Pin;
	GPIO_InitStruct.Mode   = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull   = GPIO_NOPULL;
	GPIO_InitStruct.Speed  = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(LCD_CS_Prot, &GPIO_InitStruct);
}



/***************************初始化移动电机B限位控制引脚**************************/
void EndStop_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	ENABLE_END_STOP_RCC_CLOCK;
	
	//初始化正面移动限位开关引脚
	GPIO_InitStruct.Pin=END_STOP_L_Pin;                			
	GPIO_InitStruct.Mode=GPIO_MODE_INPUT;                  //引脚输入模式
	GPIO_InitStruct.Pull=GPIO_PULLUP;
	HAL_GPIO_Init(END_STOP_L_Prot,&GPIO_InitStruct);
	
	
	//初始化背面移动限位开关引脚
	GPIO_InitStruct.Pin=END_STOP_R_Pin; 
	HAL_GPIO_Init(END_STOP_R_Prot,&GPIO_InitStruct);
}


/***************************初始化电机控制引脚**************************/
void Motor_Intt(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	ENABLE_MOTOR_A_RCC_CLOCK;
	ENABLE_MOTOR_B_RCC_CLOCK;
	ENABLE_MOTOR_C_RCC_CLOCK;
	
	//抽丝电机A控制引脚初始化
	GPIO_InitStruct.Pin    = MOTOR_A_STEP_Pin;
	GPIO_InitStruct.Mode   = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull   = GPIO_NOPULL;
	GPIO_InitStruct.Speed  = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(MOTOR_A_STEP_Prot, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin    = MOTOR_A_DIR_Pin;
	GPIO_InitStruct.Mode   = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull   = GPIO_NOPULL;
	GPIO_InitStruct.Speed  = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(MOTOR_A_DIR_Prot, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin    = MOTOR_A_EN_Pin;
	GPIO_InitStruct.Mode   = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull   = GPIO_NOPULL;
	GPIO_InitStruct.Speed  = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(MOTOR_A_EN_Prot, &GPIO_InitStruct);
	
	
	//移动电机B控制引脚初始化
	GPIO_InitStruct.Pin    = MOTOR_B_STEP_Pin;
	GPIO_InitStruct.Mode   = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull   = GPIO_NOPULL;
	GPIO_InitStruct.Speed  = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(MOTOR_B_STEP_Prot, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin    = MOTOR_B_DIR_Pin;
	GPIO_InitStruct.Mode   = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull   = GPIO_NOPULL;
	GPIO_InitStruct.Speed  = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(MOTOR_B_DIR_Prot, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin    = MOTOR_B_EN_Pin;
	GPIO_InitStruct.Mode   = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull   = GPIO_NOPULL;
	GPIO_InitStruct.Speed  = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(MOTOR_B_EN_Prot, &GPIO_InitStruct);
	
	
	
	//挤丝电机C控制引脚初始化
	GPIO_InitStruct.Pin    = MOTOR_C_STEP_Pin;
	GPIO_InitStruct.Mode   = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull   = GPIO_NOPULL;
	GPIO_InitStruct.Speed  = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(MOTOR_C_STEP_Prot, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin    = MOTOR_C_DIR_Pin;
	GPIO_InitStruct.Mode   = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull   = GPIO_NOPULL;
	GPIO_InitStruct.Speed  = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(MOTOR_C_DIR_Prot, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin    = MOTOR_C_EN_Pin;
	GPIO_InitStruct.Mode   = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull   = GPIO_NOPULL;
	GPIO_InitStruct.Speed  = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(MOTOR_C_EN_Prot, &GPIO_InitStruct);
	
	//初始化各轴脉冲发送定时器
	Steep_Init();
}




