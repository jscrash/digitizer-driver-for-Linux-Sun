/* DEC/CMS REPLACEMENT HISTORY, Element PL_READ_BASE_WIN.C */
/* *9    14-OCT-1991 16:58:18 MING "(SPR 4448) ts_snowhite on display verbs, options, parameters" */
/* *8    23-SEP-1991 15:21:36 MING "(SPR 4158) increase the length of the string line" */
/* *7    12-SEP-1991 18:03:23 MING "(SPR 0) add esi_md_basis.ah" */
/* *6    11-JUL-1991 13:58:23 MING "(SPR 0) get rid off - in the display input line" */
/* *5     6-JUN-1991 16:56:03 MING "(SPR 0) new plot file managment" */
/* *4     5-APR-1991 12:17:48 JESSIE "(SPR 0) add xs scale format checking" */
/* *3    31-JAN-1991 16:31:39 MING "(SPR -1) fix sun compile noise" */
/* *2    23-OCT-1990 15:27:37 MING "(SPR 6075) take off the margin of cross section which causing the shifting of trace" */
/* *1    12-OCT-1990 17:47:13 GILLESPIE "Merge Ernie Deltas" */
/* DEC/CMS REPLACEMENT HISTORY, Element PL_READ_BASE_WIN.C */
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
Description: Read info for the base window 

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


#define MARGIN 1.4

/* * INCLUDE FILES * */
/* ----------------- */

#include "esi_c_lib.h"
#include "esi_gl_project.h"
#ifndef ESI_FI_H
#include "esi_fi.h"
#endif
#ifndef ESI_QS_H
#include "esi_qs.h"
#endif
#ifndef ESI_MP_H
#include "esi_mp.h"
#endif
#ifndef ESI_AM_H
#include "esi_am.h"
#endif
#ifndef ESI_CT_H
#include "esi_ct.h"
#endif
#ifndef ESI_ORACLE_H
#include "esi_oracle.h"
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
#ifndef ESI_HO_FILES_H
#include "esi_ho_files.h"
#endif
#ifndef ESI_XS_DEFS_H
#include "esi_xs_defs.h"
#endif
#ifndef ESI_XS_GR_H
#include "esi_xs_gr.h"
#endif
#ifndef ESI_TU_H
#include "esi_tu.h"
#endif

/* #define MD_BASIS_INIT */
#ifdef MD_BASIS_INIT 
#include "esi_md_basis.x"
#else
#define MD_BASIS_INIT
#include "esi_md_basis.ah"
#endif

#ifndef ESI_PL_H
#include "esi_pl.h"
#endif

#ifndef ESI_PF_H
#include "esi_pf.h"
#endif

#ifndef ESI_TC_H
#include "esi_tc.h"
#endif

#if USE_PROTOTYPES
publicdef INT pl_read_base_win (PL_BASE_WINDOW *bw,PF_INFO *pf_info)
#else
publicdef INT pl_read_base_win (bw,pf_info)
PL_BASE_WINDOW *bw;
PF_INFO *pf_info;
#endif
{
  INT status = SUCCESS;

  switch (bw->type)
    {
    case PL_MAP:
      pf_info->type = PL_MAP;
      status = pl_read_map (bw,pf_info);
      break;

    case PL_XSECT:
      pf_info->type = PL_XSECT;
      status = pl_read_xs (bw,pf_info);
      break;

    default:
      am_message (AM_STATUS, "Invaild base window type");
      break;
    }
  return status;
}

