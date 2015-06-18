/* DEC/CMS REPLACEMENT HISTORY, Element SL_SEL_DESC.C*/
/* *7    11-JUN-1991 13:31:31 JULIAN "(SPR 7326) truncate select list name to 30 chars"*/
/* *6    30-JUL-1990 17:25:11 JULIAN "(SPR 0) add lynx,lease select_list"*/
/* *5    20-MAR-1990 10:29:16 WALTERS "(SPR 5109) Make FILE IS PERM the default"*/
/* *4    29-DEC-1989 11:01:51 SCD "(SPR 100) Ensure list name is UPPER."*/
/* *3     1-NOV-1989 13:15:40 GILLESPIE "(SPR 30) Remove ref to graphic object select list"*/
/* *2    11-AUG-1989 20:44:26 CONROY "(SPR 0) Revised global, selects, SQL macros & fixes"*/
/* *1    19-JUN-1989 13:25:34 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element SL_SEL_DESC.C*/
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

#include "esi_sl.h"

#ifndef ESI_AM_H
#include "esi_am.h"
#endif
#ifndef ESI_WI_H
#include "esi_wi.h"
#endif
#ifndef ESI_TS_H
#include "esi_ts.h"
#endif
#ifndef ESI_TC_H
#include "esi_tc.h"
#endif
#ifndef ESI_NL_H
#include "esi_nl.h"
#endif
#ifndef ESI_FI_H
#include "esi_fi.h"
#endif
#ifndef ESI_MG_H
#include "esi_mg.h"
#endif

#include "gb_savelist.rh"
#include "esi_sl_err.h"

/* **************************************************************************

	ROUTINE: SL_SELECT_DESCRIPTION

	DESCRIPTION:
	    To select the select list structure.

	STATUS CODES:
		SUCCESS;
		
	INPUT :
		SELECT_LIST =(NLIST_HEADER )The input select list.

	OUTPUT :
		SELECT_LIST =(NLIST_HEADER )The returned select list.
		STATUS CODES.

	AUTHOR: JAMES CHANG.
   ************************************************************************** */
