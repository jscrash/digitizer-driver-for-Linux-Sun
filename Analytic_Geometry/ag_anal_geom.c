/* DEC/CMS REPLACEMENT HISTORY, Element AG_ANAL_GEOM.C*/
/* *3    14-AUG-1990 23:56:30 GILLESPIE "(SPR 5644) Fix last checkin"*/
/* *2    14-AUG-1990 11:39:08 VINCE "(SPR 5644) Header Standardization"*/
/* *1    19-JUN-1989 11:59:04 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element AG_ANAL_GEOM.C*/
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

#include "esi_gl_defs.h"
#include "esi_c_lib.h"
#include "esi_ag.h"

/*
    Replace include of math.h and errno.h with an include of
    esi_math.h and esi_errno.h.  This will eliminate any host specific 
    code in this module and make it possible to handle "special" problems 
    of SUN math.h

            JRB, Calgary, 5/3/89
*/

#ifndef ESI_MATH_H
#include "esi_math.h"
#endif

#ifndef ESI_ERRNO_H
#include "esi_errno.h"
#endif

/* Function Description -----------------------------------------------------
Description:
    This module is intended as a grab bag of tools for doing
    analytic geometry.  If you have some handy routines for
    intersections between lines, or lines and planes, or
    polar coordinates conversion, etc.  - please put them in here
    and let people know.  Please use coded comments for rapid
    documentation.
    
    This routine calculates projection of point_in into line defined by slope, 
    intercept along line defined by bearing theta, measured in radians.
    
    Since a lot of people will be adding to this file, here's a
    first pass at a naming convention:
    Verbs:
    _calcint_ = Calculate intersection of.
    _convto_ = Convert

    Nouns:
    _2Dpoint_ = 2D point.
    _3Dpoint_ = 3D point.
    _2Dline_ = line described by two 2D points on the line.
    _3Dline_ = line described by two 3D points on the line.
    _abline_ = line described as y = (a * x) + b .
    _pnplane_ = plane described by a point and a normal vector .
    _3pplane_ = plane described by three points on plane.
    -xmat23_ = 2 by 3 homogeneous 2D coordinate transform matrix .
    -xmat34_ = 3 by 4 homogeneous 3D coordinate transform matrix .
    _area_
    _tangent_

    Example:
    ag_convert_2Dline_abline
    ag_calcint_abline_3pplane

    Please feel to free to expand or change these conventions.

Prototype:
    publicdef DOUBLE ag_proj_2dpoint_line(POINT2D *point_in, DOUBLE slope, 
                                          DOUBLE intercept, DOUBLE theta, 
                                          POINT2D point_out);
                                      
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) point_in    -(POINT2D) Pointer to a 2-D point structure that reresents the      
                        point to be projected
    (I) slope       -(DOUBLE) Slope of line.
    (I) intercept   -(DOUBLE) Intercept of line.
    (I) theta       -(DOUBLE) Angle of line of projection.
                        (NOTE: to make a projection perpendicular to the line,
                        set theta = atan(-1/slope)
    (O) point_out   -(POINT2D) Pointer to 2-D point structure that represents the 
                        projected position of point_in along projection line 
                        theta into line defined by slope and intercept.

Return Value/Status:
    Routine returns distance from point to line

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef DOUBLE ag_proj_2dpoint_line (POINT2D *point_in, DOUBLE slope,
                                       DOUBLE intercept, DOUBLE theta,
                                       POINT2D *point_out) 
#else
publicdef DOUBLE ag_proj_2dpoint_line (point_in, slope, intercept, theta, point_out)
POINT2D *point_in;
DOUBLE slope;
DOUBLE intercept;
DOUBLE theta;
POINT2D *point_out;
#endif
    {
    DOUBLE theta1;
    
    theta1 = tan (theta);
    /*
       * check for blow-up of tangent call...
    */
    if (errno EQUALS ERANGE)
        {
        point_out->x = point_in->x;
        point_out->y = slope * point_in->x + intercept;
        }
    else
        {
        if (slope EQUALS theta1)    /* line slopes are parallel */
            {
            *point_out = *point_in;
            }
        else
            {
            point_out->x = (point_in->y - theta1 * point_in->x - intercept) /
                           (slope - theta1);
            point_out->y = slope * point_out->x + intercept;
            }
        }
    return hypot (point_in->y - point_out->y, point_in->x - point_out->x);
    
    }
/* END:     */
