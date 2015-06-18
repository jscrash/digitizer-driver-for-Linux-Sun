/* DEC/CMS REPLACEMENT HISTORY, Element PL_DRAW_BORDER.C */
/* *2    16-AUG-1991 10:44:05 JTM "(SPR 0) Changed arguments to wm_gslwsc() to DOUBLE." */
/* *1    12-OCT-1990 17:37:54 GILLESPIE "Merge Ernie Deltas" */
/* DEC/CMS REPLACEMENT HISTORY, Element PL_DRAW_BORDER.C */
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


#ifndef ESI_PL_H
#include "esi_pl.h"
#endif

#ifndef ESI_LU_H
#include "esi_lu.h"
#endif

#ifndef ESI_GKS_H
#include "esi_gks.h"
#endif

#if USE_PROTOTYPES
publicdef INT pl_draw_border (INT border, DOUBLE width, DOUBLE height)
#else
publicdef INT pl_draw_border (border, width, height)
INT border;
DOUBLE width;
DOUBLE height;
#endif
{
  INT color;
  GKS_REAL x[5];
  GKS_REAL y[5];
  GKS_REAL xx[5];
  GKS_REAL yy[5];
  GKS_REAL gap = 0.1;
  DOUBLE fine = 1.0;
  DOUBLE bold = 2.0;


  x[0] = 0.0;
  y[0] = 0.0;
  x[1] = width;
  y[1] = 0.0;
  x[2] = width;
  y[2] = height;
  x[3] = 0.0;
  y[3] = height;
  x[4] = 0.0;
  y[4] = 0.0;

/* ---------------------------------------- */
/* set solid line type and black color line */
/* ---------------------------------------- */
  wm_gsln (1);
  lu_text_to_index ("COLOR",&color,"BLACK");
  wm_gsplci (color);

/* ----------- */
/* draw border */
/* ----------- */
  switch (border)
    {
    case PL_BFINE:
      wm_gslwsc (fine);
      wm_gpl (5,x,y);
      break;

    case PL_BBOLD:
      wm_gslwsc (bold);
      wm_gpl (5,x,y);
      break;

    case PL_BFINE_FINE:
      wm_gslwsc (fine);
      wm_gpl (5,x,y);
      xx[0] = x[0] + gap;
      yy[0] = y[0] + gap;
      xx[1] = x[1] - gap;
      yy[1] = y[1] + gap;
      xx[2] = x[2] - gap;
      yy[2] = y[2] - gap;
      xx[3] = x[3] + gap;
      yy[3] = y[3] - gap;
      xx[4] = xx[0];
      yy[4] = yy[0];
      wm_gpl (5,xx,yy);
      break;

    case PL_BFINE_BOLD:
      wm_gslwsc (fine);
      wm_gpl (5,x,y);
      xx[0] = x[0] + gap;
      yy[0] = y[0] + gap;
      xx[1] = x[1] - gap;
      yy[1] = y[1] + gap;
      xx[2] = x[2] - gap;
      yy[2] = y[2] - gap;
      xx[3] = x[3] + gap;
      yy[3] = y[3] - gap;
      xx[4] = xx[0];
      yy[4] = yy[0];
      wm_gslwsc (bold);
      wm_gpl (5,xx,yy);
      break;
      
    case PL_BBOLD_FINE:
      wm_gslwsc (bold);
      wm_gpl (5,x,y);
      xx[0] = x[0] + gap;
      yy[0] = y[0] + gap;
      xx[1] = x[1] - gap;
      yy[1] = y[1] + gap;
      xx[2] = x[2] - gap;
      yy[2] = y[2] - gap;
      xx[3] = x[3] + gap;
      yy[3] = y[3] - gap;
      xx[4] = xx[0];
      yy[4] = yy[0];
      wm_gslwsc (fine);
      wm_gpl (5,xx,yy);
      break;
      
    case PL_BBOLD_BOLD:
      wm_gslwsc (bold);
      wm_gpl (5,x,y);
      xx[0] = x[0] + gap;
      yy[0] = y[0] + gap;
      xx[1] = x[1] - gap;
      yy[1] = y[1] + gap;
      xx[2] = x[2] - gap;
      yy[2] = y[2] - gap;
      xx[3] = x[3] + gap;
      yy[3] = y[3] - gap;
      xx[4] = xx[0];
      yy[4] = yy[0];
      wm_gpl (5,xx,yy);
      break;
      
    case PL_BTRIPLE_FINE:
      wm_gslwsc (fine);
      wm_gpl (5,x,y);
      xx[0] = x[0] + gap;
      yy[0] = y[0] + gap;
      xx[1] = x[1] - gap;
      yy[1] = y[1] + gap;
      xx[2] = x[2] - gap;
      yy[2] = y[2] - gap;
      xx[3] = x[3] + gap;
      yy[3] = y[3] - gap;
      xx[4] = xx[0];
      yy[4] = yy[0];
      wm_gpl (5,xx,yy);
      xx[0] = xx[0] + gap;
      yy[0] = yy[0] + gap;
      xx[1] = xx[1] - gap;
      yy[1] = yy[1] + gap;
      xx[2] = xx[2] - gap;
      yy[2] = yy[2] - gap;
      xx[3] = xx[3] + gap;
      yy[3] = yy[3] - gap;
      xx[4] = xx[0];
      yy[4] = yy[0];
      wm_gpl (5,xx,yy);
      break;
    }

  return SUCCESS;
}

      
