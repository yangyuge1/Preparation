#include "my_jy901b.h"

void wind_ware_IntHandler_UART2(void);
extern short data[4];
double data_du[3];

void init_uart_jy901(void)
{
	    //????2,??????,????,PD6_U2RX,PD7_U2TX
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART2);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    GPIOPinConfigure(GPIO_PD6_U2RX);
    GPIOPinConfigure(GPIO_PD7_U2TX);
    GPIOPinTypeUART(GPIO_PORTD_BASE, GPIO_PIN_6 | GPIO_PIN_7);
    UARTClockSourceSet(UART2_BASE, UART_CLOCK_PIOSC);
    UARTConfigSetExpClk(UART2_BASE, SysCtlClockGet(), 115200,
                        (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |
                             UART_CONFIG_PAR_NONE));
//  UARTFIFODisable(UART2_BASE);
    IntMasterEnable();
    UARTIntRegister(UART2_BASE,wind_ware_IntHandler_UART2);
    IntPrioritySet(INT_UART2,0);
    IntEnable(INT_UART2);
    UARTIntEnable(UART2_BASE, UART_INT_RX |UART_INT_RT);
}

void wind_ware_IntHandler_UART2(void){
    uint32_t ui32Status;
    ui32Status = UARTIntStatus(UART2_BASE, true);
    UARTIntClear(UART2_BASE, ui32Status);
    char a=UARTCharGet(UART2_BASE);
    //????
    if(pushData(a)){
        if(changeData(data)){
//            ??????
              data[0]+=215-18+28+4+70;
              data[1]=-data[1];
              data[1]+=143-32+31-1;
							data[2]+=0;
					
              data_du[0]=(data[0]*0.0054931640625);
              data_du[1]=(data[1]*0.0054931640625);
					    data_du[2]=(data[2]*0.0054931640625);
        }
    }
    //????
}