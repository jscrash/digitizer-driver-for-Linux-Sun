/*  DEC/CMS REPLACEMENT HISTORY, Element MP_CLOSE_MAP.C*/
/*  *3     2-AUG-1990 14:23:03 JULIAN "(SPR 0) remove display map if not synchronized"*/
/*  *2    26-JUN-1989 10:52:46 GILLESPIE "(SPR 20) Fix castings"*/
/*  *1    19-JUN-1989 13:10:31 SYSTEM ""*/
/*  DEC/CMS REPLACEMENT HISTORY, Element MP_CLOSE_MAP.C*/

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

   NAME: MP_CLOSE_MAP.C

   AUTHOR: JOHN CONROY

   DESCRIPTION:	ROUTINE TO CLOSE A MAP DEFINITION. 



   ************************************************************************ */




/* * INCLUDE FILES * */
/* ----------------- */

#include "esi_c_lib.h"
#include "esi_md_defs.h"
#include "esi_md_msg.h"
#include "esi_mp.h"
#include "esi_wi.h"
#include "esi_am.h"
#include "esi_asp_yesno.h"
#include "esi_conf_ws.h"
#include "md_mapdef.rh"
#ifndef ESI_MG_H
#include "esi_mg.h"
#endif


/* * FUNCTION TITLE * */
/* ------------------ */

publicdef INT mp_close_map()

/* * ARGUEMENT LIST * */
/* ------------------ */


/* * VARIABLE LIST * */
/* ----------------- */

{
INT status = SUCCESS;
MAP_STRUCTURE *mapdef;
MAP_STRUCTURE *new_map;
ASP_YESNO_WORKSPACE yesno;
RESOURCE_ID *cdlg_id;
INT i;
INT nlen;
MAP_NAME temp_name;
MAP_NAME mapname;
CHAR number[4];
CONFIGURATION_WORKSPACE *config;

/* ********************************************************************** */


/* * CHECK FOR CHANGES TO BE SAVED * */
/* --------------------------------- */

am_open_workspace("MAPDEF",AM_APPLICATION,(VOIDPTR *)&mapdef);
if (mapdef->change_flag)
    {
    strcpy(yesno.question,mg_message(MD_SAVE_MAP_MSG));
    yesno.cancel = FALSE;
    asp_yesno(&yesno);
    if (yesno.answer EQUALS 1)
	{
	status = mp_save_map_pipe();
	}
    }


/* * SET WINDOW HEADER * */
/* --------------------- */

if (status EQUALS SUCCESS)
    {
    am_open_workspace("CDLG",AM_APPLICATION,(VOIDPTR *)&cdlg_id);
    wi_query_dialog_item(cdlg_id->overlays_id,MD_MAP_NAME,&nlen,mapname);
    if (strcmp(mapdef->map_name,mapname) != 0)
	{
	strcpy(mapdef->map_name,mapname);
	if (mapdef->map_window_id != 0)
	    {
	    mp_window_header();
	    }
	}

/* * TRANSFER MAPDEF * */
/* ------------------- */

    mp_store_workspace();
    mp_set_current_map( (MAP_STRUCTURE *)0 );

/* * CLOSE MAP DEFINITION DIALOG * */
/* ------------------------------- */

    am_open_workspace("CDLG",AM_APPLICATION,(VOIDPTR *)&cdlg_id);
    wi_close_dialog(cdlg_id->overlays_id);
    wi_open_dialog(cdlg_id->base_dialog_id);
    }

return status;
}
