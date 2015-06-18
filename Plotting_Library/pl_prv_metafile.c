/* DEC/CMS REPLACEMENT HISTORY, Element PL_PRV_METAFILE.C */
/* *5     3-MAY-1991 16:12:35 KEE "(SPR -1) MOTIF/PRIORGKS, MOTIF/DECWGKS, DECW/PRIORGKS, DECW/DECGKS version" */
/* *4     9-NOV-1990 10:34:03 MING "(SPR -1) replace ho_find_files and ho_find_files2 to ho_find_pathnames and ho_find_filenames" */
/* *3    15-OCT-1990 22:46:55 GILLESPIE "(SPR 1000) More Ernie deltas" */
/*  1B1  15-OCT-1990 17:24:27 GILLESPIE "Merge Ernie deltas" */
/* *2    15-OCT-1990 12:18:25 GILLESPIE "(SPR 1) Clean up from Ernie checkin" */
/* *1    12-OCT-1990 17:46:40 GILLESPIE "Merge Ernie Deltas" */
/* DEC/CMS REPLACEMENT HISTORY, Element PL_PRV_METAFILE.C */
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
Description:

Prototype:
    publicdef INT func (arg);

Parameters:	    (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) arg	- description

Return Value/Status:
    SUCCESS	- Successfull completion
    FAIL	- Unsuccessfull completion

Scope:
    PUBLIC
    PRIVATE to <module_description>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/


#ifndef ESI_C_LIB_H
#include "esi_c_lib.h"
#endif

#ifndef ESI_AM_H
#include "esi_am.h"
#endif

#ifndef ESI_GTYPES_H
#include "esi_gtypes.h"
#endif

#ifndef ESI_GKS_H
#include "esi_gks.h"
#endif

#ifndef ESI_TS_H
#include "esi_ts.h"
#endif

#ifndef ESI_FI_H
#include "esi_fi.h"
#endif

#ifndef ESI_HO_FILES_H
#include "esi_ho_files.h"
#endif

#ifndef ESI_TC_H
#include "esi_tc.h"
#endif

#ifndef ESI_WI_H
#include "esi_wi.h"
#endif

#ifndef ESI_WM_CLS_H
#include "esi_wm_cls.h"
#endif

#ifndef ESI_AM_ERR_H
#include "esi_am_err.h"
#endif

#ifndef ESI_WM_H
#include "esi_wm.h"
#endif

#include "pl_meta_cdlg.rh"
static CHAR PL_RESOURCE_FILE[] = "PL_META_CDLG";
publicdef INT pl_drw_metafile ();

#if UNIX
#ifndef SUN_ANSICGKS_H
#include "sun_ansicgks.h"
#endif
#endif

#ifdef vms
#include "esi_glue.h"
#endif

typedef struct metafile_workspace
  {
  TCP file_list;
  BOOL first_time;
  PATHNAME file;
  GKS_INT win_id;
  GKS_INT tnr_id;
  GKS_INT seg_id;
  } META_WS;



