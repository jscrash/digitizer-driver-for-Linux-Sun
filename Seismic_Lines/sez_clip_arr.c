/* DEC/CMS REPLACEMENT HISTORY, Element SEZ_CLIP_ARR.C */
/* *3    17-AUG-1990 22:15:07 VINCE "(SPR 5644) Code Cleanup" */
/* *2    31-OCT-1989 15:12:32 GILLESPIE "(SPR 1) Initial release" */
/* *1    31-OCT-1989 15:11:07 GILLESPIE "Moved from INTERSECTION - was sef_clip_array" */
/* DEC/CMS REPLACEMENT HISTORY, Element SEZ_CLIP_ARR.C */
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

#include "esi_se.h"

#define LEFT    2
#define RIGHT    4
#define TOP    8
#define BOT    16
#define CLIPPED  0

#if USE_PROTOTYPES
static VOID sez_clip_code (DOUBLE x, DOUBLE y, DOUBLE xmin, DOUBLE xmax,
                               DOUBLE ymin, DOUBLE ymax, INT *code);
#else
static VOID sez_clip_code ();
#endif

/* File Description ---------------------------------------------------------
Overview:

Prototypes and one line descriptions of each function in the file.
Public_Functions:
    publicdef INT sez_clip_array (FLOAT *array_x, FLOAT *array_y, UINT nb_points, DOUBLE *rect_inter, 
        INT *deb,UINT *nbp);

Private_Functions:
    static VOID sez_clip_code (DOUBLE x, DOUBLE y, DOUBLE xmin, DOUBLE xmax,
        DOUBLE ymin, DOUBLE ymax, INT *code);

-----------------------------------------------------------------------------*/

/* Function Description -----------------------------------------------------
Description:

Prototype:
    publicdef INT sez_clip_array (FLOAT *array_x, FLOAT *array_y, UINT nb_points, DOUBLE *rect_inter, 
        INT *deb,UINT *nbp);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    array_x         -(FLOAT *) -\
    array_y         -(FLOAT *)  Array_x and array_y are pointers to the float arrays containing 
                        the x , y  coordinates describing the line. 
    nb_points       -(UINT) Nb_points is the number of coordinates in the array. 
    rect_inter      -(DOUBLE *) Rect_inter is the float array containing coordinates of the rectangles
                        used for clipping:  
                            rect_inter [ 0 ] = xmin 
                            rect_inter [ 1 ] = xmax
                            rect_inter [ 2 ] = ymin 
                            rect_inter [ 3 ]  = ymax.
    deb             -(INT *) Deb is the index of the first triplets in the array which is inside 
                        the clipping rectangle.
    nbp             -(UINT *) Nbp is the number of points in the array which are inside 
                        the clipping rectangle.

Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT sez_clip_array (FLOAT *array_x, FLOAT *array_y, UINT nb_points,
                              DOUBLE *rect_inter, INT *deb, UINT *nbp)
#else
publicdef INT sez_clip_array (array_x, array_y, nb_points, rect_inter, deb, nbp)
FLOAT *array_x, *array_y;
UINT nb_points;
DOUBLE *rect_inter;
INT *deb;
UINT *nbp;
#endif
    {
    
    DOUBLE xmin, xmax, ymin, ymax;
    INT i, fin, status;
    FLOAT *point_x, *point_y;
    DOUBLE xcur, ycur;
    INT code1, code2;
    INT lst_index;
    DOUBLE *pr;
    
    pr = rect_inter;
    
    xmin = *pr++;
    xmax = *pr++;
    ymin = *pr++;
    ymax = *pr++;
    
    /*  first we begin from the beginning of the
        array and look for the first segment
        clipped by the rectangle           */
    
    /*  compute position code for first point of   */
    /*  the array                  */
    
    xcur = *array_x;
    ycur = *array_y;
    
    sez_clip_code (xcur, ycur, xmin, xmax, ymin, ymax, &code1);
    
    for (i = 2, point_x = array_x + 1, point_y = array_y + 1; i <= nb_points;
         i++, point_x++, point_y++)
        {
        xcur = *point_x;
        ycur = *point_y;
        
        *deb = i - 1;
        
        sez_clip_code (xcur, ycur, xmin, xmax, ymin, ymax, &code2);
        
        /*   the segment is clipping the rectangle if
             code1 & code2 = 0 if this is the case we
             stop the scanning             */
        
        if ((code1 & code2) EQUALS CLIPPED)
            {
            break;
            }
        else
            code1 = code2;
        
        }
    /*  now we scan from the end of the array and
        look for the first segment clipped by the
        rectangle                  */
    
    /*  compute position code for last point of    */
    /*  the array                  */
    xcur = *(array_x + nb_points - 1);
    ycur = *(array_y + nb_points - 1);
    
    sez_clip_code (xcur, ycur, xmin, xmax, ymin, ymax, &code1);
    
    for (i = nb_points - 1, point_x = array_x + nb_points - 2,
             point_y = array_y + nb_points - 2; i >= *deb; i--, point_x--, point_y--)
        {
        xcur = *point_x;
        ycur = *point_y;
        fin = i + 1;
        
        sez_clip_code (xcur, ycur, xmin, xmax, ymin, ymax, &code2);
        
        /*   the segment is clipping the rectangle if
             code1 & code2 = 0 if this is the case we
             stop the scanning             */
        
        if ((code1 & code2) EQUALS CLIPPED)
            {
            break;
            }
        else
            code1 = code2;
        
        }
    *nbp = fin - (*deb) + 1;
    return SUCCESS;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    This routine is returning a position code  of  the point  ( x,y)
    relatively to the rectangle described by (xmin , xmax,ymin , ymax ).
    This code  has a binary representation shown in this figure


             1001   |    1000        |  1010
                    |                |
            _________________________________

                    |  rectangle     |  
                    |                |
             0001   |    0000        |  0010 
                    |                |
                    |                |  
                    |                |
            _________________________________
    
             0101   |    0100        |  0110 
                    |                |

Prototype:
    static VOID sez_clip_code (DOUBLE x, DOUBLE y, DOUBLE xmin, DOUBLE xmax, DOUBLE ymin,
        DOUBLE ymax, INT *code);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    code            -(INT *)
    x               -(DOUBLE)
    y               -(DOUBLE)
    xmin            -(DOUBLE)
    xmax            -(DOUBLE)
    ymin            -(DOUBLE)
    ymax            -(DOUBLE)

Return Value/Status:
    None.
    
Scope:
    PRIVATE to <sez_clip_arr.c>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static VOID sez_clip_code (DOUBLE x, DOUBLE y, DOUBLE xmin, DOUBLE xmax,
                               DOUBLE ymin, DOUBLE ymax, INT *code)
#else
static VOID sez_clip_code (x, y, xmin, xmax, ymin, ymax, code)
INT *code;
DOUBLE x, y, xmin, xmax, ymin, ymax;
#endif
    {
    *code = CLIPPED;
    
    if (x < xmin)
        *code |= LEFT;
    if (x > xmax)
        *code |= RIGHT;
    if (y < ymin)
        *code |= BOT;
    if (y > ymax)
        *code |= TOP;
    
    }
/* END:     */
