/* DEC/CMS REPLACEMENT HISTORY, Element SO_DRFLTPOSTS.C*/
/* *12    8-JAN-1992 17:10:19 JTM "(SPR 0) BHP prompted changes. Added divide by zero check."*/
/* *11   21-DEC-1990 18:24:40 VINCE "(SPR 6174) fixed fault pointing to work for a line at any angle; added half fault processing"*/
/*  7A1B1  3-OCT-1990 11:51:48 GILLESPIE "Change file type to stream"*/
/* *10   29-SEP-1990 11:44:39 GILLESPIE "(SPR 1) MERGE BETA DELTAS"*/
/*  7A1  29-SEP-1990 11:30:39 GILLESPIE "Beta Deltas"*/
/* *9    29-AUG-1990 11:37:53 GILLESPIE "(SPR 1) CP Deltas"*/
/* *8    20-AUG-1990 17:11:14 KEE "(SPR -1) fixing compile error at sun"*/
/* *7    31-JUL-1990 17:51:14 GILLESPIE "(SPR 1) Add esi_gks.h include file"*/
/* *6    26-JUL-1990 17:18:52 GILLESPIE "(SPR 1) Lint run; incorporate changes for horizon end point posting"*/
/* *5    25-JUL-1990 20:02:57 VINCE "(SPR 5644) Header Standardization"*/
/* *4    18-JUL-1990 18:36:42 KEE "(SPR -1) Fix bug (parameter list), add < symbol for up side throw, no U/D label"*/
/* *3     1-JUL-1990 12:35:59 GILLESPIE "(SPR 1) Changes to allow more options for posting faults (incomplete)"*/
/* *2    25-JUL-1989 20:05:13 GILLESPIE "(SPR 50) Seismic system revamp"*/
/* *1    19-JUN-1989 13:26:13 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element SO_DRFLTPOSTS.C*/
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

#include "esi_c_lib.h"
#include "esi_so.h"

#ifndef ESI_WM_H
#include "esi_wm.h"
#endif

#ifndef ESI_GKS_H
#include "esi_gks.h"
#endif

#ifndef ESI_MATH_H
#include "esi_math.h"
#endif

#ifndef ESI_TS_H
#include "esi_ts.h"
#endif

