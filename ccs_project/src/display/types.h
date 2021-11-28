/*
 * <display/types.h>
 *
 * ST7735S Display Driver types.
 */

#ifndef INC_DISPLAY_TYPES_H_
#define INC_DISPLAY_TYPES_H_

#define DISPLAY_BST_ON        (1U)
#define DISPLAY_BST_OFF       (0U)

#define DISPLAY_MY_DECREMENT  (1U)
#define DISPLAY_MY_INCREMENT  (0U)

#define DISPLAY_MX_DECREMENT  (1U)
#define DISPLAY_MX_INCREMENT  (0U)

#define DISPLAY_MV_EXCHANGE   (1U)
#define DISPLAY_MV_NORMAL     (0U)

#define DISPLAY_ML_DECREMENT  (0U)
#define DISPLAY_ML_INCREMENT  (1U)

#define DISPLAY_RGB_RGB       (0U)
#define DISPLAY_RGB_BGR       (1U)

#define DISPLAY_MH_DECREMENT  (0U)
#define DISPLAY_MH_INCREMENT  (1U)

#define DISPLAY_IFPF_12    (0x03U)
#define DISPLAY_IFPF_16    (0x05U)
#define DISPLAY_IFPF_18    (0x06U)

#define DISPLAY_IDM_ON        (1U)
#define DISPLAY_IDM_OFF       (0U)

#define DISPLAY_PTL_ON        (1U)
#define DISPLAY_PTL_OFF       (0U)

#define DISPLAY_SLP_OUT       (1U)
#define DISPLAY_SLP_IN        (0U)

#define DISPLAY_NOR_ON        (1U)
#define DISPLAY_NOR_OFF       (0U)

#define DISPLAY_ST15_ON       (1U)
#define DISPLAY_ST15_OFF      (0U)

#define DISPLAY_INV_ON        (1U)
#define DISPLAY_INV_OFF       (0U)

#define DISPLAY_DIS_ON        (1U)
#define DISPLAY_DIS_OFF       (0U)

#define DISPLAY_TE_ON         (1U)
#define DISPLAY_TE_OFF        (0U)

#define DISPLAY_GC0        (0x01U)
#define DISPLAY_GC1        (0x02U)
#define DISPLAY_GC2        (0x04U)
#define DISPLAY_GC3        (0x08U)

#define DISPLAY_TEM_MODE2     (1U)
#define DISPLAY_TEM_MODE1     (0U)

typedef struct display_madctl_t_
{
    unsigned int padding : 2;

    /* Horizontal order
     *  '0' - Decrement (LCD refresh Left to Right)
     *  '1' - Increment (LCD refresh Right to Left)
     */
    unsigned int MH    : 1;

    /* RGB/RGB Order
     *  '1' - BGR
     *  '0' - RGB
     */
    unsigned int RGB   : 1;

    /* Scan address order
     *  '0' - Decrement (LCD refresh Top to Bottom)
     *  '1' - Increment (LCD refresh Bottom to Top)
     */
    unsigned int ML    : 1;

    /* Row/Column Exchange
     *  ‘1’ - Row/column exchange
     *  '0' - Normal
     */
    unsigned int MV    : 1;

    /* Column Address Order
     *  ‘1’ - Decrement, (Right to Left)
     *  ‘0’ - Increment, (Left to Right)
     */
    unsigned int MX    : 1;

    /* Row Address Order
     *  ‘1’ - Decrement, (Bottom to Top)
     *  '0' - Increment, (Top to Bottom)
     */
    unsigned int MY    : 1;

} display_madctl_t;

/* Display Status structure
 */
