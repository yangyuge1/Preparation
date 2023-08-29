/*
 * driver_atk_mwcc68d.c
 *
 *  Created on: 2023年7月19日
 *      Author: yyg
 */
#include "driver_atk_mwcc68d.h"

/**
 * @brief       ATK-MWCC68D UART printf
 * @param       fmt: 待打印的数据
 * @retval      无
 */
void atk_mwcc68d_uart_printf(char *fmt, ...)
{
    va_list ap;
    uint16_t len;

    va_start(ap, fmt);
    vsprintf((char *)g_uart_tx_buf, fmt, ap);
    va_end(ap);
    #define HAL_MAX_DELAY      0xFFFFFFFFU
    len = strlen((const char *)g_uart_tx_buf);
    ATK_MWCC68D_UART_Transmit(g_uart_tx_buf, len, HAL_MAX_DELAY);
}
/**
 * @brief       ATK-MWCC68D UART重新开始接收数据
 * @param       无
 * @retval      无
 */
void atk_mwcc68d_uart_rx_restart(void)
{
    g_uart_rx_frame.sta.len     = 0;
    g_uart_rx_frame.sta.finsh   = 0;
}

/**
 * @brief       获取ATK-MWCC68D UART接收到的一帧数据
 * @param       无
 * @retval      NULL: 未接收到一帧数据
 *              其他: 接收到的一帧数据
 */
uint8_t *atk_mwcc68d_uart_rx_get_frame(void)
{
    if (g_uart_rx_frame.sta.finsh == 1)
    {
        g_uart_rx_frame.buf[g_uart_rx_frame.sta.len] = '\0';
        return g_uart_rx_frame.buf;
    }
    else
    {
        return NULL;
    }
}

/**
 * @brief       获取ATK-MWCC68D UART接收到的一帧数据的长度
 * @param       无
 * @retval      0   : 未接收到一帧数据
 *              其他: 接收到的一帧数据的长度
 */
uint16_t atk_mwcc68d_uart_rx_get_frame_len(void)
{
    if (g_uart_rx_frame.sta.finsh == 1)
    {
        return g_uart_rx_frame.sta.len;
    }
    else
    {
        return 0;
    }
}

/**
 * @brief       ATK-MWCC68D模块初始化
 * @param       baudrate: ATK-MWCC68D模块UART通讯波特率
 * @retval      ATK_MWCC68D_EOK  : ATK-MWCC68D模块初始化成功，函数执行成功
 *              ATK_MWCC68D_ERROR: ATK-MWCC68D模块初始化失败，函数执行失败
 */
uint8_t atk_mwcc68d_init(void)
{
    uint8_t ret;

    g_uart_rx_frame.sta.finsh=0;
    g_uart_rx_frame.sta.len=0;

    atk_mwcc68d_enter_config();                     /* 进入配置模式 */
    ret = atk_mwcc68d_at_test();                    /* AT指令测试 */
    atk_mwcc68d_exit_config();                      /* 退出配置模式 */
    if (ret != ATK_MWCC68D_EOK)
    {
        return ATK_MWCC68D_ERROR;
    }

    return ATK_MWCC68D_EOK;
}

/**
 * @brief       ATK-MWCC68D模块进入配置模式
 * @param       无
 * @retval      无
 */
void atk_mwcc68d_enter_config(void)
{
    ATK_MWCC68D_MD0(1);
}

/**
 * @brief       ATK-MWCC68D模块退出配置模式
 * @param       无
 * @retval      无
 */
void atk_mwcc68d_exit_config(void)
{
    ATK_MWCC68D_MD0(0);
}
/**
 * @brief       判断ATK-MWCC68D模块是否空闲
 * @note        仅当ATK-MWCC68D模块空闲的时候，才能发送数据
 * @param       无
 * @retval      ATK_MWCC68D_EOK  : ATK-MWCC68D模块空闲
 *              ATK_MWCC68D_EBUSY: ATK-MWCC68D模块忙
 */
uint8_t atk_mwcc68d_free(void)
{
    if (ATK_MWCC68D_AUX() != 0)
    {
        return ATK_MWCC68D_EBUSY;
    }

    return ATK_MWCC68D_EOK;
}

/**
 * @brief       向ATK-MWCC68D模块发送AT指令
 * @param       cmd    : 待发送的AT指令
 *              ack    : 等待的响应
 *              timeout: 等待超时时间
 * @retval      ATK_MWCC68D_EOK     : 函数执行成功
 *              ATK_MWCC68D_ETIMEOUT: 等待期望应答超时，函数执行失败
 */
