/* DEC/CMS REPLACEMENT HISTORY, Element LG_LTDM_NLT.C*/
/* *7    17-JUL-1991 16:28:38 JANTINA "(SPR 0) Recoded lgz_extract_stroke"*/
/* *6    13-OCT-1989 09:51:41 GORDON "(SPR 5017) fix lgz_merge_tables to set nallocated properly"*/
/* *5    25-SEP-1989 17:17:19 GILLESPIE "(SPR 200) Additional changes for new n-List V4"*/
/* *4    25-SEP-1989 10:45:32 GILLESPIE "(SPR 200) Changes for new n-List V4.0"*/
/* *3    11-AUG-1989 12:30:10 JULIAN "(SPR -1) function lgz_extract_stroke has return(e) and return;"*/
/* *2    27-JUL-1989 23:41:14 GILLESPIE "(SPR 1) Fix minor casting problems"*/
/* *1    19-JUN-1989 13:05:10 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element LG_LTDM_NLT.C*/

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

/* LG_LTDM_NL_TOOLS.CC,  JAMES>LTDM>SOURCE,  JAMES CHANG,  11/17/86
   A module of N-list routines for Log Trace Data Manager.
   Copyright (c) 1986, Exploration Systems Inc, Sausalito, CA. */

/* TITLE : LG_LTDM_NL_TOOLS - A module of N-list routines for Log Trace Data Manager. */

/* START-HISTORY :
   11/17/86 JAMES    releases LG_LTDM_GET_TRACE().
   11/15/86 JAMES    modify lgz_merge_strokes().
   11/13/86 JAMES    The first release.
   10/17/86 JAMES CHANG Template constructed
   END-HISTORY */

/* START-CODE : */

#include "esi_c_lib.h"
#include "esi_nl_int.h"
#include "esi_tc.h"
#include "esi_lg_defs.h"
#include "esi_lg_err.h"


/* ******************************************************************************
    ROUTINE:  LGZ_GET_TRACE

    DESCRIPTION
         A internal help routine of Log Trace Data Manager.  This routine performs
         getting a trace from a N-list, it will accrod to the given FILENAME and
         NLISTNAME to read a N-list from Data Base, then uses the specified TOP
         and BASE range to extract the N-list.

    ASSUMPTION
         The indexed dimension must be increment.  If the indexed dimension is
         not increment, will call NL_RESAMPLE_NLIST() first to convert it.
         If the input top and base are NULL, the returns the whole trace.

    *************************************************************************** */
publicdef INT lgz_get_trace(nlist, filename, nlistName, dim, top, base, non_null_top,
                               non_null_base, multi_tables, non_append_null)
    /***************************************************************************/

NLIST_HEADER *nlist;             /*%P the returned pointer of the pointer of N-LIST */
FILENAME filename;                /*%p the given FILENAME and NLISTNAME for    */
NLIST_ID  nlistName;              /*   reading the N-list from Data Base.      */
INT dim;                          /*%P the specified dimension in the N-list,  */
                                  /*   which will be extracted.                */
