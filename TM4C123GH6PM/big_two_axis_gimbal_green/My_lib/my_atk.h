/*
 * my_atk.h
 *
 *  Created on: 2023Äê7ÔÂ2ÈÕ
 *      Author: yyg
 */

#ifndef ATK_MWCC68D_MY_ATK_H_
#define ATK_MWCC68D_MY_ATK_H_
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
#endif /* ATK_MWCC68D_MY_ATK_H_ */
