/*
 * my_eeprom.c
 *
 *  Created on: 2023��7��21��
 *      Author: yyg
 */
#include "my_eeprom.h"
/*
 * ��д���κ�EEPROM�Ĵ���֮ǰ������ͨ��EEPROM����ģʽʱ���ſؿ�
 * �ƣ�EEPROM���Ĵ������õ�EEPROMģ���ʱ�ӣ�����356ҳ��������
 * ����ִ�����³�ʼ�����裺
 * 1.�����ӳ٣�6�����ڼ��Ϻ������ÿ�������
 * 2.��EEPROM���״̬��EEDONE���Ĵ����м�鹤��λ��ֱ�����������EEPROM�������ͨ���ʼ����������=0ʱ��������
 * 3.��ȡEEPROM֧�ֿ��ƺ�״̬��EESUPP���Ĵ����е�PRETRY��ERETRYλ�����������������һλ���򷵻�һ�����󣬷���
 * ��������
 * 4.��ϵͳ���ƼĴ����ռ��ƫ����0x558����ʹ��EEPROM������ã�EEPROM���Ĵ�������EEPROMģ�顣
 * 5.�����ӳ٣�6�����ڼ��Ϻ������ÿ�������
 * 6.��EEPROM���״̬��EEDONE���Ĵ����м��ù���λ����ȷ����ʱ�����������=0ʱ��������
 * 7.��ȡEESUPP�Ĵ����е�PRETRYλ��ERETRYλ������������κ�һ��λ������һ�����󣬷���EEPROM��ʼ���Ѿ���
 * �ɣ������������ʹ�����衣
 */

//EEPROMд����ж�
void __attribute__((weak)) Flash_Handler(void)
{
    uint32_t ui32Status;
    ui32Status = EEPROMIntStatus(true);
    EEPROMIntClear(ui32Status);
    if(ui32Status==EEPROM_INT_PROGRAM){

    }
}

void my_eeprom_init()
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_EEPROM0); // EEPROM activate
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_EEPROM0))
        ;
    EEPROMInit(); // EEPROM start

    EEPROMIntEnable(EEPROM_INT_PROGRAM);
    FlashIntRegister(Flash_Handler);

    e2size = EEPROMSizeGet(); // Get EEPROM Size
    e2block = EEPROMBlockCountGet(); // Get EEPROM Block Count

}

//UARTprintf("EEPROM Test Program ,, Teknikyazi.com\r\n");
//
//uint32_t e2size,e2block;
// e2size = EEPROMSizeGet(); // Get EEPROM Size
// UARTprintf("EEPROM Size %d bytes\n", e2size);
//
// e2block = EEPROMBlockCountGet(); // Get EEPROM Block Count
// UARTprintf("EEPROM Blok Count: %d\n", e2block);
//
// int a=20;
// int b=0;
// EEPROMRead((uint32_t *)&b, 0x400, sizeof(int));
// UARTprintf("\n%d\n",b);
////
//// Program some data into the EEPROM at address 0x400.
////
//EEPROMProgram((uint32_t *)&a, 0x400, sizeof(int));
////
//// Read it back.
////
//EEPROMRead((uint32_t *)&b, 0x400, sizeof(int));
//UARTprintf("\n%d\n",b);