DOUBLE top, base;                 /*%P the given range of extracting.          */
FLOAT *non_null_top, *non_null_base;
INT multi_tables;
INT non_append_null;
{
    NLIST_HEADER wholeTrace;     /*   workspace of whole N-list read back from D.b*/
    NLIST_HEADER multitablesTrace;
    INT i;
    UINT  tabNum = 0, nCol;      /*   indexes and temperary workspaces.       */
    INT status;
    BOOL found;
    INT *DataSize, *DataType;     /*   worksapce for initializing a N-list.    */
    INT get_non_null_top = TRUE;
    INT *Units, *IncValue;
    UINT dim_list[1], tempPts = 0;
    VOIDPTR value_list[1], bytePtr = (VOIDPTR)0;
    FLOAT dummy;
    DOUBLE start_value, increment;/*   strat value and increment of computation.*/
    TABLE_HEADER *ptab;          /*   workspace of current table.            */
    COLUMN_HEADER *pinfo;      /*   workspace of current COLUMN_HEADER.  */
    DATA_HEADER *pdata;           /*   workspace of current DATA_HEADER.       */

/* ***************************************************************************** */
                                  /* read the whole trace.              */

    status = nl_read_nlist(&wholeTrace, filename, nlistName);
    if(   status  )
         return status;


                                  /* initialized the returned nlist.           */

    nCol = wholeTrace -> nCol;

    for( i = 0, pinfo = wholeTrace -> info_ptr;
         i < nCol;
         i++, pinfo++)
    {
         if( ( i + 1) EQUALS dim)
         {
              bytePtr = (VOIDPTR) TALLOC(  pinfo -> data_size + 1);
         }
    }
    status = nl_clone_nlist(wholeTrace, &multitablesTrace);
    if(  status IS_NOT_EQUAL_TO SUCCESS   )
         return status;


                                  /*   go throught all tables to find qualified*/
                                  /*   tables.                                 */

    dim_list[0] = dim;
    for( i = 0, found = FALSE;
         i < wholeTrace -> total_tables;
         i++)
    {
         ptab = nlz_set_table(wholeTrace, i + 1);
         pdata = ptab -> data_ptr + dim -1;

                                  /* error checking for the index dimension.  */

         if( !  pdata -> inc_value  )
         {
              nl_free_nlist( wholeTrace   );
              nl_free_nlist( multitablesTrace );
              return (NL_INVALID_COLUMN  );
         }
                                 /*   looking for the qualified table.        */

         if(  pdata -> start_value > base OR
              pdata -> start_value + pdata -> increment * ptab -> nrows < top )
             continue;
         nl_copy_table(wholeTrace, i+ 1, multitablesTrace, &tabNum);
         
				/* If retrieve the non top base...          */

         if( get_non_null_top )
	 {
		get_non_null_top = FALSE;
                lgz_extract_stroke(multitablesTrace, tabNum, dim, top, base,
						non_null_top, non_null_base);
         }
	 else
	 {
                lgz_extract_stroke(multitablesTrace, tabNum, dim, top, base,
						&dummy, non_null_base);
         }
         if( pdata -> start_value  < top )
         {
              start_value = pdata -> start_value;
              increment = pdata -> increment;
              tempPts = (top - start_value ) / increment;
              start_value += tempPts * increment;

/*	      start_value = ( start_value < top ) ? start_value + increment :
						    start_value;  */

	      if(ABS(top - start_value) > (increment/2)) 
			start_value = start_value + increment;

                                  /*   according to dataType, cased the start  */
                                  /*   value.                                  */

              pinfo = multitablesTrace -> info_ptr + dim -1;
              switch(  pinfo -> data_type  )
              {
                   case INT_TYPE :
                        switch( pinfo -> data_size  )
                        {
                             case sizeof(INT ):
                                  *(INT *)bytePtr = start_value;
                                  break;
                             case sizeof(SHORT):
                                  *(SHORT *) bytePtr = start_value;
                                  break;
                             case sizeof(CHAR ):
                                  break;
                        }
                        break;
                   case FLOAT_TYPE :
                        switch(  pinfo -> data_size  )
                        {
                             case sizeof(FLOAT):
                                  *(FLOAT *)bytePtr = start_value;
                                  break;

                             case sizeof(DOUBLE):
                                  *(DOUBLE *)bytePtr = start_value;
                        }
                        break;
              }
              value_list[0] = bytePtr;
              nl_set_table_info(multitablesTrace, tabNum, NL_TABLE_START_VALUE,
                                 1, dim_list, value_list);
         }
         found = TRUE;
    }
    status = ( found ) ? SUCCESS : FAIL;

                                  /*   if there are qualified tables, call help*/
                                  /*   routine to merge all tables to be one  */
                                  /*   single table.                          */
    if( found AND 
	(multi_tables AND non_append_null ) OR tabNum EQUALS 1 )
    {    
	*nlist = multitablesTrace;
	multitablesTrace -> updated = TRUE;
	return status;
    }
    else
    {
	 multitablesTrace -> updated = TRUE;
         status = lgz_merge_strokes(multitablesTrace, dim, top, base, 
    					nlist);
    }
    nl_free_nlist(multitablesTrace);



                                  /*   frees all temporary nlists.    */
#if NEED_TO_FREE
    if (bytePtr != (VOIDPTR)0)
	tc_free(bytePtr);
#endif

    return status;
}



