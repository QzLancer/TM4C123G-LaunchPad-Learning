#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "inc/hw_gpio.h"
#include "driverlib/pin_map.h "
//当按下SW1时，LED灯点亮；松开时熄灭
int main(void)
{
    SysCtlClockSet(SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ | SYSCTL_SYSDIV_5);     //系统时钟设置
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);   //GPIOF使能
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_2);  //GPIOF输出配置，蓝色LED灯
    GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_4);      //GPIOF输入配置，SW1
    GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_4, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);    //引脚功能配置输入要配置弱上拉
    while(1)
    {
        if(GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4)&&GPIO_PIN_4)
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 0x04);
        else
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 0x00);
    }
}
