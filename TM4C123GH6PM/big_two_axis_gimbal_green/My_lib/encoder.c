/*
 * encoder.c
 *
 *  Created on: 2023Äê6ÔÂ22ÈÕ
 *      Author: yyg
 */
#include "encoder.h"
void init_encoder(){
//    QEI1
//    PC6 B
//    PC5 A
//    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
//    SysCtlPeripheralEnable(SYSCTL_PERIPH_QEI1);
//    GPIOPinConfigure(GPIO_PC5_PHA1);
//    GPIOPinTypeQEI(GPIO_PORTC_BASE, GPIO_PIN_5);
//    GPIOPinConfigure(GPIO_PC6_PHB1);
//    GPIOPinTypeQEI(GPIO_PORTC_BASE, GPIO_PIN_6);
//    GPIOPadConfigSet(GPIO_PORTC_BASE, GPIO_PIN_5,
//    GPIO_STRENGTH_4MA,
//    GPIO_PIN_TYPE_STD);
//    GPIOPadConfigSet(GPIO_PORTC_BASE, GPIO_PIN_6,
//    GPIO_STRENGTH_4MA,
//    GPIO_PIN_TYPE_STD);
//    QEIConfigure(QEI1_BASE,(QEI_CONFIG_CAPTURE_A_B | QEI_CONFIG_NO_RESET |
//    QEI_CONFIG_QUADRATURE | QEI_CONFIG_NO_SWAP), 0);
//    QEIVelocityConfigure(QEI1_BASE, QEI_VELDIV_1, SysCtlClockGet()/100);
//    QEIVelocityEnable(QEI1_BASE);
//    QEIEnable(QEI1_BASE);

//    QEI0
//    PD6 A
//    PD7 B
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_QEI0);
    GPIOPinConfigure(GPIO_PD6_PHA0);
    GPIOPinTypeQEI(GPIO_PORTD_BASE, GPIO_PIN_6);
    GPIOPinConfigure(GPIO_PD7_PHB0);
    GPIOPinTypeQEI(GPIO_PORTD_BASE, GPIO_PIN_7);
    GPIOPadConfigSet(GPIO_PORTD_BASE, GPIO_PIN_6,
    GPIO_STRENGTH_4MA,
    GPIO_PIN_TYPE_STD);
    GPIOPadConfigSet(GPIO_PORTD_BASE, GPIO_PIN_7,
    GPIO_STRENGTH_4MA,
    GPIO_PIN_TYPE_STD);
    QEIConfigure(QEI0_BASE,(QEI_CONFIG_CAPTURE_A_B | QEI_CONFIG_NO_RESET |
    QEI_CONFIG_QUADRATURE | QEI_CONFIG_NO_SWAP), 0);
    QEIVelocityConfigure(QEI0_BASE, QEI_VELDIV_1, SysCtlClockGet()/100);
    QEIVelocityEnable(QEI0_BASE);
    QEIEnable(QEI0_BASE);

//    QEI0
//    PF0 A
//    PF1 B
//    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
//    SysCtlPeripheralEnable(SYSCTL_PERIPH_QEI0);
//    GPIOPinConfigure(GPIO_PF0_PHA0);
//    GPIOPinTypeQEI(GPIO_PORTF_BASE, GPIO_PIN_0);
//    GPIOPinConfigure(GPIO_PF1_PHB0);
//    GPIOPinTypeQEI(GPIO_PORTF_BASE, GPIO_PIN_1);
//    GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_0,
//    GPIO_STRENGTH_4MA,
//    GPIO_PIN_TYPE_STD);
//    GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_1,
//    GPIO_STRENGTH_4MA,
//    GPIO_PIN_TYPE_STD);
//    QEIConfigure(QEI0_BASE,(QEI_CONFIG_CAPTURE_A_B | QEI_CONFIG_NO_RESET |
//    QEI_CONFIG_QUADRATURE | QEI_CONFIG_NO_SWAP), 0);
//    QEIVelocityConfigure(QEI0_BASE, QEI_VELDIV_1, SysCtlClockGet()/100);
//    QEIVelocityEnable(QEI0_BASE);
//    QEIEnable(QEI0_BASE);
}


