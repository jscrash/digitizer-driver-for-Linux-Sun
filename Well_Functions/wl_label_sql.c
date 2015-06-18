/* DEC/CMS REPLACEMENT HISTORY, Element WL_LABEL_SQL.C*/
/* *4    14-AUG-1990 13:41:40 VINCE "(SPR 5644) Header Standardization"*/
/* *3    17-MAY-1990 13:58:04 SCD "(SPR 1) Steer queries to unique index(UWI)"*/
/* *2    17-NOV-1989 14:13:13 GILLESPIE "(SPR 1) Change blocksize to 500; qualify the SOURCE column"*/
/* *1    19-JUN-1989 13:40:08 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element WL_LABEL_SQL.C*/
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

#ifndef ESI_C_LIB_H

#include "esi_c_lib.h"

#endif

#ifndef ESI_WL_H

#include "esi_wl.h"

#endif

#ifndef ESI_QS_H

#include "esi_qs.h"

#endif

#ifndef ESI_AM_H

#include "esi_am.h"

#endif

/* Function Description -----------------------------------------------------
Description:

Prototype:
    publicdef INT wl_label_sql(WELLS *wells);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    wells           -(WELLS *)

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT wl_label_sql (WELLS *wells) 
#else
publicdef INT wl_label_sql (wells)
WELLS *wells;
#endif
    {
    
    INT status;
    CHAR *dummy;
    PROJECT_NAME project;
    
    /* ********************************************************************** */
    
    status = SUCCESS;
    
    qs_inq_c (QS_PROJECT_NAME, project, (INT *)0);
    dummy = (CHAR *)am_allocate (AM_GLOBAL, 512);
    
    /* * WELL DEPTH * */
    /* -------------- */
    
    if (wells->ov_type EQUALS 1)
        {
        if (wells->source EQUALS NULL)
            {
            sprintf (dummy, 
"\
SELECT ROUND((ELEVATION-MD),2) \
FROM %s.WELL_TOPS X,%s.WELL_HDR Y \
WHERE X.UWI = Y.UWI AND X.FORM_CODE||'' = '%s' AND Y.UWI = :v"
                    , project, project, wells->bottom_horizon);
            }
        else
            {
            sprintf (dummy, 
"\
SELECT ROUND((ELEVATION-MD),2) \
FROM %s.WELL_TOPS X,%s.WELL_HDR Y \
WHERE X.UWI = Y.UWI AND X.FORM_CODE||'' = '%s' AND X.SOURCE = '%s' AND \
Y.UWI = :v"
                    , project, project, wells->bottom_horizon, wells->source);
            }
        }
    /* * WELL ISOPACH * */
    /* ---------------- */
    
    else
        {
        if (wells->source EQUALS NULL)
            {
            sprintf (dummy, 
"\
SELECT ROUND((Y.MD-X.MD),2) \
FROM %s.WELL_TOPS X,%s.WELL_TOPS Y \
WHERE X.UWI = Y.UWI AND X.SOURCE = Y.SOURCE AND X.FORM_CODE||'' = '%s' \
AND Y.FORM_CODE||'' = '%s' AND X.UWI = :v"
                    , project, project, wells->top_horizon, wells->bottom_horizon);
            }
        else
            {
            sprintf (dummy, 
"\
SELECT ROUND((Y.MD-X.MD),2) \
FROM %s.WELL_TOPS X,%s.WELL_TOPS Y \
WHERE X.SOURCE = '%s' AND X.SOURCE = Y.SOURCE \
AND X.FORM_CODE||'' = '%s' AND Y.FORM_CODE||'' = '%s' \
AND X.UWI = Y.UWI AND X.UWI = :v"
                    , project, project, wells->source, wells->top_horizon,
                     wells->bottom_horizon);
            }
        }
    /* * SET IN RIGHT POSITION * */
    /* ------------------------- */
    
    switch (wells->label_position)
        {
    case 1:                         /* ABOVE */
        wells->above = dummy;
        break;
        
    case 2:                         /* BELOW */
        wells->below = dummy;
        break;
        
    case 3:                         /* CENTERED */
        wells->centered = dummy;
        break;
        
    case 4:                         /* LEFT */
        wells->left = dummy;
        break;
        
    case 5:                         /* RIGHT */
        wells->right = dummy;
        break;
        }
    return status;
    }
/* END:     */
