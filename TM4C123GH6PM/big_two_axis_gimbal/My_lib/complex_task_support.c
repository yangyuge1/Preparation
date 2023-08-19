#include "complex_task_support.h"
//�ڲ�����
void (*task)(void);//�ص�����ָ��
void SysTick_task_IntHandler(void);

//��ѭ���߳���ռʽ�ӳٺ���,����ֵ���1000��1S������С1��1ms��
void delay_ms(int time){
 SysCtlDelay(SysCtlClockGet()/3000*(time));
}
//��ѭ���߳���ռʽ�ӳٺ���
void delay_s(int time){
 while(time--)
 {SysCtlDelay(SysCtlClockGet()/3);}
}
//��ѭ���߳���ռʽ�ӳٺ���,����ֵ���1000��1ms������С1��1us��
void delay_us(int time){
double us=SysCtlClockGet();
SysCtlDelay((int)(us/3000000*(time)));
}
//����ϵͳ�δ���������첽�ص�����
void setTimeOut_ms(int time,void (*var)(void)){
    task=var;
    SysTickPeriodSet(SysCtlClockGet()/1000*time);
    SysTickIntRegister(SysTick_task_IntHandler);
    SysTickIntEnable();
    SysTickEnable();//������ʱ��
}
void SysTick_task_IntHandler(void){
    SysTickDisable(); //�رն�ʱ��
    if(task)          //��������Ƿ����
    task();           //ִ������
//    task=0;         //��ջص�����
}
