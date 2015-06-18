/* DEC/CMS REPLACEMENT HISTORY, Element PL_DRAW_LOGO.C */
/* *3     9-JAN-1992 12:43:37 JTM "(SPR 0) BHP prompted changes. If secret file found, plots logo from that file - otherwise uses */
/*old plot routine. Only effects VAX." */
/* *2    23-OCT-1990 15:23:18 MING "(SPR 6075) fix input parameter declaration from GKS_RAL to DOUBLE" */
/* *1    12-OCT-1990 17:39:11 GILLESPIE "Merge Ernie Deltas" */
/* DEC/CMS REPLACEMENT HISTORY, Element PL_DRAW_LOGO.C */
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

#ifndef ESI_GKS_H
#include "esi_gks.h"
#endif

#ifndef ESI_LU_H
#include "esi_lu.h"
#endif

/*
  JTM  8/1/92
  Incorporating changes from BHP so that:
 
  1) pl_draw_logo() looks for the file ESI$BIN:VECTOR_FILE.DAT
  2) if found, opens and plots using BHP's code
  3) if not found, calls the old Finder pl_draw_logo()

  Left the BHP code pretty much alone, adding wrappers to the
  #include and the else{} if the file isn't found.
*/
/*
--
--
--    File:         BHPP_LOGO.C
--
--    Functions:    pl_draw_logo
--
--    Description:  Draw the BHPP logo within the FINDER system.
--
--                  This C routine is based on the FORTRAN routine
--                  written by H. Hoang.  It has been modified to
--                  use the FINDER GKS calls.
--
-- 	                The logo outline is defined by (X,Y) coordinates, its segments
-- 	                are delimited by a pair of (-1,-1).
-- 	                The origin of the digitised logo lies at the bottom/left corner.
-- 	                The X extent is 0-8500 units, and the Y extent 0-2250 units.
-- 	                The algorithm is to load one segment at a time into an array and
-- 	                pass it as a polygon for plotting and polygon filling.
--
--
--    Author:       Steve Mellor
--    Date Created: 20th February 1990
--
--
--
--    Modification History
--
--    Revision  Date       Description
--
--      1.0     20/2/91    Creation
--      1.2      8/3/91    Modification.- color filled - HH.
--
*/

/*
--  Include files
*/

#ifndef   ESI_C_LIB_H
#include  "esi_c_lib.h"
#endif

#include  "ctype.h"

#ifndef   ESI_GKS_H
#include  "esi_gks.h"
#endif


/*
--  Some local definitions
*/
                                                  
#define MAXLEN           548
#define LENVEC           100
#define INTERIOR           2
#define XMAX            8500.
#define XMIN               0.
#define YMAX            2250.
#define YMIN               0.


/* Forward references */
#if USE_PROTOTYPES
static INT old_pl_draw_logo(DOUBLE xpos,DOUBLE ypos,
                   DOUBLE width,DOUBLE  height);
#else
static INT old_pl_draw_logo();
#endif

/**/

#if USE_PROTOTYPES
publicdef INT 
pl_draw_logo(DOUBLE xpos,
             DOUBLE ypos,
             DOUBLE width,
             DOUBLE  height)
