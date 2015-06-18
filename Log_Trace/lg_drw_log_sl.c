/* DEC/CMS REPLACEMENT HISTORY, Element LG_DRW_LOG_SL.C*/
/* *8    11-JUN-1991 17:10:57 JESSIE "(SPR 0) change null value from double to float"*/
/* *7    18-APR-1990 11:50:03 JULIAN "(SPR 0) fix casting"*/
/* *6    18-APR-1990 10:00:57 JULIAN "(SPR 0) remove nlist internal use"*/
/* *5    17-JAN-1990 15:35:26 WALTERS "(SPR 0) Add nl_init_nlist_info to name dimensions"*/
/* *4    25-SEP-1989 11:11:59 GILLESPIE "(SPR 200) Changes for new n-List Version 4"*/
/* *3    11-AUG-1989 11:40:20 JULIAN "(SPR -1) constant in conditional context"*/
/* *2    27-JUL-1989 23:09:16 GILLESPIE "(SPR 1) Fix minor casting problems"*/
/* *1    19-JUN-1989 13:04:37 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element LG_DRW_LOG_SL.C*/

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


#include "esi_nl.h"
#include "esi_gks.h"
#include "esi_tc.h"
#include "esi_math.h"


#if USE_PROTOTYPES

static INT lgz_chk_slope(UINT nrows, FLOAT *x, FLOAT *y, DOUBLE skipped_slope,
	INT normal, INT logFlag, DOUBLE translate_x, DOUBLE translate_y,
	FLOAT **returned_x, FLOAT **returned_y, UINT *returned_rows);

#else

static INT lgz_chk_slope();

#endif

#define MAXPTS (INT)4000

/* *****************************************************************************

    ROUTINE: LG_DRAW_LOG_SLOPE

    DESCRIPTION
         Draws a log polyline and skips all rows whose slope with the next
    row is within the range of slope, which with the previous row, +/-
    skipped_slope.

    ************************************************************************** */
publicdef INT lg_draw_log_slope(nlist, from, to, left, right, skipped_slope, 
	    logFlag, returned_nlist)
    /**************************************************************************/

