#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/pwm.h"
#include "driverlib/pin_map.h"
#include "driverlib/interrupt.h"
#include "driverlib/timer.h"
#include "driverlib/rom.h"
#include "driverlib/interrupt.h"
#include "inc/hw_memmap.h"
#include "inc/hw_gpio.h"
#include "inc/hw_types.h"
#include "inc/hw_ints.h"
//ͨ��PWM����������50kHz�Ļ���SPWM��������ʱ��Ϊ1/20�����ز�����50Hz
//ʹ�õ�����ΪPF2��PF3
    uint32_t Period = 50000;    //����
    int i=0;
    int SPWMData[200] = {128,131,135,139,143,147,151,155,
                          159,163,167,171,174,178,182,185,
                          189,192,196,199,202,205,208,211,
                          214,217,220,223,225,228,230,233,
                          235,237,239,241,242,244,246,247,
                          248,249,251,251,252,253,253,254,
                          254,254,255,254,254,254,253,253,
                          252,251,251,249,248,247,246,244,
                          242,241,239,237,235,233,230,228,
                          225,223,220,217,214,211,208,205,
                          202,199,196,192,189,185,182,178,
                          174,171,167,163,159,155,151,147,
                          143,139,135,131,127,124,120,116,
                          112,108,104,100,96,92,88,84,
                          81,77,73,70,66,63,59,56,
                          53,50,47,44,41,38,35,32,
                          30,27,25,22,20,18,16,14,
                          13,11,9,8,7,6,4,4,
                          3,2,2,1,1,1,1,1,
                          1,1,2,2,3,4,4,6,
                          7,8,9,11,13,14,16,18,
                          20,22,25,27,30,32,35,38,
                          41,44,47,50,53,56,59,63,
                          66,70,73,77,81,84,88,92,
                          96,100,104,108,112,116,120,124,
    };



int main(void)
{

    uint32_t PulseWid = Period*(256/SPWMData[i]);  //����
    uint32_t DeadBand = PulseWid*20;    //����
    SysCtlClockSet(SYSCTL_XTAL_16MHZ | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_SYSDIV_2_5); //����ϵͳʱ��Ϊ80MHz
    SysCtlPWMClockSet(SYSCTL_PWMDIV_1);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);    //ʹ��F��GPIO
    SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM1);    //ʹ��PWM��ģ��1
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);  //ʹ�ܶ�ʱ��0

    GPIOPinConfigure(GPIO_PF3_M1PWM7);
    GPIOPinConfigure(GPIO_PF2_M1PWM6);
    GPIOPinTypePWM(GPIO_PORTF_BASE, GPIO_PIN_2 | GPIO_PIN_3);   //GPIO���ã�ʹ��PWM����ǰ�����ȵ���GPIOPinConfigure()

    TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);    //��ʱ��0����
    TimerLoadSet(TIMER0_BASE, TIMER_A, SysCtlClockGet()/10000-1);   //��ʱ��0װ��


    PWMGenConfigure(PWM1_BASE, PWM_GEN_3, PWM_GEN_MODE_UP_DOWN | PWM_GEN_MODE_NO_SYNC | PWM_GEN_MODE_DB_NO_SYNC);    //PWM��������������
    PWMGenPeriodSet(PWM1_BASE, PWM_GEN_3, SysCtlClockGet()/Period); //PWM��������������
    PWMPulseWidthSet(PWM1_BASE, PWM_OUT_6, SysCtlClockGet()/PulseWid);    //PWM��������������
    PWMDeadBandEnable(PWM1_BASE, PWM_GEN_3, SysCtlClockGet()/DeadBand, SysCtlClockGet()/DeadBand);    //�������ã����ա�Tivaԭ����Ӧ�� 10.3.5��

    IntPrioritySet(INT_TIMER0A, 0x00);    //�ж����ȼ�����

    PWMOutputState(PWM1_BASE, PWM_OUT_6_BIT | PWM_OUT_7_BIT, true); //PWM�����״̬
    PWMGenEnable(PWM1_BASE, PWM_GEN_3); //PWM������ʹ��
    TimerIntEnable(TIMER0_BASE,TIMER_TIMA_TIMEOUT);   //���ö�ʱ��0�ж�
    IntEnable(INT_TIMER0A); //ʹ�ܶ�ʱ��0�ж�
    TimerEnable(TIMER0_BASE,TIMER_A);

    while(1)
    {

    }
}

void TIMER0IntHandler(void)
{
    TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
    if(i<=199)
    {
        PWMPulseWidthSet(PWM1_BASE, PWM_OUT_6, SysCtlClockGet()/(Period*256/SPWMData[i]));
        i++;
    }
    else if(i<=200)
    {
        PWMPulseWidthSet(PWM1_BASE, PWM_OUT_6, SysCtlClockGet()/(Period*256/SPWMData[0]));
        i=0;
    }
}

