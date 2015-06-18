/* DEC/CMS REPLACEMENT HISTORY, Element TG_GKS_TOOLS.C*/
/* *13   16-AUG-1991 10:47:24 JTM "(SPR 0) Changed arguments to wm_gslwsc() to DOUBLE."*/
/* *12    1-JUL-1991 15:04:42 KEE "(SPR 8013) Fix Polyline Color Palette Window being orphaned after CANCEL, case 2364"*/
/* *11    3-MAY-1991 16:31:26 KEE "(SPR -1) Bug fix when porting MOTIF/PRIORGKS, MOTIF/DECGKS, DECW/PRIORGKS, DECW/DECGKS version"*/
/* *10    3-MAY-1991 16:12:43 KEE "(SPR -1) MOTIF/PRIORGKS, MOTIF/DECWGKS, DECW/PRIORGKS, DECW/DECGKS version"*/
/* *9    12-OCT-1990 17:58:44 GILLESPIE "(SPR 100) Merge Ernie Deltas"*/
/* *8    20-MAY-1990 16:41:23 SCD "(SPR 1) Set palette window properties, deferall & size"*/
/* *7    19-MAY-1990 12:33:44 SCD "(SPR 1) Verify make will put into wix.olb"*/
/* *6    14-MAY-1990 14:32:27 SCD "(SPR 1) Change polyline attr pallete to small window"*/
/* *5    21-DEC-1989 15:44:24 GORDON "(SPR 0) Fix colors, text size, cleanup"*/
/* *4     1-NOV-1989 12:48:19 GILLESPIE "(SPR 30) Remove unused functions (or those that performed no net action)"*/
/* *3    14-SEP-1989 16:20:52 GORDON "(SPR 100) Gulf/Sun/Unix mods"*/
/* *2    20-JUL-1989 08:35:49 GORDON "(SPR 100) Sun mods + wm_set_mouse_menu call"*/
/* *1    19-JUN-1989 13:32:36 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element TG_GKS_TOOLS.C*/
/* DEC/CMS REPLACEMENT HISTORY, Element TG_GKS_TOOLS.C*/
/* *8    20-MAY-1990 16:41:23 SCD "(SPR 1) Set palette window properties, deferall & size"*/
/* *7    19-MAY-1990 12:33:44 SCD "(SPR 1) Verify make will put into wix.olb"*/
/* *6    14-MAY-1990 14:32:27 SCD "(SPR 1) Change polyline attr pallete to small window"*/
/* *5    21-DEC-1989 15:44:24 GORDON "(SPR 0) Fix colors, text size, cleanup"*/
/* *4     1-NOV-1989 12:48:19 GILLESPIE "(SPR 30) Remove unused functions (or those that performed no net action)"*/
/* *3    14-SEP-1989 16:20:52 GORDON "(SPR 100) Gulf/Sun/Unix mods"*/
/* *2    20-JUL-1989 08:35:49 GORDON "(SPR 100) Sun mods + wm_set_mouse_menu call"*/
/* *1    19-JUN-1989 13:32:36 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element TG_GKS_TOOLS.C*/

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



/*
 * TG_GKS_TOOLS.C
 *
 * Toolbox of GKS Compatible Graphics routines.
 */

#include "esi_math.h"
#include "esi_c_lib.h"
#include "esi_gks.h"
#include "esi_tg.h"
#include "esi_wm.h"
#include "esi_wm_cls.h"
#include "esi_ug.h"
#include "esi_lu.h"

/* Global Variables */

typedef struct tg_struct
    {
    GKS_REAL xmin;
    GKS_REAL xmax;
    GKS_REAL ymin;
    GKS_REAL ymax;
    } TG_PA_PICK_RANGE;

static APPLICATION_ID stock_app_id = -1;
static WINDOW_ID stock_window_id = -1;
static SEGMENT_ID typeMarkSeg = -1;
static SEGMENT_ID widthMarkSeg = -1;
static SEGMENT_ID colorMarkSeg = -1;
static GKS_INT black = -1, white = -1, highlight = -1;

static INT base_pick_num = 0;
static TG_PA_PICK_RANGE range_matrix[40];

#define TITLE "Select Polyline Attributes"
#define COLOR "Color : "
#define WIDTH "Width : "
#define TYPE  "Type : "
#define OK_BTN " OK     "
#define CANCEL_BTN " CANCEL "
#define MAIN_TITLE_HEIGHT   (GKS_REAL)0.03
#define SMALL_TEXT_HEIGHT   (GKS_REAL)0.023

#if USE_PROTOTYPES

static INT tg_convert (FLOAT c1[], FLOAT c2[], DOUBLE xin, DOUBLE yin,
                           FLOAT *xout, FLOAT *yout);
static INT tg_ndc2wc (TNR_ID tnr, DOUBLE xndc, DOUBLE yndc, FLOAT *xwc,
                          FLOAT *ywc);
static INT tg_pa_colors (INT change_color, INT *begin_pick);
static INT tg_pa_draw_background (INT field);
static INT tg_pa_line_type (INT *begin_pick);
static INT tg_pa_mark_color (INT marked_color);
static INT tg_pa_mark_type (INT marked_type);
static INT tg_pa_mark_width (INT marked_width);
static INT tg_pa_pick_buttons (INT *ok, INT *cancel);
static INT tg_pa_request_pick (INT *status, INT *pick, INT *button);
static INT tg_pa_texts (void);
static INT tg_wc2ndc (TNR_ID tnr, DOUBLE xwc, DOUBLE ywc, FLOAT *xndc,
                          FLOAT *yndc);

