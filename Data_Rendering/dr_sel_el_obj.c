/* DEC/CMS REPLACEMENT HISTORY, Element DR_SEL_EL_OBJ.C*/
/* *3    25-SEP-1989 11:31:51 GORDON "(SPR 201) Fix def of val_list"*/
/* *2    26-JUN-1989 10:42:31 GILLESPIE "(SPR 20) Fix castings"*/
/* *1    19-JUN-1989 12:48:32 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element DR_SEL_EL_OBJ.C*/

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

/****************************************************************************

NAME:		dr_slt_go_element_of_obj

DESCRIPTION:	return the closest line or text element to a specified point of
		previous picked object type.

      Input:    dr_data - The standard structure
		x,y     - coordinates of specified point 

     Output:	object - structure with data of the element closest to point
                status
************************************************************************/  	

#include "esi_c_lib.h"
#include "esi_nl.h"
#include "esi_mm.h"
#include "esi_mp.h"
#include "esi_dr.h"
#include "esi_am.h"

publicdef INT dr_slt_go_element_of_obj(dr_data, x, y, object)
DR_DATA *dr_data;
FLOAT *x;
FLOAT *y;
GO_USER_STRUCT *object;
{
    INT status;
    UINT dim_list[2];
    DOUBLE *val_list[2];
    NLIST_HEADER nlist_common;
    NLIST_HEADER obj_nlist;
    UINT stroke_used,total_strokes;
    DOUBLE min[2];
    DOUBLE max[2];
    DOUBLE d_x, d_y;
    UINT point, stroke = 0, new_stroke;
    size_t nbytes;
    UINT stroke_num;
    INT *index_in_whole;
    VOIDPTR strokelist;
    FLOAT wcsxmin, wcsxmax, wcsymin, wcsymax;/* world coords */
    FLOAT nwcxmin, nwcxmax, nwcymin, nwcymax;/* normalized window coords */
    GO_WORKSPACE *w, **global_w;
    MAP_STRUCTURE *mapdef;
     	
    status = am_open_workspace("GO_STATIC_SPACE",AM_GLOBAL,(VOIDPTR *)&global_w);
    w = *global_w;

    d_x = *x;
    d_y = *y;
   
    status = mp_get_current_map(&mapdef);
    wmtinq(mapdef->map_tnr, &nwcxmin, &nwcxmax, &nwcymin, &nwcymax,
    &wcsxmin, &wcsxmax, &wcsymin, &wcsymax);
	min[0] = wcsxmin;/* current window limits */
	max[0] = wcsxmax;
	min[1] = wcsymin;
	max[1] = wcsymax;
    
    /* now find the closest point */
    val_list[0] = &d_x;
    val_list[1] = &d_y;
    dim_list[0] = 1;
    dim_list[1] = 2;

    /* Construct the object nlist for the picking.  */
    status = nl_clone_nlist(w->temp_nlist, &obj_nlist);
    status = nl_inq_nlist_int(w-> temp_nlist, NL_NLIST_NSTROKES, 
				    (INT *)&total_strokes);
    index_in_whole = (INT *)tc_zalloc(sizeof(INT) * total_strokes);
    for(stroke = 1; stroke <= total_strokes; stroke++)
    {
	status = nl_set_current_point(w->temp_nlist, 1, stroke);
	if( status >= 0 )
	{
	    status = nl_inq_stroke_user_struct(w->temp_nlist, stroke, 
    					sizeof(GO_USER_STRUCT),
				(VOIDPTR)object,&nbytes);   
	    if( object -> obj_id EQUALS dr_data -> current_object_key )
	    {
		status = nl_copy_stroke(w->temp_nlist, stroke, 
					obj_nlist, &new_stroke);
		index_in_whole[new_stroke - 1] = stroke;
	    }
	}
    }
    status = nl_pick_stroke_range(obj_nlist, 2, dim_list, 
				val_list, min, max, &stroke, &point);
    
    if (stroke > 0)
	{
/* find line_id corresponding to stroke thru the stroke_header */
	status = nl_set_current_point(obj_nlist,point,stroke);
    	status = nl_inq_stroke_user_struct(obj_nlist, stroke, 
    					sizeof(GO_USER_STRUCT),object, 
    					&nbytes);  
	status = nl_set_current_point(w-> temp_nlist, 1, 
				    index_in_whole[stroke -1]);
  	}
    else
	object = (GO_USER_STRUCT *) 0;

    tc_free( index_in_whole);
    nl_free_nlist(obj_nlist);
    status = (status >= 0) ? 0 : status;
    return(status);

}
