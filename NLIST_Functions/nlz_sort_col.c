/* DEC/CMS REPLACEMENT HISTORY, Element NLZ_SORT_COL.C */
/* *4    25-OCT-1990 12:20:02 GILLESPIE "(SPR 31) Lint run - correct casting inconsistencies" */
/* *3    25-JUL-1990 14:54:58 VINCE "(SPR 5644) Header Standardization" */
/* *2     5-MAR-1990 10:45:37 GILLESPIE "(SPR 6015) Change file names to be compliant with Unix" */
/* *1    24-SEP-1989 22:34:23 GILLESPIE "New n-List V4.0" */
/* DEC/CMS REPLACEMENT HISTORY, Element NLZ_SORT_COL.C */
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
#include "esi_tc.h"
#include "esi_ho.h"

/* Function Description -----------------------------------------------------
Description:
    Function sorts an array of values.

Prototype:
    publicdef INT nlz_sort_dim (UINT len, UINT indexA[], DOUBLE *value, INT order, 
        COLUMN_HEADER *pinfo)
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    len             -(UINT)
    indexA          -(UINT [])
    value           -(DOUBLE *)
    order           -(INT)
    pinfo           -(COLUMN_HEADER *)

Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT nlz_sort_dim (UINT len, UINT indexA[], DOUBLE *value, INT order,
                            COLUMN_HEADER *pinfo)
#else
publicdef INT nlz_sort_dim (len, indexA, value, order, pinfo)
UINT len;
UINT indexA[];
DOUBLE *value;
INT order;
COLUMN_HEADER *pinfo;
#endif
    {
    UINT i, j, indexF, tempIndex;
    BOOL exchange = FALSE;
    size_t data_size;
    DOUBLE tempValue, minMax;
    DOUBLE high_null, low_null;
    DOUBLE nullValue = pinfo->null_value;
    BYTE *ptr, *value_ptr, *minMaxString, *tempString;
    
    high_null = nullValue * ((nullValue > 0) ? 1.001 : 0.999);
    low_null = nullValue * ((nullValue > 0) ? 0.999 : 1.001);
    
    value_ptr = (BYTE *)value;
    /* sort the array.  */
    if (pinfo->data_type IS_NOT_EQUAL_TO CHAR_TYPE)
        {
        for (i = 0; i < len; i++)
            {
            exchange = FALSE;
            
            /*   if the picked value is NULL value, the
                 shift the null value row to the end. */
            if (value[i] >= low_null AND value[i] <= high_null)
                {
                if (i < len - 1)
                    {
                    value[i] = value[i + 1];
                    value[i + 1] = nullValue;
                    }
                }
            minMax = value[i];
            tempIndex = i;
            for (j = i + 1; j < len; j++)
                {
                if (((order EQUALS NL_ASCENDING AND value[j] <
                      minMax)OR (order EQUALS NL_DESCENDING AND value[j] >
                                 minMax))AND (!(value[j] >= low_null AND value[j] <=
                                                high_null)))
                    {
                    exchange = TRUE;
                    indexF = j;
                    minMax = value[j];
                    }
                }
            /*   switch the value and index.   */
            
            if (exchange)
                {
                tempValue = value[i];
                value[i] = minMax;
                value[indexF] = tempValue;
                tempIndex = indexA[indexF];
                indexA[indexF] = indexA[i];
                indexA[i] = tempIndex;
                exchange = FALSE;
                }
            }
        }
    else
        {
        data_size = pinfo->data_size;
        minMaxString = (BYTE *)TALLOC (data_size + 1);
        tempString = (BYTE *)TALLOC (data_size + 1);
        for (i = 0; i < len; i++)
            {
            exchange = FALSE;
            
            hoblockmove ((VOIDPTR)(value_ptr + (i - 1) * data_size), (VOIDPTR)minMaxString, data_size);
            tempIndex = i;
            for (j = i + 1; j < len; j++)
                {
                ptr = value_ptr + (j - 1) * data_size;
                if ((order EQUALS NL_ASCENDING AND strncmp ((char *)ptr,
                                                            (char *)minMaxString,
                                                            data_size) <
                     0) OR (order EQUALS NL_DESCENDING AND strncmp ((char *)ptr,
                                                                   (char *)
                                                                   minMaxString,
                                                                   data_size) > 0))
                    {
                    exchange = TRUE;
                    indexF = j;
                    hoblockmove ((VOIDPTR)minMaxString, (VOIDPTR)ptr, data_size);
                    }
                }
            /*   switch the value and index.   */
            
            if (exchange)
                {
                hoblockmove ((VOIDPTR)(value_ptr + (i - 1) * data_size), (VOIDPTR)tempString, data_size);
                hoblockmove ((VOIDPTR)minMaxString, (VOIDPTR)(value_ptr + (i - 1) * data_size), data_size);
                hoblockmove ((VOIDPTR)tempString, (VOIDPTR)(value_ptr + (indexF - 1) * data_size),
                             data_size);
                tempIndex = indexA[indexF];
                indexA[indexF] = indexA[i];
                indexA[i] = tempIndex;
                exchange = FALSE;
                }
            }
#if NEED_TO_FREE EQUALS TRUE
        tc_free ((VOIDPTR)minMaxString);
        tc_free ((VOIDPTR)tempString);
#endif
        }
    return SUCCESS;
    }
/* END:     */
