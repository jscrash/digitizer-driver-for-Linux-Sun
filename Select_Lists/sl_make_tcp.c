/* DEC/CMS REPLACEMENT HISTORY, Element SL_MAKE_TCP.C*/
/* *2    25-JUL-1990 20:08:14 VINCE "(SPR 5644) Header Standardization"*/
/* *1    19-JUN-1989 13:25:28 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element SL_MAKE_TCP.C*/
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

#include "esi_sl.h"

#ifndef ESI_TC_H

#include "esi_tc.h"

#endif

/* Function Description -----------------------------------------------------
Description:
    Turns an n-List into a table of character pointers.
    The data in the nlist is copied to a newly allocated
    region, and a tcp is allocated to point to this area.
    This is to prevent programmers from diddling in
    the nlist internals.

Prototype:
    publicdef INT sl_make_tcp(CHAR ***ptcp, NLIST_HEADER nlist, INT dim);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    tpcp            -(CHAR ***)
    nlist           -(NLIST_HEADER)
    dim             -(INT) Dimension to be used as source of list.

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT sl_make_tcp (CHAR ***ptcp, NLIST_HEADER nlist, INT dim)
#else
publicdef INT sl_make_tcp (ptcp, nlist, dim)
CHAR ***ptcp;
NLIST_HEADER nlist;
INT dim;
#endif
    {
    INT i;
    CHAR *pobject;
    CHAR **tcp;
    UINT ndim;
    INT status;
    UINT npoints;
    size_t data_size;
    UINT Dim_List[1];
    VOIDPTR Value_List[1];
    
    Dim_List[0] = dim;
    ndim = 1;
    Value_List[0] = (VOIDPTR) & data_size;
    if (nl_inq_nlist_info (nlist, NL_NLIST_DATA_WIDTHS, ndim, Dim_List,
                           Value_List) OR nl_inq_nlist_int (nlist, NL_NLIST_NPOINTS,
                                                           (INT *) &
                                                           npoints) OR (
                                                                   npoints 
                                                                 EQUALS 0) OR (
                                                                      data_size 
                                                                             EQUALS 0))
        {
        status = FAIL;
        *ptcp = (CHAR **)NULL;
        return status;
        }
    *ptcp = tcp = (CHAR **)tc_zalloc ((npoints + 1) * sizeof(CHAR *));
    Value_List[0] = pobject = (CHAR *)tc_zalloc (data_size);
    status = nl_first_point (nlist, ndim, Dim_List, Value_List);
    
    for (i = 1; i <= npoints; i++, tcp++)
        {
        *tcp = pobject;
        if (i != npoints)
            {
            Value_List[0] = pobject = (CHAR *)tc_zalloc (data_size);
            status = nl_next_point (nlist, ndim, Dim_List, Value_List);
            }
        }
    return SUCCESS;
    }
/* END:     */
