/* DEC/CMS REPLACEMENT HISTORY, Element PF_POST_OVLY.C */
/* *4     9-OCT-1991 09:49:12 MING "(SPR 3776) fix return status to avoid extra message" */
/* *3     2-AUG-1991 17:26:27 MING "(SPR 8743) fix pf_wcs_ur_x and pf_wcs_ur_y" */
/* *2     6-JUN-1991 15:52:22 MING "(SPR 0) new checkin " */
/* *1     6-JUN-1991 15:45:31 MING "plot file posting" */
/* DEC/CMS REPLACEMENT HISTORY, Element PF_POST_OVLY.C */
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

#ifndef ESI_PF_H
#include "esi_pf.h"
#endif

#ifndef ESI_MP_H
#include "esi_mp.h"
#endif

#ifndef ESI_QS_H
#include "esi_qs.h"
#endif

#ifndef ESI_HO_FILES_H
#include "esi_ho_files.h"
#endif

#ifndef ESI_GKS_H
#include "esi_gks.h"
#endif

#ifndef ESI_C_LIB_H
#include "esi_c_lib.h"
#endif

#ifndef ESI_TS_H
#include "esi_ts.h"
#endif

#ifndef ESI_WM_CLS_H
#include "esi_wm_cls.h"
#endif

#ifndef ESI_WM_INT_H
#include "esi_wm_int.h"
#endif

#ifndef ESI_CONF_WS_H
#include "esi_conf_ws.h"
#endif


#ifdef PRIOR_GKS
#include "sun_ansicgks.h"
#endif

#define PF_NOT_IN_CUR_MAP 1
#define PF_METATYPE_NOT_SUPPORT 2
#define PF_NO_CURRENT_MAP 3
#define MARGIN 1.4

/* Function Description -----------------------------------------------------
Description:
    Plotfile overlay posting

Prototype:
    publicdef pf_post_overlay (pf)

Parameters:	    (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) PF_INFO *pf;

Return Value/Status:
    SUCCESS	- Successfull completion
    FAIL	- Unsuccessfull completion

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

#if USE_PROTOTYPES
publicdef INT pf_post_overlay (PF_INFO *pf)
#else
publicdef INT pf_post_overlay (pf)
PF_INFO *pf;
#endif
    {
    INT status = SUCCESS;
    PATHNAME metafile;
    PATHNAME dummy;
    CHAR *metatype;
    DOUBLE ndc_ll_x, ndc_ll_y, ndc_ur_x, ndc_ur_y;
    MAP_STRUCTURE *map;
    INT clip;
    INT tnr_id;
    INT seg_id;
    static INT new_seg_id = 0;
    INT win_id;
    CONFIGURATION_WORKSPACE *c;
    GKS_REAL shiftx,shifty,scalex,scaley,tm[2][3];
    GKS_INT wiss_seg = 9999;
    DOUBLE margin;
    POINT ll_xy,ur_xy;
    INT mode;
    DOUBLE pf_margin, pf_wcs_to_inches;
    DOUBLE pf_wcs_ll_x,pf_wcs_ll_y,pf_wcs_ur_x,pf_wcs_ur_y;
    WM_CURRENT *wmv_current = wm_static ();
    BOOL batch;
    INT msg_type;

    /* -------------------------- */
    /* get current map definition */
    /* -------------------------- */
    status = mp_get_current_map (&map);
    if (status != SUCCESS)
	{
	return PF_NO_CURRENT_MAP;
	}

    /* ----------------- */
    /* get plotfile info */
    /* ----------------- */
    status =  pf_get_plotfile (pf,pf->filename,pf->app,pf->analyst);

    if (status != SUCCESS)
        {
        return status;
        }
 
    /* ------------------- */
    /* check file exists ? */
    /* ------------------- */
    if (!hoexists (pf->pathname))
        {
	am_message (msg_type,"Plotfile does not exist.");
        return SUCCESS;
        }
 
    /* ------------------------------------------------- */
    /* diff error report method in interactive and batch */
    /* ------------------------------------------------- */
    qs_inq_i (QS_IF_BATCH,&batch);
    if (batch)
	{
	msg_type = AM_STATUS;
	}
    else
	{
	msg_type = AM_ALERT;
	}

    /* --------------------------------------------------- */
    /* get the NDC of the map extent inside the map window */
    /* --------------------------------------------------- */
    status = pf_get_clip_extent (pf,&ndc_ll_x,&ndc_ll_y,&ndc_ur_x,&ndc_ur_y);

    if (status != SUCCESS)
	{
        am_message (msg_type,"Warning: Plotfile is not inside the current map. Please check the extent of the plotfile and the map.");
	return SUCCESS;
	}

    /* -------------------------- */
    /* check if WISS has opened ? */
    /* -------------------------- */
    am_open_workspace ("CONFIGURATION",AM_GLOBAL,(VOIDPTR *)&c);
    wm_gdawk (MAIN_WKID);
    if (!c->wiss)
	{
        wm_gopwk (GKS_WISS_WKID,0,GKS_WISS_WTYPE);
        wm_gacwk (GKS_WISS_WKID);
	}

    /* ------------------------------------------------ */
    /* create segment not under WM                      */
    /* since it does not need to be kept track under WM */
    /* ------------------------------------------------ */
    wm_gcrsg (wiss_seg);

    /* ----------------- */
    /* draw the plotfile */
    /* ----------------- */
    status = pf_display_pf (pf->pathname,pf->meta_type);

    wm_gclsg ();

    if (!c->wiss)
	{
        wm_gdawk (GKS_WISS_WKID);
	}

    wm_gacwk (MAIN_WKID);


