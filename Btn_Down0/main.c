#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/interrupt.h"
#include "inc/hw_memmap.h"
#include "inc/hw_gpio.h"
#include "inc/hw_types.h"
//按键按下，LED状态切换
bool IfButtonDown(uint32_t ui32Port, uint8_t ui8Pins);

int main(void)
{
    int led=0;
    int button=0;
    SysCtlClockSet(SYSCTL_XTAL_16MHZ | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_SYSDIV_5); //配置系统时钟为40MHz

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);    //使能A组GPIO
    GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE, GPIO_PIN_4);  //配置PF4为输出
    GPIOPinTypeGPIOInput(GPIO_PORTA_BASE, GPIO_PIN_2 | GPIO_PIN_3);    //配置PA2和FA3为输入
    GPIOPadConfigSet(GPIO_PORTA_BASE, GPIO_PIN_2 | GPIO_PIN_3, GPIO_STRENGTH_4MA, GPIO_PIN_TYPE_STD_WPU);

    while(1)
    {

        if((button==0)&(IfButtonDown(GPIO_PORTA_BASE,GPIO_PIN_2)==1))
        {
            button=1;
            if(led==0)
                led=0x10;
            else
                led=0;
        }
        else if(IfButtonDown(GPIO_PORTA_BASE,GPIO_PIN_2)==0)
        {
            button=0;
        }
        GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_4, led);
    }
}

bool IfButtonDown(uint32_t ui32Port, uint8_t ui8Pins)
{
    int flag=0;
    if((flag==0)&((GPIOPinRead(ui32Port, ui8Pins)&&ui8Pins)==0)) //如果按键按下，标志位没有置位
        {
            SysCtlDelay(SysCtlClockGet()/100-1); //延时
            flag=1; //标志位置位
            if((flag==1)&((GPIOPinRead(ui32Port, ui8Pins)&&ui8Pins)==0)) //如果按键按下，标志位置位
            {
                //while(((GPIOPinRead(GPIO_PORTA_BASE, GPIO_PIN_2)&&0x04)==0x04));    //等待按键松开
                return 1;   //确认按键被按下
            }
            else
                return 0;   //否则确认按键不被按下
        }
    else
        return 0;
}
