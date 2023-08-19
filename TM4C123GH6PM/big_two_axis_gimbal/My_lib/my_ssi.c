/*
 * my_ssi.c
 *
 *  Created on: 2023��7��1��
 *      Author: yyg
 */
#include "my_ssi.h"

void ssi_init_ssi0_PA2_3_4_5(void){
    SysCtlPeripheralEnable(SYSCTL_PERIPH_SSI0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    GPIOPinTypeSSI(GPIO_PORTA_BASE, GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4|GPIO_PIN_5);
    GPIOPinConfigure(GPIO_PA2_SSI0CLK);
    GPIOPinConfigure(GPIO_PA3_SSI0FSS);
    GPIOPinConfigure(GPIO_PA4_SSI0RX);
    GPIOPinConfigure(GPIO_PA5_SSI0TX);
    /* SSI���� ģʽ3(Polarity = 1 Phase = 1) ���豸ģʽ ����1MHz ���ݳ���8λ*/
    SSIConfigSetExpClk(SSI0_BASE, SysCtlClockGet(), SSI_FRF_MOTO_MODE_3, SSI_MODE_MASTER, 1000000, 8);
    /* SSI���� ģʽ3(Polarity = 1 Phase = 1) ���豸ģʽ ����1MHz ���ݳ���8λ*/
//    SSIConfigSetExpClk(SSI0_BASE, SysCtlClockGet(), SSI_FRF_MOTO_MODE_3, SSI_MODE_SLAVE, 1000000, 8);
    SSIEnable(SSI0_BASE);
}
void ssi_init_ssi1_PD0_1_2_3(void){
    SysCtlPeripheralEnable(SYSCTL_PERIPH_SSI1);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    GPIOPinTypeSSI(GPIO_PORTD_BASE, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2|GPIO_PIN_3);
    GPIOPinConfigure(GPIO_PD0_SSI1CLK);
    GPIOPinConfigure(GPIO_PD1_SSI1FSS);
    GPIOPinConfigure(GPIO_PD2_SSI1RX);
    GPIOPinConfigure(GPIO_PD3_SSI1TX);
    SSIConfigSetExpClk(SSI1_BASE, SysCtlClockGet(), SSI_FRF_MOTO_MODE_3, SSI_MODE_MASTER, 1000000, 8);
    SSIEnable(SSI1_BASE);
}
void ssi_init_ssi1_PF2_3_0_1(void){
    SysCtlPeripheralEnable(SYSCTL_PERIPH_SSI1);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    GPIOPinTypeSSI(GPIO_PORTF_BASE, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2|GPIO_PIN_3);
    GPIOPinConfigure(GPIO_PF2_SSI1CLK);
    GPIOPinConfigure(GPIO_PF3_SSI1FSS);
    GPIOPinConfigure(GPIO_PF0_SSI1RX);
    GPIOPinConfigure(GPIO_PF1_SSI1TX);
    SSIConfigSetExpClk(SSI1_BASE, SysCtlClockGet(), SSI_FRF_MOTO_MODE_3, SSI_MODE_MASTER, 1000000, 8);
    SSIEnable(SSI1_BASE);
}
void ssi_init_ssi2_PB4_5_6_7(void){
    /* ʹ������ */
    SysCtlPeripheralEnable(SYSCTL_PERIPH_SSI2);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);

    /* ����SSI IO�ڵ�ģʽ */
    GPIOPinTypeSSI(GPIO_PORTB_BASE, GPIO_PIN_4 | GPIO_PIN_6 | GPIO_PIN_7);

    /* IO������ΪSSI���� */
    GPIOPinConfigure(GPIO_PB4_SSI2CLK);
    GPIOPinConfigure(GPIO_PB5_SSI2FSS);
    GPIOPinConfigure(GPIO_PB6_SSI2RX);
    GPIOPinConfigure(GPIO_PB7_SSI2TX);

    /* SSI���� ģʽ3(Polarity = 1 Phase = 1) ���豸ģʽ ����1MHz ���ݳ���8λ*/
    SSIConfigSetExpClk(SSI2_BASE, SysCtlClockGet(), SSI_FRF_MOTO_MODE_3, SSI_MODE_MASTER, 1000000, 8);
    /* ʹ��SSI2 */
    SSIEnable(SSI2_BASE);
}
void ssi_init_ssi3_PD0_1_2_3(void){
    SysCtlPeripheralEnable(SYSCTL_PERIPH_SSI1);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    GPIOPinTypeSSI(GPIO_PORTD_BASE, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2|GPIO_PIN_3);
    GPIOPinConfigure(GPIO_PD0_SSI3CLK);
    GPIOPinConfigure(GPIO_PD1_SSI3FSS);
    GPIOPinConfigure(GPIO_PD2_SSI3RX);
    GPIOPinConfigure(GPIO_PD3_SSI3TX);
    SSIConfigSetExpClk(SSI3_BASE, SysCtlClockGet(), SSI_FRF_MOTO_MODE_3, SSI_MODE_MASTER, 1000000, 8);
    SSIEnable(SSI3_BASE);
}
/* SPI��д���� */
uint8_t ucSPI_RW(uint8_t uc_SendData,unsigned int ui32Base)
{
    uint32_t ui_TempData;
    uint8_t uc_ReceiveData;

    /* ��SSI FIFOд������ */
    SSIDataPutNonBlocking(ui32Base, uc_SendData);
    /* �ȴ�SSI��æ */
    while(SSIBusy(ui32Base));
    /* ��FIFO��ȡ���� */
    SSIDataGetNonBlocking(ui32Base, &ui_TempData);

    /* ��ȡ���ݵĵͰ�λ */
    uc_ReceiveData = ui_TempData & 0xff;

    return uc_ReceiveData;
}
