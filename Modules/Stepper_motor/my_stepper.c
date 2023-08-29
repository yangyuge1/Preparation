/*
 * my_stepper.c
 *
 *  Created on: 2023Äê7ÔÂ19ÈÕ
 *      Author: yyg
 */
#include "my_stepper.h"

static void UART_Transmit_(wheel left_or_right,uint8_t *pData, uint16_t Size, uint32_t Timeout);
static void uart_init_uart1_PB0_1(void);
static void uart_init_uart5_PE4_5(void);


void stepper_init()
{
    STEPPER_MOTOR_UART_Transmit=UART_Transmit_;
    uart_init_uart1_PB0_1();
    uart_init_uart5_PE4_5();
//    delay_ms(500);
//    speed_set(0,0xff,left_wheel);
//    speed_set(0,0xff,right_wheel);
}

static void UART_Transmit_(wheel left_or_right,uint8_t *pData, uint16_t Size, uint32_t Timeout)
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

static void uart_init_uart1_PB0_1(void)
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

//    IntMasterEnable();
//    UARTIntRegister(UART1_BASE,IntHandler_UART1_);
//    IntPrioritySet(INT_UART1,0);
//    IntEnable(INT_UART1);
//    UARTIntEnable(UART1_BASE, UART_INT_RX );
}
static void uart_init_uart5_PE4_5(void)
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

//    IntMasterEnable();
//    UARTIntRegister(UART5_BASE,IntHandler_UART5_);
//    IntPrioritySet(INT_UART5,0);
//    IntEnable(INT_UART5);
//    UARTIntEnable(UART5_BASE, UART_INT_RX | UART_INT_RT);
}
