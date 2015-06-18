/* DEC/CMS REPLACEMENT HISTORY, Element DR_DEL_LN_EL.C*/
/* *5    15-OCT-1990 16:50:26 GILLESPIE "(SPR 1000) Merge Ernie deltas"*/
/* *4    12-OCT-1990 16:22:24 GILLESPIE "(SPR 100) Merge Ernie Deltas"*/
/* *3    24-SEP-1990 16:09:11 GILLESPIE "Beta Deltas"*/
/* *2    26-JUN-1989 10:40:24 GILLESPIE "(SPR 20) Fix castings"*/
/* *1    19-JUN-1989 12:46:15 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element DR_DEL_LN_EL.C*/

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

NAME:          	dr_delete_line_element

DESCRIPTION:   	Delete a line from the database and the dr_data structure.


	Input:    	dr_data 	- The standard data structure

	Output:   	dr_data 	- Updated

	Status:   	SUCCESS 	- If line was found and deleted,
     		   	FAIL	 	- If line list was empty or line not found.
     		   
    TODO:		1) 	Should mark the line as deleted, leaving database deletes
    				to be done when line saved.

******************************************************************************/

#include    "esi_dr.h"
#ifndef ESI_AM_H
#include    "esi_am.h"
#endif
#ifndef ESI_WM_H
#include "esi_wm.h"
#endif
#include    "esi_go.h"

publicdef INT dr_delete_line_element( dr_data )
DR_DATA *dr_data;
{
    INT status = SUCCESS;
    UINT stroke, point;
    OBJECT_STRUCTURE *this_object,*old_object;
    GO_WORKSPACE *w, **global_w;

    
			     /*	 Find the line, and delete from the list of */
			     /*	 lines					    */

    old_object = (OBJECT_STRUCTURE *)NULL;
    for(this_object = dr_data->line_ptr;
        this_object != (OBJECT_STRUCTURE *)NULL;
        this_object = this_object->next_object)
    {
        if(this_object->object_ptr.current_line EQUALS 
                          dr_data->current_line)
        {
    
			     /*	     Delete the current line from the	    */
			     /*	     database if it exists		    */

		if ( !this_object->new_object )
		{
			dr_delete_element( dr_data->current_object_type,
					   dr_data->current_object_key,
					   dr_data->current_line->line_id, 
					   DR_LINE_OBJECT,
					   dr_data->current_marker,
					   dr_data->current_line->line_name );
            	 }
            if(old_object EQUALS (OBJECT_STRUCTURE *)NULL)
            {
                 dr_data->line_ptr = this_object->next_object;
                 break;
            }
            else
            {
                 old_object->next_object = this_object->next_object;
                 break;
            }
        }
        else old_object = this_object;
    }
    
    if (this_object != (OBJECT_STRUCTURE *)NULL)
    {	
			     /*	 Remove item from display		    */
	
	    if(dr_data->current_line_seg > 0)
		{
	        status = wmsdelete(dr_data->current_line_seg);
		if ( status != SUCCESS )
			{
			am_message(AM_ALERT, 
			"Line segment delete failed in dr_delete_line_element!");
			}
		}

			     /*	 Now free the object & line storage	    */
	
	    nl_free_nlist(dr_data->current_line->line_nlist);
	    am_free(dr_data->current_line);
	    am_free(this_object);
	    dr_data->current_line = (LINE *)NULL;
	    dr_data->current_line_seg = 0;
	    dr_data->object_changed = TRUE;
	    dr_commit( TRUE );

			     /*	Update the software picking list.	    */
	    status = am_open_workspace("GO_STATIC_SPACE",AM_GLOBAL,
		(VOIDPTR *)&global_w);
	    w = *global_w;
	    status = nl_inq_current_point(w->temp_nlist,&point,&stroke);
	    status = nl_delete_stroke(w->temp_nlist,stroke);

    }
    else
    {
			     /*	     Empty list or object not found	    */
	    dr_commit( FALSE );
    	status = FAIL;
    }
	
    return status;
}
