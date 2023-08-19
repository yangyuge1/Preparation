//*****************************************************************************
//
// uart_echo.c - Example for reading data from and writing data to the UART in
//               an interrupt driven fashion.
//
// Copyright (c) 2012-2020 Texas Instruments Incorporated.  All rights reserved.
// Software License Agreement
// 
// Texas Instruments (TI) is supplying this software for use solely and
// exclusively on TI's microcontroller products. The software is owned by
// TI and/or its suppliers, and is protected under applicable copyright
// laws. You may not combine this software with "viral" open-source
// software in order to form a larger program.
// 
// THIS SOFTWARE IS PROVIDED "AS IS" AND WITH ALL FAULTS.
// NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT
// NOT LIMITED TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. TI SHALL NOT, UNDER ANY
// CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR CONSEQUENTIAL
// DAMAGES, FOR ANY REASON WHATSOEVER.
// 
// This is part of revision 2.2.0.295 of the EK-TM4C123GXL Firmware Package.
//
//*****************************************************************************
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "driverlib/debug.h"
#include "driverlib/fpu.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/rom_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "utils/uartstdio.h"
#include "driverlib/pwm.h"

#include "complex_task_support.h"
#include "my_stepper.h"
#include "stepper_motor_usart_controller.h"
#include "controller.h"
#include "connect.h"
#include "my_jy901b.h"
#include "encoder.h"
#include "two_axis_gimbal.h"
#include "ZDT_X42_stepper.h"
#include "us_100.h"
#define zhi  1
#define r_90 2
#define l_90 3
#define stop 0
#define zhi_ 4
#define t_180 5
#define stop_ 6
void IntHandler_UART3_(void);
void uart_init_uart3_PC6_7(void);
extern int isRed;     //画面是否有目标 0无 1有 只读
extern int mid_1;       //中点1
extern int mid_2;       //中点2
extern int width;     //宽度
extern double up,down;
extern double data_du[3];
extern int left_position,right_position;
extern int left_position,right_position;
extern int period_up,period_down;
extern int red_x,red_y,green_x,green_y;
extern int pointer_list[8];
extern int list_len;
extern int receive_mode;
extern int up_encoder,down_encoder;
extern int start_flag;
extern int distance;

#define UP_UART UART1_BASE
#define DOWM_UART UART5_BASE
extern uint32_t target_uart;
extern int up_position,down_position;
int encoder_right=0;
int encoder_left=0;

double yaw_target=202,pitch_target=117.4;

//角度标定法
typedef struct position{
  float pitch;
  float yaw;
  
  float target_pitch;
  float target_yaw;
  
  int up_position;
  int down_position;
}position;

position yuntai;

//角度标定与解算
typedef struct point
{
	double yaw;
	double pitch;
}point;
#define hight 3
#define width 3
#define x_max 250
#define y_max 250
point target;
point point_list[hight][width] = {
	{ {198.3,114} , {212.5,113.3} , {226.5,113.9} },
	{ {198.3,127.25} , {212.4,127.25} , {226.5,127.2}},
	{ {198.1,141} , {212.3,141.2} , {226.2,141} }
};

void caculate_point(int x, int y)
{
	int a = x / (x_max / (width - 1));
	int a_ = x % (x_max / (width - 1));
	int b = y / (y_max / (hight - 1));
	int b_ = y % (y_max / (hight - 1));

	target.yaw = point_list[b][a].yaw * ((x_max*1.0 / (width - 1) - a_) / (x_max * 1.0 / (width - 1))) + point_list[b + 1][a + 1].yaw * (a_ / (x_max * 1.0 / (width - 1)));
	target.pitch = point_list[b][a].pitch * ((y_max * 1.0 / (hight - 1) - b_) / (y_max * 1.0 / (hight - 1))) + point_list[b + 1][a + 1].pitch * (b_ / (y_max * 1.0 / (hight - 1)));
}

