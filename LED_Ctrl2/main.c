#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "driverlib/systick.h"
unsigned char led=2;


void GPIO_Port_F_ISR(void)
{
    GPIOIntClear(GPIO_PORTF_BASE, GPIO_PIN_4);
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, led);
    led=2*led;
}


int main(void)
{

        SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
        GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 0);


        GPIODirModeSet(GPIO_PORTF_BASE, GPIO_PIN_4, GPIO_DIR_MODE_IN);      /*��������KEY��PE0Ϊ����*/

    GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_4,              /* ����KEY������ǿ�Ⱥ����� */
                     GPIO_STRENGTH_4MA,                       /*4mA���������ǿ��*/
                     GPIO_PIN_TYPE_STD_WPU);          /*  ����LEY�˿�Ϊ������    */


    GPIOIntTypeSet(GPIO_PORTF_BASE, GPIO_PIN_4, GPIO_RISING_EDGE);  /*����KEY�жϵĴ�����ʽΪ�͵�ƽ���� */
    GPIOIntEnable(GPIO_PORTF_BASE, GPIO_PIN_4);             /*  ʹ��KEY�ж�         */
    IntEnable(INT_GPIOF);                                               /*  ʹ��GPIO PE ���ж�          */


    while (1)
        {
      // GPIOPinWrite(GPIO_PORTF_BASE, LED1, LED1);                  //����LED1
    }

}
