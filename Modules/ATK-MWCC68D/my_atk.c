/*
 * my_atk.c
 *
 *  Created on: 2023��7��19��
 *      Author: yyg
 */

#include "my_atk.h"

/* ATK-MWCC68Dģ�����ò������� */
#define DEMO_ADDR       0xf9f9                               /* �豸��ַ */
#define DEMO_NETID      0                               /* �����ַ */
#define DEMO_WLRATE     ATK_MWCC68D_WLRATE_19K2         /* �������� */
#define DEMO_CHANNEL    0                               /* �ŵ� */
#define DEMO_TPOWER     ATK_MWCC68D_TPOWER_20DBM        /* ���书�� */
#define DEMO_WORKMODE   ATK_MWCC68D_WORKMODE_NORMAL     /* ����ģʽ */
#define DEMO_TMODE      ATK_MWCC68D_TMODE_TT            /* ����ģʽ */
#define DEMO_PACKSIZE   ATK_MWCC68D_PACKSIZE_240        /* ���ݰ���С */
#define DEMO_WLTIME     ATK_MWCC68D_WLTIME_1S           /* ����ʱ�� */
#define DEMO_UARTRATE   ATK_MWCC68D_UARTRATE_115200BPS  /* UARTͨѶ������ */
#define DEMO_UARTPARI   ATK_MWCC68D_UARTPARI_NONE       /* UARTͨѶУ��λ */
#define DEMO_LBT        ATK_MWCC68D_DISABLE             /* �ŵ���� */
#define DEMO_DATAKEY    0xF1F2F3F4                      /* ���ݼ�����Կ */

void ATK_MWCC68D_UART_IRQHandler(void);
int PC6_read(void);
void PC7_write(int x);
void UART_Transmit(uint8_t *pData, uint16_t Size, uint32_t Timeout);
void atk_HW_init(void);

int atk_init(void){
    atk_HW_init();

    ATK_MWCC68D_AUX=PC6_read;
    ATK_MWCC68D_MD0=PC7_write;
    ATK_MWCC68D_UART_Transmit=UART_Transmit;
    ATK_MWCC68D_delay_ms=delay_ms;

    /* ��ʼ��ATK-MWCC68Dģ�� */
    atk_mwcc68d_init();
    /* ����ATK-MWCC68Dģ�� */
    atk_mwcc68d_enter_config();
    atk_mwcc68d_addr_config(DEMO_ADDR);
    atk_mwcc68d_netid_config(DEMO_NETID);
    atk_mwcc68d_wlrate_channel_config(DEMO_WLRATE, DEMO_CHANNEL);
    atk_mwcc68d_tpower_config(DEMO_TPOWER);
    atk_mwcc68d_workmode_config(DEMO_WORKMODE);
    atk_mwcc68d_tmode_config(DEMO_TMODE);
    atk_mwcc68d_packsize_config(DEMO_PACKSIZE);
    atk_mwcc68d_wltime_config(DEMO_WLTIME);
    atk_mwcc68d_uart_config(DEMO_UARTRATE, DEMO_UARTPARI);
    atk_mwcc68d_lbt_config(DEMO_LBT);
    atk_mwcc68d_datakey_config(DEMO_DATAKEY);
    atk_mwcc68d_exit_config();
    return ATK_MWCC68D_EOK;
}

void atk_HW_init(void){


    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART2);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_UART2))
        ;
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOC))
        ;
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOD))
        ;

    GPIOPinTypeGPIOInput(GPIO_PORTC_BASE,GPIO_PIN_6);
    GPIOPinTypeGPIOOutput(GPIO_PORTC_BASE,GPIO_PIN_7);

    GPIOPinConfigure(GPIO_PD6_U2RX);
    GPIOPinConfigure(GPIO_PD7_U2TX);
    GPIOPinTypeUART(GPIO_PORTD_BASE, GPIO_PIN_6 | GPIO_PIN_7);

    UARTClockSourceSet(UART2_BASE, UART_CLOCK_SYSTEM);
    UARTConfigSetExpClk(UART2_BASE, SysCtlClockGet(), 115200,
                            (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |
                             UART_CONFIG_PAR_NONE));

    UARTFIFOLevelSet(UART2_BASE,UART_FIFO_TX7_8,UART_FIFO_RX7_8);
    UARTFIFOEnable(UART2_BASE);

    IntMasterEnable();
    UARTIntRegister(UART2_BASE,ATK_MWCC68D_UART_IRQHandler);
    IntPrioritySet(INT_UART2,0);
    IntEnable(INT_UART2);
    UARTIntEnable(UART2_BASE, UART_INT_RX | UART_INT_RT);
}
int PC6_read(void)
{
    return GPIOPinRead(GPIO_PORTC_BASE,GPIO_PIN_6);
}