/* *****************************************************************************

    ROUTINE:  LGZ_PUT_TRACE

    DESCRIPTION
         To put a segment of trace to a existing trace.  If the target whole
         trace does not exist, create a new trace.  If the specified range of
         segment does not exist append it, otherwise replaces the segment.
         The input NLIST is a single table nlist, which has been converted in
         PUT_TRACE(), if it is multi-table Nlist.

    ASSUMPTION
         The input NLIST is a single table nlist, which has been converted in
         PUT_TRACE(), if it is multi-table Nlist.

    *************************************************************************** */
publicdef INT lgz_put_trace(nlist, filename, nlistName, dim, top, base)
    /***************************************************************************/

NLIST_HEADER nlist;              /*%P The input new N-list.                   */
FILENAME filename;                /*%P The given FILENAME and NLISTNAME of     */
NLIST_ID nlistName;               /*%P destination N-list.                     */
INT dim;                          /*%P the indexed dimension.                  */
DOUBLE top, base;                 /*%P the range of updated rows.            */
{
    NLIST_HEADER wholeTrace;     /*   the workspace of whole destination N-list*/
    INT status;
    INT i, j;                     /*   indexes.                                */

/* ***************************************************************************** */

                                  /*   read the destination N-list.  If the    */
                                  /*   specified destination N-list does not   */
                                  /*   exist, create a new one then returns.   */

    status = nl_read_nlist(&wholeTrace, filename, nlistName);
    if(  status EQUALS NL_NLIST_NOT_FOUND OR status EQUALS NL_FILE_NOT_FOUND  )
	{
         return nl_write_nlist(nlist, filename, nlistName);
	}

                                       /*   go througth all tables, finds the */
                                       /*   qualified candidate and updates    */
                                       /*   those rows are in the specified  */
                                       /*   range.                             */

    for( i = 0;
         i < wholeTrace -> nCol;
         i++)
    {
         nlz_set_table(wholeTrace, i + 1);
         lgz_update_stroke(wholeTrace, nlist, dim, top, base);
    }
    status = nl_update_nlist( wholeTrace);
    return status;
}


/* ******************************************************************************

    ROUTINE:  LGZ_MERGE_TABLES

    DESCRIPTION
         An internal help routine.  This routine performs the
         merging of tables, according to the given range( FROM, TO).  If the
         given N-list is totally empty (e.g. no table at all), then creates a
         new empty table and returns.  If there is no table in the range, then
         returns a table of NULL values.

         This routine will insert and append NULL values to make the candidate
         table really start from FROM and end at TO.

    *************************************************************************** */
publicdef INT lgz_merge_strokes(nlist, dim, from, to, outNlist)
    /* *********************************************************************** */
