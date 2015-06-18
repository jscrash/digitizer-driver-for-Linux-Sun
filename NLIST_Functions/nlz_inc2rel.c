/* DEC/CMS REPLACEMENT HISTORY, Element NLZ_INC2REL.C*/
/* *6    25-OCT-1990 12:18:59 GILLESPIE "(SPR 31) Lint run - correct casting inconsistencies"*/
/* *5    25-JUL-1990 14:53:10 VINCE "(SPR 5644) Header Standardization"*/
/* *4    12-MAR-1990 19:04:28 GILLESPIE "(SPR 6016) More error checking on mallocs; re-engineer stats calcs"*/
/* *3     5-MAR-1990 10:45:16 GILLESPIE "(SPR 6015) Change file names to be compliant with Unix"*/
/* *2    24-SEP-1989 22:27:15 GILLESPIE "(SPR 200) New n-List 4.0 - Table/Row mods"*/
/* *1    19-JUN-1989 13:16:54 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element NLZ_INC2REL.C*/
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
    To convert all increment columns to real value dimension.

Prototype:
    publicdef INT nlz_convert_inc_2_real (NLIST_HEADER nlist);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (M) nlist       -(NLIST_HEADER *) The pointer of NLIST_HEADER.

Return Value/Status:
    SUCCESS - Successful completion.
    NL_INVALID_DATA_TYPE
    NL_INVALID_DATA_SIZE
    NL_OVERFLOW
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT nlz_convert_inc_2_real (NLIST_HEADER nlist) 
#else
publicdef INT nlz_convert_inc_2_real (nlist)
NLIST_HEADER nlist;
#endif
    {
    TABLE_HEADER *ptable;
    COLUMN_HEADER *pinfo;
    DATA_HEADER *pdata;
    INT *intPtr;
    FLOAT *floatPtr;
    DOUBLE *doublePtr;
    SHORT *shortPtr;
    UINT i, j, k;
    
    for (j = 0; j < nlist->total_tables; j++)
        {
        ptable = nlz_set_table (nlist, j + 1);
        pdata = ptable->data_ptr;
        for (i = 0, pinfo = nlist->info_ptr; i < nlist->nCol; pinfo++, pdata++, i++)
            {
            /*
             * If the column is not incremental column,
             * continue.
             */
            if (!pinfo->inc_value)
                continue;
            
            pinfo->inc_value = pdata->inc_value = FALSE;
            /* allocated the space.         */
            pdata->data = (BYTE *)tc_alloc (pinfo->data_size * ptable->nallocated);
            if (pdata->data EQUALS (BYTE *)0)
                {
                return NL_OVERFLOW;
                }
            switch (pinfo->data_type)
                {
            case INT_TYPE:
                switch (pinfo->data_size)
                    {
                case sizeof(CHAR):
                case sizeof(SHORT):
                    shortPtr = (SHORT *)pdata->data;
                    for (k = 0; k < ptable->nrows; k++, shortPtr++)
                        {
                        *shortPtr = pdata->start_value + k * pdata->increment;
                        }
                    break;
                case sizeof(INT):
                    intPtr = (INT *)pdata->data;
                    for (k = 0; k < ptable->nrows; k++, intPtr++)
                        {
                        *intPtr = pdata->start_value + k * pdata->increment;
                        }
                    break;
                default:
                    return NL_INVALID_DATA_SIZE;
                    }
                break;
            case FLOAT_TYPE:
                switch (pinfo->data_size)
                    {
                case sizeof(FLOAT):
                    floatPtr = (FLOAT *)pdata->data;
                    for (k = 0; k < ptable->nrows; k++, floatPtr++)
                        {
                        *floatPtr = pdata->start_value + k * pdata->increment;
                        }
                    break;
                case sizeof(DOUBLE):
                    doublePtr = (DOUBLE *)pdata->data;
                    for (k = 0; k < ptable->nrows; k++, doublePtr++)
                        {
                        *doublePtr = pdata->start_value + k * pdata->increment;
                        }
                    break;
                    
                default:
                    return NL_INVALID_DATA_SIZE;
                    }
                break;
                
            default:
                return NL_INVALID_DATA_TYPE;
                }
            }
        }
    return SUCCESS;
    }
/* END:     */
