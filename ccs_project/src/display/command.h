/*
 * <display/command.h>
 *
 * ST7735S Display Driver commands
 */

#ifndef DRIVER_COMMAND_COMMAND_H_
#define DRIVER_COMMAND_COMMAND_H_

#pragma CHECK_MISRA("none")
#include <stdint.h>
#pragma RESET_MISRA("all")

#include "display.h"
#include "types.h"

enum display_cmd
{
    DISPLAY_CMD_NOP     = 0x00U,
    DISPLAY_CMD_SWRESET = 0x01U,
    DISPLAY_CMD_SLPIN   = 0x10U,
    DISPLAY_CMD_SLPOUT  = 0x11U,
    DISPLAY_CMD_PTLON   = 0x12U,
    DISPLAY_CMD_NORON   = 0x13U,
    DISPLAY_CMD_IVNOFF  = 0x20U,
    DISPLAY_CMD_IVNON   = 0x21U,
    DISPLAY_CMD_GAMSET  = 0x26U,
    DISPLAY_CMD_DISPOFF = 0x28U,
    DISPLAY_CMD_DISPON  = 0x29U,
    DISPLAY_CMD_CASET   = 0x2AU,
    DISPLAY_CMD_RASET   = 0x2BU,
    DISPLAY_CMD_RAMWR   = 0x2CU,
    DISPLAY_CMD_RGBSET  = 0x2DU,
    DISPLAY_CMD_PTLAR   = 0x30U,
    DISPLAY_CMD_SCRLAR  = 0x33U,
    DISPLAY_CMD_TEOFF   = 0x34U,
    DISPLAY_CMD_TEON    = 0x35U,
    DISPLAY_CMD_COLMOD  = 0x3AU,
    DISPLAY_CMD_MADCTL  = 0x36U,
    DISPLAY_CMD_VSCSAD  = 0x37U,
    DISPLAY_CMD_IDMOFF  = 0x38U,
    DISPLAY_CMD_IDMON   = 0x39U,
    DISPLAY_CMD_FRMCTR1 = 0xB1U,
    DISPLAY_CMD_FRMCTR2 = 0xB2U,
    DISPLAY_CMD_FRMCTR3 = 0xB3U,
    DISPLAY_CMD_INVCTR  = 0xB4U,
    DISPLAY_CMD_PWCTR1  = 0xC0U,
    DISPLAY_CMD_PWCTR2  = 0xC1U,
    DISPLAY_CMD_PWCTR3  = 0xC2U,
    DISPLAY_CMD_PWCTR4  = 0xC3U,
    DISPLAY_CMD_PWCTR5  = 0xC4U,
    DISPLAY_CMD_VMCTR1  = 0xC5U,
    DISPLAY_CMD_VMOFCTR = 0xC7U,
    DISPLAY_CMD_WRID2   = 0xD1U,
    DISPLAY_CMD_WRID3   = 0xD2U,
};

/** @brief No operation. */
void display_command_nop(void);

/** @brief Software Reset
 * - If Software Reset is applied during Sleep In mode, it will be necessary to wait 120msec before
 *   sending next command.
 * - The display module loads all default values to the registers during 120msec.
 * - If Software Reset is applied during Sleep Out or Display On Mode, it will be necessary to wait
 *   120msec before sending next command.
 */
void display_command_swreset(void);

/** @brief Sleep in
 *
 * - This command causes the LCD module to enter the minimum power consumption mode.
 * - In this mode the DC/DC converter is stopped, Internal display oscillator is stopped, and panel
 *   scanning is stopped.
 *
 * Notes
 * - This command has no effect when module is already in Sleep In mode. Sleep In Mode can only
 *   be exit by the Sleep Out Command (11h).
 * - When IC is in Sleep Out or Display On mode, it is necessary to wait 120msec before sending
 *   next command because of the stabilization timing for the supply voltages and clock circuits.
 *
 * This command includes the 120msec wait.
 */
