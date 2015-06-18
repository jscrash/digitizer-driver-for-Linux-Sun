/* DEC/CMS REPLACEMENT HISTORY, Element PL_BATCH_PLOT.C */
/* *8    19-JUN-1991 14:18:49 MING "(SPR 0) the data file is passed with translated path now, fix for the change" */
/* *7    18-JUN-1991 16:58:31 MING "(SPR 0) fix file name in copy system command in neutral plot file plotting" */
/* *6    17-JUN-1991 16:52:43 MING "(SPR 0) a work round for passing file name when producing neutral plot file" */
/* *5    16-JUN-1991 17:56:19 MING "(SPR 0) put processing dummy file after pf_init_pf" */
/* *4     6-JUN-1991 17:00:26 MING "(SPR 0) new plot file managment" */
/* *3    30-NOV-1990 17:38:43 MING "(SPR -1) add esi_sl_meth.ah" */
/* *2    15-OCT-1990 12:17:55 GILLESPIE "(SPR 1) Clean up from Ernie checkin" */
/* *1    12-OCT-1990 17:34:27 GILLESPIE "Merge Ernie Deltas" */
/* DEC/CMS REPLACEMENT HISTORY, Element PL_BATCH_PLOT.C */
/******************************************************************************

                Copyright Finder Graphics Systems, Inc. 1990
		   Unpublished -- All rights reserved

THIS SOFTWARE IS THE PROPRIETARY PROPERTY OF Finder Graphics Systems, Inc.  AND
MAY CONTAIN CONFIDENTIAL TRADE SECRET INFORMATION. IT IS LICENSED FOR USE ON THE
DESIGNATED EQUIPMENT ON WHICH IT WAS ORIGINALLY INSTALLED AND  MAY NOT BE
MODIFIED, DUPLICATED OR COPIED IN ANY FORM WITHOUT PRIOR WRITTEN CONSENT OF

			Finder Graphics Systems, Inc.
			201 Tamal Vista Blvd
			Corte Madera, CA  USA 94925
			(415) 927-0100

(The above notice does not constitute an admission of publication or
unrestricted dissemination of the work)

******************************************************************************/



/* Function Description -----------------------------------------------------
Description: Plotting main program.

Prototype:
    publicdef INT main (argc,argv)

Parameters:	    (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) arg	- description

Return Value/Status:
    SUCCESS	- Successfull completion
    FAIL	- Unsuccessfull completion

Scope:
    PUBLIC

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

#include "esi_c_lib.h"

#ifndef ESI_PL_H
#include "esi_pl.h"
#endif 

#ifndef ESI_OR_H
#include "esi_or.h"
#endif

#ifndef ESI_QS_H
#include "esi_qs.h"
#endif

#ifndef ESI_AM_H
#include "esi_am.h"
#endif

#ifndef ESI_HO_FILES_H
#include "esi_ho_files.h"
#endif

#ifndef ESI_TU_H
#include "esi_tu.h"
#endif

#ifndef ESI_PF_H
#include "esi_pf.h"
#endif

#ifndef ESI_MP_H
#include "esi_mp.h"
#endif

#ifndef ESI_TS_H
#include "esi_ts.h"
#endif

#ifndef ESI_SL_METH_AH
#include "esi_sl_meth.ah"
#endif

#if USE_PROTOTYPES
publicdef INT main (INT argc, CHAR **argv)
#else
publicdef INT main (argc,argv)
INT argc;
CHAR **argv;
#endif
{
  INT status = SUCCESS;
  PATHNAME parameter_file;
  PATHNAME dummy;
  PATHNAME tmp_npf;
  FILENAME tmp_fname;
  DOUBLE vdc_extent_xmin, vdc_extent_ymin, vdc_extent_xmax,vdc_extent_ymax;
  DOUBLE vdc_viewport_xmin, vdc_viewport_ymin, vdc_viewport_xmax, vdc_viewport_ymax;
  DOUBLE inch_to_mm;
  FILE *fd;
  CHAR *pdot;
  CHAR *misc;
  CHAR token[256];
  PL_BASE_WINDOW bw;         /* base window */
  PLOT_LAYOUT pl;            /* layout */
  PF_INFO pf_info;
  TCP tmp_tcp = (TCP) 0;
  MAP_STRUCTURE *map;
  CHAR command[240];

