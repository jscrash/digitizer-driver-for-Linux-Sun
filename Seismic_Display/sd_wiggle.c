/* DEC/CMS REPLACEMENT HISTORY, Element SD_WIGGLE.C*/
/* *3    17-AUG-1990 22:14:50 VINCE "(SPR 5644) Code Cleanup"*/
/* *2    11-AUG-1989 13:41:38 JULIAN "(SPR -1) function sdwiggl has return(e),and return;"*/
/* *1    19-JUN-1989 13:23:23 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element SD_WIGGLE.C*/
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

#include "esi_gtypes.h"

/* ---------- Externals ---------------------------------- */
/* Maximum points in accumulator */
#define MAXPNTS 200
/* Buffer points in polyline or fill area. */
static FLOAT xaccum[MAXPNTS], yaccum[MAXPNTS];

/* Parameters for wiggle drawing. */
static INT negci = -1, posci = -1;
static DOUBLE xscale = 1.0, baseamp = 0.0;
static INT ifneg = FALSE, ifpos = TRUE;

/* --------- MACROS -------------------------------------- */
#define SIDEOF (amplitud[ia] < baseamp) ? FALSE : TRUE

/* File Description ---------------------------------------------------------
Overview:
    Draw low level seismic primitives, wiggle traces, variable density, etc.
    This code uses GKS graphics where possible to allow use with pen plotters
    and other devices.

Prototypes and one line descriptions of each function in the file.
Public_Functions:
    publicdef INT sddfwig(DOUBLE baseline, DOUBLE xfactor, INT negfill, INT posfill);
    
    publicdef DOUBLE uginterp(DOUBLE x1,DOUBLE x2,DOUBLE xval,DOUBLE y1,DOUBLE y2);
    
    publicdef INT sdwiggl(INT numpnts, FLOAT amplitud[], DOUBLE yinc, DOUBLE startx, 
        DOUBLE starty);

Private_Functions:

-----------------------------------------------------------------------------*/

/* Function Description -----------------------------------------------------
Description:
    Define parameters for wiggle trace drawing.  

Prototype:
    publicdef INT sddfwig(DOUBLE baseline, DOUBLE xfactor, INT negfill, INT posfill);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) baseline    -(DOUBLE) Baseline amplitude for splitting colors.
    (I) xfactor     -(DOUBLE) Expansion factor to convert amplitude
                        to x world coordinates.
    (I) negfill     -(INT) Color index for negitive fill, - if no fill.
    (I) posfill     -(INT) Color index for positive fill, - if no fill.

Return Value/Status:
    None.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT sddfwig (DOUBLE baseline, DOUBLE xfactor, INT negfill, INT posfill)
#else
publicdef INT sddfwig (baseline, xfactor, negfill, posfill)
DOUBLE baseline, xfactor;
INT negfill, posfill;
#endif
    {
    /* Perform Trace Operation */
    
    /* Set external parameters. */
    baseamp = baseline;
    xscale = xfactor;
    
    /* Determine if filling is to be done. */
    ifneg = (negfill >= 0);
    ifpos = (posfill >= 0);
    negci = ABS (negfill);
    posci = ABS (posfill);
    
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    Draw a single seismic wiggle trace.  

Prototype:
    publicdef INT sdwiggl(INT numpnts, FLOAT amplitud[], DOUBLE yinc, DOUBLE startx, 
        DOUBLE starty);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) numpnts     -(INT) Number of points in amplitude array.
    (I) amplitud    -(FLOAT_ARRAY) Seismic amplitude.
    (I) yinc        -(DOUBLE) Y increment, delta y per sample.
                        (A negative YINC will cause the drawing to go down the screen.)
    (I) startx      -(DOUBLE) X World Coordinate of first sample.
    (I) starty      -(DOUBLE) Y World Coordinate of first sample.

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT sdwiggl (INT numpnts, FLOAT amplitud[], DOUBLE yinc, DOUBLE startx,
                       DOUBLE starty)