void display_command_slpin(void);

/** @brief Sleep out
 *
 * - This command turns off sleep mode.
 * - In this mode the DC/DC converter is enabled, Internal display oscillator is started, and panel
 *   scanning is started.
 *
 * Notes
 * - This command has no effect when module is already in sleep out mode. Sleep Out Mode can
 *   only be exit by the Sleep In Command.
 * - When IC is in Sleep In mode, it is necessary to wait 120msec before sending next command
 *   because of the stabilization timing for the supply voltages and clock circuits.
 * - When IC is in Sleep Out or Display On mode, it is necessary to wait 120msec before sending
 *   next command due to the download of default value of registers and the execution of
 *   self-diagnostic function.
 *
 * This command includes the 120msec wait.
 */
void display_command_slpout(void);

/** @brief Partial Display Mode On
 *
 * - This command turns on Partial mode. The partial mode window is described by the Partial
 *   Area command
 * - Exit from Partial Display Mode by calling display_command_noron()
 */
void display_command_ptlon(void);

/** @brief Normal Display Mode On
 *
 * - This command returns the display to normal mode.
 * - Normal display mode on means Partial mode off.
 * - Exit from Normal Display Mode by calling display_command_ptlon()
 */
void display_command_noron(void);

/** @brief Display Inversion Off
 *
 * This command is used to recover from display inversion mode.
 */
void display_command_ivnoff(void);

/** @brief Display Inversion On
 *
 * - This command is used to enter into display inversion mode
 * - Exit from Display Inversion On by calling display_command_invoff()
 */
void display_command_ivnon(void);

/** @brief Gamma Set
 *
 * - This command is used to select the desired Gamma curve for the current display. A maximum
 *   of 4 curves can be selected. The curve is selected by setting the appropriate bit in the
 *   parameter as described in the Table.
 */
void display_command_gamset(Uint32 gc);

/** @brief Display Off
 *
 * - This command is used to enter into DISPLAY OFF mode. In this mode, the output from
 *   Frame Memory is disabled and blank page inserted.
 * - This command makes no change of contents of frame memory.
 * - This command does not change any other status.
 * - There will be no abnormal visible effect on the display.
 */
void display_command_dispoff(void);

/** @brief Display On
 *
 * - This command is used to recover from DISPLAY OFF mode.
 * - Output from the Frame Memory is enabled.
 * - This command makes no change of contents of frame memory.
 * - This command does not change any other status.
 */
void display_command_dispon(void);

/** @brief Column Address Set
 *
 * - The value of XS [7:0] and XE [7:0] are referred when RAMWR command comes.
 * - Each value represents one column line in the Frame Memory.
 *
 * Notes:
 * - XS [15:0] always must be equal to or less than XE [15:0]
 *   When XS [15:0] or XE [15:0] is greater than maximum address like below, data of out of range
 *   will be ignored.
 * 1. 128X160 memory base (GM = ’11’)
 * (Parameter range: 0 < XS [15:0] < XE [15:0] < 127 (007Fh)): MV=”0”)
 * (Parameter range: 0 < XS [15:0] < XE [15:0] < 159 (009Fh)): MV=”1”)
 * 2. 132X132 memory base (GM = ’01’)
 * (Parameter range: 0 < XS [15:0] < XE [15:0] < 131 (0083h)): MV=”0”)
 * (Parameter range: 0 < XS [15:0] < XE [15:0] < 131 (0083h)): MV=”1”)
 * 3. 132X162 memory base (GM = ’00’)
 * (Parameter range: 0 < XS [15:0] < XE [15:0] < 131 (0083h)): MV=”0”)
 * (Parameter range: 0 < XS [15:0] < XE [15:0] < 161 (00A1h)): MV=”1”)
 */
void display_command_caset(Uint32 xs, Uint32 xe);

