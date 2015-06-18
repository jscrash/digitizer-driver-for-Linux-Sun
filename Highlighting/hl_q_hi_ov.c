/* DEC/CMS REPLACEMENT HISTORY, Element HL_Q_HI_OV.C*/
/* *2    17-AUG-1990 21:49:40 VINCE "(SPR 5644) Code Cleanup"*/
/* *1    19-JUN-1989 12:54:56 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element HL_Q_HI_OV.C*/
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
#include "esi_qs.h"
#include "esi_wm.h"
#include "esi_wm_cls.h"
#include "esi_gb.h"
#include "esi_mp.h"

/* Function Description -----------------------------------------------------
Description:
    Hl_query_hilite_overlay is called to find the WM group id
    of the highlight overlay, and associated subgroups,
    for the specified window.

    The highlight overlay, and associated subgroups, are created
    if they do not already exist.

    INITIAL CONDITIONS:

         1. The map window must exist.
         2. The map window must be closed (because WM so fussy).

Prototype:
    publicdef INT hl_query_hilite_overlay( INT app_id, INT window_id,
        INT *hilite_overlay_group,INT *well_hilite_group,INT *seismic_picking_group,
        INT *seismic_hilite_group);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    app_id          -(INT) WM application id owning window_id.
    window_id       -(INT) WM window id of the desired map window.
    hilite_overlay_group    -(INT *) Returned highlight overlay WM group id.
    well_hilite_group       -(INT *) Returned well hilite WM group id.
    seismic_picking_group   -(INT *) Returned seismic picking WM group id.
    seismic_hilite_group    -(INT *) Returned seismic hilite WM group id.

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT hl_query_hilite_overlay (INT app_id, INT window_id,
                                       INT *hilite_overlay_group,
                                       INT *well_hilite_group,
                                       INT *seismic_picking_group,
                                       INT *seismic_hilite_group)
#else
publicdef INT hl_query_hilite_overlay (app_id, window_id, hilite_overlay_group,
                                       well_hilite_group, seismic_picking_group,
                                       seismic_hilite_group)
INT app_id;
INT window_id;
INT *hilite_overlay_group;
INT *well_hilite_group;
INT *seismic_picking_group;
INT *seismic_hilite_group;
#endif
    {
    INT if_exists, status;
    MAP_STRUCTURE *mapdef;
    
    status = SUCCESS;
    
    status = wmoexists (app_id, window_id, 0, 0, &if_exists);
    if (!if_exists)
        {
        status = FAIL;
        return status;
        }
    /* if the hilite overlay has not been created yet, allocate the
       hilite overlay group and its subgroups */
    
    mp_get_current_map (&mapdef);
    status = qs_inq_array_i (QS_MAP_HILITE_OVERLAY, mapdef->map_number,
                             hilite_overlay_group);
    if ((!*hilite_overlay_group) OR (status < 0))
        {
        status = SUCCESS;
        status = wmwopen (window_id);
        status = wmgallocate (hilite_overlay_group);
        qs_set_array_i (QS_MAP_HILITE_OVERLAY, mapdef->map_number,
                        *hilite_overlay_group);
        status = wmoset (*hilite_overlay_group, WM_CLASS_HILITE_OVERLAY, "",
                         WM_CLSTR_HILITE_OVERLAY);
        
        status = wmgallocate (seismic_picking_group);
        status = qs_set_array_i (QS_MAP_SEISMIC_PICKING_GROUP, mapdef->map_number,
                                 *seismic_picking_group);
        status = wmoset (*seismic_picking_group, WM_CLASS_SEISMIC_HILITE, "",
                         "Seismic By Pointing");
        status = wmgclose (*seismic_picking_group);
        
        status = hl_hilite_group (WELLS_DATA_TYPE, well_hilite_group);
        status = hl_hilite_group (SEISMIC_DATA_TYPE, seismic_hilite_group);
        
        status = wmgclose (*hilite_overlay_group);
        status = wmwclose (window_id);
        }
    else
        {
        status = qs_inq_array_i (QS_MAP_WELL_HILITE_GROUP, mapdef->map_number,
                                 well_hilite_group);
        status = qs_inq_array_i (QS_MAP_SEISMIC_PICKING_GROUP, mapdef->map_number,
                                 seismic_picking_group);
        status = qs_inq_array_i (QS_MAP_SEISMIC_HILITE_GROUP, mapdef->map_number,
                                 seismic_hilite_group);
        }
    return status;
    }
/* END:     */
