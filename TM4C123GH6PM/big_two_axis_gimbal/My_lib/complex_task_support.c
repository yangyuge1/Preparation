#include "complex_task_support.h"
//内部声明
void (*task)(void);//回调函数指针
void SysTick_task_IntHandler(void);

//主循环线程抢占式延迟函数,输入值最大1000（1S），最小1（1ms）
void delay_ms(int time){
 SysCtlDelay(SysCtlClockGet()/3000*(time));
}
//主循环线程抢占式延迟函数
void delay_s(int time){
 while(time--)
 {SysCtlDelay(SysCtlClockGet()/3);}
}
//主循环线程抢占式延迟函数,输入值最大1000（1ms），最小1（1us）
void delay_us(int time){
double us=SysCtlClockGet();
SysCtlDelay((int)(us/3000000*(time)));
}
//基于系统滴答计数器的异步回调函数
void setTimeOut_ms(int time,void (*var)(void)){
    task=var;
    SysTickPeriodSet(SysCtlClockGet()/1000*time);
    SysTickIntRegister(SysTick_task_IntHandler);
    SysTickIntEnable();
    SysTickEnable();//开启定时器
}
void SysTick_task_IntHandler(void){
    SysTickDisable(); //关闭定时器
    if(task)          //检查任务是否存在
    task();           //执行任务
//    task=0;         //清空回调函数
}
