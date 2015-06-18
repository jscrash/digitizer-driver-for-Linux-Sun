/* DEC/CMS REPLACEMENT HISTORY, Element NL_FILL_PL_TAB.C */
/* *2    18-MAR-1991 18:09:59 JULIAN "(SPR 0) Lease Mapping Init Checkin" */
/* *1    18-MAR-1991 17:59:44 JULIAN "0" */
/* DEC/CMS REPLACEMENT HISTORY, Element NL_FILL_PL_TAB.C */
/* DEC/CMS REPLACEMENT HISTORY, Element NL_DRAW_TAB.C*/
/* *1    13-AUG-1990 17:05:09 SYSTEM "Finder 6.1"*/
/* DEC/CMS REPLACEMENT HISTORY, Element NL_DRAW_TAB.C*/
/* DEC/CMS REPLACEMENT HISTORY, Element NL_DRAW_TAB.C*/
/* *5    25-JUL-1990 16:45:42 VINCE "(SPR 5644) Fixed compile problems introduced with the new header"*/
/* *4    25-JUL-1990 14:58:28 VINCE "(SPR 5644) Header Standardization"*/
/* *3    18-MAY-1990 16:39:10 GILLESPIE "(SPR 1) Add check for updated table before using n_valid_rows"*/
/* *2     9-APR-1990 10:32:55 GILLESPIE "(SPR 1) Remove superflurous checks"*/
/* *1     5-MAR-1990 10:59:20 GILLESPIE "Rename files for Unix compliance (SPR 6015)"*/
/* DEC/CMS REPLACEMENT HISTORY, Element NL_DRAW_TAB.C*/
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
#include "esi_gks.h"
#include "esi_wm.h"
#include "esi_tc.h"

#define MAXPTS (INT)4000

/* Function Description -----------------------------------------------------
Description:
    nl_draw_table will draw two columns of the specified table in an
    N-list in the current GKS environment.  The two columns
    must be columns that can be converted ti real number(all except
    BOOL and CHAR).  This call will create table breaks at table bound-
    aries and null values. No external non-GKS clilpping will be done in
    this call.  If the given Table_number is equal to 0, the draw the
    current table.

Prototype:
    publicdef INT nl_fill_polygon_table (NLIST_HEADER nlist, UINT table_num, UINT independent, 
        UINT dependent);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    nlist           -(NLIST_HEADER) The given N-list.               
    table_num       -(UINT) The given table number.           
    independent     -(UINT) The independent axis column.
    dependent       -(UINT) The dependent axis column.

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.
    NL_INVALID_TABLE
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT nl_fill_polygon_table (NLIST_HEADER nlist, UINT table_num, UINT independent,
                             UINT dependent)
#else
publicdef INT nl_fill_polygon_table (nlist, table_num, independent, dependent)
NLIST_HEADER nlist;
UINT table_num;
UINT independent;
UINT dependent;
#endif
    {
    register INT i, j;              /*   indexes. */
    register UINT nrows, sub_nrows;
    UINT old_tab, old_row;          /*   workspace.          */
    INT istatus;                    /*   returned status of independent variable. */
    INT dstatus;                    /*   returned status of dependent variable. */
    COLUMN_HEADER *piinfo;          /*   (COLUMN_HEADER *)of independent variable. */
    COLUMN_HEADER *pdinfo;          /*   (COLUMN_HEADER *)of dependent variable. */
    TABLE_HEADER *ptable;           /*   current working table.                 */
    DATA_HEADER *pinull;            /*   (DATA_HEADER *)of independent variable. */
    DATA_HEADER *pdnull;            /*   (DATA_HEADER *)of dependent variable.   */
    FLOAT *pidata, *pddata;
    DOUBLE dnull,                   /* NULL values of independent variable.    */
        inull;                      /* NULL values of independent variable.    */
    FLOAT *x, *y, *xptr, *yptr;     /*   workspace.         */
    
    /* **************************************************************************** */
    /*   error checking.  */
    
    INIT_TEST (nlist);
    COLUMN_TEST (nlist, independent);
    COLUMN_TEST (nlist, dependent);
    EMPTY_TEST (nlist);
    
    /*   store the old current row.           */
    
    old_row = nlist->current_row;
    old_tab = nlist->current_table;
    table_num = (table_num EQUALS 0) ? old_tab : table_num;
    
    ptable = nlz_set_table (nlist, table_num);
    
    if (ptable EQUALS NULL_TABLE)
        {
        return NL_INVALID_TABLE;
        }
    /* don't draw short tables */
    
    if (ptable->nrows < 2)
        {
        return FAIL;
        }
    /*    set the NULL value */
    piinfo = nlist->info_ptr + independent - 1;
    pdinfo = nlist->info_ptr + dependent - 1;
    dnull = pdinfo->null_value;
    inull = piinfo->null_value;
    
    /*   set the DATA_HEADER pointers of variables */
    
    pinull = ptable->data_ptr + independent - 1;
    pdnull = ptable->data_ptr + dependent - 1;
    
    pidata = nlz_get_float_array (nlist, independent, &istatus);
    if (istatus < 0)
        {
        return istatus;
        }
    pddata = nlz_get_float_array (nlist, dependent, &dstatus);
    if (dstatus < 0)
        {
        return dstatus;
        }
    if (ptable->updated)
        {
        nlz_compute_table_stats (nlist, table_num);
        }
    /*   get the float value array for each dimen-
         sion and check for the NULL value.       */
    
    for (i = 0, x = pidata, y = pddata;
         i < ptable->nrows;
         i += MAXPTS - 1, x += MAXPTS - 1, y += MAXPTS - 1)
        {
        nrows = (i + MAXPTS < ptable->nrows) ? MAXPTS : ptable->nrows - i;
        
        /* check the NULL value rows */
        
        if ((ptable->nrows EQUALS pinull->n_valid_rows) &&
            (ptable->nrows EQUALS pdnull->n_valid_rows) &&
	    (nrows > 3))
            {
            /* neither of them has a NULL value */
            wm_gfa (nrows, x, y);
            }
        /* one of them has NULL value */
        else
            {
            for (j = 0, sub_nrows = 0; j < nrows; j++, sub_nrows++)
                {
                /* draw the previous segment of table.  */
                
                if (((inull < 0 && x[j] >= inull * 1.01 && x[j] <= inull * 0.99) ||
                     (inull > 0 && x[j] <= inull * 1.01 && x[j] >= inull * 0.99)) ||
                    ((dnull < 0 && y[j] >= dnull * 1.01 && y[j] <= dnull * 0.99) ||
                     (dnull > 0 && y[j] <= dnull * 1.01 && y[j] >= dnull * 0.99)))
                    {
                    if (sub_nrows > 3)  /* only draw more than two rows.  */
                        {
                        xptr = &x[j - sub_nrows];
                        yptr = &y[j - sub_nrows];
                        
                        wm_gfa (sub_nrows, xptr, yptr);
                        }
                    sub_nrows = -1;
                    }
                }
            if (sub_nrows > 3)      /* only draw more than two rows.  */
                {
                xptr = &x[j - sub_nrows];
                yptr = &y[j - sub_nrows];
                wm_gfa (sub_nrows, xptr, yptr);
                }
            }
        }
    /*
       * if either column was allocated, free it up
    */
    if (istatus EQUALS 1)
        {
        tc_free (pidata);
        }
    if (dstatus EQUALS 1)
        {
        tc_free (pddata);
        }
    nl_set_current_row (nlist, old_row, old_tab);
    
    return SUCCESS;
    }
/* END:     */
