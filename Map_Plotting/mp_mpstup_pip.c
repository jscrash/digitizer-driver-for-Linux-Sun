/*  DEC/CMS REPLACEMENT HISTORY, Element MP_MPSTUP_PIP.C*/
/*  *31   15-AUG-1991 18:48:22 CHARLIE "(SPR -1) new handling of corner and scale edittext boxes"*/
/*  *30   29-JUL-1991 10:08:11 CHARLIE "(SPR -1) fix uom checking, cleanup"*/
/*  *29   18-JUN-1991 14:39:06 CHARLIE "(SPR -1) fix xy bug;enhance uom checks"*/
/*  *28   11-JUN-1991 12:54:16 JULIAN "(SPR 0) change parse_dms to publicdef"*/
/*  *27   13-MAY-1991 18:18:10 MING "(SPR -1) more fix from charlie"*/
/*  *26    3-MAY-1991 13:36:31 MING "(SPR -1) fix miscellous bugs from charlie"*/
/*  *25    6-NOV-1990 11:27:26 JULIAN "(SPR 6086) fix bugs in map setup"*/
/*   23B1  3-OCT-1990 12:06:20 GILLESPIE "Change file type to stream"*/
/*  *24   20-AUG-1990 17:08:03 KEE "(SPR -1) fixing compile error at sun"*/
/*  *23   26-JUL-1990 18:29:56 JESSIE "(SPR 0) modify for map setup"*/
/*  *22   12-JUL-1990 17:42:31 GILLESPIE "(SPR 3) Mis-spelled ARE_DIFFERENT; if only compiler would REQUIRE prototypes..."*/
/*  *21   12-JUL-1990 13:02:42 GILLESPIE "(SPR 3) Move ct_decimal_degrees conversion routines to their own files; use LATLONG"*/
/*  *20    3-MAY-1990 09:26:42 WALTERS "(SPR 5196) Add Seconds to DMS extents, improve DMS handling, increase percision."*/
/*  *19   26-MAR-1990 14:43:40 WALTERS "(SPR 0) Remove temp fix for selector dimming problem"*/
/*  *18   23-MAR-1990 18:06:00 WALTERS "(SPR 0) Temp fix because coordinate selctor didn't dim properly"*/
/*  *17   22-MAR-1990 01:22:38 GILLESPIE "(SPR 1) Change %f to %lf in sscanf (since we are translating doubles...)"*/
/*  *16   19-MAR-1990 16:18:09 WALTERS "(SPR 0) Display decimal degrees with 3 decimal places"*/
/*  *15   16-MAR-1990 08:10:51 WALTERS "(SPR 0) Move STRM message to messages table"*/
/*  *14   14-MAR-1990 12:01:48 WALTERS "(SPR 0) Fix map size bug -- size was not converted if units not inches or cm."*/
/*  *13   13-MAR-1990 16:06:15 WALTERS "(SPR 0) Fix section-township-range bugs"*/
/*  *12   12-MAR-1990 12:11:18 WALTERS "(SPR 5113) Don't wi_enable_dialog_item for MD_COORD_SELECT (X has problem with this)"*/
/*  *11    7-MAR-1990 17:38:02 WALTERS "(SPR 0) Remove FLOAT types from system"*/
/*  *10   23-FEB-1990 09:26:36 WALTERS "(SPR 0) ct_trans_pt from proj_projection to map_projeciton after mr_get_section_corners"*/
/*  *9    21-FEB-1990 13:12:29 WALTERS "(SPR 0) Section-Township-Range-Meridian addition to map setup dialog"*/
/*  *8    16-FEB-1990 09:09:31 WALTERS "(SPR 0) Fix things lint didn't like"*/
/*  *7    15-FEB-1990 13:25:36 WALTERS "(SPR 0) Fix call to ct_get_projection_uom"*/
/*  *6    15-FEB-1990 10:45:10 WALTERS "(SPR 0) Changes in map setup dialog"*/
/*  *5    10-OCT-1989 16:58:30 CONROY "(SPR 5028) Mods for new plotting capability"*/
/*  *4    10-OCT-1989 09:58:39 JULIAN "(SPR 5020) mods for CT parameter change"*/
/*  *3     7-SEP-1989 09:41:07 GORDON "(SPR 200) Gulf changes (August trip)"*/
/*  *2    26-JUN-1989 10:57:37 GILLESPIE "(SPR 20) Fix castings"*/
/*  *1    19-JUN-1989 13:13:27 SYSTEM ""*/
/*  DEC/CMS REPLACEMENT HISTORY, Element MP_MPSTUP_PIP.C*/

/******************************************************************************

                Copyright Finder Graphics Systems, Inc. 1990
		   Unpublished -- All rights reserved

THIS SOFTWARE IS THE PROPRIETARY PROPERTY OF Finder Graphics Systems, Inc.  AND
MAY CONTAIN CONFIDENTIAL TRADE SECRET INFORMATION. IT IS LICENSED FOR USE ON THE
DESIGNATED EQUIPMENT ON WHICH IT WAS ORIGINALLY INSTALLED AND  MAY NOT BE
MODIFIED, DUPLICATED OR COPIED IN ANY FORM WITHOUT PRIOR WRITTEN CONSENT OF

			Finder Graphics Systems, Inc.
			201 Tamal Vista Blvd
			Corte Madera, CA  USA 94925
			(415) 927-0100

(The above notice does not constitute an admission of publication or
unrestricted dissemination of the work)

******************************************************************************/


/* * FILE DESCRIPTION *                                                             */
/* --------------------                                                             */
/* ************************************************************************

   NAME: MP_MAP_SETUP_PIPE.C

   AUTHOR: JOHN CONROY

   DESCRIPTION: ROUTINE TO DEFINE, OPEN, AND SERVICE THE BLACK PIPE
        MAP SETUP DIALOG.


   ************************************************************************ */

/* Modification History

	April, 1991	(CEP)	Significant re-write avoiding roundoff errors,
				goofy event handling
	July,  1991	(CEP)	Enhance UOM checking; delete old code for handling
				map scales like 1 inch = 5000 feet since it wasn't
				supported consistently in this module;
				prop up error handling
	August, 1991	(CEP)	Converted EditText boxes to StatText boxes;
				enforced event handling for map scale and
				map limits. Commented out ON/OFF calls to
				grouped radio buttons.

	To do:		USPLS coordinates no longer need to be held in 'map'
			projection, they may be left in project projection
			as retrieved from the database. However, time contraints
			suggest releasing as is. (CEP)
*/


/* * INCLUDE FILES *                                                                */
/* -----------------                                                                */

#include "esi_c_lib.h"
#include "esi_mp.h"

#ifndef ESI_GL_PROJECT_H
#include "esi_gl_project.h"
#endif

#ifndef ESI_GL_TOP_H
#include "esi_gl_top.h"
#endif

#ifndef ESI_CT_H
#include "esi_ct.h"
#endif

#ifndef ESI_AM_H
#include "esi_am.h"
#endif

#ifndef ESI_QS_H
#include "esi_qs.h"
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

#ifndef ESI_TU_H
#include "esi_tu.h"
#endif

#ifndef ESI_MD_MSG_H
#include "esi_md_msg.h"
#endif

#ifndef ESI_MP_MSG_H
#include "esi_mp_msg.h"
#endif

#ifndef ESI_MD_ERR_H
#include "esi_md_err.h"
#endif

#ifndef ESI_MG_H
#include "esi_mg.h"
#endif

#include "md_mapdef.rh"

/* some 'defines' for current coordinate system flag */
#define NON_GEODETIC	 (INT)0
#define GEODETIC_DMS     (INT)1
#define GEODETIC_DECIMAL (INT)2

/* internal margin calculations. Not sure if this pertains to the new plot
   layout facility or just accounting for labels outside graticule limits */
#define LEFT_MARGIN (DOUBLE)1.0
#define RIGHT_MARGIN (DOUBLE)1.0
#define TOP_MARGIN (DOUBLE)3.5
#define BOTTOM_MARGIN (DOUBLE)1.5

static PROJECTION_STRUCTURE *current_projection;	/* map projection */
static PROJECTION_STRUCTURE *display_projection;	/* coord system */
static PROJECTION_STRUCTURE *project_projection;	/* limits in proj. proj'tion */

#if USE_PROTOTYPES
static INT parse_uom(CHAR * unit, CHAR * uom1, CHAR * uom2);
static INT mp_edit_scale_dialog(CHAR *scale_string, DOUBLE *scale, CHAR *scale_uom);
static INT mp_edit_corners_dialog(CHAR *ll_string, CHAR *ur_string,
	INT coord_type, INT geodetic_flag, DOUBLE *ll_pointx, DOUBLE *ll_pointy,
	DOUBLE *ur_pointx, DOUBLE *ur_pointy);
