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


        GPIODirModeSet(GPIO_PORTF_BASE, GPIO_PIN_4, GPIO_DIR_MODE_IN);      /*设置连接KEY的PE0为输入*/

    GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_4,              /* 设置KEY的驱动强度和类型 */
                     GPIO_STRENGTH_4MA,                       /*4mA的输出驱动强度*/
                     GPIO_PIN_TYPE_STD_WPU);          /*  设置LEY端口为弱上拉    */


    GPIOIntTypeSet(GPIO_PORTF_BASE, GPIO_PIN_4, GPIO_RISING_EDGE);  /*设置KEY中断的触发方式为低电平触发 */
    GPIOIntEnable(GPIO_PORTF_BASE, GPIO_PIN_4);             /*  使能KEY中断         */
    IntEnable(INT_GPIOF);                                               /*  使能GPIO PE 口中断          */


    while (1)
        {
      // GPIOPinWrite(GPIO_PORTF_BASE, LED1, LED1);                  //点亮LED1
    }

}
