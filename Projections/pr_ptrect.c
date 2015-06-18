/* DEC/CMS REPLACEMENT HISTORY, Element PR_PTRECT.C*/
/* *2    17-AUG-1990 22:11:57 VINCE "(SPR 5644) Code Cleanup"*/
/* *1    19-JUN-1989 13:22:31 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element PR_PTRECT.C*/
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

#include "esi_c_lib.h"
#include "esi_pr.h"

#ifdef DEBUG_PR

#include "esi_wi.h"
static CHAR *debug_str[STMAX];
#endif

/* File Description ---------------------------------------------------------
Overview:

Prototypes and one line descriptions of each function in the file.
Public_Functions:
    publicdef POINT *PR_SetPt(POINT *point, DOUBLE x, DOUBLE y );
    
    publicdef POINT *PR_InqPt(POINT *point, FLOAT *x, FLOAT *y );
    
    publicdef INT PR_EqualPt(POINT *point1, POINT *point2 );
    
    publicdef INT PR_PtInRect(POINT *point, RECTANGLE *rect );
    
    publicdef POINT *PR_PipePt(POINT *pointIn, PIPE *pipe, POINT *pointOut );
    
    publicdef RECTANGLE *PR_SetRect(RECTANGLE *rect, DOUBLE x1, DOUBLE y1, DOUBLE x2, 
        DOUBLE y2 );
    
    publicdef RECTANGLE *PR_InqRect(RECTANGLE *rect, FLOAT *x1, FLOAT *1, FLOAT *x2, 
        FLOAT *y2 );
    
    publicdef INT PR_EqualRect(RECTANGLE *rect1, RECTANGLE *rect2 );
    
    publicdef INT PR_RectInRect(RECTANGLE *rect1, RECTANGLE *rect2 );
    
    publicdef RECTANGLE *PR_PipeRect(RECTANGLE *rectIn, PIPE *pipe, RECTANGLE *rectOut );
    
    publicdef RECTANGLE *PR_AspectRect(RECTANGLE * rect1, RECTANGLE *rect2 );
    
    publicdef PIPE *PR_SetPipe(PIPE *pipe, RECTANGLE *rectIn, RECTANGLE *rectOut );
    
    publicdef PIPE *PR_InqPipe(PIPE *pipe, RECTANGLE *rectIn, RECTANGLE *rectOut );
    
    publicdef PIPE *PR_InvertPipe(PIPE *pipeIn, PIPE *pipeOut );

Private_Functions:
    static void PR_ShowPt(POINT *point );
    
    static void PR_ShowRect(RECTANGLE *rect );
    
    static void PR_ShowPipe(PIPE *pipe );

-----------------------------------------------------------------------------*/

/* Point routines */

