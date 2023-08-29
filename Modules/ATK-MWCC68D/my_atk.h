/*
 * my_atk.h
 *
 *  Created on: 2023Äê7ÔÂ19ÈÕ
 *      Author: yyg
 */

#ifndef MY_LIB_MY_ATK_H_
#define MY_LIB_MY_ATK_H_


#include "driver_atk_mwcc68d.h"
#include "complex_task_support.h"
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_ints.h"
#include "driverlib/uart.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/timer.h"
#include "driverlib/interrupt.h"


int atk_init(void);


#endif /* MY_LIB_MY_ATK_H_ */
