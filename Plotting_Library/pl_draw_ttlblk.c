/* DEC/CMS REPLACEMENT HISTORY, Element PL_DRAW_TTLBLK.C */
/* *4     5-APR-1991 12:15:24 JESSIE "(SPR 0) change scale item format" */
/* *3    10-JAN-1991 11:47:03 MING "(SPR -1) add ts_sto_upper on project name " */
/* *2    23-OCT-1990 15:31:37 MING "(SPR 6075) pass base window name to pl_draw_ttlblk_text instead of the plot layout window */
/*name" */
/* *1    12-OCT-1990 17:41:22 GILLESPIE "Merge Ernie Deltas" */
/* DEC/CMS REPLACEMENT HISTORY, Element PL_DRAW_TTLBLK.C */
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


#include "esi_am.h"
#include "esi_gks.h"
#include "esi_wm.h"
#include "esi_lu.h"
#include "esi_time.h"
#include "esi_pl.h"
#include "esi_qs.h"
#include "esi_ts.h"
#include "esi_xs_defs.h"

#if USE_PROTOTYPES
publicdef INT pl_draw_ttlblk (PL_BASE_WINDOW *bw, CHAR *name,DOUBLE xwidth,DOUBLE yheight)
#else
publicdef INT pl_draw_ttlblk (bw, name,xwidth,yheight)
PL_BASE_WINDOW *bw;
CHAR *name;
DOUBLE xwidth;
DOUBLE yheight;
#endif
{
   static CHAR proj_label[] = {"Project Name: "};
   static CHAR auth[] = {"Author: "};
   static CHAR date[] = {"Date: "};
   static CHAR mapname[] = {"Map Name: "};
   static CHAR xsect_label[] = {"Layout Name: "};
   static CHAR cont[] = {"Contour Int.: "};
   static CHAR proj[] = {"Projection: "};
   static CHAR scale[] = {"Scale: "};
   static CHAR contour_label[] = {"Contour Int: "};
   static CHAR empty_scale[] = {"EQUAL SPACING"};
   static CHAR gaps[] = {"(GAPS)"};
   static CHAR centers[] = {"(CENTERS)"};
   
   GKS_INT color;
   GKS_INT font;
   time_t tp;
   tm_t *ttp;
   GKS_REAL height,width,xpos,ypos;  
   GKS_REAL ary_x[5],ary_y[5],gap;
   GKS_REAL line1_x[2],line2_x[2],line3_x[2],line4_x[2];
   GKS_REAL line5_x[2],line6_x[2],line7_x[2],line8_x[2];
   GKS_REAL line1_y[2],line2_y[2],line3_y[2],line4_y[2];
   GKS_REAL line5_y[2],line6_y[2],line7_y[2],line8_y[2];
   GKS_REAL current_y, unit;
   CHAR     author[40],text[200];
   PROJECT_NAME proj_name;
    
   /* draw line */
      lu_text_to_index( "COLOR", &color, "BLACK" );
      wm_gsln(1);		/* solid line */
      wm_gslwsc (1.0);
      
      wm_gsplci(color);         /* set line color to black */
      wm_gstxci(color);         /* set text color to black */
      ary_x[0]=0.0;
      ary_y[0]=0.0;

      ary_x[1]=0.0;
      ary_y[1]=yheight;

      ary_x[2]=xwidth;
      ary_y[2]=yheight;

      ary_x[3]=xwidth;
      ary_y[3]=0.0;

      ary_x[4]=0.0;
      ary_y[4]=0.0;
      wm_gpl(5,ary_x,ary_y);

      height = yheight;
      width  = xwidth;

      line1_x[0] = 0.0;
      line1_y[0] = height / 13.0;
      line1_x[1] = width;
      line1_y[1] = height / 13.0;
      wm_gpl(2,line1_x,line1_y);

      line2_x[0] = 0.0;
      line2_y[0] = (height / 13.0) * 2.0 ;
      line2_x[1] = width;
      line2_y[1] = (height / 13.0) * 2.0 ;
      wm_gpl(2,line2_x,line2_y);

      line3_x[0] = 0.0;
      line3_y[0] = (height / 13.0) * 3.0 ;
      line3_x[1] = width;
      line3_y[1] = (height / 13.0) * 3.0 ;
      wm_gpl(2,line3_x,line3_y);

      line4_x[0] = 0.0;
      line4_y[0] = (height / 13.0) * 4.0 ;
      line4_x[1] = width;
      line4_y[1] = (height / 13.0) * 4.0 ;
      wm_gpl(2,line4_x,line4_y);

      line8_x[0] = 0.0;
      line8_y[0] = (height / 13.0) * 5.0 ;
      line8_x[1] = width;
      line8_y[1] = (height / 13.0) * 5.0 ;
      wm_gpl(2,line8_x,line8_y);
      
      line5_x[0] = 0.0;
      line5_y[0] = (height / 13.0) * 10.0 ;
      line5_x[1] = width;
      line5_y[1] = (height / 13.0) * 10.0 ;
      wm_gpl(2,line5_x,line5_y);

      line6_x[0] = (width / 2.0 );
      line6_y[0] = 0.0;
      line6_x[1] = (width / 2.0 );
      line6_y[1] = (height / 13.0) ;
      wm_gpl(2,line6_x,line6_y);

      line7_x[0] = (width / 15.0 ) * 10.0;
      line7_y[0] = (height / 13.0) * 3.0 ;
      line7_x[1] = (width /15.0 ) * 10.0;
      line7_y[1] = (height /13.0) * 4.0 ;
      wm_gpl(2,line7_x,line7_y);

/* ------------------------- */
/* initailize text attribute */
/* ------------------------- */
      wm_gschup( 0.0, 1.0 ) ;
      lu_text_to_index( "FONT", &font, "SIMPLEX" );
      wm_gstxci( color ) ;
      wm_gstxp( GRIGHT );
      wm_gstxal( 1,3 );           /* set text on right position */
      wm_gstxfp(font, GSTRKP) ;
      wm_gschxp( 1.0 ) ;
      wm_gschsp( 0.0 ) ;

/* ----------------- */
/* set the font size */
/* ----------------- */
      height = (height / 48.0) ;    /* font size = 1/4 of line height */
      wm_gschh( height ) ;


      gap = width / 60.0;

/* ------------- */
/* project name  */
/* ------------- */
     qs_inq_c (QS_PROJECT_NAME,proj_name,(INT *) 0);
     ts_sto_upper (proj_name);
     sprintf(text,"%s%s",proj_label,proj_name);
     wm_gtx(gap, line4_y[0]+((line8_y[0] - line4_y[0])/2.0), text);

/* --------- */
/* user name */
/* --------- */
      qs_inq_c (QS_FINDER_ACCOUNT,author,(INT *) 0);
      sprintf(text,"%s%s",auth,author);
      wm_gtx( gap, line3_y[0]+((line4_y[0] - line3_y[0])/2), text);

/* ------------ */
/* picture name */
/* ------------ */
      switch (bw->type)
        {
	case PL_MAP:
          sprintf (text,"%s%s",mapname,bw->name);
	  break;
        case PL_XSECT:
	  sprintf (text,"%s%s",xsect_label,bw->name);
	  break;
        }
      wm_gtx( gap, line2_y[0]+((line3_y[0] - line2_y[0])/2.0), text);

/* ----- */
/* scale */
/* ----- */
      if (bw->scale_uom[0] == '\0')
        {
	sprintf(text,"%s%s",scale,empty_scale);
        }
      else
        {
	  if (bw->type EQUALS PL_XSECT)
	      {
	      switch (bw->xs_spacing)
	        {
	        case 0:    
	             sprintf(text,"%s%s",scale,empty_scale);
		     break;
	        case 1:
                     sprintf(text,"%s%.2f %s %s",scale,bw->scale,bw->scale_uom,
			     gaps);
                     break;
	        case 2:
                     sprintf(text,"%s%.2f %s %s",scale,bw->scale,bw->scale_uom,
			     centers);
                     break;
	        /*default:
		     am_messages (AM_STATUS, "Invalid xs spacing type "); */  
	        }
              }
	  else 
                 sprintf(text,"%s%.2f %s",scale,bw->scale,bw->scale_uom);
	  }
      wm_gtx( gap, line1_y[0]+((line2_y[0] - line1_y[0])/2.0), text);

/* ---------- */
/* projection */
/* ---------- */
      sprintf(text,"%s%s",proj,bw->projection);
      wm_gtx( gap, ((line1_y[0] - ary_y[0])/2.0), text);
      
/* ---- */
/* date */
/* ---- */
      time(&tp);
      ttp = localtime(&tp);
      sprintf(text,"%s%d/%d/%d",date,(ttp->tm_mon + 1),ttp->tm_mday,ttp->tm_year);
      wm_gtx((gap + line7_x[0]),line3_y[0]+((line4_y[0] - line3_y[0])/2.0), text);

/* ---------------- */
/* Contour Interval */
/* ---------------- */
      sprintf (text,"%s",contour_label);
      wm_gtx((gap + line6_x[0]), ((line1_y[0] - ary_y[0])/2.0), text);

/* ---- */
/* logo */
/* ---- */
      height = yheight - line5_y[0] ;
      width = height;
      xpos = line5_x[0] + (height / 5.0);
      ypos = line5_y[0] + (height / 5.0);
      height = (height * 3.0) / 5.0 ;
      pl_draw_logo(xpos,ypos,height,height);

/* --------- */
/* logo text */
/* --------- */
      xpos = (xwidth - width) / 2.0 + width; /* get center point of x */
      ypos = (width / 2.0 ) + line5_y[0];    /* get center point of y */
      height = (width * 3.0) / 5.0 ;
      width = line5_x[1] - width ;
      pl_draw_logo_text(xpos,ypos,height);

/* ----------------------------- */
/* draw user defined description */
/* ----------------------------- */
      pl_draw_ttlblk_text (bw->name,bw->type,xwidth,yheight,
			   (line5_y[0] - line8_y[0]) / 5.0);

      return(SUCCESS);
}
