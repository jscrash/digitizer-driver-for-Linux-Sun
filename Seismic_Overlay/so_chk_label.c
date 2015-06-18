/* DEC/CMS REPLACEMENT HISTORY, Element SO_CHK_LABEL.C*/
/* *4    26-JUL-1990 17:18:32 GILLESPIE "(SPR 1) Lint run; incorporate changes for horizon end point posting"*/
/* *3    25-JUL-1990 20:02:33 VINCE "(SPR 5644) Header Standardization"*/
/* *2    25-JUL-1989 20:04:44 GILLESPIE "(SPR 50) Seismic system revamp"*/
/* *1    19-JUN-1989 13:26:00 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element SO_CHK_LABEL.C*/
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

#include "esi_so.h"
#include "esi_math.h"

/* Function Description -----------------------------------------------------
Description:
    Function to see if a shotpoint should be labeled.

Prototype:
    publicdef INT so_check_label(SO_STRUCT *pso,CDP cdp,SP shotpoint,BOOL *sym,BOOL *lab,
        BOOL *zval,BOOL *special);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    pso             -(SO_STRUCT *) Seismic overlay structure.
    cdp             -(CDP)
    shotpoint       -(SP)
    sym             -(BOOL *)
    lab             -(BOOL *)
    zval            -(BOOL *)
    special         -(BOOL *)

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT so_check_label (SO_STRUCT *pso, CDP cdp, SP shotpoint, BOOL *sym,
                              BOOL *lab, BOOL *zval, BOOL *special)
#else
publicdef INT so_check_label (pso, cdp, shotpoint, sym, lab, zval, special)
SO_STRUCT *pso;
CDP cdp;
SP shotpoint;
BOOL *sym, *lab, *zval;
BOOL *special;
#endif
    {
    
    INT status;
    DOUBLE angle;
    INT paraperp;
    UINT ngot;
    UINT Col_List[3];
    VOIDPTR value_list[3];
    DOUBLE zang;
    
    status = FAIL;
    
    *sym = *lab = *zval = FALSE;
    
    /* see if special */
    
    if (pso->disp_opt.shot_label_nlist == (NLIST_HEADER)0)
        {
        *special = FALSE;
        }
    else
        {
        nl_inq_nlist_int (pso->disp_opt.shot_label_nlist, NL_NLIST_NPOINTS,
                          (INT *) & ngot);
        if (ngot == 0)
            {
            status = FAIL;
            }
        else
            {
            
            nl_set_current_row (pso->disp_opt.shot_label_nlist, 1, 1);
            status = so_find_point (pso->disp_opt.shot_label_nlist, cdp);
            }
        if (status == SUCCESS)
            {
            pso->seismic_generic.shotpoint_label = pso->disp_opt.shot_label_default;
            *special = TRUE;
            value_list[0] = (VOIDPTR) & angle;
            Col_List[0] = 2;
            value_list[1] = (VOIDPTR) & paraperp;
            Col_List[1] = 3;
            nl_get_points (pso->disp_opt.shot_label_nlist, 1, 2, Col_List, value_list,
                           &ngot);
            
            /* paraperp = SO_NOPARA means skip it */
            
            if (paraperp == SO_NOPARA)
                {
                so_check_shot (pso, shotpoint, pso->disp_opt.ov.shot_symbol_trigger,
                               pso->disp_opt.ov.shot_symbol_value,
                               pso->disp_opt.line.shot_symbol_trigger,
                               pso->disp_opt.line.shot_symbol_value, sym);
                if (*sym)
                    {
                    status = SUCCESS;
                    }
                else
                    {
                    status = FAIL;
                    }
                return status;
                }
            so_angpp_to_text (&pso->seismic_generic.shotpoint_label, angle, paraperp);
            zang = angle / 57.29582;
            angle = angle + 180.0;
            if (angle >= 360.0)
                {
                angle -= 360.0;
                }
            so_angpp_to_text (&pso->seismic_generic.zvalue_label, angle, paraperp);
            /* do offsets */
            
            pso->seismic_generic.shotpoint_label.text_offset.x = cos (zang) *
                                                                 pso->seismic_generic.
                                                                 shotpoint_symbol.
                                                                 symbol_size * 1.5;
            pso->seismic_generic.shotpoint_label.text_offset.y = sin (zang) *
                                                                 pso->seismic_generic.
                                                                 shotpoint_symbol.
                                                                 symbol_size * 1.5;
            
            pso->seismic_generic.zvalue_label.text_offset.x =
                                                              (-pso
                                                              ->seismic_generic.
                                                               shotpoint_label.
                                                               text_offset.x);
            pso->seismic_generic.zvalue_label.text_offset.y =
                                                              (-pso
                                                              ->seismic_generic.
                                                               shotpoint_label.
                                                               text_offset.y);
            
            *sym = TRUE;
            *lab = TRUE;
            *zval = TRUE;
            return status;
            }
        else
            {
            *special = FALSE;
            }
        }
    so_check_shot (pso, shotpoint, pso->disp_opt.ov.shot_symbol_trigger,
                   pso->disp_opt.ov.shot_symbol_value,
                   pso->disp_opt.line.shot_symbol_trigger,
                   pso->disp_opt.line.shot_symbol_value, sym);
    
    if (!*sym)
        {
        status = FAIL;
        return status;
        }
    status = SUCCESS;
    so_check_shot (pso, shotpoint, pso->disp_opt.ov.shot_label_trigger,
                   pso->disp_opt.ov.shot_label_value,
                   pso->disp_opt.line.shot_label_trigger,
                   pso->disp_opt.line.shot_label_value, lab);
    
    so_check_shot (pso, shotpoint, pso->disp_opt.ov.zvalue_trigger,
                   pso->disp_opt.ov.zvalue_value, pso->disp_opt.line.zvalue_trigger,
                   pso->disp_opt.line.zvalue_value, zval);
    
    return status;
    }
/* END:     */