//方形路径补充
#define change_number 8
#define change_len change_number*4+4+1
int pointer_list_changed[change_len][2];
void changed_pointer_list(void)
{
	int i, j;
	double yaw, pitch;
	for (i = 0; i <= 2; i++)
	{
		for (j = 0; j <= change_number; j++)
		{
			pointer_list_changed[i * (change_number + 1) + j][0] = pointer_list[i * 2] + (-pointer_list[i * 2] + pointer_list[(i + 1) * 2]) * (j / (change_number+1.0));
			pointer_list_changed[i * (change_number + 1) + j][1] = pointer_list[i * 2 + 1] + (-pointer_list[i * 2 + 1] + pointer_list[(i + 1) * 2 + 1]) * (j / (change_number + 1.0));
		}
	}
	for (j = 0; j <= change_number; j++)
	{
		pointer_list_changed[3 * (change_number+1) + j][0] = pointer_list[3 * 2] + (-pointer_list[3 * 2] + pointer_list[0]) * (j / (change_number + 1.0));
		pointer_list_changed[3 * (change_number + 1) + j][1] = pointer_list[3 * 2 + 1] + (-pointer_list[3 * 2 + 1] + pointer_list[1]) * (j / (change_number + 1.0));
	}
	pointer_list_changed[change_len-1][0] = pointer_list[0];
	pointer_list_changed[change_len-1][1] = pointer_list[1];
}

int mode=0;

//绿色角度标定与解算
extern int up_encoder, down_encoder;
//  上为正，下为负       顺时针为正，逆时针为负
double green_pitch_targrt, green_yaw_target;
double green_pitch_now, green_yaw_now;
//int green_x, green_y;
//距离中心的距离，以处于左为正
int left_or_right=1;// 1 left 0 right
double L=0.5;
//云台高度
double h=0.23;
#define screen_width 0.5
#define screen_bian 0.05
#define PI 3.1415926
int yaw_down_encoder_zero=44954;
int pitch_up_encoder_zero=5860;

void green_caculate_target(int x,int y)
{
	if (left_or_right)
	{
		green_yaw_target =(atan((x*1.0 / 250* screen_width + L - screen_width * 0.5))) / PI * 180;
		green_pitch_targrt = (atan((-y*1.0/ 250 * screen_width + screen_width+ screen_bian-h) / sqrt((x*1.0 / 250 * screen_width + L - screen_width * 0.5) * (x*1.0 / 250 * screen_width + L - screen_width * 0.5) +1) )) / PI * 180;
	}
	else
	{
		green_yaw_target = -(atan( (-x*1.0 / 250 * screen_width + L + screen_width * 0.5))) / PI * 180;
		green_pitch_targrt = (atan((-y*1.0 / 250 * screen_width + screen_width + screen_bian - h) / sqrt((-x*1.0 / 250 * screen_width + L + screen_width * 0.5) * (-x*1.0 / 250 * screen_width + L + screen_width * 0.5) + 1))) / PI * 180;
	}

}

void green_caculate_now(int up_encoder,int down_encoder)
{
    green_yaw_now=(yaw_down_encoder_zero-down_encoder)*1.0/65536*360;
    green_pitch_now=(+pitch_up_encoder_zero-up_encoder)*1.0/65536*360;
}

//新的速度引导
void move(int start_x, int start_y, int end_x, int end_y, int start_distance, int change_time, int step_distance,int min_distance,int end_wait_time)
{
	int i;
	
	int new_x, new_y;
  int x,y;
	new_x = (int)((end_x - start_x) / sqrt((start_x - end_x) * (start_x - end_x) + (start_y - end_y) * (start_y - end_y)) * start_distance+start_x);
	new_y = (int)((end_y - start_y) / sqrt((start_x - end_x) * (start_x - end_x) + (start_y - end_y) * (start_y - end_y)) * start_distance+start_y);
  int step_number = (int)(sqrt((new_x - end_x) * (new_x - end_x) + (new_y - end_y) * (new_y - end_y))/ step_distance);
  
	for (i = 0; i < step_number; i++)
	{
    x=new_x+ (int)((end_x - new_x) / sqrt((new_x - end_x) * (new_x - end_x) + (new_y - end_y) * (new_y - end_y)) * (step_distance *i));
    y=new_y + (int)((end_y - new_y)/ sqrt((new_x - end_x) * (new_x - end_x) + (new_y - end_y) * (new_y - end_y)) * (step_distance * i));
     
    green_caculate_target(x,y);
    while(mode==0){};
    yaw_target=green_yaw_target;
    pitch_target=green_pitch_targrt;
    UARTprintf("%d,%d\n",(int)green_yaw_target,(int)green_pitch_targrt);
    
		delay_ms(change_time);
	}
     while(mode==0){};
    green_caculate_target(end_x,end_y);
    yaw_target=green_yaw_target;
    pitch_target=green_pitch_targrt;
    delay_ms(end_wait_time);
	return;
}


