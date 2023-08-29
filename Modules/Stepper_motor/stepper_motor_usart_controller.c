#include "stepper_motor_usart_controller.h"

#define VERSION_3_6

typedef struct correction{
    int left_right;
    int left_d;
    int right_d;
}correction;
static correction correction_ ={1,0,1};

#ifdef VERSION_4_2
void speed_set(int speed,uint8_t acc,int left_or_right)
{

    if(correction_.left_right)
    {
        if( left_or_right==left_wheel){
            left_or_right=right_wheel;
        }else{
            left_or_right=left_wheel;
        }
    }
    if(correction_.left_d && left_or_right==left_wheel){
        speed=-speed;
    }
    if(correction_.right_d && left_or_right==right_wheel){
        speed=-speed;
    }

  if(0<=speed && speed<=1279){
    if(left_or_right){
      uint8_t control_stream[10]={0x01,0xF6,0x00|(uint8_t)(speed>>8),(uint8_t)(speed),acc,0x6B};
      if(STEPPER_MOTOR_UART_Transmit!=NULL)
          STEPPER_MOTOR_UART_Transmit( left_or_right,control_stream,6,50);
    }else{
      uint8_t control_stream[10]={0x01,0xF6,0x10|(uint8_t)(speed>>8),(uint8_t)(speed),acc,0x6B};
      if(STEPPER_MOTOR_UART_Transmit!=NULL)
          STEPPER_MOTOR_UART_Transmit( left_or_right,control_stream,6,50);
    }
  }
  if(-1279<=speed && speed<=-1){
    speed=-speed;
    if(left_or_right){
      uint8_t control_stream[10]={0x01,0xF6,0x10|(uint8_t)(speed>>8),(uint8_t)(speed),acc,0x6B};
      if(STEPPER_MOTOR_UART_Transmit!=NULL)
          STEPPER_MOTOR_UART_Transmit( left_or_right,control_stream,6,50);
    }else{
      uint8_t control_stream[10]={0x01,0xF6,0x00|(uint8_t)(speed>>8),(uint8_t)(speed),acc,0x6B};
      if(STEPPER_MOTOR_UART_Transmit!=NULL)
          STEPPER_MOTOR_UART_Transmit( left_or_right,control_stream,6,50);
    }
  }
}
#endif

#ifdef VERSION_3_6

void speed_set(int speed,uint8_t acc,wheel left_or_right)
{

  if(correction_.left_right)
  {
      if( left_or_right==left_wheel){
          left_or_right=right_wheel;
      }else{
          left_or_right=left_wheel;
      }
  }
  if(correction_.left_d && left_or_right==left_wheel){
      speed=-speed;
  }
  if(correction_.right_d && left_or_right==right_wheel){
      speed=-speed;
  }

  if(1<=speed && speed<=127){
    if(left_or_right){
      uint8_t control_stream[3]={0xe0,0xf6,(uint8_t)(speed)};
      if(STEPPER_MOTOR_UART_Transmit!=NULL)
      STEPPER_MOTOR_UART_Transmit( left_or_right,control_stream,3,50);
    }else{
      uint8_t control_stream[3]={0xe0,0xf6,(uint8_t)(speed)};
      if(STEPPER_MOTOR_UART_Transmit!=NULL)
      STEPPER_MOTOR_UART_Transmit( left_or_right,control_stream,3,50);
    }
  }
  if(-127<=speed && speed<=-1){
    speed=-speed;
    if(left_or_right){
      uint8_t control_stream[3]={0xe0,0xf6,(uint8_t)(speed)| 0x80};
      if(STEPPER_MOTOR_UART_Transmit!=NULL)
      STEPPER_MOTOR_UART_Transmit(left_or_right,control_stream,3,50);
    }else{
      uint8_t control_stream[3]={0xe0,0xf6,(uint8_t)(speed)| 0x80};
      if(STEPPER_MOTOR_UART_Transmit!=NULL)
      STEPPER_MOTOR_UART_Transmit(left_or_right,control_stream,3,50);
    }
  }
  if(speed==0){
      uint8_t control_stream[2]={0xe0,0xf7};
      if(STEPPER_MOTOR_UART_Transmit!=NULL)
      STEPPER_MOTOR_UART_Transmit(left_or_right,control_stream,2,50);
  }
}
#endif
