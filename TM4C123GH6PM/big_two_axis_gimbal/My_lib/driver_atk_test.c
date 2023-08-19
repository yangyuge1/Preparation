/*
 * driver_atk_test.c
 *
 *  Created on: 2023��7��3��
 *      Author: yyg
 */

#include "driver_atk_test.h"
#include "driver_atk_mwcc68d.h"

/* ������� */
#define ATK_MWCC68D_EOK             0       /* û�д��� */
#define ATK_MWCC68D_ERROR           1       /* ͨ�ô��� */
#define ATK_MWCC68D_ETIMEOUT        2       /* ��ʱ���� */
#define ATK_MWCC68D_EINVAL          3       /* �������� */
#define ATK_MWCC68D_EBUSY           4       /* æ���� */
#define ATK_MWCC68D_EINIT           5       /* ��ʼ������ */
/* UART�շ������С */
#define ATK_MWCC68D_UART_RX_BUF_SIZE            128
#define ATK_MWCC68D_UART_TX_BUF_SIZE            128
struct
{
    uint8_t buf[ATK_MWCC68D_UART_RX_BUF_SIZE];                /* ֡���ջ��� */
    int len  ;                                                /* ֡���ճ��ȣ�sta[14:0] */
    int finsh;                                                /* ֡������ɱ�־��sta[15] */
} atk_uart_rx_frame = {0};                                    /* ATK-MWCC68D UART����֡������Ϣ�ṹ�� */
uint8_t atk_uart_tx_buf[ATK_MWCC68D_UART_TX_BUF_SIZE];        /* ATK-MWCC68D UART���ͻ��� */

int isOK(uint8_t *p){
    int i=0;
    while( p[i]!=0 && p[i+1]!=0 ){
        if(p[i]==0x4F && p[i+1]==0x4B){
            return 1;
        }
            i++;
    }
    return 0;
}

uint8_t atk_send_at_cmd(char *cmd, uint32_t timeout)
{
    uint8_t *ret = NULL;
    atk_mwcc68d_uart_rx_restart();
    atk_mwcc68d_uart_printf("%s\r\n", cmd);
    ATK_MWCC68D_delay_ms(20);
    while (timeout > 0)
    {
        ret = atk_mwcc68d_uart_rx_get_frame();
        if (ret != NULL)
        {
            if (isOK(ret))
            {
                return ATK_MWCC68D_EOK;
            }
            else
            {
                atk_mwcc68d_uart_rx_restart();
            }
        }
        timeout--;
        ATK_MWCC68D_delay_ms(10);
    }
    return ATK_MWCC68D_ETIMEOUT;
}

void my_strcat(char *string1,char *string2){
    int i=0,j=0;
    while(*(string1+i)!=0){
        i++;
    };
    do{
        *(string1+i+j)=*(string2+j);
        j++;
    }
    while(*(string2+j)!=0);
    *(string1+i+j)=*(string2+j);
}
void int2string(char *string,int num){

}













