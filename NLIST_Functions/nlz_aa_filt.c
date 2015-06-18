/* DEC/CMS REPLACEMENT HISTORY, Element NLZ_AA_FILT.C */
/* *8    25-OCT-1990 12:17:37 GILLESPIE "(SPR 31) Lint run - correct casting inconsistencies" */
/* *7     7-JUN-1990 15:17:22 MING "(SPR 0) add checking if value_list check if data already sampled if yes use theat inc_val as */
/*oldInc otherwise return SUCCESS" */
/* *6    12-MAR-1990 19:03:03 GILLESPIE "(SPR 6016) More error checking on mallocs; re-engineer stats calcs" */
/* *5     5-MAR-1990 10:44:26 GILLESPIE "(SPR 6015) Change file names to be compliant with Unix" */
/* *4    26-FEB-1990 19:00:22 JULIAN "(SPR 6010) Port to Decstation 3100 (ULTRIX)" */
/* *3    26-JAN-1990 09:27:46 GILLESPIE "(SPR 1) Remove reference to esi_lg...h include file" */
/* *2     9-NOV-1989 13:53:28 GORDON "(SPR 5058) New Anti-aliasing filter function" */
/* *1     9-NOV-1989 13:51:59 GORDON "Anti-aliasing filter routine" */
/* DEC/CMS REPLACEMENT HISTORY, Element NLZ_AA_FILT.C */
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
#include "esi_nl_err.h"
#include "esi_ho.h"
#include "esi_tc.h"
#include "esi_math.h"

#if USE_PROTOTYPES
static INT nlz_get_filter( DOUBLE oldNyquist, DOUBLE newNyquist, UINT filterLen,
    DOUBLE *filter );
static INT nlz_hamming( DOUBLE *wArray, UINT filterLen );
static VOID nlz_convolve( DOUBLE *trace, UINT rows, DOUBLE *filter, 
    UINT filterLen, DOUBLE null_value );
#else
static INT nlz_get_filter();
static INT nlz_hamming();
static VOID nlz_convolve();
#endif

#define MAXFILTERS 60

/* ******************************************************************************

    ROUTINE: NLZ_ANTI_ALIAS_NLIST

    DESCRIPTION
         A help routine for NL_RESAMPLE_NLIST().  

*********************************************************************************/

