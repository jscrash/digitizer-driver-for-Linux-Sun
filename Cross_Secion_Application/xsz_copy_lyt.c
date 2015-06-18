/* DEC/CMS REPLACEMENT HISTORY, Element XSZ_COPY_LYT.C*/
/* *2    17-AUG-1990 22:33:37 VINCE "(SPR 5644) Code Cleanup"*/
/* *1    19-JUN-1989 13:42:02 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element XSZ_COPY_LYT.C*/
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

#ifndef ESI_C_LIB_H

#include "esi_c_lib.h"

#endif

#ifndef ESI_HO_H

#include "esi_ho.h"

#endif

#ifndef ESI_XS_DEFS_H

#include "esi_xs_defs.h"

#endif

/* Function Description -----------------------------------------------------
Description:
    Copy the template layout (plo) to the layout          
    structure for an individual well leaving GKS         
    segment and transformation ids untouched.             
    Listheads etc. must also remain intact.               

Prototype:
    publicdef INT xsz_copy_layout(LAYOUT_STRUCT *plo, LAYOUT_STRUCT *pwlo);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    plo             -(LAYOUT_STRUCT *) Pointer to template layout.
    pwlo            -(LAYOUT_STRUCT *) Pointer to well layout.

Return Value/Status:
    SUCCESS - Successful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT xsz_copy_layout (LAYOUT_STRUCT *plo, LAYOUT_STRUCT *pwlo)
#else
publicdef INT xsz_copy_layout (plo, pwlo)
LAYOUT_STRUCT *plo;
LAYOUT_STRUCT *pwlo;
#endif
    {
    DOUBLE t_ltdm_data_top;
    DOUBLE t_ltdm_data_base;
    DOUBLE t_true_data_top;
    DOUBLE t_true_data_base;
    DOUBLE t_track_top;
    DOUBLE t_track_base;
    DOUBLE t_true_rel_top;
    DOUBLE t_true_rel_base;
    
    PROGNAME ("xsz_copy_layout");
    
    t_ltdm_data_top = pwlo->interval.ltdm_data_top;
    t_ltdm_data_base = pwlo->interval.ltdm_data_base;
    t_true_data_top = pwlo->interval.true_data_top;
    t_true_data_base = pwlo->interval.true_data_base;
    t_track_top = pwlo->interval.track_top;
    t_track_base = pwlo->interval.track_base;
    t_true_rel_top = pwlo->interval.true_rel_top;
    t_true_rel_base = pwlo->interval.true_rel_base;
    hoblockmove (&plo->interval, &pwlo->interval, sizeof(LOG_INTERVAL));
    pwlo->interval.ltdm_data_top = t_ltdm_data_top;
    pwlo->interval.ltdm_data_base = t_ltdm_data_base;
    pwlo->interval.true_data_top = t_true_data_top;
    pwlo->interval.true_data_base = t_true_data_base;
    pwlo->interval.track_top = t_track_top;
    pwlo->interval.track_base = t_track_base;
    pwlo->interval.true_rel_top = t_true_rel_top;
    pwlo->interval.true_rel_base = t_true_rel_base;
    
    pwlo->data_thinning = plo->data_thinning;
    pwlo->decimate = plo->decimate;
    pwlo->delta_slope = plo->delta_slope;
    
    pwlo->log_hdr.angle = plo->log_hdr.angle;
    pwlo->log_hdr.visible_border = plo->log_hdr.visible_border;
    pwlo->log_hdr.visible = plo->log_hdr.visible;
    pwlo->log_hdr.line = plo->log_hdr.line;
    pwlo->log_hdr.well_symb.color = plo->log_hdr.well_symb.color;
    pwlo->log_hdr.well_symb.visible = plo->log_hdr.well_symb.visible;
    pwlo->log_hdr.total_items = plo->log_hdr.total_items;
    
    pwlo->scale_block.line = plo->scale_block.line;
    pwlo->scale_block.visible = plo->scale_block.visible;
    
    pwlo->text_block.line = plo->text_block.line;
    pwlo->text_block.visible = plo->text_block.visible;
    
    return SUCCESS;
    }
/* END:     */
