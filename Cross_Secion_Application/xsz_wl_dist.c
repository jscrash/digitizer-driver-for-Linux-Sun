/* DEC/CMS REPLACEMENT HISTORY, Element XSZ_WL_DIST.C*/
/* *3    14-JUN-1991 10:23:12 JESSIE "(SPR 7663) add point transform for GEODETIC project(case#1958)"*/
/* *2    17-AUG-1990 22:37:28 VINCE "(SPR 5644) Code Cleanup"*/
/* *1    19-JUN-1989 13:43:27 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element XSZ_WL_DIST.C*/
/*****************************************************************************/
/******************************************************************************

                Copyright Exploration Systems, Inc. 1989
                    Unpublished -- All rights reserved

THIS SOFTWARE IS THE PROPRIETARY PROPERTY OF Exploration Systems, Inc.  AND MAY
CONTAIN CONFIDENTIAL TRADE SECRET INFORMATION. IT IS LICENSED FOR USE ON THE
DESIGNATED EQUIPMENT ON WHICH IT WAS ORIGINALLY INSTALLED AND  MAY NOT BE
MODIFIED, DUPLICATED OR COPIED IN ANY FORM WITHOUT PRIOR WRITTEN CONSENT OF

                            Exploration Systems
                            c/o Finder Graphics Systems
                            201 Tamal Vista Blvd.
                            Corte Madera, CA  94925
                            (415) 927-0100

(The above notice does not constitute an admission of publication or
unrestricted dissemination of the work)

******************************************************************************/

#ifndef ESI_GL_DEFS_H

#include "esi_gl_defs.h"

#endif

#ifndef ESI_XS_DEFS_H

#include "esi_xs_defs.h"

#endif

#ifndef ESI_MATH_H

#include "esi_math.h"

#endif

#ifndef ESI_AM_H

#include "esi_am.h"

#endif

#ifndef ESI_CT_H

#include "esi_ct.h"

#endif

#ifndef ESI_GL_PROJECT_H

#include "esi_gl_project.h"

#endif

/* Function Description -----------------------------------------------------
Description:

Prototype:
    publicdef INT xsz_well_distance(XS_STRUCT *pxs, LAYOUT_STRUCT *p_layout1, 
        LAYOUT_STRUCT *p_layout2, DOUBLE *distance );

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    pxs             -(XS_STRUCT *)
    p_layout1       -(LAYOUT_STRUCT *)
    p_layout2       -(LAYOUT_STRUCT *)
    distance        -(DOUBLE *)

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT xsz_well_distance (XS_STRUCT *pxs, LAYOUT_STRUCT *p_layout1,
                                 LAYOUT_STRUCT *p_layout2, DOUBLE *distance)
#else
publicdef INT xsz_well_distance (pxs, p_layout1, p_layout2, distance)
XS_STRUCT *pxs;
LAYOUT_STRUCT *p_layout1;
LAYOUT_STRUCT *p_layout2;
DOUBLE *distance;
#endif
    {
    DOUBLE dx, dy;
    DOUBLE px1,px2,py1,py2;
    PROJECT_DEFAULT *project_default;
    PROJECTION_STRUCTURE *geo_projection;
    PROJECTION_STRUCTURE *map_projection;
    INT status;

    /***********************************************************/
    /* get project_defaults structure                          */
    /***********************************************************/
    status = am_define_workspace ("PROJECT_DEFAULTS", AM_GLOBAL,
				sizeof(PROJECT_DEFAULT),
				(VOIDPTR) &project_default);

    if (status IS_NOT_EQUAL_TO SUCCESS)
      {
      status = am_open_workspace ("PROJECT_DEFAULTS", AM_GLOBAL,
				(VOIDPTR) &project_default);
      }
		
    px1 = p_layout1->pwell->tophole_xy.x;
    py1 = p_layout1->pwell->tophole_xy.y;
    px2 = p_layout2->pwell->tophole_xy.x;
    py2 = p_layout2->pwell->tophole_xy.y;

    /***********************************************************/
    /* if current project projection is GEODETIC, use          */
    /* ct_transform_point to transform point from project      */
    /* projection to default map projection                    */
    /***********************************************************/
    if (strcmp (project_default->projection, "GEODETIC") == SUCCESS)
      {

      ct_get_projection("GEODETIC", &geo_projection);
      ct_get_projection(project_default->map.projection, &map_projection);

      ct_transform_point(p_layout1->pwell->tophole_xy.x,
			 p_layout1->pwell->tophole_xy.y,
			 geo_projection,
			 &px1,
			 &py1,
			 map_projection);
      ct_transform_point(p_layout2->pwell->tophole_xy.x,
			 p_layout2->pwell->tophole_xy.y,
			 geo_projection,
			 &px2,
			 &py2,
			 map_projection);
      }

    /*dy = p_layout1->pwell->tophole_xy.y - p_layout2->pwell->tophole_xy.y;
    dx = p_layout1->pwell->tophole_xy.x - p_layout2->pwell->tophole_xy.x;*/
    dy = py1 - py2;
    dx = px1 - px2;
    
    *distance = sqrt (dy * dy + dx * dx) / pxs->xs_scale;
    
    return SUCCESS;
    }
/* END:     */
