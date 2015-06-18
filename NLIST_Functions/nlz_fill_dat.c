/* DEC/CMS REPLACEMENT HISTORY, Element NLZ_FILL_DAT.C*/
/* *5    25-OCT-1990 12:18:09 GILLESPIE "(SPR 31) Lint run - correct casting inconsistencies"*/
/* *4     7-JUN-1990 15:14:40 MING "(SPR 0) add nl_set_current_row to reset the current_row = 1"*/
/* *3     9-APR-1990 10:31:37 GILLESPIE "(SPR 1) Remove superflurous checks"*/
/* *2    12-MAR-1990 19:03:41 GILLESPIE "(SPR 6016) More error checking on mallocs; re-engineer stats calcs"*/
/* *1     5-MAR-1990 10:55:32 GILLESPIE "Rename files for Unix compliance (SPR 6015)"*/
/* DEC/CMS REPLACEMENT HISTORY, Element NLZ_FILL_DAT.C*/

/******************************************************************************

                Copyright Exploration Systems, Inc. 1989
           Unpublished -- All rights reserved

THIS SOFTWARE IS THE PROPRIETARY PROPERTY OF Exploration Systems, Inc.  AND MAY
CONTAIN CONFIDENTIAL TRADE SECRET INFORMATION. IT IS LICENSED FOR USE ON THE
DESIGNATED EQUIPMENT ON WHICH IT WAS ORIGINALLY INSTALLED AND  MAY NOT BE
MODIFIED, DUPLICATED OR COPIED IN ANY FORM WITHOUT PRIOR WRITTEN CONSENT OF

            Exploration Systems, Inc.
            579 Market Street
            San Francisco, CA  USA 94105
            (415) 974-1600

(The above notice does not constitute an admission of publication or
unrestricted dissemination of the work)

******************************************************************************/

#include "esi_nl_int.h"
#include "esi_tc.h"

/* ***************************************************************************

    ROUTINE:  NLZ_FILL_DATA_ARRAY

    DESCRIPTION
         A help routine of NLZ_SPLINE_NLIST.  This routine perform the function
         of interpreting a specific table and fills the splined data into a
         returned array.

   **************************************************************************** */
publicdef INT nlz_fill_data_array (nlist, table_num, nCol, Col_List,
                                   startValue, increment, total_rows,
                                   Var_List)
/****************************************************************************/


