#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "inc/hw_gpio.h"
#include "driverlib/pin_map.h "
//������SW1ʱ��LED�Ƶ������ɿ�ʱϨ��
int main(void)
{
    SysCtlClockSet(SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ | SYSCTL_SYSDIV_5);     //ϵͳʱ������
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);   //GPIOFʹ��
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_2);  //GPIOF������ã���ɫLED��
    GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_4);      //GPIOF�������ã�SW1
    GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_4, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);    //���Ź�����������Ҫ����������
    while(1)
    {
        if(GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4)&&GPIO_PIN_4)
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 0x04);
        else
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 0x00);
    }
}