NLIST_HEADER nlist;               /* The given N-list.                       */
NLIST_HEADER *outNlist;           /* The returned n-list.                    */
DOUBLE from, to;                  /* The given range.                        */
INT dim;                          /* indexed dimension number.               */
{
    TABLE_HEADER *ptab;           /*   workspace of the current table.        */
    TABLE_HEADER *pnext;          /*   workspace of the next table of current.*/
    BOOL foundFirst = FALSE;      /*   the flag of finding the first qualified */
                                  /*   table.                                 */
    BOOL foundLast = FALSE;       /*   the flag of finding the last qualified  */
                                  /*   table.                                 */
    BOOL firstAdd = TRUE;         /*   to add the first row.                */
    INT status;
    UINT tableIndex = 1;          /*   the index number of current table.     */
    UINT rowIndex = 0;            /*   the index number of current row.      */
    INT nullrows = 0;             /*   the number of Null rows added.         */
    DOUBLE minimum, maximum;      /*   the minimum and maximum values of the     */
                                  /*   indexed dimension of current table.    */
    DOUBLE newFrom;               /*   the relative start value of range.      */
    DOUBLE increment;             /*   the increment of the tables.           */
    UINT nCol;
    INT  *units;
    size_t *dataSize;  /*   workspace of data retrieving.           */
    INT *dataType;
    UINT nrows, *dimList, nallocated;
    INT i, *incValue;
    FLOAT **valueList;
    DATA_HEADER *oldPdata, *newPdata;
    COLUMN_HEADER *pinfo;
/* *************************************************************************** */
    if(  nlist -> total_tables EQUALS 0)
         return NL_EMPTY_NLIST;

                                  /*   initializes the returned N-list as same */
                                  /*   format as the given N-list.             */

                                  /* initialized the returned nlist.           */

    nCol = nlist -> nCol;

    status = nl_clone_nlist(nlist, outNlist);
    if(  status IS_NOT_EQUAL_TO SUCCESS   )
         return status;

    nl_start_table(*outNlist, &tableIndex);

                                  /*   initializes workspace of data retrieving*/

    nCol = nlist -> nCol;
    units = (INT *) TALLOC( sizeof(INT ) * nCol);
    dataSize = (size_t *) TALLOC( sizeof(size_t) * nCol);
    dataType = (INT *) TALLOC( sizeof(INT ) * nCol);
    dimList = (UINT *) TALLOC( sizeof(UINT ) * nCol);
    valueList = (FLOAT **) TALLOC( sizeof(FLOAT *) * nCol);

    for( i = 0;
         i < nCol;
         i++)
    {
         units[i] = 0;
         dataSize[i] = sizeof(FLOAT);
         dataType[i] = FLOAT_TYPE;
         dimList[i] = i + 1;
    }


                                  /*   go througth all dimensions to set the   */
                                  /*   start value and increment, when the     */
                                  /*   dimension is INCREMENT_LIST             */

    for( i = 0, nlz_set_table(nlist, 1), 
		oldPdata = nlist -> current_table_ptr-> data_ptr,
                newPdata = (*outNlist) -> current_table_ptr -> data_ptr;
         i < nCol;
         i++, oldPdata++, newPdata++)
    {
         if(  oldPdata -> inc_value    )
         {
              newPdata -> increment = oldPdata -> increment;
              newPdata -> start_value = oldPdata -> start_value;
         }
    }
                                  /*   go through all tables to check that the    */
                                  /*   current table is inside the given range*/
                                  /*   for the first qualified table, adds    */
                                  /*   nulls to the front, if it is necessary. */
                                  /*   for the last qualified table, appends  */
                                  /*   the nulls to the end, if it is necessary*/

    for( tableIndex = 1, status = nl_set_current_row(nlist, 1, tableIndex),
         newFrom = from;
         tableIndex <= nlist -> total_tables;
         tableIndex++)
        {
        if(  status < 0 )
            {
            if(  tableIndex + 1 <= nlist -> total_tables )
               status = nl_set_current_row(nlist, 1, tableIndex + 1);
               continue;
             }
         ptab = nlist -> current_table_ptr;
         minimum = (ptab -> data_ptr + dim - 1) -> start_value;
         increment = (ptab -> data_ptr + dim - 1) -> increment;
         maximum = minimum + increment * ( ptab -> nrows - 1);
                                  /*   find the first qualified row.         */

        rowIndex  = (newFrom > minimum  ) ? 
                            ( newFrom - minimum )/increment + 1 : 1;
        minimum = minimum + ( rowIndex - 1) * increment;

                                  /*   allocates workspace for the current     */
                                  /*   table.  nallocated is the number of     */
				  /*   rows from current table we will use.    */
         nallocated = 0;    
         if( to >=  minimum ) 
	    {
	    nallocated = ( to - minimum ) / increment + 1;
            nallocated = ( rowIndex + nallocated > ptab -> nrows) ?
                 ptab->nrows - (rowIndex-1) : nallocated;
	    }

         if( nallocated EQUALS 0 )
              continue;

         for( i = 0; i < nCol; i++)
              valueList[i] = (FLOAT*)TALLOC(sizeof(FLOAT) * nallocated);

                                  /*   adds rows in the current table to the*/
                                  /*   returned N-list.                        */
         status = nl_set_current_row(nlist, rowIndex, tableIndex);
         status = nl_get_rows_convert(nlist, nallocated, nCol, dimList,
             (VOIDPTR *)valueList, units, dataSize, dataType, &nrows);

                                  /*   checks and adds NULLs in the front of   */
                                  /*   table, if it is necessary.             */

         nullrows = (minimum - newFrom) / increment;
         if(  nullrows > 0 )
	    {
            if(  firstAdd )
                {

 				  /*   if need to add null value in the front
                                       of first row, decrease the start value.*/

                for( i = 0, newPdata=(*outNlist)->current_table_ptr->data_ptr;
                   i < nCol;
                   i++, newPdata++)
                    {
                    if(  newPdata -> inc_value    )
                        newPdata -> start_value -= increment * nullrows;
                    }
                lgz_add_nulls(*outNlist, nullrows, FALSE);
             }
             else
                lgz_add_nulls(*outNlist, nullrows, TRUE);
         }
         status = nl_add_rows_convert(*outNlist, nrows, nCol, dimList, 
			   (VOIDPTR *)valueList, units, dataSize, dataType);
         firstAdd = FALSE;
                                  /*   frees allocated workspace of current    */
                                  /*   tables.                                */
#if NEED_TO_FREE
        for( i = 0; i < nCol; i++)
            tc_free(   valueList[i] );
#endif


                                  /*   if the current table is the last table*/
                                  /*   checks and appends the NULLS to the end */
                                  /*   of the table, if it is necessary.      */

         if(  tableIndex  EQUALS  nlist -> total_tables)
             {
             nullrows = (to - maximum) / increment;
             if(  nullrows > 0)
                  lgz_add_nulls( *outNlist, nullrows, FALSE);
             }

         newFrom = maximum + increment;
         if(  tableIndex + 1 <= nlist -> total_tables )
              status = nl_set_current_row(nlist, 1, tableIndex + 1);
    }

#if NEED_TO_FREE
                                  /*   frees all allocated memory.             */
    tc_free(     units     );
    tc_free(     dataSize  );
    tc_free(     dataType  );
    tc_free(     dimList   );
    tc_free(     valueList );
#endif
    return SUCCESS;
}

