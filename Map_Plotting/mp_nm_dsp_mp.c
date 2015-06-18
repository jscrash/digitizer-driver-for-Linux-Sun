/*  DEC/CMS REPLACEMENT HISTORY, Element MP_NM_DSP_MP.C*/
/*  *3    17-AUG-1990 22:07:33 VINCE "(SPR 5644) Code Cleanup"*/
/*  *2    26-JUN-1989 10:57:56 GILLESPIE "(SPR 20) Fix castings"*/
/*  *1    19-JUN-1989 13:13:38 SYSTEM ""*/
/*  DEC/CMS REPLACEMENT HISTORY, Element MP_NM_DSP_MP.C*/
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

#include "esi_c_lib.h"
#include "esi_gl_defs.h"
#include "esi_mp.h"
#include "esi_am.h"
#include "esi_qs.h"

/* Function Description -----------------------------------------------------
Description:
    Routine to return the number of maps actually displayed
    on the graphics screen.

Prototype:
    publicdef INT mp_num_displayed_maps(INT *num);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    num             -(INT *)

Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT mp_num_displayed_maps (INT *num)
#else
publicdef INT mp_num_displayed_maps (num)
INT *num;
#endif
    {
    
    PROGNAME ("MP_NUM_DISPLAYED_MAPS");
    INT status;
    INT *nmaps;
    INT i, returned_num = 0;
    INT current_map;
    MAP_STRUCTURE *map;
    
    /* ********************************************************************** */
    
    *num = 0;
    if (am_open_workspace ("MAP_NUMBER", AM_GLOBAL, (VOIDPTR *) & nmaps) EQUALS SUCCESS)
        {
        qs_inq_i (QS_CUR_MAP_NUMBER, &current_map);
        for (i = 1; i <= *nmaps; i++)
            {
            qs_set_i (QS_CUR_MAP_NUMBER, i);
            if (mp_get_current_map (&map) EQUALS SUCCESS AND map->map_window_id != 0)
                {
                returned_num++;
                }
            }
        qs_set_i (QS_CUR_MAP_NUMBER, current_map);
        }
    *num = returned_num;
    
    return SUCCESS;
    }
/* END:     */
