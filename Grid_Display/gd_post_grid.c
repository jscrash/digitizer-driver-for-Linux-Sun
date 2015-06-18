/* DEC/CMS REPLACEMENT HISTORY, Element GD_POST_GRID.C*/
/* *11    8-AUG-1991 16:30:26 MING "(SPR 0) change label from 6 decimal places to 2"*/
/* *10   15-JUL-1991 11:25:30 MING "(SPR 0) print message when map area of interest is not the same as the grid file"*/
/* *9     5-JUL-1991 16:41:10 JULIAN "(SPR 3039) Inserted a check not to drawn anything if type is NO  SYMBOL"*/
/* *8     1-JUL-1991 15:46:18 MING "(SPR 8475) label clipping"*/
/* *7    18-JUN-1991 14:38:40 JULIAN "(SPR 2940) (Jantina) Removed check on coordinate system flag - only need to check map*/
/*projections"*/
/* *6    17-DEC-1990 17:26:34 GILLESPIE "(SPR 6263) Fix label position logic; out of sync with expression_codes"*/
/* *5    14-AUG-1990 13:27:40 VINCE "(SPR 5644) Header Standardization"*/
/* *4    18-DEC-1989 11:51:19 CONROY "(SPR 5078) Mods for plotting GKS polymarkers"*/
/* *3     1-NOV-1989 10:12:08 GILLESPIE "(SPR 30) Adjust for include file changes; add pragmas to HO routines"*/
/* *2    18-JUL-1989 14:49:24 GORDON "(SPR 100) Add mods from Sun port"*/
/* *1    19-JUN-1989 12:52:34 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element GD_POST_GRID.C*/
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
#include "esi_gd_defs.h"

#ifndef ESI_CO_H

#include "esi_co.h"

#endif

#ifndef ESI_DG_DEFS_H

#include "esi_dg_defs.h"

#endif

#ifndef ESI_CO_ERR_H

#include "esi_co_err.h"

#endif

#ifndef ESI_OV_ERR_H

#include "esi_ov_err.h"

#endif

#ifndef ESI_WM_CLS_H

#include "esi_wm_cls.h"

#endif

#ifndef ESI_WM_H

#include "esi_wm.h"

#endif

#ifndef ESI_NL_H

#include "esi_nl.h"

#endif

#ifndef ESI_MP_H

#include "esi_mp.h"

#endif

#ifndef ESI_MM_H

#include "esi_mm.h"

#endif

#ifndef ESI_TS_H

#include "esi_ts.h"

#endif

#ifndef ESI_AM_H

#include "esi_am.h"

#endif

#ifndef ESI_FM_H

#include "esi_fm.h"

#endif

#ifndef ESI_LU_H

#include "esi_lu.h"

#endif

