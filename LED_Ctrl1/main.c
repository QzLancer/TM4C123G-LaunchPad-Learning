#include <stdint.h>
#include <stdbool.h>
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "inc/hw_sysctl.h"
#include "inc/hw_memmap.h"
#include "inc/hw_gpio.h"
#include "driverlib/interrupt.h"
#include "inc/hw_ints.h"
#include "inc/hw_types.h"
//����SW1��ͨ���жϵ�����ɫLED��
void GPIOFIntHandler(void);
int led = 2;
int main(void)
{
    SysCtlClockSet(SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ | SYSCTL_SYSDIV_5);    //ϵͳʱ������

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);    //GPIOFʹ��
    GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_4);   //GPIO��������
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1 |GPIO_PIN_2 |GPIO_PIN_3); //GPIO�������
    GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_4, GPIO_STRENGTH_4MA, GPIO_PIN_TYPE_STD_WPU);  //GPIO����ǿ�Ⱥ�����
//����IO���жϣ�IO�˿��ж�
    GPIOIntTypeSet(GPIO_PORTF_BASE, GPIO_PIN_4, GPIO_LOW_LEVEL);    //GPIOF�����ж�����:�͵�ƽ�ж�
    GPIOIntEnable(GPIO_PORTF_BASE, GPIO_PIN_4);
    IntEnable(INT_GPIOF);   //ʹ��GPIOF�ж�
    IntMasterEnable();  //�����������ж�
    while(1)
    {
        GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3,0x00);
    }
}

void GPIOFIntHandler(void)
{
    GPIOIntClear(GPIO_PORTF_BASE, GPIO_PIN_4);
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, led);
}
