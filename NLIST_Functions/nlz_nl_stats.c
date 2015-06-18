/* DEC/CMS REPLACEMENT HISTORY, Element NLZ_NL_STATS.C*/
/* *7    25-OCT-1990 12:19:37 GILLESPIE "(SPR 31) Lint run - correct casting inconsistencies"*/
/* *6    25-JUL-1990 14:53:59 VINCE "(SPR 5644) Header Standardization"*/
/* *5    22-MAR-1990 01:34:39 GILLESPIE "(SPR 1) Correct problems with stats on delete"*/
/* *4    12-MAR-1990 19:04:59 GILLESPIE "(SPR 6016) More error checking on mallocs; re-engineer stats calcs"*/
/* *3     5-MAR-1990 10:45:24 GILLESPIE "(SPR 6015) Change file names to be compliant with Unix"*/
/* *2    24-SEP-1989 22:27:24 GILLESPIE "(SPR 200) New n-List 4.0 - Table/Row mods"*/
/* *1    19-JUN-1989 13:17:03 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element NLZ_NL_STATS.C*/
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

#include "esi_nl_int.h"
#include "esi_tc.h"
#include "esi_math.h"

/* Function Description -----------------------------------------------------
Description:
    Scan the nlist and reset the statistics for the data.

Prototype:
    publicdef INT nlz_compute_nlist_stats (NLIST_HEADER nlist);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    nlist           -(NLIST_HEADER)

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT nlz_compute_nlist_stats (NLIST_HEADER nlist) 
#else
publicdef INT nlz_compute_nlist_stats (nlist)
NLIST_HEADER nlist;
#endif
    {
    register TABLE_HEADER *ptable;
    DATA_HEADER *pdata;
    register COLUMN_HEADER *pinfo;
    register UINT i, j;
    DOUBLE x;
    
    /*
       * reset all of the column stats
    */
    if (nlist->defer_stats OR !nlist->updated)
        {
        return SUCCESS;
        }
    for (i = 0, pinfo = nlist->info_ptr; i < nlist->nCol; i++, pinfo++)
        {
        pinfo->min = HUGE_VAL;
        pinfo->max = -HUGE_VAL;
        pinfo->sum_x = pinfo->sum_x_squared = 0.0;
        pinfo->n_valid_rows = 0;
        }
    /*     scan all tables and do your thing        */
    for (i = 0, ptable = nlz_set_table (nlist, (UINT)1); ptable != NULL_TABLE;i++,
         ptable = nlz_set_table (nlist, i + 1))
        {
        if (ptable->nrows EQUALS 0)
            {
            continue;
            }
        /* recompute table stats, if necessary */
        if (ptable->updated)
            {
            if (ptable->data_loaded EQUALS NL_DATA_NOT_LOADED)
                {
                INT status;
                if ((status = nlz_load_data (nlist))IS_NOT_EQUAL_TO SUCCESS)
                    {
                    return status;
                    }
                }
            nlz_compute_table_stats (nlist, i + 1);
            }
        for (j = 0, pdata = ptable->data_ptr, pinfo = nlist->info_ptr; j < nlist->nCol;
             j++, pdata++, pinfo++)
            {
            if (pdata->min == pinfo->null_value)
                {
                continue;
                }
            pinfo->sum_x += pdata->sum_x;
            pinfo->sum_x_squared += pdata->sum_x_squared;
            pinfo->n_valid_rows += pdata->n_valid_rows;
            x = pdata->min;
            if (x < pinfo->min)
                {
                pinfo->min = x;
                }
            x = pdata->max;
            if (x > pinfo->max)
                {
                pinfo->max = x;
                }
            }
        }
    nlist->updated = FALSE;
    return SUCCESS;
    }
/* END:     */
