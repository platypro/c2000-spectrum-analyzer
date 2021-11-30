//Authors: Brian Card, Aeden McClain
/*
 *  ======== main.c ========
 *
 */

#include <xdc/std.h>
#include <xdc/runtime/Error.h>
#include <xdc/runtime/System.h>

#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Swi.h>
#include <ti/sysbios/knl/Semaphore.h>
#include <F2837xD_device.h>

#include "dsp.h"
#include "fpu32/fpu_cfft.h"
#include "fpu32/fpu_vector.h"
#include "spi.h"
#include "math.h"
#include "FFT.h"
#include <stdio.h>
#include <string.h>

#include "display/display.h"
#include "display/command.h"
#include "driverlib.h"
#include "device.h"
#include "board.h"

#define xdc__strict //suppress typedef warnings


#define N 256
#define PI 3.1415926535

//function prototypes:
extern void DeviceInit(void);

Uint16 limit=0;
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
extern const Swi_Handle Swi1;
extern Semaphore_Handle sem1;
extern Semaphore_Handle semDisplayTrigger;
//function prototypes:
extern void DeviceInit(void);

/*
 *  ======== HWI - sample_adc - Section written by BC ========
 *  This function is triggered every 20000 CPU cycles.
 *  The function samples an ADC value, then based on the flag buffers the sample
 *  Once a buffer is full it will post the SWI to the FFT function
 */
Void sample_adc(Void) //Configured to sample at 10kHz or 100us period between samples in .cfg
{
//    GpioDataRegs.GPATOGGLE.bit.GPIO3 = 1; //toggle blue LED
    AdcaRegs.ADCSOCFRC1.all = 0x0001;
    AdcaRegs.ADCINTFLGCLR.bit.ADCINT1 = 1; //clear interrupt flag

    //read ADC value from microphone
    mic_reading = (float32)(AdcaResultRegs.ADCRESULT0); //get reading
    if(buf_flag && !buf_1_full){ //fill buffer according to flag
        buf_1[2*buf_index]=mic_reading; //load real array position
        buf_1[1+2*buf_index]=0.0; //zero previous complex data if any
        buf_index++; //increment buffer index
    }
    else if(!buf_flag && !buf_0_full){
        buf_0[2*buf_index]=mic_reading;
        buf_0[1+2*buf_index]=0.0;
        buf_index++; //increment buffer index
    }
    if(buf_index>=N){ //check if buffer index out of range/full
        buf_index=0; //reset to zero
        buf_flag^=1; //switch buffers
        Swi_post(Swi1); //test post function
    }
//    GpioDataRegs.GPATOGGLE.bit.GPIO3 = 1; //toggle blue LED
}


/*
 *  ======== SWI- calc_fft - Section written by BC========
 *  This function is triggered when a Swi1 is posted
 *  This function checks the buffer flag and performs an FFT and magnitude calculation
 *  Once the function is complete it posts sem0 to the display function
 */
Void calc_fft(UArg a0, UArg a1) //will contain the fft function call
{

    if(buf_flag){

        cfft_hnd->InPtr = buf_0;
        CFFT_f32u(cfft_hnd);
        CFFT_f32_mag_TMU0(cfft_hnd);
        buf_0_full=1;

    }
    if(!buf_flag){

        cfft_hnd->InPtr = buf_1;
        CFFT_f32u(cfft_hnd);
        CFFT_f32_mag_TMU0(cfft_hnd);
        buf_1_full=1;

    }
    Semaphore_post(sem1);

}

/*
 *  ======== taskFxn - Test function - Section written by AM ========
 */
Void display_trigger(Void)
{
//    Semaphore_post(semDisplayTrigger);
}

#pragma DATA_SECTION(writeData, "displaybuf");
Uint16 writeData[256]; //char
/*
 *  ======== taskFxn - display_updtask - Section written by BC and AM ========
 *  This function is sitting in a loop until a sem0 is posted by the FFT function (indicating a full and processed buffer)
 *  This function writes the data to the LCD screen
 *  Once complete it clears the full buffer flag, allowing new samples to be buffered
 */
Void display_updtask(Void)
{
    display_init();

    // Shift to appease SPI (Wants in higher byte)
    Uint16 i,j;

//    for(i = 0; i<62; i++)
//        {  writeData[i] <<= 8; }

    display_command_raset(0+3, 127+3); //adjust LCD offsets
    display_command_caset(0+1, 127+1);
    display_command_ramwr();
//    display_colour_t colour1 = {.r=0,.g=31,.b=0};
    display_colour_t colour2 = {.r=0,.g=0,.b=15};
    for(;;)
    {
//        Semaphore_pend(semDisplayTrigger, BIOS_WAIT_FOREVER); //timer post for testing
        Semaphore_pend(sem1, BIOS_WAIT_FOREVER); //wait for FFT post

        //following block checks full buffer flags and writes data to LCD
        if(buf_0_full){
            for(i=1; i<=N/2;i++){
                limit = (Uint16)(buf_0[i]/N);
                if(limit>128)
                    limit=128;
                for(j=0;j<limit;j++){
                    writeData[j * 2]     = *((Uint16*)&colour2) & 0xFF00U;
                    writeData[j * 2 + 1] = (*((Uint16*)&colour2) & 0x00FFU) << 8;

                }
                for(j=limit;j<128;j++){
                    writeData[j * 2]     = 0xFF00;
                    writeData[j * 2 + 1] = 0xFF00;

                }
                display_write((Uint16*)writeData, 256);
            }
            buf_0_full=0;
        }
        if(buf_1_full){
            for(i=1; i<=N/2;i++){
                limit = (Uint16)(buf_1[i]/N);
                if(limit>128)
                    limit=128;
                for(j=0;j<limit;j++){
                    writeData[j * 2]     = *((Uint16*)&colour2) & 0xFF00U;
                    writeData[j * 2 + 1] = (*((Uint16*)&colour2) & 0x00FFU) << 8;
                }
                for(j=limit;j<128;j++){
                    writeData[j * 2]     = 0xFF00;
                    writeData[j * 2 + 1] = 0xFF00;

                }
                display_write((Uint16*)writeData, 256);
            }
            buf_1_full=0;
        }

    }
}
/*
 *  ======== idleFxn - check_limit - Section written by BC========
 *  This function runs in the background (lowest priority) and will turn on the green LED when a signal level threshold is reached
 */
Void check_limit(Void){
    if(limit >= 32){
        GpioDataRegs.GPASET.bit.GPIO2 = 1; //turn on green led
    }
    else{
        GpioDataRegs.GPACLEAR.bit.GPIO2 = 1; //turn off green led
    }
}

/*
 *  ======== main - Section written by BC========
 */
Int main()
{ 
    Error_Block eb;
    System_printf("enter main()\n");
    DeviceInit();
    Board_init();
    Error_init(&eb);
// Set cfft object pointers
    cfft_hnd->OutPtr = buf_out;
    cfft_hnd->CoefPtr = twid;
    cfft_hnd->FFTSize = N;
    cfft_hnd->Stages = logbase2(N);
    CFFT_f32_sincostable(cfft_hnd); // build triddle factors

    BIOS_start();    /* does not return */
    return(0);
}
