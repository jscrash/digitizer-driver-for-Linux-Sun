/* DEC/CMS REPLACEMENT HISTORY, Element LM_DRW_ANNOT.C */
/* *3    11-SEP-1991 13:40:38 JULIAN "(SPR 0) add comments" */
/* *2    18-MAR-1991 18:08:25 JULIAN "(SPR 0) Lease Mapping Init Checkin" */
/* *1    18-MAR-1991 17:52:24 JULIAN "Lease Mapping" */
/* DEC/CMS REPLACEMENT HISTORY, Element LM_DRW_ANNOT.C */
#include "esi_lm.h"

#ifndef ESI_WM_H
#include "esi_wm.h"                      /* window manager */
#endif

#ifndef ESI_QS_H
#include "esi_qs.h"
#endif

#ifndef ESI_TS_H
#include "esi_ts.h"
#endif

#ifndef ESI_CT_H
#include "esi_ct.h"
#endif

#ifndef ESI_GKS_H
#include "esi_gks.h"
#endif

#ifndef ESI_MP_H
#include "esi_mp.h"
#endif

#define SMALL_NUMBER -1000

/* Function Description -----------------------------------------------------
Description:
    Function to draw annotation on lease overlay

Prototype:
    publicdef INT lm_draw_annotation(LM_LEASE_ID lease_id,
                                     NLIST_HEADER polygons,
				     LM_STRUCT *pex,
				     BOOL clipped)

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) lease_id    -(INT)
    (I) polygons    -(NLIST_HEADER)
    (I) clipped     -(INT)
    (I) plm	    -(LM_STRUCT *)

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT lm_draw_annotation(LM_LEASE_ID lease_id,NLIST_HEADER polygons,
	LM_STRUCT *plm,BOOL clipped)
#else
publicdef INT lm_draw_annotation(lease_id,polygons,plm,clipped)
LM_LEASE_ID lease_id;
NLIST_HEADER polygons; 
LM_STRUCT *plm; 
BOOL clipped;
#endif
    {
    GKS_REAL delta_y,node_x, node_y;
    INT status;
    INT num_annotate,i,longest_text_index;
    COORD center_x,center_y;
    CHAR *text_string;
    DOUBLE width,height,max_width,max_height;
    DOUBLE lower_y,ratio;
    COLUMN_NAME column,temp_text;    
    COORD *x_pos,*y_pos;
    INT total_polygons,poly_no;
    INT total_points,pts_no;
    UINT ret_pts;
    UINT nrows,ncol=2;
    static UINT col_list[2] = { 1,2 };
    static VOIDPTR value_list[2];
    DOUBLE new_text_height,new_height;
    DOUBLE final_text_height,final_height;
    INT isense;
    INT max_len,len;
    TCP column_tcp;
    MAP_STRUCTURE *mapdef;
    DOUBLE minx,miny,maxx,maxy;

	/** GET THE MAP **/ 
    status = mp_get_current_map(&mapdef);
    if (status != SUCCESS)
        {
        return status;
        }
     
	/*   Char expansion factor */
    wm_gschxp(1.0);

	/*   Char spacing */
    wm_gschsp(0.0);

	/*   Char Up Vector (vertical)	*/
    wm_gschup (0.0,1.0);

        /*   Text path (left to right)	*/
    wm_gstxp (GRIGHT);

        /*   Text Font & Precision              */
    plm->text_precision = GSTRKP;
    wm_gstxfp (plm->text_font, plm->text_precision);

        /*   Text Color                 */
    wm_gstxci (plm->text_color);

	/* get number of anotation text */
    num_annotate=ts_tcp_len(plm->annotate_tcp);   

	/* get max len of those annotation text and also get the column entry
	   of the annotation for specified polygon id  */
    column_tcp = (TCP)0;
    max_len=0;
    for(i = 0;i<num_annotate;i++)
	{
	status = lm_retrieve_column_entry(plm->annotate_tcp[i], 
                        lease_id,temp_text);
	if(ARE_SAME(plm->annotate_tcp[i],"PRICE"))
	   sprintf(column,"$%s/h",temp_text);
	else if(ARE_SAME(plm->annotate_tcp[i],"AREA"))
	   sprintf(column,"A/H:%s",temp_text);
	else if (ARE_SAME(plm->annotate_tcp[i],"PERCNT"))
	   sprintf(column,"%s%%",temp_text);
	else
	   strcpy(column,temp_text);
	ts_tcp_append(&column_tcp,column);
	len = strlen(column);
	if(max_len < len)
	    {
	    max_len = len;
	    longest_text_index = i;
	    }
     	}
    width = (max_len + 2) * plm->text_height;

	/* get ratio of width/height of the box surrounding
	text */
    height = ((num_annotate * 1.5)-0.5) * plm->text_height; 
    ratio = width / height ;

	/* get number of polygon for the specified lease_id */
    status = nl_inq_nlist_int(polygons,NL_NLIST_NTABLES,&total_polygons);
    if(status != SUCCESS)
	return status;

	/* loop for each polygon */
    for(poly_no = 1 ; poly_no <= total_polygons;poly_no++)
	{
	status = nl_set_current_row(polygons,1,poly_no);
	if(status < 0)
	   return status;
	
	    /* get no of points for each polygon, and put all points
		in array of coords for function call to
		get the annotation location */

	status = nl_inq_table_int(polygons,poly_no,NL_TABLE_NROWS,
		&total_points);
        if(status != SUCCESS OR total_points < 0)
	    continue;

	    /* inquire extents on polygon */
	value_list[0] = (VOIDPTR)&minx;
	value_list[1] = (VOIDPTR)&miny;
	status = nl_inq_table_info(polygons,poly_no,NL_TABLE_MIN,ncol,col_list,
                                value_list);
        value_list[0] = (VOIDPTR)&maxx;
        value_list[1] = (VOIDPTR)&maxy;
        status = nl_inq_table_info(polygons,poly_no,NL_TABLE_MAX,ncol,col_list,
				value_list);
        max_height = maxy - miny;

	x_pos = (COORD *)tc_alloc(sizeof(COORD)*total_points);
	y_pos = (COORD *)tc_alloc(sizeof(COORD)*total_points);

	value_list[0] = (VOIDPTR)x_pos;
	value_list[1] = (VOIDPTR)y_pos;

	status = nl_get_rows(polygons,total_points,ncol,col_list,
				value_list,&ret_pts);
	if (status < 0)
	    return status;
	    
	    /* initialize center_x and center_y to check if the function return 
		a value or not */
	center_x = center_y = SMALL_NUMBER;

	    /* calculate center_x,center_y and max_width */
	isense = -1;
    	ag_poly_labelxy(x_pos,y_pos,total_points,isense,ratio,
	    &center_x,&center_y,&max_width);

	    /* if poly_labelxy fails, estimate center_x,center_y and max_width */
	if(max_width EQUALS 0 OR center_x EQUALS SMALL_NUMBER OR 
	   center_y EQUALS SMALL_NUMBER)	
	    {
	    max_width = maxx - minx;
	    center_x = (max_width / 2) + minx ;
	    center_y = (max_height / 2) + miny ;
	    }
	

	    /* if center coord of posting the text is out of the window
		boundary, then don't post */
	if(clipped AND ((center_x < mapdef->lower_left_xy.x) OR
            (center_y < mapdef->lower_left_xy.y) OR
            (center_x > mapdef->upper_right_xy.x) OR
            (center_y > mapdef->upper_right_xy.y)))
            {
	    continue;
	    }

	    /* if max_width is smaller than calculated width,
		resize text */
	if(width > max_width)
	    {
	    tg_fit_string(column_tcp[longest_text_index],
		center_x,center_y,
		center_x + max_width,center_y+plm->text_height,
		plm->text_height,&new_text_height);
	    new_height = ((num_annotate * 1.5)-0.5) * new_text_height; 
	    }
	else 
	    {
	    new_text_height = plm->text_height;
	    new_height = height;
	    }

	    /* if max_height is smaller than calculated height,
                resize text */
	if(new_height > max_height)
	    {
	    final_height = max_height;
	    final_text_height = (new_text_height * final_height) / new_height;
	    }
	else
	    {
	    final_height = new_height;
	    final_text_height = new_text_height;
	    }
	    
	        /*   Char height */
	wm_gschh (final_text_height);

		/*   Text Alignment (HORIZ=center,VERT=base) */
	wm_gstxal (GACENT,GAVNOR);

		/* get position for drawing text */
	lower_y = center_y - (final_height/2);
	node_y = (GKS_REAL)lower_y;
	node_x = (GKS_REAL)center_x;
	delta_y = (GKS_REAL)(final_text_height * 1.5) ;

	    /* draw each annotation in a polygon */
	for(i=num_annotate-1;i>=0;i--)
	    {	
	    wm_gtx (node_x, node_y, column_tcp[i]);
	    node_y += delta_y;
	    }

	    /* free allocated memory */
	tc_free(x_pos);
	tc_free(y_pos);
        }
    return SUCCESS;
}
/* END */
