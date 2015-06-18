/* DEC/CMS REPLACEMENT HISTORY, Element SX_DOLINELABS.C*/
/* *5    17-AUG-1990 22:21:31 VINCE "(SPR 5644) Code Cleanup"*/
/* *4    14-FEB-1990 13:19:09 GILLESPIE "(SPR 6005) Make nlist window clipping a method"*/
/* *3     2-NOV-1989 09:54:45 GILLESPIE "(SPR 30) Changes from moving mapping structure out of esi_so.h"*/
/* *2    25-JUL-1989 20:12:10 GILLESPIE "(SPR 50) Seismic system revamp"*/
/* *1    19-JUN-1989 13:29:18 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element SX_DOLINELABS.C*/
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
#include "esi_sx.h"

#ifndef ESI_LU_H
#include "esi_lu.h"
#endif

#ifndef ESI_TS_H
#include "esi_ts.h"
#endif

#ifndef ESI_MP_H
#include "esi_mp.h"
#endif

#include "esi_nl_meth_hdr.h"

/* Function Description -----------------------------------------------------
Description:
    Function to draw / erase line label.

Prototype:
    publicdef INT sx_do_line_labels(SX_STRUCT *psx,INT mode,INT which);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    psx             -(SX_STRUCT *) Seismic line editor struct.
    mode            -(INT) Erase or normal mode or blink.
    which           -(INT) First, last, or both.

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT sx_do_line_labels (SX_STRUCT *psx, INT mode, INT which)
#else
publicdef INT sx_do_line_labels (psx, mode, which)
SX_STRUCT *psx;
INT mode;
INT which;
#endif
    {
    INT status;
    COLOR_INDEX color_save;
    UINT dim_list[3];               /* local nlist stuff */
    VOIDPTR value_list[3];
    UINT npoints;
    CDP cdp;
    FLOAT x, y;
    SO_STRUCT *pso = psx->pso;      /* seismic overlay structure */
    MAP_STRUCTURE *pmap = (MAP_STRUCTURE *)pso->map;
    BOOL do_first, do_last;
    NLM_CLIP_HEADER clip;
    VOIDPTR point[2];
    
    CHAR text[80];
    INT id1, len;
    
    status = SUCCESS;
    
    /* any points ? */
    
    if (psx->nshots == 0 OR (!pso->seismic_generic.draw_seismic_label))
        {
        SX_RETURN;
        }
    point[0] = (VOIDPTR) & x;
    point[1] = (VOIDPTR) & y;
    
    /* do which ones */
    
    do_first = ((which == SX_FIRST_LABEL) OR (which ==
                                             SX_BOTH_LABELS)) AND (pso->disp_opt.
                                                             first_line_label_paraperp 
                                                                  != SO_NOPARA);
    do_last = ((which == SX_LAST_LABEL) OR (which ==
                          SX_BOTH_LABELS)) AND (pso->disp_opt.
                                               last_line_label_paraperp != SO_NOPARA);
    
    /* remember color if mode is erase */
    
    if (mode == SX_ERASE)
        {
        color_save = pso->seismic_generic.seismic_label1.text_color;
        pso->seismic_generic.seismic_label1.text_color =
            psx->graphics.background_color;
        pso->seismic_generic.seismic_label2.text_color =
            psx->graphics.background_color;
        }
    else if (mode == SX_BLINK)
        {
        color_save = pso->seismic_generic.seismic_label1.text_color;
        lu_text_to_index("COLOR",
	    (INDEX *)&pso->seismic_generic.seismic_label1.text_color, "HIGHLIGHT");
        pso->seismic_generic.seismic_label2.text_color =
	    pso->seismic_generic.seismic_label1.text_color;
        }
    value_list[0] = (VOIDPTR) & cdp;
    value_list[1] = (VOIDPTR) & x;
    value_list[2] = (VOIDPTR) & y;
    
    dim_list[0] = 1;
    dim_list[1] = 2;
    dim_list[2] = 3;
    
    nl_first_point (pso->cdp_xy, 3, dim_list, value_list);
    if (do_first)
        {
        sprintf (text, "%s-%s", pso->current_line.line_name, pso->current_line.survey);
        pso->seismic_generic.seismic_label1.text_string = text;
        
        if (x < pmap->lower_left_xy.x OR x > pmap->upper_right_xy.x OR y <
            pmap->lower_left_xy.y OR y > pmap->upper_right_xy.y)
            {
            /* clip */
            clip.Col_List[0] = 2;
            clip.Col_List[1] = 3;
            clip.minList[0] = pmap->lower_left_xy.x;
            clip.minList[1] = pmap->lower_left_xy.y;
            clip.maxList[0] = pmap->upper_right_xy.x;
            clip.maxList[1] = pmap->upper_right_xy.y;
            status = nl_invoke_nlist_method (pso->cdp_xy, NL_CLIP_METHOD, &clip);
            nl_first_point (clip.outNlist, 2, dim_list, point);
            nl_free_nlist (clip.outNlist);
            }
        pso->seismic_generic.seismic_label1.text_position.x = x;
        pso->seismic_generic.seismic_label1.text_position.y = y;
        mm_draw_text (&pso->seismic_generic.seismic_label1, FALSE);
        }
    nl_last_point (pso->cdp_xy, 3, dim_list, value_list);
    if (do_last)
        {
        sprintf (text, "%s-%s", pso->current_line.line_name, pso->current_line.survey);
        
        pso->seismic_generic.seismic_label2.text_string = text;
        
        if (x < pmap->lower_left_xy.x OR x > pmap->upper_right_xy.x OR y <
            pmap->lower_left_xy.y OR y > pmap->upper_right_xy.y)
            {
            /* clip */
            clip.Col_List[0] = 2;
            clip.Col_List[1] = 3;
            clip.minList[0] = pmap->lower_left_xy.x;
            clip.minList[1] = pmap->lower_left_xy.y;
            clip.maxList[0] = pmap->upper_right_xy.x;
            clip.maxList[1] = pmap->upper_right_xy.y;
            status = nl_invoke_nlist_method (pso->cdp_xy, NL_CLIP_METHOD, &clip);
            nl_last_point (clip.outNlist, 2, dim_list, point);
            nl_free_nlist (clip.outNlist);
            }
        pso->seismic_generic.seismic_label2.text_position.x = x;
        pso->seismic_generic.seismic_label2.text_position.y = y;
        mm_draw_text (&pso->seismic_generic.seismic_label2, FALSE);
        }
    /* reset color */
    
    if (mode == SX_ERASE OR mode == SX_BLINK)
        {
        pso->seismic_generic.seismic_label1.text_color = color_save;
        pso->seismic_generic.seismic_label2.text_color = color_save;
        }
    SX_RETURN;
    }
/* END:     */
