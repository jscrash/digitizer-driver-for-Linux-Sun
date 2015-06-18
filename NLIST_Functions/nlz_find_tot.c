/* DEC/CMS REPLACEMENT HISTORY, Element NLZ_FIND_TOT.C*/
/* *6    25-OCT-1990 12:18:14 GILLESPIE "(SPR 31) Lint run - correct casting inconsistencies"*/
/* *5    25-JUL-1990 14:52:22 VINCE "(SPR 5644) Header Standardization"*/
/* *4     9-APR-1990 10:31:43 GILLESPIE "(SPR 1) Remove superflurous checks"*/
/* *3     5-MAR-1990 10:44:49 GILLESPIE "(SPR 6015) Change file names to be compliant with Unix"*/
/* *2    24-SEP-1989 22:26:57 GILLESPIE "(SPR 200) New n-List 4.0 - Table/Row mods"*/
/* *1    19-JUN-1989 13:16:35 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element NLZ_FIND_TOT.C*/
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

#define NLZ_GET_CONVERT_VALUE(pinfo, bytePtr, value) \
switch((pinfo) -> data_type )\
    {\
    case INT_TYPE : \
     switch( (pinfo) -> data_size  )\
     {\
     case sizeof(INT):\
          value = *(INT *)(bytePtr);\
          break;\
     case sizeof(SHORT):\
          value = *(SHORT *)(bytePtr);\
          break;\
     case sizeof( CHAR):\
          value = *(CHAR *)(bytePtr);\
          break;\
     }\
     break;\
    case FLOAT_TYPE:\
     switch( (pinfo) -> data_size )\
     {\
     case sizeof(FLOAT):\
          value = *(FLOAT *)(bytePtr);\
          break;\
     case sizeof(DOUBLE):\
          value = *(DOUBLE *)(bytePtr);\
          break;\
     }\
     break;\
    case BOOL_TYPE:\
     value = *(BOOL *)(bytePtr);\
     break;\
    }
        

/* Function Description -----------------------------------------------------
Description:
    A help routine of NLZ_SPLINE_NLIST().  The function of this routine is
    that will calculate the start value and the total number of rows, which
    wiil be added to the new table.  The start value is belongs to the
    independent variable column only.

Prototype:
    publicdef INT nlz_find_start_and_total (NLIST_HEADER nlist, UINT table_num, UINT nCol,
        DOUBLE increment, DOUBLE *startValue, UINT *total_rows);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    nlist           -(NLIST_HEADER) The given N-list.                   
    table_num       -(UINT) The number of given table.    
    nCol            -(UINT) The indexed column number.    
    total_rows      -(UINT *) The result of function.     
    increment       -(DOUBLE) The increment value of new indexed nCol.
    startValue      -(DOUBLE *) The result of function.    

Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT nlz_find_start_and_total (NLIST_HEADER nlist, UINT table_num, UINT nCol,
                                        DOUBLE increment, DOUBLE *startValue,
                                        UINT *total_rows) 
#else
publicdef INT nlz_find_start_and_total (nlist, table_num, nCol, increment, startValue,
                                        total_rows)
NLIST_HEADER nlist;
UINT table_num;
UINT nCol;
UINT *total_rows;
DOUBLE increment;
DOUBLE *startValue;
#endif
    {
    INT times;                      /*   the multiple times of increment.        */
    INT sign;                       /*   the numeric sign of the value */
	UINT Col_List[1];
    INT status;                     /*   returned status.                        */
    VOIDPTR Var_List[1];            /*   workspace.                              */
    BYTE *bytes;                    /*   workspace.                              */
    COLUMN_HEADER *pinfo;           /*   workspace for current COLUMN_HEADER. */
    /*--------------------------------------*/
    /* maxmum and minmum value of specified */
    /* table and column.                    */
    /*--------------------------------------*/
    DOUBLE maxValue, minValue;
    
    /* ***************************************************************************** */
    /*   inquire the maxmum and minmum value and */
    /*   convert it to be DOUBLE type.           */
    
    Var_List[0] = (VOIDPTR)(bytes = (BYTE *)TALLOC (sizeof(BYTE) * 10));
	Col_List[0] = nCol;
    pinfo = nlist->info_ptr + nCol - 1;
    status = nl_inq_table_int (nlist, table_num, NL_TABLE_NROWS, (INT *)total_rows);
    if (status < 0)
        {
        return status;
        }
    if (*total_rows EQUALS 0)
        {
        return NL_EMPTY_NLIST;
        }
    status = nl_inq_table_info (nlist, table_num, NL_TABLE_MAX, (UINT)1, Col_List, Var_List);
    if (status < 0)
        {
        return status;
        }
    NLZ_GET_CONVERT_VALUE (pinfo, bytes, maxValue);
    
    status = nl_inq_table_info (nlist, table_num, NL_TABLE_MIN, (UINT)1, Col_List, Var_List);
    if (status < 0)
        {
        return status;
        }
    NLZ_GET_CONVERT_VALUE (pinfo, bytes, minValue);
    
    /*   find first multiple increment between   */
    /*   maxmum and minmum value.                */
    times = minValue / increment;
    sign = (minValue >= 0) ? 1 : 0;
    *startValue = (ABS (times * increment) >= ABS (minValue)) ?
                  minValue : (times + sign) * increment;
    
    /*   calculate the total rows in the range. */
    *total_rows = (maxValue - *startValue) / increment + 1;
#if NEED_TO_FREE EQUALS TRUE
    tc_free (Var_List[0]);
#endif
    return SUCCESS;
    }
/* END:     */
