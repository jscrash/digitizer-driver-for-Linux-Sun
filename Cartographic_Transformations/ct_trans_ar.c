/* DEC/CMS REPLACEMENT HISTORY, Element CT_TRANS_AR.C*/
/* *4    14-AUG-1990 23:38:14 GILLESPIE "(SPR 5644) Fix up last checkin"*/
/* *3    14-AUG-1990 13:07:23 VINCE "(SPR 5644) Header Standardization"*/
/* *2    17-OCT-1989 15:45:37 CONROY "(SPR 5020) rework with new paramters"*/
/* *1    19-JUN-1989 12:45:27 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element CT_TRANS_AR.C*/
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
#include "esi_tc.h"
#include "esi_ct.h"
#include "esi_math.h"

#if USE_PROTOTYPES
static DOUBLE ct_length (DOUBLE x1, DOUBLE y1, DOUBLE x2, DOUBLE y2);
#else
static DOUBLE ct_length ();
#endif

/* File Description ---------------------------------------------------------
Overview:
    Transform points from one projection to another.
    
Prototypes and one line descriptions of each function in the file.
Public_Functions:
    publicdef INT ct_transform_array(DOUBLE *x_array_in, DOUBLE *y_array_in,
                                 INT no_of_pts_in, PROJECTION_STRUCTURE *projection_in,
                                 DOUBLE **x_array_out, DOUBLE **y_array_out,
                                 INT *no_of_pts_out, PROJECTION_STRUCTURE *projection_out,
                                 DOUBLE conversion_tolerance);

Private_Functions:
    static DOUBLE ct_length(DOUBLE x1, DOUBLE y1, DOUBLE x2, DOUBLE y2);

-----------------------------------------------------------------------------*/

