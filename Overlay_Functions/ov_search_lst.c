/* DEC/CMS REPLACEMENT HISTORY, Element OV_SEARCH_LST.C*/
/* *3    25-JUL-1990 19:59:59 VINCE "(SPR 5644) Header Standardization"*/
/* *2    26-JUN-1989 11:02:24 GILLESPIE "(SPR 20) Fix castings"*/
/* *1    19-JUN-1989 13:22:13 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element OV_SEARCH_LST.C*/
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
#include "esi_am.h"
#include "esi_tc.h"
#include "esi_ho.h"

/* Function Description -----------------------------------------------------
Description:
    Function to search the overlay name list for the supplied 
    name.

Prototype:
    publicdef BOOL ov_search_list(CHAR *name,BOOL sub_groups,INT *position);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    name            -(CHAR *)
    sub_groups      -(BOOL)
    position        -(INT *)

Return Value/Status:
    Function returns the value of found (TRUE/FALSE).
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef BOOL ov_search_list (CHAR *name, BOOL sub_groups, INT *position)
#else
publicdef BOOL ov_search_list (name, sub_groups, position)
CHAR *name;
BOOL sub_groups;
INT *position;
#endif
    {
    MAP_STRUCTURE *mapdef;
    INT i;
    BOOL found = FALSE;
    INT *idum;
    CHAR **cdum;
    
    /* ********************************************************************** */
    
    am_open_workspace ("MAPDEF", AM_APPLICATION, (VOIDPTR *) & mapdef);
    
    /* * SEARCH LIST (USER NAMED OVERLAY) * */
    /* ------------------------------------ */
    
    if (sub_groups)
        {
        for (i = 0; i < mapdef->wm_nalloc; i++)
            {
            if (mapdef->wm_group_names[i] EQUALS NULL)
                {
                break;
                }
            else if (strcmp (mapdef->wm_group_names[i], name) EQUALS 0)
                {
                found = TRUE;
                break;
                }
            }
        }
    /* * SEARCH LIST (DEFAULT OVERLAY NAME) * */
    /* -------------------------------------- */
    
    else
        {
        for (i = 0; i < mapdef->wm_nalloc; i++)
            {
            if (mapdef->wm_group_names[i] EQUALS NULL)
                {
                break;
                }
            else if (strcmp (mapdef->wm_group_names[i],
                             name) EQUALS 0 AND mapdef->wm_id_count[i] EQUALS 0)
                {
                found = TRUE;
                break;
                }
            }
        }
    /* * CHECK POSITION AND REALLOC IF NECCESSARY * */
    /* -------------------------------------------- */
    
    *position = i;
    if (NOT found AND (*position) > mapdef->wm_nalloc)
        {
        i = 100 + mapdef->wm_nalloc;
        
        idum = (INT *)am_allocate (AM_GLOBAL, i * sizeof(INT));
        tc_zeroes (idum, i * sizeof(INT));
        hoblockmove (mapdef->wm_id_list, idum, mapdef->wm_nalloc * sizeof(INT));
        am_free (mapdef->wm_id_list);
        mapdef->wm_id_list = idum;
        
        idum = (INT *)am_allocate (AM_GLOBAL, i * sizeof(INT));
        tc_zeroes (idum, i * sizeof(INT));
        hoblockmove (mapdef->wm_id_count, idum, mapdef->wm_nalloc * sizeof(INT));
        am_free (mapdef->wm_id_count);
        mapdef->wm_id_count = idum;
        
        cdum = (CHAR **)am_allocate (AM_GLOBAL, i * sizeof(CHAR *));
        tc_zeroes (cdum, i * sizeof(CHAR *));
        hoblockmove (mapdef->wm_group_names, cdum, mapdef->wm_nalloc * sizeof(CHAR *));
        am_free (mapdef->wm_group_names);
        mapdef->wm_group_names = cdum;
        
        mapdef->wm_nalloc = i;
        }
    return found;
    }
/* END:     */
