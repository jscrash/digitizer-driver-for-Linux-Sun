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
/*   NAME: VI_UPDATE_GRID.C                                                     */
/*                                                                              */
/*   AUTHOR: D.A. VOSSLER                                                       */
/*                                                                              */
/*   DESCRIPTION: Store an updated version of a grid in an existing nlist.      */
/*                                                                              */
/*   Modified History:                                                          */
/*   Hung Hoang - BHPP Melbourne - Aug/1st/1991 -                               */
/*        Modified to accept the output gridname from the user interface,       */
/*        The program will fail if the ouput gridname has been selected.        */
/*                                                                              */
/*   ************************************************************************   */


/* * INCLUDE FILES *                                                            */
/* -----------------                                                            */
#include "esi_c_lib.h"
#include "esi_vi.h"

#ifndef ESI_MATH_H
#include "esi_math.h"
#endif

#ifndef ESI_C_LIB_H
#include "esi_c_lib.h"
#endif

#include "esi_co.h"

#ifndef ESI_CO_ERR_H
#include "esi_co_err.h"
#endif

#ifndef ESI_CO_MSG_H
#include "esi_co_msg.h"
#endif

#ifndef ESI_MG_H
#include "esi_mg.h"
#endif

#ifndef ESI_MP_H
#include "esi_mp.h"
#endif

#ifndef ESI_FM_H
#include "esi_fm.h"
#endif

#ifndef ESI_DG_DEFS_H
#include "esi_dg_defs.h"
#endif

#ifndef ESI_AM_H
#include "esi_am.h"
#endif

#ifndef ESI_NL_H
#include "esi_nl.h"
#endif

#ifndef ESI_QS_H
#include "esi_qs.h"
#endif

#ifndef ESI_HO_H
#include "esi_ho.h"
#endif

#ifndef ESI_OV_H
#include "esi_ov.h"
#endif

#ifndef ESI_HO_DESC_H
#include "esi_ho_desc.h"
#endif

#ifndef ESI_HO_FILES_H
#include "esi_ho_files.h"
#endif

/* * FUNCTION TITLE *                                                       */
/* ------------------                                                       */
#if USE_PROTOTYPES
publicdef INT vi_update_grid (FM_FILE gridname, FM_FILE gridname_out, 
                              BOOL ren_flag, FLOAT *grid_z)
#else
publicdef INT vi_update_grid (gridname, gridname_out, ren_flag, grid_z)

/* * ARGUMENT LIST * */
/* ----------------- */

FM_FILE gridname;                   /* GRID NAME                            */
FM_FILE gridname_out;               /* GRID NAME OUT                        */
BOOL ren_flag;			    /* Rename the grid flag                 */
FLOAT *grid_z;                      /* GRIDDED Z VALUES                     */
#endif
    {
    
    /* * VARIABLE LIST * */
    /* ----------------- */
    
    INT status;                     /* RETURN STATUS                        */
    FLOAT *dummy = (FLOAT *)0;      /* DUMMY FLOAT POINTER                  */
    FILENAME os_name;               /* INTERNAL NAME                        */
    INT i;                          /* DUMMY INDEX                          */
    NLIST_HEADER grid_nlist = (NLIST_HEADER)0;  /* NLIST TO STORE GRID_Z    */
    GRID_NLIST_USER_STRUCT grid_user;   /* GRID NLIST USER STRUCT           */
    BOOL perm = TRUE;
    CHAR desc[241];
    /* NLIST INFORMATION                                                    */
    INT units[1];                   /*      UNITS                           */
    UINT d_list[1];                 /*      DIM LIST                        */
    size_t d_size[1];               /*      DIM SIZE IN BYTES               */
    INT d_type[1];                  /*      DIM TYPE                        */
    INT v_type[1];                  /*      VALUE TYPE                      */
    VOIDPTR v_list[1];              /*      VALUE LIST                      */
    static CHAR *c_name[2] = 
        {
        "Z", NULL
        };
    INT nrows;                      /* No. rows in grid                     */
    INT ncols;                      /* No. cols in grid                     */
    
    /* **********************************************************************/
    
    status = SUCCESS;
    
    /* PRE-PROCESSING FOR GRID INPUT */
    /* ----------------------------- */
    
    strcpy (desc, "");
    if ((status = fm_init_file ("ESI$GRIDS", gridname, "GRID", "r", -1, desc,
                                NULL_FUNCTION_PTR, os_name, "MAPPING")) EQUALS SUCCESS)
        {
        status = (nl_read_nlist (&grid_nlist, os_name,
                                 "GRIDS") >= 0) ? SUCCESS : CO_GRID_FILE_ERROR;
        }

    if (status != SUCCESS)
        {
        goto finished;
        }

    nl_inq_nlist_user_struct (grid_nlist, sizeof(GRID_NLIST_USER_STRUCT),
                              (VOIDPTR)&grid_user, (size_t *)&i);
    
    /* GET ROW AND COL INFORMATION */
    /* --------------------------- */    
    nl_inq_nlist_int (grid_nlist, NL_NLIST_NSTROKES, &nrows);
    nl_inq_stroke_int (grid_nlist, 1, NL_STROKE_NPOINTS, &ncols);
    
    /* INITIALIZE GRID NLIST PARMS */
    /* --------------------------- */    
    units[0] = 0;
    d_size[0] = sizeof(FLOAT);
    d_type[0] = FLOAT_TYPE;
    v_type[0] = VALUE_LIST;
    d_list[0] = 1;
    
    /* FILL UPDATED GRID VALUES */
    /* ------------------------ */   
    dummy = grid_z;
    for (i = 1; i < nrows + 1; i++)
        {
        nl_set_current_point (grid_nlist, 1, i);
        v_list[0] = (VOIDPTR)dummy;
        status = nl_replace_rows_convert (grid_nlist, ncols, 1, d_list, v_list, units,
                                          d_size, d_type);
        dummy += ncols;
        }

    /** HH - BHPP - Aug/1st 1991	**/

    if (ren_flag)
       {
       strcpy (desc, "");
       strcpy (gridname, gridname_out);
       if ((status = fm_init_file ("ESI$GRIDS", gridname, "GRID", "w", -1, desc,
                                NULL_FUNCTION_PTR, os_name, "MAPPING")) != SUCCESS)
          {
          fprintf(stderr,"Problem from 'fm_init_file' new output gridname:status = %d\n",status);
          }
      } 
       
    /* * GET NLIST FILE NAME AND WRITE NLIST * */
    /* --------------------------------------- */
    
       nl_delete_nlist (os_name, "GRIDS");
       status = nl_write_nlist (grid_nlist, os_name, "GRIDS");
       fm_release_file ("ESI$GRIDS", gridname, "GRID");


    /* * FREE UP ALLOCATED MEMORY * */
    /* ---------------------------- */
    
finished:
    nl_free_nlist (grid_nlist);
    
    return status;
    }