#else

static INT tg_convert ();
static INT tg_ndc2wc ();
static INT tg_pa_colors ();
static INT tg_pa_draw_background ();
static INT tg_pa_line_type ();
static INT tg_pa_mark_color ();
static INT tg_pa_mark_type ();
static INT tg_pa_mark_width ();
static INT tg_pa_pick_buttons ();
static INT tg_pa_request_pick ();
static INT tg_pa_texts ();
static INT tg_wc2ndc ();

#endif

/* ====================tg_calculat_3points_transform ========================== */



publicdef INT tg_calc_3p_xform (xpoint, ypoint, x1, y1, x2, y2, xform, angle)

/* %S Calculate the  tranform matrix for registration.
*/
/* %P
    Input Parameters:
     XPOINT    =(FLOAT *) an array of x_coord. value of three points.
     YPOINT    =(FLOAT *) an array of y_coord. value of three points.
     X1        =(FLOAT) the x_coord. value of the origin point in the
                viewport.
     Y1        =(FLOAT) the y_coord. value of the origin point in the
                viepwort.
     X2        =(FLOAT) the x_coord. value of the upper right point in
                the viewport.
     Y2        =(FLOAT) the y_coord. value of the upper right point in
                the viewport.

    Output Parameters:
     XFORM     =(FLOAT *) the returned prameters in the transform matrix.

*/
DOUBLE x1, x2, y1, y2;
FLOAT xpoint[], ypoint[], xform[], *angle;

    {
    DOUBLE theta, wcos, wsin, wdx, wdy, dx01, dx21, dy01, dy21, xratio,
        yratio, angle1;

    INT status = SUCCESS;


    /* calculate the angle of three points.     */
    /*  using determinant for sine, and dot product for cosine  */

    dx01 = xpoint[1] - xpoint[0];
    dy01 = ypoint[1] - ypoint[0];
    dx21 = xpoint[2] - xpoint[1];
    dy21 = ypoint[2] - ypoint[1];

    angle1 = atan2 ((dx01 * dy21 - dy01 * dx21), (dx01 * dx21 + dy01 * dy21));

        

    *angle = R_TO_D (angle1);


    /* to calculate the theta.   */

    theta = atan2 (dy01, dx01);
    wcos = cos (theta);
    wsin = sin (theta);

    /* calculat the scaling.     */

    wdx = x2 - x1;
    wdy = y2 - y1;

    xratio = wdx / sqrt (dx01 * dx01 + dy01 * dy01);
    yratio = wdy / sqrt (dx21 * dx21 + dy21 * dy21);

    /* return the transform format */

    xform[0] = xratio * wcos;
    xform[1] = -yratio * wsin;
    xform[2] = xratio * wsin;
    xform[3] = yratio * wcos;
    xform[4] = x1 - (xform[0] * xpoint[0] + xform[2] * ypoint[0]);
    xform[5] = y1 - (xform[1] * xpoint[0] + xform[3] * ypoint[0]);

    return status;
    }

/* ====================== HELP ROUTINE tg_convert ============================ */
static INT tg_convert (c1, c2, xin, yin, xout, yout)

/* %S To convert a point from one coordinate system to another one.    */

/* %P
    Input Parameters:
     C1     =(FLOAT ARRAY) the original coordinate system.
     C2     =(FLOAT ARRAY) the destination coordinate system.
     XIN    =(FLOAT ) the input x_coord. value.
     YIN    =(FLOAT ) the input y_coord. value.

    Output Parameters:
     XOUT   =(FLOAT *) the returned x_coord. value.
     YOUT   =(FLOAT *) the returned y_coord. value.

*/
DOUBLE xin, yin;
FLOAT c1[], c2[];
FLOAT *xout, *yout;

    {
    FLOAT scalex, scaley, a, b;
    INT status = SUCCESS;

    scalex = (c2[1] - c2[0]) / (c1[1] - c1[0]);
    scaley = (c2[3] - c2[2]) / (c1[3] - c1[2]);
    a = c2[0] - scalex * c1[0];
    b = c2[2] - scaley * c1[2];
    *xout = xin * scalex + a;
    *yout = yin * scaley + b;

    return status;
    }

/* ====================== TG HELP ROUTINES tg_help_convert =================== */
publicdef INT tg_help_convert (tnr, status, xpos, ypos, xreturn, yreturn)

/* %S An internal routine to convert the point to be NDC system.      */

/* %P
    Input Parameters:
     TNR     =(INT) normalization transformation number.
     STATUS  =(INT) 0---> cancelled, 1 ---> complete.
     XPOS    =(FLOAT) the x_coord. value of point.
     YPOS    =(FLOAT) the y_coord. value of point.

    Output Parameters:
     XRETURN =(FLOAT *) the returned x_coord. value.
     YRETURN =(FLOAT *) the returned y_coord. value.

*/
INT tnr, status;
DOUBLE xpos, ypos;
FLOAT *xreturn, *yreturn;

    {
    if (status)
        {
        if (tnr != 0)
            {
            tg_wc2ndc (tnr, xpos, ypos, xreturn, yreturn);
            }
        else
            {
            *xreturn = xpos;
            *yreturn = ypos;
            }
        }

    return status;
    }

/* ====================== tg_ndc_to_wc ======================================= */
static INT tg_ndc2wc (tnr, xndc, yndc, xwc, ywc)

/* %S  Covert a point in NDC to a world coordinates.       */

