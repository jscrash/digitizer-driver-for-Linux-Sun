/* DEC/CMS REPLACEMENT HISTORY, Element SO_DRW_SH_LBS.C */
/* *4    24-SEP-1990 17:22:33 GILLESPIE "Beta Deltas" */
/* *3    26-JUL-1990 17:19:31 GILLESPIE "(SPR 1) Lint run; incorporate changes for horizon end point posting" */
/* *2    25-JUL-1990 20:03:27 VINCE "(SPR 5644) Header Standardization" */
/* *1     5-MAR-1990 17:02:12 VINCE "(spr 6015) shorten name for UNIX compatiblilty." */
/* DEC/CMS REPLACEMENT HISTORY, Element SO_DRW_SH_LBS.C */
/* DEC/CMS REPLACEMENT HISTORY, Element SO_DRW_SH_LBS.C*/
/* *2    31-AUG-1990 18:24:45 SCD "(SPR 5750) Add hooks to compute precision posting angles"*/
/* *1    13-AUG-1990 17:11:40 SYSTEM "Finder 6.1"*/
/* DEC/CMS REPLACEMENT HISTORY, Element SO_DRW_SH_LBS.C*/
/* DEC/CMS REPLACEMENT HISTORY, Element SO_DRW_SH_LBLS.C*/
/* *2    10-OCT-1989 10:00:45 JULIAN "(SPR 5020) mods for CT parameter change"*/
/* *1    26-JUL-1989 00:15:34 GILLESPIE "New seismic system mods"*/
/* DEC/CMS REPLACEMENT HISTORY, Element SO_DRW_SH_LBLS.C*/
/* DEC/CMS REPLACEMENT HISTORY, Element SO_DRW_LAB.C*/
/* *1    19-JUN-1989 13:26:16 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element SO_DRW_LAB.C*/
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
#ifndef ESI_SE_H
#include "esi_se.h"
#endif
#ifndef ESI_WM_H
#include "esi_wm.h"
#endif
#ifndef ESI_CT_H
#include "esi_ct.h"
#endif

/* Function Description -----------------------------------------------------
Description:

Prototype:
    publicdef INT so_draw_shot_labels(SO_STRUCT *pso,CDP *list,UINT npts);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    pso             -(SO_STRUCT *)
    list            -(CDP *)
    npts            -(UINT)

Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT so_draw_shot_labels (SO_STRUCT *pso, CDP *list, UINT npts)
#else
publicdef INT so_draw_shot_labels (pso, list, npts)
SO_STRUCT *pso;
CDP *list;
UINT npts;
#endif
    {
    SP shot;
    UINT i;
    CHAR label[64];
    DOUBLE pnt_in[2], pnt_out[2];
    DOUBLE offset_x, offset_y;  /* local offset if enhanced label requested */

    
    /**************************************************************************/
    
    /* * SET GKS PARAMTERS * */
    /* --------------------- */
    
    wm_gstxp (pso->seismic_generic.shotpoint_label.text_path);
    wm_gschup (pso->seismic_generic.shotpoint_label.text_up_x,
               pso->seismic_generic.shotpoint_label.text_up_y);
    wm_gschh (pso->seismic_generic.shotpoint_label.text_size);
    wm_gschxp (pso->seismic_generic.shotpoint_label.text_expansion_factor);
    wm_gstxfp (pso->seismic_generic.shotpoint_label.text_font,
               pso->seismic_generic.shotpoint_label.text_precision);
    wm_gstxal (pso->seismic_generic.shotpoint_label.text_alignment[0],
               pso->seismic_generic.shotpoint_label.text_alignment[1]);
    wm_gstxci (pso->seismic_generic.shotpoint_label.text_color);
    
    for (i=0; i<npts; i++)
        {
        se_sp_from_cdp(&(pso->current_line),list[i],&shot);
        sprintf(label,"%.0f",shot);
    
        se_xy_from_cdp(&(pso->current_line),list[i],0.0,&(pnt_in[0]),&(pnt_in[1]));
        ct_map_xy(pnt_in[0],pnt_in[1],&pnt_out[0],&pnt_out[1]);
        if (i!=0 AND i!=npts-1 AND pso->seismic_generic.enhanced_shot_label_option)
            {
            so_enhanced_shot_label(pso, list[i], &offset_x, &offset_y);
            pnt_out[0] += offset_x;
            pnt_out[1] += offset_y;
            }
        else
            {
            pnt_out[0] += pso->seismic_generic.shotpoint_label.text_offset.x;
            pnt_out[1] += pso->seismic_generic.shotpoint_label.text_offset.y;
            }
        wm_gtx(pnt_out[0],pnt_out[1],label);
        }
    return SUCCESS;
    }
/* END:     */
