/* DEC/CMS REPLACEMENT HISTORY, Element PL_DRAW_ARROW.C */
/* *4     8-JAN-1992 16:11:18 JTM "(SPR 0) BHP prompted change. Use lookup calls for colors instead of hard coded numbers.  */
/*Commented out draw logo call." */
/* *3    21-DEC-1990 13:43:24 MING "(SPR -1) add more messages" */
/* *2    15-OCT-1990 12:18:00 GILLESPIE "(SPR 1) Clean up from Ernie checkin" */
/* *1    12-OCT-1990 17:36:54 GILLESPIE "Merge Ernie Deltas" */
/* DEC/CMS REPLACEMENT HISTORY, Element PL_DRAW_ARROW.C */

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


#include "esi_wm.h"
#include "esi_lu.h"
#include "esi_pl.h"
#include "esi_gtypes.h"
#include "esi_math.h"
#include "esi_mp.h"
#include "esi_am.h"
#include "esi_ct.h"
#ifndef ESI_AM_ERR_H
#include "esi_am_err.h"
#endif

#if USE_PROTOTYPES
publicdef INT pl_draw_arrow (GKS_REAL xwidth,GKS_REAL yheight)
#else
publicdef INT pl_draw_arrow (xwidth,yheight)
GKS_REAL xwidth;
GKS_REAL yheight;
#endif
{
   INT status = SUCCESS;
   GKS_INT color, solid, no_fill;
   INT     font;
   INT i;
   GKS_REAL height,width,xpos,ypos;  
   GKS_REAL ary_x[5],ary_y[5];
   GKS_REAL center_x,center_y;
   GKS_REAL area1,area2,area3;
   GKS_REAL px[4],py[4];
   GKS_REAL x[5],y[5];
   PROJECTION_STRUCTURE *map_projection;
   PROJECTION_STRUCTURE *geo_projection;
   MAP_STRUCTURE *map;
   DOUBLE center[2], test_point[2], geo_point[2];
   DOUBLE cos_theta, sin_theta, theta;
    
   status = am_open_workspace ("MAPDEF",AM_APPLICATION,(VOIDPTR *) &map);

   if (status == AM_ERR_NOT_DEFINED)
     {
     am_message (AM_STATUS,"North arrow: Base window is not a map.");
     return FAIL;
     }

   if ((status = ct_get_projection ("GEODETIC",&geo_projection)) != SUCCESS)
     {
     am_message (AM_STATUS,"North arrow: Fail to retrieve GEODETIC projection.");
     return status;
     }

   center[0] = (map->lower_left_xy.x + map->upper_right_xy.x) / 2.0;
   center[1] = (map->lower_left_xy.y + map->upper_right_xy.y) / 2.0;

   if ((status = ct_get_projection (map->map_projection,&map_projection)) != SUCCESS)
     {
     am_message (AM_STATUS,"North arrow: Fail to retrieve map projection.");
     return status;
     }

   if ((status = ct_transform_point (center[0],center[1],map_projection,
				     &geo_point[0],&geo_point[1],
				     geo_projection)) != SUCCESS)
     {
     am_message (AM_STATUS,"North arrow: Fail to transform projections.");
     return status;
     }

   geo_point[1] += 1.0;

   if ((status = ct_transform_point (geo_point[0], geo_point[1],geo_projection,
				     &test_point[0],&test_point[1],
				     map_projection)) != SUCCESS)
     {
     am_message (AM_STATUS,"NOrth arrow: Fail to transform projections.");
     return status;
     }
   theta = 1.570796 - atan2 (test_point[1] - center[1], test_point[0] - center[0]);

   cos_theta = cos (theta);
   sin_theta = sin (theta);

   lu_text_to_index( "FILL_STYLE", &solid, "SOLID" );
   lu_text_to_index( "FILL_STYLE", &no_fill, "HOLLOW");
   lu_text_to_index( "COLOR", &color, "BLACK" );
   wm_gsln(1);		/* solid line */
      
   wm_gsplci(color);         /* set line color to black */
   wm_gstxci(color);         /* set text color to black */

   area1 = yheight / 5.0 ;
   area2 = (yheight * 3.0) / 5.0;
   area3 = (yheight * 4.0) / 5.0;

   /* draw point on north arrow */

   wm_gsfaci(color);       /*   set fill color to black */
   wm_gsfais(solid);         /* set fill interior solid */
   height = ((yheight - area3) * 3.0 ) / 5.0 ;
   px[0]  = xwidth / 2.0 ;
   px[1]  = px[0] - (height / 2.0);
   px[2]  = px[0] + (height / 2.0);
   py[0]  = area3 + (((yheight - area3) * 4.0 ) / 5.0);
   py[1]  = area3 + ((yheight - area3) / 5.0);
   py[2]  = py[1];
   px[3]  = px[0];
   py[3]  = py[0];


/* -------- */
/* Rotation */
/* -------- */
   for (i = 0; i < 4; ++i)
     {
     x[i] = px[i] * cos_theta + py[i] * sin_theta;
     y[i] = -px[i] * sin_theta + py[i] * cos_theta;
     }

   wm_gfa(4,x,y);
   wm_gsfais(no_fill);                 /* set fill interior hollow */

   /* draw vertical line */
   x[1] = xwidth / 2.0;
   y[1] = (area2 - area1) / 2.0 + area1;

   wm_gslwsc(1.0);
   wm_gpl(2,x,y);

   /* draw N on north arrow */ 

   wm_gslwsc(3.0);
   height = ((area3 - area2) * 3.0 ) / 5.0;
   width  = xwidth / 2.0;
   ary_x[0] = width - (height / 2.0);
   ary_x[1] = ary_x[0];
   ary_x[2] = width + (height / 2.0);
   ary_x[3] = ary_x[2];
   ary_y[1] = area2 + (((area3 - area2) * 4.0 ) / 5.0);
   ary_y[0] = area2 + ((area3 - area2) / 5.0);
   ary_y[2] = ary_y[0];
   ary_y[3] = ary_y[1];

/* -------- */
/* Rotation */
/* -------- */
	for (i = 0; i < 4; ++i)
   	{
		x[i] = ary_x[i] * cos_theta + ary_y[i] * sin_theta;
		y[i] = -ary_x[i] * sin_theta + ary_y[i] * cos_theta;
   	}
   	wm_gpl(4,x,y);

#if 0
/*
   JTM 8/1/92
   BHP prompted change. Comment out the draw logo call.
*/
   /* call logo function */

      height = area2 - area1 ;
      xpos = (xwidth / 2.0) - (height / 2.0);
      ypos = area1;
      xpos = xpos + (height / 5.0);   /* adjust x position a little right  */
      ypos = ypos + (height / 5.0);   /* adjust y position a little bit up */
      height = (height * 3.0) / 5.0;    /* draw area is 3/5 of height */ 
      pl_draw_logo(xpos,ypos,height,height);

   /* call logo text function */
      height = area1;
      xpos = xwidth / 2.0;   /* get the center position of x */
      ypos = area1 / 2.0;    /* get the center position of y */ 
      pl_draw_logo_text(xpos,ypos,height);

#endif  /* draw logo call removal */

	return ( SUCCESS ) ;
}
