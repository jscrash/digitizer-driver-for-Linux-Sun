/* DEC/CMS REPLACEMENT HISTORY, Element XS_GR_TOOLS.C*/
/*  19    7-FEB-1992 10:45:59 JEFF "(SPR 5767) fixed grid depth shift bug (see also xs_drw_trk)"*/
/* *18   14-AUG-1991 12:02:17 KEE "(SPR -1) Put in File Lock/Unlock logic"*/
/* *17    3-MAY-1991 16:15:41 KEE "(SPR -1) MOTIF/PRIORGKS, MOTIF/DECWGKS, DECW/PRIORGKS, DECW/DECGKS version"*/
/* *16   27-MAR-1991 13:29:49 KEE "(SPR -1) Cross Section bug fixing and enhancement for Canadian Hunter"*/
/* *15   15-OCT-1990 22:56:27 GILLESPIE "(SPR 1000) Fix Ernie checkin"*/
/* *14   12-OCT-1990 18:09:49 GILLESPIE "(SPR 100) Merge Ernie Deltas"*/
/*  11B1 12-OCT-1990 18:09:00 GILLESPIE "Merge Ernie Deltas"*/
/* *13   14-AUG-1990 17:12:14 GILLESPIE "(SPR 2001) Use LINE_ATTRIBUTEs"*/
/* *12   14-AUG-1990 13:45:49 MING "(SPR -1) fix the vdc extent "*/
/* *11    9-JUL-1990 09:36:08 MING "(SPR -1) produce CGM"*/
/* *10   19-JUN-1990 10:50:50 JULIAN "(SPR 0) fix aspect ratio on neutral file plotting"*/
/* *9    30-MAY-1990 17:23:13 SCD "(SPR 1) Ensure window is allocated before inquire and set of deferrals."*/
/* *8    17-APR-1990 23:52:34 JULIAN "(SPR -1) Check for width <= 0 make very small"*/
/* *7    14-APR-1990 15:28:57 SCD "(SPR 1) Eliminate redundant redraw following xsz_display_tops."*/
/* *6    17-JAN-1990 22:17:48 GILLESPIE "(SPR 6001) Change include file"*/
/* *5    20-SEP-1989 15:02:40 GORDON "(SPR 200) Get in sync with gulf code"*/
/* *4    19-SEP-1989 13:40:27 GILLESPIE "(SPR 100) GULF MODS"*/
/* *3    14-SEP-1989 18:45:54 GORDON "(SPR -1) checkin from gulf"*/
/* *2    14-SEP-1989 16:24:15 GORDON "(SPR 100) Gulf/Sun/Unix mods"*/
/* *1    19-JUN-1989 13:44:26 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element XS_GR_TOOLS.C*/
/******************************************************************************

                Copyright Exploration Systems, Inc. 1989
		   Unpublished -- All rights reserved

THIS SOFTWARE IS THE PROPRIETARY PROPERTY OF Exploration Systems, Inc.  AND MAY
CONTAIN CONFIDENTIAL TRADE SECRET INFORMATION. IT IS LICENSED FOR USE ON THE
DESIGNATED EQUIPMENT ON WHICH IT WAS ORIGINALLY INSTALLED AND  MAY NOT BE
MODIFIED, DUPLICATED OR COPIED IN ANY FORM WITHOUT PRIOR WRITTEN CONSENT OF

			Exploration Systems, Inc.
			579 Market Street
			San Francisco, CA  USA 94105
			(415) 974-1600

(The above notice does not constitute an admission of publication or
unrestricted dissemination of the work)

******************************************************************************/


#include "esi_c_lib.h"

#ifndef ESI_XS_DEFS_H
#include "esi_xs_defs.h"
#endif

#ifndef ESI_XS_GR_H
#include "esi_xs_gr.h"
#endif

#ifndef ESI_WM_CLS_H
#include "esi_wm_cls.h"
#endif

#ifndef ESI_GL_TXDEF_H
#include "esi_gl_txdef.h"
#endif

#ifndef ESI_QS_H
#include "esi_qs.h"
#endif

#ifndef ESI_TC_H
#include "esi_tc.h"
#endif

#ifndef ESI_TG_H
#include "esi_tg.h"
#endif

#ifndef ESI_AM_H
#include "esi_am.h"
#endif

#ifndef ESI_GL_PROJECT_H
#include "esi_gl_project.h"
#endif

#ifndef ESI_LU_H
#include "esi_lu.h"
#endif

#ifndef ESI_UG_H
#include "esi_ug.h"
#endif

#ifndef ESI_HO_FILES_H
#include "esi_ho_files.h"
#endif

#if USE_PROTOTYPES

static INT xsz_draw_section (XS_STRUCT *pxs);
static INT xsz_draw_plot (XS_STRUCT *pxs, CHAR *metafile);
static INT zz_draw_marks (XS_STRUCT *pxs, LAYOUT_STRUCT *pls, TRACK_STRUCT *pt);
static INT zz_draw_detail_marks (XS_STRUCT *pxs, LAYOUT_STRUCT *pls,TRACK_STRUCT *pt);

#else	    /* no prototypes */

static INT xsz_draw_section ();
static INT xsz_draw_plot ();
static INT zz_draw_marks ();
static INT zz_draw_detail_marks ();

#endif	    /* of #if USE_PROTOTYPES */


/******************************************************************
* Copyright 1987 Exploration Systems Inc.  Sausalito CA. 94965
*
* Cross Section Graphic Toolbox. 
*
* This module contains the routines neccessary to maintain and
* generate the various graphics for the Cross Section/Log Analysis
* system.
*
* All of the routines are of type INT and return either SUCCESS or
* FAIL (unless otherwise noted).
*
* Julian Carlisle   02/24/87
* James Chang       07/29/87
******************************************************************/

