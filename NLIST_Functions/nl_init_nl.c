/* DEC/CMS REPLACEMENT HISTORY, Element NL_INIT_NL.C*/
/* *4    25-OCT-1990 12:22:55 GILLESPIE "(SPR 31) Lint run - correct casting inconsistencies"*/
/* *3    25-JUL-1990 14:59:38 VINCE "(SPR 5644) Header Standardization"*/
/* *2    12-MAR-1990 19:07:05 GILLESPIE "(SPR 6016) More error checking on mallocs; re-engineer stats calcs"*/
/* *1     5-MAR-1990 10:59:35 GILLESPIE "Rename files for Unix compliance (SPR 6015)"*/
/* DEC/CMS REPLACEMENT HISTORY, Element NL_INIT_NL.C*/
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
#include "esi_c_lib.h"

#ifndef ESI_MATH_H

#include "esi_math.h"

#endif

#ifndef ESI_TC_H

#include "esi_tc.h"

#endif

#include "esi_time.h"

/* Function Description -----------------------------------------------------
Description:
    NL_INIT_NLIST allocates a new n_List header and initializes the
    information header for each column specified in the call. No
    tables are allocated by this call.

Prototype:
    publicdef INT nl_init_nlist (NLIST_HEADER *pnlist, UINT nCol, INT *unit, size_t *size, 
        INT *type, INT *inc_value, UINT blocksize);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    pnlist          -(NLIST_HEADER *) Pointer to n_list header pointer.
    nCol            -(UINT) Number of columns in n-list.
    unit            -(INT *) Units code array for each column.
    size            -(size_t *) Array containing the size of each column.
    type            -(INT *) Array containing data type of each column.
    inc_value       -(INT *) Array containing incrementing indicators.
    blocksize       -(UINT) Allocation blocksize.

Return Value/Status:
    SUCCESS - Successful completion.
    NL_INVALID_VALUE_INCREMENT
    NL_INVALID_DATA_SIZE
    NL_OVERFLOW
    NL_INVALID_COLUMN
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT nl_init_nlist (NLIST_HEADER *pnlist, UINT nCol, INT *unit, size_t *size,
                             INT *type, INT *inc_value, UINT blocksize)
#else
publicdef INT nl_init_nlist (pnlist, nCol, unit, size, type, inc_value, blocksize)
NLIST_HEADER *pnlist;
UINT nCol;
INT *unit;
size_t *size;
INT *type;
INT *inc_value;
UINT blocksize;
#endif
    {
    UINT i;
    UINT *Col_List;
    INT *intPtr = (INT *)0;
    SHORT *shortPtr = (SHORT *)0;
    FLOAT *floatPtr = (FLOAT *)0;
    DOUBLE *doublePtr = (DOUBLE *)0;
    NLIST_HEADER nlist;
    COLUMN_HEADER *pinfo;
    VOIDPTR *Var_List;
    
    /* ********************************************************************** */
    /* check the security. */
#ifdef SECURITY
    time_t current_time;
    INT differ_time;
    time (&current_time);
    differ_time = NL_EXPIRATION - current_time;
    if (differ_time < 0)
        return NL_LICENSE_EXPIRED;
#endif
    
    /* assume worst case - initialize output
     * pointer to NULL
     */
    *pnlist = (NLIST_HEADER)0;
    
    /*   Insure that the number of columns
         is ok. */
    
    if (nCol < 1)
        {
        return NL_INVALID_COLUMN;
        }
    /*   Allocate memory for new n-list header. */
    
    Col_List = (UINT *) TALLOC (sizeof(UINT) * nCol);
    Var_List = (VOIDPTR *) TCP_ALLOCATE (sizeof(VOIDPTR) * nCol);
    
    nlist = (NLIST_HEADER) tc_zalloc (sizeof(NLIST_STRUCT) + sizeof(COLUMN_HEADER) *
                                      (nCol - 1));
    if (nlist == (NLIST_HEADER)0)
        {
        return NL_OVERFLOW;
        }
    nlist->nCol = nCol;
    nlist->n_value_cols = nCol;
    nlist->blocksize = MAX (NL_DEFAULT_BLOCKSIZE, blocksize);
    
    for (i = 0, pinfo = nlist->info_ptr; i < nCol;
         i++, pinfo++, size++, unit++, type++, inc_value++)
        {
        Col_List[i] = i + 1;
        pinfo->min = HUGE_VAL;
        pinfo->max = -HUGE_VAL;
        
        /*
           * Set the data size for each column...
           * only a few data types allowed: FLOAT_TYPE, INT_TYPE, CHAR_TYPE
        */
        if (*size < 1)
            {
            return NL_INVALID_DATA_SIZE;
            }
        switch (*type)
            {
        case INT_TYPE:
            strcpy ((char *)pinfo->convert_format, "%d");
            switch (*size)
                {
            case sizeof(CHAR):
            case sizeof(SHORT):
                shortPtr = (SHORT *) TALLOC (sizeof(SHORT));
                *shortPtr = NL_DEFAULT_NULL;
                Var_List[i] = (VOIDPTR)shortPtr;
                break;
            case sizeof(INT):
                intPtr = (INT *) TALLOC (sizeof(INT));
                *intPtr = NL_DEFAULT_NULL;
                Var_List[i] = (VOIDPTR)intPtr;
                break;
            default:
                return NL_INVALID_DATA_SIZE;
                }
            break;
        case FLOAT_TYPE:
            strcpy ((char *)pinfo->convert_format, "%f");
            switch (*size)
                {
            case sizeof(FLOAT):
                floatPtr = (FLOAT *) TALLOC (sizeof(FLOAT));
                *floatPtr = NL_DEFAULT_NULL;
                Var_List[i] = (VOIDPTR)floatPtr;
                break;
            case sizeof(DOUBLE):
                doublePtr = (DOUBLE *) TALLOC (sizeof(DOUBLE));
                *doublePtr = NL_DEFAULT_NULL;
                Var_List[i] = (VOIDPTR)doublePtr;
                break;
                
            default:
                return NL_INVALID_DATA_SIZE;
                }
            break;
        case CHAR_TYPE:
            strcpy ((char *)pinfo->convert_format, "%f");
            Var_List[i] = (VOIDPTR) TALLOC (*size);
            break;
        case BOOL_TYPE:
            strcpy ((char *)pinfo->convert_format, "%d");
            break;
        default:
            return NL_INVALID_DATA_TYPE;
            }
        pinfo->data_size = *size;
        pinfo->data_type = *type;
        
        switch (*inc_value)
            {
        case INCREMENT_LIST:
            pinfo->inc_value = TRUE;
            (nlist->n_value_cols)--;
            break;
        case VALUE_LIST:
            pinfo->inc_value = FALSE;
            break;
        default:
            return NL_INVALID_VALUE_INCREMENT;
            }
        /*       pinfo -> null_value = NL_DEFAULT_NULL;  */
        }
    nlist->initialized = NLIST_INIT;
    nlist->defer_stats = FALSE;
    
    /*
       * set the default NULL value.
    */
    nl_set_nlist_info (nlist, NL_NLIST_NULL_VALUE, nCol, Col_List, Var_List);
    

#if NEED_TO_FREE 
    tc_free ((VOIDPTR)Col_List);
    
    for (i = 0; i < nCol; i++)
        {
        if (Var_List[i] != (VOIDPTR)0)
            tc_free (Var_List[i]);
        }
#endif
    tc_free ((VOIDPTR)Var_List);
    
    *pnlist = nlist;
    return SUCCESS;
    }
/* END:     */
