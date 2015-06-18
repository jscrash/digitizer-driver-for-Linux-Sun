/* DEC/CMS REPLACEMENT HISTORY, Element SO_EXPLODE.C*/
/* *6    26-JUL-1990 17:19:57 GILLESPIE "(SPR 1) Lint run; incorporate changes for horizon end point posting"*/
/* *5    25-JUL-1990 20:03:57 VINCE "(SPR 5644) Header Standardization"*/
/* *4    17-JAN-1990 15:39:26 WALTERS "(SPR 0) Add nl_init_nlist_info to name dimensions"*/
/* *3    11-AUG-1989 14:11:41 JULIAN "(SPR -1) illegal pointer combination"*/
/* *2    25-JUL-1989 20:05:56 GILLESPIE "(SPR 50) Seismic system revamp"*/
/* *1    19-JUN-1989 13:26:36 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element SO_EXPLODE.C*/
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
#include "esi_am.h"

static CHAR *c_name[3] = 
    {
    "CDP",
    "TIMES",
    NULL
    };
    
/* Function Description -----------------------------------------------------
Description:
    Take a horizon nlist and make a value for every cdp.
    
Prototype:
    publicdef INT so_explode(NLIST_HEADER nlist,NLIST_HEADER *newlist,DOUBLE bulk_shift);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    nlist           -(NLIST_HEADER) Old nlist.
    newlist         -(NLIST_HEADER *) New nlist.
    bulk_shift      -(DOUBLE) Bulk shift for line.

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT so_explode (NLIST_HEADER nlist, NLIST_HEADER *newlist, DOUBLE bulk_shift)
#else
publicdef INT so_explode (nlist, newlist, bulk_shift)
NLIST_HEADER nlist;
NLIST_HEADER *newlist;
DOUBLE bulk_shift;
#endif
    {
    
    VOIDPTR Var_List[2];          /* nlist stuff */
    UINT Col_List[2];
    INT units[2];
    INT value_inc[2];
    UINT ndim = 2;
    INT data_types[2];
    size_t data_sizes[2];
    
    /* local nlist interpolation variables */
    
    UINT ntables, nrows, table_num, table, ngot;
    CDP cdp, lastcdp;
    INT i, status;
    DOUBLE m, b;
    FLOAT newtime;                  /* 4 byte float */
    FLOAT *times;
    DOUBLE lasttime, addcdp;
    CDP *cdps, newcdp;
    /* initialize new nlist */
    
    units[0] = units[1] = 0;
    value_inc[0] = value_inc[1] = VALUE_LIST;
    data_types[0] = FLOAT_TYPE;
    data_types[1] = FLOAT_TYPE;
    data_sizes[0] = sizeof(FLOAT);
    data_sizes[1] = sizeof(FLOAT);
    Col_List[0] = 1;
    Col_List[1] = 2;
    
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
    if (ntables == 0)
        {
        nl_free_nlist (*newlist);
        return status;
        }
    table_num = 0;
    
    for (table = 0; table < ntables; table++)
        {
        nl_inq_table_int (nlist, table + 1, NL_TABLE_NROWS, (INT *) & nrows);
        if (nrows == 0)
            {
            continue;
            }
        /* get uninterpolated rows */
        
        nl_set_current_row (nlist, 1, table + 1);
        times = (FLOAT *)am_allocate (AM_APPLICATION, nrows * sizeof(FLOAT));
        cdps = (CDP *)am_allocate (AM_APPLICATION, nrows * sizeof(CDP));
        Var_List[0] = (VOIDPTR)cdps;
        Var_List[1] = (VOIDPTR)times;
        nl_get_rows (nlist, nrows, ndim, Col_List, Var_List, &ngot);
        nl_start_table (*newlist, &table_num);
        Var_List[0] = (VOIDPTR) & addcdp;
        Var_List[1] = (VOIDPTR) & newtime;
        
        /* add first row */
        
        newcdp = *cdps;
        newtime = *times;
        lastcdp = newcdp;
        lasttime = newtime;
        addcdp = newcdp;
        nl_add_rows (*newlist, 1, ndim, Col_List, Var_List);
        
        for (i = 1; i < nrows; i++)
            {
            if (cdps[i] == lastcdp)
                {
                continue;
                }
            /* make new rows */
            
            m = (times[i] - lasttime) / (cdps[i] - lastcdp);
            b = lasttime - m * lastcdp;
            for (cdp = lastcdp + 1; cdp <= cdps[i]; cdp++)
                {
                newcdp = cdp;
                newtime = cdp * m + b + bulk_shift;
                addcdp = cdp;
                nl_add_rows (*newlist, 1, ndim, Col_List, Var_List);
                
                }
            /* end cdp for */
            lastcdp = cdps[i];
            lasttime = times[i];
            }
        /* end nrows for */
        
        am_free ((VOIDPTR)cdps);
        am_free ((VOIDPTR)times);
        
        }
    /* end ntables for */
    
    /* set current row to 1 */
    
    if (table_num != 0)
        {
        nl_set_current_row (*newlist, 1, 1);
        }
    else
        {
        nl_free_nlist (*newlist);
        status = FAIL;
        }
    return status;
    }
/* END:     */
