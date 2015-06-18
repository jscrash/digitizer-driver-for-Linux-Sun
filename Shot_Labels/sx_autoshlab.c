/* DEC/CMS REPLACEMENT HISTORY, Element SX_AUTOSHLAB.C*/
/* *3    23-AUG-1990 17:21:56 JESSIE "(SPR 1) fix include files"*/
/* *2    25-JUL-1989 20:09:41 GILLESPIE "(SPR 50) Seismic system revamp"*/
/* *1    19-JUN-1989 13:28:09 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element SX_AUTOSHLAB.C*/

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

/***********************************************************************/
/*                                                                     */
/* NAME:     SX_AUTO_SHOT_LABELS.PC                                    */
/* AUTHOR:   STEVE SPENCE                                              */
/* FUNCTION: FUNCTION TO SET DEFAULT SHOT LABEL POSITIONS              */
/*                                                                     */
/***********************************************************************/

#include "esi_sx.h"
#ifndef ESI_GKS_H
#include "esi_gks.h"
#endif
#ifndef ESI_SO_H
#include "esi_so.h"
#endif
#include "esi_math.h"
#ifndef ESI_WM_H
#include "esi_wm.h"
#endif

publicdef INT sx_auto_shot_labels(psx)

SX_STRUCT *psx;
{

    INT   status;
    DOUBLE zang,slope;
    FLOAT x1,y1,x2,y2;/* 4 byte floats */
    UINT   dim_list[2];
    VOIDPTR value_list[2];
    SO_STRUCT *pso = psx->pso;

    DOUBLE  shot_label_angle;
    INT     shot_label_paraperp = SO_PERP;

    status = SUCCESS;


    /* erase line and redraw segments */

    wm_grsgwk(MAIN_WKID);

    /* default line label position data to determine slope for
       default shot label position */

    dim_list[0]=2;
    dim_list[1]=3;
    value_list[0] = (VOIDPTR) &x1;
    value_list[1] = (VOIDPTR) &y1;
    nl_first_point(pso->cdp_xy,2,dim_list,value_list);
    value_list[0] = (VOIDPTR) &x2;
    value_list[1] = (VOIDPTR) &y2;
    nl_last_point(pso->cdp_xy,2,dim_list,value_list);

    if(x2 != x1)
        {
        slope = (y2-y1)/(x2-x1);
        zang = atan ( slope );
        }
    else
        {
        zang = PI / 2.0;
        }

    /* shot labels defaults */

    shot_label_angle = zang * 57.29582 + 90.0;
    if(shot_label_angle < 0.0)
        {
        shot_label_angle += 360.0;
        }

    zang = shot_label_angle/57.295827;

    pso->disp_opt.shot_label_angle = shot_label_angle;
    pso->disp_opt.shot_label_paraperp = shot_label_paraperp;

    pso->disp_opt.shot_label_default =
    pso->disp_opt.zvalue_default =
         pso->seismic_generic.shotpoint_label;


    so_angpp_to_text(&pso->disp_opt.shot_label_default,
                      pso->disp_opt.shot_label_angle,
                      pso->disp_opt.shot_label_paraperp);

    pso->disp_opt.shot_label_default.text_offset.x =
         cos(zang) * pso->seismic_generic.shotpoint_symbol.symbol_size * 1.5;


    pso->disp_opt.shot_label_default.text_offset.y =
         sin(zang) * pso->seismic_generic.shotpoint_symbol.symbol_size*1.5;

    pso->disp_opt.zvalue_default.text_offset.x = 
        (- pso->disp_opt.shot_label_default.text_offset.x);

    pso->disp_opt.zvalue_default.text_offset.y =
        (- pso->disp_opt.shot_label_default.text_offset.y);

    pso->disp_opt.shot_label_default.box_flag =
    pso->disp_opt.zvalue_default.box_flag = FALSE;

    zang = pso->disp_opt.shot_label_angle + 180.0;
    if(zang >= 360.0) 
        {
        zang -= 360.0;
        }

    so_angpp_to_text(&pso->disp_opt.zvalue_default,zang,
                      pso->disp_opt.shot_label_paraperp);

    pso->disp_opt.ntrigger_last = -999;

    so_set_shotpoint_defaults(pso);

    so_draw_seismic_line(pso);

    SX_RETURN;
}
