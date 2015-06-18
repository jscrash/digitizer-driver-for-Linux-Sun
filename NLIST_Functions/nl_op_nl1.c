/* DEC/CMS REPLACEMENT HISTORY, Element NL_OP_NL1.C*/
/* *4    25-OCT-1990 12:24:23 GILLESPIE "(SPR 31) Lint run - correct casting inconsistencies"*/
/* *3    25-JUL-1990 17:35:36 VINCE "(SPR 5644) Fixed compile errors introduced with the header changes"*/
/* *2    25-JUL-1990 15:01:21 VINCE "(SPR 5644) Header Standardization"*/
/* *1     5-MAR-1990 10:59:46 GILLESPIE "Rename files for Unix compliance (SPR 6015)"*/
/* DEC/CMS REPLACEMENT HISTORY, Element NL_OP_NL1.C*/
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
    Function to perform an unary arithmetric operation on an n_list. 

Prototype:
    publicdef INT nl_operate_nlist_1(NLIST_HEADER nlist, UINT nCol,
                  UINT Col_List[], INT opcode, NLIST_HEADER maskList,
                  UINT maskDim[], NLIST_HEADER outList);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    nlist           -(NLIST_HEADER)
    maskList        -(NLIST_HEADER)
    outList         -(NLIST_HEADER)
    nCol            -(UINT)
    Col_List        -(UINT [])
    maskDim         -(UINT [])
    opcode          -(INT)

Return Value/Status:
    SUCCESS - Successful completion.
    NL_INVALID_CODE
    NL_INVALID_NLIST1_INCREMENT
    NL_INVALID_NLIST2_INCREMENT
    NL_TWO_LISTS_NOT_SAME_INCREMENT
    NL_INVALID_TABLE
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT nl_operate_nlist_1 (NLIST_HEADER nlist, UINT nCol, UINT Col_List[],
                                  INT opcode, NLIST_HEADER maskList, UINT maskDim[],
                                  NLIST_HEADER *outList)
#else
publicdef INT nl_operate_nlist_1 (nlist, nCol, Col_List, opcode, maskList, maskDim,
                                  outList)
