/*
 * data_structure.h
 *
 *  Created on: 2023Äê6ÔÂ13ÈÕ
 *      Author: yyg
 */

#ifndef CMPLEX_TASK_SUPPORT_DATASTRUCTURE_DATA_STRUCTURE_H_
#define CMPLEX_TASK_SUPPORT_DATASTRUCTURE_DATA_STRUCTURE_H_

#include <stdint.h>
#include <stdio.h>
#include "utils/uartstdio.h"

#define data_type int
typedef struct linked_list{
    data_type data;
    struct linked_list* pre;
    struct linked_list* next;
}linked_list;
void insert_pre(linked_list* p,data_type data);
void insert_next(linked_list* p,data_type data);
char * getMemory(int size);
int freeMemory(char *p);
void printMemory(int size);
#endif /* CMPLEX_TASK_SUPPORT_DATASTRUCTURE_DATA_STRUCTURE_H_ */