uint8_t atk_mwcc68d_send_at_cmd(char *cmd, char *ack, uint32_t timeout)
{
    uint8_t *ret = NULL;

    atk_mwcc68d_uart_rx_restart();
    atk_mwcc68d_uart_printf("%s\r\n", cmd);

    if ((ack == NULL) || (timeout == 0))
    {
        return ATK_MWCC68D_EOK;
    }
    else
    {
        while (timeout > 0)
        {
            ret = atk_mwcc68d_uart_rx_get_frame();
            if (ret != NULL)
            {
                if (strstr((const char *)ret, ack) != NULL)
                {
                    return ATK_MWCC68D_EOK;
                }
                else
                {
                    atk_mwcc68d_uart_rx_restart();
                }
            }
            timeout--;
            ATK_MWCC68D_delay_ms(1);
        }

        return ATK_MWCC68D_ETIMEOUT;
    }
}

/**
 * @brief       ATK-MWCC68D模块AT指令测试
 * @param       无
 * @retval      ATK_MWCC68D_EOK  : AT指令测试成功
 *              ATK_MWCC68D_ERROR: AT指令测试失败
 */
uint8_t atk_mwcc68d_at_test(void)
{
    uint8_t ret;
    uint8_t i;

    for (i=0; i<10; i++)
    {
        ret = atk_mwcc68d_send_at_cmd("AT", "OK", ATK_MWCC68D_AT_TIMEOUT);
        if (ret == ATK_MWCC68D_EOK)
        {
            return ATK_MWCC68D_EOK;
        }
    }

    return ATK_MWCC68D_ERROR;
}

/**
 * @brief       ATK-MWCC68D模块指令回显配置
 * @param       enable: ATK_MWCC68D_DISABLE: 关闭指令回显
 *                      ATK_MWCC68D_ENABLE : 开启指令回显
 * @retval      ATK_MWCC68D_EOK   : 指令回显配置成功
 *              ATK_MWCC68D_ERROR : 指令回显配置失败
 *              ATK_MWCC68D_EINVAL: 输入参数有误
 */
uint8_t atk_mwcc68d_echo_config(atk_mwcc68d_enable_t enable)
{
    uint8_t ret;
    char cmd[5] = {0};

    switch (enable)
    {
        case ATK_MWCC68D_ENABLE:
        {
            sprintf(cmd, "ATE1");
            break;
        }
        case ATK_MWCC68D_DISABLE:
        {
            sprintf(cmd, "ATE0");
            break;
        }
        default:
        {
            return ATK_MWCC68D_EINVAL;
        }
    }

    ret = atk_mwcc68d_send_at_cmd(cmd, "OK", ATK_MWCC68D_AT_TIMEOUT);
    if (ret != ATK_MWCC68D_EOK)
    {
        return ATK_MWCC68D_ERROR;
    }

    return ATK_MWCC68D_EOK;
}

/**
 * @brief       ATK-MWCC68D模块软件复位
 * @param       无
 * @retval      ATK_MWCC68D_EOK  : 软件复位成功
 *              ATK_MWCC68D_ERROR: 软件复位失败
 */
uint8_t atk_mwcc68d_sw_reset(void)
{
    uint8_t ret;

    ret = atk_mwcc68d_send_at_cmd("AT+RESET", "OK", ATK_MWCC68D_AT_TIMEOUT);
    if (ret != ATK_MWCC68D_EOK)
    {
        return ATK_MWCC68D_ERROR;
    }

    return ATK_MWCC68D_EOK;
}

/**
 * @brief       ATK-MWCC68D模块参数保存配置
 * @param       enable: ATK_MWCC68D_DISABLE: 不保存参数
 *                      ATK_MWCC68D_ENABLE : 保存参数
 * @retval      ATK_MWCC68D_EOK   : 参数保存配置成功
 *              ATK_MWCC68D_ERROR : 参数保存配置失败
 *              ATK_MWCC68D_EINVAL: 输入参数有误
 */
uint8_t atk_mwcc68d_flash_config(atk_mwcc68d_enable_t enable)
{
    uint8_t ret;
    char cmd[11] = {0};

    switch (enable)
    {
        case ATK_MWCC68D_DISABLE:
        {
            sprintf(cmd, "AT+FLASH=0");
            break;
        }
        case ATK_MWCC68D_ENABLE:
        {
            sprintf(cmd, "AT+FLASH=1");
            break;
        }
        default:
        {
            return ATK_MWCC68D_EINVAL;
        }
    }

    ret = atk_mwcc68d_send_at_cmd(cmd, "OK", ATK_MWCC68D_AT_TIMEOUT);
    if (ret != ATK_MWCC68D_EOK)
    {
        return ATK_MWCC68D_ERROR;
    }

    return ATK_MWCC68D_EOK;
}

