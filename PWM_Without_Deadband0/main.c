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
//ͨ��PWM����������50kHz��һ·PWM��������ʱ�䣬ռ�ձ�ͨ���������Ϊ25%
//PF3

int main(void)
{
    uint32_t Period = 50000;    //����
    uint32_t PulseWid = Period*4;  //����

    SysCtlClockSet(SYSCTL_XTAL_16MHZ | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_SYSDIV_5); //����ϵͳʱ��Ϊ40MHz
    SysCtlPWMClockSet(SYSCTL_PWMDIV_1);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);    //ʹ��F��GPIO
    SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM1);    //ʹ��PWM��ģ��1

    GPIOPinConfigure(GPIO_PF3_M1PWM7);
    GPIOPinTypePWM(GPIO_PORTF_BASE, GPIO_PIN_3);   //GPIO���ã�ʹ��PWM����ǰ�����ȵ���GPIOPinConfigure()

    PWMGenConfigure(PWM1_BASE, PWM_GEN_3, PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC | PWM_GEN_MODE_DB_NO_SYNC);    //PWM��������������
    PWMGenPeriodSet(PWM1_BASE, PWM_GEN_3, SysCtlClockGet()/Period); //PWM��������������
    PWMPulseWidthSet(PWM1_BASE, PWM_OUT_7, SysCtlClockGet()/PulseWid);    //PWM��������������

    PWMOutputState(PWM1_BASE, PWM_OUT_7_BIT, true); //PWM�����״̬
    PWMGenEnable(PWM1_BASE, PWM_GEN_3); //PWM������ʹ��

    while(1)
    {

    }
}