/*
 * command.c
 *
 *  Created on: 2023��5��29��
 *      Author: yyg
 */
#include "command.h"

//8*2
void set_short(short * p,short value){
    *p=value;
}
//8*4
void set_int(int * p,int value){
    *p=value;
}
//8*4
void set_long(long * p,long value){
    *p=value;
}
//8*4 �����ܾ�ȷ��ʾС�����6λ��Ч����
void set_float(float * p,float value){
    *p=value;
}
//8*8 �����ܾ�ȷ��ʾС�����12λ��Ч����
void set_double(double * p,double value){
    *p=value;
}
//8*1
void set_char(char * p,char value){
    *p=value;
}
//����ʶ��
//���ַ���ת��Ϊ��Ӧ����������
//ʹ��pointer�����ֽڸ�����
short transform2short(char input_1,char input_0 ){

    union result
    {
        short d;
        unsigned char data[2];
    }transform;
    transform.data[0]=input_0;
    transform.data[1]=input_1;
    return transform.d;
}
short transform2short_pointer(char *p){
    return transform2short(p[1],p[0]);
}

int transform2int(char input_3,char input_2,char input_1,char input_0 ){
    union result
    {
        int d;
        unsigned char data[4];
    }transform;
    transform.data[0]=input_0;
    transform.data[1]=input_1;
    transform.data[2]=input_2;
    transform.data[3]=input_3;
    return transform.d;
}
int transform2int_pointer(char *p){
    return transform2int(p[3],p[2],p[1],p[0]);
}

float transform2float(char input_3,char input_2,char input_1,char input_0 ){
    union result
    {
        float d;
        unsigned char data[4];
    }transform;
    transform.data[0]=input_0;
    transform.data[1]=input_1;
    transform.data[2]=input_2;
    transform.data[3]=input_3;
    return transform.d;
}
float transform2float_pointer(char *p){
    return transform2float(p[3],p[2],p[1],p[0]);
}

double transform2double(char input_7,char input_6,char input_5,char input_4,char input_3,char input_2,char input_1,char input_0 ){
    union result
    {
        double d;
        unsigned char data[8];
    }transform;
    transform.data[0]=input_0;
    transform.data[1]=input_1;
    transform.data[2]=input_2;
    transform.data[3]=input_3;
    transform.data[4]=input_4;
    transform.data[5]=input_5;
    transform.data[6]=input_6;
    transform.data[7]=input_7;
    return transform.d;
}
double transform2double_pointer(char *p){
    return transform2double(p[7],p[6],p[5],p[4],p[3],p[2],p[1],p[0]);
}
//�ַ�ʶ��
//�ո� 0x20
//. 0x2E
//1 0x31

#define length_1 12
//֡����Ӧ��С�ڵ���length-1,�����Ե�����ĸ��β
//�䳤�������ݽ������ַ���תint
int charStream2int(int *save_target,char input){

    static char c[length_1];
    static int x=0,y=0;

    int i=0;
    c[x]=input;
    x++;
    if(x==length_1){
        x=0;
    }

    if( (0x30<=input && input<=0x39) || input==0x2D){
        return 0;
    }else{
        *save_target=0;
        y=move(x,-2,length_1);
        for(;y!=move(x,-1,length_1);y=move(y,-1,length_1)){
            if(0x30<=c[y] && c[y]<=0x39){
                *save_target+=(c[y]-0x30)*pow(10,i);
                i++;
            }
            else if(c[y]==0x2d){
                *save_target=-(*save_target);
                c[y]='n';
                x=move(y,1,length_1);
                return 1;
            }else{
                x=move(y,1,length_1);
                return 1;
            }
        }
        return 1;
    }
}
//ѭ������ָ����λ����
int move(int position,int step,int length){
    position+=step;
    position%=length;
    if(position<0){
        position+=length;
    }
    return position;
}

#define length_2 16
int charStream2float(float *save_target,char input){
    //
}





