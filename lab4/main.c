#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/debug.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"
#include "driverlib/timer.h"

/**
 * main.c
 */

int main(void)
{

	SysCtlClockSet(SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ | SYSCTL_SYSDIV_5);

	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);    //GPIOF使能
	GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 |GPIO_PIN_3);  //GPIO输出配置

	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);    //定时器0使能
	TimerConfigure(TIMER0_BASE,TIMER_CFG_PERIODIC);     //定时器0配置

	TimerLoadSet(TIMER0_BASE,TIMER_A,SysCtlClockGet()-1); //计时器的参数设置（第三个参数填频率？？）
	IntEnable(INT_TIMER0A);   //使能定时器0A中断
	TimerIntEnable(TIMER0_BASE,TIMER_TIMA_TIMEOUT);   //配置定时器0中断，TIME0A超时中断
	IntMasterEnable();   //开启处理器中断




	TimerEnable(TIMER0_BASE,TIMER_A);  //计数开始
	while(1)
	{


	}
}

void Timer0IntHandler(void)
{
    TimerIntClear(TIMER0_BASE,TIMER_TIMA_TIMEOUT); //中断标志位清除;
    if(GPIOPinRead(GPIO_PORTF_BASE,GPIO_PIN_2))
    {
        GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3,0x00);
    }
    else
    {
        GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_2,0x04);
    }
}
