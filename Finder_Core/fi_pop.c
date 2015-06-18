/* DEC/CMS REPLACEMENT HISTORY, Element FI_POP.C*/
/* *6    14-AUG-1990 18:25:25 GILLESPIE "(SPR 1) Remove bogus line @ bottom"*/
/* *5    14-AUG-1990 13:23:48 VINCE "(SPR 5644) Header Standardization"*/
/* *4    27-NOV-1989 15:40:00 GORDON "(SPR 0) Use NULL_MENU_ID, not 0"*/
/* *3    26-JUN-1989 10:44:12 GILLESPIE "(SPR 20) Fix castings"*/
/* *2    20-JUN-1989 11:15:24 GILLESPIE "(SPR 20) Fix minor casting problem"*/
/* *1    19-JUN-1989 12:50:12 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element FI_POP.C*/
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

#ifndef ESI_FI_INT_H

#include "esi_fi_int.h"

#endif

#ifndef ESI_AM_H

#include "esi_am.h"

#endif

#ifndef ESI_WI_H

#include "esi_wi.h"

#endif

#include "esi_fi_err.h"

/* Function Description -----------------------------------------------------
Description:
    FI_POP restores the environment from that caused by a previous
    FI_PUSH call.

Prototype:
    publicdef INT fi_pop();
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    None.
    
Return Value/Status:
    FI_PUSHED, SUCCESS - Successful pop.
    FI_NOT_PUSHED - Environment not pushed previous to pop.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT fi_pop (void) 
#else
publicdef INT fi_pop ()
#endif
    {
    INT status;                     /* completion status */
    INT i;                          /* counter */
    FINDER_WORKSPACE *f;            /* pointer to Finder global workspace */
    FINDER_MENU_STRUCT *pa;
    
    /************************************************************************/
    
    /*  Access Finder global workspace. */
    
    status = am_open_workspace ("FINDER", AM_GLOBAL, (VOIDPTR *)(VOIDPTR *) & f);
    
    /*  If not pushed, indicate grief. */
    
    if (status == SUCCESS)
        if (f->fi_push_level == 0)
            status = FI_NOT_PUSHED;
        
        /*  If pushed, however, then pop workstation
            and indicate popped. */
        
        if (status == SUCCESS)
            {
            if (f->fi_push_level > 0)
                {
                wi_pop ();
                f->fi_push_level--;
                }
            /*  If popped all the way back to level 0,
                then restore static Finder menus. As
                in FI_PUSH, dont touch global menu. */
            
            if (f->fi_push_level == 0)
                {
                for (pa = fiz_finder_menus (); pa->id != NULL_MENU_ID; pa += pa->item)
                    {
                    if (pa->flag EQUALS ON)
                        wivismn (pa->id, ON);
                    }
                }
            /*  If not popped back to level zero, then
                indicate still pushed. This is not an
                error, just status. */
            
            else
                status = FI_PUSHED;
            }
    return status;
    }
/* END:     */
