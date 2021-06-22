#ifndef _STEEP_H
#define _STEEP_H

#include "map_pin.h"
#include "sys.h"

extern TIM_HandleTypeDef TIM9_Handler;      	//��ʱ��9���
extern TIM_HandleTypeDef TIM10_Handler;      	//��ʱ��10���
extern TIM_HandleTypeDef TIM11_Handler;      	//��ʱ��11���
extern TIM_HandleTypeDef TIM12_Handler;      	//��ʱ��12���

//��ǰ��˿���A��ת��
extern float Motor_A_Speed_Now;
//��ǰ�ƶ����B��ת��
extern float Motor_B_Speed_Now;
//��ǰ��˿���C��ת��
extern float Motor_C_Speed_Now;

//��������ʼ��ʱ�ٶ����ݣ��ַ�����ʽ��
extern u8 Motor_Init_Speed_Str[4];

//�������ٶ����ݣ��ַ�����ʽ��
extern u8 Motor_A_Speed_Str[4];
extern u8 Motor_B_Speed_Str[4];
extern u8 Motor_C_Speed_Str[4];

//������Ƶ����ʱ���Զ���װ��ֵ�Ĵ���
extern u16 Motor_A_Step_ARR;
extern u16 Motor_B_Step_ARR;
extern u16 Motor_C_Step_ARR;

//��˿���A����ֵ
#define MOTOR_A_STEP   10000*4.25f //��˿������ٱ�Ϊ��4.25
//�ƶ����B����ֵ
#define MOTOR_B_STEP   10000
//��˿���C����ֵ
#define MOTOR_C_STEP   10000

//��˿���A��ʼ��ת��(r/min)
#define MOTOR_A_SPEED   							1.0
//�ƶ����B��ʼ��ת��(r/min)
#define MOTOR_B_SPEED   							1.0
//��˿���C��ʼ��ת��(r/min)
#define MOTOR_C_SPEED   							1.0

//��˿���A�ٶ�ֵ�洢��ַ
#define W25Q_Motor_A_Speed_Add      	0
//�ƶ����B�ٶ�ֵ�洢��ַ
#define W25Q_Motor_B_Speed_Add      	4
//��˿���C�ٶ�ֵ�洢��ַ
#define W25Q_Motor_C_Speed_Add      	8


//��˿���A��������
#define Motor_A_Pulse  PAout(2)
#define Motor_A_Dir    PCout(2)
#define Motor_A_EN     PAout(4)
//��˿���B��������
#define Motor_B_Pulse  PAout(5)
#define Motor_B_Dir    PCout(4)
#define Motor_B_EN     PBout(0)
//�ƶ����C��������
#define Motor_C_Pulse  PAout(1)
#define Motor_C_Dir    PCout(3)
#define Motor_C_EN     PAout(3)

//�ƶ����C��λ��������ӳ��
#define END_STOP_L     PEin(5)
#define END_STOP_R     PEin(6)

void Steep_Init(void);
void Read_Motor_Speed(void);

#endif
