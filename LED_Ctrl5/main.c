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
//�ж�Ƕ�׳��򣬰���SW1��ͨ����ʱ���жϺ��ⲿ�ж�ʹLED����ɫ��1sÿ�β��ϱ仯
void GPIOFIntHandler(void);
void TIMER0IntHandler(void);

uint8_t led = 2;
int led_flag = 0;

int main(void)
{
    SysCtlClockSet(SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ | SYSCTL_SYSDIV_5);    //ϵͳʱ������

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);    //GPIOFʹ��
    GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_4);   //GPIO��������
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1 |GPIO_PIN_2 |GPIO_PIN_3); //GPIO�������
    GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_4, GPIO_STRENGTH_4MA, GPIO_PIN_TYPE_STD_WPU);  //GPIO����ǿ�Ⱥ�����

    GPIOIntTypeSet(GPIO_PORTF_BASE, GPIO_PIN_4, GPIO_LOW_LEVEL);    //GPIOF�����ж�����:�͵�ƽ�ж�
    GPIOIntEnable(GPIO_PORTF_BASE, GPIO_PIN_4);
    IntEnable(INT_GPIOF);   //ʹ��GPIOF�ж�

    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);   //TIMER0ʹ��
    TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);    //TIMER0����
    TimerLoadSet(TIMER0_BASE, TIMER_A, SysCtlClockGet()/2-1);   //TIMER0װ��

    IntPrioritySet(INT_GPIOF, 0x01);
    IntPrioritySet(INT_TIMER0A, 0x00);    //�ж����ȼ�����

    TimerIntEnable(TIMER0_BASE,TIMER_TIMA_TIMEOUT);   //���ö�ʱ��0�ж�
    IntEnable(INT_TIMER0A); //ʹ�ܶ�ʱ��0�ж�
    while(1)
    {
//        led=2;
//        flag=0;
        GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3,0x00);
    }
}

void GPIOFIntHandler(void)
{
    TimerEnable(TIMER0_BASE,TIMER_A);   //��ʱ����ʼ������ʼ
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
