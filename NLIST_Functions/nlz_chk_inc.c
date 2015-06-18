/* DEC/CMS REPLACEMENT HISTORY, Element NLZ_CHK_INC.C*/
/* *4    25-JUL-1990 14:51:31 VINCE "(SPR 5644) Header Standardization"*/
/* *3     5-MAR-1990 10:44:41 GILLESPIE "(SPR 6015) Change file names to be compliant with Unix"*/
/* *2    24-SEP-1989 22:26:42 GILLESPIE "(SPR 200) New n-List 4.0 - Table/Row mods"*/
/* *1    19-JUN-1989 13:16:14 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element NLZ_CHK_INC.C*/
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
    To check the specified N-list has same increment value in all tables.
    And all start_value in the table are at MOD increment.

Prototype:
    publicdef INT nlz_check_start_inc (NLIST_HEADER nlist, UINT nCol);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    nlist           -(NLIST_HEADER)
    nCol            -(UINT)

Return Value/Status:
    SUCCESS - Successful completion.
    NL_INVALID_NLIST1_INCREMENT
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT nlz_check_start_inc (NLIST_HEADER nlist, UINT nCol) 
#else
publicdef INT nlz_check_start_inc (nlist, nCol)
NLIST_HEADER nlist;
UINT nCol;
#endif
    {
    DATA_HEADER *pdata;
    DOUBLE increment, startValue;
    TABLE_HEADER *ptable;
    UINT table_num;
    INT pointGap = 0, firstStk = TRUE;
    
    for (table_num = 0; table_num < nlist->total_tables; table_num)
        {
        ptable = nlist->table_elements[table_num].ptable;
        if (ptable EQUALS NULL_TABLE)
            ptable = nlz_read_table_header (nlist, &nlist->table_elements[table_num]);
        pdata = ptable->data_ptr + nCol - 1;
        if (firstStk)
            {
            firstStk = FALSE;
            startValue = pdata->start_value;
            increment = pdata->increment;
            }
        if (increment IS_NOT_EQUAL_TO pdata->increment)
            return NL_INVALID_NLIST1_INCREMENT;
        pointGap = (pdata->start_value - startValue) / increment;
        if (pointGap * increment + startValue IS_NOT_EQUAL_TO pdata->start_value)
            return NL_INVALID_NLIST1_INCREMENT;
        }
    return SUCCESS;
    }
/* END:     */
