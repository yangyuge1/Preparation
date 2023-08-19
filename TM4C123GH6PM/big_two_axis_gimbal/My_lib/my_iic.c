/*
 * my_iic.c
 *
 *  Created on: 2023年7月2日
 *      Author: yyg
 */

#include "my_iic.h"

void i2c_init_i2c0_PB2_3(void){
    SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C0);
    //
    // 引脚时钟使能
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    //
    // I2C 引脚设置
    //
    GPIOPinTypeI2CSCL(GPIO_PORTB_BASE, GPIO_PIN_2);
    GPIOPinTypeI2C(GPIO_PORTB_BASE, GPIO_PIN_3);
    GPIOPinConfigure(GPIO_PB2_I2C0SCL);
    GPIOPinConfigure(GPIO_PB3_I2C0SDA);
    //
    // 配置 I2C1
    //
    I2CMasterInitExpClk(I2C0_BASE, SysCtlClockGet(), true);
}
void i2c_init_i2c1_PA6_7(void){
    SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C1);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    GPIOPinTypeI2CSCL(GPIO_PORTA_BASE, GPIO_PIN_6);
    GPIOPinTypeI2C(GPIO_PORTA_BASE, GPIO_PIN_7);
    GPIOPinConfigure(GPIO_PA6_I2C1SCL);
    GPIOPinConfigure(GPIO_PA7_I2C1SDA);
    I2CMasterInitExpClk(I2C1_BASE, SysCtlClockGet(), true);
}
void i2c_init_i2c2_PE4_3(void){
    SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C2);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    GPIOPinTypeI2CSCL(GPIO_PORTE_BASE, GPIO_PIN_4);
    GPIOPinTypeI2C(GPIO_PORTE_BASE, GPIO_PIN_3);
    GPIOPinConfigure(GPIO_PE4_I2C2SCL);
    GPIOPinConfigure(GPIO_PE5_I2C2SDA);
    I2CMasterInitExpClk(I2C2_BASE, SysCtlClockGet(), true);
}
//待完成
void i2c_init_i2c3_PD0_1(void){
    SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C3);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    GPIOPinTypeI2CSCL(GPIO_PORTD_BASE, GPIO_PIN_0);
    GPIOPinTypeI2C(GPIO_PORTD_BASE, GPIO_PIN_0);
    GPIOPinConfigure(GPIO_PD0_I2C3SCL);
    GPIOPinConfigure(GPIO_PD1_I2C3SDA);
    I2CMasterInitExpClk(I2C3_BASE, SysCtlClockGet(), true);
}
//as5600 读角度参考
//uint8_t data_h,data_l;
//uint16_t retVal = 0;
//I2CReadRegOneByte(I2C0_BASE,0X36,0x0c,&data_h);
//I2CReadRegOneByte(I2C0_BASE,0X36,0x0d,&data_l);
//retVal = (data_h&0x0F) << 8;
//retVal = retVal+data_l;
//retVal=0;
