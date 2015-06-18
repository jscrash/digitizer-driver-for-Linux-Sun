/* DEC/CMS REPLACEMENT HISTORY, Element SO_DRW_SLINE.C*/
/* *4    26-JUL-1990 17:19:37 GILLESPIE "(SPR 1) Lint run; incorporate changes for horizon end point posting"*/
/* *3    25-JUL-1990 20:03:32 VINCE "(SPR 5644) Header Standardization"*/
/* *2    25-JUL-1989 20:05:35 GILLESPIE "(SPR 50) Seismic system revamp"*/
/* *1    19-JUN-1989 13:26:26 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element SO_DRW_SLINE.C*/
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
#include "esi_am.h"

/* Function Description -----------------------------------------------------
Description:
    Function to draw the seismic line.

Prototype:
    publicdef INT so_draw_seismic_line(SO_STRUCT *pso);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    pso             -(SO_STRUCT *) Seismic overlay structure.

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT so_draw_seismic_line (SO_STRUCT *pso)
#else
publicdef INT so_draw_seismic_line (pso)
SO_STRUCT *pso;
#endif
    {
    INT status;
    
    status = SUCCESS;
    
    /* draw the shot point labels and symbols and z value labels */
    
    if (pso->seismic_generic.draw_shotpoint_symbol OR pso->seismic_generic.
        draw_shotpoint_label)
        {
        status = so_process_shotpoints (pso);
        }
    /* draw the graphic object line of the seismic line */
    /* SPR 4321 draw line label even if "suppress line" option is enabled */
    
    /*    if (pso->seismic_generic.draw_seismic_line)
              {
    */
    status = so_draw_line_itself (pso);
    if (status != SUCCESS)
        {
        return status;
        }
    /*
            }
    */
    
    return status;
    }
/* END:     */
