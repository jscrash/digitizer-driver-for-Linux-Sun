/* DEC/CMS REPLACEMENT HISTORY, Element FIH_Q_M_HELP.C*/
/* *3    14-AUG-1990 13:12:28 VINCE "(SPR 5644) Header Standardization"*/
/* *2    18-NOV-1989 09:48:21 HALL "(SPR 5075) Global overhaul"*/
/* *1    19-JUN-1989 12:49:33 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element FIH_Q_M_HELP.C*/
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

#ifndef ESI_AM_INT_H

#include "esi_am_int.h"

#endif

#ifndef ESI_C_LIB_H

#include "esi_c_lib.h"

#endif

#ifndef ESI_WI_H

#include "esi_wi.h"

#endif

#ifndef ESI_TS_H

#include "esi_ts.h"

#endif

/* Function Description -----------------------------------------------------
Description:
    Get menu help key.
    
Prototype:
    publicdef INT fih_query_menu_helpkey (INT menuid, INT itemid, CHAR help_key[]);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) menuid      -(INT *) Pointer to a place to store the menu's ID.
    (I) itemid      -(INT *) Pointer to a place to store the menu's ITEM.
    (O) help_key    -(CHAR []) Help key retrieved.

Return Value/Status:
    SUCCESS - Successful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT fih_query_menu_helpkey (INT menuid, INT itemid, CHAR help_key[]) 
#else
publicdef INT fih_query_menu_helpkey (menuid, itemid, help_key)
INT menuid, itemid;
CHAR help_key[];
#endif
    {
    INT status, len, i;
    CHAR text[81], *cp;
    FINDER_MENU_STRUCT *m;
    
    help_key[0] = '\0';
    
    for (m = fiz_finder_menus (); m->id != 0; m++)
        {
        if ((m->id == menuid) && (m->item == itemid))
            {
            if (m->AppName != (CHAR *)NULL)
                {
                if (strlen (m->AppName) > 0)
                    {
                    strcpy (help_key, m->AppName);
                    ts_sto_upper (help_key);
                    }
                }
            else
                strcpy (help_key, "FINDER");
            break;
            }
        }
    return SUCCESS;
    }
/* END:     */
