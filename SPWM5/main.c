#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/pwm.h"
#include "driverlib/pin_map.h"
#include "driverlib/adc.h"
#include "driverlib/interrupt.h"
#include "driverlib/timer.h"
#include "driverlib/rom.h"
#include "driverlib/interrupt.h"
#include "inc/hw_memmap.h"
#include "inc/hw_gpio.h"
#include "inc/hw_types.h"
#include "inc/hw_ints.h"
//通过PWM发生器产生50kHz的互补SPWM波，死区时间为1/20脉宽，信号波周期50Hz,通过拓展版上的K3(PA3)和K4(PA2)按键来调节正弦波的频率，为20Hz到100Hz之间
//使用的引脚为PF2和PF3
bool ButtonDown(uint32_t ui32Port, uint8_t ui8Pins);

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
                          96,100,104,108,112,116,120,124
    };
    int frequency = 50; //初始频率为50Hz
    uint32_t Period = 50000;    //载波周期
    uint32_t PulseWid = 25000;  //脉宽





int main(void)
{
    int IfButtonK3Down = 0; //K3按下标志位
    int IfButtonK4Down = 0; //K4按下标志位
    uint32_t DeadBand = PulseWid*20;    //死区
    SysCtlClockSet(SYSCTL_XTAL_16MHZ | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_SYSDIV_2_5); //配置系统时钟为80MHz
    SysCtlPWMClockSet(SYSCTL_PWMDIV_1);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);    //使能F组GPIO
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);    //使能F组GPIO
    SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM1);    //使能PWM主模块1
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);  //使能定时器0


    GPIOPinTypeGPIOInput(GPIO_PORTA_BASE, GPIO_PIN_2 | GPIO_PIN_3);    //配置PA2和FA3为输入
    GPIOPadConfigSet(GPIO_PORTA_BASE, GPIO_PIN_2 | GPIO_PIN_3, GPIO_STRENGTH_4MA, GPIO_PIN_TYPE_STD_WPU);   //弱上拉输入

    GPIOPinConfigure(GPIO_PF3_M1PWM7);
    GPIOPinConfigure(GPIO_PF2_M1PWM6);
    GPIOPinTypePWM(GPIO_PORTF_BASE, GPIO_PIN_2 | GPIO_PIN_3);   //GPIO配置，使用PWM功能前必须先调用GPIOPinConfigure()

    TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);    //定时器0配置
    TimerLoadSet(TIMER0_BASE, TIMER_A, SysCtlClockGet()/(frequency*200)-1);   //定时器0装载

    PWMGenConfigure(PWM1_BASE, PWM_GEN_3, PWM_GEN_MODE_UP_DOWN | PWM_GEN_MODE_NO_SYNC | PWM_GEN_MODE_DB_NO_SYNC);    //PWM发生器功能配置
    PWMGenPeriodSet(PWM1_BASE, PWM_GEN_3, SysCtlClockGet()/Period); //PWM发生器周期配置
    PWMPulseWidthSet(PWM1_BASE, PWM_OUT_6, SysCtlClockGet()/PulseWid);    //PWM发生器脉宽配置
    PWMDeadBandEnable(PWM1_BASE, PWM_GEN_3, SysCtlClockGet()/DeadBand, SysCtlClockGet()/DeadBand);    //死区配置，参照《Tiva原理与应用 10.3.5》

    IntPrioritySet(INT_TIMER0A, 0x00);    //中断优先级设置

    PWMOutputState(PWM1_BASE, PWM_OUT_6_BIT | PWM_OUT_7_BIT, true); //PWM输出口状态
    PWMGenEnable(PWM1_BASE, PWM_GEN_3); //PWM发生器使能
    TimerIntEnable(TIMER0_BASE,TIMER_TIMA_TIMEOUT);   //配置定时器0中断
    IntEnable(INT_TIMER0A); //使能定时器0中断
    TimerEnable(TIMER0_BASE,TIMER_A);

    while(1)
    {
        if((IfButtonK3Down==0)&(IfButtonK4Down==0)&(ButtonDown(GPIO_PORTA_BASE, GPIO_PIN_3)|ButtonDown(GPIO_PORTA_BASE, GPIO_PIN_2)))
        {
            if(ButtonDown(GPIO_PORTA_BASE, GPIO_PIN_3))
            {
                IfButtonK3Down=1;
                if(frequency>=20)
                    frequency--;
                    Period = frequency*100;
                TimerLoadSet(TIMER0_BASE, TIMER_A, SysCtlClockGet()/(frequency*200)-1);
            }
            else if(ButtonDown(GPIO_PORTA_BASE, GPIO_PIN_2))
            {
                IfButtonK4Down=1;
                if(frequency<=100)
                    frequency++;
                    Period = frequency*100;
                TimerLoadSet(TIMER0_BASE, TIMER_A, SysCtlClockGet()/(frequency*200)-1);
            }
        }
        if(ButtonDown(GPIO_PORTA_BASE, GPIO_PIN_3)==0)
            IfButtonK3Down=0;
        if(ButtonDown(GPIO_PORTA_BASE, GPIO_PIN_2)==0)
            IfButtonK4Down=0;

    }
}

bool ButtonDown(uint32_t ui32Port, uint8_t ui8Pins)
{
    int flag=0;
    if((flag==0)&((GPIOPinRead(ui32Port, ui8Pins)&&ui8Pins)==0)) //如果按键按下，标志位没有置位
        {
            SysCtlDelay(SysCtlClockGet()/100-1); //延时
            flag=1; //标志位置位
            if((flag==1)&((GPIOPinRead(ui32Port, ui8Pins)&&ui8Pins)==0)) //如果按键按下，标志位置位
            {
                //while(((GPIOPinRead(GPIO_PORTA_BASE, GPIO_PIN_2)&&ui8Pins)==0x04));    //等待按键松开
                return 1;   //确认按键被按下
            }
            else
                return 0;   //否则确认按键不被按下
        }
    else
        return 0;
}

void TIMER0IntHandler(void)
{
    TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
    if(i<100)
    {
        Period = frequency*1000;
        PulseWid = Period*(256/SPWMData[i]);
        PWMGenPeriodSet(PWM1_BASE, PWM_GEN_3, SysCtlClockGet()/Period); //PWM发生器周期配置
        PWMPulseWidthSet(PWM1_BASE, PWM_OUT_6, SysCtlClockGet()/(Period*256/SPWMData[i]));
        i++;
    }
    else if((i>=100)&(i<200))
    {
        Period = frequency*1000;
        PulseWid = -Period*(256/SPWMData[i]);
        PWMGenPeriodSet(PWM1_BASE, PWM_GEN_3, SysCtlClockGet()/Period); //PWM发生器周期配置
        PWMPulseWidthSet(PWM1_BASE, PWM_OUT_6, SysCtlClockGet()/(Period*256/SPWMData[i]));
        i++;
    }
    else if(i>=200)
    {
        Period = frequency*1000;
        PulseWid = Period*(256/SPWMData[i]);
        PWMGenPeriodSet(PWM1_BASE, PWM_GEN_3, SysCtlClockGet()/Period); //PWM发生器周期配置
        PWMPulseWidthSet(PWM1_BASE, PWM_OUT_6, SysCtlClockGet()/(Period*256/SPWMData[0]));
        i=1;
    }
}


