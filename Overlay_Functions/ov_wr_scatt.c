/* DEC/CMS REPLACEMENT HISTORY, Element OV_WR_SCATT.C*/
/* *2    25-JUL-1990 20:00:16 VINCE "(SPR 5644) Header Standardization"*/
/* *1    19-JUN-1989 13:22:22 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element OV_WR_SCATT.C*/
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
#include "esi_ov.h"
#include "esi_nl.h"

/* Function Description -----------------------------------------------------
Description:
    Function to write abs(npts) points to the supplied scatter
    file.  If npts<0 => first supplied point is to be first
    point in scatter file (use different format).

Prototype:
    publicdef INT ov_write_scatter(NLIST_HEADER scatter,FLOAT *x,FLOAT *y,FLOAT *z,
        INT npts);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    scatter         -(NLIST_HEADER) File to write to.
    x               -(FLOAT *) Pointer to X values to write.
    y               -(FLOAT *) Pointer to Y values to write.
    z               -(FLOAT *) Pointer to Z values to write.
    npts            -(INT) Number of points to write.

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT ov_write_scatter (NLIST_HEADER scatter, FLOAT *x, FLOAT *y, FLOAT *z,
                                INT npts)
#else
publicdef INT ov_write_scatter (scatter, x, y, z, npts)
NLIST_HEADER scatter;
FLOAT *x;
FLOAT *y;
FLOAT *z;
INT npts;
#endif
    {
    
    INT status;                     /* RETURN STATUS */
    UINT d_list[3];
    VOIDPTR v_list[3];
    
    /* ********************************************************************** */
    
    d_list[0] = 1;
    d_list[1] = 2;
    d_list[2] = 3;
    v_list[0] = (VOIDPTR)x;
    v_list[1] = (VOIDPTR)y;
    v_list[2] = (VOIDPTR)z;
    
    if ((status = nl_add_points (scatter, npts, 3, d_list, v_list)) >= SUCCESS)
        {
        status = SUCCESS;
        }
    return status;
    }
/* END:     */
