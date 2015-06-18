/* DEC/CMS REPLACEMENT HISTORY, Element GB_WHEREAMI.C*/
/* *6    28-JUN-1990 15:36:25 KEE "(SPR -1) Add am_declare(Zoom);"*/
/* *5    18-NOV-1989 10:02:42 GILLESPIE "(SPR 5075) Global overhaul"*/
/* *4    13-NOV-1989 13:51:54 JULIAN "(SPR 1) Integrate external tablet functions into finder."*/
/* *3    10-OCT-1989 09:52:12 JULIAN "(SPR 5020) mods for CT parameter change"*/
/* *2    26-JUN-1989 10:45:19 GILLESPIE "(SPR 20) Fix castings"*/
/* *1    19-JUN-1989 12:52:22 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element GB_WHEREAMI.C*/

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

/* GB_WHEREAMI.CC,  GUY>GB>SOURCE,  GUY H BINNS,  10/02/86
   Global Function 'Where am I ?'
   Copyright (c) 1986, Exploration Systems Inc, Sausalito, CA. */

/* TITLE : GB_WHEREAMI - Global Function 'Where am I ?' */

#include "esi_c_lib.h"
#include "esi_gb.h"
#ifndef ESI_AM_H
#include "esi_am.h"
#endif
#ifndef ESI_MP_H
#include "esi_mp.h"
#endif
#ifndef ESI_CT_H
#include "esi_ct.h"
#endif
#ifndef ESI_QS_H
#include "esi_qs.h"
#endif
#ifndef ESI_WI_H
#include "esi_wi.h"
#endif
#ifndef ESI_WM_H
#include "esi_wm.h"
#endif
#ifndef ESI_WM_CLS_H
#include "esi_wm_cls.h"
#endif
#ifndef ESI_GB_MSG_H
#include "esi_gb_msg.h"
#endif
#ifndef ESI_MG_H
#include    "esi_mg.h"
#endif
#ifndef ESI_TS_H
#include "esi_ts.h"
#endif
#ifndef ESI_TU_H
#include "esi_tu.h"
#endif

#include "gb_whereami.rh"

#if USE_PROTOTYPES
static INT gbz_set_projection_prompt(INT id,
    PROJECTION_STRUCTURE *current_projection, CHAR *format);
#else
static INT gbz_set_projection_prompt();
#endif

static CHAR DIALOG_FILE[] = "gb_whereami";

/* Our local buttons: */

#define GBZ_BUTTON_LOCATE            1
#define GBZ_BUTTON_CHANGE_PROJECTION 2


/* NOTE: These arrays are all mapped to eachother. The menu choice string "Zoom In" is 
    selected by button number WM_BUTTON_ZOOM, and is displayed in the Mac dialog box
    field number ITEM_WAI_ZOOMIN.  Changing one array necessitates keeping the other
    2 in sync. */

static CHAR *menu_choices[] = 
    {
    "Locate",
    "Change Projection",
    "Zoom In",
    "Pan to Center",
    "Cancel",
    "Done",
    "Reset/Unzoom",
    NULL
    };

static INT buttons[] = 
    {
    GBZ_BUTTON_LOCATE,
    GBZ_BUTTON_CHANGE_PROJECTION,
    WM_BUTTON_ZOOMIN,
    WM_BUTTON_PAN,
    WM_BUTTON_CANCEL,
    WM_BUTTON_DONE,
    WM_BUTTON_RESET
    };

static INT fields[] = 
    {
    ITEM_WAI_LOCATE,
    ITEM_WAI_CHANGE_PROJECTION,
    ITEM_WAI_ZOOMIN,
    ITEM_WAI_PAN,
    ITEM_WAI_CANCEL,
    ITEM_WAI_DONE,
    ITEM_WAI_RESET
    };

/* === GB_WHEREAMI == */

publicdef INT gb_whereami(dummy)
BYTE *dummy;   /* an empty pointer passed by the BOZO bus */