/* %P
    Input Parameters:
     TNR      =(INT) normalization transformation number.
     XNDC     =(FLOAT) the x_coord. value of world NDC.
     YNDC     =(FLOAT) the y_coord. value of world NDC.

    Output Parameters:
     XWC      =(FLOAT *) the returned x_coord. value of world coordinate system.
     YWC      =(FLOAT *) the returned y_coord. value of world coordinate system.

*/
TNR_ID tnr;
DOUBLE xndc, yndc;
FLOAT *xwc, *ywc;

    {
    GKS_REAL window[4], viewpt[4];
    INT status;

    /* inquire the window and viewpt of tnr  */
    wm_gqnt (tnr, &status, window, viewpt);
    if (status != 0)
        {
        return(status);
        }

    /* conver the value from window to viewport.    */
    tg_convert (viewpt, window, xndc, yndc, xwc, ywc);

    return status;
    }

/* ====================== tg_tnr_to_tnr ====================================== */
publicdef INT tg_tnr2tnr (tnrin, xin, yin, tnrout, xout, yout)

/* %S Convert a point from one TNR system to another TNR system.     */

/* %P
    Input Parameters:
     TNRIN   =(INT) the original TNR.
     TNROUT  =(INT) the destination TNR.
     XIN     =(FLOAT  ) the input x_coord. value.
     YIN     =(FLOAT  ) the output y_coord. value.

    Output Parameters:
     XOUT    =(FLOAT *) the output x_coord. value.
     YOUT    =(FLOAT *) the output y_coord. value.
*/

DOUBLE xin, yin;
FLOAT *xout, *yout;
INT tnrin, tnrout;
    {
    FLOAT x, y;
    INT status = SUCCESS;

    tg_wc2ndc (tnrin, xin, yin, &x, &y);
    tg_ndc2wc (tnrout, x, y, xout, yout);

    return status;
    }


/* ====================== tg_ndc_to_wc ======================================= */
static INT tg_wc2ndc (tnr, xwc, ywc, xndc, yndc)

/* %S Convert from world coordinates to NDC.       */

/* %P
    Input Parameters:
     TNR      =(INT) normalization transformation number.
     XWC     =(FLOAT) the x_coord. value of world coordinate system.
     YWC     =(FLOAT) the y_coord. value of world coordinate system.

    Output Parameters:
     XNDC      =(FLOAT *) the returned x_coord. value of NDC.
     YNDC      =(FLOAT *) the returned y_coord. value of NDC.

*/
TNR_ID tnr;
DOUBLE xwc, ywc;
FLOAT *xndc, *yndc;

    {
    GKS_REAL window[4], viewpt[4];
    INT status;

    /* inquire the window and viewpt of tnr  */
    wm_gqnt (tnr, &status, window, viewpt);
    if (status != 0)
        return(status);

    /* conver the value from window to viewport.    */
    tg_convert (window, viewpt, xwc, ywc, xndc, yndc);

    return status;
    }

/* ==================tg_polylinr_attribute_colors ================================= */
static INT tg_pa_colors (change_color, begin_pick)


/* %S A help routine of TG_ASK_POLYLINE_ATTRIBUTES. Paints colors on the screen. */



/* %P
    Input Parameters:
     CHANGE_COLOR  =(INT) flag to indicate whether to draw WHITE frame or
                  COLOR squares.

    Output Parameters:
     BEGIN_PICK    =(INT *) thefirst pick id of colors.

*/
INT change_color, *begin_pick;

    {
    FLOAT xinc, yinc, areax[4], areay[4];
    FLOAT wx, wy, to_frame = 0.0;
    INT i, j, nsides = 4;
    INT color;
    INT status = SUCCESS;

    xinc = 0.19;
    yinc = 0.11;
    to_frame = (change_color) ? 0.01 : 0.0;

    /* dispaly colors  */
    for (j = 0, wx = 0.01; j < 2; j++, wx += xinc)
        {
        for (i = 0, wy = 0.01; i < 8; i++, wy += yinc)
            {
            areax[0] = areax[3] = wx + to_frame;
            areax[1] = areax[2] = wx + xinc - to_frame;
            areay[0] = areay[1] = wy + to_frame;
            areay[2] = areay[3] = wy + yinc - to_frame;

            /* Create a pick */
            if (change_color)
                {
                color = j * 8 + i;
                wm_gsfaci (color);
                if (i == 0 && j == 0)
                    *begin_pick = base_pick_num;
                range_matrix[base_pick_num].xmin = MIN (areax[0], areax[1]);
                range_matrix[base_pick_num].xmax = MAX (areax[0], areax[1]);
                range_matrix[base_pick_num].ymin = MIN (areay[0], areay[2]);
                range_matrix[base_pick_num].ymax = MAX (areay[0], areay[2]);
                base_pick_num++;
                }
            wm_gfa (nsides, areax, areay);
            }
        }


    return status;
    }


/* ==================polylinr_attribute_color_picked ================================= */
static INT tg_pa_mark_color (marked_color)

/* %S A help routine of TG_ASK_POLYLINE_ATTRIBUTES. Marks the picked color. */