/* ********************************************************************** */


/* ------------------------ */
/* * CHECK ARGUMENT LIST  * */
/* ------------------------ */
  if (argc != 9)
    {
    printf("\nERROR: WRONG NUMBER OF PARAMETERS (%d) PASSED!",argc);
    exit (EXIT_FAILURE);
    }


/* ------------------- */
/* * LOGIN TO ORACLE * */
/* ------------------- */
  status = or_login (2,argv);
  if (status < 0)
    {
    printf ("\nORACLE LOGIN ERROR: status = %d",status);
    exit (EXIT_FAILURE);
    }

/* ----------------- */
/*   initailization  */
/* ----------------- */
  qs_set_i (QS_IF_BATCH,TRUE);

  qs_set_c (QS_PROJECT_NAME,argv[2],0);

  qs_set_c (QS_WORKSTATION_TYPE,"METAFILE",0);

  status = am_initialize ();

  status = finder_config_project (FALSE);

  tc_zeroes (&bw,sizeof(PL_BASE_WINDOW));

  strcpy (pf_info.layout,argv[3]);
  bw.type = atoi (argv[4]);
  strcpy (pf_info.filename,argv[5]);
  ts_sto_upper (pf_info.filename);
  strcpy (pf_info.meta_type,argv[6]);
  ts_sto_upper (pf_info.meta_type);
  strcpy (bw.file,argv[7]);
  misc = argv[8];
  pdot = ts_next_token (misc,token,'@');
  pf_info.save = atoi (token);
  pdot = ts_next_token (pdot,pf_info.expired_date,'@');
  strcpy (pf_info.remark,pdot);

  switch (bw.type)
    {
    case PL_MAP:
      strcpy (pf_info.app,"MAPPING");
      break;
    case PL_XSECT:
      strcpy (pf_info.app,"XSECT");
      break;
    }

  /* -------------------- */
  /* initialize plot file */
  /* -------------------- */
  status = pf_init_plotfile (&pf_info);

  /* ---------------------------------------------------- */
  /* open a dummy file to store the plot file os pathname */
  /* since the name is not known until this executable    */
  /* is run. so in MP_PLOT_MAP or XS_LAUNCH_PLOTS need a  */
  /* way to recognize the pathname.                       */
  /* ---------------------------------------------------- */
  strcpy (dummy,bw.file);
  pdot = strrchr (dummy,'.') + 1;
  strcpy (pdot,"DUMMY");

  fd = ho_open (dummy,"w",NULL);
  if (fd == NULL_FILE_PTR)
      {
      am_message (AM_STATUS,"Cannot open dummy file");
      exit (EXIT_FAILURE);
      }

  fprintf (fd,"%s",pf_info.pathname);
  fclose (fd);
  
/* ---------------*/
/* parameter file */
/* ---------------*/
  sprintf (parameter_file,"%s.PAR",pf_info.pathname);

/* ----------------------- */
/* init well symbol legend */
/* ----------------------- */
  pl_init_well_symbol ();

/* ---------------- */
/* read base window */
/* ---------------- */
  status = pl_read_base_win (&bw,&pf_info);
  if (status != SUCCESS)
    {
    am_message (AM_STATUS,"Read base window fail");
    exit (EXIT_FAILURE);
    }

/* ----------- */
/* read layout */
/* ----------- */
  status = pl_read_layout (&pl,pf_info.layout,bw.type);
  if (status != SUCCESS)
    {
    am_message (AM_STATUS,"Read layout fail");
    exit (EXIT_FAILURE);
    }

/* ------------ */
/* place window */
/* ------------ */
  status = pl_place_win (&pl,&bw);
  if (status != SUCCESS)
    {
    am_message (AM_STATUS,"Place windows fail");
    exit (EXIT_FAILURE);
    }

