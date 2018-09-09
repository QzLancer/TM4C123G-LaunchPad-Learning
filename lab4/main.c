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

	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);    //GPIOFʹ��
	GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 |GPIO_PIN_3);  //GPIO�������

	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);    //��ʱ��0ʹ��
	TimerConfigure(TIMER0_BASE,TIMER_CFG_PERIODIC);     //��ʱ��0����

	TimerLoadSet(TIMER0_BASE,TIMER_A,SysCtlClockGet()-1); //��ʱ���Ĳ������ã�������������Ƶ�ʣ�����
	IntEnable(INT_TIMER0A);   //ʹ�ܶ�ʱ��0A�ж�
	TimerIntEnable(TIMER0_BASE,TIMER_TIMA_TIMEOUT);   //���ö�ʱ��0�жϣ�TIME0A��ʱ�ж�
	IntMasterEnable();   //�����������ж�




	TimerEnable(TIMER0_BASE,TIMER_A);  //������ʼ
	while(1)
	{


	}
}

void Timer0IntHandler(void)
{
    TimerIntClear(TIMER0_BASE,TIMER_TIMA_TIMEOUT); //�жϱ�־λ���;
    if(GPIOPinRead(GPIO_PORTF_BASE,GPIO_PIN_2))
    {
        GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3,0x00);
    }
    else
    {
        GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_2,0x04);
    }
}
