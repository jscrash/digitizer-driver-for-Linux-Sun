/* DEC/CMS REPLACEMENT HISTORY, Element AG_INSIDE.C */
/*  3B1  24-SEP-1990 16:08:01 GILLESPIE "Merge: Beta Deltas" */
/*  2B1  24-SEP-1990 15:26:46 GILLESPIE "Beta Deltas" */
/* *3    14-AUG-1990 11:39:26 VINCE "(SPR 5644) Header Standardization" */
/* *2    12-FEB-1990 10:00:11 GILLESPIE "(SPR 6008) Move from jg_inside.c" */
/* *1    12-FEB-1990 09:51:28 GILLESPIE "Test to determine if a point is inside a polygon" */
/* DEC/CMS REPLACEMENT HISTORY, Element AG_INSIDE.C */
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
    Boolean function to determine if the point (test_x,test_y) is
    inside (inclusive of boundary) the polygon defined by
    the x and y arrays from start_pnt to start_pnt+npts.

    Algorithm:  Count how many times the polygon passes underneath
                the test point.  If it does so an odd number of
                times then the point is inside, else it is outside.
                Note, if the point lies on the polygon boundary it
                is reported as being inside the polygon.  Also note
                that this routine could be change to report point as
                being inside, outside, or on boundary.  This would
                require possible modifications to those routines
                calling this one.

Prototype:
    publicdef BOOL ag_inside(DOUBLE test_x, DOUBLE test_y, DOUBLE *x,
                         DOUBLE *y, INT start_pnt, INT npts);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) test_x      -(DOUBLE) X coordinate of specified point.
    (I) test_y      -(DOUBLE) Y coordinate of specified point.
    (I) x           -(DOUBLE *) X coord. of array of points in polygon.
    (I) y           -(DOUBLE *) Y coord. of array or points in polygon.
    (I) start_pnt   -(INT) Starting point of polygon.
    (I) npts        -(INT) Number of points in polygon.

Return Value/Status:
    This function returns TRUE or FALSE based on whether or not the specified
    point is inside the polygon.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef BOOL ag_inside (DOUBLE test_x, DOUBLE test_y, DOUBLE *x, DOUBLE *y,
                          INT start_pnt, INT npts) 
#else
publicdef BOOL ag_inside (test_x, test_y, x, y, start_pnt, npts)
DOUBLE test_x;
DOUBLE test_y;
DOUBLE *x;
DOUBLE *y;
INT start_pnt;
INT npts;
#endif
/* * VARIABLE LIST * */
/* ----------------- */
    {
    INT i;
    INT flag = 0;
    BOOL inside = FALSE;
    DOUBLE slope;
    DOUBLE intercept;
    DOUBLE interp_y;
    
    /* ********************************************************************** */
    
    npts += start_pnt - 1;
    
    /* * TEST COINCIDENCE WITH LAST POINT * */
    /* ------------------------------------ */
    
    if (x[npts]EQUALS test_x AND y[npts]EQUALS test_y)
        {
        inside = TRUE;
        }
    /* * RUN AROUND POLYGON, COUNT NUMBER OF TIMES POLYGON PASSES UNDER POINT * */
    /* ------------------------------------------------------------------------ */
    
    if (NOT inside)
        {
        for (i = start_pnt; i < npts; i++)
            {
            
            /* TEST FOR LINE SEG PASSING UNDER POINT */
            
            if ((y[i] > test_y AND y[i + 1] > test_y)OR (x[i] < test_x AND x[i + 1] <=
                                                         test_x)OR (x[i] >
                                                                    test_x AND 
                                                                    x[i + 1] >=
                                                                    test_x))
                {
                continue;
                }
            /* TEST FOR VERTICAL LINE PASSING THRU POINT */
            
            if (x[i]EQUALS x[i + 1])
                {
                if (y[i] < test_y AND y[i + 1] < test_y)
                    {
                    continue;
                    }
                else
                    {
                    flag = 1;
                    break;
                    }
                }
            /* TEST FOR HORIZONTAL LINE PASSING THRU POINT */
            
            if (y[i]EQUALS test_y AND y[i + 1]EQUALS test_y)
                {
                flag = 1;
                break;
                }
            /* TEST FOR LINE SEG TRULY UNDER POINT */
            
            if (y[i] < test_y AND y[i + 1] < test_y)
                {
                flag++;
                }
            else
                {
                slope = (y[i] - y[i + 1]) / (x[i] - x[i + 1]);
                intercept = 0.5 * (y[i] + y[i + 1] - slope * (x[i] + x[i + 1]));
                interp_y = slope * test_x + intercept;
                
                /* LINE SEG PASSES THRU POINT */
                
                if (test_y EQUALS interp_y)
                    {
                    flag = 1;
                    break;
                    }
                /* LINE SEG PASSES UNDER POINT */
                
                else if (test_y > interp_y)
                    {
                    flag++;
                    }
                }
            }
        if ((flag % 2)EQUALS 1)
            {
            inside = TRUE;
            }
        }
    /* * RETURN * */
    /* ---------- */
    
    return inside;
    }
/* END:     */
