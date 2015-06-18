/* DEC/CMS REPLACEMENT HISTORY, Element CO_MAKE_GRID.C*/
/* *10   23-OCT-1991 15:11:17 JULIAN "(SPR 0) add initialization to prev mods"*/
/* *9    24-SEP-1991 11:39:52 JULIAN "(SPR 4149) free unused allocated memory"*/
/* *8    11-MAR-1991 10:11:24 KEE "(SPR -1) Modify Charlie Purmoy's Trend and Residual base on QA new requirements"*/
/* *7    20-NOV-1990 09:23:52 MING "(SPR 6173) close the .RPT file"*/
/* *6    14-AUG-1990 13:05:35 VINCE "(SPR 5644) Header Standardization"*/
/* *5     6-FEB-1990 10:23:47 MING "(SPR 0) change date/time declaration to new data type DATE_TIME"*/
/* *4    24-JAN-1990 16:15:56 WALTERS "(SPR 0) Fix bug in nl_set_nlist_info call"*/
/* *3    17-JAN-1990 15:29:31 WALTERS "(SPR 0) Add nl_init_nlist_info to name dimensions"*/
/* *2    14-SEP-1989 18:13:11 GORDON "(SPR -1) checkin from gulf"*/
/* *1    19-JUN-1989 12:43:20 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element CO_MAKE_GRID.C*/
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