#else
static INT parse_uom();
static INT mp_edit_scale_dialog();
static INT mp_edit_corners_dialog();
#endif

/* * FUNCTION TITLE *                                                               */
/* ------------------                                                               */

#if USE_PROTOTYPES
publicdef INT mp_map_setup_pipe(void)
#else
publicdef INT mp_map_setup_pipe()
#endif

/* * VARIABLE LIST *                                                                */
/* -----------------                                                                */
{

    INT status, stat;		/* general status variables */
    RESOURCE_ID *cdlg_id;	/* for map setup dialog id */
    MAP_STRUCTURE *mapdef;	/* current open map definition */
    INT id;			/* dialog id */
    INT event_class, event_id, event_item;	/* for wi_request_event */
    CHAR text[60];		/* used for parsing text edit boxes */
    TCP projection_list;	/* map projection list */
    TCP coordinate_list;	/* map coordinate system list */
    INT nitems, nitems_coord, selected;	/* counts of items */
    INT len;			/* for strlen */
    INT i, nd = -1, nd2 = -1, display_index;	/* misc. indexes */
    CHAR lower_left_ext[60], upper_right_ext[60];	/* 'external' static text */
    CHAR dia_map_scale_ext[60];
    INT map_coord_sys_flag, dia_coord_sys_flag;	/* code for USPLS or XY */
    INT dia_scale_sizes;	/* local 'state' variables */
    INT dia_with_borders;
    INT dia_in_inches;
    CHAR dia_map_scale_uom[60];
    CHAR dia_map_coordinate_projection[60];
    DOUBLE dia_map_scale;
    CHAR hold_ll_strm[60], hold_ur_strm[60];	/* USPLS strings (temp) */
    DOUBLE map_size_x, map_size_y;	/* map size dimensions */
    CHAR out_lon[60], out_lat[60];	/* temp output variables */
    CHAR units_left[60], units_right[60];	/* map_scale_uom */
    DOUBLE dummy, temp;
    DOUBLE ll_x, ll_y, ur_x, ur_y;	/* local map size calculations */
    DOUBLE l_proj_ll_x, l_proj_ll_y;	/* local current coords in proj projection */
    DOUBLE l_proj_ur_x, l_proj_ur_y;
    UOM_NAME uom;		/* unit conversion */
    UOM_STRUCTURE uom_code_1, uom_code_2;	/* map scale (feet/inch) */
    INT geodetic_display;	/* geodetic/xy flag */
    XYNODE ll_point, ur_point;
    LATLONG latitude, longitude;

 /* **********************************************************************       */

    status = SUCCESS;

    fi_push();	/* this is black pipe dialog */

 /* * OPEN NEEDED WORKSPACES *                                                   */
 /* --------------------------                                                   */

    am_open_workspace("CDLG", AM_APPLICATION, (VOIDPTR *) & cdlg_id);
    am_open_workspace("MAPDEF", AM_APPLICATION, (VOIDPTR *) & mapdef);

 /* * GET THE PROJECTIONS TCP *                                                  */
 /* ---------------------------                                                  */

    ct_projection_list(&projection_list, &nitems, TRUE);

 /* * PUT UP THE MAP SETUP DIALOG *                                              */
 /* -------------------------------                                              */
 /* this shouldn't be able to fail as the dialog is defined as mapping is inited */
    if (cdlg_id->map_setup_id EQUALS 0)
    {
	status = am_define_resource(AM_DIALOG, "MD_MAPDEF", MD_MAP_SETUP,
	    NULL_FUNCTION_PTR, &cdlg_id->map_setup_id, NULL);
	if (status != SUCCESS)
	{
	    am_message(AM_ALERT, mg_message(status));
	    status = FAIL;
	    goto finished;
	}
    }
    id = cdlg_id->map_setup_id;

 /* * INIT THE MESSAGE FIELD *                                                   */
 /* --------------------------                                                   */

    wi_set_dialog_text(id, MD_SETUP_MSG, "");

 /* * INITIALIZE PROJECT PROJECTION STRUCTURE *                                  */
 /* ------------------------------------                                         */

    qs_inq_c(QS_PROJECT_PROJECTION, text, &len);
    ct_get_projection(text, &project_projection);

 /* * INIT MAP PROJECTION SELECTOR BOX *                                         */
 /* ------------------------------------                                         */

    for (i = 0; i < nitems; i++)
    {
	if (ARE_SAME(projection_list[i], mapdef->map_projection))
	{
	    nd = i + 1;
	    break;
	}
    }
/* check for catastrophic failure here? like using a map not built for this proj? */
/* if (nd == -1)... \* error *\                                                   */
    wi_set_selections(id, MD_PROJ_SELECT, MD_PROJ_SCROLL, nitems, 1, projection_list);
    wi_set_default_sel(id, MD_PROJ_SELECT, nd);
    wi_activate(id, MD_PROJ_SELECT, ON);
    wi_set_selection_limit(id, MD_PROJ_SELECT, 1);

    ct_get_projection(mapdef->map_projection, &current_projection);

 /* * INIT THE COORDINATE SYSTEM SELECTOR BOX *                                  */
 /* -------------------------------------------                                  */

 /* add in the "Geodetic" options */
    ts_tcp_copy(&coordinate_list, projection_list);
    ts_tcp_append(&coordinate_list, "GEODETIC DMS");
    ts_tcp_append(&coordinate_list, "GEODETIC DEC");
    ts_tcp_sort(coordinate_list);
    nitems_coord = ts_tcp_len(coordinate_list);

    for (i = 0; i < nitems_coord; i++)
    {
	if (ARE_SAME(coordinate_list[i], mapdef->map_coordinate_system))
	{
	    nd2 = i + 1;
	    strcpy(dia_map_coordinate_projection, mapdef->map_coordinate_system);
	    break;
	}
    }
/* note: if it can't find coordinate system, which will happen when
   coordinate system is 'USPLS', it assumes coords. are in MAP projection
   which SHOULD be valid. It's a place to start if the user switches
   back to XY from USPLS (coords will be converted from project_projection to the
   current MAP projection. */

    if (nd2 < 0)
    {
	for (i = 0; i < nitems_coord; i++)
	{
	    if (ARE_SAME(coordinate_list[i], mapdef->map_projection))
	    {
		nd2 = i + 1;
		strcpy(dia_map_coordinate_projection, mapdef->map_projection);
		break;
	    }
	}
    }
/* check for catastrophic error here? should have been done at mp_create/open map */
/* if (nd2 == -1)... \* bad map projection was stored *\                          */
    wi_set_selections(id, MD_COORD_SELECT, MD_COORD_SCROLL, nitems_coord, 1,
	coordinate_list);
    wi_set_default_sel(id, MD_COORD_SELECT, nd2);
    wi_activate(id, MD_COORD_SELECT, ON);
    wi_set_selection_limit(id, MD_COORD_SELECT, 1);

 /* * INIT the 'COORDINATE SYSTEM' **                                         */
 /* ---------------------------------                                         */

    display_index = nd2 - 1;
    if (ARE_SAME(coordinate_list[display_index], "GEODETIC DMS"))
    {
	ct_get_projection("GEODETIC", &display_projection);
	geodetic_display = GEODETIC_DMS;
	wi_set_dialog_text(id, MD_COORD_FORMAT, "Format: Long,Lat");
    }
    else if (ARE_SAME(coordinate_list[display_index], "GEODETIC DEC"))
    {
	ct_get_projection("GEODETIC", &display_projection);
	geodetic_display = GEODETIC_DECIMAL;
	wi_set_dialog_text(id, MD_COORD_FORMAT, "Format: Long,Lat");
    }
    else
    {
	ct_get_projection(coordinate_list[display_index], &display_projection);
	geodetic_display = NON_GEODETIC;
	wi_set_dialog_text(id, MD_COORD_FORMAT, "Format: X,Y");
    }

 /* * INIT THE MAP SCALE BOX AND PLOTTING SIZE CHECK BOX *                       */
 /* ------------------------------------------------------                       */

    if (mapdef->map_scale != 0.0)
    {
	sprintf(dia_map_scale_ext, "%.0f %s", mapdef->map_scale,
	    mapdef->map_scale_uom);
	wi_set_dialog_text(id, MD_MAP_SCALE, dia_map_scale_ext);
    }
    else
	mapdef->map_scale = 1.0;
    dia_map_scale = mapdef->map_scale;
    strcpy(dia_map_scale_uom, mapdef->map_scale_uom);

    dia_scale_sizes = mapdef->scale_sizes_for_plotting;
    wi_set_dialog_value(id, MD_SIZE_SCALE, dia_scale_sizes);

 /* * INIT THE MAP SIZE BOX  *                                                   */
 /* --------------------------                                                   */

    status = SUCCESS;
    if (ARE_SAME(mapdef->map_coordinate_system, "USPLS"))
    {
    /* get proper mapdef->lower_left, mapdef->upper_right for future use */
	status = mr_get_section_corners(mapdef->lower_left_str,
	    &mapdef->lower_left_xy.x,
	    &mapdef->lower_left_xy.y, &dummy, &dummy);
	if (status != SUCCESS && IS_STRING(mapdef->lower_left_str))
	{
	    wi_beep(1);
/* (CP) make this bad lower left strm */
	    wi_set_dialog_text(id, MD_SETUP_MSG, mg_message(MD_BAD_STRM));
	}
	if (status == SUCCESS)
	{
	    status = mr_get_section_corners(mapdef->upper_right_str, &dummy, &dummy,
		&mapdef->upper_right_xy.x,
		&mapdef->upper_right_xy.y);
	    if (status != SUCCESS && IS_STRING(mapdef->upper_right_str))
	    {
		wi_beep(1);
/* (CP) make this bad upper right strm */
		wi_set_dialog_text(id, MD_SETUP_MSG, mg_message(MD_BAD_STRM));
	    }
	    if (status == SUCCESS)
	    {
		l_proj_ll_x = mapdef->lower_left_xy.x;
		l_proj_ll_y = mapdef->lower_left_xy.y;
		l_proj_ur_x = mapdef->upper_right_xy.x;
		l_proj_ur_y = mapdef->upper_right_xy.y;
	    /* * convert from project projection to current coordinate system * */
		if (ARE_DIFFERENT(current_projection->projection_id,
			project_projection->projection_id))
		{
		    ct_transform_point(mapdef->lower_left_xy.x, mapdef->lower_left_xy.y,
			project_projection, &mapdef->lower_left_xy.x,
			&mapdef->lower_left_xy.y, current_projection);
		    ct_transform_point(mapdef->upper_right_xy.x, mapdef->upper_right_xy.y,
			project_projection, &mapdef->upper_right_xy.x,
			&mapdef->upper_right_xy.y, current_projection);
		}
	    }
	}
    }

 /* keep local copy of map limits in PROJECT projection */
    else	/* !USPLS */
    {
       ct_transform_point(mapdef->lower_left_xy.x, mapdef->lower_left_xy.y,
       	current_projection, &l_proj_ll_x, &l_proj_ll_y, project_projection);
       ct_transform_point(mapdef->upper_right_xy.x, mapdef->upper_right_xy.y,
       	current_projection, &l_proj_ur_x, &l_proj_ur_y, project_projection);
    }
    /* set some defaults; even if status BAD */
    wi_set_dialog_value(id, MD_INCHES_RADIO, ON);
    wi_set_dialog_value(id, MD_BOARDERS_RADIO, ON);

 /* * parse map scale string to figure out map size in x and y * */
    if (status == SUCCESS)	/* we have good corner coords */
    {
	status = parse_uom(mapdef->map_scale_uom, units_left, units_right);
	if (status == SUCCESS)	/* both units right and left are good */
	{
	    tu_verify_uom(units_left, &uom_code_1);

	    ct_get_projection_uom(mapdef->map_projection, uom);
	    tu_verify_uom(uom, &uom_code_2);

	/* * convert from uom of map projection to units_left of scale string * */

	    tu_unit_conversion(mapdef->lower_left_xy.x, uom, &ll_x,
		uom_code_1.uom_name);
	    tu_unit_conversion(mapdef->lower_left_xy.y, uom, &ll_y,
		uom_code_1.uom_name);
	    tu_unit_conversion(mapdef->upper_right_xy.x, uom, &ur_x,
		uom_code_1.uom_name);
	    tu_unit_conversion(mapdef->upper_right_xy.y, uom, &ur_y,
		uom_code_1.uom_name);

	    map_size_x = (ur_x - ll_x) / mapdef->map_scale;
	    map_size_y = (ur_y - ll_y) / mapdef->map_scale;

	    tu_verify_uom(units_right, &uom_code_1);

/* (CP) ah what a wicked web we weave...especially when we use hard-coded integers
	with incorrect values...
        code was: if (abs(uom_code_1.uom_code) == 1)
*/
	    if (abs(uom_code_1.uom_code) == 3)	/* (CP) actual code for inches */
	    {
		wi_set_dialog_value(id, MD_INCHES_RADIO, ON);
		/* this should work automatically */
		/* wi_set_dialog_value(id, MD_CENTI_RADIO, OFF); */
		dia_in_inches = TRUE;
	    }
	    else if (abs(uom_code_1.uom_code) == 7)	/* code for centimeters */
	    {
		wi_set_dialog_value(id, MD_CENTI_RADIO, ON);
		/* this should be automatic */
		/* wi_set_dialog_value(id, MD_INCHES_RADIO, OFF); */
		dia_in_inches = FALSE;
	    }
	    else
	    /* * scale like 5000 meters/meter will be converted to inches * */
	    {
		wi_set_dialog_value(id, MD_INCHES_RADIO, ON);
		/* this shouldn't be necessary */
		/* wi_set_dialog_value(id, MD_CENTI_RADIO, OFF); */
		tu_unit_conversion((DOUBLE) map_size_x, uom_code_1.uom_name,
			&map_size_x, "IN");
		tu_unit_conversion((DOUBLE) map_size_y, uom_code_1.uom_name,
			&map_size_y, "IN");
		dia_in_inches = TRUE;
	    }
	/* note: this gets skipped now if saved map scale is bad */
	/* but ALWAYS was skipped if saved USPLS coordinates wouldn't translate */
	/* moved copy of inits outside of if (status)... code */

	    /* start with borders on */

	    wi_set_dialog_value(id, MD_BOARDERS_RADIO, ON);
	    /* shouldn't be necessary */
	    /* wi_set_dialog_value(id, MD_WO_BOARDERS_RADIO, OFF); */
	    dia_with_borders = TRUE;

	    if (abs(uom_code_1.uom_code) == 7)	/* add border in cm */
	    {
		tu_unit_conversion((DOUBLE) LEFT_MARGIN + RIGHT_MARGIN, "IN", &temp, "CM");
		map_size_x += temp;
		tu_unit_conversion((DOUBLE) TOP_MARGIN + BOTTOM_MARGIN, "IN", &temp, "CM");
		map_size_y += temp;
		strcpy(uom, "CM");
	    }
	    else
	    {
	    /* add border in inches */
		map_size_x = map_size_x + LEFT_MARGIN + RIGHT_MARGIN;
		map_size_y = map_size_y + TOP_MARGIN + BOTTOM_MARGIN;
		strcpy(uom, "INCHES");
	    }
	    /* UOM is reflected in the buttons, try text item without uom.
	    See who complains. */
	    sprintf(text, "%.1f X %.1f %s", map_size_x, map_size_y,
		uom);
	    /* sprintf(text, "%.1f X %.1f", map_size_x, map_size_y); */
	    wi_set_dialog_text(id, MD_MAP_SIZE, text);
	}	/* parse_uom */
    }	/* if status... */
    if (status != SUCCESS)
    {
	strcpy(text, "Map Size is Undefined");
	wi_set_dialog_text(id, MD_MAP_SIZE, text);
    }
 /* * INIT THE COORDINATE SYSTEM BUTTONS *                                       */
 /* --------------------------------------                                       */

 /* get "good" ll_point and ur_point structures */
    if (ARE_SAME(mapdef->map_coordinate_system, "USPLS"))
    {
	map_coord_sys_flag = 2;
	wi_set_dialog_value(id, MD_USPLS_RADIO, ON);
	/* shouldn't be necessary */
	/* wi_set_dialog_value(id, MD_XY_RADIO, OFF); */
	wi_enable_dialog_item(id, MD_COORD_SELECT, OFF);
	strcpy(lower_left_ext, mapdef->lower_left_str);
	strcpy(upper_right_ext, mapdef->upper_right_str);
	wi_set_dialog_text(id, MD_COORD_FORMAT, "Format: S-T-R-M");
	wi_set_dialog_text(id, MD_SETUP_MSG,
	    "Map Corners Format S-T-R-M, e.g. 2-17N-12W-6");

/*!!!no reason to convert anymore, but leaving as is for now */
	ll_point.x = mapdef->lower_left_xy.x;	/* set ll_point to mapdef limits */
	ll_point.y = mapdef->lower_left_xy.y;	/* for USPLS */
	ur_point.x = mapdef->upper_right_xy.x;
	ur_point.y = mapdef->upper_right_xy.y;
/*!!! */
    }
    else
    {
	map_coord_sys_flag = 1;
	wi_set_dialog_value(id, MD_XY_RADIO, ON);
	/* shouldn't be necessary */
	/* wi_set_dialog_value(id, MD_USPLS_RADIO, OFF); */
	wi_enable_dialog_item(id, MD_COORD_SELECT, ON);
    /* * INIT THE MAP CORNERS FIELDS *                                              */
    /* -------------------------------                                              */

/* (CP) this is a carry over from the "old days" when you could start out with
essentially a 'null' map. if a map is ever made at the "zero" coordinate,
ct_transform_point may be unnecessarily called */
	if ((mapdef->lower_left_xy.x != 0.0) AND(ARE_SAME(mapdef->map_projection,
		    coordinate_list[display_index])))
	{
	    sprintf(lower_left_ext, "%.2f, %.2f", mapdef->lower_left_xy.x,
		mapdef->lower_left_xy.y);
	    sprintf(upper_right_ext, "%.2f, %.2f", mapdef->upper_right_xy.x,
		mapdef->upper_right_xy.y);

	    ll_point.x = mapdef->lower_left_xy.x;	/* display == current */
	    ll_point.y = mapdef->lower_left_xy.y;

	    ur_point.x = mapdef->upper_right_xy.x;
	    ur_point.y = mapdef->upper_right_xy.y;
	}
	else
	{
	    ct_transform_point(mapdef->lower_left_xy.x, mapdef->lower_left_xy.y,
		current_projection, &ll_point.x, &ll_point.y,
		display_projection);

	    ct_transform_point(mapdef->upper_right_xy.x, mapdef->upper_right_xy.y,
		current_projection, &ur_point.x, &ur_point.y,
		display_projection);

	/* if display_projection is geodetic, ll_point and ur_point are in
	   GEODETIC_DECIMAL. We may be required here to convert it to
	   GEODETIC_DMS and handle accordingly */

	    if (geodetic_display == GEODETIC_DMS)
	    {
		latitude.decimal = ll_point.y;
		ct_decimal_to_dms(&latitude, LATITUDE, out_lat);
		longitude.decimal = ll_point.x;
		ct_decimal_to_dms(&longitude, LONGITUDE, out_lon);
		sprintf(lower_left_ext, "%s, %s", out_lon, out_lat);

		latitude.decimal = ur_point.y;
		ct_decimal_to_dms(&latitude, LATITUDE, out_lat);
		longitude.decimal = ur_point.x;
		ct_decimal_to_dms(&longitude, LONGITUDE, out_lon);
		sprintf(upper_right_ext, "%s, %s", out_lon, out_lat);
		wi_set_dialog_text(id, MD_COORD_FORMAT, "Format: Long,Lat");
	    }
	    else if (geodetic_display == GEODETIC_DECIMAL)
	    {
		sprintf(lower_left_ext, "%.5f, %.5f", ll_point.x, ll_point.y);
		sprintf(upper_right_ext, "%.5f, %.5f", ur_point.x, ur_point.y);
		wi_set_dialog_text(id, MD_COORD_FORMAT, "Format: Long,Lat");
	    }
	    else
	    {
		sprintf(lower_left_ext, "%.2f, %.2f", ll_point.x, ll_point.y);
		sprintf(upper_right_ext, "%.2f, %.2f", ur_point.x, ur_point.y);
		wi_set_dialog_text(id, MD_COORD_FORMAT, "Format: X,Y");
	    }
	}
    }
 /* if mapdef->map_coordinate_system is an 'XY' system. these strings will be
    empty, which will be ok when switching between USPLS and XY */
    strcpy(hold_ll_strm, mapdef->lower_left_str);
    strcpy(hold_ur_strm, mapdef->upper_right_str);
    dia_coord_sys_flag = map_coord_sys_flag;

    if (IS_STRING(lower_left_ext))
    {
	wi_set_dialog_text(id, MD_LL_CORNER, lower_left_ext);
    }
    if (IS_STRING(upper_right_ext))
    {
	wi_set_dialog_text(id, MD_UR_CORNER, upper_right_ext);
    }
 /* * MAKE THE DIALOG VISIBLE *                                                  */
 /* ---------------------------                                                  */

    wi_open_dialog(id);

 /* * SERVICE THE EVENTS *                                                       */
 /* ----------------------                                                       */

    status = FAIL;
    while (status EQUALS FAIL)
    {
	wi_request_event(0, &event_class, &event_id, &event_item);
	wi_set_dialog_text(id, MD_SETUP_MSG, "");
	stat = SUCCESS;
	if (event_id EQUALS id)
	{
	/* NOTE: for all events below, ur_point and ll_point are now in the
	   display coordinate system IF dia_coord_sys_flag = 1 */
	    switch (event_item)
	    {
	    case MD_EDIT_SCALE:
		stat = mp_edit_scale_dialog(dia_map_scale_ext, &dia_map_scale,
			dia_map_scale_uom);
		if (stat == SUCCESS)	/* changes not saved if CANCEL */
		{
			/* bring dialog in sync */
			wi_set_dialog_text(id, MD_MAP_SCALE, dia_map_scale_ext);
		}
		break;

	    case MD_EDIT_CORNERS:
		stat = mp_edit_corners_dialog(lower_left_ext, upper_right_ext,
			dia_coord_sys_flag, geodetic_display, 
			&ll_point.x, &ll_point.y,
			&ur_point.x, &ur_point.y);
		if (stat == SUCCESS)
		{
			/* bring dialog in sync, update ll_point & l_proj_ll, etc. */
			wi_set_dialog_text(id, MD_LL_CORNER, lower_left_ext);
			wi_set_dialog_text(id, MD_UR_CORNER, upper_right_ext);
/* (CP) one (possibly) unresolvable problem here. If the ct_transform routines fail
   (can't transform) then you would want to force the user back into the corner
   routine. But this 'feels' like another out-of-sync problem. We just put three
   sync problems to bed, but may have created one here. Not sure what to do
   (if anything) at this time */
			if (dia_coord_sys_flag == 1)
			{
		    	stat = ct_transform_point(ll_point.x, ll_point.y,
				display_projection,
				&ll_x, &ll_y, project_projection);
/* these are nested so that one failure isn't lost if the second pair are ok */
			if (stat == SUCCESS)
			{
			l_proj_ll_x = ll_x;
			l_proj_ll_y = ll_y;
		    	stat = ct_transform_point(ur_point.x, ur_point.y,
				display_projection,
				&ur_x, &ur_y, project_projection);
			if (stat == SUCCESS)
			{
			l_proj_ur_x = ur_x;
			l_proj_ur_y = ur_y;
			}
			}
			}
			else	/* dia_coord_sys_flag == 2 */
			{
			ll_x = ll_point.x;
			ll_y = ll_point.y;
			ur_x = ur_point.x;
			ur_y = ur_point.y;
			stat = ct_transform_point(ll_point.x, ll_point.y,
				project_projection,
				&ll_point.x, &ll_point.y, current_projection);
			if (stat == SUCCESS)
			{
			l_proj_ll_x = ll_x;	/* NOW allow alter of l_proj_ll_x */
			l_proj_ll_y = ll_y;
			stat = ct_transform_point(ur_point.x, ur_point.y,
				project_projection,
				&ur_point.x, &ur_point.y, current_projection);
			if (stat == SUCCESS)
			{
			l_proj_ur_x = ur_x;
			l_proj_ur_y = ur_y;
			}
			}
			}
		}
		break;
	    /*-----------------------------------------------------------------------------*/
	      case MD_XY_RADIO:	/* XY Coordinate system has been selected */
	    /*-----------------------------------------------------------------------------*/
		if (dia_coord_sys_flag == 1)
		{
		    break;
		}

		wi_set_dialog_value(id, MD_XY_RADIO, ON);
		/* shouldn't be necessary */
		/*wi_set_dialog_value(id, MD_USPLS_RADIO, OFF);*/
		wi_enable_dialog_item(id, MD_COORD_SELECT, ON);

		if (geodetic_display == GEODETIC_DMS OR geodetic_display ==
		    GEODETIC_DECIMAL)
		{
		    wi_set_dialog_text(id, MD_COORD_FORMAT, "Format: Long,Lat");
		}
		else
		{
		    wi_set_dialog_text(id, MD_COORD_FORMAT, "Format: X,Y");
		}
	    /* keep former section-twp-rge-mer text just in case we switch back */
		strcpy(hold_ll_strm, lower_left_ext);
		strcpy(hold_ur_strm, upper_right_ext);

/* use 'current' limits in project projection for display on dialog. This will
be values found during 'USPLS_RADIO' use or carried over from previous 'XY_RADIO'
if no changes were made while using USPLS input format */

	    /* update map limits in 'display' projection from project
	       projection limits */
		ct_transform_point(l_proj_ll_x, l_proj_ll_y, project_projection,
			&ll_point.x, &ll_point.y, display_projection);

		ct_transform_point(l_proj_ur_x, l_proj_ur_y, project_projection,
			&ur_point.x, &ur_point.y, display_projection);
		if (geodetic_display == GEODETIC_DMS)
		{
		    latitude.decimal = ll_point.y;
		    ct_decimal_to_dms(&latitude, LATITUDE, out_lat);
		    longitude.decimal = ll_point.x;
		    ct_decimal_to_dms(&longitude, LONGITUDE, out_lon);
		    sprintf(lower_left_ext, "%s, %s", out_lon, out_lat);

		    latitude.decimal = ur_point.y;
		    ct_decimal_to_dms(&latitude, LATITUDE, out_lat);
		    longitude.decimal = ur_point.x;
		    ct_decimal_to_dms(&longitude, LONGITUDE, out_lon);
		    sprintf(upper_right_ext, "%s, %s", out_lon, out_lat);
		}
		else if (geodetic_display == GEODETIC_DECIMAL)
		{
		    sprintf(lower_left_ext, "%.5f, %.5f", ll_point.x, ll_point.y);
		    sprintf(upper_right_ext, "%.5f, %.5f", ur_point.x, ur_point.y);
		}
		else
		{
		    sprintf(lower_left_ext, "%.2f, %.2f", ll_point.x, ll_point.y);
		    sprintf(upper_right_ext, "%.2f, %.2f", ur_point.x, ur_point.y);
		}
		wi_set_dialog_text(id, MD_LL_CORNER, lower_left_ext);
		wi_set_dialog_text(id, MD_UR_CORNER, upper_right_ext);

		dia_coord_sys_flag = 1;
/* (CP) it is assumed that this event will always be successful. It may not
be necessarily so if a bad map definition was allowed to be saved */
		break;

	    /*--------------------------------------------------------------------------*/
	      case MD_USPLS_RADIO:	/* USPLS system has been selected */
	    /*--------------------------------------------------------------------------*/
		if (dia_coord_sys_flag == 2)
		{
			break;
		}

		wi_set_dialog_value(id, MD_USPLS_RADIO, ON);
		/* shouldn't be necessary */
		/*wi_set_dialog_value(id, MD_XY_RADIO, OFF);*/
		wi_enable_dialog_item(id, MD_COORD_SELECT, OFF);

		strcpy(lower_left_ext, hold_ll_strm);
		strcpy(upper_right_ext, hold_ur_strm);
		wi_set_dialog_text(id, MD_LL_CORNER, lower_left_ext);
		wi_set_dialog_text(id, MD_UR_CORNER, upper_right_ext);
		wi_set_dialog_text(id, MD_COORD_FORMAT, "Format: S-T-R-M");
		wi_set_dialog_text(id, MD_SETUP_MSG,
		    "Map Corners Format S-T-R-M, e.g. 2-17N-12W-6");

		dia_coord_sys_flag = 2;

	    	/* make map size 'undefined' if strings are empty */
		if (hold_ll_strm[0] == (CHAR) 0 OR hold_ur_strm[0] == (CHAR) 0)
		{
		    stat = FAIL;
		}
	    	/* else go get map limits according to the text strings */
		else
		{
		    stat = mr_get_section_corners(hold_ll_strm,
			&ll_point.x, &ll_point.y, &dummy, &dummy);
		    if (stat != SUCCESS AND IS_STRING(lower_left_ext))
		    {
			wi_beep(1);
			am_message(AM_ALERT, mg_message(MD_BAD_STRM));
			wi_set_dialog_text(id, MD_SETUP_MSG,
			    mg_message(MD_BAD_STRM));
			break;
		    }
		    stat = mr_get_section_corners(hold_ur_strm, &dummy, &dummy,
			&ur_point.x, &ur_point.y);
		    if (stat != SUCCESS AND IS_STRING(upper_right_ext))
		    {
			wi_beep(1);
			am_message(AM_ALERT, mg_message(MD_BAD_STRM));
			wi_set_dialog_text(id, MD_SETUP_MSG,
			    mg_message(MD_BAD_STRM));
			break;
		    }
		}
		if (stat == SUCCESS)
		{
		    l_proj_ll_x = ll_point.x;
		    l_proj_ll_y = ll_point.y;
		    l_proj_ur_x = ur_point.x;
		    l_proj_ur_y = ur_point.y;
/*!!! this isn't required anymore, but left alone for less busy days */
		    ct_transform_point(l_proj_ll_x, l_proj_ll_y, project_projection,
			&ll_point.x, &ll_point.y, current_projection);

		    ct_transform_point(l_proj_ur_x, l_proj_ur_y, project_projection,
			&ur_point.x, &ur_point.y, current_projection);
/*!!! */
		}
		break;

	    /*--------------------------------------------------------------------------*/
	      case MD_PROJ_SELECT:	/* New Projection */
	    /*--------------------------------------------------------------------------*/

		wi_query_selections(id, MD_PROJ_SELECT, &nitems, &selected);
		if (nitems <= 0)
		{
		    wi_set_dialog_text(id, MD_SETUP_MSG,
			mg_message(MD_SELECT_PROJECTION));
		    break;
		}
		wi_set_default_sel(id, MD_PROJ_SELECT, selected);
		ct_get_projection(projection_list[selected - 1], &current_projection);
		break;

	    /*--------------------------------------------------------------------------*/
	      case MD_COORD_SELECT:	/* New Coordinate system */
	    /*--------------------------------------------------------------------------*/

		wi_query_selections(id, MD_COORD_SELECT, &nitems_coord, &selected);
		if (nitems_coord <= 0)
		{
		    wi_set_dialog_text(id, MD_SETUP_MSG,
			mg_message(MD_SELECT_PROJECTION));
		    break;
		}
		wi_set_default_sel(id, MD_COORD_SELECT, selected);
		i = selected - 1;
		strcpy(dia_map_coordinate_projection, coordinate_list[i]);

		if (strstr(coordinate_list[i], "GEODETIC"))
		{
		    ct_get_projection("GEODETIC", &display_projection);
		}
		else
		{
		    ct_get_projection(coordinate_list[i], &display_projection);
		}
	    	/* start with a 'clean' copy of limits in project projection */
		ct_transform_point(l_proj_ll_x, l_proj_ll_y,
		    project_projection, &ll_point.x, &ll_point.y,
		    display_projection);
		ct_transform_point(l_proj_ur_x, l_proj_ur_y,
		    project_projection, &ur_point.x, &ur_point.y,
		    display_projection);

		if (display_index != i)
		{
/*!!! dia_coord_sys_flag MUST be equal to 1, since we are generating COORDINATE
SYSTEM events (which is turned off for USPLS). This is a harmless but useless test */
		    if ((dia_coord_sys_flag EQUALS 1) &&
			(geodetic_display != GEODETIC_DMS))
		    {

			if (strstr(coordinate_list[i], "GEODETIC DMS"))
			{

			    latitude.decimal = ll_point.y;
			    ct_decimal_to_dms(&latitude, LATITUDE, out_lat);
			    longitude.decimal = ll_point.x;
			    ct_decimal_to_dms(&longitude, LONGITUDE, out_lon);
			    sprintf(lower_left_ext, "%s, %s", out_lon, out_lat);

			    latitude.decimal = ur_point.y;
			    ct_decimal_to_dms(&latitude, LATITUDE, out_lat);
			    longitude.decimal = ur_point.x;
			    ct_decimal_to_dms(&longitude, LONGITUDE, out_lon);
			    sprintf(upper_right_ext, "%s, %s", out_lon, out_lat);
			}
			else if (strstr(coordinate_list[i], "GEODETIC DEC"))
			{
			    sprintf(lower_left_ext, "%.5f, %.5f", ll_point.x, ll_point.y);
			    sprintf(upper_right_ext, "%.5f, %.5f", ur_point.x, ur_point.y);
			}
			else
			{
			    sprintf(lower_left_ext, "%.2f, %.2f", ll_point.x, ll_point.y);
			    sprintf(upper_right_ext, "%.2f, %.2f", ur_point.x, ur_point.y);
			}
		    }
		    else if (strstr(coordinate_list[i], "GEODETIC DEC"))
		    {
			sprintf(lower_left_ext, "%.5f, %.5f", ll_point.x, ll_point.y);
			sprintf(upper_right_ext, "%.5f, %.5f", ur_point.x, ur_point.y);
		    }
		    else if ((dia_coord_sys_flag EQUALS 1) &&
			(geodetic_display == GEODETIC_DMS))
		    {
			sprintf(lower_left_ext, "%.2f, %.2f", ll_point.x, ll_point.y);
			sprintf(upper_right_ext, "%.2f, %.2f", ur_point.x, ur_point.y);
		    }
		    display_index = i;
		    wi_set_dialog_text(id, MD_LL_CORNER, lower_left_ext);
		    wi_set_dialog_text(id, MD_UR_CORNER, upper_right_ext);

		    if (ARE_SAME(coordinate_list[i], "GEODETIC DMS"))
		    {
			geodetic_display = GEODETIC_DMS;
			wi_set_dialog_text(id, MD_COORD_FORMAT, "Format: Long,Lat");
		    }
		    else if (ARE_SAME(coordinate_list[i], "GEODETIC DEC"))
		    {
			geodetic_display = GEODETIC_DECIMAL;
			wi_set_dialog_text(id, MD_COORD_FORMAT, "Format: Long,Lat");
		    }
		    else
		    {
			geodetic_display = NON_GEODETIC;
			wi_set_dialog_text(id, MD_COORD_FORMAT, "Format: X,Y");
		    }
		}
		stat = SUCCESS;
		break;

	    /*--------------------------------------------------------------------------*/
	      case MD_SIZE_SCALE:	/* Scale sizes for plotting toggled */
	    /*--------------------------------------------------------------------------*/

		dia_scale_sizes = NOT dia_scale_sizes;
		stat = CANCEL;	/* no reason to recalc map size */
		break;

	    /*--------------------------------------------------------------------------*/
	      case MD_INCHES_RADIO:	/* Show map size in inches */
	    /*--------------------------------------------------------------------------*/

		if (dia_in_inches)
		{
		    break;
		}
		dia_in_inches = TRUE;
		break;

	    /*--------------------------------------------------------------------------*/
	      case MD_CENTI_RADIO:	/* Show map size in centimeters */
	    /*--------------------------------------------------------------------------*/

		if (!dia_in_inches)
		{
		    break;
		}
		dia_in_inches = FALSE;
		break;

	    /*--------------------------------------------------------------------------*/
	      case MD_BOARDERS_RADIO:	/* Show map size with borders */
	    /*--------------------------------------------------------------------------*/

		if (dia_with_borders)
		{
		    break;
		}
		dia_with_borders = TRUE;
		break;

	    /*--------------------------------------------------------------------------*/
	      case MD_WO_BOARDERS_RADIO:	/* Show map size w/o borders */
	    /*--------------------------------------------------------------------------*/

		if (!dia_with_borders)
		{
		    break;
		}
		dia_with_borders = FALSE;
		break;

	    /*--------------------------------------------------------------------------*/
	      case MD_SETUP_OK:	/* OK Button presssed */
	    /*--------------------------------------------------------------------------*/
/* (CP) to do: make this section smarter about hitting OK with NO changes */

		if (dia_coord_sys_flag EQUALS 2)	/* USPLS */
		{
		    stat = mr_get_section_corners(lower_left_ext,
			&mapdef->lower_left_xy.x,
			&mapdef->lower_left_xy.y, &dummy,
			&dummy);
		    if (stat != SUCCESS )	/* no matter what, we're screwed */
		    {
			wi_beep(1);
/* (CP) make this bad lower left STRM */
			am_message(AM_ALERT, mg_message(MD_BAD_STRM));
			wi_set_dialog_text(id, MD_SETUP_MSG,
			    mg_message(MD_BAD_STRM));
			break;
		    }
		    mapdef->change_flag = TRUE;
		    stat = mr_get_section_corners(upper_right_ext, &dummy,
			&dummy, &mapdef->upper_right_xy.x,
			&mapdef->upper_right_xy.y);
		    if (stat != SUCCESS)
		    {
			wi_beep(1);
/* (CP) make this bad upper right STRM */
			am_message(AM_ALERT, mg_message(MD_BAD_STRM));
			wi_set_dialog_text(id, MD_SETUP_MSG, mg_message(MD_BAD_STRM));
			break;
		    }
		    mapdef->change_flag = TRUE;
		    if (ARE_DIFFERENT(current_projection->projection_id,
			    project_projection->projection_id))
		    {
			ct_transform_point(mapdef->lower_left_xy.x,
			    mapdef->lower_left_xy.y, project_projection,
			    &mapdef->lower_left_xy.x,
			    &mapdef->lower_left_xy.y,
			    current_projection);
			ct_transform_point(mapdef->upper_right_xy.x,
			    mapdef->upper_right_xy.y,
			    project_projection,
			    &mapdef->upper_right_xy.x,
			    &mapdef->upper_right_xy.y,
			    current_projection);
		    }

		strcpy(mapdef->lower_left_str, lower_left_ext);
		strcpy(mapdef->upper_right_str, upper_right_ext);
		}
		else	/* XY */
		{
		    ct_transform_point(l_proj_ll_x, l_proj_ll_y, project_projection,
			    &mapdef->lower_left_xy.x,
			    &mapdef->lower_left_xy.y, current_projection);
		    ct_transform_point(l_proj_ur_x, l_proj_ur_y, project_projection,
			    &mapdef->upper_right_xy.x,
			    &mapdef->upper_right_xy.y, current_projection);
		    mapdef->change_flag = TRUE;
		}
		if ((mapdef->upper_right_xy.x <= mapdef->lower_left_xy.x) OR
		    (mapdef->upper_right_xy.y <= mapdef->lower_left_xy.y))
		{
		    wi_beep(1);
		    wi_set_dialog_text(id, MD_SETUP_MSG,
			mg_message(MD_INVALID_MAP_LIMITS));
		    stat = FAIL;
		    break;
		}
		mapdef->map_scale = dia_map_scale;
		strcpy(mapdef->map_scale_uom, dia_map_scale_uom);

		if (dia_coord_sys_flag != map_coord_sys_flag)
		{
		    mapdef->change_flag = TRUE;
		    switch (dia_coord_sys_flag)
		    {
		      case 1:
			strcpy(mapdef->map_coordinate_system,
			    dia_map_coordinate_projection);
			break;
		      case 2:
			strcpy(mapdef->map_coordinate_system, "USPLS");
			break;
		    }
		}
		if (dia_scale_sizes != mapdef->scale_sizes_for_plotting)
		{
		    mapdef->change_flag = TRUE;
		    mapdef->scale_sizes_for_plotting = dia_scale_sizes;
		}
		if (dia_coord_sys_flag == 1 AND
		    ARE_DIFFERENT(dia_map_coordinate_projection,
			mapdef->map_coordinate_system))
		{
		    mapdef->change_flag = TRUE;
		    strcpy(mapdef->map_coordinate_system,
			dia_map_coordinate_projection);
		}
	    /* GET THE MAP PROJECTION                                           */

		strcpy(mapdef->map_projection, current_projection->projection_id);
		qs_set_c(QS_MAP_PROJECTION, mapdef->map_projection,
		    strlen(mapdef->map_projection));

		mapdef->setup_ok = TRUE;
		status = SUCCESS;	/* break out of event loop */
		stat = CANCEL;
		break;

	    /*-----------------------------------------------------------------------------*/
	      case MD_SETUP_CANCEL:	/* CANCEL Button pressed                        */
	    /*-----------------------------------------------------------------------------*/

		status = CANCEL;
		goto finished;
		break;	/* unreachable */

	      default:	/* lights on, nobody home */
		break;
	    }	/* switch */
	}	/* event_id = id */
	else
	{
	    wi_beep(1);
	}

	if (stat == SUCCESS)	/* recalc map size box */
	{
		/* get x and y in units of current_projection */
		ct_transform_point(l_proj_ll_x, l_proj_ll_y, project_projection,
		    &ll_x, &ll_y, current_projection);
		ct_transform_point(l_proj_ur_x, l_proj_ur_y, project_projection,
		    &ur_x, &ur_y, current_projection);

		if (ll_x >= ur_x)
		{
		    wi_beep(1);
		    am_message(AM_ALERT, mg_message(MD_WRONG_X_MAP_SIZE));
		    wi_set_dialog_text(id, MD_MAP_SIZE, "Map Size is Undefined");
		    continue;	/* next event */
		}
		if (ll_y >= ur_y)
		{
		    wi_beep(1);
		    am_message(AM_ALERT, mg_message(MD_WRONG_Y_MAP_SIZE));
		    wi_set_dialog_text(id, MD_MAP_SIZE, "Map Size is Undefined");
		    continue;	/* next event */
		}
/* this test should be a waste of time if edit scale function works */
		stat = parse_uom(dia_map_scale_uom, units_left, units_right);
		if (stat != SUCCESS)
		{
		    wi_beep(1);
		    wi_set_dialog_text(id, MD_SETUP_MSG,
			mg_message(MD_BAD_MAP_SCALE));
		    wi_set_dialog_text(id, MD_MAP_SIZE, "Map Size is Undefined");
		    continue;	/* next event */
		}
		tu_verify_uom(units_left, &uom_code_1);
		ct_get_projection_uom(current_projection->projection_id, uom);
		tu_verify_uom(uom, &uom_code_2);

		/* convert the corners to the 'units_left' UOM */
		if (abs(uom_code_1.uom_code) != abs(uom_code_2.uom_code))
		{
		    tu_unit_conversion(ll_x, uom, &ll_x,
			uom_code_1.uom_name);
		    tu_unit_conversion(ll_y, uom, &ll_y,
			uom_code_1.uom_name);
		    tu_unit_conversion(ur_x, uom, &ur_x,
			uom_code_1.uom_name);
		    tu_unit_conversion(ur_y, uom, &ur_y,
			uom_code_1.uom_name);
		}

		map_size_x = (ur_x - ll_x) / dia_map_scale;
		map_size_y = (ur_y - ll_y) / dia_map_scale;
		tu_verify_uom(units_right, &uom_code_1);
		/* Now calculate final size based on state of dia_map_scale flag
		   which should have a valid value. (Want to stop flip/flop of
		   inch/cm units when edits are made) */
		/* now convert size from 'units_right' UOM to inches or cm */
		if (!dia_in_inches)
		{
		    tu_unit_conversion((DOUBLE) map_size_x, uom_code_1.uom_name,
			&map_size_x, "CM");
		    tu_unit_conversion((DOUBLE) map_size_y, uom_code_1.uom_name,
			&map_size_y, "CM");
		    if (dia_with_borders)
		    {
		    /* only add border size IF border flag is set */
			tu_unit_conversion((DOUBLE) LEFT_MARGIN + RIGHT_MARGIN,
				"IN", &temp, "CM");
			map_size_x += temp;
			tu_unit_conversion((DOUBLE) TOP_MARGIN + BOTTOM_MARGIN,
				"IN", &temp, "CM");
			map_size_y += temp;
		    }
		    strcpy(uom, "CM");
		}
		else	/* inches radio button set */
		{
		    tu_unit_conversion((DOUBLE) map_size_x, uom_code_1.uom_name,
			&map_size_x, "IN");
		    tu_unit_conversion((DOUBLE) map_size_y, uom_code_1.uom_name,
			&map_size_y, "IN");
		    if (dia_with_borders)
		    {
		    /* only add border size if dialog border flag set */
			map_size_x = map_size_x + LEFT_MARGIN + RIGHT_MARGIN;
			map_size_y = map_size_y + TOP_MARGIN + BOTTOM_MARGIN;
		    }
		    strcpy(uom, "INCHES");
		}
		/* give 'em map size without units since the units are 
		   accurately reflected in the buttons */
		sprintf(text, "%.1f X %.1f %s", map_size_x, map_size_y,
		    uom);
		/* sprintf(text, "%.1f X %.1f", map_size_x, map_size_y); */
		wi_set_dialog_text(id, MD_MAP_SIZE, text);

	}	/* if stat == SUCCESS */
	if (stat == FAIL)	/* ignore CANCEL */
	{
	    wi_set_dialog_text(id, MD_MAP_SIZE, "Map Size is Undefined");
	}
    }		/* end of event loop */
finished:
    wi_close_dialog(id);
    ts_tcp_free(projection_list);
    ts_tcp_free(coordinate_list);
    fi_pop();

    return status;
}

