/* DEC/CMS REPLACEMENT HISTORY, Element WE_EDIT_WELL.C */
/* *7    17-JUL-1991 16:53:28 KEE "(SPR -1) Add map registered checking if using tablet " */
/* *6    19-JUN-1991 14:47:13 KEE "(SPR -1) Fix bugs reported by Eric at 6-11-91 " */
/* *5    21-MAR-1991 14:19:25 KEE "(SPR -1) Edit Well bug fixes for vms version" */
/* *4    11-MAR-1991 11:05:17 KEE "(SPR -1) Totally modify Charlie Purmoy's Edit Well Depth Application base on QA new */
/*specifications" */
/* *3    21-SEP-1990 12:31:03 CHARLIE "(SPR -1) fix include file change" */
/* *2    21-SEP-1990 08:54:32 CHARLIE "(SPR -1) initial well depth editor (non-graphic)" */
/* *1    21-SEP-1990 08:32:22 CHARLIE "non_graphic modify well depth (from Robb Hall)" */
/* DEC/CMS REPLACEMENT HISTORY, Element WE_EDIT_WELL.C */
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

/* The functions in this module allow the user to edit the WELL DEPTHS     */
/* that are posted on a map.  There MUST be a map displayed, with wells on */
/* it, and those wells MUST have their depths shown.                       */

/* FINDER include files */

#ifndef ESI_GL_DEFS_H
#include "esi_gl_defs.h"
#endif

#ifndef ESI_C_LIB_H
#include "esi_c_lib.h"
#endif

#ifndef ESI_WE_WELLS_H
#include "esi_we_wells.h"
#endif

#ifndef ESI_WE_MSG_H 
#include "esi_we_msg.h"
#endif

#ifndef ESI_WE_ERR_H 
#include "esi_we_err.h"
#endif

#ifndef ESI_AM_H
#include "esi_am.h"
#endif

#ifndef ESI_MP_H
#include "esi_mp.h"
#endif

#ifndef ESI_MG_H
#include "esi_mg.h"
#endif

#ifndef ESI_TC_H
#include "esi_tc.h"
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

#ifndef ESI_WL_H
#include "esi_wl.h"
#endif

#ifndef ESI_LU_H
#include "esi_lu.h"
#endif

#ifndef ESI_GKS_H
#include "esi_gks.h"
#endif

#ifndef ESI_WE_EDIT_H
#include "esi_we_edit.h"
#endif

#ifndef ESI_TS_H
#include "esi_ts.h" 
#endif

#ifndef ESI_MD_BASIS_X
#include "esi_md_basis.x" 
#endif

#ifndef ESI_CS_H
#include "esi_cs.h"
#endif

#ifndef ESI_AM_ERR_H
#include "esi_am_err.h"
#endif

#ifndef ESI_UG_H
#include "esi_ug.h"
#endif

#include "we_edit_well.rh"

#define WE_EDIT_CDLG "we_edit_well"

static CHAR error_message_string[256];

