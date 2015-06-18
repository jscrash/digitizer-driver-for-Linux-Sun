/* DEC/CMS REPLACEMENT HISTORY, Element SX_DLG_CLPLSD.C*/
/* *2    25-JUL-1989 20:11:49 GILLESPIE "(SPR 50) Seismic system revamp"*/
/* *1    19-JUN-1989 13:29:04 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element SX_DLG_CLPLSD.C*/

/******************************************************************************

                Copyright Exploration Systems, Inc. 1989
		   Unpublished -- All rights reserved

THIS SOFTWARE IS THE PROPRIETARY PROPERTY OF Exploration Systems, Inc.  AND MAY
CONTAIN CONFIDENTIAL TRADE SECRET INFORMATION. IT IS LICENSED FOR USE ON THE
DESIGNATED EQUIPMENT ON WHICH IT WAS ORIGINALLY INSTALLED AND  MAY NOT BE
MODIFIED, DUPLICATED OR COPIED IN ANY FORM WITHOUT PRIOR WRITTEN CONSENT OF

			Exploration Systems, Inc.
			579 Market Street
			San Francisco, CA  USA 94105
			(415) 974-1600

(The above notice does not constitute an admission of publication or
unrestricted dissemination of the work)

******************************************************************************/

/*****************************************************************************/
/*                                                                           */
/* PROGRAM:  SX_DIALOG_CLP_LSDTERM.C                                         */
/* AUTHOR:   STEVE SPENCE                                                    */
/* FUNCTION: SET SHOT POINT LABEL POSITION DEFAULT TERMINATION PROGRAM       */
/*                                                                           */
/*****************************************************************************/
#include "esi_sx.h"
#include "esi_math.h"
#include "esi_gks.h"

publicdef INT sx_dialog_clp_lsdterm(pclp)
SX_CLP_STRUCT *pclp;
{
    INT status;
    FLOAT zang;

    status = SUCCESS;

    pclp->psx->pso->disp_opt.shot_label_angle = pclp->angle;
    pclp->psx->pso->disp_opt.shot_label_paraperp = pclp->paraperp;

    pclp->psx->pso->disp_opt.shot_label_default =
    pclp->psx->pso->disp_opt.zvalue_default =
         pclp->psx->pso->seismic_generic.shotpoint_label;


    so_angpp_to_text(&pclp->psx->pso->disp_opt.shot_label_default,
                      pclp->psx->pso->disp_opt.shot_label_angle,
                      pclp->psx->pso->disp_opt.shot_label_paraperp);

    zang = pclp->angle / 57.29582;

    pclp->psx->pso->disp_opt.shot_label_default.text_offset.x =
         cos(zang) * 
         pclp->psx->pso->seismic_generic.shotpoint_symbol.symbol_size * 1.5;


    pclp->psx->pso->disp_opt.shot_label_default.text_offset.y =
         sin(zang) *
         pclp->psx->pso->seismic_generic.shotpoint_symbol.symbol_size*1.5;

    pclp->psx->pso->disp_opt.zvalue_default.text_offset.x = 
        (- pclp->psx->pso->disp_opt.shot_label_default.text_offset.x);

    pclp->psx->pso->disp_opt.zvalue_default.text_offset.y =
        (- pclp->psx->pso->disp_opt.shot_label_default.text_offset.y);

    pclp->psx->pso->disp_opt.shot_label_default.box_flag =
    pclp->psx->pso->disp_opt.zvalue_default.box_flag = FALSE;

    zang = pclp->psx->pso->disp_opt.shot_label_angle + 180.0;
    if(zang >= 360.0) 
        {
        zang -= 360.0;
        }

    so_angpp_to_text(&pclp->psx->pso->disp_opt.zvalue_default,zang,
                      pclp->psx->pso->disp_opt.shot_label_paraperp);

    pclp->psx->pso->disp_opt.ntrigger_last = -999;

    so_set_shotpoint_defaults(pclp->psx->pso);

    wm_grsgwk(MAIN_WKID);
    so_draw_seismic_line(pclp->psx->pso);
    
    pclp->psx->nchanges ++;
    pclp->psx->need_to_save = TRUE;

    SX_RETURN;
}
