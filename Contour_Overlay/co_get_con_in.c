/* DEC/CMS REPLACEMENT HISTORY, Element CO_GET_CON_IN.C*/
/* *3    14-AUG-1990 11:42:27 VINCE "(SPR 5644) Header Standardization"*/
/* *2     7-MAR-1990 18:11:57 GILLESPIE "(SPR 1) Change tu... argument from FLOAT to DOUBLE"*/
/* *1    19-JUN-1989 12:02:25 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element CO_GET_CON_IN.C*/
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

/* Function Description -----------------------------------------------------
Description:
    Function to calculate and return the contour separation
    interval, and label precision information.

Prototype:
    publicdef INT co_get_con_info(CONTOURS *contours, DOUBLE *c_interval, 
                              INT *places);
                              
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) contours    -(CONTOURS *) Contours overlay information.
    (I) c_interval  -(DOUBLE *) Contour interval in map units.
    (I) places      -(INT *) Decimal places to display.

Return Value/Status:
    SUCCESS - Successful completion
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT co_get_con_info (CONTOURS *contours, DOUBLE *c_interval, INT *places) 
#else
publicdef INT co_get_con_info (contours, c_interval, places)
CONTOURS *contours;
DOUBLE *c_interval;
INT *places;
#endif
/* * VARIABLE LIST * */
/* ----------------- */
    {
    
    PROGNAME ("CO_GET_SEP");
    INT status;                     /* RETURN STATUS */
    UOM_NAME uom;                   /* UOM STRUCTURE */
    UOM_NAME dummy;                 /* DUMMY UOM STRUCTURE */
    CHAR *cptr;                     /* DUMMY POINTER */
    BOOL decimal_found = FALSE;     /* DECIMAL POINT FLAG */
    
    /* ********************************************************************** */
    
    status = SUCCESS;
    
    /* * INTERVAL VALUE * */
    /* ------------------ */
    
    tu_parse_unit (contours->contour_interval, c_interval, uom, dummy);
    
    /* * DETERMINE LABELLING PRECISION * */
    /* --------------------------------- */
    
    *places = 0;
    if (strchr (contours->contour_interval, '.') != NULL)
        {
        cptr = contours->contour_interval;
        FOREVER
            {
            
            /* LOOK FOR SPACE AFTER DECIMAL POINT */
            
            if (decimal_found)
                {
                if (*cptr EQUALS' 'OR * cptr EQUALS'\0')
                    {
                    break;
                    }
                (*places)++;
                }
            /* LOOK FOR TRUE DECIMAL POINT */
            
            else
                {
                if (*cptr EQUALS' 'OR * cptr EQUALS'\0')
                    {
                    break;
                    }
                else if (*cptr EQUALS'.')
                    {
                    (*places)++;
                    decimal_found = TRUE;
                    }
                }
            cptr++;
            }
        }
    (*places)--;
    
    return status;
    }
/* END:     */
