/*
 * controller.c
 *
 *  Created on: 2023Äê6ÔÂ7ÈÕ
 *      Author: yyg
 */
#include "controller.h"

int pid_update(pid_data *data,int err){
    switch(data->Config.calculation_method){
    case Positional:
        positional_pid_controller(data,err);
        break;
    case Incremental:
        incremental_pid_controller(data,err);
        break;
    }
    return data->output;
}

int positional_pid_controller(pid_data *data,int err){
    data->err_i+=err;
    if(data->err_i<=data->Config.err_i_min){
        data->err_i=data->Config.err_i_min;
    }
    if(data->err_i>=data->Config.err_i_max){
        data->err_i=data->Config.err_i_max;
    }

    data->ll_err=data->l_err;
    data->l_err=data->err;
    data->err=err;

    data->output=(
            data->P*data->err             +
            data->I*data->err_i           +
            data->D*(data->err-data->l_err)
                 );

    if(data->output<=data->Config.output_min){
        data->output=data->Config.output_min;
    }
    if(data->output>=data->Config.output_max){
        data->output=data->Config.output_max;
    }

    return data->output;
}

int incremental_pid_controller(pid_data *data,int err){

    data->ll_err=data->l_err;
    data->l_err=data->err;
    data->err=err;

    data->output_change=(
            data->P*(data->err-data->l_err)    +
            data->I*data->err                  +
            data->D*(data->err-data->l_err-data->l_err+data->ll_err)
                 );
    if(data->output_change<=data->Config.output_change_min){
        data->output_change=data->Config.output_change_min;
    }
    if(data->output_change>=data->Config.output_change_max){
        data->output_change=data->Config.output_change_max;
    }

    data->output+=data->output_change;
    if(data->output<=data->Config.output_min){
        data->output=data->Config.output_min;
    }
    if(data->output>=data->Config.output_max){
        data->output=data->Config.output_max;
    }

    return data->output;
}
