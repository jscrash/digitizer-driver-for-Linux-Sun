/* DEC/CMS REPLACEMENT HISTORY, Element CT_TRANS_NL.C*/
/* *6    14-AUG-1990 13:07:48 VINCE "(SPR 5644) Header Standardization"*/
/* *5    16-NOV-1989 13:26:22 GILLESPIE "(SPR 1) Reset current row to 1st row in table prior to replace"*/
/* *4    18-OCT-1989 09:46:04 JULIAN "(SPR 0) mod d_width type from INT to size_t"*/
/* *3    10-OCT-1989 09:45:06 JULIAN "(SPR 5020) mods for CT parameter change"*/
/* *2    30-JUN-1989 10:34:11 GILLESPIE "(SPR 33) Fix INT/UINT typedefs in calling sequence"*/
/* *1    19-JUN-1989 12:45:30 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element CT_TRANS_NL.C*/
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

#ifndef ESI_NL_H

#include "esi_nl.h"

#endif

#ifndef ESI_TC_H

#include "esi_tc.h"

#endif

/* Function Description -----------------------------------------------------
Description:
    Transform the points contained in an nlist from one projection
    to another.  

Prototype:
    publicdef INT ct_transform_nlist(NLIST_HEADER nlist_in, PROJECTION_STRUCTURE *projection_in,
                                 NLIST_HEADER *nlist_out, PROJECTION_STRUCTURE *projection_out,
                                 UINT Col_List[], DOUBLE conversion_tolerance);
                                 
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) nlist_in        -(NLIST_HEADER) Nlist to be transformed.
    (I) projection_in   -(PROJECTION_STRUCTURE *) Projection of input points.
    (O) nlist_out       -(NLIST_HEADER *) Transformed nlist to be output.
    (O) projection_out  -(PROJECTION_STRUCTURE *) Destination projection.
    (I) Col_List        -(UINT []) Array of dimension numbers.
    (I) conversion_tolerance    -(DOUBLE) Maximum distance between returned points.

Return Value/Status:
    SUCCESS - Successful completion
    Function returns status of nl_copy_nlist, nl_inq_nlist_int.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT ct_transform_nlist (NLIST_HEADER nlist_in,
                                  PROJECTION_STRUCTURE *projection_in,
                                  NLIST_HEADER *nlist_out,
                                  PROJECTION_STRUCTURE *projection_out,
                                  UINT Col_List[], DOUBLE conversion_tolerance) 
#else
publicdef INT ct_transform_nlist (nlist_in, projection_in, nlist_out, projection_out,
                                  Col_List, conversion_tolerance)
NLIST_HEADER nlist_in;
PROJECTION_STRUCTURE *projection_in;
NLIST_HEADER *nlist_out;
PROJECTION_STRUCTURE *projection_out;
UINT Col_List[2];
DOUBLE conversion_tolerance;
#endif
    {
    UINT ntables;
    /*-------------------------------*/
    /* Addresses of each dimension   */
    /* Destination size for points   */
    /*-------------------------------*/
    VOIDPTR pts_array[2];
    
    static size_t d_width[] = 
        {
        sizeof(DOUBLE), sizeof(DOUBLE)
        };
    static INT d_type[] = 
        {
        FLOAT_TYPE,
        FLOAT_TYPE
        };
    static INT d_units[] = 
        {
        0,
        0
        };
    INT status, i;
    UINT cur_table;                 /*  Current table                 */
    UINT max_pts = 0;
    UINT nrows;
    DOUBLE *dim1, *dim2;
    
    /**************************************************************************/
    
    /* * INITIALIZE NLIST_OUT * */
    /* ------------------------ */
    
    if ((status = nl_copy_nlist (nlist_in, nlist_out)) < 0)
        {
        return status;
        }
    /* * GET THE NUMBER OF TABLES * */
    /* ----------------------------- */
    
    if ((status = nl_inq_nlist_int (nlist_in, NL_NLIST_NTABLES,
                                    (INT *) & ntables)) < 0)
        {
        return status;
        }
    /* * ALLOCATE POINT ARRAYS * */
    /* ------------------------- */
    
    if ((status = nl_inq_nlist_int (*nlist_out, NL_NLIST_NROWS,
                                    (INT *) & max_pts)) < 0)
        {
        return status;
        }
    if (max_pts EQUALS 0)
        {
        return SUCCESS;
        }
    pts_array[0] = (VOIDPTR)TALLOC (sizeof(DOUBLE) * max_pts);
    pts_array[1] = (VOIDPTR)TALLOC (sizeof(DOUBLE) * max_pts);
    dim1 = (DOUBLE *)pts_array[0];
    dim2 = (DOUBLE *)pts_array[1];
    
    /* * PROCESS TABLE BY TABLE * */
    /* ---------------------------- */
    
    for (cur_table = 1; cur_table <= ntables; cur_table++)
        {
        
        /* SET THE TABLE */
        
        if (nl_set_current_row (*nlist_out, 1, cur_table) < 0)
            {
            break;
            }
        /* EXTRACT THE ROWS */
        
        if (nl_get_rows_convert (*nlist_out, max_pts, 2, Col_List, pts_array, d_units,
                                 d_width, d_type, &nrows) < 0 OR (nrows EQUALS 0))
            {
            continue;
            }
        /* TRANSFORM THE ROWS */
        
        for (i = 0; i < nrows; i++)
            {
            status = ct_transform_point (dim1[i], dim2[i], projection_in, &dim1[i],
                                         &dim2[i], projection_out);
            }
        /* STORE THE TRANSFORMED POINTS */
        
        if (nl_set_current_row (*nlist_out, 1, cur_table) < 0)
            {
            break;
            }
        nl_replace_points_convert (*nlist_out, nrows, 2, Col_List, pts_array, d_units,
                                   d_width, d_type);
        }
#if NEED_TO_FREE
    tc_free (pts_array[0]);
    tc_free (pts_array[1]);
#endif
    return SUCCESS;
    }
/* END:     */