/* Function Description -----------------------------------------------------
Description:

Prototype:
    publicdef POINT *PR_SetPt(POINT *point, DOUBLE x, DOUBLE y );

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    point           -(POINT *)
    x               -(DOUBLE)
    y               -(DOUBLE)

Return Value/Status:
    Function returns the value of point.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef POINT *PR_SetPt (POINT *point, DOUBLE x, DOUBLE y)
#else
publicdef POINT *PR_SetPt (point, x, y)
POINT *point;
DOUBLE x, y;
#endif
    {
    
    point->x = x;
    point->y = y;
    
    return(point);
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:

Prototype:
    publicdef POINT *PR_InqPt(POINT *point, FLOAT *x, FLOAT *y );

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    point           -(POINT *)
    x               -(FLOAT *)
    y               -(FLOAT *)

Return Value/Status:
    Function returns the value of point.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef POINT *PR_InqPt (POINT *point, FLOAT *x, FLOAT *y)
#else
publicdef POINT *PR_InqPt (point, x, y)
POINT *point;
FLOAT *x, *y;
#endif
    {
    
    *x = point->x;
    *y = point->y;
    
    return(point);
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:

Prototype:
    publicdef INT PR_EqualPt(POINT *point1, POINT *point2 );

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    point1          -(POINT *)
    point2          -(POINT *)

Return Value/Status:
    Function returns the value of ifEqual.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT PR_EqualPt (POINT *point1, POINT *point2)
#else
publicdef INT PR_EqualPt (point1, point2)
POINT *point1, *point2;
#endif
    {
    INT ifEqual;
    DOUBLE dx, dy;
    DOUBLE tol = 0.00001;
    
    dx = ABS (point1->x - point2->x);
    dy = ABS (point1->y - point2->y);
    ifEqual = ((dx <= tol) && (dy <= tol));
    
    return(ifEqual);
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:

Prototype:
    publicdef INT PR_PtInRect(POINT *point, RECTANGLE *rect );

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    point           -(POINT *)
    rect            -(RECTANGLE *)

Return Value/Status:
    Function returns the value of ifInRect.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT PR_PtInRect (POINT *point, RECTANGLE *rect)
#else
publicdef INT PR_PtInRect (point, rect)
POINT *point;
RECTANGLE *rect;
#endif
    {
    INT ifInRect;
    
    ifInRect = ((point->x > rect->min.x) && (point->x < rect->max.x) &&
                (point->y > rect->min.y) && (point->y < rect->max.y));
    
    return(ifInRect);
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:

Prototype:
    publicdef POINT *PR_PipePt(POINT *pointIn, PIPE *pipe, POINT *pointOut );

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    pointIn         -(POINT *)
    pointOut        -(POINT *)
    pipe            -(PIPE *)

Return Value/Status:
    Function returns the value of pointOut.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef POINT *PR_PipePt (POINT *pointIn, PIPE *pipe, POINT *pointOut)
#else
publicdef POINT *PR_PipePt (pointIn, pipe, pointOut)
POINT *pointIn, *pointOut;
PIPE *pipe;
#endif
    {
    DOUBLE dxIn, dyIn, dxOut, dyOut;
    
    dxIn = pipe->in.max.x - pipe->in.min.x;
    dyIn = pipe->in.max.y - pipe->in.min.y;
    dxOut = pipe->out.max.x - pipe->out.min.x;
    dyOut = pipe->out.max.y - pipe->out.min.y;
    pointOut->x = pipe->out.min.x + (((pointIn->x - pipe->in.min.x) / dxIn) * dxOut);
    pointOut->y = pipe->out.min.y + (((pointIn->y - pipe->in.min.y) / dyIn) * dyOut);
    
    return(pointOut);
    }
/* END:     */

/* Rectangle routines */

