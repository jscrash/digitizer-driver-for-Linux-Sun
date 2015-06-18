/* DEC/CMS REPLACEMENT HISTORY, Element JG_CLIP_LINE.C*/
/* *3    17-AUG-1990 21:55:55 VINCE "(SPR 5644) Code Cleanup"*/
/* *2    10-FEB-1990 09:45:25 GILLESPIE "(SPR 6005) Use ag_clip_code instead of nlz_code"*/
/* *1    19-JUN-1989 13:01:24 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element JG_CLIP_LINE.C*/
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
    Clipping routine copied (with modifications) from 
    NLZ_CLIP_VIS.  This routine accept two points and the boundaries
    of window, the returned visible flag will be set to be TRUE if part
    of the segment is inside the window, otherwise will be FALSE.

    The returned position of points will be changed and the modified flag
    will be set to be TRUE if the segment has been clipped.

    Thus there are four states that can be returned by this routine:

        1) visible = FALSE, modified = FALSE (segment outside window),
        2) visible = FALSE, modified = TRUE  (segment outside window),
        3) visible = TRUE, modified = FALSE  (segment fully inside window),
        4) visible = TRUE, modified = TRUE   (segment partly inside window),

Prototype:
    publicdef INT jg_clip_line(DOUBLE *x1,DOUBLE *y1,DOUBLE *x2,DOUBLE *y2,DOUBLE *minx,
        DOUBLE *miny,DOUBLE *maxx,DOUBLE *maxy,BOOL *visible,BOOL *modified);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) x1          -(DOUBLE *) The x-coord. of GIVEN POINT #1.
    (I) y1          -(DOUBLE *) The Y-coord. of GIVEN POINT #1.
    (I) x2          -(DOUBLE *) The x-coord. of GIVEN POINT #2.
    (I) y2          -(DOUBLE *) The Y-coord. of GIVEN POINT #2.
    (I) minx        -(DOUBLE) The minimum x-coord. value of window. 
    (I) miny        -(DOUBLE) The minimum y-coord. value of window.
    (I) maxx        -(DOUBLE) The maximum X-coord. value of window.
    (I) maxy        -(DOUBLE) The maximum y-coord. value of window.
    (O) visible     -(BOOL *) The returned visible flag.         
    (O) modified    -(BOOL *) Has the segment been clipped?

Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT jg_clip_line (DOUBLE *x1, DOUBLE *y1, DOUBLE *x2, DOUBLE *y2,
                            DOUBLE minx, DOUBLE miny, DOUBLE maxx, DOUBLE maxy,
                            BOOL *visible, BOOL *modified)
#else
publicdef INT jg_clip_line (x1, y1, x2, y2, minx, miny, maxx, maxy, visible, modified)
DOUBLE *x1;
DOUBLE *y1;
DOUBLE *x2;
DOUBLE *y2;
DOUBLE minx;
DOUBLE miny;
DOUBLE maxx;
DOUBLE maxy;
BOOL *visible;
BOOL *modified;
#endif
    {
    
    INT c1, c2, c;                  /*%P     flag of position code.            */
    DOUBLE x, y;                    /*%P     temp. point.                      */
    DOUBLE const_temp;
    
    /* ********************************************************************** */
    
    *visible = TRUE;
    *modified = FALSE;
    
    /* GET POSITION CODES FOR POINTS */
    
    ag_clip_code (*x1, *y1, minx, miny, maxx, maxy, &c1);
    ag_clip_code (*x2, *y2, minx, miny, maxx, maxy, &c2);
    
    while ((c1 != AG_IN_WINDOW) OR (c2 != AG_IN_WINDOW))
        {
        
        /* LINE SEGMENT TOTALLY OUTSIDE WINDOW */
        
        if ((c1 & c2) IS_NOT_EQUAL_TO AG_IN_WINDOW)
            {
            *visible = FALSE;
            break;
            }
        /* LINE SEGMENT MAY BE PARTIALLY IN WINDOW */
        
        else
            {
            *modified = TRUE;
            c = (c1 EQUALS AG_IN_WINDOW) ? c2 : c1;
            
            if ((c & AG_LEFT_WINDOW) EQUALS AG_LEFT_WINDOW)
                {
                x = minx;
                const_temp = (minx - *x1) / (*x2 - *x1);
                y = *y1 + (*y2 - *y1) * const_temp;
                }
            else if ((c & AG_RIGHT_WINDOW) EQUALS AG_RIGHT_WINDOW)
                {
                x = maxx;
                const_temp = (maxx - *x1) / (*x2 - *x1);
                y = *y1 + (*y2 - *y1) * const_temp;
                }
            else if ((c & AG_BOTTOM_WINDOW) EQUALS AG_BOTTOM_WINDOW)
                {
                y = miny;
                const_temp = (miny - *y1) / (*y2 - *y1);
                x = *x1 + (*x2 - *x1) * const_temp;
                }
            else if ((c & AG_TOP_WINDOW) EQUALS AG_TOP_WINDOW)
                {
                y = maxy;
                const_temp = (maxy - *y1) / (*y2 - *y1);
                x = *x1 + (*x2 - *x1) * const_temp;
                }
            /* MODIFY 1st POINT AND TRY AGAIN */
            if (c EQUALS c1)
                {
                *x1 = x;
                *y1 = y;
                ag_clip_code (x, y, minx, miny, maxx, maxy, &c1);
                }
            /* MODIFY 2nd POINT AND TRY AGAIN */
            else
                {
                *x2 = x;
                *y2 = y;
                ag_clip_code (x, y, minx, miny, maxx, maxy, &c2);
                }
            }
        }
    return SUCCESS;
    }
/* END:     */
