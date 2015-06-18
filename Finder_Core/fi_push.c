/* DEC/CMS REPLACEMENT HISTORY, Element FI_PUSH.C*/
/* *6    14-AUG-1990 22:50:37 GILLESPIE "(SPR 5644) Fix problems with last checkin"*/
/* *5    14-AUG-1990 13:23:53 VINCE "(SPR 5644) Header Standardization"*/
/* *4    27-NOV-1989 15:40:23 GORDON "(SPR 0) Use NULL_MENU_ID, not 0"*/
/* *3    26-JUN-1989 10:44:20 GILLESPIE "(SPR 20) Fix castings"*/
/* *2    20-JUN-1989 11:15:30 GILLESPIE "(SPR 20) Fix minor casting problem"*/
/* *1    19-JUN-1989 12:50:15 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element FI_PUSH.C*/
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

#include "esi_fi_int.h"
#include "esi_am.h"
#include "esi_wi.h"
#include "esi_fi_err.h"

/* Function Description -----------------------------------------------------
Description:
    FI_PUSH pushes the environment to make way for closed pipe applications
    which request their own events. Unlike it's near relative WI_PUSH,
    FI_PUSH also takes care of eliminating all static menus, with the
    exception of the special global menu.

    FI_PUSH only disables the static Finder menus when it is called
    with an empty "stack". Subsequent calls to fi_push only push the
    workstation environment (wi_push), but do not affect the already
    disabled Finder menus.

    FI_PUSH does the following:

        1. Turns off all the static menus, except the global menu.

        2. Pushes the workstation environment.

    Specifically, FI_PUSH does not push the global menu.

    You should call FI_POP when you are done with your charade.

Prototype:
    publicdef INT fi_push();
    
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
publicdef INT fi_push () 
#else
publicdef INT fi_push ()
#endif
    {
    INT status;                     /* completion status */
    INT i;                          /* counter */
    FINDER_WORKSPACE *f;            /* pointer to Finder global workspace */
    FINDER_MENU_STRUCT *pa;
    
    /************************************************************************/
    
    /*  Access Finder global workspace. */
    
    status = am_open_workspace ("FINDER", AM_GLOBAL, (VOIDPTR *)(VOIDPTR *) & f);
    
    /*  If no problem accessing workspace ... */
    
    if (status == SUCCESS)
        {
        
        /*  If level zero, then Finder menus need
            to be pushed. */
        
        if (f->fi_push_level == 0)
            {
            
            /*  Turn off all but the Finder menu itself. 
            COM BY JJC 02-NOV-1988, Should not do this, this routine will mess up the 
            finder_menus workspaces.
            fi_sync_mn(fiz_finder_menus()->id); */
            
            /*  For static menus which remain on, but
                which are not subordinate to the Finder
                menu, turn off all the ones which are
                on. Don't fuck with the global menu. */
            
            for (pa = fiz_finder_menus (); pa->id != NULL_MENU_ID; pa += pa->item)
                {
                if (pa->flag EQUALS ON)
                    {
                    wivismn (pa->id, OFF);
                    }
                }
            }
        /*  If environment is already pushed one or
            more levels, then indicate status. */
        
        else
            status = FI_PUSHED;
        
        /*  Increment level to which we are
            now pushed. */
        
        f->fi_push_level++;
        
        /*  Push workstation environment. */
        
        wipush ();
        
        /*  Reenable all types of events. This is
            necessary because the wipush disables
            these. */
        
        wismode (WI_MENU_EVENT, WI_EVENT_MODE);
        wismode (WI_DIALOG_EVENT, WI_EVENT_MODE);
        wismode (WI_SELECT_EVENT, WI_EVENT_MODE);
        }
    return SUCCESS;
    }
/* END:     */