/* %P
    Input Parameters:
     MARKED_COLOR   =(INT) the color id of requiring to be marked.

    Output Parameters : none.

*/
INT marked_color;

    {
    FLOAT xinc, yinc, areax[4], areay[4];
    FLOAT wx, wy;
    INT i, j, nsides = 4, color;
    INT status = SUCCESS;

    /* delete the previous segment.  */
    if (colorMarkSeg > 0)
        wmsdelete (colorMarkSeg);

    /* create a new segment    */
    wmsallocate (&colorMarkSeg);
    xinc = 0.19;
    yinc = 0.11;

    /* finds the color id and marks it.  */
    for (j = 0, wx = 0.01; j < 2; j++, wx += xinc)
        {
        for (i = 0, wy = 0.01; i < 8; i++, wy += yinc)
            {
            wm_gsfaci (highlight);
            color = j * 8 + i;
            if (color == marked_color)
                {
                areax[0] = areax[3] = wx + 0.25 * xinc;
                areax[1] = areax[2] = wx + 0.75 * xinc;
                areay[0] = areay[1] = wy + 0.25 * yinc;
                areay[2] = areay[3] = wy + 0.75 * yinc;
                wm_gfa (nsides, areax, areay);

                }
            }
        }
    /* close segment   */
    status = wmsclose (colorMarkSeg);

    return status;
    }

/* ====================polyline_attribute_line_type ========================== */
static INT tg_pa_line_type (begin_pick)

/* %S A help routine of TG_ASK_POLYLINE_ATTRIBUTES. Draws types on the screen. */



/* %P
    Input Parameters: none.

    Output Parameters:
     BEGIN_PICK   =(INT *) the first pick id of type.

*/

INT *begin_pick;
    {
    FLOAT rad, sta, yinc, lx1, lx2, ly1, ly2;
    INT i, num_across;
    DOUBLE scale = 1.0;
    GKS_REAL xa[2], ya[2];
    INT status = SUCCESS;

    num_across = 4;
    rad = 1.0 / (FLOAT)(4 * num_across);
    lx1 = 0.5 + 1.5 * rad;
    sta = 0.35 + 1.5 * rad;
    lx2 = lx1 + .3;
    yinc = 0.7 * rad;

    wm_gslwsc (scale);
    wm_gsplci (white);
    /* dispaly line_type  */
    for (i = 1; i < 12; i++)
        {
        if (i == 1)
            *begin_pick = base_pick_num;
        ly1 = sta + (i - 1) * yinc;
        ly2 = ly1;
        wm_gsln (i);
        xa[0] = lx1, xa[1] = lx2, ya[0] = ly1, ya[1] = ly2;
        wm_gpl (2, xa, ya);
        range_matrix[base_pick_num].xmin = lx1;
        range_matrix[base_pick_num].xmax = lx2;
        range_matrix[base_pick_num].ymin = ly1 - yinc * 0.5;
        range_matrix[base_pick_num++].ymax = ly1 + yinc * .5;
        }

    return status;
    }

/* =======================polyline_attribute_draw_backgound =================== */
static INT tg_pa_draw_background (field)

/* %S A help routine of TG_ASK_POLYLINE_ATTRIBUTES. Paints the background fields. */



/* %P
    Input Parameters:
     FIELD  =(INT) field index.

    Output Parameters: none.
*/
INT field;
    {
    FLOAT areax[4], areay[4];
    INT npts = 4;
    INT fill;
    INT status = SUCCESS;

    /* paint the background  for line_type area.   */
    switch (field)
        {
    case 0:
        areax[0] = areax[3] = 0.47;
        areax[1] = areax[2] = 0.9;
        areay[0] = areay[1] = 0.42;
        areay[2] = areay[3] = 0.9;
        lu_text_to_index ("FILL_STYLE", &fill, "SOLID");
        wm_gsfais (fill);
        wm_gsfaci (black);
        wm_gfa (npts, areax, areay);
        break;

        /* paint the background    */
    case 1:
        areax[0] = 0.5;
        areax[1] = 0.90;
        areax[2] = 0.90;
        areax[3] = 0.5;
        areay[0] = areay[1] = 0.18;
        areay[2] = areay[3] = 0.38;
        lu_text_to_index ("FILL_STYLE", &fill, "SOLID");
        wm_gsfais (fill);
        wm_gsfaci (black);
        wm_gfa (npts, areax, areay);
        break;
        }

    return status;
    }


/* ====================polyline_attribute_line_width ========================== */
static INT tg_pa_line_width (begin_pick)
/* %S A help routine of TG_ASK_POLYLINE_ATTRIBUTES. To display width on the screen. */



/* %P
    Input Parameters: none.

    Output Parameters:
     BEGIN_PICK   =(INT) the first id of width.

*/
INT *begin_pick;
    {
    FLOAT rad, sta, yinc, lx1, lx2, ly1, ly2;
    DOUBLE scale;
    GKS_REAL xa[2], ya[2];
    INT i, num_across;
    INT lwidth;
    INT status = SUCCESS;

    num_across = 4;
    rad = 1.0 / (FLOAT)(4 * num_across);
    sta = 1.5 * rad;
    lx1 = sta + 0.5;
    lx2 = lx1 + .3;
    yinc = 0.7 * rad;

    /* display line_width */
    lwidth = 1, wm_gsln (lwidth);
    wm_gsplci (white);
    for (i = 1, ly1 = sta - yinc + 0.1, scale = 0; i < 17; i = i * 2)
        {
        if (i == 1)
            *begin_pick = base_pick_num;
        ly1 += yinc;
        ly2 = ly1;
        scale = 1.0 * i;
        wm_gslwsc (scale);
        xa[0] = lx1, xa[1] = lx2, ya[0] = ly1, ya[1] = ly2;
        wm_gpl (2, xa, ya);
        range_matrix[base_pick_num].xmin = lx1;
        range_matrix[base_pick_num].xmax = lx2;
        range_matrix[base_pick_num].ymin = ly1 - 0.49 * yinc;
        range_matrix[base_pick_num++].ymax = ly1 + 0.49 * yinc;
        }

    return status;
    }