/* ******************************************************************************

    ROUTINE:  LGZ_EXTRACT_STROKE

    DESCRIPTIO
         A internal help routine of extracting table.

    ASSUMPTION
         there is at least one row is inside the range.

    *************************************************************************** */
publicdef INT lgz_extract_stroke(nlist, tabNum, dim, from, to, 
                                                 non_null_top, non_null_base)
    /***************************************************************************/

NLIST_HEADER nlist;              /*%P the given pointer of N-LIST.            */
INT tabNum;                       /*%P the specified table.                   */
INT dim;                          /*%P the indexed dimension.                  */
DOUBLE from, to;                  /*%P the extracting range( FROM, TO).        */
FLOAT *non_null_top, *non_null_base;
{
    DATA_HEADER *pdata;           /*   workspace of current DATA_HEADER.       */
    TABLE_HEADER *ptab;          /*   workspace of current table.            */
    INT startrow = 0, endrow = 0; /*  indexes of rows.               */
    DOUBLE startValue, increment; /*   computation variables.                  */
    INT status;

/* ***************************************************************************** */

    if(  from EQUALS 0.0 AND to EQUALS 0.0)
         return;
                                  /* set the specified table.                 */

    status = nl_set_current_row(nlist, 1, tabNum );
    if(  status < SUCCESS )
         return status ;

                                  /*   set the current data, startValue and    */
                                  /*   increment.                              */

    ptab = nlist -> current_table_ptr;
    pdata = ptab -> data_ptr + dim -1;
    startValue = pdata -> start_value;
    increment = pdata -> increment;

                                  /*   to find the first qualified row and   */
                                  /*   the last row.                         */
    if(from < startValue)
        startrow = 1;
    else
    {
        startrow = (from - startValue)/increment + 1;
        if(ABS((startValue + (startrow - 1)*increment) - from) > (increment/2))
            startrow++;
    }
    *non_null_top = startValue + (startrow - 1)*increment;

    endrow = (to - startValue)/increment + 1;
    if(ABS((startValue + (endrow - 1)*increment) - to) > (increment/2))
        endrow++;
    if(endrow > ptab->nrows)
        endrow = ptab->nrows;
    *non_null_base = startValue + (endrow - 1)*increment;

    /*   delete the tail part unqualified rows.*/

    status = nl_set_current_row(nlist, endrow + 1, tabNum);
    if(  ptab -> nrows - endrow > 0  AND status > 0)
         nl_delete_rows(nlist, ptab -> nrows - endrow);


    /*  delete the front unqualified rows.*/

    nl_set_current_row(nlist, 1, tabNum);
    if(  startrow   >  1 )
         nl_delete_rows(nlist,startrow-1);
    status = SUCCESS;

    return status;

}


