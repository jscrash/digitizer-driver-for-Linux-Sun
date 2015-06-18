/* DEC/CMS REPLACEMENT HISTORY, Element NLZ_GD_ARR.C*/
/* *8    25-OCT-1990 12:18:24 GILLESPIE "(SPR 31) Lint run - correct casting inconsistencies"*/
/* *7    25-JUL-1990 14:52:35 VINCE "(SPR 5644) Header Standardization"*/
/* *6     9-APR-1990 10:31:53 GILLESPIE "(SPR 1) Remove superflurous checks"*/
/* *5    12-MAR-1990 19:03:59 GILLESPIE "(SPR 6016) More error checking on mallocs; re-engineer stats calcs"*/
/* *4     5-MAR-1990 10:44:59 GILLESPIE "(SPR 6015) Change file names to be compliant with Unix"*/
/* *3     6-DEC-1989 09:10:27 GORDON "(SPR 0) Fix spacing around OR"*/
/* *2    24-SEP-1989 22:27:01 GILLESPIE "(SPR 200) New n-List 4.0 - Table/Row mods"*/
/* *1    19-JUN-1989 13:16:42 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element NLZ_GD_ARR.C*/
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

/* Function Description -----------------------------------------------------
Description:
    Returns address of an array of values for current table.

Prototype:
    publicdef DOUBLE *nlz_get_double_array (NLIST_HEADER nlist, UINT nCol, INT *status);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    nlist           -(NLIST_HEADER)
    nCol            -(UINT)
    status          -(INT *)

Return Value/Status:
    Status returns SUCCESS or NL_OVERFLOW..
    Function returns the value of (DOUBLE *)(pdata->data).
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef DOUBLE *nlz_get_double_array (NLIST_HEADER nlist, UINT nCol, INT *status) 
#else
publicdef DOUBLE *nlz_get_double_array (nlist, nCol, status)
NLIST_HEADER nlist;
UINT nCol;
INT *status;
#endif
    {
    register COLUMN_HEADER *pinfo;
    UINT i;
    DATA_HEADER *pdata;
    register TABLE_HEADER *ptable = nlist->current_table_ptr;
    DOUBLE *pfirst;
    register DOUBLE *pdouble;
    
    /*
       * if the table has not been loaded, then loads first.
    */
    if (ptable->data_loaded EQUALS NL_DATA_NOT_LOADED AND ptable->nrows)
        {
        *status = nlz_load_data (nlist);
        if (*status IS_NOT_EQUAL_TO SUCCESS)
            {
            return(DOUBLE *)0;
            }
        ptable->data_loaded = NL_DATA_LOADED;
        }
    pinfo = nlist->info_ptr + nCol - 1;
    pdata = ptable->data_ptr + nCol - 1;
    
    /*
       * gotta make a new array if data type isn't right
    */
    if (pinfo->data_type IS_NOT_EQUAL_TO FLOAT_TYPE OR pinfo->
        data_size IS_NOT_EQUAL_TO sizeof(DOUBLE) OR pdata->inc_value EQUALS TRUE)
        {
        *status = 1;
        pdouble = pfirst = (DOUBLE *)tc_alloc (ptable->nrows * sizeof(DOUBLE));
        if (pdouble == (DOUBLE *)0)
            {
            *status = NL_OVERFLOW;
            return(DOUBLE *)0;
            }
        for (i = 1; i <= ptable->nrows; i++, pdouble++)
            {
            nlz_get_value (pdata, pinfo, i, pdouble);
            }
        return pfirst;
        }
    else
        {
        *status = SUCCESS;
        return(DOUBLE *)(pdata->data);
        }
    }
/* END:     */