publicdef INT nlz_anti_alias_filter(nlist, nCol, Col_List, newIncr, filterLen )
NLIST_HEADER nlist;         /*    (I) Pointer to NLIST_HEADER.            */
UINT nCol;             /*    (I) Number of columns in the Col_List.        */
UINT Col_List[];         /*    (I) The columns to be resampled.        */
DOUBLE newIncr;             /*    (I) The new sample increment.            */
UINT filterLen;             /*    (I) Length of filter                */
{
    UINT ii;             /*    indexes.                    */
    INT status = SUCCESS;    /*    the result status.                */
	UINT n_rows;
    DOUBLE oldNyquist;         /*    nyquist of old sample interval.            */
    DOUBLE newNyquist;         /*    nyquist of new sample interval.            */
    DOUBLE oldIncr;         /*    the current sample increment value.        */
	DOUBLE start_val;
	DOUBLE sum_x;
	DOUBLE expected_inc;
	DOUBLE expected_max;
    DOUBLE *filter = (DOUBLE*)0; /*    array of filter values.            */
    DOUBLE **VarList = (DOUBLE**)0; /*    array of pointers to each column's  */
                 /*    data                        */
    TABLE_HEADER *pTab;
    UINT rowsRead;
    UINT col;
    size_t *dataSize = (size_t*)0;
    INT *dataType = (INT*)0;

/* ***************************************************************************** */

    if(  nlist -> total_rows < 1)
     {
         return NL_EMPTY_NLIST;
     }

    pTab = nlz_set_table( nlist, (UINT)1 );

                 /*    The first column in Col_List is the
                     "resampled" column, so that's the one whose
                     increment we pay attention to.            */


    /* the following test is to see if the data is increment list */
	/* if yes then pass else we want to check if the data are already sampled*/
	/* in some increment value, the test is done by getting the expected */
	/* increment value from the sum_x and get the expected max value by this */
	/* increment value, if this expected max value is equal to the max value */
	/* in the table, use this as the oldincr else return */
				 
    if (pTab->data_ptr[Col_List[0]-1].inc_value == FALSE)
	  {
	  start_val = pTab->data_ptr[Col_List[0]-1].start_value;
	  sum_x     = pTab->data_ptr[Col_List[0]-1].sum_x;
	  n_rows    = pTab->data_ptr[Col_List[0]-1].n_valid_rows;
      expected_inc = (sum_x - n_rows*start_val) * 2 /(n_rows - 1)/n_rows;
	  expected_max = start_val + (n_rows - 1)*expected_inc;
	  if (expected_max == pTab->data_ptr[Col_List[0]-1].max)
        {	    
        oldIncr = expected_inc;
		}
      else
	    {		
	    return SUCCESS;
		}
	  }
    else
      {
      oldIncr = pTab->data_ptr[Col_List[0]-1].increment;
      }	  

                 /*    No need to filter if new sample rate is
                     finer than old one.                */

    if (fabs(newIncr) <= fabs(oldIncr))    
    {
    return SUCCESS;
    }

                 /*      get the filter                */
    oldNyquist = 1.0 / (2.0 * fabs(oldIncr));
    newNyquist = 1.0 / (2.0 * fabs(newIncr));

    if (filterLen > MAXFILTERS)
    {
    return NL_FILTER_TOO_LONG;
    }
                                                                                
                 /*    Get space for the filter.            */
    filter = (DOUBLE *)tc_alloc(filterLen * sizeof(DOUBLE));

    status = nlz_get_filter( oldNyquist, newNyquist, filterLen,    filter );

    if (status != SUCCESS)
    {
    goto finished;
    }

                 /*    Get space for the data.                */
    VarList = (DOUBLE**)tc_alloc( nCol * sizeof(DOUBLE*) );
    for (ii=0; ii<nCol; ++ii)
    {
    VarList[ii] = (DOUBLE*)tc_alloc( nlist->total_rows * sizeof(DOUBLE) );
    }
 
                 /*    Prepare to read in the rows.            */
    dataSize = (size_t *)tc_alloc( nCol * sizeof(size_t) );
    dataType = (INT *)tc_alloc( nCol * sizeof(INT) );

                 /*    All column(s) must be doubles.            */
    for (ii=0; ii<nCol; ++ii)
    {
    dataSize[ii] = sizeof(DOUBLE);
    dataType[ii] = FLOAT_TYPE;
    }

    for (ii = 1; ii <= nlist->total_tables; ++ii)
    {
    nlz_set_table( nlist, ii );

                 /*    Read in all the rows in the table        */
        status = nl_get_rows_convert( nlist, nlist->total_rows, nCol, Col_List, 
        (VOIDPTR*)VarList, (INT*)0, dataSize, dataType, &rowsRead );

    if (status < SUCCESS)
        {
        goto finished;
        }
        
                 /*    Starting with the 2nd column in Col_List,
                     apply the filter to each column
                     (dimension).                    */

    for (col = 1; col < nCol; ++col)
        {
        nlz_convolve( VarList[col], rowsRead, filter, filterLen,
        (nlist->info_ptr+Col_List[col]-1)->null_value );
        }

                 /*    Now put the filtered rows back into the        */
                 /*    nlist.                        */
        nl_set_current_row( nlist, (UINT)1, ii );
    status = nl_replace_rows_convert( nlist, nlist->total_rows, nCol-1, 
        Col_List+1, (VOIDPTR *)(VarList+1), (INT*)0, &dataSize[1], 
        &dataType[1] );
    }

finished:
                 /*      free all allocated memory.            */
    if (filter != (DOUBLE*)0)
    {
    tc_free( (VOIDPTR)filter );
    }
    if (VarList != (DOUBLE**)0)
    {    
    for (ii=0; ii>nCol; ++ii)
        {
        tc_free( (VOIDPTR)VarList[ii] );
        }
    tc_free( (VOIDPTR)VarList );
    }
    if (dataSize != (size_t*)0)    
    {
    tc_free( (VOIDPTR)dataSize );
    }
    if (dataType != (INT*)0)
    {
    tc_free( (VOIDPTR)dataType );
    }

                 /*    Ignore positive stati (nlist flags)        */
    return status < SUCCESS ? status : SUCCESS;
}