/* declare types of functions in this module */

/*** Some local definitions ***/
#define DEBUG 1
#define DO_SEGS  1
#define DRAW_SCALE_TICKS 0
#define DRAW_BORDERS 0
#define TWO_HEAD_SUPPORT  TRUE

/* this minor interdependancy is allowed for convenience */
/* any routine in this module may reference these */
static INT xs_tnr = 0;
static INT xs_wid = 0;


/******************************************************************
* Copyright 1987 Exploration Systems Inc.  Sausalito CA. 94965
*
* Cross Section Graphic Toolbox. 
*
* Initialize a graphics window for the cross section display
*
* INPUT        - Pointer to xs_struct
* OUTPUT    - SUCCESS, FAIL
*
******************************************************************/
publicdef INT xs_gr_open_window(pxs,ptnr,pwid,metafile)
XS_STRUCT    *pxs;
INT        *ptnr;        /* returned id for xs window xform */
INT        *pwid;        /* returned id for xs window */
CHAR       *metafile;    /* metafile pointer */
{

INT		tstatus,
	    wstatus,
	    status = SUCCESS;

INT batch   = 1;
COLOR_INDEX   bgColor;
GKS_REAL    ndcMinX,	    /* normalized device coords */
		    ndcMaxX,
		    ndcMinY,
		    ndcMaxY;

DOUBLE      nwcMinX,	    /* normalized window coords */
		    nwcMaxX,
		    nwcMinY,
		    nwcMaxY;

DOUBLE      wcsMinX,	    /* window coordinate system window coords */
		    wcsMaxX,
		    wcsMinY,
		    wcsMaxY;

GKS_REAL rwindow[4], cwindow[4], rviewp[4], cviewp[4];
GKS_INT wkid = 1, errind = 0, tus;
FLOAT ll_dev_x;
FLOAT ll_dev_y;
FLOAT ur_dev_x;
FLOAT ur_dev_y;
DOUBLE vdc_extent_xmin;
DOUBLE vdc_extent_xmax;
DOUBLE vdc_extent_ymin;
DOUBLE vdc_extent_ymax;
DOUBLE user_unit_to_mm;
DOUBLE scale_factor;
DOUBLE temp_ndc;
PATHNAME param_file;
CHAR *pdot;
FILE *fd;
LINE_ATTRIBUTE line;

/* determine the extents of this workstation */
wm_inq_wkwn(&ndcMinX, &ndcMaxX, &ndcMinY, &ndcMaxY);
#if 0
/* doesn't return the correct values 4095 instead of 32767 */ 
wm_inq_wkvp_max(&ll_dev_x,&ur_dev_x,&ll_dev_y,&ur_dev_y); 
#else   
/* hard code for now */
/* assign only for ur_dev_x as it's only the one being used for scaling */
#ifdef SUN_GKS_OR_PRIOR_GKS
   ur_dev_x = 32767.0;
#else
   ur_dev_x = 1.0;
#endif
#endif

qs_inq_i(QS_IF_BATCH, &batch);
if( batch )
{
    ndcMinX = ndcMinY = 0.0;
	if (metafile EQUALS (CHAR *) NULL)
	  {
      temp_ndc = MAX(pxs->drawing.width,pxs->drawing.height);
      ndcMaxX = ndcMaxX * (pxs -> drawing.width / temp_ndc);
      ndcMaxY = ndcMaxY * (pxs -> drawing.height / temp_ndc);
      }	  
    else
	  {
      temp_ndc = MAX(pxs->drawing.width,pxs->drawing.height);
      ndcMaxX = pxs -> drawing.width / temp_ndc;
      ndcMaxY = pxs -> drawing.height / temp_ndc;
      strcpy(param_file,metafile);
	  pdot = strchr(param_file,'.') + 1;
	  strcpy (pdot,"PAR");
	  fd = fopen(param_file,"w");
          status = ho_lock(fd, ON);
	  if (fd == NULL_FILE_PTR || status != SUCCESS)
	      {
	      printf ("Parameter file %s cannot open\n",param_file);
	      return FAIL;
	      }
	      
            /* get the normalized range of the metafile */
          user_unit_to_mm = 1.0;
		  tu_unit_conversion(user_unit_to_mm,"MM",&user_unit_to_mm,"INCHES");
	    scale_factor =pxs->drawing.width/user_unit_to_mm/(ndcMaxX*ur_dev_x);
            printf("Metafile %s\n",metafile);
            printf("Parameter file %s\n",param_file);

            fprintf (fd,"vdc_extent(0.0,0.0,%lf,%lf)\n",ndcMaxX*ur_dev_x,
							ndcMaxY*ur_dev_x);
            fprintf (fd,"vdc_viewport(0.0,0.0,%lf,%lf)\n",
				   pxs->drawing.width/user_unit_to_mm,
				   pxs->drawing.height/user_unit_to_mm);
            status = ho_lock(fd, OFF);
            fclose(fd);
            printf ("vdc_viewport (mm): width %lf, height %lf\n",
				   pxs->drawing.width/user_unit_to_mm,
				   pxs->drawing.height/user_unit_to_mm);
            printf ("vdc_extent(0.0,0.0,%lf,%lf)\n",ndcMaxX*ur_dev_x,
						    ndcMaxY*ur_dev_x);
	    printf ("scale factor (mm/vdc) :%lf\n",scale_factor);
          }

    wstatus = wmwallocate(&xs_wid,ndcMinX,ndcMaxX,ndcMinY,ndcMaxY, -1);
    line.style = 1;
    line.width = 1.0;
    lu_text_to_index ("COLOR",&line.color,"BLACK");
    wm_draw_window_border (xs_wid,&line);
}
else
{
    wmwclose(pxs->drawing.window_id);   /* close the old window before each draw*/
    wmwdelete(pxs->drawing.window_id);  /* and delete it */

#if TWO_HEAD_SUPPORT == TRUE
    if (ug_if_gpx() == FALSE)
	wm_set_current_screen( 2 );	/* assign to RIGHT screen, if available */
#endif

    lu_text_to_index("COLOR", (INDEX *)&bgColor, "BACKGROUND" );
    wstatus = wmwallocate(&xs_wid,ndcMinX,ndcMaxX,ndcMinY,ndcMaxY, bgColor);
    line.style = 1;
    line.width = 1.0;
    lu_text_to_index ("COLOR",&line.color,"BLACK");
    wm_draw_window_border (xs_wid,&line);
}

if (wstatus != SUCCESS)
    {
    status = FAIL;
    }


wcsMinX = pxs->drawing.xorigin;
wcsMaxX = pxs->drawing.width;

wcsMinY = pxs->drawing.yorigin; 
wcsMaxY = pxs->drawing.height;

nwcMinX = 0.000;	/* full screen */
nwcMaxX = 1.00;

nwcMinY = 0.000;
nwcMaxY = 1.00 ;

tstatus = wmtallocate(&xs_tnr, nwcMinX,nwcMaxX,nwcMinY,nwcMaxY,
               wcsMinX,wcsMaxX,wcsMinY,wcsMaxY);

if (tstatus != SUCCESS)
    {
    status = FAIL;
    }

*ptnr = xs_tnr;		/* pass the new i.d.s back to caller */
*pwid = xs_wid;

return status;
}