/***************************************************************************/
/*                                                                         */
/*    Routine:        WE_EDIT_WELL_INIT                                    */
/*                                                                         */
/*    Function:       This routine brings up the EDIT WELL DEPTHS          */
/*                    dialog box, so that the user can PICK wells from     */
/*                    a displayed map and EDIT their depths.  It will then */
/*                    make the user pick the first well for editing.       */
/*                                                                         */
/*    Author:         Robb Hall                                            */
/*    Modified:	      Charlie Pumroy/Kee Yip 		                   */
/*                                                                         */
/***************************************************************************/
publicdef INT we_edit_well_init(dummy)
CHAR *dummy;	/* passed data pointer */
{/* WE_EDIT_WELL_INIT start */

    INT status = SUCCESS;
    MAP_STRUCTURE *map = (MAP_STRUCTURE *) NULL;
    WELL_EDIT_ARRAY *g_pwe = (WELL_EDIT_ARRAY *) NULL;
    WELL_EDIT *pwe = (WELL_EDIT *) NULL;
    INT well_depth_count = 0;
    INT i, row, well_depth_ndx = -1;

 /* make sure that a map is currently displayed;  */
 /* otherwise, there's nought to be done, so exit */
    status = mp_front_map(&map);

    if (status != SUCCESS)
      {
	mp_set_current_map((MAP_STRUCTURE *) 0);
	am_message(AM_ALERT, mg_message(WE_NO_WP_MAP)); 
	return status;
      }
    
    if (!map->map_is_registered AND
        (!ug_if_gpx() OR wm_if_tablet()))
        {
        am_message (AM_ALERT, mg_message(WE_MAP_NOT_REGISTERED));
        }

    status = am_declare("Edit Well Depth");
    status = am_open_workspace("WELL_EDIT_ARRAY",AM_APPLICATION,
			       (VOIDPTR *)&g_pwe);
    if (status == AM_ERR_NOT_DEFINED)
      {
	status = am_define_workspace("WELL_EDIT_ARRAY",AM_APPLICATION,
				     sizeof(WELL_EDIT_ARRAY),
				     (VOIDPTR *)&g_pwe);
	g_pwe->pwe_array = (WELL_EDIT *) tc_zalloc(sizeof(WELL_EDIT) * 10);
	g_pwe->num_map = 1;
	pwe = g_pwe->pwe_array;
	pwe->map_number = map->map_number;
	strcpy(pwe->map_name,map->map_name);
      }
    else
      {
	for (i=0; i<g_pwe->num_map; i++)
	  {
	    if (map->map_number == (g_pwe->pwe_array+i)->map_number &&
		strcmp(map->map_name,(g_pwe->pwe_array+i)->map_name) == 0)
	      {
		pwe = g_pwe->pwe_array+i;
		break;
	      }
	  }
	if (i == g_pwe->num_map) /* not found one */
	  {
	    if (g_pwe->num_map % 10 == 0)   /* if memory allocation is needed */
	      {
		g_pwe->pwe_array = 
		  (WELL_EDIT *) tc_realloc (g_pwe->pwe_array, 
					    sizeof(WELL_EDIT) *
					    (g_pwe->num_map + 10));
	      }
	    g_pwe->num_map++;
	    pwe = g_pwe->pwe_array + (g_pwe->num_map - 1);
	    pwe->map_number = map->map_number;
	    strcpy(pwe->map_name,map->map_name);
	  }
      }

    if (g_pwe->pwe_array == (WELL_EDIT *) NULL)
      {
	am_message(AM_ALERT, "Could not allocate enough memory");
	return FAIL;
      }

    /* check for one overlay well_depth in the map */
    for (i=0; i<map->display->last_row_used; i++)
      {
	if (map->compiled->verb_code[map->display->compiled_row[i]] == MP_WELLDPTH)
	  {
	    well_depth_ndx = i;
	    mp_set_duom (well_depth_ndx);
	    row = map->display->compiled_row[i];
	    pwe->well_picks = (PICKABLE_WELLS *) map->compiled->picks[row];
	    pwe->overlay_ndx = i;
	    well_depth_count++;
	    if (well_depth_count > 1) /* too many well depth ovelay */
	      {
		am_message(AM_ALERT, mg_message(WE_TOO_MANY_WP));
		return WE_TOO_MANY_WP;
	      }
	  }
	for (i++; map->display->compiled_row[i] != -1; i++);
      }
    if (well_depth_count == 0) /* no well depth overlay */
      {
	am_message(AM_ALERT, mg_message(WE_NO_WP_MAP));
	return WE_NO_WP_MAP;
      }

    if (pwe->seg_id_label == (SEGMENT_ID *) 0)
      {
	pwe->seg_id_label = (SEGMENT_ID *) tc_zalloc 
	  (sizeof (SEGMENT_ID) * (pwe->well_picks->nused + 1));
      }
    if (pwe->seg_id_symbol == (SEGMENT_ID *) 0)
      {
	pwe->seg_id_symbol = (SEGMENT_ID *) tc_zalloc 
	  (sizeof (SEGMENT_ID) * (pwe->well_picks->nused + 1));
      }
    if (pwe->save_well == (BOOL *) 0)
      {
	pwe->save_well = (BOOL *) tc_zalloc 
	  (sizeof (BOOL) * (pwe->well_picks->nused + 1));
      }

    for (i = 0; i < map->compiled->verb_ptr[row]->nrows; i++)
      {
	if (map->compiled->verb_ptr[row]->key_used[i] != 0)
	  {
	    if (map->compiled->verb_ptr[row]->key_code[i] == 10)
	      {   /* supperss symbols */
		pwe->suppress_symbols = TRUE;
	      }
	    if (map->compiled->verb_ptr[row]->key_code[i] == 11)
	      {   /* suppress text */
		pwe->suppress_text = TRUE;
	      }
	  }
	if (map->compiled->verb_ptr[row]->key_code[i] == 19)
	  {	/* HORIZON - MARKER */
	    strcpy(pwe->horizon_name, map->compiled->verb_ptr[row]->parm_ptr[i]->parmval.cval);
	  }
	else if (map->compiled->verb_ptr[row]->key_code[i] == 22)
	  {	/* source */
	    strcpy(pwe->source, map->compiled->verb_ptr[row]->parm_ptr[i]->parmval.cval);
	  }
	else if (map->compiled->verb_ptr[row]->key_code[i] == 21)
	  {	/* label position */
	    pwe->label_position = map->compiled->verb_ptr[row]->parm_ptr[i]->parmval.ival;
	  }
	else if (map->compiled->verb_ptr[row]->key_code[i] == 15)
	  {	/* text_color */
	    pwe->text_color = map->compiled->verb_ptr[row]->parm_ptr[i]->parmval.ival;
	  }
	else if (map->compiled->verb_ptr[row]->key_code[i] == 12)
	  {	/* symbol_color */
	    pwe->symbol_color = map->compiled->verb_ptr[row]->parm_ptr[i]->parmval.ival;
	  }
	else if (map->compiled->verb_ptr[row]->key_code[i] == 16)
	  {	/* text_size */
	    pwe->text_size = map->compiled->verb_ptr[row]->parm_ptr[i]->parmval.dbval;
	  }
	else if (map->compiled->verb_ptr[row]->key_code[i] == 13)
	  {	/* symbol_size */
	    pwe->symbol_size = map->compiled->verb_ptr[row]->parm_ptr[i]->parmval.dbval;
	  }
      }
    if (well_depth_ndx != -1)
      {
	mp_reset_duom (well_depth_ndx);
      }

 /* set up EDIT WELL DEPTHS dialog resource */
    status = am_define_resource(AM_DIALOG, WE_EDIT_CDLG,
	WELL_EDIT_CDLG, we_edit_well_srvr, &pwe->cdlgid, pwe);
    if (status != SUCCESS)
    {
	am_message(AM_ALERT, "Can NOT open resource file !!!");
	we_edit_well_quit(pwe);
	return status;
    }

/* Because of changes in some calls we now use a WELLS structure for getting
   what was displayed on the map. We allocate some space for these strings */
    fi_push();
    /* make the dialog box visible */
    status = wi_open_dialog(pwe->cdlgid);
    if (status != SUCCESS)
    {
	sprintf(error_message_string,
	    "Unable to open dialog in WE_EDIT_WELL_INIT (status = %d)",
	    status);
	am_message(AM_ALERT, error_message_string);
	we_edit_well_quit(pwe);
	return status;
    }

 /* have the user pick a well */
    status = we_pick_one_well(pwe);
    return SUCCESS;
}/* WE_EDIT_WELL_INIT end */


