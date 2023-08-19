/*
 * data_structure.c
 *
 *  Created on: 2023��6��13��
 *      Author: yyg
 */
//˫������
#include "data_structure.h"
//�����ڴ��
#define S_SIZE 8
#define S_NUM 2000
#define M_SIZE S_SIZE*S_NUM
//cortex_M4��ѭ�ߵ�ַ�����ֽڣ�������������������������������
char memory_pool[M_SIZE];
char memory_sheet[S_NUM]={0};
char * getMemory(int size){
//�����ڴ�����ȷ������
    int k=size/S_SIZE;
    if(size%S_SIZE>0){
       k++;
    }
//��������Ѱ�ҿ��ڴ��
    int i,j,flag;
    for(i=0,flag=0;i<S_NUM-k+1 && flag==0;i++){
        if(memory_sheet[i]==0 && memory_sheet[i+k-1]==0){
            for(j=0;j<=k-1 && memory_sheet[i+j]==0;j++){
            }
            if(j==k){
                flag=1;
            }
        }
    }
//��¼�ڴ���䲢���ص�ַ
    if(flag){
        i--;
        memory_sheet[i]=1;
        for(j=1;j<=k-1;j++){
             memory_sheet[i+j]=2;
        }
        return &memory_pool[i*S_SIZE];
    }else{
        return NULL;
    }
}
int freeMemory(char *p){
    int k=(p-memory_pool)/S_SIZE;

    int j;
    if(memory_sheet[k]==1){
       memory_sheet[k]=0;
       for(j=0;j<S_SIZE;j++){
           memory_pool[k*S_SIZE+j]=0;
       }
       memory_pool[k*S_SIZE]=0;
       int i;
       for(i=1;memory_sheet[k+i]==2 && (k+i)<S_NUM;i++){
           memory_sheet[k+i]=0;
           for(j=0;j<S_SIZE;j++){
               memory_pool[(k+i)*S_SIZE+j]=0;
           }
       }
       return 1;
    }
    else{
        return 0;
    }
}
void printMemory(int size){
    UARTprintf("\n memory_pool$memory_sheet: ");
    int i;
    for(i=0;i<size*S_SIZE;i++){
        if(i%S_SIZE==0){
            UARTprintf("\n Segment %d[%d]:",i/S_SIZE,memory_sheet[i/S_SIZE]);
        }
        UARTprintf("%d ", memory_pool[i] );

    }
}

void insert_pre(linked_list* p,data_type data){
    linked_list * mid=(linked_list*)getMemory(sizeof(linked_list));
    mid->data=data;
    if(p->pre!=NULL){
       p->pre->next=mid;
       mid->pre=p->pre;
    }
    mid->next=p;
    p->pre=mid;
}
void insert_next(linked_list* p,data_type data){
    if(p->next!=NULL){
        p->next->pre=(linked_list*)getMemory(sizeof(linked_list));
        p->next->pre->next=p->next;
        p->next->pre->data=data;
        p->next->pre->pre=p;
        p->next=p->next->pre;
    }
    else{
        p->next=(linked_list*)getMemory(sizeof(linked_list));
        p->next->data=data;
        p->next->pre=p;
        p->next->next=NULL;
    }
}
