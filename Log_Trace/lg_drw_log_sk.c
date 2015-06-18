/* DEC/CMS REPLACEMENT HISTORY, Element LG_DRW_LOG_SK.C*/
/* *3    17-JAN-1990 15:35:11 WALTERS "(SPR 0) Add nl_init_nlist_info to name dimensions"*/
/* *2    27-JUL-1989 23:09:08 GILLESPIE "(SPR 1) Fix minor casting problems"*/
/* *1    19-JUN-1989 13:04:34 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element LG_DRW_LOG_SK.C*/

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

/* *****************************************************************************

    ROUTINE: LG_DRAW_LOG_SKIP

    DESCRIPTION
         Draws a log polyline for every nth points.

    ************************************************************************** */
publicdef INT lg_draw_log_skip(nlist, from, to, left, right,
	skipped_points, logFlag, returned_nlist)
    /**************************************************************************/

NLIST_HEADER nlist;               /*%P the given pointer of N-list.  */
DOUBLE from;                      /*%P the start value of vertical dimension. */
DOUBLE to;                        /*%P the end value of vertical dimension.   */
DOUBLE right;                     /*%P the right boundary of HOR. dimension.  */
DOUBLE left;                      /*%P the left boundary of V. dimension.     */
UINT skipped_points;		  		  /*%P the number of points should be skipped.*/
INT logFlag;
NLIST_HEADER *returned_nlist;
/* **************************************************************************** */
{

     UINT dim_list[2];
     INT units[2];
     size_t data_sizes[2];
     INT data_types[2];
     INT value_inc[2];
     VOIDPTR value_list[2];
     FLOAT *old_dim1, *old_dim2, *new_dim1, *new_dim2;
     INT total_strokes;
	 UINT total_points, total_new_points;
	 INT i, j, k;
     INT status = SUCCESS;
     UINT stroke_num;
     static CHAR *c_name[3] = {"DEPTH", "VALUE", NULL};

     status = nl_inq_nlist_int(nlist, NL_NLIST_NSTROKES, (INT *)&total_strokes);
     if( status != SUCCESS )
     	return status ;

	for (i = 0; i < 2; i++)
		{
		dim_list[i] = i + 1;
		units[i] = 0;
		data_sizes[i] = sizeof (FLOAT);
		data_types[i] = FLOAT_TYPE;
		value_inc[i] = VALUE_LIST;
		}
		
     						/* initialize the returned */
     						/* nlist.                  */

     status = nl_init_nlist(returned_nlist, 2, units, data_sizes, data_types,
     				value_inc, 500);
     status = nl_set_nlist_info(*returned_nlist, NL_NLIST_COLUMN_NAME, 2,
				dim_list, (VOIDPTR)c_name);
     if (status != SUCCESS)
     {
	return status;
     }

     for( i = 1; i <= total_strokes AND status >= SUCCESS; i++)
     {
     						/* start a new stroke.	   */

     	status = nl_start_stroke(*returned_nlist, &stroke_num);
     	status = nl_set_current_point(nlist, 1, i);
     	if( status < SUCCESS )
     		continue;
         
     	status = nl_inq_stroke_int(nlist, i, NL_STROKE_NPOINTS, (INT *)&total_points);
      	old_dim1 =(FLOAT *)tc_alloc(sizeof(FLOAT) * (total_points + 1) );
      	old_dim2 =(FLOAT *)tc_alloc(sizeof(FLOAT) * (total_points + 1) );
     	total_new_points = total_points / skipped_points;
      	new_dim1 =(FLOAT *)tc_alloc(sizeof(FLOAT) * (total_new_points + 1) );
      	new_dim2 =(FLOAT *)tc_alloc(sizeof(FLOAT) * (total_new_points + 1) );
     	value_list[0] = (VOIDPTR)old_dim1;
     	value_list[1] = (VOIDPTR)old_dim2;
     	status = nl_get_points(nlist, total_points, 2, dim_list, value_list,
     					&total_points);

     	for( j = 0, k = 0; j < total_points; j += skipped_points, k++)
     	{
     		new_dim1[k] = old_dim1[j];
     		new_dim2[k] = old_dim2[j];
     	}
     	value_list[0] = (VOIDPTR)new_dim1;
     	value_list[1] = (VOIDPTR)new_dim2;
     	status = nl_add_points(*returned_nlist, total_new_points, 2, 
     				dim_list, value_list);
     	tc_free(old_dim1 );
     	tc_free(old_dim2 );
     	tc_free(new_dim1 );
     	tc_free(new_dim2 );
     }
     

     return SUCCESS;
}
