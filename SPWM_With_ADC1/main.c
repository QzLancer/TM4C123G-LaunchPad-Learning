#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/pwm.h"
#include "driverlib/pin_map.h"
#include "driverlib/interrupt.h"
#include "driverlib/adc.h"
#include "driverlib/timer.h"
#include "driverlib/rom.h"
#include "driverlib/interrupt.h"
#include "inc/hw_memmap.h"
#include "inc/hw_gpio.h"
#include "inc/hw_types.h"
#include "inc/hw_ints.h"
//ͨ��PWM����������50kHz�Ļ���SPWM��������ʱ��Ϊ1/20�����ز�����50Hz,ͨ����չ���ϵ�K3(PA3)��K4(PA2)�������������Ҳ���Ƶ�ʣ�Ϊ20Hz��100Hz֮��
//ʹ�õ�����ΪPF2��PF3
//�ɼ��˿�ΪPE2
int i=0;
float k=1;
typedef struct
{
    float SetVoltage;    //�趨��ѹ
    float ActualVoltage;      //ʵ��ֵ
    float err;      //k,����ƫ��ֵ
    float err_last;       //k-1,��һ��ƫ��ֵ
    float err_last_next;  //k-2
    float Kp, Ki, Kd;     //p,i,dϵ��
    float OutputVoltage;
    }PID;

int SPWMData[200] = {128,131,135,139,143,147,151,155,
                          159,163,167,171,174,178,182,185,
                          189,192,196,199,202,205,208,211,
                          214,217,220,223,225,228,230,233,
                          235,237,239,241,242,244,246,247,
                          248,249,251,251,252,253,253,254,
                          254,254,255,254,254,254,253,253,
                          252,251,251,249,248,247,246,244,
                          242,241,239,237,235,233,230,228,
                          225,223,220,217,214,211,208,205,
                          202,199,196,192,189,185,182,178,
                          174,171,167,163,159,155,151,147,
                          143,139,135,131,127,124,120,116,
                          112,108,104,100,96,92,88,84,
                          81,77,73,70,66,63,59,56,
                          53,50,47,44,41,38,35,32,
                          30,27,25,22,20,18,16,14,
                          13,11,9,8,7,6,4,4,
                          3,2,2,1,1,1,1,1,
                          1,1,2,2,3,4,4,6,
                          7,8,9,11,13,14,16,18,
                          20,22,25,27,30,32,35,38,
                          41,44,47,50,53,56,59,63,
                          66,70,73,77,81,84,88,92,
                          96,100,104,108,112,116,120,124,
};
int frequency = 50; //��ʼƵ��Ϊ50Hz
uint32_t Period = 50000;    //�ز�����
uint32_t PulseWid = 25000;  //����
uint32_t ADC0_Value = 0;
float OutputVoltage;
float ActualVoltage;

bool ButtonDown(uint32_t ui32Port, uint8_t ui8Pins);
void PID_Init(PID *PID_Instance);
float PIDCalculator(PID *PID_Instance,float Voltage);

