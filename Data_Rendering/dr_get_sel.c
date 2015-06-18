/* DEC/CMS REPLACEMENT HISTORY, Element DR_GET_SEL.C*/
/* *6    12-OCT-1990 16:27:23 GILLESPIE "(SPR 100) Merge Ernie Deltas"*/
/* *5    18-JUL-1990 08:18:28 GILLESPIE "(SPR 1) Change calling arg type"*/
/* *4    12-JUN-1990 10:35:37 KEE "(SPR -1) Bug fixing and new enhancement of Electronic Drafting Text Edit for (SCD)"*/
/* *3    10-AUG-1989 08:47:28 GORDON "(SPR 105) if no items to choose from, treat OK like CANCEL"*/
/* *2    26-JUN-1989 10:41:29 GILLESPIE "(SPR 20) Fix castings"*/
/* *1    19-JUN-1989 12:46:47 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element DR_GET_SEL.C*/
/* DEC/CMS REPLACEMENT HISTORY, Element DR_GET_SEL.C*/
/* *5    18-JUL-1990 08:18:28 GILLESPIE "(SPR 1) Change calling arg type"*/
/* *4    12-JUN-1990 10:35:37 KEE "(SPR -1) Bug fixing and new enhancement of Electronic Drafting Text Edit for (SCD)"*/
/* *3    10-AUG-1989 08:47:28 GORDON "(SPR 105) if no items to choose from, treat OK like CANCEL"*/
/* *2    26-JUN-1989 10:41:29 GILLESPIE "(SPR 20) Fix castings"*/
/* *1    19-JUN-1989 12:46:47 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element DR_GET_SEL.C*/

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

NAME:           dr_get_selection

DESCRIPTION:    Puts up a selector box and retrieves a single item


     Input:     prompt - a prompt string
                list - a TCP of selectable items
                no_items - the number of items in list

     Output:    selected - the selected item(from list)
                cancel - TRUE, if canceled 

     Status:    SUCCESS

******************************************************************************/

#include    "esi_dr.h"
#include    "esi_am.h"
#include    "esi_wi.h"
#include    "esi_ts.h"
#include    "esi_lu.h"
#include    "dr_resources.rh"

#if USE_PROTOTYPES
publicdef INT dr_get_selection(CHAR *prompt, TCP list, INT no_items,
    CHAR *selected, BOOL *cancel)
#else
publicdef INT dr_get_selection(prompt,list,no_items,selected,cancel)
CHAR *prompt;
TCP list;
INT no_items;
CHAR *selected;
BOOL *cancel;
#endif
{
    INT status;
    CDLG_ID *cdlg_id;
    BOOL iquit, found;
    INT cdlgid,itemid,cdvalue;
    CHAR cdtext[80];
    INT item_selected[1], the_item, i;

/*  Put up the dialog   */

    am_open_workspace("CDLG_ID",AM_APPLICATION,(VOIDPTR *)&cdlg_id);
    wi_set_selections(cdlg_id->selector_id,DR_SELECTOR,DR_SELECTOR_SCROLL,
        no_items,1,list);
    wi_set_dialog_text(cdlg_id->selector_id,DR_SELECT_MSG,prompt);
    wi_open_dialog(cdlg_id->selector_id);
    
/* set the default value. */
    for(i = 0, found = FALSE; ! found AND i < no_items; i++)
    {
	if( ARE_DIFFERENT(list[i], selected) )
	{
	    found = TRUE;
	    wi_set_default_sel(cdlg_id->selector_id,DR_SELECTOR, i + 1);
	}
    }
    if ( ! found )
    {
        wi_set_default_sel(cdlg_id->selector_id,DR_SELECTOR, 1);
    }
/*  Request a selection   */

    iquit = FALSE;
    do
    {
        wi_request_dialog_item(&cdlgid,&itemid,&cdvalue,cdtext);
        
        switch(itemid)
        {

            case DR_SELECT_OK:
            
                wi_query_selections(cdlgid,DR_SELECTOR,&no_items,
                                    item_selected);
		if (no_items > 0)
		    {
		    the_item = item_selected[0];
		    strcpy( selected, list[the_item-1] );
                    *cancel = FALSE;
		    }
		else	    /* no items selected? treat it like cancel */
		    {
		    selected[0] = '\0';
		    *cancel = TRUE;
		    }
                iquit = TRUE;
                break;

            case DR_SELECT_CANCEL:

                selected[0] = 0;
                iquit = TRUE;
                *cancel = TRUE; 
                break;
        }
    }while (iquit == FALSE);

    wi_close_dialog(cdlg_id->selector_id);

    return SUCCESS;
}


/******************************************************************************/

#if USE_PROTOTYPES
publicdef INT dr_get_alignment_selection(CHAR *prompt, INT h_current_index,
    CHAR *h_selected, BOOL *cancel)
#else
publicdef INT dr_get_alignment_selection(prompt,h_current_index,h_selected,cancel)
CHAR *prompt;
INT  h_current_index;
CHAR *h_selected;
BOOL *cancel;
#endif
{
    INT status;
    BOOL iquit;
    INT cdlgid,itemid,cdvalue;
    CHAR cdtext[80];
    CHAR current_align_text[80];
    CDLG_ID *cdlg_id;

/*  Put up the dialog   */

    am_open_workspace("CDLG_ID",AM_APPLICATION,(VOIDPTR *)&cdlg_id);
    wi_set_dialog_text(cdlg_id->alignment_id,DR_SELECT_MSG,prompt);
    /* ------------------------------------- */
    /* Set the state of dialog radio buttons */
    /* to match the current text alignment   */
    /* ------------------------------------- */
    status = lu_index_to_text( "TEXT_HORIZ_ALIGN", cdtext, h_current_index );
    if (status == SUCCESS)
	{
	if ( ARE_SAME(cdtext, "LEFT" )) itemid = DR_H_LEFT_RADIO;
	else if ( ARE_SAME(cdtext, "RIGHT" )) itemid = DR_H_RIGHT_RADIO;
	else if ( ARE_SAME(cdtext, "CENTER" )) itemid = DR_H_CENTER_RADIO;
	else status = FAIL;
	}
    if (status == SUCCESS)
	{
	wi_set_dialog_value(cdlg_id->alignment_id, itemid, ON);
	}

    wi_open_dialog(cdlg_id->alignment_id);
    iquit = FALSE;
    do
    {
        wi_request_dialog_item(&cdlgid,&itemid,&cdvalue,cdtext);
        
        switch(itemid)
        {
	    case DR_H_LEFT_RADIO:
		strcpy (h_selected, "LEFT");
		break;
	    case DR_H_RIGHT_RADIO:
		strcpy (h_selected, "RIGHT");
		break;
	    case DR_H_CENTER_RADIO:
		strcpy (h_selected, "CENTER");
		break;
            case DR_ALIGN_OK:
                iquit = TRUE;
                *cancel = FALSE;
		break;
            case DR_ALIGN_CANCEL:
                iquit = TRUE;
                *cancel = TRUE;
		break;
	}
    }while (iquit == FALSE);

    wi_close_dialog(cdlg_id->alignment_id);

    return SUCCESS;
}