/************************************************************************************/

#if USE_PROTOTYPES
static INT parse_uom(CHAR * unit, CHAR * uom1, CHAR * uom2)
#else
static INT parse_uom(unit, uom1, uom2)
CHAR *unit, *uom1, *uom2;
#endif
{
    CHAR text[60];
    INT len, status;
    UOM_STRUCTURE uom_struc;

    len = sprintf(text, "%s", unit);
    ts_str_convert(text, len, '/', ' ');
    sscanf(text, "%s %s", uom1, uom2);

/* (CP) now instead of just checking status, we also check for days/liter, etc.
   as requested by Hotline case 1314, Arlene Fox. I'd like to meet the geoscientist
   who would seriously try this for a map scale. */
    status = tu_verify_uom(uom1, &uom_struc);
    if (status != SUCCESS OR strcmp(uom_struc.uom_type,"DISTANCE") != 0)
	return FAIL;
    status = tu_verify_uom(uom2, &uom_struc);
    if (status != SUCCESS OR strcmp(uom_struc.uom_type,"DISTANCE") != 0)
	return FAIL;

    return SUCCESS;
}

#if USE_PROTOTYPES
static INT mp_edit_scale_dialog(CHAR *scale_string, DOUBLE *scale, CHAR *scale_uom)
#else
static INT mp_edit_scale_dialog(scale_string, scale, scale_uom)
CHAR *scale_string;
DOUBLE *scale;
CHAR *scale_uom;
#endif
{
INT stat, status, cnvrt, junk;
INT id, event_class, event_id, event_item;
CHAR text[STMAX];
CHAR units_left[60], units_right[60];
DOUBLE tmp_scale;
CHAR tmp_scale_uom[60];

/* set up and put up dialog */
status = fi_push();	/* 'dim' map setup dialog until we're through and OK */
status = am_define_resource(AM_DIALOG, "MD_MAPDEF", MP_SCALE_EDIT_DLG,
	NULL_FUNCTION_PTR, &id, NULL);
if (status != SUCCESS)
{
	am_message(AM_ALERT, mg_message(status));
	fi_pop();
	return FAIL;
}
wi_set_dialog_text(id, MP_SCALE_EDIT, scale_string);
wi_open_dialog(id);

/* loop for events */
status = FAIL;
while(status == FAIL)
{
	wi_request_event(0, &event_class, &event_id, &event_item);
	wi_set_dialog_text(id, MP_SCALE_MSG, "");
	/* switch for each event */
	switch (event_item)
	{
	case MP_SCALE_OK:
		/* first, check the dialog */
		wi_query_dialog_item(id, MP_SCALE_EDIT, &junk, text);
		if (strcmp(text, scale_string) == 0)
		{
			/* this is treated like cancel, no changes */
			status = CANCEL;
			break;
		}
		else
		{
		/* see if scale change is valid */
		ts_sto_upper(text);
                cnvrt = sscanf(text, "%lf %s", &tmp_scale, tmp_scale_uom);
                if (cnvrt == 2)	/* we're looking for two values */
                {
                        stat = parse_uom(tmp_scale_uom, units_left,
                                units_right);
                        if (stat != SUCCESS OR tmp_scale <= 0.0)
                        {
                                wi_beep(1);
                                wi_set_dialog_text(id, MP_SCALE_MSG,
                                        mg_message(MD_BAD_MAP_SCALE));
				break;
                        }
		/* we're clear, so save */
		status = SUCCESS;
		*scale = tmp_scale;
		strcpy(scale_uom, tmp_scale_uom);
		strcpy(scale_string, text);
                }
                else    /* cnvrt != 2, oops */
                {
                        if (IS_EMPTY_STRING(text))
                        {
                                wi_beep(1);
                                am_message(AM_ALERT, mg_message(MP_SPEC_MAP_SCALE));
                        }
                        else
                        {
                                wi_beep(1);
                                wi_set_dialog_text(id, MP_SCALE_MSG,
                                        mg_message(MD_BAD_MAP_SCALE));
                        }
		}
		}
		break;

	case MP_SCALE_CANCEL:
		status = CANCEL;
		break;

	default:
		break;

	}

}	/* while */
/* status is SUCCESS or CANCEL */
am_release_resource(id);
fi_pop();
return status;
}

