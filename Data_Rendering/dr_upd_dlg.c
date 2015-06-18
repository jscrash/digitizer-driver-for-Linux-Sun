/* DEC/CMS REPLACEMENT HISTORY, Element DR_UPD_DLG.C*/
/* *3    12-JUN-1990 10:36:33 KEE "(SPR -1) Bug fixing and new enhancement of Electronic Drafting Text Edit for (SCD)"*/
/* *2    27-DEC-1989 10:24:35 SCD "(SPR 100) Eliminate enable VSCROLL control (bad news for X)."*/
/* *1    19-JUN-1989 12:48:56 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element DR_UPD_DLG.C*/

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

NAME:          	dr_update_dialog

DESCRIPTION:   	Update the text dialog state to match
		the current state of the text object.

     Input:    	dialog_data - Structure containing the dialog state
               	dr_data - The standard data structure 
 
     Output:   	dr_data - Updated

     Status:   	SUCCESS
     
******************************************************************************/

#include    "esi_dr.h"
#include    "esi_wi.h"
#include    "esi_wi_rsrc.h"
#include    "dr_resources.rh"
#include    "dr_txt_dlg.rh"

/******************************************************************************/
/*   Global Definitions                                                       */
/******************************************************************************/

#define null_string ""

/******************************************************************************/
/*   End of Global Definitions                                                */
/******************************************************************************/

publicdef INT dr_update_dialog( dialog_data, dr_data, cdlg_id)

DR_DIALOG_DATA *dialog_data;
DR_DATA *dr_data;
CDLG_ID *cdlg_id;
{
    INT status;
    WI_RSRC_INFO rsrc_info;
    INT i;
    BOOL curr_text_exists = FALSE;

/*  Make sure the dialog reflects the current state of the data  */

    curr_text_exists = (dr_data->current_text != (TEXT *)NULL);
    
/*  Enable the dialog   */

    for(i=0;i<32;i++)
    {
       dialog_data->enabled_new[i] = FALSE;
    }
    dialog_data->enabled_new[DR_ET_OK-1] = TRUE;
    dialog_data->enabled_new[DR_ET_NEW-1] = TRUE;
    dialog_data->enabled_new[DR_ET_SELECT-1] = 
    	(dr_data->current_object > 0 && 
	 dr_data->text_ptr != (OBJECT_STRUCTURE *) NULL);
    dialog_data->enabled_new[DR_ET_DISPLAY-1] = curr_text_exists;
    dialog_data->enabled_new[DR_ET_ANGLE-1] = curr_text_exists;
#ifdef USE_X
    dialog_data->enabled_new[DR_ET_COLOR_MENU-1] = curr_text_exists;
    dialog_data->enabled_new[DR_ET_FONT_MENU-1] = curr_text_exists;
    dialog_data->enabled_new[DR_ET_ALIGNMENT_MENU-1] = curr_text_exists;
#else
    dialog_data->enabled_new[DR_ET_COLOR-1] = curr_text_exists;
    dialog_data->enabled_new[DR_ET_FONT-1] = curr_text_exists;
    dialog_data->enabled_new[DR_ET_ALIGNMENT-1] = curr_text_exists;
#endif
    dialog_data->enabled_new[DR_ET_PLACE-1] = curr_text_exists;
    dialog_data->enabled_new[DR_ET_DELETE-1] = curr_text_exists;
    dialog_data->enabled_new[DR_ET_POSITION-1] =  
		(curr_text_exists && dr_data->current_text_seg != 0);
    dialog_data->enabled_new[DR_ET_TEXT_LINES-1] = curr_text_exists; 
    dialog_data->enabled_new[DR_ET_SCROLL-1] = curr_text_exists; 
	/* vscroll always same as edittext enable flag */
	
    if (curr_text_exists)
	{
        switch (dr_data->default_text.text_precision)
	    {
	    case 0: /* String */
		dialog_data->enabled_new[DR_ET_ANGLE-1] = FALSE;
		break;
	    case 1: /* Character */
		dialog_data->enabled_new[DR_ET_ANGLE-1] = TRUE;
		break;
	    case 2: /* Stroke */
		dialog_data->enabled_new[DR_ET_ANGLE-1] = TRUE;
		break;
	    }
	}

    wi_query_info(cdlg_id->text_dialog_id,&rsrc_info);
    dialog_data->no_dialog_items = rsrc_info.numitems;
    for(i=0;i<rsrc_info.numitems;i++)
    {
        if(dialog_data->enabled_new[i] != dialog_data->enabled_old[i])
        {
            dialog_data->enabled_old[i] = dialog_data->enabled_new[i];
            wi_enable_dialog_item(cdlg_id->text_dialog_id, i+1,
                                  dialog_data->enabled_new[i] );
        }
    }
    
/*	Diplay in the edit text the current text string -- if there is none
	then signify by displaying empty string */

	wi_set_dialog_text	(cdlg_id->text_dialog_id,
						DR_ET_TEXT_LINES,
						(dr_data->current_text != (TEXT *)NULL ?
							dr_data->current_text->text_string :
							null_string) );
							
    return SUCCESS;
}
