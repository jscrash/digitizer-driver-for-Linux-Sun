/* DEC/CMS REPLACEMENT HISTORY, Element NL_QNL_INFC.C*/
/* *5    25-OCT-1990 12:25:00 GILLESPIE "(SPR 31) Lint run - correct casting inconsistencies"*/
/* *4    25-JUL-1990 15:02:48 VINCE "(SPR 5644) Header Standardization"*/
/* *3    29-MAR-1990 18:05:32 GILLESPIE "(SPR 1) Clean up logic"*/
/* *2    18-MAR-1990 01:52:58 GILLESPIE "(SPR 1) SMALL TWEEKS, APPLE CHEEKS"*/
/* *1     5-MAR-1990 11:00:12 GILLESPIE "Rename files for Unix compliance (SPR 6015)"*/
/* DEC/CMS REPLACEMENT HISTORY, Element NL_QNL_INFC.C*/
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
#include "esi_nl_msg.h"
#include "esi_math.h"
#include "esi_tc.h"

/* Function Description -----------------------------------------------------
Description:
    Retrieve information about column information.   

Prototype:
    publicdef INT nl_inq_nlist_info_convert (NLIST_HEADER nlist, INT type, UINT nCol, 
        UINT Col_List[], VOIDPTR *Var_List, INT units[], size_t dataSize[], 
        INT dataType[]);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    nlist           -(NLIST_HEADER)
    type            -(INT)
    nCol            -(UINT)
    Col_List        -(UINT [])
    Var_List        -(VOIDPTR *)
    units           -(INT [])
    dataType        -(INT [])
    dataSize        -(size_t [])

Return Value/Status:
    SUCCESS - Successful completion.
    NL_INVALID_CODE
    NL_NULL_VALUES_RETURNED
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT nl_inq_nlist_info_convert (NLIST_HEADER nlist, INT type, UINT nCol,
                                         UINT Col_List[], VOIDPTR *Var_List,
                                         INT units[], size_t dataSize[],
                                         INT dataType[])
#else
publicdef INT nl_inq_nlist_info_convert (nlist, type, nCol, Col_List, Var_List, units,
                                         dataSize, dataType)
NLIST_HEADER nlist;
INT type;
UINT nCol;
UINT Col_List[];
VOIDPTR *Var_List;
INT units[];
INT dataType[];
size_t dataSize[];
#endif
    {
    register UINT i;
    INT status = SUCCESS;
    register COLUMN_HEADER *pinfo;
    DOUBLE avge = 0.0, result = 0.0;
    
    INIT_TEST (nlist);
    EMPTY_TEST (nlist);
    
    for (i = 0; i < nCol; i++)
        {
        COLUMN_TEST (nlist, Col_List[i]);
        }
    /*
       * if the OPCODE TYPE are..., then compute the status of nlist.
    */
    if (nlist->updated)
        {
        BOOL defer_stats = nlist->defer_stats;
        
        switch (type)
            {
        case NL_NLIST_MIN:
        case NL_NLIST_MAX:
        case NL_NLIST_AVG:
        case NL_NLIST_STD_DEV:
            nlist->defer_stats = FALSE;
            nlz_compute_nlist_stats (nlist);
            nlist->defer_stats = defer_stats;
            break;
            }
        }
    for (i = 0; i < nCol; i++)
        {
        pinfo = nlist->info_ptr + Col_List[i] - 1;
        switch (type)
            {
        case NL_NLIST_AVG:
            if (pinfo->min EQUALS HUGE_VAL OR pinfo->n_valid_rows EQUALS 0)
                {
                status = NL_NULL_VALUES_RETURNED;
                result = pinfo->null_value;
                }
            else
                {
                result = pinfo->sum_x / pinfo->n_valid_rows;
                }
            break;
            
        case NL_NLIST_DATA_TYPES:
            *((INT *)Var_List[i]) = pinfo->data_type;
            continue;
            
        case NL_NLIST_DATA_WIDTHS:
            *((INT *)Var_List[i]) = (INT)pinfo->data_size;
            continue;
            
        case NL_NLIST_FIND_TOLERANCE:
            result = pinfo->tolerance;
            break;
            
        case NL_NLIST_MAX:
            if (pinfo->max EQUALS - HUGE_VAL)
                {
                status = NL_NULL_VALUES_RETURNED;
                result = pinfo->null_value;
                }
            else
                {
                result = pinfo->max;
                }
            break;
            
        case NL_NLIST_MIN:
            if (pinfo->min EQUALS HUGE_VAL)
                {
                status = NL_NULL_VALUES_RETURNED;
                result = pinfo->null_value;
                }
            else
                {
                result = pinfo->min;
                }
            break;
            
        case NL_NLIST_NON_NULL:
            *((INT *)Var_List[i]) = (INT)pinfo->n_valid_rows;
            continue;
            
        case NL_NLIST_NULL_VALUE:
            result = pinfo->null_value;
            break;
            
        case NL_NLIST_STD_DEV:
            if (pinfo->min EQUALS HUGE_VAL OR pinfo->n_valid_rows EQUALS 0)
                {
                status = NL_NULL_VALUES_RETURNED;
                result = pinfo->null_value;
                }
            else
                {
                avge = pinfo->sum_x / pinfo->n_valid_rows;
                result = pinfo->sum_x_squared - 2 * avge * pinfo->sum_x +
                         pinfo->n_valid_rows * avge * avge;
                result = (DOUBLE)sqrt ((double)(result / pinfo->n_valid_rows));
                }
            break;
            
        case NL_NLIST_UNITS:
            *((INT *)Var_List[i]) = pinfo->unit;
            continue;
            
        case NL_NLIST_VALUE_INC:
            *((INT *)Var_List[i]) = (INT)pinfo->inc_value;
            continue;
            
        default:
            return NL_INVALID_CODE;
            }
        nlz_move_data (Var_List[i], dataType[i], dataSize[i], result);
        }
    return status;
    }
/* END:     */
