/* DEC/CMS REPLACEMENT HISTORY, Element DR_NEW_TEXT.C*/
/* *3    18-JUL-1990 08:32:04 GILLESPIE "(SPR 1) Add esi_dr_msg.h"*/
/* *2    27-JUL-1989 15:04:39 GILLESPIE "(SPR 1) Fix minor casing problem"*/
/* *1    19-JUN-1989 12:47:17 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element DR_NEW_TEXT.C*/

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

NAME:         dr_new_text

DESCRIPTION:  Create a new text element


     Input:   dr_data - The standard data structure

     Output:  dr_data - Updated

     Status:  SUCCESS

******************************************************************************/

#include    "esi_dr.h"
#ifndef ESI_MP_H
#include    "esi_mp.h"
#endif
#ifndef ESI_AM_H
#include    "esi_am.h"
#endif
#ifndef ESI_WM_H
#include    "esi_wm.h"
#endif
#ifndef ESI_MG_H
#include    "esi_mg.h"
#endif
#include "esi_dr_msg.h"

#if USE_PROTOTYPES
publicdef INT dr_new_text(DR_DATA *dr_data)
#else
publicdef INT dr_new_text(dr_data)
DR_DATA *dr_data;
#endif
{
    INT status;
    OBJECT_STRUCTURE *this_object;
    INT map_tnr;
    INT nlen;
    MAP_STRUCTURE *mapdef;
    DOUBLE x0,y0,fx,fy;

    dr_data->current_text_seg = 0;

/*  Create new text structure & append to list of texts  */

    dr_append_object( dr_data, (VOIDPTR)&dr_data->default_text, DR_TEXT_OBJECT, TRUE);

    dr_new_project_id(&dr_data->current_text->text_id);

/*  Display the new element at the center of the current window  */

    mp_get_current_map(&mapdef);
    wmtinqpanzoom(mapdef->map_tnr,&x0,&y0,&fx,&fy);

    dr_data->current_text->text_position.x = x0;
    dr_data->current_text->text_position.y = y0;
    if( dr_data->current_text->text_string != (CHAR *) 0)
	am_free(dr_data->current_text->text_string );
    dr_data->current_text->text_string = (CHAR *)am_allocate(AM_APPLICATION,
						    DR_MAX_TEXT_STRING);
    strcpy(dr_data->current_text->text_string,mg_message(DR_NEW_TEXT));

    return SUCCESS;
}
