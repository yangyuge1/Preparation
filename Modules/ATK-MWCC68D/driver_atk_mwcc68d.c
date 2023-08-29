/*
 * driver_atk_mwcc68d.c
 *
 *  Created on: 2023��7��19��
 *      Author: yyg
 */
#include "driver_atk_mwcc68d.h"

/**
 * @brief       ATK-MWCC68D UART printf
 * @param       fmt: ����ӡ������
 * @retval      ��
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
 * @brief       ATK-MWCC68D UART���¿�ʼ��������
 * @param       ��
 * @retval      ��
 */
void atk_mwcc68d_uart_rx_restart(void)
{
    g_uart_rx_frame.sta.len     = 0;
    g_uart_rx_frame.sta.finsh   = 0;
}

/**
 * @brief       ��ȡATK-MWCC68D UART���յ���һ֡����
 * @param       ��
 * @retval      NULL: δ���յ�һ֡����
 *              ����: ���յ���һ֡����
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
 * @brief       ��ȡATK-MWCC68D UART���յ���һ֡���ݵĳ���
 * @param       ��
 * @retval      0   : δ���յ�һ֡����
 *              ����: ���յ���һ֡���ݵĳ���
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
 * @brief       ATK-MWCC68Dģ���ʼ��
 * @param       baudrate: ATK-MWCC68Dģ��UARTͨѶ������
 * @retval      ATK_MWCC68D_EOK  : ATK-MWCC68Dģ���ʼ���ɹ�������ִ�гɹ�
 *              ATK_MWCC68D_ERROR: ATK-MWCC68Dģ���ʼ��ʧ�ܣ�����ִ��ʧ��
 */
uint8_t atk_mwcc68d_init(void)
{
    uint8_t ret;

    g_uart_rx_frame.sta.finsh=0;
    g_uart_rx_frame.sta.len=0;

    atk_mwcc68d_enter_config();                     /* ��������ģʽ */
    ret = atk_mwcc68d_at_test();                    /* ATָ����� */
    atk_mwcc68d_exit_config();                      /* �˳�����ģʽ */
    if (ret != ATK_MWCC68D_EOK)
    {
        return ATK_MWCC68D_ERROR;
    }

    return ATK_MWCC68D_EOK;
}

/**
 * @brief       ATK-MWCC68Dģ���������ģʽ
 * @param       ��
 * @retval      ��
 */
void atk_mwcc68d_enter_config(void)
{
    ATK_MWCC68D_MD0(1);
}

/**
 * @brief       ATK-MWCC68Dģ���˳�����ģʽ
 * @param       ��
 * @retval      ��
 */