typedef struct display_status_t_
{
    /* Booster voltage status
     *  '1' - Booster on
     *  '0' - Booster off
     */
    unsigned int BST    : 1;

    /* Row Address Order
     *  ‘1’ - Decrement, (Bottom to Top)
     *  '0' - Increment, (Top to Bottom)
     */
    unsigned int MY     : 1;

    /* Column Address Order
     *  ‘1’ - Decrement, (Right to Left)
     *  ‘0’ - Increment, (Left to Right)
     */
    unsigned int MX     : 1;

    /* Row/Column Exchange
     *  ‘1’ - Row/column exchange
     *  '0' - Normal
     */
    unsigned int MV     : 1;

    /* Scan address order
     *  '0' - Decrement (LCD refresh Top to Bottom)
     *  '1' - Increment (LCD refresh Bottom to Top)
     */
    unsigned int ML     : 1;

    /* RGB/RGB Order
     *  '1' - BGR
     *  '0' - RGB
     */
    unsigned int RGB    : 1;

    /* Horizontal order
     *  '0' - Decrement (LCD refresh Left to Right)
     *  '1' - Increment (LCD refresh Right to Left)
     */
    unsigned int MH     : 1;

    unsigned int ST24   : 1; /* Reserved */
    unsigned int ST23   : 1; /* Reserved */

    /* Interface pixel format definition
     *  '0b011' - 12-bit / pixel (DISPLAY_IFPF_12)
     *  '0b101' - 16-bit / pixel (DISPLAY_IFPF_16)
     *  '0b110' - 18-bit / pixel (DISPLAY_IFPF_18)
     */
    unsigned int IFPF   : 3;

    /* Idle mode
     *  '1' - On
     *  '0' - Off
     */
    unsigned int IDM    : 1;

    /* Partial Mode
     *  '1' - On
     *  '0' - Off
     */
    unsigned int PTL    : 1;

    /* Sleep Mode
     *  '1' = Sleep Out
     *  '0' = Sleep In
     */
    unsigned int SLP    : 1;

    /* Display Normal mode
     *  '1' Normal display
     *  '0' Partial display
     */
    unsigned int NOR    : 1;

    /* Vertical Scrolling
     *  '1' Scroll on
     *  '0' Scroll off
     */
    unsigned int ST15   : 1;

    /* Horizontal Scrolling
     *  Constant '0'
     */
    unsigned int ST14   : 1;

    /* Inversion
     *  '1' - On
     *  '0' - Off
     */
    unsigned int INV    : 1;

    /* All pixels on (Unused)
     *  Constant '0'
     */
    unsigned int ST12 : 1;

    /* All pixels off (Unused)
     *  Constant '0'
     */
    unsigned int ST11  : 1;

    /* Display Power
     *  '1' - On
     *  '0' - Off
     */
    unsigned int DIS   : 1;

    /* Tearing effect line
     *  '1' - On
     *  '0' - Off
     */
    unsigned int TE    : 1;

    /* Gamma Curve Selection
     *  '0b000' - GC0 (DISPLAY_GC0)
     *  '0b001' - GC1 (DISPLAY_GC1)
     *  '0b010' - GC2 (DISPLAY_GC2)
     *  '0b011' - GC3 (DISPLAY_GC3)
     *  '0bxxx' - Undefined
     */
    unsigned int GCSEL : 3;

    /* Tearing effect line mode
     *  '0' - Mode1
     *  '1' - Mode2
     */
    unsigned int TEM   : 1;

    unsigned int ST4   : 1; /* Reserved */
    unsigned int ST3   : 1; /* Reserved */
    unsigned int ST2   : 1; /* Reserved */
    unsigned int ST1   : 1; /* Reserved */
    unsigned int ST0   : 1; /* Reserved */

} display_status_t;

#define DISPLAY_NLX_DOT     (0U)
#define DISPLAY_NLX_COLUMN  (1U)

/* Use DISPLAY_NLX_* for fields in here */
typedef struct display_invctr_t_
{
    /* Inversion setting in full colors partial mode (Partial mode on / Idle mode off) */
    unsigned int NLC      : 1;

    /* Inversion setting in Idle mode (Idle mode on)
     */
    unsigned int NLB      : 1;

    /* Inversion setting in full colors normal mode (Normal mode on)
     */
    unsigned int NLA      : 1;

    unsigned int padding1 : 5;

} display_invctr_t;

#define DISPLAY_AVDD_4V5  (0x00U)
#define DISPLAY_AVDD_4V6  (0x01U)
#define DISPLAY_AVDD_4V7  (0x02U)
#define DISPLAY_AVDD_4V8  (0x03U)
#define DISPLAY_AVDD_4V9  (0x04U)
#define DISPLAY_AVDD_5V0  (0x05U)
#define DISPLAY_AVDD_5V1  (0x06U)
#define DISPLAY_AVDD_5V2  (0x07U)

#define DISPLAY_POWER_MODE_2X       (0x0U)
#define DISPLAY_POWER_MODE_TESTING1 (0x1U)
#define DISPLAY_POWER_MODE_AUTO     (0x2U)
#define DISPLAY_POWER_MODE_TESTING2 (0x3U)