void PC7_write(int x)
{
    if(x){
        GPIOPinWrite(GPIO_PORTC_BASE,GPIO_PIN_7,GPIO_PIN_7);
    }else{
        GPIOPinWrite(GPIO_PORTC_BASE,GPIO_PIN_7,0);
    }
}
/**
 * @brief       ATK-MWCC68D UART�жϻص�����
 * @param       ��
 * @retval      ��
 */
void ATK_MWCC68D_UART_IRQHandler(void)
{
     uint32_t ui32Status;
     uint8_t tmp;
     // Get the interrrupt status.
     ui32Status = UARTIntStatus(UART2_BASE, true);
     // Clear the asserted interrupts.
     UARTIntClear(UART2_BASE, ui32Status);

     if(ui32Status==UART_INT_RX)                                /* UART�����ж� */
     {
         while(UARTCharsAvail(UART2_BASE))
         {
             tmp=UARTCharGetNonBlocking(UART2_BASE);
             UARTCharPutNonBlocking(UART0_BASE, tmp);

             if (g_uart_rx_frame.sta.len < (ATK_MWCC68D_UART_RX_BUF_SIZE - 1))   /* �ж�UART���ջ����Ƿ����
                                                                                  * ����һλ��������'\0'
                                                                                  */
             {
                 g_uart_rx_frame.buf[g_uart_rx_frame.sta.len] = tmp;             /* �����յ�������д�뻺�� */
                 g_uart_rx_frame.sta.len++;                                      /* ���½��յ������ݳ��� */
             }
             else                                                                /* UART���ջ������ */
             {
                 g_uart_rx_frame.sta.len = 0;                                    /* ����֮ǰ�յ������� */
                 g_uart_rx_frame.buf[g_uart_rx_frame.sta.len] = tmp;             /* �����յ�������д�뻺�� */
                 g_uart_rx_frame.sta.len++;                                      /* ���½��յ������ݳ��� */
             }
         }
     }

     if(ui32Status==UART_INT_RT)                                /* UART���ճ�ʱ�ж� */
     {
         while(UARTCharsAvail(UART2_BASE))
         {
             tmp=UARTCharGetNonBlocking(UART2_BASE);
             UARTCharPutNonBlocking(UART0_BASE, tmp);

             if (g_uart_rx_frame.sta.len < (ATK_MWCC68D_UART_RX_BUF_SIZE - 1))   /* �ж�UART���ջ����Ƿ����
                                                                                  * ����һλ��������'\0'
                                                                                  */
             {
                 g_uart_rx_frame.buf[g_uart_rx_frame.sta.len] = tmp;             /* �����յ�������д�뻺�� */
                 g_uart_rx_frame.sta.len++;                                      /* ���½��յ������ݳ��� */
             }
             else                                                                /* UART���ջ������ */
             {
                 g_uart_rx_frame.sta.len = 0;                                    /* ����֮ǰ�յ������� */
                 g_uart_rx_frame.buf[g_uart_rx_frame.sta.len] = tmp;             /* �����յ�������д�뻺�� */
                 g_uart_rx_frame.sta.len++;                                      /* ���½��յ������ݳ��� */
             }
         }
         g_uart_rx_frame.sta.finsh = 1;
     }
}

void UART_Transmit(uint8_t *pData, uint16_t Size, uint32_t Timeout)
{
    int i=0;
    while(i<Size){
        UARTCharPutNonBlocking(UART2_BASE,pData[i]);
        i++;
    }
}
//͸�����䷢��ʾ��
//if (atk_mwcc68d_free() != ATK_MWCC68D_EBUSY)
//{
//    atk_mwcc68d_uart_printf("12345\n");
//}