/*************************************************
*
* Draw the cross section appropriately based
* on Interactive or Batch execution mode.
*               (PLOTTING)
* Julian Carlisle   may 9, 1988
*************************************************/
INT xs_gr_draw_section(pxs,metafile)
XS_STRUCT *pxs;
CHAR *metafile;
{
BOOL batch = FALSE;
INT status = SUCCESS;

qs_inq_i(QS_IF_BATCH, &batch);

status = (batch) ? xsz_draw_plot(pxs,metafile) : xsz_draw_section(pxs);
return status;
}











/******************************************************************
* Copyright 1987 Exploration Systems Inc.  Sausalito CA. 94965
*
* Cross Section Graphic Toolbox. - Main graphics entry point 
*
* XSZ_DRAW_SECTION	-  Draw the cross section from the xs_struct
*
* This routine draws the cross section as described by the data in
* the Cross Section object: pxs.  When this call is made, it is
* assumed that all of the formatting, data loading etc. is complete
* and represented in the object.
*
* Input		- Pointer to xs_struct
* Output	- A Cross Section Display.
* Returns	- SUCCESS unless object is incomplete or invalid.
*
******************************************************************/
static INT xsz_draw_section(pxs)
XS_STRUCT    *pxs;      /* the main Cross Section object */
{
LAYOUT_STRUCT  *pls;
INT n,status;
INT nlayouts;

TNR_ID 	tnr;            /* Transformation id. */
WINDOW_ID wid;            /* Window id. */
COLOR_INDEX  normalColor;
XS_DIS *pdraw;      /* Drawinfo struct.  See ESI_XS_GR.H for desc. */
GKS_INT  defmod, regmod;
DOUBLE min_char_height;


/****************************************************************/

/* CLOSE THE OLD WINDOW AND OPEN A NEW ONE */
status = xs_gr_open_window(pxs,&tnr,&wid,(CHAR *)0);                    
if (status == FAIL)
	{
	return FAIL;
	}

/* Save old deferral mode, set new one. */
wm_inq_deferral(&defmod, &regmod);
wm_hold_updates();  /* ASAP, IRG_SUPPRESSED */

/* PROPAGATE THE XFORM & WINDOW IDS BACK TO THE MAIN OBJECT */
pxs->drawing.tnr_id = tnr;
pxs->drawing.window_id = wid;


nlayouts = llcount(&pxs->drawing.log_plots);	/* COUNT WELLS & CHECK LISTS */
if (nlayouts <= 0)
    return FAIL;

status = wmtselect(pxs->drawing.tnr_id); /* select XS xform */

/* Fill out the draw_info structure once just before drawing */
/* This info is used, i.e. by the Scale Block code, to speed */
/* things up by supplying oft-sought, pain-to-reach trivia */
/* regarding the forthcoming Cross Section Display. */

pdraw = (XS_DIS *)tc_zalloc(sizeof(XS_DIS));
if ((status = xs_gr_get_drawinfo(pxs,pdraw)) == FAIL)
    return FAIL;

status = xs_get_hdr_min_height(pxs, &min_char_height);

/* LOOP ON WELLS DRAWING EACH ONE AND IT'S COMPONENTS */
for (n = 0; n < nlayouts; n++)
    {
    status = (n EQUALS 0) ? llfirst(&pxs->drawing.log_plots, (BYTE *)&pls) :
                            llnext(&pxs->drawing.log_plots, (BYTE *)&pls);

    pls->log_hdr.tnr_id = pxs->drawing.tnr_id;	/* PROPAGATE XFORM ID */
/*****************************************
    if (pls->group_id != 0)		
	status = wmgdelete(pls->group_id);
	
    status = wmgallocate(&pls->group_id); 
******************************************/
        
/*****************************
*
*    DRAW THE TEXT BLOCK 
*
*****************************/	
    if (pls->text_block.visible EQUALS TRUE)
	{
	if (pls->text_block.seg_id != 0)
	    status = wmsdelete(pls->text_block.seg_id);

	status = wmsallocate(&pls->text_block.seg_id);
	status = xs_gr_draw_text_blk(pxs,pls);	
	status = wmsclose(pls->text_block.seg_id);
	wmoset(pls->text_block.seg_id, WM_CLASS_SEGMENT, "","Text Block");
	}


/**********************************
*
*    DRAW TRACKS AND TRACES
*
**********************************/	

    status = xs_gr_draw_tracks(pxs,pls,FALSE);	/* DO ALL TRACKS ON THIS WELL */
    status = xs_gr_draw_traces(pxs,pls);	/* AND ALL THE TRACES */


/*********************************
*
*    DRAW THE TRACE SCALE BLOCK
*
*********************************/	
    if (pls->scale_block.visible EQUALS TRUE)
	{
	if (pls->scale_block.seg_id != 0)
	    status = wmsdelete(pls->scale_block.seg_id);

	status = wmsallocate(&pls->scale_block.seg_id);
	status = lg_gr_plot_draw_scale(pxs,pls,pdraw);    
	status = wmsclose(pls->scale_block.seg_id);
	wmoset(pls->scale_block.seg_id, WM_CLASS_SEGMENT, "","Scale Block");
	}


/********************************
*
*    DRAW THE LOG HEADER BLOCK
*
*********************************/	
    if (pls->log_hdr.visible EQUALS TRUE)
	{
	if (pls->log_hdr.seg_id != 0)
	    status = wmsdelete(pls->log_hdr.seg_id);

	status = wmsallocate(&pls->log_hdr.seg_id);
	status = xs_gr_plot_draw_hdr(pxs, pls, min_char_height);	
	status = wmsclose(pls->log_hdr.seg_id);
	wmoset(pls->text_block.seg_id, WM_CLASS_SEGMENT, "","Log Header Block");
	}
	
	
/****************************************************
    status = wmgclose(pls->group_id);
    sprintf(groupid,"well: %s", pls->pwell->uwi);
    status = wmoset(pls->group_id, WM_CLASS_GROUP, "",groupid);
***************************************************/

    status = wmtselect(pxs->drawing.tnr_id); /* RESTORE TRANSFORMATION */


/*************************************************************
*
* IF COMPILED IN, DRAW A FANCY BORDER AROUND THE LAYOUT 
*
**************************************************************/
#if DRAW_BORDERS == TRUE
    bminx = pls->xorigin;
    bmaxx = pls->width + bminx;

    bminy = pls->text_block.yorigin;
    bmaxy = pls->log_hdr.yorigin + pls->log_hdr.height;

    bdx = bmaxx - bminx;
    bdy = bmaxy - bminy;

    bminx -= (bdx * BORDER);
    bmaxx += (bdx * BORDER);

    bminy -= (bdx * BORDER);
    bmaxy += (bdx * BORDER);

    lu_text_to_index( "COLOR", (INDEX *)&normalColor, "NORMAL" );

    status = xs_gr_rect(bminx,bminy,bmaxx,bmaxy,normalColor, 2.0);
#endif

    }        

if (pxs->xs_suppress_vbar EQUALS FALSE)
  status = xs_gr_vbar(pxs);	/* DRAW SCALE BAR IF NOT NO_DATUM SECTION */

status = xsz_display_tops(pxs, min_char_height);
                                /* DONE WITH LAYOUTS.  DRAW THE VISIBLE TOPS */
/* --------------------------- */
/* The following update should */
/* not be required             */
/* --------------------------- */
/* wm_update(); */


/* CLOSE AND NAME THE WINDOW */
status = wmwclose(wid);	
status = wmoset(wid, WM_CLASS_WINDOW, "","Cross Section Window");

wm_set_deferral(defmod, regmod);

return SUCCESS;
}


