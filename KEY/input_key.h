#ifndef __INPUT_KEY_H
#define __INPUT_KEY_H

#include "sys.h"

#define Encoder_KEY   PGin(13)
#define Encoder_A     PGin(11)
#define Encoder_B     PEin(2)
#define BEEP_ON       PCout(1) = 1
#define BEEP_OFF      PCout(1) = 0

extern uint8_t menu_show;         //��ʾ���˵������ַ�Ĵ���
extern uint8_t control_menu_show; //��ʾ���ƻ����ַ�Ĵ���

extern int8_t menu_current;       //���õ�ǰ�˵����λ��
extern uint8_t menu_update_flage; //��Ļˢ�±�־λ


extern u8 KEY_IsPress;
extern u8 Encoder_Data;

extern u8 Beep_run;

void Input_Encoder_Init(void);

#endif

