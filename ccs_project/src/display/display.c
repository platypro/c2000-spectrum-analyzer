/*(
 * display.c
 *
 *  Created on: Nov. 27, 2021
 *      Author: platypro
 */

#include "display.h"

#include "board.h"
#include "F2837xD_device.h"

#include <ti/sysbios/knl/Semaphore.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/BIOS.h>

extern Semaphore_Handle semDisplay;

void display_finish()
{
    DMA_stopChannel(DMA_CH6_BASE);
    while(!GPIO_readPin(19)) { continue; }
    GPIO_writePin(124, 1);
    Semaphore_post(semDisplay);
}

void display_begin()
{
    Semaphore_pend(semDisplay, 0);
}

void display_init()
{
    Semaphore_pend(semDisplay, 0);
    DMA_initController();

    DMA_configMode(DMA_CH6_BASE,DMA_TRIGGER_SPIATX, DMA_CFG_ONESHOT_DISABLE);
    DMA_setInterruptMode(DMA_CH6_BASE,DMA_INT_AT_END);
    DMA_enableTrigger(DMA_CH6_BASE);
    DMA_enableInterrupt(DMA_CH6_BASE);

    GPIO_setDirectionMode(124, GPIO_DIR_MODE_OUT);
    GPIO_writePin(124, 1);

    GPIO_setDirectionMode(160, GPIO_DIR_MODE_OUT);
    GPIO_setPinConfig(GPIO_160_GPIO160);
    GPIO_writePin(160, 1);

    Semaphore_post(semDisplay);

    //TODO: Post start-up commands here
}

void display_write8(uint32_t value)
{
    Semaphore_pend(semDisplay, BIOS_WAIT_FOREVER);
    GPIO_writePin(124, 0);
    SPI_writeDataBlockingFIFO(spi_display_BASE, (value & 0x000000FF) << 8);
    while(!GPIO_readPin(19)) { continue; }
    GPIO_writePin(124, 1);
    Semaphore_post(semDisplay);
}

void display_write16(uint32_t value)
{
    Semaphore_pend(semDisplay, BIOS_WAIT_FOREVER);
    GPIO_writePin(124, 0);
    SPI_writeDataBlockingFIFO(spi_display_BASE, (value & 0x0000FF00));
    SPI_writeDataBlockingFIFO(spi_display_BASE, (value & 0x000000FF) << 8);
    while(!GPIO_readPin(19)) { continue; }
    GPIO_writePin(124, 1);
    Semaphore_post(semDisplay);
}

void display_write24(uint32_t value)
{
    Semaphore_pend(semDisplay, BIOS_WAIT_FOREVER);
    GPIO_writePin(124, 0);
    SPI_writeDataBlockingFIFO(spi_display_BASE, (value & 0x00FF0000) >> 8);
    SPI_writeDataBlockingFIFO(spi_display_BASE, (value & 0x0000FF00));
    SPI_writeDataBlockingFIFO(spi_display_BASE, (value & 0x000000FF) << 8);
    while(!GPIO_readPin(19)) { continue; }
    GPIO_writePin(124, 1);
    Semaphore_post(semDisplay);
}

void display_cmd(uint32_t cmd)
{
    Semaphore_pend(semDisplay, BIOS_WAIT_FOREVER);
    GPIO_writePin(160, 0);
    Task_sleep(2);
    GPIO_writePin(124, 0);
    SPI_writeDataBlockingFIFO(spi_display_BASE, (cmd & 0x000000FF) << 8);
    while(!GPIO_readPin(19)) { continue; }
    GPIO_writePin(124, 1);
    GPIO_writePin(160, 1);
    Task_sleep(2);
    Semaphore_post(semDisplay);
}

void display_write(uint16_t* data, size_t length)
{
    Semaphore_pend(semDisplay, 0);
    GPIO_writePin(124, 0);
    DMA_configAddresses(DMA_CH6_BASE, (const void*)&SpiaRegs.SPITXBUF, data);
    DMA_configBurst(DMA_CH6_BASE, 1, 1, 0);
    DMA_configTransfer(DMA_CH6_BASE, length, 1, 0);

    DMA_startChannel(DMA_CH6_BASE);
    DMA_forceTrigger(DMA_CH6_BASE);

    Semaphore_pend(semDisplay, 0);
}