/**
 * @brief       ATK-MWCC68D模块恢复出厂配置
 * @param       无
 * @retval      ATK_MWCC68D_EOK   : 恢复出厂配置成功
 *              ATK_MWCC68D_ERROR : 恢复出厂配置失败
 */
uint8_t atk_mwcc68d_default(void)
{
    uint8_t ret;

    ret = atk_mwcc68d_send_at_cmd("AT+DEFAULT", "OK", ATK_MWCC68D_AT_TIMEOUT);
    if (ret != ATK_MWCC68D_EOK)
    {
        return ATK_MWCC68D_ERROR;
    }

    return ATK_MWCC68D_EOK;
}

/**
 * @brief       ATK-MWCC68D模块设备地址配置
 * @param       addr: 设备地址
 * @retval      ATK_MWCC68D_EOK   : 设备地址配置成功
 *              ATK_MWCC68D_ERROR : 设备地址配置失败
 */
uint8_t atk_mwcc68d_addr_config(uint16_t addr)
{
    uint8_t ret,addrh,addrl;
    addrh=(uint8_t)(addr >> 8);
    addrl=(uint8_t)addr;
    char cmd[14] = {0};
    sprintf(cmd, "AT+ADDR=%02X,%02X", addrh,addrl);
    ret = atk_mwcc68d_send_at_cmd(cmd, "OK", ATK_MWCC68D_AT_TIMEOUT);
    if (ret != ATK_MWCC68D_EOK)
    {
        return ATK_MWCC68D_ERROR;
    }

    return ATK_MWCC68D_EOK;
}

/**
 * @brief       ATK-MWCC68D模块发射功率配置
 * @param       tpower: ATK_MWCC68D_TPOWER_9DBM : 9dBm
 *                      ATK_MWCC68D_TPOWER_11DBM: 11dBm
 *                      ATK_MWCC68D_TPOWER_14DBM: 14dBm
 *                      ATK_MWCC68D_TPOWER_17DBM: 17dBm
 *                      ATK_MWCC68D_TPOWER_20DBM: 20dBm（默认）
 *                      ATK_MWCC68D_TPOWER_22DBM: 22dBm
 * @retval      ATK_MWCC68D_EOK   : 发射功率配置成功
 *              ATK_MWCC68D_ERROR : 发射功率配置失败
 *              ATK_MWCC68D_EINVAL: 输入参数有误
 */
uint8_t atk_mwcc68d_tpower_config(atk_mwcc68d_tpower_t tpower)
{
    uint8_t ret;
    char cmd[12] = {0};

    switch (tpower)
    {
        case ATK_MWCC68D_TPOWER_9DBM:
        case ATK_MWCC68D_TPOWER_11DBM:
        case ATK_MWCC68D_TPOWER_14DBM:
        case ATK_MWCC68D_TPOWER_17DBM:
        case ATK_MWCC68D_TPOWER_20DBM:
        case ATK_MWCC68D_TPOWER_22DBM:
        {
            break;
        }
        default:
        {
            return ATK_MWCC68D_EINVAL;
        }
    }

    sprintf(cmd, "AT+TPOWER=%d", tpower);

    ret = atk_mwcc68d_send_at_cmd(cmd, "OK", ATK_MWCC68D_AT_TIMEOUT);
    if (ret != ATK_MWCC68D_EOK)
    {
        return ATK_MWCC68D_ERROR;
    }

    return ATK_MWCC68D_EOK;
}

/**
 * @brief       ATK-MWCC68D模块工作模式配置
 * @param       workmode: ATK_MWCC68D_WORKMODE_NORMAL  : 一般模式（默认）
 *                        ATK_MWCC68D_WORKMODE_WAKEUP  : 唤醒模式
 *                        ATK_MWCC68D_WORKMODE_LOWPOWER: 省电模式
 *                        ATK_MWCC68D_WORKMODE_SIGNAL  : 信号强度模式
 *                        ATK_MWCC68D_WORKMODE_SLEEP   : 睡眠模式
 *                        ATK_MWCC68D_WORKMODE_RELAY   : 中继模式
 * @retval      ATK_MWCC68D_EOK   : 工作模式配置成功
 *              ATK_MWCC68D_ERROR : 工作模式配置失败
 *              ATK_MWCC68D_EINVAL: 输入参数有误
 */