/*************************************************************************
This filter is based on a FORTRAN filter written 
    by L.R.Rabiner & C.A.McGonegal of Bell Laboraties and D.Paul of 
    MIT Lincoln Laboratory.
*****************************************************************************/
static INT nlz_get_filter(oldNyquist, newNyquist, filterLen, filter)
DOUBLE oldNyquist;    /* (I) nyquist using old sample interval */
DOUBLE newNyquist;    /* (I) nyquist using new sample interval */
UINT filterLen;        /* (I) number of filter elements */
DOUBLE *filter;        /* (O) the returned filter array */
{
    DOUBLE window[MAXFILTERS], halfFilter[MAXFILTERS];
    DOUBLE xn, tempf;
    DOUBLE cutoff;         /*    The normalized cutoff frequency (0.5 is the
                     Nyquist) 0 <= cutoff <= 0.5            */

    INT ii, jj, halfLen;
    BOOL isOdd;
                                                                                
    halfLen = ( filterLen+1) / 2;

    cutoff = (newNyquist/oldNyquist) * 0.5;                                              
    if (cutoff < 0.0 OR cutoff > 0.5)                                            
    return NL_INTERVAL_RANGE;                                                                    
                                                                                
    isOdd = (filterLen % 2) ? TRUE : FALSE;
                                                                                
                 /*    COMPUTE IDEAL (UNWINDOWED) IMPULSE RESPONSE */
                 /*    FOR FILTER                    */
                                                                                
    if (isOdd) 
        halfFilter[0] = 2.0 * cutoff;                                                         

    for (ii=isOdd ? 1 : 0; ii<halfLen; ii++)                                   
    {                                                                 
        xn = (DOUBLE)(ii); 
        if (!isOdd)
        xn += 0.5;                                                         
        tempf = xn * PI;  
    halfFilter[ii] = sin(2.0 * (tempf * cutoff))/tempf;                                                          
    }                                                                       

                 /*     Compute a rectangular window.            */
                                                                                
    for (ii=0; ii<halfLen; ii++)                                     
    window[ii] = 1.0;                                                                
                                   
                 /*    Use filterLen + 2 so zero endpoints are not */
                 /*    part of window.                    */
    nlz_hamming(window, filterLen+2);
                                                                                
    for (ii = 0; ii < halfLen; ii++)                                     
    halfFilter[ii] *= window[ii];                                                
                                                                                
                 /*    Fill in the filter appropriately with the   */
                 /*    halfFilter array.                */
                                                                                
    jj = 0;                                                                     
    for (ii = halfLen-1; ii >= 0; ii--)                                     
    filter[jj++] = halfFilter[ii];
                                                                                
    for (ii = 1; ii < halfLen; ii++)                                     
    filter[jj++] = halfFilter[ii];                                                  
                                                              
    return SUCCESS;                                                                                
} 
                                                                                
/*******************************************************************************************************
  Generalized Hamming window routine.
********************************************************************************************************/
static INT nlz_hamming(window, filterLen) 
DOUBLE *window;
UINT filterLen; 
{                                             
    DOUBLE fi;
    UINT ii, halfLen;
    BOOL isEven;
    DOUBLE alpha = 0.54, beta = 0.46;

    isEven = filterLen % 2 ? FALSE : TRUE;
    halfLen = ( filterLen+1)/2;     
                                     
    for (ii=0; ii<halfLen; ii++)                                  
    {                                                                       
        fi = (DOUBLE)ii;
        if (isEven)
        {
        fi += 0.5;;
        }
    window[ii]= alpha + (beta * cos((PI * 2.0 * fi)/filterLen));                                 
    }                                                                       
    return SUCCESS;
}                                                                             

/*******************************************************************************************************
    NLZ_CONVOLVE - Apply the anti-aliasing filter to the given array, the last step before interpolation.
********************************************************************************************************/                                                                               
static VOID nlz_convolve( array, rows, filter, filterLen, null_value )
DOUBLE *array;        /* (I/O) The array. */
UINT rows;        /* (I) The number of array elements. */
DOUBLE *filter;        /* (I) The filter to apply to the array */
UINT filterLen;        /* (I) The number of filter elements. */
DOUBLE null_value;  /* (I) The null value for the array (ignore these in computations) */
{
    UINT ii;
    UINT jj;
    UINT index;
    UINT halfLen = filterLen / 2;
    DOUBLE *tempArr;

    tempArr = (DOUBLE *)tc_alloc( rows * sizeof(DOUBLE) );

    for (ii=0; ii<rows; ++ii)
    {
    if (array[ii] == null_value)
        {
        tempArr[ii] = null_value;
        continue;
        }
    for (jj=halfLen; jj>halfLen-filterLen; --jj)
        {
        index = ii - jj;
                 /*    These checks effectively extend the input
                     array to be as long as it needs to be on
                     each end to allow for the whole filter to
                     be applied.  It would be even more accurate
                     to extend the values using the same slope
                     as produced by the last 2 values on each
                     end.                        */

        if (index < 0)
        index = 0; 
        if (index > rows - 1)
        index = rows - 1;

                 /*    Apply the filter here.                */
        if (array[index] != null_value)            
        tempArr[ii] += array[index] * filter[halfLen-jj];
        }
    }

                 /*    Now move the convolved array back on top of */
                 /*    the original.                    */
    hoblockmove( (VOIDPTR)tempArr, (VOIDPTR)array, rows * sizeof(DOUBLE) );
    tc_free( (VOIDPTR)tempArr );
    return;
}     
