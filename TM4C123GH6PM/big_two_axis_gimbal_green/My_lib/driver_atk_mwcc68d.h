/*
 * driver_atk_mwcc68d.h
 *
 *  Created on: 2023年7月2日
 *      Author: yyg
 */

#ifndef ATK_MWCC68D_DRIVER_ATK_MWCC68D_H_
#define ATK_MWCC68D_DRIVER_ATK_MWCC68D_H_

#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "utils/uartstdio.h"
/* AT响应等待超时时间（毫秒） */
#define ATK_MWCC68D_AT_TIMEOUT  500
/* UART收发缓冲大小 */
#define ATK_MWCC68D_UART_RX_BUF_SIZE            128
#define ATK_MWCC68D_UART_TX_BUF_SIZE            128

/**
 * 需要做好外设初始化
 * 包括:
 * 1、一个USART与ATK-MWCC68D通信
 *      波特率115200
 *      数据位8
 *      停止位1
 *      校验位0
 *      收发模式
 *      无硬件流控
 *      中断接收
 * 2、一个IO口用于读取电平
 *      Input Floating Mode
 *      Pull-up activation
 *      High speed
 * 3、一个IO口用于输出电平
 *      Output Push Pull Mode
 *      Pull-down activation
 *      High speed
 * 下列接口是使用时需要实现的接口
 */
/**
 * @brief       ATK-MWCC68D AUX引脚电平读取函数
 * @param       无
 * @retval      0  :低电平
 *              1  :高电平
 */
int (*ATK_MWCC68D_AUX)(void);
/**
 * @brief       ATK-MWCC68D MD0引脚电平控制函数
 * @param       flag:0  :低电平
 *                   1  :高电平
 * @retval      无
 */
void (*ATK_MWCC68D_MD0)(int flag);
/**
 * @brief       ATK-MWCC68D UART字符流发送函数
 * @param       pData:字符串地址
 *              Size:字符串长度，不包括空结束字符
 *              Timeout:最大发送时间，发送数据超过该时间退出发送
 * @retval      无
 */
void (*ATK_MWCC68D_UART_Transmit)(uint8_t *pData, uint16_t Size, uint32_t Timeout);
/**
 * @brief       标准延时函数
 * @param       time:延时时间，单位ms
 * @retval      无
 */
void (*ATK_MWCC68D_delay_ms)(int time);

static struct
{
    uint8_t buf[ATK_MWCC68D_UART_RX_BUF_SIZE];              /* 帧接收缓冲 */
    struct
    {
        int len  ;                               /* 帧接收长度，sta[14:0] */
        int finsh;                                /* 帧接收完成标志，sta[15] */
    } sta;                                                  /* 帧状态信息 */
} g_uart_rx_frame = {0};                                    /* ATK-MWCC68D UART接收帧缓冲信息结构体 */
static uint8_t g_uart_tx_buf[ATK_MWCC68D_UART_TX_BUF_SIZE]; /* ATK-MWCC68D UART发送缓冲 */

/* 错误代码 */
#define ATK_MWCC68D_EOK             0       /* 没有错误 */
#define ATK_MWCC68D_ERROR           1       /* 通用错误 */
#define ATK_MWCC68D_ETIMEOUT        2       /* 超时错误 */
#define ATK_MWCC68D_EINVAL          3       /* 参数错误 */
#define ATK_MWCC68D_EBUSY           4       /* 忙错误 */
/* 使能枚举 */
typedef enum
{
    ATK_MWCC68D_DISABLE             = 0x00,
    ATK_MWCC68D_ENABLE,
} atk_mwcc68d_enable_t;

/* 发射功率枚举 */
typedef enum
{
    ATK_MWCC68D_TPOWER_9DBM         = 0,   /* 9dBm */
    ATK_MWCC68D_TPOWER_11DBM        = 1,   /* 11dBm */
    ATK_MWCC68D_TPOWER_14DBM        = 2,   /* 14dBm */
    ATK_MWCC68D_TPOWER_17DBM        = 3,   /* 17dBm */
    ATK_MWCC68D_TPOWER_20DBM        = 4,   /* 20dBm（默认） */
    ATK_MWCC68D_TPOWER_22DBM        = 5,   /* 22dBm */
} atk_mwcc68d_tpower_t;