/* ------------ */
/* draw windows */
/* ------------ */
  status = pl_draw_win (&pl,&bw,pf_info.pathname,pf_info.meta_type);
  if (status != SUCCESS)
    {
    am_message (AM_STATUS,"Draw windows fail");
    exit (EXIT_FAILURE);
    }

/* neutral plot file plotting: 
   In neutral plot file plotting, the filename and the size
   the plot need to be passed as standard input. Under the new plotting
   mechanism, the FM os filename is not known until the 
   pl_batch_plot/pl_plot_meta is executed. 
   Therefore, it is a problem in the script MP_PLOT_MAP/XS_LAUNCH_PLOTS
   that it does not know what file name need to be put for the neutral
   plot file plotting. To temprarily fix it, the file is in user home
   directory and its file name is taken as
   the scratch data file name with the extension ".NPF". After the plot 
   file is created, then rename it right away, so it won't screw up 
   the FILE MANAGMENT system 
*/
#if vms
   if (ARE_SAME (pf_info.meta_type,"NPF"))
      {
      pdot = strchr (bw.file,']') + 1;
      strcpy (tmp_fname,pdot);
	  
	  pdot = strchr (tmp_fname,'.') + 1;
	  strcpy (pdot,"NPF");

      ho_translate_symbol ("SYS$LOGIN",&tmp_tcp);
      sprintf (tmp_npf,"%s%s",tmp_tcp[0],tmp_fname);

      sprintf (command,"COPY %s %s.",tmp_npf,pf_info.pathname);
      system (command);
      sprintf (command,"DELETE %s;",tmp_npf,pf_info.pathname);
      system (command);
      }
#endif

/* --------------------- */
/* Create parameter file */
/* --------------------- */
  fd = ho_open (parameter_file,"w",NULL);
  if (fd == NULL_FILE_PTR)
    {
    am_message (AM_STATUS,"Cannot create parameter file");
    exit (EXIT_FAILURE);
    }

/* ---------- */
/* Vdc extent */
/* ---------- */
  vdc_extent_xmin = 0.00000;
  vdc_extent_ymin = 0.00000;
  if (pl.width > pl.height)
    {
    vdc_extent_xmax = 1.00000;
    vdc_extent_ymax = pl.height / pl.width;
    }
  else
    {
    vdc_extent_xmax = pl.width / pl.height;
    vdc_extent_ymax = 1.00000;
    }
  fprintf (fd,"vdc_extent(%lf,%lf,%lf,%lf)\n",
	   vdc_extent_xmin,vdc_extent_ymin,
	   vdc_extent_xmax,vdc_extent_ymax);

/* ------------ */
/* Vdc viewport */
/* ------------ */
  inch_to_mm = 1.0;
  tu_unit_conversion (inch_to_mm,"INCHES",&inch_to_mm,"MM");
  vdc_viewport_xmin = 0.00000;
  vdc_viewport_ymin = 0.00000;
  vdc_viewport_xmax = pl.width * inch_to_mm;
  vdc_viewport_ymax = pl.height * inch_to_mm;
  fprintf (fd,"vdc_viewport(%lf,%lf,%lf,%lf)\n",
	       vdc_viewport_xmin,vdc_viewport_ymin,
	       vdc_viewport_xmax,vdc_viewport_ymax);

  fclose (fd);

/* -------------------------------- */
/* put into FM_HDR AND PLOTFILE HDR */
/* -------------------------------- */
  if (pf_info.save == TRUE)
      {
      pf_info.xsize = pl.width;
      pf_info.ysize = pl.height;
      status = pf_put_plotfile (&pf_info);
      if (status != SUCCESS)
	  {
	  am_message (AM_STATUS,"Fail to save plotfile. File may not be saved.");
	  exit (EXIT_FAILURE);
	  }
      }

  fm_update_expiration_date ("ESI$PLOTS",pf_info.filename,"PLOTFILE",pf_info.expired_date);
  fm_release_file ("ESI$PLOTS",pf_info.filename,"PLOTFILE");

  or_logout (TRUE);
  exit (EXIT_SUCCESS);
}
