#include "stepper_motor_usart_controller.h"
void (*STEPPER_MOTOR_UART_Transmit)(wheel left_or_right,uint8_t *pData, uint16_t Size, uint32_t Timeout);
#define VERSION_4_2
correction correction_ ={0,0,0};


int left_position,right_position;
double a,b;

uint8_t right_buffer_queue[15];
uint8_t left_buffer_queue[15];
#ifdef VERSION_4_2

void speed_set_uart(int speed,uint8_t acc,wheel left_or_right)
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
void position_get(void)
{
      uint8_t control_stream[3]={0x01,0x36,0x6B};
      if(STEPPER_MOTOR_UART_Transmit!=NULL){
          STEPPER_MOTOR_UART_Transmit(left_wheel,control_stream,3,50);		
					STEPPER_MOTOR_UART_Transmit(right_wheel,control_stream,3,50);				
			}
}
void position_caculate_left(uint8_t left_buffer)
{
	static int left_p=0;

	left_buffer_queue[left_p]=left_buffer;
	left_p++;
	if(left_p>=6)
	{
		if(left_buffer_queue[left_p-1]==0x6B&&left_buffer_queue[left_p-6]==0x01)
		{
		left_position=(int32_t)(
													 ((int32_t)left_buffer_queue[left_p-5] << 24) |
													 ((int32_t)left_buffer_queue[left_p-4] << 16) |
													 ((int32_t)left_buffer_queue[left_p-3] << 8)  |
													 ((int32_t)left_buffer_queue[left_p-2] << 0)
													 );
		a=left_position;
		left_p=0;			
		}
	}
}

void position_caculate_right(uint8_t right_buffer)
{

	static int right_p=0;

	right_buffer_queue[right_p]=right_buffer;
	right_p++;
	
	if(right_p>=6)
	{
		if(right_buffer_queue[right_p-1]==0x6B&&right_buffer_queue[right_p-6]==0x01)
		{
		right_position=(int32_t)(
													 ((int32_t)right_buffer_queue[right_p-5] << 24) |
													 ((int32_t)right_buffer_queue[right_p-4] << 16) |
													 ((int32_t)right_buffer_queue[right_p-3] << 8)  |
													 ((int32_t)right_buffer_queue[right_p-2] << 0)
													 );
		b=-right_position;
		right_p=0;			
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
