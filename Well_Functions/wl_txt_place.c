/* DEC/CMS REPLACEMENT HISTORY, Element WL_TXT_PLACE.C*/
/* *3    23-MAY-1991 10:06:01 MING "(SPR -1) correct comment and casting (CHARLIE)"*/
/* *2    14-AUG-1990 13:43:10 VINCE "(SPR 5644) Header Standardization"*/
/* *1    19-JUN-1989 13:40:24 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element WL_TXT_PLACE.C*/
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

#include "esi_math.h"

#ifndef ESI_C_LIB_H

#include "esi_c_lib.h"

#endif

#define HALF_PI (PI/2.0)

/* Function Description -----------------------------------------------------
Description:

Prototype:
    publicdef INT wl_text_placement(DOUBLE size,DOUBLE x1,DOUBLE y1,DOUBLE x2,DOUBLE y2,
        DOUBLE x3,DOUBLE y3,DOUBLE *x_offset,DOUBLE *y_offset,DOUBLE *x_up,DOUBLE *y_up);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    size            -(DOUBLE)
    x1              -(DOUBLE)
    y1              -(DOUBLE)
    x2              -(DOUBLE)
    y2              -(DOUBLE)
    x3              -(DOUBLE)
    y3              -(DOUBLE)
    x_offset        -(DOUBLE *)
    y_offset        -(DOUBLE *)
    x_up            -(DOUBLE *)
    y_up            -(DOUBLE *)

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT wl_text_placement (DOUBLE size, DOUBLE x1, DOUBLE y1, DOUBLE x2,
                                 DOUBLE y2, DOUBLE x3, DOUBLE y3, DOUBLE *x_offset,
                                 DOUBLE *y_offset, DOUBLE *x_up, DOUBLE *y_up) 
#else
publicdef INT wl_text_placement (size, x1, y1, x2, y2, x3, y3, x_offset, y_offset,
                                 x_up, y_up)
DOUBLE size;
DOUBLE x1;
DOUBLE y1;
DOUBLE x2;
DOUBLE y2;
DOUBLE x3;
DOUBLE y3;
DOUBLE *x_offset;
DOUBLE *y_offset;
DOUBLE *x_up;
DOUBLE *y_up;
#endif
    {
    
    INT status;
    DOUBLE theta;
    DOUBLE phi;
    DOUBLE up_ang;
    DOUBLE offset_ang;
    DOUBLE dummy;
    
    /* ********************************************************************** */
    
    status = SUCCESS;
    
    /* * SENT THREE COINCIDENT POINTS * */
    /* -------------------------------- */
    
    if ((x1 EQUALS x2) AND (x2 EQUALS x3) AND (y1 EQUALS y2) AND (y2 EQUALS y3))
        {
        *x_offset = size;
        *y_offset = 0.0;
        *x_up = 0.0;
        *y_up = 1.0;
        goto finished;
        }
    /* * 2ND POINT DISTINCT FROM 1ST AND 3RD POINT * */
    /* --------------------------------------------- */
    
    else if (((x1 != x2) OR (y1 != y2)) AND ((x2 != x3) OR (y2 != y3)))
        {
        theta = atan2 ((y1 - y2), (x1 - x2));
        phi = atan2 ((y3 - y2), (x3 - x2));
        offset_ang = (theta + phi) / 2.0;
        }
    /* * 2ND POINT DIFFERENT THAN 1ST POINT * */
    /* -------------------------------------- */
    
    else if ((x1 != x2) OR (y1 != y2))
        {
        offset_ang = atan2 ((y1 - y2), (x1 - x2)) + HALF_PI;
        }
    /* * 2ND POINT DIFFERENT THAN 3RD POINT * */
    /* -------------------------------------- */
    
    else if ((x2 != x3) OR (y2 != y3))
        {
        offset_ang = atan2 ((y3 - y2), (x3 - x2)) + HALF_PI;
        }
    /* * COMPUTE OFFSET ANGLE (BISECTOR) AND UP ANGLE (TANGENT) * */
    /* ---------------------------------------------------------- */
    
    if (fabs (offset_ang) > HALF_PI)
        {
        offset_ang = (offset_ang < 0.0) ? offset_ang + PI : offset_ang - PI;
        }
    up_ang = offset_ang + HALF_PI;
    
    /* * CALCULATE X_UP AND Y_UP * */
    /* --------------------------- */
    
    dummy = fabs (up_ang);
    
    /* VERTICAL LINE */
    if (1.55 < dummy AND dummy < 1.59)
        {
        *x_up = 0.0;
        *y_up = 1.0;
        }
    /* NON-VERTICAL LINE */
    else if (up_ang < HALF_PI)
        {
        *x_up = 1.0;
        *y_up = tan (up_ang);
        }
    else
        {
        *x_up = -1.0;
        *y_up = -tan (up_ang);
        }
    /* * CALCULATE X_OFFSET AND Y_OFFSET * */
    /* ----------------------------------- */
    
    dummy = fabs (offset_ang);
    
    /* VERTICAL LINE */
    if (1.55 < dummy AND dummy < 1.59)
        {
        *x_offset = 0.0;
        *y_offset = size;
        }
    /* NON-VERTICAL LINE */
    else
        {
        dummy = tan (offset_ang);
        
        *x_offset = size / sqrt (1.0 + (dummy * dummy));
        *y_offset = dummy * (*x_offset);
        }
finished:
    return status;
    }
/* END:     */
