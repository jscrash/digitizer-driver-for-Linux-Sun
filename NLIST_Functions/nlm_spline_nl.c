/* DEC/CMS REPLACEMENT HISTORY, Element NLM_SPLINE_NL.C*/
/* *10    8-JAN-1991 15:33:20 VINCE "(SPR 6281) restored initialization of nrows"*/
/* *9    23-OCT-1990 11:56:53 GILLESPIE "(SPR 1) Use nlz_copy_structure"*/
/* *8    17-MAR-1990 13:01:13 JULIAN "(SPR 0) undo last mods"*/
/* *7    16-MAR-1990 11:23:51 JULIAN "(SPR 5135) add nl_strip_points"*/
/* *6    26-FEB-1990 19:00:18 JULIAN "(SPR 6010) Port to Decstation 3100 (ULTRIX)"*/
/* *5    26-JAN-1990 14:27:36 WALTERS "(SPR 0) Fix bug in nl_set_nlist_info"*/
/* *4    17-JAN-1990 15:36:37 WALTERS "(SPR 0) Add nl_init_nlist_info to name dimensions"*/
/* *3    28-NOV-1989 14:54:50 GORDON "(SPR 0) Fix call to nlz_anti_alias_filter"*/
/* *2     9-NOV-1989 13:33:31 GORDON "(SPR 5058) New version of spline nlist resampling"*/
/* *1     9-NOV-1989 13:32:13 GORDON "New spline resampling routine"*/
/* DEC/CMS REPLACEMENT HISTORY, Element NLM_SPLINE_NL.C*/
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
#include "esi_nl_meth_hdr.h"
#include "esi_ho.h"
#include "esi_tc.h"

/* ********************************************************************	*/
/*									*/
/*    ROUTINE: NLM_SPLINE_NLIST						*/
/*									*/
/*    DESCRIPTION A type of resampling.  To spline fit to data.		*/
/*									*/
/* ******************************************************************** */

