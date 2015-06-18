/* DEC/CMS REPLACEMENT HISTORY, Element AG_CLIP_2.C */
/* *3    14-AUG-1990 11:39:18 VINCE "(SPR 5644) Header Standardization" */
/* *2     9-FEB-1990 12:48:53 GILLESPIE "(SPR 6005) Moved from nlz_clip_vis.c" */
/* *1     9-FEB-1990 12:38:56 GILLESPIE "Cohen - Sutherland clipping window clipping algorithm" */
/* DEC/CMS REPLACEMENT HISTORY, Element AG_CLIP_2.C */
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

/* Function Description -----------------------------------------------------
Description:
    Cohen-Sutherland clipping algorithm.  This routine accept two rows and
    the boundaries of window, the returned visible flag will be set to be
    TRUE, if part of the segment, which is constructed by two given rows,
    is inside the window, otherwise will be FALSE.
   
Prototype:
    publicdef INT ag_clip_2(DOUBLE *x1, DOUBLE *y1, DOUBLE *x2, DOUBLE *y2, 
                        DOUBLE minx, DOUBLE miny, DOUBLE maxx, DOUBLE maxy, 
                        DOUBLE *const1, DOUBLE *const2, BOOL *visible);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) x1          -(DOUBLE *) The x-coord. of GIVEN ROW #1.
    (I) y1          -(DOUBLE *) The Y-coord. of GIVEN ROW #1.
    (I) x2          -(DOUBLE *) The x-coord. of GIVEN ROW #2.
    (I) y2          -(DOUBLE *) The Y-coord. of GIVEN ROW #2.
    (I) minx        -(DOUBLE) The minmum x-coord. value of window.
    (I) miny        -(DOUBLE) The minmum y-coord. value of window.
    (I) maxx        -(DOUBLE) The maxmum X-coord. value of window.
    (I) maxy        -(DOUBLE) The maxmum y-coord. value of window.
    (I) const1      -(DOUBLE *) The interpreting constant of row 1.
    (I) const2      -(DOUBLE *) The interpreting constant of row 2.
    (O) visible     -(BOOL *) The returned visible flag.

Return Value/Status:
    The returned position of rows will be changed, if the segment have been
    clipped, that means part of segment is inside of window and part of it is
    outside of window.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT ag_clip_2 (DOUBLE *x1, DOUBLE *y1, DOUBLE *x2, DOUBLE *y2, DOUBLE minx,
                         DOUBLE miny, DOUBLE maxx, DOUBLE maxy, DOUBLE *const1,
                         DOUBLE *const2, BOOL *visible) 
#else
publicdef INT ag_clip_2 (x1, y1, x2, y2, minx, miny, maxx, maxy, const1, const2,
                         visible)
DOUBLE *x1;
DOUBLE *y1;
DOUBLE *x2;
DOUBLE *y2;
DOUBLE minx;
DOUBLE miny;
DOUBLE maxx;
DOUBLE maxy;
DOUBLE *const1;
DOUBLE *const2;
BOOL *visible;
#endif
    {
    INT c1, c2, c;
    BOOL keepon = TRUE;
    DOUBLE x, y;
    DOUBLE const_temp;
    
    /*   convert position code for rows.        */
    *const1 = *const2 = 1.0;
    ag_clip_code (*x1, *y1, minx, miny, maxx, maxy, &c1);
    ag_clip_code (*x2, *y2, minx, miny, maxx, maxy, &c2);
    
    while (keepon EQUALS TRUE)
        {
        
        /* to decide the visibility for each row.*/
        
        if (c1 EQUALS AG_IN_WINDOW AND c2 EQUALS AG_IN_WINDOW)
            keepon = FALSE;
        else
            {
            if ((c1 & c2)IS_NOT_EQUAL_TO AG_IN_WINDOW)
                {
                *visible = FALSE;
                return *visible;
                }
            else
                {
                c = (c1 EQUALS AG_IN_WINDOW) ? c2 : c1;
                if ((c & AG_LEFT_WINDOW)EQUALS AG_LEFT_WINDOW)
                    {
                    x = minx;
                    const_temp = (minx - *x1) / (*x2 - *x1);
                    y = *y1 + (*y2 - *y1) * const_temp;
                    }
                else if ((c & AG_RIGHT_WINDOW)EQUALS AG_RIGHT_WINDOW)
                    {
                    x = maxx;
                    const_temp = (maxx - *x1) / (*x2 - *x1);
                    y = *y1 + (*y2 - *y1) * const_temp;
                    }
                else if ((c & AG_BOTTOM_WINDOW)EQUALS AG_BOTTOM_WINDOW)
                    {
                    y = miny;
                    const_temp = (miny - *y1) / (*y2 - *y1);
                    x = *x1 + (*x2 - *x1) * const_temp;
                    }
                else if ((c & AG_TOP_WINDOW)EQUALS AG_TOP_WINDOW)
                    {
                    y = maxy;
                    const_temp = (maxy - *y1) / (*y2 - *y1);
                    x = *x1 + (*x2 - *x1) * const_temp;
                    }
                if (c EQUALS c1)
                    {
                    *const1 = const_temp;
                    *x1 = x;
                    *y1 = y;
                    ag_clip_code (x, y, minx, miny, maxx, maxy, &c1);
                    }
                else
                    {
                    *const2 = const_temp;
                    *x2 = x;
                    *y2 = y;
                    ag_clip_code (x, y, minx, miny, maxx, maxy, &c2);
                    }
                }
            }
        }
    return *visible = TRUE;
    }
/* END:     */