uint8_t atk_mwcc68d_workmode_config(atk_mwcc68d_workmode_t workmode)
{
    uint8_t ret;
    char cmd[12] = {0};

    switch (workmode)
    {
        case ATK_MWCC68D_WORKMODE_NORMAL:
        case ATK_MWCC68D_WORKMODE_WAKEUP:
        case ATK_MWCC68D_WORKMODE_LOWPOWER:
        case ATK_MWCC68D_WORKMODE_SIGNAL:
        case ATK_MWCC68D_WORKMODE_SLEEP:
        case ATK_MWCC68D_WORKMODE_RELAY:
        {
            break;
        }
        default:
        {
            return ATK_MWCC68D_EINVAL;
        }
    }

    sprintf(cmd, "AT+CWMODE=%d", workmode);

    ret = atk_mwcc68d_send_at_cmd(cmd, "OK", ATK_MWCC68D_AT_TIMEOUT);
    if (ret != ATK_MWCC68D_EOK)
    {
        return ATK_MWCC68D_ERROR;
    }

    return ATK_MWCC68D_EOK;
}

/**
 * @brief       ATK-MWCC68D模块发送模式配置
 * @param       tmode: ATK_MWCC68D_TMODE_TT: 透明传输（默认）
 *                     ATK_MWCC68D_TMODE_DT: 定向传输
 * @retval      ATK_MWCC68D_EOK   : 发送模式配置成功
 *              ATK_MWCC68D_ERROR : 发送模式配置失败
 *              ATK_MWCC68D_EINVAL: 输入参数有误
 */
uint8_t atk_mwcc68d_tmode_config(atk_mwcc68d_tmode_t tmode)
{
    uint8_t ret;
    char cmd[11] = {0};

    switch (tmode)
    {
        case ATK_MWCC68D_TMODE_TT:
        case ATK_MWCC68D_TMODE_DT:
        {
            break;
        }
        default:
        {
            return ATK_MWCC68D_EINVAL;
        }
    }

    sprintf(cmd, "AT+TMODE=%d", tmode);

    ret = atk_mwcc68d_send_at_cmd(cmd, "OK", ATK_MWCC68D_AT_TIMEOUT);
    if (ret != ATK_MWCC68D_EOK)
    {
        return ATK_MWCC68D_ERROR;
    }

    return ATK_MWCC68D_EOK;
}

/**
 * @brief       ATK-MWCC68D模块空中速率和信道配置
 * @param       wlrate : ATK_MWCC68D_WLRATE_1K2 : 1.2Kbps
 *                       ATK_MWCC68D_WLRATE_2K4 : 2.4Kbps
 *                       ATK_MWCC68D_WLRATE_4K8 : 4.8Kbps
 *                       ATK_MWCC68D_WLRATE_9K6 : 9.6Kbps
 *                       ATK_MWCC68D_WLRATE_19K2: 19.2Kbps（默认）
 *                       ATK_MWCC68D_WLRATE_38K4: 38.4Kbps
 *                       ATK_MWCC68D_WLRATE_62K5: 62.5Kbps
 *              channel: 信道，范围0~83
 * @retval      ATK_MWCC68D_EOK   : 空中速率和信道配置成功
 *              ATK_MWCC68D_ERROR : 空中速率和信道配置失败
 *              ATK_MWCC68D_EINVAL: 输入参数有误
 */
uint8_t atk_mwcc68d_wlrate_channel_config(atk_mwcc68d_wlrate_t wlrate, uint8_t channel)
{
    uint8_t ret;
    char cmd[15] = {0};

    switch (wlrate)
    {
        case ATK_MWCC68D_WLRATE_1K2:
        case ATK_MWCC68D_WLRATE_2K4:
        case ATK_MWCC68D_WLRATE_4K8:
        case ATK_MWCC68D_WLRATE_9K6:
        case ATK_MWCC68D_WLRATE_19K2:
        case ATK_MWCC68D_WLRATE_38K4:
        case ATK_MWCC68D_WLRATE_62K5:
        {
            break;
        }
        default:
        {
            return ATK_MWCC68D_EINVAL;
        }
    }

    if (channel > 83)
    {
        return ATK_MWCC68D_EINVAL;
    }

    sprintf(cmd, "AT+WLRATE=%d,%d", channel, wlrate);

    ret = atk_mwcc68d_send_at_cmd(cmd, "OK", ATK_MWCC68D_AT_TIMEOUT);
    if (ret != ATK_MWCC68D_EOK)
    {
        return ATK_MWCC68D_ERROR;
    }

    return ATK_MWCC68D_EOK;
}

