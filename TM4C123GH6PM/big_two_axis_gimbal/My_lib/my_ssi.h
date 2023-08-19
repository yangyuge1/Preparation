/*
 * my_ssi.h
 *
 *  Created on: 2023年7月1日
 *      Author: yyg
 */

#ifndef SSI_MY_SSI_H_
#define SSI_MY_SSI_H_
#include <stdint.h>
#include <stdbool.h>
#include "driverlib/ssi.h"
#include "inc/hw_memmap.h"
#include "inc/hw_ints.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"


//接口规则 CLK FSS RX TX
void ssi_init_ssi0_PA2_3_4_5(void);
void ssi_init_ssi1_PD0_1_2_3(void);
void ssi_init_ssi1_PF2_3_0_1(void);
void ssi_init_ssi2_PB4_5_6_7(void);
void ssi_init_ssi3_PD0_1_2_3(void);
uint8_t ucSPI_RW(uint8_t uc_SendData,unsigned int ui32Base);

#endif /* SSI_MY_SSI_H_ */