/** @brief Row Address Set
 *
 * - The value of YS [7:0] and YE [7:0] are referred when RAMWR command comes.
 * - Each value represents one column line in the Frame Memory.
 *
 * Notes:
 * - YS [15:0] always must be equal to or less than YE [15:0]
 *   When YS [15:0] or YE [15:0] is greater than maximum address like below, data of out of range
 *   will be ignored.
 * 1. 128X160 memory base (GM = ’11’)
 *    (Parameter range: 0 < XS [15:0] < XE [15:0] < 127 (007Fh)): MV=”0”)
 *    (Parameter range: 0 < XS [15:0] < XE [15:0] < 159 (009Fh)): MV=”1”)
 * 2. 132X132 memory base (GM = ’01’)
 *    (Parameter range: 0 < XS [15:0] < XE [15:0] < 131 (0083h)): MV=”0”)
 *    (Parameter range: 0 < XS [15:0] < XE [15:0] < 131 (0083h)): MV=”1”)
 * 3. 132X162 memory base (GM = ’00’)
 *    (Parameter range: 0 < XS [15:0] < XE [15:0] < 131 (0083h)): MV=”0”)
 *    (Parameter range: 0 < XS [15:0] < XE [15:0] < 161 (00A1h)): MV=”1”)
 */
void display_command_raset(Uint32 ys, Uint32 ye);

/** @brief Memory write
 *
 * In all color modes, there is no restriction on length of parameters.
 * 1. 128X160 memory base (GM = ‘11’)
 *    128x160x18-bit memory can be written by this command
 *    Memory range: (0000h, 0000h) -> (007Fh, 09Fh)
 * 2. 132x132 memory base (GM = ‘01’)
 *    132x132x18-bit memory can be written on this command.
 *    Memory range: (0000h, 0000h) -> (0083h, 0083h)
 * 3. 132x162 memory base (GM = ‘00’)
 *    132x162x18-bit memory can be written on this command.
 *    Memory range: (0000h, 0000h) -> (0083h, 00A1h)
 *
 * This command should be followed by calls to display_write_multiple.
 */
void display_command_ramwr(void);

/** @brief Color Set for 4K, 65K, 262K and 16.7M
 *
 * This command is used to define the look-up table for 12bits-to-16bits / 16-bit-to-18bits color depth
 * conversations.
 * 128-Bytes must be written to the LUT regardless of the color mode. Only the values in
 * Section 9.18 are referred.
 * In this condition, 4K-color (4-4-4) and 65K-color(5-6-5) data input are transferred
 * 6(R)-6(G)-6(B) through RGB LUT table.
 * This command has no effect on other commands/parameters and Contents of frame
 * memory.
 * Visible change takes effect next time the Frame Memory is written to.
 * Do not send any command before the last data is sent or LUT is not defined correctly.
 *
 * This command should be followed by calls to display_write_* to fill in the table
 *
 */
void display_command_rgbset(void);

/** @brief Partial Area
 *
 * - This command defines the partial mode’s display area.
 * - There are 4 parameters associated with this command, the first defines the Start Row (PSL) and
 *   the second the End Row (PEL), as illustrated in the figures below. PSL and PEL refer to the
 *   Frame Memory row address counter.
 * - See datasheet pg. 135 for more info
 */
void display_command_ptlar(Uint32 psl, Uint32 pel);

