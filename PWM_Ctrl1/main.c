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
//通过PWM发生器产生100kHz的PWM波，改变电位器来控制拓展版灯的亮度
//配置好寄存器即可自动产生PWM波，不用占用CPU资源
//ADC采集，需要先使能采集序列，再进行采集。触发方式可以由处理器也可以由其他方式

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
    SysCtlClockSet(SYSCTL_XTAL_16MHZ | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_SYSDIV_5); //配置系统时钟为40MHz
    SysCtlPWMClockSet(SYSCTL_PWMDIV_1);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);    //使能F组GPIO
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);    //使能E组GPIO
    SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM1);    //使能PWM发生器1
    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);   //使能ADC0

    GPIOPinConfigure(GPIO_PF3_M1PWM7);
    GPIOPinTypePWM(GPIO_PORTF_BASE, GPIO_PIN_3);    //GPIO配置，使用PWM功能前必须先调用GPIOPinConfigure()
    GPIOPinTypeADC(GPIO_PORTE_BASE, GPIO_PIN_1);    //GPIO配置，ADC采样为PE1


    ADCSequenceConfigure(ADC0_BASE, 0, ADC_TRIGGER_PROCESSOR, 0);   //ADC序列配置：选择ADC0，序列号为0，由处理器进行触发，优先级最高设为0
    ADCSequenceStepConfigure(ADC0_BASE, 0, 0, ADC_CTL_CH2); //ADC采集步序配置：采样序列号0，只采集AIN2

    ADCReferenceGet(ADC_REF_EXT_3V); //ADC参考电压设置

    PWMGenConfigure(PWM1_BASE, PWM_GEN_3, PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC);    //PWM发生器功能配置
    PWMGenPeriodSet(PWM1_BASE, PWM_GEN_3, 800); //PWM发生器周期配置
    PWMPulseWidthSet(PWM1_BASE, PWM_OUT_7, 100);    //PWM发生器脉宽配置

    PWMOutputState(PWM1_BASE, PWM_OUT_7_BIT, true); //PWM输出口状态
    PWMGenEnable(PWM1_BASE, PWM_GEN_3); //PWM发生器使能
    ADCSequenceEnable(ADC0_BASE, 0); //ADC采集使能
    ADCIntClear(ADC0_BASE, 0);
    ADCIntEnable(ADC0_BASE, 0);

    IntMasterEnable();

    while(1)
    {
        ADCProcessorTrigger(ADC0_BASE, 0);    //进行一次处理器ADC采集
        while(ADCIntStatus(ADC0_BASE, 0, false));
        ADCSequenceDataGet(ADC0_BASE, 0, &ADC0_Value);
        PWM_Width = ADC0_Value/5;
//        PWMPulseWidthSet(PWM1_BASE, PWM_OUT_7, PWM_Width);
    }
}
