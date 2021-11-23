/*
 *  ======== main.c ========
 */
#include <math.h>
#include <xdc/std.h>
#include <xdc/runtime/Error.h>
#include <xdc/runtime/System.h>

#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Swi.h>
#include <Headers/F2837xD_device.h>
#include "FFT.h"
#define xdc__strict //suppress typedef warnings

#define N 64
#define PI 3.1415926535

//function prototypes:
extern void DeviceInit(void);

int16 mic_reading;
int16 mask[N]={0};
COMPLEX buf_0[N]={0};
COMPLEX buf_1[N]={0};
COMPLEX twid[N]={0};
int16 buf_index=0;
bool buf_flag=0;
bool buf_1_full=0;
bool buf_2_full=0;
float32 test_data[N];
COMPLEX test_buf[N]={0};


//Swi handle defined in .cfg file:
extern const Swi_Handle Swi0;

//function prototypes:
extern void DeviceInit(void);


/*
 *  ======== HWIFxn ========
 */
Void sample_microphone(Void) //Configured to sample at 10kHz or 100us period between samples in .cfg
{
    GpioDataRegs.GPATOGGLE.bit.GPIO3 = 1; //toggle blue LED

    //read ADC value from microphone
//    mic_reading = (int16)(AdcaResultRegs.ADCRESULT0); //get reading
//    AdcaRegs.ADCINTFLGCLR.bit.ADCINT1 = 1; //clear interrupt flag

    mic_reading=test_data[buf_index];

    if(buf_index>=N){ //check if buffer index out of range/full
        buf_index=0; //reset to zero
        buf_flag^=1; //switch buffers
        Swi_post(Swi0); //test post function
    }
    if(buf_flag) //fill buffer according to flag
        buf_1[buf_index].real=mic_reading;
    else
        buf_0[buf_index].real=mic_reading;
    buf_index++; //increment buffer index

    GpioDataRegs.GPATOGGLE.bit.GPIO3 = 1; //toggle blue LED
}

/*
 *  ======== SwiFxn ========
 */
Void SwiFxn(UArg a0, UArg a1) //will contain the fft function call
{

    GpioDataRegs.GPATOGGLE.bit.GPIO2 = 1; //toggle green LED
    // Reverse Input Array
    reversearray(test_buf,N,mask);
    // Calculate Frequency Spectrum Array
    calcfftN(test_buf,twid,N);
    GpioDataRegs.GPATOGGLE.bit.GPIO2 = 1; //toggle green LED

}

/*
 *  ======== taskFxn ========
 */
Void taskFxn(UArg a0, UArg a1) //will contain the rendering portion
{
    System_printf("enter taskFxn()\n");

    System_printf("exit taskFxn()\n");

    System_flush(); /* force SysMin output to console */
}

/*
 *  ======== main ========
 */
Int main()
{ 
    Task_Handle task;
    Error_Block eb;

    System_printf("enter main()\n");
    DeviceInit();

    Error_init(&eb);
    task = Task_create(taskFxn, NULL, &eb);
    if (task == NULL) {
        System_printf("Task_create() failed!\n");
        BIOS_exit(0);
    }

    // Create Twiddle Factors
    twiddle(twid,N);
    // Generate sample waveforms:
    int i;
    for(i=0; i < (N); i++)
    {
        test_data[i]=sinf(2*PI*i/N);
    }

    BIOS_start();    /* does not return */
    return(0);
}
