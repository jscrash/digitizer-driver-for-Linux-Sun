/* DEC/CMS REPLACEMENT HISTORY, Element AG_CLIP_CODE.C */
/* *3    14-AUG-1990 11:39:22 VINCE "(SPR 5644) Header Standardization" */
/* *2     9-FEB-1990 12:36:51 GILLESPIE "(SPR 6005) Old nlz_code in new module" */
/* *1     9-FEB-1990 12:34:18 GILLESPIE "Window clipping code generator" */
/* DEC/CMS REPLACEMENT HISTORY, Element AG_CLIP_CODE.C */
/*****************************************************************************/
/******************************************************************************

                Copyright Finder Graphics Systems, Inc. 1990
                    Unpublished -- All rights reserved

THIS SOFTWARE IS THE PROPRIETARY PROPERTY OF Exploration Systems, Inc.  AND MAY
CONTAIN CONFIDENTIAL TRADE SECRET INFORMATION. IT IS LICENSED FOR USE ON THE
DESIGNATED EQUIPMENT ON WHICH IT WAS ORIGINALLY INSTALLED AND  MAY NOT BE
MODIFIED, DUPLICATED OR COPIED IN ANY FORM WITHOUT PRIOR WRITTEN CONSENT OF

                            Finder Graphics Systems
                            201 Tamal Vista Blvd.
                            Corte Madera, CA  94925
                            (415) 927-0100

(The above notice does not constitute an admission of publication or
unrestricted dissemination of the work)

******************************************************************************/

#include "esi_ag.h"

/* Function Description -----------------------------------------------------
Description:
    According to the given window boundary, returns the position code of
    point, whose format is (LEFT, RIGTH, TOP, BOTTOM).

Prototype:
    publicdef INT ag_clip_code( DOUBLE x, DOUBLE y, DOUBLE minx, DOUBLE miny, 
                            DOUBLE maxx, DOUBLE maxy, INT *code);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) x           -(DOUBLE) The x-coord. of given point.
    (I) y           -(DOUBLE) The y-coord. of given point.
    (I) minx        -(DOUBLE) The minimum x-coord. value of window. 
    (I) miny        -(DOUBLE) The minimum y-coord. value of window.
    (I) maxx        -(DOUBLE) The maximum x-coord. value of window.
    (I) maxy        -(DOUBLE) The maximum y-coord. value of window.
    (O) code        -(INT *) The return code of visibility.

Return Value/Status:
    Returns whether or not the specified point is inside the window (0,1).
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT ag_clip_code (DOUBLE x, DOUBLE y, DOUBLE minx, DOUBLE miny, DOUBLE maxx,
                            DOUBLE maxy, INT *code) 
#else
publicdef INT ag_clip_code (x, y, minx, miny, maxx, maxy, code)
DOUBLE x;
DOUBLE y;
DOUBLE minx;
DOUBLE miny;
DOUBLE maxx;
DOUBLE maxy;
INT *code;
#endif
    {
    *code = AG_IN_WINDOW;
    
    if (x < minx)
        *code |= AG_LEFT_WINDOW;
    else if (x > maxx)
        *code |= AG_RIGHT_WINDOW;
    if (y < miny)
        *code |= AG_BOTTOM_WINDOW;
    else if (y > maxy)
        *code |= AG_TOP_WINDOW;
    
    return *code;
    }
/* END:     */
