/* DEC/CMS REPLACEMENT HISTORY, Element UGCMTM.C*/
/* *1    19-JUN-1989 13:34:36 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element UGCMTM.C*/

/******************************************************************************

                Copyright Exploration Systems, Inc. 1989
		   Unpublished -- All rights reserved

THIS SOFTWARE IS THE PROPRIETARY PROPERTY OF Exploration Systems, Inc.  AND MAY
CONTAIN CONFIDENTIAL TRADE SECRET INFORMATION. IT IS LICENSED FOR USE ON THE
DESIGNATED EQUIPMENT ON WHICH IT WAS ORIGINALLY INSTALLED AND  MAY NOT BE
MODIFIED, DUPLICATED OR COPIED IN ANY FORM WITHOUT PRIOR WRITTEN CONSENT OF

			Exploration Systems, Inc.
			579 Market Street
			San Francisco, CA  USA 94105
			(415) 974-1600

(The above notice does not constitute an admission of publication or
unrestricted dissemination of the work)

******************************************************************************/

/********************************************************************
*      Copyright 1988, Exploration Systems Inc. San Francisco
*
* FUNCTION:  ugcmtm
*
* SYNOPSIS: VOID ugcmtm( matrx1, matrx2, comtrx )
*           FLOAT matrx1[];     Input: Segment transformation 1
*           FLOAT matrx2[];     Input: Segment transformation 2
*           FLOAT comtrx[];     Output: Output matrix
*
* DESCRIPTION: ESI's version of NOVA's GKCMTM. Compose Transformation Matrix.
*
* RETURNS: None.
*
* 01/16/89 G Shannon - created
*********************************************************************/
#include "esi_gl_defs.h"

publicdef VOID ugcmtm(matrx1,matrx2,comtrx)
FLOAT matrx1[6];
FLOAT matrx2[6];
FLOAT comtrx[6];
{
    FLOAT arr[2][3];
    INT a, b, c, d, e, f;

/* translate array into matrix which emulates Fortran-order */
    arr[0][0] = matrx2[0];
    arr[0][1] = matrx2[2];
    arr[0][2] = matrx2[4];
    arr[1][0] = matrx2[1];
    arr[1][1] = matrx2[3];
    arr[1][2] = matrx2[5];

/*        REAL MATRX1(6),MATRX2(2,3) */

/* COMTRX (I, J) = SUM <K = 1,2> {MATRX2 (I, K) * MATRX1 (K, J)},
		       FOR I, J = 1, 2                  */

    for (b=1; b<3; ++b)
	{
	d = 2 * (b - 1);
	for (a=0; a<2; ++a)
	    {
	    f = d + a;
	    comtrx[f] = 0;
	    for (e=0; e<2; ++e)
		{
		comtrx[f] += arr[a][e]*matrx1[d+e];
		}
	}
    }

/* COMTRX (I, 3) = MATRX2 (I, 3) + SUM <J = 1,2> {MATRX2 (I, J) * MATRX1 (J, 3)}
		       FOR I = 1, 2          */

    for (a=0; a<2; ++a)
	{
	c = 4 + a;
	comtrx[c] = arr[a][2];
	for (b=0; b<2; ++b)
	    {
	    comtrx[c] += arr[a][b] * matrx1[4+b];
	    }
	}
    return;
}

                                                          
