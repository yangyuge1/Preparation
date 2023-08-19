/*
 * command.h
 *
 *  Created on: 2023Äê5ÔÂ29ÈÕ
 *      Author: yyg
 */

#ifndef CMPLEX_TASK_SUPPORT_COMMAND_COMMAND_H_
#define CMPLEX_TASK_SUPPORT_COMMAND_COMMAND_H_

#include <math.h>

short transform2short(char input_1,char input_0 );
short transform2short_pointer(char *p);

int transform2int(char input_2,char input_3,char input_1,char input_0 );
int transform2int_pointer(char *p);

float transform2float(char input_3,char input_2,char input_1,char input_0 );
float transform2float_pointer(char *p);

double transform2double(char input_7,char input_6,char input_5,char input_4,char input_3,char input_2,char input_1,char input_0 );
double transform2double_pointer(char *p);

int charStream2int(int *save_target,char input);
int move(int position,int step,int length);
#endif /* CMPLEX_TASK_SUPPORT_COMMAND_COMMAND_H_ */