/* 工作模式枚举 */
typedef enum
{
    ATK_MWCC68D_WORKMODE_NORMAL     = 0,    /* 一般模式（默认） */
    ATK_MWCC68D_WORKMODE_WAKEUP     = 1,    /* 唤醒模式 */
    ATK_MWCC68D_WORKMODE_LOWPOWER   = 2,    /* 省电模式 */
    ATK_MWCC68D_WORKMODE_SIGNAL     = 3,    /* 信号强度模式 */
    ATK_MWCC68D_WORKMODE_SLEEP      = 4,    /* 睡眠模式 */
    ATK_MWCC68D_WORKMODE_RELAY      = 5,    /* 中继模式 */
} atk_mwcc68d_workmode_t;

/* 发射模式枚举 */
typedef enum
{
    ATK_MWCC68D_TMODE_TT            = 0,    /* 透明传输（默认） */
    ATK_MWCC68D_TMODE_DT            = 1,    /* 定向传输 */
} atk_mwcc68d_tmode_t;

/* 空中速率枚举 */
typedef enum
{
    ATK_MWCC68D_WLRATE_1K2          = 1,    /* 1.2Kbps */
    ATK_MWCC68D_WLRATE_2K4          = 2,    /* 2.4Kbps */
    ATK_MWCC68D_WLRATE_4K8          = 3,    /* 4.8Kbps */
    ATK_MWCC68D_WLRATE_9K6          = 4,    /* 9.6Kbps */
    ATK_MWCC68D_WLRATE_19K2         = 5,    /* 19.2Kbps（默认） */
    ATK_MWCC68D_WLRATE_38K4         = 6,    /* 38.4Kbps */
    ATK_MWCC68D_WLRATE_62K5         = 7,    /* 62.5Kbps */
} atk_mwcc68d_wlrate_t;

/* 休眠时间枚举 */
typedef enum
{
    ATK_MWCC68D_WLTIME_1S           = 0,    /* 1秒（默认） */
    ATK_MWCC68D_WLTIME_2S           = 1,    /* 2秒 */
} atk_mwcc68d_wltime_t;

/* 数据包大小枚举 */
typedef enum
{
    ATK_MWCC68D_PACKSIZE_32         = 0,    /* 32字节 */
    ATK_MWCC68D_PACKSIZE_64         = 1,    /* 64字节 */
    ATK_MWCC68D_PACKSIZE_128        = 2,    /* 128字节 */
    ATK_MWCC68D_PACKSIZE_240        = 3,    /* 240字节 */
} atk_mwcc68d_packsize_t;

/* 串口通信波特率枚举 */
typedef enum
{
    ATK_MWCC68D_UARTRATE_1200BPS    = 0,    /* 1200bps */
    ATK_MWCC68D_UARTRATE_2400BPS    = 1,    /* 2400bps */
    ATK_MWCC68D_UARTRATE_4800BPS    = 2,    /* 4800bps */
    ATK_MWCC68D_UARTRATE_9600BPS    = 3,    /* 9600bps */
    ATK_MWCC68D_UARTRATE_19200BPS   = 4,    /* 19200bps */
    ATK_MWCC68D_UARTRATE_38400BPS   = 5,    /* 38400bps */
    ATK_MWCC68D_UARTRATE_57600BPS   = 6,    /* 57600bps */
    ATK_MWCC68D_UARTRATE_115200BPS  = 7,    /* 115200bps（默认） */
} atk_mwcc68d_uartrate_t;

/* 串口通讯校验位枚举 */
typedef enum
{
    ATK_MWCC68D_UARTPARI_NONE       = 0,    /* 无校验（默认） */
    ATK_MWCC68D_UARTPARI_EVEN       = 1,    /* 偶校验 */
    ATK_MWCC68D_UARTPARI_ODD        = 2,    /* 奇校验 */
} atk_mwcc68d_uartpari_t;