#else
publicdef INT pl_draw_logo(xpos,ypos,width,height)
DOUBLE xpos;
DOUBLE ypos;
DOUBLE width;
DOUBLE height;
#endif

  {
  CHAR    tmp_buf[80];
  CHAR    *result;
  CHAR    *vfname = "ESI$BIN:VECTOR_FILE.DAT";

  FILE    *fp;

  FLOAT   x[LENVEC];
  FLOAT   y[LENVEC];
  FLOAT   xlogo[MAXLEN];
  FLOAT   ylogo[MAXLEN];
  FLOAT   endmark = (FLOAT) -1.0;
  FLOAT   x_ratio;
  FLOAT   y_ratio;
  FLOAT   scale;

  GKS_INT black;
  GKS_INT red;
  GKS_INT fill_area_style;
  GKS_INT no_fill_area_style;

  INT     nvec;
  INT     icount;
  INT     ncount;
  INT     status = SUCCESS;
  INT     nchar = 80;

  /*
  --  Start code here
  */

  /*
  --  Set up the colour indices
  */
  lu_text_to_index ("COLOR", &black,   "BLACK");
  lu_text_to_index ("COLOR", &red,     "RED");
/*  lu_text_to_index ("AREA", &fill_area_style, "SOLID"); */
  lu_text_to_index ("FILL_STYLE", &fill_area_style, "SOLID"); 
  lu_text_to_index ("FILL_STYLE", &no_fill_area_style, "HOLLOW"); 

  /*
  --  Get the data
  */
  status = FAIL;
  if ((fp = fopen(vfname, "r")) != (FILE *)0)
    {
    status = SUCCESS;
    ncount = 0;
    for (icount = 0; (icount < MAXLEN) && (status == SUCCESS); icount++)
      {
      status = FAIL;
      if ((result = fgets(tmp_buf,nchar,fp)) != (CHAR *)0)
        {
        ncount++;
        result += 18;
        sscanf(result, "%f ,   %f",&xlogo[icount], &ylogo[icount]);
        status = SUCCESS;
        }
      }
    fclose(fp);

    if (status >= SUCCESS)
      {                                           
      x_ratio = (XMAX - XMIN) / ((FLOAT) width);
      y_ratio = (YMAX - YMIN) / ((FLOAT) height);
      scale = x_ratio;
      if (x_ratio > y_ratio) scale = y_ratio;
      nvec = 0;

      wm_gsfaci(red);

      wm_gsfais(fill_area_style);
      wm_gsplci(red); 

/*    wm_gsfasi(1);    */

      for (icount = 0; icount < ncount; icount++)
        {
        if (xlogo[icount] != endmark)
          {
          x[nvec] = (FLOAT) xpos + (xlogo[icount] / scale);
          y[nvec] = (FLOAT) ypos + (ylogo[icount] / scale);
          nvec=nvec+1;
          if (nvec >= LENVEC)
            {
            printf("** Polygon points exceeded : %d", LENVEC);
            nvec = LENVEC;
            break;
            }
          }
        else
          {
          /*
          --  Plot the vector built so far
          */
/*        wm_gpl(nvec, x, y);                     */
          
	  wm_gfa(nvec, x, y);         

          wm_gsfaci(black);
          wm_gsfais(fill_area_style);
          wm_gsplci(black);
          nvec=0;
          }
        }
      }
    }
	/* JTM 8/1/92  if no file found at secret spot, do normal logo */
	else                                                       
	{
		return( old_pl_draw_logo( xpos, ypos, width, height ) );
	}

  wm_gsfais(no_fill_area_style);
  return(status);
  }


#if USE_PROTOTYPES
static INT old_pl_draw_logo(DOUBLE xpos,DOUBLE ypos,
                   DOUBLE width,DOUBLE  height)
#else
static INT old_pl_draw_logo(xpos,ypos,width,height)
DOUBLE xpos;
DOUBLE ypos;
DOUBLE width;
DOUBLE height;
#endif

{
     INT      i,j;
     GKS_INT  color;
     GKS_INT  line_style;
     GKS_REAL save_x[2],save_y[2],start_x[2],start_y[2];
     GKS_REAL inter_x,inter_y,next_x;

     save_x[0] = start_x[0] = xpos;
     save_y[0] = start_y[0] = (height * 2.0) / 5.0 + ypos;
     save_x[1] = start_x[1] = ((width * 7.5) / 12.0) + xpos;
     save_y[1] = start_y[1] = start_y[0];
     inter_y = (height + ypos - start_y[0]) / 7.0 ;
     inter_x = (width * 10.0) / 252.0;
     next_x  = width / 12.0 ;

     lu_text_to_index ("COLOR",&color,"BLACK");
     wm_gsplci (color);
     lu_text_to_index ("LINE_STYLE",&line_style,"SOLID");
     wm_gsln (line_style);
     wm_gslwsc(2.0);
     for(j = 0; j < 3 ; j++ )
     {
        wm_gpl(2,start_x,start_y);
	for(i = 0; i < 7 ; i++)
	{
	    start_y[0] += inter_y;
	    start_x[0] += inter_x;
	    start_y[1] += inter_y;
	    start_x[1] += inter_x;
            wm_gpl(2,start_x,start_y);
	}
	save_x[0] += next_x;
	save_x[1] += next_x;
	if ( j == 0)
             save_y[0]  = height / 5.0 + ypos;
	else{
	     if (j == 1)
	        save_y[0] = ypos;
            }
	save_y[1]  = save_y[0] ;
	start_x[0] = save_x[0] ;
	start_y[0] = save_y[0] ;
	start_x[1] = save_x[1] ;
	start_y[1] = save_y[1] ;
     }
     return (SUCCESS);
 }


#if USE_PROTOTYPES
publicdef INT pl_draw_logo_text(DOUBLE center_x,DOUBLE center_y, 
			DOUBLE height)
#else
publicdef INT pl_draw_logo_text(center_x,center_y,height)
DOUBLE center_x;
DOUBLE center_y;
DOUBLE height;
#endif
{
     static char header[] = {"FINDER Graphics Systems, Inc."};
     GKS_INT color;
     GKS_INT font;
     GKS_INT precision;

     lu_text_to_index ("COLOR",&color,"BLACK");
     lu_text_to_index ("TEXT_PRECISION",&precision,"STROKE");
     lu_text_to_index ("FONT",&font,"SIMPLEX");
     wm_gstxci (color);
     wm_gstxfp (font,precision);
     wm_gschup( 0.0, 1.0 ) ;
     wm_gstxp( GRIGHT );
     wm_gstxal(2,3);
     wm_gschh(height/6.0);
     wm_gtx(center_x,center_y,header);
     return(SUCCESS);
}
