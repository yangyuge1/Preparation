#include "ZDT_X42_stepper.h"

#define     __IO    volatile

static void IntHandler_UART1__(void);
static void IntHandler_UART5__(void);

static void position_caculate_down(uint8_t right_buffer);
static void position_caculate_up(uint8_t left_buffer);

void Timer0BIntHandler(void);


 char up_buffer_queue[30];
 char down_buffer_queue[30];

int up_position,down_position;

#define UP_UART UART1_BASE
#define DOWM_UART UART5_BASE

uint32_t target_uart=UP_UART;
stepper_PID up_PID,down_PID;

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
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART1);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_UART1))
        ;
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOB))
        ;

    GPIOPinConfigure(GPIO_PB0_U1RX);
    GPIOPinConfigure(GPIO_PB1_U1TX);
    GPIOPinTypeUART(GPIO_PORTB_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    UARTClockSourceSet(UART1_BASE, UART_CLOCK_SYSTEM);
    UARTConfigSetExpClk(UART1_BASE, SysCtlClockGet(), 115200,
                            (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |
                             UART_CONFIG_PAR_NONE));
//    UARTFIFODisable(UART1_BASE);

    IntMasterEnable();
    UARTIntRegister(UART1_BASE,IntHandler_UART1__);
    IntPrioritySet(INT_UART1,0);
    IntEnable(INT_UART1);
    UARTIntEnable(UART1_BASE, UART_INT_RX |UART_INT_RT);
		
		SysCtlPeripheralEnable(SYSCTL_PERIPH_UART5);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_UART5))
        ;
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOE))
        ;

    GPIOPinConfigure(GPIO_PE4_U5RX);
    GPIOPinConfigure(GPIO_PE5_U5TX);
    GPIOPinTypeUART(GPIO_PORTE_BASE, GPIO_PIN_4 | GPIO_PIN_5);

    UARTClockSourceSet(UART5_BASE, UART_CLOCK_SYSTEM);
    UARTConfigSetExpClk(UART5_BASE, SysCtlClockGet(), 115200,
                            (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |
                             UART_CONFIG_PAR_NONE));
