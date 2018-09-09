#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/uart.h"
#include "inc/hw_memmap.h"
#include "inc/hw_gpio.h"
#include "inc/hw_types.h"
//字符串发送

void UARTStringPut(uint32_t ui32Base, char *UART_Input);

int main(void)
{
    SysCtlClockSet(SYSCTL_XTAL_16MHZ | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_SYSDIV_5); //配置系统时钟为40MHz；

    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);

    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
    UARTConfigSetExpClk(UART0_BASE,  SysCtlClockGet() , 115200,  UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_ONE);

    UARTEnable(UART0_BASE);
    UARTStringPut(UART0_BASE, "Finish");
    while(1)
    {

    }

}

void UARTStringPut(uint32_t ui32Base, char *UART_Input)
{
    int i=0;
    while(*(UART_Input+i)!='\0')
    {
        UARTCharPut(ui32Base, *(UART_Input+i));
        i++;
    }
}
