/* DEC/CMS REPLACEMENT HISTORY, Element GB_RECTANGLE.C*/
/* *5    30-AUG-1990 12:32:55 GILLESPIE "(SPR 1) Remove unused include files"*/
/* *4    14-AUG-1990 13:26:56 VINCE "(SPR 5644) Header Standardization"*/
/* *3    11-AUG-1989 20:38:19 CONROY "(SPR 0) Revised global, selects, SQL macros & fixes"*/
/* *2    13-JUL-1989 13:46:15 GORDON "(SPR 0) added call to wm_set_mouse_menu"*/
/* *1    19-JUN-1989 12:51:56 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element GB_RECTANGLE.C*/
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

#include "esi_c_lib.h"
#include "esi_gb.h"
#include "esi_am.h"
#include "esi_wm.h"
#include "esi_nl.h"
#include "esi_mp.h"

/* wm classes                                                           */

static WINDOW_ID gbWindowId = 0;
static TNR_ID gbTnrId = 0;

static APPLICATION_ID appSave;
static WINDOW_ID winSave;
static TNR_ID tnrSave;
static GROUP_ID groSave;

#define PICK_PROMPT "Band a Rectangle to Select Wells."

/* macros                                                               */
#define WARNING_RETURN(s) \
    { \
       am_message( AM_ALERT,  s ) ; \
       return ( FAIL ) ; \
    }
        
#define ALERT_RETURN(s) \
    { \
       am_message(AM_ALERT, s ) ; \
       return ( FAIL ) ; \
    }


/* Internal routines                                                    */
#if USE_PROTOTYPES
static INT zzInit (void);
static INT zzTerm (void);
#else
static INT zzInit ();
static INT zzTerm ();
#endif

/* File Description ---------------------------------------------------------
Overview:

Prototypes and one line descriptions of each function in the file.
Public_Functions:
    publicdef INT gb_well_rectangle (NLIST_HEADER *listPtr);

Private_Functions:
    static INT zzInit ();

    static INT zzTerm ();

-----------------------------------------------------------------------------*/

/* Function Description -----------------------------------------------------
Description:

Prototype:
    publicdef INT gb_well_rectangle (NLIST_HEADER *listPtr);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    listPtr         -(NLIST_HEADER *) Pointer to created select list.

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT gb_well_rectangle (NLIST_HEADER *listPtr)
#else
publicdef INT gb_well_rectangle (listPtr)
NLIST_HEADER *listPtr;
#endif
    {
    INT status = SUCCESS;
    INT done, wmStatus, button;
    FLOAT wx1, wy1, wx2, wy2;
    DOUBLE xmin, ymin, xmax, ymax;
    INT numAdded, numWells;
    CHAR prompt[STMAX], aMsgStr[STMAX];
    NLIST_ID list_name;
    
    status = zzInit ();
    if (status == SUCCESS)
        {
        wmwopen (gbWindowId);
        wmtselect (gbTnrId);
        
        strncpy (prompt, PICK_PROMPT, STMAX);
        prompt[STMAX - 1] = 0;
        done = FALSE;
        
        am_message (AM_DIALOG, prompt);
        wmtlocate (&wmStatus, &wx1, &wy1, &button);
        if (wmStatus == SUCCESS && button != 12)
            {
            wmtbandrectangle (&wmStatus, wx1, wy1, &wx2, &wy2, &button);
            }
        if (wmStatus == SUCCESS && button != 12)
            {
            xmin = MIN (wx1, wx2);
            xmax = MAX (wx1, wx2);
            ymin = MIN (wy1, wy2);
            ymax = MAX (wy1, wy2);
            gb_rect_pick_well (xmin, ymin, xmax, ymax, listPtr, FALSE);
            }
        status = wmStatus;
        }
    zzTerm ();
    
    am_message (AM_DIALOG, "");
    
    return(status);
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:

Prototype:
    static INT zzInit ();

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    None.
    
Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PRIVATE to <gb_rectangle.c>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static INT zzInit (void)
#else
static INT zzInit ()
#endif
    {
    INT status = SUCCESS;
    INT wmStatus, ifExists;
    INT i, ii, length;
    INT group, segment, numGroups, numSegments, totalSegments;
    MAP_STRUCTURE *cur_map_workspace;
    
    /* clear global stuff                                               */
    gbWindowId = 0;
    gbTnrId = 0;
    
    /* save wm state and close out to application                       */
    wmowhere (&appSave, &winSave, &tnrSave, &groSave, &wmStatus);
    
    /* front the map                                                    */
    status = mp_front_map (&cur_map_workspace);
    if (status < SUCCESS)
        {
        ALERT_RETURN ("No map displayed");
        }
    gbWindowId = cur_map_workspace->map_window_id;
    
    /* map tnr id                                                       */
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
    /* attach to top level                                              */
    wmoattach (appSave, 0, 0, 0, &wmStatus);
    if (wmStatus != SUCCESS)
        {
        WARNING_RETURN ("wmoattach() failed");
        }

    return SUCCESS;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:

Prototype:
    static INT zzTerm ();

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    None.
    
Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PRIVATE to <gb_rectangle.c>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static INT zzTerm (void)
#else
static INT zzTerm ()
#endif
    {
    INT wmStatus = SUCCESS;
    
    /* return to saved wm state                                         */
    wmoattach (appSave, winSave, tnrSave, groSave, &wmStatus);
    
    return wmStatus;
    }
/* END:     */