//    UARTFIFODisable(UART5_BASE);

    IntMasterEnable();
    UARTIntRegister(UART5_BASE,IntHandler_UART5__);
    IntPrioritySet(INT_UART5,0);
    IntEnable(INT_UART5);
    UARTIntEnable(UART5_BASE, UART_INT_RX | UART_INT_RT);
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
void IntHandler_UART1__(void)
{
    uint32_t ui32Status;
		uint8_t buffer;
    ui32Status = UARTIntStatus(UART1_BASE, true);
    UARTIntClear(UART1_BASE, ui32Status);
    if(ui32Status==UART_INT_RX)                                /* UART接收中断 */
    {
        while(UARTCharsAvail(UART1_BASE))
        {
            buffer=UARTCharGetNonBlocking(UART1_BASE);
						position_caculate_up(buffer);
        }
    }

    if(ui32Status==UART_INT_RT)                                /* UART接收超时中断 */
    {
        while(UARTCharsAvail(UART1_BASE))
        {
            buffer=UARTCharGetNonBlocking(UART1_BASE);
						position_caculate_up(buffer);
        }
    }
}
void IntHandler_UART5__(void)
{
    uint32_t ui32Status;
		uint8_t buffer;
    ui32Status = UARTIntStatus(UART5_BASE, true);
    UARTIntClear(UART5_BASE, ui32Status);
    if(ui32Status==UART_INT_RX)                                /* UART接收中断 */
    {
        while(UARTCharsAvail(UART5_BASE))
        {
            buffer=UARTCharGetNonBlocking(UART5_BASE);
						position_caculate_down(buffer);
        }
    }

    if(ui32Status==UART_INT_RT)                                /* UART接收超时中断 */
    {
        while(UARTCharsAvail(UART5_BASE))
        {
            buffer=UARTCharGetNonBlocking(UART5_BASE);
						position_caculate_down(buffer);
        }
    }
}
void position_caculate_up(uint8_t left_buffer)
{
	static int left_p=0;

	up_buffer_queue[left_p]=left_buffer;
	left_p++;
	
	if(left_p>=5)
	{
		if(up_buffer_queue[left_p-1]==0x6B&&up_buffer_queue[left_p-5]==0x01&&up_buffer_queue[left_p-4]==0x30)
		{
		up_position=(int32_t)(
													 ((int32_t)up_buffer_queue[left_p-3] << 8)  |
													 ((int32_t)up_buffer_queue[left_p-2] << 0)
													 );
		left_p=0;			
		}
	}
	
	if(left_p>=6)
	{
		if(up_buffer_queue[left_p-1]==0x6B&&up_buffer_queue[left_p-6]==0x01)
		{
		up_position=(int32_t)(
													 ((int32_t)up_buffer_queue[left_p-5] << 24) |
													 ((int32_t)up_buffer_queue[left_p-4] << 16) |
													 ((int32_t)up_buffer_queue[left_p-3] << 8)  |
													 ((int32_t)up_buffer_queue[left_p-2] << 0)
													 );
		left_p=0;			
		}
	}
	if(left_p>=19)
	{
		if(up_buffer_queue[left_p-1]==0x6B&&up_buffer_queue[left_p-19]==0x01&&up_buffer_queue[left_p-18]==0x21)
		{
			up_PID.Traj_Position_P=(int32_t)(
														 ((int32_t)up_buffer_queue[left_p-17] << 24) |
														 ((int32_t)up_buffer_queue[left_p-16] << 16) |
														 ((int32_t)up_buffer_queue[left_p-15] << 8)  |
														 ((int32_t)up_buffer_queue[left_p-14] << 0)
														 );
				
					up_PID.Bypass_Position_LV_P=(int32_t)(
														 ((int32_t)up_buffer_queue[left_p-13] << 24) |
														 ((int32_t)up_buffer_queue[left_p-12] << 16) |
														 ((int32_t)up_buffer_queue[left_p-11] << 8)  |
														 ((int32_t)up_buffer_queue[left_p-10] << 0)
														 );
						up_PID.speed_P=(int32_t)(
														 ((int32_t)up_buffer_queue[left_p-9] << 24) |
														 ((int32_t)up_buffer_queue[left_p-8] << 16) |
														 ((int32_t)up_buffer_queue[left_p-7] << 8)  |
														 ((int32_t)up_buffer_queue[left_p-6] << 0)
														 );
						up_PID.speed_I=(int32_t)(
														 ((int32_t)up_buffer_queue[left_p-5] << 24) |
														 ((int32_t)up_buffer_queue[left_p-4] << 16) |
														 ((int32_t)up_buffer_queue[left_p-3] << 8)  |
														 ((int32_t)up_buffer_queue[left_p-2] << 0)
														 );
		left_p=0;			
		}
	}
	if(left_p>=30)
	{
		left_p=0;
	}
}

void position_caculate_down(uint8_t right_buffer)
{

	static int right_p=0;

	down_buffer_queue[right_p]=right_buffer;
	right_p++;
	if(right_p>=5)
	{
		if(down_buffer_queue[right_p-1]==0x6B&&down_buffer_queue[right_p-5]==0x01&&down_buffer_queue[right_p-4]==0x30)
		{
		down_position=(int32_t)(
													 ((int32_t)down_buffer_queue[right_p-3] << 8)  |
													 ((int32_t)down_buffer_queue[right_p-2] << 0)
													 );
		right_p=0;			
		}
	}
	if(right_p>=6)
	{
		if(down_buffer_queue[right_p-1]==0x6B&&down_buffer_queue[right_p-6]==0x01)
		{
		down_position=(int32_t)(
													 ((int32_t)down_buffer_queue[right_p-5] << 24) |
													 ((int32_t)down_buffer_queue[right_p-4] << 16) |
													 ((int32_t)down_buffer_queue[right_p-3] << 8)  |
													 ((int32_t)down_buffer_queue[right_p-2] << 0)
													 );
		right_p=0;			
		}
	}
	
	if(right_p>=19)
	{
		if(down_buffer_queue[right_p-1]==0x6B&&down_buffer_queue[right_p-19]==0x01&&down_buffer_queue[right_p-18]==0x21)
		{
			down_PID.Traj_Position_P=(int32_t)(
														 ((int32_t)down_buffer_queue[right_p-17] << 24) |
														 ((int32_t)down_buffer_queue[right_p-16] << 16) |
														 ((int32_t)down_buffer_queue[right_p-15] << 8)  |
														 ((int32_t)down_buffer_queue[right_p-14] << 0)
														 );
				
					down_PID.Bypass_Position_LV_P=(int32_t)(
														 ((int32_t)down_buffer_queue[right_p-13] << 24) |
														 ((int32_t)down_buffer_queue[right_p-12] << 16) |
														 ((int32_t)down_buffer_queue[right_p-11] << 8)  |
														 ((int32_t)down_buffer_queue[right_p-10] << 0)
														 );
						down_PID.speed_P=(int32_t)(
														 ((int32_t)down_buffer_queue[right_p-9] << 24) |
														 ((int32_t)down_buffer_queue[right_p-8] << 16) |
														 ((int32_t)down_buffer_queue[right_p-7] << 8)  |
														 ((int32_t)down_buffer_queue[right_p-6] << 0)
														 );
						down_PID.speed_I=(int32_t)(
														 ((int32_t)down_buffer_queue[right_p-5] << 24) |
														 ((int32_t)down_buffer_queue[right_p-4] << 16) |
														 ((int32_t)down_buffer_queue[right_p-3] << 8)  |
														 ((int32_t)down_buffer_queue[right_p-2] << 0)
														 );
		right_p=0;			
		}
	}
	
	if(right_p>=30)
	{
		right_p=0;
	}
}

 void UART_Transmit__(uint32_t ui32Base,uint8_t *pData, uint16_t Size)
{
		int i=0;
		while(i<Size){
				UARTCharPut(ui32Base,pData[i]);
				i++;
		}
}



