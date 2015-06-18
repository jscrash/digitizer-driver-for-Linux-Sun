/* DEC/CMS REPLACEMENT HISTORY, Element DR_REDRW_TXT.C*/
/* *3    12-OCT-1990 16:37:31 GILLESPIE "(SPR 100) Merge Ernie Deltas"*/
/* *2    18-JUL-1989 14:46:17 GORDON "(SPR 100) Add mods from Sun port"*/
/* *1    19-JUN-1989 12:48:14 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element DR_REDRW_TXT.C*/
/* DEC/CMS REPLACEMENT HISTORY, Element DR_REDRW_TXT.C*/
/* *2    18-JUL-1989 14:46:17 GORDON "(SPR 100) Add mods from Sun port"*/
/* *1    19-JUN-1989 12:48:14 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element DR_REDRW_TXT.C*/

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

/* DEC/CMS REPLACEMENT HISTORY, Element DR_REDRAW_TEXT.C*/
/* *7    15-FEB-1989 15:38:02 JAMES "(SPR 5002) Pass right length to tc_zalloc()."*/
/* *6     1-FEB-1989 12:21:50 GORDON "(SPR 4981) add esi_tc.h"*/
/* *5    16-JAN-1989 10:39:23 JAMES "(SPR 5002) To finish the software picking task."*/
/* *4    17-DEC-1988 16:02:52 GORDON "(SPR 4981) Unisource"*/
/* *3     5-OCT-1988 14:02:34 CHARLIE "(SPR 4906) Clipping the objects where the system redraws objects."*/
/* *2    13-JUL-1987 08:05:34 SCD "Kill clipping"*/
/* *1     7-JUL-1987 10:40:00 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element DR_REDRAW_TEXT.C*/
/******************************************************************************

NAME:           dr_redraw_text

DESCRIPTION:    Remove segment and redraw


     Input:     dr_data - The standard data structure

     Output:    dr_data - Updated

     Status:    SUCCESS

******************************************************************************/

#include    "esi_gl_defs.h"
#ifndef ESI_MP_H
#include    "esi_mp.h"
#endif
#ifndef ESI_DR_H
#include    "esi_dr.h"
#endif
#ifndef ESI_WM_H
#include    "esi_wm.h"
#endif
#ifndef ESI_WM_CLS_H
#include    "esi_wm_cls.h"
#endif
#ifndef ESI_GKS_H
#include    "esi_gks.h"
#endif
#ifndef ESI_TC_H
#include "esi_tc.h"
#endif

/******************************************************************************/
/*   Global Definitions                                                       */
/******************************************************************************/

#define null_string ""

/******************************************************************************/
/*   End of Global Definitions                                                */
/******************************************************************************/

publicdef INT dr_redraw_text(dr_data)
DR_DATA *dr_data;
{
    INT status;
    INT old_segment = dr_data->current_text_seg;
    CHAR pos[20];

/* allocated the internal segments control structure. */
    if( dr_data -> mod_obj_segments.nallocated == 0)
        {
        dr_data -> mod_obj_segments.nallocated = 10;
        dr_data -> mod_obj_segments.old_seg_array = (INT *)tc_zalloc(
                            sizeof(INT ) * 10 );
        dr_data -> mod_obj_segments.new_seg_array = (INT *)tc_zalloc(
                            sizeof(INT ) * 10 );
        }
    else if( dr_data -> mod_obj_segments.nallocated ==
        dr_data -> mod_obj_segments.nused )
        {
        dr_data -> mod_obj_segments.nallocated += 10;
        dr_data -> mod_obj_segments.old_seg_array = (INT *)tc_realloc(
                dr_data -> mod_obj_segments.old_seg_array,
                dr_data -> mod_obj_segments.nallocated * sizeof(INT));
        dr_data -> mod_obj_segments.new_seg_array = (INT *)tc_realloc(
                dr_data -> mod_obj_segments.new_seg_array,
                dr_data -> mod_obj_segments.nallocated * sizeof(INT));
        }

    if ( dr_data->current_text != (TEXT *) NULL)
        {
        /* store the old segment to the array */
    
        dr_data -> mod_obj_segments.old_seg_array[
            dr_data -> mod_obj_segments.nused] = dr_data->current_text_seg;
        ov_new_element( &dr_data->current_text_seg,
                    WM_CLASS_TEXT,
                    null_string,
                    dr_data->current_text->text_id );
    
        mm_draw_text(dr_data->current_text,TRUE);
    
        wmsclose(dr_data->current_text_seg);
        
        /* wm_update(); */
        
        dr_data -> mod_obj_segments.new_seg_array[
            dr_data -> mod_obj_segments.nused++] = dr_data->current_text_seg;
        }    

    if( old_segment != 0)
        {
        wmsvis( old_segment, GINVIS);
        }

    return SUCCESS;
}
                                                                                                                