/* ====================polyline_attribute_mark_type ========================== */
static INT tg_pa_mark_type (marked_type)
/* %S A help routine of TG_ASK_POLYLINE_ATTRIBUTES. Marks the selected type.  */



/* %p
    Input Parameters:
     MARKED_TYPE  =(INT ) the type id of being required to be marked.

    Output Parameters: none.

*/
INT marked_type;
    {
    FLOAT rad, sta, yinc, lx1, lx2, ly1, ly2;
    FLOAT markx[3], marky[3];
    INT i, num_across, npts = 3;
    DOUBLE scale = 4.0;
    INT done = FALSE;
    INT lwidth;
    INT status = SUCCESS;

    /* error checking */
    if (marked_type <= 0)
        return FAIL;

    /* erase the previous segment.    */
    if (typeMarkSeg > 0)
        {
        wmsdelete (typeMarkSeg);

        }

    /* create a new segment.    */
    status = wmsallocate (&typeMarkSeg);
    if (status < SUCCESS)
        {
        return status;
        }

    num_across = 4;
    rad = 1.0 / (FLOAT)(4 * num_across);
    lx1 = 0.5 + 1.5 * rad;
    sta = 0.35 + 1.5 * rad;
    lx2 = lx1 + .3;
    yinc = 0.7 * rad;

    /* MARKS line_type  */
    for (i = 1; (!done) && i < 12; i++)
        {
        ly1 = sta + (i - 1) * yinc;
        ly2 = ly1;
        if (i == marked_type)
            {
            lwidth = 1, wm_gsln (lwidth);
            marky[0] = marky[2] = ly1 + (yinc * 0.25);
            marky[1] = ly1 - (yinc * 0.25);
            markx[1] = (lx1 + lx2) / 2.0;
            markx[0] = (lx1 * 3.0 + lx2) / 4.0;
            markx[2] = (lx1 + lx2 * 3.0) / 4.0;
            wm_gsplci (highlight);
            wm_gslwsc (scale);
            wm_gpl (npts, markx, marky);
            done = TRUE;
            }
        }

    /* close  segment  */
    status = wmsclose (typeMarkSeg);

    return status;
    }

/* ====================polyline_mark_erase_width ========================== */
static INT tg_pa_mark_width (marked_width)
/* %S A help routine of TG_ASK_POLYLINE_ATTRIBUTES. Marks the selected width.   */



/* %P
    Input Parameters:
     MARKED_WIDTH  =(INT ) the width id of being required to be marked.

    Output Parameters: none.

*/
INT marked_width;
    {
    FLOAT rad, sta, yinc, lx1, lx2, ly1, markx[3], marky[3];
    DOUBLE scale = 4.0;
    INT i, num_across, done = FALSE, mark_it = 1;
    INT lwidth;
    INT npts = 3;
    INT status = SUCCESS;

    /* error checking   */
    if (marked_width <= 0)
        return FAIL;

    /* erase the previous segment.   */
    if (widthMarkSeg > 0)
        {
        wmsdelete (widthMarkSeg);

        }

    /* create a new segment.    */
    status = wmsallocate (&widthMarkSeg);
    if (status < SUCCESS)
        {
        return status;
        }

    for (i = 0, mark_it = 1; i < marked_width - 1; i++, mark_it = mark_it * 2)
        ;
    num_across = 4;
    rad = 1.0 / (FLOAT)(4 * num_across);
    sta = 1.5 * rad;
    lx1 = sta + 0.5;
    lx2 = lx1 + .3;
    yinc = 0.7 * rad;

    /* marks line_width */
    lwidth = 1, wm_gsln (lwidth);
    wm_gslwsc (scale);
    for (i = 1, ly1 = sta - yinc + 0.1; (!done) && i < 17; i = i * 2)
        {
        ly1 += yinc;
        if (i == mark_it)
            {
            marky[0] = marky[2] = ly1 + (yinc * 0.25);
            marky[1] = ly1 - (yinc * 0.25);
            markx[1] = (lx1 + lx2) / 2.0;
            markx[0] = (lx1 * 3.0 + lx2) / 4.0;
            markx[2] = (lx1 + lx2 * 3.0) / 4.0;
            wm_gsplci (highlight);
            wm_gpl (npts, markx, marky);
            done = TRUE;
            }
        }

    /* close the segment   */
    status = wmsclose (widthMarkSeg);


    return status;
    }

/* ==================polylinr_pick_buttons ==================================== */
static INT tg_pa_pick_buttons (ok, cancel)
/* %S A help routine of TG_ASK_POLYLINE_ATTRIBUTES. To display buttons.   */

