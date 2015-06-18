/* DEC/CMS REPLACEMENT HISTORY, Element SO_LINE_LOAD.C*/
/* *7    30-JAN-1991 10:05:57 MING "(SPR 6544) keep the original cdp_xy if endpoints only to fix the seismic line clipping*/
/*problem"*/
/* *6    26-JUL-1990 17:21:23 GILLESPIE "(SPR 1) Lint run; incorporate changes for horizon end point posting"*/
/* *5    25-JUL-1990 20:05:04 VINCE "(SPR 5644) Header Standardization"*/
/* *4     2-NOV-1989 09:54:00 GILLESPIE "(SPR 30) Changes from moving mapping structure out of esi_so.h"*/
/* *3    10-OCT-1989 10:02:05 JULIAN "(SPR 5020) mods for CT parameter change"*/
/* *2    25-JUL-1989 20:07:01 GILLESPIE "(SPR 50) Seismic system revamp"*/
/* *1    19-JUN-1989 13:27:00 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element SO_LINE_LOAD.C*/
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

#ifndef ESI_AM_H
#include "esi_am.h"
#endif

#ifndef ESI_MP_H
#include "esi_mp.h"
#endif

#ifndef ESI_CT_H

#include "esi_ct.h"
#include "esi_ct_msg.h"
#include "esi_ct_err.h"

#endif

#ifdef SO_USE_SIS
#include "esi_sis.h"
#endif

#if USE_PROTOTYPES
static INT soz_translate_defaults (SO_TRIGGER *ov, SO_TRIGGER *line, INT flag,
                                       INT *interval, INT *offset);
#else
static INT soz_translate_defaults ();
#endif

/* File Description ---------------------------------------------------------
Overview:

Prototypes and one line descriptions of each function in the file.
Public_Functions:
    publicdef INT so_line_load(SO_STRUCT *pso);

Private_Functions:
    static INT soz_translate_defaults(SO_TRIGGER *ov,SO_TRIGGER *line,INT flag,
        INT *interval,INT *offset);

-----------------------------------------------------------------------------*/

/* Function Description -----------------------------------------------------
Description:
    Function to load seismic line data.

Prototype:
    publicdef INT so_line_load(SO_STRUCT *pso);

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
publicdef INT so_line_load (SO_STRUCT *pso)
#else
publicdef INT so_line_load (pso)
SO_STRUCT *pso;
#endif
    {

#ifdef SO_USE_SIS
    SIS_LINE_STRUCT *pline = (SIS_LINE_STRUCT *)0;  /* SIS line type */
#endif
    MAP_STRUCTURE *pmap = (MAP_STRUCTURE *)pso->map;
    
    UINT Col_List[2];               /* nl clipping stuff */
    UINT temp_Col_List[3];
    VOIDPTR Var_List[3];
    CDP cdp;
    VOIDPTR min_list[2];
    VOIDPTR max_list[2];
    UINT nCol = 2;
    UINT stroke;
    NLIST_HEADER temp_nlist;
    INT status;
    FLOAT x, y;                     /* 4 byte floats */
    FLOAT xmin, xmax, ymin, ymax;   /* 4 byte floats */
    UINT ct_Col_List[3];
    COORD point_in[2];
    COORD point_out[2];
    
    status = SUCCESS;
    
    ct_Col_List[0] = 2, ct_Col_List[1] = 3, ct_Col_List[2] = 1;
    pso->seismic_generic.horizons_ok = FALSE;
    
    /* determine data type and load line */
    
    switch (pso->data_source)
        {
    case SO_INTERPRET:
        
        status = se_rd_nl_shot_xy (&(pso->current_line), &pso->cdp_xy);
        if (status != SUCCESS)
            {
            status = SO_NO_LOCATIONS;
            }
        pso->bulk_shift = 0.0;
        break;

#ifdef SO_USE_SIS
    case SO_SIS:
        pline = (SIS_LINE_STRUCT *)am_allocate (AM_APPLICATION,
                                                sizeof(SIS_LINE_STRUCT));
        status = sis_line_load (&(pso->current_line), pline);
        if (status == SUCCESS)
            {
            pso->cdp_xy = pline->xypl;
            if (pso->cdp_xy == (NLIST_HEADER0))
                {
                status = SO_NO_LOCATIONS;
                }
            pso->bulk_shift = pline->bulk_shift;
            }
        break;
#endif
        }
    if (status != SUCCESS)
        {
        return status;
        }
#ifdef SO_SLOW
    
    /* make cdp dimension float so we can clip to window and      
       explode nlist to every cdp, so one can post at arbitrary frequency,
       (not captured only) and check for no data points */
    
    status = so_explode_cdpxy (pso->cdp_xy, &temp_nlist);
    pso->cdp_xy = temp_nlist;
    
    if (status != SUCCESS)
        {
        nl_free_nlist (pso->cdp_xy);
        return status;
        }
