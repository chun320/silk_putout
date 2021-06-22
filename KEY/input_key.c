#include "input_key.h"
#include "u8g2_user.h"
#include "usart.h"
#include "map_pin.h"
#include "delay.h"
#include "steep.h"
#include "w25qxx.h"


TIM_HandleTypeDef TIM7_Handler;      //��ʱ��7���

uint8_t menu_show;     			    //0: ��ʾ���˵������ַ�Ĵ���
uint8_t control_menu_show;      //1: ��ʾ���ƻ����ַ�Ĵ���

int8_t menu_current = 0;        //���õ�ǰ�˵����λ��
uint8_t menu_update_flage;      //��Ļˢ�±�־λ

u8 Encoder_KEY_nu = 0;          //��������������������
u8 Encoder_KEY_flage = 0;       //�������������ͷ�̧����λ
u8 Encoder_AB_nu = 0;           //��������ת����������
u8 Encoder_AB_flage = 0;        //��������ת���������λ

u8 Beep_run = 0;

//���������û�����ʱ��7�жϳ�ʼ��
//2ms����һ���ж�,���Լ�ⰴ�����µ�ʱ��
void TIM7_Init(void)
{
	TIM7_Handler.Instance=TIM7;             //������ʱ��7
	TIM7_Handler.Init.Prescaler=840-1;      //��Ƶϵ��
	TIM7_Handler.Init.Period=200-1;         //�Զ�װ��ֵ
	HAL_TIM_Base_Init(&TIM7_Handler);
    
	HAL_TIM_Base_Start_IT(&TIM7_Handler); //ʹ�ܶ�ʱ��7�Ͷ�ʱ��7�����жϣ�TIM_IT_UPDATE
}


void Input_Encoder_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  
  ENABLE_KEY_RCC_CLOCK;               //�������ư���ʱ��
	
	//��ʼ����ת������A����������
	GPIO_InitStruct.Pin=KEY_A_Pin;                			
	GPIO_InitStruct.Mode=GPIO_MODE_IT_RISING_FALLING;      //�½��ش����ⲿ�ж�
	GPIO_InitStruct.Pull=GPIO_PULLUP;
	HAL_GPIO_Init(KEY_A_Prot,&GPIO_InitStruct);
	
	//��ʼ����ת������B����������
	GPIO_InitStruct.Pin=KEY_B_Pin;                			
	GPIO_InitStruct.Mode=GPIO_MODE_INPUT;                  //��������ģʽ
	GPIO_InitStruct.Pull=GPIO_PULLUP;
	HAL_GPIO_Init(KEY_B_Prot,&GPIO_InitStruct);
	
	//��ʼ����������������
	GPIO_InitStruct.Pin = KEY_BEEP_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(KEY_BEEP_Prot, &GPIO_InitStruct);	
	
	//�����ж���15_10
	HAL_NVIC_SetPriority(EXTI15_10_IRQn,2,0);       //��ռ���ȼ�Ϊ2�������ȼ�Ϊ0
	HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);             //ʹ���ж���	
	
	//��ʼ���������Ķ�ʱ��7
	TIM7_Init();
}