/** @brief Scroll Area Set
 *
 * - This command just defines the Vertical Scrolling Area of the display and not performs vertical
 *   scroll
 * - The 1sd & 2nd parameter TFA [15:0] describes the Top Fixed Area (in No. of lines from Top of
 *   the Frame Memory and Display).
 * - The 3rd & 4th parameter VSA [15:0] describes the height of the Vertical Scrolling Area (in No. of
 *   lines of the Frame Memory [not the display] from the Vertical Scrolling Start Address) The first
 *   line appears immediately after the bottom most line of the Top Fixed Area.
 * - The 4th & 5th parameter BFA [6:0] describes the Bottom Fixed Area (in No. of lines from Bottom
 *   of the Frame Memory and Display).
 *   TFA, VSA and BFA refer to the Frame Memory Line Pointer
 *
 * Notes:
 *
 * The condition is (TFA+VSA+BFA) = 162, otherwise Scrolling mode is undefined.
 * In Vertical Scroll Mode, MADCTR parameter MV should be set to ‘0’-this only affects the Frame
 * Memory Write.
 * TFA[15:0], VSA[15:0] and BFA[15:0] is based on line unit.
 * TFA[15:0]= 0000h, 0001h, 0002h, 0003h, ... , 00A2h
 * VSA[15:0]= 0000h, 0001h, 0002h, 0003h, ... , 00A2h
 * BFA[15:0]= 0000h, 0001h, 0002h, 0003h, ... , 00A2h
 */
void display_command_scrlar(Uint32 tfa, Uint32 vsa, Uint32 bfa);

/** @brief Tearing Effect Line Off
 *
 * - This command is used to turn OFF (Active Low) the Tearing Effect output signal from the TE signal
 *   line.
 */
void display_command_teoff(void);

/** @brief Tearing Effect Line On
 *
 * - This command is used to turn ON the Tearing Effect output signal from the TE signal line.
 * - This output is not affected by changing MADCTL bit ML.
 * - The Tearing Effect Line On has one parameter, which describes the mode of the Tearing
 * Effect Output Line:
 * - When TEM =’0’: The Tearing Effect output line consists of V-Blanking information only
 *   Vertical time scale
 * - When TEM =’1’: The Tearing Effect output Line consists of both V-Blanking and H-Blanking
 *   information
 *
 * Note: During Sleep In Mode with Tearing Effect Line On, Tearing Effect Output pin will be
 * active Low.
 */
void display_command_teon(void);

/** @brief Memory Data Access Control
 *
 * This command defines read/ write scanning direction of frame memory
 */
void display_command_madctl(display_madctl_t ctl);

/** @brief Scroll Area
 *
 * - This command is used together with Vertical Scrolling Definition (33h).
 * - These two commands describe the scrolling area and the scrolling mode.
 * - The Vertical Scrolling Start Address command has one parameter which describes which line in
 *   the Frame Memory will be written as the first line after the last line of the Top Fixed Area on the
 *   display as illustrated below:
 * - This command Start the scrolling.
 * - Exit from V-scrolling mode by commands Partial mode On (12h) or Normal mode On (13h)
 *
 * Notes
 * - When new Pointer position and Picture Data are sent, the result on the display will happen
 *   at the next Panel Scan to avoid tearing effect.
 *   SSA refers to the Frame Memory line Pointer
 * - Since the value of the Vertical Scrolling Start Address is absolute (with reference to the Frame
 *   Memory), it must not enter the fixed area (defined by Vertical Scrolling Definition (33h)-otherwise
 *   undesirable image will be displayed on the Panel.
 *   SSA [6:0] is based on line unit.
 *   SSA [6:0] = 00h, 01h, 02h, 03h, ... , A1h
 */
void display_command_vscsad(Uint32 ssa);

/** @brief Idle Mode Off
 *
 * - This command is used to recover from Idle mode on.
 * - In the idle off mode,
 *   1. LCD can display 4096, 65k or 262k colors.
 *   2. Normal frame frequency is applied.
 */
void display_command_idmoff(void);

/** @brief Idle Mode On
 *
 * - This command is used to enter into Idle mode on.
 * - There will be no abnormal visible effect on the display mode change transition.
 * - In the idle on mode,
 *   1. Color expression is reduced. The primary and the secondary colors using MSB of each R,G
 *      and B in the Frame Memory, 8 color depth data is displayed.
 *   2. 8-Color mode frame frequency is applied.
 *   3. Exit from IDMON by Idle Mode Off (38h) command
 */
void display_command_idmon(void);

