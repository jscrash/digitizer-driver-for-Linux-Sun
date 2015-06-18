/* DEC/CMS REPLACEMENT HISTORY, Element CT_TRANS_PT.C*/
/* *6    14-AUG-1990 13:07:55 VINCE "(SPR 5644) Header Standardization"*/
/* *5     6-NOV-1989 14:25:08 GILLESPIE "(SPR 30) Change name of glue routine"*/
/* *4    19-OCT-1989 08:35:38 GORDON "(SPR 999) Undo last fix - it was wrong"*/
/* *3    18-OCT-1989 12:52:34 GORDON "(SPR 999) fix call to ct_gtp$0_gl (address of return vars)"*/
/* *2    10-OCT-1989 09:45:34 JULIAN "(SPR 5020) mods for CT parameter change"*/
/* *1    19-JUN-1989 12:45:33 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element CT_TRANS_PT.C*/
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

#include "esi_ct.h"

/* Function Description -----------------------------------------------------
Description:
    This function transforms a point in one projection to another.

Prototype:
    publicdef INT ct_transform_point(DOUBLE x_coord_in, DOUBLE y_coord_in,
                                 PROJECTION_STRUCTURE *projection_in,
                                 DOUBLE *x_coord_out, DOUBLE *y_coord_out,
                                 PROJECTION_STRUCTURE *projection_out);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) x_coord_in      -(DOUBLE) X-coordinate input point.
    (I) y_coord_in      -(DOUBLE) Y-coordinate input point.
    (O) x_coord_out     -(DOUBLE *) X-coordinate output transformed point.
    (O) y_coord_out     -(DOUBLE *) Y-coordinate output transformed point.
    (I) projection_in   -(PROJECTION_STRUCTURE *) Projection of input point.
    (O) projection_out  -(PROJECTION_STRUCTURE *) Projection of output point.
                                 
Return Value/Status:
    SUCCESS - Successful completion.
    If function returns a non-zero, then an error occured.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT ct_transform_point (DOUBLE x_coord_in, DOUBLE y_coord_in,
                                  PROJECTION_STRUCTURE *projection_in,
                                  DOUBLE *x_coord_out, DOUBLE *y_coord_out,
                                  PROJECTION_STRUCTURE *projection_out) 
#else
publicdef INT ct_transform_point (x_coord_in, y_coord_in, projection_in, x_coord_out,
                                  y_coord_out, projection_out)
DOUBLE x_coord_in;
DOUBLE y_coord_in;
DOUBLE *x_coord_out;
DOUBLE *y_coord_out;
PROJECTION_STRUCTURE *projection_in;
PROJECTION_STRUCTURE *projection_out;
#endif
    {
    
    INT error_flag = SUCCESS;       /*  Error flag returned from GCTP  */
    
    /*  Pass arguments to Fortran interface for GCTP routines   */
    
    if (projection_in EQUALS projection_out)
        {
        *x_coord_out = x_coord_in;
        *y_coord_out = y_coord_in;
        }
    else
        {
        ctg_gtp$0(x_coord_in, y_coord_in, projection_in, x_coord_out, y_coord_out,
                  projection_out, &error_flag);
        }
    /*  Error_flag non_zero means error occured  */
    
    return error_flag;
    
    }
/* END:     */