#endif
    
    /* see if we need to clip nlist to map */
    
    Col_List[0] = 2;
    Col_List[1] = 3;
    
    min_list[0] = (VOIDPTR) & xmin;
    min_list[1] = (VOIDPTR) & ymin;
    nl_inq_nlist_info (pso->cdp_xy, NL_NLIST_MIN, 2, Col_List, min_list);
    point_in[0] = xmin;
    point_in[1] = ymin;
    ct_map_xy (point_in[0], point_in[1], &point_out[0], &point_out[1]);
    xmin = point_out[0];
    ymin = point_out[1];
    
    max_list[0] = (VOIDPTR) & xmax;
    max_list[1] = (VOIDPTR) & ymax;
    nl_inq_nlist_info (pso->cdp_xy, NL_NLIST_MAX, 2, Col_List, max_list);
    point_in[0] = xmax;
    point_in[1] = ymax;
    ct_map_xy (point_in[0], point_in[1], &point_out[0], &point_out[1]);
    xmax = point_out[0];
    ymax = point_out[1];
    
    /* see if on map at all */
    
    if (xmax < pmap->lower_left_xy.x OR xmin > pmap->upper_right_xy.x OR ymax <
        pmap->lower_left_xy.y OR ymin > pmap->upper_right_xy.y)
        {
        status = SO_NO_LOCATIONS;
        nl_free_nlist (pso->cdp_xy);
        return status;
        }
    /* clip nlist to map if necessary */
    
    if (xmin < pmap->lower_left_xy.x OR xmax > pmap->upper_right_xy.x OR ymin <
        pmap->lower_left_xy.y OR ymax > pmap->upper_right_xy.y)
        {
        pso->clipped = TRUE;
        }
    else
        {
        pso->clipped = FALSE;
        }

    /* transform nlist to map nlist */
    
    status = ct_map_nlist_xy (pso->cdp_xy, &temp_nlist, ct_Col_List, 0.0);
    if (status IS_NOT_EQUAL_TO CT_SAME_PROJECTIONS)
        {
        nl_free_nlist (pso->cdp_xy);
        if (status != SUCCESS)
            {
            return status;
            }
        pso->cdp_xy = temp_nlist;
        }
    else
        {
        status = SUCCESS;
        }
    if (status == SUCCESS)
        {
        so_load_disp_opt (pso);
        if (NOT pso->disp_opt.use_map_shot_lbl_parms)
            {
            soz_translate_defaults (&(pso->disp_opt.ov), &(pso->disp_opt.line), 1,
                                    &(pso->disp_opt.shot_label_int),
                                    &(pso->disp_opt.shot_label_int_offset));
            }
        if (NOT pso->disp_opt.use_map_shot_sym_parms)
            {
            soz_translate_defaults (&(pso->disp_opt.ov), &(pso->disp_opt.line), 2,
                                    &(pso->disp_opt.shot_symbol_int),
                                    &(pso->disp_opt.shot_symbol_int_offset));
            }
        if (NOT pso->disp_opt.use_map_data_lbl_parms)
            {
            soz_translate_defaults (&(pso->disp_opt.ov), &(pso->disp_opt.line), 3,
                                    &(pso->disp_opt.data_label_int),
                                    &(pso->disp_opt.data_label_int_offset));
            }
        }
    return status;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:

Prototype:
    static INT soz_translate_defaults(SO_TRIGGER *ov,SO_TRIGGER *line,INT flag,
        INT *interval,INT *offset);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    ov              -(SO_TRIGGER *)
    line            -(SO_TRIGGER *)
    flag            -(INT)
    interval        -(INT *)
    offset          -(INT *)

Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PRIVATE to <so_line_load.c>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static INT soz_translate_defaults (SO_TRIGGER *ov, SO_TRIGGER *line, INT flag,
                                       INT *interval, INT *offset)
#else
static INT soz_translate_defaults (ov, line, flag, interval, offset)
SO_TRIGGER *ov;
SO_TRIGGER *line;
INT flag;
INT *interval;
INT *offset;
#endif
    {
    
    CHAR trigger;
    INT value;
    
    switch (flag)
        {
    case 1:                         /* SHOT LABELS */
        if (ov->shot_label_trigger[0] EQUALS '\0')
            {
            trigger = ov->shot_label_trigger[0];
            value = ov->shot_label_value;
            }
        else
            {
            trigger = line->shot_label_trigger[0];
            value = line->shot_label_value;
            }
        break;
        
    case 2:                         /* SHOT SYMBOLS */
        if (ov->shot_symbol_trigger[0] EQUALS '\0')
            {
            trigger = ov->shot_symbol_trigger[0];
            value = ov->shot_symbol_value;
            }
        else
            {
            trigger = line->shot_symbol_trigger[0];
            value = line->shot_symbol_value;
            }
        break;
        
    case 3:                         /* DATA LABELS */
        if (ov->zvalue_trigger[0] EQUALS '\0')
            {
            trigger = ov->zvalue_trigger[0];
            value = ov->zvalue_value;
            }
        else
            {
            trigger = line->zvalue_trigger[0];
            value = line->zvalue_value;
            }
        break;
        }
    switch (trigger)
        {
    case 'N':
    case 'M':
        *interval = value;
        *offset = 0;
        break;
        
    case 'E':
        *interval = 10;
        *offset = value;
        break;
        }
    return SUCCESS;
    }
/* END:     */