void change_angle_with_s_a(double pitch_change,double yaw_change,int speed, uint8_t acc)
{
  if(pitch_change>=0)
    position_set_uart(-speed,acc,(int)(pitch_change*8.88888888888),up_wheel);
  else
    position_set_uart(speed,acc,(int)(-pitch_change*8.88888888888),up_wheel);
  
  if(yaw_change>=0)
    position_set_uart(speed,acc,(int)(yaw_change*8.88888888888),down_wheel);
  else
    position_set_uart(-speed,acc,(int)(-yaw_change*8.88888888888),down_wheel);
}
void change_angle(double pitch_change,double yaw_change)
{
change_angle_with_s_a(pitch_change,yaw_change,20,20);
}

double green_L_caculate(double yaw,int x)
{
  double L_;
  if(left_or_right)
  {
    L_=fabs(tan(yaw/180*PI)*1+ (125-x)*1.0/250*screen_width);
  }else{
    L_=fabs(-tan(yaw/180*PI)*1+ (-125+x)*1.0/250*screen_width);
  }
  UARTprintf("L_=%d\n" ,(int)(L_*100));
  if(0.3<=L_ && L_<=1.1)
  {
    return L_;
  }else{
    return 0;
  }
}

void corrective_shooting_elements(void)
{
#define point_list_mode 0
#define point_feedback_mode 1
  encoder_get();
  delay_ms(300);
  UARTprintf("%d,%d\n" ,up_encoder,down_encoder);
  receive_mode=point_feedback_mode;
  UARTCharPutNonBlocking(UART7_BASE,'B');
  delay_ms(600);
  UARTCharPutNonBlocking(UART7_BASE,'C');
  green_caculate_now(up_encoder, down_encoder);
   UARTprintf("green_yaw_now=%d,x=%d\n" ,(int)(green_yaw_now),green_x);
  L=green_L_caculate(green_yaw_now,green_x);
  
}


pid_data pid,pid_direct,pid_turn,pid_down,pid_up;
int angle_caculate(int current,int change);
	void init_controll_loop(void);
void position_set(float yaw,float pitch);
  
int close_motor_flag=0;  


