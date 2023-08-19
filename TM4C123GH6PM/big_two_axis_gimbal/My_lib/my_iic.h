/*
 * my_iic.h
 *
 *  Created on: 2023Äê7ÔÂ2ÈÕ
 *      Author: yyg
 */

#ifndef IIC_MY_IIC_H_
#define IIC_MY_IIC_H_

#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_ints.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/i2c.h"
void i2c_init_i2c0_PB2_3(void);

#endif /* IIC_MY_IIC_H_ */
