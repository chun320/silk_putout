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
	HAL_Init();                    	//��ʼ��HAL��    
	Stm32_Clock_Init(336,8,2,7);  	//����ʱ��,168Mhz
	delay_init(168);               	//��ʼ����ʱ����
	uart_init(115200);              //��ʼ������1
	LED_Init();                     //��ʼ��LED�˿�
	LCD12864_Init();                //��ʼ��LCD12864��ʾ�˿�
	Input_Encoder_Init();           //��ʼ�����밴ť�˿�
	EndStop_Init();                 //��ʼ���ƶ����B��λ�˿�
	Motor_Intt();                   //��ʼ��������ƶ˿�
	W25QXX_Init();                  //��ʼ��FLASH�洢���˿�
	Read_Motor_Speed();             //������ȡ����ٶ�
	
	//���õ����ʼʧ��
	Motor_A_EN = 1;
  Motor_B_EN = 1;
	Motor_C_EN = 1;
	
	//���õ����ʼ����
	Motor_A_Dir = 0;
	Motor_B_Dir = 0;
	Motor_C_Dir = 0;
	
	delay_ms(500);

	while(1)
	{
		//����LCD12864��ʾ
		if(menu_update_flage)
		{
			LCD12864_UpDate();
		}
		
		//�����ƶ������λ���ر�����
		if(END_STOP_L == 1)
		{
			//������ʱ
			delay_ms(10);
			if(END_STOP_L == 1)
				Motor_C_Dir = 1;
		}
		//�����ƶ������λ���ر�����
		if(END_STOP_R == 1)	
		{
			//������ʱ
			delay_ms(10);
			if(END_STOP_R == 1)
				Motor_C_Dir = 0;
		}
		
		
		//����������
		if(Beep_run)
		{
			BEEP_ON;
			delay_ms(40);
			BEEP_OFF;
			Beep_run = 0;
		}
	}                                      	
}	