/* ******************************************************************************

    ROUTINE: LGZ_ADD_NULLS

    DESCRIPTION
         A help routines to add N null value rows before/after current row.

   **************************************************************************** */
publicdef INT lgz_add_nulls(nlist, nrows, beforeCp )
    /***************************************************************************/

NLIST_HEADER nlist;              /*%P The destination N-list.                 */
INT beforeCp;                     /*%P flag of indicating add before C.P. or   */
                                  /*%P after C.P.                              */
INT nrows;                      /*%P the total number of rows will be added*/

{
    UINT *dimList, nCol = nlist -> nCol, i, j;
    COLUMN_HEADER *pinfo;      /*   worlspace of current COLUMN_HEADER.  */
    DOUBLE **valueList;           /*   an array of pointers of DOUBLE type     */
                                  /*   NULL values.                            */
    size_t *dataSize;                /*   data type of converting.                */
    INT *dataType;                /*   data size of converting.                */
    INT *units;                   /*   unit of measure array.                  */

/* ***************************************************************************** */

                                  /*   allocates the workspace.                */

    valueList = (DOUBLE **) TALLOC( sizeof( DOUBLE *) * (nCol + 1) );
    dimList = (UINT *) TALLOC( sizeof( UINT ) * ( nCol + 1) );

                                  /*   set the data size, data type, NULL value*/
                                  /*   array for each dimension.               */

    dataSize = (size_t *) TALLOC( sizeof(size_t) * (nCol + 1));
    dataType = (INT *) TALLOC( sizeof(INT ) * (nCol + 1));
    units = (INT *) TALLOC( sizeof(INT ) * (nCol + 1));
    for( i = 0;
         i < nCol;
         i++)
    {
         pinfo = nlist -> info_ptr + i;
         dimList[i] = i + 1;
         dataSize[i] = sizeof(DOUBLE );
         dataType[i] = FLOAT_TYPE;
         units[i] = 0;
         valueList[i] = (DOUBLE *) TALLOC( sizeof(DOUBLE) * nrows);

         for( j = 0;
              j < nrows;
              j++)
              valueList[i][j] = pinfo -> null_value;
    }

                                  /*   add NULL values array before/after C.P. */

    if(  beforeCp  )
         nl_insert_rows_convert(nlist, nrows, nCol, dimList, (VOIDPTR *)valueList,
                                               units, dataSize, dataType);
    else
         nl_add_rows_convert(nlist, nrows, nCol, dimList,
				(VOIDPTR *)valueList,
                                               units, dataSize, dataType );

#if NEED_TO_FREE
                                  /*   free all allocated memory.              */
   for( i = 0;
        i < nCol ;
        i++)
        tc_free( valueList[i]);

   tc_free( dimList );
   tc_free( dataSize);
   tc_free( dataType);
   tc_free( units );
   tc_free(valueList );
#endif

   return SUCCESS;

}


/* ******************************************************************************

    ROUTINE:  LGZ_UPDATE_STROKE

    DESCRIPTION
         A internal help routine of PUT_TRACE().  This routine performs the
         replacing the spcified range in the oldNlist by the newNlist.

    ASSUMPTION
         the incremnet value of indexed dimension of both N-lists must be same.

    *************************************************************************** */
publicdef INT lgz_update_stroke(oldNlist, newNlist, dim, from, to)
    /***************************************************************************/