#if USE_PROTOTYPES
publicdef INT nlm_spline_nlist (NLIST_HEADER nlist, VOIDPTR param)
#else
publicdef INT nlm_spline_nlist (nlist, param)
NLIST_HEADER nlist;		    /* the pointer given N-list.	*/
VOIDPTR param;
#endif
{
    NLM_RESAMPLE_HEADER *p = (NLM_RESAMPLE_HEADER *)param;
    INT i, j;			    /*	 indexes.  the result status.	*/
    INT status;			    /*	 table number.  total rows	*/
    UINT table_num;		    /*	 will be added to the table.	*/
    UINT total_rows;		    /*	 the start value of table of	*/
    DOUBLE startValue;		    /*	 independent variable column	*/
				    /*	 of output N-list.  an array	*/
    size_t *dataSize;		    /*	 of data size of specified	*/
    INT *dataType;		    /*	 p->ncol.  an array of data	*/
    INT *units;			    /*	 type of specified p->ncol.	*/
    INT *incValue;		    /*	 an array of unit of specified	*/
    DOUBLE **Var_List;		    /*	 columns an array of		*/
				    /*	 INCREMENT_LIST or VALUE_LIST	*/
    UINT *newDimList;		    /*	 the array of pointer of each	*/
    COLUMN_HEADER *pinfo;	    /*	 column's data.  the new	*/
				    /*	 column list for output N-list	*/
				    /*	 workspace of COLUMN_HEADER.	*/
    COLUMN_HEADER *poldinfo;
    INT dimsize;
    TABLE_HEADER *poldtab, *pnewtab;
    NLIST_HEADER pnew;
    UINT *Col_List;
    UINT nrows = 0;
    
    status = SUCCESS;

    /*	 error checking.						*/

    INIT_TEST (nlist);
    
    for (i = 0; i < p->ncol; i++)
        {
        COLUMN_TEST (nlist, p->Col_List[i]);
        }

    if (nlist->total_rows EQUALS 0)
        {
        return NL_EMPTY_NLIST;
        }

    /* Apply anti-aliasing filter first, if appropriate.		*/

    if (p->filterLen > 0)
        {
        status = nlz_anti_alias_filter (nlist, p->ncol, p->Col_List,
					p->resampleInc, p->filterLen);
        }

    /*	 allocate memory for initializing N-list			*/
    
    dimsize = sizeof(INT) * p->ncol;
    dataSize = (size_t *) tc_alloc (sizeof(size_t) * p->ncol);
    dataType = (INT *) tc_alloc (dimsize);
    units = (INT *) tc_alloc (dimsize);
    incValue = (INT *) tc_alloc (dimsize);
    Var_List = (DOUBLE * *) tc_alloc (sizeof(DOUBLE *)*p->ncol);
    newDimList = (UINT *) tc_alloc (sizeof(UINT) * p->ncol);
    Col_List = (UINT *) tc_alloc (sizeof(UINT) * p->ncol);
    
    for (i = 0; i < p->ncol; i++)
        {
        newDimList[i] = (UINT)i + 1;
        pinfo = nlist->info_ptr + p->Col_List[i] - 1;
        dataSize[i] = pinfo->data_size;
        dataType[i] = pinfo->data_type;
        units[i] = pinfo->unit;
        incValue[i] = (pinfo->inc_value) ? INCREMENT_LIST : VALUE_LIST;
        Col_List[i] = i + 1;
        }

    /*	 the first column of returned N-list will be INCREMENT_LIST.	*/
    
    incValue[0] = INCREMENT_LIST;
    nl_init_nlist (&pnew, p->ncol, units, dataSize, dataType, incValue,
                   nlist->blocksize);
    p->outNlist = pnew;

    /* preserve the stats deferral flag					*/
    p->outNlist->defer_stats = nlist->defer_stats;
    
    for (j = 0, poldinfo = nlist->info_ptr, pinfo = pnew->info_ptr; j < p->ncol;
         j++, pinfo++, poldinfo++)
        {
        dataSize[j] = sizeof(DOUBLE);
        dataType[j] = FLOAT_TYPE;
        strcpy ((char *)pinfo->column_name, (char *)poldinfo->column_name);
        }

    /* spline each table.						*/
    for (i = 0; i < nlist->total_tables; i++)
        {
	/*   find out the start value of the indexed column and the	*/
	/*   total number of rows will be added to the output N-list.	*/
        
        status = nlz_find_start_and_total (nlist,
					   i + 1,
					   p->Col_List[0],
                                           p->resampleInc,
					   &startValue,
					   &total_rows);
        
        if (status != SUCCESS)
            {
            nl_start_table (pnew, &table_num);
            continue;
            }

	/*   create a new table in the output N-list and set the start	*/
	/*   value and increment.					*/
        
        nl_start_table (pnew, &table_num);
        pnew->current_table_ptr->data_ptr->start_value = startValue;
        pnew->current_table_ptr->data_ptr->increment = p->resampleInc;
        
	/* allocate more memory if needed				*/
        
        if (total_rows > nrows)
            {
            for (j = 0; j < p->ncol; j++)
                {
                if (nrows > 0)
                    {
                    tc_free (Var_List[j]);
                    }
                Var_List[j] = (DOUBLE *) tc_alloc (sizeof(DOUBLE) * total_rows);
                }
            nrows = total_rows;
            }

	/*   fill the splined data in the buffer.			*/
        
        status = nlz_fill_data_array (nlist, i + 1, p->ncol, p->Col_List,
				      startValue, p->resampleInc, total_rows,
				      Var_List);
        
        if (status IS_NOT_EQUAL_TO SUCCESS)
            continue;
        
	/*   add the rows to the output N-list.				*/
        status = nl_add_rows_convert (pnew, total_rows, p->ncol, newDimList,
                                      (VOIDPTR *)Var_List, units, dataSize,
				      dataType);
        
        if (status)
            {
            break;
            }
        }

    /*	 free all allocated memory.					*/
    
    for (j = 0; j < p->ncol; j++)
        {
        tc_free ((VOIDPTR)Var_List[j]);
        }

    tc_free ((VOIDPTR)dataType);
    tc_free ((VOIDPTR)units);
    tc_free ((VOIDPTR)incValue);
    tc_free ((VOIDPTR)Var_List);
    tc_free ((VOIDPTR)dataSize);
    tc_free ((VOIDPTR)newDimList);
    
    /*	 copy the information in the N-list header			*/
    /*									*/
    /*	 * copy a user structure, if needed				*/
    /*									*/

    if (nlist->user_struct_len > 0)
        {
        status = nlz_copy_structure (nlist->user_struct,
				     nlist->user_struct_len,
                                     &pnew->user_struct,
				     &pnew->user_struct_len);
        if (status IS_NOT_EQUAL_TO SUCCESS)
            {
            return status;
            }
        }

    /*									*/
    /*	 * copy the n-List name, if needed				*/
    /*									*/

    if (nlist->name IS_NOT_EQUAL_TO NULL)
        {
        pnew->name = STRING_ALLOCATE (nlist->name);
        strcpy (pnew->name, nlist->name);
        }

    if (nlist->next_nlist IS_NOT_EQUAL_TO NULL)
        {
        pnew->next_nlist = STRING_ALLOCATE (nlist->next_nlist);
        strcpy (pnew->next_nlist, nlist->next_nlist);
        }

    if (nlist->prev_nlist IS_NOT_EQUAL_TO NULL)
        {
        pnew->prev_nlist = STRING_ALLOCATE (nlist->prev_nlist);
        strcpy (pnew->prev_nlist, nlist->prev_nlist);
        }

    hoblockmove (nlist->creation_date, pnew->creation_date, sizeof(DATE));

    /*									*/
    /*	 * now copy the tables						*/
    /*									*/
    
    for (i = 0, poldtab = nlz_set_table (nlist, i + 1),
         pnewtab = nlz_set_table (pnew, i + 1);
         i < nlist->total_tables AND poldtab != NULL_TABLE;
         i++, poldtab = nlz_set_table (nlist, i + 1),
         pnewtab = nlz_set_table (pnew, i + 1))
        {
        
        /*								*/
	/*   * copy the table header and column data headers		*/
	/*								*/
        status = nlz_copy_structure (poldtab->user_struct,
				     poldtab->user_struct_len,
                                     &pnewtab->user_struct,
				     &pnewtab->user_struct_len);
        if (status IS_NOT_EQUAL_TO SUCCESS)
            {
            return status;
            }

        /*								*/
	/*   * copy the table name, if needed				*/
	/*								*/
        if (poldtab->name IS_NOT_EQUAL_TO NULL)
            {
            pnewtab->name = STRING_ALLOCATE (poldtab->name);
            strcpy (pnewtab->name, poldtab->name);
            }

        pnewtab->totalBytesInTable = poldtab->totalBytesInTable;        
        }

    return(status < SUCCESS) ? status : SUCCESS;
}
