/*
 * yyg_jy901b.c
 *
 *  Created on: 2023年3月26日
 *      Author: yyg
 */

/*
 * 注意少量数据发送时串口缓冲区会导致滞留，需要合理设置
 * */

#ifndef JY901B_YYG_JY901B_C_
#define JY901B_YYG_JY901B_C_
#include "yyg_jy901b.h"

// 数据帧
unsigned char rawData[11]; //原始数据队列
 short data[4];    //处理后的数据队列, 乘以0.0054931640625得到度为单位的数值,乘以0.0000958737976得到弧度为单位的数值
int position_=0; //当前数据位置，配合pushData
void (*send)(unsigned char data_u8);//为隔离硬件差异设置函数指针，使用时需要实现该接口并为其赋值，注意实现的接口需要即时发送8位数据

//连续取11位入队，形成原始数据队列，到达队尾产生溢出信号1，该函数带自动对齐功能
int pushData(unsigned char data_u8){
    //未对齐则跳出
    if(position_==0 && data_u8!=0x55){
        position_=0;
//        UARTprintf("x=2\n");
        return 0;
    }
    // 数据入列
    rawData[position_]=data_u8;
    position_++;
    if(position_==11) //数据溢出
    {
        position_=0;
        return 1;
    }
    return 0;//数据未溢出
}
//数据解算，只调整数据类型，不进行单位换算
int changeData(short *p){
    unsigned char sumcrc=rawData[0]+rawData[1]+rawData[2]+rawData[3]+rawData[4]+rawData[5]+rawData[6]+rawData[7]+rawData[8]+rawData[9];
    if(sumcrc!=rawData[10]){
        return 0;//校验失败，抛弃数据
    }
    p[0]=  ((unsigned short)rawData[3]<<8) | (unsigned short)rawData[2];
    p[1]=  ((unsigned short)rawData[5]<<8) | (unsigned short)rawData[4];
    p[2]=((unsigned short)rawData[7]<<8) | (unsigned short)rawData[6];
    p[3]=((unsigned short)rawData[9]<<8) | (unsigned short)rawData[8];
    return 1;//解算成功
}

int write_gy901b_data(unsigned char addr,unsigned char data_L,unsigned char data_H){
    if(send){
        send(WRITE_HEAD_1);
        send(WRITE_HEAD_2);
        send(addr);
        send(data_L);
        send(data_H);
        return 1;
    }
    else
        return 0;
}
int write_gy901b_start(void){
    if(send){
        send(WRITE_HEAD_1);
        send(WRITE_HEAD_2);
        send(UNLOCK_1);
        send(UNLOCK_2);
        send(UNLOCK_3);
        return 1;
    }
    else
        return 0;
}
int write_gy901b_end(void){
    if(send){
        send(WRITE_HEAD_1);
        send(WRITE_HEAD_2);
        send(SAVE_1);
        send(SAVE_2);
        send(SAVE_3);
        return 1;
    }
    else
        return 0;
}
#endif /* JY901B_YYG_JY901B_C_ */
