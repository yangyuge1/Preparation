#ifndef _MEMORYPOOL_H_
#define _MEMORYPOOL_H_

#include <stdint.h>
#include <stdio.h>

uint8_t *getMemory(int size);
uint8_t freeMemory(char* p);
void printMemory(int size);
#endif