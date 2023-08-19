/*
 * us_100.c
 *
 *  Created on: 2023年4月16日
 *      Author: yyg
 */
#include "us_100.h"
uint8_t us_100_data[2]={0};
int us_100_dataPointer=0;
void us_100_init(void){
    SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN |
                       SYSCTL_XTAL_16MHZ);
    //开启串口0,对接pc,printf模式,需要存在uartstdio.c
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
    UARTStdioConfig(0, 115200, SysCtlClockGet());

    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART1);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    GPIOPinConfigure(GPIO_PB0_U1RX);
    GPIOPinConfigure(GPIO_PB1_U1TX);
    GPIOPinTypeUART(GPIO_PORTB_BASE, GPIO_PIN_0 | GPIO_PIN_1);
    UARTConfigSetExpClk(UART1_BASE, SysCtlClockGet(), 9600,
                            (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |
                             UART_CONFIG_PAR_NONE));
    UARTFIFODisable(UART1_BASE);

    IntMasterEnable();
    UARTIntRegister(UART1_BASE,us_100_IntHandler);
    IntPrioritySet(INT_UART1,0);
    IntEnable(INT_UART1);
    UARTIntEnable(UART1_BASE, UART_INT_RX);
}
int us_100_dataChange(uint8_t dataStream){
    us_100_data[us_100_dataPointer]=dataStream;
    us_100_dataPointer++;
    if(us_100_dataPointer==2){
        us_100_dataPointer=0;
        return (us_100_data[0]*256+us_100_data[1]);
    }
    else
        return 0;
}
void us_100_IntHandler(void){
    uint32_t ui32Status;
    ui32Status = UARTIntStatus(UART1_BASE, true);
    UARTIntClear(UART1_BASE, ui32Status);

    unsigned char a=UARTCharGet(UART1_BASE);
    int out=us_100_dataChange(a);
    if(out){
        distance=out;
    }
}