/* Function Description -----------------------------------------------------
Description:
    Function to draw fault posts.

    The following is an attempt to illustrate the points and angles used
    in the code to calculate the lines to be drawn for a fault symbol.

    The fault gap of the seismic line is represented by the line D:S

    Angle ALPHA is the angle (in radians) between the positive X axis
    and the line D:S in the range 0 -> +/-PI  (0 -> +/-180 deg).

                                     Y
                                     |
                                     |
                                     |     O <-(arrow_1)
                                     |                  O <-(bar_1)
                                     |   *          *
          (arrow_3)-> O              |          *
                           *         | *    *
                                *    |  *
                    -----------------O---------------- X
                                 *   |\(D)  )
                             *       | \   )<-(angle alpha)
                         *           |  \ )
                     *               |   \
       (bar_2)-> O                   |    \
                                     |     \
                                     |      O <-(S)

    delta_x = S(x) - D(x)
    delta_y = S(y) - D(y)

    alpha = asin ( delta_y / sqrt( (delta_x*delta_x) + (delta_y*delta_y) ) )

    point 1:	bar_1x = D(x) + length * cos(alpha + 90)
    		bar_1y = D(y) + length * sin(alpha + 90)

    point 2:	bar_2x = D(x) + length * cos(alpha - 90)
		bar_2y = D(y) + length * sin(alpha - 90)

    point 3:	arrow_1x = D(x) + length * cos(alpha + 90 + 45)
		arrow_1y = D(y) + length * sin(alpha + 90 + 45)

		NOTE: arrow_2xy = D(xy)

    point 4:	arrow_3x = D(x) + length * cos(alpha - 90 + 45)
		arrow_3y = D(y) + length * sin(alpha - 90 + 45)


	NOTE:	all lengths are relative to charHeight.
		- total bar length will be 2 * charHeight
		- each arrow leg will be 0.5 * charHeight

Prototype:
    publicdef INT so_draw_fault_posts (COORD iX, COORD iY,
                                       SEIS_HOR_PICK_TYPE iflag,
                                       SEIS_SURFACE_NAME iname,
                                       COORD fX, COORD fY,
                                       SEIS_HOR_PICK_TYPE fflag,
                                       SEIS_SURFACE_NAME fname, DOUBLE Throw,
                                       DOUBLE charHeight);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    iX              -(COORD)
    iY              -(COORD)
    iflag           -(SEIS_HOR_PICK_TYPE)
    iname           -(SEIS_SURFACE_NAME)
    fX              -(COORD)
    fY              -(COORD)
    fflag           -(SEIS_HOR_PICK_TYPE)
    fname           -(SEIS_SURFACE_NAME)
    Throw           -(DOUBLE)
    charHeight      -(DOUBLE)

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT so_draw_fault_posts (COORD iX, COORD iY, SEIS_HOR_PICK_TYPE iflag,
                                   SEIS_SURFACE_NAME iname, COORD fX, COORD fY,
                                   SEIS_HOR_PICK_TYPE fflag,
                                   SEIS_SURFACE_NAME fname,
                                   DOUBLE Throw, DOUBLE charHeight)
#else
publicdef INT so_draw_fault_posts (iX, iY, iflag, iname, fX, fY, fflag, fname,
                                   Throw, charHeight)
COORD iX, iY;
SEIS_HOR_PICK_TYPE iflag;
SEIS_SURFACE_NAME iname;
COORD fX, fY;
SEIS_HOR_PICK_TYPE fflag;
SEIS_SURFACE_NAME fname;
DOUBLE Throw;
DOUBLE charHeight;
#endif
    {
    INT    status = SUCCESS;
    DOUBLE dx, dy;
    DOUBLE alpha, a90, a45, a30;
    CHAR   faultThrow[10];

    /* coordinates of the fault points */
    COORD deep_x, mx, shallow_x;
    COORD deep_y, my, shallow_y;

    /* coordinates for the perpendicular lines */
    DOUBLE bar_length;
    COORD  deep_bar_x1,  deep_bar_x2,  shallow_bar_x1, shallow_bar_x2;
    COORD  deep_bar_y1,  deep_bar_y2,  shallow_bar_y1, shallow_bar_y2;

    /* coordinates for the arrow lines */
    DOUBLE arrow_length;
    COORD  deep_arrow_x1, deep_arrow_x3, shallow_arrow_x1, shallow_arrow_x3;
    COORD  deep_arrow_y1, deep_arrow_y3, shallow_arrow_y1, shallow_arrow_y3;

    /* positions for text labels */
    DOUBLE label_offset, text_up_angle;
    COORD  label_x, label_y;  /* fault throw label   */
    COORD  name_x,  name_y;   /* fault name string   */

    /* variables for GKS calls */
    GKS_REAL px[3], py[3];
    DOUBLE   chh, chux, chuy, chxp, chsp;
    GKS_INT  npts, txalh, txalv, txp;
    
    /* Things to do:
     * 1) handle half-faults (DONE).
     * 2) Introduce the notion of a symbol corresponding to the flag values
     */
    
    /* ---------------------------------------------------------------- */
    /* NOTE:								*/
    /* 1) bar length is used for each side of the bar so it will	*/
    /*    really come out to be 2 bar_lengths long			*/
    /*									*/
    /* 2) since the labels are rotated to the same angle as the	line	*/
    /*	  and the vertical/horizontal justification is set to centered	*/
    /*    a label_offset of 1.5 * charHeight will put the characters	*/
    /*    one-half character height away from the line.			*/
    /* ---------------------------------------------------------------- */

    bar_length   = charHeight;
    arrow_length = charHeight / 2;
    label_offset = 1.5 * charHeight;	/* distance of label from line */

    a90 = D_TO_R( 90.0 );
    a45 = D_TO_R( 45.0 );
	a30 = D_TO_R( 30.0 );  /* JTM 1/8/92 from BHP */

    /* -------------------------------------------------------------------- */
    /* If the throw is positive or there is no initial point then the main  */
    /* point of interest is the final point, otherwise its the initial	    */
    /* point.								    */
    /* -------------------------------------------------------------------- */

    if ((Throw > 0.0) ||
	(iflag == (SEIS_HOR_PICK_TYPE) -1))
        {
        deep_x       = fX;
        deep_y       = fY;
        shallow_x    = iX;
        shallow_y    = iY;
        }
    else
        {
        deep_x       = iX;
        deep_y       = iY;
        shallow_x    = fX;
        shallow_y    = fY;
        }

    if (ABS(Throw) < 1.0)
	strcpy (faultThrow, "equal");
    else
	sprintf (faultThrow, "%.0f", ABS(Throw) );

    /* -------------------- */
    /* calculate the deltas */
    /* -------------------- */

    dx = shallow_x - deep_x;
    dy = shallow_y - deep_y;

    /* ------------------------------------------------------------ */
    /* calculate the small angle of the line relative to the X axis */
    /* this will be in the range 0 -> +/-PI/2  (0 -> +/-90 deg)	    */
    /* ------------------------------------------------------------ */

	/* JTM 1/8/92    BHP submitted the following check for 0 test */
	if( (dx != 0.0) || (dy != 0.0) )
	{
	    alpha = asin(dy / sqrt((dx * dx) + (dy * dy)));
    }
	else
	{
		alpha = -a30;
	}

    /* --------------------------------------------------------	*/
    /* make the angle relative to the POSITIVE X axis this	*/
    /* will change the range to 0 -> +/-PI  (0 -> +/-180 deg)	*/
    /* << subtract it from +/- 180 >>				*/
    /* --------------------------------------------------------	*/

    if (dx < 0.0)
	{
	if (dy < 0)
	    alpha = D_TO_R(-180.0) - alpha;
	else
	    alpha = D_TO_R( 180.0) - alpha;
	}

    /* -------------------------------------------------------- */
    /* calculate the end points of the perpendicular lines	*/
    /*								*/
    /* deep line						*/
    deep_bar_x1 = deep_x + (bar_length * cos(alpha + a90));
    deep_bar_y1 = deep_y + (bar_length * sin(alpha + a90));
    deep_bar_x2 = deep_x + (bar_length * cos(alpha - a90));
    deep_bar_y2 = deep_y + (bar_length * sin(alpha - a90));
    /*								*/
    /* shallow line						*/
    shallow_bar_x1 = shallow_x + (bar_length * cos(alpha + a90));
    shallow_bar_y1 = shallow_y + (bar_length * sin(alpha + a90));
    shallow_bar_x2 = shallow_x + (bar_length * cos(alpha - a90));
    shallow_bar_y2 = shallow_y + (bar_length * sin(alpha - a90));
    /*								*/
    /* -------------------------------------------------------- */

    /* -------------------------------------------------------- */
    /* Calculate the end points for the throw symbol (>)	*/
    /* The symbol is on the deep side, outside the gap,		*/
    /* pointing upslope						*/
    /*								*/
    deep_arrow_x1 = deep_x + (arrow_length * cos(alpha + a90 + a45));
    deep_arrow_y1 = deep_y + (arrow_length * sin(alpha + a90 + a45));
    deep_arrow_x3 = deep_x + (arrow_length * cos(alpha - a90 - a45));
    deep_arrow_y3 = deep_y + (arrow_length * sin(alpha - a90 - a45));
    /*								*/
    /* if Throw is 0.0 the symbol goes on both lines		*/
    /* (also outside the gap, pointing toward the gap)		*/
    /*								*/
    if (ABS(Throw) < 1.0)
        {
        shallow_arrow_x1 = shallow_x + (arrow_length * cos(alpha + a45));
        shallow_arrow_y1 = shallow_y + (arrow_length * sin(alpha + a45));
        shallow_arrow_x3 = shallow_x + (arrow_length * cos(alpha - a45));
        shallow_arrow_y3 = shallow_y + (arrow_length * sin(alpha - a45));
        }
    /*								*/
    /* -------------------------------------------------------- */

    /* -------------------------------------------------------------------- */
    /* Text string positions						    */
    /*									    */
    /* If there were two valid points put the label between them,	    */
    /* otherwise use deep_xy.						    */
    if ((iflag == (SEIS_HOR_PICK_TYPE) -1) ||
	(fflag == (SEIS_HOR_PICK_TYPE) -1)  )
	{
	mx = deep_x;
	my = deep_y;
	}
    else
	{
	mx = deep_x + (dx / 2.0);	/* find the middle of the gap	    */
	my = deep_y + (dy / 2.0);
	}
    /*									    */
    /* position for throw value text string; always above the line	    */
    if (alpha > 0.0)
	{
	label_x = mx + (label_offset * cos(alpha - a90) );
	label_y = my + (label_offset * sin(alpha - a90) );
	}
    else
	{
	label_x = mx + (label_offset * cos(alpha + a90) );
	label_y = my + (label_offset * sin(alpha + a90) );
	}
    /*									    */
    /* position for fault name string; always below the line		    */
    if (alpha > 0.0)
	{
	name_x = mx + (label_offset * cos(alpha + a90) );
	name_y = my + (label_offset * sin(alpha + a90) );
	}
    else
	{
	name_x = mx + (label_offset * cos(alpha - a90) );
	name_y = my + (label_offset * sin(alpha - a90) );
	}
    /*									    */
    /* -------------------------------------------------------------------- */
    
    /*----------------------------------*/
    /* draw perpendicular lines         */
    /*----------------------------------*/
    npts = 2;
    px[0] = deep_bar_x1;
    py[0] = deep_bar_y1;
    px[1] = deep_bar_x2;
    py[1] = deep_bar_y2;
    wm_gpl (npts, px, py);

    /* ------------------------------------------------------------ */
    /* we only draw the rest of the lines if both points were valid */
    /* ------------------------------------------------------------ */

    if ((iflag != (SEIS_HOR_PICK_TYPE) -1) &&
	(fflag != (SEIS_HOR_PICK_TYPE) -1)  )
	{
	npts = 2;
	px[0] = shallow_bar_x1;
	py[0] = shallow_bar_y1;
	px[1] = shallow_bar_x2;
	py[1] = shallow_bar_y2;
	wm_gpl (npts, px, py);

	/*----------------------*/
	/* draw < for deep side */
	/*----------------------*/
	npts = 3;
	px[0] = deep_arrow_x1;
	py[0] = deep_arrow_y1;
	px[1] = deep_x;
	py[1] = deep_y;
	px[2] = deep_arrow_x3;
	py[2] = deep_arrow_y3;
	wm_gpl (npts, px, py);

	/*----------------------------------------------------*/
	/* if the throw is zero label the "shallow" side also */
	/*----------------------------------------------------*/
	if (ABS(Throw) < 1.0)
	    {
	    npts = 3;
	    px[0] = shallow_arrow_x1;
	    py[0] = shallow_arrow_y1;
	    px[1] = shallow_x;
	    py[1] = shallow_y;
	    px[2] = shallow_arrow_x3;
	    py[2] = shallow_arrow_y3;
	    wm_gpl (npts, px, py);
	    }
	}

    /*---------------------------------*/
    /* set the text attributes         */
    /*---------------------------------*/
    chh = charHeight;
    wm_gschh (chh);                 /* character height (from caller)       */

    /* calculate an up vector for the text that will be parallel to the	    */
    /* seismic line and still readable (i.e. not upside down!)		    */

    if (ABS(alpha) < a90)	text_up_angle = alpha + a90;
    else if (ABS(alpha) > a90)	text_up_angle = alpha - a90;
    else /* alpha == +/-90 */	text_up_angle = 180.0;

    chux = cos(text_up_angle);
    chuy = sin(text_up_angle);
    wm_gschup (chux, chuy);

    txp = GRIGHT;
    wm_gstxp (txp);                 /* text path (right)                    */

    chxp = 1.0;
    wm_gschxp (chxp);               /* character expansion (normal)         */

    chsp = 0.0;
    wm_gschsp (chsp);               /* character spacing (normal)           */
    
    /*----------------------------------*/
    /* draw fault throw label           */
    /*----------------------------------*/
    txalh = GACENT;                 /* horizontal center */
    txalv = GAHALF;                 /* vertical center   */
    wm_gstxal (txalh, txalv);
    wm_gtx (label_x, label_y, faultThrow);
    
    /*----------------------------------*/
    /* draw fault name label            */
    /*----------------------------------*/
    if (IS_STRING (iname))
        {
        wm_gstxal (txalh, txalv);
        wm_gtx (name_x, name_y, iname);
        }
    
    /* -------------------------------------------- */
    /* reset text alignment and up vector to normal */
    /* -------------------------------------------- */
    
    txalh = GAHNOR;                 /* normal */
    txalv = GAVNOR;                 /* normal */
    wm_gstxal (txalh, txalv);

    chux = 0.0; chuy = 1.0;
    wm_gschup (chux, chuy);         /* up vector (normal)                   */
    
    return(status);
    }
/* END:     */
