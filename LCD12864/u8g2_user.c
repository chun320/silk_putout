#include "u8g2_user.h"
#include "input_key.h"
#include "delay.h"
#include "gpio.h"
#include "steep.h"

#include "usart.h"

u8g2_t u8g2;

//���������б�־λ
uint8_t Motor_All_Run_flage = 0;
uint8_t Motor_All_En_flage = 0;

uint8_t Motor_A_Run_flage = 0;
uint8_t Motor_B_Run_flage = 0;
uint8_t Motor_C_Run_flage = 0;

uint8_t Motor_A_En_flage = 0;
uint8_t Motor_B_En_flage = 0;
uint8_t Motor_C_En_flage = 0;

//�������˵�Ŀ¼
const char *Main_Screen_Start[] = { "Start Motors", "Manual Control", "Speed Setting", "Save Speed", "Speed Init" };
const char *Main_Screen_Stop[] = { "Stop Motors", "Manual Control", "Speed Setting", "Save Speed", "Speed Init" };
const char *Control[] = {"Back", "MotorA_EN", "MotorB_EN", "MotorC_EN", "MotorA_RUN", "MotorB_RUN", "MotorC_RUN"};
const char *Setting[] = {"Back", "A_Motor_Speed", "B_Motor_Speed", "C_Motor_Speed"};
const char *menu_motorA[] = {"MotorA_Sp "};
const char *menu_motorB[] = {"MotorB_Sp "};
const char *menu_motorC[] = {"MotorC_Sp "};

uint8_t menu_main = sizeof(Main_Screen_Start)/sizeof(Main_Screen_Start[0]);        //��ȡ���˵�����
uint8_t menu_control = sizeof(Control)/sizeof(Control[0]);             //��ȡ���Ʋ˵�����
uint8_t menu_setting = sizeof(Setting)/sizeof(Setting[0]);             //��ȡ���Ʋ˵�����

uint8_t u8g2_gpio_and_delay_stm32(U8X8_UNUSED u8x8_t *u8x8, U8X8_UNUSED uint8_t msg, U8X8_UNUSED uint8_t arg_int, U8X8_UNUSED void *arg_ptr)
{
	switch(msg){
		//��ʼ������
		case U8X8_MSG_GPIO_AND_DELAY_INIT:
			LCD12864_Pin_Init();
		break;

		//���뼶��ʱ
		case U8X8_MSG_DELAY_MILLI:
			delay_ms(arg_int);
		break;
		
		//��ʱ10us
		case U8X8_MSG_DELAY_10MICRO:
			delay_us(10);
		break;
		
		//��ʱ100ns
		case U8X8_MSG_DELAY_100NANO:
			__NOP();
			__NOP();
		break;
		
		case U8X8_MSG_GPIO_SPI_CLOCK:
			if (arg_int) LCD_SCLK=1;
			else LCD_SCLK=0;
		break;
		
		case U8X8_MSG_GPIO_SPI_DATA:
			if (arg_int) LCD_SID=1;
			else LCD_SID=0;
		break;
		
		case U8X8_MSG_GPIO_CS:
			if (arg_int) LCD_RS=1	;
			else LCD_RS=0;
		break;
		
		default:
			return 0;
	}
	return 1;
}

void LCD12864_Init(void)
{
	u8g2_Setup_st7920_s_128x64_f(&u8g2, U8G2_R0, u8x8_byte_4wire_sw_spi, u8g2_gpio_and_delay_stm32);
  u8g2_InitDisplay(&u8g2);
  u8g2_SetPowerSave(&u8g2, 0);
	
	//LCD��ʼ����ɺ󣬿�ʼ��ʾ�˵�
	menu_update_flage = 1;
}

