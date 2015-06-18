/****************************************************************************** */
/*                                                                              */
/*                Copyright Exploration Systems, Inc. 1990                      */
/*                    Unpublished -- All rights reserved                        */
/*                                                                              */
/*THIS SOFTWARE IS THE PROPRIETARY PROPERTY OF Finder Graphics Systems, Inc. AND*/
/*MAY CONTAIN CONFIDENTIAL TRADE SECRET INFORMATION. IT IS LICENSED FOR USE ON  */
/*THE DESIGNATED EQUIPMENT ON WHICH IT WAS ORIGINALLY INSTALLED AND  MAY NOT BE */
/*MODIFIED, DUPLICATED OR COPIED IN ANY FORM WITHOUT PRIOR WRITTEN CONSENT OF   */
/*                                                                              */
/*                            Finder Graphics Systems                           */
/*                            201 Tamal Vista Blvd.                             */
/*                            Corte Madera, CA  94925                           */
/*                            (415) 927-0100                                    */
/*                                                                              */
/*(The above notice does not constitute an admission of publication or          */
/*unrestricted dissemination of the work)                                       */
/*                                                                              */
/****************************************************************************** */
/* * FILE DESCRIPTION *                                                         */
/* --------------------                                                         */
/* ************************************************************************     */
/*                                                                              */
/*   NAME: vi_restore_grid                                                      */
/*                                                                              */
/*   AUTHOR: D.A. VOSSLER                                                       */
/*                                                                              */
/*   DESCRIPTION: Restore a grid from disk to memory.                           */
/*                                                                              */
/*   ************************************************************************   */


/* * INCLUDE FILES *                                                            */
/* -----------------                                                            */

#include "esi_c_lib.h"
#include "esi_co.h"

#ifndef ESI_DG_DEFS_H
#include "esi_dg_defs.h"
#endif

#ifndef ESI_CO_ERR_H
#include "esi_co_err.h"
#endif

#ifndef ESI_CO_MSG_H
#include "esi_co_msg.h"
#endif

#ifndef ESI_OV_ERR_H
#include "esi_ov_err.h"
#endif

#ifndef ESI_FM_H
#include "esi_fm.h"
#endif

#ifndef ESI_FM_ERR_H
#include "esi_fm_err.h"
#endif

#ifndef ESI_NL_H
#include "esi_nl.h"
#endif

#ifndef ESI_TS_H
#include "esi_ts.h"
#endif

#ifndef ESI_AM_H
#include "esi_am.h"
#endif

#ifndef ESI_MG_H
#include "esi_mg.h"
#endif

#ifdef PROTOTYPES
publicdef INT vi_restore_grid (FM_FILE gridname, DOUBLE *znull, DOUBLE *xmin,
                               DOUBLE *xmax, DOUBLE *ymin, DOUBLE *ymax, 
                               INT *nrows, INT *ncols, FLOAT **grid_z)
#else
publicdef INT vi_restore_grid (gridname, znull, xmin, xmax, ymin, ymax, 
                               nrows, ncols, grid_z) 

/* * ARGUMENT LIST * */
/* ----------------- */

FM_FILE gridname;                   /* GRID NAME                            */
DOUBLE *znull;                      /* NULL VALUE FOR GRID                  */
DOUBLE *xmin;                       /* MINIMUM GRID X VALUE                 */
DOUBLE *xmax;                       /* MAXIMUM GRID X VALUE                 */
DOUBLE *ymin;                       /* MINIMUM GRID Y VALUE                 */
DOUBLE *ymax;                       /* MAXIMUM GRID Y VALUE                 */
INT *nrows;                         /* NUMBER OF GRID ROWS                  */
INT *ncols;                         /* NUMBER OF GRID COLUMNS               */
FLOAT **grid_z;                     /* GRIDDED Z VALUES                     */
                                    /* allocation done here */
                                    /* use am_free to release */
#endif

/* * VARIABLE LIST *                                                        */
/* -----------------                                                        */
    {
    
    INT status;                     /* RETURN STATUS                        */
    FLOAT *dummy;                   /* DUMMY FLOAT POINTER                  */
    FLOAT temp;
    FILENAME os_name;               /* GRID FILE NAME                       */
    INT i;                          /* DUMMY INDEX                          */
    UINT j;                         /* DUMMY INDEX                          */
    NLIST_HEADER grid_nlist = (NLIST_HEADER)0;  /* NLIST TO STORE GRID_Z    */
    GRID_NLIST_USER_STRUCT grid_user;   /* NLIST USER STRUCT FOR CONTOURS   */
    FM_DESCRIB desc;
    
                                    /* NLIST INFORMATION                    */
    UINT d_list[1];                 /*      DIM LIST                        */
    VOIDPTR v_list[1];              /*      VALUE LIST                      */
    
    /* **********************************************************************/
    
    status = SUCCESS;
    
    /* PRE-PROCESSING FOR GRID RESTORE */
    /* ------------------------------- */
    
    strcpy (desc, "");
    if ((status = fm_init_file ("ESI$GRIDS", gridname, "GRID", "r", -1, desc,
                                NULL_FUNCTION_PTR, os_name, "MAPPING")) EQUALS SUCCESS)
        {
        status = (nl_read_nlist (&grid_nlist, os_name,
                                 "GRIDS") >= 0) ? SUCCESS : CO_GRID_FILE_ERROR;
        fm_release_file ("ESI$GRIDS", gridname, "GRID");
        }
    if (status != SUCCESS)
        {
        goto finished;
        }

    nl_inq_nlist_user_struct (grid_nlist, sizeof(GRID_NLIST_USER_STRUCT),
                              (VOIDPTR)&grid_user, (size_t *)&i);
    
    /* GET ROW AND COL INFORMATION */
    /* --------------------------- */   
    nl_inq_nlist_int (grid_nlist, NL_NLIST_NSTROKES, nrows);
    nl_inq_stroke_int (grid_nlist, 1, NL_STROKE_NPOINTS, ncols);
    
    /* ALLOCATE AND FILL GRID */
    /* ---------------------- */    
    *grid_z = (FLOAT *) am_allocate (AM_APPLICATION,
                                     (*ncols * (*nrows)) * sizeof(FLOAT));
    dummy = *grid_z;
    d_list[0] = 1;
    for (i = 1; i < *nrows + 1; i++)
        {
        nl_set_current_point (grid_nlist, 1, i);
        v_list[0] = (VOIDPTR)dummy;
        nl_get_points (grid_nlist, *ncols, 1, d_list, v_list, &j);
        dummy += *ncols;
        }

    /* GET NULL VALUE */
    /* -------------- */    
    d_list[0] = 1;
    v_list[0] = (VOIDPTR)&temp;
    nl_inq_nlist_info (grid_nlist, NL_NLIST_NULL_VALUE, 1, d_list, v_list);
    *znull = temp;
    
    /* Return x,y min & max */
    /* -------------------- */
    *xmin = grid_user.xmin;
    *xmax = grid_user.xmax;
    *ymin = grid_user.ymin;
    *ymax = grid_user.ymax;
    
    /* * FREE UP ALLOCATED MEMORY * */
    /* ---------------------------- */
    
finished:
    nl_free_nlist (grid_nlist);
    
    return status;
    }
