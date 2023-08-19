/*
 * my_usart.c
 *
 *  Created on: 2023Äê7ÔÂ2ÈÕ
 *      Author: yyg
 */
#include "my_usart.h"

void IntHandler_UART0_(void);
void IntHandler_UART1_(void);
void IntHandler_UART2_(void);
void IntHandler_UART3_(void);
void IntHandler_UART4_(void);
void IntHandler_UART5_(void);
void IntHandler_UART6_(void);
void IntHandler_UART7_(void);

void uart_init_uart0_printf(void){
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
    UARTStdioConfig(0, 115200, SysCtlClockGet());
    UARTFIFODisable(UART0_BASE);
    IntMasterEnable();
    UARTIntRegister(UART0_BASE,IntHandler_UART0_);
    IntPrioritySet(INT_UART0,0);
    IntEnable(INT_UART0);
    UARTIntEnable(UART0_BASE, UART_INT_RX | UART_INT_RT);
}
void IntHandler_UART0_(void)
{
    uint32_t ui32Status;
    ui32Status = UARTIntStatus(UART0_BASE, true);
    UARTIntClear(UART0_BASE, ui32Status);

}
void uart_init_uart1_PC4_5(void){
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART1);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
    GPIOPinConfigure(GPIO_PC4_U1RX);
    GPIOPinConfigure(GPIO_PC5_U1TX);
    GPIOPinTypeUART(GPIO_PORTC_BASE, GPIO_PIN_4 | GPIO_PIN_5);
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
void uart_init_uart1_PB0_1(void){
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART1);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    GPIOPinConfigure(GPIO_PB0_U1RX);
    GPIOPinConfigure(GPIO_PB1_U1TX);
    GPIOPinTypeUART(GPIO_PORTB_BASE, GPIO_PIN_0 | GPIO_PIN_1);
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
void IntHandler_UART1_(void)
{
    uint32_t ui32Status;
    ui32Status = UARTIntStatus(UART1_BASE, true);
    UARTIntClear(UART1_BASE, ui32Status);

}

void uart_init_uart2_PD6_7(void){
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART2);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    GPIOPinConfigure(GPIO_PD6_U2RX);
    GPIOPinConfigure(GPIO_PD7_U2TX);
    GPIOPinTypeUART(GPIO_PORTD_BASE, GPIO_PIN_6 | GPIO_PIN_7);
    UARTClockSourceSet(UART2_BASE, UART_CLOCK_PIOSC);
    UARTConfigSetExpClk(UART2_BASE, SysCtlClockGet(), 230400,
                            (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |
                             UART_CONFIG_PAR_NONE));
//    UARTFIFODisable(UART2_BASE);

//    IntMasterEnable();
//    UARTIntRegister(UART2_BASE,IntHandler_UART2_);
//    IntPrioritySet(INT_UART2,0);
//    IntEnable(INT_UART2);
//    UARTIntEnable(UART2_BASE, UART_INT_RX );

}
void IntHandler_UART2_(void)
{
    uint32_t ui32Status;
    ui32Status = UARTIntStatus(UART2_BASE, true);
    UARTIntClear(UART2_BASE, ui32Status);

}

void uart_init_uart3_PC6_7(void){
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART3);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
    GPIOPinConfigure(GPIO_PC6_U3RX);
    GPIOPinConfigure(GPIO_PC7_U3TX);
    GPIOPinTypeUART(GPIO_PORTC_BASE, GPIO_PIN_6 | GPIO_PIN_7);
    UARTConfigSetExpClk(UART3_BASE, SysCtlClockGet(), 115200,
                            (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |
                             UART_CONFIG_PAR_NONE));
//    UARTFIFODisable(UART3_BASE);

//    IntMasterEnable();
//    UARTIntRegister(UART3_BASE,IntHandler_UART3_);
//    IntPrioritySet(INT_UART3,0);
//    IntEnable(INT_UART3);
//    UARTIntEnable(UART3_BASE, UART_INT_RX );
}
void IntHandler_UART3_(void)
{
    uint32_t ui32Status;
    ui32Status = UARTIntStatus(UART3_BASE, true);
    UARTIntClear(UART3_BASE, ui32Status);

}

