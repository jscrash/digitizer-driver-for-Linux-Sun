/* DEC/CMS REPLACEMENT HISTORY, Element DR_NEW_OBJ_TP.C*/
/* *3    18-JUL-1990 08:27:20 GILLESPIE "(SPR 1) Add esi_dr_msg.h"*/
/* *2    21-DEC-1989 15:41:38 GORDON "(SPR 0) Fix status check for cancel"*/
/* *1    19-JUN-1989 12:47:09 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element DR_NEW_OBJ_TP.C*/

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

NAME:          dr_new_object_type 

DESCRIPTION:   Create a new object type


     Input:    dr_data - The standard data structure

     Output:   dr_data - updated

     Status:   SUCCESS;

******************************************************************************/

#include    "esi_c_lib.h"
#include    "esi_dr.h"
#include    "esi_ts.h"
#include    "esi_wi.h"
#include    "esi_wm.h"
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
publicdef INT dr_new_object_type(DR_DATA *dr_data)
#else
publicdef INT dr_new_object_type(dr_data)
DR_DATA *dr_data;
#endif
{
    INT status = 0;
    CHAR msg[80];
    BOOL ifyes;
    INT nitems;
    BOOL iquit;
    CHAR object_type[41];
    CHAR *s;

/*  Query whether to save or release the object  */

    if((dr_data->object_changed) && (dr_data->current_object_key > 0))
    {
        sprintf(msg,mg_message(DR_SAVE_CHANGES),dr_data->current_object_key);
        wiyesno(msg,null_string,&ifyes,&status);
        if(ifyes)
            status = dr_save_object(dr_data);
        else
            status = dr_release_object(dr_data);
    }
    else if(dr_data->current_object_key > 0)
        status = dr_release_object(dr_data);

/*  Quit if an error has occurred   */

    if(status != SUCCESS)
    {
        return status;
    }

/*  Close last overlay group    */

    if (dr_data->current_overlay > 0)
	{
        wmgclose(dr_data->current_overlay);
	dr_data->current_overlay = 0;

	if (dr_data->named_overlay > 0)
	    {
	    wmgclose(dr_data->current_overlay);
	    dr_data->named_overlay = 0;
	    }
	}

/*  Get the new type from the user/operator  */

	strcpy( object_type, "");
    wi_request_string(mg_message(DR_NEW_OBJECT_TYPE),null_string,null_string,
                     40,object_type,&status);

    if(status != FAIL) 
    {
/*	Convention is all Oracle strings are upper case, with blanks -> underscores */
		ts_str_convert( object_type,strlen(object_type),' ','_');
		ts_sto_upper( object_type );
        
/* 	Add the new object type to OVERLAY_CODES & GRAPHIC_OBJECT_CODES */
        
        status = dr_create_object_type(object_type);
        
        if(status == SUCCESS) dr_change_object_type(object_type,dr_data);
    }

    return SUCCESS;
}    
