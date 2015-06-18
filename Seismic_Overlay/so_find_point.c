/* DEC/CMS REPLACEMENT HISTORY, Element SO_FIND_POINT.C*/
/* *5    18-JUL-1991 15:51:55 MING "(SPR 0) add nl_set_current_point"*/
/* *4    26-JUL-1990 17:20:37 GILLESPIE "(SPR 1) Lint run; incorporate changes for horizon end point posting"*/
/* *3    25-JUL-1990 20:04:08 VINCE "(SPR 5644) Header Standardization"*/
/* *2    25-JUL-1989 20:06:11 GILLESPIE "(SPR 50) Seismic system revamp"*/
/* *1    19-JUN-1989 13:26:43 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element SO_FIND_POINT.C*/
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

#include "esi_so.h"

/* Function Description -----------------------------------------------------
Description:
    Function to find a cdp in a cdp nlist.

Prototype:
    publicdef INT so_find_point(NLIST_HEADER nlist,CDP cdp);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    nlist           -(NLIST_HEADER) Nlist header.
    cdp             -(CDP) Cdp to find.

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT so_find_point (NLIST_HEADER nlist, CDP cdp)
#else
publicdef INT so_find_point (nlist, cdp)
NLIST_HEADER nlist;
CDP cdp;
#endif
    {
    INT status;
    UINT point, stroke, npoints, ndim, ngot;
    CDP lookcdp;
    UINT i;
    UINT Col_List[1];
    VOIDPTR Var_List[1];
    INT save_point;
    
    status = FAIL;
    
    /* check empty list */
    
    nl_inq_nlist_int (nlist, NL_NLIST_NROWS, (INT *) & npoints);
    
    if (npoints == 0)
        {
        return status;
        }
    Col_List[0] = 1;
    Var_List[0] = (VOIDPTR) & lookcdp;
    ndim = 1;
    
    nl_inq_current_point (nlist, &point, &stroke);
    nl_get_points (nlist, 1, ndim, Col_List, Var_List, &ngot);
    
    for (i = 0; i < npoints; i++)
        {
        if (lookcdp > cdp)
            {
            status = FAIL;
            break;
            }
        if (lookcdp == cdp)
            {
            status = SUCCESS;
	    save_point = point + i;
            break;
            }
        status = nl_next_point (nlist, 1, Col_List, Var_List);
        if (status < 0)
            {
            status = FAIL;
            break;
            }
        }
    if (status == FAIL)
        {
        nl_set_current_point (nlist, point, stroke);
        }
    else
	{
        nl_set_current_point (nlist, save_point, stroke);
        }

    return status;
    }
/* END:     */
