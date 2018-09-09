#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/pwm.h"
#include "driverlib/pin_map.h"
#include "driverlib/adc.h"
#include "driverlib/rom.h"
#include "driverlib/interrupt.h"
#include "inc/hw_memmap.h"
#include "inc/hw_gpio.h"
#include "inc/hw_types.h"
//ͨ��PWM����������100kHz��PWM�����ı��λ����������չ��Ƶ�����
//���úüĴ��������Զ�����PWM��������ռ��CPU��Դ
//ADC�ɼ�����Ҫ��ʹ�ܲɼ����У��ٽ��вɼ���������ʽ�����ɴ�����Ҳ������������ʽ

#ifdef DEBUG
void
__error__(char *pcFilename, unsigned long ulLine)
{
}
#endif

int main(void)
{
    uint32_t ADC0_Value=0;
    uint32_t PWM_Width=0;
    SysCtlClockSet(SYSCTL_XTAL_16MHZ | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_SYSDIV_5); //����ϵͳʱ��Ϊ40MHz
    SysCtlPWMClockSet(SYSCTL_PWMDIV_1);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);    //ʹ��F��GPIO
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);    //ʹ��E��GPIO
    SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM1);    //ʹ��PWM������1
    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);   //ʹ��ADC0

    GPIOPinConfigure(GPIO_PF3_M1PWM7);
    GPIOPinTypePWM(GPIO_PORTF_BASE, GPIO_PIN_3);    //GPIO���ã�ʹ��PWM����ǰ�����ȵ���GPIOPinConfigure()
    GPIOPinTypeADC(GPIO_PORTE_BASE, GPIO_PIN_1);    //GPIO���ã�ADC����ΪPE1


    ADCSequenceConfigure(ADC0_BASE, 0, ADC_TRIGGER_PROCESSOR, 0);   //ADC�������ã�ѡ��ADC0�����к�Ϊ0���ɴ��������д��������ȼ������Ϊ0
    ADCSequenceStepConfigure(ADC0_BASE, 0, 0, ADC_CTL_CH2); //ADC�ɼ��������ã��������к�0��ֻ�ɼ�AIN2

    ADCReferenceGet(ADC_REF_EXT_3V); //ADC�ο���ѹ����

    PWMGenConfigure(PWM1_BASE, PWM_GEN_3, PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC);    //PWM��������������
    PWMGenPeriodSet(PWM1_BASE, PWM_GEN_3, 800); //PWM��������������
    PWMPulseWidthSet(PWM1_BASE, PWM_OUT_7, 100);    //PWM��������������

    PWMOutputState(PWM1_BASE, PWM_OUT_7_BIT, true); //PWM�����״̬
    PWMGenEnable(PWM1_BASE, PWM_GEN_3); //PWM������ʹ��
    ADCSequenceEnable(ADC0_BASE, 0); //ADC�ɼ�ʹ��
    ADCIntClear(ADC0_BASE, 0);
    ADCIntEnable(ADC0_BASE, 0);

    IntMasterEnable();

    while(1)
    {
        ADCProcessorTrigger(ADC0_BASE, 0);    //����һ�δ�����ADC�ɼ�
        while(ADCIntStatus(ADC0_BASE, 0, false));
        ADCSequenceDataGet(ADC0_BASE, 0, &ADC0_Value);
        PWM_Width = ADC0_Value/5;
//        PWMPulseWidthSet(PWM1_BASE, PWM_OUT_7, PWM_Width);
    }
}