#if USE_PROTOTYPES
static INT mp_edit_corners_dialog(CHAR *ll_string, CHAR *ur_string,
	INT coord_type, INT geodetic_flag, DOUBLE *ll_pointx, DOUBLE *ll_pointy,
	DOUBLE *ur_pointx, DOUBLE *ur_pointy)
#else
static INT mp_edit_corners_dialog(ll_string, ur_string, coord_type, 
	geodetic_flag, ll_pointx, ll_pointy, ur_pointx, ur_pointy)
CHAR *ll_string, *ur_string;
INT coord_type, geodetic_flag;
DOUBLE *ll_pointx, *ll_pointy, *ur_pointx, *ur_pointy;
#endif
{
INT status, stat, junk, len, cnvrt;
INT id, event_class, event_id, event_item;
CHAR ll_text[STMAX], ll_text_out[STMAX], ur_text[STMAX], ur_text_out[STMAX];
CHAR text[STMAX];
DOUBLE ll_x, ll_y, ur_x, ur_y, dummy;
LATLONG latitude, longitude;
CHAR out_lat[60], out_lon[60];

/* set up and put up dialog */
status = fi_push();
status = am_define_resource(AM_DIALOG, "MD_MAPDEF", MP_CORNER_EDIT_DLG,
	NULL_FUNCTION_PTR, &id, NULL);
if (status != SUCCESS)
{
	am_message(AM_ALERT, mg_message(status));
	fi_pop();
	return FAIL;
}
wi_set_dialog_text(id, MP_LL_EDIT, ll_string);
wi_set_dialog_text(id, MP_UR_EDIT, ur_string);
if (coord_type == 2)	/* USPLS */
{
	wi_set_dialog_text(id, MP_COORD_FORMAT, "Format: S-T-R-M");
}
else
{
	if (geodetic_flag == NON_GEODETIC)
	{
		wi_set_dialog_text(id, MP_COORD_FORMAT, "Format: X,Y");
	}
	else
	{
		wi_set_dialog_text(id, MP_COORD_FORMAT, "Format: Long,Lat");
	}
}
wi_open_dialog(id);
/* we need default values for these in case user just changes one item */
ll_x = *ll_pointx;
ll_y = *ll_pointy;
ur_x = *ur_pointx;
ur_y = *ur_pointy;
strcpy(ll_text_out, ll_string);
strcpy(ur_text_out, ur_string);

/* loop for events */
status = FAIL;
while(status == FAIL)
{
        wi_request_event(0, &event_class, &event_id, &event_item);
        wi_set_dialog_text(id, MP_CORNER_MSG, "");
        /* switch for each event */
        switch (event_item)
        {
        case MP_CORNER_OK:
		/* first, check the lower left corner */
                wi_query_dialog_item(id, MP_LL_EDIT, &junk, ll_text);
		if (!IS_STRING(ll_text))
		{
			wi_beep(1);
/* (CP) WARNING: this is a hard-coded message which needs to be entered into
   the message facility */
			wi_set_dialog_text(id, MP_CORNER_MSG, "Enter Lower Left Value");
			break;
		}
                if (strcmp(ll_text, ll_string) != 0)
                {
			/* we process a lower left change */
			if ((coord_type == 1) && (geodetic_flag != GEODETIC_DMS))
			{
				/* XY and NOT DMS */
				strcpy(text, ll_text);
				len = strlen(text);
				ts_str_convert(text, len, ',', ' ');
				cnvrt = sscanf(text, "%lf %lf", &ll_x, &ll_y);
				if (cnvrt != 2)
				{
					wi_beep(1);
					wi_set_dialog_text(id, MP_CORNER_MSG,
						mg_message(MD_INVALID_MAP_LIMITS));
					break;
				}
				if (geodetic_flag == NON_GEODETIC)
				{
					sprintf(ll_text_out, "%.2f, %.2f", ll_x, ll_y);
				}
				else
				{
					sprintf(ll_text_out, "%.5f, %.5f", ll_x, ll_y);
				}
			}
			if ((coord_type == 1) && (geodetic_flag == GEODETIC_DMS))
			{
				/* XY DMS */
				if ((ts_parse_dms(ll_text, &latitude, &longitude))
					!= SUCCESS)
				{
					wi_beep(1);
					wi_set_dialog_text(id, MP_CORNER_MSG,
						mg_message(MD_BAD_DMS));
					break;
				}
                    		ct_dms_to_decimal(&latitude);
                    		ct_dms_to_decimal(&longitude);
                    		/* these are in 'display' coordinate system */
                    		ll_x = longitude.decimal;
                    		ll_y = latitude.decimal;
				ct_decimal_to_dms(&latitude, LATITUDE, out_lat);
				ct_decimal_to_dms(&longitude, LONGITUDE, out_lon);
				sprintf(ll_text_out, "%s, %s", out_lon, out_lat);
			}
			if (coord_type == 2)
			{
				/* USPLS */
				stat = mr_get_section_corners(ll_text, &ll_x, &ll_y,
					&dummy, &dummy);
				if (stat != SUCCESS)
				{
					wi_beep(1);
/* (CP) expand this error message to say bad LL STRM */
					wi_set_dialog_text(id, MD_SETUP_MSG,
						mg_message(MD_BAD_STRM));
					break;
				}
				strcpy(ll_text_out, ll_text);
				ts_sto_upper(ll_text_out);
			}
                }

		/* now check the upper right edit text box */
                wi_query_dialog_item(id, MP_UR_EDIT, &junk, ur_text);
		if (!IS_STRING(ur_text))
		{
			wi_beep(1);
/* (CP) WARNING: this is a hard-coded message which needs to be entered into
   the message facility */
			wi_set_dialog_text(id, MP_CORNER_MSG,
				"Enter Upper Right Value");
			break;
		}
                if (strcmp(ur_text, ur_string) != 0)
                {
			/* we process an upper right change */
			if ((coord_type == 1) && (geodetic_flag != GEODETIC_DMS))
			{
				/* XY and NOT DMS */
				strcpy(text, ur_text);
				len = (INT) strlen(text);
				ts_str_convert(text, len, ',', ' ');
				cnvrt = sscanf(text, "%lf %lf", &ur_x, &ur_y);
				if (cnvrt != 2)
				{
					wi_beep(1);
					wi_set_dialog_text(id, MP_CORNER_MSG,
						mg_message(MD_INVALID_MAP_LIMITS));
					break;
				}
				if (geodetic_flag == NON_GEODETIC)
				{
					sprintf(ur_text_out, "%.2f, %.2f", ur_x, ur_y);
				}
				else
				{
					sprintf(ur_text_out, "%.5f, %.5f", ur_x, ur_y);
				}
			}
			if ((coord_type == 1) && (geodetic_flag == GEODETIC_DMS))
			{
				/* XY DMS */
				if ((ts_parse_dms(ur_text, &latitude, &longitude))
					!= SUCCESS)
				{
					wi_beep(1);
					wi_set_dialog_text(id, MP_CORNER_MSG,
						mg_message(MD_BAD_DMS));
					break;
				}
                    		ct_dms_to_decimal(&latitude);
                    		ct_dms_to_decimal(&longitude);
                    		/* these are in 'display' coordinate system */
                    		ur_x = longitude.decimal;
                    		ur_y = latitude.decimal;
				ct_decimal_to_dms(&latitude, LATITUDE, out_lat);
				ct_decimal_to_dms(&longitude, LONGITUDE, out_lon);
				sprintf(ur_text_out, "%s, %s", out_lon, out_lat);
			}
			if (coord_type == 2)
			{
				/* USPLS */
				stat = mr_get_section_corners(ur_text, &dummy,
					&dummy, &ur_x, &ur_y);
				if (stat != SUCCESS)
				{
					wi_beep(1);
/* (CP) expand this error message to say bad UR STRM */
					wi_set_dialog_text(id, MD_SETUP_MSG,
						mg_message(MD_BAD_STRM));
					break;
				}
				strcpy(ur_text_out, ur_text);
				ts_sto_upper(ur_text_out);
			}
                }
		/* check for corners reversed here. Don't let 'em out if bad. */
		if (ll_x >= ur_x)
		{
		    wi_beep(1);
                    am_message(AM_ALERT, mg_message(MD_WRONG_X_MAP_SIZE));
                    break;
                }
                if (ll_y >= ur_y)
                {
                    wi_beep(1);
                    am_message(AM_ALERT, mg_message(MD_WRONG_Y_MAP_SIZE));
                    break;
                }
		/* we've done all we can do. go back to map setup */	
		status = SUCCESS;
		if (strcmp(ll_string, ll_text) != 0)
		{
                	strcpy(ll_string, ll_text_out);
			*ll_pointx = ll_x;
			*ll_pointy = ll_y;
		}
		if (strcmp(ur_string, ur_text) != 0)
		{
                	strcpy(ur_string, ur_text_out);
			*ur_pointx = ur_x;
			*ur_pointy = ur_y;
		}
		break;
	
	case MP_CORNER_CANCEL:
		status = CANCEL;
		break;

	default:
		break;

	}
}
/* status is SUCCESS or CANCEL */
am_release_resource(id);
fi_pop();
return status;
}