NLIST_HEADER nlist;            /*%P the given pointer of NLIST_HEADER.      */
UINT table_num;                /*%P the number of being operated table.    */
UINT nCol;                    /*%P the number of totally operated dims.    */
UINT Col_List[];            /*%P the array of column index.           */
UINT total_rows;            /*%P the number of rows that should be returned */
DOUBLE increment, startValue;    /*%P the start value and increment of table.*/
DOUBLE *Var_List[];

    {
    UINT i, j, k;            /*   indexes.       */
    INT *units;                /*   array of data units for retrieving data.*/
    INT *dataType;            /*   array of data sizes and data types for  */
    /*   retrieving DOUBLE data.                 */
    size_t *dataSize;
    INT reverse = FALSE;
    UINT returnPts;            /*   returned rows for NL_GET_POINTS_CONVERT*/
    DOUBLE **floatArray;    /*   the array of float value of table.    */
    TABLE_HEADER *ptable;    /*   workspace for current table.           */
    DOUBLE *nullValuesMax;    /*   an array of null values of specified nCol.*/
    DOUBLE *nullValuesMin;    /*   an array of null values of specified nCol.*/
    DOUBLE *nullValues;        /*   an array of null values of specified nCol.*/
    DOUBLE beginX, endX;    /*   boundaries of the interpreting segment of*/
                            /*   independent variable column.         */

    /* ***************************************************************************** */

        
    ptable = nlz_set_table (nlist, table_num);

    if (ptable EQUALS NULL_TABLE)
        {
        return NL_INVALID_TABLE;
        }

    if (ptable->nrows < 2)
        {
        return NL_EMPTY_TABLE;
        }
    /*  allocates workspace for rows in the table. */

    floatArray = (DOUBLE * *)TALLOC (sizeof(DOUBLE *) * nCol);
    dataSize = (size_t *)TALLOC (sizeof(size_t) * nCol);
    dataType = (INT *)TALLOC (sizeof(INT) * nCol);
    units = (INT *)TALLOC (sizeof(INT) * nCol);
    nullValuesMin = (DOUBLE *)TALLOC (sizeof(DOUBLE) * nCol);
    nullValuesMax = (DOUBLE *)TALLOC (sizeof(DOUBLE) * nCol);
    nullValues = (DOUBLE *)TALLOC (sizeof(DOUBLE) * nCol);

    reverse = ((nlist->info_ptr + Col_List[0] - 1)->null_value > 0) ?
        FALSE : TRUE;
    for (i = 0; i < nCol; i++)
        {
        nullValues[i] = (nlist->info_ptr + Col_List[i] - 1)->null_value;
        if (reverse)
            {
            nullValuesMax[i] = 0.99 * nullValues[i];
            nullValuesMin[i] = 1.01 * nullValues[i];
            }
        else
            {
            nullValuesMax[i] = 1.01 * nullValues[i];
            nullValuesMin[i] = 0.99 * nullValues[i];
            }
        floatArray[i] = (DOUBLE *)TALLOC (sizeof(DOUBLE) * ptable->nrows);
        dataSize[i] = sizeof(DOUBLE);
        dataType[i] = FLOAT_TYPE;
        }


    /* reset row number */
	
    nl_set_current_row (nlist, (UINT)1, table_num);
	
	/*   retrieve the DOUBLE type rows.        */

    nl_get_rows_convert (nlist, ptable->nrows, nCol, Col_List,
                         (VOIDPTR *)floatArray, units, dataSize, dataType,
                         &returnPts);


    /*   interpret rows in each column.    */
    /*   fills the first indexed column.     */

    for (i = 0; i < total_rows; i++)
        {
        Var_List[0][i] = startValue + i * increment;
        }

    /*   use the first column to be the indexed*/
    /*   column, then interperts others column.*/

    for (i = 1; i < nCol; i++)
        {
        for (j = 0, k = 0; j < returnPts - 1; j++)
            {
            beginX = floatArray[0][j];
            endX = floatArray[0][j + 1];
            for (; k < total_rows AND Var_List[0][k] <= endX; k++)
                {
                if ((floatArray[i][j] <=
                     nullValuesMax[i] AND floatArray[i][j] >=
                     nullValuesMin[i]) OR (floatArray[i][j + 1] <=
                                          nullValuesMax[i] AND floatArray[i]
                                          [j + 1] >= nullValuesMin[i]))
                    {
                    if (Var_List[0][k] EQUALS beginX)
                        Var_List[i][k] = floatArray[i][j];
                    else if (Var_List[0][k] EQUALS endX)
                        Var_List[i][k] = floatArray[i][j + 1];
                    else
                        Var_List[i][k] = nullValues[i];
                    }
                else if (beginX EQUALS endX)
                    {
                    Var_List[i][k] = floatArray[i][j];
                    }
                else
                    {
                    Var_List[i][k] = (floatArray[i][j + 1] -
                                      floatArray[i][j]) * (Var_List[0][k] -
                                                           beginX) /
                        (endX - beginX) + floatArray[i][j];
                    }
                }
            }
        }
#if NEED_TO_FREE EQUALS TRUE
    /*   free allocated memory.                  */
    for (i = 0; i < nCol; i++)
        {
        tc_free ((VOIDPTR)floatArray[i]);
        }

    tc_free ((VOIDPTR)floatArray);
    tc_free ((VOIDPTR)dataSize);
    tc_free ((VOIDPTR)dataType);
    tc_free ((VOIDPTR)units);
    tc_free ((VOIDPTR)nullValuesMin);
    tc_free ((VOIDPTR)nullValuesMax);
    tc_free ((VOIDPTR)nullValues);
#endif

    return SUCCESS;
    }