/******************************************************************
* Copyright 1987 Exploration Systems Inc.  Sausalito CA. 94965
*
* Cross Section Graphic Toolbox. 
*
* PLOT THE DISPLAY....Just like display above but does'nt use
* groups or extra segments.
*
* May 9, 1988     Julian Carlisle
******************************************************************/
static INT xsz_draw_plot(pxs,metafile)
XS_STRUCT    *pxs;      /* the main Cross Section object */
CHAR *metafile;
{
LAYOUT_STRUCT  *pls;
INT	n = 0,
	status = SUCCESS,
	nlayouts = 0;
DOUBLE  min_char_height;
XS_DIS  *pdraw;      /* Drawinfo struct.  See ESI_XS_GR.H for desc. */


/****************************************************************/


/* CLOSE THE OLD WINDOW AND OPEN A NEW ONE */
status = xs_gr_open_window(pxs,&pxs->drawing.tnr_id,&pxs->drawing.window_id,
                                                                   metafile);
if (status == FAIL)
	{
	return FAIL;
	}

nlayouts = llcount(&pxs->drawing.log_plots);	/* COUNT WELLS & CHECK LISTS */
if (nlayouts <= 0)
    return FAIL;

/*
*** FILL OUT THE DRAW_INFO STRUCTURE ONCE JUST BEFORE DRAWING 
*/
pdraw = (XS_DIS *)tc_zalloc(sizeof(XS_DIS));
if ((status = xs_gr_get_drawinfo(pxs,pdraw)) == FAIL)
    return FAIL;

status = xs_get_hdr_min_height(pxs, &min_char_height);

/*
*** LOOP ON WELLS DRAWING EACH ONE AND IT'S COMPONENTS 
*/
for (n = 0; n < nlayouts; n++)
    {
    status = (n EQUALS 0) ? llfirst(&pxs->drawing.log_plots, (BYTE *)&pls) :
                            llnext(&pxs->drawing.log_plots, (BYTE *)&pls);

    pls->log_hdr.tnr_id = pxs->drawing.tnr_id;	/* PROPAGATE XFORM ID */

			    /* EACH WELL IN ONE SEGMENT FOR PLOTTING */
    status = wmsallocate(&pls->seg_id);
    status = wmsvis(pls->seg_id, GVISI);
	
	        
/*
**** DRAW THE TEXT BLOCK *****
*/	
    if (pls->text_block.visible EQUALS TRUE)
	{
	status = xs_gr_draw_text_blk(pxs,pls);	
	}



/*
****    DRAW TRACKS AND TRACES
*/	
    status = xs_gr_draw_tracks(pxs,pls,TRUE);	/* DO ALL TRACKS ON THIS WELL */
    status = xs_gr_draw_traces(pxs,pls);	/* AND ALL THE TRACES */



/*
****  DRAW THE TRACE SCALE BLOCK
*/	
    if (pls->scale_block.visible EQUALS TRUE) 
	{
	status = lg_gr_plot_draw_scale(pxs,pls,pdraw);    
	}


/*
****  DRAW THE LOG HEADER BLOCK
*/	
    if (pls->log_hdr.visible EQUALS TRUE)
	{
	status = xs_gr_plot_draw_hdr(pxs, pls, min_char_height);	
	}
	
	
    status = wmsclose(pls->seg_id);    
    }        


status = xsz_display_tops(pxs, min_char_height);	
                                /* DONE WITH LAYOUTS.  DRAW THE VISIBLE TOPS */
status = xs_gr_vbar(pxs);	/* DRAW SCALE BAR IF NOT NO_DATUM SECTION */


/* CLOSE AND NAME THE WINDOW */
status = wmwclose(pxs->drawing.window_id);	
status = wmoset(pxs->drawing.window_id, WM_CLASS_WINDOW, "","Cross Section Window");

return SUCCESS;

}






