#ifndef _MAP_PIN_H
#define _MAP_PIN_H

/************************LED���Ŷ���****************************/
#define ENABLE_LED_RCC_CLOCK        __HAL_RCC_GPIOF_CLK_ENABLE();
//��ɫָʾ�����Ŷ���
#define LED_RED_Prot                GPIOF
#define LED_RED_Pin                 GPIO_PIN_9

//��ɫָʾ�����Ŷ���
#define LED_GRE_Prot                GPIOF
#define LED_GRE_Pin                 GPIO_PIN_10


 /*********************LCD12864���Ŷ���*************************/
#define ENABLE_LCD12864_RCC_CLOCK    __HAL_RCC_GPIOG_CLK_ENABLE();\
                                     __HAL_RCC_GPIOE_CLK_ENABLE();
 
 //LCD12864_E���Ŷ���
#define LCD_CLK_Prot                 GPIOG
#define LCD_CLK_Pin                  GPIO_PIN_14

//LCD12864_R/W���Ŷ���
#define LCD_SID_Prot                 GPIOG
#define LCD_SID_Pin                  GPIO_PIN_12

//LCD12864_RS���Ŷ���
#define LCD_CS_Prot                  GPIOE
#define LCD_CS_Pin                   GPIO_PIN_3

/**********************KEY��ť���Ŷ���**************************/
#define ENABLE_KEY_RCC_CLOCK         __HAL_RCC_GPIOG_CLK_ENABLE();\
																	   __HAL_RCC_GPIOC_CLK_ENABLE();\
																	   __HAL_RCC_GPIOE_CLK_ENABLE();
																		 	 
//LCD12864_KEY_EN���Ŷ���
#define KEY_EN_Prot                  GPIOG
#define KEY_EN_Pin                   GPIO_PIN_13

//LCD12864_KEY_BEEP���Ŷ���
#define KEY_BEEP_Prot                GPIOC
#define KEY_BEEP_Pin                 GPIO_PIN_1		

//LCD12864_KEY_A���Ŷ���
#define KEY_A_Prot                   GPIOG
#define KEY_A_Pin                    GPIO_PIN_11

//LCD12864_KEY_B���Ŷ���
#define KEY_B_Prot                   GPIOE
#define KEY_B_Pin                    GPIO_PIN_2

/******************EndStop��λ�������Ŷ���**********************/
#define ENABLE_END_STOP_RCC_CLOCK    __HAL_RCC_GPIOE_CLK_ENABLE();

//�����ƶ���λ�������Ŷ���
#define END_STOP_L_Prot              GPIOE
#define END_STOP_L_Pin               GPIO_PIN_5

//�����ƶ���λ�������Ŷ���
#define END_STOP_R_Prot              GPIOE
#define END_STOP_R_Pin               GPIO_PIN_6


/**********************��˿���A�������Ŷ���**************************/
#define ENABLE_MOTOR_A_RCC_CLOCK     __HAL_RCC_GPIOA_CLK_ENABLE();\
																	   __HAL_RCC_GPIOC_CLK_ENABLE();

//��˿���A����������Ŷ���
#define MOTOR_A_STEP_Prot            GPIOA
#define MOTOR_A_STEP_Pin             GPIO_PIN_2

//��˿���A����������Ŷ���
#define MOTOR_A_DIR_Prot             GPIOC
#define MOTOR_A_DIR_Pin              GPIO_PIN_2

//��˿���Aʹ�ܿ������Ŷ���
#define MOTOR_A_EN_Prot              GPIOA
#define MOTOR_A_EN_Pin               GPIO_PIN_4


/**********************�ƶ����B�������Ŷ���**************************/
#define ENABLE_MOTOR_B_RCC_CLOCK     __HAL_RCC_GPIOA_CLK_ENABLE();\
																	   __HAL_RCC_GPIOC_CLK_ENABLE();

//�ƶ����B����������Ŷ���
#define MOTOR_B_STEP_Prot            GPIOA
#define MOTOR_B_STEP_Pin             GPIO_PIN_1

//�ƶ����B����������Ŷ���
#define MOTOR_B_DIR_Prot             GPIOC
#define MOTOR_B_DIR_Pin              GPIO_PIN_3

//�ƶ����Bʹ�ܿ������Ŷ���
#define MOTOR_B_EN_Prot              GPIOA
#define MOTOR_B_EN_Pin               GPIO_PIN_3


/**********************��˿���C�������Ŷ���**************************/
#define ENABLE_MOTOR_C_RCC_CLOCK     __HAL_RCC_GPIOA_CLK_ENABLE();\
                                     __HAL_RCC_GPIOB_CLK_ENABLE();\
																	   __HAL_RCC_GPIOC_CLK_ENABLE();

//��˿���C����������Ŷ���
#define MOTOR_C_STEP_Prot            GPIOA
#define MOTOR_C_STEP_Pin             GPIO_PIN_5

//��˿���C����������Ŷ���
#define MOTOR_C_DIR_Prot             GPIOC
#define MOTOR_C_DIR_Pin              GPIO_PIN_4

//��˿���Cʹ�ܿ������Ŷ���
#define MOTOR_C_EN_Prot              GPIOB
#define MOTOR_C_EN_Pin               GPIO_PIN_0

#endif
