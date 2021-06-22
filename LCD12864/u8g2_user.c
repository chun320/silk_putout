#include "u8g2_user.h"
#include "input_key.h"
#include "delay.h"
#include "gpio.h"
#include "steep.h"

#include "usart.h"

u8g2_t u8g2;

//各轴电机运行标志位
uint8_t Motor_All_Run_flage = 0;
uint8_t Motor_All_En_flage = 0;

uint8_t Motor_A_Run_flage = 0;
uint8_t Motor_B_Run_flage = 0;
uint8_t Motor_C_Run_flage = 0;

uint8_t Motor_A_En_flage = 0;
uint8_t Motor_B_En_flage = 0;
uint8_t Motor_C_En_flage = 0;

//定义主菜单目录
const char *Main_Screen_Start[] = { "Start Motors", "Manual Control", "Speed Setting", "Save Speed", "Speed Init" };
const char *Main_Screen_Stop[] = { "Stop Motors", "Manual Control", "Speed Setting", "Save Speed", "Speed Init" };
const char *Control[] = {"Back", "MotorA_EN", "MotorB_EN", "MotorC_EN", "MotorA_RUN", "MotorB_RUN", "MotorC_RUN"};
const char *Setting[] = {"Back", "A_Motor_Speed", "B_Motor_Speed", "C_Motor_Speed"};
const char *menu_motorA[] = {"MotorA_Sp "};
const char *menu_motorB[] = {"MotorB_Sp "};
const char *menu_motorC[] = {"MotorC_Sp "};

uint8_t menu_main = sizeof(Main_Screen_Start)/sizeof(Main_Screen_Start[0]);        //获取主菜单个数
uint8_t menu_control = sizeof(Control)/sizeof(Control[0]);             //获取控制菜单个数
uint8_t menu_setting = sizeof(Setting)/sizeof(Setting[0]);             //获取控制菜单个数