#if USE_PROTOTYPES
publicdef INT pl_read_map (PL_BASE_WINDOW *bw, PF_INFO *pf_info)
#else
publicdef INT pl_read_map (bw,pf_info)
PL_BASE_WINDOW *bw;
PF_INFO *pf_info;
#endif
{
  INT status;
  PROJECT_NAME project;
  MAP_NAME map_name;
  MAP_STRUCTURE *map;
  MD_DISPLAY *display;
  MD_COMPILED_MAPDEF *compiled;
  MD_COMPILED_VERB *verbstruc;
  MD_PARM_STRUCT *parmstruc;
  PROJECT_DEFAULT *defaults;
  CHAR dummy[100];
  UOM_NAME uom1,uom2;
  INT verb_ndx,key_ndx;
  MD_KEY_INFO *key_info;
  GKS_BATCH_POLYMARKERS *polymarkers;
  FILE *file;
  INT string_length;
  INT next_parm;
  INT parm_type;
  BOOL first_parm;
  INT i,j,k;
  INT *map_number;
  DOUBLE ll_wcs_x, ll_wcs_y, ur_wcs_x, ur_wcs_y;
  DOUBLE margin_wcs;
  MP_TTLBLK_VARS *ttlblk;
  INT len;
  CHAR line[260];

  qs_inq_c (QS_PROJECT_NAME,project,(INT *)0);
  
/* ---------------------------- */
/* * DEFINE NEEDED WORKSPACES * */
/* ---------------------------- */

  am_define_workspace ("MAP_NUMBER",AM_GLOBAL,sizeof(INT),(VOIDPTR*)&map_number);
  *map_number = 1;

  am_define_workspace ("MAPDEF",AM_APPLICATION,sizeof(MAP_STRUCTURE),(VOIDPTR*)&map);
  pf_info->parm.map = map;
  map->map_number = 1;

  am_define_workspace ("GKS_BATCH_POLYMARKERS",AM_GLOBAL,
                sizeof(GKS_BATCH_POLYMARKERS),(VOIDPTR *)&polymarkers);
  
  am_define_workspace ("MP_TTLBLK_VARS",AM_APPLICATION,
                sizeof(MP_TTLBLK_VARS),(VOIDPTR *)&ttlblk);
  

/* ------------------------------------ */
/* * OPEN MAPDEF FILE AND READ MAPDEF * */
/* ------------------------------------ */

  file = ho_open (bw->file,"r", NULL );
  if (file == NULL_FILE_PTR)
    {
    printf("\nCouldn't open mapdef file\n");
    return FAIL;
    }

  fgets(map_name,80,file);
  map_name[strlen(map_name)-1] = 0;

  strcpy (map->map_name,map_name);

  fgets(map->map_projection,80,file);
  map->map_projection[strlen(map->map_projection)-1] = 0;
  qs_set_c (QS_MAP_PROJECTION,map->map_projection,0);

  fgets (map->map_coordinate_system,80,file);
  map->map_coordinate_system[strlen(map->map_coordinate_system)-1] = 0;

  fscanf (file,"%lf %lf\n",&(map->lower_left_xy.x),&(map->lower_left_xy.y));
  fscanf (file,"%lf %lf\n",&(map->upper_right_xy.x),&(map->upper_right_xy.y));

  fgets (map->lower_left_str,80,file);
  map->lower_left_str[strlen(map->lower_left_str)-1] = 0;
  if (strcmp(map->lower_left_str,NULL_FOR_COMM) EQUALS 0)
    {
    strcpy(map->lower_left_str,"");
    }

  fgets(map->upper_right_str,80,file);
  map->upper_right_str[strlen(map->upper_right_str)-1] = 0;
  if (strcmp(map->upper_right_str,NULL_FOR_COMM) EQUALS 0)
    {
    strcpy(map->upper_right_str,"");
    }


  fscanf(file,"%lf ",&(map->map_scale));
  fgets(map->map_scale_uom,80,file);
  map->map_scale_uom[strlen(map->map_scale_uom)-1] = 0;


/* -------------------------------------------------------------------- */
/* * COMPUTE SCALE FACTOR BETWEEN WORLD COORDINATES AND PLOTTER UNITS * */
/* -------------------------------------------------------------------- */

  am_open_workspace ("PROJECT_DEFAULTS", AM_GLOBAL, (VOIDPTR*)&defaults);
  sprintf(dummy,"%lf %s",map->map_scale,map->map_scale_uom);

  tu_parse_unit(dummy,&(map->wcs_to_inches),uom1,uom2);

  map->wcs_to_inches = 1.0;
  tu_unit_conversion(map->wcs_to_inches,defaults->unit.horizontal,
					&(map->wcs_to_inches),uom1);
  map->wcs_to_inches /= map->map_scale;
  tu_unit_conversion(map->wcs_to_inches,uom2,&(map->wcs_to_inches),"INCHES");

  polymarkers->inch_to_wcs_factor = 1.0 / map->wcs_to_inches;



/* * GET DISPLAY STRUCTURE * */
/* ------------------------- */

  fscanf(file,"%d\n",&i);
  mp_init_display_struct(&display,i);
  map->display = display;
  display->last_row_used = i;

  for (i=0; i<display->last_row_used; i++)
    {
    fscanf(file,"%d %d %d\n",&(display->verb_ndx[i]),&(display->key_ndx[i]),
						&(display->compiled_row[i]));
    display->parameters[i] = (CHAR *) am_allocate (AM_GLOBAL,260*sizeof (CHAR));
    fgets (line,84,file);
    ts_snowhite (line);
    strcpy (display->verbs[i],line+1);
    fgets (line,84,file);
    ts_snowhite (line);
    strcpy (display->keywords[i],line+1);
    fgets (line,256,file);
    ts_snowhite (line);
    strcpy (display->parameters[i],line+1);
    }


/* --------------------------- */
/* * GET COMPILED STRUCTURES * */
/* --------------------------- */

  fscanf(file,"%d\n",&i);
  mp_init_compiled_mapdef(&compiled,i+1);
  map->compiled = compiled;
  compiled->last_row_used = i;

  for (i=0; i<compiled->last_row_used+1; i++)
    {
    fscanf(file,"%d\n",&(compiled->verb_code[i]));

    for (j=0; j<display->last_row_used; j++)
	{
	if (display->compiled_row[j] EQUALS i)
	    {
	    break;
	    }
	}

    mp_verb_lookup(compiled->verb_code[i],&verb_ndx);
    if (j < display->last_row_used)
	{
	mp_init_comp_verb(&verbstruc,&(overlay_verbs[display->verb_ndx[j]]));
	compiled->verb_ptr[i] = verbstruc;

	for (j=0; j<verbstruc->nrows; j++)
	    {
	    fscanf(file,"%d %d %d\n",&(verbstruc->key_code[j]),
					&(verbstruc->key_used[j]),&next_parm);
	    mp_key_lookup(verbstruc->key_code[j],verb_ndx,&key_ndx);
	    key_info = &(overlay_verbs[verb_ndx].key_list[key_ndx]);

					/* PARMSTRUC LOOP */

	    first_parm = TRUE;
	    while (next_parm)
		{
		fscanf(file,"%d %d %d\n",&next_parm,&parm_type,&string_length);

		if (first_parm)
		    {
		    parmstruc = (MD_PARM_STRUCT *)
			    am_allocate(AM_APPLICATION,sizeof(MD_PARM_STRUCT));
		    verbstruc->parm_ptr[j] = parmstruc;
		    first_parm= FALSE;
		    }
		else
		    {
		    parmstruc->nxtparm = (MD_PARM_STRUCT *)
			    am_allocate(AM_APPLICATION,sizeof(MD_PARM_STRUCT));
		    parmstruc = parmstruc->nxtparm;
		    }

		parmstruc->parmtype = parm_type;
		switch (parm_type)
		    {
		    case 1:
			parmstruc->parmval.cval =
			    (CHAR *)am_allocate(AM_APPLICATION,string_length+1);
			fgets(parmstruc->parmval.cval,string_length+1,file);
			break;

		    case 2:
			fscanf(file,"%ld\n",&(parmstruc->parmval.ival));
			break;

		    case 3:
			fscanf(file,"%lf\n",&(parmstruc->parmval.dbval));
			if (key_info->val_code EQUALS UOMD$)
			    {
			    parmstruc->parmval.dbval /= map->wcs_to_inches;
			    }
			break;
		    }
		}
	    }
	}
    }


  fclose(file);

/* -------------------- */
/* init display element */
/* -------------------- */
  map->wm_id_list = (INT *) am_allocate(AM_GLOBAL,
					    display->last_row_used*sizeof(INT));
  tc_zeroes(map->wm_id_list,display->last_row_used*sizeof(INT));
  map->wm_id_count = (INT *) am_allocate(AM_GLOBAL,
					    display->last_row_used*sizeof(INT));
  tc_zeroes(map->wm_id_count,display->last_row_used*sizeof(INT));
  map->wm_group_names = (CHAR **) am_allocate(AM_GLOBAL,
					display->last_row_used*sizeof(CHAR *));
  tc_zeroes(map->wm_group_names,display->last_row_used*sizeof(CHAR *));
  map->wm_nalloc = display->last_row_used;

/*------------------------------ */
/* set map to be the current map */
/* ----------------------------- */
  mp_set_current_map(map);

/* ----------------------------------------- */
/* calculate the width and height of the map */
/* ----------------------------------------- */
  margin_wcs = MARGIN /map->wcs_to_inches;
  ll_wcs_y = map->lower_left_xy.y - margin_wcs;
  ll_wcs_x = map->lower_left_xy.x - margin_wcs;
  ur_wcs_x = map->upper_right_xy.x + margin_wcs;
  ur_wcs_y = map->upper_right_xy.y + margin_wcs;

/* ------------------------------------------------- */
/* stuff information into base window data sturcture */
/* ------------------------------------------------- */
  strcpy (bw->name,map_name);

  strcpy (bw->scale_uom,map->map_scale_uom);
  bw->scale = map->map_scale;
  strcpy (bw->projection,map->map_projection);

  bw->ll_wcs_x = ll_wcs_x;
  bw->ll_wcs_y = ll_wcs_y;
  bw->ur_wcs_x = ur_wcs_x;
  bw->ur_wcs_y = ur_wcs_y;

  bw->width = (ur_wcs_x-ll_wcs_x)*map->wcs_to_inches;
  bw->height = (ur_wcs_y-ll_wcs_y)*map->wcs_to_inches;

  return SUCCESS;

}


