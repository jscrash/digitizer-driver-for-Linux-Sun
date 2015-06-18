/* DEC/CMS REPLACEMENT HISTORY, Element S3_DRW_SURVEY.C*/
/* *5    17-AUG-1990 22:13:35 VINCE "(SPR 5644) Code Cleanup"*/
/* *4    18-JUN-1990 12:06:13 GILLESPIE "(SPR 1) Changes to allow 3-D lines to be handles as select list entryes (IES deltas)"*/
/* *3    10-FEB-1990 11:14:40 GILLESPIE "(SPR 1) Change call from nlz_clip_vis to ag_clip_2"*/
/* *2    28-JUL-1989 12:17:06 GILLESPIE "(SPR 1) Fix minor casting problems"*/
/* *1    19-JUN-1989 13:23:03 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element S3_DRW_SURVEY.C*/
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

#include "esi_s3.h"
#include "esi_math.h"

#ifndef ESI_NL_H
#include "esi_nl.h"                 /* cdp and line endpoints nlist */
#endif

#ifndef ESI_WM_H
#include "esi_wm.h"                 /* gks calls */
#endif

#ifndef ESI_AG_H
#include "esi_ag.h"                 /* clip routine*/
#endif

#ifndef ESI_TC_H
#include "esi_tc.h"                 /* allocates */
#endif

#ifndef ESI_SO_H
#include "esi_so.h"                 /* borrowed seismic overlay code */
#endif

#ifndef ESI_MP_H
#include "esi_mp.h"                 /* map structure */
#endif

#ifndef ESI_MM_H
#include "esi_mm.h"                 /* text labels */
#endif

#if USE_PROTOTYPES
static INT s3z_plot_line (FLOAT *x, FLOAT *y, DOUBLE minxy[], DOUBLE maxxy[]);
#else
static INT s3z_plot_line ();
#endif

/* File Description ---------------------------------------------------------
Overview:
    Determines if lines, symbols and labels are                        
    to be drawn, then draws them                                                     

Prototypes and one line descriptions of each function in the file.
Public_Functions:
    publicdef INT s3_draw_survey(S3_STRUCT *ps3);

Private_Functions:
    static INT s3z_plot_line(FLOAT *x, FLOAT *y, DOUBLE minxy[], DOUBLE maxxy[]);

-----------------------------------------------------------------------------*/

