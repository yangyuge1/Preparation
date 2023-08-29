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
    int err_i_max;
    int err_i_min;
    int output_max;
    int output_min;
    int output_change_max;
    int output_change_min;
}pid_config;

typedef struct{
    double P;
    double I;
    double D;
    int err;
    int l_err;
    int ll_err;
    int err_i;
    int output;
    int output_change;
    pid_config Config;
}pid_data;
/**
 * ���
 * 1��ʹ��ʱ��Ҫ����pid_data����������ɳ�ʼ�����ɴ�ȷ��һ������������ÿһ������ʹ��pid_update���и���
 * 2�����к�����Ϊ��״̬������ʹ�������������룬���ڵ�������������
 * 3�����ݶ������������ʵʱ��ؿ����������仯
 */
int pid_update(pid_data *data,int err);
int positional_pid_controller(pid_data *data,int err);
int incremental_pid_controller(pid_data *data,int err);
#endif /* CMPLEX_TASK_SUPPORT_CONTROLLER_CONTROLLER_H_ */