void uart_init_uart4_PC4_5(void){
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART4);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
    GPIOPinConfigure(GPIO_PC4_U4RX);
    GPIOPinConfigure(GPIO_PC5_U4TX);
    GPIOPinTypeUART(GPIO_PORTC_BASE, GPIO_PIN_4 | GPIO_PIN_5);
    UARTConfigSetExpClk(UART4_BASE, SysCtlClockGet(), 115200,
                            (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |
                             UART_CONFIG_PAR_NONE));
//    UARTFIFODisable(UART4_BASE);

//    IntMasterEnable();
//    UARTIntRegister(UART4_BASE,IntHandler_UART4_);
//    IntPrioritySet(INT_UART4,0);
//    IntEnable(INT_UART4);
//    UARTIntEnable(UART4_BASE, UART_INT_RX );
}
void IntHandler_UART4_(void)
{
    uint32_t ui32Status;
    ui32Status = UARTIntStatus(UART4_BASE, true);
    UARTIntClear(UART4_BASE, ui32Status);

}

void uart_init_uart5_PE4_5(void){
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART5);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    GPIOPinConfigure(GPIO_PE4_U5RX);
    GPIOPinConfigure(GPIO_PE5_U5TX);
    GPIOPinTypeUART(GPIO_PORTE_BASE, GPIO_PIN_4 | GPIO_PIN_5);
    UARTClockSourceSet(UART5_BASE, UART_CLOCK_PIOSC);
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
void IntHandler_UART5_(void)
{
    uint32_t ui32Status;
    ui32Status = UARTIntStatus(UART5_BASE, true);
    UARTIntClear(UART5_BASE, ui32Status);

}

void uart_init_uart6_PD4_5(void){
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART6);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    GPIOPinConfigure(GPIO_PD4_U6RX);
    GPIOPinConfigure(GPIO_PD5_U6TX);
    GPIOPinTypeUART(GPIO_PORTD_BASE, GPIO_PIN_4 | GPIO_PIN_5);
    UARTClockSourceSet(UART6_BASE, UART_CLOCK_PIOSC);
    UARTConfigSetExpClk(UART6_BASE, SysCtlClockGet(), 115200,
                            (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |
                             UART_CONFIG_PAR_NONE));
//    UARTFIFODisable(UART6_BASE);

//    IntMasterEnable();
//    UARTIntRegister(UART6_BASE,IntHandler_UART6_);
//    IntPrioritySet(INT_UART6,0);
//    IntEnable(INT_UART6);
//    UARTIntEnable(UART6_BASE, UART_INT_RX | UART_INT_RT);
}
void IntHandler_UART6_(void)
{
    uint32_t ui32Status;
    ui32Status = UARTIntStatus(UART6_BASE, true);
    UARTIntClear(UART6_BASE, ui32Status);

}

void uart_init_uart7_PE0_1(void){
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART7);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    GPIOPinConfigure(GPIO_PE0_U7RX);
    GPIOPinConfigure(GPIO_PE1_U7TX);
    GPIOPinTypeUART(GPIO_PORTE_BASE, GPIO_PIN_0 | GPIO_PIN_1);
    UARTClockSourceSet(UART7_BASE, UART_CLOCK_PIOSC);
    UARTConfigSetExpClk(UART7_BASE, SysCtlClockGet(), 115200,
                            (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |
                             UART_CONFIG_PAR_NONE));
//    UARTFIFODisable(UART7_BASE);

//    IntMasterEnable();
//    UARTIntRegister(UART7_BASE,IntHandler_UART7_);
//    IntPrioritySet(INT_UART7,0);
//    IntEnable(INT_UART7);
//    UARTIntEnable(UART7_BASE, UART_INT_RX | UART_INT_RT);
}
void IntHandler_UART7_(void)
{
    uint32_t ui32Status;
    ui32Status = UARTIntStatus(UART7_BASE, true);
    UARTIntClear(UART7_BASE, ui32Status);

}

