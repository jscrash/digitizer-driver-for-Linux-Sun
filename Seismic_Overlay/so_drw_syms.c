/* DEC/CMS REPLACEMENT HISTORY, Element SO_DRW_SYMS.C*/
/* *4    26-JUL-1990 17:19:42 GILLESPIE "(SPR 1) Lint run; incorporate changes for horizon end point posting"*/
/* *3    25-JUL-1990 20:03:37 VINCE "(SPR 5644) Header Standardization"*/
/* *2    10-OCT-1989 10:01:14 JULIAN "(SPR 5020) mods for CT parameter change"*/
/* *1    26-JUL-1989 00:15:38 GILLESPIE "New seismic system mods"*/
/* DEC/CMS REPLACEMENT HISTORY, Element SO_DRW_SYMS.C*/
/* DEC/CMS REPLACEMENT HISTORY, Element SO_DR_SH_SYM.C*/
/* *1    19-JUN-1989 13:26:30 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element SO_DR_SH_SYM.C*/
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
#include "esi_se.h"
#include "esi_mm.h"
#include "esi_ct.h"

/* Function Description -----------------------------------------------------
Description:
    Function to draw the shotpoint symbols.

Prototype:
    publicdef INT so_draw_symbols(SO_STRUCT *pso,CDP *list,UINT npts);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    pso             -(SO_STRUCT *) Seismic overlay structure.
    list            -(CDP *)
    npts            -(UINT)

Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT so_draw_symbols (SO_STRUCT *pso, CDP *list, UINT npts)
#else
publicdef INT so_draw_symbols (pso, list, npts)
SO_STRUCT *pso;
CDP *list;
UINT npts;
#endif
    {
    UINT i;
    DOUBLE pnt_in[2], pnt_out[2];
    
    /**************************************************************************/
    
    for (i = 0; i < npts; i++)
        {
        se_xy_from_cdp (&(pso->current_line), list[i], 0.0, &(pnt_in[0]),
                        &(pnt_in[1]));
        ct_map_xy (pnt_in[0], pnt_in[1], &pnt_out[0], &pnt_out[1]);
        pso->seismic_generic.shotpoint_symbol.symbol_location.x = pnt_out[0];
        pso->seismic_generic.shotpoint_symbol.symbol_location.y = pnt_out[1];
        mm_draw_symbol (&pso->seismic_generic.shotpoint_symbol);
        }
    return SUCCESS;
    }
/* END:     */