#if 0
    wm_gqclip (&clip);
#endif

    mp_get_plot_wcs_to_inches (pf->parm.map,&pf_wcs_to_inches);
    margin = MARGIN / map->wcs_to_inches;
    pf_margin = MARGIN/pf_wcs_to_inches;

    /* --------------------------------------------- */
    /* calcuate the actual plotfile extent in WCS    */
    /* for additonal spacing is allocated for border */
    /* --------------------------------------------- */
    pf_wcs_ll_x = pf->parm.map->lower_left_xy.x - pf_margin;
    pf_wcs_ll_y = pf->parm.map->lower_left_xy.y - pf_margin;

    if (pf->vdc_ur_x == 1.0)    
	{
/* ---------------------------------------------------------------------------------- */
/* since the metafile is in square coordinate (0,1) (0,1) so the WCS extent should be */
/* square. after we get the WCS for the side covered the full range, and add the same */
/* distance to the side that does not cover the full range.                           */
/* ---------------------------------------------------------------------------------- */
	pf_wcs_ur_x = pf->parm.map->upper_right_xy.x + pf_margin;
	pf_wcs_ur_y = pf_wcs_ll_y + pf_wcs_ur_x - pf_wcs_ll_x;
        }
    else
	{
	pf_wcs_ur_y = pf->parm.map->upper_right_xy.y + pf_margin;
	pf_wcs_ur_x = pf_wcs_ll_x + pf_wcs_ur_y - pf_wcs_ll_y;
	}

    /* -------------------------------------------------------------------- */
    /* calculate the rotating and translating factors of the plotfile image */
    /* -------------------------------------------------------------------- */
    ll_xy.x = wmv_current->ndc.min.x;
    ll_xy.y = wmv_current->ndc.min.y;
    ur_xy.x = wmv_current->ndc.max.x;
    ur_xy.y = wmv_current->ndc.max.y;

    scalex = (pf_wcs_ur_x - pf_wcs_ll_x)/ (map->upper_right_xy.x - map->lower_left_xy.x + 2*margin) * (ur_xy.x - ll_xy.x); 
    scaley = (pf_wcs_ur_y - pf_wcs_ll_y)/ (map->upper_right_xy.y - map->lower_left_xy.y + 2*margin) * (ur_xy.y - ll_xy.y);
    

    shiftx = (pf->parm.map->lower_left_xy.x - pf_margin - map->lower_left_xy.x + margin)/(map->upper_right_xy.x - map->lower_left_xy.x + 2*margin)*(ur_xy.x-ll_xy.x) + ll_xy.x;
    shifty = (pf->parm.map->lower_left_xy.y - pf_margin - map->lower_left_xy.y + margin)/(map->upper_right_xy.y - map->lower_left_xy.y + 2*margin)*(ur_xy.y-ll_xy.y) + ll_xy.y;

    /* ----------------------------- */
    /* get the transformation matrix */
    /* ----------------------------- */
    mode = 1;
    wm_gevtm (0.0,0.0,shiftx,shifty,0.0,scalex,scaley,mode,(GKS_REAL *)tm);

    /* -------------------- */
    /* define clip viewport */
    /* -------------------- */
    wmctallocate (&tnr_id,ndc_ll_x,ndc_ur_x,ndc_ll_y,ndc_ur_y,0.0,1.0,0.0,1.0);
    clip = 0;
    wm_gsclip (clip);

    if (new_seg_id != 0) /* delete the old segment */
	{
	wmsdelete (new_seg_id);
	}

    /* -------------------------------------------------------- */
    /* insert the segment from WISS with the new transformation */
    /* allocate a new segment for this transformed segment      */
    /* -------------------------------------------------------- */
    wmsallocate (&new_seg_id);
    wmoset (new_seg_id,WM_CLASS_SEGMENT,"",pf->filename);
    wm_ginsg (wiss_seg,(GKS_REAL *)tm);
    wmsclose (new_seg_id);
    wm_gdsg (wiss_seg);

    clip = 1;
    wm_gsclip (clip);
    wmtselect (map->map_tnr);

    if (!c->wiss)
	{
        wm_gclwk (GKS_WISS_WKID);
	}
    return SUCCESS;
    }




