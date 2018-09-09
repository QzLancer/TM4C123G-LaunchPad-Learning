#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/pwm.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/interrupt.h"
#include "inc/hw_memmap.h"
#include "inc/hw_gpio.h"
#include "inc/hw_types.h"
//通过PWM发生器产生50kHz的互补路PWM波，死区时间，占空比通过程序调节为25%
//产生互补的的引脚，目前有三种猜测，一是同一个PWM主模块，同一生成模块不同路PWM互补，二是不同PWM主模块，同一生成模块同路互补，三是同一个PWM主模块，同一生成模块不同路PWM同步，
//但其中一路需要通过PWMOutputInvert()取反
//本程序验证猜想1和猜想3
//使用的引脚为PF2和PF3

int main(void)z{
    uint32_t Period = 50000;    //周期
    uint32_t PulseWid = Period*4;  //脉宽
    uint32_t DeadBand = Period*4*20;    //死区

    SysCtlClockSet(SYSCTL_XTAL_16MHZ | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_SYSDIV_5); //配置系统时钟为40MHz
    SysCtlPWMClockSet(SYSCTL_PWMDIV_1);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);    //使能F组GPIO
    SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM1);    //使能PWM主模块1

    GPIOPinConfigure(GPIO_PF3_M1PWM7);
    GPIOPinConfigure(GPIO_PF2_M1PWM6);
    GPIOPinTypePWM(GPIO_PORTF_BASE, GPIO_PIN_2 | GPIO_PIN_3);   //GPIO配置，使用PWM功能前必须先调用GPIOPinConfigure()

    PWMGenConfigure(PWM1_BASE, PWM_GEN_3, PWM_GEN_MODE_UP_DOWN | PWM_GEN_MODE_NO_SYNC | PWM_GEN_MODE_DB_NO_SYNC);    //PWM发生器功能配置
    PWMGenPeriodSet(PWM1_BASE, PWM_GEN_3, SysCtlClockGet()/Period); //PWM发生器周期配置
    PWMPulseWidthSet(PWM1_BASE, PWM_OUT_6, SysCtlClockGet()/PulseWid);    //PWM发生器脉宽配置
    PWMPulseWidthSet(PWM1_BASE, PWM_OUT_7, SysCtlClockGet()/PulseWid);    //PWM发生器脉宽配置，如两个PWM同步，尝试备注此段
    PWMDeadBandEnable(PWM1_BASE, PWM_GEN_3, SysCtlClockGet()/DeadBand, SysCtlClockGet()/DeadBand);    //死区配置，参照《Tiva原理与应用 10.3.5》

    //PWMOutputInvert(PWM1_BASE,PWM_OUT_7_BIT, true)   //如不能正常工作，尝试取消此条备注
    PWMOutputState(PWM1_BASE, PWM_OUT_6_BIT | PWM_OUT_7_BIT, true); //PWM输出口状态
    PWMGenEnable(PWM1_BASE, PWM_GEN_3); //PWM发生器使能

    while(1)
    {

    }
}
