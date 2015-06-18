/* DEC/CMS REPLACEMENT HISTORY, Element DR_DEL_OBJ_TP.C*/
/* *5    11-JUN-1991 11:21:40 KEE "(SPR 74707476) Fix Graphic Object Type deletion bug, case 1753 and case 1759"*/
/* *4    15-OCT-1990 16:53:56 GILLESPIE "(SPR 1000) Merge Ernie deltas"*/
/* *3    18-JUL-1990 07:37:04 GILLESPIE "(SPR 1) Add esi_dr_msg.h"*/
/* *2    26-JUN-1989 10:40:34 GILLESPIE "(SPR 20) Fix castings"*/
/* *1    19-JUN-1989 12:46:18 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element DR_DEL_OBJ_TP.C*/
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

NAME:          	dr_delete_object_type 

DESCRIPTION:  	Delete an object type from the database.

	Input:    	dr_data - The standard data structure.

	Output:   	Database deletes performed.

	Status:   	SUCCESS
     			FAIL
               	
 	TODO:		1)	Add inquiry of WM tree to find associated overlay
 					group.  If it exists, enumerate each object and delete
 					the associated dr_data structures, and delete the
 					overlay group.

******************************************************************************/

#include    "esi_c_lib.h"
#include    "esi_dr.h"
#include    "esi_am.h"
#include    "esi_wi.h"

#ifndef ESI_MG_H
#include "esi_mg.h"
#endif
#include    "dr_menus.rh"
#include    "dr_resources.rh"
#include    "esi_dr_msg.h"

#define null_string ""
        
publicdef INT dr_delete_object_type(dr_data)
DR_DATA *dr_data;
{
    INT     status;
    CDLG_ID *cdlg_id;
    CHAR    msg[80];
    TCP     object_type_list;
    INT     no_items;
    BOOL    cancel;
    BOOL    ifyes;
    INT     cdlgid,itemid,cdvalue;
    INT     nitems;
    BOOL    iquit;
    CHAR    object_type[41];
    CHAR    cdtext[80];
    INT     selected;

    /*  Query whether to save or release the current object  */

    if ((dr_data->object_changed) && (dr_data->current_object_key > 0))
	{
        sprintf (msg, mg_message(DR_SAVE_CHANGES), dr_data->current_object_key);
        wiyesno (msg, null_string, &ifyes, &status);

        if (ifyes)
            status = dr_save_object (dr_data);
        else
            status = dr_release_object (dr_data);
	}
    else
	{
	if (dr_data->current_object_key > 0)
	    status = dr_release_object (dr_data);
	}

    /*  Retrieve the list of object types  */

    status = dr_get_object_list (&object_type_list, &no_items);

    if (status == SUCCESS)
	{
	/*  Put up the dialog   */
	am_open_workspace ("CDLG_ID", AM_APPLICATION, (VOIDPTR *)&cdlg_id);

	wi_set_selections (cdlg_id->selector_id,
			    DR_SELECTOR, DR_SELECTOR_SCROLL,
			    no_items, 1, object_type_list);

	wi_set_dialog_text (cdlg_id->selector_id, DR_SELECT_MSG,
			    mg_message(DR_OBJECT_TYPE));

	wi_open_dialog (cdlg_id->selector_id);

	/*  Request a selection   */
	iquit = FALSE;
	while (iquit == FALSE)
	    {
	    wi_request_dialog_item (&cdlgid, &itemid, &cdvalue, cdtext);

	    switch (itemid)
	        {
	        case DR_SELECT_OK:
		    wi_query_selections (cdlgid, DR_SELECTOR, &nitems, &selected);
		    strcpy (object_type, object_type_list[selected-1]);
		    iquit = TRUE;
		    cancel = FALSE;
		    break;

		case DR_SELECT_CANCEL:
		    object_type[0] = 0;
		    iquit = TRUE;
		    cancel = TRUE; 
		    break;
		}
	    }

	wi_close_dialog (cdlg_id->selector_id);

	/*  Store the new type  */
	if (!cancel)
	    {
		status = dr_delete_object_type_sql (object_type, dr_data);
	    }
	}

    return status;
}