publicdef INT pl_prv_meta_srvr (id,item,meta_ws)
INT id;
INT item;
META_WS *meta_ws;
{
  INT status = SUCCESS;
  INT meta_selected[1];
  INT i;
  GKS_INT color;
  GKS_INT fill_style;
  GKS_REAL ndc_ll_x,ndc_ll_y,ndc_ur_x,ndc_ur_y;
  GKS_REAL wcs_ll_x,wcs_ll_y,wcs_ur_x,wcs_ur_y;
  GKS_REAL x[5],y[5];
  INT nmetas = 0;
  BOOL is_gksm;
  PATHNAME metafile;

  switch (item)
    {
    case PL_QUIT:
      if (!meta_ws->first_time)
	{
	wmsdelete (meta_ws->seg_id);
        wmwdelete (meta_ws->win_id);
	}
      ts_tcp_free (meta_ws->file_list);
      am_quit ();
      break;
                
    case PL_PREVIEW:
      wi_query_selections (id,PL_META_SELECTOR,&nmetas,meta_selected);
      if (nmetas != 1)
        {
        wi_beep (1);
        am_message (AM_STATUS,"No plot file selected.");
        }
      else
        {
        strcpy (metafile, meta_ws->file_list[meta_selected[0]-1]);
        ho_add_path ("ESI$PLOTS",metafile,sizeof (PATHNAME),metafile);
        strcpy (meta_ws->file,metafile);
        if (meta_ws->first_time)
          {
          lu_text_to_index ("COLOR",&color,"BACKGROUND");
          wmwallocate (&meta_ws->win_id,0.0,1.0,0.0,1.0,color);
	  wmoset (meta_ws->win_id, WM_CLASS_WINDOW,"","Plot Preview");
	  meta_ws->first_time = FALSE;
          }
        else
          {
          wmwopen (meta_ws->win_id);
	  wmsdelete (meta_ws->seg_id);
          wmtinq (meta_ws->tnr_id,&ndc_ll_x,&ndc_ur_x,&ndc_ll_y,&ndc_ur_y,
		       &wcs_ll_x,&wcs_ur_x,&wcs_ll_y,&wcs_ur_y);
          x[0] = wcs_ll_x;
	  x[1] = wcs_ur_x;
	  x[2] = wcs_ur_x;
	  x[3] = wcs_ll_x;
	  x[4] = wcs_ll_x;
	  y[0] = wcs_ll_y;
	  y[1] = wcs_ll_y;
	  y[2] = wcs_ur_y;
	  y[3] = wcs_ur_y;
	  y[4] = wcs_ll_y;

	  i = 5;
	  lu_text_to_index ("FILL_STYLE",&fill_style,"SOLID");
	  wm_gsfais (fill_style);
	  lu_text_to_index ("COLOR",&color,"BACKGROUND");
	  wm_gsfaci (color);
	  wm_gfa (i,x,y);
          wm_grsgwk (MAIN_WKID);
	  }
      
          pl_drw_metafile (metafile);
          wmwclose (meta_ws->win_id);
        }
      break;
    }

  return status;
  }

