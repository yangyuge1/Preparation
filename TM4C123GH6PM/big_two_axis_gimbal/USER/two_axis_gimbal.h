#include <stdarg.h>
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
#include "utils/uartstdio.h"
#include "driverlib/pwm.h"

#include "complex_task_support.h"

int init_two_axis_gimbal(int fre);
void set_two_axis_gimbal(double up,double down);
void caculate_two_axis_gimbal(double x,double y,double z);
void calibration_two_axis_gimbal(void);