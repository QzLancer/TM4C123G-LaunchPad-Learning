#include <stdint.h>
#include <stdbool.h>
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/timer.h"
#include "inc/hw_sysctl.h"
#include "inc/hw_memmap.h"
#include "inc/hw_gpio.h"
#include "driverlib/interrupt.h"
#include "inc/hw_ints.h"
#include "inc/hw_types.h"
//中断嵌套程序，按下SW1后，通过定时器中断和外部中断使LED灯颜色以1s每次不断变化
void GPIOFIntHandler(void);
void TIMER0IntHandler(void);

uint8_t led = 2;
int led_flag = 0;

int main(void)
{
    SysCtlClockSet(SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ | SYSCTL_SYSDIV_5);    //系统时钟设置

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);    //GPIOF使能
    GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_4);   //GPIO输入配置
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1 |GPIO_PIN_2 |GPIO_PIN_3); //GPIO输出配置
    GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_4, GPIO_STRENGTH_4MA, GPIO_PIN_TYPE_STD_WPU);  //GPIO驱动强度和类型

    GPIOIntTypeSet(GPIO_PORTF_BASE, GPIO_PIN_4, GPIO_LOW_LEVEL);    //GPIOF输入中断配置:低电平中断
    GPIOIntEnable(GPIO_PORTF_BASE, GPIO_PIN_4);
    IntEnable(INT_GPIOF);   //使能GPIOF中断

    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);   //TIMER0使能
    TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);    //TIMER0配置
    TimerLoadSet(TIMER0_BASE, TIMER_A, SysCtlClockGet()/2-1);   //TIMER0装载

    IntPrioritySet(INT_GPIOF, 0x01);
    IntPrioritySet(INT_TIMER0A, 0x00);    //中断优先级设置

    TimerIntEnable(TIMER0_BASE,TIMER_TIMA_TIMEOUT);   //配置定时器0中断
    IntEnable(INT_TIMER0A); //使能定时器0中断
    while(1)
    {
//        led=2;
//        flag=0;
        GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3,0x00);
    }
}

void GPIOFIntHandler(void)
{
    TimerEnable(TIMER0_BASE,TIMER_A);   //定时器开始工作开始
    GPIOIntClear(GPIO_PORTF_BASE, GPIO_PIN_4);
}

void TIMER0IntHandler(void)
{
    TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
    TimerDisable(TIMER0_BASE,TIMER_A);
    GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3,led);
    if((led <= 0x0e)&(led_flag == 0))
    {
        led++;
        if(led==0x0e)
            led_flag = 1;
    }
    else if((led >= 0x02)&(led_flag==1))
    {
        led--;
        if(led==0x02)
            led_flag = 0;
    }
}