/* Function Description -----------------------------------------------------
Description:

Prototype:
    publicdef RECTANGLE *PR_SetRect(RECTANGLE *rect, DOUBLE x1, DOUBLE y1, DOUBLE x2, 
        DOUBLE y2 );

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    rect            -(RECTANGLE *)
    x1              -(DOUBLE)
    y1              -(DOUBLE)
    x2              -(DOUBLE)
    y2              -(DOUBLE)

Return Value/Status:
    Function returns the value of rect.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef RECTANGLE *PR_SetRect (RECTANGLE *rect, DOUBLE x1, DOUBLE y1, DOUBLE x2,
                                 DOUBLE y2)
#else
publicdef RECTANGLE *PR_SetRect (rect, x1, y1, x2, y2)
RECTANGLE *rect;
DOUBLE x1, y1, x2, y2;
#endif
    {
    rect->min.x = MIN (x1, x2);
    rect->min.y = MIN (y1, y2);
    rect->max.x = MAX (x1, x2);
    rect->max.y = MAX (y1, y2);
    
    return(rect);
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:

Prototype:
    publicdef RECTANGLE *PR_InqRect(RECTANGLE *rect, FLOAT *x1, FLOAT *1, FLOAT *x2, 
        FLOAT *y2 );

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    rect            -(RECTANGLE *)
    x1              -(FLOAT *)
    y1              -(FLOAT *)
    x2              -(FLOAT *)
    y2              -(FLOAT *)

Return Value/Status:
    Function returns the value of rect.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef RECTANGLE *PR_InqRect (RECTANGLE *rect, FLOAT *x1, FLOAT *y1, FLOAT *x2,
                                 FLOAT *y2)
#else
publicdef RECTANGLE *PR_InqRect (rect, x1, y1, x2, y2)
RECTANGLE *rect;
FLOAT *x1, *y1, *x2, *y2;
#endif
    {
    *x1 = rect->min.x;
    *y1 = rect->min.y;
    *x2 = rect->max.x;
    *y2 = rect->max.y;
    
    return(rect);
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:

Prototype:
    publicdef INT PR_EqualRect(RECTANGLE *rect1, RECTANGLE *rect2 );

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    rect1           -(RECTANGLE *)
    rect2           -(RECTANGLE *)

Return Value/Status:
    Function returns the value of ifEqual.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT PR_EqualRect (RECTANGLE *rect1, RECTANGLE *rect2)
#else
publicdef INT PR_EqualRect (rect1, rect2)
RECTANGLE *rect1, *rect2;
#endif
    {
    INT ifEqual;
    
    ifEqual = (PR_EqualPt (&(rect1->min), &(rect2->min)) &&
               PR_EqualPt (&(rect1->max), &(rect2->max)));
    
    return(ifEqual);
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:

Prototype:
    publicdef INT PR_RectInRect(RECTANGLE *rect1, RECTANGLE *rect2 );

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    rect1           -(RECTANGLE *)
    rect2           -(RECTANGLE *)

Return Value/Status:
    Function returns the value of ifln.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT PR_RectInRect (RECTANGLE *rect1, RECTANGLE *rect2)
#else
publicdef INT PR_RectInRect (rect1, rect2)
RECTANGLE *rect1, *rect2;
#endif
    {
    INT ifIn;
    
    ifIn = (PR_PtInRect (&(rect1->min), rect2) && PR_PtInRect (&(rect1->max), rect2));
    
    return(ifIn);
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:

Prototype:
    publicdef RECTANGLE *PR_PipeRect(RECTANGLE *rectIn, PIPE *pipe, RECTANGLE *rectOut );

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    rectIn          -(RECTANGLE *)
    rectOut         -(RECTANGLE *)
    pipe            -(PIPE *)

Return Value/Status:
    Function returns the value of rectOut.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef RECTANGLE *PR_PipeRect (RECTANGLE *rectIn, PIPE *pipe, RECTANGLE *rectOut)
#else
publicdef RECTANGLE *PR_PipeRect (rectIn, pipe, rectOut)
RECTANGLE *rectIn, *rectOut;
PIPE *pipe;
#endif
    {
    PR_PipePt (&(rectIn->min), pipe, &(rectOut->min));
    PR_PipePt (&(rectIn->max), pipe, &(rectOut->max));
    
    return(rectOut);
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:

Prototype:
    publicdef RECTANGLE *PR_AspectRect(RECTANGLE * rect1, RECTANGLE *rect2 );

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    rect1           -(RECTANGLE *)
    rect2           -(RECTANGLE *)

Return Value/Status:
    Function returns the value of rect2.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef RECTANGLE *PR_AspectRect (RECTANGLE *rect1, RECTANGLE *rect2)
#else
publicdef RECTANGLE *PR_AspectRect (rect1, rect2)
RECTANGLE *rect1, *rect2;
#endif
    {
    DOUBLE dx1, dy1, m1;
    DOUBLE dx2, dy2, m2;
    
    dx1 = rect1->max.x - rect1->min.x;
    dy1 = rect1->max.y - rect1->min.y;
    m1 = dy1 / dx1;
    dx2 = rect2->max.x - rect2->min.x;
    dy2 = rect2->max.y - rect2->min.y;
    m2 = dy2 / dx2;
    
    if (m1 != m2)
        {
        if (m1 > m2)
            {
            rect2->max.x = rect2->min.x + (dy2 / m1);
            }
        else
            {
            rect2->max.y = rect2->min.y + (dx2 * m1);
            }
        }
    return(rect2);
    }
/* END:     */

/* Pipe routines */

