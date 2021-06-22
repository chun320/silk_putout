#ifndef _MAP_PIN_H
#define _MAP_PIN_H

/************************LED引脚定义****************************/
#define ENABLE_LED_RCC_CLOCK        __HAL_RCC_GPIOF_CLK_ENABLE();
//红色指示灯引脚定义
#define LED_RED_Prot                GPIOF
#define LED_RED_Pin                 GPIO_PIN_9

//绿色指示灯引脚定义
#define LED_GRE_Prot                GPIOF
#define LED_GRE_Pin                 GPIO_PIN_10


 /*********************LCD12864引脚定义*************************/
#define ENABLE_LCD12864_RCC_CLOCK    __HAL_RCC_GPIOG_CLK_ENABLE();\
                                     __HAL_RCC_GPIOE_CLK_ENABLE();
 
 //LCD12864_E引脚定义
#define LCD_CLK_Prot                 GPIOG
#define LCD_CLK_Pin                  GPIO_PIN_14

//LCD12864_R/W引脚定义
#define LCD_SID_Prot                 GPIOG
#define LCD_SID_Pin                  GPIO_PIN_12

//LCD12864_RS引脚定义
#define LCD_CS_Prot                  GPIOE
#define LCD_CS_Pin                   GPIO_PIN_3

/**********************KEY按钮引脚定义**************************/
#define ENABLE_KEY_RCC_CLOCK         __HAL_RCC_GPIOG_CLK_ENABLE();\
																	   __HAL_RCC_GPIOC_CLK_ENABLE();\
																	   __HAL_RCC_GPIOE_CLK_ENABLE();
																		 	 
//LCD12864_KEY_EN引脚定义
#define KEY_EN_Prot                  GPIOG
#define KEY_EN_Pin                   GPIO_PIN_13

//LCD12864_KEY_BEEP引脚定义
#define KEY_BEEP_Prot                GPIOC
#define KEY_BEEP_Pin                 GPIO_PIN_1		

//LCD12864_KEY_A引脚定义
#define KEY_A_Prot                   GPIOG
#define KEY_A_Pin                    GPIO_PIN_11

//LCD12864_KEY_B引脚定义
#define KEY_B_Prot                   GPIOE
#define KEY_B_Pin                    GPIO_PIN_2

/******************EndStop限位挡块引脚定义**********************/
#define ENABLE_END_STOP_RCC_CLOCK    __HAL_RCC_GPIOE_CLK_ENABLE();

//正面移动限位控制引脚定义
#define END_STOP_L_Prot              GPIOE
#define END_STOP_L_Pin               GPIO_PIN_5

//背面移动限位控制引脚定义
#define END_STOP_R_Prot              GPIOE
#define END_STOP_R_Pin               GPIO_PIN_6


/**********************抽丝电机A控制引脚定义**************************/
#define ENABLE_MOTOR_A_RCC_CLOCK     __HAL_RCC_GPIOA_CLK_ENABLE();\
																	   __HAL_RCC_GPIOC_CLK_ENABLE();

//抽丝电机A脉冲控制引脚定义
#define MOTOR_A_STEP_Prot            GPIOA
#define MOTOR_A_STEP_Pin             GPIO_PIN_2

//抽丝电机A方向控制引脚定义
#define MOTOR_A_DIR_Prot             GPIOC
#define MOTOR_A_DIR_Pin              GPIO_PIN_2

//抽丝电机A使能控制引脚定义
#define MOTOR_A_EN_Prot              GPIOA
#define MOTOR_A_EN_Pin               GPIO_PIN_4


/**********************移动电机B控制引脚定义**************************/
#define ENABLE_MOTOR_B_RCC_CLOCK     __HAL_RCC_GPIOA_CLK_ENABLE();\
																	   __HAL_RCC_GPIOC_CLK_ENABLE();

//移动电机B脉冲控制引脚定义
#define MOTOR_B_STEP_Prot            GPIOA
#define MOTOR_B_STEP_Pin             GPIO_PIN_1

//移动电机B方向控制引脚定义
#define MOTOR_B_DIR_Prot             GPIOC
#define MOTOR_B_DIR_Pin              GPIO_PIN_3

//移动电机B使能控制引脚定义
#define MOTOR_B_EN_Prot              GPIOA
#define MOTOR_B_EN_Pin               GPIO_PIN_3


/**********************挤丝电机C控制引脚定义**************************/
#define ENABLE_MOTOR_C_RCC_CLOCK     __HAL_RCC_GPIOA_CLK_ENABLE();\
                                     __HAL_RCC_GPIOB_CLK_ENABLE();\
																	   __HAL_RCC_GPIOC_CLK_ENABLE();

//挤丝电机C脉冲控制引脚定义
#define MOTOR_C_STEP_Prot            GPIOA
#define MOTOR_C_STEP_Pin             GPIO_PIN_5

//挤丝电机C方向控制引脚定义
#define MOTOR_C_DIR_Prot             GPIOC
#define MOTOR_C_DIR_Pin              GPIO_PIN_4

//挤丝电机C使能控制引脚定义
#define MOTOR_C_EN_Prot              GPIOB
#define MOTOR_C_EN_Pin               GPIO_PIN_0

#endif