/* Function Description -----------------------------------------------------
Description:
    Function to create the grid file from the supplied XYZ file.

Prototype:
    publicdef INT co_make_grid(CONTOURS *contours);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) contours    -(CONTOURS *) Contour information structure.
    
Return Value/Status:
    SUCCESS - Successful completion
    CO_CANT_OPEN_GRID - Can't open n-list grid file.
    CO_GRID_TOO_DENSE - Returned if am_allocate returns NUL (i.e. no space
        for grid.
    CO_NO_SCATTER_DATA - Returned if no scatter points are passed.
    Function returns status from nl_write_nlist, nl_add_points (+/-), 
        nl_set_nlist_user_struct (+/-), co_grid, nl_set_nlist_info,
        nl_init_nlist.
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT co_make_grid (CONTOURS *contours) 
#else
publicdef INT co_make_grid (contours)
CONTOURS *contours;
#endif
/* * VARIABLE LIST * */
/* ----------------- */
{
    
  INT status;                     /* RETURN STATUS */
  NLIST_HEADER scatter;           /* SCATTER FILE */
  FLOAT *scat_x = (FLOAT *)0;     /* SCATTER FILE X COORDS */
  FLOAT *scat_y = (FLOAT *)0;     /* SCATTER FILE Y COORDS */
  FLOAT *scat_z;                  /* SCATTER FILE Z COORDS */
  INT scat_npts;                  /* NUMBER OF SCATTER POINTS */
  FLOAT *grid_z = (FLOAT *)0;     /* GRIDDED Z VALUES */
  INT nrows;                      /* NUMBER OF GRID ROWS */
  INT ncols;                      /* NUMBER OF GRID COLUMNS */
  FLOAT temp;
  DOUBLE znull;                   /* NULL VALUE FOR GRID */
  DOUBLE ztol;                    /* TOLERANCE FOR NULL VALUE */
  FLOAT *dummy = (FLOAT *)0;      /* DUMMY FLOAT POINTER */
  DOUBLE xmin;                    /* MINIMUM GRID X VALUE */
  DOUBLE xmax;                    /* MAXIMUM GRID X VALUE */
  DOUBLE ymin;                    /* MINIMUM GRID Y VALUE */
  DOUBLE ymax;                    /* MAXIMUM GRID Y VALUE */
  DOUBLE grid_min;                /* MINIMUM ALLOWABLE GRID VALUE */
  DOUBLE grid_max;                /* MAXIMUM ALLOWABLE GRID VALUE */
  DATE_TIME date;
  FM_FILE gridname;               /* MAP NAME */
  FILENAME os_name;               /* MAP NAME */
  FILENAME report_file;           /* MAP NAME */
  INT i;                          /* DUMMY INDEX */
  UINT j;                         /* DUMMY INDEX */
  NLIST_HEADER grid_nlist = (NLIST_HEADER)0;  /* NLIST TO STORE GRID_Z */
  GRID_NLIST_USER_STRUCT grid_user;   /* GRID NLIST USER STRUCT */
  MAP_STRUCTURE *map;             /* MAP INFORMATION STRUCTURE */
  INT iflglg = 1;
  INT iflgrp = 1;
  INT funit;
  INT imxline = 0;
  BOOL perm = FALSE;
  CHAR desc[241];
  /* NLIST INFORMATION */
  INT units[1];                   /*      UNITS */
  UINT d_list[1];                 /*      DIM LIST */
  size_t d_size[1];               /*      DIM SIZE IN BYTES */
  INT d_type[1];                  /*      DIM TYPE */
  INT v_type[1];                  /*      VALUE TYPE */
  VOIDPTR v_list[1];              /*      VALUE LIST */
  static CHAR *c_name[2] = 
    {
      "Z",
      NULL
      };
  FLOAT *polynom = (FLOAT *)0;
  FLOAT *wrk_array = (FLOAT *)0;
  FLOAT *new_scat_x = (FLOAT *)0;
  FLOAT *new_scat_y = (FLOAT *)0;
  FLOAT *new_scat_z = (FLOAT *)0;
  FLOAT total_scat_z;
  FLOAT average_scat_z;
  INT polysize = 16;
  INT working_size;
  INT ret_flag;
  INT config_check = 1;
  DOUBLE divisor;
  FLOAT *hold_x, *hold_y, *hold_z;
  DOUBLE xrange, yrange;

  /* ********************************************************************** */
    
  status = SUCCESS;
  report_file[0] = 0;
  
  mp_get_current_map (&map);
    
  /* PRE-PROCESSING FOR GRID CONSTRUCTION * */
  /* -------------------------------------- */
    
  /* GET SCATTER POINTS */
  
  status = ov_open_scatter (&scatter, contours->xyz_file, "r", os_name);
  if (status < 0)
    {
      goto finished;
    }
  ov_read_scatter (scatter, &scat_x, &scat_y, &scat_z, &scat_npts);
  ov_get_scatter_horizons (scatter, grid_user.base_surface, grid_user.top_surface);
  ov_close_scatter (scatter, contours->xyz_file, "r", os_name);
  if (scat_npts EQUALS 0)
    {
      status = CO_NO_SCATTER_DATA;
      goto finished;
    }
  /* CALCULATE GRID CELL INFORMATION */
  
  if (contours->grid_interval EQUALS NULL)
    {
      co_grid_size (contours, scat_x, scat_y, scat_npts);
    }
  co_clc_grid_cells (contours, &ncols, &xmin, &xmax, &nrows, &ymin, &ymax);
  
  /* ALLOCATE SPACE FOR GRID */
  
  grid_z = (FLOAT *)am_allocate (AM_APPLICATION, (ncols * nrows) * sizeof(FLOAT));
  if (grid_z == (FLOAT *)0)
    {
      status = CO_GRID_TOO_DENSE;
      goto finished;
    }
  /* INITIALIZE GRID NLIST */
    
  units[0] = 0;
  d_size[0] = sizeof(FLOAT);
  d_type[0] = FLOAT_TYPE;
  v_type[0] = VALUE_LIST;
  d_list[0] = 1;
  
  status = nl_init_nlist (&grid_nlist, 1, units, d_size, d_type, v_type, ncols);
  if (status IS_NOT_EQUAL_TO SUCCESS)
    {
      return status;
    }
  status = nl_set_nlist_info (grid_nlist, NL_NLIST_COLUMN_NAME, 1, d_list,
			      (VOIDPTR)c_name);
  if (status IS_NOT_EQUAL_TO SUCCESS)
    {
      return status;
    }
  /* GET NULL VALUE */
  
  znull = -2000000000.0;
  temp = znull;
  v_list[0] = (VOIDPTR) & temp;
  d_list[0] = 1;
  nl_set_nlist_info (grid_nlist, NL_NLIST_NULL_VALUE, 1, d_list, v_list);
  ztol = fabs (znull * 0.00001);
    
    
  /* * GRID CONSTRUCTION * */
  /* --------------------- */
    
  /* OPEN REPORT FILE */
    
  qs_inq_c (QS_FINDER_ACCOUNT, report_file, (INT *)0);
  /* 12/13/88 GS - make the report file name unique */
  sprintf (report_file, "%s_%03d", report_file, ho_user_number ());
  if (contours->grid_type == TREND_TYPE)
    strcat(report_file,"_TREND.RPT");
  else if (contours->grid_type == RESIDUAL_TYPE)
    strcat(report_file,"_RESID.RPT");
  else
    strcat (report_file, "_GRID.RPT");
  ho_add_path ("ESI$REPORTS", report_file, sizeof(FILENAME), report_file);
  co_new_file_gc (report_file, &funit, &status);
  if (contours->grid_type == TREND_TYPE OR
      contours->grid_type == RESIDUAL_TYPE)
    {
      co_settun_gc(funit,3);
      co_bgntrd_gc();
      new_scat_x = (FLOAT *) tc_alloc(sizeof(FLOAT) * scat_npts);
      new_scat_y = (FLOAT *) tc_alloc(sizeof(FLOAT) * scat_npts);
      
      xrange = xmax-xmin;
      yrange = ymax-ymin;
      divisor = (xrange > yrange) ? xrange : yrange;

      for (i=0; i < scat_npts; i++)
        {
	  new_scat_x[i] = (scat_x[i] - xmin) / divisor;
	  new_scat_y[i] = (scat_y[i] - ymin) / divisor;
        }
      
#if 0
      status = co_rescale_xy( scat_x, xmin, xmax,
			     scat_y, ymin, ymax, scat_npts,
			     new_scat_x, new_scat_y, divisor);
      tc_free (scat_x);
      tc_free (scat_y);
#endif
      hold_x = scat_x;
      hold_y = scat_y;
      scat_x = new_scat_x;
      scat_y = new_scat_y;
      xrange = (xmax-xmin)/divisor;
      yrange = (ymax-ymin)/divisor;
      
      co_setpgd_gc(ncols,0.0,xrange,nrows,0.0,yrange);
      
      status = co_clcidw_gc(contours->trend_or_resid_order,
			    scat_npts, &working_size);
      working_size *= 5; /* make extra work area */
      wrk_array = (FLOAT *) tc_alloc(sizeof(FLOAT) * working_size);
      polynom = (FLOAT *) tc_alloc(sizeof(FLOAT) * polysize * polysize);
      /* add null check */
      status = co_clctrd_gc( new_scat_x, new_scat_y, scat_z, scat_npts,
			    polynom, polysize, contours->trend_or_resid_order,
			    config_check, wrk_array,
			    working_size, &ret_flag);
      
      if (contours->grid_type == TREND_TYPE )
        {
	  status = co_clcpgd_gc( polynom, polysize,
                                contours->trend_or_resid_order,
                                grid_z, ncols, nrows);
        }
      else if (contours->grid_type == RESIDUAL_TYPE)
	{
	  new_scat_z = (FLOAT *) tc_alloc(sizeof(FLOAT) * scat_npts);
	  if (contours->trend_or_resid_order == 0)
	    {
	      for (i=0, total_scat_z=0.0; i < scat_npts; i++)
		{
		  total_scat_z += scat_z[i];
		}
	      average_scat_z = total_scat_z / scat_npts;
	      for (i=0; i < scat_npts; i++)
		{
		  new_scat_z[i] = scat_z[i] - average_scat_z;
		}
	    }
	  else
	    {
	      status = co_clcres_gc( scat_x, scat_y, scat_z, scat_npts,
				    polynom, polysize, 
				    contours->trend_or_resid_order,
				    new_scat_z );
	    }
#if 0
	  tc_free(scat_z);
#endif
	  hold_z = scat_z;
	  scat_z = new_scat_z;
        }
      scat_x = hold_x;
      scat_y = hold_y;
      
      co_endtrd_gc();
      if (contours->grid_type == TREND_TYPE)
	{
	  tfg_close (&funit);
	}
    }


  /* START DGI GRIDDING */
  if (contours->grid_type == SURFACE_TYPE OR
      contours->grid_type == RESIDUAL_TYPE)
    {
      /* START DGI GRIDDING */
      
      co_bgngrd_gc ();
      
      co_setglg_gc (iflglg, funit);
      co_setgrp_gc (iflgrp, funit, imxline);
      
      /* SET GRID CELL INFORMATION */
      
      co_setgrd_gc (ncols, xmin, xmax, nrows, ymin, ymax);
      
      /* SET GRID NULL VALUE */
    
      co_setgnl_gc (1, znull, ztol);
    
      /* SET GRID LIMIT CONSTRAINTS */
    
      if (contours->grid_minimum != NULL OR contours->grid_maximum != NULL)
        {
	  co_get_grid_limits (contours, scat_z, scat_npts, &grid_min, &grid_max);
	  co_setglm_gc (1, grid_min, grid_max);
        }

      /* additional calls which may be helpful are:
	 1) setgft which allows more control of the estimate of grid nodes-
	 this technique weights unevenly the two estimates
	 (grid neighbor vs. data points and grid neighbors)
	 2) setmdp is the number of nearest neighbors
	 3) setpnt (and usrsdp) used in clcfht (clchgt) and fltmap to improve
	 output by including the data points in estimates
	 4) setxya controls harmonic vs bi-harmonic smoothing
	 5) setxyw provides unequal weights for x and y directions */
      
      /* GRID THE SCATTER DATA */
    
      status = co_grid (contours, scat_x, scat_y, scat_z, scat_npts, nrows, ncols,
			grid_z);
      
      /* ONLY WANT GRID ON POLYGON */
      
      if (status >= 0 AND contours->bounded_by_data)
        {
	  co_polygon (znull, scat_x, scat_y, scat_z, scat_npts, nrows, ncols, xmin, xmax,
		      ymin, ymax, grid_z);
        }
      /* END DGI GRIDDING */
    
      co_endgrd_gc ();
      tfg_close (&funit);
    }
		
  if (status < 0)
    {
      goto finished;
    }
  /* * STORE GRID IN NLIST (EACH ROW IS A STROKE) * */
  /* ---------------------------------------------- */
  
  strcpy (grid_user.projection, map->map_projection);
  strcpy (grid_user.coord_system, map->map_coordinate_system);
  grid_user.xmin = xmin;
  grid_user.xmax = xmax;
  grid_user.ymin = ymin;
  grid_user.ymax = ymax;
  if (contours->faulted != NULL)
    {
      strcpy (grid_user.fault, contours->faulted);
    }
  else
    {
      strcpy (grid_user.fault, "");
    }
  grid_user.uniform = TRUE;
  status = nl_set_nlist_user_struct (grid_nlist, &grid_user,
				     sizeof(GRID_NLIST_USER_STRUCT));
  if (status < 0)
    {
      goto finished;
    }
  dummy = grid_z;
  for (i = 0; i < nrows; i++)
    {
      status = nl_start_stroke (grid_nlist, &j);
      if (status < 0)
	{
	  goto finished;
	}
      v_list[0] = (VOIDPTR)dummy;
      status = nl_add_points (grid_nlist, ncols, 1, d_list, v_list);
      if (status < 0)
	{
	  goto finished;
	}
      dummy += ncols;
    }
  /* * GET NLIST FILE NAME AND WRITE NLIST * */
  /* --------------------------------------- */
    
  if (contours->grid_file EQUALS NULL)
    {
      ho_date_tag (date);
      sprintf (gridname, "ZZZ_GRID %s", date);
      contours->grid_file = (CHAR *)am_allocate (AM_APPLICATION,
						 strlen (gridname) + 1);
      strcpy (contours->grid_file, gridname);
      perm = FALSE;
    }
  else
    {
      strcpy (gridname, contours->grid_file);
      perm = TRUE;
    }
  desc[0] = '\0';
  if (fm_init_file ("ESI$GRIDS", gridname, "GRID", "w", perm, desc,
		    fm_grid_term_func, os_name, "MAPPING") EQUALS SUCCESS)
    {
      nl_delete_nlist (os_name, "GRIDS");
      status = nl_write_nlist (grid_nlist, os_name, "GRIDS");
      fm_release_file ("ESI$GRIDS", gridname, "GRID");
      
      sprintf (desc, mg_message (CO_CREATE_FROM_XYZ), contours->xyz_file);
      fm_add_history_record ("ESI$GRIDS", gridname, "GRID", "ESI$SCATTERS",
			     contours->xyz_file, "SCATTER", desc);
    }
  else
    {
      status = CO_CANT_OPEN_GRID;
    }
  /* * FREE UP ALLOCATED MEMORY * */
  /* ---------------------------- */
  
finished:
  am_free (grid_z);
  nl_free_nlist (grid_nlist);
  am_free (scat_x);
  am_free (scat_y);

  /* FREE allocated memory (case 4149) */
  if(new_scat_x != (FLOAT *)0)
    tc_free (new_scat_x);
  if(new_scat_y != (FLOAT *)0)
    tc_free (new_scat_y);
  if(new_scat_z != (FLOAT *)0)
    tc_free (new_scat_z);

  if(wrk_array != (FLOAT *)0)
    tc_free (wrk_array);
  if(polynom != (FLOAT *)0)
    tc_free (polynom);

  if (report_file[0] != 0 && hoexists(report_file) == TRUE)
    {
      ho_delete_file (report_file, &funit);
    }
    
  return status;
}
/* END:     */
