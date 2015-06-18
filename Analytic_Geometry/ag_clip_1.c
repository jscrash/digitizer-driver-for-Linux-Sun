/* DEC/CMS REPLACEMENT HISTORY, Element AG_CLIP_1.C */
/* *3    14-AUG-1990 11:39:13 VINCE "(SPR 5644) Header Standardization" */
/* *2    10-FEB-1990 14:00:50 GILLESPIE "(SPR 6005) Change window clipping logic in nl_window_clip_nlist" */
/* *1     9-FEB-1990 12:38:12 GILLESPIE "New clipping algorithm" */
/* DEC/CMS REPLACEMENT HISTORY, Element AG_CLIP_1.C */
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

#include "esi_ag.h"

/* File Description ---------------------------------------------------------
Overview:
    This function determines clips a line segment so that it can fit into a 
    specified window boundary.
    Liang Barsky Window Clipping Algorithm
    from Hearn, Donald, and Baker, M Pauline, 1986, Computer Graphics, Prentice-Hall,
    Englewood Cliffs, New Jersey 07632, p. 133-134

Prototypes and one line descriptions of each function in the file.
Public_Functions:
    Clipping routine.
    publicdef INT ag_clip_1(DOUBLE *x1, DOUBLE *y1, DOUBLE *x2, DOUBLE *y2, DOUBLE xwmin,
                            DOUBLE ywmin, DOUBLE xwmax, DOUBLE ywmax, DOUBLE *u1, 
                            DOUBLE *u2, BOOL *visible);

Private_Functions:
    Clipping test routine.
    static INT cliptest(DOUBLE p, DOUBLE q, DOUBLE *u1, DOUBLE *u2);

-----------------------------------------------------------------------------*/

/* Function Description -----------------------------------------------------
Description:
    This function tests whether or not the given line can be fit into the window,
    through successive clipping.
    
Prototype:
    static INT cliptest(DOUBLE p, DOUBLE q, DOUBLE *u1, DOUBLE *u2);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) p           -(DOUBLE) Parameters passed by public function.
    (I) q           -(DOUBLE) Parameters passed by public function.
    (M) u1          -(DOUBLE *) Modified parameters of line segment.
    (M) u2          -(DOUBLE *) Modified parameters of line segment.

Return Value/Status:
    Returns the whether or not a specified line segment is within a
    window boundary.
    
Scope:
    PRIVATE to <ag_clip_1>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static INT cliptest (DOUBLE p, DOUBLE q, DOUBLE *u1, DOUBLE *u2) 
#else
static INT cliptest (p, q, u1, u2)
DOUBLE p;
DOUBLE q;
DOUBLE *u1;
DOUBLE *u2;
#endif
    {
    DOUBLE r;
    INT result = 1;
    
    if (p < 0.0)
        {
        r = q / p;
        if (r > *u2)
            {
            result = 0;
            }
        else if (r > *u1)
            {
            *u1 = r;
            }
        }
    else if (p > 0.0)
        {
        r = q / p;
        if (r < *u1)
            {
            result = 0;
            }
        else if (r < *u2)
            {
            *u2 = r;
            }
        }
    else
        {
        if (q < 0)
            {
            result = 0;
            }
        }
    return result;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    This routine will clip a line segment so that it can fit into a window.
    The line is specified by two ordered pairs: (x1,y1) and (x2,y2).  The line
    is then generated in parametric form:
    
        x = x1 + dx u
        y = y1 + dy u
        
    Where:
    
        dx = x2 - x1
        dy = y2 - y1
        0 =< u <= 1
        
    The window is defined by (xwmin, ywmin) and (xwmax, ywmax).  The function
    cliptest checks, via the function
    
        u = q/p
        
    Whether or not the line segment is within the boundaries of the given window.
    
    For more specific information, see Hearn/Baker, "Computer Graphics", 1986, 
    pp 128 - 134. 
    
Prototype:
    publicdef INT ag_clip_1(DOUBLE *x1, DOUBLE *y1, DOUBLE *x2, DOUBLE *y2, DOUBLE xwmin,
                            DOUBLE ywmin, DOUBLE xwmax, DOUBLE ywmax, DOUBLE *u1, 
                            DOUBLE *u2, BOOL *visible);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) x1          -(DOUBLE *) First X coordinate of line segment to be clipped.
    (I) y1          -(DOUBLE *) First Y coordinate of line segment to be clipped.
    (I) x2          -(DOUBLE *) Second X coordinate of line segment to be clipped.
    (I) y2          -(DOUBLE *) Second Y coordinate of line segment to be clipped.
    (I) xwmin       -(DOUBLE) X minimum value for window 
                        (dimensions ((xwmin, ywmin),(xwmax,ywmax))).
    (I) ywmin       -(DOUBLE) Y minimum value for window.
    (I) xwmax       -(DOUBLE) X maximum value for window.
    (I) ywmax       -(DOUBLE) Y maximum value for window.
    (M) u1          -(DOUBLE *) First parameter for line segment (must be 0).
    (M) u2          -(DOUBLE *) Second parameter for line segment (must be 1).
    (O) visible     -(BOOL *) Is the line visible in window?

Return Value/Status:
    Returns the result of the clipping test.
    
Scope:
    PUBLIC

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT ag_clip_1 (DOUBLE *x1, DOUBLE *y1, DOUBLE *x2, DOUBLE *y2, DOUBLE xwmin,
                         DOUBLE ywmin, DOUBLE xwmax, DOUBLE ywmax, DOUBLE *u1,
                         DOUBLE *u2, BOOL *visible) 
#else
publicdef INT ag_clip_1 (x1, y1, x2, y2, xwmin, ywmin, xwmax, ywmax, u1, u2, visible)
DOUBLE *x1;
DOUBLE *y1;
DOUBLE *x2;
DOUBLE *y2;
DOUBLE xwmin;
DOUBLE ywmin;
DOUBLE xwmax;
DOUBLE ywmax;
DOUBLE *u1;
DOUBLE *u2;
BOOL *visible;
#endif
    {
    DOUBLE dx = *x2 - *x1;
    *u1 = 0.0;
    *u2 = 1.0;
    
    if (cliptest (-dx, *x1 - xwmin, u1, u2))
        {
        if (cliptest (dx, xwmax - *x1, u1, u2))
            {
            DOUBLE dy = *y2 - *y1;
            if (cliptest (-dy, *y1 - ywmin, u1, u2))
                {
                if (cliptest (dy, ywmax - *y1, u1, u2))
                    {
                    DOUBLE x0 = *x1;
                    DOUBLE y0 = *y1;
                    if (*u1 > 0.0)
                        {
                        *x1 = x0 + *u1 * dx;
                        *y1 = y0 + *u1 * dy;
                        }
                    if (*u2 < 1.0)
                        {
                        *x2 = x0 + *u2 * dx;
                        *y2 = y0 + *u2 * dy;
                        }
                    return *visible = TRUE;
                    }
                }
            }
        }
    return *visible = FALSE;
    }
/* END:     */
