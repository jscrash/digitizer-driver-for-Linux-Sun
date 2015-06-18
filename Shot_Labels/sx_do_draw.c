/* DEC/CMS REPLACEMENT HISTORY, Element SX_DO_DRAW.C*/
/* *3    17-AUG-1990 22:21:37 VINCE "(SPR 5644) Code Cleanup"*/
/* *2    25-JUL-1989 20:12:16 GILLESPIE "(SPR 50) Seismic system revamp"*/
/* *1    19-JUN-1989 13:29:21 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element SX_DO_DRAW.C*/
/******************************************************************************

                Copyright Exploration Systems, Inc. 1989
                    Unpublished -- All rights reserved

THIS SOFTWARE IS THE PROPRIETARY PROPERTY OF Exploration Systems, Inc.  AND MAY
CONTAIN CONFIDENTIAL TRADE SECRET INFORMATION. IT IS LICENSED FOR USE ON THE
DESIGNATED EQUIPMENT ON WHICH IT WAS ORIGINALLY INSTALLED AND  MAY NOT BE
MODIFIED, DUPLICATED OR COPIED IN ANY FORM WITHOUT PRIOR WRITTEN CONSENT OF

                            Exploration Systems
                            c/o Finder Graphics Systems
                            201 Tamal Vista Blvd.
                            Corte Madera, CA  94925
                            (415) 927-0100

(The above notice does not constitute an admission of publication or
unrestricted dissemination of the work)

******************************************************************************/

#include "esi_sx.h"

#ifndef ESI_SO_H
#include "esi_so.h"
#endif

#ifndef ESI_LU_H
#include "esi_lu.h"
#endif

/* Function Description -----------------------------------------------------
Description:
    Function to manage shotpoint labeling and Z values.

Prototype:
    publicdef INT sx_do_draw(SX_STRUCT *psx,DOUBLE x,DOUBLE y,INT cdp,SP shot_point,INT mode);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    psx             -(SX_STRUCT *)
    x               -(DOUBLE)
    y               -(DOUBLE)
    cdp             -(INT)
    shot_point      -(SP)
    mode            -(INT)

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT sx_do_draw (SX_STRUCT *psx, DOUBLE x, DOUBLE y, INT cdp, SP shot_point,
                          INT mode)
#else
publicdef INT sx_do_draw (psx, x, y, cdp, shot_point, mode)
SX_STRUCT *psx;
DOUBLE x, y;
INT cdp;
SP shot_point;
INT mode;
#endif
    {
    INT status;
    BOOL special;
    INT color_save;
    
    DOUBLE zvalue;
    
    BOOL lab, zval, sym;
    
    SO_STRUCT *pso = psx->pso;
    
    status = SUCCESS;
    
    /* see if special, we do need to do this one */
    
    if (so_check_label (pso, cdp, shot_point, &sym, &lab, &zval, &special) != SUCCESS)
        {
        SX_RETURN;
        }
    /* draw symbol */
    
    if (mode == SX_ERASE)
        {
        color_save = pso->seismic_generic.shotpoint_symbol.symbol_color;
        pso->seismic_generic.shotpoint_symbol.symbol_color =
                                                             psx->graphics.
                                                             background_color;
        }
    else if (mode == SX_BLINK)
        {
        color_save = pso->seismic_generic.shotpoint_symbol.symbol_color;
        lu_text_to_index("COLOR",
	    (INDEX *)&pso->seismic_generic.shotpoint_symbol.symbol_color,
	    "HIGHLIGHT");
        }
    so_draw_shot_symbol (pso, x, y);
    
    if (mode == SX_ERASE OR mode == SX_BLINK)
        {
        pso->seismic_generic.shotpoint_symbol.symbol_color = color_save;
        }
    /* process z value */
    
    if (pso->zzzz)
        {
        zvalue = SO_ZZZZ;
        }
    else
        {
        zvalue = SO_NULL;
        }
    /* draw label */
    
    if (mode == SX_ERASE)
        {
        color_save = pso->seismic_generic.shotpoint_label.text_color;
        pso->seismic_generic.shotpoint_label.text_color =
                                                          psx->graphics.
                                                          background_color;
        }
    so_draw_label (pso, x, y, zvalue, shot_point, special, lab, zval);
    
    if (mode == SX_ERASE)
        {
        pso->seismic_generic.shotpoint_label.text_color = color_save;
        }
    return status;
    }
/* END:     */
