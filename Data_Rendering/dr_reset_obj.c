/* DEC/CMS REPLACEMENT HISTORY, Element DR_RESET_OBJ.C*/
/* *1    19-JUN-1989 12:48:21 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element DR_RESET_OBJ.C*/

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

NAME:          dr_reset_object 

DESCRIPTION:   Reset state variables to indicate no current object,
			   and free all related space for line, text, ... elements.


     Input:    dr_data - the standard object structure

     Output:   dr_data - updated

     Status:    SUCCESS

******************************************************************************/


#include    "esi_dr.h"
#include    "esi_nl.h"
#include    "esi_am.h"

publicdef INT dr_reset_object(dr_data)
DR_DATA *dr_data;
{
    INT status;
    OBJECT_STRUCTURE *this_object,*old_object;


/*  Reset object  */

    dr_data->object_changed = FALSE;
    dr_data->current_object_key = 0;
    dr_data->current_object = 0;
    dr_data->pick_object = 0;
    dr_data->current_line = (LINE*)NULL;
    dr_data->current_text = (TEXT*)NULL;

/*  Reset lines  */

    this_object = dr_data->line_ptr;
    while(this_object != (OBJECT_STRUCTURE *)NULL)
    {
        old_object = this_object;
        this_object = this_object->next_object;
        nl_free_nlist(old_object->object_ptr.current_line->line_nlist);
        am_free(old_object);
    }
    dr_data->line_ptr = (OBJECT_STRUCTURE *)NULL;

/*  Reset Text  */

    this_object = dr_data->text_ptr;
    while(this_object != (OBJECT_STRUCTURE *)NULL)
    {
        old_object = this_object;
        this_object = this_object->next_object;
        am_free(old_object->object_ptr.current_text->text_string);
        am_free(old_object);
    }
    dr_data->text_ptr = (OBJECT_STRUCTURE *)NULL;

    return SUCCESS;
}

