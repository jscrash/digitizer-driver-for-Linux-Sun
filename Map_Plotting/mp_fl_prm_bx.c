/*  DEC/CMS REPLACEMENT HISTORY, Element MP_FL_PRM_BX.C*/
/*  *8     6-JUL-1990 13:30:33 KEE "(SPR -1) Fixing bug in select list option - clear out MD_PARM_SELECT too"*/
/*  *7     5-JUL-1990 16:09:57 KEE "(SPR -1) Fixing bug in select list option - add empty list condition testing"*/
/*  *6    23-OCT-1989 10:35:10 CONROY "(SPR 5043) Mods for new MAPPING UI programmer tools"*/
/*  *5    20-OCT-1989 14:07:45 CONROY "(SPR 5030) copy keyword into allocated string"*/
/*  *4    20-OCT-1989 11:33:55 CONROY "(SPR 5030) remove include file esi_md_tlu, make use of lu routines"*/
/*  *3    10-OCT-1989 14:19:43 GORDON "(SPR 4999) Mods for new lookup method"*/
/*  *2    26-JUN-1989 10:55:47 GILLESPIE "(SPR 20) Fix castings"*/
/*  *1    19-JUN-1989 13:11:56 SYSTEM ""*/
/*  DEC/CMS REPLACEMENT HISTORY, Element MP_FL_PRM_BX.C*/
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
/* ************************************************************************

   NAME: MP_FILL_PARM_BOX.C

   AUTHOR: JOHN CONROY

   DESCRIPTION: ROUTINE TO FILL THE PARAMETER SELECTOR BOX IF THIS IS
		THE APPROPRIATE PARAMETER TYPE.

   ************************************************************************ */
#include    "esi_math.h"
#include    "esi_c_lib.h"
#include    "esi_mp.h"
#include    "esi_lu.h"
#include    "esi_am.h"
#include    "esi_mg.h"
#include    "esi_wi.h"
#include    "esi_nl.h"
#include    "esi_ho.h"
#include    "esi_ts.h"
#include    "esi_md_dbdefs.h"
#include    "esi_md_msg.h"
#include    "esi_md_err.h"

#include    "md_mapdef.rh"
                               

publicdef INT mp_fill_parm_box(temp_key_info)
MD_KEY_INFO *temp_key_info;

{
INT status;
MD_STATE_WORKSPACE *md_state;
FILENAME select_file;
INT i;
INT no_parms;
INT nparms;
INT id;
RESOURCE_ID *cdlg_id;
CHAR **tcp;
INT size;
static BOOL inited = FALSE;
static struct {
		    INT nkeys;
		    CHAR **keywords;
		    INT *obj_id;
		  } lu;

status = SUCCESS;


/* * OPEN THE WORKSPACE * */
/* ---------------------- */

am_open_workspace("MAPDEF_STATE",AM_GLOBAL,(VOIDPTR *)&md_state);
am_open_workspace("CDLG",AM_APPLICATION,(VOIDPTR *)&cdlg_id);
id = cdlg_id->overlays_id;


/* * FILL THE PARAMETER SELECTOR BOX (IF NEEDED) * */
/* ----------------------------------------------- */

switch (temp_key_info->val_code)
    {
    case TLU$: case TLIST$:
	if ( (status = lu_get_tcp(temp_key_info->val_arg,&tcp,&size))
							    EQUALS SUCCESS)
	    {
	    if (NOT inited)
		{
		lu_num_of_keywords(&i);
		lu.keywords = ts_tcp_alloc(i);
		lu.nkeys = 0;
		lu.obj_id = (INT *) tc_alloc(i*sizeof(INT));
		inited = TRUE;
		}

	    for (i=0; i<lu.nkeys; i++)
		{
		if (ARE_SAME(temp_key_info->val_arg,lu.keywords[i]))
		    {
		    break;
		    }
		}

	    if (i EQUALS lu.nkeys)
		{
		lu.nkeys++;
		lu.keywords[i] = 
			(CHAR *) tc_alloc(strlen(temp_key_info->val_arg)+1);
		strcpy(lu.keywords[i],temp_key_info->val_arg);
		wi_define_text_object(tcp,size,&(lu.obj_id[i]));
		}

	    strcpy(md_state->keyword,temp_key_info->val_arg);
	    md_state->parm_list = tcp;
	    md_state->no_of_parameters = size;
	    if (md_state->last_parm_obj EQUALS 0  OR
				lu.obj_id[i] != md_state->last_parm_obj)
		{
		md_state->last_parm_obj = lu.obj_id[i];
		nparms = (temp_key_info->val_code EQUALS TLU$) ? 1 : size;
		wi_set_selections_obj(id,MD_PARM_SELECT,MD_PARM_SCROLL,
					    size,nparms,lu.obj_id[i]);
		}
	    }
	else
	    {
	    am_message( AM_ALERT, mg_message(status) );
	    wi_set_selections(id,MD_PARM_SELECT,MD_PARM_SCROLL,0,0,(CHAR **)0);
	    }
	break;


    case PLU$: case PLIST$:
	mp_project_parm_list(temp_key_info,&(md_state->parm_list),&no_parms);
	if (no_parms EQUALS 0)
	    {
	    status = MD_NO_PARAMETERS;
	    break;
	    }

	md_state->no_of_parameters = no_parms;
	md_state->last_parm_obj = 0;

	nparms = (temp_key_info->val_code EQUALS PLU$) ? 1 : no_parms;
	wi_set_selections(id,MD_PARM_SELECT,MD_PARM_SCROLL,no_parms,
						nparms,md_state->parm_list);
	break;

    case SLE$:
	sl_inq_slist_names(temp_key_info->val_arg, &md_state->parm_list,
	    &no_parms);
    if (no_parms != 0 && md_state->parm_list != (TCP) 0)
      {
        md_state->no_of_parameters = no_parms;
        md_state->last_parm_obj = 0;
        wi_set_selections(id,MD_PARM_SELECT,MD_PARM_SCROLL,no_parms,
                          1,md_state->parm_list);
      }
    else
      {
        wi_set_dialog_text(id,MD_PARM_SELECT,""); 
        /* clear the MD_PARM_SELECT selector */
      }
	break;

    case UOM$:
	i = atol(temp_key_info->val_arg);   
	tu_get_tcp(&(md_state->parm_list),i);
	
	md_state->no_of_parameters = ts_tcp_len(md_state->parm_list);
	md_state->last_parm_obj = 0;

	wi_set_selections(id,MD_PARM_SELECT,MD_PARM_SCROLL,
			    md_state->no_of_parameters,1,md_state->parm_list);
	break;


    default:
	status = MD_NON_FILL_OPTION;
	break;
    }


return status;
}
