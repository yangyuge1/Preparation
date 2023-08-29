/*
 * my_eeprom.h
 *
 *  Created on: 2023Äê7ÔÂ21ÈÕ
 *      Author: yyg
 */

#ifndef MY_LIB_EEPROM_MY_EEPROM_H_
#define MY_LIB_EEPROM_MY_EEPROM_H_
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_gpio.h"
#include "inc/hw_ints.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"
#include "driverlib/eeprom.h"
#include "driverlib/flash.h"
uint32_t e2size,e2block;
void my_eeprom_init();


#endif /* MY_LIB_EEPROM_MY_EEPROM_H_ */
