#include "stepper_motor_usart_controller.h"
void (*STEPPER_MOTOR_UART_Transmit)(wheel up_or_down,uint8_t *pData, uint16_t Size, uint32_t Timeout);
#define VERSION_4_2
extern int up_position,down_position;
int up_encoder,down_encoder;
double a,b;

extern uint8_t up_buffer_queue[15];
extern uint8_t down_buffer_queue[15];
#ifdef VERSION_4_2

void speed_set_uart(int speed,uint8_t acc,wheel up_or_down)
{

  if(0<=speed && speed<=1279){
    if(up_or_down){
      uint8_t control_stream[10]={0x01,0xF6,0x00|(uint8_t)(speed>>8),(uint8_t)(speed),acc,0x6B};
      if(STEPPER_MOTOR_UART_Transmit!=NULL)
          STEPPER_MOTOR_UART_Transmit( up_or_down,control_stream,6,50);
    }else{
      uint8_t control_stream[10]={0x01,0xF6,0x10|(uint8_t)(speed>>8),(uint8_t)(speed),acc,0x6B};
      if(STEPPER_MOTOR_UART_Transmit!=NULL)
          STEPPER_MOTOR_UART_Transmit( up_or_down,control_stream,6,50);
    }
  }
  if(-1279<=speed && speed<=-1){
    speed=-speed;
    if(up_or_down){
      uint8_t control_stream[10]={0x01,0xF6,0x10|(uint8_t)(speed>>8),(uint8_t)(speed),acc,0x6B};
      if(STEPPER_MOTOR_UART_Transmit!=NULL)
          STEPPER_MOTOR_UART_Transmit( up_or_down,control_stream,6,50);
    }else{
      uint8_t control_stream[10]={0x01,0xF6,0x00|(uint8_t)(speed>>8),(uint8_t)(speed),acc,0x6B};
      if(STEPPER_MOTOR_UART_Transmit!=NULL)
          STEPPER_MOTOR_UART_Transmit( up_or_down,control_stream,6,50);
    }
  }
}
void position_set_uart(int speed,uint8_t acc,uint32_t pulse_number,wheel up_or_down)
{
  if(0<=speed && speed<=1279){
    if(up_or_down){
      uint8_t control_stream[10]={0x01,0xFD,0x00|(uint8_t)(speed>>8),(uint8_t)(speed),acc,(uint8_t)(pulse_number>>16),(uint8_t)(pulse_number>>8),(uint8_t)pulse_number,0x6B};
      if(STEPPER_MOTOR_UART_Transmit!=NULL)
          STEPPER_MOTOR_UART_Transmit( up_or_down,control_stream,9,50);
    }else{
      uint8_t control_stream[10]={0x01,0xFD,0x10|(uint8_t)(speed>>8),(uint8_t)(speed),acc,(uint8_t)(pulse_number>>16),(uint8_t)(pulse_number>>8),(uint8_t)pulse_number,0x6B};
      if(STEPPER_MOTOR_UART_Transmit!=NULL)
          STEPPER_MOTOR_UART_Transmit( up_or_down,control_stream,9,50);
    }
  }
  if(-1279<=speed && speed<=-1){
    speed=-speed;
    if(up_or_down){
      uint8_t control_stream[10]={0x01,0xFD,0x10|(uint8_t)(speed>>8),(uint8_t)(speed),acc,(uint8_t)(pulse_number>>16),(uint8_t)(pulse_number>>8),(uint8_t)pulse_number,0x6B};
      if(STEPPER_MOTOR_UART_Transmit!=NULL)
          STEPPER_MOTOR_UART_Transmit( up_or_down,control_stream,9,50);
    }else{
      uint8_t control_stream[10]={0x01,0xFD,0x00|(uint8_t)(speed>>8),(uint8_t)(speed),acc,(uint8_t)(pulse_number>>16),(uint8_t)(pulse_number>>8),(uint8_t)pulse_number,0x6B};
      if(STEPPER_MOTOR_UART_Transmit!=NULL)
          STEPPER_MOTOR_UART_Transmit( up_or_down,control_stream,9,50);
    }
  }
  
}
void position_get(void)
{
      uint8_t control_stream[3]={0x01,0x36,0x6B};
      if(STEPPER_MOTOR_UART_Transmit!=NULL){
          STEPPER_MOTOR_UART_Transmit(up_wheel,control_stream,3,50);		
					STEPPER_MOTOR_UART_Transmit(down_wheel,control_stream,3,50);				
			}
}
void encoder_get(void)
{
      uint8_t control_stream[3]={0x01,0x30,0x6B};
      if(STEPPER_MOTOR_UART_Transmit!=NULL){
          STEPPER_MOTOR_UART_Transmit(up_wheel,control_stream,3,50);		
					STEPPER_MOTOR_UART_Transmit(down_wheel,control_stream,3,50);				
			}
}