/* Function Description -----------------------------------------------------
Description:
    Function to post the supplied grid.

Prototype:
    publicdef INT gd_post_grid(GRID_DISPLAY *grid_display,CONTOURS *contours,SYMBOL *symbol);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    grid_display    -(GRID_DISPLAY *)
    contours        -(CONTOURS *) Contour overlay information.
    symbol          -(SYMBOL *)

Return Value/Status:
    SUCCESS - Successful completion.
    CO_NO_GRID_DATA
    OV_PROJECTION_MISMATCH
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT gd_post_grid (GRID_DISPLAY *grid_display, CONTOURS *contours,
                            SYMBOL *symbol)
#else
publicdef INT gd_post_grid (grid_display, contours, symbol)
GRID_DISPLAY *grid_display;
CONTOURS *contours;
SYMBOL *symbol;
#endif
    {
    INT status = SUCCESS;           /* RETURN STATUS */
    FLOAT *grid_z = 0;              /* GRIDDED Z VALUES */
    INT nrows;                      /* NUMBER OF GRID ROWS */
    INT ncols;                      /* NUMBER OF GRID COLUMNS */
    FLOAT znull;                    /* NULL VALUE FOR GRID */
    FILENAME grid_name;             /* GRID FILE NAME */
    FLOAT *sub_x = 0;
    FLOAT *sub_y = 0;
    INT sub_npts;
    FLOAT *dummy;
    FLOAT x;
    FLOAT y;
    INT xint;
    INT yint;
    INT i;                          /* DUMMY INDEX */
    UINT j;                         /* DUMMY INDEX */
    UINT k;
    NLIST_HEADER grid_nlist;        /* NLIST TO STORE GRID_Z */
    GRID_NLIST_USER_STRUCT grid_user;   /* NLIST USER STRUCT FOR CONTOURS */
    MAP_STRUCTURE *map;             /* MAP INFORMATION STRUCTURE */
    CHAR label[30];
    INT seg_id;
    CHAR segname[100];
    FLOAT xpost;
    FLOAT ypost;
    DOUBLE xoffset;
    DOUBLE yoffset;
    FM_DESCRIB desc;
    BOOL no_symbol;
    CHAR message[240];
    
    /* NLIST INFORMATION */
    UINT d_list[1];                 /*      DIM LIST */
    VOIDPTR v_list[1];              /*      VALUE LIST */
    
    /* ********************************************************************** */
    /* * CHECK IF GOING TO DISPLAY ANYTHING * */
    /* -------------------------------------- */
    
    if (grid_display->suppress_text AND grid_display->suppress_symbols)
        {
        goto finished;
        }

    /* check for no symbol case, don't display anything if symbol type is
       NO SYMBOL (purna - 7/3/91) */

    no_symbol = FALSE;
    if(symbol->symbol_type <= 0)
      no_symbol = TRUE;

    /* * READ AND CHECK GRID * */
    /* ----------------------- */
    
    strcpy (desc, "");
    if ((status = fm_init_file ("ESI$GRIDS", contours->grid_file, "GRID", "r", -1,
                                desc, NULL_FUNCTION_PTR, grid_name, "MAPPING")) < 0)
        {
        goto finished;
        }
    status = nl_read_nlist (&grid_nlist, grid_name, "GRIDS");
    fm_release_file ("ESI$GRIDS", contours->grid_file, "GRID");
    if (status < 0)
        {
        goto finished;
        }
    nl_inq_nlist_user_struct (grid_nlist, sizeof(GRID_NLIST_USER_STRUCT),
                              (VOIDPTR)&grid_user, (size_t *)&i);
    
    mp_get_current_map (&map);
    if (ARE_DIFFERENT (grid_user.projection,map->map_projection))
        {
        status = OV_PROJECTION_MISMATCH;
        goto finished;
        }
    /* GET ROW AND COL INFORMATION */
    
    nl_inq_nlist_int (grid_nlist, NL_NLIST_NTABLES, &nrows);
    nl_inq_table_int (grid_nlist, 1, NL_TABLE_NROWS, &ncols);
    if (nrows EQUALS 0 OR ncols EQUALS 0)
        {
        status = CO_NO_GRID_DATA;
        goto finished;
        }
    /* ALLOCATE AND FILL GRID */
    
    grid_z = (FLOAT *) am_allocate (AM_APPLICATION, (ncols * nrows) * sizeof(FLOAT));
    dummy = grid_z;
    d_list[0] = 1;
    for (i = 1; i < nrows + 1; i++)
        {
        nl_set_current_row (grid_nlist, 1, i);
        v_list[0] = (VOIDPTR)dummy;
        nl_get_rows (grid_nlist, ncols, 1, d_list, v_list, &j);
        dummy += ncols;
        }
    /* * GET NULL VALUE * */
    /* ------------------ */
    
    v_list[0] = (VOIDPTR)&znull;
    nl_inq_nlist_info (grid_nlist, NL_NLIST_NULL_VALUE, 1, d_list, v_list);
    
    /* * GET DELTA X AND Y * */
    /* --------------------- */
    
    xint = (grid_user.xmax - grid_user.xmin) / (ncols - 1);
    yint = (grid_user.ymax - grid_user.ymin) / (nrows - 1);
    
    /* * ALLOCATE SEGMENT * */
    /* -------------------- */
    
    wmsallocate (&seg_id);
    
    sprintf (segname, "GRID: %s", contours->grid_file);
    wmoset (seg_id, WM_CLASS_SEGMENT, "", segname);
    
    /* * INIT GKS SYMBOL ATTRIBUTES * */
    /* ------------------------------ */
    
    if (NOT grid_display->suppress_symbols AND NOT no_symbol)
        {
        wm_gspmci (symbol->symbol_color);
        wm_gsmk (symbol->symbol_type);
        x = symbol->symbol_size * map->wcs_to_inches;
        wm_gsmksc (x);
        
        sub_x = (FLOAT *) am_allocate (AM_GLOBAL, nrows * ncols * sizeof(FLOAT));
        sub_y = (FLOAT *) am_allocate (AM_GLOBAL, nrows * ncols * sizeof(FLOAT));
        sub_npts = 0;
        }
    /* * INIT GKS TEXT ATTRIBUTES * */
    /* ---------------------------- */
    
    if (NOT grid_display->suppress_text)
        {
        GKS_INT i1, i2;
        
        wm_gstxci (grid_display->text_color);
        wm_gschh (grid_display->text_size);
        lu_text_to_index ("TEXT_PATH", (INDEX *)&i1, "RIGHT");
        wm_gstxp (i1);
        lu_text_to_index ("FONT", (INDEX *)&i1, "SIMPLEX");
        lu_text_to_index ("TEXT_PRECISION", (INDEX *)&i2, "STROKE");
        wm_gstxfp (i1, i2);
        wm_gschxp (1.0);
        wm_gschsp (0.0);
        wm_gschup (0.0, 1.0);
        
        switch (grid_display->label_position)
            {
        case 1:                     /* ABOVE */
            xoffset = 0.0;
            yoffset = 0.75 * symbol->symbol_size;
            lu_text_to_index ("TEXT_HORIZ_ALIGN", (INDEX *)&i1, "CENTER");
            lu_text_to_index ("TEXT_VERT_ALIGN", (INDEX *)&i2, "BOTTOM");
            break;
            
        case 2:                     /* BELOW */
            xoffset = 0.0;
            yoffset = -0.75 * symbol->symbol_size;
            lu_text_to_index ("TEXT_HORIZ_ALIGN", (INDEX *)&i1, "CENTER");
            lu_text_to_index ("TEXT_VERT_ALIGN", (INDEX *)&i2, "TOP");
            break;
            
        case 3:                     /* CENTERED */
            xoffset = 0.0;
            yoffset = 0.0;
            lu_text_to_index ("TEXT_HORIZ_ALIGN", (INDEX *)&i1, "CENTER");
            lu_text_to_index ("TEXT_VERT_ALIGN", (INDEX *)&i2, "HALF");
            break;
            
        case 4:                     /* LEFT */
            xoffset = -0.75 * symbol->symbol_size;
            yoffset = 0.0;
            lu_text_to_index ("TEXT_HORIZ_ALIGN", (INDEX *)&i1, "RIGHT");
            lu_text_to_index ("TEXT_VERT_ALIGN", (INDEX *)&i2, "HALF");
            break;
            
        case 5:                     /* RIGHT */
            xoffset = 0.75 * symbol->symbol_size;
            yoffset = 0.0;
            lu_text_to_index ("TEXT_HORIZ_ALIGN", (INDEX *)&i1, "LEFT");
            lu_text_to_index ("TEXT_VERT_ALIGN", (INDEX *)&i2, "HALF");
            break;
            
            }
        wm_gstxal (i1, i2);
        }

    if (map->lower_left_xy.x != grid_user.xmin OR
	map->lower_left_xy.y != grid_user.ymin OR
	map->upper_right_xy.x != grid_user.xmax OR
	map->upper_right_xy.y != grid_user.ymax)
        {
        sprintf (message,"'%s' overlay does not exactly match the map area of interest.",contours->grid_file);
        am_message (AM_STATUS,message);
        }
    for (j = 0; j < nrows; j += grid_display->row_int)
        {
        y = grid_user.ymin + (j * yint);
        if (y >= map->lower_left_xy.y AND y <= map->upper_right_xy.y)
            {
            ypost = y + yoffset;
            for (i = 0; i < ncols; i += grid_display->col_int)
                {
                x = grid_user.xmin + (i * xint);
                if (x >= map->lower_left_xy.x AND x <= map->upper_right_xy.x)
                    {
                    if (NOT grid_display->suppress_symbols AND NOT no_symbol)
                        {
                        sub_x[sub_npts] = x;
                        sub_y[sub_npts] = y;
                        sub_npts++;
                        }
                    if (NOT grid_display->suppress_text)
                        {
                        k = (j * ncols) + i;
                        if (grid_z[k] != znull)
                            {
                            xpost = x + xoffset;

			    /* ming - 8/9/91 */
			    /* change from default 6 demical places to 2 */
			    /* ----------------------------------------- */
                            sprintf (label, "%.2f", grid_z[k]);

                            if (xpost >= map->lower_left_xy.x 
		                AND xpost <= map->upper_right_xy.x 
				AND ypost >= map->lower_left_xy.y 
				AND ypost <= map->upper_right_xy.y)
				{
                                wm_gtx (xpost, ypost, label);
			        }
                            }
                        }
                    }
                }
            }
        }
    if (NOT grid_display->suppress_symbols AND NOT no_symbol)
        {
        wm_gpm (sub_npts, sub_x, sub_y);
        }
    wmsclose (seg_id);
    
finished:
    am_free (grid_z);
    am_free (sub_x);
    am_free (sub_y);
    return status;
    }
/* END:     */
