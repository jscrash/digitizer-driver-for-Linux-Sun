/* DEC/CMS REPLACEMENT HISTORY, Element DR_CHG_TP.C*/
/* *2    18-JUL-1990 07:34:29 GILLESPIE "(SPR 1) Add esi_dr_err.h"*/
/* *1    19-JUN-1989 12:46:12 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element DR_CHG_TP.C*/

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

NAME:           dr_change_object_type

DESCRIPTION:    Change the current object type


     Input:     Object_type - The new object type
                dr_data - The central data structure

     Output:    dr_data - updated

     Status:    SUCCESS

******************************************************************************/

#include    "esi_dr.h"
#ifndef ESI_FI_H
#include    "esi_fi.h"
#endif
#ifndef ESI_QS_H
#include    "esi_qs.h"
#endif
#ifndef ESI_AM_H
#include    "esi_am.h"
#endif
#ifndef ESI_WI_H
#include    "esi_wi.h"
#endif
#ifndef ESI_MD_BASIS_X
#include    "esi_md_basis.x"
#endif
#ifndef ESI_MG_H
#include "esi_mg.h"
#endif
#ifndef ESI_MP_H
#include "esi_mp.h"
#endif
#ifndef ESI_TS_H
#include "esi_ts.h"
#endif
#ifndef ESI_WM_H
#include "esi_wm.h"
#endif

#include    "dr_menus.rh"
#include    "esi_dr_err.h"

/******************************************************************************/
/*   Global Definitions                                                       */
/******************************************************************************/

#define null_string ""

/******************************************************************************/
/*   End of Global Definitions                                                */
/******************************************************************************/

#if USE_PROTOTYPES
publicdef INT dr_change_object_type( CHAR *object_type, DR_DATA *dr_data )
#else
publicdef INT dr_change_object_type( object_type, dr_data )
CHAR *object_type;
DR_DATA *dr_data;
#endif
{
    INT status;
    ANALYST_NAME user_name;
    INT i,j;
    FILENAME graphic_object_codes;
    CHAR msg[81];
    ORACLE_NAME std_name;
    MAP_STRUCTURE *map;
    MD_DISPLAY *display;
    MD_COMPILED_MAPDEF *compiled;
    MD_KEY_INFO *temp_key;
    INT target_verb_ndx,target_key_ndx;


/*  Close last overlay group  */

    if (dr_data->current_overlay > 0)
	{
        wmgclose(dr_data->current_overlay);
	dr_data->current_overlay = 0;

	if (dr_data->named_overlay > 0)
	    {
	    wmgclose(dr_data->named_overlay);
	    dr_data->named_overlay = 0;
	    }
	}

/*  Get the graphics object overlay class  */

    dr_data->current_overlay_class = MD_NAMED_OVERLAY + MP_GRA_OBJS;

/*  Look up the object class  */

    qs_inq_c(QS_PROJECT_NAME,graphic_object_codes,&i);
    strcat(graphic_object_codes,".GRAPHIC_OBJECT_CODES");
    
    status = dr_table_lookup_dynamic(graphic_object_codes,object_type,
               &dr_data->current_object_class);
               
    if(status != SUCCESS)
    {
        sprintf(msg,mg_message(DR_BAD_OBJECT_TYPE),object_type,
                                      graphic_object_codes);
        am_message(AM_ALERT,msg);
        return status;
    }

/*  Reset the state variables    */

    dr_data->current_marker[0] = 0;
    dr_data->current_top[0] = 0;
    dr_data->current_base[0] = 0;
    strcpy(dr_data->current_object_type,object_type);
    dr_data->current_object_key = 0;
    dr_data->current_object = 0;

/*  Get the verb and keyword indices to look for in the display structure */

    for (i=0; i<MD_MAX_OVERLAY_TYPES; i++)
	{
	if (overlay_verbs[i].verb_code EQUALS MP_GRA_OBJS)
	    {
	    target_verb_ndx = i;
	    for (j=0; j<overlay_verbs[i].no_of_keywords; j++)
		{
		temp_key = &(overlay_verbs[i].key_list[j]);
		if (temp_key->key_code EQUALS 20)
		    {
		    target_key_ndx = j;
		    break;
		    }
		}
	    break;
	    }
	}

/*  Open the overlay group, if it already exists in WM tree   */

    mp_get_current_map(&map);
    display = map->display;
    compiled = map->compiled;
    for (i=0; i<display->last_row_used; i++)
	{
	if (display->verb_ndx[i] EQUALS target_verb_ndx  AND
		    display->key_ndx[i] EQUALS target_key_ndx  AND
		    strcmp(display->parameters[i],object_type) EQUALS 0)
	    {
	    dr_data->overlay_exists = TRUE;
	    if (compiled->verb_wm_id[display->compiled_row[i]] EQUALS 0)
		{
		dr_data->current_overlay = 
			    compiled->overlay_wm_id[display->compiled_row[i]];
		wmgopen(dr_data->current_overlay);
		}
	    else
		{
		dr_data->current_overlay = 
			    compiled->verb_wm_id[display->compiled_row[i]];
		dr_data->named_overlay = 
			    compiled->overlay_wm_id[display->compiled_row[i]];
		wmgopen(dr_data->named_overlay);
		wmgopen(dr_data->current_overlay);
		}
	    break;
	    }
	}
	    
    if(dr_data->current_overlay == 0)
    {
    	dr_data->overlay_exists = FALSE;
        wmgallocate(&dr_data->current_overlay);
        strcpy(std_name, dr_data->current_object_type);
        ts_standard_name(std_name);
        sprintf( user_name, "%s Overlay", std_name);
        wmoset(	dr_data->current_overlay,
        		dr_data->current_overlay_class,
            	null_string,
            	user_name);
    }
    else 
    {
    }

    return SUCCESS;
}

