/* DEC/CMS REPLACEMENT HISTORY, Element HL_HI_WELL.C*/
/* *8    16-AUG-1991 11:41:41 JTM "(SPR 0) Cast arg to wm_gslwsc() to DOUBLE."*/
/* *7    17-AUG-1990 21:49:19 VINCE "(SPR 5644) Code Cleanup"*/
/* *6    20-MAR-1990 16:45:18 WALTERS "(SPR 5088) Restrict highlighting to the current maps boundries"*/
/* *5    17-JAN-1990 15:31:46 WALTERS "(SPR 0) Add nl_init_nlist_info to name dimensions"*/
/* *4    10-OCT-1989 09:55:09 JULIAN "(SPR 5020) mods for CT parameter change"*/
/* *3    11-AUG-1989 21:05:17 CONROY "(SPR 0) Revised global, selects, SQL macros & fixes"*/
/* *2    30-JUL-1989 01:10:10 GILLESPIE "(SPR 1) Fix some minor casting problems"*/
/* *1    19-JUN-1989 12:54:45 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element HL_HI_WELL.C*/
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

#ifndef ESI_GL_DEFS_H

#include "esi_gl_defs.h"

#endif

#ifndef ESI_GB_H

#include "esi_gb.h"

#endif

#ifndef ESI_WM_H

#include "esi_wm.h"

#endif

#ifndef ESI_WM_CLS_H

#include "esi_wm_cls.h"

#endif

#ifndef ESI_GKS_H

#include "esi_gks.h"

#endif

#ifndef ESI_CS_H

#include "esi_cs.h"

#endif

#ifndef ESI_WE_WELLS_H

#include "esi_we_wells.h"

#endif

#ifndef ESI_NL_H

#include "esi_nl.h"

#endif

#ifndef ESI_HO_H

#include "esi_ho.h"

#endif

#ifndef ESI_MG_H

#include "esi_mg.h"

#endif

#ifndef ESI_AM_H

#include "esi_am.h"

#endif

#ifndef ESI_MP_H

#include "esi_mp.h"

#endif

#ifndef ESI_HL_MSG_H

#include "esi_hl_msg.h"

#endif

#ifndef ESI_HL_ERR_H

#include "esi_hl_err.h"

#endif

#define MAX_POINTS 100
static GKS_REAL gks_px[MAX_POINTS], gks_py[MAX_POINTS];

/* well {uwi|x|y} nlist bookeeping declarations */
static UINT dim_list[3] = 
    {
    1,
    2,
    3
    };


static INT units[3] = 
    {
    0,
    0,
    0
    };


static size_t data_sizes[3] = 
    {
    sizeof(UWI), sizeof(FLOAT), sizeof(FLOAT)
    };


static INT data_types[3] = 
    {
    CHAR_TYPE,
    FLOAT_TYPE,
    FLOAT_TYPE
    };


static INT value_inc[3] = 
    {
    VALUE_LIST,
    VALUE_LIST,
    VALUE_LIST
    };


static CHAR *c_name[4] = 
    {
    "UWI",
    "X",
    "Y",
    NULL
    };


static VOIDPTR value_list[3];

typedef struct xy_loc
    {
    FLOAT x, y;
    } XY_LOC;

static INT hlz_drawWells ();
static INT hlz_connectWells ();

/* File Description ---------------------------------------------------------
Overview:
    High-lights the wells in the current select list of wells on the mapping
    display.

Prototypes and one line descriptions of each function in the file.
Public_Functions:
    publicdef INT hl_hilite_wells(SELECT_STRUCT  *select_list, NLIST_HEADER well_nlist );

Private_Functions:
    static INT hlz_connectWells(NLIST_HEADER nlist);
    
    static INT hlz_drawWells(NLIST_HEADER nlist);

-----------------------------------------------------------------------------*/

