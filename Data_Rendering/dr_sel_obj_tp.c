/* DEC/CMS REPLACEMENT HISTORY, Element DR_SEL_OBJ_TP.C*/
/* *2    18-JUL-1990 08:46:12 GILLESPIE "(SPR 1) Add esi_dr_msg.h"*/
/* *1    19-JUN-1989 12:48:38 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element DR_SEL_OBJ_TP.C*/

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

NAME:          dr_select_object_type 

DESCRIPTION:   Get an object type from the user/operator


     Input:    dr_data - The standard data structure

     Output:   dr_data - updated

     Status:   SUCCESS;

******************************************************************************/

#include    "esi_dr.h"
#include    "esi_ts.h"
#include    "esi_wi.h"
#ifndef ESI_MG_H
#include    "esi_mg.h"
#endif

#include "esi_dr_msg.h"

/******************************************************************************/
/*   Global Definitions                                                       */
/******************************************************************************/

#define null_string ""
        
/******************************************************************************/
/*   End of Global Definitions                                                */
/******************************************************************************/

#if USE_PROTOTYPES
publicdef INT dr_select_object_type( DR_DATA *dr_data )
#else
publicdef INT dr_select_object_type( dr_data )
DR_DATA *dr_data;
#endif
{
    INT status;
    CHAR msg[80];
    CHAR **object_type_list;
    INT no_items = 0;
    BOOL ifyes;
    BOOL cancel;
    CHAR object_type[41];


/*  Query whether to save or release the object  */

    if((dr_data->object_changed) AND (dr_data->current_object_key > 0))
    {
        sprintf(msg,mg_message(DR_SAVE_CHANGES),dr_data->current_object_key);
        wiyesno(msg,null_string,&ifyes,&status);
        if(ifyes)
	    {
            status = dr_save_object(dr_data);
	    }
        else
	    {
            status = dr_release_object(dr_data);
	    }
    }
    else if(dr_data->current_object_key > 0)
	{
        status = dr_release_object(dr_data);
	}

/*  Retrieve the list of object types  */

    status = dr_get_object_list(&object_type_list,&no_items);

    if (status EQUALS SUCCESS AND no_items > 0)
	{
/*  Get the selection   */

	status = dr_get_selection( mg_message(DR_OBJECT_TYPE), object_type_list,
		       no_items, object_type, &cancel);
	
/*  Store the new type  */

	if(!cancel)
	    {
	    status = dr_change_object_type( object_type, dr_data );
	    }

	ts_tcp_free(object_type_list);
	}

    return SUCCESS;
}    
