/* DEC/CMS REPLACEMENT HISTORY, Element S3_DRW_SYMBOL.C*/
/* *4    17-AUG-1990 22:13:52 VINCE "(SPR 5644) Code Cleanup"*/
/* *3    18-JUN-1990 12:06:26 GILLESPIE "(SPR 1) Changes to allow 3-D lines to be handles as select list entryes (IES deltas)"*/
/* *2    28-JUL-1989 12:17:16 GILLESPIE "(SPR 1) Fix minor casting problems"*/
/* *1    19-JUN-1989 13:23:07 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element S3_DRW_SYMBOL.C*/
/* DEC/CMS REPLACEMENT HISTORY, Element S3_DRW_SYMBOL.C                             */
/* *2    28-JUL-1989 12:17:16 GILLESPIE "(SPR 1) Fix minor casting problems"        */
/* *1    19-JUN-1989 13:23:07 SYSTEM ""                                             */
/* DEC/CMS REPLACEMENT HISTORY, Element S3_DRW_SYMBOL.C                             */
/*****************************************************************************/
/******************************************************************************

                Copyright Finder Graphics Systems, Inc. 1990
                    Unpublished -- All rights reserved

THIS SOFTWARE IS THE PROPRIETARY PROPERTY OF Exploration Systems, Inc.  AND MAY
CONTAIN CONFIDENTIAL TRADE SECRET INFORMATION. IT IS LICENSED FOR USE ON THE
DESIGNATED EQUIPMENT ON WHICH IT WAS ORIGINALLY INSTALLED AND  MAY NOT BE
MODIFIED, DUPLICATED OR COPIED IN ANY FORM WITHOUT PRIOR WRITTEN CONSENT OF

                            Finder Graphics Systems
                            201 Tamal Vista Blvd.
                            Corte Madera, CA  94925
                            (415) 927-0100

(The above notice does not constitute an admission of publication or
unrestricted dissemination of the work)

******************************************************************************/
#include "esi_s3.h"

#ifndef ESI_SE_H

#include "esi_se.h"

#endif

#ifndef ESI_MM_H

#include "esi_mm.h"

#endif

/* Function Description -----------------------------------------------------
Description:
    For each line and cdp, draw the symbol at the                       
    specified interval.                                                              

Prototype:
    publicdef INT s3_draw_symbols(S3_STRUCT *ps3, INT current, INT type);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    ps3             -(S3_STRUCT *)
    current         -(INT)
    type            -(INT)

Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT s3_draw_symbols (S3_STRUCT *ps3, INT current, INT type)
#else
publicdef INT s3_draw_symbols (ps3, current, type)
S3_STRUCT *ps3;
INT current;
INT type;
#endif
    {
    INT i;
    INT trigger;
    INT nline;
    INT ncdp;
    COORD x, y;
    INT status;
    
    if (type == S3_LINE)
        {
        if (ps3->disp_opt.x_symb_incr == 0)
            return SUCCESS;
        /* for each cdp, see if symbol drawn. if so, draw symbol using
            mm_draw_text. Also determine if label to be drawn with the symbol.
            And dont forget the horizon label */
        nline = current;
        trigger = -1;
        for (i = ps3->grid_3d.lower_cdp; i <= ps3->grid_3d.upper_cdp;
                                                                           i +=
                                                                           ps3->
                                                                           grid_3d.
                                                                           cdp_inc)
            {
            trigger++;
            if (i == ps3->grid_3d.upper_cdp)
                trigger = 0;
            if (trigger % ps3->disp_opt.x_symb_incr == 0)
                {
                /* this one is for plotting                                         */
                status = ss_xy_cdpli ((FLOAT)i, (FLOAT)nline, &x, &y);
                ps3->seismic_generic.shotpoint_xsymbol.symbol_location.x = x;
                ps3->seismic_generic.shotpoint_xsymbol.symbol_location.y = y;
                mm_draw_symbol (&ps3->seismic_generic.shotpoint_xsymbol);
                }
            }
        }
    else if (type == S3_CDP)
        {
        if (ps3->disp_opt.y_symb_incr == 0)
            return SUCCESS;
        /* for each line, see if symbol drawn. if so, draw symbol using
            mm_draw_text. Also determine if label to be drawn with the symbol.
            And dont forget the horizon label */
        ncdp = current;
        trigger = -1;
        for (i = ps3->grid_3d.lower_line; i <= ps3->grid_3d.upper_line;
                                                                             i +=
                                                                             ps3->
                                                                             grid_3d.
                                                                             line_inc)
            {
            trigger++;
            if (i == ps3->grid_3d.upper_line)
                trigger = 0;
            if (trigger % ps3->disp_opt.y_symb_incr == 0)
                {
                /* this one is for plotting                                         */
                status = ss_xy_cdpli ((FLOAT)ncdp, (FLOAT)i, &x, &y);
                ps3->seismic_generic.shotpoint_ysymbol.symbol_location.x = x;
                ps3->seismic_generic.shotpoint_ysymbol.symbol_location.y = y;
                mm_draw_symbol (&ps3->seismic_generic.shotpoint_ysymbol);
                }
            }
        }
    return SUCCESS;
    }
/* END:     */