/* Function Description -----------------------------------------------------
Description:
    Transform the points contained in an array from one projection to another.
    If conversion_tolerance is non-zero, interpolate so that the distance     
    between pairs of returned points is <= conversion_tolerance.

Prototype:
    publicdef INT ct_transform_array(DOUBLE *x_array_in, DOUBLE *y_array_in,
                                 INT no_of_pts_in, PROJECTION_STRUCTURE *projection_in,
                                 DOUBLE **x_array_out, DOUBLE **y_array_out,
                                 INT *no_of_pts_out, PROJECTION_STRUCTURE *projection_out,
                                 DOUBLE conversion_tolerance);
                                 
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) x_array_in      -(DOUBLE *) Points to be transformed.
    (I) y_array_in      -(DOUBLE *) Points to be transformed.
    (I) no_of_pts_in    -(INT) Number of points to be transformed.
    (I) projection_in   -(PROJECTION_STRUCTURE *) Projection of input points.
    (O) x_array_out     -(DOUBLE **) Transformed points to be output.
    (O) y_array_out     -(DOUBLE **) Transformed points to be output.
    (O) no_of_pts_out   -(INT *) Number of output points.
    (O) projection_out  -(PROJECTION_STRUCTURE *) Destination projection.
    (I) conversion_tolerance-(DOUBLE) Maximum distance between returned points.

Return Value/Status:
    SUCCESS - Successful completion
    Function also returns status of ct_tranform_point.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT ct_transform_array (DOUBLE *x_array_in, DOUBLE *y_array_in,
                                  INT no_of_pts_in,
                                  PROJECTION_STRUCTURE *projection_in,
                                  DOUBLE **x_array_out, DOUBLE **y_array_out,
                                  INT *no_of_pts_out,
                                  PROJECTION_STRUCTURE *projection_out,
                                  DOUBLE conversion_tolerance) 
#else
publicdef INT ct_transform_array (x_array_in, y_array_in, no_of_pts_in, projection_in,
                                  x_array_out, y_array_out, no_of_pts_out,
                                  projection_out, conversion_tolerance)
DOUBLE *x_array_in;
DOUBLE *y_array_in;
INT no_of_pts_in;
PROJECTION_STRUCTURE *projection_in;
DOUBLE **x_array_out;
DOUBLE **y_array_out;
INT *no_of_pts_out;
PROJECTION_STRUCTURE *projection_out;
DOUBLE conversion_tolerance;
#endif
    {
    INT status;
    INT in, out, n, j;              /*  Loop indices                 */
    DOUBLE distance;                /*  Distance between points      */
    DOUBLE yinc, xinc;
    DOUBLE xtemp, ytemp;
    DOUBLE *xbuf, *ybuf;
    
    /***************************************************************************/
    
    xbuf = (DOUBLE *)tc_alloc (no_of_pts_in * sizeof(DOUBLE));
    ybuf = (DOUBLE *)tc_alloc (no_of_pts_in * sizeof(DOUBLE));
    
    if (conversion_tolerance <= 0.0)
        {
        
        for (out = 0; out < no_of_pts_in; out++)
            {
            if ((status = ct_transform_point (x_array_in[out], y_array_in[out],
                                              projection_in, &(xbuf[out]),
                                              &(ybuf[out]),
                                              projection_out)) != SUCCESS)
                {
                break;
                }
            }
        }
    else
        {
        out = 0;
        n = no_of_pts_in;
        
        for (in = 0; in < no_of_pts_in; in++)
            {
            xtemp = x_array_in[in];
            ytemp = y_array_in[in];
            
            if (in < no_of_pts_in - 1)
                {
                j = in + 1;
                distance = ct_length (xtemp, ytemp, x_array_in[j], y_array_in[j]);
                xinc = (xtemp - x_array_in[j]) / distance * conversion_tolerance;
                yinc = (ytemp - y_array_in[j]) / distance * conversion_tolerance;
                }
            else
                {
                distance = 0.0;
                xinc = 0.0;
                yinc = 0.0;
                }
            distance = MAX (distance, conversion_tolerance / 2.0);
            
            FOREVER
                {
                if (out EQUALS n)
                    {
                    n += 20;
                    xbuf = (DOUBLE *)tc_realloc (xbuf, n * sizeof(DOUBLE));
                    ybuf = (DOUBLE *)tc_realloc (ybuf, n * sizeof(DOUBLE));
                    }
                if ((status = ct_transform_point (xtemp, ytemp, projection_in,
                                                  &(xbuf[out]), &(ybuf[out]),
                                                  projection_out)) != SUCCESS)
                    {
                    break;
                    }
                out++;
                
                distance -= conversion_tolerance;
                if (distance <= 0.0)
                    {
                    break;
                    }
                xtemp += xinc;
                ytemp += yinc;
                }
            }
        }
    if (status != SUCCESS)
        {
        tc_free (xbuf);
        tc_free (ybuf);
        *no_of_pts_out = 0;
        }
    else
        {
        *x_array_out = xbuf;
        *y_array_out = ybuf;
        *no_of_pts_out = out;
        }
    return status;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    Function to compute distance between two points.
    
Prototype:
    static DOUBLE ct_length(DOUBLE x1, DOUBLE y1, DOUBLE x2, DOUBLE y2);
                                 
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) x1          -(DOUBLE) X-coordinate of distance formula (1).
    (I) y1          -(DOUBLE) Y-coordinate of distance formula (1).
    (I) x2          -(DOUBLE) X-coordinate of distance formula (2).
    (I) y2          -(DOUBLE) Y-coordinate of distance formula (2).
    
Return Value/Status:
    Function returns the distance from (x1,y1) to (x2,y2).
    
Scope:
    PRIVATE to <ct_transform_array>
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static DOUBLE ct_length (DOUBLE x1, DOUBLE y1, DOUBLE x2, DOUBLE y2) 
#else
static DOUBLE ct_length (x1, y1, x2, y2)
DOUBLE x1, y1, x2, y2;
#endif
    {
    return sqrt ((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
    }
/* END:     */
