/* DEC/CMS REPLACEMENT HISTORY, Element DR_EDIT_TEXT.C*/
/* *3    12-JUN-1990 10:35:19 KEE "(SPR -1) Bug fixing and new enhancement of Electronic Drafting Text Edit for (SCD)"*/
/* *2    26-JUN-1989 10:41:09 GILLESPIE "(SPR 20) Fix castings"*/
/* *1    19-JUN-1989 12:46:31 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element DR_EDIT_TEXT.C*/

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

NAME:          dr_edit_text 

DESCRIPTION:   	Edit a text element


     Input:    	dr_data - The standard data structure 
 
     Output:   	dr_data - Updated

     Status:   	SUCCESS
				DR_TEXT_EDIT_ERROR

******************************************************************************/

#include    "esi_gl_defs.h"
#include    "esi_dr.h"
#include    "esi_am.h"
#include    "esi_wi.h"
#include    "esi_wi_rsrc.h"

#include    "dr_resources.rh"
#include    "dr_txt_dlg.rh"

INT dr_edit_text(dr_data)
DR_DATA *dr_data;
{
    INT status;
    CDLG_ID *cdlg_id;
    WI_RSRC_INFO rsrc_info;
    DR_DIALOG_DATA dialog_data;
    INT i;

/*  Open the dialog workspace  */

    am_open_workspace( "CDLG_ID", AM_APPLICATION, (VOIDPTR *)&cdlg_id );

/*  Enable the dialog   */

    for(i=0;i<32;i++)
    {
       dialog_data.enabled_new[i] = FALSE;
       dialog_data.enabled_old[i] = -1;
    } 

    wi_query_info( cdlg_id->text_dialog_id, &rsrc_info );
    dialog_data.no_dialog_items = rsrc_info.numitems;

    /*	Set edit text window to allow multiple lines */
    
    wi_set_dialog_range(cdlg_id->text_dialog_id,
			DR_ET_TEXT_LINES,
			0,
			DR_MAX_TEXT_LINES -1 );

/*  Start Editing  */
    
status = dr_text_dialog_server(&dialog_data, dr_data);

/*  Close the dialog  */

    wi_close_dialog(cdlg_id->text_dialog_id);


    return status;
}
