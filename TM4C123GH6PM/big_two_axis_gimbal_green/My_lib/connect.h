/*
 * connect.h
 *
 *  Created on: 2023Äê7ÔÂ21ÈÕ
 *      Author: yyg
 */

#ifndef CONNECT_CONNECT_H_
#define CONNECT_CONNECT_H_


#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "driverlib/debug.h"
#include "driverlib/fpu.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/rom_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"



void connect_uart_init_uart7_PE0_1(void);

#endif /* CONNECT_CONNECT_H_ */