#if USE_PROTOTYPES
publicdef INT pl_prv_metafile (void)
#else
publicdef INT pl_prv_metafile ()
#endif
  {
  BOOL done = FALSE;
  INT status = SUCCESS; 
  INT i;
  INT id;
  CHAR *meta_string;
  PATHNAME dummy;
  PATHNAME directory;
  META_WS *meta_ws;

  status = am_open_workspace ("PL_META_WS",AM_APPLICATION,
                                              (VOIDPTR *) &meta_ws);
  if (status == AM_ERR_NOT_DEFINED)
    {
    status = am_define_workspace ("PL_META_WS",AM_APPLICATION,
				 sizeof (META_WS),(VOIDPTR *)&meta_ws);
    }

  meta_ws->file_list = (TCP) NULL;

  status = am_define_resource (AM_DIALOG, PL_RESOURCE_FILE, PL_META_CDLG,
                               pl_prv_meta_srvr, &id, meta_ws);

  if (status != SUCCESS)
    {
    am_message (AM_STATUS,"Cannot open resource file");
    return FAIL;
    }

  ho_translate_filename ("ESI$PLOTS",directory);
#if UNIX
  sprintf (dummy,"%s/*.*",directory);
#endif
#ifdef vms
  sprintf (dummy,"%s*.*",directory);
#endif

  ho_find_filenames (dummy,&meta_ws->file_list);
  ts_tcp_sort (meta_ws->file_list);
  i = ts_tcp_len (meta_ws->file_list);
  meta_string = ts_pack_strings (i,meta_ws->file_list);

  wi_set_dialog_text (id,PL_META_SELECTOR,meta_string);

  wi_set_selection_limit (id,PL_META_SELECTOR,1);

  meta_ws->first_time = TRUE;

  status = wi_open_dialog (id);

  tc_free (meta_string);

  return SUCCESS;

}
#if USE_PROTOTYPES
publicdef INT pl_drw_metafile (CHAR *metafile)
#else
publicdef INT pl_drw_metafile (metafile)
CHAR *metafile;
#endif
{
  INT status = SUCCESS;
  INT ws_mi;
  INT tnr_id;
  INT seg_id;
  BOOL is_gksm;
  GKS_INT type;
  GKS_INT length;
  GKS_INT color;
  GKS_INT meta_type;
  CHAR *metatype;
  CHAR *pdot;
  FILE *fd;
  CHAR msg[80];
  CHAR parm_file[256];
  CHAR buffer[256];
  CHAR dummy[256];
  CHAR wstype[20];
  INT max_length;
  GKS_REAL datarec[1000];
  DOUBLE ll_wcs_x,ll_wcs_y,ur_wcs_x,ur_wcs_y;
  META_WS *meta_ws;
#ifdef SUN_GKS_OR_PRIOR_GKS
  Ggksmit item;
  Gchar *data;
#endif

/* ---------------------------------- */
/* get a workstation for the metafile */
/* ---------------------------------- */
  ws_mi = GKS_WISS_WKID + 1;

/* ---------------- */
/* get the metatype */
/* ---------------- */
  strcpy (dummy,metafile);
  metatype = strchr (dummy,'.') + 1;
  ts_sto_upper (metatype);

#ifdef vms
  pdot = strchr (metatype,';');
  *pdot = '\0';
#endif

/* ---------------------- */
/* get the parameter file */
/* ---------------------- */
  strcpy (parm_file,metafile);
  pdot = strchr (parm_file,'.') + 1;
  strcpy (pdot,"PAR");

/* ------------------------------------------------------- */
/* get the vdc_extent which will be used as the wcs extent */
/* ------------------------------------------------------- */

/* ------------------------------------ */
/* if file doesn't exist, get unity wcs */
/* ------------------------------------ */
  if ((fd = ho_open (parm_file,"r",NULL)) == (FILE *) NULL)
    {
    ll_wcs_x = 0.0;
    ll_wcs_y = 0.0;
    ur_wcs_x = 1.0;
    ur_wcs_y = 1.0;
    }
  else
    {
/* ------------------------------------------------ */
/* otherwise get the values from the parameter file */
/* ------------------------------------------------ */
    fgets (buffer,256,fd);

    pdot = strchr (buffer,'(') + 1;
    sscanf (pdot,"%lf",&ll_wcs_x);

    pdot = strchr (pdot,',') + 1;
    sscanf (pdot,"%lf",&ll_wcs_y);

    pdot = strchr (pdot,',') + 1;
    sscanf (pdot,"%lf",&ur_wcs_x);

    pdot = strchr (pdot,',') + 1;
    sscanf (pdot,"%lf",&ur_wcs_y);

    }

/* ------------------------------------------------------------ */
/* make the border of the plot away from the edge of the window */
/* ------------------------------------------------------------ */
  ll_wcs_x -= 0.001;
  ll_wcs_y -= 0.001;
  ur_wcs_x += 0.001;
  ur_wcs_y += 0.001;

/* --------------------------------------------------- */
/* for SUN, vdc entent is in the range from 0 to 32767 */
/* --------------------------------------------------- */
#if UNIX
    ll_wcs_x *= 32767.0;
    ll_wcs_y *= 32767.0;
    ur_wcs_x *= 32767.0;
    ur_wcs_y *= 32767.0;
#endif

  am_open_workspace ("PL_META_WS",AM_APPLICATION,
				 (VOIDPTR *) &meta_ws);

  wmtallocate (&meta_ws->tnr_id,0.0,1.0,0.0,1.0,
	       ll_wcs_x,ur_wcs_x,ll_wcs_y,ur_wcs_y);
  wmsallocate (&meta_ws->seg_id);
  sprintf (msg,"Plot file: %s",metafile);
  wmoset (meta_ws->seg_id,WM_CLASS_SEGMENT,"",msg);

#ifdef PRIOR_GKS /* ========== BEGIN PRIOR GKS ========== */
  if (ARE_SAME (metatype,"CGMT"))
    {
    strcpy (wstype,"mi");
    is_gksm = FALSE;
    }
  else if (ARE_SAME (metatype,"CGMC"))
    {
    strcpy (wstype,"mi");
    is_gksm = FALSE;
    }
  else if (ARE_SAME (metatype,"CGMB"))
    {
    strcpy (wstype,"mi");
    is_gksm = FALSE;
    }
  else if (ARE_SAME (metatype,"GKSM"))
    {
    strcpy (wstype,"mi");
    is_gksm = TRUE;
    }
  else 
    {
    sprintf (msg,"%s metafile type not support.",metatype);
    am_message (AM_STATUS,msg);
    return FAIL;
    }
  sprintf (msg,"Displaying plot file %s.",metafile);
  am_message (AM_DIALOG,msg);
  gopenws (ws_mi,metafile,wstype);
  do {
#if UNIX
/*   Waiting for ming to fix the following for VMS - PRIOR GKS */
     item = ggetgksm (ws_mi);
     data = greadgksm (ws_mi,item.length); 
#endif
     if (item.type > 0)
       {
       if (is_gksm && item.type < 70)
	 {
         ginterpret (&item,data);
	 }
       else if (!is_gksm && item.type < 70 && item.type != 1)
	 {
         ginterpret (&item,data);
	 }
       }
     free (data);
   } while (item.type > 0);
   am_message (AM_DIALOG,"");
#else    /* ========== END PRIOR GKS = BEGIN NON PRIOR GKS ==========*/
#if UNIX /*========== BEGIN SUN GKS ========== */
  if (ARE_SAME (metatype,"CGMT"))
    {
    strcpy (wstype,"cgmi");
    is_gksm = FALSE;
    }
  else if (ARE_SAME (metatype,"CGMC"))
    {
    strcpy (wstype,"cgmi");
    is_gksm = FALSE;
    }
  else if (ARE_SAME (metatype,"CGMB"))
    {
    strcpy (wstype,"cgmi");
    is_gksm = FALSE;
    }
  else if (ARE_SAME (metatype,"GKSM"))
    {
    strcpy (wstype,"mi");
    is_gksm = TRUE;
    }
  else 
    {
    sprintf (msg,"%s metafile type not support.",metatype);
    am_message (AM_STATUS,msg);
    return FAIL;
    }
  sprintf (msg,"Displaying plot file %s.",metafile);
  am_message (AM_DIALOG,msg);
  gopenws (ws_mi,metafile,wstype);
  do {
     item = ggetgksm (ws_mi);
     data = greadgksm (ws_mi,item.length);
     if (item.type > 0)
       {
       if (is_gksm && item.type < 70)
	 {
         ginterpret (&item,data);
	 }
       else if (!is_gksm && item.type < 70 && item.type != 1)
	 {
         ginterpret (&item,data);
	 }
       }
     free (data);
   } while (item.type > 0);
   am_message (AM_DIALOG,"");
#else   /* ========== END SUN GKS = BEGIN DECW GKS ========== */
#if 0
  if (ARE_SAME(metatype, "CGMT"))
    {
    meta_type = GKS$K_CGM_INPUT | GKS$M_CLEAR_TEXT_ENCODING;
    is_gksm = FALSE;
    }


  else if (ARE_SAME(metatype, "CGMC"))
    {
    meta_type = GKS$K_CGM_INPUT | GKS$M_CHARACTER_ENCODING;
    is_gksm = FALSE;
    }
#endif
  if (ARE_SAME(metatype, "GKSM"))
    {
    meta_type = GKS$K_GKSM_INPUT;
    is_gksm = TRUE;
    }
  else 
    {
    sprintf (msg,"%s metafile type not support.",metatype);
    am_message (AM_STATUS,msg);
    return FAIL;
    }
  sprintf (msg,"Displaying plot file %s.",metafile);
  am_message (AM_DIALOG,msg);
  gks$open_ws(&ws_mi, hoz_str_descpt(metafile), &meta_type);
  max_length = 1000;
  do {
     gks$get_item (&ws_mi,&type,&length);
     gks$read_item (&ws_mi,&max_length,datarec);
     
     if (type > 0)
       {
       if (is_gksm && type < 70)
	 {
         gks$interpret_item (&type,&length,datarec);
	 }
       else if (!is_gksm && type < 70 && type != 1)
	 {
         gks$interpret_item (&type,&length,datarec);
	 }
       }
   } while (type > 0);
   am_message (AM_DIALOG,"");
#endif  /* ========== END DECW GKS ========== */
#endif  /* ========== END NON PRIOR GKS ========== */
  wm_gclwk (ws_mi);
  wm_grsgwk (MAIN_WKID);
  wmsclose (meta_ws->seg_id);
  return SUCCESS;
}


