/* DEC/CMS REPLACEMENT HISTORY, Element SX_DLGCLPCLTM.C*/
/* *4    30-OCT-1990 11:54:45 GILLESPIE "(SPR 37) Fix Sun side of code"*/
/* *3    17-AUG-1990 22:20:37 VINCE "(SPR 5644) Code Cleanup"*/
/* *2    25-JUL-1989 20:11:01 GILLESPIE "(SPR 50) Seismic system revamp"*/
/* *1    19-JUN-1989 13:28:41 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element SX_DLGCLPCLTM.C*/
/* DEC/CMS REPLACEMENT HISTORY, Element SX_DLGCLPCLTM.C                             */
/* *3    17-AUG-1990 22:20:37 VINCE "(SPR 5644) Code Cleanup"                       */
/* *2    25-JUL-1989 20:11:01 GILLESPIE "(SPR 50) Seismic system revamp"            */
/* *1    19-JUN-1989 13:28:41 SYSTEM ""                                             */
/* DEC/CMS REPLACEMENT HISTORY, Element SX_DLGCLPCLTM.C                             */
/************************************************************************************/
/******************************************************************************     */
/*                                                                                  */
/*                Copyright Exploration Systems, Inc. 1989                          */
/*                    Unpublished -- All rights reserved                            */
/*                                                                                  */
/*THIS SOFTWARE IS THE PROPRIETARY PROPERTY OF Exploration Systems, Inc.  AND MAY   */
/*CONTAIN CONFIDENTIAL TRADE SECRET INFORMATION. IT IS LICENSED FOR USE ON THE      */
/*DESIGNATED EQUIPMENT ON WHICH IT WAS ORIGINALLY INSTALLED AND  MAY NOT BE         */
/*MODIFIED, DUPLICATED OR COPIED IN ANY FORM WITHOUT PRIOR WRITTEN CONSENT OF       */
/*                                                                                  */
/*                            Exploration Systems                                   */
/*                            c/o Finder Graphics Systems                           */
/*                            201 Tamal Vista Blvd.                                 */
/*                            Corte Madera, CA  94925                               */
/*                            (415) 927-0100                                        */
/*                                                                                  */
/*(The above notice does not constitute an admission of publication or              */
/*unrestricted dissemination of the work)                                           */
/*                                                                                  */
/******************************************************************************     */

#include "esi_sx.h"
#include "esi_math.h"

/* Function Description -----------------------------------------------------       */
/*Description:                                                                      */
/*    Set line label position termination program.                                  */
/*                                                                                  */
/*Prototype:                                                                        */
/*    publicdef INT sx_dialog_clp_clterm(SX_CLP_STRUCT *pclp);                      */
/*                                                                                  */
/*Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)        */
/*    pclp            -(SX_CLP_STRUCT *)                                            */
/*                                                                                  */
/*Return Value/Status:                                                              */
/*    SUCCESS - Successful completion.                                              */
/*    FAIL    - Unsuccessful completion.                                            */
/*                                                                                  */
/*Scope:                                                                            */
/*    PUBLIC                                                                        */
/*                                                                                  */
/*Limitations/Assumptions:                                                          */
/*                                                                                  */
/*-----------------------------------------------------------------------------     */

/* BEGIN:                                                                           */
#if USE_PROTOTYPES
publicdef INT sx_dialog_clp_clterm (SX_CLP_STRUCT *pclp)
#else
publicdef INT sx_dialog_clp_clterm (pclp)
SX_CLP_STRUCT *pclp;
#endif
    {
    INT status;
    FLOAT zang, ox, oy;
    INT which;
    
    status = SUCCESS;
    
    /* erase line labels                                                            */
    
    which = (pclp->first) ? SX_FIRST_LABEL : SX_LAST_LABEL;
    
    sx_do_line_labels (pclp->psx, SX_ERASE, which);
    
    zang = pclp->angle / 57.29582;
    
    ox = cos (zang) * pclp->psx->pso->seismic_generic.shotpoint_symbol.symbol_size;
    oy = sin (zang) * pclp->psx->pso->seismic_generic.shotpoint_symbol.symbol_size;
    
    if (pclp->first)
        {
        pclp->psx->pso->disp_opt.first_line_label_angle = pclp->angle;
        pclp->psx->pso->disp_opt.first_line_label_paraperp = pclp->paraperp;
        so_angpp_to_text (&pclp->psx->pso->seismic_generic.seismic_label1,
                          pclp->psx->pso->disp_opt.first_line_label_angle,
                          pclp->psx->pso->disp_opt.first_line_label_paraperp);
        pclp->psx->pso->seismic_generic.seismic_label1.text_offset.x = ox;
        pclp->psx->pso->seismic_generic.seismic_label1.text_offset.y = oy;
        }
    else
        {
        pclp->psx->pso->disp_opt.last_line_label_angle = pclp->angle;
        pclp->psx->pso->disp_opt.last_line_label_paraperp = pclp->paraperp;
        so_angpp_to_text (&pclp->psx->pso->seismic_generic.seismic_label2,
                          pclp->psx->pso->disp_opt.last_line_label_angle,
                          pclp->psx->pso->disp_opt.last_line_label_paraperp);
        pclp->psx->pso->seismic_generic.seismic_label2.text_offset.x = ox;
        pclp->psx->pso->seismic_generic.seismic_label2.text_offset.y = oy;
        }
    /* draw new line labels                                                         */
    
    pclp->psx->pso->seismic_generic.draw_seismic_label = TRUE;
    sx_do_line_labels (pclp->psx, SX_VISIBLE, which);
    pclp->psx->nchanges++;
    pclp->psx->need_to_save = TRUE;
    sx_menu_ls_setup (pclp->psx);
    
    SX_RETURN;
    }
/* END:                                                                             */