typedef struct display_pwctr1_t_
{
    unsigned int VRHP5    : 1; /* Refer to page 163 of datasheet */
    unsigned int VRHN5    : 1; /* Refer to page 163 of datasheet */
    unsigned int padding2 : 4;
    unsigned int MODE     : 2;
    unsigned int VRHN     : 5; /* Refer to page 163 of datasheet */
    unsigned int padding1 : 3;
    unsigned int VRHP     : 5; /* Refer to page 163 of datasheet */
    unsigned int AVDD     : 3;

} display_pwctr1_t;

#define DISPLAY_VGH25_2V1 (0x0U)
#define DISPLAY_VGH25_2V2 (0x1U)
#define DISPLAY_VGH25_2V3 (0x2U)
#define DISPLAY_VGH25_2V4 (0x3U)

#define DISPLAY_VGLSEL_7V5  (0x0U)
#define DISPLAY_VGLSEL_10V0 (0x1U)
#define DISPLAY_VGLSEL_12V5 (0x2U)
#define DISPLAY_VGLSEL_13V0 (0x3U)

typedef struct display_pwctr2_t_
{
    unsigned int VGHBT    : 2; /* refer to page 165 of datasheet */
    unsigned int VGLSEL   : 2;
    unsigned int padding1 : 2;
    unsigned int VGH25    : 2;

} display_pwctr2_t;

#define DISPLAY_DC98_DIV1 (0x0U)
#define DISPLAY_DC98_DIV3 (0x1U)
#define DISPLAY_DC98_DIV2 (0x2U)
#define DISPLAY_DC98_DIV4 (0x3U)

#define DISPLAY_DC76_DIV3 (0x0U)
#define DISPLAY_DC76_DIV1 (0x1U)
#define DISPLAY_DC76_DIV4 (0x2U)
#define DISPLAY_DC76_DIV2 (0x3U)

#define DISPLAY_DC54_DIV1 (0x0U)
#define DISPLAY_DC54_DIV3 (0x1U)
#define DISPLAY_DC54_DIV2 (0x2U)
#define DISPLAY_DC54_DIV4 (0x3U)

#define DISPLAY_DC32_DIV1 (0x0U)
#define DISPLAY_DC32_DIV3 (0x1U)
#define DISPLAY_DC32_DIV2 (0x2U)
#define DISPLAY_DC32_DIV4 (0x3U)

#define DISPLAY_DC10_DIV1 (0x0U)
#define DISPLAY_DC10_DIV3 (0x1U)
#define DISPLAY_DC10_DIV2 (0x2U)
#define DISPLAY_DC10_DIV4 (0x3U)

#define DISPLAY_AP_OP           (0x0U)
#define DISPLAY_AP_SMALL        (0x1U)
#define DISPLAY_AP_MEDIUM_LOW   (0x2U)
#define DISPLAY_AP_MEDIUM       (0x3U)
#define DISPLAY_AP_MEDIUM_HIGH  (0x4U)
#define DISPLAY_AP_LARGE        (0x5U)

#define DISPLAY_SAP_OP          (0x0U)
#define DISPLAY_SAP_SMALL       (0x1U)
#define DISPLAY_SAP_MEDIUM_LOW  (0x2U)
#define DISPLAY_SAP_MEDIUM      (0x3U)
#define DISPLAY_SAP_MEDIUM_HIGH (0x4U)
#define DISPLAY_SAP_LARGE       (0x5U)

typedef struct display_pwctrn_t_
{
    unsigned int DC10     : 2; /* Booster circuit Step-up cycle in Normal mode/ full colors. */
    unsigned int DC32     : 2; /* Booster circuit Step-up cycle in Normal mode/ full colors. */
    unsigned int DC54     : 2; /* Booster circuit Step-up cycle in Normal mode/ full colors. */
    unsigned int DC76     : 2; /* Booster circuit Step-up cycle in Normal mode/ full colors. */

    unsigned int AP       : 3;

    /* Amount of fixed current from the fixed current source in the operational amplifier for the
     * source driver.
     */
    unsigned int SAP      : 3;

    unsigned int DC98     : 2; /* Booster circuit Step-up cycle in Normal mode/ full colors. */

} display_pwctrn_t;


#endif /* INC_DISPLAY_TYPES_H_ */
