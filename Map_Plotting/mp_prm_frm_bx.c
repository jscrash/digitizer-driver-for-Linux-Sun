/*  DEC/CMS REPLACEMENT HISTORY, Element MP_PRM_FRM_BX.C*/
/*  *4    23-OCT-1989 10:36:13 CONROY "(SPR 5043) Mods for new MAPPING UI programmer tools"*/
/*  *3    10-OCT-1989 14:21:16 GORDON "(SPR 4999) Mods for new lookup method"*/
/*  *2    26-JUN-1989 10:59:15 GILLESPIE "(SPR 20) Fix castings"*/
/*  *1    19-JUN-1989 13:14:21 SYSTEM ""*/
/*  DEC/CMS REPLACEMENT HISTORY, Element MP_PRM_FRM_BX.C*/

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





/* * FILE DESCRIPTION * */
/* -------------------- */
/* ************************************************************************

   NAME: MP_PARM_FROM_BOX.C

   AUTHOR: JOHN CONROY

   DESCRIPTION: ROUTINE TO GET THE PARAMETER INPUT FROM THE PARAMETER
		BOX ON THE MAP DEFINITION DIALOG.

   ************************************************************************ */




/* * INCLUDE FILES * */
/* ----------------- */

#include "esi_c_lib.h"
#include "esi_gl_defs.h"
#include "esi_mp.h"
#include "esi_am.h"
#include "esi_mg.h"
#include "esi_md_dbdefs.h"
#include "esi_md.h"
#include "esi_ts.h"
#include "esi_wi.h"
#include "esi_lu.h"
#include "md_mapdef.rh"
#include "esi_md_msg.h"
#include "esi_md_err.h"


/* * FUNCTION TITLE * */
/* ------------------ */

INT mp_parm_from_box(edit_flag,key_info,parm_string,parmstruc)

/* * ARGUMENT LIST * */
/* ------------------ */

BOOL edit_flag;
MD_KEY_INFO *key_info;
CHAR parm_string[MD_MAX_PARM_STRING];
MD_PARM_STRUCT **parmstruc;

/* * VARIABLE LIST * */
/* ----------------- */

{

INT status;
MD_PARM_STRUCT *temp_parm;
MD_PARM_STRUCT *temp_parm2;
MD_STATE_WORKSPACE *md_state;
CHAR temp_string[MD_MAX_PARM_STRING];
RESOURCE_ID *cdlg_id;
INT *multi_parms;
INT nitems;
INT value;
INT event_class;
INT event_id;
INT event_item;
INT i;

/* ********************************************************************** */

am_open_workspace("CDLG",AM_APPLICATION,(VOIDPTR *)&cdlg_id);
am_open_workspace("MAPDEF_STATE",AM_GLOBAL,(VOIDPTR *)&md_state);


/* * SET BUTTONS * */
/* --------------- */

md_state->button_status[2] = ON;
mp_set_mapdef_buttons();

multi_parms = (INT *) am_allocate(AM_GLOBAL,
			md_state->no_of_parameters*(sizeof(INT)));



/****************************************************************************/

/* * GET THE PARAMETER FROM THE USER * */
/* ----------------------------------- */

wi_enable_dialog(cdlg_id->overlays_id,TRUE);
status = FAIL;
while (status EQUALS FAIL)
    {
    wi_request_event(0,&event_class,&event_id,&event_item);
    if (event_id EQUALS cdlg_id->overlays_id)
	{
	if (event_item EQUALS MD_PARM_ADD_BTN)
	    {
	    wi_query_selections(cdlg_id->overlays_id,MD_PARM_SELECT,&nitems,multi_parms);
	    if (nitems EQUALS 0)
		{
		status = MD_PLEASE_SELECT;
		}
	    else if (	   (key_info->val_code EQUALS TLU$  OR 
			    key_info->val_code EQUALS PLU$  OR
			    key_info->val_code EQUALS UOM$  OR
			    key_info->val_code EQUALS SLE$) AND (nitems != 1) )
		{
		status = MD_SELECT_ONE;
		}
	    else
		{
		status = SUCCESS;
		temp_parm2 = NULL_PARM;
		strcpy(temp_string,"");

		for (i=0; i<nitems; i++)
		    {
		    temp_parm = (MD_PARM_STRUCT *) 
				am_allocate(AM_GLOBAL,sizeof(MD_PARM_STRUCT));
		    temp_parm->nxtparm = temp_parm2;
		    temp_parm2 = temp_parm;
		    value = multi_parms[i] - 1;

		    if (key_info->translate)
			{
			temp_parm->parmtype = 2;
			lu_text_to_index( md_state->keyword,
			    &temp_parm->parmval.ival, 
			    md_state->parm_list[value] );
			}
		    else
			{
			temp_parm->parmtype = 1;
			temp_parm->parmval.cval = (CHAR *)am_allocate(AM_GLOBAL,
					strlen(md_state->parm_list[value])+1);
			strcpy(temp_parm->parmval.cval,md_state->parm_list[value]);
			}

		    temp_parm->nxtparm = NULL_PARM;

		    strcat(temp_string,md_state->parm_list[value]);
		    if (i != nitems-1)
			{
			strcat(temp_string,",");
			}
		    }

		if (	    key_info->val_code EQUALS PLU$  OR
			    key_info->val_code EQUALS PLIST$  OR
			    key_info->val_code EQUALS SLE$)
		    {
		    ts_tcp_free(md_state->parm_list);
		    }
		}
	    }

	else if (event_item EQUALS MD_PARM_CANCEL)
	    {
	    status = CANCEL;
	    }

	else
	    {
	    wi_beep(1);
	    break;
	    }
	}


/* * EVENT NOT FROM PARAMETER BOX BUTTONS * */
/* ---------------------------------------- */

    else
	{
	wi_beep(1);
	}


/* DETERMINE STATUS AND NEXT ACTION * */
/* ---------------------------------- */

    if (status EQUALS SUCCESS)
	{
	md_free_parms(*parmstruc);
	*parmstruc = temp_parm;
	strcpy(parm_string,temp_string);
	}
    else if (status != CANCEL AND status != FAIL)
	{
	wi_alert(0,"","PARAMETER ENTRY ERROR:",mg_message(status));
	status = FAIL;
	}

    }

am_free(multi_parms);
return status;
}
