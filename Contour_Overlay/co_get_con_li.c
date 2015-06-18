/* DEC/CMS REPLACEMENT HISTORY, Element CO_GET_CON_LI.C*/
/* *3    14-AUG-1990 11:42:32 VINCE "(SPR 5644) Header Standardization"*/
/* *2     7-MAR-1990 18:12:04 GILLESPIE "(SPR 1) Change tu... argument from FLOAT to DOUBLE"*/
/* *1    19-JUN-1989 12:02:27 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element CO_GET_CON_LI.C*/
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

#ifndef ESI_CO_H

#include "esi_co.h"

#endif

#ifndef ESI_TU_H

#include "esi_tu.h"

#endif

#ifndef ESI_NL_H

#include "esi_nl.h"

#endif

/* Function Description -----------------------------------------------------
Description:
    Function to determine and return the contour line limit
    information to send to the DGI.

Prototype:
    publicdef INT co_get_contour_limits(CONTOURS *contours, NLIST_HEADER grid_nlist,
                                    DOUBLE *contour_min, DOUBLE *contour_max);
                                    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) contours    -(CONTOURS *) Contour overlay information.
    (I) grid_nlist  -(NLIST_HEADER) N-list containing grid information.
    (I) contuor_min -(DOUBLE *) Minimum value for contour.
    (I) contour_max -(DOUBLE *) Maximum value for contour.

Return Value/Status:
    SUCCESS - Successful completion
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT co_get_contour_limits (CONTOURS *contours, NLIST_HEADER grid_nlist,
                                     DOUBLE *contour_min, DOUBLE *contour_max) 
#else
publicdef INT co_get_contour_limits (contours, grid_nlist, contour_min, contour_max)
CONTOURS *contours;
NLIST_HEADER grid_nlist;
DOUBLE *contour_min;
DOUBLE *contour_max;
#endif
/* * VARIABLE LIST * */
/* ----------------- */
    {
    
    PROGNAME ("CO_GET_CONTOUR_LIMITS");
    INT status;                     /* RETURN STATUS */
    UOM_NAME uom1;
    UOM_NAME uom2;
    DOUBLE range;
    DOUBLE grid_min;
    DOUBLE grid_max;
    INT i;
    
    UINT d_list[1];
    VOIDPTR v_list[1];
    
    /* ********************************************************************** */
    
    status = SUCCESS;
    
    if (contours->contour_minimum EQUALS NULL OR contours->contour_maximum EQUALS NULL)
        {
        d_list[0] = 1;
        v_list[0] = (VOIDPTR) & grid_min;
        nl_inq_nlist_info (grid_nlist, NL_NLIST_MIN, 1, d_list, v_list);
        
        v_list[0] = (VOIDPTR) & grid_max;
        nl_inq_nlist_info (grid_nlist, NL_NLIST_MAX, 1, d_list, v_list);
        
        range = 2.0 * (grid_max - grid_min);
        
        *contour_max = grid_max + range;
        
        if (contours->isopach)
            {
            *contour_min = 0.0;
            }
        else
            {
            *contour_min = grid_min - range;
            }
        }
    /* * GET CONTOUR MINIMUM FROM SUPPLIED STRING * */
    /* ----------------------------------------- */
    
    if (contours->contour_minimum != NULL)
        {
        tu_parse_unit (contours->contour_minimum, contour_min, uom1, uom2);
        }
    /* * GET CONTOUR MAXIMUM FROM SUPPLIED STRING * */
    /* ----------------------------------------- */
    
    if (contours->contour_maximum != NULL)
        {
        tu_parse_unit (contours->contour_maximum, contour_max, uom1, uom2);
        }
    return status;
    }
/* END:     */
