/*
 * command.c
 *
 * ST7735S Command implementations.
 */

#include "command.h"
#include "display.h"

#include <ti/sysbios/knl/Task.h>

display_status_t display_status;

extern void DelayUs(Uint16);

void display_command_nop(void)
{
    display_cmd(DISPLAY_CMD_NOP);
}

void display_command_swreset(void)
{
    display_cmd(DISPLAY_CMD_SWRESET);
    *((Uint32*)&display_status) = ((Uint32)0x00010000) | (*((Uint32*)&display_status) & ((Uint32)0x7C700000));
}

void display_command_slpin(void)
{
    display_cmd(DISPLAY_CMD_SLPIN);
    display_status.SLP = DISPLAY_SLP_IN;

    DelayUs(1000);
}

void display_command_slpout(void)
{
    display_cmd(DISPLAY_CMD_SLPOUT);
    display_status.SLP = DISPLAY_SLP_OUT;

    DelayUs(1000);
}

void display_command_ptlon(void)
{
    display_cmd(DISPLAY_CMD_PTLON);
    display_status.PTL = DISPLAY_PTL_ON;
    display_status.NOR = DISPLAY_NOR_OFF;
}

void display_command_noron(void)
{
    display_cmd(DISPLAY_CMD_NORON);
    display_status.PTL = DISPLAY_PTL_OFF;
    display_status.NOR = DISPLAY_NOR_ON;
}

void display_command_ivnoff(void)
{
    display_cmd(DISPLAY_CMD_IVNOFF);
    display_status.INV = DISPLAY_INV_OFF;
}

void display_command_ivnon(void)
{
    display_cmd(DISPLAY_CMD_IVNON);
    display_status.INV = DISPLAY_INV_ON;
}

void display_command_gamset(Uint32 gc)
{
    display_cmd(DISPLAY_CMD_GAMSET);
    display_write8(gc);
    display_status.GCSEL = gc;
}

void display_command_dispoff(void)
{
    display_cmd(DISPLAY_CMD_DISPOFF);
    display_status.DIS = DISPLAY_DIS_OFF;
}

void display_command_dispon(void)
{
    display_cmd(DISPLAY_CMD_DISPON);
    display_status.DIS = DISPLAY_DIS_ON;
}

void display_command_caset(Uint32 xs, Uint32 xe)
{
    display_cmd(DISPLAY_CMD_CASET);
    display_write16(xs);
    display_write16(xe);
}

void display_command_raset(Uint32 ys, Uint32 ye)
{
    display_cmd(DISPLAY_CMD_RASET);
    display_write16(ys);
    display_write16(ye);
}

void display_command_ramwr(void)
{
    display_cmd(DISPLAY_CMD_RAMWR);
}

void display_command_rgbset(void)
{
    display_cmd(DISPLAY_CMD_RGBSET);
}

void display_command_ptlar(Uint32 psl, Uint32 pel)
{
    display_cmd(DISPLAY_CMD_PTLAR);
    display_write16(psl);
    display_write16(pel);
}

void display_command_scrlar(Uint32 tfa, Uint32 vsa, Uint32 bfa)
{
    display_cmd(DISPLAY_CMD_SCRLAR);
    display_write16(tfa);
    display_write16(vsa);
    display_write16(bfa);
}

void display_command_teoff(void)
{
    display_cmd(DISPLAY_CMD_TEOFF);
    display_status.TE = DISPLAY_TE_OFF;
}

void display_command_teon(void)
{
    display_cmd(DISPLAY_CMD_TEON);
    display_status.TE = DISPLAY_TE_ON;
}

void display_command_colmod(Uint32 ifpf)
{
    display_cmd(DISPLAY_CMD_COLMOD);
    display_write8(ifpf);
}

void display_command_madctl(display_madctl_t ctl)
{
    display_cmd(DISPLAY_CMD_MADCTL);
    uint32_t ccc = *(uint32_t*)&ctl;
    display_write8(*((Uint32*)&ctl));
}

void display_command_vscsad(Uint32 ssa)
{
    display_cmd(DISPLAY_CMD_VSCSAD);
    display_write8(0x00U);
    display_write8(ssa);
}

void display_command_idmoff(void)
{
   display_cmd(DISPLAY_CMD_IDMOFF);
   display_status.IDM = DISPLAY_IDM_OFF;
}

void display_command_idmon(void)
{
   display_cmd(DISPLAY_CMD_IDMON);
   display_status.IDM = DISPLAY_IDM_ON;
}

void display_command_frmctr1(Uint32 rtna, Uint32 fpa, Uint32 bpa)
{
    display_cmd(DISPLAY_CMD_FRMCTR1);
    display_write8(rtna);
    display_write8(fpa);
    display_write8(bpa);
}

void display_command_frmctr2(Uint32 rtnb, Uint32 fpb, Uint32 bpb)
{
    display_cmd(DISPLAY_CMD_FRMCTR2);
    display_write8(rtnb);
    display_write8(fpb);
    display_write8(bpb);
}

void display_command_frmctr3(Uint32 rtnc, Uint32 fpc, Uint32 bpc, Uint32 rtnd, Uint32 fpd, Uint32 bpd)
{
    display_cmd(DISPLAY_CMD_FRMCTR3);
    display_write8(rtnc);
    display_write8(fpc);
    display_write8(bpc);
    display_write8(rtnd);
    display_write8(fpd);
    display_write8(bpd);
}

void display_command_invctr(display_invctr_t ctl)
{
    display_cmd(DISPLAY_CMD_INVCTR);
    display_write8(*((Uint32*)&ctl));
}

void display_command_pwctr1(display_pwctr1_t power)
{
    display_cmd(DISPLAY_CMD_PWCTR1);

    power.padding1 = ((Uint32)0);
    power.padding2 = ((Uint32)1);

    display_write16(*((Uint32*)&power));
    display_write8 (*(( Uint32*)&power) + 2U);
}

void display_command_pwctr2(display_pwctr2_t power)
{
    display_cmd(DISPLAY_CMD_PWCTR2);
    display_write8 (*(( Uint32*)&power));
}

void display_command_pwctr3(display_pwctrn_t power)
{
    display_cmd(DISPLAY_CMD_PWCTR3);
    display_write16(*((Uint32*)&power));
}

void display_command_pwctr4(display_pwctrn_t power)
{
    display_cmd(DISPLAY_CMD_PWCTR4);
    display_write16(*((Uint32*)&power));
}

void display_command_pwctr5(display_pwctrn_t power)
{
    display_cmd(DISPLAY_CMD_PWCTR5);
    display_write16(*((Uint32*)&power));
}

void display_command_vmctr1(Uint32 vcoms)
{
    display_cmd(DISPLAY_CMD_VMCTR1);
    display_write8(vcoms);
}

void display_command_vmofctr(Uint32 vmf)
{
    display_cmd(DISPLAY_CMD_VMOFCTR);
    display_write8(vmf);
}

void display_command_wrid2(Uint32 id2)
{
    display_cmd(DISPLAY_CMD_WRID3);
    display_write8(id2);
}

void display_command_wrid3(Uint32 id3)
{
    display_cmd(DISPLAY_CMD_WRID3);
    display_write8(id3);
}
