#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/uart.h"
#include "driverlib/interrupt.h"
#include "inc/hw_memmap.h"
#include "inc/hw_gpio.h"
#include "inc/hw_types.h"
#include "inc/hw_ints.h"

//采用中断的方式读取字符串的接收

void UARTStringPut(uint32_t ui32Base, char *UART_Input);
void UART0IntHandler(void);
char a;

int main(void)
{

    SysCtlClockSet(SYSCTL_XTAL_16MHZ | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_SYSDIV_5); //配置系统时钟为40MHz；

    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);

    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
    UARTConfigSetExpClk(UART0_BASE,  SysCtlClockGet() , 115200,  UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_ONE);

    UARTIntEnable(UART0_BASE, UART_INT_RX);
    IntEnable(INT_UART0);

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

void UART0IntHandler(void)
{
    UARTIntClear(UART0_BASE, UART_INT_RX);
    UARTCharPut(UART0_BASE, UARTCharGet(UART0_BASE));
}
