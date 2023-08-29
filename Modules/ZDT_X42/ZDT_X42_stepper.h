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
***	ZDT_X57步进闭环控制例程
***	控制例程：STM32_脉冲控制_while循环发送脉冲
***	编写作者：ZHANGDATOU
***	技术支持：张大头闭环伺服
***	淘宝店铺：https://zhangdatou.taobao.com
***	CSDN博客：http s://blog.csdn.net/zhangdatou666
***	qq交流群：262438510
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
	S_VER   = 0,			/* 读取固件版本和对应的硬件版本 */
	S_RL    = 1,			/* 读取读取相电阻和相电感 */
	S_PID   = 2,			/* 读取PID参数 */
	S_VBUS  = 3,			/* 读取总线电压 */
	S_CBUS  = 4,			/* 读取总线平均电流 */
	S_CPHA  = 5,			/* 读取相电流 */
	S_ENC   = 6,			/* 读取编码器原始值 */
	S_ENCL  = 7,			/* 读取经过线性化校准后的编码器值 */
	S_TPOS  = 8,			/* 读取电机目标位置 */
	S_VEL   = 9,			/* 读取电机实时转速 */
	S_CPOS  = 10,			/* 读取电机实时位置 */
	S_PERR  = 11,			/* 读取电机位置误差 */
	S_TEMP  = 12,			/* 读取电机实时温度 */
	S_FLAG  = 13,			/* 读取状态标志位 */
	S_Conf  = 14,			/* 读取驱动参数 */
	S_State = 15,			/* 读取系统状态参数 */
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
