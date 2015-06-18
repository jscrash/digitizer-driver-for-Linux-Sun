/* DEC/CMS REPLACEMENT HISTORY, Element SO_DRW_DT_LBS.C */
/* *4    24-SEP-1990 17:19:55 GILLESPIE "Beta Deltas" */
/* *3    26-JUL-1990 17:19:03 GILLESPIE "(SPR 1) Lint run; incorporate changes for horizon end point posting" */
/* *2    25-JUL-1990 20:03:05 VINCE "(SPR 5644) Header Standardization" */
/* *1     5-MAR-1990 17:01:24 VINCE "(spr 6015) shorten name for UNIX compatiblilty." */
/* DEC/CMS REPLACEMENT HISTORY, Element SO_DRW_DT_LBS.C */
/* DEC/CMS REPLACEMENT HISTORY, Element SO_DRW_DT_LBS.C*/
/* *2    31-AUG-1990 18:25:14 SCD "(SPR 5750) Add hooks to compute precision posting angles"*/
/* *1    13-AUG-1990 17:11:33 SYSTEM "Finder 6.1"*/
/* DEC/CMS REPLACEMENT HISTORY, Element SO_DRW_DT_LBS.C*/

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
#include "esi_se.h"
#include "esi_wm.h"
#include "esi_ct.h"

/* Function Description -----------------------------------------------------
Description:
    Function to draw shot label and Z value arrays.

Prototype:
    publicdef INT so_draw_data_labels(SO_STRUCT *pso,CDP *list,UINT npts);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    pso             -(SO_STRUCT *) Seismic overlay structure.
    list            -(CDP *)
    npts            -(UINT)

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT so_draw_data_labels (SO_STRUCT *pso, CDP *list, UINT npts)
#else
publicdef INT so_draw_data_labels (pso, list, npts)
SO_STRUCT *pso;
CDP *list;
UINT npts;
#endif
    {
    UINT i;
    INT status;
    CHAR label[64];
    DOUBLE zvalue;
    DOUBLE xin, yin, xout, yout;
    DOUBLE offset_x, offset_y;  /* local offset if enhanced label requested */

    /***********************************************************************/
    
    /* * SET GKS ATTRIBUTES * */
    /* ---------------------- */
    
    wm_gstxp (pso->seismic_generic.zvalue_label.text_path);
    wm_gschup (pso->seismic_generic.zvalue_label.text_up_x,
               pso->seismic_generic.zvalue_label.text_up_y);
    wm_gschh (pso->seismic_generic.zvalue_label.text_size);
    wm_gschxp (pso->seismic_generic.zvalue_label.text_expansion_factor);
    wm_gstxfp (pso->seismic_generic.zvalue_label.text_font,
               pso->seismic_generic.zvalue_label.text_precision);
    wm_gstxal (pso->seismic_generic.zvalue_label.text_alignment[0],
               pso->seismic_generic.zvalue_label.text_alignment[1]);
    wm_gstxci (pso->seismic_generic.zvalue_label.text_color);
    
    for (i = 0; i < npts; i++)
        {
        se_xy_from_cdp (&(pso->current_line), list[i], 0.0, &xin, &yin);
        
        if (pso->zzzz)
            {
            strcpy ((char *)label, "zzzz");
            }
        else
            {
            status = so_zvalue (pso, list[i], list[i], xin, yin, &zvalue);
            sprintf ((char *)label, "%.0f", zvalue);
            }
        if ((status != SUCCESS) OR (zvalue == (DOUBLE)SO_NULL))
            continue;
        pso->seismic_generic.zvalue_label.text_string = label;
        
        ct_map_xy (xin, yin, &xout, &yout);
        
        if (i!=0 AND i!=npts-1 AND pso->seismic_generic.enhanced_shot_label_option)
            {
            so_enhanced_data_label(pso, list[i], &offset_x, &offset_y);
            xout += offset_x;
            yout += offset_y;
            }
        else
            {
            xout += pso->seismic_generic.zvalue_label.text_offset.x;
            yout += pso->seismic_generic.zvalue_label.text_offset.y;
            }
        wm_gtx(xout, yout, label);
        }
    return SUCCESS;
    }
/* END:     */
