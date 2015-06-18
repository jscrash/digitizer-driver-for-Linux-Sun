/* DEC/CMS REPLACEMENT HISTORY, Element GB_DRW_PLYARR.C */
/* *6    16-AUG-1991 11:35:10 JTM "(SPR 0) Cast arg to wm_gslwsc() to DOUBLE." */
/* *5    15-JUL-1991 17:38:05 JULIAN "(SPR 1570) fixed cancel from polygon retrieval problem" */
/* *4    29-SEP-1989 13:21:01 GILLESPIE "(SPR 1) Normal is NOT spelled NROMAL" */
/* *3    28-SEP-1989 10:19:50 GILLESPIE "(SPR 1) Change WHITE to NORMAL" */
/* *2    14-SEP-1989 18:17:40 GORDON "(SPR -1) checkin from gulf" */
/* *1    14-SEP-1989 17:32:23 GORDON "a" */
/* DEC/CMS REPLACEMENT HISTORY, Element GB_DRW_PLYARR.C */
/* **********************************************************************
 
    ROUTINE:    gb_draw_polygon_array
    
    AUTHOR: S. C. Darden, June 11, 1989.
 
    DESCRIPTION:
 
            gb_draw_polygon_array interacts with user to construct a retrieval
            polygon.
 
    INITIAL CONDITIONS:
    
            1.  map window is opened and transformation selected
    
    PARAMETERS:
     
            INT     *npoints    output: number of points in returned polygon
            DOUBLE  *xmin       output: bounding rectangle surrounding polygon, ....
            DOUBLE  *ymin
            DOUBLE  *xmax
            DOUBLE  *ymax
            DOUBLE  **x         output: x vector of polygon
            DOUBLE  **y         output: y vector of polygon
            
    RETURNS STATUS:
    
            SUCCESS     if valid polygon returned
            FAIL        if user canceled, or drew less than 3 points
 
    ********************************************************************** */

/* **************************** INCLUDE FILES ***************************** */

#include "esi_gl_defs.h"
#include "esi_gks.h"
#include "esi_math.h"
#include "esi_nl.h"
#include "esi_tc.h"
#include "esi_lu.h"
#include "esi_wm.h"

#define ALLOC_POINTS 5      /* number of polygon points allocated per block */
static INT zz_add_point ();

/* ****************************** PROCEDURE ******************************** */

publicdef INT gb_draw_polygon_array (npoints, xmin, ymin, xmax, ymax, x, y)
INT *npoints;
DOUBLE *xmin,*ymin,*xmax,*ymax;
DOUBLE **x;
DOUBLE **y;
    {
    INT status = SUCCESS, wm_status, button, done;
    INT max_points = ALLOC_POINTS;                      /* num points currently allocated */
    FLOAT firstx, firsty, lastx, lasty, wx, wy;
    GKS_REAL xarray[2], yarray[2];
    GKS_REAL old_wid; 
    GKS_INT  old_clr,gks_stat;
    GKS_INT  color;

    *xmin = HUGE_VAL, *ymin = HUGE_VAL, *xmax = -HUGE_VAL, *ymax = -HUGE_VAL;

    /*----------------------------------------------------*/
    /* allocate the returned arrays to initial block size */
    /*----------------------------------------------------*/

    *x = (DOUBLE *) tc_alloc (max_points * sizeof(DOUBLE));
    *y = (DOUBLE *) tc_alloc (max_points * sizeof(DOUBLE));

    *npoints = 0;
    wm_gqplci(&gks_stat, &old_clr);
    wm_gqlwsc(&gks_stat, &old_wid);
    lu_text_to_index( "COLOR", &color, "NORMAL" );
    wm_gsplci(color);
    wm_gslwsc(1.0);
    wmtlocate (&wm_status, &wx, &wy, &button);
    if (wm_status == SUCCESS && button != WM_BUTTON_CANCEL &&
        button != WM_BUTTON_DONE)
        {
        /*--------------------*/
        /* plant anchor point */
        /*--------------------*/

        firstx = wx, firsty = wy;
        zz_add_point (wx, wy, x, y, npoints, &max_points);
        *xmin = MIN(*xmin, wx), *ymin = MIN(*ymin, wy);
        *xmax = MAX(*xmax, wx), *ymax = MAX(*ymax, wy);
        done = FALSE;
        while (!done)
            {
            lastx = wx;
            lasty = wy;
            wmtbandvector (&wm_status, lastx, lasty, &wx, &wy, &button);
            if (wm_status == SUCCESS && button != WM_BUTTON_CANCEL)
                {
                if (button != WM_BUTTON_DONE)
                    {
                    /*--------------------------------------*/
                    /* plant next & draw vector just banded */
                    /*--------------------------------------*/
                    
                    zz_add_point (wx, wy, x, y, npoints, &max_points);
                    *xmin = MIN(*xmin, wx), *ymin = MIN(*ymin, wy);
                    *xmax = MAX(*xmax, wx), *ymax = MAX(*ymax, wy);
                    xarray[0] = lastx, xarray[1] = wx;
                    yarray[0] = lasty, yarray[1] = wy;
                    wm_gpl (2, xarray, yarray);
                    }
                else
                    done = TRUE;
                }
	    else
	      done = TRUE;
            }
        if (*npoints >= 3 AND button != WM_BUTTON_CANCEL)
            {
            /*-------------------------------------------*/
            /* close the polygon and draw closing vector */
            /*-------------------------------------------*/
            
            zz_add_point (firstx, firsty, x, y, npoints, &max_points);
            xarray[0] = lastx, xarray[1] = firstx;
            yarray[0] = lasty, yarray[1] = firsty;
            wm_gpl (2, xarray, yarray);
            }
        else
            status = FAIL;
        }
    else
        status = FAIL;
    wm_gsplci(old_clr);
    wm_gslwsc((DOUBLE)old_wid);

    return status;
    }

/*---------------------------------------------------*/
/* Private function zz_add_point() is used to        */
/* add a point to the x, y arrays, and to            */
/* ensure that the array bounds are not exceeded     */
/* by dynamically increasing size of the arrays      */
/* to hold as many points as user wants to digitize. */
/*---------------------------------------------------*/

static INT zz_add_point ( newx, newy, x, y, npoints, max_points )
DOUBLE newx, newy;
DOUBLE **x, **y;
INT *npoints;
INT *max_points;
    {
    DOUBLE *px, *py;
    px = *x, py = *y;
    if (*npoints > *max_points - 1)
        {
        *max_points += ALLOC_POINTS;
        *x = px = (DOUBLE *) tc_realloc (*x, *max_points * sizeof(DOUBLE));
        *y = py = (DOUBLE *) tc_realloc (*y, *max_points * sizeof(DOUBLE));
        }
    px[*npoints] = newx, py[*npoints] = newy, (*npoints)++;
    return SUCCESS;
    }