/* Function Description -----------------------------------------------------
Description:
    Return NDC of the base map in the window

Prototype:
    publicdef pf_get_clip_extent (pf,ndc_ll_x,ndc_ll_y,ndc_ur_x,ndc_ur_y)

Parameters:	    (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) PF_INFO *pf;
    (O) COORD *ndc_ll_x;
    (O) COORD *ndc_ll_y;
    (O) COORD *ndc_ur_x;
    (O) COORD *ndc_ur_y;

Return Value/Status:
    SUCCESS	- Successfull completion
    FAIL	- Unsuccessfull completion

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

#if USE_PROTOTYPES
publicdef INT pf_get_clip_extent (PF_INFO *pf,COORD *ndc_ll_x,
			      COORD *ndc_ll_y,COORD *ndc_ur_x,COORD *ndc_ur_y)
#else 
publicdef INT pf_get_clip_extent (pf,ndc_ll_x,ndc_ll_y,ndc_ur_x,ndc_ur_y)
PF_INFO *pf;
COORD *ndc_ll_x;
COORD *ndc_ll_y;
COORD *ndc_ur_x;
COORD *ndc_ur_y;
#endif
    {
    INT status;
    MAP_STRUCTURE *map;
    DOUBLE margin;
    DOUBLE wcs_ll_x,wcs_ll_y,wcs_ur_x,wcs_ur_y;
    DOUBLE range;

    status = mp_get_current_map (&map);

    if (status != SUCCESS)
	{
	return FAIL;
	}

    margin = MARGIN / map->wcs_to_inches;
    wcs_ll_x = map->lower_left_xy.x - margin;
    wcs_ll_y = map->lower_left_xy.y - margin;
    wcs_ur_x = map->upper_right_xy.x + margin;
    wcs_ur_y = map->upper_right_xy.y + margin;

    if (pf->parm.map->lower_left_xy.x > map->upper_right_xy.x ||
        pf->parm.map->lower_left_xy.y > map->upper_right_xy.y ||
        pf->parm.map->upper_right_xy.x < map->lower_left_xy.x ||
        pf->parm.map->upper_right_xy.y < map->lower_left_xy.y)
	{
	return PF_NOT_IN_CUR_MAP;
	}

    *ndc_ll_x = (map->lower_left_xy.x - wcs_ll_x) / (wcs_ur_x - wcs_ll_x);
    *ndc_ll_y = (map->lower_left_xy.y - wcs_ll_y) / (wcs_ur_y - wcs_ll_y);
    *ndc_ur_x = (map->upper_right_xy.x - wcs_ll_x) / (wcs_ur_x - wcs_ll_x);
    *ndc_ur_y = (map->upper_right_xy.y - wcs_ll_y) / (wcs_ur_y - wcs_ll_y);

    return status;
    }




/* Function Description -----------------------------------------------------
Description:
    Get NDC space in the plotfile 

Prototype:
    publicdef pf_read_parfile (metafile,vdc_ll_x,vdc_ll_y,vdc_ur_x,vdc_ur_y)

Parameters:	    (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) PATHNAME metafile;
    (O) DOUBLE *vdc_ll_x;
    (O) DOUBLE *vdc_ll_y;
    (O) DOUBLE *vdc_ur_x;
    (O) DOUBLE *vdc_ur_y;

Return Value/Status:
    SUCCESS	- Successfull completion
    FAIL	- Unsuccessfull completion

Limitations/Assumptions:

-----------------------------------------------------------------------------*/
#if USE_PROTOTYPES
publicdef INT pf_read_parfile (PATHNAME metafile,DOUBLE *vdc_ll_x,DOUBLE *vdc_ll_y,
 			    DOUBLE *vdc_ur_x, DOUBLE *vdc_ur_y)
