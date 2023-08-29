/*
 * yyg_jy901b.h
 *
 *  Created on: 2023��3��26��
 *      Author: yyg
 */
/*
 ����WIT˽��Э��
 Э��μ� https://wit-motion.yuque.com/wumwnr/ltst03/vl3tpy?#%20%E3%80%8AWIT%E7%A7%81%E6%9C%89%E5%8D%8F%E8%AE%AE%E3%80%8B
 jy901bĬ��Ϊ����ģʽ���Է����ϵ����ⷢ������
 Ŀǰ���谭��
 ������λ���޷��������ΪIICģʽ��ͨ����Ƭ��д������ʱ���ڹ��ϣ����ۿ��У�ʵ�ʲ��У�ԭ��������������ʱ�޷�ʵ��IICͨ��
 ͨ����λ�����ã�ŷ���ǵ�����ЧƵ�����100hz����ʱ������230400
 �������´����жϽ��գ�����ռ��Լ1.2%
*/

#ifndef JY901B_YYG_JY901B_H_
#define JY901B_YYG_JY901B_H_

// ����֡
unsigned char rawData[11]; //ԭʼ���ݶ���
 short data[4];    //���������ݶ���, ����0.0054931640625�õ���Ϊ��λ����ֵ,����0.0000958737976�õ�����Ϊ��λ����ֵ
// ��ȡ��ʽ
#define READ_HEAD  0x55 //Э��ͷ
#define TIME       0x50 //ʱ��
#define ACC        0x51 //���ٶ�
#define GYRO       0x52 //���ٶ�
#define ANGLE      0x53 //�Ƕ�
#define MAG        0x54 //�ų�
#define PORT       0x55 //�˿�״̬
#define PRESS      0x56 //��ѹ�߶�
#define GPS        0x57 //��γ��
#define VELOCITY   0x58 //����
#define QUATER     0x59 //��Ԫ��
#define GSA        0x5A //GPS��λ����
#define READ_REG   0x5F //��ȡ�Ĵ�������ֵ

int pushData(unsigned char data_u8);
int changeData(short *p);

//д��ʽ
#define WRITE_HEAD_1 0xFF
#define WRITE_HEAD_2 0xAA

#define UNLOCK_1     0x69
#define UNLOCK_2     0x88
#define UNLOCK_3     0xB5

#define SAVE_1       0x00
#define SAVE_2       0x00
#define SAVE_3       0x00
//�Ĵ�����ַ
#define SAVE         0x00
#define CALSW        0x01 //У׼�Ĵ���
/*
FF AA 01 04 00 ���������
FF AA 01 07 00 �ų�У׼
FF AA 01 01 00 �Զ��Ӽ�У׼
*/
#define RSW          0x02 //������ݼĴ���
/* 16λ�Ĵ�����1-������Ӧ�����0-�ر���Ӧ���
 * 15~11 10  9      8        7   6     5    4   3     2    1   0
 *   \   GSA QUATER VELOCITY GPS PRESS PORT MAG ANGLE GYRO ACC TIME
*/
/*
FF AA 02 04 00 ֻ����Ƕ�
*/
#define RRATE        0x03 //������ʼĴ���
#define BAUD         0x04 //���ڲ�����
/**
 * 01 00 4800bps
 * 02 00 9600bps
 * 03 00 19200bps
 * 04 00 38400bps
 * 05 00 57600bps
 * 06 00 115200bps
 * 07 00 230400bps
 */
//��ƫ����ϵ��

#define IICADDR      0x1A //I2C�豸��ַ
#define AXIS6        0x24 //�㷨ѡ��
/*
 FF AA 24 00 00 9���㷨
 FF AA 24 01 00 6���㷨
 */
#define FILTK        0x25 //Kֵ�˲���

//��Ԫ��ϵ��
#define Q0           0x51
#define Q1           0x52
#define Q2           0x53
#define Q3           0x54

#define DELAYT       0x59 //�����ź���ʱ
//�Ƕȱ�����ֵϵ��
//д���ݺ���
int write_gy901b_data(unsigned char addr,unsigned char data_L,unsigned char data_H);
int write_gy901b_start(void);
int write_gy901b_end(void);

#endif /* JY901B_YYG_JY901B_H_ */