void LCD12864_UpDate(void)
{
	uint8_t i, h;
  u8g2_uint_t w, d;
	u8 menu_offset = 0;                 //��ʾ�˵�ƫ����
	
	u8g2_ClearDisplay(&u8g2);           //�����һ��LCD��Ļ
  u8g2_SetFont(&u8g2, u8g_font_7x14); //������ʾ��������
	u8g2_SetFontPosTop(&u8g2);          //������ʾ����ö�
  //��ȡ����߶�
	h = u8g2_GetFontAscent(&u8g2)-u8g2_GetFontDescent(&u8g2)+1;
  //��ȡ��ʾ�����
	w = u8g2_GetDisplayWidth(&u8g2);
	
	//ͨ����ǰ�˵����λ�ã�������ʾ�˵�ƫ����
	if(menu_current-LCD_LINE_NUM > -1)
	{
		menu_offset = menu_current-LCD_LINE_NUM+1;
	}
	
	switch(menu_show)
	{
		//������˵���ʾ
		case 0:
			for(i = 0; i+menu_offset < LCD_LINE_NUM+menu_offset; i++)
      {
				//������ַ����м���λ�ã�ʹ�ַ�������ʾ
				d =  Motor_All_Run_flage ? (w-u8g2_GetStrWidth(&u8g2, Main_Screen_Stop[i+menu_offset]))/2:\
															     (w-u8g2_GetStrWidth(&u8g2, Main_Screen_Start[i+menu_offset]))/2;

        //���û�����ɫΪʵ����ɫ
        u8g2_SetDrawColor(&u8g2, 1);

        //�б�ǰ�˵��Ƿ�ѡ�У�����ѡ������ʾΪ����ɫ
        if(i+menu_offset == menu_current)
        {
          //����һ������ʵ��
          u8g2_DrawBox(&u8g2, 0, i*h, w, h);
          //���û�����ɫΪ͸����ɫ
          u8g2_SetDrawColor(&u8g2, 0);
        }
				//����Ӧ���ַ���ʾ����Ļ��
				Motor_All_Run_flage ? u8g2_DrawStr(&u8g2, d, i*h, Main_Screen_Stop[i+menu_offset]):\
				                      u8g2_DrawStr(&u8g2, d, i*h, Main_Screen_Start[i+menu_offset]);
      }
			break;
			
		//���ƽ���˵���ʾ
		case 1:
			for(i = 0; i+menu_offset < LCD_LINE_NUM+menu_offset; i++)
      {
				//������ַ����м���λ�ã�ʹ�ַ�������ʾ
				d = (w-u8g2_GetStrWidth(&u8g2, Control[i+menu_offset]))/2;

        //���û�����ɫΪʵ����ɫ
        u8g2_SetDrawColor(&u8g2, 1);

        //�б�ǰ�˵��Ƿ�ѡ�У�����ѡ������ʾΪ����ɫ
        if(i+menu_offset == menu_current)
        {
          //����һ������ʵ��
          u8g2_DrawBox(&u8g2, 0, i*h, w, h);
          //���û�����ɫΪ͸����ɫ
          u8g2_SetDrawColor(&u8g2, 0);
        }
				switch(i+menu_offset)
				{
					case 0:
						//����Ӧ���ַ���ʾ����Ļ��
						u8g2_DrawStr(&u8g2, d, i*h, Control[i+menu_offset]);
					break;
						
					case 1:
						//����Ӧ���ַ���ʾ����Ļ��
						u8g2_DrawStr(&u8g2, d-30, i*h, Control[i+menu_offset]);
						u8g2_DrawStr(&u8g2, d+46, i*h, Motor_A_En_flage ? "Enable":"Disable");
					break;
					
					case 2:
						//����Ӧ���ַ���ʾ����Ļ��
						u8g2_DrawStr(&u8g2, d-30, i*h, Control[i+menu_offset]);
						u8g2_DrawStr(&u8g2, d+46, i*h, Motor_B_En_flage ? "Enable":"Disable");
					break;
					
					case 3:
						//����Ӧ���ַ���ʾ����Ļ��
						u8g2_DrawStr(&u8g2, d-30, i*h, Control[i+menu_offset]);
						u8g2_DrawStr(&u8g2, d+46, i*h, Motor_C_En_flage ? "Enable":"Disable");
					break;
					
					case 4:
						//����Ӧ���ַ���ʾ����Ļ��
						u8g2_DrawStr(&u8g2, d-26, i*h, Control[i+menu_offset]);
						u8g2_DrawStr(&u8g2, d+50, i*h, Motor_A_Run_flage ? "RUN":"STOP");
					break;
					
					case 5:
						//����Ӧ���ַ���ʾ����Ļ��
						u8g2_DrawStr(&u8g2, d-26, i*h, Control[i+menu_offset]);
						u8g2_DrawStr(&u8g2, d+50, i*h, Motor_B_Run_flage ? "RUN":"STOP");
					break;
					
					case 6:
						//����Ӧ���ַ���ʾ����Ļ��
						u8g2_DrawStr(&u8g2, d-26, i*h, Control[i+menu_offset]);
						u8g2_DrawStr(&u8g2, d+50, i*h, Motor_C_Run_flage ? "RUN":"STOP");
					break;
				}   
      }
		break;
		
		//���ƽ���˵���ʾ
		case 2:
			for(i = 0; i+menu_offset < LCD_LINE_NUM+menu_offset-1; i++)
      {
				//������ַ����м���λ�ã�ʹ�ַ�������ʾ
				d = (w-u8g2_GetStrWidth(&u8g2, Setting[i+menu_offset]))/2;

        //���û�����ɫΪʵ����ɫ
        u8g2_SetDrawColor(&u8g2, 1);

        //�б�ǰ�˵��Ƿ�ѡ�У�����ѡ������ʾΪ����ɫ
        if(i+menu_offset == menu_current)
        {
          //����һ������ʵ��
          u8g2_DrawBox(&u8g2, 0, i*h, w, h);
          //���û�����ɫΪ͸����ɫ
          u8g2_SetDrawColor(&u8g2, 0);
        }
				//����Ӧ���ַ���ʾ����Ļ��
				u8g2_DrawStr(&u8g2, d, i*h, Setting[i+menu_offset]);
      }
			break;
		
		case 3:  //��ʾ���A�ٶ����ý���
      for(i = 0; i < 1; i++)
      {
				//������ַ����м���λ�ã�ʹ�ַ�������ʾ
				d = (w-u8g2_GetStrWidth(&u8g2, menu_motorA[i]));

        //���û�����ɫΪʵ����ɫ
        u8g2_SetDrawColor(&u8g2, 1);
				
				//��float�͵�Motor_A_Speed_Nowת��Ϊstr��Motor_A_Speed_Str
				sprintf((char*)Motor_A_Speed_Str, "%.1f", Motor_A_Speed_Now);
				
				//����Ӧ���ַ���ʾ����Ļ��
				u8g2_DrawStr(&u8g2, 0, u8g2_GetDisplayHeight(&u8g2)/2-h/2, menu_motorA[i]);
				u8g2_DrawStr(&u8g2, d+10, u8g2_GetDisplayHeight(&u8g2)/2-h/2, (char*)Motor_A_Speed_Str);
				u8g2_DrawStr(&u8g2, d+10+u8g2_GetStrWidth(&u8g2, (char*)Motor_A_Speed_Str)+2, \
					            u8g2_GetDisplayHeight(&u8g2)/2-h/2, "r/min");
      }
    break;

    case 4:  //��ʾ���B�ٶ����ý���
      for(i = 0; i < 1; i++)
      {
				//������ַ����м���λ�ã�ʹ�ַ�������ʾ
				d = (w-u8g2_GetStrWidth(&u8g2, menu_motorB[i]));

        //���û�����ɫΪʵ����ɫ
        u8g2_SetDrawColor(&u8g2, 1);
				
				//��float�͵�Motor_B_Speed_Nowת��Ϊstr��Motor_B_Speed_Str
				sprintf((char*)Motor_B_Speed_Str, "%.1f", Motor_B_Speed_Now);
				
				//����Ӧ���ַ���ʾ����Ļ��
				u8g2_DrawStr(&u8g2, 0, u8g2_GetDisplayHeight(&u8g2)/2-h/2, menu_motorB[i]);
				u8g2_DrawStr(&u8g2, d+10, u8g2_GetDisplayHeight(&u8g2)/2-h/2, (char*)Motor_B_Speed_Str);
				u8g2_DrawStr(&u8g2, d+10+u8g2_GetStrWidth(&u8g2, (char*)Motor_B_Speed_Str)+2, \
					            u8g2_GetDisplayHeight(&u8g2)/2-h/2, "r/min");
      }
    break;

    case 5:  //��ʾ���C�ٶ����ý���
     for(i = 0; i < 1; i++)
      {
				//������ַ����м���λ�ã�ʹ�ַ�������ʾ
				d = (w-u8g2_GetStrWidth(&u8g2, menu_motorC[i]));

        //���û�����ɫΪʵ����ɫ
        u8g2_SetDrawColor(&u8g2, 1);
				
				//��float�͵�Motor_C_Speed_Nowת��Ϊstr��Motor_C_Speed_Str
				sprintf((char*)Motor_C_Speed_Str, "%.1f", Motor_C_Speed_Now);
				
				//����Ӧ���ַ���ʾ����Ļ��
				u8g2_DrawStr(&u8g2, 0, u8g2_GetDisplayHeight(&u8g2)/2-h/2, menu_motorC[i]);
				u8g2_DrawStr(&u8g2, d+10, u8g2_GetDisplayHeight(&u8g2)/2-h/2, (char*)Motor_C_Speed_Str);
				u8g2_DrawStr(&u8g2, d+10+u8g2_GetStrWidth(&u8g2, (char*)Motor_C_Speed_Str)+2, \
					            u8g2_GetDisplayHeight(&u8g2)/2-h/2, "r/min");
      }
    break;
	
	}
	u8g2_SendBuffer(&u8g2);
	menu_update_flage = 0;
}