/**
	* @brief   USART发送多个字节
	* @param   无
	* @retval  无
	*/
void usart_SendCmd( __IO uint8_t *cmd, uint8_t len)
{
	UART_Transmit__(target_uart,cmd,len);
}


/**********************************************************
***	ZDT_X57步进闭环控制例程
***	控制例程：STM32_脉冲控制_while循环发送脉冲
***	编写作者：ZHANGDATOU
***	技术支持：张大头闭环伺服
***	淘宝店铺：https://zhangdatou.taobao.com
***	CSDN博客：http s://blog.csdn.net/zhangdatou666
***	qq交流群：262438510
**********************************************************/

/**
	*	@brief		将当前位置清零
	*	@param		addr	：电机地址
	*	@retval		地址 + 功能码 + 命令状态 + 校验字节
	*/
void ZDT_X57_Reset_CurPos_To_Zero(uint8_t addr)
{
	__IO uint8_t cmd[16] = {0};
	
	// 装载命令
	cmd[0] =  addr;												// 地址
	cmd[1] =  0x0A;												// 功能码
	cmd[2] =  0x6D;												// 辅助码
	cmd[3] =  0x6B;												// 校验字节
	
	// 发送命令
	usart_SendCmd(cmd, 4);
}

/**
	*	@brief		解除过压保护/过流保护/过热保护/堵转保护
	*	@param		addr	：电机地址
	*	@retval		地址 + 功能码 + 命令状态 + 校验字节
	*/
void ZDT_X57_Reset_Clog_Pro(uint8_t addr)
{
	__IO uint8_t cmd[16] = {0};
	
	// 装载命令
	cmd[0] =  addr;												// 地址
	cmd[1] =  0x0E;												// 功能码
	cmd[2] =  0x52;												// 辅助码
	cmd[3] =  0x6B;												// 校验字节
	
	// 发送命令
	usart_SendCmd(cmd, 4);
}

/**
	*	@brief		读取系统参数
	*	@param		addr	：电机地址
	*	@param		s			：系统参数类型
	*	@retval		地址 + 功能码 + 命令状态 + 校验字节
	*/
