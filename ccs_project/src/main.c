/*
 *  ======== main.c ========
 */

#include <xdc/std.h>
#include <xdc/runtime/Error.h>
#include <xdc/runtime/System.h>

#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Swi.h>
#include <ti/sysbios/knl/Semaphore.h>
#include <F2837xD_device.h>

#include "dsp/include/dsp.h"
#include "dsp/include/fpu32/fpu_cfft.h"
#include "dsp/include/fpu32/fpu_vector.h"
#include "spi.h"
#include "math.h"
#include "FFT.h"
#include <stdio.h>
#include <string.h>

#include "display/display.h"
#include "driverlib.h"
#include "device.h"
#include "board.h"
#include "usb/include/usb_hal.h"
#include "usb/include/usblib.h"
#include "usb/include/usb_ids.h"
#include "usb/include/device/usbdevice.h"
#include "usb/include/device/usbdbulk.h"

#define xdc__strict //suppress typedef warnings

#define N 256
#define PI 3.1415926535

//function prototypes:
extern void DeviceInit(void);


int16 buf_index=0;
int16 buf_flag=0;
int16 buf_0_full=0;
int16 buf_1_full=0;
float32 mic_reading;
float32 test_data[N];
float32 twid[N]={0};
float32 buf_0[2*N]={0};
float32 buf_1[2*N]={0};
float32 buf_out[2*N]={0};
CFFT_F32_STRUCT cfft;
CFFT_F32_STRUCT_Handle cfft_hnd = &cfft;


//Swi handle defined in .cfg file:
extern const Swi_Handle Swi0;
extern const Swi_Handle Swi1;
extern Semaphore_Handle sem0;
extern Semaphore_Handle semDisplayTrigger;
//function prototypes:
extern void DeviceInit(void);

/*
 *  ======== HWIFxn ========
 */
Void sample_adc(Void) //Configured to sample at 10kHz or 100us period between samples in .cfg
{
    AdcaRegs.ADCSOCFRC1.all = 0x0001;
    AdcaRegs.ADCINTFLGCLR.bit.ADCINT1 = 1; //clear interrupt flag

    //read ADC value from microphone
    mic_reading = (float32)(AdcaResultRegs.ADCRESULT0); //get reading
    if(buf_flag){ //fill buffer according to flag
        buf_1[2*buf_index]=mic_reading; //load real array position
        buf_1[1+2*buf_index]=0.0; //zero previous complex data if any
    }
    else{
        buf_0[2*buf_index]=mic_reading;
        buf_0[1+2*buf_index]=0.0;
    }
    buf_index++; //increment buffer index
    if(buf_index>=N){ //check if buffer index out of range/full
        buf_index=0; //reset to zero
        buf_flag^=1; //switch buffers
        Swi_post(Swi1); //test post function
    }
}

Void display_trigger(Void)
{
    Semaphore_post(semDisplayTrigger);
}

#pragma DATA_SECTION(testData, "displaybuf");
char testData[62] = "When the moon hits your eye like a big pizza pie that's amore.";

Void display_updtask(Void)
{
    // Shift to appease SPI (Wants in higher byte)
    int i;
    for(i = 0; i<62; i++)
        {  testData[i] <<= 8; }

    for(;;)
    {
        Semaphore_pend(semDisplayTrigger, BIOS_WAIT_FOREVER);
        //display_write((uint16_t*)testData, 62);
    }
}

/*
 *  ======== SwiFxn ========
 */
Void load_buffer(UArg a0, UArg a1)
{
    DINT;
//    GpioDataRegs.GPATOGGLE.bit.GPIO3 = 1; //toggle blue LED
//    //read ADC value from microphone
//    mic_reading = (float32)(AdcaResultRegs.ADCRESULT0); //get reading
//    if(buf_flag){ //fill buffer according to flag
//        buf_1[2*buf_index]=mic_reading; //load real array position
//        buf_1[1+2*buf_index]=0.0; //zero previous complex data if any
//    }
//    else{
//        buf_0[2*buf_index]=mic_reading;
//        buf_0[1+2*buf_index]=0.0;
//    }
//    buf_index++; //increment buffer index
//    if(buf_index>=N){ //check if buffer index out of range/full
//        buf_index=0; //reset to zero
//        buf_flag^=1; //switch buffers
//        Swi_post(Swi1); //test post function
//    }
//    GpioDataRegs.GPATOGGLE.bit.GPIO3 = 1; //toggle blue LED
    EINT;
}
Void calc_fft(UArg a0, UArg a1) //will contain the fft function call
{
    if(buf_flag){
        GpioDataRegs.GPATOGGLE.bit.GPIO2 = 1; //toggle green LED
        cfft_hnd->InPtr = buf_0;
        CFFT_f32u(cfft_hnd);
        CFFT_f32_mag_TMU0(cfft_hnd);
        GpioDataRegs.GPATOGGLE.bit.GPIO2 = 1; //toggle green LED
//        memset(buf_0,0.0,sizeof(buf_0));
    }
    else{
//        GpioDataRegs.GPATOGGLE.bit.GPIO3 = 1; //toggle blue LED
        cfft_hnd->InPtr = buf_1;
        CFFT_f32u(cfft_hnd);
        CFFT_f32_mag_TMU0(cfft_hnd);
//        GpioDataRegs.GPATOGGLE.bit.GPIO3 = 1; //toggle blue LED
//        memset(buf_1,0.0,sizeof(buf_1));
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

    Board_init();

    display_init();

    Error_init(&eb);
    task = Task_create(taskFxn, NULL, &eb);
    if (task == NULL) {
        System_printf("Task_create() failed!\n");
        BIOS_exit(0);
    }

    cfft_hnd->OutPtr = buf_out;
    cfft_hnd->CoefPtr = twid;
    cfft_hnd->FFTSize = N;
    cfft_hnd->Stages = logbase2(N);
    CFFT_f32_sincostable(cfft_hnd);

    // Generate sample waveforms:
//    int i;
//    for(i=0; i < (N); i++)
//    {
//        test_data[i]=sinf(2*PI*i/N);
//    }
    BIOS_start();    /* does not return */
    return(0);
}
