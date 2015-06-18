/* DEC/CMS REPLACEMENT HISTORY, Element MM_DRAW_TEXT.C*/
/* *2    17-AUG-1990 22:00:03 VINCE "(SPR 5644) Code Cleanup"*/
/* *1    19-JUN-1989 13:08:31 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element MM_DRAW_TEXT.C*/
/******************************************************************************

                Copyright Finder Graphics Systems, Inc. 1990
                    Unpublished -- All rights reserved

THIS SOFTWARE IS THE PROPRIETARY PROPERTY OF Exploration Systems, Inc.  AND MAY
CONTAIN CONFIDENTIAL TRADE SECRET INFORMATION. IT IS LICENSED FOR USE ON THE
DESIGNATED EQUIPMENT ON WHICH IT WAS ORIGINALLY INSTALLED AND  MAY NOT BE
MODIFIED, DUPLICATED OR COPIED IN ANY FORM WITHOUT PRIOR WRITTEN CONSENT OF

                            Finder Graphics Systems
                            201 Tamal Vista Blvd.
                            Corte Madera, CA  94925
                            (415) 927-0100

(The above notice does not constitute an admission of publication or
unrestricted dissemination of the work)

******************************************************************************/

#include "esi_c_lib.h"
#include "esi_mm.h"
#include "esi_mp.h"
#include "esi_wm.h"
#include "esi_gks.h"
#include "esi_math.h"

/* Function Description -----------------------------------------------------
Description:
    Draw and optionally, index a text object.

Prototype:
    publicdef INT mm_draw_text(TEXT *pText,BOOL clip_flag);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) pText       -(TEXT *) A pointer to a text structure.
    (I) clip_flag   -(BOOL) TRUE -> clip, FALSE -> don't clip.

Return Value/Status:
    SUCCESS - Successful completion.
    MM_INVALID_TEXT
    MM_MAP_NOT_INIT
    MM_WINDOW_NOT_INIT

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT mm_draw_text (TEXT *pText, BOOL clip_flag)
#else
publicdef INT mm_draw_text (pText, clip_flag)
TEXT *pText;
BOOL clip_flag;
#endif
    {
    INT status;
    struct 
        {
        FLOAT x, y;
        } box_base;
    FLOAT cx, cy;
    CHAR tmp_text_str[256];
    CHAR *tmp_str_end, *tmp_str_begin;
    DOUBLE delta_x, delta_y;
    DOUBLE angle;
    MAP_STRUCTURE *map;
    
    /*  Starting Position for Text      */
    box_base.x = pText->text_position.x + pText->text_offset.x;
    box_base.y = pText->text_position.y + pText->text_offset.y;
    
    /*  Clip the text, if required:
        Following is a primitive clip, which checks only that
        the adjusted anchor point of the text is within the
        map window          */
    
    if (clip_flag)
        {
        mp_get_current_map (&map);
        if (box_base.x > map->upper_right_xy.x || box_base.y > map->upper_right_xy.y ||
            box_base.x < map->lower_left_xy.x || box_base.y < map->lower_left_xy.y)
            {
            return SUCCESS;
            }
        }
    /*  Set the text attributes         */
    
    /*  Text path                   */
    wm_gstxp (pText->text_path);
    
    /*  Text Up Vector              */
    wm_gschup (pText->text_up_x, pText->text_up_y);
    
    /*  Text height                 */
    wm_gschh (pText->text_size);
    
    /*  Character Expansion Factor          */
    wm_gschxp (pText->text_expansion_factor);
    
    /*  Text Font & Precision           */
    wm_gstxfp (pText->text_font, pText->text_precision);
    
    /*  Text Alignment              */
    wm_gstxal (pText->text_alignment[0], pText->text_alignment[1]);
    
    /*  Text Color                  */
    wm_gstxci (pText->text_color);
    
    /*  Calculate delta x and delta y for multiple lines */
    if (pText->text_up_x == 0.0)
        angle = PI / 2.0;
    else
        angle = atan (pText->text_up_y / pText->text_up_x);
    
    delta_x = cos (angle) * pText->text_line_spacing;
    delta_y = sin (angle) * pText->text_line_spacing;
    
    delta_x = (delta_x >= 0.0) ? delta_x : delta_x * (-1);
    delta_y = (delta_y >= 0.0) ? delta_y : delta_y * (-1);
    /*  Copy string to temporary location       */
    strcpy (tmp_text_str, pText->text_string);
    tmp_str_begin = tmp_text_str;
    tmp_str_end = tmp_text_str;
    
    /*  Loop on every line feed         */
    while (tmp_str_end != NULL)
        {
        /*  If the text is boxed, draw the box  */
        if (pText->box_flag)
            {
            
            /*  Query on the text box.          */
            wm_gqtxx (MAIN_WKID, box_base.x, box_base.y, pText->text_string, &status,
                      &cx, &cy, pText->box_boundary[0], pText->box_boundary[1]);
            
            /*  Fill Code               */
            wm_gsfais (pText->box_fill_code);
            
            /*  Fill Color              */
            wm_gsfaci (pText->box.color);
            
            /* Line style               */
            wm_gsln (pText->box.style);
            
            /* Line thickness               */
            wm_gslwsc (pText->box.width);
            
            /* Line color               */
            wm_gsplci (pText->box.color);
            
            /*  Draw the box                */
            wm_gfa (4, pText->box_boundary[0], pText->box_boundary[1]);
            
            }
        /*  find line feed and NULL it out.     */
        tmp_str_end = strchr (tmp_str_end, '\n');
        if (tmp_str_end != NULL)
            {
            *tmp_str_end = '\0';
            ++tmp_str_end;
            }
        /*  Post the text               */
        wm_gtx (box_base.x, box_base.y, tmp_str_begin);
        
        /*  Set new begining            */
        tmp_str_begin = tmp_str_end;
        
        /*  Set position for next text string   */
        if (pText->text_up_x >= 0.0 AND pText->text_up_y == 0.0)
            {
            box_base.x -= delta_x;
            }
        else if (pText->text_up_x < 0.0 AND pText->text_up_y == 0.0)
            {
            box_base.x += delta_x;
            }
        else if (pText->text_up_x == 0.0 AND pText->text_up_y >= 0.0)
            {
            box_base.y -= delta_y;
            }
        else if (pText->text_up_x == 0.0 AND pText->text_up_y < 0.0)
            {
            box_base.y += delta_y;
            }
        else if (pText->text_up_x > 0.0 AND pText->text_up_y > 0.0)
            {
            box_base.x -= delta_x;
            box_base.y -= delta_y;
            }
        else if (pText->text_up_x < 0.0 AND pText->text_up_y < 0.0)
            {
            box_base.x += delta_x;
            box_base.y += delta_y;
            }
        else if (pText->text_up_x > 0.0 AND pText->text_up_y < 0.0)
            {
            box_base.x -= delta_x;
            box_base.y += delta_y;
            }
        else if (pText->text_up_x < 0.0 AND pText->text_up_y > 0.0)
            {
            box_base.x += delta_x;
            box_base.y -= delta_y;
            }
        }
    return SUCCESS;
    }
/* END:     */
