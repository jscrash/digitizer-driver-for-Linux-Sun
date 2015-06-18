/* DEC/CMS REPLACEMENT HISTORY, Element NL_SNL_INFO.C */
/* *7    25-OCT-1990 12:26:46 GILLESPIE "(SPR 31) Lint run - correct casting inconsistencies" */
/*  5B1   3-OCT-1990 10:58:23 GILLESPIE "Change file type to stream" */
/* *6    20-AUG-1990 17:10:46 KEE "(SPR -1) fixing compile error at sun" */
/* *5    25-JUL-1990 15:05:34 VINCE "(SPR 5644) Header Standardization" */
/* *4    24-MAR-1990 11:09:57 GILLESPIE "(SPR 1) Check var_list for null pointers" */
/* *3    12-MAR-1990 19:09:38 GILLESPIE "(SPR 6016) More error checking on mallocs; re-engineer stats calcs" */
/* *2     5-MAR-1990 10:50:34 GILLESPIE "(SPR 6015) Change file names to be compliant with Unix" */
/* *1    24-SEP-1989 22:36:34 GILLESPIE "New n-List V4.0" */
/* DEC/CMS REPLACEMENT HISTORY, Element NL_SNL_INFO.C */
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
#include "esi_c_lib.h"

/* Function Description -----------------------------------------------------
Description:
    Set the infomation for the specified column(s).

Prototype:
    publicdef INT nl_set_nlist_info(NLIST_HEADER nlist, INT opcode, UINT nCol, 
        UINT *Col_List, VOIDPTR *Var_List);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    nlist           -(NLIST_HEADER)
    nCol            -(UINT)
    opcode          -(INT)
    Col_List        -(UINT *)
    Var_List        -(VOIDPTR *)

Return Value/Status:
    SUCCESS - Successful completion.
    NL_INVALID_POINTER
    NL_INVALID_CODE
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT nl_set_nlist_info (NLIST_HEADER nlist, INT opcode, UINT nCol,
                                 UINT *Col_List, VOIDPTR *Var_List)
#else
publicdef INT nl_set_nlist_info (nlist, opcode, nCol, Col_List, Var_List)
NLIST_HEADER nlist;
UINT nCol;
INT opcode;
UINT *Col_List;
VOIDPTR *Var_List;
#endif
    {
    UINT i;
    INT status = SUCCESS;
    COLUMN_HEADER *pinfo;
    UINT current_table, current_row;
    BOOL defer_stats;
    TABLE_HEADER *ptable;
    
    INIT_TEST (nlist);
    
    for (i = 0; i < nCol; i++)
        {
        COLUMN_TEST (nlist, Col_List[i]);
        if (Var_List[i] == (VOIDPTR)0)
            {
            return NL_INVALID_POINTER;
            }
        }
    /*
       * issue a warning if the null value is changed part way through
    */
    
    for (i = 0; i < nCol; i++)
        {
        pinfo = nlist->info_ptr + Col_List[i] - 1;
        switch (opcode)
            {
        case NL_NLIST_COLUMN_NAME:
            strcpy ((char *)pinfo->column_name, (char *)Var_List[i]);
            break;
        case NL_NLIST_NULL_VALUE:
            switch (pinfo->data_type)
                {
            case INT_TYPE:
                switch (pinfo->data_size)
                    {
                case sizeof(CHAR):
                    pinfo->null_value = (DOUBLE)*(CHAR *)Var_List[i];
                    break;
                case sizeof(SHORT):
                    pinfo->null_value = (DOUBLE)*(SHORT *)Var_List[i];
                    break;
                case sizeof(INT):
                    pinfo->null_value = (DOUBLE)*(INT *)Var_List[i];
                    break;
                    }
                break;
            case FLOAT_TYPE:
                switch (pinfo->data_size)
                    {
                case sizeof(FLOAT):
                    pinfo->null_value = (DOUBLE)*(FLOAT *)Var_List[i];
                    break;
                case sizeof(DOUBLE):
                    pinfo->null_value = *(DOUBLE *)Var_List[i];
                    break;
                    }
                break;
                }
            break;
        case NL_NLIST_NUMERIC_FORMAT:
            strcpy ((char *)pinfo->convert_format, (char *)Var_List[i]);
            break;
        case NL_NLIST_FIND_TOLERANCE:
            switch (pinfo->data_type)
                {
            case INT_TYPE:
                switch (pinfo->data_size)
                    {
                case sizeof(CHAR):
                    pinfo->tolerance = (DOUBLE)*(CHAR *)Var_List[i];
                    break;
                case sizeof(SHORT):
                    pinfo->tolerance = (DOUBLE)*(SHORT *)Var_List[i];
                    break;
                case sizeof(INT):
                    pinfo->tolerance = (DOUBLE)*(INT *)Var_List[i];
                    break;
                    }
                break;
            case FLOAT_TYPE:
                switch (pinfo->data_size)
                    {
                case sizeof(FLOAT):
                    pinfo->tolerance = (DOUBLE)*(FLOAT *)Var_List[i];
                    break;
                case sizeof(DOUBLE):
                    pinfo->tolerance = *(DOUBLE *)Var_List[i];
                    break;
                    }
                break;
                }
            break;
            
        default:
            status = NL_INVALID_CODE;
            }
        }
    /* if the opcode is NL_NLIST_NULL_VALUE, we need to calculate statistic data.*/
    if (opcode EQUALS NL_NLIST_NULL_VALUE)
        {
        if(nlist -> total_tables EQUALS 0)
	        return status;
        current_table = nlist->current_table;
        current_row = nlist->current_row;
        for (i = 0, ptable = nlz_set_table (nlist, (UINT)1); ptable != NULL_TABLE;
             i++, ptable = nlz_set_table (nlist, i + 1))
            {
            ptable->updated = TRUE;
            }
        nlist->updated = TRUE;
        defer_stats = nlist->defer_stats;
        nlist->defer_stats = FALSE;
        nlz_compute_nlist_stats (nlist);
        nlist->defer_stats = defer_stats;
        nl_set_current_row (nlist, current_row, current_table);
        }
    return status;
    }
/* END:     */