void keya_interrupt(void)
{
    uint32_t ui32Status = GPIOIntStatus(GPIO_PORTA_BASE, true);
    GPIOIntClear(GPIO_PORTA_BASE, ui32Status);
    
    if((ui32Status&GPIO_PIN_4) == GPIO_PIN_4)
    {
    while(!GPIOPinRead(GPIO_PORTA_BASE, GPIO_PIN_4));
        //TODO
        mode=1;      
    }
		if((ui32Status&GPIO_PIN_5) == GPIO_PIN_5)
    {
        while(!GPIOPinRead(GPIO_PORTA_BASE, GPIO_PIN_5));
        //TODO
       mode=2;
          yaw_target=green_yaw_now;
          pitch_target=green_pitch_now; 

    }
}
void keye_interrupt(void)
{
    uint32_t ui32Status = GPIOIntStatus(GPIO_PORTE_BASE, true);
    GPIOIntClear(GPIO_PORTE_BASE, ui32Status);
    
		if((ui32Status&GPIO_PIN_2) == GPIO_PIN_2)
    {
        while(!GPIOPinRead(GPIO_PORTE_BASE, GPIO_PIN_2));
        //TODO
        left_or_right=0;
        UARTprintf("right\n");
        corrective_shooting_elements();
        UARTprintf("green_x=%d\n" ,green_x);
        UARTprintf("L=%d\n" ,(int)(L*100));
    }
		if((ui32Status&GPIO_PIN_3) == GPIO_PIN_3)
    {
        while(!GPIOPinRead(GPIO_PORTE_BASE, GPIO_PIN_3));
        //TODO
      if(mode==0){
        mode=1;      
      }
      else{
        mode=0;
      }
    }
}
void draw(void)
{
  delay_ms(100);

  encoder_get();
  delay_ms(100);
  green_caculate_now(up_encoder,down_encoder);
  green_caculate_target(0,0);
  change_angle(green_pitch_targrt-green_pitch_now,green_yaw_target-green_yaw_now);
  delay_ms(3000);

  encoder_get();
  delay_ms(100);
  green_caculate_now(up_encoder,down_encoder);
  green_caculate_target(250,0);
  change_angle(green_pitch_targrt-green_pitch_now,green_yaw_target-green_yaw_now);
  delay_ms(3000);

  encoder_get();
  delay_ms(100);
  green_caculate_now(up_encoder,down_encoder);
  green_caculate_target(250,250);
  change_angle(green_pitch_targrt-green_pitch_now,green_yaw_target-green_yaw_now);
  delay_ms(3000);

  encoder_get();
  delay_ms(100);
  green_caculate_now(up_encoder,down_encoder);
  green_caculate_target(0,250);
  change_angle(green_pitch_targrt-green_pitch_now,green_yaw_target-green_yaw_now);
  delay_ms(3000);

  encoder_get();
  delay_ms(100);
  green_caculate_now(up_encoder,down_encoder);
  green_caculate_target(0,0);
  change_angle(green_pitch_targrt-green_pitch_now,green_yaw_target-green_yaw_now);
  delay_ms(3000);  
}
int
main(void)
{
    pid_down.P=0.0127;
	pid_down.I=0;
	pid_down.D=0;
	pid_down.Config.output_max=0.1;
	pid_down.Config.output_min=-0.1;

	pid_down.Config.calculation_method=Positional;	
	
	pid_up.P=0.0127;
	pid_up.I=0;
  pid_up.D=0;
	pid_up.Config.output_max=0.1;
	pid_up.Config.output_min=-0.1;

	pid_up.Config.calculation_method=Positional;
  
    SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN |
                       SYSCTL_XTAL_16MHZ);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
    UARTStdioConfig(0, 115200, SysCtlClockGet());
		
    //声光提示
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
		GPIOPinTypeGPIOOutput(GPIO_PORTC_BASE, GPIO_PIN_6|GPIO_PIN_5);
		GPIOPinWrite(GPIO_PORTC_BASE,GPIO_PIN_6,GPIO_PIN_6);//6为声 低电平触发
		GPIOPinWrite(GPIO_PORTC_BASE,GPIO_PIN_5,0);         //5为光 高电平触发
    
  //key
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
	while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOE))
	{}
	GPIODirModeSet(GPIO_PORTA_BASE, GPIO_PIN_4|GPIO_PIN_5, GPIO_DIR_MODE_IN);
	GPIODirModeSet(GPIO_PORTE_BASE, GPIO_PIN_2|GPIO_PIN_3, GPIO_DIR_MODE_IN);
		
	GPIOPadConfigSet(GPIO_PORTA_BASE, GPIO_PIN_4|GPIO_PIN_5, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
	GPIOPadConfigSet(GPIO_PORTE_BASE, GPIO_PIN_2|GPIO_PIN_3, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
		
	GPIOIntTypeSet(GPIO_PORTA_BASE, GPIO_PIN_4|GPIO_PIN_5, GPIO_FALLING_EDGE);
	GPIOIntTypeSet(GPIO_PORTE_BASE, GPIO_PIN_2|GPIO_PIN_3, GPIO_FALLING_EDGE);		
		
	GPIOIntRegister(GPIO_PORTA_BASE, keya_interrupt); 
  GPIOIntRegister(GPIO_PORTE_BASE, keye_interrupt); 
		
	GPIOIntEnable(GPIO_PORTA_BASE, GPIO_INT_PIN_4|GPIO_INT_PIN_5);
	GPIOIntEnable(GPIO_PORTE_BASE, GPIO_INT_PIN_2|GPIO_INT_PIN_3);
		
	IntEnable(INT_GPIOA);
	IntEnable(INT_GPIOE);
	IntMasterEnable();
  
    
	connect_uart_init_uart7_PE0_1();
  speed_set(0,0);
  init_ZDT_X42_stepper();
  stepper_init();
  
  delay_ms(300);
  enable(0);
  delay_ms(300);
  
//先诸元矫正
  UARTprintf("left\n");
  corrective_shooting_elements();
  UARTprintf("green_x=%d\n" ,green_x);
  UARTprintf("L=%d\n" ,(int)(L*100));
  
//  delay_ms(300);
//  enable(0);
//  delay_ms(300);
//  draw();
//然后开启循环控制
//green_caculate_target(125,125);
speed_set(0,0);
// 
// receive_mode=point_list_mode;
// list_len=0;
// UARTCharPutNonBlocking(UART7_BASE,'A');
// delay_ms(400);

//    green_caculate_target(pointer_list[0],pointer_list[1]);
//    yaw_target=green_yaw_target;
//    pitch_target=green_pitch_targrt;

init_controll_loop();
   
		while(1)
		{
      
//      encoder_get();
//      delay_ms(100);
//      UARTprintf("%d,%d\n" ,down_encoder,up_encoder);
      
      switch(mode){
        case 0:
//         GPIOPinWrite(GPIO_PORTC_BASE,GPIO_PIN_6,GPIO_PIN_6);//6为声 低电平触发
//         GPIOPinWrite(GPIO_PORTC_BASE,GPIO_PIN_5,0);         //5为光 高电平触发
        
        break;
        case 1:      
          
           GPIOPinWrite(GPIO_PORTC_BASE,GPIO_PIN_6,GPIO_PIN_6);//6为声 低电平触发
           GPIOPinWrite(GPIO_PORTC_BASE,GPIO_PIN_5,0);         //5为光 高电平触发
        
          green_caculate_target(125,125);
          yaw_target=green_yaw_target;
          pitch_target=green_pitch_targrt;
          delay_ms(1000);
        
          GPIOPinWrite(GPIO_PORTC_BASE,GPIO_PIN_6,0);//6为声 低电平触发
		      GPIOPinWrite(GPIO_PORTC_BASE,GPIO_PIN_5,GPIO_PIN_5);         //5为光 高电平触发
          mode=0;
          break;
        case 2:      
           GPIOPinWrite(GPIO_PORTC_BASE,GPIO_PIN_6,GPIO_PIN_6);//6为声 低电平触发
           GPIOPinWrite(GPIO_PORTC_BASE,GPIO_PIN_5,0);         //5为光 高电平触发
        
           UARTCharPutNonBlocking(UART7_BASE,'C');
           delay_ms(300);
           receive_mode=point_list_mode;
           UARTCharPutNonBlocking(UART7_BASE,'A');
           delay_ms(300);
          green_caculate_target(pointer_list[0],pointer_list[1]);
          yaw_target=green_yaw_target;
          pitch_target=green_pitch_targrt; 
          delay_ms(1000);
        
          GPIOPinWrite(GPIO_PORTC_BASE,GPIO_PIN_6,0);//6为声 低电平触发
		      GPIOPinWrite(GPIO_PORTC_BASE,GPIO_PIN_5,GPIO_PIN_5);         //5为光 高电平触发
        
          move(pointer_list[0],pointer_list[1],pointer_list[2],pointer_list[3],2,20,2,4,500);
          move(pointer_list[2],pointer_list[3],pointer_list[4],pointer_list[5],2,20,2,4,500);
          move(pointer_list[4],pointer_list[5],pointer_list[6],pointer_list[7],2,20,2,4,500);
          move(pointer_list[6],pointer_list[7],pointer_list[0],pointer_list[1],2,20,2,4,500); 
        
          GPIOPinWrite(GPIO_PORTC_BASE,GPIO_PIN_6,GPIO_PIN_6);//6为声 低电平触发
          GPIOPinWrite(GPIO_PORTC_BASE,GPIO_PIN_5,0);         //5为光 高电平触发
          
          mode=0;
          break;
      }
  
		}
}


int angle_caculate(int current,int change)
{
	int new_angle=0;
	if(-180<=current&&current<=180 && -180<=change&&change<=180 )
	{
		new_angle=current+change;
		if(new_angle>180){new_angle=360-new_angle;}
		if(new_angle<-180){new_angle=360+new_angle;}			
	}
	return new_angle;
}

void uart_init_uart3_PC6_7(void){
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART3);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
    GPIOPinConfigure(GPIO_PC6_U3RX);
    GPIOPinConfigure(GPIO_PC7_U3TX);
    GPIOPinTypeUART(GPIO_PORTC_BASE, GPIO_PIN_6 | GPIO_PIN_7);
    UARTConfigSetExpClk(UART3_BASE, SysCtlClockGet(), 9600,
                            (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |
                             UART_CONFIG_PAR_NONE));
//    UARTFIFODisable(UART3_BASE);

    IntMasterEnable();
    UARTIntRegister(UART3_BASE,IntHandler_UART3_);
    IntPrioritySet(INT_UART3,0);
    IntEnable(INT_UART3);
    UARTIntEnable(UART3_BASE, UART_INT_RX|UART_INT_RT );
}
void IntHandler_UART3_(void)
{
    uint32_t ui32Status;
		uint8_t buffer;
    ui32Status = UARTIntStatus(UART3_BASE, true);
    UARTIntClear(UART3_BASE, ui32Status);
		if(ui32Status==UART_INT_RX)                                /* UART接收中断 */
    {
        while(UARTCharsAvail(UART3_BASE))
        {
            buffer=UARTCharGetNonBlocking(UART3_BASE);
//						UARTCharPutNonBlocking(UART3_BASE,buffer);
        }
    }

    if(ui32Status==UART_INT_RT)                                /* UART接收超时中断 */
    {
        while(UARTCharsAvail(UART3_BASE))
        {
            buffer=UARTCharGetNonBlocking(UART3_BASE);
//						UARTCharPutNonBlocking(UART3_BASE,buffer);
        }
    }
}

