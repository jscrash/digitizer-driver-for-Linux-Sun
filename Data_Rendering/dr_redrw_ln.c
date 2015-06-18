/* DEC/CMS REPLACEMENT HISTORY, Element DR_REDRW_LN.C*/
/* *2    12-OCT-1990 16:36:28 GILLESPIE "(SPR 100) Merge Ernie Deltas"*/
/* *1    19-JUN-1989 12:48:07 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element DR_REDRW_LN.C*/
/* DEC/CMS REPLACEMENT HISTORY, Element DR_REDRW_LN.C*/
/* *1    19-JUN-1989 12:48:07 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element DR_REDRW_LN.C*/

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


/* DEC/CMS REPLACEMENT HISTORY, Element DR_REDRAW_LINE.C*/
/* *7    15-FEB-1989 15:36:48 JAMES "(SPR 5002) pass right length to TC_REALLOC()."*/
/* *6     1-FEB-1989 12:18:48 GORDON "(SPR 4981) add esi_tc.h"*/
/* *5    16-JAN-1989 10:38:26 JAMES "(SPR 5002) To finish the software picking task."*/
/* *4    17-DEC-1988 16:02:46 GORDON "(SPR 4981) Unisource"*/
/* *3     5-OCT-1988 14:04:42 CHARLIE "(SPR 4906) Clipping the mobjects when the system redraws objects."*/
/* *2    13-JUL-1987 08:04:42 SCD "Kill clipping"*/
/* *1     7-JUL-1987 10:39:56 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element DR_REDRAW_LINE.C*/
/******************************************************************************

NAME:           dr_redraw_line

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
#ifndef ESI_LU_H
#include "esi_lu.h"
#endif

publicdef INT dr_redraw_line (dr_data)
DR_DATA *dr_data;
    {
    INT status = 0;
    INT old_color;
    INT old_segment = dr_data->current_line_seg;
    BOOL clip_flag = TRUE;
    static INT erase_color = -1;
    
    if ( erase_color < 0 )
        {
        /* ---------------------------------- */
        /* Remember the erase color for speed */
        /* ---------------------------------- */
        lu_text_to_index ("COLOR", &erase_color, "BACKGROUND");
        }
    
    /* ------------------------------------------------ */
    /* Allocate the internal segments control structure */
    /* (Used to delete no-longer-used segments)         */
    /* ------------------------------------------------ */
    if (dr_data->mod_obj_segments.nallocated == 0)
        {
        dr_data->mod_obj_segments.nallocated = 10;
        dr_data->mod_obj_segments.old_seg_array = (INT *
                                                   )tc_zalloc (
                                                             sizeof(
                                                                  INT) * 10);
        dr_data->mod_obj_segments.new_seg_array = (INT *
                                                   )tc_zalloc (
                                                             sizeof(
                                                                  INT) * 10);
        }
    else if (dr_data->mod_obj_segments.nallocated ==
             dr_data->mod_obj_segments.nused)
        {
        dr_data->mod_obj_segments.nallocated += 10;
        dr_data->mod_obj_segments.old_seg_array = (INT *
                                       )tc_realloc (dr_data->mod_obj_segments.
                                                    old_seg_array,
                                                   dr_data->mod_obj_segments.
                                                    nallocated * sizeof(INT));
        dr_data->mod_obj_segments.new_seg_array = (INT *
                                       )tc_realloc (dr_data->mod_obj_segments.
                                                    new_seg_array,
                                                   dr_data->mod_obj_segments.
                                                    nallocated * sizeof(INT));
        }    


    if (dr_data->current_line != (LINE *)NULL)
        {       
#if 0        
        /* --------------------------------- */
        /* Erase the old segment by drawing  */
        /* it in background color.  This is  */
        /* only useful when changing from a  */
        /* filled polygon to an unfilled     */
        /* polygon. Otherwise, we are just   */
        /* drawing on top of the same line   */                        
        /* --------------------------------- */
        
        if (dr_data->current_line_seg != 0)
            {
            old_color = dr_data->current_line->line_color;
            dr_data->current_line->line_color = erase_color;
            dr_data->current_line->polygon_fill_color = erase_color;

            mm_draw_line (dr_data->current_line, clip_flag);

            dr_data->current_line->line_color = old_color;
            dr_data->current_line->polygon_fill_color = old_color;
            }
#endif
        /* ------------------------------------------------------ */
        /* store the old segment to the array to be deleted later */
        /* ------------------------------------------------------ */

        dr_data->mod_obj_segments.old_seg_array[dr_data->mod_obj_segments.
                                                nused] =
            dr_data->current_line_seg;
            
        /* ------------------------------ */
        /* Create new segment and draw it */
        /* ------------------------------ */
        
        ov_new_element (&dr_data->current_line_seg,
                        (dr_data->current_line->polygon_flag ?
                         WM_CLASS_POLYGON : WM_CLASS_LINE),
                        dr_data->current_line->line_name,
                        dr_data->current_line->line_id);
        /* ---------------------------------- */
        /* Save new segment for possible undo */
        /* ---------------------------------- */
        dr_data->mod_obj_segments.new_seg_array[dr_data->mod_obj_segments.nused++] =
                dr_data->current_line_seg;

        mm_draw_line (dr_data->current_line, clip_flag);

        wmsclose (dr_data->current_line_seg);

        /* ------------------------------- */
        /* Post the new segment to display */
        /* ------------------------------- */
        /* wm_update (); */
        }        
    /* --------------------------------- */
    /* Make old segment invisible        */
    /* We do this afer the wm_update, to */
    /* avoid forcing a full refresh      */
    /* --------------------------------- */
    
    if ( old_segment != 0)
        {
        wmsvis ( old_segment, GINVIS );
        }

    return SUCCESS;
    }