/******************************************************************
* Copyright 1987 Exploration Systems Inc.  Sausalito CA. 94965
*
* Cross Section Graphic Toolbox. 
*
* Draw a text block
*
* INPUT        - Pointer to layout_struct
* OUTPUT    - SUCCESS, FAIL
*
******************************************************************/
publicdef INT xs_gr_draw_text_blk(pxs,pls)
XS_STRUCT *pxs;
LAYOUT_STRUCT *pls;
{
DOUBLE    tb_xmin,tb_xmax,tb_ymin,tb_ymax;

wmtselect(pxs->drawing.tnr_id);
tb_xmin = pls->text_block.xorigin;
tb_ymin = pls->text_block.yorigin;
tb_xmax = tb_xmin + pls->text_block.width;
tb_ymax = tb_ymin + pls->text_block.height;

xs_gr_rect(tb_xmin,tb_ymin,tb_xmax,tb_ymax,pls->text_block.line.color,
pls->text_block.line.width);

return SUCCESS;
}



/**************************************************************
* Copyright 1987, Exploration Systems Inc. Sausalito CA. 94965
*
* PROGRAM:  xs_gr_draw_marks()   -  Draw Log Track Markings.
*
* This is an optimized version of, and intended replacement
* for, xs_gr_draw_marks.
*
* Julian   Aug 12, 1987
**************************************************************/
publicdef INT xs_gr_draw_marks(pxs,pls,pt)
XS_STRUCT	*pxs;
LAYOUT_STRUCT   *pls;
TRACK_STRUCT    *pt;
{
BOOL batch;
INT status;

qs_inq_i(QS_IF_BATCH, &batch);
status = (batch) ? zz_draw_detail_marks(pxs,pls,pt) :
                   zz_draw_marks(pxs,pls,pt);

return status;
}

