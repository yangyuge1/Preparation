/*
 * yyg_jy901b.h
 *
 *  Created on: 2023年3月26日
 *      Author: yyg
 */
/*
 基于WIT私有协议
 协议参见 https://wit-motion.yuque.com/wumwnr/ltst03/vl3tpy?#%20%E3%80%8AWIT%E7%A7%81%E6%9C%89%E5%8D%8F%E8%AE%AE%E3%80%8B
 jy901b默认为串口模式，自发不断的向外发送数据
 目前的阻碍：
 串口上位机无法将其调整为IIC模式，通过单片机写配置暂时存在故障（理论可行，实际不行，原因不明），所以暂时无法实现IIC通信
 通过上位机配置，欧拉角单传有效频率最大100hz，此时波特率230400
 该条件下串口中断接收，性能占用约1.2%
*/

#ifndef JY901B_YYG_JY901B_H_
#define JY901B_YYG_JY901B_H_

// 数据帧
unsigned char rawData[11]; //原始数据队列
 short data[4];    //处理后的数据队列, 乘以0.0054931640625得到度为单位的数值,乘以0.0000958737976得到弧度为单位的数值
// 读取格式
#define READ_HEAD  0x55 //协议头
#define TIME       0x50 //时间
#define ACC        0x51 //加速度
#define GYRO       0x52 //角速度
#define ANGLE      0x53 //角度
#define MAG        0x54 //磁场
#define PORT       0x55 //端口状态
#define PRESS      0x56 //气压高度
#define GPS        0x57 //经纬度
#define VELOCITY   0x58 //地速
#define QUATER     0x59 //四元数
#define GSA        0x5A //GPS定位精度
#define READ_REG   0x5F //读取寄存器返回值

int pushData(unsigned char data_u8);
int changeData(short *p);

//写格式
#define WRITE_HEAD_1 0xFF
#define WRITE_HEAD_2 0xAA

#define UNLOCK_1     0x69
#define UNLOCK_2     0x88
#define UNLOCK_3     0xB5

#define SAVE_1       0x00
#define SAVE_2       0x00
#define SAVE_3       0x00
//寄存器地址
#define SAVE         0x00
#define CALSW        0x01 //校准寄存器
/*
FF AA 01 04 00 航向角置零
FF AA 01 07 00 磁场校准
FF AA 01 01 00 自动加计校准
*/
#define RSW          0x02 //输出内容寄存器
/* 16位寄存器，1-开启相应输出，0-关闭相应输出
 * 15~11 10  9      8        7   6     5    4   3     2    1   0
 *   \   GSA QUATER VELOCITY GPS PRESS PORT MAG ANGLE GYRO ACC TIME
*/
/*
FF AA 02 04 00 只输出角度
*/
#define RRATE        0x03 //输出速率寄存器
#define BAUD         0x04 //串口波特率
/**
 * 01 00 4800bps
 * 02 00 9600bps
 * 03 00 19200bps
 * 04 00 38400bps
 * 05 00 57600bps
 * 06 00 115200bps
 * 07 00 230400bps
 */
//零偏设置系列

#define IICADDR      0x1A //I2C设备地址
#define AXIS6        0x24 //算法选择
/*
 FF AA 24 00 00 9轴算法
 FF AA 24 01 00 6轴算法
 */
#define FILTK        0x25 //K值滤波器

//四元数系列
#define Q0           0x51
#define Q1           0x52
#define Q2           0x53
#define Q3           0x54

#define DELAYT       0x59 //报警信号延时
//角度报警阈值系列
//写数据函数
int write_gy901b_data(unsigned char addr,unsigned char data_L,unsigned char data_H);
int write_gy901b_start(void);
int write_gy901b_end(void);

#endif /* JY901B_YYG_JY901B_H_ */
