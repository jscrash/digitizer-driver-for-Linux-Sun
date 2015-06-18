/* DEC/CMS REPLACEMENT HISTORY, Element MM_DRW_SYM.C*/
/* *2    17-AUG-1990 22:00:16 VINCE "(SPR 5644) Code Cleanup"*/
/* *1    19-JUN-1989 13:08:39 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element MM_DRW_SYM.C*/

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

#include "esi_mm.h"
#include "esi_mp.h"
#include "esi_wm.h"
#include "esi_nl.h"

/* Function Description -----------------------------------------------------
Description:
    Draws and indexes a centered symbol.

Prototype:
    publicdef INT mm_draw_symbol(SYMBOL *pSymbol);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) pSymbol     -(SYMBOL *) A pointer to a symbol object.

Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT mm_draw_symbol (SYMBOL *pSymbol)
#else
publicdef INT mm_draw_symbol (pSymbol)
SYMBOL *pSymbol;
#endif
    {
    INT status;
    DOUBLE scale_factor;
    
    /*  Set the size                */
    
    cs_set_height (pSymbol->symbol_size);
    cs_query_scale_factor (&scale_factor);
    
    /*  Draw the symbol             */
    
    cs_draw_symbol (pSymbol->symbol_group, pSymbol->symbol_type, scale_factor,
                    pSymbol->symbol_color, pSymbol->symbol_location.x,
                    pSymbol->symbol_location.y);
    
    return SUCCESS;
    }
/* END:     */
