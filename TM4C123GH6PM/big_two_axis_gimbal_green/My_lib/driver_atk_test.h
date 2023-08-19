/*
 * driver_atk_test.h
 *
 *  Created on: 2023Äê7ÔÂ3ÈÕ
 *      Author: yyg
 */

#ifndef ATK_MWCC68D_DRIVER_ATK_TEST_H_
#define ATK_MWCC68D_DRIVER_ATK_TEST_H_
#include <stdint.h>
#include <stdbool.h>
uint8_t atk_send_at_cmd(char *cmd, uint32_t timeout);
void atk_cmd_printf(char *fmt, ...);


#endif /* ATK_MWCC68D_DRIVER_ATK_TEST_H_ */