#if USE_PROTOTYPES 
publicdef INT pl_read_xs (PL_BASE_WINDOW *bw,PF_INFO *pf_info)
#else
publicdef INT pl_read_xs (bw,pf_info)
PL_BASE_WINDOW *bw;
PF_INFO *pf_info;
#endif
{
  INT status = SUCCESS;
  XS_STRUCT **pxs;
  FILE *file;
  UOM_NAME scale_uom;
  DOUBLE scale;

/* -------------- */
/* read data file */
/* -------------- */
  file = ho_open (bw->file,"r", NULL );
  if (file == NULL_FILE_PTR)
    {
    printf ("\nCouldn't open cross section file\n");
    return FAIL;
    }

/* -------------- */
/* read XS_STRUCT */
/* -------------- */
  am_define_workspace ("XS_STRUCT",AM_APPLICATION,
                        sizeof (XS_STRUCT **),(VOIDPTR *)&pxs);
  *pxs = xs_obj_load (bw->file);
  
  if (*pxs == (XS_STRUCT *)0)
    {
    printf ("Load cross section failed.\n");
    return FAIL;
  }

  pf_info->parm.xs = *pxs;

/* ------------------------------------------------- */
/* stuff information into base window data structure */
/* ------------------------------------------------- */
  strcpy (bw->name,(*pxs)->layout_name);

  status = tu_parse_scale ((*pxs)->xs_scale_str,&scale,scale_uom);

  if (status != SUCCESS)
    {
    strcpy (bw->scale_uom,"");
    bw->scale = 0.0;
    }
  else
    {
    strcpy (bw->scale_uom,scale_uom);
    bw->scale = scale;
    }
  strcpy (bw->projection,"");

  bw->ll_wcs_x = 0.0;
  bw->ll_wcs_y = 0.0;
  bw->ur_wcs_x = (*pxs)->drawing.width;
  bw->ur_wcs_y = (*pxs)->drawing.height;

  bw->width = (*pxs)->drawing.width;
  bw->height = (*pxs)->drawing.height;

  return SUCCESS;
}