#else
publicdef INT pf_read_parfile (metafile,vdc_ll_x,vdc_ll_y,vdc_ur_x,vdc_ur_y)
PATHNAME metafile;
DOUBLE *vdc_ll_x;
DOUBLE *vdc_ll_y;
DOUBLE *vdc_ur_x;
DOUBLE *vdc_ur_y;
#endif
    {
    PATHNAME par_file;
    FILE *fd;
    CHAR *pdot;
    DOUBLE vp_ll_x;
    DOUBLE vp_ll_y;
    DOUBLE vp_ur_x;
    DOUBLE vp_ur_y;

    strcpy (par_file,metafile);
    strcat (par_file,".PAR");
 
    if ((fd = ho_open (par_file,"r",NULL)) == (FILE *) NULL)
        {
	/* ------------------------------------------------ */
	/* default values when parameter file doesn's exist */
	/* ------------------------------------------------ */
        *vdc_ll_x = 0.0;
        *vdc_ll_y = 0.0;
        *vdc_ur_x = 1.0;
        *vdc_ur_y = 1.0;
	vp_ll_x = 0.0;
	vp_ll_y = 0.0;
	vp_ur_x = 1.0;
	vp_ur_y = 1.0;
        }
    else
        {
        CHAR buffer[256];
 
	/* get vdc extent */

        fgets (buffer,256,fd);
        pdot = strchr (buffer,'(') + 1;
        sscanf (pdot,"%lf",vdc_ll_x);
 
        pdot = strchr (pdot,',') + 1;
        sscanf (pdot,"%lf",vdc_ll_y);
 
        pdot = strchr (pdot,',') + 1;
        sscanf (pdot,"%lf",vdc_ur_x);
 
        pdot = strchr (pdot,',') + 1;
        sscanf (pdot,"%lf",vdc_ur_y);

	/* get vdc viewport */

        fgets (buffer,256,fd);
        pdot = strchr (buffer,'(') + 1;
        sscanf (pdot,"%lf",&vp_ll_x);
 
        pdot = strchr (pdot,',') + 1;
        sscanf (pdot,"%lf",&vp_ll_y);
 
        pdot = strchr (pdot,',') + 1;
        sscanf (pdot,"%lf",&vp_ur_x);
 
        pdot = strchr (pdot,',') + 1;
        sscanf (pdot,"%lf",&vp_ur_y);
        }
    }    
 
 
