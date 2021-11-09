/*
 *  ======== main.c ========
 */

#include <xdc/std.h>
#include <xdc/runtime/Error.h>
#include <xdc/runtime/System.h>

#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>
#include <Headers/F2837xD_device.h>

#define xdc__strict //suppress typedef warnings


#define getTempSlope (*(int (*)(void))0x7036E)
#define getTempOffset (*(int (*)(void))0x70372)
#define VREFHI 3.0

//function prototypes:
extern void DeviceInit(void);

int16 temp_slope;
int16 temp_offset;
int16 temp_reading;
int32 temp_celsius;


/*
 *  ======== HWIFxn ========
 */
Void myHwi(Void)
{
    //read ADC value from temperature sensor:
    temp_reading = (int16)((VREFHI / 2.5) * AdcaResultRegs.ADCRESULT0); //get reading and scale re VREFHI
    AdcaRegs.ADCINTFLGCLR.bit.ADCINT1 = 1; //clear interrupt flag

    //convert reading to Celsius:
    temp_celsius = (int32)(temp_reading - temp_offset) * (int32)temp_slope;

    GpioDataRegs.GPATOGGLE.bit.GPIO31 = 1; //toggle blue LED
}

/*
 *  ======== taskFxn ========
 */
Void taskFxn(UArg a0, UArg a1)
{
    System_printf("enter taskFxn()\n");

    Task_sleep(10);

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

    Error_init(&eb);
    task = Task_create(taskFxn, NULL, &eb);
    if (task == NULL) {
        System_printf("Task_create() failed!\n");
        BIOS_exit(0);
    }

    temp_slope = getTempSlope();
    temp_offset = getTempOffset();


    BIOS_start();    /* does not return */
    return(0);
}