/* Function Description -----------------------------------------------------
Description:

Prototype:
    publicdef PIPE *PR_SetPipe(PIPE *pipe, RECTANGLE *rectIn, RECTANGLE *rectOut );

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    pipe            -(PIPE *)
    rectIn          -(RECTANGLE *)
    rectOut         -(RECTANGLE *)

Return Value/Status:
    Function returns the value of pipe.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef PIPE *PR_SetPipe (PIPE *pipe, RECTANGLE *rectIn, RECTANGLE *rectOut)
#else
publicdef PIPE *PR_SetPipe (pipe, rectIn, rectOut)
PIPE *pipe;
RECTANGLE *rectIn, *rectOut;
#endif
    {
    PR_SetRect (&(pipe->in), rectIn->min.x, rectIn->min.y, rectIn->max.x,
                rectIn->max.y);
    
    PR_SetRect (&(pipe->out), rectOut->min.x, rectOut->min.y, rectOut->max.x,
                rectOut->max.y);
    
    return(pipe);
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:

Prototype:
    publicdef PIPE *PR_InqPipe(PIPE *pipe, RECTANGLE *rectIn, RECTANGLE *rectOut );

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    pipe            -(PIPE *)
    rectIn          -(RECTANGLE *)
    rectOut         -(RECTANGLE *)

Return Value/Status:
    Function returns the value of pipe.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef PIPE *PR_InqPipe (PIPE *pipe, RECTANGLE *rectIn, RECTANGLE *rectOut)
#else
publicdef PIPE *PR_InqPipe (pipe, rectIn, rectOut)
PIPE *pipe;
RECTANGLE *rectIn, *rectOut;
#endif
    {
    PR_InqRect (&(pipe->in), &(rectIn->min.x), &(rectIn->min.y), &(rectIn->max.x),
                &(rectIn->max.y));
    
    PR_InqRect (&(pipe->out), &(rectOut->min.x), &(rectOut->min.y), &(rectOut->max.x),
                &(rectOut->max.y));
    
    return(pipe);
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:

Prototype:
    publicdef INT PR_EqualPipe(PIPE *pipe1, PIPE *pipe2 );

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    pipe1           -(PIPE *)
    pipe2           -(PIPE *)

Return Value/Status:
    Function returns the value of ifEqual.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT PR_EqualPipe (PIPE *pipe1, PIPE *pipe2)
#else
publicdef INT PR_EqualPipe (pipe1, pipe2)
PIPE *pipe1, *pipe2;
#endif
    {
    INT ifEqual;
    
    ifEqual = (PR_EqualRect (&(pipe1->in), &(pipe2->in)) &&
               PR_EqualRect (&(pipe1->out), &(pipe2->out)));
    
    return(ifEqual);
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:

Prototype:
    publicdef PIPE *PR_InvertPipe(PIPE *pipeIn, PIPE *pipeOut );

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    pipeIn          -(PIPE *)
    pipeOut         -(PIPE *)   

Return Value/Status:
    Function returns the value of pipeOut.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef PIPE *PR_InvertPipe (PIPE *pipeIn, PIPE *pipeOut)
#else
publicdef PIPE *PR_InvertPipe (pipeIn, pipeOut)
PIPE *pipeIn, *pipeOut;
#endif
    {
    RECTANGLE inRect, outRect;
    
    inRect = pipeIn->in;
    outRect = pipeIn->out;
    pipeOut->in = outRect;
    pipeOut->out = inRect;
    
    return(pipeOut);
    
    }
/* END:     */

#ifdef DEBUG_PR

/* Debug routines */

/* Function Description -----------------------------------------------------
Description:

Prototype:
    static void PR_ShowPt(POINT *point );

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    point           -(POINT *)

Return Value/Status:
    None.
    
Scope:
    PRIVATE to <pr_ptrect.c>
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static void PR_ShowPt (POINT *point)
#else
static void PR_ShowPt (point)
POINT *point;
#endif
    {
    sprintf (debug_str, "Pt  : %p = (%f, %f)", point, point->x, point->y);
    wimsg (debug_str);
    
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:

Prototype:
    static void PR_ShowRect(RECTANGLE *rect );

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    rect            -(RECTANGLE *)

Return Value/Status:
    None.
    
Scope:
    PRIVATE to <pr_ptrect.c>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static void PR_ShowRect (RECTANGLE *rect)
#else
static void PR_ShowRect (rect)
RECTANGLE *rect;
#endif
    {
    sprintf (debug_str, "Rect: %p = (%f, %f), (%f, %f)", rect, rect->min.x,
             rect->min.y, rect->max.x, rect->max.y);
    wimsg (debug_str);
    
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:

Prototype:
    static void PR_ShowPipe(PIPE *pipe );

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    pipe            -(PIPE *)

Return Value/Status:
    None.
    
Scope:
    PRIVATE to <pr_ptrect.c>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static void PR_ShowPipe (PIPE *pipe)
#else
static void PR_ShowPipe (pipe)
PIPE *pipe;
#endif
    {
    sprintf (debug_str, "Pipe: %p = in((%f, %f), (%f, %f)), out((%f, %f), (%f, %f))",
             pipe, pipe->in.min.x, pipe->in.min.y, pipe->in.max.x, pipe->in.max.y,
             pipe->out.min.x, pipe->out.min.y, pipe->out.max.x, pipe->out.max.y);
    wimsg (debug_str);
    
    }
/* END:     */

#endif
