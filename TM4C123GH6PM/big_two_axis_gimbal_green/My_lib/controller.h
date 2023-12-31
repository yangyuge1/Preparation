/*
 * controller.h
 *
 *  Created on: 2023年6月7日
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
 * 理念：
 * 1、使用时需要创建pid_data变量，并完成初始化，由此确认一个控制器，在每一周期中使用pid_update进行更新
 * 2、下列函数皆为无状态函数，使得数据与计算分离，便于调整控制器数量
 * 3、数据独立，方便外界实时监控控制器参数变化
 */
double pid_update(pid_data *data,double err);
double positional_pid_controller(pid_data *data,double err);
double incremental_pid_controller(pid_data *data,double err);
#endif /* CMPLEX_TASK_SUPPORT_CONTROLLER_CONTROLLER_H_ */