int main(void)
{
    PID pid0;
    int IfButtonK3Down = 0; //K3���±�־λ
    int IfButtonK4Down = 0; //K4���±�־λ
    uint32_t DeadBand = PulseWid*20;    //����
    float OutputVoltage;
    float ActualVoltage;
    SysCtlClockSet(SYSCTL_XTAL_16MHZ | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_SYSDIV_2_5); //����ϵͳʱ��Ϊ80MHz
    SysCtlPWMClockSet(SYSCTL_PWMDIV_1);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);    //ʹ��F��GPIO
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);    //ʹ��A��GPIO
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);    //ʹ��E��GPIO
    SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM1);    //ʹ��PWM��ģ��1
    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);   //ʹ��ADC0
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);  //ʹ�ܶ�ʱ��0


    ADCSequenceConfigure(ADC0_BASE, 0, ADC_TRIGGER_PROCESSOR, 0);   //ADC�������ã�ѡ��ADC0�����к�Ϊ0���ɴ��������д��������ȼ������Ϊ0
    ADCSequenceStepConfigure(ADC0_BASE, 0, 0, ADC_CTL_CH1); //ADC�ɼ��������ã��������к�0��ֻ�ɼ�AIN1
    ADCReferenceGet(ADC_REF_EXT_3V); //ADC�ο���ѹ����

    ADCSequenceEnable(ADC0_BASE, 0); //ADC�ɼ�ʹ��
    ADCIntClear(ADC0_BASE, 0);
    ADCIntEnable(ADC0_BASE, 0);

    GPIOPinTypeGPIOInput(GPIO_PORTA_BASE, GPIO_PIN_2 | GPIO_PIN_3);    //����PA2��FA3Ϊ����
    GPIOPadConfigSet(GPIO_PORTA_BASE, GPIO_PIN_2 | GPIO_PIN_3, GPIO_STRENGTH_4MA, GPIO_PIN_TYPE_STD_WPU);   //����������

    GPIOPinConfigure(GPIO_PF3_M1PWM7);
    GPIOPinConfigure(GPIO_PF2_M1PWM6);
    GPIOPinTypePWM(GPIO_PORTF_BASE, GPIO_PIN_2 | GPIO_PIN_3);   //GPIO���ã�ʹ��PWM����ǰ�����ȵ���GPIOPinConfigure()

    TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);    //��ʱ��0����
    TimerLoadSet(TIMER0_BASE, TIMER_A, SysCtlClockGet()/(frequency*200)-1);   //��ʱ��0װ��

    PWMGenConfigure(PWM1_BASE, PWM_GEN_3, PWM_GEN_MODE_UP_DOWN | PWM_GEN_MODE_NO_SYNC | PWM_GEN_MODE_DB_NO_SYNC);    //PWM��������������
    PWMGenPeriodSet(PWM1_BASE, PWM_GEN_3, SysCtlClockGet()/Period); //PWM��������������
    PWMPulseWidthSet(PWM1_BASE, PWM_OUT_6, SysCtlClockGet()/(k*PulseWid));    //PWM��������������
    PWMDeadBandEnable(PWM1_BASE, PWM_GEN_3, SysCtlClockGet()/DeadBand, SysCtlClockGet()/DeadBand);    //�������ã����ա�Tivaԭ����Ӧ�� 10.3.5��

    IntPrioritySet(INT_TIMER0A, 0x00);    //�ж����ȼ�����

    PWMOutputState(PWM1_BASE, PWM_OUT_6_BIT | PWM_OUT_7_BIT, true); //PWM�����״̬
    PWMGenEnable(PWM1_BASE, PWM_GEN_3); //PWM������ʹ��
    TimerIntEnable(TIMER0_BASE,TIMER_TIMA_TIMEOUT);   //���ö�ʱ��0�ж�
    IntEnable(INT_TIMER0A); //ʹ�ܶ�ʱ��0�ж�
    TimerEnable(TIMER0_BASE,TIMER_A);
    PID_Init(&pid0);

    while(1)
    {

       ActualVoltage=ADC0_Value*3.3/4096.0;

//PID���ƣ��ı�ռ�ձ�
       OutputVoltage = PIDCalculator(&pid0,ActualVoltage);
//������ֵ�����ռ�ձ����ı�����
            k=OutputVoltage/3.3;
//�������£��ı�����Ƶ��
        if((IfButtonK3Down==0)&(IfButtonK4Down==0)&(ButtonDown(GPIO_PORTA_BASE, GPIO_PIN_3)|ButtonDown(GPIO_PORTA_BASE, GPIO_PIN_2)))
        {
            if(ButtonDown(GPIO_PORTA_BASE, GPIO_PIN_3))
            {
                IfButtonK3Down=1;
                if(frequency>=20)
                    frequency--;
                    Period = frequency*100;
                TimerLoadSet(TIMER0_BASE, TIMER_A, SysCtlClockGet()/(frequency*200)-1);
            }
            else if(ButtonDown(GPIO_PORTA_BASE, GPIO_PIN_2))
            {
                IfButtonK4Down=1;
                if(frequency<=100)
                    frequency++;
                    Period = frequency*100;
                TimerLoadSet(TIMER0_BASE, TIMER_A, SysCtlClockGet()/(frequency*200)-1);
            }
        }
        if(ButtonDown(GPIO_PORTA_BASE, GPIO_PIN_3)==0)
            IfButtonK3Down=0;
        if(ButtonDown(GPIO_PORTA_BASE, GPIO_PIN_2)==0)
            IfButtonK4Down=0;

    }
}

