/*
 * my_usart.h
 *
 *  Created on: 2023Äê7ÔÂ2ÈÕ
 *      Author: yyg
 */

#ifndef UART_MY_USART_H_
#define UART_MY_USART_H_
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
// RX TX
void uart_init_uart0_printf(void);
void uart_init_uart1_PB0_1(void);
void uart_init_uart1_PC4_5(void);
void uart_init_uart2_PD6_7(void);
void uart_init_uart3_PC6_7(void);
void uart_init_uart4_PC4_5(void);
void uart_init_uart5_PE4_5(void);
void uart_init_uart6_PD4_5(void);
void uart_init_uart7_PE0_1(void);

#endif /* UART_MY_USART_H_ */
