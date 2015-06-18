/* DEC/CMS REPLACEMENT HISTORY, Element SO_DRW_LINE.C*/
/* *11   17-APR-1991 09:49:56 MING "(SPR 1294) charlie fix on missing seismic"*/
/* *10    6-DEC-1990 11:17:03 JULIAN "(SPR 6288) add line id option on line label annotation"*/
/* *9    24-SEP-1990 17:21:47 GILLESPIE "Beta Deltas"*/
/* *8    26-JUL-1990 17:19:22 GILLESPIE "(SPR 1) Lint run; incorporate changes for horizon end point posting"*/
/* *7    25-JUL-1990 20:03:21 VINCE "(SPR 5644) Header Standardization"*/
/* *6    14-FEB-1990 13:18:39 GILLESPIE "(SPR 6005) Make nlist window clipping a method"*/
/* *5     2-NOV-1989 09:53:45 GILLESPIE "(SPR 30) Changes from moving mapping structure out of esi_so.h"*/
/* *4    14-SEP-1989 16:20:40 GORDON "(SPR 100) Gulf/Sun/Unix mods"*/
/* *3    28-JUL-1989 18:14:20 CHARLIE "(SPR 0) add line_label_annotation to 2d seismic overlays"*/
/* *2    25-JUL-1989 20:05:28 GILLESPIE "(SPR 50) Seismic system revamp"*/
/* *1    19-JUN-1989 13:26:20 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element SO_DRW_LINE.C*/
/*****************************************************************************/
/******************************************************************************

                Copyright Exploration Systems, Inc. 1989
                    Unpublished -- All rights reserved

THIS SOFTWARE IS THE PROPRIETARY PROPERTY OF Exploration Systems, Inc.  AND MAY
CONTAIN CONFIDENTIAL TRADE SECRET INFORMATION. IT IS LICENSED FOR USE ON THE
DESIGNATED EQUIPMENT ON WHICH IT WAS ORIGINALLY INSTALLED AND  MAY NOT BE
MODIFIED, DUPLICATED OR COPIED IN ANY FORM WITHOUT PRIOR WRITTEN CONSENT OF

                            Exploration Systems
                            c/o Finder Graphics Systems
                            201 Tamal Vista Blvd.
                            Corte Madera, CA  94925
                            (415) 927-0100

(The above notice does not constitute an admission of publication or
unrestricted dissemination of the work)

******************************************************************************/

#include "esi_c_lib.h"
#include "esi_so.h"
#include "esi_ts.h"
#include "esi_mp.h"
#include "esi_nl_meth_hdr.h"

#if USE_PROTOTYPES
static INT soz_generate_label (SO_STRUCT *pso, CHAR *text);
#else
static INT soz_generate_label ();
#endif

/* File Description ---------------------------------------------------------
Overview:
    Function to draw the seismic line itself and label it.

Prototypes and one line descriptions of each function in the file.
Public_Functions:
    publicdef INT so_draw_line_itself(SO_STRUCT *pso);

Private_Functions:
    static INT soz_generate_label(SO_STRUCT *pso, CHAR *text);

-----------------------------------------------------------------------------*/