//定时器周期中断--控制循环
int control_count=0;
void controll_loop(void);
void init_controll_loop(void)
{

		
		SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER1);
		TimerConfigure(TIMER1_BASE, TIMER_CFG_SPLIT_PAIR | TIMER_CFG_B_PERIODIC);
		TimerLoadSet(TIMER1_BASE, TIMER_B, SysCtlClockGet() / 500);
		//
    // Enable processor interrupts.
    //
    IntMasterEnable();
		
		TimerIntRegister(TIMER1_BASE, TIMER_B,controll_loop);
    //
    // Configure the Timer0B interrupt for timer timeout.
    //
    TimerIntEnable(TIMER1_BASE, TIMER_TIMB_TIMEOUT);

    //
    // Enable the Timer0B interrupt on the processor (NVIC).
    //
    IntEnable(INT_TIMER1B);
		//
    // Enable Timer0B.
    //
    TimerEnable(TIMER1_BASE, TIMER_B);
}

void controll_loop(void)
{
	  TimerIntClear(TIMER1_BASE, TIMER_TIMB_TIMEOUT);
		control_count++;
		if(control_count>=5)//50HZ
		{
			control_count=0;
    
      encoder_get();
      green_caculate_now(up_encoder,down_encoder);
        switch(mode)
        {
          case 0:speed_set(0,0);break;
          case 1:          
            pid_update(&pid_down,yaw_target-green_yaw_now);
            pid_update(&pid_up,pitch_target-green_pitch_now);
            speed_set(-pid_up.output,pid_down.output);
            break;
          case 2:          
            pid_update(&pid_down,yaw_target-green_yaw_now);
            pid_update(&pid_up,pitch_target-green_pitch_now);
            speed_set(-pid_up.output,pid_down.output);
            break;
        }
      
		}
}

