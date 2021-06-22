#ifndef __U8G2_USER_H
#define __U8G2_USER_H

#include "u8g2.h"

#define LCD_LINE_NUM     5        //LCD12864显示菜单行数

#define LCD_RS	         PEout(3)   //LCD12864片选引脚
#define LCD_SCLK         PGout(14)  //LCD12864时钟引脚
#define LCD_SID          PGout(12)  //LCD12864数据引脚

extern uint8_t menu_main;
extern uint8_t menu_control;
extern uint8_t menu_setting;
extern uint8_t Motor_All_Run_flage;
extern uint8_t Motor_All_En_flage;
extern uint8_t Motor_A_Run_flage;
extern uint8_t Motor_B_Run_flage;
extern uint8_t Motor_C_Run_flage;
extern uint8_t Motor_A_En_flage;
extern uint8_t Motor_B_En_flage;
extern uint8_t Motor_C_En_flage;

extern u8g2_t u8g2;

void LCD12864_Init(void);
void LCD12864_UpDate(void);

#endif