/* Function Description -----------------------------------------------------
Description:

Prototype:
    publicdef INT so_draw_line_itself(SO_STRUCT *pso);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    pso             -(SO_STRUCT *) Seismic overlay structure.

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT so_draw_line_itself (SO_STRUCT *pso)
#else
publicdef INT so_draw_line_itself (pso)
SO_STRUCT *pso;
#endif
    {
    INT status = SUCCESS;
    UINT Col_List[2];               /* local nlist stuff */
    VOIDPTR Var_List[2];
    FLOAT x, y;
    NLM_CLIP_HEADER clip;
    VOIDPTR point[2];
    CHAR text[80];
    MAP_STRUCTURE *pmap = (MAP_STRUCTURE *)pso->map;
    
    pso->seismic_generic.seismic_line.polygon_flag = FALSE;
    pso->seismic_generic.seismic_line.line_nlist = pso->cdp_xy;
    
    
    /* -------------------------------------------------- */
    /* (CP) Draw the connecting line connecting only the  */
    /* actual shotpoint symbols which have been posted    */
    /* -------------------------------------------------- */
    if (pso->seismic_generic.draw_shotpoint_symbol AND
       (pso->sym_xy != (NLIST_HEADER) 0))
        pso->seismic_generic.seismic_line.line_nlist = pso->sym_xy;
        
    
    pso->seismic_generic.seismic_line.line_id = 0;
    *pso->seismic_generic.seismic_line.line_name = '\0';
    
    if (pso->seismic_generic.draw_seismic_line)
        {
        mm_draw_line_dim (&pso->seismic_generic.seismic_line, pso->clipped, 2, 3);
        }
    Var_List[0] = (VOIDPTR) & x, Var_List[1] = (VOIDPTR) & y;
    Col_List[0] = 2, Col_List[1] = 3;
    point[0] = (VOIDPTR) & x, point[1] = (VOIDPTR) & y;
    
    if (pso->seismic_generic.draw_seismic_label)
        {
        if (pso->disp_opt.first_line_label_paraperp != SO_NOPARA)
            {
            /* determine here what is to get posted (format) */
            soz_generate_label (pso, text);
            pso->seismic_generic.seismic_label1.text_string = text;
            
            nl_first_row (pso->cdp_xy, 2, Col_List, Var_List);
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
                status = nl_invoke_nlist_method (pso->cdp_xy, NL_CLIP_METHOD, (VOIDPTR)&clip);
                nl_first_row (clip.outNlist, 2, Col_List, point);
                nl_free_nlist (clip.outNlist);
                }
            pso->seismic_generic.seismic_label1.text_position.x = x;
            pso->seismic_generic.seismic_label1.text_position.y = y;
            mm_draw_text (&pso->seismic_generic.seismic_label1, FALSE);
            }
        if (pso->disp_opt.last_line_label_paraperp != SO_NOPARA)
            {
            /* determine format for text (if any) */
            soz_generate_label (pso, text);
            pso->seismic_generic.seismic_label2.text_string = text;
            
            nl_last_row (pso->cdp_xy, 2, Col_List, Var_List);
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
                status = nl_invoke_nlist_method (pso->cdp_xy, NL_CLIP_METHOD, (VOIDPTR)&clip);
                nl_last_row (clip.outNlist, 2, Col_List, point);
                nl_free_nlist (clip.outNlist);
                }
            pso->seismic_generic.seismic_label2.text_position.x = x;
            pso->seismic_generic.seismic_label2.text_position.y = y;
            mm_draw_text (&pso->seismic_generic.seismic_label2, FALSE);
            }
        }
    return status;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:

Prototype:
    static INT soz_generate_label(SO_STRUCT *pso, CHAR *text);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    pso             -(SO_STRUCT *)
    text            -(CHAR *)

Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PRIVATE to <so_drw_line.c>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static INT soz_generate_label (SO_STRUCT *pso, CHAR *text)
#else
static INT soz_generate_label (pso, text)
SO_STRUCT *pso;
CHAR *text;
#endif
    {
    switch (pso->seismic_generic.line_label_annotation)
        {
    case SO_LINE_ONLY:
        strcpy ((char *)text, (char *)pso->current_line.line_name);
        break;
    case SO_LINE_SURVEY:
        sprintf ((char *)text, "%s-%s", pso->current_line.line_name, pso->current_line.survey);
        break;
    case SO_SURVEY_LINE:
        sprintf ((char *)text, "%s-%s", pso->current_line.survey, pso->current_line.line_name);
        break;
    case SO_ID_ONLY:
       sprintf ((char *)text, "%d", pso->current_line.id);
        break;
    default:
        text[0] = (CHAR)0;
        }
    return SUCCESS;
    }
/* END:     */
