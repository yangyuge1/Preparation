/*
 * my_stepper.c
 *
 *  Created on: 2023年7月19日
 *      Author: yyg
 */
#include "my_stepper.h"
extern void (*STEPPER_MOTOR_UART_Transmit)(wheel left_or_right,uint8_t *pData, uint16_t Size, uint32_t Timeout);
void pwm_init(void);
static void IntHandler_UART5_(void);
static void IntHandler_UART1_(void);

void speed_set_pwm(double speed_left,double speed_right)
{
#define HZ 16000000/8

		if(speed_left<1 && speed_left>-1)
		{
				PWMPulseWidthSet(PWM0_BASE, PWM_OUT_3,0);
		}else
		{
				double Period_left=HZ/speed_left;
			if(Period_left>0)
			{
				//左前
				 PWMGenPeriodSet(PWM0_BASE, PWM_GEN_1, (int)(Period_left));
				PWMPulseWidthSet(PWM0_BASE, PWM_OUT_3,(int)(Period_left/2));
				GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_4,0);				
			}else
			{
				 PWMGenPeriodSet(PWM0_BASE, PWM_GEN_1, (int)(-Period_left));
				PWMPulseWidthSet(PWM0_BASE, PWM_OUT_3,(int)(-Period_left/2));
				GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_4,GPIO_PIN_4);	
			}
		
		}
		if(speed_right<1 && speed_right>-1)
		{
		    PWMPulseWidthSet(PWM0_BASE, PWM_OUT_0,0);
		}else
		{
			double Period_right=HZ/speed_right;		
			if(Period_right>0)
			{
				//右前
				PWMGenPeriodSet(PWM0_BASE, PWM_GEN_0, (int)(Period_right));
				PWMPulseWidthSet(PWM0_BASE, PWM_OUT_0,(int)(Period_right/2));
				GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_5,GPIO_PIN_5);						
			}
			else
			{
				//右后
				PWMGenPeriodSet(PWM0_BASE, PWM_GEN_0, (int)(-Period_right));
				PWMPulseWidthSet(PWM0_BASE, PWM_OUT_0,(int)(-Period_right/2));
				GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_5,0);					
			}
		}
}

 void UART_Transmit_(wheel left_or_right,uint8_t *pData, uint16_t Size, uint32_t Timeout)
{
    if(left_or_right){
        int i=0;
        while(i<Size){
            UARTCharPut(UART1_BASE,pData[i]);
//            UARTCharPut(UART0_BASE,pData[i]);
            i++;
        }
    }else{
        int i=0;
        while(i<Size){
            UARTCharPut(UART5_BASE,pData[i]);
//            UARTCharPut(UART0_BASE,pData[i]);
            i++;
        }
    }

}

 void uart_init_uart1_PB0_1(void)
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
    UARTIntRegister(UART1_BASE,IntHandler_UART1_);
    IntPrioritySet(INT_UART1,0);
    IntEnable(INT_UART1);
    UARTIntEnable(UART1_BASE, UART_INT_RX |UART_INT_RT);
}
 void uart_init_uart5_PE4_5(void)
{
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
    UARTIntRegister(UART5_BASE,IntHandler_UART5_);
    IntPrioritySet(INT_UART5,0);
    IntEnable(INT_UART5);
    UARTIntEnable(UART5_BASE, UART_INT_RX | UART_INT_RT);
}
void IntHandler_UART1_(void)
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
						position_caculate_left(buffer);
        }
    }

    if(ui32Status==UART_INT_RT)                                /* UART接收超时中断 */
    {
        while(UARTCharsAvail(UART1_BASE))
        {
            buffer=UARTCharGetNonBlocking(UART1_BASE);
						position_caculate_left(buffer);
        }
    }
}
void IntHandler_UART5_(void)
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
						position_caculate_right(buffer);
        }
    }

    if(ui32Status==UART_INT_RT)                                /* UART接收超时中断 */
    {
        while(UARTCharsAvail(UART5_BASE))
        {
            buffer=UARTCharGetNonBlocking(UART5_BASE);
						position_caculate_right(buffer);
        }
    }
}
void stepper_init(void)
{
    STEPPER_MOTOR_UART_Transmit=UART_Transmit_;
    uart_init_uart1_PB0_1();
    uart_init_uart5_PE4_5();
		pwm_init();
		
//    delay_ms(500);
//    speed_set(0,0xff,left_wheel);
//    speed_set(0,0xff,right_wheel);
}

void pwm_init(void)
{
//    PWM??
//    PE4 M0PWM2 ? PE5 M0PWM3
//    PB7 M0PWM1 ? PB6 M0PWM0
    SysCtlPWMClockSet(SYSCTL_PWMDIV_8);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);

    GPIOPinTypePWM(GPIO_PORTB_BASE, GPIO_PIN_6);
    GPIOPinTypePWM(GPIO_PORTB_BASE, GPIO_PIN_5);

    GPIOPinConfigure(GPIO_PB6_M0PWM0);//1
    GPIOPinConfigure(GPIO_PB5_M0PWM3);//1

    PWMGenConfigure(PWM0_BASE, PWM_GEN_0, PWM_GEN_MODE_DOWN |
                    PWM_GEN_MODE_NO_SYNC);
    PWMGenConfigure(PWM0_BASE, PWM_GEN_1, PWM_GEN_MODE_DOWN |
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
    PWMGenPeriodSet(PWM0_BASE, PWM_GEN_0, 5000);
    PWMGenPeriodSet(PWM0_BASE, PWM_GEN_1, 5000);
    // Set PWM0 to a duty cycle of 25%.  You set the duty cycle as a function
    // of the period.  Since the period was set above, you can use the
    // PWMGenPeriodGet() function.  For this example the PWM will be high for
    // 25% of the time or 4000 clock ticks (16000 / 4).
    //
    PWMPulseWidthSet(PWM0_BASE, PWM_OUT_0,
                     0);
    PWMPulseWidthSet(PWM0_BASE, PWM_OUT_3,
                     0);
    // Enable the PWM0 Bit0 (PD0) output signal.
    //
    PWMOutputState(PWM0_BASE, PWM_OUT_0_BIT, true);
    PWMOutputState(PWM0_BASE, PWM_OUT_3_BIT, true);
    //
    // Enable the PWM generator block.
    //
    PWMGenEnable(PWM0_BASE, PWM_GEN_0);
    PWMGenEnable(PWM0_BASE, PWM_GEN_1);


		SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
		GPIOPinTypeGPIOOutput(GPIO_PORTC_BASE, GPIO_PIN_4|GPIO_PIN_5);
}
