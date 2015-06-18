/* DEC/CMS REPLACEMENT HISTORY, Element DR_EL_DR.C*/
/* *8    12-OCT-1990 23:36:42 GILLESPIE "(SPR 1) Merge Ernie deltas"*/
/*  6B1  12-OCT-1990 16:25:23 GILLESPIE "Merge Ernie Deltas"*/
/* *7    14-AUG-1990 16:17:23 GILLESPIE "(SPR 2001) Change to use LINE_ATTRIBUTEs"*/
/* *6    18-JUL-1990 10:57:00 GILLESPIE "(SPR 1) Add esi_dr_err.h"*/
/* *5    12-JUN-1990 10:35:29 KEE "(SPR -1) Bug fixing and new enhancement of Electronic Drafting Text Edit for (SCD)"*/
/* *4    19-SEP-1989 13:36:31 GILLESPIE "(SPR 100) GULF MODS"*/
/* *3    20-JUL-1989 13:04:34 GORDON "(SPR 100) Sun port mods"*/
/* *2    26-JUN-1989 10:41:18 GILLESPIE "(SPR 20) Fix castings"*/
/* *1    19-JUN-1989 12:46:35 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element DR_EL_DR.C*/
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

NAME:          dr_electronic_drafting

DESCRIPTION:   Root program for the electronic drafting module



     Input:    Standard AM stuff

     Output:   None

     Status:   SUCCESS

******************************************************************************/

#include    "esi_dr.h"
#ifndef ESI_MP_H
#include    "esi_mp.h"
#endif
#ifndef ESI_FI_H
#include    "esi_fi.h"
#endif
#ifndef ESI_AM_H
#include    "esi_am.h"
#endif
#ifndef ESI_WI_H
#include    "esi_wi.h"
#endif
#ifndef ESI_WM_H
#include    "esi_wm.h"
#endif
#ifndef ESI_LU_H
#include "esi_lu.h"
#endif
#ifndef ESI_MG_H
#include    "esi_mg.h"
#endif

#include    "esi_dr_err.h"
#include    "dr_menus.rh"

/******************************************************************************/
/*   Global Definitions                                                       */
/******************************************************************************/

#define PICK_FACTOR 0.01
#define TEXT_FACTOR 0.01

/******************************************************************************/
/*   End of Global Definitions                                                */
/******************************************************************************/

