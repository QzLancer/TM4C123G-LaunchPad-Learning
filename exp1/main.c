#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "driverlib/debug.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"

/**
 * main.c
 */
int main(void)
{
    uint8_t u18PinData=0x02;
    int32_t PinRead;
    SysCtlClockSet(SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ | SYSCTL_SYSDIV_5); //主频被PLL倍频到400MHz,通过二分频到200MHz，系统分频器5分频，系统时钟40MHz
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);    //使能外设
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE,GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);
    GPIOPinTypeGPIOInput(GPIO_PORTF_BASE,GPIO_PIN_0);
    while(1)
    {
        PinRead=GPIOPinRead(GPIO_PORTF_BASE,GPIO_PIN_0);

    }
}

