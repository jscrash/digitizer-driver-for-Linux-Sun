/* DEC/CMS REPLACEMENT HISTORY, Element OV_CL_OVL.C*/
/* *3    25-JUL-1990 19:59:01 VINCE "(SPR 5644) Header Standardization"*/
/* *2    26-JUN-1989 11:02:08 GILLESPIE "(SPR 20) Fix castings"*/
/* *1    19-JUN-1989 13:21:47 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element OV_CL_OVL.C*/
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

#include "esi_gl_defs.h"
#include "esi_mp.h"
#include "esi_md_defs.h"
#include "esi_am.h"
#include "esi_wm.h"

/* Function Description -----------------------------------------------------
Description:
    Function to close the overlay.

Prototype:
    publicdef INT ov_close_overlay(INT display_line);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    display_line    -(INT)

Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT ov_close_overlay (INT display_line)
#else
publicdef INT ov_close_overlay (display_line)
INT display_line;
#endif
    {
    INT status;                     /* RETURN STATUS */
    MAP_STRUCTURE *mapdef;
    MD_DISPLAY *display;
    MD_COMPILED_MAPDEF *compiled;
    INT row;
    
    /* ********************************************************************** */
    
    status = SUCCESS;
    
    am_open_workspace ("MAPDEF", AM_APPLICATION, (VOIDPTR *) & mapdef);
    display = mapdef->display;
    compiled = mapdef->compiled;
    
    row = display->compiled_row[display_line];
    
    if (compiled->verb_wm_id[row] != 0)
        {
        wmgclose (compiled->verb_wm_id[row]);
        }
    wmgclose (compiled->overlay_wm_id[row]);
    
    return status;
    }
/* END:     */
