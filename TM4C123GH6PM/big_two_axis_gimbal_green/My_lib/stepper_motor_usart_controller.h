/*
 * stepper_motor_usart_controller.h
 *
 *  Created on: 2023��7��19��
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
 * @brief       STEPPER_MOTOR UART�ַ������ͺ���
 * @param       pData:�ַ�����ַ
 *              Size:�ַ������ȣ��������ս����ַ�
 *              Timeout:�����ʱ�䣬�������ݳ�����ʱ���˳�����
 * @retval      ��
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
