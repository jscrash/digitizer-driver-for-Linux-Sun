/* DEC/CMS REPLACEMENT HISTORY, Element GBZ_MAP_WM.C */
/* *3    14-AUG-1990 13:25:47 VINCE "(SPR 5644) Header Standardization" */
/* *2    11-AUG-1989 20:35:43 CONROY "(SPR 0) Revised global, selects, SQL macros & fixes" */
/* *1    11-AUG-1989 19:42:19 CONROY "Select Pickable Wells on Map" */
/* DEC/CMS REPLACEMENT HISTORY, Element GBZ_MAP_WM.C */
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
#include "esi_am.h"
#include "esi_gb.h"
#include "esi_mp.h"
#include "esi_wm.h"
static INT gbWindowId = 0;
static INT gbTnrId = 0;
static INT appSave, winSave, tnrSave, groSave;

#define PICK_PROMPT "Draw a Polygon to Select Seismic Lines."
/* macros */
#define WARNING_RETURN(s) \
    { \
        am_message( AM_ALERT,   s ) ; \
        return ( FAIL ) ; \
    }   
        
#define ALERT_RETURN(s) \
    { \
        am_message(AM_ALERT, s ) ; \
        return ( FAIL ) ; \
    }


/* File Description ---------------------------------------------------------
Overview:

Prototypes and one line descriptions of each function in the file.
Public_Functions:
    publicdef INT gbz_term_map_window ();
    
    publicdef INT gbz_init_map_window ();

Private_Functions:

-----------------------------------------------------------------------------*/

/* Function Description -----------------------------------------------------
Description:
    Functions for managing global connection to map window.

Prototype:
    publicdef INT gbz_init_map_window ();

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    None.
    
Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT gbz_init_map_window ()
#else
publicdef INT gbz_init_map_window ()
#endif
    {
    INT status = SUCCESS;
    INT wmStatus, ifExists;
    INT i, ii, length;
    INT group, segment, numGroups, numSegments, totalSegments;
    MAP_STRUCTURE *cur_map_workspace;
    
    /* clear global stuff */
    gbWindowId = 0;
    gbTnrId = 0;
    
    /* save wm state and close out to application */
    wmowhere (&appSave, &winSave, &tnrSave, &groSave, &wmStatus);
    
    /* front the map */
    status = mp_front_map (&cur_map_workspace);
    if (status < SUCCESS)
        {
        ALERT_RETURN ("No map displayed");
        }
    gbWindowId = cur_map_workspace->map_window_id;
    
    /* map tnr id */
    gbTnrId = cur_map_workspace->map_tnr;
    if (!(gbTnrId))
        {
        ALERT_RETURN ("No map transformation");
        }
    wmoexists (appSave, gbWindowId, gbTnrId, 0, &ifExists);
    if (!(ifExists))
        {
        WARNING_RETURN ("Map window does not exist");
        }
    /* attach to top level */
    wmoattach (appSave, 0, 0, 0, &wmStatus);
    if (wmStatus != SUCCESS)
        {
        WARNING_RETURN ("wmoattach() failed");
        }
    wmwopen (gbWindowId);
    wmtselect (gbTnrId);
    status = SUCCESS;
    
    return(status);
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:

Prototype:
    publicdef INT gbz_term_map_window ();

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    None.
    
Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT gbz_term_map_window ()
#else
publicdef INT gbz_term_map_window ()
#endif
    {
    INT status = SUCCESS;
    INT wmStatus;
    
    /* return to saved wm state */
    wmoattach (appSave, winSave, tnrSave, groSave, &wmStatus);
    
    return(status);
    }
/* END:     */
