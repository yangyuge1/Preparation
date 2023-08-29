/*
 * complex_task_support.h
 *
 *  Created on: 2023Äê4ÔÂ5ÈÕ
 *      Author: yyg
 */

#ifndef CMPLEX_TASK_SUPPORT_COMPLEX_TASK_SUPPORT_H_
#define CMPLEX_TASK_SUPPORT_COMPLEX_TASK_SUPPORT_H_

#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_ints.h"
#include "driverlib/sysctl.h"
#include "driverlib/systick.h"

void delay_ms(int time);
void delay_s(int time);
void delay_us(int time);
void setTimeOut_ms(int time,void (*var)(void));

#endif /* CMPLEX_TASK_SUPPORT_COMPLEX_TASK_SUPPORT_H_ */
