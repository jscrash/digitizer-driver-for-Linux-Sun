/* DEC/CMS REPLACEMENT HISTORY, Element SO_DR_SYM_ARR.C*/
/* *5    26-JUL-1990 17:19:52 GILLESPIE "(SPR 1) Lint run; incorporate changes for horizon end point posting"*/
/* *4    25-JUL-1990 20:03:51 VINCE "(SPR 5644) Header Standardization"*/
/* *3     2-NOV-1989 09:53:54 GILLESPIE "(SPR 30) Changes from moving mapping structure out of esi_so.h"*/
/* *2    25-JUL-1989 20:05:50 GILLESPIE "(SPR 50) Seismic system revamp"*/
/* *1    19-JUN-1989 13:26:33 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element SO_DR_SYM_ARR.C*/
/*****************************************************************************/
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

#include "esi_so.h"

#ifndef ESI_MP_H
#include "esi_mp.h"
#endif

#ifndef ESI_WM_H
#include "esi_wm.h"
#endif

/* Function Description -----------------------------------------------------
Description:
    Function to draw the shotpoint symbol array.

Prototype:
    publicdef INT so_draw_shot_symbol_arr(SO_STRUCT *pso,FLOAT    *x,FLOAT    *y,
        INT nsymbols);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    pso             -(SO_STRUCT *) Seismic overlay structure.
    x               -(FLOAT *) -\
    y               -(FLOAT *) X and y location of shotpoints.
    nsymbols        -(INT)

Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT so_draw_shot_symbol_arr (SO_STRUCT *pso, FLOAT *x, FLOAT *y,
                                       INT nsymbols)
#else
publicdef INT so_draw_shot_symbol_arr (pso, x, y, nsymbols)
SO_STRUCT *pso;
FLOAT *x, *y;
INT nsymbols;
#endif
    {
    INT status = SUCCESS;
    FLOAT dummy;
    MAP_STRUCTURE *pmap = (MAP_STRUCTURE *)pso->map;
    
    if (NOT pso->seismic_generic.draw_shotpoint_symbol)
        {
        return status;
        }
    wm_gspmci (pso->seismic_generic.shotpoint_symbol.symbol_color);
    wm_gsmk (pso->seismic_generic.shotpoint_symbol.symbol_type);
    dummy = pso->seismic_generic.shotpoint_symbol.symbol_size * pmap->wcs_to_inches;
    wm_gsmksc (dummy);
    wm_gpm (nsymbols, x, y);
    return status;
    }
/* END:     */
