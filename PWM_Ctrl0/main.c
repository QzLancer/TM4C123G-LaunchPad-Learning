#include <stdint.h>
#include <stdbool.h>
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/pwm.h"
#include "driverlib/pin_map.h"
#include "inc/hw_memmap.h"
#include "inc/hw_gpio.h"
#include "inc/hw_types.h"
//通过PWM发生器产生100kHz的PWM波，控制拓展版灯的亮度
//配置好寄存器即可自动产生PWM波，不用占用CPU资源


int main(void)
{
    SysCtlClockSet(SYSCTL_XTAL_16MHZ | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_SYSDIV_5); //配置系统时钟为40MHz
    SysCtlPWMClockSet(SYSCTL_PWMDIV_1);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);    //使能F组GPIO
    SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM1);    //使能PWM发生器1

    GPIOPinConfigure(GPIO_PF3_M1PWM7);
    GPIOPinTypePWM(GPIO_PORTF_BASE, GPIO_PIN_3);    //GPIO配置，使用PWM功能前必须先调用GPIOPinConfigure()

    PWMGenConfigure(PWM1_BASE, PWM_GEN_3, PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC);    //PWM发生器功能配置
    PWMGenPeriodSet(PWM1_BASE, PWM_GEN_3, 800); //PWM发生器周期配置
    PWMPulseWidthSet(PWM1_BASE, PWM_OUT_7, 400);    //PWM发生器脉宽配置

    PWMOutputState(PWM1_BASE, PWM_OUT_7_BIT, true); //PWM输出口状态
    PWMGenEnable(PWM1_BASE, PWM_GEN_3); //PWM发生器使能

    while(1)
    {

    }
}
