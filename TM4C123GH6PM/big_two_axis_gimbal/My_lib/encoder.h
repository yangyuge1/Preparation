/*
 * encoder.h
 *
 *  Created on: 2023Äê6ÔÂ22ÈÕ
 *      Author: yyg
 */

#ifndef ENCODER_ENCODER_H_
#define ENCODER_ENCODER_H_


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
#include "driverlib/qei.h"

void init_encoder();
#endif /* ENCODER_ENCODER_H_ */