/*************************************
*
* Quick mark drawing routine. no frills.
*
**************************************/
static INT zz_draw_marks(pxs, pls, pt)
XS_STRUCT	*pxs;
LAYOUT_STRUCT   *pls;
TRACK_STRUCT    *pt;
{
static FLOAT	*xc,*yc,*xc2;			/* to be allocated: Left & Right coord arrays */
FLOAT  	*px,*py,*px2;			/* Pointers to above arrays */

INT	depth,ibase,itop,interval;	

DOUBLE	sp,gap,tick_width,
	xmin,xmax,ymin,ymax,
	ltick_end,rtick_start;

DOUBLE	pix_per_ft;
		
INT	npoints;
static BOOL first_time = TRUE;

PROJECT_DEFAULT *proj;
CHAR stored_units[32];
CHAR interval_units[32];
DOUBLE top, base;

/* allocate the coord arrays. they are re-used. */
if (first_time)
	{
	xc = (FLOAT *)tc_alloc(sizeof(FLOAT) * 8190);
	yc = (FLOAT *)tc_alloc(sizeof(FLOAT) * 8190);
	xc2 = (FLOAT *)tc_alloc(sizeof(FLOAT) * 8190);
	first_time = FALSE;
	}

am_open_workspace ("PROJECT_DEFAULTS", AM_GLOBAL, (VOIDPTR *)&proj);
strcpy (stored_units, proj->unit.vertical);
switch (pxs->depth_interval_scale_units)
  {
  case XS_INCH_UNITS:
    strcpy (interval_units, "FEET");
    break;
  case XS_CENTIMETER_UNITS:
  default:
    strcpy(interval_units,"METERS");
    break;
  }

tu_unit_conversion (pls->interval.track_top, stored_units,
		    &top, interval_units);
tu_unit_conversion (pls->interval.track_base, stored_units,
		    &base, interval_units);
itop = (INT)top;
ibase = (INT)base;

pix_per_ft = pt->length / (base - top);
						   
xmin = pt->xorigin;
xmax = xmin + pt->width;

ymin = pt->yorigin;
ymax = pt->length + ymin;

/* ticks vary in size with the track width */
tick_width = (xmax - xmin) * XTICKSCALE;
if(pt->tertiary.type != NONE && pt->tertiary.frequency > 0.0)
	{
	interval = (INT)pt->tertiary.frequency;
	depth = ((ibase / interval) * interval) -interval;

	gap = pt->tertiary.frequency * pix_per_ft;

	wm_gsplci(pt->tertiary.line.color);
	wm_gsln(pt->tertiary.line.style);
	wm_gslwsc(pt->tertiary.line.width);

	if (pt->tertiary.type EQUALS LINES)
		{
		px = xc;	py = yc;
		npoints = 0;
		for(sp = ymax - ((depth-itop) * pix_per_ft);
			sp < ymax;
			sp += gap,npoints += 2)
			{
			*px = xmin;	*py = sp;
			*(px + 1) = xmax;	*(py + 1) = sp;

			sp += gap;
			npoints += 2;			
			if (sp >= ymax)  break;
			*(px + 2) = xmax;	*(py + 2) = sp;
			*(px + 3) = xmin;	*(py + 3) = sp;
			px += 4;			
			py += 4;
			}
		if (npoints > 1)
		    wm_gpl(npoints, xc, yc);
		}
	else if (pt->tertiary.type EQUALS TICS)
		{
		ltick_end = xmin + tick_width;
		rtick_start = xmax - tick_width; 
		npoints = 0;

		px = xc;
                py = yc;
                px2 = xc2;
		for(sp = ymax - ((depth-itop) * pix_per_ft);
		    sp < ymax;
		    sp += gap,npoints += 3)
			{
			*px = xmin;
			*(px + 1) = ltick_end;
			*(px + 2) = xmin;
			px += 3;
			*px2 = xmax;
			*(px2 + 1) = rtick_start;
			*(px2 + 2) = xmax;
			px2 += 3;
			
			*py = *(py + 1) = *(py + 2) = sp;
			py += 3;
			}
		if (npoints > 1)
			{
			wm_gpl(npoints, xc, yc);
			wm_gpl(npoints, xc2, yc);
			}
		}
	}		
				   
/* now do secondary marks */
if(pt->secondary.type != NONE && pt->secondary.frequency > 0.0)
	{
	interval = (INT)pt->secondary.frequency;
	depth = ((ibase / interval) * interval) -interval;


	gap = pt->secondary.frequency * pix_per_ft;
	if (pt->secondary.line.style != pt->tertiary.line.style)
	    wm_gsln(pt->secondary.line.style);

	if (pt->secondary.line.color != pt->tertiary.line.color)
	    wm_gsplci(pt->secondary.line.color);

	if (pt->secondary.line.width != pt->tertiary.line.width)
	    wm_gslwsc(pt->secondary.line.width);

	if (pt->secondary.type EQUALS LINES)
		{
		px = xc;	py = yc;
		npoints = 0;
		for(sp = ymax - ((depth-itop) * pix_per_ft);
			sp < ymax;
			sp += gap,npoints += 3)
			{
			*px = xmin;
			*(px + 1) = xmax;
			*(px + 2) = xmin;
			px += 3;
			*py = *(py + 1) = *(py + 2) = sp;
			py += 3;
			}
		if (npoints > 1)
			wm_gpl(npoints, xc, yc);
		}
	else if (pt->secondary.type EQUALS TICS)
		{
		ltick_end = xmin + tick_width;
		rtick_start = xmax - tick_width; 
		npoints = 0;

		px = xc;	py = yc; px2 = xc2;
		for(sp = ymax-((depth-itop)*pix_per_ft);
		    sp < ymax;
			sp += gap,npoints += 3)
			{
			*px = xmin;
			*(px + 1) = ltick_end;
			*(px + 2) = xmin;
			px += 3;
			*px2 = xmax;
			*(px2 + 1) = rtick_start;
			*(px2 + 2) = xmax;
			px2 += 3;
			
			*py = *(py + 1) = *(py + 2) = sp;
			py += 3;
			}
		if (npoints > 1)
		    {	
		    wm_gpl(npoints, xc, yc);
		    wm_gpl(npoints, xc2, yc);
		    }
		}
	}		
				   


/* now do primary marks */
if(pt->primary.type != NONE && pt->primary.frequency > 0.0)
	{
	interval = (INT)pt->primary.frequency;
	depth = ((ibase / interval) * interval);


	gap = pt->primary.frequency * pix_per_ft;
	if (pt->primary.line.style != pt->secondary.line.style)
	    wm_gsln(pt->primary.line.style);

	if (pt->primary.line.color != pt->secondary.line.color)
	    wm_gsplci(pt->primary.line.color);

	if (pt->primary.line.width != pt->secondary.line.width)
	    wm_gslwsc(pt->primary.line.width);

	if (pt->primary.type EQUALS LINES)
		{
		px = xc;	py = yc;
		npoints = 0;
		for(sp = ymax-((depth-itop)*pix_per_ft);
			sp < ymax;
			sp += gap,npoints += 3)
			{
			*px = xmin;
			*(px + 1) = xmax;
			*(px + 2) = xmin;
			px += 3;
			*py = *(py + 1) = *(py + 2) = sp;
			py += 3;
			}
		if (npoints > 1)
			wm_gpl(npoints, xc, yc);
		}
	else if (pt->primary.type EQUALS TICS)
		{
		ltick_end = xmin + tick_width;
		rtick_start = xmax - tick_width; 
		npoints = 0;

		px = xc;	py = yc; px2 = xc2;
		for(sp = ymax-((depth-itop)*pix_per_ft);
		    sp < ymax;
			sp += gap,npoints += 3)
			{
			*px = xmin;
			*(px + 1) = ltick_end;
			*(px + 2) = xmin;
			px += 3;
			*px2 = xmax;
			*(px2 + 1) = rtick_start;
			*(px2 + 2) = xmax;
			px2 += 3;
			
			*py = *(py + 1) = *(py + 2) = sp;
			py += 3;
			}
		if (npoints > 1)
			{
			wm_gpl(npoints, xc, yc);
			wm_gpl(npoints, xc2, yc);
			}
		}
	}		
				   
return SUCCESS;
}


