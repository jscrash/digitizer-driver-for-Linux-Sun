/* DEC/CMS REPLACEMENT HISTORY, Element SX_AUTOLNLAB.C*/
/* *3    17-AUG-1990 22:19:45 VINCE "(SPR 5644) Code Cleanup"*/
/* *2    25-JUL-1989 20:09:29 GILLESPIE "(SPR 50) Seismic system revamp"*/
/* *1    19-JUN-1989 13:28:05 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element SX_AUTOLNLAB.C*/
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

#include "esi_sx.h"
#include "esi_math.h"

/* Function Description -----------------------------------------------------
Description:
    Function to set default line label positions.

Prototype:
    publicdef INT sx_auto_line_labels(SX_STRUCT *psx);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    psx             -(SX_STRUCT *)

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT sx_auto_line_labels (SX_STRUCT *psx)
#else
publicdef INT sx_auto_line_labels (psx)
SX_STRUCT *psx;
#endif
    {
    
    INT status;
    DOUBLE zang, slope;
    FLOAT x1, y1, x2, y2;           /* 4 byte floats */
    UINT dim_list[2];
    VOIDPTR value_list[2];
    
    DOUBLE first_line_label_angle;
    INT first_line_label_paraperp = SO_PERP;
    DOUBLE last_line_label_angle;
    INT last_line_label_paraperp = SO_PERP;
    SO_STRUCT *pso = psx->pso;
    
    status = SUCCESS;
    
    /* erase line labels */
    
    sx_do_line_labels (psx, SX_ERASE, SX_BOTH_LABELS);
    
    /* line label position data */
    
    dim_list[0] = 2;
    dim_list[1] = 3;
    value_list[0] = (VOIDPTR) & x1;
    value_list[1] = (VOIDPTR) & y1;
    nl_first_point (pso->cdp_xy, 2, dim_list, value_list);
    value_list[0] = (VOIDPTR) & x2;
    value_list[1] = (VOIDPTR) & y2;
    nl_last_point (pso->cdp_xy, 2, dim_list, value_list);
    
    zang = atan2 (y1 - y2, x1 - x2);
    
    first_line_label_angle = zang * 57.29582;
    if (first_line_label_angle < 0.0)
        {
        first_line_label_angle += 360.0;
        }
    pso->disp_opt.first_line_label_angle = first_line_label_angle;
    pso->disp_opt.first_line_label_paraperp = first_line_label_paraperp;
    so_angpp_to_text (&pso->seismic_generic.seismic_label1,
                      pso->disp_opt.first_line_label_angle,
                      pso->disp_opt.first_line_label_paraperp);
    
    last_line_label_angle = first_line_label_angle + 180.0;
    if (last_line_label_angle > 360.0)
        {
        last_line_label_angle -= 360.0;
        }
    pso->disp_opt.last_line_label_angle = last_line_label_angle;
    pso->disp_opt.last_line_label_paraperp = last_line_label_paraperp;
    so_angpp_to_text (&pso->seismic_generic.seismic_label2,
                      pso->disp_opt.last_line_label_angle,
                      pso->disp_opt.last_line_label_paraperp);
    
    pso->seismic_generic.seismic_label1.text_offset.x = cos (zang) *
                                                        pso->seismic_generic.
                                                        shotpoint_symbol.symbol_size;
    pso->seismic_generic.seismic_label2.text_offset.x =
                                                        (-pso
                                                        ->seismic_generic.
                                                         seismic_label1.text_offset.x);
    
    pso->seismic_generic.seismic_label1.text_offset.y = sin (zang) *
                                                        pso->seismic_generic.
                                                        shotpoint_symbol.symbol_size;
    pso->seismic_generic.seismic_label2.text_offset.y =
                                                        (-pso
                                                        ->seismic_generic.
                                                         seismic_label1.text_offset.y);
    
    psx->pso->seismic_generic.draw_seismic_label = TRUE;
    sx_do_line_labels (psx, SX_VISIBLE, SX_BOTH_LABELS);
    
    SX_RETURN;
    }
/* END:     */
