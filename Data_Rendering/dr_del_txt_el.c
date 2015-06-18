/* DEC/CMS REPLACEMENT HISTORY, Element DR_DEL_TXT_EL.C*/
/* *3    24-SEP-1990 16:11:41 GILLESPIE "Beta Deltas"*/
/* *2    26-JUN-1989 10:40:46 GILLESPIE "(SPR 20) Fix castings"*/
/* *1    19-JUN-1989 12:46:24 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element DR_DEL_TXT_EL.C*/

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

NAME:          dr_delete_text_element

DESCRIPTION:   Delete a text from the dr_data structure


     Input:    dr_data - The standard data structure

     Output:   dr_data - Updated

     Status:   SUCCESS

******************************************************************************/

#include    "esi_dr.h"
#include    "esi_go.h"
#include    "esi_am.h"

publicdef INT dr_delete_text_element(dr_data)
DR_DATA *dr_data;
{
    INT status;
    UINT stroke, point;
    GO_WORKSPACE *w, **global_w;
    OBJECT_STRUCTURE *this_object,*old_object;


			     /*	 Find the text, and delete from the list of */
			     /*	 texts					    */

    old_object = (OBJECT_STRUCTURE *)NULL;
    for(this_object = dr_data->text_ptr;
        this_object != (OBJECT_STRUCTURE *)NULL;
        this_object = this_object->next_object)
    {
        if(this_object->object_ptr.current_text EQUALS 
                          dr_data->current_text)
        {
			     /*	     Delete the current text from the	    */
			     /*	     database if it exists		    */

	    if ( !this_object->new_object )
	    {
	    
		dr_delete_element( 	dr_data->current_object_type,
					    dr_data->current_object_key,
					    dr_data->current_text->text_id, 
					    DR_TEXT_OBJECT,
					    (CHAR *) 0,
					    (CHAR *) 0 );
            } 				
            if(old_object EQUALS (OBJECT_STRUCTURE *)NULL)
            {
                 dr_data->text_ptr = this_object->next_object;
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
	
	    if(dr_data->current_text_seg > 0)
	        wmsdelete(dr_data->current_text_seg);
	
			     /*	 Now free the text storage		    */
	
	    am_free(dr_data->current_text->text_string);
	    am_free(dr_data->current_text);
	    am_free(this_object);
	    dr_data->current_text = (TEXT *)NULL;
	    dr_data->current_text_seg = 0;
	    dr_commit( TRUE );

			     /*	Update the software picking list.	    */
	    status = am_open_workspace("GO_STATIC_SPACE",AM_GLOBAL,(VOIDPTR *)&global_w);
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
    return SUCCESS;
}
