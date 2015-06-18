/* DEC/CMS REPLACEMENT HISTORY, Element CO_CON_INT.C*/
/* *2    14-AUG-1990 11:42:03 VINCE "(SPR 5644) Header Standardization"*/
/* *1    19-JUN-1989 12:01:47 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element CO_CON_INT.C*/
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
#include "esi_co.h"

#ifndef ESI_NL_H

#include "esi_nl.h"

#endif

#ifndef ESI_AM_H

#include "esi_am.h"

#endif

/* Function Description -----------------------------------------------------
Description:
    Function to automatically calculate a "reasonable" contour
    interval.

Prototype:
    publicdef INT co_contour_interval(CONTOURS *contours, NLIST_HEADER grid_nlist);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) contours    -(CONTOURS *) Contour overlay information.
    (O) grid_nlist  -(NLIST_HEADER) N-list to store grid_z.

Return Value/Status:
    SUCCESS - Successful completion
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT co_contour_interval (CONTOURS *contours, NLIST_HEADER grid_nlist) 
#else
publicdef INT co_contour_interval (contours, grid_nlist)
CONTOURS *contours;
NLIST_HEADER grid_nlist;
#endif
/* * VARIABLE LIST * */
/* ----------------- */
    {
    
    INT status;                     /* RETURN STATUS */
    INT i;                          /* DUMMY INDEX */
    FLOAT max;
    FLOAT min;
    DOUBLE interval;
    DOUBLE adjustment;
    INT power = 0;
    /* NLIST INFORMATION */
    UINT d_list[1];                 /*  DIM LIST */
    VOIDPTR v_list[1];              /*  VALUE LIST */
    
    /* ********************************************************************** */
    
    status = SUCCESS;
    
    /* GET MAX VALUE */
    
    d_list[0] = 1, v_list[0] = (VOIDPTR) & max;
    nl_inq_nlist_info (grid_nlist, NL_NLIST_MAX, 1, d_list, v_list);
    
    /* GET MIN VALUE */
    
    v_list[0] = (VOIDPTR) & min;
    nl_inq_nlist_info (grid_nlist, NL_NLIST_MIN, 1, d_list, v_list);
    
    /* ROUGH ESTIMATE OF INTERVAL */
    
    interval = (max - min) / 15.0;
    
    /* ORDER OF MAGNITUDE OF ESTIMATE */
    
    if (interval < 1.0)
        {
        adjustment = 10.0;
        
        FOREVER
            {
            interval *= adjustment;
            power++;
            if (interval >= 1.0)
                {
                break;
                }
            }
        }
    else
        {
        adjustment = 0.1;
        
        FOREVER
            {
            if (interval <= 10.0)
                {
                break;
                }
            interval *= adjustment;
            power++;
            }
        }
    /* REFINE ESTIMATE */
    
    if (interval < 1.6)
        {
        interval = 1.0;
        }
    else if (interval < 4.1)
        {
        interval = 2.0;
        }
    else
        {
        interval = 5.0;
        }
    for (i = 0; i < power; i++)
        {
        interval /= adjustment;
        }
    contours->contour_interval = (CHAR *)am_allocate (AM_APPLICATION, 50);
    sprintf (contours->contour_interval, "%f", interval);
    
    if (adjustment < 1.0)
        {
        contours->contour_interval[power + 1] = '\0';
        }
    else
        {
        contours->contour_interval[power + 2] = '\0';
        }
    return status;
    }
/* END:     */
