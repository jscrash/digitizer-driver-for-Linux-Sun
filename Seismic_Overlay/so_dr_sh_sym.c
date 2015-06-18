/* DEC/CMS REPLACEMENT HISTORY, Element SO_DR_SH_SYM.C*/
/* *4    26-JUL-1990 17:19:46 GILLESPIE "(SPR 1) Lint run; incorporate changes for horizon end point posting"*/
/* *3    25-JUL-1990 20:03:42 VINCE "(SPR 5644) Header Standardization"*/
/* *2    25-JUL-1989 20:05:42 GILLESPIE "(SPR 50) Seismic system revamp"*/
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

/* Function Description -----------------------------------------------------
Description:
    Function to draw the shotpoint symbols.

Prototype:
    publicdef INT so_draw_shot_symbol(SO_STRUCT *pso,COORD x,COORD y);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    pso             -(SO_STRUCT *) Seismic overlay structure.
    x               -(COORD) -\
    y               -(COORD) X and y location of shotpoint.

Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT so_draw_shot_symbol (SO_STRUCT *pso, COORD x, COORD y)
#else
publicdef INT so_draw_shot_symbol (pso, x, y)
SO_STRUCT *pso;
COORD x, y;
#endif
    {
    INT status;
    
    status = SUCCESS;
    
    if (NOT pso->seismic_generic.draw_shotpoint_symbol)
        {
        return status;
        }
    pso->seismic_generic.shotpoint_symbol.symbol_location.x = x;
    pso->seismic_generic.shotpoint_symbol.symbol_location.y = y;
    mm_draw_symbol (&pso->seismic_generic.shotpoint_symbol);
    
    return status;
    }
/* END:     */
