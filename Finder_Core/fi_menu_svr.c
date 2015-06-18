/* DEC/CMS REPLACEMENT HISTORY, Element FI_MENU_SVR.C*/
/* *7     5-AUG-1991 17:02:34 JTM "(SPR 0) Added warning display for beta items."*/
/* *6    14-AUG-1990 13:23:43 VINCE "(SPR 5644) Header Standardization"*/
/* *5     2-MAR-1990 11:13:54 JULIAN "(SPR 6012) fix up USE_X logic to be IFDEF only"*/
/* *4     6-FEB-1990 00:17:54 JULIAN "(SPR -1) X Version "*/
/* *3    27-NOV-1989 17:21:29 GILLESPIE "(SPR 0) Syntax error - left out !="*/
/* *2    27-NOV-1989 15:39:28 GORDON "(SPR 0) Use NULL_MENU_ID, not 0"*/
/* *1    19-JUN-1989 12:50:09 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element FI_MENU_SVR.C*/
/*****************************************************************************/
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

#include "esi_fi_int.h"

#ifndef ESI_WI_H

#include "esi_wi.h"

#endif

/* Function Description -----------------------------------------------------
Description:

Prototype:
    publicdef INT fi_menu_server(INT id, INT item, BYTE *ptr);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    id              -(INT) 
    item            -(INT)
    ptr             -(BYTE *)

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT fi_menu_server (INT id, INT item, BYTE *ptr) 
#else
publicdef INT fi_menu_server (id, item, ptr)
INT id;
INT item;
BYTE *ptr;
#endif
    {
    FINDER_MENU_STRUCT *pm;
    INT status;
    
    /* loop through each entry in the externally defined menu structure
    find the one that matches this id */
    
    for (pm = fiz_finder_menus (); pm->id IS_NOT_EQUAL_TO NULL_MENU_ID; pm += pm->item)
        {
        if (pm->id EQUALS id)
            {

#ifdef USE_X
            fi_sync_mn (id);
#endif
            
            /* look at the nth item down this list */
            pm += item;
            if (pm->Type EQUALS NextMenu)
                {
                status = fi_sync_smn (fiz_finder_menus ()->id, pm->Activates.menu);
                }
            else
                {
#if 0
                fi_sync_mn (fiz_finder_menus ()->id);
#endif

                if( pm->BetaLogical != (CHAR *)0 )
		{
		 char *txt = "                 *** WARNING ***\n\"%s\" is a BETA function!\nProceed?";
			      
		 char msg[100];
		 INT yes;

		 sprintf(msg, txt, pm->Name);

		 wiyesno(msg, "", &yes, &status);
                 
		 if( yes == 0 ) return( FAIL );
                }


                status = fih_start_application (pm);
                }
            return status;
            }
        }
    return FAIL;
    }
/* END:     */