/*************************************
*
* This is the batch mode mark draw rtn.
*
* It draws slightly nicer, but much slower.
* Note: I just plugged the old routine in 
* here. It should probably be cleaned up.
********************************************/
static INT zz_draw_detail_marks(pxs, pls, pt)
XS_STRUCT	*pxs;
LAYOUT_STRUCT   *pls;
TRACK_STRUCT    *pt;
{
DOUBLE	xmin,
	xmax,
	ymin,
	ymax;
     
DOUBLE	sp,
	gap,
	tick_width;

GKS_REAL	xarray[5];

DOUBLE	top,
	base,
	mark_freq,
	total_depth;

DOUBLE	pix_per_ft;

INT	depth,
	label_freq,
	itop,
	ibase;

PROJECT_DEFAULT *proj;
CHAR stored_units[32];
CHAR interval_units[32];

/* top & base depth plus their integer counter parts */
am_open_workspace ("PROJECT_DEFAULTS", AM_GLOBAL, (VOIDPTR *)&proj);
strcpy (stored_units, proj->unit.vertical);
switch (pxs->depth_interval_scale_units)
  {
  case XS_INCH_UNITS:
    strcpy (interval_units, "FEET");
    break;
  case XS_CENTIMETER_UNITS:
  default:
    strcpy(interval_units,"METERS");
    break;
  }

tu_unit_conversion (pls->interval.track_top, stored_units,
		    &top, interval_units);
tu_unit_conversion (pls->interval.track_base, stored_units,
		    &base, interval_units);
itop = (INT)top;		/* integer for modulo operation later */
ibase = (INT)base;

/* our total depth range to work with */
total_depth = base - top;

/* a depth to screen pos. transform */
pix_per_ft = pt->length / total_depth;

/* initialize the positioning vars. */
xmin = pt->xorigin;
ymin = pt->yorigin;

xmax = pt->width + xmin;
ymax = pt->length + ymin;

/* create a border around the edges of the track for neatness */
xmin  += ((xmax - xmin) * 0.02);
xmax -=  ((xmax - xmin) * 0.02);

/* determine the size of each tick */
tick_width = (xmax - xmin) * XTICKSCALE;
 
xarray[0] = xmin;
xarray[1] = xmax;


if(pt->tertiary.type != NONE)
{
/* tertiary marks */
mark_freq = pt->tertiary.frequency;
label_freq = (INT)pt->tertiary.frequency;

/* loop to get a rounded depth to start with */
for (depth = ibase; depth > itop; depth -= 1)
    {
    sp = ymax - ((depth - itop) * pix_per_ft);
    if (NOT(depth % label_freq))
        break;
    }

gap = mark_freq * pix_per_ft;

wm_gsln(pt->tertiary.line.style);
wm_gsplci(pt->tertiary.line.color);
wm_gslwsc(pt->tertiary.line.width);
for (; sp < ymax; sp += gap)
    {
    if(pt->tertiary.type EQUALS LINES)
        {
	xs_gr_drawline(xmin,sp,xmax,sp);
        }
    else
        {
	xs_gr_drawline(xmin,sp,(xmin+tick_width),sp);
	xs_gr_drawline((xmax-tick_width),sp,xmax,sp);
        }
    }
}
if(pt->secondary.type != NONE)
{
/* secondary */
mark_freq =  pt->secondary.frequency;
label_freq = (INT)pt->secondary.frequency;
for (depth = ibase; depth > itop; depth -= 1)
    {
    sp = ymax - ((depth - itop) * pix_per_ft);
    if (NOT(depth % label_freq))
	{
        break;
	}
    }

gap = mark_freq * pix_per_ft;
if (pt->secondary.line.style != pt->tertiary.line.style)
    wm_gsln(pt->secondary.line.style);

if (pt->secondary.line.color != pt->tertiary.line.color)
    wm_gsplci(pt->secondary.line.color);

if (pt->secondary.line.width != pt->tertiary.line.width)
    wm_gslwsc(pt->secondary.line.width);

for (; sp < ymax; sp += gap)
    {
    if(pt->secondary.type EQUALS LINES)
        {
	xs_gr_drawline(xmin,sp,xmax,sp);
        }
    else
        {
	xs_gr_drawline(xmin,sp,(xmin+tick_width),sp);
	xs_gr_drawline((xmax-tick_width),sp,xmax,sp);
        }
    }
}
if(pt->primary.type != NONE)
{
/* primary marks */
mark_freq = pt->primary.frequency;
label_freq = (INT)pt->primary.frequency;
for (depth = ibase; depth > itop; depth -= 1)
    {
    sp = ymax - ((depth - itop) * pix_per_ft);
    if (NOT(depth % label_freq))
	{
        break;
	}
    }

gap = mark_freq * pix_per_ft;
if (pt->primary.line.style != pt->secondary.line.style)
    wm_gsln(pt->primary.line.style);

if (pt->primary.line.color != pt->secondary.line.color)
    wm_gsplci(pt->primary.line.color);

if (pt->primary.line.width != pt->secondary.line.width)
    wm_gslwsc(pt->primary.line.width);

for (; sp < ymax; sp += gap)
    {
    if(pt->primary.type EQUALS LINES)
        {
	xs_gr_drawline(xmin,sp,xmax,sp);
        }
    else
        {
	xs_gr_drawline(xmin,sp,(xmin+tick_width),sp);
	xs_gr_drawline((xmax-tick_width),sp,xmax,sp);
        }
    }
}
/* finit */
return SUCCESS;
}