NLIST_HEADER nlist;               /*%P the given pointer of N-list.  */
DOUBLE from;                      /*%P the start value of vertical dimension. */
DOUBLE to;                        /*%P the end value of vertical dimension.   */
DOUBLE right;                     /*%P the right boundary of HOR. dimension.  */
DOUBLE left;                      /*%P the left boundary of V. dimension.     */
DOUBLE skipped_slope;		  /*%P the tolerance of skipped_slope.        */
INT logFlag;
NLIST_HEADER *returned_nlist;
/* **************************************************************************** */
{
    BOOL  normal, first;
    INT status = SUCCESS;
    INT target_table = 1;
    UINT  dim_list[2], num_rows, cur_table, table_num; 
    INT  start, end;
    INT   i, j, data_x, data_y;
	UINT nallocated, sub_nrows ;
    FLOAT *value_list[2], *value_add_list[2], *px, *py, 
	  min_y, max_y, *xptr, *yptr;
    DOUBLE translate_x, translate_y;
    DOUBLE high_null, low_null;
    FLOAT nulls[2];                  /* NULL values of variables.              */
    INT units[2];
    size_t data_sizes[2];
    INT data_types[2];
    INT value_inc[2];
    VOIDPTR v_list[2];
    static CHAR *c_name[3] = {"VALUE", "DEPTH", NULL};

	for (i = 0; i < 2; i++)
		{
		units[i] = 0;
		data_sizes[i] = sizeof (FLOAT);
		value_inc[i] = VALUE_LIST;
		data_types[i] = FLOAT_TYPE;
		}

     						/* initialize the returned */
     						/* nlist.                  */

     status = nl_init_nlist(returned_nlist, 2, units, data_sizes, data_types,
     				value_inc, 500);
    dim_list[0] = 2;      /* the 'x' dimension (parallel to the bottom of the
                             plot) is the second data value ('dependent') */
    dim_list[1] = 1;      /* the 'y' dimension (parallel to the left size of the
                             plot) is the depth dimension.  This is stored
                             in the first data position */
    status = nl_set_nlist_info(*returned_nlist, NL_NLIST_COLUMN_NAME, 2, 
				dim_list, (VOIDPTR)c_name);
    if (status != SUCCESS)
    {
	return status;
    }

    normal = (left < right);
    translate_x = right + left;
    translate_y = from + to;

    for  (cur_table = 1;
          nl_set_current_row(nlist, 1, cur_table) >= 0;
          cur_table++)
    {
	status = nl_start_table(*returned_nlist, &table_num);
        py = &max_y;
        nl_inq_table_info(nlist, cur_table, NL_TABLE_MAX, 1, &dim_list[1],
	    (VOIDPTR *)&py);

        if (max_y < from)
            continue;  /* get next table */

        py = &min_y;
        nl_inq_table_info(nlist, cur_table, NL_TABLE_MIN, 1, &dim_list[1],
	    (VOIDPTR *)&py);

        if (min_y > to)
            break;    /* all done */

        nl_inq_table_int(nlist, cur_table, NL_TABLE_NROWS,
	     (INT *)&nallocated);
        nallocated++;   /* see if this helps - doesn't hurt to allocate a bit more than you need */

        px = (FLOAT *) tc_alloc(nallocated * sizeof(FLOAT));
        py = (FLOAT *) tc_alloc(nallocated * sizeof(FLOAT));

	value_list[0] = px;
	value_list[1] = py;

        nl_get_rows(nlist, nallocated, 2, dim_list, (VOIDPTR *)value_list,
		&num_rows);

                                 /*    set the NULL value */

	v_list[0]=(VOIDPTR)&nulls[0];
	v_list[1]=(VOIDPTR)&nulls[1];
        status=nl_inq_nlist_info(nlist,NL_NLIST_NULL_VALUE,2,dim_list,v_list);
        if(status != SUCCESS) return status;

                                  /*   set the high null and low null.         */

         high_null = (DOUBLE)( nulls[0] > 0 ) ? 1.01 * nulls[0] : 0.99 * nulls[0];
         low_null = (DOUBLE)( nulls[0] > 0 ) ? 0.99 * nulls[0] : 1.01 * nulls[0];

        /* invert the Y axis */
        for (i = 0, first = TRUE;
             (i < nallocated) AND (*py < to);
             i++, py++)
        {
            if (*py < from)
                continue;

            if (first)
               {
               start = i;
               first = FALSE;
               }

            *py = translate_y - *py;
        }

/*         end = i;   MOD by JJC.           */
         end = ( i EQUALS nallocated ) ? i - 1 : i;

        if (logFlag)
	    {
            translate_x = log10(right);
	    for (i = start, px += start;
		 (i < end);
		 i++, px++)
		{
		if ((*px >= low_null AND *px <= high_null) OR
		    (*px < 0.0))
		    {
		    *px = nulls[0];
		    }
		else
		    {
		    *px = normal ? log10(*px) : log10(right) - log10(*px);
		    }
		}
	    }
	else if (NOT normal)	    /* translate non-logarithmic scales */
	    {
	    for (i = start, px += start;
		 (i < end);
		 i++, px++)
		{
		if (*px <= low_null OR *px >= high_null)
		    {
		    *px = translate_x - *px;
		    }
		}
	    }

        for (i = start; i < end; i += MAXPTS)
        {
            if ((i+MAXPTS) > end)
               {
               num_rows = end - i;
               }
            else
               {
               num_rows = MAXPTS;
               }

            px = value_list[0]+i;
            py = value_list[1]+i;

                                  /*   if there is no NULL value rows, draw  */
                                  /*   it directly.                            */
            status=nl_inq_table_int(nlist,cur_table,NL_TABLE_NROWS,
		(INT *)&sub_nrows);
            if(status != SUCCESS) return status;
            v_list[0]=(VOIDPTR)&data_x;
            v_list[1]=(VOIDPTR)&data_y;
            status=nl_inq_table_info(nlist,cur_table,NL_TABLE_NON_NULL,2,
                dim_list,v_list);
            if(status != SUCCESS) return status;
 
             if(  ( sub_nrows EQUALS data_x) AND
                 ( sub_nrows EQUALS data_x)  )
            {
		 lgz_chk_slope(num_rows, px, py, 
				skipped_slope, normal, logFlag, translate_x, 
				translate_y,&xptr, &yptr, &num_rows);
		 value_add_list[0] = xptr;
                 value_add_list[1] = yptr;
		 status = nl_add_rows(*returned_nlist, num_rows, 2,
					dim_list, (VOIDPTR *)value_add_list);
		 tc_free(xptr);
		 tc_free(yptr);
            }
            else
            {
                                  /*   if there are some NULL value rows, cut*/
                                  /*   it to be serval segment.                */


	       for( j = 0, sub_nrows = 0;
		    j < num_rows;
		    j++, sub_nrows++)
	       {
		    if(  (px[j] <= high_null AND px[j] >= low_null) )
		    {

			      /*   only draws more than two rows.        */

			 if(   sub_nrows > 1 )
			 {
			      xptr = &px[j - sub_nrows];
			      yptr = &py[j - sub_nrows];
		              lgz_chk_slope(sub_nrows, 
					xptr, yptr, skipped_slope, 
					normal, logFlag, translate_x,
					translate_y, &xptr, 
					&yptr, &sub_nrows);
			      value_add_list[0] = xptr;
			      value_add_list[1] = yptr;
			      status = nl_add_rows(*returned_nlist, 
					num_rows, 2,
					dim_list, (VOIDPTR *)value_add_list);
			      tc_free(xptr);
			      tc_free(yptr);
			 }
			 sub_nrows = -1;

		    }
	       }
                                  /*   draw the last table, if there is one.  */
              if(   sub_nrows > 1 )
              {
                    xptr = &px[j - sub_nrows];
                    yptr = &py[j - sub_nrows];
		    lgz_chk_slope(sub_nrows, 
					xptr, yptr, skipped_slope, normal, logFlag, translate_x,
					translate_y, &xptr, 
					&yptr, &sub_nrows);
		    value_add_list[0] = xptr;
                    value_add_list[1] = yptr;
                    status = nl_add_rows(*returned_nlist, num_rows, 2,
					dim_list, (VOIDPTR *)value_add_list);
		    tc_free(xptr);
		    tc_free(yptr);
               }
            }
        }

        tc_free(value_list[0]);
        tc_free(value_list[1]);
    }

    return SUCCESS;
}

