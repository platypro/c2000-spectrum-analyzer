// devInit.c
// Driver Initalization
//
// Authors: Brian Card, Aeden McClain
// Adapted from code by David Romalo

#include <F2837xD_device.h>

#include "board.h"

extern void DelayUs(Uint16);

void DeviceInit(void)
{
EALLOW;

    //initialize GPIO lines:
    GpioCtrlRegs.GPAMUX2.bit.GPIO31 = 0; //D10 (blue LED)
    GpioCtrlRegs.GPADIR.bit.GPIO31 = 1;
    GpioDataRegs.GPACLEAR.bit.GPIO31 = 1;

    GpioCtrlRegs.GPBMUX1.bit.GPIO34 = 0; //D9 (red LED)
    GpioCtrlRegs.GPBDIR.bit.GPIO34 = 1;
    GpioDataRegs.GPBCLEAR.bit.GPIO34 = 1;

    //configure green led
    GpioCtrlRegs.GPAMUX1.bit.GPIO2 = 0; //set pin as gpio
    GpioCtrlRegs.GPADIR.bit.GPIO2 = 1; //set gpio as output
    GpioDataRegs.GPASET.bit.GPIO2 = 0; //initialize output value to "1"
    //configure blue led
    GpioCtrlRegs.GPAMUX1.bit.GPIO3 = 0; //set pin as gpio
    GpioCtrlRegs.GPADIR.bit.GPIO3 = 1; //set gpio as output
    GpioDataRegs.GPASET.bit.GPIO3 = 0; //initialize output value to "1"

    //---------------------------------------------------------------
    // INITIALIZE A-D
    //---------------------------------------------------------------
    CpuSysRegs.PCLKCR13.bit.ADC_A = 1; //enable A-D clock for ADC-A
    AdcaRegs.ADCCTL2.bit.PRESCALE = 6;//0xf;
    AdcaRegs.ADCCTL2.bit.RESOLUTION = 0;
    AdcaRegs.ADCCTL2.bit.SIGNALMODE = 0;
    AdcaRegs.ADCCTL1.bit.ADCPWDNZ = 1;

    //generate INT pulse on end of conversion:
    AdcaRegs.ADCCTL1.bit.INTPULSEPOS = 1;

    //wait 1 ms after power-up before using the ADC:
    DelayUs(1000);

//    AnalogSubsysRegs.TSNSCTL.bit.ENABLE = 1; //connect temp sensor to ADCIN13 on ADC-A

    AdcaRegs.ADCSOC0CTL.bit.TRIGSEL = 0;//2; //trigger source = CPU1 Timer 1
    AdcaRegs.ADCSOC0CTL.bit.CHSEL = 1;//13; //set SOC0 to sample A5 ***modified to 1 with jumper cable to sample microphone***
    AdcaRegs.ADCSOC0CTL.bit.ACQPS = 50; //set SOC0 window to 139 SYSCLK cycles
    AdcaRegs.ADCINTSEL1N2.bit.INT1SEL = 0; //connect interrupt ADCINT1 to EOC0
    AdcaRegs.ADCINTSEL1N2.bit.INT1E = 1; //enable interrupt ADCINT1*/
    AdcaRegs.ADCINTFLGCLR.bit.ADCINT1 = 1; //make sure INT1 flag is cleared

    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_SPIA);
    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_DMA);

    SysCtl_selectSecMaster(0, SYSCTL_SEC_MASTER_DMA);
EDIS;
}

void __error__(char *filename, uint32_t line)
    { ESTOP0; }
