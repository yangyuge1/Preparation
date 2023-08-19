#include "two_axis_gimbal.h"
#include <math.h>

double up,down;
extern double data_du[3];
extern short data[4];

short calibration_matrix[22][11]={0};

int init_two_axis_gimbal(int fre)
{
	
	int time=16000000/8/fre;
	//??PWM??(??USEPWMDIV???)
	SysCtlPWMClockSet(SYSCTL_PWMDIV_8);																									//PWM?? 16M

	//????
	SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM1);			//??PWM??1??																		
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);		//??GPIOF??																		
	
	//??????PWM??
	GPIOPinTypePWM(GPIO_PORTF_BASE,GPIO_PIN_2);
	GPIOPinTypePWM(GPIO_PORTF_BASE,GPIO_PIN_3);
	
	//PWM????
	GPIOPinConfigure(GPIO_PF2_M1PWM6);					//PF2->PWM??1??6																							
	GPIOPinConfigure(GPIO_PF3_M1PWM7);					//PF3->PWM??1??7																								

	//??PWM???
	//??1->???3->????,???
	PWMGenConfigure(PWM1_BASE,PWM_GEN_3,PWM_GEN_MODE_DOWN|PWM_GEN_MODE_NO_SYNC);	
	
	//??PWM??
	PWMGenPeriodSet(PWM1_BASE,PWM_GEN_3,time);			//64*10^3/16/10^6=4ms																			
	
	//??PWM???
	PWMPulseWidthSet(PWM1_BASE,PWM_OUT_6,PWMGenPeriodGet(PWM1_BASE, PWM_GEN_3)*0.025);		//????????????,25%
	PWMPulseWidthSet(PWM1_BASE,PWM_OUT_7,PWMGenPeriodGet(PWM1_BASE, PWM_GEN_3)*0.025);		//????????????,75%

	//??PWM??1??
	PWMOutputState(PWM1_BASE,PWM_OUT_6_BIT,true);
	PWMOutputState(PWM1_BASE,PWM_OUT_7_BIT,true);
	
	//??PWM???
	PWMGenEnable(PWM1_BASE,PWM_GEN_3);
	
	return time;
		
}
void set_two_axis_gimbal(double up,double down)
{
	if(up<0){
		up=0;
	}
	if(up>1){
		up=1;
	}
	if(down<0){
		down=0;
	}
	if(down>1){
		down=1;
	}
	
	PWMPulseWidthSet(PWM1_BASE,PWM_OUT_6,PWMGenPeriodGet(PWM1_BASE, PWM_GEN_3)*(0.025+up*0.7*0.1));		//????????????,25%
	PWMPulseWidthSet(PWM1_BASE,PWM_OUT_7,PWMGenPeriodGet(PWM1_BASE, PWM_GEN_3)*(0.025+down*0.1));		//????????????,75%
}


void caculate_two_axis_gimbal(double x, double y, double z)
{
#define PI 3.1415926
	double yaw,pitch;
	static double last_dowm=0;

	if (x >= 0 && y >= 0)
	{
		yaw = asin(y / sqrt(x * x + y * y));
		pitch= PI-asin(z / sqrt(x * x + y * y + z* z));
	}
	if (x >= 0 && y <= 0)
	{
		yaw = PI - asin(fabs(y) / sqrt(x * x + y * y));
		pitch = asin(z / sqrt(x * x + y * y + z * z));
	}
	if (x <= 0 && y >= 0)
	{
		yaw = PI - asin(fabs(y) / sqrt(x * x + y * y));
		pitch = PI - asin(z / sqrt(x * x + y * y + z * z));
	}
	if (x <= 0 && y <= 0)
	{
		yaw = asin(fabs(y) / sqrt(x * x + y * y));
		pitch = asin(z / sqrt(x * x + y * y + z * z));
	}
	if(x==0&&y==0)
	{
		up=0.5* 0.7;
		down=last_dowm;
		return;
	}
	last_dowm=down;
	up = pitch / PI * 0.7;
	down = yaw / PI ;
	return;
}

void calibration_two_axis_gimbal(void)
{
	int i=0,j;
	set_two_axis_gimbal(0,0);
	delay_ms(2000);
	for(j=0;j<=10;j++)
	{
		for(i=0;i<=10;i++)
		{
			set_two_axis_gimbal(i*0.1,j*0.1);
			UARTprintf("%d,%d,%d,%d,%d\n",i,j,(int)(data_du[2]),(int)(data_du[1]),(int)(data_du[0]));
			delay_ms(1000);
		}		
	}

	
	
}