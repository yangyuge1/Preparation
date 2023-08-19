/*
 * connect.c
 *
 *  Created on: 2023年7月21日
 *      Author: yyg
 */
#include "connect.h"

uint8_t buffer;
uint8_t buffer_queue[10];
int isRed=0;     //画面是否有目标 0无 1有 只读
int mid_1=0;       //中点1
int mid_2=0;       //中点2
int mid_1_=0;       //中点1
int mid_2_=0;       //中点2

int width=0;     //宽度

int pointer_list[8];
int list_len;

int red_x,red_y,green_x,green_y;
int start_flag;

#define point_list_mode 0
#define point_feedback_mode 1
int receive_mode=0;
void push_buffer_queue(uint8_t a);

void receive(uint8_t buffer){
  
  switch(receive_mode)
  {
    case point_list_mode:
        pointer_list[list_len]=buffer;
        list_len++;
        if(list_len>=8)
        {list_len=0;}
      break;
    case point_feedback_mode:
      push_buffer_queue(buffer);
      break;
  }
}

void push_buffer_queue(uint8_t a)
{
  static int len=0;
  buffer_queue[len]=a;
  len++;
  if(len>=5)
  {
    if(buffer_queue[len-1]==255)
    {
      red_x=buffer_queue[len-5];
      red_y=buffer_queue[len-4];
      green_x=buffer_queue[len-3];
      green_y=buffer_queue[len-2];
      len=0;
      
      if(((red_x-green_x)*(red_x-green_x)+(red_y-green_y)*(red_y-green_y))<=50)
      {
        start_flag++;
      }else{
        start_flag=0;
      }
      if(start_flag>=3000){
        start_flag=0;
      }
    }
  }
  if(len>=20)
  {
    len=0;
  }
}

void IntHandler_UART7_(void)
{
    uint32_t ui32Status;
    ui32Status = UARTIntStatus(UART7_BASE, true);
    UARTIntClear(UART7_BASE, ui32Status);

    if(ui32Status==UART_INT_RX)                                /* UART接收中断 */
    {
        while(UARTCharsAvail(UART7_BASE))
        {
            buffer=UARTCharGetNonBlocking(UART7_BASE);
//            UARTCharPutNonBlocking(UART0_BASE,buffer);
            receive(buffer);
        }
    }

    if(ui32Status==UART_INT_RT)                                /* UART接收超时中断 */
    {
        while(UARTCharsAvail(UART7_BASE))
        {
            buffer=UARTCharGetNonBlocking(UART7_BASE);
//            UARTCharPutNonBlocking(UART0_BASE,buffer);
            receive(buffer);
        }
    }
}
void connect_uart_init_uart7_PE0_1(void)
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART7);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_UART7))
        ;
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOE))
        ;
    GPIOPinConfigure(GPIO_PE0_U7RX);
    GPIOPinConfigure(GPIO_PE1_U7TX);
    GPIOPinTypeUART(GPIO_PORTE_BASE, GPIO_PIN_0 | GPIO_PIN_1);
    UARTClockSourceSet(UART7_BASE, UART_CLOCK_SYSTEM);
    UARTConfigSetExpClk(UART7_BASE, SysCtlClockGet(), 115200,
                            (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |
                             UART_CONFIG_PAR_NONE));
//    UARTFIFODisable(UART7_BASE);
    IntMasterEnable();
    UARTIntRegister(UART7_BASE,IntHandler_UART7_);
    IntPrioritySet(INT_UART7,0);
    IntEnable(INT_UART7);
    UARTIntEnable(UART7_BASE, UART_INT_RX | UART_INT_RT);
}