/* %P
    Input Parameters: none.

    Output Parameters:
     OK   =(INT *) the pick id of OK_BUTTON.
     CANCEL =(INT *) the pick id of CANCEL_BUTTON.

*/
INT *ok, *cancel;
    {
    FLOAT areax[4], areay[4], cx, cy;
    INT pick, nsides = 4;
    INT fill;
    INT status = SUCCESS;

    wm_gschh (SMALL_TEXT_HEIGHT);

    /* create the ok button.    */

    *ok = base_pick_num;
    areax[0] = areax[3] = 0.5;
    areax[1] = areax[2] = 0.7;
    areay[0] = areay[1] = 0.05;
    areay[2] = areay[3] = 0.10;
    range_matrix[base_pick_num].xmin = 0.5;
    range_matrix[base_pick_num].xmax = 0.7;
    range_matrix[base_pick_num].ymin = 0.05;
    range_matrix[base_pick_num++].ymax = 0.10;
    cx = 0.55;
    cy = 0.08;
    lu_text_to_index ("FILL_STYLE", &fill, "SOLID");
    wm_gsfais (fill);
    wm_gsfaci (black);
    wm_gfa (nsides, areax, areay);
    lu_text_to_index ("FILL_STYLE", &fill, "HOLLOW");
    wm_gsfais (fill);
    wm_gsfaci (white);
    wm_gfa (nsides, areax, areay);
    wm_gtx (cx, cy, OK_BTN);

    /* create the CANCEL button.    */

    *cancel = base_pick_num;
    range_matrix[base_pick_num].xmin = areax[0] = areax[3] = 0.73;
    range_matrix[base_pick_num].xmax = areax[1] = areax[2] = 0.93;
    range_matrix[base_pick_num].ymin = areay[0] = areay[1] = 0.05;
    range_matrix[base_pick_num++].ymax = areay[2] = areay[3] = 0.10;
    cx = 0.75;
    cy = 0.08;
    lu_text_to_index ("FILL_STYLE", &fill, "SOLID");
    wm_gsfais (fill);
    wm_gsfaci (black);
    wm_gfa (nsides, areax, areay);
    lu_text_to_index ("FILL_STYLE", &fill, "HOLLOW");
    wm_gsfais (fill);
    wm_gsfaci (white);
    wm_gfa (nsides, areax, areay);
    wm_gtx (cx, cy, CANCEL_BTN);
    /*   status = wmpclose( pick ) ;    pick isn't initialized  */


    return status;
    }

/* =========================polyline_attribute_texts ========================= */
static INT tg_pa_texts ()
/* %S A help routine of TG_ASK_POLYLINE_ATTRIBUTES. To dispaly the TEXTS.   */



/* %P
    Input Parameters: none.
    Output Parameters: none.
*/
    {
    FLOAT cx, cy;
    GKS_REAL space = 0.0, chxp = 1.0, upx = 0.0, upy = 1.0;
    INT status = SUCCESS;
    GKS_INT font, prec, path, align_horiz, align_vert;

    /* set the attributes of text first.    */
    wm_gstxci (white);
    wm_gschsp (space);
    wm_gschxp (chxp);
    lu_text_to_index ("FONT", &font, "SIMPLEX");
    lu_text_to_index ("TEXT_PRECISION", &prec, "STROKE");
    wm_gstxfp (font, prec);
    lu_text_to_index ("TEXT_PATH", &path, "RIGHT");
    wm_gstxp (path);
    lu_text_to_index ("TEXT_ALIGNMENT", &align_horiz, "LEFT");
    lu_text_to_index ("TEXT_ALIGNMENT", &align_vert, "TOP");
    wm_gstxal (align_horiz, align_vert);
    wm_gschup (upx, upy);

    /* writes the tilte */
    cy = 1.0 - 0.03;
    cx = 0.15;
    wm_gschh (MAIN_TITLE_HEIGHT);
    /* wm_gtx(cx, cy, TITLE); */
    /* write subtitles.   */
    cy = 1.0 - 0.07;
    cx = 0.04;
    wm_gschh (SMALL_TEXT_HEIGHT);
    /* wm_gtx (cx, cy, COLOR); */

    /* **** */
    cy = 1.0 - 0.07;
    cx = 0.45;
    /* wm_gtx (cx, cy, TYPE); */

    /* **** */
    cy = 0.41;
    cx = 0.45;
    /* wm_gtx (cx, cy, WIDTH); */


    return status;

    }


/* ======================tg_ask_polyline_attributes =========================== */
publicdef INT tg_ask_polyline_attributes (color_returned, type_returned,
                                          width_returned, status_returned)

/* %S to ask attributes, color, type, width, of polyline.  first, sets the attributes
     to be default value, if the user cancels of  selecting, the returned value
     would be the default value, otherwise would be selected values.

*/

/* %P
    Input Parameters:
     COLOR_RETURNED = (INT *) the default value of color.
     TYPE_RETURNED  = (INT *) the default value of type.
     WIDTH_RETURNED = (GKS_REAL *) the default value of width.

    Output Parameters:
     COLOR_RETURNED = (INT *) the returned value of color.
     TYPE_RETURNED  = (INT *) the returned value of type.
     WIDTH_RETURNED = (GKS_REAL *) the returned value of width.
     TG_PA_ASK_ATTRIBUTES =(INT )
                  = 1 --- user clicks OK button and returns attributes.
                  = 0 -- user clicks CANCEL button and returns default
     STATUS_RETURNED = ( INT *)
             = 1 --- user clicks OK button and returns attributes.
             = 0 -- user clicks CANCEL button and returns default
                attributes.
*/

