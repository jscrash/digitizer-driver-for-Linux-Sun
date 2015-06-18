/* DEC/CMS REPLACEMENT HISTORY, Element DR_UPD_MENUS.C*/
/* *1    19-JUN-1989 12:48:59 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element DR_UPD_MENUS.C*/

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

NAME:          dr_update_menus 

DESCRIPTION:   Updates electronic drafting menus


     Input:    dr_data - the common data structure for drafting
               menu_data - the menu data structure 

     Output:   Updated versions of the inputs

     Status:   SUCCESS

******************************************************************************/

#include    "esi_c_lib.h"
#include    "esi_dr.h"
#include    "esi_ts.h"
#include    "esi_wi.h"

#include    "dr_menus.rh"

/******************************************************************************/
/*   Global Definitions                                                       */
/******************************************************************************/

#define null_string ""
#define UNDEFINED "%undef%"

/******************************************************************************/
/*   End of Global Definitions                                                */
/******************************************************************************/

publicdef INT dr_update_menus( dr_data, reset)

DR_DATA *dr_data;
BOOL reset;
{
    INT status;
    INT i;
    CHAR std_name[41], mtext[80];
    static CHAR status_object_type[41] = UNDEFINED;		/* Current object type */
    static INT status_object_id = -1;					/* Current object id */

/*  Drafting menu     */

    if( reset )
    {
	strcpy(mtext, "No Current Object Type");
	strcpy(status_object_type, UNDEFINED);
	wi_set_menu_text(dr_data->menu.draft.id, DR_STATUS_TYPE, mtext);
	strcpy(mtext, "No Current Object");
	wi_set_menu_text(dr_data->menu.draft.id, DR_STATUS_OBJECT, mtext);
	strcpy(dr_data -> current_object_type, "");
	status_object_id = -1;
    }

    for(i=0;i<dr_data->menu.draft.num_items;i++)
	    dr_data->menu.draft.items_new[i] = FALSE;
    dr_data->menu.draft.items_new[DR_SELECT_TYPE-1] = TRUE;
    dr_data->menu.draft.items_new[DR_NEW_TYPE-1] = TRUE;
    dr_data->menu.draft.items_new[DR_DELETE_TYPE-1] = TRUE;
    dr_data->menu.draft.items_new[DR_STATUS_TYPE-1] = TRUE;
    dr_data->menu.draft.items_new[DR_STATUS_OBJECT-1] = TRUE;
    dr_data->menu.draft.items_new[DR_QUIT-1] = TRUE;
    for(i=1;i<=dr_data->menu.draft.num_items;i++)
    {
        if(dr_data->menu.draft.items_new[i-1] != dr_data->menu.draft.items_old[i-1])
        {
            dr_data->menu.draft.items_old[i-1] = dr_data->menu.draft.items_new[i-1];
            wi_enable_menu_item(dr_data->menu.draft.id,i,ON);
        }
    }

/*  Objects menu       */
    
    for(i=0;i<dr_data->menu.object.num_items;i++)dr_data->menu.object.items_new[i] = FALSE;
/*    dr_data->menu.object.items_new[DR_OB_SELECT_OBJ-1] = dr_data->overlay_exists; 
 */
    dr_data->menu.object.items_new[DR_OB_SELECT_OBJ-1] = TRUE;

    dr_data->menu.object.items_new[DR_OB_NEW_OBJ-1] = 
                 (strcmp(dr_data->current_object_type,null_string) != 0); 
    dr_data->menu.object.items_new[DR_OB_MOVE-1] = FALSE;
    
/*	TODO: temp make objects always saveable
    dr_data->menu.object.items_new[DR_OB_SAVE-1] =
              ((dr_data->object_changed) && (dr_data->current_object_key > 0)); 
*/

    dr_data->menu.object.items_new[DR_OB_SAVE-1] = (dr_data->current_object_key > 0);
    
    dr_data->menu.object.items_new[DR_OB_DELETE-1] = 
              (dr_data->current_object_key > 0);
    for(i=1;i<=dr_data->menu.object.num_items;i++)
    {
        if(dr_data->menu.object.items_new[i-1] != dr_data->menu.object.items_old[i-1])
        {
            dr_data->menu.object.items_old[i-1] = dr_data->menu.object.items_new[i-1];
            wi_enable_menu_item(dr_data->menu.object.id,i,
                      dr_data->menu.object.items_new[i-1]);
        }
    }

/*  Lines Menu   */


    for(i=0;i<dr_data->menu.line.num_items;i++)dr_data->menu.line.items_new[i] = FALSE;
    dr_data->menu.line.items_new[DR_LI_SELECT-1] = 
    				(dr_data->current_object_key > 0
    				&& dr_data->line_ptr != (OBJECT_STRUCTURE *) NULL);
    dr_data->menu.line.items_new[DR_LI_NEW-1] = 
    				(dr_data->current_object_key > 0);
    dr_data->menu.line.items_new[DR_LI_NEW_RECTANGLE-1] =
     				(dr_data->current_object_key > 0);   
    dr_data->menu.line.items_new[DR_LI_EDIT-1] =
                    (dr_data->current_line != (LINE *)NULL);
    dr_data->menu.line.items_new[DR_LI_DELETE-1] = 
    				(dr_data->current_line != (LINE *)NULL);
    dr_data->menu.line.items_new[DR_LI_SETNAME-1] =
                  	(dr_data->current_line != (LINE *)NULL);
    for(i=1;i<=dr_data->menu.line.num_items;i++)
    {
        if(dr_data->menu.line.items_new[i-1] != dr_data->menu.line.items_old[i-1])
        {
            dr_data->menu.line.items_old[i-1] = dr_data->menu.line.items_new[i-1];
            wi_enable_menu_item(dr_data->menu.line.id,i,
                                dr_data->menu.line.items_new[i-1]);
        }
    }

/*  Text Menu     */

    for(i=0;i<dr_data->menu.text.num_items;i++)dr_data->menu.text.items_new[i] = FALSE;
    dr_data->menu.text.items_new[DR_TX_SELECT-1] = 
    				(dr_data->current_object_key > 0
    				&& dr_data->text_ptr != (OBJECT_STRUCTURE *) NULL);
    dr_data->menu.text.items_new[DR_TX_NEW-1] = 
    				(dr_data->current_object_key > 0);
    dr_data->menu.text.items_new[DR_TX_EDIT-1] =
                    (dr_data->current_text != (TEXT *)NULL);
    dr_data->menu.text.items_new[DR_TX_DELETE-1] = 
    				(dr_data->current_text != (TEXT *)NULL);
    for(i=1;i<=dr_data->menu.text.num_items;i++)
    {
        if(dr_data->menu.text.items_new[i-1] != dr_data->menu.text.items_old[i-1])
        {
            dr_data->menu.text.items_old[i-1] = dr_data->menu.text.items_new[i-1];
            wi_enable_menu_item(dr_data->menu.text.id,i,
                     dr_data->menu.text.items_new[i-1]);
        }
    }

/*  Preferences Menu    */
    if( reset )
    {
	dr_data -> line_poly = dr_data -> line_fill = FALSE;
	wi_check_menu_item(dr_data->menu.pref.id,
			dr_data->menu.pref.items_new[DR_PR_POLYGON-1], OFF);
	wi_check_menu_item(dr_data->menu.pref.id,
			dr_data->menu.pref.items_new[DR_PR_FILL-1], OFF);
    }
    for(i=0;i<dr_data->menu.pref.num_items;i++)dr_data->menu.pref.items_new[i] = FALSE;
    dr_data->menu.pref.items_new[DR_PR_LINE-1] = TRUE;
    dr_data->menu.pref.items_new[DR_PR_POLYGON-1] = TRUE;
    dr_data->menu.pref.items_new[DR_PR_FILL-1] = TRUE;

    for(i=1;i<=dr_data->menu.pref.num_items;i++)
    {
        if(dr_data->menu.pref.items_new[i-1] != dr_data->menu.pref.items_old[i-1])
        {
            dr_data->menu.pref.items_old[i-1] = dr_data->menu.pref.items_new[i-1];
            wi_enable_menu_item(dr_data->menu.pref.id,i,
                     dr_data->menu.pref.items_new[i-1]);
        }
    }
    
/*	Set status indicators in main menu */
	
	if (strcmp(dr_data->current_object_type, status_object_type))
	{
		if (dr_data->current_object_type[0] == '\0')
		{
			strcpy(mtext, "No Current Object Type");
			strcpy(status_object_type, UNDEFINED);
		}
		else
		{
	        strcpy(std_name, dr_data->current_object_type);
	        ts_standard_name(std_name);
			sprintf(mtext, "Current Object Type is: %s", std_name);
			strcpy(status_object_type, dr_data->current_object_type);
		}
		wi_set_menu_text(dr_data->menu.draft.id, DR_STATUS_TYPE, mtext);
	}
	if (dr_data->current_object_key != status_object_id)
	{
		if (dr_data->current_object_key == 0)
		{
			strcpy(mtext, "No Current Object");
		}
		else
		{
			sprintf(mtext, 
					"Current Object Id is: %d", 
					dr_data->current_object_key);
		}
		wi_set_menu_text(dr_data->menu.draft.id, DR_STATUS_OBJECT, mtext);
	}
	status_object_id = dr_data->current_object_key;
	
/*  Set checkmarks per current state of preferences   */

    wi_check_menu_item(dr_data->menu.pref.id,DR_PR_POLYGON,dr_data->line_poly);
    wi_check_menu_item(dr_data->menu.pref.id,DR_PR_FILL,dr_data->line_fill);

    return SUCCESS;
}
