#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/adc.h"
#include "driverlib/pin_map.h"
#include "driverlib/interrupt.h"
#include "inc/hw_memmap.h"
#include "inc/hw_gpio.h"
#include "inc/hw_types.h"
#include "inc/hw_ints.h"
//ADC测试，引脚为PE2


int main(void)
{
    uint32_t ADC0_Value = 0;
    SysCtlClockSet(SYSCTL_XTAL_16MHZ | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_SYSDIV_5); //配置系统时钟为40MHz
    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);   //使能ADC0
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);    //使能E组GPIO
    GPIOPinTypeADC(GPIO_PORTE_BASE, GPIO_PIN_2);    //GPIO配置，ADC采样为PE2

    ADCSequenceConfigure(ADC0_BASE, 0, ADC_TRIGGER_PROCESSOR, 0);   //ADC序列配置：选择ADC0，序列号为0，由处理器进行触发，优先级最高设为0
    ADCSequenceStepConfigure(ADC0_BASE, 0, 0, ADC_CTL_CH1); //ADC采集步序配置：采样序列号0，只采集AIN1

    ADCReferenceGet(ADC_REF_EXT_3V); //ADC参考电压设置

    ADCSequenceEnable(ADC0_BASE, 0); //ADC采集使能
    ADCIntClear(ADC0_BASE, 0);
    ADCIntEnable(ADC0_BASE, 0);

    while(1)
    {
        ADCProcessorTrigger(ADC0_BASE, 0);    //进行一次处理器ADC采集
        while(ADCIntStatus(ADC0_BASE, 0, false));
        ADCSequenceDataGet(ADC0_BASE, 0, &ADC0_Value);
    }
}



