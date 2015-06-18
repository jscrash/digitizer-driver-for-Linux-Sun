/* DEC/CMS REPLACEMENT HISTORY, Element HL_HI_GRP.C*/
/* *2    17-AUG-1990 21:48:59 VINCE "(SPR 5644) Code Cleanup"*/
/* *1    19-JUN-1989 12:54:33 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element HL_HI_GRP.C*/
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

#include "esi_gb.h"
#include "esi_c_lib.h"

#ifndef ESI_GL_DEFS_H

#include "esi_gl_defs.h"

#endif

#ifndef ESI_QS_H

#include "esi_qs.h"

#endif

#ifndef ESI_WM_H

#include "esi_wm.h"

#endif

#ifndef ESI_MP_H

#include "esi_mp.h"

#endif

#ifndef ESI_WM_CLS_H

#include "esi_wm_cls.h"

#endif

/* Function Description -----------------------------------------------------
Description:
    Hl_hilite_group is called to allocate the hilite group
    for the specified data type.

    INITIAL CONDITIONS:

         1. The map window must exist.
         2. The map window must be closed (because WM so fussy).
         3. The hilite overlay group must be allocated and OPEN.

Prototype:
    publicdef INT hl_hilite_group(SL_DATA_TYPE data_type, INT *hilite_group_id);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    data_type       -(SL_DATA_TYPE) One of {WELLS_DATA_TYPE, SEISMIC_DATA_TYPE, ...}.
    hilite_group_id -(INT *) Returned id of allocated group.

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT hl_hilite_group (SL_DATA_TYPE data_type, INT *hilite_group_id)
#else
publicdef INT hl_hilite_group (data_type, hilite_group_id)
SL_DATA_TYPE data_type;
INT *hilite_group_id;
#endif
    {
    INT status, qs_group_index, wm_class;
    CHAR wm_clstr[31];
    MAP_STRUCTURE *mapdef;
    
    status = SUCCESS;
    
    mp_get_current_map (&mapdef);
    switch (data_type)
        {
        
    case WELLS_DATA_TYPE:
        qs_group_index = QS_MAP_WELL_HILITE_GROUP;
        wm_class = WM_CLASS_WELL_HILITE;
        strcpy (wm_clstr, WM_CLSTR_WELL_HILITE);
        break;
        
    case SEISMIC_DATA_TYPE:
        qs_group_index = QS_MAP_SEISMIC_HILITE_GROUP;
        wm_class = WM_CLASS_SEISMIC_HILITE;
        strcpy (wm_clstr, WM_CLSTR_SEISMIC_HILITE);
        break;
        
    default:
        status = FAIL;
        }
    if (status EQUALS SUCCESS)
        {
        wmgallocate (hilite_group_id);
        wmgclose (*hilite_group_id);
        wmoset (*hilite_group_id, wm_class, "", wm_clstr);
        qs_set_array_i (qs_group_index, mapdef->map_number, *hilite_group_id);
        }
    return status;
    }
/* END:     */
