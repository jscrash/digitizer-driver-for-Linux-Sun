/* DEC/CMS REPLACEMENT HISTORY, Element NL_OP_NL2.C*/
/* *3    25-OCT-1990 12:24:29 GILLESPIE "(SPR 31) Lint run - correct casting inconsistencies"*/
/* *2    25-JUL-1990 15:01:38 VINCE "(SPR 5644) Header Standardization"*/
/* *1     5-MAR-1990 10:59:51 GILLESPIE "Rename files for Unix compliance (SPR 6015)"*/
/* DEC/CMS REPLACEMENT HISTORY, Element NL_OP_NL2.C*/
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
#include "esi_math.h"

/* Function Description -----------------------------------------------------
Description:
    Perform an unary arithmetric operation on on n_list. 

Prototype:
    publicdef INT nl_operate_nlist_2(NLIST_HEADER nlist1, UINT nCol, UINT dimList1[], 
        INT opcode, NLIST_HEADER nlist2, UINT dimList2[], NLIST_HEADER maskList, 
        UINT maskDim[], NLIST_HEADER *outList);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    nlist1          -(NLIST_HEADER)
    nlist2          -(NLIST_HEADER)
    maskList        -(NLIST_HEADER)
    outList         -(NLIST_HEADER *)
    nCol            -(UINT)
    dimList1        -(UINT [])
    dimList2        -(UINT [])
    maskDim         -(UINT [])
    opcode          -(INT)

Return Value/Status:
    SUCCESS - Successful completion.
    NL_INVALID_CODE
    NL_INVALID_TABLE
    NL_TWO_LISTS_NOT_SAME_INCREMENT
    NL_INVALID_DATA_TYPE
    NL_INVALID_NLIST2_INCREMENT
    NL_INVALID_NLIST1_INCREMENT
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT nl_operate_nlist_2 (NLIST_HEADER nlist1, UINT nCol, UINT dimList1[],
                                  INT opcode, NLIST_HEADER nlist2, UINT dimList2[],
                                  NLIST_HEADER maskList, UINT maskDim[],
                                  NLIST_HEADER *outList)
#else
publicdef INT nl_operate_nlist_2 (nlist1, nCol, dimList1, opcode, nlist2, dimList2,
                                  maskList, maskDim, outList)
NLIST_HEADER nlist1, nlist2, maskList, *outList;
UINT nCol, dimList1[], dimList2[], maskDim[];
INT opcode;
#endif
    {
    NLIST_HEADER tempList1, tempList2;
    DATA_HEADER *pdata1, *pdata2, *pdataMask;
    COLUMN_HEADER *pinfo1, *pinfo2;
    TABLE_HEADER *pstk1;
    UINT i, j, k;
    BOOL noMask = FALSE, defer_stats;
    INT status = SUCCESS;
    UINT twoDim2[2];
    INT int_newValue, int_value1, int_value2;
    DOUBLE increment, value1, value2, newValue, startValue;
    
    noMask = (maskList EQUALS (NLIST_HEADER)NULL) ? TRUE : FALSE;
    /*
       * error checking.
    */
    if (nlz_check_start_inc (nlist1, dimList1[0]))
        {
        return NL_INVALID_NLIST1_INCREMENT;
        }
    if (nlz_check_start_inc (nlist2, dimList2[0]))
        {
        return NL_INVALID_NLIST2_INCREMENT;
        }
    nlz_set_table (nlist1, (UINT)1);
    nlz_set_table (nlist2, (UINT)1);
    pdata1 = nlist1->current_table_ptr->data_ptr + dimList1[0] - 1;
    pdata2 = nlist2->current_table_ptr->data_ptr + dimList2[0] - 1;
    if (pdata1->increment IS_NOT_EQUAL_TO pdata2->increment)
        {
        return NL_TWO_LISTS_NOT_SAME_INCREMENT;
        }
    increment = pdata1->increment;
    
    if (!noMask)
        {
        if (nlz_check_start_inc (maskList, maskDim[0]))
            {
            return NL_INVALID_NLIST2_INCREMENT;
            }
        pdataMask = maskList->current_table_ptr->data_ptr;
        if (pdataMask->increment IS_NOT_EQUAL_TO increment)
            {
            return NL_TWO_LISTS_NOT_SAME_INCREMENT;
            }
        }
    /*
       * If there is a mask N-list, operates each N-list with maskList individualy,
       * then do the real operation between two N-list.
    */
    if (!noMask)
        {
        nl_operate_nlist_1 (nlist1, nCol, dimList1, NL_IDENTICAL, maskList, maskDim,
                            &tempList1);
        nl_operate_nlist_1 (nlist2, nCol, dimList2, NL_IDENTICAL, maskList, maskDim,
                            &tempList2);
        }
    else
        {
        tempList1 = nlist1;
        tempList2 = nlist2;
        }
    /*
       * make a copy of N-list, and set it to be the returned N-list.
    */
    nl_copy_nlist (tempList1, outList);
    
    /*
        * If the opcode is one of NL_CLIP_MIN, NL_CLIP_MAX, NL_CRUSH_MIN,
        * NL_CRUSH_MAX, call nlz_compute_nlist_stats() first.
    */
    
    if (
  opcode EQUALS NL_CLIP_MIN OR opcode EQUALS NL_CLIP_MAX OR opcode EQUALS NL_CRUSH_MIN 
        OR opcode EQUALS NL_CRUSH_MAX)
        {
        defer_stats = tempList2->defer_stats;
        tempList2->defer_stats = FALSE;
        nlz_compute_nlist_stats (tempList2);
        tempList2->defer_stats = defer_stats;
        }
    /*
       * work on each column.
    */
    for (i = 0; i < nCol - 1; i++)
        {
        twoDim2[0] = dimList2[0];
        twoDim2[1] = dimList2[i + 1];
        pinfo1 = (*outList)->info_ptr + dimList1[i + 1] - 1;
        pinfo2 = (*outList)->info_ptr + dimList2[i + 1] - 1;
        for (j = 0; j < tempList1->total_tables; j++)
            {
            pstk1 = nlz_set_table (*outList, j + 1);
            if (pstk1 EQUALS NULL_TABLE)
                {
                return NL_INVALID_TABLE;
                }
            pdata1 = pstk1->data_ptr + dimList1[0] - 1;
            startValue = pdata1->start_value;
            pdata1 = pstk1->data_ptr + dimList1[i + 1] - 1;
            for (k = 0; k < pstk1->nrows; k++)
                {
                nl_set_current_row (*outList, k + 1, (*outList)->current_table);
                nlz_get_value (pdata1, pinfo1, k + 1, &value1);
                
                /*
                   * if the row in the N-list1 is NULL value, then skips it.
                */
                if (value1 EQUALS pinfo1->null_value)
                    continue;
                
                /*
                   * if there is no rows associated with the index value in N-list2, replace
                   * the respective value in N-list1 with NULL value.
                */
                if (nlz_get_index_value (tempList2, startValue + increment * k,
                                         twoDim2, (VOIDPTR)&value2, TRUE))
                    {
                    newValue = pinfo1->null_value;
                    nlz_replace_value (pdata1, pinfo1, k + 1, &newValue);
                    continue;
                    }
                /*
                   * if the value of rows in the N-list2 are NULL value, then  replaces the
                   * respective rows in the N-list1 with NULL value.
                */
                if (value2 EQUALS pinfo2->null_value)
                    {
                    newValue = pinfo1->null_value;
                    nlz_replace_value (pdata1, pinfo1, k + 1, &newValue);
                    continue;
                    }
                switch (opcode)
                    {
                case NL_ADD:
                    newValue = value1 + value2;
                    break;
                case NL_SUB:
                    newValue = value1 - value2;
                    break;
                case NL_MULT:
                    newValue = value1 * value2;
                    break;
                case NL_DIV:
                    newValue = value1 / value2;
                    break;
                case NL_EXP:
                    newValue = (DOUBLE)pow ((double)value1, (double)value2);
                    break;
                case NL_MOD:
                    if ((pinfo1->
                         data_type IS_NOT_EQUAL_TO INT_TYPE) OR (pinfo2->
                                                             data_type IS_NOT_EQUAL_TO 
                                                                 INT_TYPE))
                        {
                        status = NL_INVALID_DATA_TYPE;
                        newValue = pinfo1->null_value;
                        break;
                        }
                    else
                        {
                        int_value1 = (INT)value1;
                        int_value2 = (INT)value2;
                        int_newValue = int_value1 % int_value2;
                        newValue = (DOUBLE)int_newValue;
                        }
                    break;
                case NL_CRUSH_MIN:
                    newValue = (value1 <= value2) ? pinfo2->null_value : value1;
                    break;
                case NL_CLIP_MIN:
                    newValue = (value1 <= value2) ? pinfo2->min : value1;
                    break;
                case NL_CRUSH_MAX:
                    newValue = (value1 >= value2) ? pinfo2->null_value : value1;
                    break;
                case NL_CLIP_MAX:
                    newValue = (value1 >= value2) ? pinfo2->null_value : value1;
                    break;
                default:
                    return NL_INVALID_CODE;
                    
                    }
                nlz_replace_value (pdata1, pinfo1, k + 1, &newValue);
                }
            }
        }
    nlz_delete_nulls (*outList, nCol, dimList1);
    return status;
    
    }
/* END:     */
