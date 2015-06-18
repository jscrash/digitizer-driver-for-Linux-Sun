/* DEC/CMS REPLACEMENT HISTORY, Element NL_REP_ROWSC.C*/
/* *2    25-JUL-1990 15:05:12 VINCE "(SPR 5644) Header Standardization"*/
/* *1     5-MAR-1990 11:01:00 GILLESPIE "Rename files for Unix compliance (SPR 6015)"*/
/* DEC/CMS REPLACEMENT HISTORY, Element NL_REP_ROWSC.C*/
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
    Replace 1 or more rows in an N-list beginning with the current row,
    convert the data type and unit before insert.

Prototype:
    publicdef INT nl_replace_rows_convert(NLIST_HEADER nlist, UINT nrows, UINT nCol, 
        UINT *Col_List, VOIDPTR *Var_List, INT *dataUnits,  size_t *dataWidths, 
        INT *dataTypes);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    nlist           -(NLIST_HEADER) Pointer of given NLIST_HEADER.    
    nCol            -(UINT) The number of requested columns.       
    Col_List        -(UINT *) List of requested columns.       
    nrows           -(UINT)
    Var_List        -(VOIDPTR *) List of pointers of returned values.    
    dataUnits       -(INT *) List of converting measure units.     
    dataWidths      -(size_t *) List of converting data sizes.  
    dataTypes       -(INT *) List of converting data type.    

Return Value/Status:
    Function returns status from nlz_aid_rows_convert.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT nl_replace_rows_convert (NLIST_HEADER nlist, UINT nrows, UINT nCol,
                                       UINT *Col_List, VOIDPTR *Var_List,
                                       INT *dataUnits, size_t *dataWidths,
                                       INT *dataTypes)
#else
publicdef INT nl_replace_rows_convert (nlist, nrows, nCol, Col_List, Var_List,
                                       dataUnits, dataWidths, dataTypes)
NLIST_HEADER nlist;
UINT nCol;
UINT *Col_List;
UINT nrows;
VOIDPTR *Var_List;
INT *dataUnits;
size_t *dataWidths;
INT *dataTypes;
#endif
    {
    
    return nlz_aid_rows_convert (nlist, nrows, nCol, Col_List, Var_List, dataUnits,
                                 dataWidths, dataTypes, (INT)2);
    }
/* END:     */
