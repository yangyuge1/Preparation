/*
 * us_100.c
 *
 *  Created on: 2023Äê4ÔÂ16ÈÕ
 *      Author: yyg
 */
#include "us_100.h"
uint8_t us_100_data[2]={0};
int distance;
int us_100_dataPointer=0;
void us_100_init(void){
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
    UARTIntRegister(UART3_BASE,us_100_IntHandler);
    IntPrioritySet(INT_UART3,0);
    IntEnable(INT_UART3);
    UARTIntEnable(UART3_BASE, UART_INT_RX );
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
    ui32Status = UARTIntStatus(UART3_BASE, true);
    UARTIntClear(UART3_BASE, ui32Status);

    unsigned char a=UARTCharGet(UART3_BASE);
    int out=us_100_dataChange(a);
    if(out){
        distance=out;
    }
}
