/* DEC/CMS REPLACEMENT HISTORY, Element SO_EXP_CDPXY.C*/
/* *6    26-JUL-1990 17:20:29 GILLESPIE "(SPR 1) Lint run; incorporate changes for horizon end point posting"*/
/* *5    25-JUL-1990 20:04:02 VINCE "(SPR 5644) Header Standardization"*/
/* *4    17-JAN-1990 15:39:37 WALTERS "(SPR 0) Add nl_init_nlist_info to name dimensions"*/
/* *3    11-AUG-1989 14:11:53 JULIAN "(SPR -1) illegal pointer combination"*/
/* *2    25-JUL-1989 20:06:04 GILLESPIE "(SPR 50) Seismic system revamp"*/
/* *1    19-JUN-1989 13:26:40 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element SO_EXP_CDPXY.C*/
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
#include "esi_tc.h"

static CHAR *c_name[4] = 
    {
    "CDP",
    "X",
    "Y",
    NULL
    };

/* Function Description -----------------------------------------------------
Description:
    Take a cdp x,y nlist and make a value for every cdp.      
    This function makes the interpolates the nlist so that an
    entry exists for each cdp.

Prototype:
    publicdef INT so_explode_cdpxy(NLIST_HEADER nlist,NLIST_HEADER *newlist);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    nlist           -(NLIST_HEADER) Old nlist.
    newlist         -(NLIST_HEADER *) New nlist.

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT so_explode_cdpxy (NLIST_HEADER nlist, NLIST_HEADER *newlist)
#else
publicdef INT so_explode_cdpxy (nlist, newlist)
NLIST_HEADER nlist;
NLIST_HEADER *newlist;
#endif
    {
    
    VOIDPTR Var_List[3];          /* nlist stuff */
    UINT Col_List[3];
    INT units[3];
    INT value_inc[3];
    UINT ndim = 3;
    INT data_types[3];
    size_t data_sizes[3];
    
    /* local nlist interpolation variables */
    
    UINT ntables, nrows, table_num, table, ngot;
    UINT n = 0;
    CDP cdp, lastcdp;
    UINT i;
    INT status;
    COORD mx, bx, my, by;
    FLOAT newx, newy;               /* 4 byte floats */
    FLOAT *xs, *ys;
    COORD lastx, lasty;
    CDP *cdps, addcdp;
    
    /* initialize new nlist */
    
    units[0] = units[1] = units[2] = 0;
    value_inc[0] = value_inc[1] = value_inc[2] = VALUE_LIST;
    data_types[0] = INT_TYPE;
    data_types[1] = data_types[2] = FLOAT_TYPE;
    data_sizes[0] = data_sizes[1] = data_sizes[2] = sizeof(FLOAT);
    Col_List[0] = 1;
    Col_List[1] = 2;
    Col_List[2] = 3;
    
    /* assume SUCCESS */
    
    status = SUCCESS;
    
    status = nl_init_nlist (newlist, ndim, units, data_sizes, data_types, value_inc,
                            0);
    
    if (status != SUCCESS)
        {
        return status;
        }
    status = nl_set_nlist_info (*newlist, NL_NLIST_COLUMN_NAME, ndim, Col_List,
                                (VOIDPTR *)c_name);
    if (status != SUCCESS)
        {
        return status;
        }
    /* find out about old nlist */
    
    nl_inq_nlist_int (nlist, NL_NLIST_NTABLES, (INT *) & ntables);
    
    for (table = 0; table < ntables; table++)
        {
        nl_inq_table_int (nlist, table + 1, NL_TABLE_NROWS, (INT *) & nrows);
        if (nrows == 0)
            {
            continue;
            }
        /* get uninterpolated rows */
        
        if (nrows > n)
            {
            if (n > 0)
                {
                xs = (FLOAT *)tc_realloc ((VOIDPTR)xs, nrows * sizeof(FLOAT));
                ys = (FLOAT *)tc_realloc ((VOIDPTR)ys, nrows * sizeof(FLOAT));
                cdps = (CDP *)tc_realloc ((VOIDPTR)cdps, nrows * sizeof(CDP));
                }
            else
                {
                xs = (FLOAT *)tc_alloc (nrows * sizeof(FLOAT));
                ys = (FLOAT *)tc_alloc (nrows * sizeof(FLOAT));
                cdps = (CDP *)tc_alloc (nrows * sizeof(CDP));
                }
            n = nrows;
            }
        Var_List[0] = (VOIDPTR)cdps;
        Var_List[1] = (VOIDPTR)xs;
        Var_List[2] = (VOIDPTR)ys;
        nl_get_rows (nlist, nrows, ndim, Col_List, Var_List, &ngot);
        status = nl_start_table (*newlist, &table_num);
        
        Var_List[0] = (VOIDPTR) & addcdp;
        Var_List[1] = (VOIDPTR) & newx;
        Var_List[2] = (VOIDPTR) & newy;
        
        /* add first row */
        
        addcdp = *cdps;
        newx = *xs;
        newy = *ys;
        
        lastcdp = *cdps;
        lastx = newx;
        lasty = newy;
        
        status = nl_add_rows (*newlist, 1, ndim, Col_List, Var_List);
        
        for (i = 1; i < nrows; i++)
            {
            if (cdps[i] == lastcdp)
                {
                continue;
                }
            /* make new rows */
            
            mx = (xs[i] - lastx) / (cdps[i] - lastcdp);
            bx = lastx - mx * lastcdp;
            my = (ys[i] - lasty) / (cdps[i] - lastcdp);
            by = lasty - my * lastcdp;
            
            for (cdp = lastcdp + 1; cdp <= cdps[i]; cdp++)
                {
                addcdp = cdp;
                newx = cdp * mx + bx;
                newy = cdp * my + by;
                status = nl_add_rows (*newlist, 1, ndim, Col_List, Var_List);
                
                }
            /* end cdp for */
            
            lastcdp = cdps[i];
            lastx = xs[i];
            lasty = ys[i];
            }
        /* end nrows for */
        
        }
    /* end ntables for */
    
    tc_free ((VOIDPTR)cdps);
    tc_free ((VOIDPTR)xs);
    tc_free ((VOIDPTR)ys);
    nl_free_nlist (nlist);
    
    return status;
    }
/* END:     */
