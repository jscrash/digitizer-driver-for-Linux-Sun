/* DEC/CMS REPLACEMENT HISTORY, Element TG_DRAWLINE.C*/
/* *2    17-AUG-1990 22:26:48 VINCE "(SPR 5644) Code Cleanup"*/
/* *1    19-JUN-1989 13:32:31 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element TG_DRAWLINE.C*/
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

#include "esi_tg.h"
#include "esi_wm.h"

/* Function Description -----------------------------------------------------
Description:

Prototype:
    publicdef INT tg_drawline(TG_LINE_STRUCT  *pl);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    pl              -(TG_LINE_STRUCT *)

Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT tg_drawline (TG_LINE_STRUCT *pl)
#else
publicdef INT tg_drawline (pl)
TG_LINE_STRUCT *pl;
#endif
    {
    /* THIS ROUTINE HAS BEEN COMPLETELY STRIPPED TO SPEED THINGS UP */
    wm_gpl (pl->npts, pl->xarray, pl->yarray);
    
    return SUCCESS;
    }
/* END:     */