/*****************************************************************
* Copyright 1987 Exploration Systems Inc.  Sausalito CA. 94965
*
* Cross Section Graphic Toolbox. 
*
* XS_GR_DRAW_TICKS - take a ticks array and draw it
*
* This routine is used to draw the markings on the tracks.
* the pticks arg points to an array of X values for each
* division.
*
* Julian Carlisle
*****************************************************************/
publicdef INT xs_gr_draw_ticks(pxs,pls,pticks, ymin,ymax,pt)
XS_STRUCT  *pxs;
LAYOUT_STRUCT *pls;
FLOAT  *pticks;
DOUBLE  ymin,ymax;
TRACK_STRUCT  *pt;
{
GKS_REAL	xarray[300],
		yarray[300];
GKS_REAL	*px,*py;

GKS_INT npoints;

/* set the GKS attributes within the line description structure */
wm_gsln(pt->divisions.line.style);
wm_gsplci( pt->divisions.line.color);
wm_gslwsc(pt->divisions.line.width);

npoints = 0;

for(px = xarray, py = yarray; *pticks > 0; pticks++,npoints += 3)
    {
    /* draw track division ticks */
    *px++ = *pticks;
    *px++ = *pticks;
    *px++ = *pticks;
    *py++ = ymax;
    *py++ = ymin;
    *py++ = ymax;
    }
wm_gpl(npoints,xarray,yarray);    
return SUCCESS;
}




/******************************************************************
* Copyright 1987 Exploration Systems Inc.  Sausalito CA. 94965
*
* Cross Section Graphic Toolbox. 
*
* Draw a rectangle 
*
******************************************************************/
publicdef INT xs_gr_rect(xmin,ymin,xmax,ymax,color,thickness)
DOUBLE xmin,ymin,
         xmax,ymax;
	 
COLOR_INDEX      color;
DOUBLE   thickness;
{
GKS_REAL    xpoints[5], ypoints[5];

xpoints[0] = (GKS_REAL)xmin;
ypoints[0] = (GKS_REAL)ymin;

xpoints[1] = (GKS_REAL)xmin;
ypoints[1] = (GKS_REAL)ymax;

xpoints[2] = (GKS_REAL)xmax;
ypoints[2] = (GKS_REAL)ymax;

xpoints[3] = (GKS_REAL)xmax;
ypoints[3] = (GKS_REAL)ymin;

xpoints[4] = (GKS_REAL)xmin;
ypoints[4] = (GKS_REAL)ymin;

wm_gsln(1);
wm_gsplci(color);
wm_gslwsc(thickness);
wm_gpl(5,xpoints,ypoints);

return SUCCESS;

}


/******************************************************************
* Copyright 1987 Exploration Systems Inc.  Sausalito CA. 94965
*
* Cross Section Graphic Toolbox. 
*
* Set up the GKS text defaults for the cross section
*
* This routine sets the text alignment to normal upright left-
* to-right drawing, the character spacing factor to 0.0 and
* installs the height, width & color values passed in.
*
* Set the 'center' argument to TRUE to use stroke precision text
* centering.
*
* JSC 
******************************************************************/
publicdef INT xs_gr_set_text(height,width,color,center)
DOUBLE height,width;
COLOR_INDEX color;
BOOL center;        /* if TRUE text will be drawn centered */
{
DOUBLE   text_x3d, 
        text_y3d;
static INT if_gpx = -1;

if (if_gpx != TRUE AND if_gpx != FALSE)
    if_gpx = ug_if_gpx();
    
wm_gschh(height);	/* set char height */
if ( width <= 0.0)
    width = 0.125;
 
wm_gschxp(width);	/* and width */
wm_gstxci(color);	/* and color */


text_x3d = 0.0;     /* normally aligned text */
text_y3d = 1.0;
wm_gschup(text_x3d, text_y3d);

if(center EQUALS TRUE)
    {
    if (!if_gpx)	/* currently isn't working well on GPX. JSC */
	wm_gstxal((GKS_INT)center, (GKS_INT)normal);
    else
	wm_gstxal((GKS_INT)normal, (GKS_INT)normal);
    }
else
    {
    wm_gstxal((GKS_INT)normal, (GKS_INT)normal);
    }
return SUCCESS;
}


/************************************************
*
	Normalize gks attributes
*
************************************************/
publicdef INT xs_gr_reset_attr()
{

return SUCCESS;
}

/***********************************************
*
	Mindless here-to-there line draw.
*
***********************************************/
publicdef INT xs_gr_drawline(x1,y1,x2,y2)
DOUBLE	x1,y1,x2,y2;
{
GKS_REAL	xa[3],ya[3];
	
	xa[0] = (GKS_REAL)x1;
	xa[1] = (GKS_REAL)x2;
	ya[0] = (GKS_REAL)y1;
	ya[1] = (GKS_REAL)y2;
	wm_gpl(2,xa,ya);

return SUCCESS;
}
