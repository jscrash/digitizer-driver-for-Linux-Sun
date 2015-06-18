/* DEC/CMS REPLACEMENT HISTORY, Element SO_PRC_SHPTS.C*/
/* *6    24-SEP-1990 17:26:54 GILLESPIE "Beta Deltas"*/
/* *5    26-JUL-1990 17:22:25 GILLESPIE "(SPR 1) Lint run; incorporate changes for horizon end point posting"*/
/* *4    25-JUL-1990 20:06:23 VINCE "(SPR 5644) Header Standardization"*/
/* *3     1-SEP-1989 14:38:12 CONROY "(SPR 9999) MODS FOR NEW FULL_LINE_STRUCT"*/
/* *2    25-JUL-1989 20:07:57 GILLESPIE "(SPR 50) Seismic system revamp"*/
/* *1    19-JUN-1989 13:27:25 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element SO_PRC_SHPTS.C*/
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

#ifndef ESI_TC_H
#include "esi_tc.h"
#endif
publicdef INT soz_build_sym_xy();

/* Function Description -----------------------------------------------------
Description:
    Function to manage shotpoint labeling and Z values.

Prototype:
    publicdef INT so_process_shotpoints(SO_STRUCT *pso);

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
publicdef INT so_process_shotpoints (SO_STRUCT *pso)
#else
publicdef INT so_process_shotpoints (pso)
SO_STRUCT *pso;
#endif
    {
    INT status;
    CDP first_cdp;
    CDP last_cdp;
    SP first_shot;
    SP last_shot;
    INT cdps_per_shot;
    UINT n_shot_lbls;
    UINT n_shot_syms;
    UINT n_data_lbls;
    CDP *shot_lbl_list = (CDP *)0;
    CDP *shot_sym_list = (CDP *)0;
    CDP *data_lbl_list = (CDP *)0;
    
    /***************************************************************************/
    
    /* * GET END POINTS AND CDPS_PER * */
    /* ------------------------------- */
    
    status = so_get_ends (pso, &first_cdp, &last_cdp, &first_shot, &last_shot,
                          &cdps_per_shot);
    if (status IS_NOT_EQUAL_TO SUCCESS)
        {
        return status;
        }
    /* * DRAW SHOTS USING APPROPRIATE METHOD * */
    /* --------------------------------------- */
    
    if (pso->disp_opt.shot_label_nlist EQUALS (NLIST_HEADER)0)
        {
        if (pso->seismic_generic.draw_endpoints_only)
            {
            shot_sym_list = shot_lbl_list = data_lbl_list = (CDP *)
                            tc_alloc (2 * sizeof(CDP));
            shot_sym_list[0] = first_cdp;
            shot_sym_list[1] = last_cdp;
            
            n_shot_lbls = n_shot_syms = n_data_lbls = 2;
            }
        else
            {
            if (pso->seismic_generic.draw_shotpoint_symbol)
                {
                so_make_cdp_list (first_cdp, last_cdp, first_shot, cdps_per_shot,
                                  pso->disp_opt.shot_symbol_int,
                                  pso->disp_opt.shot_symbol_int_offset, &shot_sym_list,
                                  &n_shot_syms);
                }
            if (pso->seismic_generic.draw_shotpoint_label)
                {
                so_make_cdp_list (first_cdp, last_cdp, first_shot, cdps_per_shot,
                                  pso->disp_opt.shot_label_int,
                                  pso->disp_opt.shot_label_int_offset, &shot_lbl_list,
                                  &n_shot_lbls);
                
                if (pso->seismic_generic.horizons_ok OR pso->
                    zzzz AND (pso->ov_type != SO_SEISMIC_MISTIE AND pso->ov_type !=
                              SO_SEISMIC))
                    {
                    so_make_cdp_list (first_cdp, last_cdp, first_shot, cdps_per_shot,
                                      pso->disp_opt.data_label_int,
                                      pso->disp_opt.data_label_int_offset,
                                      &data_lbl_list, &n_data_lbls);
                    }
                }
            }
        if (pso->seismic_generic.draw_shotpoint_label)
            {
            so_draw_shot_labels (pso, shot_lbl_list, n_shot_lbls);
            if (pso->seismic_generic.horizons_ok OR pso->zzzz AND (pso->ov_type !=
                                                                   SO_SEISMIC AND pso->
                                                                   ov_type !=
                                                                   SO_SEISMIC_MISTIE))
                {
                so_draw_data_labels (pso, data_lbl_list, n_data_lbls);
                }
            if (NOT pso->seismic_generic.draw_endpoints_only)
                {
                tc_free (shot_lbl_list);
                if (pso->seismic_generic.horizons_ok OR pso->
                    zzzz AND (pso->ov_type != SO_SEISMIC AND pso->ov_type !=
                              SO_SEISMIC_MISTIE))
                    {
                    tc_free (data_lbl_list);
                    }
                }
            }
        }
    else
        {
        so_special_shot_points (pso, first_cdp, last_cdp, &shot_sym_list,
                                &n_shot_syms);
        }
    if (pso->seismic_generic.draw_shotpoint_symbol)
        {
        so_draw_symbols(pso, shot_sym_list, n_shot_syms);
                /* ------------------------------------------ */
                /* Construct the pso->sym_xy nlist of cdp,x,y */
                /* which will later allow us to draw the      */
                /* connecting line only between the shotpoints*/
                /* actually posted                            */
                /* ------------------------------------------ */
                soz_build_sym_xy(pso,shot_sym_list,n_shot_syms);
        tc_free((VOIDPTR)shot_sym_list);
        }
    if (pso->seismic_generic.draw_horz_terminator AND pso->seismic_generic.
        horizons_ok AND pso->ov_type != SO_SEISMIC_MISTIE)
        {
        so_draw_horz_terminators(pso, first_cdp, last_cdp );
        }
    return SUCCESS;
    }
/* END:     */


/* Function Description -----------------------------------------------------
Description:
    Function to manage shotpoint labeling and Z values.

Prototype:
    publicdef INT soz_build_sym_xy(SO_STRUCT *pso, CDP *sym_list, INT n_shot);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    pso             -(SO_STRUCT *)      Seismic overlay structure.
    sym_list        -(CDP *)            List of CDPs for shotpoints posted
    n_shot          -(INT)              Number of CDPs in sym_list

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT soz_build_sym_xy(SO_STRUCT *pso, CDP *sym_list, INT n_shot)
#else
publicdef INT soz_build_sym_xy(pso, sym_list, n_shot)
SO_STRUCT *pso;
CDP *sym_list;
INT n_shot;
#endif
{
INT status;
UINT i;
DOUBLE x,y;
FLOAT f_x, f_y;
UINT col_list[3];
VOIDPTR var_list[3];
CDP cdp;

status = nl_clone_nlist(pso->cdp_xy, &pso->sym_xy);
status = nl_start_stroke(pso->sym_xy, &i);
col_list[0] = 1;
col_list[1] = 2;
col_list[2] = 3;
var_list[1] = (VOIDPTR) &f_x;
var_list[2] = (VOIDPTR) &f_y;
var_list[0] = (VOIDPTR) &cdp;

for (i=0; i<n_shot; i++)
        {
        cdp = sym_list[i];
        se_xy_from_cdp(&(pso->current_line), sym_list[i], 0.0, &x, &y);
        ct_map_xy(x, y, &x, &y);
        f_x = x; f_y = y;
        status = nl_add_rows(pso->sym_xy, 1, 3, col_list, var_list);
        }

return SUCCESS;
}
/* END:     */