/***************************************************************************/
/*                                                                         */
/*    Routine:        WE_EDIT_WELL_SRVR                                    */
/*                                                                         */
/*    Function:       This routine services events which occur within the  */
/*                    WELL EDIT custom dialog box that was initialized by  */
/*                    the function WE_EDIT_WELL_INIT.                      */
/*                                                                         */
/*    Author:         Robb Hall                                            */
/*    Modified:       Charlie Pumroy/Kee Yip  			           */
/*                                                                         */
/***************************************************************************/
publicdef INT we_edit_well_srvr(id, item, pwe)
INT id;	/* dialog box identifier */
INT item;	/* ITEM that was touched */
WELL_EDIT *pwe;	/* passed data pointer   */
{/* WE_EDIT_WELL_SRVR start */
    INT status;
    INT dummy;
    CHAR well_string[128];
    BOOL ifyes;
    FLOAT orig_new_depth;
    WELL_HDR_2 *well_ptr = (WELL_HDR_2 *) pwe->well_header->detail.ptr;

 /* decide which button was pressed, and process it */
    if (item == WE_WELL_PICK_BUTTON) /* repick button */
      {
	status = we_display_graphic_updates(pwe, FALSE);
	status = we_pick_one_well(pwe);
	return(status);
      }

    status = wiqccdi(pwe->cdlgid, WE_WELL_NDEPTH_ETEXT, &dummy, well_string);
    if (IS_EMPTY_STRING(well_string) == FALSE)
      {
	sscanf(well_string, "%f", &pwe->new_depth);
      }	
    if (item == WE_WELL_QUIT_BUTTON)
      { 
	/* if not save, ask user whether save it or not */
	if (IS_EMPTY_STRING(well_string) == TRUE)
	  {
	    /* updates the well symbol and label to its original states */
	    we_display_graphic_updates(pwe, FALSE);
	    we_edit_well_quit(pwe);
	    return(SUCCESS);
	  }
	else
	  {
	    if (IS_EMPTY_STRING(pwe->orig_new_depth_str) == FALSE)
	      {
		sscanf(pwe->orig_new_depth_str, "%f", &orig_new_depth);
	      }
	    if (IS_EMPTY_STRING(pwe->orig_new_depth_str) == TRUE OR
		pwe->new_depth != orig_new_depth)
	      {
		wiyesno(mg_message(WE_SAVE_CHANGE), "", &ifyes, &status);
		if (!ifyes)
		  {
		    pwe->new_depth = orig_new_depth;
		    /* updates the well symbol and label to its original states */
		    we_display_graphic_updates(pwe, FALSE);
		    we_edit_well_quit(pwe);
		    return(SUCCESS);
		  }
	      }
	    else
	      {
		/* updates the well symbol and label to its original states */
		we_display_graphic_updates(pwe, FALSE);
		we_edit_well_quit(pwe);
		return(SUCCESS);
	      }
	  }
      }

    status = SUCCESS;
    if (item == WE_WELL_SAVE_BUTTON && IS_EMPTY_STRING(well_string) == TRUE) 
      {
	status = WE_NULL_DEPTH;
      }
    else
      {
	if (pwe->has_drillers_td == TRUE && pwe->new_depth < 
	    (well_ptr->elevation.value - pwe->cur_drillers_td))
	  {
	    status = WE_OUTRANGE_DEPTH;
	  }
	else if (pwe->new_depth > well_ptr->elevation.value)
	  {
	    status = WE_SHALLOW_DEPTH;
	  }
      }
    if (status != SUCCESS)
      {
	am_message(AM_ALERT, mg_message(status));
	return status;
      }	    
    else
      {
	status = we_edit_well_updates(pwe);  /* update edit well */
      }
    
    if (item == WE_WELL_QUIT_BUTTON) /* quit edit well */
      {
	/* updates the well symbol and label to its original states */
	we_display_graphic_updates(pwe, FALSE);
	status = we_edit_well_quit(pwe);
      }
    else /* save button */
      {
	status = we_pick_one_well(pwe); /* pick another well depth */
      }

    return(status);

}/* WE_EDIT_WELL_SRVR end */