/* Function Description -----------------------------------------------------
Description:
    Hl_hilite_wells is called to highlight the current list of wells.
    The hilite symbols or connecting line are placed into the
    Well Hilites group in the Highlight Overlay, behind the other
    map overlays.

    The highlight overlay is created if it does not already exist.

    INITIAL CONDITIONS:

         1. The map window must exist.
         2. The current well list must exist.
         3. Hilite symbol, color, size, connect option must have been selected.

    TODO:

         1. Eliminate select list parameter -- pass color, width, symbol etc.
         3. As in hl_hilite_seis, we assume that application_id that created
            the map window is the same as the current application_id.  This
            is not right.

Prototype:
    publicdef INT hl_hilite_wells(SELECT_STRUCT  *select_list, NLIST_HEADER well_nlist );

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    select_list     -(SELECT_STRUCT *)  
    well_nlist      -(NLIST_HEADER)

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT hl_hilite_wells (SELECT_STRUCT *select_list, NLIST_HEADER well_nlist)
#else
publicdef INT hl_hilite_wells (select_list, well_nlist)
SELECT_STRUCT *select_list;
NLIST_HEADER well_nlist;
#endif
    {
    NLIST_HEADER well_xy_list;
    NLIST_HEADER nlist;
    INT status = SUCCESS;
    GKS_INT plType, plColi;
    GKS_REAL plWidth;
    INT appFrom, winFrom, tnrFrom, groFrom;
    INT appTo, winTo, tnrTo, groTo, segTo, picTo;
    INT segment, wmStatus;
    INT map_window, map_transform, hilite_overlay_group;
    INT well_hilite_group, seismic_picking_group, seismic_hilite_group;
    INT free_xy_list = FALSE;
    CHAR hilite_name[STMAX];
    UINT npts;
    INT i;
    SELECT_KEY_STRING dim1_key;
    UWI uwi;
    DOUBLE val_x, val_y;
    FLOAT well_x, well_y;
    INT highlighted_wells = 0;
    UINT ndim = 3;
    UINT w_ndim = 1;
    VOIDPTR w_value_list[1];
    UINT w_dim_list[1];
    INT blocksize = 100;
    INT main_wkid = MAIN_WKID;
    MAP_STRUCTURE *mapdef;
    
    if (select_list == (SELECT_STRUCT *)NULL)
        {
        return(FAIL);
        }
    wmowhere (&appFrom, &winFrom, &tnrFrom, &groFrom, &wmStatus);
    
    status = mp_front_map (&mapdef);
    if (status != SUCCESS)
        {
        return status;
        }
    map_window = select_list->window_id;
    map_transform = select_list->transform_id;
    
    if (hl_query_hilite_overlay (appFrom, map_window, &hilite_overlay_group,
                                 &well_hilite_group, &seismic_picking_group,
                                 &seismic_hilite_group))
        {
        return FAIL;
        }
    /* create a well xy nlist from the input well uwi nlist */
    status = nl_init_nlist (&well_xy_list, ndim, units, data_sizes, data_types,
                            value_inc, blocksize);
    status = nl_set_nlist_info (well_xy_list, NL_NLIST_COLUMN_NAME, ndim, dim_list,
                                (VOIDPTR)c_name);
    if (status != SUCCESS)
        {
        return status;
        }
    status = nl_start_stroke (well_xy_list, &npts);
    
    status = nl_inq_nlist_int (well_nlist, NL_NLIST_NPOINTS, (INT *) & npts);
    
    w_dim_list[0] = 1, w_value_list[0] = (VOIDPTR)dim1_key;
    value_list[0] = (VOIDPTR)uwi;
    value_list[1] = (VOIDPTR) & well_x;
    value_list[2] = (VOIDPTR) & well_y;
    
    highlighted_wells = 0;
    
    /* loop through each well and compute its coords then stuff in new list */
    for (i = 0; i < npts; i++)
        {
        status = (i == 0) ? nl_first_point (well_nlist, w_ndim, w_dim_list,
                                            w_value_list) :
                 nl_next_point (well_nlist, w_ndim, w_dim_list, w_value_list);
        
        hoblockmove (dim1_key, uwi, sizeof(UWI));
        uwi[sizeof(UWI) - 1] = '\0';
        
        status = ct_get_well_location (uwi, &val_x, &val_y);
        well_x = val_x;
        well_y = val_y;
        
        if (status == SUCCESS AND mapdef->lower_left_xy.x <
            well_x AND mapdef->upper_right_xy.x > well_x AND mapdef->lower_left_xy.y <
            well_y AND mapdef->upper_right_xy.y > well_y)
            {
            highlighted_wells++;
            status = nl_add_points (well_xy_list, 1, ndim, dim_list, value_list);
            }
        }
    if (highlighted_wells > 0)
        free_xy_list = TRUE;
    
    /* attach directly to the well hilite group */
    
    wmoattach (appFrom, map_window, map_transform, well_hilite_group, &wmStatus);
    
    /* build one new segment containing hilites for current list */
    
    wmsallocate (&segment);
    sprintf (hilite_name, mg_message (HL_WELL_HILITE), highlighted_wells);
    
    wmoset (segment, WM_CLASS_WELL_HILITE, "", hilite_name);
    
    if (select_list->connected_flag)
        {
        plType = 1;
        wm_gsln (plType);
        plWidth = (GKS_REAL)select_list->width;
        wm_gslwsc ((DOUBLE)plWidth);
        plColi = select_list->color;
        wm_gsplci (plColi);
        
        /* draw the connecting line between the wells */
        status = hlz_connectWells (well_xy_list);
        }
    else
        {
        if (select_list->symbol_set != NULL)
            {
            cs_set_group (select_list->symbol_set);
            }
        cs_set_code (select_list->symbol);
        cs_set_color (select_list->color);
        cs_set_scale_factor (select_list->size);
        
        /* draw the well hilite symbols */
        status = hlz_drawWells (well_xy_list);
        }
    wmsclose (segment);
    
    /* place the hilite symbols behind the normal overlays */
    
    wmsrelprior (segment, select_list->segment_id, LOWER);
    wm_grsgwk (main_wkid);
    
    /* attach back to where we came from */
    wmoattach (appFrom, winFrom, tnrFrom, groFrom, &wmStatus);
    
    /* free allocated temporary well xy nlist if needed */
    if (free_xy_list)
        nl_free_nlist (well_xy_list);
    
    return(status);
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:

