# IO引脚分配
LCD12864控制引脚分配
CLK->E   ：PG14
SID->R/W : PG12
CS->RS   : PE3

编码器按钮控制引脚分配
KEY_EN   :PG13
KEY_A    :PG11
KEY_B    :PE2
BEEP     :PC1

电机驱动控制引脚分配
抽丝电机A：
step     :PA2
dir      :PC2
en       :PA4

挤丝电机B：
step     :PA1
dir      :PC3
en       :PA3

移动电机C：
step     :PA5
dir      :PC4
en       :PB0

移动电机B限位控制引脚
endstop正面 : PE6
endstop背面 : PE5

# 电机参数
抽丝电机A： 
57步进电机，带减速机
减速比为：4.25:1

挤丝电机B：
57步进电机，不带减速机
依靠两边的限位开关做往复运动

移动电机C：
42步进电机，不带减速机

# 驱动器设置
抽丝电机A驱动器：
步进值：10000
电流：根据实际情况定

挤丝电机B驱动器：
步进值：10000
电流：根据实际情况定

移动电机C驱动器：
步进值：10000
电流：根据实际情况定