void ZDT_X57_Read_Sys_Params(uint8_t addr, SysParams_t s)
{
	__IO uint8_t cmd[16] = {0};
	
	// 装载命令
	cmd[0] =  addr;												// 地址

	switch(s)															// 功能码
	{
		case S_VER	: cmd[1] = 0x1F; break;
		case S_RL		: cmd[1] = 0x20; break;
		case S_PID	: cmd[1] = 0x21; break;
		case S_VBUS	: cmd[1] = 0x24; break;
		case S_CBUS	: cmd[1] = 0x26; break;
		case S_CPHA	: cmd[1] = 0x27; break;
		case S_ENC	: cmd[1] = 0x30; break;
		case S_ENCL	: cmd[1] = 0x31; break;
		case S_TPOS	: cmd[1] = 0x33; break;
		case S_VEL	: cmd[1] = 0x35; break;
		case S_CPOS	: cmd[1] = 0x36; break;
		case S_PERR	: cmd[1] = 0x37; break;
		case S_TEMP	: cmd[1] = 0x39; break;
		case S_FLAG	: cmd[1] = 0x3A; break;
		case S_Conf	: cmd[1] = 0x42; break;
		case S_State: cmd[1] = 0x43; break;
		default: break;
	}

	cmd[2] =  0x6B;												// 校验字节
	
	// 发送命令
	usart_SendCmd(cmd, 3);
}

/**
	*	@brief		使能信号控制
	*	@param		addr	：电机地址
	*	@param		state	：使能状态			，true为使能电机，false为关闭电机
	*	@param		multi	：多机同步标志	，0为不启用，其余值启用
	*	@retval		地址 + 功能码 + 命令状态 + 校验字节
	*/
void ZDT_X57_En_Control(uint8_t addr, bool state, uint8_t multi)
{
	__IO uint8_t cmd[16] = {0};
	
	// 装载命令
	cmd[0] =  addr;												// 地址
	cmd[1] =  0xF3;												// 功能码
	cmd[2] =  0xAB;												// 辅助码
	cmd[3] =  (uint8_t)state;							// 使能状态
	cmd[4] =  multi;											// 多机同步运动标志
	cmd[5] =  0x6B;												// 校验字节
	
	// 发送命令
	usart_SendCmd(cmd, 6);
}

/**
	*	@brief		力矩模式
	*	@param		addr	：电机地址
	*	@param		sign	：符号					，0为正，其余值为负
	*	@param		t_ramp：斜率(Ma/s)		，范围0 - 65535Ma/s
	*	@param		torque：力矩(Ma)			，范围0 - 4000Ma
	*	@param		multi	：多机同步标志	，0为不启用，其余值启用
	*	@retval		地址 + 功能码 + 命令状态 + 校验字节
	*/
void ZDT_X57_Torque_Control(uint8_t addr, uint8_t sign, uint16_t t_ramp, uint16_t torque, uint8_t multi)
{
	__IO uint8_t cmd[16] = {0};
	
	// 装载命令
	cmd[0] =  addr;												// 地址
	cmd[1] =  0xF5;												// 功能码
	cmd[2] =  sign;												// 符号（方向）
	cmd[3] =  (uint8_t)(t_ramp >> 8);			// 力矩斜率(Ma/s)高8位字节
	cmd[4] =  (uint8_t)(t_ramp >> 0);			// 力矩斜率(Ma/s)低8位字节
	cmd[5] =  (uint8_t)(torque >> 8);			// 力矩(Ma)高8位字节
	cmd[6] =  (uint8_t)(torque >> 0);			// 力矩(Ma)低8位字节
	cmd[7] =  multi;											// 多机同步运动标志
	cmd[8] =  0x6B;												// 校验字节
	
	// 发送命令
	usart_SendCmd(cmd, 9);
}

/**
	*	@brief		速度模式
	*	@param		addr	：电机地址
	*	@param		dir			：方向					，0为CW，其余值为CCW
	*	@param		v_ramp	：斜率(RPM/s)	，范围0 - 65535RPM/s
	*	@param		velocity：速度(RPM)		，范围0.0 - 4000.0RPM
	*	@param		multi		：多机同步标志	，0为不启用，其余值启用
	*	@retval		地址 + 功能码 + 命令状态 + 校验字节
	*/
void ZDT_X57_Velocity_Control(uint8_t addr, uint8_t dir, uint16_t v_ramp, float velocity, uint8_t multi)
{
	__IO uint8_t cmd[16] = {0}; __IO uint16_t vel = 0;

	// 将速度放大10倍发送过去
	vel = (uint16_t)ABS(velocity * 10.0f);

	// 装载命令
	cmd[0] =  addr;												// 地址
	cmd[1] =  0xF6;												// 功能码
	cmd[2] =  dir;												// 符号（方向）
	cmd[3] =  (uint8_t)(v_ramp >> 8);			// 速度斜率(RPM/s)高8位字节
	cmd[4] =  (uint8_t)(v_ramp >> 0);			// 速度斜率(RPM/s)低8位字节
	cmd[5] =  (uint8_t)(vel >> 8);				// 速度(RPM)高8位字节
	cmd[6] =  (uint8_t)(vel >> 0);				// 速度(RPM)低8位字节
	cmd[7] =  multi;											// 多机同步运动标志
	cmd[8] =  0x6B;												// 校验字节
	
	// 发送命令
	usart_SendCmd(cmd, 9);
}

