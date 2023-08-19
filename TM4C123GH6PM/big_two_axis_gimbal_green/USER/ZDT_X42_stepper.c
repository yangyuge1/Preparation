#include "ZDT_X42_stepper.h"

#define     __IO    volatile

void Timer0BIntHandler(void);

char up_buffer_queue[30];
char down_buffer_queue[30];

int up_position,down_position;

#define UP_UART UART1_BASE
#define DOWM_UART UART5_BASE

uint32_t target_uart=UP_UART;


//软件脉冲控制变量区===start
int up_pwm_count,down_pwm_count;//脉冲累计
int test_flag=0;

int period_up=0,period_down=0;
int up_count=0,down_count=0;
int up_flag=0,down_flag=0;
int up_dir=0,down_dir=0;
int up_pos=0,down_pos=0;
int up_stop=0,down_stop=0;
void fuction_A(void)
{
	up_count++;down_count++;
	if(up_count>=period_up){
		up_count=0;
		
		//先改方向	
		if(up_dir)
		{
			GPIOPinWrite(GPIO_PORTD_BASE,GPIO_PIN_1,GPIO_PIN_1);
		}else{
			GPIOPinWrite(GPIO_PORTD_BASE,GPIO_PIN_1,0);
		}
		
		if(up_stop)
		{
			//停止
			GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1,0);
		}else
		{
			if(up_flag)
			{
				up_flag=0;
				GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1,GPIO_PIN_1);
			}else{
				up_flag=1;
				GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1,0);
				
				//脉冲计数
				if(up_dir)
				{
					up_pos++;
				}else{
					up_pos--;
				}
				
			}
		}
	}
	
	//down控制
	if(down_count>=period_down){
		down_count=0;
		
		//先改方向	
		if(down_dir)
		{
			GPIOPinWrite(GPIO_PORTD_BASE,GPIO_PIN_0,GPIO_PIN_0);
		}else{
			GPIOPinWrite(GPIO_PORTD_BASE,GPIO_PIN_0,0);
		}
		
		if(down_stop)
		{
			//停止
			GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_2,0);
		}else
		{
			//产生脉冲
			if(down_flag)
			{
				down_flag=0;
				GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_2,GPIO_PIN_2);
				
			}else{
				down_flag=1;
				GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_2,0);
				//脉冲计数
				if(down_dir)
				{
					down_pos++;
				}else{
					down_pos--;
				}
			}		
		}

	}
}
void speed_set(double up,double down)
{
	#define timer_fre 32000
	if(fabs(down)<=0.0001)
	{
		down_stop=1;
	}else
	{
		period_down=(int)(timer_fre/fabs(down)/3200);
		down_stop=0;
		if(down>0)
		{
			down_dir=1;
		}else{
			down_dir=0;
		}
	}

	if(fabs(up)<=0.0001)
	{
		up_stop=1;
	}else
	{
		period_up=(int)(timer_fre/fabs(up)/3200);
		up_stop=0;
		if(up>0)
		{
			up_dir=0;
		}else{
			up_dir=1;
		}
	}
	
}
//软件脉冲控制变量区===end

