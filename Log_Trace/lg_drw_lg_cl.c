/* DEC/CMS REPLACEMENT HISTORY, Element LG_DRW_LG_CL.C*/
/*  10    4-MAR-1992 11:42:52 JEFF "(SPR 6180) fixed prototyping error"*/
/*  9    27-FEB-1992 09:22:03 JESSIE "(SPR 6180) add TVD display of traces and tops"*/
/* *8     2-AUG-1991 17:19:03 JESSIE "(SPR 0) fix mis-drawing line between two point"*/
/* *7    11-JUN-1991 16:04:21 JESSIE "(SPR 7838) bug fixing for log trace gaps"*/
/* *6     7-JUN-1991 14:49:40 JESSIE "(SPR 7840) bug fixing for unclipped log traces(case # 2138) and null values handle(case*/
/*#2788)"*/
/* *5    18-APR-1990 10:00:46 JULIAN "(SPR 0) remove nlist internal use"*/
/* *4    25-SEP-1989 11:03:36 GILLESPIE "(SPR 200) Changes for n-List Version 4"*/
/* *3    11-AUG-1989 11:40:06 JULIAN "(SPR -1) constant in conditional context"*/
/* *2    27-JUL-1989 22:53:48 GILLESPIE "(SPR 1) Fix minor casting problem"*/
/* *1    19-JUN-1989 13:04:31 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element LG_DRW_LG_CL.C*/

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
#include "esi_wm.h"
#include "esi_tc.h"
#include "esi_ho.h"
#include "esi_math.h"

#if USE_PROTOTYPES
static INT lg_dlc_throw_away(NLIST_HEADER nlist, DOUBLE from,
		      DOUBLE to, DOUBLE right, DOUBLE left, INT logFlag );
#else
static INT lg_dlc_throw_away();
#endif

#define MAXPTS (INT)4000

/* *****************************************************************************

    ROUTINE: LG_DRAW_LOG_CLIP

    DESCRIPTION
	 Draws a log polyline with clipping.
	 There are several method to do the clipping.
	 1. Throw away all the rows outside the clipping range.
	 2. ...
	 3. ...

	For the time being, we only implement the method 1.

    ************************************************************************** */
publicdef INT lg_draw_log_clip(nlist, from, to, left, right, logFlag, method )
    /**************************************************************************/

NLIST_HEADER nlist;               /*%P the given pointer of N-list.  */
DOUBLE from;                      /*%P the start value of vertical column. */
DOUBLE to;                        /*%P the end value of vertical column.   */
DOUBLE right;                     /*%P the right boundary of HOR. column.  */
DOUBLE left;                      /*%P the left boundary of V. column.     */
INT logFlag;
INT method ;
/* **************************************************************************** */
{
    INT status = 0;

	switch (method )
	{
		case 1 :
			status = lg_dlc_throw_away(nlist, from, to, left,
					right, logFlag);
			break;
		default: break;
	}
	return status;
}
    /************************************************************************ */
static INT lg_dlc_throw_away(nlist, from, to, left, right, logFlag )
    /**************************************************************************/

