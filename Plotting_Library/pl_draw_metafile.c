/* DEC/CMS REPLACEMENT HISTORY, Element PL_DRAW_METAFILE.C */
/* *7    16-JUL-1991 13:47:32 MING "(SPR 0) redo the missing fix" */
/* *6     3-MAY-1991 16:12:28 KEE "(SPR -1) MOTIF/PRIORGKS, MOTIF/DECWGKS, DECW/PRIORGKS, DECW/DECGKS version" */
/* *5    18-JAN-1991 14:05:25 MING "(SPR -1) redo the change that is missing for whatever reason" */
/* *4    30-OCT-1990 13:14:42 GILLESPIE "(SPR 37) Add esi_ho_files.h" */
/* *3    25-OCT-1990 17:12:17 MING "(SPR 6075) add ho_translate_filename" */
/* *2    15-OCT-1990 12:18:13 GILLESPIE "(SPR 1) Clean up from Ernie checkin" */
/* *1    12-OCT-1990 17:40:53 GILLESPIE "Merge Ernie Deltas" */
/* DEC/CMS REPLACEMENT HISTORY, Element PL_DRAW_METAFILE.C */
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

#include "esi_c_lib.h"
#ifndef ESI_PL_H
#include "esi_pl.h"
#endif

#ifndef ESI_AM_H
#include "esi_am.h"
#endif

#ifndef ESI_GTYPES_H
#include "esi_gtypes.h"
#endif

#ifndef ESI_HO_FILES_H
#include "esi_ho_files.h"
#endif

#ifndef ESI_TS_H
#include "esi_ts.h"
#endif

#ifndef ESI_GKS_H
#include "esi_gks.h"
#endif

#ifdef SUN_GKS_OR_PRIOR_GKS
#include "sun_ansicgks.h"
#endif

#if USE_PROTOTYPES
publicdef INT pl_draw_metafile (CHAR *metafile,DOUBLE ll_ndc_x, DOUBLE ur_ndc_x,
				DOUBLE ll_ndc_y, DOUBLE ur_ndc_y)
#else
publicdef INT pl_draw_metafile (metafile,ll_ndc_x,ur_ndc_x,ll_ndc_y,ur_ndc_y)
CHAR *metafile;
DOUBLE ll_ndc_x;
DOUBLE ur_ndc_x;
DOUBLE ll_ndc_y;
DOUBLE ur_ndc_y;
#endif
{
#ifdef  SUN_GKS_OR_PRIOR_GKS
  INT ws_mi;
  INT tnr_id;
  CHAR *metatype;
  CHAR *pdot;
  FILE *fd;
  CHAR msg[80];
  CHAR parm_file[256];
  CHAR buffer[256];
  CHAR dummy[256];
  CHAR wstype[20];
  GKS_INT type;
  GKS_INT length;
  GKS_INT win_id;
  DOUBLE ll_wcs_x,ll_wcs_y,ur_wcs_x,ur_wcs_y;
  Ggksmit item;
  Gchar *data;
  GKS_REAL tm[2][3];
  GKS_REAL shiftx, shifty, scalex, scaley;
  GKS_INT seg_id;
  BOOL is_gksm;

  ho_translate_filename (metafile,metafile);

  if (!hoexists (metafile))
      {
      sprintf (msg,"Plot composer: Metafile %s for metafile window doesn't exist",metafile);
      am_message (AM_STATUS,msg);
      return FAIL;
      }
  
  ws_mi = GKS_METAIN_WKID;
  strcpy (dummy,metafile);
  metatype = strchr (dummy,'.') + 1;
  ts_sto_upper (metatype);

  strcpy (parm_file,metafile);
  pdot = strchr (parm_file,'.') + 1;

  strcpy (pdot,"PAR");
  

  if ((fd = ho_open (parm_file,"r",NULL)) == (FILE *) NULL)
    {
    ll_wcs_x = 0.0;
    ll_wcs_y = 0.0;
    ur_wcs_x = 1.0;
    ur_wcs_y = 1.0;
    }
  else
    {
    /* ------------------------------ */
    /* get vdc extent of the metafile */
    /* ------------------------------ */
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
    sprintf (msg,"Plot composer: Metafile window: %s metatype not support.\n",metatype);
    am_message (AM_STATUS,"msg");
    return FAIL;
    }
  /* ------------------------------------------------------------------- */
  /* deactivate main ws so that input metafile will not insert to the ws */
  /* then read in the metafile into WISS and insert into the main ws     */
  /* after the segment transformation                                    */
  /* ------------------------------------------------------------------- */
  wm_gdawk (MAIN_WKID);

  /* --------------------------------------- */
  /* open WISS and allocate window & segment */
  /* --------------------------------------- */
  wm_gopwk (GKS_WISS_WKID,0,GKS_WISS_WTYPE);
  wm_gacwk (GKS_WISS_WKID);

  wmwallocate (&win_id,ll_ndc_x,ur_ndc_x,ll_ndc_y,ur_ndc_y,-1);
  wmtallocate (&tnr_id,0.0,1.0,0.0,1.0,ll_wcs_x,ur_wcs_x,ll_wcs_y,ur_wcs_y);
  wmsallocate (&seg_id);

  /* ------------------------------- */
  /* read in metafile in one segment */
  /* ------------------------------- */
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
  wmsclose (seg_id);

  /* -------------------------------- */
  /* create the transformation matrix */
  /* -------------------------------- */
  scalex = (ur_ndc_x-ll_ndc_x)/(ur_wcs_x-ll_wcs_x);
  scaley = (ur_ndc_y-ll_ndc_y)/(ur_wcs_y-ll_wcs_y);
  shiftx = ll_ndc_x;
  shifty = ll_ndc_y;
  wm_gevtm (0.0,0.0,shiftx,shifty,0.0,scalex,scaley,0,tm);

  /* ----------------------------------------- */
  /* active the main ws and insert the segment */
  /* ----------------------------------------- */
  wm_gacwk (MAIN_WKID);
  wm_ginsg (seg_id,tm);

  /* ------------------ */
  /* close workstations */
  /* ------------------ */
  wmwclose (win_id);
  wm_gdawk (GKS_WISS_WKID);
  wm_gclwk (GKS_WISS_WKID);
  wm_gclwk (ws_mi);
#endif
  return SUCCESS;
}

