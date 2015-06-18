/* DEC/CMS REPLACEMENT HISTORY, Element SO_PRC_LINE.C*/
/* *8    15-OCT-1991 15:56:21 MING "(SPR 4491) comment out wmsdelete"*/
/* *7     1-AUG-1990 13:18:56 VINCE "(SPR 5615) updated for integer line id"*/
/* *6    26-JUL-1990 17:22:02 GILLESPIE "(SPR 1) Lint run; incorporate changes for horizon end point posting"*/
/* *5    25-JUL-1990 20:05:52 VINCE "(SPR 5644) Header Standardization"*/
/* *4    14-SEP-1989 18:30:51 GORDON "(SPR -1) checkin from gulf"*/
/* *3     1-SEP-1989 14:37:45 CONROY "(SPR 9999) MODS FOR NEW FULL_LINE_STRUCT"*/
/* *2    25-JUL-1989 20:07:34 GILLESPIE "(SPR 50) Seismic system revamp"*/
/* *1    19-JUN-1989 13:27:14 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element SO_PRC_LINE.C*/
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

#include "esi_so.h"
#include "esi_wm.h"
#include "esi_wm_cls.h"

/* Function Description -----------------------------------------------------
Description:
    Function to process seismic line for mapping.

Prototype:
    publicdef INT so_process_line(SO_STRUCT *pso,BOOL segments,INT       *segment_id);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    pso             -(SO_STRUCT *) Seismic overlay structure.
    segments        -(BOOL) TRUE if should open segment.
    segment_id      -(INT *) Returns segment of line displayed.

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT so_process_line (SO_STRUCT *pso, BOOL segments, INT *segment_id)
#else
publicdef INT so_process_line (pso, segments, segment_id)
SO_STRUCT *pso;
BOOL segments;
INT *segment_id;
#endif
    {
    INT segment;
    INT status;
    CHAR dum_string[43];
    
    /* load seismic line */
    
    status = so_line_load (pso);
    if (status != SUCCESS)
        {
        return status;
        }
    /* see if we need the horizon */
    
    if (pso->seismic_generic.do_scatter OR pso->seismic_generic.
        draw_shotpoint_label OR pso->ov_type == SO_SEISMIC_MISTIE)
        {
        status = so_hor_load (pso);
        pso->seismic_generic.horizons_ok = status EQUALS SUCCESS;
        }
    else
        {
        pso->seismic_generic.horizons_ok = FALSE;
        }
    /* draw seismic line */
    
    if (segments)
        {
        wmsallocate (&segment);
        sprintf (dum_string, "%d:(%s)%s", pso->current_line.id,
                 pso->current_line.survey, pso->current_line.line_name);
        wmoset (segment, WM_CLASS_SEISMIC, dum_string, dum_string);
        *segment_id = segment;
        }

    status = so_draw_seismic_line (pso);
    
    if (status != SUCCESS)
        {
        if (segments)
            {
            wmsclose (segment);
/*
  CASE #4491
  The following function call is being commented out because it caused
  segment regeneration in the CGM hardcopy.

            wmsdelete (segment);
*/
            }
        return status;
        }
    /* draw fault posts */
    
    so_process_fault_posts (pso);
    
    /* close down segment */
    
    if (segments)
        {
        wmsclose (segment);
        }
    status = SUCCESS;
    return status;
    }
/* END:     */
