/* DEC/CMS REPLACEMENT HISTORY, Element SL_CH_LIST.C*/
/* *6    19-JUL-1991 14:35:05 MING "(SPR 0) add SL_NO_SLISTS_SELECTED"*/
/* *5    30-JUL-1990 17:23:30 JULIAN "(SPR 0) add lynx,lease select_list"*/
/* *4    29-DEC-1989 11:00:44 SCD "(SPR 100) Ensure list name is UPPER."*/
/* *3     7-DEC-1989 11:02:59 JULIAN "(SPR 5069) add cp_select_list_name"*/
/* *2    12-AUG-1989 21:27:24 CONROY "(SPR 0) Revised SL convention of $$ hidden lists"*/
/* *1    19-JUN-1989 13:25:13 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element SL_CH_LIST.C*/

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
#ifndef ESI_NL_H
#include "esi_nl.h"
#endif
#ifndef ESI_WI_H
#include "esi_wi.h"
#endif
#ifndef ESI_TS_H
#include "esi_ts.h"
#endif
#ifndef ESI_FI_H
#include "esi_fi.h"
#endif
#ifndef ESI_SL_ERR_H
#include "esi_sl_err.h"
#endif
#ifndef ESI_SL_MSG_H
#include "esi_sl_msg.h"
#endif
#ifndef ESI_CP_H
#include "esi_cp.h"
#endif

#include "sl_select_cdlg.rh"

#if USE_PROTOTYPES
 extern CHAR *mg_message(INT id);
#else
 extern CHAR *mg_message();
#endif
static INT slz_select_init();


/* **************************************************************************

	ROUTINE: SL_CHOOSE_LIST

	DESCRIPTION:
		To display all select lists, whose data type is DATA_TYPE,
		on the select box to allow user to choose a select list.
		Then returns the selected list.

	STATUS CODES:
		SUCCESS;

	INPUT :
		DATA_TYPE  =(INT )The specified data type.
		SELECT_LIST =(NLIST_HEADER *)The returned select list.

	OUTPUT :
		SELECT_LIST =(NLIST_HEADER *)The returned select list.
		STATUS CODES.

	AUTHOR: JAMES CHANG.
   ************************************************************************** */
INT sl_choose_list(data_type, select_list)
SL_DATA_TYPE data_type;
NLIST_HEADER *select_list;
{
	PROGNAME("sl_choose_list");
	INT status = SUCCESS;
	INT num_items = 0;
	INT item = 0, id = 0, class = 0, cdlg_id;
	INT selected_items[2];
	INT normal = TRUE;
	CHAR data_type_name[32];
	static CHAR **lists;
/* ************************************************************************* */
	status = sl_inq_dtype_string("DATA_TYPE_NAME",data_type,data_type_name);
	if(status != SUCCESS) return status;

					/* construct the TCP by using the    */
					/* qualified lists.                  */

	status = sl_inq_slist_names(data_type_name, &lists, &num_items);

					/* if there is no items returned,    */
					/* bring up the alert box.           */

	if( num_items <= 0 )
	   am_message(AM_ALERT, mg_message( SL_NO_SLISTS_AVAILABLE ));

	if( status )
	{
	    *select_list = NULL;
	    return FAIL;
	}

					/* initializes the CDLG.             */
	status = fi_push();
	status = slz_select_init(lists, num_items , &cdlg_id);
	do
	{
	     status = wirqevt(0, &class, &id, &item);
	     if( id != cdlg_id )
	     {
		  normal = FALSE;
		  wibeep(1);
	     }
	     else
	     {
	     normal = TRUE;
	     switch( item )
	     {
		case SL_BTN_DONE:
			wiqsel(id, SL_SELECT_BOX, &num_items,
						    selected_items);
			if( num_items > 0 )
				{
				strcpy(cp_select_list_name,lists[selected_items[0] -1]);
				status = sl_read_list(data_type,
					    lists[selected_items[0] -1],
					    select_list);
				}
                        else
                                {
				status = SL_NO_SLISTS_SELECTED;
                                }
			break;

		case SL_BTN_CANCEL:
		default :
			status = CANCEL;
			break;
	      }
	     }
	}while(  ! normal );
	am_release_resource( id );
	ts_tcp_free( lists );
	fi_pop();
	return status;
}


/* **************************************************************************

	ROUTINE: SLZ_SELECT_INIT

	DESCRIPTION:
		This routine to define the display select list dialoug box.

	INPUT :
		LISTS =( CHAR **)The TCP of existing lists.
		NUM_ITEMS =(INT )The total number of name in the LISTS.
	OUTPUT :
		SUCCESS;
   ************************************************************************** */
static INT slz_select_init( lists, num_items, id)
CHAR *lists[];
INT num_items, *id;
{
	INT status = SUCCESS;
/* ************************************************************************** */

	PROGNAME( "slz_select_init");

	status = am_define_resource(AM_DIALOG,
				    "sl_select_cdlg",
				    SL_SELECT_LIST,
				    NULL_FUNCTION_PTR,
				    id,
				    NULL);

					/* set the texts to the selecting box.*/

	wisselt(*id, SL_SELECT_BOX, SL_SELECT_BAR, num_items, 1, lists);

	status = wiopcd(*id);
	return status;
}
                                                                                                                      