uint8_t u8g2_gpio_and_delay_stm32(U8X8_UNUSED u8x8_t *u8x8, U8X8_UNUSED uint8_t msg, U8X8_UNUSED uint8_t arg_int, U8X8_UNUSED void *arg_ptr)
{
	switch(msg){
		//初始化引脚
		case U8X8_MSG_GPIO_AND_DELAY_INIT:
			LCD12864_Pin_Init();
		break;

		//毫秒级延时
		case U8X8_MSG_DELAY_MILLI:
			delay_ms(arg_int);
		break;
		
		//延时10us
		case U8X8_MSG_DELAY_10MICRO:
			delay_us(10);
		break;
		
		//延时100ns
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
	
	//LCD初始化完成后，开始显示菜单
	menu_update_flage = 1;
}

void LCD12864_UpDate(void)
{
	uint8_t i, h;
  u8g2_uint_t w, d;
	u8 menu_offset = 0;                 //显示菜单偏移量
	
	u8g2_ClearDisplay(&u8g2);           //清除上一次LCD屏幕
  u8g2_SetFont(&u8g2, u8g_font_7x14); //设置显示文字字体
	u8g2_SetFontPosTop(&u8g2);          //设置显示光标置顶
  //获取字体高度
	h = u8g2_GetFontAscent(&u8g2)-u8g2_GetFontDescent(&u8g2)+1;
  //获取显示器宽度
	w = u8g2_GetDisplayWidth(&u8g2);
	
	//通过当前菜单光标位置，计算显示菜单偏移量
	if(menu_current-LCD_LINE_NUM > -1)
	{
		menu_offset = menu_current-LCD_LINE_NUM+1;
	}
	
	switch(menu_show)
	{
		//主界面菜单显示
		case 0:
			for(i = 0; i+menu_offset < LCD_LINE_NUM+menu_offset; i++)
      {
				//计算出字符的中间宽度位置；使字符居中显示
				d =  Motor_All_Run_flage ? (w-u8g2_GetStrWidth(&u8g2, Main_Screen_Stop[i+menu_offset]))/2:\
															     (w-u8g2_GetStrWidth(&u8g2, Main_Screen_Start[i+menu_offset]))/2;

        //设置绘制颜色为实体颜色
        u8g2_SetDrawColor(&u8g2, 1);

        //判别当前菜单是否被选中；若被选中则显示为背景色
        if(i+menu_offset == menu_current)
        {
          //绘制一个矩形实心
          u8g2_DrawBox(&u8g2, 0, i*h, w, h);
          //设置绘制颜色为透明颜色
          u8g2_SetDrawColor(&u8g2, 0);
        }
				//将对应的字符显示到屏幕中
				Motor_All_Run_flage ? u8g2_DrawStr(&u8g2, d, i*h, Main_Screen_Stop[i+menu_offset]):\
				                      u8g2_DrawStr(&u8g2, d, i*h, Main_Screen_Start[i+menu_offset]);
      }
			break;
			
		//控制界面菜单显示
		case 1:
			for(i = 0; i+menu_offset < LCD_LINE_NUM+menu_offset; i++)
      {
				//计算出字符的中间宽度位置；使字符居中显示
				d = (w-u8g2_GetStrWidth(&u8g2, Control[i+menu_offset]))/2;

        //设置绘制颜色为实体颜色
        u8g2_SetDrawColor(&u8g2, 1);

        //判别当前菜单是否被选中；若被选中则显示为背景色
        if(i+menu_offset == menu_current)
        {
          //绘制一个矩形实心
          u8g2_DrawBox(&u8g2, 0, i*h, w, h);
          //设置绘制颜色为透明颜色
          u8g2_SetDrawColor(&u8g2, 0);
        }
				switch(i+menu_offset)
				{
					case 0:
						//将对应的字符显示到屏幕中
						u8g2_DrawStr(&u8g2, d, i*h, Control[i+menu_offset]);
					break;
						
					case 1:
						//将对应的字符显示到屏幕中
						u8g2_DrawStr(&u8g2, d-30, i*h, Control[i+menu_offset]);
						u8g2_DrawStr(&u8g2, d+46, i*h, Motor_A_En_flage ? "Enable":"Disable");
					break;
					
					case 2:
						//将对应的字符显示到屏幕中
						u8g2_DrawStr(&u8g2, d-30, i*h, Control[i+menu_offset]);
						u8g2_DrawStr(&u8g2, d+46, i*h, Motor_B_En_flage ? "Enable":"Disable");
					break;
					
					case 3:
						//将对应的字符显示到屏幕中
						u8g2_DrawStr(&u8g2, d-30, i*h, Control[i+menu_offset]);
						u8g2_DrawStr(&u8g2, d+46, i*h, Motor_C_En_flage ? "Enable":"Disable");
					break;
					
					case 4:
						//将对应的字符显示到屏幕中
						u8g2_DrawStr(&u8g2, d-26, i*h, Control[i+menu_offset]);
						u8g2_DrawStr(&u8g2, d+50, i*h, Motor_A_Run_flage ? "RUN":"STOP");
					break;
					
					case 5:
						//将对应的字符显示到屏幕中
						u8g2_DrawStr(&u8g2, d-26, i*h, Control[i+menu_offset]);
						u8g2_DrawStr(&u8g2, d+50, i*h, Motor_B_Run_flage ? "RUN":"STOP");
					break;
					
					case 6:
						//将对应的字符显示到屏幕中
						u8g2_DrawStr(&u8g2, d-26, i*h, Control[i+menu_offset]);
						u8g2_DrawStr(&u8g2, d+50, i*h, Motor_C_Run_flage ? "RUN":"STOP");
					break;
				}   
      }
		break;
		
		//控制界面菜单显示
		case 2:
			for(i = 0; i+menu_offset < LCD_LINE_NUM+menu_offset-1; i++)
      {
				//计算出字符的中间宽度位置；使字符居中显示
				d = (w-u8g2_GetStrWidth(&u8g2, Setting[i+menu_offset]))/2;

        //设置绘制颜色为实体颜色
        u8g2_SetDrawColor(&u8g2, 1);

        //判别当前菜单是否被选中；若被选中则显示为背景色
        if(i+menu_offset == menu_current)
        {
          //绘制一个矩形实心
          u8g2_DrawBox(&u8g2, 0, i*h, w, h);
          //设置绘制颜色为透明颜色
          u8g2_SetDrawColor(&u8g2, 0);
        }
				//将对应的字符显示到屏幕中
				u8g2_DrawStr(&u8g2, d, i*h, Setting[i+menu_offset]);
      }
			break;
		
		case 3:  //显示电机A速度设置界面
      for(i = 0; i < 1; i++)
      {
				//计算出字符的中间宽度位置；使字符居中显示
				d = (w-u8g2_GetStrWidth(&u8g2, menu_motorA[i]));

        //设置绘制颜色为实体颜色
        u8g2_SetDrawColor(&u8g2, 1);
				
				//将float型的Motor_A_Speed_Now转换为str型Motor_A_Speed_Str
				sprintf((char*)Motor_A_Speed_Str, "%.1f", Motor_A_Speed_Now);
				
				//将对应的字符显示到屏幕中
				u8g2_DrawStr(&u8g2, 0, u8g2_GetDisplayHeight(&u8g2)/2-h/2, menu_motorA[i]);
				u8g2_DrawStr(&u8g2, d+10, u8g2_GetDisplayHeight(&u8g2)/2-h/2, (char*)Motor_A_Speed_Str);
				u8g2_DrawStr(&u8g2, d+10+u8g2_GetStrWidth(&u8g2, (char*)Motor_A_Speed_Str)+2, \
					            u8g2_GetDisplayHeight(&u8g2)/2-h/2, "r/min");
      }
    break;

    case 4:  //显示电机B速度设置界面
      for(i = 0; i < 1; i++)
      {
				//计算出字符的中间宽度位置；使字符居中显示
				d = (w-u8g2_GetStrWidth(&u8g2, menu_motorB[i]));

        //设置绘制颜色为实体颜色
        u8g2_SetDrawColor(&u8g2, 1);
				
				//将float型的Motor_B_Speed_Now转换为str型Motor_B_Speed_Str
				sprintf((char*)Motor_B_Speed_Str, "%.1f", Motor_B_Speed_Now);
				
				//将对应的字符显示到屏幕中
				u8g2_DrawStr(&u8g2, 0, u8g2_GetDisplayHeight(&u8g2)/2-h/2, menu_motorB[i]);
				u8g2_DrawStr(&u8g2, d+10, u8g2_GetDisplayHeight(&u8g2)/2-h/2, (char*)Motor_B_Speed_Str);
				u8g2_DrawStr(&u8g2, d+10+u8g2_GetStrWidth(&u8g2, (char*)Motor_B_Speed_Str)+2, \
					            u8g2_GetDisplayHeight(&u8g2)/2-h/2, "r/min");
      }
    break;

    case 5:  //显示电机C速度设置界面
     for(i = 0; i < 1; i++)
      {
				//计算出字符的中间宽度位置；使字符居中显示
				d = (w-u8g2_GetStrWidth(&u8g2, menu_motorC[i]));

        //设置绘制颜色为实体颜色
        u8g2_SetDrawColor(&u8g2, 1);
				
				//将float型的Motor_C_Speed_Now转换为str型Motor_C_Speed_Str
				sprintf((char*)Motor_C_Speed_Str, "%.1f", Motor_C_Speed_Now);
				
				//将对应的字符显示到屏幕中
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
