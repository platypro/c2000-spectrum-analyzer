/*
 *  ======== main.c ========
 */

#include <xdc/std.h>
#include <xdc/runtime/Error.h>
#include <xdc/runtime/System.h>

#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Swi.h>
#include <F2837xD_device.h>

#include "dsp/include/dsp.h"
#include "dsp/include/fpu32/fpu_vector.h"
#include "dsp/include/fpu32/fpu_cfft.h"
#include "math.h"

#include "FFT.h"

#define xdc__strict //suppress typedef warnings

#define N 256
#define PI 3.1415926535

//function prototypes:
extern void DeviceInit(void);

int16 mic_reading;
int16 mask[N]={0};
int16 buf_index=0;
bool buf_flag=0;
bool buf_0_full=0;
bool buf_1_full=0;
float32 buf_0[2*N]={0};
float32 buf_1[2*N]={0};
float32 test_data[N];

CFFT_F32_STRUCT cfft;
CFFT_F32_STRUCT_Handle cfft_hnd = &cfft;
float32 cfft_twid[N];
float32 cfft_out[2*N];

//Swi handle defined in .cfg file:
extern const Swi_Handle Swi0;

//function prototypes:
extern void DeviceInit(void);


/*
 *  ======== HWIFxn ========
 */
Void sample_microphone(Void) //Configured to sample at 10kHz or 100us period between samples in .cfg
{

    //read ADC value from microphone
//    mic_reading = (int16)(AdcaResultRegs.ADCRESULT0); //get reading
//    AdcaRegs.ADCINTFLGCLR.bit.ADCINT1 = 1; //clear interrupt flag

    mic_reading=test_data[buf_index];


    if(buf_flag) //fill buffer according to flag
        buf_1[2*buf_index]=mic_reading;
    else
        buf_0[2*buf_index]=mic_reading;
    buf_index++; //increment buffer index

    if(buf_index>=N){ //check if buffer index out of range/full
        buf_index=0; //reset to zero
        buf_flag^=1; //switch buffers
        Swi_post(Swi0); //test post function
    }

}

/*
 *  ======== SwiFxn ========
 */
Void SwiFxn(UArg a0, UArg a1) //will contain the fft function call
{
    if(buf_flag){
        GpioDataRegs.GPATOGGLE.bit.GPIO2 = 1; //toggle green LED
        cfft_hnd->InPtr = buf_0;
        CFFT_f32u(cfft_hnd);
        CFFT_f32s_mag(cfft_hnd);
        GpioDataRegs.GPATOGGLE.bit.GPIO2 = 1; //toggle green LED

    }
    else{
        GpioDataRegs.GPATOGGLE.bit.GPIO3 = 1; //toggle blue LED
        cfft_hnd->InPtr = buf_1;
        CFFT_f32u(cfft_hnd);
        CFFT_f32s_mag(cfft_hnd);
        GpioDataRegs.GPATOGGLE.bit.GPIO3 = 1; //toggle blue LED

    }
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

    cfft_hnd->OutPtr = cfft_out;
    cfft_hnd->CoefPtr = cfft_twid;
    cfft_hnd->FFTSize = N;
    cfft_hnd->Stages = logbase2(N);
    CFFT_f32_sincostable(cfft_hnd);

    // Generate sample waveforms:
    int i;
    for(i=0; i < (N); i++)
    {
        test_data[i]=sinf(2*PI*i/N);
    }

    BIOS_start();    /* does not return */
    return(0);
}