NLIST_HEADER nlist, maskList, *outList;
UINT nCol, Col_List[], maskDim[];
INT opcode;
#endif
    {
    UINT i, j, k, nrows;
    BOOL noMask = FALSE, boolean;
    UINT twoDim[2];
    DOUBLE increment, value, newValue, startValue;
    DATA_HEADER *pdata, *pdataMask, *pdataMaskLast;
    COLUMN_HEADER *pinfo;
    TABLE_HEADER *ptable;
    
    noMask = (maskList EQUALS (NLIST_HEADER)NULL) ? TRUE : FALSE;
    /*
       * error checking.
    */
    if (nlz_check_start_inc (nlist, Col_List[0]))
        return NL_INVALID_NLIST1_INCREMENT;
    if (!noMask)
        {
        if (nlz_check_start_inc (maskList, Col_List[0]))
            {
            return NL_INVALID_NLIST2_INCREMENT;
            }
        nlz_set_table (nlist, (UINT)1);
        pdata = nlist->current_table_ptr->data_ptr + Col_List[0] - 1;
        pdataMask = maskList->current_table_ptr->data_ptr;
        increment = pdataMask->increment;
        if (pdata->increment IS_NOT_EQUAL_TO increment)
            {
            return NL_TWO_LISTS_NOT_SAME_INCREMENT;
            }
        }
    /*
       * make a copy of N-list, and set it to be the returned N-list.
    */
    nl_copy_nlist (nlist, outList);
    
    /*
       * work on each qualified table.
    */
    if (noMask)
        {
        /*
           * work on each column.
        */
        for (i = 0; i < nCol - 1; i++)
            {
            pinfo = (*outList)->info_ptr + Col_List[i + 1] - 1;
            for (j = 0; j < nlist->total_tables; j++)
                {
                ptable = nlz_set_table (*outList, j + 1);
                if (ptable EQUALS NULL_TABLE)
                    {
                    return NL_INVALID_TABLE;
                    }
                pdata = ptable->data_ptr + Col_List[i + 1] - 1;
                for (k = 0; k < ptable->nrows; k++)
                    {
                    nl_set_current_row (*outList, k + 1, (*outList)->current_table);
                    nlz_get_value (pdata, pinfo, k + 1, &value);
                    if (value EQUALS pinfo->null_value)
                        continue;
                    switch (opcode)
                        {
                    case NL_ABS:
                        newValue = (DOUBLE)ABS (value);
                        break;
                        
                    case NL_LOG10:
                        newValue = (DOUBLE)log10 ((double)value);
                        break;
                        
                    case NL_LOGE:
                        newValue = (DOUBLE)log ((double)value);
                        break;
                        
                    case NL_SIN:
                        newValue = (DOUBLE)sin ((double)value);
                        break;
                        
                    case NL_COS:
                        newValue = (DOUBLE)cos ((double)value);
                        break;
                        
                    case NL_TAN:
                        newValue = (DOUBLE)tan ((double)value);
                        break;
                        
                    case NL_ASIN:
                        newValue = (DOUBLE)asin ((double)value);
                        break;
                        
                    case NL_ACOS:
                        newValue = (DOUBLE)acos ((double)value);
                        break;
                        
                    case NL_ATAN:
                        newValue = (DOUBLE)atan ((double)value);
                        break;
                        
                    default:
                        return NL_INVALID_CODE;
                        
                        }
                    nlz_replace_value (pdata, pinfo, k + 1, &newValue);
                    }
                }
            }
        return SUCCESS;
        }
    /*
       * If there is a mask N-list, then delete tables, which are out of
       * the maskList range first.
    */
    ptable = nlz_set_table (maskList, maskList->total_tables);
    if (ptable EQUALS NULL_TABLE)
        {
        return NL_INVALID_TABLE;
        }
    pdataMaskLast = ptable->data_ptr;
    if (!noMask)
        {
        nrows = ptable->nrows;
        for (i = nlist->total_tables; i > 0; i--)
            {
            ptable = nlz_set_table (*outList, i);
            if (ptable EQUALS NULL_TABLE)
                {
                return NL_INVALID_TABLE;
                }
            pdata = ptable->data_ptr + Col_List[0] - 1;
            if ((pdata->start_value >
                 pdataMaskLast->start_value + increment * (nrows -
                                                           1)) OR
				(pdata->start_value + (ptable->nrows - 1) * increment <
                 pdataMask-> start_value))
                nl_delete_table (*outList, i);
            }
        }
    /*
       * work on each qualified table.
    */
    for (i = 0; i < (*outList)->total_tables; i++)
        {
        ptable = nlz_set_table (*outList, i + 1);
        if (ptable EQUALS NULL_TABLE)
            {
            return NL_INVALID_TABLE;
            }
        for (j = 0; j < nCol - 1; j++)
            {
            pinfo = (*outList)->info_ptr + Col_List[j + 1] - 1;
            pdata = ptable->data_ptr + Col_List[0] - 1;
            startValue = pdata->start_value;
            pdata = ptable->data_ptr + Col_List[j + 1] - 1;
            
            for (k = ptable->nrows; k > 0; k--)
                {
                nl_set_current_row (*outList, k, (*outList)->current_table);
                twoDim[0] = 1;
                twoDim[1] = 2;
                if (nlz_get_index_value (maskList, increment * (k - 1) + startValue,
                                         twoDim, (VOIDPTR)&boolean, FALSE))
                    {
                    newValue = pinfo->null_value;
                    nlz_replace_value (pdata, pinfo, k, &newValue);
                    continue;
                    }
                if (!boolean AND opcode EQUALS NL_IDENTICAL)
                    {
                    continue;
                    }
                twoDim[0] = Col_List[0];
                twoDim[1] = Col_List[j + 1];
                if (!nlz_get_index_value (*outList, increment * (k - 1) + startValue,
                                          twoDim, (VOIDPTR)&value, TRUE))
                    {
                    if (value EQUALS pinfo->null_value)
                        continue;
                    switch (opcode)
                        {
                    case NL_ABS:
                        newValue = ABS (value);
                        ;
                        break;
                        
                    case NL_LOG10:
                        newValue = (DOUBLE)log10 ((double)value);
                        break;
                        
                    case NL_LOGE:
                        newValue = (DOUBLE)log ((double)value);
                        break;
                        
                    case NL_SIN:
                        newValue = (DOUBLE)sin ((double)value);
                        break;
                        
                    case NL_COS:
                        newValue = (DOUBLE)cos ((double)value);
                        break;
                        
                    case NL_TAN:
                        newValue = (DOUBLE)tan ((double)value);
                        break;
                        
                    case NL_ASIN:
                        newValue = (DOUBLE)asin ((double)value);
                        break;
                        
                    case NL_ACOS:
                        newValue = (DOUBLE)acos ((double)value);
                        break;
                        
                    case NL_ATAN:
                        newValue = (DOUBLE)atan ((double)value);
                        break;
                        
                    default:
                        return NL_INVALID_CODE;
                        }
                    nlz_replace_value (pdata, pinfo, k, &newValue);
                    }
                }
            }
        }
    nlz_delete_nulls (*outList, nCol, Col_List);
    return SUCCESS;
    }
/* END:     */