/** @brief Interface Pixel Format
 *
 * This command is used to define the format of RGB picture data, which is to be transferred via
 * the MCU interface. The formats are shown in the table:
 * Notes
 * - In 12-bit/Pixel, 16-bit/Pixel or 18-bit/Pixel mode, the LUT is applied to transfer data into
 *   the Frame Memory.
 * - This Command should be set at 55h when writing 16-bit/pixel data into frame
 *   memory, but should be re-set to 66h when reading pixel data from frame memory.
 *   Please check the LUT in chapter 9.17 of the datasheet when using memory read function.
 *
 * Use DISPLAY_IFPF_* constants here
 */
void display_command_colmod(Uint32 ifpf);

/** @brief Frame Rate Control (In normal mode/ Full colors)
 *
 * - Set the frame frequency of the full colors normal mode.
 * - Frame rate=fosc/((RTNA x 2 + 40) x (LINE + FPA + BPA +2))
 * - fosc = 850kHz
 * - FPA > 0, BPA > 0
 */
void display_command_frmctr1(Uint32 rtna, Uint32 fpa, Uint32 bpa);

/** @brief Frame Rate Control (In Idle mode/ 8-colors)
 *
 * - Set the frame frequency of the Idle mode.
 * - Frame rate=fosc/((RTNA x 2 + 40) x (LINE + FPB + BPB +2))
 * - fosc = 850kHz
 * - FPB > 0, BPB > 0
 */
void display_command_frmctr2(Uint32 rtnb, Uint32 fpb, Uint32 bpb);

/** @brief Frame Rate Control (In Partial mode/ full colors)
 *
 * - Set the frame frequency of the Partial mode/ full colors.
 * - 1st parameter to 3rd parameter are used in dot inversion mode.
 * - 4th parameter to 6th parameter are used in column inversion mode.
 * - Frame rate=fosc/((RTNA x 2 + 40) x (LINE + FPC + BPC +2))
 * - fosc = 850kHz
 * - FPC > 0, BPC > 0
 */
void display_command_frmctr3(Uint32 rtnc, Uint32 fpc, Uint32 bpc, Uint32 rtnd, Uint32 fpd, Uint32 bpd);

/** @brief Display Inversion Control
 */
void display_command_invctr(display_invctr_t ctl);

/** @brief Power Control 1
 */
void display_command_pwctr1(display_pwctr1_t power);

/** @brief Power Control 2
 *
 * Set the VGH and VGL supply power level
 *
 * - The deviation value of VGH/ VGL between with Measurement and Specification: Max <= 1V
 * - VGH-VGL <= 32V
 */
void display_command_pwctr2(display_pwctr2_t power);

/** @brief Power Control 3 (in Normal mode/ Full colors)
 */
void display_command_pwctr3(display_pwctrn_t power);

/** @brief Power Control 4 (in Idle mode/ 8-colors)
 */
void display_command_pwctr4(display_pwctrn_t power);

/** @brief Power Control 5 (in Partial mode/ full-colors)
 */
void display_command_pwctr5(display_pwctrn_t power);

/** @brief VCOM Control 1
 *
 * VCOM voltage setting.
 *
 * See datasheet pg. 173 for values
 */
void display_command_vmctr1(Uint32 vcoms);

/** @brief VCOM Offset Control
 *
 * - Set VCOM Voltage level for reduce the flicker issue
 * - Before using, the bit VMF_EN of NVFCTR1 must be enabled
 *
 * See pg. 175 of datasheet for values
 */
void display_command_vmofctr(Uint32 vmf);

/** @brief Write ID2 Value
 *
 * - Write 7-bit data of LCD module version to save it to NVM.
 * - The parameter ID2[6:0] is LCD Module version ID.
 */
void display_command_wrid2(Uint32 id2);

/** @brief Write ID3 Value
 *
 * - Write 8-bit data of project code module to save it to NVM.
 * - The parameter ID3[7:0] is product project ID.
 */
void display_command_wrid3(Uint32 id3);

#endif /* DRIVER_COMMAND_COMMAND_H_ */