void enable(int flag)
{
      if(flag){
        uint8_t control_stream[4]={0x01,0xF3,0x01,0x6B};
        if(STEPPER_MOTOR_UART_Transmit!=NULL){
            STEPPER_MOTOR_UART_Transmit(up_wheel,control_stream,4,50);		
            STEPPER_MOTOR_UART_Transmit(down_wheel,control_stream,4,50);				
        }      
      }else{
        uint8_t control_stream[4]={0x01,0xF3,0x00,0x6B};
        if(STEPPER_MOTOR_UART_Transmit!=NULL){
            STEPPER_MOTOR_UART_Transmit(up_wheel,control_stream,4,50);		
            STEPPER_MOTOR_UART_Transmit(down_wheel,control_stream,4,50);				
        }    
      }
}

void position_caculate_up(uint8_t up_buffer)
{
	static int up_p=0;

	up_buffer_queue[up_p]=up_buffer;
	up_p++;
	if(up_p>=6)
	{
		if(up_buffer_queue[up_p-1]==0x6B&&up_buffer_queue[up_p-6]==0x01)
		{
		up_position=(int32_t)(
													 ((int32_t)up_buffer_queue[up_p-5] << 24) |
													 ((int32_t)up_buffer_queue[up_p-4] << 16) |
													 ((int32_t)up_buffer_queue[up_p-3] << 8)  |
													 ((int32_t)up_buffer_queue[up_p-2] << 0)
													 );
		up_p=0;			
		}
	}
}

void position_caculate_down(uint8_t down_buffer)
{

	static int down_p=0;

	down_buffer_queue[down_p]=down_buffer;
	down_p++;
	
	if(down_p>=6)
	{
		if(down_buffer_queue[down_p-1]==0x6B&&down_buffer_queue[down_p-6]==0x01)
		{
		down_position=(int32_t)(
													 ((int32_t)down_buffer_queue[down_p-5] << 24) |
													 ((int32_t)down_buffer_queue[down_p-4] << 16) |
													 ((int32_t)down_buffer_queue[down_p-3] << 8)  |
													 ((int32_t)down_buffer_queue[down_p-2] << 0)
													 );
		b=-down_position;
		down_p=0;			
		}
	}
}
void encoder_caculate_up(uint8_t up_buffer)
{
	static int up_p=0;

	up_buffer_queue[up_p]=up_buffer;
	up_p++;
	if(up_p>=4)
	{
		if(up_buffer_queue[up_p-1]==0x6B&&up_buffer_queue[up_p-4]==0x01)
		{
		up_encoder=(int32_t)(
													 ((int32_t)up_buffer_queue[up_p-3] << 8)  |
													 ((int32_t)up_buffer_queue[up_p-2] << 0)
													 );
		up_p=0;			
		}
	}
}

void encoder_caculate_down(uint8_t down_buffer)
{

	static int down_p=0;

	down_buffer_queue[down_p]=down_buffer;
	down_p++;
	
	if(down_p>=4)
	{
		if(down_buffer_queue[down_p-1]==0x6B&&down_buffer_queue[down_p-4]==0x01)
		{
		down_encoder=(int32_t)(
													 ((int32_t)down_buffer_queue[down_p-3] << 8)  |
													 ((int32_t)down_buffer_queue[down_p-2] << 0)
													 );
		b=-down_encoder;
		down_p=0;			
		}
	}
}
#endif

#ifdef VERSION_3_6

void speed_set(int speed,uint8_t acc,wheel up_or_down)
{

  if(correction_.up_down)
  {
      if( up_or_down==up_wheel){
          up_or_down=down_wheel;
      }else{
          up_or_down=up_wheel;
      }
  }
  if(correction_.up_d && up_or_down==up_wheel){
      speed=-speed;
  }
  if(correction_.down_d && up_or_down==down_wheel){
      speed=-speed;
  }

  if(1<=speed && speed<=127){
    if(up_or_down){
      uint8_t control_stream[3]={0xe0,0xf6,(uint8_t)(speed)};
      if(STEPPER_MOTOR_UART_Transmit!=NULL)
      STEPPER_MOTOR_UART_Transmit( up_or_down,control_stream,3,50);
    }else{
      uint8_t control_stream[3]={0xe0,0xf6,(uint8_t)(speed)};
      if(STEPPER_MOTOR_UART_Transmit!=NULL)
      STEPPER_MOTOR_UART_Transmit( up_or_down,control_stream,3,50);
    }
  }
  if(-127<=speed && speed<=-1){
    speed=-speed;
    if(up_or_down){
      uint8_t control_stream[3]={0xe0,0xf6,(uint8_t)(speed)| 0x80};
      if(STEPPER_MOTOR_UART_Transmit!=NULL)
      STEPPER_MOTOR_UART_Transmit(up_or_down,control_stream,3,50);
    }else{
      uint8_t control_stream[3]={0xe0,0xf6,(uint8_t)(speed)| 0x80};
      if(STEPPER_MOTOR_UART_Transmit!=NULL)
      STEPPER_MOTOR_UART_Transmit(up_or_down,control_stream,3,50);
    }
  }
  if(speed==0){
      uint8_t control_stream[2]={0xe0,0xf7};
      if(STEPPER_MOTOR_UART_Transmit!=NULL)
      STEPPER_MOTOR_UART_Transmit(up_or_down,control_stream,2,50);
  }
}
#endif
