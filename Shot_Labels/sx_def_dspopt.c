/* DEC/CMS REPLACEMENT HISTORY, Element SX_DEF_DSPOPT.C*/
/* *3    17-AUG-1990 22:20:17 VINCE "(SPR 5644) Code Cleanup"*/
/* *2    25-JUL-1989 20:10:31 GILLESPIE "(SPR 50) Seismic system revamp"*/
/* *1    19-JUN-1989 13:28:31 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element SX_DEF_DSPOPT.C*/
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
#include "esi_sx.h"
#include "esi_math.h"

/* Function Description -----------------------------------------------------
Description:
    Set up default disp opts for a new line.

Prototype:
    publicdef INT sx_default_disp_opt(SO_STRUCT *pso);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    pso             -(SO_STRUCT *)

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT sx_default_disp_opt (SO_STRUCT *pso)
#else
publicdef INT sx_default_disp_opt (pso)
SO_STRUCT *pso;
#endif
    {
    INT status;
    DOUBLE zang;
    
    /* assume success */
    
    status = SUCCESS;
    
    /* connect shot points */
    
    strcpy (pso->disp_opt.connect_shot_points, "Y");
    
    /* line labels */
    
    pso->disp_opt.first_line_label_angle = pso->disp_opt.last_line_label_angle = 0.0;
    pso->disp_opt.first_line_label_paraperp = pso->disp_opt.last_line_label_paraperp =
                                              SO_PERP;
    so_angpp_to_text (&pso->seismic_generic.seismic_label1, 0.0, 0);
    so_angpp_to_text (&pso->seismic_generic.seismic_label2, 0.0, 0);
    zang = 0;
    pso->seismic_generic.seismic_label1.text_offset.x = cos (zang) *
                                                        pso->seismic_generic.
                                                        shotpoint_symbol.symbol_size;
    pso->seismic_generic.seismic_label1.text_offset.y = sin (zang) *
                                                        pso->seismic_generic.
                                                        shotpoint_symbol.symbol_size;
    
    /* shot and zvalue labels */
    
    pso->disp_opt.shot_label_default = pso->seismic_generic.shotpoint_label;
    pso->disp_opt.zvalue_default = pso->disp_opt.shot_label_default;
    pso->disp_opt.shot_label_angle = 180.0;
    pso->disp_opt.shot_label_paraperp = SO_PERP;
    so_angpp_to_text (&pso->disp_opt.shot_label_default, 180.0, 0);
    so_angpp_to_text (&pso->disp_opt.zvalue_default, 0.0, 0);
    
    zang = PI;
    pso->disp_opt.shot_label_default.text_offset.x = cos (zang) *
                                                     pso->seismic_generic.
                                                     shotpoint_symbol.symbol_size *
        1.5;
    pso->disp_opt.shot_label_default.text_offset.y = sin (zang) *
                                                     pso->seismic_generic.
                                                     shotpoint_symbol.symbol_size *
        1.5;
    pso->disp_opt.zvalue_default.text_offset.x =
                                                 (-pso
                                                 ->disp_opt.shot_label_default.
                                                  text_offset.x);
    pso->disp_opt.zvalue_default.text_offset.y =
                                                 (-pso
                                                 ->disp_opt.shot_label_default.
                                                  text_offset.y);
    
    so_set_shotpoint_defaults (pso);
    strcpy (pso->disp_opt.shot_label_format, "%.0f");
    
    /* triggers */
    
    pso->disp_opt.line.shot_symbol_value = 0;
    strcpy (pso->disp_opt.line.shot_symbol_trigger, "E");
    pso->disp_opt.line.shot_label_value = 0;
    strcpy (pso->disp_opt.line.shot_label_trigger, "E");
    pso->disp_opt.line.zvalue_value = 0;
    strcpy (pso->disp_opt.line.zvalue_trigger, "E");
    
    /* symbols */
    
    pso->seismic_generic.shotpoint_symbol = pso->disp_opt.default_symbol;
    pso->seismic_generic.shotpoint_symbol.symbol_fill_code = FALSE;
    
    SX_RETURN;
    }
/* END:     */