/**
 * @brief       ATK-MWCC68D模块网络地址配置
 * @param       netid: 网络地址，范围（0~255）
 * @retval      ATK_MWCC68D_EOK   : 网络地址配置成功
 *              ATK_MWCC68D_ERROR : 网络地址配置失败
 */
uint8_t atk_mwcc68d_netid_config(uint8_t netid)
{
    uint8_t ret;
    char cmd[13] = {0};

    sprintf(cmd, "AT+NETID=%d", netid);

    ret = atk_mwcc68d_send_at_cmd(cmd, "OK", ATK_MWCC68D_AT_TIMEOUT);
    if (ret != ATK_MWCC68D_EOK)
    {
        return ATK_MWCC68D_ERROR;
    }

    return ATK_MWCC68D_EOK;
}

/**
 * @brief       ATK-MWCC68D模块休眠时间配置
 * @param       wltime: ATK_MWCC68D_WLTIME_1S: 1秒（默认）
 *                      ATK_MWCC68D_WLTIME_2S: 2秒
 * @retval      ATK_MWCC68D_EOK   : 休眠时间配置成功
 *              ATK_MWCC68D_ERROR : 休眠时间配置失败
 *              ATK_MWCC68D_EINVAL: 输入参数有误
 */
uint8_t atk_mwcc68d_wltime_config(atk_mwcc68d_wltime_t wltime)
{
    uint8_t ret;
    char cmd[12] = {0};

    switch (wltime)
    {
        case ATK_MWCC68D_WLTIME_1S:
        case ATK_MWCC68D_WLTIME_2S:
        {
            break;
        }
        default:
        {
            return ATK_MWCC68D_EINVAL;
        }
    }

    sprintf(cmd, "AT+WLTIME=%d", wltime);

    ret = atk_mwcc68d_send_at_cmd(cmd, "OK", ATK_MWCC68D_AT_TIMEOUT);
    if (ret != ATK_MWCC68D_EOK)
    {
        return ATK_MWCC68D_ERROR;
    }

    return ATK_MWCC68D_EOK;
}

/**
 * @brief       ATK-MWCC68D模块数据包大小配置
 * @param       packsize: ATK_MWCC68D_PACKSIZE_32 : 32字节
 *                        ATK_MWCC68D_PACKSIZE_64 : 64字节
 *                        ATK_MWCC68D_PACKSIZE_128: 128字节
 *                        ATK_MWCC68D_PACKSIZE_240: 240字节
 * @retval      ATK_MWCC68D_EOK   : 数据包大小配置成功
 *              ATK_MWCC68D_ERROR : 数据包大小配置失败
 *              ATK_MWCC68D_EINVAL: 输入参数有误
 */
uint8_t atk_mwcc68d_packsize_config(atk_mwcc68d_packsize_t packsize)
{
    uint8_t ret;
    char cmd[14] = {0};

    switch (packsize)
    {
        case ATK_MWCC68D_PACKSIZE_32:
        case ATK_MWCC68D_PACKSIZE_64:
        case ATK_MWCC68D_PACKSIZE_128:
        case ATK_MWCC68D_PACKSIZE_240:
        {
            break;
        }
        default:
        {
            return ATK_MWCC68D_EINVAL;
        }
    }

    sprintf(cmd, "AT+PACKSIZE=%d", packsize);

    ret = atk_mwcc68d_send_at_cmd(cmd, "OK", ATK_MWCC68D_AT_TIMEOUT);
    if (ret != ATK_MWCC68D_EOK)
    {
        return ATK_MWCC68D_ERROR;
    }

    return ATK_MWCC68D_EOK;
}

/**
 * @brief       ATK-MWCC68D模块数据加密密钥配置
 * @param       datakey: 数据加密密钥，范围0~0xFFFFFFFF
 * @retval      ATK_MWCC68D_EOK   : 数据包大小配置成功
 *              ATK_MWCC68D_ERROR : 数据包大小配置失败
 *              ATK_MWCC68D_EINVAL: 输入参数有误
 */
