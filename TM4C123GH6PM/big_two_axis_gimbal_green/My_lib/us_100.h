/*
 * us_100.h
 *
 *  Created on: 2023��4��16��
 *      Author: yyg
 */
//����TM4�ĳ�����ģ�飬����ͨ��,9600,�жϽ���
//20cm���ھ���mm�������ȣ���Ч���Ƶ��50hz����λmm���ڲ�ͬ�����ϴ��ڲ�ͬ���(<1cm)����ҪУ׼�������
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
