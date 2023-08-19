/*
 * yyg_jy901b.c
 *
 *  Created on: 2023��3��26��
 *      Author: yyg
 */

/*
 * ע���������ݷ���ʱ���ڻ������ᵼ����������Ҫ��������
 * */

#ifndef JY901B_YYG_JY901B_C_
#define JY901B_YYG_JY901B_C_
#include "yyg_jy901b.h"

// ����֡
unsigned char rawData[11]; //ԭʼ���ݶ���
 short data[4];    //���������ݶ���, ����0.0054931640625�õ���Ϊ��λ����ֵ,����0.0000958737976�õ�����Ϊ��λ����ֵ
int position_=0; //��ǰ����λ�ã����pushData
void (*send)(unsigned char data_u8);//Ϊ����Ӳ���������ú���ָ�룬ʹ��ʱ��Ҫʵ�ָýӿڲ�Ϊ�丳ֵ��ע��ʵ�ֵĽӿ���Ҫ��ʱ����8λ����

//����ȡ11λ��ӣ��γ�ԭʼ���ݶ��У������β��������ź�1���ú������Զ����빦��
int pushData(unsigned char data_u8){
    //δ����������
    if(position_==0 && data_u8!=0x55){
        position_=0;
//        UARTprintf("x=2\n");
        return 0;
    }
    // ��������
    rawData[position_]=data_u8;
    position_++;
    if(position_==11) //�������
    {
        position_=0;
        return 1;
    }
    return 0;//����δ���
}
//���ݽ��㣬ֻ�����������ͣ������е�λ����
int changeData(short *p){
    unsigned char sumcrc=rawData[0]+rawData[1]+rawData[2]+rawData[3]+rawData[4]+rawData[5]+rawData[6]+rawData[7]+rawData[8]+rawData[9];
    if(sumcrc!=rawData[10]){
        return 0;//У��ʧ�ܣ���������
    }
    p[0]=  ((unsigned short)rawData[3]<<8) | (unsigned short)rawData[2];
    p[1]=  ((unsigned short)rawData[5]<<8) | (unsigned short)rawData[4];
    p[2]=((unsigned short)rawData[7]<<8) | (unsigned short)rawData[6];
    p[3]=((unsigned short)rawData[9]<<8) | (unsigned short)rawData[8];
    return 1;//����ɹ�
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