/***************************************************************************/
/*                                                                         */
/*    Routine:        WE_EDIT_WELL_UPDATES                                 */
/*                                                                         */
/*    Function:       This routine is called when it is necessary to update*/
/*                    the well_top info both on the graphics and databse   */
/*                                                                         */
/*    Author:         Kee Yip                                              */
/*                                                                         */
/***************************************************************************/

publicdef INT we_edit_well_updates(pwe)
WELL_EDIT *pwe;
{/* WE_EDIT_WELL_UPDATES start */
    INT status = SUCCESS;
    TOP_STRUCT ptop;
    WELL_HDR_2 *well_ptr = (WELL_HDR_2 *) pwe->well_header->detail.ptr;

    tc_zeroes(&ptop, sizeof ptop);
    ptop.Depth.md = well_ptr->elevation.value - pwe->new_depth;
    pwe->save_well[pwe->well_pick_ndx] = TRUE;
    /* graphic updates - highlightsymbol = FALSE */
    status = we_display_graphic_updates(pwe, FALSE);
    status = ls_put_top(pwe->well_picks->list[pwe->well_pick_ndx].pWell->uwi, 
			pwe->horizon_name, &ptop); /* commit to database */
    if (status != SUCCESS)
      {
	sprintf(error_message_string,
		"LS_PUT_TOP failure in WE_EMPTY_LIST (status = %d)",
		status);
	am_message(AM_ALERT, error_message_string);
	we_edit_well_quit(pwe);
	return(status);
      }
    return(SUCCESS);

}/* WE_EDIT_WELL_UPDATES end */

