#include "complex_task_support.h"
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include "inc/hw_memmap.h"
#include "inc/hw_ints.h"
#include "driverlib/uart.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/timer.h"
#include "driverlib/interrupt.h"
#include "driverlib/pwm.h"

/**********************************************************
***	ZDT_X57�����ջ���������
***	�������̣�STM32_�������_whileѭ����������
***	��д���ߣ�ZHANGDATOU
***	����֧�֣��Ŵ�ͷ�ջ��ŷ�
***	�Ա����̣�https://zhangdatou.taobao.com
***	CSDN���ͣ�http s://blog.csdn.net/zhangdatou666
***	qq����Ⱥ��262438510
**********************************************************/

#define		ABS(x)		((x) > 0 ? (x) : -(x)) 
typedef struct stepper_PID
{
	int Traj_Position_P;
	int Bypass_Position_LV_P;
	int speed_P;
	int speed_I;
}stepper_PID;
typedef enum {
	S_VER   = 0,			/* ��ȡ�̼��汾�Ͷ�Ӧ��Ӳ���汾 */
	S_RL    = 1,			/* ��ȡ��ȡ���������� */
	S_PID   = 2,			/* ��ȡPID���� */
	S_VBUS  = 3,			/* ��ȡ���ߵ�ѹ */
	S_CBUS  = 4,			/* ��ȡ����ƽ������ */
	S_CPHA  = 5,			/* ��ȡ����� */
	S_ENC   = 6,			/* ��ȡ������ԭʼֵ */
	S_ENCL  = 7,			/* ��ȡ�������Ի�У׼��ı�����ֵ */
	S_TPOS  = 8,			/* ��ȡ���Ŀ��λ�� */
	S_VEL   = 9,			/* ��ȡ���ʵʱת�� */
	S_CPOS  = 10,			/* ��ȡ���ʵʱλ�� */
	S_PERR  = 11,			/* ��ȡ���λ����� */
	S_TEMP  = 12,			/* ��ȡ���ʵʱ�¶� */
	S_FLAG  = 13,			/* ��ȡ״̬��־λ */
	S_Conf  = 14,			/* ��ȡ�������� */
	S_State = 15,			/* ��ȡϵͳ״̬���� */
}SysParams_t;

void init_ZDT_X42_stepper(void);

void ZDT_X57_Reset_CurPos_To_Zero(uint8_t addr);
void ZDT_X57_Reset_Clog_Pro(uint8_t addr);
void ZDT_X57_Read_Sys_Params(uint8_t addr, SysParams_t s);
void ZDT_X57_En_Control(uint8_t addr, bool state, uint8_t multi);
void ZDT_X57_Torque_Control(uint8_t addr, uint8_t sign, uint16_t t_ramp, uint16_t torque, uint8_t multi);
void ZDT_X57_Velocity_Control(uint8_t addr, uint8_t dir, uint16_t v_ramp, float velocity, uint8_t multi);
void ZDT_X57_Bypass_Position_Control(uint8_t addr, uint8_t dir, float position, uint8_t raf, uint8_t multi);
void ZDT_X57_Bypass_Position_LV_Control(uint8_t addr, uint8_t dir, float velocity, float position, uint8_t raf, uint8_t multi);
void ZDT_X57_Traj_Position_Control(uint8_t addr, uint8_t dir, uint16_t acc, uint16_t dec, float velocity, float position, uint8_t raf, uint8_t multi);
void ZDT_X57_Stop_Now(uint8_t addr, uint8_t multi);
void ZDT_X57_Synchronous_motion(uint8_t addr);
int software_pwm_control(double speed,int acc,int count);
void speed_set(double up,double down);