publicdef INT dr_electronic_drafting(p)
BYTE *p;
{
    MAP_STRUCTURE *mapdef;
    CDLG_ID *cdlg_id;
    MENU_ID *menu_id;
    MAP_NAME map_name;
    INT nlen;
    INT status=0;
    INT i,j,k;
    DR_DATA *dr_data = {0};
    CHAR **object_type_list;
    CHAR go_of_map[128];
    INT nitems;
    FLOAT wx_min,wx_max,wy_min,wy_max;
    FLOAT vx_min,vx_max,vy_min,vy_max;
    GO_WORKSPACE *w, **global_w;


/*  Make this application a black pipe   */

    fi_push();

/*  Get the current map   */

    status = mp_front_map(&mapdef);
    if(status != SUCCESS)
    {
    	am_message(AM_ALERT,mg_message(DR_NO_MAP));
        fi_pop();
	am_open_workspace("MENU",AM_APPLICATION,(VOIDPTR *)&menu_id);

/* * TURN ON THE MAPPING MENU * */
/* ----------------------------- */

	menu_id->menu_status[0] = ON;
	mp_menu_status();
        return SUCCESS;
    }


/*  Open or Allocate the main data structure  */

    if(am_open_workspace("DR_DATA",AM_APPLICATION,(VOIDPTR *)&dr_data) != SUCCESS)
    {
    status = am_define_workspace("DR_DATA",AM_APPLICATION,
                                      sizeof(DR_DATA),(VOIDPTR *)&dr_data);
    }
    else
    {
	dr_data -> current_object = dr_data -> current_object_key = 0;
    }
    if(status != SUCCESS)
    {
        am_message(AM_ALERT,mg_message(status));
        fi_pop();
	mp_dim_window(mapdef->map_name);
	am_open_workspace("MENU",AM_APPLICATION,(VOIDPTR *)&menu_id);

/* * TURN ON THE MAPPING MENU * */
/* ----------------------------- */

	menu_id->menu_status[0] = ON;
	mp_menu_status();
        return SUCCESS;
    }
        
/*  Initialize resources & menus  */

    status = dr_init_resources(dr_data);
    if(status != SUCCESS)
    {
        am_message(AM_ALERT,mg_message(status));

/*  Wipe out any orphaned status box  */

        am_message(AM_DIALOG,NULL);
    
/*	Close the little dialog that signifies electronic drafting */

    	status = am_open_workspace("CDLG_ID",AM_APPLICATION,(VOIDPTR *)&cdlg_id);
        if(status EQUALS SUCCESS)
        {
            if(cdlg_id->base_dialog_id != 0)
                  wi_close_dialog( cdlg_id->base_dialog_id );
        }
        fi_pop();
	mp_dim_window(mapdef->map_name);
	am_open_workspace("MENU",AM_APPLICATION,(VOIDPTR *)&menu_id);

/* * TURN ON THE MAPPING MENU * */
/* ----------------------------- */

	menu_id->menu_status[0] = ON;
	mp_menu_status();
        return SUCCESS;
    }


/* Open or define the static workspace for picking */

    status = am_open_workspace("GO_STATIC_SPACE",AM_GLOBAL,(VOIDPTR *)&global_w);
    if (status != SUCCESS)
	{
	status = am_define_workspace("GO_STATIC_SPACE",AM_GLOBAL,
				sizeof(GO_WORKSPACE **),(VOIDPTR *)&global_w);
	}
/* open the GO workspace for this specified map. */
    
    sprintf(go_of_map, "GO_SPACE_%d", mapdef->map_number);
    status = am_open_workspace(go_of_map, AM_GLOBAL,(VOIDPTR *)&w);
    if (status != SUCCESS)
	{
	status = am_define_workspace(go_of_map,AM_GLOBAL,
				    sizeof(GO_WORKSPACE),(VOIDPTR *)&w);
	w -> changed = TRUE;
	}
    *global_w = w;    
    if (w->changed EQUALS TRUE)
    	{
	w->changed = FALSE;
    	if (w->temp_nlist != (NLIST_HEADER) NULL)
	    status = nl_free_nlist(w->temp_nlist);    
	
    	if (w->master_nlist != (NLIST_HEADER) NULL)
	    status = nl_free_nlist(w->master_nlist);
	w-> temp_nlist = w -> master_nlist = (NLIST_HEADER ) NULL;
    	status = go_build_common_nlist(&w->temp_nlist);
    	status = nl_copy_nlist(w->temp_nlist,&w->master_nlist);
	}

    status = SUCCESS;    
    
/*  Open the map window   */

    wmwopen(mapdef->map_window_id);
    wmtselect(mapdef->map_tnr);

/*	Indicate no object type selected yet */

    dr_data->current_object_type[0] = 0;
    dr_data->current_object = 0;
    dr_data->current_overlay = 0;
    dr_data->named_overlay = 0;
        
/*  Define the pick aperture    */

    wmtinq(mapdef->map_tnr,&vx_min,&vx_max,&vy_min,&vy_max,
                           &wx_min,&wx_max,&wy_min,&wy_max);
    dr_data->x_toler = (wx_max - wx_min)*PICK_FACTOR;
    dr_data->y_toler = (wy_max - wy_min)*PICK_FACTOR; 

/*  Init the text editor  */

    dr_data->current_text = (TEXT *)NULL;
    dr_data->current_text_seg = 0;

/*	Allocate default TEXT and init attributes */
    
    dr_data->default_text.text_size = (wy_max - wy_min)*TEXT_FACTOR;
    dr_data->default_text.text_line_spacing = 
  		DR_DEFAULT_LINE_SPACING * dr_data->default_text.text_size;
    dr_data->default_text.text_expansion_factor = 1.0; 

    lu_text_to_index( "TEXT_HORIZ_ALIGN", 
        &dr_data->default_text.text_alignment[0], "CENTER" );  
    lu_text_to_index( "TEXT_VERT_ALIGN", 
        &dr_data->default_text.text_alignment[1], "HALF" );
    lu_text_to_index( "TEXT_PATH", &dr_data->default_text.text_path,
        "RIGHT" );
    dr_data->default_text.text_up_x = 0.;		/*  90 deg.*/
    dr_data->default_text.text_up_y = 1.;
    dr_data->default_text.box_flag = FALSE;
    lu_text_to_index( "FILL_STYLE", &dr_data->default_text.box_fill_code,
        "HOLLOW" );
    lu_text_to_index( "COLOR", &dr_data->default_text.box.color,
        "NORMAL" );
    lu_text_to_index( "LINE_STYLE", &dr_data->default_text.box.style,
        "SOLID" );
    dr_data->default_text.box.width = 1.; 
    dr_data->default_text.arrow_flag = FALSE;
    lu_text_to_index( "FONT", &dr_data->default_text.text_font, "SIMPLEX" );
    lu_text_to_index( "COLOR", &dr_data->default_text.text_color, "BLACK" );
    lu_text_to_index( "TEXT_PRECISION", 
        &dr_data->default_text.text_precision, "CHARACTER" );
    dr_data->default_text.text_expansion_factor = 1.;
	
    dr_data->default_text.text_offset.x = 0.0;
    dr_data->default_text.text_offset.y = 0.0;

/*	Allocate default LINE and init attributes */
   		       
    dr_data->default_line.line_nlist = ((NLIST_HEADER) 0 );
   		       
    lu_text_to_index( "COLOR", &dr_data->default_line.line.color, "GREEN" );
    lu_text_to_index( "LINE_STYLE", &dr_data->default_line.line.style,
        "SOLID" );
    dr_data->default_line.line.width = 1.0;
    dr_data->default_line.polygon_flag = FALSE;
    lu_text_to_index( "FILL_STYLE", 
        &dr_data->default_line.polygon_fill_style, "HOLLOW" );
    lu_text_to_index( "COLOR", &dr_data->default_line.polygon_fill_color,
        "GREEN" );
    
    return SUCCESS;
}
