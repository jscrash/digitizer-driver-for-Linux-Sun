/* DEC/CMS REPLACEMENT HISTORY, Element OV_READ_SCATT.C*/
/* *2    25-JUL-1990 19:59:51 VINCE "(SPR 5644) Header Standardization"*/
/* *1    19-JUN-1989 13:22:10 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element OV_READ_SCATT.C*/
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
#include "esi_gl_defs.h"
#include "esi_am.h"
#include "esi_nl.h"
#include "esi_ov.h"

/* Function Description -----------------------------------------------------
Description:
    Function to read and return all the (x,y,z) values from
    the supplied scatter file. Also the number of points read
    is returned.

Prototype:
    publicdef INT ov_read_scatter(NLIST_HEADER scatter,FLOAT **x,FLOAT **y,FLOAT **z,
        INT *npts);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    scatter         -(NLIST_HEADER) Scatter file to read.
    x               -(FLOAT **) X value array.
    y               -(FLOAT **) Y value array.
    z               -(FLOAT **) Z value array.
    npts            -(INT *) Number of triplets read.

Return Value/Status:
    SUCCESS - Successful completion.
    Function returns status from nl_get_points and nl_inq_nlist_int.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT ov_read_scatter (NLIST_HEADER scatter, FLOAT **x, FLOAT **y, FLOAT **z,
                               INT *npts)
#else
publicdef INT ov_read_scatter (scatter, x, y, z, npts)
NLIST_HEADER scatter;
FLOAT **x;
FLOAT **y;
FLOAT **z;
INT *npts;
#endif
    {
    
    INT status;                     /* RETURN STATUS */
    UINT d_list[3];
    VOIDPTR v_list[3];
    UINT n;
    
    /* ********************************************************************** */
    
    if ((status = nl_inq_nlist_int (scatter, NL_NLIST_NPOINTS, npts)) EQUALS SUCCESS)
        {
        *x = (FLOAT *)am_allocate (AM_APPLICATION, (*npts) * sizeof(FLOAT));
        *y = (FLOAT *)am_allocate (AM_APPLICATION, (*npts) * sizeof(FLOAT));
        *z = (FLOAT *)am_allocate (AM_APPLICATION, (*npts) * sizeof(FLOAT));
        
        d_list[0] = 1;
        d_list[1] = 2;
        d_list[2] = 3;
        v_list[0] = (VOIDPTR)(*x);
        v_list[1] = (VOIDPTR)(*y);
        v_list[2] = (VOIDPTR)(*z);
        if ((status = nl_get_points (scatter, *npts, 3, d_list, v_list,
                                     &n)) >= SUCCESS)
            {
            status = SUCCESS;
            }
        }
    return status;
    }
/* END:     */