//�˺����ᱻHAL_TIM_Base_Init()��������
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
	if(htim->Instance==TIM7)
	{
		GPIO_InitTypeDef GPIO_InitStruct;
		
		ENABLE_KEY_RCC_CLOCK;                   //�������ư���ʱ��
		__HAL_RCC_TIM7_CLK_ENABLE();            //ʹ��TIM7ʱ��
		
		//��ʼ����ת����������
		GPIO_InitStruct.Pin=KEY_EN_Pin;                			
		GPIO_InitStruct.Mode=GPIO_MODE_INPUT;                  //��������ģʽ
		GPIO_InitStruct.Pull=GPIO_PULLUP;
		HAL_GPIO_Init(KEY_EN_Prot,&GPIO_InitStruct);
		
		HAL_NVIC_SetPriority(TIM7_IRQn,2,1);    //�����ж����ȼ�����ռ���ȼ�1�������ȼ�3
		HAL_NVIC_EnableIRQ(TIM7_IRQn);          //����ITM7�ж�   
	}
	
	if(htim->Instance==TIM9)
	{
		__HAL_RCC_TIM9_CLK_ENABLE();
		
		HAL_NVIC_SetPriority(TIM1_BRK_TIM9_IRQn,1,0);    //�����ж����ȼ�����ռ���ȼ�1�������ȼ�0
		HAL_NVIC_EnableIRQ(TIM1_BRK_TIM9_IRQn);          //����ITM9�ж�  
	}
	
	if(htim->Instance==TIM10)
	{
		__HAL_RCC_TIM10_CLK_ENABLE();
		
		HAL_NVIC_SetPriority(TIM1_UP_TIM10_IRQn,1,1);    //�����ж����ȼ�����ռ���ȼ�1�������ȼ�1
		HAL_NVIC_EnableIRQ(TIM1_UP_TIM10_IRQn);          //����ITM9�ж�  
	}
	
	if(htim->Instance==TIM11)
	{
		__HAL_RCC_TIM11_CLK_ENABLE();
		
		HAL_NVIC_SetPriority(TIM1_TRG_COM_TIM11_IRQn,1,2);    //�����ж����ȼ�����ռ���ȼ�1�������ȼ�2
		HAL_NVIC_EnableIRQ(TIM1_TRG_COM_TIM11_IRQn);          //����ITM9�ж�  
	}
}

//��ʱ��7�жϷ�����
void TIM7_IRQHandler(void)
{
	HAL_TIM_IRQHandler(&TIM7_Handler);       //���ö�ʱ���жϴ����ú���
}

//�ⲿ�ж�10-15�жϷ�����
void EXTI15_10_IRQHandler(void)
{
	HAL_GPIO_EXTI_IRQHandler(KEY_A_Pin);		//�����ⲿ�жϴ����ú���
}