Prototype:
    static INT hlz_drawWells(NLIST_HEADER nlist);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    nlist           -(NLIST_HEADER)

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PRIVATE to <hl_hi_well.c>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static INT hlz_drawWells (NLIST_HEADER nlist)
#else
static INT hlz_drawWells (nlist)
NLIST_HEADER nlist;
#endif
    {
    INT status = FAIL;
    INT flags;
    UINT total_points = 0;
    INT i = 0;
    UINT ndim = 3;
    UWI uwi;
    XY_LOC loc;
    
    if (nlist == (NLIST_HEADER)NULL)
        {
        return(FAIL);
        }
    value_list[0] = (VOIDPTR)uwi;
    value_list[1] = (VOIDPTR) & loc.x;
    value_list[2] = (VOIDPTR) & loc.y;
    
    status = nl_inq_nlist_int (nlist, NL_NLIST_NPOINTS, (INT *) & total_points);
    for (i = 0, flags = nl_first_point (nlist, ndim, dim_list, value_list);
         (flags >= 0) AND (i < total_points);
                                                          i++,
                                                          flags =
                                                          nl_next_point (
                                                                 nlist,
                                                           ndim, dim_list, value_list))
        {
        status = SUCCESS;
        cs_draw_one ((DOUBLE)loc.x, (DOUBLE)loc.y);
        }
    return(status);
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:

Prototype:
    static INT hlz_connectWells(NLIST_HEADER nlist);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    nlist           -(NLIST_HEADER)

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PRIVATE to <hl_hi_well.c>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static INT hlz_connectWells (NLIST_HEADER nlist)
#else
static INT hlz_connectWells (nlist)
NLIST_HEADER nlist;
#endif
    {
    INT status;
    UINT ndim = 3;
    INT i, flags;
    UINT total_points;
    DOUBLE wx, wy;
    UWI uwi;
    XY_LOC loc;
    GKS_INT npts;
    
    if (nlist == (NLIST_HEADER)NULL)
        {
        return(FAIL);
        }
    value_list[0] = (VOIDPTR)uwi;
    value_list[1] = (VOIDPTR) & loc.x;
    value_list[2] = (VOIDPTR) & loc.y;
    
    npts = 0;
    status = nl_inq_nlist_int (nlist, NL_NLIST_NPOINTS, (INT *) & total_points);
    for (i = 0, flags = nl_first_point (nlist, ndim, dim_list, value_list);
         (flags >= 0) AND (i < total_points);
                                                          i++,
                                                          flags =
                                                          nl_next_point (
                                                                 nlist,
                                                           ndim, dim_list, value_list))
        {
        status = SUCCESS;
        
        gks_px[npts] = loc.x;
        gks_py[npts] = loc.y;
        
        npts++;
        if (npts == MAX_POINTS)
            {
            wm_gpl (npts, gks_px, gks_py);
            gks_px[0] = gks_px[npts - 1];
            gks_py[0] = gks_py[npts - 1];
            npts = 1;
            }
        }
    /* draw any points remaining in buffer */
    if (npts > 1)
        {
        wm_gpl (npts, gks_px, gks_py);
        }
    return(status);
    }
/* END:     */