static INT lgz_chk_slope(nrows, x, y, skipped_slope, normal, logFlag,
		    translate_x, translate_y, returned_x, returned_y,
				     returned_rows)
UINT nrows, *returned_rows;
INT normal, logFlag;
FLOAT x[], y[], **returned_x, **returned_y;
DOUBLE skipped_slope, translate_x, translate_y;
{
    DOUBLE pre_slope, cur_slope;
    INT status = SUCCESS;
    INT num_rows;
    INT start, end;
    FLOAT *temp_x, *temp_y;
    DOUBLE abs_skipped_slope, pow_y;

    abs_skipped_slope = ( skipped_slope >= 0 ) ? skipped_slope :
						( -1.0 ) * skipped_slope;

					/* allocates the returned space.  */

    temp_x = (FLOAT *)tc_alloc( sizeof(FLOAT ) * nrows );
    temp_y = (FLOAT *)tc_alloc( sizeof(FLOAT ) * nrows );

					/* set the first pair of rows.  */

     if( ! normal AND ! logFlag )
     {
         temp_x[0] = translate_x - x[0];
         temp_x[1] = translate_x - x[1];
     }
     else 
     if( logFlag )
     {
		pow_y = x[0];
		temp_x[0] = ( normal ) ? pow(10.0, pow_y) :
				     pow(10.0, translate_x ) - pow(10, pow_y);
     }
     else
     {
         temp_x[0] = x[0];
         temp_x[1] = x[1];
     }
     temp_y[0] = translate_y - y[0];
     temp_y[1] = translate_y - y[1];
   
    if( x[1] EQUALS x[0] )
	pre_slope = 9999.0;
    else
	pre_slope = (y[1] - y[0] ) / (x[1] - x[0]);


     if( ! normal AND ! logFlag )
     {
    	for( start = 1, end = 2, num_rows = 2;
		 end < nrows;	)
    	{
					/* calculates the slope of next pair.*/
		if( x[end] EQUALS x[start])
	    	     cur_slope = 99999.9;
		else
	    	     cur_slope = (y[end] - y[ start] ) / (x[end] - x[start]);
        	if( ( cur_slope <= pre_slope + abs_skipped_slope ) AND
	    		( cur_slope >= pre_slope - abs_skipped_slope ) )
		{
					/* if the slope within the skipped   */
					/* range.			     */
	    		end++;
		}
		else
		{
					/* the slope is not in the range.    */
            		pre_slope = cur_slope;
	                temp_x[num_rows] = translate_x - x[end];
		        temp_y[num_rows++] = translate_y - y[end];
	    		start = end;
	    		end++;
		}
    	}    
     }
     else 
     if( logFlag )
     {
	for( start = 1, end = 2, num_rows = 2;
		 end < nrows;	)
    	{
					/* calculates the slope of next pair.*/
		if( x[end] EQUALS x[start])
	    		cur_slope = 99999.9;
		else
	    		cur_slope = (y[end] - y[ start] ) / (x[end] - x[start]);

        	if( ( cur_slope <= pre_slope + abs_skipped_slope ) AND
	    		( cur_slope >= pre_slope - abs_skipped_slope ) )
		{
					/* if the slope within the skipped   */
					/* range.			     */
	    		end++;
		}
		else
		{
					/* the slope is not in the range.    */
            		pre_slope = cur_slope;
			pow_y = x[end];
			temp_x[num_rows] = ( normal ) ? pow(10.0, pow_y) :
				     pow(10.0, translate_x ) - pow(10, pow_y);
	    	}
		temp_y[num_rows++] = translate_y - y[end];
	    	start = end;
	    	end++;
	}
     }
     else
     {
	    for( start = 1, end = 2, num_rows = 2;
		 end < nrows;	)
    	{
					/* calculates the slope of next pair.*/
		if( x[end] EQUALS x[start])
		    cur_slope = 99999.9;
		else
		    cur_slope = (y[end] - y[ start] ) / (x[end] - x[start]);
        	if( ( cur_slope <= pre_slope + abs_skipped_slope ) AND
	    		( cur_slope >= pre_slope - abs_skipped_slope ) )
		{
					/* if the slope within the skipped   */
					/* range.			     */
	   		 end++;
		}
		else
		{
					/* the slope is not in the range.    */
            		pre_slope = cur_slope;
           		if( ! normal AND ! logFlag )
                		temp_x[num_rows] = translate_x - x[end];
            		else 
				temp_x[num_rows] = x[end];
	    		temp_y[num_rows++] = translate_y - y[end];
	    		start = end;
	    		end++;
		}
    	}    
     }
    *returned_rows = num_rows;
    *returned_x = temp_x;
    *returned_y = temp_y;

    return SUCCESS;
}
