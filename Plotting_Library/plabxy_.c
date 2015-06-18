/* DEC/CMS REPLACEMENT HISTORY, Element PLABXY_.C */
/* *2     1-MAY-1991 11:38:04 KEE "(SPR -1) Modified Gulf Canada DLS/NTS for vms and sun version" */
/* *1     1-MAY-1991 11:34:01 KEE "DLS/NTS" */
/* DEC/CMS REPLACEMENT HISTORY, Element PLABXY_.C */
/************************************************************************


                  Copyright Gulf Canada Resources, 1990
		   Unpublished -- All rights reserved

THIS SOFTWARE IS THE PROPRIETARY PROPERTY OF Gulf Canada Resources.
IT MAY NOT BE USED, MODIFIED, DUPLICATED OR COPIED IN ANY FORM
WITHOUT PRIOR WRITTEN CONSENT OF

			Gulf Canada Resources, Inc.
			401 9th Avenue SW
			Calgary, Alberta
                        Canada T2P 2H7
			(403) 233-4000

(The above notice does not constitute an admission of publication or
unrestricted dissemination of the work)
************************************************************************/
/*Translated by FOR_C++, v0.2, on 09/14/90 at 14:34:21 */
/*FOR_C++ Options SET: cio op=a c */
#include "esi_gl_defs.h"
#include "esi_glue.h"
#include <stdio.h>
#include <math.h>
/* list PLABXY
 *
 *     *** Find optimal label position within an irregular polygon ***
 *
 *     Calling Sequence:
 *
 *     call PLABXY(xpoly,ypoly,np,isense,wratio,xlab,ylab,wmax)
 *
 *     Arguments:
 *
 *     Name    Input/     Type       Description
 *             output
 *
 *     xpoly     I      Real(np)     X coordinates for polygon.
 *
 *     ypoly     I      Real(np)     Y coordinates for polygon.
 *
 *     np        I      Integer      Number of points in polygon.
 *
 *     isense    I      Integer      Sense of direction (-1=clockwise,
 *                                   1=counter-clockwise)
 *
 *     wratio    I       Real        Width to height ratio for rectangle
 *                                   containing label.
 *
 *     xlab      O       Real        X coordinate for label position.
 *
 *     ylab      O       Real        Y coordinate for label position.
 *
 *     wmax      O       Real        Maximum width of label that can be
 *                                   plotted inside the polygon.
 *
 *     Comments:
 *
 *     This routine is Fortran glue to the C routines
 *
 * nolist PLABXY
 */

#ifdef vms
void /*FUNCTION*/ plabxy(xpoly, ypoly, np, isense, wratio, xlab, ylab, 
	 wmax)
float xpoly[], ypoly[];
int *np, *isense;
float *wratio, *xlab, *ylab, *wmax;
{
     double xwpoly[500],ywpoly[500];
     double xwlab,ywlab,wwmax,wwrat,scale_factor;
     int i,nvalues,status;
     static  CHAR request[] = {"GET SCALE FACTOR"};
     static  DSC request_dsc = {10, DSC$K_DTYPE_T, DSC$K_CLASS_S, request};
/*
 *   Get scale factor 
 */
     nvalues=1;
     dstore(&request_dsc,&scale_factor,&nvalues,&status);
     for (i = 0; i < *np; i++)
     {
/*
 *      Call Fortran routine to convert plot inches to world coordinates
 */
        unscl(&xpoly[i] , &ypoly[i], &xwpoly [ i], &ywpoly[i]);
     }
     wwrat = *wratio;
     ag_poly_labelxy(xwpoly, ywpoly, *np, *isense, wwrat, &xwlab, &ywlab, &wwmax);
     geoscl(&xwlab,&ywlab,xlab,ylab);
     *wmax = wwmax * scale_factor;

} /* end of function */
#endif

#if UNIX
void /*FUNCTION*/ plabxy_(xpoly, ypoly, np, isense, wratio, xlab, ylab, 
	 wmax)
float xpoly[], ypoly[];
int *np, *isense;
float *wratio, *xlab, *ylab, *wmax;
{
     double xwpoly[500],ywpoly[500];
     double xwlab,ywlab,wwmax,wwrat,scale_factor;
     char request[40];
     CHAR request_dsc[MAXLEN];
     int i,nvalues,status;
/*
 *   Get scale factor 
 */
     strcpy(request,"GET SCALE FACTOR");
     nvalues=1;
     ctofstr(request, request_dsc, strlen(request));
     dstore_(request_dsc,&scale_factor,&nvalues,&status,strlen(request));
     for (i = 0; i < *np; i++)
     {
/*
 *      Call Fortran routine to convert plot inches to world coordinates
 */
        unscl_(&xpoly[i] , &ypoly[i], &xwpoly [ i], &ywpoly[i]);
     }
     wwrat = *wratio;
     ag_poly_labelxy(xwpoly, ywpoly, *np, *isense, wwrat, &xwlab, &ywlab, &wwmax);
     geoscl_(&xwlab,&ywlab,xlab,ylab);
     *wmax = wwmax * scale_factor;

} /* end of function */
#endif

#ifdef primos
void /*FUNCTION*/ plabxy(xpoly, ypoly, np, isense, wratio, xlab, ylab, 
	 wmax)
float xpoly[], ypoly[];
int *np, *isense;
float *wratio, *xlab, *ylab, *wmax;
{
     double xwpoly[500],ywpoly[500];
     double xwlab,ywlab,wwmax,wwrat,scale_factor;
     char request[40];
     CHAR request_dsc[MAXLEN];
     int i,nvalues,status;
/*
 *   Get scale factor 
 */
     strcpy(request,"GET SCALE FACTOR");
     nvalues=1;
     ctofstr((CHAR *)&request, request_dsc, strlen(request));
     dstore(request_dsc,scale_factor,nvalues,status,strlen(request),strlen(request));
     ftocstr((CHAR *)&request, request_dsc, strlen(request));
     for (i = 0; i < *np; i++)
     {
/*
 *      Call Fortran routine to convert plot inches to world coordinates
 */
        unscl(xpoly[i] , ypoly[i], xwpoly [ i], ywpoly[i]);
     }
     wwrat = *wratio;
     ag_poly_labelxy(xwpoly, ywpoly, *np, *isense, wwrat, &xwlab, &ywlab, &wwmax);
     geoscl(xwlab,ywlab,*xlab,*ylab);
     *wmax = wwmax * scale_factor;

} /* end of function */
#endif

