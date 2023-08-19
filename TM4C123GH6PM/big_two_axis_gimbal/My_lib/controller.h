/*
 * controller.h
 *
 *  Created on: 2023��6��7��
 *      Author: yyg
 */

#ifndef CMPLEX_TASK_SUPPORT_CONTROLLER_CONTROLLER_H_
#define CMPLEX_TASK_SUPPORT_CONTROLLER_CONTROLLER_H_

#define Positional 0
#define Incremental 1
typedef struct{
    int calculation_method;
    double err_i_max;
    double err_i_min;
    double output_max;
    double output_min;
    double output_change_max;
    double output_change_min;
}pid_config;

typedef struct{
    double P;
    double I;
    double D;
    double err;
    double l_err;
    double ll_err;
    double err_i;
    double output;
    double output_change;
    pid_config Config;
}pid_data;
/**
 * ���
 * 1��ʹ��ʱ��Ҫ����pid_data����������ɳ�ʼ�����ɴ�ȷ��һ������������ÿһ������ʹ��pid_update���и���
 * 2�����к�����Ϊ��״̬������ʹ�������������룬���ڵ�������������
 * 3�����ݶ������������ʵʱ��ؿ����������仯
 */
double pid_update(pid_data *data,double err);
double positional_pid_controller(pid_data *data,double err);
double incremental_pid_controller(pid_data *data,double err);
#endif /* CMPLEX_TASK_SUPPORT_CONTROLLER_CONTROLLER_H_ */