//��ʱ���ص���������ʱ���жϷ���������
//�������´������Ͱ�����������
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim==(&TIM7_Handler))
	{
		if(Encoder_KEY != 0){
			Encoder_KEY_nu = 0;        //����������������ļ���
			Encoder_KEY_flage = 1;     //��ǰ������ͷ�
		}
		else{
			Encoder_KEY_nu++;
			//��������8�μ����������£�����һ��ȷ�ϰ����ѱ��ͷ�
			if(Encoder_KEY_nu>=8 && Encoder_KEY_flage)
			{
				Encoder_KEY_flage = 0;     //��ǰ���������
				switch(menu_show)
				{
					//���˵����ѡ��ȷ�Ͽ���
					case 0:
						switch(menu_current)
						{
							case 0:
								//�������ʹ�ܱ�־
								if(Motor_All_En_flage ==0)
								{
									//�򿪸������ʹ�ܶ˿�
									Motor_A_EN = 0;
									Motor_B_EN = 0;
									Motor_C_EN = 0;
									//���ø����ʹ�ܱ�־λ
									Motor_A_En_flage = 1;
									Motor_B_En_flage = 1;
									Motor_C_En_flage = 1;
									Motor_All_En_flage =1;
								}
								else
								{
									//�رո������ʹ�ܶ˿�
									Motor_A_EN = 1;
									Motor_B_EN = 1;
									Motor_C_EN = 1;
									//���ø����ʹ�ܱ�־λ
									Motor_A_En_flage = 0;
									Motor_B_En_flage = 0;
									Motor_C_En_flage = 0;
									Motor_All_En_flage = 0;
								}	
							
								//����������б�־
								if(Motor_All_Run_flage ==0)
								{
									//���ø�������б�־λ
									Motor_A_Run_flage = 1;
									Motor_B_Run_flage = 1;
									Motor_C_Run_flage = 1;
									Motor_All_Run_flage =1;
									HAL_TIM_Base_Start_IT(&TIM9_Handler); //ʹ�ܶ�ʱ��9
									HAL_TIM_Base_Start_IT(&TIM10_Handler); //ʹ�ܶ�ʱ��10
									HAL_TIM_Base_Start_IT(&TIM11_Handler); //ʹ�ܶ�ʱ��11
								}
								else
								{
									//���ø�������б�־λ
									Motor_A_Run_flage = 0;
									Motor_B_Run_flage = 0;
									Motor_C_Run_flage = 0;
									Motor_All_Run_flage = 0;
									HAL_TIM_Base_Stop_IT(&TIM9_Handler);		//�رն�ʱ��9
									HAL_TIM_Base_Stop_IT(&TIM10_Handler);		//�رն�ʱ��10
									HAL_TIM_Base_Stop_IT(&TIM11_Handler);		//�رն�ʱ��11
								}	
								menu_show = 0;      //��ʾ�����ƽ���˵�
								menu_current = 0;		//���ù��Ϊ��ʼλ��
							break;
								
							case 1:
								menu_show = 1;      //ת���ֶ����ƽ���˵�
								menu_current = 0;		//���ù��Ϊ��ʼλ��
							break;
							
							case 2:
								menu_show = 2;      //ת������ٶ����ý���˵�
								menu_current = 0;		//���ù��Ϊ��ʼλ��
							break;
							
							case 3:/**���浱ǰ���������ת�ٶ�**/
								sprintf((char*)Motor_A_Speed_Str, "%.1f", Motor_A_Speed_Now); 
								sprintf((char*)Motor_B_Speed_Str, "%.1f", Motor_B_Speed_Now);
								sprintf((char*)Motor_C_Speed_Str, "%.1f", Motor_C_Speed_Now);
							
								W25QXX_Write((u8*)Motor_A_Speed_Str,W25Q_Motor_A_Speed_Add,4);
								W25QXX_Write((u8*)Motor_B_Speed_Str,W25Q_Motor_B_Speed_Add,4);
								W25QXX_Write((u8*)Motor_C_Speed_Str,W25Q_Motor_C_Speed_Add,4);
							break;
							
							case 4:/**��ʼ�����������ת�ٶ�**/
								W25QXX_Write((u8*)Motor_Init_Speed_Str,W25Q_Motor_A_Speed_Add,4);
								W25QXX_Write((u8*)Motor_Init_Speed_Str,W25Q_Motor_B_Speed_Add,4);
								W25QXX_Write((u8*)Motor_Init_Speed_Str,W25Q_Motor_C_Speed_Add,4);
							break;
						}
					break;
						
					//�ֶ����ƽ�����ѡ��ȷ�Ͽ���
					case 1:
						switch(menu_current)
						{
							case 0:
								menu_show = 0;      //���ؿ��ƽ���˵�
								menu_current = 0;		//���ù��Ϊ��ʼλ��
							break;
							
							case 1:
								Motor_A_En_flage =! Motor_A_En_flage;
								if(Motor_A_En_flage == 1)
									Motor_A_EN = 0; //ʹ�ܳ�˿���A
								else
									Motor_A_EN = 1; //�رճ�˿���A
							break;
							
							case 2:
								Motor_B_En_flage =! Motor_B_En_flage;
								if(Motor_B_En_flage == 1)
									Motor_B_EN = 0; //ʹ�ܼ�˿���B
								else
									Motor_B_EN = 1; //�رռ�˿���B
							break;
							
							case 3:
								Motor_C_En_flage =! Motor_C_En_flage;
								if(Motor_C_En_flage == 1)
									Motor_C_EN = 0; //ʹ���ƶ����C
								else
									Motor_C_EN = 1; //�ر��ƶ����C
							break;
								
							case 4:
								Motor_A_Run_flage =! Motor_A_Run_flage;
								if(Motor_A_Run_flage == 1)
									HAL_TIM_Base_Start_IT(&TIM9_Handler); //ʹ�ܶ�ʱ��9
								else
									HAL_TIM_Base_Stop_IT(&TIM9_Handler);		//�رն�ʱ��9
							break;
								
							case 5:
								Motor_B_Run_flage =! Motor_B_Run_flage;
								if(Motor_B_Run_flage == 1)
									HAL_TIM_Base_Start_IT(&TIM10_Handler); //ʹ�ܶ�ʱ��10
								else
									HAL_TIM_Base_Stop_IT(&TIM10_Handler);		//�رն�ʱ��10
							break;
								
							case 6:
								Motor_C_Run_flage =! Motor_C_Run_flage;
								if(Motor_C_Run_flage == 1)
									HAL_TIM_Base_Start_IT(&TIM11_Handler); //ʹ�ܶ�ʱ��11
								else
									HAL_TIM_Base_Stop_IT(&TIM11_Handler);		//�رն�ʱ��11
							break;	
						}	
					break;
					
					//����ٶ����ý�����ѡ��ȷ�Ͽ���
					case 2:
						switch(menu_current)
						{
							case 0:/*������һ��*/
								menu_show = 0;      //���ؿ��ƽ���˵�
								menu_current = 0;		//���ù��Ϊ��ʼλ��
							break;
							
							case 1:/*ת�����A�ٶ����ý���*/
								menu_show = 3;      //ת����˿���A�ٶ����ý���
							break;
							
							case 2:/*ת�����B�ٶ����ý���*/
								menu_show = 4;      //ת����˿���B�ٶ����ý���
							break;
							
							case 3:/*ת�����C�ٶ����ý���*/
								menu_show = 5;      //ת���ƶ����C�ٶ����ý���
							break;
						}
					break;
						
					case 3:
						menu_show = 2;      //���ؿ��ƽ���˵�
						menu_current = 0;		//���ù��Ϊ��ʼλ��
					break;
					
					case 4:
						menu_show = 2;      //���ؿ��ƽ���˵�
						menu_current = 0;		//���ù��Ϊ��ʼλ��
					break;
					
					case 5:
						menu_show = 2;      //���ؿ��ƽ���˵�
						menu_current = 0;		//���ù��Ϊ��ʼλ��
					break;
					
				}	
				//����������
				Beep_run = 1;
				
				//����LCD12864��ʾ�˵�
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


//�ⲿ�жϷ������
//��ת���뿪�ش�����
//GPIO_Pin:�ж����ź�
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	switch(GPIO_Pin)
	{
		//��ת��������ť��ת��
		case KEY_A_Pin:
			//��һ���жϣ�����A�����½��أ�������������
			if(Encoder_A==0 && Encoder_AB_nu==0)
			{
				Encoder_AB_flage=0;
				if(Encoder_B == 1)     //����B�࣬�趨��־
					Encoder_AB_flage=1;
				Encoder_AB_nu=1;
			}
			
			//�ڶ����жϣ�����A���������أ�ȷ�ϱ�����ת�������жϷ���
			if(Encoder_A==1 && Encoder_AB_nu==1)
			{
				//��������ʱ��ת��
				if(Encoder_B==0 && Encoder_AB_flage==1)
				{
					switch(menu_show)
					{
						//���˵���������ƶ�����
						case 0:   //������˵�
							menu_current--;
							if(Motor_All_Run_flage==1 && menu_current==1)
								menu_current--;
							if(menu_current<0)
								menu_current = menu_main-1;
						break;
							
						case 1:  //���ƽ���˵�
							menu_current--;
							if(menu_current<0)
								menu_current = menu_control-1;
						break;
							
						case 2:  //����ٶ����ý���˵�
							menu_current--;
							if(menu_current<0)
								menu_current = menu_setting-1;
						break;
							
						case 3:  //��˿���A�ٵ��ڶȽ���
							Motor_A_Speed_Now += 0.1f;
							if(Motor_A_Speed_Now >5.0f)
								Motor_A_Speed_Now = 5.0f;
							Motor_A_Step_ARR = ((10*1000*1000*(1/((Motor_A_Speed_Now*MOTOR_A_STEP)/60)))+5)/10;
							//�����Զ���װ��ֵ
							__HAL_TIM_SET_AUTORELOAD(&TIM9_Handler, Motor_A_Step_ARR-1);
						
							//printf("Motor_A_Step_ARR->%d\r\n", Motor_A_Step_ARR);
						break;
						
						case 4:  //��˿���B�ٵ��ڶȽ���
							Motor_B_Speed_Now += 0.1f;
							if(Motor_B_Speed_Now >5.0f)
								Motor_B_Speed_Now = 5.0f;
							Motor_B_Step_ARR = ((10*100*1000*(1/((Motor_B_Speed_Now*MOTOR_B_STEP)/60)))+5)/10;
							//�����Զ���װ��ֵ
							__HAL_TIM_SET_AUTORELOAD(&TIM10_Handler, Motor_B_Step_ARR-1);
						
							//printf("Motor_B_Step_ARR->%d\r\n", Motor_B_Step_ARR);
						break;
						
						case 5:  //�ƶ����C�ٵ��ڶȽ���
							Motor_C_Speed_Now += 0.1f;
							if(Motor_C_Speed_Now >5.0f)
								Motor_C_Speed_Now = 5.0f;
							Motor_C_Step_ARR = ((10*100*1000*(1/((Motor_C_Speed_Now*MOTOR_C_STEP)/60)))+5)/10;
							//�����Զ���װ��ֵ
							__HAL_TIM_SET_AUTORELOAD(&TIM11_Handler, Motor_C_Step_ARR-1);
						
							//printf("Motor_C_Step_ARR->%d\r\n", Motor_C_Step_ARR);
						break;
					}
					
					//����LCD12864��ʾ�˵�
					menu_update_flage = 1;
				}
				
				//������˳ʱ��ת��
				if(Encoder_B==1 && Encoder_AB_flage==0)
				{
					switch(menu_show)
					{
						//���˵���������ƶ�����
						case 0:                     //���˵�
							menu_current++;
							if(Motor_All_Run_flage==1 && menu_current==1)
								menu_current++;
							if(menu_current>=menu_main)
								menu_current = 0;
						break;
							
						case 1:  //���ƽ���˵�
							menu_current++;
							if(menu_current>=menu_control)
								menu_current = 0;
						break;
						
						case 2:  //����ٶ����ý���˵�
							menu_current++;
							if(menu_current>=menu_setting)
								menu_current = 0;
						break;

						case 3:  //��˿���A�ٵ��ڶȽ���
							Motor_A_Speed_Now -= 0.1f;
							if(Motor_A_Speed_Now <0.1f)
								Motor_A_Speed_Now = 0.1f;
							Motor_A_Step_ARR = ((10*1000*1000*(1/((Motor_A_Speed_Now*MOTOR_A_STEP)/60)))+5)/10;
							//�����Զ���װ��ֵ
							__HAL_TIM_SET_AUTORELOAD(&TIM9_Handler, Motor_A_Step_ARR-1);
						
							//printf("Motor_A_Step_ARR->%d\r\n", Motor_A_Step_ARR);
						break;
						
						case 4:  //��˿���B�ٵ��ڶȽ���
							Motor_B_Speed_Now -= 0.1f;
							if(Motor_B_Speed_Now <0.1f)
								Motor_B_Speed_Now = 0.1f;
							Motor_B_Step_ARR = ((10*100*1000*(1/((Motor_B_Speed_Now*MOTOR_B_STEP)/60)))+5)/10;
							//�����Զ���װ��ֵ
							__HAL_TIM_SET_AUTORELOAD(&TIM10_Handler, Motor_B_Step_ARR-1);
						
							//printf("Motor_B_Step_ARR->%d\r\n", Motor_B_Step_ARR);
						break;
						
						case 5:  //�ƶ����C�ٵ��ڶȽ���
							Motor_C_Speed_Now -= 0.1f;
							if(Motor_C_Speed_Now <0.1f)
								Motor_C_Speed_Now = 0.1f;
							Motor_C_Step_ARR = ((10*100*1000*(1/((Motor_C_Speed_Now*MOTOR_C_STEP)/60)))+5)/10;
							//�����Զ���װ��ֵ
							__HAL_TIM_SET_AUTORELOAD(&TIM11_Handler, Motor_C_Step_ARR-1);
						
							//printf("Motor_C_Step_ARR->%d\r\n", Motor_C_Step_ARR);
						break;
					}
					
					//����LCD12864��ʾ�˵�
					menu_update_flage = 1;
				}
				Encoder_AB_nu = 0;
			}
		break;
	}
}


