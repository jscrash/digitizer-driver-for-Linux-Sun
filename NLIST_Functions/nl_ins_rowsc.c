/* DEC/CMS REPLACEMENT HISTORY, Element NL_INS_ROWSC.C*/
/* *3    25-OCT-1990 12:23:06 GILLESPIE "(SPR 31) Lint run - correct casting inconsistencies"*/
/* *2    25-JUL-1990 14:59:53 VINCE "(SPR 5644) Header Standardization"*/
/* *1     5-MAR-1990 10:59:38 GILLESPIE "Rename files for Unix compliance (SPR 6015)"*/
/* DEC/CMS REPLACEMENT HISTORY, Element NL_INS_ROWSC.C*/
/* DEC/CMS REPLACEMENT HISTORY, Element NL_INSPTS_CVT.C*/
/* *1    19-JUN-1989 13:19:01 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element NL_INSPTS_CVT.C*/
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

/* Function Description -----------------------------------------------------
Description:
    Insert one or more data rows to the current table in an n-List
    and perform any required data type conversion.

Prototype:
    publicdef INT nl_insert_rows_convert(NLIST_HEADER nlist, UINT nrows, UINT nCol, 
        UINT *Col_List, VOIDPTR *Var_List, INT *Data_Units, size_t *Data_Widths, 
        INT *Data_Types);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) nlist       -(NLIST_HEADER) The n-List header.
    (I) nrows       -(UINT) Number of rows to be added.
    (I) nCol        -(UINT) Number of columns referenced.
    (I) Col_List    -(UINT *) Array of column <nCol> index numbers.
    (I) Var_List    -(VOIDPTR *) Array of addresses of start of data for
                        each column.
    Data_Units      -(INT *) Saved for the future.         
    Data_Types      -(INT *) Array of data types represented by input
                        data (FLOAT_TYPE, INT_TYPE, CHAR_TYPE,
                        BOOL_TYPE) one per input column.
    Data_Widths     -(size_t *) Array of data widths represented by        
                        input data.

Return Value/Status:
    Function returns status from nlz_aid_rows_convert.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:
    Assumptions:
       1) The n-List has been initialized.
       2) A table has been initialized.
       3) The data rows will be added after the 'Current Point'
       4) 1 <= nCol <= total # of columns in the n-List
       4) Var_List coontains valid addresses of source data
       5) all entries in Col_List satisfy 1 <= Col_List[i] <= total # columns
       6) nrows > 0
       7) There is a correspondence between the ith position in each of the
            input arrays.  That is, the first value in each array describes
            the first column to be input, the second value in each array
            describes the second value, etc.
       8) Any column defined for the n-List but not referenced in Col_List
            that is not an indexed column will have a null value inserted
       9) The input data will be converted, if necessary, from the current
            representation defined by Data_Widths[i] and Data_Types[i] into
            its internal representation as defined in the n-List header

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT nl_insert_rows_convert (NLIST_HEADER nlist, UINT nrows, UINT nCol,
                                      UINT *Col_List, VOIDPTR *Var_List,
                                      INT *Data_Units, size_t *Data_Widths,
                                      INT *Data_Types)
#else
publicdef INT nl_insert_rows_convert (nlist, nrows, nCol, Col_List, Var_List,
                                      Data_Units, Data_Widths, Data_Types)
NLIST_HEADER nlist;
UINT nrows;
UINT nCol;
UINT *Col_List;
VOIDPTR *Var_List;
INT *Data_Units;
INT *Data_Types;
size_t *Data_Widths;
#endif
    {
    INT status;                     /*  retruned flag.                                    */
    
    /*  *************************************************************************  */
    status = nlz_aid_rows_convert (nlist, nrows, nCol, Col_List, Var_List, Data_Units,
                                   Data_Widths, Data_Types, (INT)1);
    return status;
    }
/* END:     */