NLIST_HEADER nlist;               /*%P the given pointer of N-list.  */
DOUBLE from;                      /*%P the start value of vertical column. */
DOUBLE to;                        /*%P the end value of vertical column.   */
DOUBLE right;                     /*%P the right boundary of HOR. column.  */
DOUBLE left;                      /*%P the left boundary of V. column.     */
INT logFlag;
/* **************************************************************************** */
{
    INT status=SUCCESS;
    BOOL  normal, first, clip, prev_out_range = FALSE;
    BOOL  begin = TRUE;
    BOOL  pre_null = FALSE;
    UINT   dim_list[2], cur_table, u_nallocated;
	UINT num_rows;
    INT   data_x, data_y, i, j, nallocated, sub_nrows, start, end, table_num=1;
    FLOAT *value_list[2], *px, *py, min_y, max_y, min_x, px_temp,
				max_x, *xptr, *yptr, *px_clip, *py_clip;
    FLOAT backup_x[2], backup_y[2];
    DOUBLE translate_x, translate_y;
    DOUBLE clip_min, clip_max;
    DOUBLE high_null, low_null;
    /*DOUBLE nulls[2];                   NULL values of variables.   */
    FLOAT nulls[2];                  /* NULL values returned from  */
				     /* nl_inq_nlist_info function */
    VOIDPTR v_list[2];

    dim_list[0] = 2;      /* the 'x' column (parallel to the bottom of the
			     plot) is the second data value ('dependent') */
    dim_list[1] = 1;      /* the 'y' column (parallel to the left size of the
			     plot) is the depth column.  This is stored
			     in the first data position */
    normal = (left < right);
    translate_x = right + left;
    translate_y = from + to;

			  /* set the min and max of clipping.            */
    if( normal )
    {
	clip_min = left;
	clip_max = right;
    }
    else
    {
	clip_min = translate_x - left;
	clip_max = translate_x - right;
    }

    if (logFlag)   
    {
	    clip_min = log10(clip_min);
    	    clip_max = log10(clip_max);
    }

					/* allocates a temp. buffer for clipping*/
    for  (cur_table = 1, u_nallocated = 0;
	  nl_set_current_row(nlist, 1, cur_table) >= 0;
	  cur_table++)
    {
	status=nl_inq_table_int(nlist,cur_table,NL_TABLE_NROWS,&sub_nrows);
	if( sub_nrows > u_nallocated )
		u_nallocated = sub_nrows;
    }
    px_clip = (FLOAT *) TALLOC((u_nallocated + 2)* sizeof(FLOAT));
    py_clip = (FLOAT *) TALLOC((u_nallocated + 2)* sizeof(FLOAT));

    for  (cur_table = 1;
	  nl_set_current_row(nlist, 1, cur_table) >= 0;
	  cur_table++)
    {
	prev_out_range = FALSE;

	value_list[0] = &max_x;
	value_list[1] = &max_y;
	nl_inq_table_info(nlist, cur_table, NL_TABLE_MAX, 2, dim_list,
					(VOIDPTR *)value_list);

	if (max_y < from)
	    continue;  /* get next table */

	value_list[0] = &min_x;
	value_list[1] = &min_y;
	nl_inq_table_info(nlist, cur_table, NL_TABLE_MIN, 2, dim_list,
					(VOIDPTR *)value_list);

	if (logFlag)
	    {
	    max_x    = log10(max_x);
	    min_x    = log10(min_x);
	    }

	if (min_y > to)
	    break;    /* all done */

	if( max_x < clip_min OR min_x > clip_max )
	    continue;
	else
	if( max_x <= clip_max AND min_x >= clip_min )
	{
	     clip = FALSE;
	}
	else
	{
	     clip = TRUE;
	}
	nl_inq_table_int(nlist, cur_table, NL_TABLE_NROWS, &nallocated);
	nallocated++;   /* see if this helps - doesn't hurt to
			    allocate a bit more than you need */

	px = (FLOAT *) TALLOC(nallocated * sizeof(FLOAT));
	py = (FLOAT *) TALLOC(nallocated * sizeof(FLOAT));

	value_list[0] = px;
	value_list[1] = py;
	u_nallocated = nallocated;
	nl_get_rows(nlist, u_nallocated, 2, dim_list, (VOIDPTR *)value_list,
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
	     (i < num_rows) AND (*py < to);
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

         end = i;   /* MOD by JJC.           */
/*	 end = ( i EQUALS num_rows ) ? i - 1 : i;*/

	if (logFlag)
	    {
	    for (i = start, px += start;
		 (i < end);
		 i++, px++)
		{
		if ((*px >= low_null AND *px <= high_null) OR
		    (*px <= 0.0))
		    {
		    *px = nulls[0];
		    }
		else
		    {
		    *px = normal ? log10(*px) : log10(right) - log10(*px);
		    }
		}
	    }
	else if (NOT normal)        /* translate non-logarithmic scales */
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

					/* If there is no clipping, start    */
					/* drawing, else drawing and clipping.*/

	if( NOT clip )     /* not clipping */
	for (i = start; i < end; i += MAXPTS-1)
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
	    status=nl_inq_table_int(nlist,cur_table,NL_TABLE_NROWS,&sub_nrows);
	    if(status != SUCCESS) return status;
	    v_list[0]=(VOIDPTR)&data_x;
	    v_list[1]=(VOIDPTR)&data_y;
	    status=nl_inq_table_info(nlist,cur_table,NL_TABLE_NON_NULL,2,
		dim_list,v_list);
	    if(status != SUCCESS) return status;

	    if(  ( sub_nrows EQUALS data_x) AND 
		 ( sub_nrows EQUALS data_y)  )  /* if there is no NULL value */
	    {
		 if (begin == FALSE)		/* connect two point between */
		   {				/* two drawing segment, if   */ 
		     backup_x[1] = px[0];	/* not in first segment      */
		     backup_y[1] = py[0]; 
		     wm_gpl (2, backup_x, backup_y);
		   }
		 wm_gpl(num_rows, px, py);
		 backup_x[0] = px[num_rows - 1];
		 backup_y[0] = py[num_rows - 1];
		 begin = FALSE;
	    }
	    else
	    {
				  /*   if there are some NULL value rows, cut*/
				  /*   it to be serval segment.                */
		pre_null = FALSE;

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
		 	      if (begin == FALSE) /* don't draw if first time
			      */
		   		{
				  if (pre_null == FALSE) /* don't draw gap if
				  previous point is NULL */
				    {
		   		    backup_x[1] = xptr[0];
		   		    backup_y[1] = yptr[0];
		   		    wm_gpl (2, backup_x, backup_y);
				    }
				}
			      wm_gpl(sub_nrows, xptr, yptr);
		 	      backup_x[0] = xptr[sub_nrows - 1];
		 	      backup_y[0] = yptr[sub_nrows - 1];
		 	      begin = FALSE;
			      pre_null = TRUE;
			 }
			 sub_nrows = -1;

		    }
	       }
				  /*   draw the last table, if there is one.  */
	      if(   sub_nrows >= 1 )
	      {
		    xptr = &px[j - sub_nrows];
		    yptr = &py[j - sub_nrows];
		     if (begin == FALSE)
		   	{
			if (pre_null == FALSE)
			  {
		   	  backup_x[1] = xptr[0];
		   	  backup_y[1] = yptr[0];
		   	  wm_gpl (2, backup_x, backup_y);
			  }
		   	}
		      wm_gpl(sub_nrows, xptr, yptr);
	              backup_x[0] = xptr[sub_nrows - 1];
		      backup_y[0] = yptr[sub_nrows - 1];
		      begin = FALSE;
		      pre_null = FALSE;
	       }
	    }
	}
	else  /* Clippeing */
	for (i = start; i < end; i += MAXPTS-1)
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

				  /*   if there are some NULL value rows or */
				  /*   outside the clipping range, cut        */
				  /*   it to be serval segment.               */

	    if (begin == FALSE)
	    {

	    /* begin of checking for the connecting point between two poligan */

	 	  backup_x[1] = px[0];
	  	  backup_y[1] = py[0];

		  /* if both points are outside boundry, do not thing */
		  if (pre_null == TRUE)
			pre_null = FALSE;
		  if (backup_x[0] > clip_max AND backup_x[1] < clip_min)
			;
		  else if (backup_x[1] > clip_max AND backup_x[0] < clip_min)
			;
		  else if (backup_x[0] > clip_max AND backup_x[1] > clip_max)
			;
		  else if (backup_x[0] < clip_min AND backup_x[1] < clip_min)
			;

		  else if (backup_x[0] > clip_max OR 
			   backup_x[0] < clip_min)
		  /* if first point is outside of the border */
		    {
		    			/* recalc the first drawing point */

		      px_temp = (backup_x[0] > clip_max) ?
					clip_max : clip_min;
		      backup_y[0] = backup_y[1] -
			(backup_x[1] - px_temp ) *
			(backup_y[1] - backup_y[0])/
			(backup_x[1] - backup_x[0]);
		      backup_x[0] = px_temp;
	   	      wm_gpl (2, backup_x, backup_y);
		    }

		  else if (backup_x[1] > clip_max OR 
			   backup_x[1] < clip_min)
		  /* if last point is outside of the border */
		    {
		    			/* recalc the last drawing point */

		    px_temp = (backup_x[1] > clip_max) ?
					clip_max : clip_min;
		    backup_y[1] = backup_y[1] -
			(backup_x[1] - px_temp ) *
			(backup_y[1] - backup_y[0])/
			(backup_x[1] - backup_x[0]);
		    backup_x[1] = px_temp;
	   	    wm_gpl (2, backup_x, backup_y);
		    }
		  else
		    {
					/* draw if both points are inside */
		    wm_gpl ( 2, backup_x, backup_y);
		    }

	    /* end of checking for the connecting point between two poligan */

	    }
	    begin = FALSE;

	       for( j = 0, sub_nrows = 0;
		    j < num_rows;
		    j++, sub_nrows++)
	       {
		    if(  (px[j] <= high_null AND px[j] >= low_null)  )
		    /* if this point is NULL value then draw previous points */
		    {

			      /*   only draws more than two rows.        */

			 if(   sub_nrows > 1 )
			 {
			      xptr = &px[j - sub_nrows];
			      yptr = &py[j - sub_nrows];
			      wm_gpl(sub_nrows, xptr, yptr);
			 }
			 prev_out_range = FALSE;
			 pre_null = TRUE;
			 sub_nrows = -1;

		    }
		    else  /* if this point is out of boundery */
		    if(  (px[j] < clip_min OR px[j] > clip_max)  )
		    {

			      /*   only draws more than two rows.        */

			 if(   sub_nrows >= 1 )
			 {
			      hoblockmove(&px[j - sub_nrows], px_clip,
					sizeof(FLOAT) * sub_nrows );
			      hoblockmove(&py[j - sub_nrows], py_clip,
					sizeof(FLOAT) * sub_nrows );
			      px_clip[sub_nrows] = (px[j] > clip_max) ?
						clip_max : clip_min;
			      py_clip[sub_nrows] = py[j] -
				(px[j] - px_clip[sub_nrows] ) *
				(py[j] - py[j - 1])/(px[j] - px[j - 1]);
			      sub_nrows++;
			      wm_gpl(sub_nrows, px_clip, py_clip);
			      prev_out_range = TRUE;
			 }
			 sub_nrows = -1;
		    }
		    else
		    {
					/* if the previous row is out of   */
					/* range, clips it and add to the    */
					/* buffer.                           */
			if( prev_out_range )
			{
			      prev_out_range = FALSE;
			      px_temp = (px[j - 1] > clip_max) ?
						clip_max : clip_min;
			      py[j - 1] = py[j] -
				(px[j] - px_temp ) *
				(py[j] - py[j - 1])/(px[j] - px[j - 1]);
			      px[j - 1] = px_temp;
			      backup_x[0] = px[j-1];
			      backup_y[0] = py[j-1];
			      j--;
			}
		    }

	       }
				  /*   draw the last table, if there is one.  */
	      if(   sub_nrows >= 1 )
	      {
		    xptr = &px[j - sub_nrows];
		    yptr = &py[j - sub_nrows];
		    wm_gpl(sub_nrows, xptr, yptr);
	            backup_x[0] = xptr[sub_nrows - 1];
		    backup_y[0] = yptr[sub_nrows - 1];
		    begin = FALSE;
		    pre_null = FALSE;
	       }
	}
#if NEED_TO_FREE EQUALS TRUE
	tc_free(value_list[0]);
	tc_free(value_list[1]);
#endif
    }
#if NEED_TO_FREE EQUALS TRUE
    tc_free(px_clip );
    tc_free(py_clip );
#endif
    return SUCCESS;
}
