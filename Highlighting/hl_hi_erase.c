/* DEC/CMS REPLACEMENT HISTORY, Element HL_HI_ERASE.C*/
/* *2    17-AUG-1990 21:48:54 VINCE "(SPR 5644) Code Cleanup"*/
/* *1    19-JUN-1989 12:54:27 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element HL_HI_ERASE.C*/
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

#ifndef ESI_SL_H

#include "esi_sl.h"

#endif

#ifndef ESI_QS_H

#include "esi_qs.h"

#endif

#ifndef ESI_WM_H

#include "esi_wm.h"

#endif

#ifndef ESI_WM_CLS_H

#include "esi_wm_cls.h"

#endif

#ifndef ESI_GKS_H

#include "esi_gks.h"

#endif

#ifndef ESI_MP_H

#include "esi_mp.h"

#endif

#ifndef ESI_AM_H

#include "esi_am.h"

#endif

#ifndef ESI_GB_H

#include "esi_gb.h"

#endif

#ifndef ESI_GB_ERR_H

#include "esi_gb_err.h"

#endif

/* Function Description -----------------------------------------------------
Description:
    Hl_erase_highlight is called to erase the highlight group
    associated with the specified data_type in the current map window.

    The highlight group is recreated after deletion, to prepare
    for new highlights.

    INITIAL CONDITIONS:

        1. The current map window must exist, and must be closed.
        2. The highlight overlay and the desired highlight group must exist.

Prototype:
    publicdef INT hl_erase_highlight(SL_DATA_TYPE data_type);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) data_type   -SL_DATA_TYPE) Specified type set to either WELLS_DATA_TYPE
                        or SEISMIC_DATA_TYPE.

Return Value/Status:
    SUCCESS - If required window & group are defined, and 
                data_type validates O.K.,
    FAIL - Otherwise.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT hl_erase_highlight (SL_DATA_TYPE data_type)
#else
publicdef INT hl_erase_highlight (data_type)
SL_DATA_TYPE data_type;
#endif
    {
    INT status, window_id, hilite_overlay_id, hilite_group_id, curr_map_number, length;
    MAP_STRUCTURE *cur_map_workspace;
    status = SUCCESS;
    
    /* get appropriate wm group id to be erased */
    /* query system state variables */
    
    status = mp_front_map (&cur_map_workspace);
    if (status != SUCCESS)
        {
        am_message (AM_ALERT, "No map defined");
        return status;
        }
    window_id = cur_map_workspace->map_window_id;
    curr_map_number = cur_map_workspace->map_number;
    
    switch (data_type)
        {
    case WELLS_DATA_TYPE:
        qs_inq_array_i (QS_MAP_WELL_HILITE_GROUP, curr_map_number, &hilite_group_id);
        break;
        
    case SEISMIC_DATA_TYPE:
        qs_inq_array_i (QS_MAP_SEISMIC_HILITE_GROUP, curr_map_number,
                        &hilite_group_id);
        break;
        
    default:
        status = FAIL;
        break;
        }
    if (window_id)
        {
        qs_inq_array_i (QS_MAP_HILITE_OVERLAY, curr_map_number, &hilite_overlay_id);
        if (hilite_overlay_id && hilite_group_id)
            {
            wmwopen (window_id);
            wmgopen (hilite_overlay_id);
            wmgdelete (hilite_group_id);
            wm_grsgwk (MAIN_WKID);
            status = hl_hilite_group (data_type, &hilite_group_id);
            wmgclose (hilite_overlay_id);
            wmwclose (window_id);
            wm_update ();
            }
        else
            {
            status = FAIL;          /* a group is missing */
            }
        }
    else
        {
        status = FAIL;              /* the window is missing */
        }
    return status;
    }
/* END:     */
