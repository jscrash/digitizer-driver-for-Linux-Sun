/* DEC/CMS REPLACEMENT HISTORY, Element NLZ_UPD_ROWS.C */
/* *12   25-OCT-1990 12:20:29 GILLESPIE "(SPR 31) Lint run - correct casting inconsistencies" */
/* *11    7-JUN-1990 15:12:42 MING "(SPR 0) fix bug on if pinfo->inc_value = TURE then value = ... * k (not j)" */
/* *10   10-APR-1990 15:16:38 GILLESPIE "(SPR 1) Misplaced some lines in last checkin.  They're better now." */
/* *9     9-APR-1990 10:32:21 GILLESPIE "(SPR 1) Remove superflurous checks" */
/* *8    23-MAR-1990 22:11:50 GILLESPIE "(SPR 1) More stats changes" */
/* *7    22-MAR-1990 01:34:56 GILLESPIE "(SPR 1) Correct problems with stats on delete" */
/* *6    18-MAR-1990 01:50:01 GILLESPIE "(SPR 1) FIX UP LOGIC WRT NULL VALUES" */
/* *5    16-MAR-1990 14:45:07 GILLESPIE "(SPR 1) Continue chasing mysterious statistics bug" */
/* *4    15-MAR-1990 13:55:49 GILLESPIE "(SPR 1) Change handling of incrementing dimensions" */
/* *3    12-MAR-1990 19:05:48 GILLESPIE "(SPR 6016) More error checking on mallocs; re-engineer stats calcs" */
/* *2     5-MAR-1990 10:45:44 GILLESPIE "(SPR 6015) Change file names to be compliant with Unix" */
/* *1    24-SEP-1989 22:34:35 GILLESPIE "New n-List V4.0" */
/* DEC/CMS REPLACEMENT HISTORY, Element NLZ_UPD_ROWS.C */
/******************************************************************************

                Copyright Finder Graphics Systems, Inc. 1990
           Unpublished -- All rights reserved

THIS SOFTWARE IS THE PROPRIETARY PROPERTY OF Finder Graphics Systems, Inc.  AND
MAY CONTAIN CONFIDENTIAL TRADE SECRET INFORMATION. IT IS LICENSED FOR USE ON THE
DESIGNATED EQUIPMENT ON WHICH IT WAS ORIGINALLY INSTALLED AND  MAY NOT BE
MODIFIED, DUPLICATED OR COPIED IN ANY FORM WITHOUT PRIOR WRITTEN CONSENT OF

            Finder Graphics Systems, Inc.
            201 Tamal Vista Blvd
            Corte Madera, CA  USA 94925
            (415) 927-0100

(The above notice does not constitute an admission of publication or
unrestricted dissemination of the work)

******************************************************************************/

#include "esi_nl_int.h"

/* **************************************************************** */

publicdef INT nlz_update_valid_rows (nlist, nrows, nCol, Col_List, Var_List)

/* %S a help routine to update the # of valid
    (non-null) rows for each specified
    column in current table         */

NLIST_HEADER nlist;
UINT nrows;
UINT nCol;
UINT *Col_List;
VOIDPTR *Var_List;
    {
    register UINT i, j, k;
    DOUBLE value;
    TABLE_HEADER *ptable = nlist->current_table_ptr;
    BYTE *data_ptr;
    register COLUMN_HEADER *pinfo;
    register DATA_HEADER *pdata;
    BOOL skip_it;

    for (i = 0; i < nlist -> nCol; i++)
        {
        pinfo = nlist->info_ptr + i;
        for (j = 0, skip_it = TRUE; j < nCol; j++)
            {
            if (Col_List[j] EQUALS i + 1 OR pinfo -> inc_value)
                {
                skip_it = FALSE;
                break;
                }
            }
        if (skip_it)
            {
            continue;
            }
        pdata = ptable->data_ptr + i;
        data_ptr = (BYTE *)Var_List[j];
            
        for (k = 0; k < nrows; k++)
            {
            if (pinfo->inc_value)
                /* This column is INCREMENTING, so all  */
                /* rows are valid           */
                {
                pdata->n_valid_rows++;
                pinfo->n_valid_rows++;
                value = pdata->start_value + pdata->increment * (DOUBLE)k;
                }
            else
                {
                switch (pinfo->data_type)
                    {
                case BOOL_TYPE:
                case INT_TYPE:
                    switch (pinfo->data_size)
                        {
                    case sizeof(CHAR):
                        value = (DOUBLE)*data_ptr;
                        data_ptr += sizeof(CHAR);
                        break;

                    case sizeof(SHORT):
                        value = (DOUBLE)*(SHORT *)data_ptr;
                        data_ptr += sizeof(SHORT);
                        break;

                    case sizeof(INT):
                        value = (DOUBLE)*(INT *)data_ptr;
                        data_ptr += sizeof(INT);
                        break;
                    default:
                        return NL_INVALID_DATA_SIZE;
                        }
                    break;

                case FLOAT_TYPE:
                    switch (pinfo->data_size)
                        {
                    case sizeof(FLOAT):
                        value = (DOUBLE)*(FLOAT *)data_ptr;
                        data_ptr += sizeof(FLOAT);
                        break;

                    case sizeof(DOUBLE):
                        value = *(DOUBLE *)data_ptr;
                        data_ptr += sizeof(DOUBLE);
                        break;

                    default:
                        return NL_INVALID_DATA_SIZE;
                        }
                    break;
                case CHAR_TYPE:
                    continue;
                default:
                    return NL_INVALID_DATA_TYPE;

                    }

                /* See if the value is the NULL value */
                skip_it = FALSE;
                if (pinfo->null_value < 0.0)
                    {
                    if (value <= pinfo->null_value * 0.9999)
                        {
                        skip_it = value >= pinfo->null_value * 1.0001;
                        }
                    }
                else
                    {
                    if (value <= pinfo->null_value * 1.0001)
                        {
                        skip_it = (value >= pinfo->null_value * 0.9999);
                        }
                    }
                if (skip_it)
                    {
                    continue;
                    }
                else
            /* Value is not null, so increment # of */
                    /* rows in this column */
                    {
                    pdata->n_valid_rows++;
                    pinfo->n_valid_rows++;
                    }
                }

            /* Perform min/max tests */
            if (value < pdata->min)
                {
                pdata->min = value;
                if (value < pinfo->min)
                    {
                    pinfo->min = value;
                    }
                }
            if (value > pdata->max)
                {
                pdata->max = value;
                if (value > pinfo->max)
                    {
                    pinfo->max = value;
                    }
                }
            /* Statistics are now computed */
            pdata->sum_x += value;
            pinfo->sum_x += value;
            value *= value;
            pdata->sum_x_squared += value;
            pinfo->sum_x_squared += value;
            }
        }
    return SUCCESS;
    }
