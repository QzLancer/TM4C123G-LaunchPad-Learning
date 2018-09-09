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
//ADC���ԣ�����ΪPE2


int main(void)
{
    uint32_t ADC0_Value = 0;
    SysCtlClockSet(SYSCTL_XTAL_16MHZ | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_SYSDIV_5); //����ϵͳʱ��Ϊ40MHz
    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);   //ʹ��ADC0
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);    //ʹ��E��GPIO
    GPIOPinTypeADC(GPIO_PORTE_BASE, GPIO_PIN_2);    //GPIO���ã�ADC����ΪPE2

    ADCSequenceConfigure(ADC0_BASE, 0, ADC_TRIGGER_PROCESSOR, 0);   //ADC�������ã�ѡ��ADC0�����к�Ϊ0���ɴ��������д��������ȼ������Ϊ0
    ADCSequenceStepConfigure(ADC0_BASE, 0, 0, ADC_CTL_CH1); //ADC�ɼ��������ã��������к�0��ֻ�ɼ�AIN1

    ADCReferenceGet(ADC_REF_EXT_3V); //ADC�ο���ѹ����

    ADCSequenceEnable(ADC0_BASE, 0); //ADC�ɼ�ʹ��
    ADCIntClear(ADC0_BASE, 0);
    ADCIntEnable(ADC0_BASE, 0);

    while(1)
    {
        ADCProcessorTrigger(ADC0_BASE, 0);    //����һ�δ�����ADC�ɼ�
        while(ADCIntStatus(ADC0_BASE, 0, false));
        ADCSequenceDataGet(ADC0_BASE, 0, &ADC0_Value);
    }
}