uint8_t atk_mwcc68d_datakey_config(uint32_t datakey)
{
    uint8_t ret;
    char cmd[20] = {0};

    sprintf(cmd, "AT+DATAKEY=%08X", datakey);

    ret = atk_mwcc68d_send_at_cmd(cmd, "OK", ATK_MWCC68D_AT_TIMEOUT);
    if (ret != ATK_MWCC68D_EOK)
    {
        return ATK_MWCC68D_ERROR;
    }

    return ATK_MWCC68D_EOK;
}

/**
 * @brief       ATK-MWCC68D模块串口配置
 * @param       baudrate: ATK_MWCC68D_UARTRATE_1200BPS  : 1200bps
 *                        ATK_MWCC68D_UARTRATE_2400BPS  : 2400bps
 *                        ATK_MWCC68D_UARTRATE_4800BPS  : 4800bps
 *                        ATK_MWCC68D_UARTRATE_9600BPS  : 9600bps
 *                        ATK_MWCC68D_UARTRATE_19200BPS : 19200bps
 *                        ATK_MWCC68D_UARTRATE_38400BPS : 38400bps
 *                        ATK_MWCC68D_UARTRATE_57600BPS : 57600bps
 *                        ATK_MWCC68D_UARTRATE_115200BPS: 115200bps（默认）
 *              parity  : ATK_MWCC68D_UARTPARI_NONE: 无校验（默认）
 *                        ATK_MWCC68D_UARTPARI_EVEN: 偶校验
 *                        ATK_MWCC68D_UARTPARI_ODD : 奇校验
 * @retval      ATK_MWCC68D_EOK   : 串口配置成功
 *              ATK_MWCC68D_ERROR : 串口配置失败
 *              ATK_MWCC68D_EINVAL: 输入参数有误
 */
uint8_t atk_mwcc68d_uart_config(atk_mwcc68d_uartrate_t baudrate, atk_mwcc68d_uartpari_t parity)
{
    uint8_t ret;
    char cmd[12] = {0};

    switch (baudrate)
    {
        case ATK_MWCC68D_UARTRATE_1200BPS:
        case ATK_MWCC68D_UARTRATE_2400BPS:
        case ATK_MWCC68D_UARTRATE_4800BPS:
        case ATK_MWCC68D_UARTRATE_9600BPS:
        case ATK_MWCC68D_UARTRATE_19200BPS:
        case ATK_MWCC68D_UARTRATE_38400BPS:
        case ATK_MWCC68D_UARTRATE_57600BPS:
        case ATK_MWCC68D_UARTRATE_115200BPS:
        {
            break;
        }
        default:
        {
            return ATK_MWCC68D_EINVAL;
        }
    }

    switch (parity)
    {
        case ATK_MWCC68D_UARTPARI_NONE:
        case ATK_MWCC68D_UARTPARI_EVEN:
        case ATK_MWCC68D_UARTPARI_ODD:
        {
            break;
        }
        default:
        {
            return ATK_MWCC68D_EINVAL;
        }
    }

    sprintf(cmd, "AT+UART=%d,%d", baudrate, parity);

    ret = atk_mwcc68d_send_at_cmd(cmd, "OK", ATK_MWCC68D_AT_TIMEOUT);
    if (ret != ATK_MWCC68D_EOK)
    {
        return ATK_MWCC68D_ERROR;
    }

    return ATK_MWCC68D_EOK;
}

/**
 * @brief       ATK-MWCC68D模块信道检测配置
 * @param       enable: ATK_MWCC68D_DISABLE: 关闭信道检测
 *                      ATK_MWCC68D_ENABLE : 打开信道检测
 * @retval      ATK_MWCC68D_EOK   : 信道检测配置成功
 *              ATK_MWCC68D_ERROR : 信道检测配置失败
 *              ATK_MWCC68D_EINVAL: 输入参数有误
 */
uint8_t atk_mwcc68d_lbt_config(atk_mwcc68d_enable_t enable)
{
    uint8_t ret;
    char cmd[9] = {0};

    switch (enable)
    {
        case ATK_MWCC68D_DISABLE:
        {
            sprintf(cmd, "AT+LBT=0");
            break;
        }
        case ATK_MWCC68D_ENABLE:
        {
            sprintf(cmd, "AT+LBT=0");
            break;
        }
        default:
        {
            return ATK_MWCC68D_EINVAL;
        }
    }

    ret = atk_mwcc68d_send_at_cmd(cmd, "OK", ATK_MWCC68D_AT_TIMEOUT);
    if (ret != ATK_MWCC68D_EOK)
    {
        return ATK_MWCC68D_ERROR;
    }

    return ATK_MWCC68D_EOK;
}



