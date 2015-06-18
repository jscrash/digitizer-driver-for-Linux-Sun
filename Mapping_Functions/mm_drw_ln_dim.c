/* DEC/CMS REPLACEMENT HISTORY, Element MM_DRW_LN_DIM.C*/
/* *5     4-JAN-1991 12:33:10 MING "(SPR 6272) initialize autopurge "*/
/* *4    17-AUG-1990 22:00:08 VINCE "(SPR 5644) Code Cleanup"*/
/* *3    14-FEB-1990 13:18:00 GILLESPIE "(SPR 6005) Make nlist window clipping a method"*/
/* *2    28-JUL-1989 10:35:16 GILLESPIE "(SPR 1) Fix minor casting problems"*/
/* *1    19-JUN-1989 13:08:33 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element MM_DRW_LN_DIM.C*/
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

#include "esi_mm.h"

#ifndef ESI_MP_H
#include "esi_mp.h"
#endif

#ifndef ESI_WM_H
#include "esi_wm.h"
#endif

#ifndef ESI_NL_H
#include "esi_nl.h"
#endif

#include "esi_nl_meth_hdr.h"

/* Function Description -----------------------------------------------------
Description:
    Draw and optionally, index a line object, specify dimensions
    of nlist. 

Prototype:
    publicdef INT mm_draw_line_dim(LINE *pLine,BOOL clip_flag,INT xdim,INT ydim);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) pLine       -(LINE *) A pointer to a line structure.
    (I) clip_flag   -(BOOL) TRUE -> clip, FALSE -> don't clip.
    xdim            -(INT)
    ydim            -(INT)
    
Return Value/Status:
    SUCCESS - Successful completion.
    MM_INVALID_LINE
    MM_MAP_NOT_INIT
    MM_WINDOW_NOT_INIT
    MM_NLIST_NOT_FOUND

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT mm_draw_line_dim (LINE *pLine, BOOL clip_flag, INT xdim, INT ydim)
#else
publicdef INT mm_draw_line_dim (pLine, clip_flag, xdim, ydim)
LINE *pLine;
BOOL clip_flag;
INT xdim, ydim;
#endif
    {
    INT status;
    MAP_STRUCTURE *map;
    NLM_CLIP_HEADER clip;
    
    /* If line is a polygon, draw as a polygon instead */
    /*  this  will later be changed to include all the code */
    /*  for drawing a polygon inside this routine. */
    if (pLine->polygon_flag == TRUE)
        {
        return mm_draw_polygon (pLine, clip_flag);
        }
    /* Set the graphic attributes           */
    
    /* Line style                   */
    
    wm_gsln (pLine->line.style);
    
    /* Line thickness               */
    
    wm_gslwsc (pLine->line.width);
    
    /* Line color                   */
    
    wm_gsplci (pLine->line.color);
    
    /* Clip the line, if required           */
    
    if (clip_flag)
        {
        /* Get the map limits               */
        
        mp_get_current_map (&map);
        clip.Col_List[0] = xdim;
        clip.Col_List[1] = ydim;
        clip.minList[0] = map->lower_left_xy.x;
        clip.minList[1] = map->lower_left_xy.y;
        clip.maxList[0] = map->upper_right_xy.x;
        clip.maxList[1] = map->upper_right_xy.y;
	clip.autopurge  = FALSE;
        
        /*  Clip                    */
        
        status = nl_invoke_nlist_method (pLine->line_nlist, NL_CLIP_METHOD, &clip);
        
        /*  Draw                    */
        
        if (status EQUALS SUCCESS)
            {
            nl_draw_nlist (clip.outNlist, xdim, ydim);
            nl_free_nlist (clip.outNlist);
            }
        }
    else
        {
        
        /*  Draw un-clipped             */
        
        nl_draw_nlist (pLine->line_nlist, xdim, ydim);
        }
    return SUCCESS;
    }
/* END:     */