bool ButtonDown(uint32_t ui32Port, uint8_t ui8Pins)
{
    int flag=0;
    if((flag==0)&((GPIOPinRead(ui32Port, ui8Pins)&&ui8Pins)==0)) //����������£���־λû����λ
        {
            SysCtlDelay(SysCtlClockGet()/100-1); //��ʱ
            flag=1; //��־λ��λ
            if((flag==1)&((GPIOPinRead(ui32Port, ui8Pins)&&ui8Pins)==0)) //����������£���־λ��λ
            {
                //while(((GPIOPinRead(GPIO_PORTA_BASE, GPIO_PIN_2)&&ui8Pins)==0x04));    //�ȴ������ɿ�
                return 1;   //ȷ�ϰ���������
            }
            else
                return 0;   //����ȷ�ϰ�����������
        }
    else
        return 0;
}

void TIMER0IntHandler(void)
{

    ADCProcessorTrigger(ADC0_BASE, 0);    //����һ�δ�����ADC�ɼ�
    while(ADCIntStatus(ADC0_BASE, 0, false));
    ADCSequenceDataGet(ADC0_BASE, 0, &ADC0_Value);
    TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
    if(i<=199)
    {
        Period = frequency*1000;
        PulseWid = Period*(256/SPWMData[i]);
        PWMGenPeriodSet(PWM1_BASE, PWM_GEN_3, SysCtlClockGet()/Period); //PWM��������������
        PWMPulseWidthSet(PWM1_BASE, PWM_OUT_6, k*SysCtlClockGet()/(Period*256/SPWMData[i])+(1-k)*SysCtlClockGet()/(Period*256/128));
        i++;
    }
    else if(i>=200)
    {
        Period = frequency*1000;
        PulseWid = Period*(256/SPWMData[i]);
        PWMGenPeriodSet(PWM1_BASE, PWM_GEN_3, SysCtlClockGet()/Period); //PWM��������������
        PWMPulseWidthSet(PWM1_BASE, PWM_OUT_6, k*SysCtlClockGet()/(Period*256/SPWMData[0])+(1-k)*SysCtlClockGet()/(Period*256/128));
        i=1;
    }
}


void PID_Init(PID *PID_Instance)//�������ֵ���У������
{
    PID_Instance->SetVoltage=0.4;    //�趨��ѹ ��
    PID_Instance->ActualVoltage=0;      //ʵ��ֵ
    PID_Instance->err=0;      //k,����ƫ��ֵ
    PID_Instance->err_last=0;       //k-1,��һ��ƫ��ֵ
    PID_Instance->err_last_next=0;  //k-2
    PID_Instance->Kp=25;
    PID_Instance->Ki=0;
    PID_Instance->Kd=0;     //p,i,dϵ��
    PID_Instance->OutputVoltage=0;
}

//����ʽPID
float PIDCalculator(PID *PID_Instance,float Voltage)
{
    float IncreaseVoltage;
    PID_Instance->ActualVoltage=Voltage;
    PID_Instance->err=PID_Instance->SetVoltage-PID_Instance->ActualVoltage;
    IncreaseVoltage=PID_Instance->Kp*(PID_Instance->err-PID_Instance->err_last)+PID_Instance->Ki*PID_Instance->err;   //����PI����
    PID_Instance->OutputVoltage+=IncreaseVoltage;
    if(PID_Instance->OutputVoltage>=3.3)
        PID_Instance->OutputVoltage=3.3;
    else if(PID_Instance->OutputVoltage<=0)
        PID_Instance->OutputVoltage=0;
    PID_Instance->err_last_next=PID_Instance->err_last;
    PID_Instance->err_last=PID_Instance->err;
    return PID_Instance->OutputVoltage;
}


//�Ͽ������ߣ������˵�ƽ�Ƕ��٣���
//OK
