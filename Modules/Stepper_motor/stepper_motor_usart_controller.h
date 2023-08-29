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
    left_wheel             = 0,
    right_wheel            = 1,
} wheel;

/**
 * @brief       STEPPER_MOTOR UART字符流发送函数
 * @param       pData:字符串地址
 *              Size:字符串长度，不包括空结束字符
 *              Timeout:最大发送时间，发送数据超过该时间退出发送
 * @retval      无
 */
void (*STEPPER_MOTOR_UART_Transmit)(wheel left_or_right,uint8_t *pData, uint16_t Size, uint32_t Timeout);
void speed_set(int speed,uint8_t acc,wheel left_or_right);

#endif /* MY_LIB_STEPPER_MOTOR_STEPPER_MOTOR_USART_CONTROLLER_H_ */
