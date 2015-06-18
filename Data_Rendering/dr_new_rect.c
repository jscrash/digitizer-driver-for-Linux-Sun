/* DEC/CMS REPLACEMENT HISTORY, Element DR_NEW_RECT.C*/
/* *8    24-SEP-1990 15:34:25 GILLESPIE "Beta Deltas"*/
/* *7    18-JUL-1990 10:59:39 GILLESPIE "(SPR 1) Add esi_dr_msg.h"*/
/* *6    16-FEB-1990 14:29:12 GILLESPIE "(SPR 6005) Add setting of n-List polygon clipping method"*/
/* *5    17-JAN-1990 15:30:28 WALTERS "(SPR 0) Add nl_init_nlist_info to name dimensions"*/
/* *4    12-JUL-1989 07:38:59 GORDON "(SPR 0) add wm_set_mouse_menu call"*/
/* *3     3-JUL-1989 10:03:01 GORDON "(SPR 0) initialize dim_list"*/
/* *2    26-JUN-1989 10:41:46 GILLESPIE "(SPR 20) Fix castings"*/
/* *1    19-JUN-1989 12:47:14 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element DR_NEW_RECT.C*/
/* DEC/CMS REPLACEMENT HISTORY, Element DR_NEW_RECT.C*/
/* *7    18-JUL-1990 10:59:39 GILLESPIE "(SPR 1) Add esi_dr_msg.h"*/
/* *6    16-FEB-1990 14:29:12 GILLESPIE "(SPR 6005) Add setting of n-List polygon clipping method"*/
/* *5    17-JAN-1990 15:30:28 WALTERS "(SPR 0) Add nl_init_nlist_info to name dimensions"*/
/* *4    12-JUL-1989 07:38:59 GORDON "(SPR 0) add wm_set_mouse_menu call"*/
/* *3     3-JUL-1989 10:03:01 GORDON "(SPR 0) initialize dim_list"*/
/* *2    26-JUN-1989 10:41:46 GILLESPIE "(SPR 20) Fix castings"*/
/* *1    19-JUN-1989 12:47:14 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element DR_NEW_RECT.C*/

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

/******************************************************************************

NAME:          dr_new_rectangle 

DESCRIPTION:   Band a rectangle to create a line object. 


     Input:    dr_data

     Output:   dr_data - Updated

     Status:   SUCCESS

******************************************************************************/

#include    "esi_dr.h"
#include    "esi_am.h"
#include    "esi_wm.h"
#include    "esi_wi.h"
#ifndef ESI_MG_H
#include    "esi_mg.h"
#endif

#include "esi_dr_msg.h"

#if USE_PROTOTYPES
publicdef INT dr_new_rectangle(DR_DATA *dr_data)
#else
publicdef INT dr_new_rectangle(dr_data)
DR_DATA *dr_data;
#endif
{
    INT status;
    FLOAT x1, y1, x2, y2;
    INT button;
    LINE *this_line;
    INT i;
    UINT ndim = 2;
    UINT dim_list[2];
    INT units[2];
    size_t data_sizes[2];
    INT data_types[2];
    INT value_inc[2];
    VOIDPTR value_list[2];
    FLOAT x_array[4], y_array[4];
    UINT stroke;
    UINT npoints;
    GO_WORKSPACE *w, **global_w;                     
    GO_USER_STRUCT user_struct;
    static CHAR *c_name[3] = {"X", "Y", NULL};
    

   /*  Band a rectangle  */

    if (ug_if_gpx())
	wm_set_mouse_menu( (CHAR**)0, (INT*)0 );

    am_message(AM_DIALOG, mg_message(DR_BAND_RECTANGLE));
    wmtlocate(&status, &x1, &y1, &button);
    if (button == WM_BUTTON_CANCEL || button == WM_BUTTON_DONE) 
    {
        am_message(AM_DIALOG, NULL);
        return SUCCESS;
    }  
    wmtbandrectangle(&status, (DOUBLE)x1, (DOUBLE)y1, &x2, &y2, &button);
    if (button == WM_BUTTON_CANCEL || button == WM_BUTTON_DONE) 
    {
        am_message(AM_DIALOG, NULL);
        return SUCCESS;
    }  
   /*  Store the new line   */

    dr_data->line_poly = TRUE;
    status = dr_new_line(dr_data);
    this_line = dr_data->current_line;
    this_line->polygon_flag = TRUE;

    for (i = 0; i < ndim; i++)
	{
	units[i] = 0;
	data_sizes[i] = sizeof (FLOAT);
	data_types[i] = FLOAT_TYPE;
	value_inc[i] = VALUE_LIST;
	dim_list[i] = i + 1;
	}
    nl_init_nlist(&this_line->line_nlist, ndim, units, data_sizes, data_types, 
                   value_inc, 20);    
    status = nl_set_nlist_info(this_line->line_nlist, NL_NLIST_COLUMN_NAME, ndim,
		      dim_list, (VOIDPTR)c_name);
    if (status != SUCCESS)
    {
	return status;
    }
    nl_set_nlist_method(this_line->line_nlist, NL_CLIP_METHOD,
	"NLM_WINDOW_CLIP_POLY", nlm_window_clip_poly);
    nl_start_table(this_line->line_nlist, &stroke);
        
    x_array[0] = x1;
    y_array[0] = y1;
    x_array[1] = x2;
    y_array[1] = y1;
    x_array[2] = x2;
    y_array[2] = y2;
    x_array[3] = x1;
    y_array[3] = y2;

    value_list[0] = (VOIDPTR)x_array;
    value_list[1] = (VOIDPTR)y_array;

    npoints = 4;
    nl_add_rows(this_line->line_nlist, npoints, ndim, dim_list, value_list);

   /* Set current point to beginning to workaround bug in mm_draw_polygon */
   nl_set_current_row(this_line->line_nlist, 1, 1);
	
    dr_data->object_changed = TRUE;
    strcpy(this_line->line_name, "Rectangle");
    dr_redraw_line( dr_data );
    
   am_message(AM_DIALOG, NULL);
  	
   /* ------------------------------- */
   /* update common nlist for picking */                            
   /* ------------------------------- */
   status = am_open_workspace("GO_STATIC_SPACE",AM_GLOBAL,
		(VOIDPTR)&global_w);
   w = *global_w;
   status = go_copy_stroke(this_line->line_nlist,1,w->temp_nlist, &stroke);
   user_struct.elem_id = this_line->line_id;
   user_struct.segment_id = dr_data->current_line_seg;
   user_struct.if_text = FALSE; 
   user_struct.obj_id = dr_data->current_object_key;    
   user_struct.group_id = dr_data->current_object;      
   status = nl_set_table_user_struct(w->temp_nlist,stroke,
		(VOIDPTR)&user_struct, sizeof(GO_USER_STRUCT));
    return SUCCESS;
}