NLIST_HEADER oldNlist;           /*%P the pointer of N-list contains old values */
NLIST_HEADER newNlist;           /*%P the pointer of N-list contains old values */
INT dim;                          /*%P the indexed dimension.                  */
DOUBLE from, to;                  /*%P the range of repleacing rows.         */

{
    DATA_HEADER *poldata;         /*   workspace of current DATA_HEADER of old */
                                  /*   N-list.                                 */
    DATA_HEADER *pnewdata;        /*   workspace of current DATA_HEADER of new */
                                  /*   N-list.                                 */
    COLUMN_HEADER *pnewInfo;   /*   workspace of current COLUMN_HEADER of*/
                                  /*   new N-list.                             */
    COLUMN_HEADER *poldInfo;   /*   workspace of current COLUMN_HEADER of*/
                                  /*   new N-list.                             */
    TABLE_HEADER *poldtab;       /*   workspace of pointer of current TABLE_ */
                                  /*   HEADER of old N-list.                   */
    TABLE_HEADER *pnewtab;       /*   workspace of pointer of current TABLE_ */
                                  /*   HEADER of new N-list.                   */
    DOUBLE oldEndValue, newEndValue;/* the last index value of both table.   */
    DOUBLE oldStartValue, newStartValue; /* the first index value of both tables*/
    DOUBLE increment;             /*   calculating variable.                   */
    INT replaceBegin = 0;         /*  the squential number of row to start replaing*/
    INT replacerows = 0;        /*  the total number of rows to be replaced. */
    INT loadBegin = 0;	          /* the squential number of row to start
			     	 loading into array in the new table.	    */
    INT loadrows = 0;           /*  the total number of rows loaded into array.*/
    INT i, indexP = 0, match = FALSE; /* indexes and logical flag.             */
    UINT nCol = newNlist -> nCol, *dimList;
    INT status;
    INT updateType = 0;
    VOIDPTR *valueList;
/**************************************************************************** */

                                  /*   set the current pointers for both old   */
                                  /*   and new N-list.                         */

    poldtab = oldNlist -> current_table_ptr;
    pnewtab = newNlist -> current_table_ptr;
                                  /* if one of tables is empty, return.      */
    if(  poldtab -> nrows EQUALS 0 OR pnewtab -> nrows EQUALS 0 )
         return NL_EMPTY_TABLE;

    poldata = poldtab -> data_ptr + dim -1;
    pnewdata = pnewtab -> data_ptr + dim -1;


                                  /*   if two increment values are not same.   */
                                  /*   return error.                           */

    if(  poldata -> increment IS_NOT_EQUAL_TO pnewdata -> increment )
         return( NL_INVALID_VALUE_INCREMENT  );

                                  /*   set the calculate variables.            */

    oldStartValue = poldata -> start_value;
    newStartValue = pnewdata -> start_value;
    increment = poldata -> increment;
    oldEndValue = oldStartValue + ( poldtab -> nrows - 1) * increment;
    newEndValue = newStartValue + ( pnewtab -> nrows - 1) * increment;

                                  /*   if the current table of old N-list is  */
                                  /*   not inside the range, returns.          */

    if(  oldStartValue > newEndValue OR
         oldEndValue < newStartValue )
         return SUCCESS;

/*
   * If the index value are not matched, call help routine to match the index.
*/
    indexP = 0;

                                  /*   check the index values are match.       */
                                  /*   If not, call routine to set the match   */
                                  /* QQQ NLZ_MATCH_INDEX has not been done.    */

    if(  newStartValue > oldStartValue )
    {
#if 0
******** COMMENDED OUT BY JJC *****************************************

         while( newStartValue != oldStartValue + increment * indexP AND
                indexP < poldtab -> nrows  )
                indexP++;
         if(  indexP EQUALS poldtab -> nrows )
              nlz_match_index(newNlist, oldStartValue);
 ****************************************************************** *
#endif
	 indexP = ( newStartValue - oldStartValue) / increment;
	 if( newStartValue IS_NOT_EQUAL_TO 
                           (oldStartValue + ( increment * indexP) ) )
	 {
	         return(LG_INVALID_START_VALUE  );
	 }
         loadBegin = 0;
         loadrows = pnewtab -> nrows;
         replaceBegin = indexP + 1;
    }
    else
    if(  newStartValue < oldStartValue )
    {
#if 0
/* **********************COMMENDED OUT BY JJC *****************************
         while( oldStartValue > newStartValue + increment * indexP )
              indexP++;
         if( oldStartValue < newStartValue + increment * indexP)
              nlz_match_index(newNlist, oldStartValue);
   ************************************************************************/
#endif
	 indexP = ( oldStartValue - newStartValue) / increment;
	 if( oldStartValue IS_NOT_EQUAL_TO 
                                ( newStartValue + ( increment * indexP) ) )
	 {
	         return(LG_INVALID_START_VALUE  );
	 }
         replaceBegin = 1;
         loadBegin = indexP;
         loadrows = pnewtab -> nrows - loadBegin + 1;
    }
    else
    {
         replaceBegin = 0;
         loadBegin = 0;
         loadrows = pnewtab -> nrows;
     }

				  /*  define the udpateType.            */
    if(  newStartValue EQUALS oldStartValue AND newEndValue EQUALS oldEndValue )
         updateType = 1;
    else
    if(  newStartValue >= oldStartValue AND newEndValue <= oldEndValue  )
         updateType = 2;
    else
    if(  newStartValue >= oldStartValue AND newEndValue >= oldEndValue  )
         updateType = 3;
    else
    if(  newStartValue <= oldStartValue AND newEndValue <= oldEndValue  )
         updateType = 4;
    else
    if(  newStartValue <= oldStartValue AND newEndValue >= oldEndValue  )
         updateType = 5;    
/*
   * replace the specified range.
*/

                                  /*   moves the array to a temporary array.*/

    dimList = (UINT *) TALLOC( sizeof(UINT ) * (nCol + 1));
    valueList = (VOIDPTR*) TALLOC(  sizeof(VOIDPTR) * ( nCol + 1) );
    for( i = 0;
         i < nCol;
         i++)
    {
         dimList[i] = i + 1;
         pnewInfo = newNlist -> info_ptr + i;
         valueList[i] = (VOIDPTR) TALLOC( pnewInfo -> data_size *
                                     ( pnewtab -> nrows + 1) );
         pnewdata = pnewtab -> data_ptr + i;
         if(! pnewdata -> inc_value  )
         {
         hoblockmove( pnewdata -> data + ( pnewInfo -> data_size * loadBegin),
                      (VOIDPTR)valueList[i],
                      pnewInfo -> data_size * loadrows);
         }
    }

    			          /* start replacing the rows.          */
    
    nl_set_current_row(oldNlist, replaceBegin, oldNlist -> current_table);
    
    switch(  updateType  )
    {
         case 1: status = nl_replace_rows( oldNlist, poldtab -> nrows, nCol, 
                                             dimList, (VOIDPTR *)valueList);
                 break;
         case 2: status = nl_replace_rows(oldNlist, pnewtab -> nrows, nCol,
                                             dimList, (VOIDPTR *)valueList);
                 break;
         case 3: replacerows = pnewtab -> nrows - replaceBegin + 1;
                 status = nl_replace_rows(oldNlist, replacerows, nCol, dimList,
                                                      (VOIDPTR *)valueList);
                 break;
  
	 case 4: replacerows = poldtab -> nrows - loadBegin + 1;
                 status = nl_replace_rows(oldNlist, replacerows, nCol, dimList,
                                                      (VOIDPTR *)valueList);
         case 5: replacerows = poldtab -> nrows;
                 status = nl_replace_rows(oldNlist, replacerows, nCol, dimList,
                                                      (VOIDPTR *)valueList);
                 break;
             
         default : break;
     }

#if NEED_TO_FREE
                                  /*   free the allocated memory.              */
    tc_free(dimList);
    for( i = 0;
         i < nCol;
         tc_free( valueList[i++]))
    {
    }
    tc_free(valueList);
#endif

    return status;
}
/* END-CODE */
