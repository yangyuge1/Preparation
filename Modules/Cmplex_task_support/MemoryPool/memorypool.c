#include "memorypool.h"

//construct the memory pool
#define S_SIZE 8   //segment size
#define S_NUM 2000 //segment number
#define M_SIZE S_SIZE*S_NUM //total size
static uint8_t memory_pool[M_SIZE]; //memory pool
static uint8_t memory_sheet[S_NUM] = { 0 }; //A table to record memory usage

uint8_t* getMemory(int size) {
    //根据内存需求，确定段数
    int k = size / S_SIZE;
    if (size % S_SIZE > 0) {
        k++;
    }
    //滑动窗口寻找空内存段
    int i, j, flag;
    for (i = 0, flag = 0; i < S_NUM - k + 1 && flag == 0; i++) {
        if (memory_sheet[i] == 0 && memory_sheet[i + k - 1] == 0) {
            for (j = 0; j <= k - 1 && memory_sheet[i + j] == 0; j++) {
            }
            if (j == k) {
                flag = 1;
            }
        }
    }
    //记录内存分配并返回地址
    if (flag) {
        i--;
        memory_sheet[i] = 1;
        for (j = 1; j <= k - 1; j++) {
            memory_sheet[i + j] = 2;
        }
        return &memory_pool[i * S_SIZE];
    }
    else {
        return NULL;
    }
}
uint8_t freeMemory(char* p)
{
    int k = (p - memory_pool) / S_SIZE;

    int j;
    if (memory_sheet[k] == 1) {
        memory_sheet[k] = 0;
        for (j = 0; j < S_SIZE; j++) {
            memory_pool[k * S_SIZE + j] = 0;
        }
        memory_pool[k * S_SIZE] = 0;
        int i;
        for (i = 1; memory_sheet[k + i] == 2 && (k + i) < S_NUM; i++) {
            memory_sheet[k + i] = 0;
            for (j = 0; j < S_SIZE; j++) {
                memory_pool[(k + i) * S_SIZE + j] = 0;
            }
        }
        return 1;
    }
    else {
        return 0;
    }
}
void printMemory(int size)
{
    printf("\n memory_pool$memory_sheet: ");
    int i;
    for (i = 0; i < size * S_SIZE; i++) {
        if (i % S_SIZE == 0) {
            printf("\n Segment %3d[%12d][%d]:", i / S_SIZE, &memory_pool[i] ,memory_sheet[i / S_SIZE]);
        }
        printf("%d ", memory_pool[i]);

    }
}