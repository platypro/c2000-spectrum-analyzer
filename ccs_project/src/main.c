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

#include <dsp.h>
#include "fpu32/fpu_vector.h"

#define xdc__strict //suppress typedef warnings

#define N 4
//function prototypes:
extern void DeviceInit(void);

int16 mic_reading;
int16 buf_0[N]={0};
int16 buf_1[N]={0};
int16 buf_index=0;
bool buf_flag=0;
bool buf_1_full=0;
bool buf_2_full=0;

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
    mic_reading = (int16)(AdcaResultRegs.ADCRESULT0); //get reading
    AdcaRegs.ADCINTFLGCLR.bit.ADCINT1 = 1; //clear interrupt flag

    if(buf_index>=N){ //check if buffer index out of range/full
        buf_index=0; //reset to zero

        buf_flag^=1; //switch buffers
    }
    if(buf_flag) //fill buffer according to flag
        buf_1[buf_index]=mic_reading;
    else
        buf_0[buf_index]=mic_reading;
    buf_index++; //increment buffer index

    GpioDataRegs.GPATOGGLE.bit.GPIO3 = 1; //toggle blue LED

    Swi_post(Swi0); //test post function
}

/*
 *  ======== SwiFxn ========
 */
Void SwiFxn(UArg a0, UArg a1) //will contain the fft function call
{
    GpioDataRegs.GPATOGGLE.bit.GPIO2 = 1; //toggle green LED
}

/*
 *  ======== taskFxn ========
 */
Void taskFxn(UArg a0, UArg a1) //will contain the rendering portion
{
    System_printf("enter taskFxn()\n");

    Task_sleep(10);

    System_printf("exit taskFxn()\n");

    System_flush(); /* force SysMin output to console */
}

// Test function for fpu library
void test_fpulib()
{
    float y[4];
    float w[4];
    float x[4];
    w[0] = 5.0f;
    x[0] = 6.0f;
    mpy_SP_RVxRV_2 (y, w, x,4);
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

    BIOS_start();    /* does not return */
    return(0);
}
