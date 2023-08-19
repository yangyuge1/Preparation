/*
 * my_ssi.c
 *
 *  Created on: 2023年7月1日
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
    /* SSI配置 模式3(Polarity = 1 Phase = 1) 主设备模式 速率1MHz 数据长度8位*/
    SSIConfigSetExpClk(SSI0_BASE, SysCtlClockGet(), SSI_FRF_MOTO_MODE_3, SSI_MODE_MASTER, 1000000, 8);
    /* SSI配置 模式3(Polarity = 1 Phase = 1) 从设备模式 速率1MHz 数据长度8位*/
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
    /* 使能外设 */
    SysCtlPeripheralEnable(SYSCTL_PERIPH_SSI2);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);

    /* 设置SSI IO口的模式 */
    GPIOPinTypeSSI(GPIO_PORTB_BASE, GPIO_PIN_4 | GPIO_PIN_6 | GPIO_PIN_7);

    /* IO口配置为SSI功能 */
    GPIOPinConfigure(GPIO_PB4_SSI2CLK);
    GPIOPinConfigure(GPIO_PB5_SSI2FSS);
    GPIOPinConfigure(GPIO_PB6_SSI2RX);
    GPIOPinConfigure(GPIO_PB7_SSI2TX);

    /* SSI配置 模式3(Polarity = 1 Phase = 1) 主设备模式 速率1MHz 数据长度8位*/
    SSIConfigSetExpClk(SSI2_BASE, SysCtlClockGet(), SSI_FRF_MOTO_MODE_3, SSI_MODE_MASTER, 1000000, 8);
    /* 使能SSI2 */
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
/* SPI读写函数 */
uint8_t ucSPI_RW(uint8_t uc_SendData,unsigned int ui32Base)
{
    uint32_t ui_TempData;
    uint8_t uc_ReceiveData;

    /* 向SSI FIFO写入数据 */
    SSIDataPutNonBlocking(ui32Base, uc_SendData);
    /* 等待SSI不忙 */
    while(SSIBusy(ui32Base));
    /* 从FIFO读取数据 */
    SSIDataGetNonBlocking(ui32Base, &ui_TempData);

    /* 截取数据的低八位 */
    uc_ReceiveData = ui_TempData & 0xff;

    return uc_ReceiveData;
}