/* Function Description -----------------------------------------------------
Description:

Prototype:
    publicdef INT s3_draw_survey(S3_STRUCT *ps3);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    ps3             -(S3_STRUCT *) Seismic overlay structure.

Return Value / Status :
    SUCCESS-Successful completion
    FAIL-Unsuccessful completion

Scope : PUBLIC Limitations /

Assumptions :
-----------------------------------------------------------------------------*/ 

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT s3_draw_survey (S3_STRUCT *ps3)
#else
publicdef INT s3_draw_survey (ps3)
S3_STRUCT *ps3;
#endif
    {
    INT status;
    UINT line, cdp;                 /* current value                                */
    NLIST_HEADER clipped_line;
    FLOAT x[2], y[2];               /* for gks and se routines                      */
    DOUBLE point1[2], point2[2], minxy[2], maxxy[2];
    DOUBLE zang, zang2, opposite_ang;   /* for label posting                        */
    UINT nlines, ncdps;             /* total count (for loops)                      */
    INT i;
    CHAR text[80];
    MAP_STRUCTURE *map;
    
    UINT ndim = 5;
    static UINT Col_List[5] = 
        {
        1,
        2,
        3,
        4,
        5
        };
    static INT units[5] = 
        {
        0,
        0,
        0,
        0,
        0
        };
    static INT value_incs[5] = 
        {
        INCREMENT_LIST,
        VALUE_LIST,
        VALUE_LIST,
        VALUE_LIST,
        VALUE_LIST
        };
    static size_t data_sizes[5] = 
        {
        sizeof(INT), sizeof(FLOAT), sizeof(FLOAT), sizeof(FLOAT), sizeof(FLOAT)
        };
    static INT data_types[5] = 
        {
        INT_TYPE,
        FLOAT_TYPE,
        FLOAT_TYPE,
        FLOAT_TYPE,
        FLOAT_TYPE
        };
    VOIDPTR Var_List[5];
    
    status = SUCCESS;
    
    /* retrieve the current map information.                                        */
    mp_get_current_map (&map);
    maxxy[0] = map->upper_right_xy.x;
    maxxy[1] = map->upper_right_xy.y;
    minxy[0] = map->lower_left_xy.x;
    minxy[1] = map->lower_left_xy.y;
    
    /* set up nlist for lines                                                       */
    Var_List[0] = (VOIDPTR) & line;
    Var_List[1] = (VOIDPTR) & x[0];
    Var_List[2] = (VOIDPTR) & y[0];
    Var_List[3] = (VOIDPTR) & x[1];
    Var_List[4] = (VOIDPTR) & y[1];
    
    /* examine nlist for line, x, and y coordinates                                 */
    nl_inq_nlist_int (ps3->line_nlist, NL_NLIST_NROWS, (INT *) & nlines);
    nl_first_row (ps3->line_nlist, 5, Col_List, Var_List);
    
    ps3->seismic_generic.seismic_xlabel1.text_string = (CHAR *)
                                                       tc_alloc (40 * sizeof(CHAR));
    ps3->seismic_generic.seismic_xlabel2.text_string = (CHAR *)
                                                       tc_alloc (40 * sizeof(CHAR));
    
    /* set up for line_style,line_thickness,color                                   */
    wm_gsln (ps3->seismic_generic.seismic_xline.line.style);
    wm_gslwsc (ps3->seismic_generic.seismic_xline.line.width);
    wm_gsplci (ps3->seismic_generic.seismic_xline.line.color);
    
    /* setup calculations for line labels (at endpoints)                            */
    if (abs (x[0] - x[1]) < 0.001)
        zang = 90.0;
    else
        zang = atan2 (y[0] - y[1], x[0] - x[1]) * (180.0 / PI);
    if (zang < 0.0)
        zang += 360.0;
    
    so_angpp_to_text (&ps3->seismic_generic.seismic_xlabel1, zang, SO_PERP);
    
    opposite_ang = zang + 180.0;
    if (opposite_ang > 360.0)
        opposite_ang -= 360.0;
    zang2 = opposite_ang / (180.0 / PI);
    
    so_angpp_to_text (&ps3->seismic_generic.seismic_xlabel2, opposite_ang, SO_PERP);
    
    zang = zang / (180.0 / PI);
    
    ps3->seismic_generic.seismic_xlabel1.text_offset.x = cos (zang) *
                                                         ps3->seismic_generic.
                                                         shotpoint_xsymbol.symbol_size;
    ps3->seismic_generic.seismic_xlabel2.text_offset.x = cos (zang2) *
                                                         ps3->seismic_generic.
                                                         shotpoint_xsymbol.symbol_size;
    
    ps3->seismic_generic.seismic_xlabel1.text_offset.y = sin (zang) *
                                                         ps3->seismic_generic.
                                                         shotpoint_xsymbol.symbol_size;
    ps3->seismic_generic.seismic_xlabel2.text_offset.y = sin (zang2) *
                                                         ps3->seismic_generic.
                                                         shotpoint_xsymbol.symbol_size;
    
    /*    ps3->seismic_generic.shotpoint_xsymbol.symbol_type =
        ps3->disp_opt.default_symbol.symbol_type;*/
    ps3->seismic_generic.shotpoint_xsymbol.symbol_fill_code = FALSE;
    
    ps3->disp_opt.x_trigger = -1;   /* init.                                        */
    ps3->disp_opt.s_trigger = -1;   /* init.                                        */
    
    for (i = 0; i < nlines; i++)
        {
        
        /* see if this line should be plotted                                       */
        ps3->disp_opt.x_trigger++;
        
        /* if this is the last one, plot it also                                    */
        if (i == nlines - 1)
            ps3->disp_opt.x_trigger = 0;
        if (ps3->disp_opt.x_line_plot_incr > 0)
            {
            if (ps3->disp_opt.x_trigger % ps3->disp_opt.x_line_plot_incr == 0)
                {
                /* this one is for plotting                                         */
                /* draw the line                                                    */
                if (ps3->seismic_generic.draw_seismic_line)
                    status = s3z_plot_line (x, y, minxy, maxxy);
                /* see if the endpoints are to be labeled                           */
                if (ps3->seismic_generic.draw_seismic_label)
                    {
                    sprintf (text, "%d", line);
                    strcpy (ps3->seismic_generic.seismic_xlabel1.text_string, text);
                    strcpy (ps3->seismic_generic.seismic_xlabel2.text_string, text);
                    
                    ps3->seismic_generic.seismic_xlabel1.text_position.x = x[0];
                    ps3->seismic_generic.seismic_xlabel1.text_position.y = y[0];
                    mm_draw_text (&ps3->seismic_generic.seismic_xlabel1, TRUE);
                    
                    ps3->seismic_generic.seismic_xlabel2.text_position.x = x[1];
                    ps3->seismic_generic.seismic_xlabel2.text_position.y = y[1];
                    mm_draw_text (&ps3->seismic_generic.seismic_xlabel2, TRUE);
                    
                    }
                /* now see if the symbols are to be plotted                         */
                ps3->disp_opt.s_trigger++;
                /* if this is the last one, plot it also                            */
                if (i == nlines - 1)
                    ps3->disp_opt.s_trigger = 0;
                if (ps3->disp_opt.x_symb_line_plot_incr > 0)
                    {
                    if (ps3->disp_opt.s_trigger %
                        ps3->disp_opt.x_symb_line_plot_incr == 0)
                        {
                        /* this line is to have symbols plotted                     */
                        if (ps3->seismic_generic.draw_shotpoint_symbol)
                            s3_draw_symbols (ps3, line, S3_LINE);
                        }
                    }
                }
            }
        /* now go to the next line                                                  */
        nl_next_row (ps3->line_nlist, 5, Col_List, Var_List);
        }
    tc_free (ps3->seismic_generic.seismic_xlabel1.text_string);
    tc_free (ps3->seismic_generic.seismic_xlabel2.text_string);
    
    /* change line nlist to cdp nlist                                               */
    Var_List[0] = (VOIDPTR) & cdp;
    
    nl_inq_nlist_int (ps3->cdp_nlist, NL_NLIST_NROWS, (INT *) & ncdps);
    nl_first_row (ps3->cdp_nlist, 5, Col_List, Var_List);
    
    ps3->seismic_generic.seismic_ylabel1.text_string = (CHAR *)
                                                       tc_alloc (40 * sizeof(CHAR));
    ps3->seismic_generic.seismic_ylabel2.text_string = (CHAR *)
                                                       tc_alloc (40 * sizeof(CHAR));
    
    wm_gsln (ps3->seismic_generic.seismic_yline.line.style);
    wm_gslwsc (ps3->seismic_generic.seismic_yline.line.width);
    wm_gsplci (ps3->seismic_generic.seismic_yline.line.color);
    
    /* for labels at endpoints                                                      */
    if (abs (x[0] - x[1]) < 0.001)
        zang = 90.0;
    else
        zang = atan2 (y[0] - y[1], x[0] - x[1]) * (180 / PI);
    if (zang < 0.0)
        zang += 360.0;
    
    so_angpp_to_text (&ps3->seismic_generic.seismic_ylabel2, zang, SO_PERP);
    
    opposite_ang = zang + 180.0;
    if (opposite_ang > 360.0)
        opposite_ang -= 360.0;
    zang2 = opposite_ang / (180.0 / PI);
    
    so_angpp_to_text (&ps3->seismic_generic.seismic_ylabel1, opposite_ang, SO_PERP);
    
    zang = zang / (180.0 / PI);
    
    ps3->seismic_generic.seismic_ylabel2.text_offset.x = cos (zang) *
                                                         ps3->seismic_generic.
                                                         shotpoint_ysymbol.symbol_size;
    ps3->seismic_generic.seismic_ylabel1.text_offset.x = cos (zang2) *
                                                         ps3->seismic_generic.
                                                         shotpoint_ysymbol.symbol_size;
    
    ps3->seismic_generic.seismic_ylabel2.text_offset.y = sin (zang) *
                                                         ps3->seismic_generic.
                                                         shotpoint_ysymbol.symbol_size;
    ps3->seismic_generic.seismic_ylabel1.text_offset.y = sin (zang2) *
                                                         ps3->seismic_generic.
                                                         shotpoint_ysymbol.symbol_size;
    
    /*    ps3->seismic_generic.shotpoint_ysymbol.symbol_type =
        ps3->disp_opt.default_symbol.symbol_type;*/
    ps3->seismic_generic.shotpoint_ysymbol.symbol_fill_code = FALSE;
    
    ps3->disp_opt.y_trigger = -1;   /* init.                                        */
    ps3->disp_opt.s_trigger = -1;   /* init.                                        */
    
    /* for each cdp in grid                                                         */
    for (i = 0; i < ncdps; i++)
        {
        
        /* see if this cdp should be plotted                                        */
        ps3->disp_opt.y_trigger++;
        
        /* if last one, force it to be plotted                                      */
        if (i == ncdps - 1)
            ps3->disp_opt.y_trigger = 0;
        if (ps3->disp_opt.y_line_plot_incr > 0)
            {
            if (ps3->disp_opt.y_trigger % ps3->disp_opt.y_line_plot_incr == 0)
                {
                
                /* plot the line along this cdp                                     */
                if (ps3->seismic_generic.draw_seismic_line)
                    status = s3z_plot_line (x, y, minxy, maxxy);
                if (ps3->seismic_generic.draw_seismic_label)
                    {
                    sprintf (text, "%d", cdp);
                    strcpy (ps3->seismic_generic.seismic_ylabel1.text_string, text);
                    strcpy (ps3->seismic_generic.seismic_ylabel2.text_string, text);
                    
                    ps3->seismic_generic.seismic_ylabel2.text_position.x = x[1];
                    ps3->seismic_generic.seismic_ylabel2.text_position.y = y[1];
                    mm_draw_text (&ps3->seismic_generic.seismic_ylabel2, TRUE);
                    
                    ps3->seismic_generic.seismic_ylabel1.text_position.x = x[0];
                    ps3->seismic_generic.seismic_ylabel1.text_position.y = y[0];
                    mm_draw_text (&ps3->seismic_generic.seismic_ylabel1, TRUE);
                    
                    }
                /* now determine which cdp symbols should be plotted                */
                ps3->disp_opt.s_trigger++;
                /* if this is the last one, plot it also                            */
                if (i == ncdps - 1)
                    ps3->disp_opt.s_trigger = 0;
                if (ps3->disp_opt.y_symb_line_plot_incr > 0)
                    {
                    if (ps3->disp_opt.s_trigger %
                        ps3->disp_opt.y_symb_line_plot_incr == 0)
                        {
                        /* this cdp is to have symbols plotted                      */
                        if (ps3->seismic_generic.draw_shotpoint_symbol)
                            s3_draw_symbols (ps3, cdp, S3_CDP);
                        }
                    }
                }
            }
        /* now on to next cdp                                                       */
        nl_next_row (ps3->cdp_nlist, 5, Col_List, Var_List);
        }
    tc_free (ps3->seismic_generic.seismic_ylabel1.text_string);
    tc_free (ps3->seismic_generic.seismic_ylabel2.text_string);
    
    /* the following has been placed on hold. the endpoints only are labeled        */
    /* now do CDP values                                                            */
    /*!!!!!s3_draw_cdp_values!!!!!                                                  */
    /*!!!!!end of s3_draw_cdp_values!!!!!                                           */
    
    return status;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:

Prototype:
    static INT s3z_plot_line(FLOAT *x, FLOAT *y, DOUBLE minxy[2], DOUBLE maxxy[2]);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    x               -(FLOAT *)
    y               -(FLOAT *)
    minxy           -(DOUBLE [])
    maxxy           -(DOUBLE [])

Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PRIVATE to <s3.drw.survey.c>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static INT s3z_plot_line (FLOAT *x, FLOAT *y, DOUBLE minxy[2], DOUBLE maxxy[2])
#else
static INT s3z_plot_line (x, y, minxy, maxxy)
FLOAT *x, *y;
DOUBLE minxy[2], maxxy[2];
#endif
    {
    DOUBLE point1[2], point2[2], aconst[2];
    INT visible;
    
    point1[0] = x[0];
    point1[1] = y[0];
    point2[0] = x[1];
    point2[1] = y[1];
    ag_clip_2 (&point1[0], &point1[1], &point2[0], &point2[1], minxy[0], minxy[1],
               maxxy[0], maxxy[1], &aconst[0], &aconst[1], &visible);
    if (visible)
        {
        x[0] = point1[0];
        x[1] = point2[0];
        y[0] = point1[1];
        y[1] = point2[1];
        wm_gpl (2, x, y);
        }
    return SUCCESS;
    }
/* END:     */
