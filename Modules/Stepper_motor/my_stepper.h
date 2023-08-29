/*
 * my_stepper.h
 *
 *  Created on: 2023Äê7ÔÂ19ÈÕ
 *      Author: yyg
 */

#ifndef MY_LIB_STEPPER_MOTOR_MY_STEPPER_H_
#define MY_LIB_STEPPER_MOTOR_MY_STEPPER_H_

#include "stepper_motor_usart_controller.h"
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

void stepper_init();

#endif /* MY_LIB_STEPPER_MOTOR_MY_STEPPER_H_ */