/**
	*	@brief		直通位置模式
	*	@param		addr	：电机地址
	*	@param		dir			：方向										，0为CW，其余值为CCW
	*	@param		position：位置(°)								，范围0.0°- (2^32 - 1)°
	*	@param		raf			：相位位置/绝对位置标志	，0为相对位置，其余值为绝对位置
	*	@param		multi		：多机同步标志						，0为不启用，其余值启用
	*	@retval		地址 + 功能码 + 命令状态 + 校验字节
	*/
void ZDT_X57_Bypass_Position_Control(uint8_t addr, uint8_t dir, float position, uint8_t raf, uint8_t multi)
{
	__IO uint8_t cmd[16] = {0}; __IO uint32_t pos = 0;

	// 将位置放大10倍发送过去
	pos = (uint32_t)ABS(position * 10.0f);

	// 装载命令
	cmd[0] =  addr;												// 地址
	cmd[1] =  0xFA;												// 功能码
	cmd[2] =  dir;												// 符号（方向）
	cmd[3] =  (uint8_t)(pos >> 24);				// 位置(bit24 - bit31)
	cmd[4] =  (uint8_t)(pos >> 16);				// 位置(bit16 - bit23)
	cmd[5] =  (uint8_t)(pos >> 8);				// 位置(bit8  - bit15)
	cmd[6] =  (uint8_t)(pos >> 0);				// 位置(bit0  - bit7 )
	cmd[7] =  raf;												// 相位位置/绝对位置标志
	cmd[8] =  multi;											// 多机同步运动标志
	cmd[9] =  0x6B;												// 校验字节
	
	// 发送命令
	usart_SendCmd(cmd, 10);
}

/**
	*	@brief		直通位置限速模式
	*	@param		addr	：电机地址
	*	@param		dir			：方向										，0为CW，其余值为CCW
	*	@param		velocity：最大速度(RPM)					，范围0.0 - 4000.0RPM
	*	@param		position：位置(°)								，范围0.0°- (2^32 - 1)°
	*	@param		raf			：相位位置/绝对位置标志	，0为相对位置，其余值为绝对位置
	*	@param		multi		：多机同步标志						，0为不启用，其余值启用
	*	@retval		地址 + 功能码 + 命令状态 + 校验字节
	*/
void ZDT_X57_Bypass_Position_LV_Control(uint8_t addr, uint8_t dir, float velocity, float position, uint8_t raf, uint8_t multi)
{
	__IO uint8_t cmd[16] = {0}; __IO uint16_t vel = 0; __IO uint32_t pos = 0;

	// 将速度和位置放大10倍发送过去
	vel = (uint16_t)ABS(velocity * 10.0f); pos = (uint32_t)ABS(position * 10.0f);

	// 装载命令
	cmd[0]  =  addr;											// 地址
	cmd[1]  =  0xFB;											// 功能码
	cmd[2]  =  dir;												// 符号（方向）
	cmd[3]  =  (uint8_t)(vel >> 8);				// 最大速度(RPM)高8位字节
	cmd[4]  =  (uint8_t)(vel >> 0);				// 最大速度(RPM)低8位字节	
	cmd[5]  =  (uint8_t)(pos >> 24);			// 位置(bit24 - bit31)
	cmd[6]  =  (uint8_t)(pos >> 16);			// 位置(bit16 - bit23)
	cmd[7]  =  (uint8_t)(pos >> 8);				// 位置(bit8  - bit15)
	cmd[8]  =  (uint8_t)(pos >> 0);				// 位置(bit0  - bit7 )
	cmd[9]  =  raf;												// 相位位置/绝对位置标志
	cmd[10] =  multi;											// 多机同步运动标志
	cmd[11] =  0x6B;											// 校验字节
	
	// 发送命令
	usart_SendCmd(cmd, 12);
}

