/* DEC/CMS REPLACEMENT HISTORY, Element NL_CMP_NL.C*/
/* *4    25-OCT-1990 12:21:30 GILLESPIE "(SPR 31) Lint run - correct casting inconsistencies"*/
/* *3    25-JUL-1990 14:56:42 VINCE "(SPR 5644) Header Standardization"*/
/* *2    12-MAR-1990 19:06:28 GILLESPIE "(SPR 6016) More error checking on mallocs; re-engineer stats calcs"*/
/* *1     5-MAR-1990 10:57:59 GILLESPIE "Rename files for Unix compliance (SPR 6015)"*/
/* DEC/CMS REPLACEMENT HISTORY, Element NL_CMP_NL.C*/
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
    This function performs the comparison operation on two n-Lists.   

Prototype:
    publicdef INT nl_compare_nlist (NLIST_HEADER nlist1, UINT Col_List1[], INT opcode, 
        NLIST_HEADER nlist2, UINT Col_List2[], NLIST_HEADER *nlist3);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    nlist1          -(NLIST_HEADER)
    nlist2          -(NLIST_HEADER)
    nlist3          -(NLIST_HEADER *)
    Col_List        -(UINT [])
    Col_List2       -(UINT [])
    opcode          -(INT)

Return Value/Status:
    SUCCESS - Successful completion.
    NL_INVALID_CODE
    NL_OVERFLOW
    NL_TWO_LISTS_NOT_SAME_INCREMENT
    NL_INVALID_DATA_TYPE
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT nl_compare_nlist (NLIST_HEADER nlist1, UINT Col_List1[], INT opcode,
                                NLIST_HEADER nlist2, UINT Col_List2[],
                                NLIST_HEADER *nlist3)