INT *color_returned, *type_returned, *status_returned;
GKS_REAL *width_returned;
    {

    static INT attrSeg = 0, color_begin_pick = 0, typeBackSeg = 0,
        widthBackSeg = 0, type_begin_pick = 0, width_begin_pick = 0,
        textSeg = 0;
    INT window, errind = 0, return_cancel = FALSE, width_default = 0;
    GKS_INT visibility = GVISI, old_color, old_type;
    GKS_REAL old_real_width;
    FLOAT ndcMinX, ndcMaxX, ndcMinY, ndcMaxY;
    FLOAT nwcMinX, nwcMaxX, nwcMinY, nwcMaxY;
    FLOAT wcsMinX, wcsMaxX, wcsMinY, wcsMaxY;
    INT status, segPicked, pick, button, change_color = 0, dummy = 0, i;
    static INT color_picked = -1, type_picked = -1, width_picked = -1,
        ok_button = -1, cancel_button = -1, tnr;
    INT appSave, winSave, tnrSave, groSave;
    INT id = MAIN_WKID;
    INT fill;
    INT defmod, regmod;

    /* query where in wm */
    wmowhere (&appSave, &winSave, &tnrSave, &groSave, &status);

    /* query the old polyline atrributes.   */
    width_default = log (*width_returned) / log (2) + 1.2;
    wm_gqplci (&errind, &old_color);
    wm_gqlwsc (&errind, &old_real_width);
    wm_gqln (&errind, &old_type);

    lu_text_to_index ("COLOR", &black, "BLACK");
    lu_text_to_index ("COLOR", &white, "WHITE");
    lu_text_to_index ("COLOR", &highlight, "HIGHLIGHT");

    /* Create window */
    if (stock_window_id == -1)
        {
        stock_app_id = appSave;
        wmoattach (stock_app_id, 0, 0, 0, &status);

        window = 0;

        /* to calculate the left_upper conor.   */
        wm_inq_wkwn (&ndcMinX, &ndcMaxX, &ndcMinY, &ndcMaxY);
        /* ------------------------------- */
        /* Only size the window into upper */
        /* left quarter of graphics window */
        /* when we don't have multi        */
        /* graphics windows                */
        /* ------------------------------- */
#ifndef USE_X
        ndcMinY = (ndcMinY + ndcMaxY) / 2.0;
        ndcMaxX = (ndcMinX + ndcMaxX) / 2.0;
#endif

        /* ----------------------------------- */
        /* Allocate a tool pallete size window */
        /* ----------------------------------- */
#ifdef PRIOR_GKS
        wmwallocate_sized (&window, ndcMinX, ndcMaxX, ndcMinY, ndcMaxY, black,
                           0.4);
#else
        wmwallocate_sized (&window, ndcMinX, ndcMaxX, ndcMinY, ndcMaxY, black,
                           1.00);
#endif
        stock_window_id = window;
        wmoset (window, WM_CLASS_WINDOW, "", "Polyline Palette");

        wm_inq_deferral (&defmod, &regmod);
        wm_set_deferral (GASAP, GSUPPD);

        /* Create transformation */
        tnr = 0;
        nwcMinX = 0.05;
        nwcMaxX = 0.95;     /* 5 persent margin */
        nwcMinY = 0.05;
        nwcMaxY = 0.95;
        wcsMinX = 0.0;
        wcsMaxX = 1.0;
        wcsMinY = 0.0;
        wcsMaxY = 1.0;
        wmtallocate (&tnr, nwcMinX, nwcMaxX, nwcMinY, nwcMaxY, wcsMinX,
                     wcsMaxX, wcsMinY, wcsMaxY);

        /* the segment#1 for TITLE.   */
        wmsallocate (&textSeg);

        /* writes it */
        tg_pa_texts ();
        lu_text_to_index ("FILL_STYLE", &fill, "HOLLOW");
        wm_gsfais (fill);
        wm_gsfaci (white);
        change_color = FALSE;
        tg_pa_colors (change_color, &dummy);

        /* Close segment */
        wmsclose (textSeg);

        /* the segment for the background of LINE TYPE.   */
        wmsallocate (&typeBackSeg);

        /* Draw it  */
        tg_pa_draw_background (0);

        /* Close segment */
        wmsclose (typeBackSeg);

        /* the segment for the background of LINE WIDTH.   */
        wmsallocate (&widthBackSeg);

        /* Draw it   */
        tg_pa_draw_background (1);

        /* Close segment */
        wmsclose (widthBackSeg);


        /* the segment#1 for COLOR.   */
        wmsallocate (&attrSeg);

        /* Draw it */
        lu_text_to_index ("FILL_STYLE", &fill, "SOLID");
        wm_gsfais (fill);
        change_color = TRUE;
        tg_pa_colors (change_color, &color_begin_pick);
        tg_pa_line_type (&type_begin_pick);
        tg_pa_line_width (&width_begin_pick);
        /* Don't need these funny looking buttons:
           tg_pa_pick_buttons(&ok_button, &cancel_button); */
        /* Close segment */
        wmsclose (attrSeg);

        /* Make segment detectable
           wmsdtec( attrSeg, TRUE ) ;  */

        }
    else
        {
        /* opens and displays the window again.    */
        visibility = GVISI;

        /* Attach to where window was allocated */
        wmoattach (stock_app_id, stock_window_id, tnr, 0, &status);
#ifndef USE_X
        /* ------------------------------------------- */
        /* When multi-windows available, we just leave */
        /* the window up and visible                   */
        /* ------------------------------------------- */
        wmwvis (stock_window_id, visibility);
        wmsvis (widthBackSeg, visibility);
        wmsvis (typeBackSeg, visibility);
        wmsvis (attrSeg, visibility);
        wmsvis (textSeg, visibility);
#endif

        }

    /* set the default attributes.    */
    tg_pa_mark_color (*color_returned);
    tg_pa_mark_width (width_default);
    tg_pa_mark_type (*type_returned);
    color_picked = *color_returned;
    type_picked = *type_returned;
    width_picked = width_default;

    /* Allow picking until Cancel or Done */
    wm_update ();
    do
        {
        status = 0;
        segPicked = 0;
        pick = 0;
        button = 0;
        tg_pa_request_pick (&status, &pick, &button);

        /* decide which attribute is selected.   */
        if (pick == ok_button || pick == cancel_button || pick == 99)
            ;
        else if (pick >= width_begin_pick)
            {
            /* erase the previous choice.   */
            visibility = GVISI;
            wmsvis (widthBackSeg, visibility);
            wmsvis (attrSeg, visibility);
            if (colorMarkSeg != -1)
                wmsvis (colorMarkSeg, visibility);
            if (typeMarkSeg != -1)
                wmsvis (typeMarkSeg, visibility);
            width_picked = pick - width_begin_pick + 1;

            /* temperary only allows user to pick the standard thinkness 1, when the type
               of line is not SOILD. */

#ifndef PRIOR_GKS
            width_picked = (type_picked == 1) ? width_picked : 1;
#endif
            tg_pa_mark_width (width_picked);
            }
        else if (pick >= type_begin_pick)
            {
            /* erase the previous choice.   */
            visibility = GVISI;
            wmsvis (typeBackSeg, visibility);
            wmsvis (attrSeg, visibility);
            if (colorMarkSeg != -1)
                wmsvis (colorMarkSeg, visibility);
            if (widthMarkSeg != -1)
                wmsvis (widthMarkSeg, visibility);
            type_picked = pick - type_begin_pick + 1;

            /* temperary only allow user to pick the type SOILD, when the thinkness of line
               is greater than 1. */
#ifndef PRIOR_GKS
            type_picked = (width_picked == 1) ? type_picked : 1;
#endif
            tg_pa_mark_type (type_picked);
            }
        else if (pick >= color_begin_pick)
            {
            /* erase the previous choice.   */
            visibility = GVISI;
            wmsvis (attrSeg, visibility);
            if (widthMarkSeg != -1)
                wmsvis (widthMarkSeg, visibility);
            if (typeMarkSeg != -1)
                wmsvis (typeMarkSeg, visibility);
            lu_text_to_index ("FILL_STYLE", &fill, "SOLID");
            wm_gsfais (fill);
            color_picked = pick - color_begin_pick;
            tg_pa_mark_color (color_picked);
            }


        wm_update ();
        /* updates choice.    */
        }
    while (button != WM_BUTTON_CANCEL && button != WM_BUTTON_DONE &&
           pick != ok_button && pick != cancel_button);

    return_cancel = ((pick == ok_button) ||
                     (button == WM_BUTTON_DONE)) ? FALSE : TRUE;

    /* delete mark segments.    */
    if (colorMarkSeg > 0)
        {
        wmsdelete (colorMarkSeg);
        colorMarkSeg = -1;
        }

    if (typeMarkSeg > 0)
        {
        wmsdelete (typeMarkSeg);
        typeMarkSeg = -1;
        }

    if (widthMarkSeg > 0)
        {
        wmsdelete (widthMarkSeg);
        widthMarkSeg = -1;
        }

#ifndef USE_X
    /* ------------------------------------------- */
    /* When multi-windows available, we just leave */
    /* the window up and visible                   */
    /* ------------------------------------------- */
    /* erase the window.      */
    visibility = GINVIS;
    wmsvis (widthBackSeg, visibility);
    wmsvis (typeBackSeg, visibility);
    wmsvis (attrSeg, visibility);
    wmsvis (textSeg, visibility);
    wmwvis (stock_window_id, visibility);
#endif

    /* Close window */
    wmwclose (stock_window_id);
    if (ug_if_gpx ())
        wm_update ();
    else
        wm_grsgwk (id);
    wm_set_deferral (defmod, regmod);

    /* Attach to saved wm state */
    wmoattach (appSave, winSave, tnrSave, groSave, &status);

    /* returns picked attributes.    */
    if (!return_cancel)
        {
        *color_returned = color_picked;
        for (i = 1, *width_returned = 1; i < width_picked;
             * width_returned = *width_returned * 2, i++)
            ;
        *type_returned = type_picked;
        *status_returned = 2;   /* done */
        return 1;
        }
    else
        {
	if (stock_window_id IS_NOT_EQUAL_TO -1)
	  {
	    tg_delete_window();
	  }
        *status_returned = 1;   /* cancel */
        return 0;
        }
    }

/* ********************************************************************** */
static INT tg_pa_request_pick (status, pick, button)
INT *status, *pick, *button;
    {
    INT i;

    GKS_REAL x, y;

    if (ug_if_gpx ())
        wm_set_mouse_menu ((CHAR * *)0, (INT *)0);  /* use default menu */

    wmtlocate (status, &x, &y, button);
    if (*button == WM_BUTTON_DONE || *button == WM_BUTTON_CANCEL)
        {
        *pick = 99;
        return FAIL;
        }


    for (i = 0; i < base_pick_num; i++)
        {
        if (x <= range_matrix[i].xmax && x >= range_matrix[i].xmin &&
            y <= range_matrix[i].ymax && y >= range_matrix[i].ymin)
            {
            *pick = i;
            return SUCCESS;
            }
        }
    *pick = 99;
    return FAIL;

    }

publicdef INT tg_window_id()
{
  return(stock_window_id);
}

publicdef VOID tg_delete_window()
{
  wmwdelete(stock_window_id);
  stock_app_id = -1;
  stock_window_id = -1;
  typeMarkSeg = -1;
  widthMarkSeg = -1;
  colorMarkSeg = -1;
  black = -1;
  white = -1;
  highlight = -1;
  base_pick_num = 0;
}