publicdef INT sl_select_description( select_list)
NLIST_HEADER select_list;
{
	INT status = SUCCESS;
	INT num_items = 0, i;
	INT item = 0, class = 0;
        static id = 0;                     /* remember dialog id */
	INT value, total_changed = 0;
	BOOL quit = FALSE;
	size_t nbytes = 0;
	INT changed_items[13];
	CHAR text[256];
	CHAR today[16], tomorrow[16];
	CHAR data_type_name[32];
	CHAR total_items_text[10];
	SELECT_STRUCT temp_struct;
	
/* ************************************************************************* */	
					/* error checking.		     */
	if( select_list EQUALS NULL )
	    return SL_INVALID_SELECT_LIST;

	status = nl_inq_nlist_user_struct(select_list, sizeof(SELECT_STRUCT),
			    (VOIDPTR)&temp_struct, &nbytes);
	if(  status IS_NOT_EQUAL_TO 0)
		{
	    return status;
		}
					/* inquire the total items in select */
					/* list.			     */
	status = nl_inq_nlist_int(select_list, NL_NLIST_NPOINTS, &num_items);
	if( status )
		{
	    return status;
		}
	sprintf(total_items_text, "%d", num_items);

					/* constructs the TEXTS for data type*/
	status = sl_inq_dtype_string("DATA_TYPE_NAME",temp_struct.data_type,data_type_name);
	if(status != SUCCESS) return status;

					/* initializes the CDLG.	     */
	status = fi_push();

        if ( id == 0 )
            {
	    status = am_define_resource(AM_DIALOG,
				    "gb_savelist",
				    SAVE_LIST,
				    NULL_FUNCTION_PTR,
				    &id,
				    (VOIDPTR)0);
            if (status != SUCCESS)
		{
		am_message(AM_ALERT, mg_message(status));
		return status;
		}                 
	    } else wi_open_dialog( id );


        tc_today(today);
        tc_tomorrow(tomorrow );
	temp_struct.permanent_flag = 1;
	wiscdv(id, SAVE_LIST_PERMANENT, temp_struct.permanent_flag);
	if( temp_struct.permanent_flag )
	{
            strcpy(temp_struct.expiration_date, "");
	    wi_set_dialog_text(id, SAVE_LIST_EDIT_EXPIRATION_DATE, "");
	}
	else
	{
	    strcpy(temp_struct.expiration_date, tomorrow);		
	    wi_set_dialog_text(id, SAVE_LIST_EDIT_EXPIRATION_DATE, tomorrow);
	}
        wi_set_dialog_text(id, SAVE_LIST_MESSAGE, "");
	wi_set_dialog_text(id, SAVE_LIST_EDIT_NAME, temp_struct.name);	
	wi_set_dialog_text(id, SAVE_LIST_EDIT_DESCRIPTION, temp_struct.description);
	wi_set_dialog_text(id, SAVE_LIST_EDIT_PARENT, temp_struct.parent_file);
	wi_set_dialog_text(id, SAVE_LIST_EDIT_SELECT, temp_struct.select_phrase);
	wi_set_dialog_text(id, SAVE_LIST_EDIT_CONTROL, temp_struct.control_list);
	wi_set_dialog_text(id, SAVE_LIST_EDIT_DATA_TYPE, data_type_name);	
	wi_set_dialog_text(id, SAVE_LIST_EDIT_AREA_NAME, temp_struct.project_name);
	wi_set_dialog_text(id, SAVE_LIST_EDIT_CREATED_BY, temp_struct.account_name);
	wi_set_dialog_text(id, SAVE_LIST_EDIT_COUNT, total_items_text);	
	wi_set_dialog_text(id, SAVE_LIST_EDIT_CREATION_DATE, temp_struct.date_created);
	wi_set_dialog_text(id, SAVE_LIST_EDIT_LAST_REFERENCE, today);	
	status = wiopcd(id);
	/*----------------------------------------------*/
	/* auto select of the list name edit text field */
	/*----------------------------------------------*/
	wi_activate(id, SAVE_LIST_EDIT_NAME, ON);	

	while( !quit )
        {
	    status = wirqevt(0, &class, &id, &item);

	    switch( item )
	    {
		case SAVE_LIST_PERMANENT:
		    wiqccdi(id, item, &value, text);
		    temp_struct.permanent_flag = value;
		    if( value)
		    {
		        wi_set_dialog_text(id, SAVE_LIST_EDIT_EXPIRATION_DATE, "");
			strcpy(temp_struct.expiration_date, "");
		    }
		    else
		    {
			wi_set_dialog_text(id, SAVE_LIST_EDIT_EXPIRATION_DATE, tomorrow);
			strcpy(temp_struct.expiration_date, tomorrow);
		    }
		    break;

		case SAVE_LIST_OK:
			quit = TRUE;
					    /* inquire the edited fields and */
					    /* update them.		      */

			wiqdrcd(id, 12, &total_changed, changed_items);
			for( i = 0; i < total_changed; i++)
			{
			    wiqccdi(id, changed_items[i], &value, text);
			    switch( changed_items[i])
			    {
				case SAVE_LIST_EDIT_NAME:
/* check for empty or blank name */  if (strlen(text) == 0 ||
				         *(ts_snowhite(text)) == '\0')
					 {
					 am_message(AM_ALERT,
					     mg_message(SL_BLANK_NAME));
					 quit = FALSE;
					 continue;
					 }

				     /* check if char > 30, if it is, truncate
					to 30. If it's > 30, it will have
					problem saving it to nlist later.
					(purna - June 7, 1991) */

				     else if (strlen(text) > 30)
				       {
					 am_message(AM_ALERT,
					     mg_message(SL_NAME_TOO_LONG));
					 text[30]=0;
				       }

				     strcpy(temp_struct.name, ts_sto_upper(text));
				     break;
				case SAVE_LIST_EDIT_DESCRIPTION:
				     strcpy(temp_struct.description, text);
				     break;
				case SAVE_LIST_EDIT_EXPIRATION_DATE:
				     strcpy(temp_struct.expiration_date, text);
				default: break;
			    }
			}

					    /* update the user structure and  */
					    /* returnes.		      */

			if (quit)
			    status = nl_set_nlist_user_struct(select_list, 
			        (VOIDPTR)&temp_struct, sizeof(SELECT_STRUCT) );
			break;
	
		case SAVE_LIST_CANCEL:
		default : 
			status = CANCEL;
			quit = TRUE;
			break;
	    }
	}
	wi_close_dialog( id );
	fi_pop();
	return status;
}
