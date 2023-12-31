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
typedef struct correction{
    int left_right;
    int left_d;
    int right_d;
}correction;
/**
 * @brief       STEPPER_MOTOR UART字符流发送函数
 * @param       pData:字符串地址
 *              Size:字符串长度，不包括空结束字符
 *              Timeout:最大发送时间，发送数据超过该时间退出发送
 * @retval      无
 */

void speed_set_uart(int speed,uint8_t acc,wheel left_or_right);

void position_get(void);
void position_caculate_left(uint8_t left_buffer);
void position_caculate_right(uint8_t right_buffer);
#endif /* MY_LIB_STEPPER_MOTOR_STEPPER_MOTOR_USART_CONTROLLER_H_ */