/**
	*	@brief		梯形曲线位置模式
	*	@param		addr	：电机地址
	*	@param		dir			：方向										，0为CW，其余值为CCW
	*	@param		acc			：加速加速度(RPM/s)			，0为CW，其余值为CCW
	*	@param		dec			：减速加速度(RPM/s)			，0为CW，其余值为CCW
	*	@param		velocity：最大速度(RPM)					，范围0.0 - 4000.0RPM
	*	@param		position：位置(°)								，范围0.0°- (2^32 - 1)°
	*	@param		raf			：相位位置/绝对位置标志	，0为相对位置，其余值为绝对位置
	*	@param		multi		：多机同步标志						，0为不启用，其余值启用
	*	@retval		地址 + 功能码 + 命令状态 + 校验字节
	*/
void ZDT_X57_Traj_Position_Control(uint8_t addr, uint8_t dir, uint16_t acc, uint16_t dec, float velocity, float position, uint8_t raf, uint8_t multi)
{
	__IO uint8_t cmd[16] = {0}; __IO uint16_t vel = 0; __IO uint32_t pos = 0;

	// 将速度和位置放大10倍发送过去
	vel = (uint16_t)ABS(velocity * 10.0f); pos = (uint32_t)ABS(position * 10.0f);

	// 装载命令
	cmd[0]  =  addr;											// 地址
	cmd[1]  =  0xFD;											// 功能码
	cmd[2]  =  dir;												// 符号（方向）
	cmd[3]  =  (uint8_t)(acc >> 8);				// 加速加速度(RPM/s)高8位字节
	cmd[4]  =  (uint8_t)(acc >> 0);				// 加速加速度(RPM/s)低8位字节	
	cmd[5]  =  (uint8_t)(dec >> 8);				// 减速加速度(RPM/s)高8位字节
	cmd[6]  =  (uint8_t)(dec >> 0);				// 减速加速度(RPM/s)低8位字节	
	cmd[7]  =  (uint8_t)(vel >> 8);				// 最大速度(RPM)高8位字节
	cmd[8]  =  (uint8_t)(vel >> 0);				// 最大速度(RPM)低8位字节	
	cmd[9]  =  (uint8_t)(pos >> 24);			// 位置(bit24 - bit31)
	cmd[10] =  (uint8_t)(pos >> 16);			// 位置(bit16 - bit23)
	cmd[11] =  (uint8_t)(pos >> 8);				// 位置(bit8  - bit15)
	cmd[12] =  (uint8_t)(pos >> 0);				// 位置(bit0  - bit7 )
	cmd[13] =  raf;												// 相位位置/绝对位置标志
	cmd[14] =  multi;											// 多机同步运动标志
	cmd[15] =  0x6B;											// 校验字节
	
	// 发送命令
	usart_SendCmd(cmd, 16);
}

/**
	*	@brief		立即停止（所有控制模式都通用）
	*	@param		addr	：电机地址
	*	@param		multi	：多机同步标志，0为不启用，其余值启用
	*	@retval		地址 + 功能码 + 命令状态 + 校验字节
	*/
void ZDT_X57_Stop_Now(uint8_t addr, uint8_t multi)
{
	__IO uint8_t cmd[16] = {0};
	
	// 装载命令
	cmd[0] =  addr;												// 地址
	cmd[1] =  0xFE;												// 功能码
	cmd[2] =  0x98;												// 辅助码
	cmd[3] =  multi;											// 多机同步运动标志
	cmd[4] =  0x6B;												// 校验字节
	
	// 发送命令
	usart_SendCmd(cmd, 5);
}

/**
	*	@brief		多机同步运动
	*	@param		addr	：电机地址
	*	@retval		地址 + 功能码 + 命令状态 + 校验字节
	*/
void ZDT_X57_Synchronous_motion(uint8_t addr)
{
	__IO uint8_t cmd[16] = {0};
	
	// 装载命令
	cmd[0] =  addr;												// 地址
	cmd[1] =  0xFF;												// 功能码
	cmd[2] =  0x66;												// 辅助码
	cmd[3] =  0x6B;												// 校验字节
	
	// 发送命令
	usart_SendCmd(cmd, 4);
}