#else
publicdef INT sdwiggl (numpnts, amplitud, yinc, startx, starty)
INT numpnts;
FLOAT amplitud[];
DOUBLE yinc, startx, starty;
#endif
    {
    DOUBLE xcross, ycross;          /* baseline crossing point */
    DOUBLE x1, x2, y1, y2;          /* temp values used for displaying points. */
    INT ia, accumpntr;
    INT oldside, newside;
    INT faci, ifshaded, fais;
    DOUBLE uginterp ();
    
    /* Perform Trace operation */
    
    /* Check for bad count. */
    if (numpnts <= 0)
        {
        return FAIL;
        }
    /* Initialize indices. */
    accumpntr = 0;
    ia = 0;
    oldside = SIDEOF;
    
    /* set initial points in trace */
    xaccum[accumpntr] = startx;
    yaccum[accumpntr] = starty;
    accumpntr++;
    
    /* Set fill area interior style to solid. */
    fais = 1;
    wm_gsfais (fais);
    
    /* Loop through all points in trace. */
    for (ia = 0; ia < numpnts; ia++)
        {
        /* calculate next point to display */
        x1 = x2;
        y1 = y2;
        x2 = startx + (amplitud[ia] * xscale);
        y2 = starty + (yinc * ia);
        
        /* Determine side of baseline and -> the mode. */
        newside = SIDEOF;
        if ((oldside != newside) || (accumpntr >= MAXPNTS - 4))
            {
            /* Set parameters based on side of baseline. */
            faci = (oldside) ? posci : negci;
            ifshaded = (oldside) ? ifpos : ifneg;
            
            /* Calculate baseline crofssing. */
            xcross = startx + (baseamp * xscale);
            ycross = uginterp (x1, x2, xcross, y1, y2);
            
            /* Add crossing point to end of accumulated array. */
            xaccum[accumpntr] = xcross;
            yaccum[accumpntr] = ycross;
            accumpntr++;
            
            /* Display as filled area or polyline */
            if (ifshaded)
                {                   /* draw as filled area */
                wm_gsfaci (faci);
                wm_gfa (accumpntr, xaccum, yaccum); /* filled area */
                }
            else
                {
                wm_gsplci (faci);
                wm_gpl (accumpntr, xaccum, yaccum);
                }
            /* Add crossing point to beginning of accumulated array. */
            accumpntr = 0;
            xaccum[accumpntr] = xcross;
            yaccum[accumpntr] = ycross;
            accumpntr++;
            oldside = newside;
            }
        /* Accumulate points for output */
        xaccum[accumpntr] = x2;
        yaccum[accumpntr] = y2;
        accumpntr++;
        
        }
    /* Dump last of accumulated points. -------------------------- */
    /* Set parameters based on side of baseline. */
    faci = (oldside) ? posci : negci;
    ifshaded = (oldside) ? ifpos : ifneg;
    
    /* Calculate baseline crofssing. */
    xcross = startx + (baseamp * xscale);
    ycross = y2;
    
    /* Add crossing point to end of accumulated array. */
    xaccum[accumpntr] = xcross;
    yaccum[accumpntr] = ycross;
    accumpntr++;
    
    /* Display as filled area or polyline */
    if (ifshaded)
        {                           /* draw as filled area */
        wm_gsfaci (faci);
        wm_gfa (accumpntr, xaccum, yaccum); /* filled area */
        }
    else
        {
        wm_gsplci (faci);
        wm_gpl (accumpntr, xaccum, yaccum);
        }
    return SUCCESS;
    
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    Interpolate a y value from a known x value on a line.

Prototype:
    publicdef DOUBLE uginterp(DOUBLE x1,DOUBLE x2,DOUBLE xval,DOUBLE y1,DOUBLE y2);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    x1              -(DOUBLE) -\
    x2              -(DOUBLE) Line x endpoints.
    xval            -(DOUBLE) X value of special point on line.
    y1              -(DOUBLE) -\
    y2              -(DOUBLE) Line y endpoints.

Return Value/Status:
    Function returns the value of yinterp.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef DOUBLE uginterp (DOUBLE x1, DOUBLE x2, DOUBLE xval, DOUBLE y1, DOUBLE y2)
#else
publicdef DOUBLE uginterp (x1, x2, xval, y1, y2)
DOUBLE x1, x2;
DOUBLE xval;
DOUBLE y1, y2;
#endif
    {
    DOUBLE ratio, yinterp, xdenom;
    
    /* Check for zero divide */
    xdenom = x2 - x1;
    if (xdenom == 0.0)
        {                           /* return degenerate solution */
        yinterp = y1;
        }
    else
        {                           /* interpolate for y value */
        ratio = (xval - x1) / xdenom;
        yinterp = ratio * (y2 - y1) + y1;
        }
    return(yinterp);
    }
/* END:     */
