/* DEC/CMS REPLACEMENT HISTORY, Element NL_DEL_ROWS.C */
/* *11   25-OCT-1990 12:22:07 GILLESPIE "(SPR 31) Lint run - correct casting inconsistencies" */
/* *10   25-JUL-1990 14:57:54 VINCE "(SPR 5644) Header Standardization" */
/* *9     7-MAY-1990 20:19:17 CHARLIE "(SPR -1) once more with feeling, fix current row" */
/* *8     3-MAY-1990 09:34:28 JULIAN "(SPR 5276) fix nl statistics on ptable nrows" */
/* *7    10-APR-1990 11:57:58 GILLESPIE "(SPR 1) Previous delta got dropped - put it back in" */
/* *6     9-APR-1990 10:32:45 GILLESPIE "(SPR 1) Remove superflurous checks" */
/* *5     1-APR-1990 12:19:46 GILLESPIE "(SPR 5176) Fix setting of current row on interior deletes" */
/* *4    22-MAR-1990 01:35:06 GILLESPIE "(SPR 1) Correct problems with stats on delete" */
/* *3    12-MAR-1990 19:07:00 GILLESPIE "(SPR 6016) More error checking on mallocs; re-engineer stats calcs" */
/* *2     5-MAR-1990 10:46:03 GILLESPIE "(SPR 6015) Change file names to be compliant with Unix" */
/* *1    24-SEP-1989 22:34:46 GILLESPIE "New n-List V4.0" */
/* DEC/CMS REPLACEMENT HISTORY, Element NL_DEL_ROWS.C */
/******************************************************************************

                Copyright Finder Graphics Systems, Inc. 1990
                    Unpublished -- All rights reserved

THIS SOFTWARE IS THE PROPRIETARY PROPERTY OF Exploration Systems, Inc.  AND MAY
CONTAIN CONFIDENTIAL TRADE SECRET INFORMATION. IT IS LICENSED FOR USE ON THE
DESIGNATED EQUIPMENT ON WHICH IT WAS ORIGINALLY INSTALLED AND  MAY NOT BE
MODIFIED, DUPLICATED OR COPIED IN ANY FORM WITHOUT PRIOR WRITTEN CONSENT OF

                            Finder Graphics Systems
                            201 Tamal Vista Blvd.
                            Corte Madera, CA  94925
                            (415) 927-0100

(The above notice does not constitute an admission of publication or
unrestricted dissemination of the work)

******************************************************************************/

#include "esi_nl_int.h"
#include "esi_ho.h"
#include "esi_tc.h"

/* Function Description -----------------------------------------------------
Description:
    Delete one or more rows.
    If the current row number is not equal to the last row in the
    pointlist, then the current row number does not change
    if it is equal to the last row, it is decremented by one.
    if this is the last row in the table, then the cp is set to
    zero.  It is not possible to delete rows across table boundaries
    using this routine

Prototype:
    publicdef INT nl_delete_rows (NLIST_HEADER nlist, UINT nrows);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    nlist           -(NLIST_HEADER) Pointer of given NLIST_HEADER.
    nrows           -(UINT) The number of rows requested to be deleted.

Return Value/Status:
    SUCCESS - Successful completion.
    NL_EMPTY_TABLE
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT nl_delete_rows (NLIST_HEADER nlist, UINT nrows)
#else
publicdef INT nl_delete_rows (nlist, nrows)
NLIST_HEADER nlist;
UINT nrows;
#endif
    {
    DOUBLE x;
    UINT current_row;               /* workspace of current row.               */
    /*---------------------------------*/
    /* workspace of the data header of */
    /* specifeid column.               */
    /*---------------------------------*/
    register DATA_HEADER *pdata;
    
    /*-----------------------------------*/
    /* workspace of the column header of */
    /* specified column.                 */
    /*-----------------------------------*/
    register COLUMN_HEADER *pinfo;
    
    TABLE_HEADER *ptable;           /* workspace of the current table.         */
    UINT i, j;
    size_t data_size;
    UINT checkPoints;
    INT status;
    
    /*     ************************************************************************** */
    /*   error checking.                         */
    
    INIT_TEST (nlist);
    EMPTY_TEST (nlist);
    
    /* the specified nrows is equal to 0 */
    
    NROWS_TEST (nrows);
    
    ptable = nlz_set_table (nlist, nlist->current_table);
    if (ptable->nrows EQUALS 0)
        {
        return NL_EMPTY_TABLE;
        }
    current_row = nlist->current_row;
    
    /*
     * if the table has not been loaded, then loads first.
     */
    if (ptable->data_loaded EQUALS NL_DATA_NOT_LOADED AND ptable->nrows)
        {
        status = nlz_load_data (nlist);
        if (status IS_NOT_EQUAL_TO SUCCESS)
            {
            return status;
            }
        }
    /*
     * recalculate statistics for this table
     */
    
    checkPoints = ((current_row + nrows - 1) <= ptable->nrows) ?
        nrows : (ptable->nrows - current_row + 1);
    
    if (nlist->defer_stats)
        {
        ptable->updated = TRUE;
        }
    else
        {
        /* Recompute statistics on the rows to be deleted */
        
        for (i = 0, pinfo = nlist->info_ptr, pdata = ptable->data_ptr; i < nlist->nCol;
             i++, pinfo++, pdata++)
            {
            for (j = 0; j < checkPoints; j++)
                {
                if (pinfo->inc_value)
                    {
                    /* Incrementing dimensions present special problems, since the actual data
                    is virtual rather than actual.  Deleting an internal incrementing dimension
                    removes values from the end of the list - it does not create gaps */
                    ptable->updated = TRUE;
                    goto breakout;
                    }
                if ((nlz_get_value (pdata, pinfo, nlist->current_row + j,
                                    &x) < 0) OR (x EQUALS pinfo->null_value))
                    {
                    continue;
                    }
                /* If the deleted value matches the min or max value for the column, then
                a complete pass through the data will be necessary to establish the new
                min/max value, and continuing on here is just a waste of CPU cycles... */
                if (x == pdata->min || x == pdata->max)
                    {
                    ptable->updated = TRUE;
                    goto breakout;
                    }
                pdata->n_valid_rows--;
                pinfo->n_valid_rows--;
                pdata->sum_x -= x;
                pinfo->sum_x -= x;
                x *= x;
                pdata->sum_x_squared -= x;
                pinfo->sum_x_squared -= x;
                }
            }
        }
    /* See if this is an interior delete, requiring data to be shifted */
breakout:
    if ((current_row + checkPoints) <= ptable->nrows)
        {
        for (i = 0, pinfo = nlist->info_ptr, pdata = ptable->data_ptr; i < nlist->nCol;
             i++, pinfo++, pdata++)
            {
            if (pdata->inc_value EQUALS FALSE)
                {
                data_size = pinfo->data_size;
                hoblockmove ((VOIDPTR)(pdata->data + (current_row + checkPoints - 1) *
                                       data_size),
                             (VOIDPTR)(pdata->data + (current_row - 1) * data_size),
                             (ptable->nrows - current_row - checkPoints + 1) *
                             data_size);
                }
            }
        }
    else
        {
        /* if it was an interior delete, we left it alone (there was something
            left to be pointed to). However, if we deleted the remainder of the
            list we simply reset the current_row pointer. (CP) */
        
        nlist->current_row = ptable->nrows - checkPoints;
        }
    nlist->total_rows -= checkPoints;
    ptable->nrows -= checkPoints;
    ptable->data_loaded = NL_DATA_UPDATED;
    nlist->updated = ptable->updated;
    
    return SUCCESS;
    }
/* END:     */
