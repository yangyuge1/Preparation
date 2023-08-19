/*
 * stepper_motor_usart_controller.h
 *
 *  Created on: 2023年7月19日
 *      Author: yyg
 */

#ifndef MY_LIB_STEPPER_MOTOR_STEPPER_MOTOR_USART_CONTROLLER_H_
#define MY_LIB_STEPPER_MOTOR_STEPPER_MOTOR_USART_CONTROLLER_H_
#include <stdio.h>
#include <string.h>
#include <stdint.h>

typedef enum
{
    up_wheel             = 0,
    down_wheel            = 1,
} wheel;

/**
 * @brief       STEPPER_MOTOR UART字符流发送函数
 * @param       pData:字符串地址
 *              Size:字符串长度，不包括空结束字符
 *              Timeout:最大发送时间，发送数据超过该时间退出发送
 * @retval      无
 */

void speed_set_uart(int speed,uint8_t acc,wheel up_or_down);
void position_set_uart(int speed,uint8_t acc,uint32_t pulse_number,wheel up_or_down);
void position_get(void);
void enable(int flag);
void encoder_get(void);
void position_caculate_up(uint8_t up_buffer);
void position_caculate_down(uint8_t down_buffer);
void encoder_caculate_up(uint8_t down_buffer);
void encoder_caculate_down(uint8_t down_buffer);
#endif /* MY_LIB_STEPPER_MOTOR_STEPPER_MOTOR_USART_CONTROLLER_H_ */