/***************************************************************************/
/*                                                                         */
/*    Routine:        WE_EDIT_WELL_QUIT                                    */
/*                                                                         */
/*    Function:       This routine is called when the WELL EDIT DIALOG BOX */
/*                    is to be removed, and its server disconnected.       */
/*                                                                         */
/*    Author:         Robb Hall                                            */
/*    Modified:       Charlie Pumroy/Kee Yip                               */
/*                                                                         */
/***************************************************************************/
publicdef INT we_edit_well_quit(pwe)
WELL_EDIT *pwe;
{/* WE_EDIT_WELL_QUIT start */
    MENU_ID *menu_id;
    
    /* release the resource, if it was defined (that is, ID isn't < 0) */
    if (pwe->cdlgid >= 0)
      {
	am_release_resource(pwe->cdlgid);
      }
    /* (CP) now that we're under MAPPING menu, we look up menu and turn it back on */
    /* quit the application */
    am_open_workspace("MENU", AM_APPLICATION, (VOIDPTR *) & menu_id);
    menu_id->menu_status[0] = ON;
    mp_menu_status();
    fi_pop(); 
    return SUCCESS;
  }/* WE_EDIT_WELL_QUIT end */


/***************************************************************************/
/*                                                                         */
/*    Routine:        WE_PICK_ONE_WELL                                     */
/*                                                                         */
/*    Function:       This routine will allow the     user to point at a   */
/*                    single well (posted on the map), and pick it for     */
/*                    editing.  WE_PICK_ONE_WELL will look-up that well's  */
/*                    UWI, and thereby gain access to its well depths.     */
/*                                                                         */
/*    Author:         Robb Hall                                            */
/*    Modified:       Charlie Pumroy/Kee Yip                               */
/*                                                                         */
/***************************************************************************/
publicdef INT we_pick_one_well(pwe)
WELL_EDIT *pwe;
{/* WE_PICK_ONE_WELL start */

  INT status = SUCCESS;
  INT wmstatus, button;
  INT i;
  FLOAT label_x, label_y;
  MAP_STRUCTURE *map;
  CHAR sqlstmt[512];	/* sql stmt used to draw original label */
  CHAR drawn_label[41];	/* actual label displayed */

  status = mp_front_map(&map);
  if (status != SUCCESS)
    {
      sprintf(error_message_string,
	      "MP_FRONT_MAP failure in WE_PICK_ONE_WELL (status = %d)",
	      status);
      am_message(AM_ALERT, error_message_string);
      we_edit_well_quit(pwe);
      return status;
    }
  
  /* abort if there's not a map window available */
  if (map->map_window_id == 0)
    {
      sprintf(error_message_string,
	      "no MAP WINDOW ID available for WE_PICK_ONE_WELL (status = %d)",
	      status);
      am_message(AM_ALERT, error_message_string);
      we_edit_well_quit(pwe);
      return status;
    }
  
  /* abort if there's not a transformation available */
  if (map->map_tnr == 0)
    {
      sprintf(error_message_string,
	      "no MAP TRANSFORM available for WE_PICK_ONE_WELL (status = %d)",
	      status);
      am_message(AM_ALERT, error_message_string);
      we_edit_well_quit(pwe);
      return status;
    }

  /* open window and select transformation */
  status = wmwopen(map->map_window_id);
  if (status != SUCCESS)
    {
      sprintf(error_message_string,
	      "WMWOPEN failure in WE_PICK_ONE_WELL (status = %d)",
	      status);
      am_message(AM_ALERT, error_message_string);
      we_edit_well_quit(pwe);
      return status;
    }

  status = wmtselect(map->map_tnr);
  if (status != SUCCESS)
    {
      sprintf(error_message_string,
	      "WMTSELECT failure in WE_PICK_ONE_WELL (status = %d)",
	      status);
      am_message(AM_ALERT, error_message_string);
      we_edit_well_quit(pwe);
      return status;
    }

  /* let the user pick a well from the map */
  /* dont confuse user if the pick fails, so we clean up dialog */
  wi_set_dialog_text(pwe->cdlgid, WE_WELL_MESSAGE, mg_message(WE_MESSAGE1));
  wi_set_dialog_text(pwe->cdlgid, WE_WELL_UWI_STEXT, "");
  wi_set_dialog_text(pwe->cdlgid, WE_WELL_SNAME_STEXT, "");
  wi_set_dialog_text(pwe->cdlgid, WE_WELL_FORMATION_STEXT, "");
  wi_set_dialog_text(pwe->cdlgid, WE_WELL_SUBSEADEPTH_STEXT, "");
  wi_set_dialog_text(pwe->cdlgid, WE_WELL_CSOURCE_STEXT, "");
  wi_set_dialog_text(pwe->cdlgid, WE_WELL_CDEPTH_STEXT, "");
  wi_set_dialog_text(pwe->cdlgid, WE_WELL_NSOURCE_STEXT, "");
  wi_set_dialog_text(pwe->cdlgid, WE_WELL_NDEPTH_ETEXT, "");
  
  wi_enable_dialog_item(pwe->cdlgid, WE_WELL_SAVE_BUTTON, OFF);
  wi_enable_dialog_item(pwe->cdlgid, WE_WELL_PICK_BUTTON, OFF);
  wi_enable_dialog_item(pwe->cdlgid, WE_WELL_QUIT_BUTTON, OFF);
  wi_enable_dialog_item(pwe->cdlgid, WE_WELL_NDEPTH_ETEXT, OFF);

  status = wmtlocate(&wmstatus, &label_x, &label_y, &button);
  
  /* discover if DONE can be processed here (user wants to CANCEL) */
  if (button == WM_BUTTON_DONE OR button == WM_BUTTON_CANCEL)
    {
      we_edit_well_quit(pwe);
      return SUCCESS;	
    }
  else
    {
      pwe->label_x = label_x;
      pwe->label_y = label_y;
    }
  if (status != SUCCESS)
    {
      sprintf(error_message_string,
	      "WMTLOCATE failure in WE_PICK_ONE_WELL (status = %d)",
	      status);
      am_message(AM_ALERT, error_message_string);
      we_edit_well_quit(pwe);
      return status;
    }


  /* (CP) and a few other things. This replaces the gb_pick_well call */
  /* (CP) for graphics, we also need to capture the text color and size
     to overwrite in highlight color and calculate the size of the "postit"
     which goes over the text */
  status = we_get_well_xy(pwe->label_x, pwe->label_y, 
			   &pwe->label_x, &pwe->label_y, 
			   pwe->well_picks, &pwe->well_pick_ndx);

  /* echo the well's info, so the user knows what's been picked */
  wi_set_dialog_text(pwe->cdlgid, WE_WELL_MESSAGE, mg_message(WE_MESSAGE2));
  wi_enable_dialog_item(pwe->cdlgid, WE_WELL_SAVE_BUTTON, ON);
  wi_enable_dialog_item(pwe->cdlgid, WE_WELL_PICK_BUTTON, ON);
  wi_enable_dialog_item(pwe->cdlgid, WE_WELL_QUIT_BUTTON, ON);
  wi_enable_dialog_item(pwe->cdlgid, WE_WELL_NDEPTH_ETEXT, ON);
  status = we_display_well_info(pwe);
  if (status != SUCCESS)
    {
      if (status == WE_NO_ELEVATION)
	{
	  am_message(AM_ALERT, mg_message(status));
	  status = we_pick_one_well(pwe);
	}
      else
	{
	  sprintf(error_message_string,
		  "WE_DISPLAY_WELL_INFO failure in WE_PICK_ONE_WELL (status = %d)",
		  status);
	  am_message(AM_ALERT, error_message_string);
	  we_edit_well_quit(pwe);
	}
      return status;
    }

  /* lookup that well's WELL HEADER */
  status = 
    we_get_well_header(pwe->well_picks->list[pwe->well_pick_ndx].pWell->uwi,
		       WELL_HDR_2_DETAIL, &pwe->well_header);
  if (status != SUCCESS)
    {
      sprintf(error_message_string,
	      "WE_GET_WELL_HEADER failure in WE_PICK_ONE_WELL (status = %d)",
	      status);
      am_message(AM_ALERT, error_message_string);
      we_edit_well_quit(pwe);
      return status;
    }

  /* update graphics - highlightsymbol = TRUE */
  status = we_display_graphic_updates(pwe, TRUE);
  return status;
}/* WE_PICK_ONE_WELL end */


