#include "steep.h"
#include "w25qxx.h"
#include "string.h"
#include "usart.h"
#include "stdlib.h"

TIM_HandleTypeDef TIM9_Handler;      	//��ʱ��9���
TIM_HandleTypeDef TIM10_Handler;      //��ʱ��10���
TIM_HandleTypeDef TIM11_Handler;      //��ʱ��11���

//��ǰ��˿���A��ת��
float Motor_A_Speed_Now = 0.0;
//��ǰ��˿���B��ת��
float Motor_B_Speed_Now = 0.0;
//��ǰ�ƶ����C��ת��
float Motor_C_Speed_Now = 0.0;

//������Ƶ����ʱ���Զ���װ��ֵ�Ĵ���
u16 Motor_A_Step_ARR = 0;
u16 Motor_B_Step_ARR = 0;
u16 Motor_C_Step_ARR = 0;

//�������ٶ����ݣ��ַ�����ʽ��
u8 Motor_A_Speed_Str[4];
u8 Motor_B_Speed_Str[4];
u8 Motor_C_Speed_Str[4];

//��������ʼ��ʱ�ٶ����ݣ��ַ�����ʽ��
u8 Motor_Init_Speed_Str[4] = {"0.0"};

void Steep_Init(void)
{ 
	TIM9_Handler.Instance=TIM9;                        //������ʱ��9
	TIM9_Handler.Init.Prescaler=84-1;                 //��Ƶϵ��
	TIM9_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;  //������ʵ��ģʽ
	TIM9_Handler.Init.Period=60000-1;                    //�Զ�װ��ֵ
	HAL_TIM_Base_Init(&TIM9_Handler);
  
	__HAL_TIM_CLEAR_IT(&TIM9_Handler, TIM_IT_UPDATE);  //����TIM����ʱ���жϱ�ʶ
	
	
	TIM10_Handler.Instance=TIM10;                        //������ʱ��10
	TIM10_Handler.Init.Prescaler=840-1;                 //��Ƶϵ��
	TIM10_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;  //������ʵ��ģʽ
	TIM10_Handler.Init.Period=6000-1;                    //�Զ�װ��ֵ
	HAL_TIM_Base_Init(&TIM10_Handler);
  
	__HAL_TIM_CLEAR_IT(&TIM10_Handler, TIM_IT_UPDATE);  //����TIM����ʱ���жϱ�ʶ
	
	TIM11_Handler.Instance=TIM11;                        //������ʱ��11
	TIM11_Handler.Init.Prescaler=840-1;                 //��Ƶϵ��
	TIM11_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;  //������ʵ��ģʽ
	TIM11_Handler.Init.Period=6000-1;                    //�Զ�װ��ֵ
	HAL_TIM_Base_Init(&TIM11_Handler);
  
	__HAL_TIM_CLEAR_IT(&TIM11_Handler, TIM_IT_UPDATE);  //����TIM����ʱ���жϱ�ʶ
}

//��ʱ��9�жϷ�����
void TIM1_BRK_TIM9_IRQHandler(void)
{
	HAL_TIM_IRQHandler(&TIM9_Handler);       //���ö�ʱ���жϴ����ú���
}

//��ʱ��10�жϷ�����
void TIM1_UP_TIM10_IRQHandler(void)
{
	HAL_TIM_IRQHandler(&TIM10_Handler);       //���ö�ʱ���жϴ����ú���
}

//��ʱ��11�жϷ�����
void TIM1_TRG_COM_TIM11_IRQHandler(void)
{
	HAL_TIM_IRQHandler(&TIM11_Handler);       //���ö�ʱ���жϴ����ú���
}

//��ȡW25Q��FLASH�и�������ٶ�����
void Read_Motor_Speed(void)
{
	W25QXX_Read(Motor_A_Speed_Str,W25Q_Motor_A_Speed_Add,4);
	W25QXX_Read(Motor_B_Speed_Str,W25Q_Motor_B_Speed_Add,4);
	W25QXX_Read(Motor_C_Speed_Str,W25Q_Motor_C_Speed_Add,4);
	
	//��ȡ��˿���A�������ٶ�
	if(strcmp((char*)Motor_A_Speed_Str, "0.0") == 0)
		Motor_A_Speed_Now = MOTOR_A_SPEED;
	else
		Motor_A_Speed_Now = atof((char*)Motor_A_Speed_Str);
	
	
	//��ȡ��˿���B�������ٶ�
	if(strcmp((char*)Motor_B_Speed_Str, "0.0") == 0)
		Motor_B_Speed_Now = MOTOR_B_SPEED;
	else
		Motor_B_Speed_Now = atof((char*)Motor_B_Speed_Str);
	
	
	//��ȡ�ƶ����C�������ٶ�
	if(strcmp((char*)Motor_C_Speed_Str, "0.0") == 0)
		Motor_C_Speed_Now = MOTOR_C_SPEED;
	else
		Motor_C_Speed_Now = atof((char*)Motor_C_Speed_Str);
	
	
	//�����������ƶ�ʱ�����Զ���װ��ֵ������ת��Ϊ����(10*f + 5)/10;
	Motor_A_Step_ARR = ((10*1000*1000*(1/((Motor_A_Speed_Now*MOTOR_A_STEP)/60)))+5)/10;
	Motor_B_Step_ARR = ((10*100*1000*(1/((Motor_B_Speed_Now*MOTOR_B_STEP)/60)))+5)/10;
	Motor_C_Step_ARR = ((10*100*1000*(1/((Motor_C_Speed_Now*MOTOR_C_STEP)/60)))+5)/10;
	
	//printf("Motor_A_Speed_Now->%.1f\r\n", Motor_A_Speed_Now);
	//printf("Motor_B_Speed_Now->%.1f\r\n", Motor_B_Speed_Now);
	//printf("Motor_C_Speed_Now->%.1f\r\n", Motor_C_Speed_Now);
	
	//Ϊ��ʱ�������Զ���װ��ֵ
	__HAL_TIM_SET_AUTORELOAD(&TIM9_Handler, Motor_A_Step_ARR-1);
	__HAL_TIM_SET_AUTORELOAD(&TIM10_Handler, Motor_B_Step_ARR-1);
	__HAL_TIM_SET_AUTORELOAD(&TIM11_Handler, Motor_C_Step_ARR-1);
}


