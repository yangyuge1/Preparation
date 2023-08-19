/*
 * my_eeprom.c
 *
 *  Created on: 2023年7月21日
 *      Author: yyg
 */
#include "my_eeprom.h"
/*
 * 在写入任何EEPROM寄存器之前，必须通过EEPROM运行模式时钟门控控
 * 制（EEPROM）寄存器启用到EEPROM模块的时钟（见第356页），并且
 * 必须执行以下初始化步骤：
 * 1.插入延迟（6个周期加上函数调用开销）。
 * 2.在EEPROM完成状态（EEDONE）寄存器中检查工作位，直到清除，表明EEPROM已完成其通电初始化。当工作=0时，继续。
 * 3.读取EEPROM支持控制和状态（EESUPP）寄存器中的PRETRY和ERETRY位。如果设置了其中任一位，则返回一个错误，否则
 * 将继续。
 * 4.在系统控制寄存器空间的偏移量0x558处，使用EEPROM软件重置（EEPROM）寄存器重置EEPROM模块。
 * 5.插入延迟（6个周期加上函数调用开销）。
 * 6.在EEPROM完成状态（EEDONE）寄存器中检查该工作位，以确定何时清除。当工作=0时，继续。
 * 7.读取EESUPP寄存器中的PRETRY位和ERETRY位。如果设置了任何一个位，返回一个错误，否则EEPROM初始化已经完
 * 成，软件可以正常使用外设。
 */

//EEPROM写完成中断
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
