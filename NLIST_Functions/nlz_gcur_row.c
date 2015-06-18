/* DEC/CMS REPLACEMENT HISTORY, Element NLZ_GCUR_ROW.C */
/* *6    25-OCT-1990 12:18:19 GILLESPIE "(SPR 31) Lint run - correct casting inconsistencies" */
/* *5    25-JUL-1990 14:52:28 VINCE "(SPR 5644) Header Standardization" */
/* *4     9-APR-1990 10:31:49 GILLESPIE "(SPR 1) Remove superflurous checks" */
/* *3    12-MAR-1990 19:03:49 GILLESPIE "(SPR 6016) More error checking on mallocs; re-engineer stats calcs" */
/* *2     5-MAR-1990 10:44:54 GILLESPIE "(SPR 6015) Change file names to be compliant with Unix" */
/* *1    24-SEP-1989 22:33:46 GILLESPIE "New n-List V4.0" */
/* DEC/CMS REPLACEMENT HISTORY, Element NLZ_GCUR_ROW.C */
/* DEC/CMS REPLACEMENT HISTORY, Element NLZ_GCUR_PT.C*/
/* *1    19-JUN-1989 13:16:39 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element NLZ_GCUR_PT.C*/
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
#include "esi_ho.h"

/* Function Description -----------------------------------------------------
Description:
    Retrieve the value of current row.

Prototype:
    publicdef INT nlz_get_current_row (NLIST_HEADER nlist, UINT nCol, UINT *Col_List, 
        VOIDPTR *Var_List);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    nlist           -(NLIST_HEADER) Pointer of given NLIST_HEADER.    
    nCol            -(UINT) Number of requested column.       
    Col_List        -(UINT *) List of requested column.          
    Var_List        -(VOIDPTR *) List of pointers of returned value.  

Return Value/Status:
    NL_INVALID_DATA_TYPE
    NL_INVALID_DATA_SIZE
    The function returns the status of nlz_inq_row_flag.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT nlz_get_current_row (NLIST_HEADER nlist, UINT nCol, UINT *Col_List,
                                   VOIDPTR *Var_List) 
#else
publicdef INT nlz_get_current_row (nlist, nCol, Col_List, Var_List)
NLIST_HEADER nlist;
UINT nCol;
UINT *Col_List;
VOIDPTR *Var_List;
#endif
    {
    register COLUMN_HEADER *pinfo;
    DATA_HEADER *pdata;
    UINT offset;                    /*      byte offset of data.                */
    register UINT i;                /*      index.                    */
    INT status;                     /*      returned flag.               */
    register BYTE *pbyte;           /*      byte pointer rows to data.            */
    DOUBLE x;
    
    /*  ************************************************************************   */
    
    offset = nlist->current_row - 1;
    
    /*      if the current table has not been        */
    /*      loaded, then loads first.            */
    
    if (nlist->current_table_ptr->data_loaded EQUALS NL_DATA_NOT_LOADED AND nlist->
        current_table_ptr->nrows)
        {
        if ((status = nlz_load_data (nlist))IS_NOT_EQUAL_TO SUCCESS)
            {
            return status;
            }
        nlist->current_table_ptr->data_loaded = NL_DATA_LOADED;
        }
    /*   error checking.                         */
    for (i = 0; i < nCol; i++)
        {
        pinfo = nlist->info_ptr + Col_List[i] - 1;
        pdata = nlist->current_table_ptr->data_ptr + Col_List[i] - 1;
        pbyte = *(BYTE **)(Var_List + i);
        if (pdata->inc_value)
            {
            
            /*   cast the calculated data value     */
            /*   into the appropriate data type     */
            
            x = pdata->start_value + offset * pdata->increment;
            switch (pinfo->data_type)
                {
            case INT_TYPE:
                switch (pinfo->data_size)
                    {
                case sizeof(CHAR):
                    *pbyte = x + NL_ROUND_ERROR;
                    break;
                case sizeof(SHORT):
                    *(SHORT *)pbyte = x + NL_ROUND_ERROR;
                    break;
                case sizeof(INT):
                    *(INT *)pbyte = x + NL_ROUND_ERROR;
                    break;
                default:
                    return NL_INVALID_DATA_SIZE;
                    }
                break;
            case FLOAT_TYPE:
                switch (pinfo->data_size)
                    {
                case sizeof(FLOAT):
                    *(FLOAT *)pbyte = (FLOAT)x;
                    break;
                case sizeof(DOUBLE):
                    *(DOUBLE *)pbyte = x;
                    break;
                default:
                    return NL_INVALID_DATA_SIZE;
                    }
                break;
            default:
                /*    Only numeric values are candidates for an   */
                /*    incrementing column                */
                return NL_INVALID_DATA_TYPE;
                }
            }
        else
            {
            /*      do this for non-incrementing columns   */
            
            hoblockmove ((VOIDPTR)(pdata->data + offset * pinfo->data_size),
                         (VOIDPTR)pbyte, pinfo->data_size);
            }
        }
    return nlz_inq_row_flag (nlist);
    }
/* END:     */