/***************************************************************************/
/*                                                                         */
/*    Routine:        WE_DISPLAY_GRAPHIC_UPDATES                           */
/*                                                                         */
/*    Function:       This routine update the well symbol and well label   */
/*                    text                                                 */
/*                                                                         */
/*    Author:         Kee Yip                                              */
/*                                                                         */
/***************************************************************************/
publicdef INT we_display_graphic_updates(pwe, highlightsymbol)
WELL_EDIT *pwe;
BOOL highlightsymbol;
{
  CHAR label[31];
  INT status, dummy;
  INT hor_align;
  INT vert_align;
  COLOR_INDEX color_code;
  SEGMENT_ID seg_id;
  FLOAT x_offset, y_offset;
  FLOAT label_x, label_y;
  MAP_STRUCTURE *map;
  
  label[0] = 0;
  mp_front_map(&map);
  wmwopen(map->map_window_id);
  wmtselect(map->map_tnr);

  if (pwe->seg_id_symbol[pwe->well_pick_ndx] != 0)
    {
      wmsdelete(pwe->seg_id_symbol[pwe->well_pick_ndx]);
    }
  wmsallocate(&seg_id);
  wmoset(seg_id, WM_CLASS_WELL_SYMBOL, "", "SYMBOL");
  pwe->seg_id_symbol[pwe->well_pick_ndx] = seg_id;
  if (highlightsymbol == TRUE ||
      (highlightsymbol == FALSE &&
       pwe->save_well[pwe->well_pick_ndx] == TRUE))
    {
      lu_text_to_index("COLOR", &color_code, "HIGHLIGHT");
      wm_gsplci (color_code);
    }
  else if (pwe->suppress_symbols == FALSE)
    { /* symbol original color */
      color_code = pwe->symbol_color;
      wm_gsplci (color_code);
    }
  wm_gsln (1);
  wm_gslwsc (1.0);
  strcpy (pwe->symbol.symbol_group, CS_V4_WELLS_GROUP);
  pwe->symbol.symbol_color = color_code;
  pwe->symbol.symbol_size = pwe->symbol_size;
  pwe->symbol.symbol_location.x = (GKS_REAL) pwe->label_x;
  pwe->symbol.symbol_location.y = (GKS_REAL) pwe->label_y;
  pwe->symbol.symbol_type = 
    pwe->well_picks->list[pwe->well_pick_ndx].pWell->symbol_code;
  mm_draw_symbol (&pwe->symbol);
  status = wmsclose(seg_id);
  status = wmsvis(seg_id, GVISI);

  if (pwe->seg_id_label[pwe->well_pick_ndx] != 0)
    {
      wmsdelete(pwe->seg_id_label[pwe->well_pick_ndx]);
    }
  wmsallocate(&seg_id);
  pwe->seg_id_label[pwe->well_pick_ndx] = seg_id;

  switch (pwe->label_position) /* reference from wl_overlay */
    {
    case 1: /* above */
      wmoset(seg_id, WM_CLASS_TEXT, "", "ABOVE");
      break;
    case 2: /* below */      
      wmoset(seg_id, WM_CLASS_TEXT, "", "BELOW");
      break;
    case 3: /* center */
      wmoset(seg_id, WM_CLASS_TEXT, "", "CENTER");
      break;
    case 4: /* left */
      wmoset(seg_id, WM_CLASS_TEXT, "", "LEFT");
      break;
    case 5: /* right */
      wmoset(seg_id, WM_CLASS_TEXT, "", "RIGHT");
      break;
    }
  
  wm_gschh(pwe->text_size);
  wm_gschup(0.0, 1.0);
  wm_gstxp(0);
  wm_gstxfp(SIMPLEX, GSTRKP);
  wm_gschxp(1.0);
  wm_gschsp(0.0);
  
  switch (pwe->label_position) /* reference from wl_overlay */
    {
    case 1: /* above */
      hor_align = 2;
      vert_align = 5;
      x_offset = 0.0;
      y_offset = 0.75 * MAX(pwe->symbol_size, pwe->text_size);
      break;
    case 2: /* below */
      hor_align = 2;
      vert_align = 1;
      x_offset = 0.0;
      y_offset = (-0.75) * MAX(pwe->symbol_size, pwe->text_size);
      break;
    case 3: /* center */
      hor_align = 2;
      vert_align = 3;
      x_offset = 0.0;
      y_offset = 0.0;
      break;
    case 4: /* left */
      hor_align = 3;
      vert_align = 3;
      x_offset = (-0.75) * pwe->symbol_size;
      y_offset = 0.0;
      break;
    case 5: /* right */
      hor_align = 1;
      vert_align = 3;
      x_offset = 0.75 * pwe->symbol_size;
      y_offset = 0.0;
      break;
    default:
      break;
    }
  wm_gstxal(hor_align, vert_align);
  label_x = pwe->label_x + x_offset;
  label_y = pwe->label_y + y_offset;

  /* good bye to the old */
  lu_text_to_index("COLOR", &color_code, "BACKGROUND");
  wm_gstxci(color_code);
  wiqccdi (pwe->cdlgid, WE_WELL_CDEPTH_STEXT, &dummy, label);
  if (IS_EMPTY_STRING(label) == FALSE && 
      strlen(label) > 2 &&
      label[strlen(label) - 1] == '0' &&
      label[strlen(label) - 2] == '.')
    {
      label[strlen(label) - 2] = 0;
    }
  wm_gtx(label_x, label_y, label);
  wm_gtx(label_x, label_y, pwe->orig_new_depth_str);

  /* hello to the new */
  if (pwe->save_well[pwe->well_pick_ndx] == TRUE)
    {
      lu_text_to_index("COLOR", &color_code, "HIGHLIGHT");
      wm_gstxci(color_code);
      sprintf(label, "%.1f", pwe->new_depth);
      if (IS_EMPTY_STRING(label) == FALSE && 
	  strlen(label) > 2 &&
	  label[strlen(label) - 1] == '0' &&
	  label[strlen(label) - 2] == '.')
	{
	  label[strlen(label) - 2] = 0;
	}
      wm_gtx(label_x, label_y, label);
    }
  else if (pwe->suppress_text == FALSE)
    {
      wm_gstxci(pwe->text_color);
      wm_gtx(label_x, label_y, label);
    }
  status = wmsclose(seg_id);
  status = wmsvis(seg_id, GVISI);

  return SUCCESS;
}