#else
publicdef INT nl_compare_nlist (nlist1, Col_List1, opcode, nlist2, Col_List2, nlist3)
NLIST_HEADER nlist1, nlist2, *nlist3;
UINT Col_List1[], Col_List2[];
INT opcode;
#endif
    {
    VOIDPTR Var_List[2];
    VOIDPTR value1, value2;
    COLUMN_HEADER *pinfo1, *pinfo2;
    TABLE_HEADER *ptab1, *ptab2;
    DATA_HEADER *pdata1, *pdata2;
    UINT i;
    INT status;
    UINT maxLength = 0, ndim = 2, stkNum;
    INT *result;
    BOOL returnDouble = TRUE;
    UINT rowIndex;
    UINT totalrows;
    BOOL startNewtable;
    UINT Col_List3[2];
    INT incValue[2];
    INT dataType[2], units[2];
    size_t dataSize[2];
    DOUBLE minIndex, maxIndex, indexValue, increment;
    CHAR c_name[2][32];
    
    INIT_TEST (nlist1);
    INIT_TEST (nlist2);
    
    Col_List3[0] = 1, Col_List3[1] = 2;
    incValue[0] = INCREMENT_LIST, incValue[1] = VALUE_LIST;
    
    /*
       * error checking.
    */
    if (nlz_check_start_inc (nlist1, Col_List1[0]))
        {
        return NL_INVALID_NLIST1_INCREMENT;
        }
    if (nlz_check_start_inc (nlist2, Col_List2[0]))
        {
        return NL_INVALID_NLIST2_INCREMENT;
        }
    pinfo1 = nlist1->info_ptr + Col_List1[0] - 1;
    pinfo2 = nlist2->info_ptr + Col_List2[0] - 1;
    if (!pinfo1->inc_value || !pinfo2->inc_value)
        {
        return NL_INVALID_VALUE_INCREMENT;
        }
    ptab1 = nlz_read_table_header (nlist1, &nlist1->table_elements[0]);
    ptab2 = nlz_read_table_header (nlist2, &nlist2->table_elements[0]);
    
    pdata1 = ptab1->data_ptr + Col_List1[0] - 1;
    pdata2 = ptab2->data_ptr + Col_List2[0] - 1;
    if (pdata1->increment IS_NOT_EQUAL_TO pdata2->increment)
        {
        return NL_TWO_LISTS_NOT_SAME_INCREMENT;
        }
    /*
       * check the data_type for those only allows BOOLEAN opcode.
    */
    if (opcode EQUALS NL_AND || opcode EQUALS NL_OR || opcode EQUALS NL_XOR ||
        opcode EQUALS NL_NAND || opcode EQUALS NL_NOR)
        {
        if (pinfo1->data_type IS_NOT_EQUAL_TO INT_TYPE &&
            pinfo1->data_size IS_NOT_EQUAL_TO sizeof(INT) &&
            pinfo2->data_type IS_NOT_EQUAL_TO INT_TYPE &&
            pinfo2->data_size IS_NOT_EQUAL_TO sizeof(INT))
            {
            return NL_INVALID_DATA_TYPE;
            }
        returnDouble = TRUE;
        }
    /*
     * find the maxlength for the new N-list3.
     */
    
    /*
     * find the minIndex.
     */
    minIndex = MIN (pdata1->start_value, pdata2->start_value);
    dataSize[0] = pinfo1->data_size;
    dataSize[1] = sizeof(INT);
    dataType[0] = pinfo1->data_type;
    dataType[1] = INT_TYPE;
    units[0] = pinfo1->unit;
    units[1] = 0;
    /*
       * find the maxIndex.
    */
    
    ptab1 = nlz_read_table_header (nlist1,
                                   &nlist1->table_elements[nlist1->total_tables - 1]);
    ptab2 = nlz_read_table_header (nlist2,
                                   &nlist2->table_elements[nlist2->total_tables - 1]);
    pdata1 = ptab1->data_ptr + Col_List1[0] - 1;
    pdata2 = ptab2->data_ptr + Col_List2[0] - 1;
    increment = pdata1->increment;
    maxIndex = MAX (pdata1->start_value + (ptab1->nrows - 1) * increment,
                    pdata2->start_value + (ptab2->nrows - 1) * increment);
    maxLength = (maxIndex - minIndex) / pdata1->increment + 1;
    
    /*
       * initialize N-list3.
    */
    nl_init_nlist (nlist3, ndim, units, dataSize, dataType, incValue,
                   nlist1->blocksize);
    nl_inq_nlist_info (nlist1, NL_NLIST_COLUMN_NAME, (UINT)2, Col_List1, (VOIDPTR *)c_name);
    status = nl_set_nlist_info (*nlist3, NL_NLIST_COLUMN_NAME, (UINT)2, Col_List3,
                                (VOIDPTR *)c_name);
    if (status != SUCCESS)
        {
        return status;
        }
    value1 = tc_alloc (sizeof(INT) * 2);
    if (value1 == (VOIDPTR)0)
        {
        return NL_OVERFLOW;
        }
    value2 = tc_alloc (sizeof(INT) * 2);
    if (value2 == (VOIDPTR)0)
        {
        return NL_OVERFLOW;
        }
    Var_List[0] = (VOIDPTR)tc_alloc (sizeof(BYTE) * 8);
    if (Var_List[0] == (VOIDPTR)0)
        {
        return NL_OVERFLOW;
        }
    Var_List[1] = (VOIDPTR)(result = (INT *)tc_alloc (sizeof(INT) * (maxLength + 10)));
    if (result == (INT *)0)
        {
        return NL_OVERFLOW;
        }
    for (i = 0; i < maxLength; i++)
        {
        
        /*
           * if one of N-nlists has NULL value/ table break, the the result value is
           * FALSE. And continue to do next one.
        */
        indexValue = minIndex + i * increment;
        if (nlz_get_index_value (nlist1, indexValue, Col_List1, value1,
                                 returnDouble) ||
            nlz_get_index_value (nlist2, indexValue, Col_List2, value2, returnDouble))
            {
            result[i] = 99999;
            continue;
            }
        /*
           * otherwise, analyze the OPCODE and do the operation.
        */
        switch (opcode)
            {
        case NL_AND:
            result[i] = (INT)(*(INT *)value1 && *(INT *)value2);
            break;
            
        case NL_NAND:
            result[i] = (INT)(!*(INT *)value1 || !*(INT *)value2);
            break;
            
        case NL_OR:
            result[i] = (INT)(*(INT *)value1 || *(INT *)value2);
            break;
            
        case NL_NOR:
            result[i] = (INT)(NOT * (INT *)value1 && NOT * (INT *)value2);
            break;
            
        case NL_XOR:
            result[i] = (INT)((*(INT *)value1 && *(INT *)value2) ^ (*(INT *)value2 &&
                                                               *(INT *)value1));
            break;
        case NL_GT:
            result[i] = (INT)(*(DOUBLE *)value1 > *(DOUBLE *)value2);
            break;
            
        case NL_LT:
            result[i] = (INT)(*(DOUBLE *)value1 < *(DOUBLE *)value2);
            break;
            
        case NL_GE:
            result[i] = (INT)(*(DOUBLE *)value1 >= *(DOUBLE *)value2);
            break;
            
        case NL_LE:
            result[i] = (INT)(*(DOUBLE *)value1 <= *(DOUBLE *)value2);
            break;
            
        case NL_EQ:
            result[i] = (INT)(*(DOUBLE *)value1 == *(DOUBLE *)value2);
            break;
            
        case NL_NE:
            result[i] = (INT)(*(DOUBLE *)value1 != *(DOUBLE *)value2);
            break;
            
        default:
            return NL_INVALID_CODE;
            }
        }
    /*
       * update the result N-list3.
    */
    for (i = 0, startNewtable = TRUE; i < maxLength; i++)
        {
        if (result[i] EQUALS 99999 && startNewtable EQUALS TRUE)
            continue;
        if (startNewtable)
            {
            nl_start_table (*nlist3, &stkNum);
            (*nlist3)->current_table_ptr->data_ptr->start_value = increment * i +
                                                                  minIndex;
            (*nlist3)->current_table_ptr->data_ptr->increment = increment;
            rowIndex = i;
            totalrows = 1;
            startNewtable = FALSE;
            continue;
            }
        if (result[i] EQUALS 99999)
            {
            startNewtable = TRUE;
            Var_List[1] = (VOIDPTR) & result[rowIndex];
            nl_add_rows (*nlist3, totalrows, (UINT)2, Col_List3, Var_List);
            }
        else
            {
            totalrows += 1;
            }
        }
    /*
     * free the allocated memory.
     */
    tc_free ((VOIDPTR)value1);
    tc_free ((VOIDPTR)value2);
    tc_free ((VOIDPTR)Var_List[0]);
    tc_free ((VOIDPTR)result);
    return SUCCESS;
    }
/* END:     */