void init_ZDT_X42_stepper(void)
{
#define IO_mode 
#ifdef PWM_mode
		
//    PWM??
//    PE4 M0PWM2 ? PE5 M0PWM3
//    PB7 M0PWM1 ? PB6 M0PWM0
    SysCtlPWMClockSet(SYSCTL_PWMDIV_8);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM1);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

    GPIOPinTypePWM(GPIO_PORTF_BASE, GPIO_PIN_1);
    GPIOPinTypePWM(GPIO_PORTF_BASE, GPIO_PIN_2);

    GPIOPinConfigure(GPIO_PF1_M1PWM5);//1
    GPIOPinConfigure(GPIO_PF2_M1PWM6);//1

    PWMGenConfigure(PWM1_BASE, PWM_GEN_2, PWM_GEN_MODE_DOWN |
                    PWM_GEN_MODE_NO_SYNC);
    PWMGenConfigure(PWM1_BASE, PWM_GEN_3, PWM_GEN_MODE_DOWN |
                    PWM_GEN_MODE_NO_SYNC);
    // Set the PWM period to 10kHz.  To calculate the appropriate parameter
    // use the following equation: N = (1 / f) * SysClk.  Where N is the
    // function parameter, f is the desired frequency, and SysClk is the
    // system clock frequency.
    // In this case you get: (1 / 20000Hz) * 16MHz = 800 cycles.  Note that
    // the maximum period you can set is 2^16.
    // TODO: modify this calculation to use the clock frequency that you are
    // using.
    //
    PWMGenPeriodSet(PWM1_BASE, PWM_GEN_2, 5000);
    PWMGenPeriodSet(PWM1_BASE, PWM_GEN_3, 5000);
    // Set PWM0 to a duty cycle of 25%.  You set the duty cycle as a function
    // of the period.  Since the period was set above, you can use the
    // PWMGenPeriodGet() function.  For this example the PWM will be high for
    // 25% of the time or 4000 clock ticks (16000 / 4).
    //
    PWMPulseWidthSet(PWM1_BASE, PWM_OUT_5,
                     0);
    PWMPulseWidthSet(PWM1_BASE, PWM_OUT_6,
                     0);
    // Enable the PWM0 Bit0 (PD0) output signal.
    //
    PWMOutputState(PWM1_BASE, PWM_OUT_5_BIT, true);
    PWMOutputState(PWM1_BASE, PWM_OUT_6_BIT, true);
    //
    // Enable the PWM generator block.
    //
    PWMGenEnable(PWM1_BASE, PWM_GEN_2);
    PWMGenEnable(PWM1_BASE, PWM_GEN_3);
		
		SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
		GPIOPinTypeGPIOOutput(GPIO_PORTD_BASE, GPIO_PIN_0|GPIO_PIN_1);
#endif

#ifdef IO_mode
		SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
		GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_2|GPIO_PIN_1);
		GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_2,0);
		GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1,0);
		SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
		GPIOPinTypeGPIOOutput(GPIO_PORTD_BASE, GPIO_PIN_0|GPIO_PIN_1);
		GPIOPinWrite(GPIO_PORTD_BASE,GPIO_PIN_0,0);
		GPIOPinWrite(GPIO_PORTD_BASE,GPIO_PIN_1,0);
		
		//定时器测试
		SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
		GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, GPIO_PIN_4);
		GPIOPinWrite(GPIO_PORTB_BASE,GPIO_PIN_4,0);
		
		SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
		TimerConfigure(TIMER0_BASE, TIMER_CFG_SPLIT_PAIR | TIMER_CFG_B_PERIODIC);
		TimerLoadSet(TIMER0_BASE, TIMER_B, SysCtlClockGet() / 32000 * 0.5);
		//
    // Enable processor interrupts.
    //
    IntMasterEnable();
		
		TimerIntRegister(TIMER0_BASE, TIMER_B,Timer0BIntHandler);
    //
    // Configure the Timer0B interrupt for timer timeout.
    //
    TimerIntEnable(TIMER0_BASE, TIMER_TIMB_TIMEOUT);

    //
    // Enable the Timer0B interrupt on the processor (NVIC).
    //
    IntEnable(INT_TIMER0B);
		//
    // Enable Timer0B.
    //
    TimerEnable(TIMER0_BASE, TIMER_B);
#endif
}

//*****************************************************************************
//
// The interrupt handler for the Timer0B interrupt.
//
//*****************************************************************************
void
Timer0BIntHandler(void)
{
    TimerIntClear(TIMER0_BASE, TIMER_TIMB_TIMEOUT);
		if(test_flag)
		{
			GPIOPinWrite(GPIO_PORTB_BASE,GPIO_PIN_4,0);
			test_flag=0;
			//最小周期计算点A
			fuction_A();
		}else{
			GPIOPinWrite(GPIO_PORTB_BASE,GPIO_PIN_4,GPIO_PIN_4);
			test_flag=1;
			//最小周期计算点B
		  fuction_A();
		}
}


int software_pwm_control(double speed,int acc,int count)
{
	#define COUNT 3200
	int i;
	int time=(int)(1000000/(fabs(speed)*COUNT)/2);
	if(speed>=0)
	{
		GPIOPinWrite(GPIO_PORTD_BASE,GPIO_PIN_0,GPIO_PIN_0);
			for(i=1;i<=count;i++)
			{
				GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_2,GPIO_PIN_2);
				delay_us(time);
				GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_2,0);
				down_pwm_count++;
				delay_us(time);
			}
	}else{
		GPIOPinWrite(GPIO_PORTD_BASE,GPIO_PIN_0,0);
			for(i=1;i<=count;i++)
			{
				GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_2,GPIO_PIN_2);
				delay_us(time);
				GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_2,0);
				down_pwm_count--;
				delay_us(time);
			}
	}
	return 1;
}



