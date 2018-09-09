#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/pwm.h"
#include "driverlib/pin_map.h"
#include "driverlib/adc.h"
#include "driverlib/rom.h"
#include "driverlib/interrupt.h"
#include "inc/hw_memmap.h"
#include "inc/hw_gpio.h"
#include "inc/hw_types.h"
//网上的四轴控制程序


int main(void)
{
    PWMPulseWidthSet(PWM1_BASE, PWM_OUT_0,bs004_fly_m3_out);
    PWMPulseWidthSet(PWM1_BASE, PWM_OUT_1,bs004_fly_m4_out);
    PWMPulseWidthSet(PWM0_BASE, PWM_OUT_2,bs004_fly_m1_out);
    PWMPulseWidthSet(PWM0_BASE, PWM_OUT_3,bs004_fly_m2_out);

    //PWM信号初始化：
    //etootle:PWM clock is processor clock /4=80/4=20M
    SysCtlPWMClockSet(SYSCTL_PWMDIV_4);
    //
    //etootle: enable the GPIOB/D clk becaue PB4 and PB5 is used for PWM
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    //
    //etootle:enable PWM modul M0 and M1
    SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM1);
    //
    //etootle:Set GPIO type
    GPIOPadConfigSet(GPIO_PORTB_BASE, GPIO_PIN_4|GPIO_PIN_5,GPIO_STRENGTH_4MA,GPIO_PIN_TYPE_STD);
    GPIOPadConfigSet(GPIO_PORTD_BASE, GPIO_PIN_0|GPIO_PIN_1,GPIO_STRENGTH_4MA,GPIO_PIN_TYPE_STD);
    //
    //etootle: define GPIO to PWM pin
    GPIOPinTypePWM(GPIO_PORTB_BASE, GPIO_PIN_4|GPIO_PIN_5);
    GPIOPinTypePWM(GPIO_PORTD_BASE, GPIO_PIN_0|GPIO_PIN_1);
    //
    //etootle:map GPIO to PWM
    GPIOPinConfigure(GPIO_PB4_M0PWM2);
    GPIOPinConfigure(GPIO_PB5_M0PWM3);
    GPIOPinConfigure(GPIO_PD0_M1PWM0);
    GPIOPinConfigure(GPIO_PD1_M1PWM1);
    //
    //etootle:PWM mode
    PWMGenConfigure(PWM0_BASE, PWM_GEN_1, PWM_GEN_MODE_UP_DOWN | PWM_GEN_MODE_NO_SYNC);
    PWMGenConfigure(PWM1_BASE, PWM_GEN_0, PWM_GEN_MODE_UP_DOWN | PWM_GEN_MODE_NO_SYNC);
    //
    //etootle:PWM period 1000-1=20K
    PWMGenPeriodSet(PWM0_BASE, PWM_GEN_1, 1000);
    PWMGenPeriodSet(PWM1_BASE, PWM_GEN_0, 1000);
    //
    //etootle:invert m2 and m4
    PWMOutputInvert(PWM0_BASE,PWM_OUT_3_BIT, true);
    PWMOutputInvert(PWM1_BASE,PWM_OUT_1_BIT, true);
    //
    //etootle:set pwm dulty cycle
    PWMPulseWidthSet(PWM0_BASE, PWM_OUT_2,0);
    PWMPulseWidthSet(PWM0_BASE, PWM_OUT_3,0);
    PWMPulseWidthSet(PWM1_BASE, PWM_OUT_0,0);
    PWMPulseWidthSet(PWM1_BASE, PWM_OUT_1,0);
    //
    //etootle: enable the PWM counter
    PWMGenEnable(PWM0_BASE, PWM_GEN_1);
    PWMGenEnable(PWM1_BASE, PWM_GEN_0);
    //
    //etootle:enable the pwm output
    PWMOutputState(PWM0_BASE,PWM_OUT_2_BIT|PWM_OUT_3_BIT, true);
    PWMOutputState(PWM1_BASE,PWM_OUT_0_BIT|PWM_OUT_1_BIT, true);
    while(1)
    {

    }
}