/* 错误代码 */
#define ATK_MWCC68D_EOK             0       /* 没有错误 */
#define ATK_MWCC68D_ERROR           1       /* 通用错误 */
#define ATK_MWCC68D_ETIMEOUT        2       /* 超时错误 */
#define ATK_MWCC68D_EINVAL          3       /* 参数错误 */
#define ATK_MWCC68D_EBUSY           4       /* 忙错误 */
#define ATK_MWCC68D_EINIT           5       /* 初始化错误 */

/* 操作函数 */
void atk_mwcc68d_uart_printf(char *fmt, ...);       /* ATK-MWCC68D UART printf */
void atk_mwcc68d_uart_rx_restart(void);             /* ATK-MWCC68D UART重新开始接收数据 */
uint8_t *atk_mwcc68d_uart_rx_get_frame(void);       /* 获取ATK-MWCC68D UART接收到的一帧数据 */
uint16_t atk_mwcc68d_uart_rx_get_frame_len(void);   /* 获取ATK-MWCC68D UART接收到的一帧数据的长度 */

uint8_t atk_mwcc68d_init(void);                                                                     /* ATK-MWCC68D初始化 */
void atk_mwcc68d_enter_config(void);                                                                /* ATK-MWCC68D模块进入配置模式 */
void atk_mwcc68d_exit_config(void);                                                                 /* ATK-MWCC68D模块进退出置模式 */
uint8_t atk_mwcc68d_free(void);                                                                     /* 判断ATK-MWCC68D模块是否空闲 */
uint8_t atk_mwcc68d_send_at_cmd(char *cmd, char *ack, uint32_t timeout);                            /* 向ATK-MWCC68D模块发送AT指令 */
uint8_t atk_mwcc68d_at_test(void);                                                                  /* ATK-MWCC68D模块AT指令测试 */
uint8_t atk_mwcc68d_echo_config(atk_mwcc68d_enable_t enable);                                       /* ATK-MWCC68D模块指令回显配置 */
uint8_t atk_mwcc68d_sw_reset(void);                                                                 /* ATK-MWCC68D模块软件复位 */
uint8_t atk_mwcc68d_flash_config(atk_mwcc68d_enable_t enable);                                      /* ATK-MWCC68D模块参数保存配置 */
uint8_t atk_mwcc68d_default(void);                                                                  /* ATK-MWCC68D模块恢复出厂配置 */
uint8_t atk_mwcc68d_addr_config(uint16_t addr);                                                     /* ATK-MWCC68D模块设备地址配置 */
uint8_t atk_mwcc68d_tpower_config(atk_mwcc68d_tpower_t tpower);                                     /* ATK-MWCC68D模块发射功率配置 */
uint8_t atk_mwcc68d_workmode_config(atk_mwcc68d_workmode_t workmode);                               /* ATK-MWCC68D模块工作模式配置 */
uint8_t atk_mwcc68d_tmode_config(atk_mwcc68d_tmode_t tmode);                                        /* ATK-MWCC68D模块发送模式配置 */
uint8_t atk_mwcc68d_wlrate_channel_config(atk_mwcc68d_wlrate_t wlrate, uint8_t channel);            /* ATK-MWCC68D模块空中速率和信道配置 */
uint8_t atk_mwcc68d_netid_config(uint8_t netid);                                                    /* ATK-MWCC68D模块网络地址配置 */
uint8_t atk_mwcc68d_wltime_config(atk_mwcc68d_wltime_t wltime);                                     /* ATK-MWCC68D模块休眠时间配置 */
uint8_t atk_mwcc68d_packsize_config(atk_mwcc68d_packsize_t packsize);                               /* ATK-MWCC68D模块数据包大小配置 */
uint8_t atk_mwcc68d_datakey_config(uint32_t datakey);                                               /* ATK-MWCC68D模块数据加密密钥配置 */
uint8_t atk_mwcc68d_uart_config(atk_mwcc68d_uartrate_t baudrate, atk_mwcc68d_uartpari_t parity);    /* ATK-MWCC68D模块串口配置 */
uint8_t atk_mwcc68d_lbt_config(atk_mwcc68d_enable_t enable);

#endif /* ATK_MWCC68D_DRIVER_ATK_MWCC68D_H_ */