void atk_mwcc68d_exit_config(void)
{
    ATK_MWCC68D_MD0(0);
}
/**
 * @brief       �ж�ATK-MWCC68Dģ���Ƿ����
 * @note        ����ATK-MWCC68Dģ����е�ʱ�򣬲��ܷ�������
 * @param       ��
 * @retval      ATK_MWCC68D_EOK  : ATK-MWCC68Dģ�����
 *              ATK_MWCC68D_EBUSY: ATK-MWCC68Dģ��æ
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
 * @brief       ��ATK-MWCC68Dģ�鷢��ATָ��
 * @param       cmd    : �����͵�ATָ��
 *              ack    : �ȴ�����Ӧ
 *              timeout: �ȴ���ʱʱ��
 * @retval      ATK_MWCC68D_EOK     : ����ִ�гɹ�
 *              ATK_MWCC68D_ETIMEOUT: �ȴ�����Ӧ��ʱ������ִ��ʧ��
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
 * @brief       ATK-MWCC68Dģ��ATָ�����
 * @param       ��
 * @retval      ATK_MWCC68D_EOK  : ATָ����Գɹ�
 *              ATK_MWCC68D_ERROR: ATָ�����ʧ��
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
 * @brief       ATK-MWCC68Dģ��ָ���������
 * @param       enable: ATK_MWCC68D_DISABLE: �ر�ָ�����
 *                      ATK_MWCC68D_ENABLE : ����ָ�����
 * @retval      ATK_MWCC68D_EOK   : ָ��������óɹ�
 *              ATK_MWCC68D_ERROR : ָ���������ʧ��
 *              ATK_MWCC68D_EINVAL: �����������
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
 * @brief       ATK-MWCC68Dģ�������λ
 * @param       ��
 * @retval      ATK_MWCC68D_EOK  : �����λ�ɹ�
 *              ATK_MWCC68D_ERROR: �����λʧ��
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
 * @brief       ATK-MWCC68Dģ�������������
 * @param       enable: ATK_MWCC68D_DISABLE: ���������
 *                      ATK_MWCC68D_ENABLE : �������
 * @retval      ATK_MWCC68D_EOK   : �����������óɹ�
 *              ATK_MWCC68D_ERROR : ������������ʧ��
 *              ATK_MWCC68D_EINVAL: �����������
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
 * @brief       ATK-MWCC68Dģ��ָ���������
 * @param       ��
 * @retval      ATK_MWCC68D_EOK   : �ָ��������óɹ�
 *              ATK_MWCC68D_ERROR : �ָ���������ʧ��
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
 * @brief       ATK-MWCC68Dģ���豸��ַ����
 * @param       addr: �豸��ַ
 * @retval      ATK_MWCC68D_EOK   : �豸��ַ���óɹ�
 *              ATK_MWCC68D_ERROR : �豸��ַ����ʧ��
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
 * @brief       ATK-MWCC68Dģ�鷢�书������
 * @param       tpower: ATK_MWCC68D_TPOWER_9DBM : 9dBm
 *                      ATK_MWCC68D_TPOWER_11DBM: 11dBm
 *                      ATK_MWCC68D_TPOWER_14DBM: 14dBm
 *                      ATK_MWCC68D_TPOWER_17DBM: 17dBm
 *                      ATK_MWCC68D_TPOWER_20DBM: 20dBm��Ĭ�ϣ�
 *                      ATK_MWCC68D_TPOWER_22DBM: 22dBm
 * @retval      ATK_MWCC68D_EOK   : ���书�����óɹ�
 *              ATK_MWCC68D_ERROR : ���书������ʧ��
 *              ATK_MWCC68D_EINVAL: �����������
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
 * @brief       ATK-MWCC68Dģ�鹤��ģʽ����
 * @param       workmode: ATK_MWCC68D_WORKMODE_NORMAL  : һ��ģʽ��Ĭ�ϣ�
 *                        ATK_MWCC68D_WORKMODE_WAKEUP  : ����ģʽ
 *                        ATK_MWCC68D_WORKMODE_LOWPOWER: ʡ��ģʽ
 *                        ATK_MWCC68D_WORKMODE_SIGNAL  : �ź�ǿ��ģʽ
 *                        ATK_MWCC68D_WORKMODE_SLEEP   : ˯��ģʽ
 *                        ATK_MWCC68D_WORKMODE_RELAY   : �м�ģʽ
 * @retval      ATK_MWCC68D_EOK   : ����ģʽ���óɹ�
 *              ATK_MWCC68D_ERROR : ����ģʽ����ʧ��
 *              ATK_MWCC68D_EINVAL: �����������
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
 * @brief       ATK-MWCC68Dģ�鷢��ģʽ����
 * @param       tmode: ATK_MWCC68D_TMODE_TT: ͸�����䣨Ĭ�ϣ�
 *                     ATK_MWCC68D_TMODE_DT: ������
 * @retval      ATK_MWCC68D_EOK   : ����ģʽ���óɹ�
 *              ATK_MWCC68D_ERROR : ����ģʽ����ʧ��
 *              ATK_MWCC68D_EINVAL: �����������
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
 * @brief       ATK-MWCC68Dģ��������ʺ��ŵ�����
 * @param       wlrate : ATK_MWCC68D_WLRATE_1K2 : 1.2Kbps
 *                       ATK_MWCC68D_WLRATE_2K4 : 2.4Kbps
 *                       ATK_MWCC68D_WLRATE_4K8 : 4.8Kbps
 *                       ATK_MWCC68D_WLRATE_9K6 : 9.6Kbps
 *                       ATK_MWCC68D_WLRATE_19K2: 19.2Kbps��Ĭ�ϣ�
 *                       ATK_MWCC68D_WLRATE_38K4: 38.4Kbps
 *                       ATK_MWCC68D_WLRATE_62K5: 62.5Kbps
 *              channel: �ŵ�����Χ0~83
 * @retval      ATK_MWCC68D_EOK   : �������ʺ��ŵ����óɹ�
 *              ATK_MWCC68D_ERROR : �������ʺ��ŵ�����ʧ��
 *              ATK_MWCC68D_EINVAL: �����������
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
 * @brief       ATK-MWCC68Dģ�������ַ����
 * @param       netid: �����ַ����Χ��0~255��
 * @retval      ATK_MWCC68D_EOK   : �����ַ���óɹ�
 *              ATK_MWCC68D_ERROR : �����ַ����ʧ��
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
 * @brief       ATK-MWCC68Dģ������ʱ������
 * @param       wltime: ATK_MWCC68D_WLTIME_1S: 1�루Ĭ�ϣ�
 *                      ATK_MWCC68D_WLTIME_2S: 2��
 * @retval      ATK_MWCC68D_EOK   : ����ʱ�����óɹ�
 *              ATK_MWCC68D_ERROR : ����ʱ������ʧ��
 *              ATK_MWCC68D_EINVAL: �����������
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
 * @brief       ATK-MWCC68Dģ�����ݰ���С����
 * @param       packsize: ATK_MWCC68D_PACKSIZE_32 : 32�ֽ�
 *                        ATK_MWCC68D_PACKSIZE_64 : 64�ֽ�
 *                        ATK_MWCC68D_PACKSIZE_128: 128�ֽ�
 *                        ATK_MWCC68D_PACKSIZE_240: 240�ֽ�
 * @retval      ATK_MWCC68D_EOK   : ���ݰ���С���óɹ�
 *              ATK_MWCC68D_ERROR : ���ݰ���С����ʧ��
 *              ATK_MWCC68D_EINVAL: �����������
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
 * @brief       ATK-MWCC68Dģ�����ݼ�����Կ����
 * @param       datakey: ���ݼ�����Կ����Χ0~0xFFFFFFFF
 * @retval      ATK_MWCC68D_EOK   : ���ݰ���С���óɹ�
 *              ATK_MWCC68D_ERROR : ���ݰ���С����ʧ��
 *              ATK_MWCC68D_EINVAL: �����������
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
 * @brief       ATK-MWCC68Dģ�鴮������
 * @param       baudrate: ATK_MWCC68D_UARTRATE_1200BPS  : 1200bps
 *                        ATK_MWCC68D_UARTRATE_2400BPS  : 2400bps
 *                        ATK_MWCC68D_UARTRATE_4800BPS  : 4800bps
 *                        ATK_MWCC68D_UARTRATE_9600BPS  : 9600bps
 *                        ATK_MWCC68D_UARTRATE_19200BPS : 19200bps
 *                        ATK_MWCC68D_UARTRATE_38400BPS : 38400bps
 *                        ATK_MWCC68D_UARTRATE_57600BPS : 57600bps
 *                        ATK_MWCC68D_UARTRATE_115200BPS: 115200bps��Ĭ�ϣ�
 *              parity  : ATK_MWCC68D_UARTPARI_NONE: ��У�飨Ĭ�ϣ�
 *                        ATK_MWCC68D_UARTPARI_EVEN: żУ��
 *                        ATK_MWCC68D_UARTPARI_ODD : ��У��
 * @retval      ATK_MWCC68D_EOK   : �������óɹ�
 *              ATK_MWCC68D_ERROR : ��������ʧ��
 *              ATK_MWCC68D_EINVAL: �����������
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
 * @brief       ATK-MWCC68Dģ���ŵ��������
 * @param       enable: ATK_MWCC68D_DISABLE: �ر��ŵ����
 *                      ATK_MWCC68D_ENABLE : ���ŵ����
 * @retval      ATK_MWCC68D_EOK   : �ŵ�������óɹ�
 *              ATK_MWCC68D_ERROR : �ŵ��������ʧ��
 *              ATK_MWCC68D_EINVAL: �����������
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



