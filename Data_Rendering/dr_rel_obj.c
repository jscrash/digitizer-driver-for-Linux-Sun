/* DEC/CMS REPLACEMENT HISTORY, Element DR_REL_OBJ.C*/
/* *3    31-JUL-1990 22:03:12 GILLESPIE "(SPR 1) Added esi_gks.h"*/
/* *2    14-SEP-1989 18:14:22 GORDON "(SPR -1) checkin from gulf"*/
/* *1    19-JUN-1989 12:48:17 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element DR_REL_OBJ.C*/

/******************************************************************************

NAME:        dr_release_object

DESCRIPTION: Releases locks & window manager for current object


     Input:  dr_data - Standard data structure

     Output: dr_data - Updated

     Status: SUCCESS

******************************************************************************/

#include    "esi_dr.h"
#include    "esi_wm.h"
#ifndef ESI_GKS_H
#include "esi_gks.h"
#endif

#if USE_PROTOTYPES
publicdef INT dr_release_object(DR_DATA *dr_data)
#else
publicdef INT dr_release_object(dr_data)
DR_DATA *dr_data;
#endif
{
    INT status, i;
    INT main_wkid = MAIN_WKID; 

/*  Close the group    */
    for(i = 0; i < dr_data -> mod_obj_segments.nused; i++)
    {
	wmsvis(dr_data -> mod_obj_segments.old_seg_array[i], GVISI);
    }
    for(i = 0; i < dr_data -> mod_obj_segments.nused; i++)
    {
	wmsdelete(dr_data -> mod_obj_segments.new_seg_array[i]);
    }
    dr_data -> mod_obj_segments.nused = 0;

    wmgclose(dr_data->current_object);

/*  Release all locks  */

    if(!dr_data->new_object)
    {
        dr_commit(TRUE);
    }
    else
    {
         wmgdelete(dr_data->current_object);
    }
    wm_grsgwk( main_wkid );		/* Redraw to erase */

    status = dr_reset_object(dr_data);

    return SUCCESS;
}


