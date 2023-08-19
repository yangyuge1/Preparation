/*
 * us_100.h
 *
 *  Created on: 2023年4月16日
 *      Author: yyg
 */
//适配TM4的超声波模块，串口通信,9600,中断接收
//20cm以内具有mm级灵敏度，有效输出频率50hz，单位mm，在不同距离上存在不同误差(<1cm)，需要校准输出曲线
/*
US-100  TM4
vcc     3.3v/5.0v
tx      tx
rx      rx
gnd     gnd
*/

/*
example:
                extern int distance;
                int
                main(void)
                {
                    us_100_init();
                    while(1)
                    {
                        UARTCharPut(UART1_BASE,0x55);
                        UARTprintf("%d\n" ,distance);
                        delay_ms(20);
                    }
                }
*/


#ifndef US_100_US_100_H_
#define US_100_US_100_H_
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_ints.h"
#include "driverlib/uart.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "utils/uartstdio.h"
#include "driverlib/timer.h"
#include "driverlib/interrupt.h"

void us_100_init(void);
void us_100_IntHandler(void);
int us_100_dataChange(uint8_t dataStream);


#endif /* US_100_US_100_H_ */