{
    INT status;
    INT id;
    INT win, tnr;
    INT i;
    INT curr_map_num;
    INT selected[1];
    INT appSave, winSave, tnrSave, groSave;
    INT class;
    INT button ;
    INT *nmaps;
    INT ii;
    CHAR *prompt, dbsStr[WM_DBS_STR_SIZE], usrStr[WM_USR_STR_SIZE] ;
    CHAR itemStr[STMAX] ;
    CHAR **proj_list = (CHAR **)0;
    CHAR coord_format[21];
    FLOAT wx, wy ;
    DOUBLE x_coord,y_coord;
    PROJECTION_STRUCTURE *current_projection;
    PROJECTION_STRUCTURE *map_projection;
    MAP_STRUCTURE *mp = (MAP_STRUCTURE *)0;
    BOOL isamap = FALSE;
    BOOL transform = FALSE;

/************************************************************************/

    fi_push();
    wmowhere( &appSave, &winSave, &tnrSave, &groSave, &status ) ;
    wm_inq_front_window(&win);
    if ( (status = wmwopen(win)) != SUCCESS)
	{
	goto finished;
	}


/* * SEE IF WINDOW IS A MAP * */
/* -------------------------- */

			     /* GET NUMBER OF DEFINED MAPS 		    */

    status = am_open_workspace("MAP_NUMBER",AM_GLOBAL,(VOIDPTR *)&nmaps);
    if (status EQUALS SUCCESS  AND  *nmaps > 0  AND  win > 0)
	{
			     /*	SAVE THE CURRENT MAP NUMBER		    */

	qs_inq_i(QS_CUR_MAP_NUMBER,&curr_map_num);

			    /* LOOP THRU DEFINED MAPS FOR WINDOW ID	    */

	status = curr_map_num + (*nmaps);
	for (i=curr_map_num; i<status; i++)
	    {
	    id = (i <= *nmaps) ? i : i%(*nmaps);
	    qs_set_i(QS_CUR_MAP_NUMBER,id);
	    if ( (mp_get_current_map(&mp) EQUALS SUCCESS)  AND
				    (mp->map_window_id EQUALS win) )
		{
		isamap = TRUE;
		qs_set_c(QS_MAP_PROJECTION,mp->map_projection,
						strlen(mp->map_projection));
		ct_get_projection(mp->map_projection,&map_projection);
		current_projection = map_projection;
		wmtselect(mp->map_tnr);
		break;
		}
	    }
	}


/* * IF WINDOW NOT A MAP, SET TRANSFORMATION * */
/* ------------------------------------------- */

    if (NOT isamap)
	{
	class = WM_CLASS_TNR ;
	id = 1 ;
	wmtselector(mg_message(GB_SELECT_XFORM),class,id,&i,&tnr,&status ) ;
	if (status EQUALS SUCCESS)
	    {
	    wmtselect( tnr ) ;
	    }
	else
	    {
	    goto finished;
	    }
	}




/* * PUT UP THE DIALOG BOX * */
/* ------------------------- */
	am_declare("Zoom");
    am_define_resource(AM_DIALOG, DIALOG_FILE, CDLG_WAI, NULL_FUNCTION_PTR,
		 &id, NULL) ;

    wm_set_mouse_menu( menu_choices, buttons );

    for( ii=0; menu_choices[ii] != (CHAR*)0; ++ii )
	{
	sprintf(usrStr, "%X = %s", buttons[ii], menu_choices[ii]);
	wi_set_dialog_text(id, fields[ii], usrStr);
	}


			     /*	Set the name of the current window in the   */
			     /*	dialog box				    */

    wmoinq( win, &class, dbsStr, usrStr ) ;
    wi_set_dialog_text( id, ITEM_WAI_WINDOW, usrStr ) ;

    if (isamap)
	{
	gbz_set_projection_prompt(id,current_projection,coord_format);
	}
    else
	{
	wmoinq( tnr, &class, dbsStr, usrStr ) ;
	wi_set_dialog_text( id, ITEM_WAI_TNR, usrStr ) ;
	strcpy(coord_format,"%9.2f");
	}

    wi_set_dialog_text( id, ITEM_WAI_X, "0.0" ) ;
    wi_set_dialog_text( id, ITEM_WAI_Y, "0.0" ) ;

    wi_open_dialog( id ) ;


/* * ACTION LOOP, CONTINUE UNTIL "CANCEL" OR "DONE" * */
/* -------------------------------------------------- */

    while (button != WM_BUTTON_CANCEL AND button != WM_BUTTON_DONE)
	{
	wmtlocate( &status, &wx, &wy, &button );
	switch (button)
	    {
	    case WM_BUTTON_CANCEL: 
	    case WM_BUTTON_DONE:
		break;
		
	    case GBZ_BUTTON_CHANGE_PROJECTION:
		if (isamap)
		    {
			    /*	CHOOSE A NEW PROJECTION			    */

		    ct_projection_list(&proj_list, &i, FALSE);
		    wi_ssst("", i, 1, proj_list);
		    wi_request_selection("Select a cartographic projection",
						    &i, selected, &status);

			     /*	MAKE IT THE CURRENT ONE			    */

		    if (status EQUALS SUCCESS)
			{
			ct_get_projection(proj_list[selected[0] -1],&current_projection);
			gbz_set_projection_prompt(id,current_projection,
								coord_format);
			transform = (current_projection != map_projection);
			}
		    ts_tcp_free(proj_list);
		    break;
		    }

			     /*	DISPLAY LOCATION			    */
	    case GBZ_BUTTON_LOCATE:
	    default:
		if (transform)
		    {
		    x_coord=wx;
		    y_coord=wy;
		    ct_transform_point(x_coord,y_coord, map_projection,
					&x_coord,&y_coord, current_projection);
		    wx=x_coord;
		    wy=y_coord;
		    }
		sprintf(itemStr, coord_format, wx) ;
		wi_set_dialog_text( id, ITEM_WAI_X, itemStr ) ;
		sprintf(itemStr, coord_format, wy);
		wi_set_dialog_text( id, ITEM_WAI_Y, itemStr ) ;
		break;
	    }
	}
    am_release_resource( id ) ;


/* * RESTORE CURRENT MAP * */
/* ----------------------- */
	
    if (isamap  AND  (curr_map_num != mp->map_number))
	{
	qs_set_i(QS_CUR_MAP_NUMBER,curr_map_num);
	if (mp_get_current_map(&mp) EQUALS SUCCESS)
	    {
	    qs_set_c(QS_MAP_PROJECTION,mp->map_projection,
						strlen(mp->map_projection));
	    }
	}


/* * RETURN * */
/* ---------- */

finished:
    wmwclose(win);
			     /*	Reset the window manager to the previous    */
			     /*	state					    */
    wmoattach( appSave, winSave, tnrSave, groSave, &status ) ;
    am_quit(); /* a mate for AM_START done by BOZO bus */
    fi_pop();  /* a mate for FI_PUSH above */
    return status;
}

/******************************************************************************/

static INT gbz_set_projection_prompt(id, current_projection, coord_format)
INT id;
PROJECTION_STRUCTURE *current_projection;
CHAR *coord_format;
{

    UOM_NAME uom;

    wi_set_dialog_text( id, ITEM_WAI_TNR, current_projection->projection_id) ;
    if (strcmp(current_projection->projection_id, "GEODETIC") EQUALS 0)
	{
	wi_set_dialog_text(id, ITEM_WAI_X_LONG_PROMPT, "Longitude:");
	wi_set_dialog_text(id, ITEM_WAI_Y_LAT_PROMPT, "Latitude:");
	strcpy(coord_format, "%9.5f ");
	}
    else
	{
	wi_set_dialog_text(id, ITEM_WAI_X_LONG_PROMPT, "        X:");
	wi_set_dialog_text(id, ITEM_WAI_Y_LAT_PROMPT, "        Y:");
	strcpy(coord_format, "%9.1f ");
	}

    ct_get_projection_uom(current_projection->projection_id,uom);
    strcat(coord_format,uom);
    wi_set_dialog_text(id, ITEM_WAI_X, "0.0");
    wi_set_dialog_text(id, ITEM_WAI_Y, "0.0");

    return SUCCESS;
}
