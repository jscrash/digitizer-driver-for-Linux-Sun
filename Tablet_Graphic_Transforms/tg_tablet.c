/* DEC/CMS REPLACEMENT HISTORY, Element TG_TABLET.C*/
/* *4    31-OCT-1990 09:31:19 GILLESPIE "(SPR 37) Sun compile fixes"*/
/* *3    17-AUG-1990 22:27:33 VINCE "(SPR 5644) Code Cleanup"*/
/* *2    13-NOV-1989 11:51:56 JULIAN "(SPR 1) Added support for CALCOMP tablets"*/
/* *1    19-JUN-1989 13:32:46 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element TG_TABLET.C*/
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

#include "esi_tg_mat.h"

#ifndef ESI_TB_DEFS_H

#include "esi_tb_defs.h"

#endif

#ifndef ESI_MATH_H

#include "esi_math.h"

#endif

#ifndef ESI_TG_ERR_H

#include "esi_tg_err.h"

#endif
publicdef INT tg_template_case ();

#ifdef REG_INT

#undef REG_INT

#endif

#ifdef primos

#define REG_INT INT

#else

#define REG_INT register INT

#endif

#define MAXTEMPL 5

/********************************************/
/*  GLOBAL TEMPLATE XFORM MATRIX        */

static MATRIX TEM_XFORM = 
    {
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0
    };



/***************************************/
/*  GLOBAL REG XFORM MATRIX        */

static MATRIX REG_XFORM = 
    {
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0
    };



struct template 
    {
    INT xmin;
    INT ymin;
    INT xmax;
    INT ymax;
    };

static  struct template templ;

static INT nbtempl;
static DOUBLE templ_ext[MAXTEMPL][4];
static INT templ_box[MAXTEMPL][2];

static glob_xform_init_flag = FALSE;

#if USE_PROTOTYPES
static INT zz_get_coord (INT *x, INT *y, INT *b, INT done_btn);
static INT zz_msg (CHAR *str);
#else
static INT zz_get_coord ();
static INT zz_msg ();
#endif

/* File Description ---------------------------------------------------------
Overview:

Prototypes and one line descriptions of each function in the file.
Public_Functions:
    publicdef INT tg_calc_reg_xform(EXTENTS *t_reg,EXTENTS *t_wcs,FLOAT   *pangle,
        MATPTR pxform);
    
    publicdef INT tg_register_tab(EXTENTS *wcs_ext);
    
    publicdef INT tg_set_xform(EXTENTS  *t_wcs);
    
    publicdef INT tg_reg_to_wcs(DOUBLE  xreg,DOUBLE yreg,DOUBLE *xwcs,DOUBLE *ywcs,
        MATPTR  XFORM);
    
    publicdef INT tg_calc_xform(EXTENTS *t_reg,EXTENTS  *t_wcs,FLOAT *pangle,BOOL template);
    
    publicdef INT tg_template_locator(INT   nreq,DOUBLE *x_arr,DOUBLE *y_arr,INT    *b_arr,
        BOOL templ_on,INT   *t_arr,INT  *c_arr, INT *nret,INT   *state);
    
    publicdef INT tg_ini_templ_rect(INT t_xmin,INT  t_ymin,INT  t_xmax,INT  t_ymax);
    
    publicdef INT tg_ini_templates(INT  nt,INT  (*t_box)[MAXTEMPL][2],DOUBLE    (*t_ext)[MAXTEMPL][4]);
    
    publicdef INT tg_template_case(DOUBLE   x,DOUBLE    y,INT   *t,INT  *c);

Private_Functions:
    static INT zz_get_coord(INT *x,INT *y,INT *b,INT done_btn);
    
    static INT zz_msg(CHAR *str);

-----------------------------------------------------------------------------*/

/* Function Description -----------------------------------------------------
Description:
    Given an extents structure describing the three registered 
    corners of a document on the tablet, and an extents structure
    describing the users WCS coord. system, create the registration
    transform that converts raw tablet coords into fully angle-
    corrected, scaled and translated WCS XY coords.
       
    The calculation permits registration of media that is not 
    aligned normal to the axis of the tablet by applying a 
    correcting rotation.  The direction of the correcting rotate
    as well as the number of degrees to follow through is found
    by analyzing the 'Normalness' of the media axis to the axis
    of the tablet.  
    
    There is one limitation with regards to media alignment:
    Since the digitized corners are eventually tied to the extents
    of a WCS description, it is necessary to have a notion of 
    'Max Y position' 'Max X Postion' 'Min x,y position' etc.
    I enforce this at registration time by rejecting input where
    Upper Left Y is for example smaller than Lower Left Y etc. 
     
    One can get around this easily however by arranging the WCS
    extents struct such that it matches the scheme registered.
    
Prototype:
    publicdef INT tg_calc_reg_xform(EXTENTS *t_reg,EXTENTS *t_wcs,FLOAT   *pangle,
        MATPTR pxform);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) t_reg       -(EXTENTS *) Struct of digitized media limits.
    (I) t_wcs       -(EXTENTS *) Struct describing Curr. WCS space.
    pangle          -(FLOAT *)
    pxform          -(MATPTR)
    
Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT tg_calc_reg_xform (EXTENTS *t_reg, EXTENTS *t_wcs, FLOAT *pangle,
                                 MATPTR pxform)
#else
publicdef INT tg_calc_reg_xform (t_reg, t_wcs, pangle, pxform)
EXTENTS *t_reg;
EXTENTS *t_wcs;
FLOAT *pangle;
MATPTR pxform;
#endif
    {
    INT status = SUCCESS;
    REG_INT i, j;
    
    MATRIX final, smat, tmat, tmat2, rmat;
    
    DOUBLE xscale, yscale;
    
    DOUBLE ang1, ang2, ang3, z1, z2, ct, st, theta, dy1, dx1, dy2, dx2, ang_deg;
    
    MATRIX XFORM;
    
    hoblockmove (pxform, XFORM, sizeof(MATRIX));
    
    /* clear the temp & REG matrices */
    tg_zeromat (final);
    tg_zeromat (XFORM);
    
    /* set translation matrix for move to origin. */
    tg_zeromat (tmat);
    tg_set_identity (tmat);
    
    tmat[MAXIND][MAXIND] = 1.0;
    tmat[MAXIND][0] = -t_reg->ll_x;
    tmat[MAXIND][1] = -t_reg->ll_y;
    
    /* set translation matrix for move to back into place.  */
    /* not currently used.                  */
    tg_zeromat (tmat2);
    tg_set_identity (tmat2);
    tmat2[MAXIND][MAXIND] = 1.0;
    tmat2[MAXIND][0] = t_wcs->ll_x;
    tmat2[MAXIND][1] = t_wcs->ll_y;
    
    /* set scale matrix */
    tg_zeromat (smat);
    smat[MAXIND][MAXIND] = 1.0;
    
    /* x scale */
    z1 = (t_reg->lr_x - t_reg->ll_x);
    z1 *= z1;
    z2 = (t_reg->lr_y - t_reg->ll_y);
    z2 *= z2;
    z1 += z2;
    xscale = ((t_wcs->lr_x - t_wcs->ll_x) / sqrt (z1));
    smat[0][0] = xscale;
    
    /* y scale */
    z1 = (t_reg->ul_x - t_reg->ll_x);
    z1 *= z1;
    z2 = (t_reg->ul_y - t_reg->ll_y);
    z2 *= z2;
    z1 += z2;
    yscale = ((t_wcs->ul_y - t_wcs->ll_y) / sqrt (z1));
    smat[1][1] = yscale;
    
    /* 
        Calc. the required angle for correction 
    */
    
    dy1 = t_reg->ll_y - t_reg->lr_y;    /* x axis angle correction/check */
    dx1 = t_reg->lr_x - t_reg->ll_x;
    
    dx2 = t_reg->ul_x - t_reg->ll_x;    /* Y axis angle correction/check */
    dy2 = t_reg->ul_y - t_reg->ll_y;
    ang1 = atan2 (dy1, dx1);
    ang2 = atan2 (dx2, dy2);
    theta = ((ang1 + ang2) / 2.0);
    *pangle = (FLOAT)(((ang2 - ang1) * 57.29577951) + 90.00);
    
    ct = cos (theta);
    st = sin (theta);
    
    tg_zeromat (rmat);
    rmat[MAXIND][MAXIND] = 1.0;
    
    /* original */
    rmat[0][0] = ct;
    rmat[0][1] = -1.0 * st;
    rmat[1][0] = st;
    rmat[1][1] = ct;
    
    /* multiply the translate-to-origin matrix with the rotate-by-theta matrix */
    /* with the scale matrix - CONCATENATION    */
    
    tg_mat_mult (rmat, tmat, final);    /* final = tmat * smat      */
    tg_mat_mult (smat, final, XFORM);   /* XFORM = final * rmat     */
    XFORM[0][2] = t_wcs->ll_x;
    XFORM[1][2] = t_wcs->ll_y;
    
    hoblockmove (XFORM, pxform, sizeof(MATRIX));
    glob_xform_init_flag = TRUE;
    
    return status;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    Returns 1 or more registered, transformed WCS coordinates from
    the digitizing tablet. 

Prototype:
    publicdef INT tg_req_locator(INT nreq, DOUBLE *x_arr,DOUBLE *y_arr,INT  *b_arr, 
        INT *nret,INT   *state);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) nreq        -(INT) Max # of points to fetch.
    (I) x_arr       -(DOUBLE) -\
    (I) y_arr       -(DOUBLE) Coord buffers.
    (I) b_arr       -(INT) Assoc. button states.
    (O) nret        -(INT *) Number of points returned.
    (O) state       -(INT *) State of operation at exit.

Return Value/Status:
    SUCCESS - Successful completion.
    TB_NO_POINTS
    TB_NOT_INIT
    TB_BUF_OVERFLOW
    TG_BAD_XFORM
    TG_BAD_COUNT

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT tg_req_locator (INT nreq, DOUBLE *x_arr, DOUBLE *y_arr, INT *b_arr,
                              INT *nret, INT *state)
#else
publicdef INT tg_req_locator (nreq, x_arr, y_arr, b_arr, nret, state)
INT nreq;
DOUBLE *x_arr;
DOUBLE *y_arr;
INT *b_arr;
INT *nret;
INT *state;
#endif
    {
    INT status = SUCCESS;
    INT npts;
    INT index, i, j;
    INT ix, iy, ib, retcnt;
    DOUBLE x, y;
    
    if (nreq <= 0)
        {
        *state = TG_BAD_COUNT;
        status = *state;
        }
    else if (glob_xform_init_flag != TRUE)
        {
        *state = TG_BAD_XFORM;
        status = *state;
        }
    else
        {
        for (i = 0; i < nreq; i++)
            {
            ib = 0;
            ix = 0;
            iy = 0;
            status = tbh_get_points (1, &ix, &iy, &ib, &retcnt);
            
            if (status == TB_NO_POINTS OR (retcnt != 1))
                break;
            x = (DOUBLE)ix;
            y = (DOUBLE)iy;
            
            if (status = tg_reg_to_wcs (x, y, x_arr++, y_arr++, (MATPTR)REG_XFORM))
                break;
            *b_arr++ = ib;
            }
        if ((i == 0))
            {
            *nret = 0;
            *state = TB_NO_POINTS;
            }
        else
            {
            *nret = i;
            *state = SUCCESS;
            }
        }
    return status;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    Prompt the user through a registration process.

Prototype:
    publicdef INT tg_register_tab(EXTENTS *wcs_ext);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) wcs_ext     -(EXTENTS *) Min,Max xy description of WCS space.

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT tg_register_tab (EXTENTS *wcs_ext)
#else
publicdef INT tg_register_tab (wcs_ext)
EXTENTS *wcs_ext;
#endif
    {
    INT status = SUCCESS;
    
    static CHAR *ulprompt =
        "Digitize the top left corner.  (1 = enter, C = cancel)",
        *llprompt = "Digitize the bottom left corner.  (1 = enter, C = cancel)",
        *lrprompt = "Digitize the bottom right corner.  (1 = enter, C = cancel)";
    
    EXTENTS tabraw;
    EXTENTS tabwcs;
    INT ix, iy, ibtn, done_btn;
    DOUBLE wcs_x, wcs_y, fx, fy, nx, ny;
    EXTENTS tabsize;
    DOUBLE tdx, tdy, wdx, wdy, ndx, ndy;
    FLOAT theta;
    
    /* ask the driver how big the current tablet is. */
    status = tbh_inq_tablet (TB_INQ_EXTENTS, (VOIDPTR)&tabsize);
    
    tdx = tabsize.lr_x - tabsize.ll_x;
    tdy = tabsize.ul_y - tabsize.ll_y;
    
    done_btn = 1;                   /* D key on puck */
    
do_ul:
    zz_msg (ulprompt);
    if ((status = zz_get_coord (&ix, &iy, &ibtn, done_btn)) == CANCEL)
        goto tb_cancel;
    if (status == TB_NOT_INIT)
        {
        zz_msg ("Tablet driver is not initialized.... Aborting.");
        return status;
        }
    fx = (DOUBLE)ix;
    fy = (DOUBLE)iy;
    /* convert to NDC coords. */
    nx = (fx / tdx);
    ny = (fy / tdy);
    tabraw.ul_x = fx;
    tabraw.ul_y = fy;
    if (tabraw.ul_y <= 0.0001)
        {
        zz_msg ("Error...Upper left point must be greater than 0.");
        goto do_ul;
        }
do_ll:
    zz_msg (llprompt);
    if ((status = zz_get_coord (&ix, &iy, &ibtn, done_btn)) == CANCEL)
        goto tb_cancel;
    fx = (DOUBLE)ix;
    fy = (DOUBLE)iy;
    /* convert to NDC coords from RAW tablet. */
    nx = (fx / tdx);
    ny = (fy / tdy);
    tabraw.ll_x = fx;
    tabraw.ll_y = fy;
    
    if (tabraw.ul_y <= (tabraw.ll_y + 0.0001))
        {
        zz_msg ("Error...pbottom left point must be less than top corner.");
        goto do_ll;
        }
do_lr:
    zz_msg (lrprompt);
    if ((status = zz_get_coord (&ix, &iy, &ibtn, done_btn)) == CANCEL)
        goto tb_cancel;
    
    fx = (DOUBLE)ix;
    fy = (DOUBLE)iy;
    /* convert to NDC coords from RAW tablet. */
    nx = (fx / tdx);
    ny = (fy / tdy);
    tabraw.lr_x = fx;
    tabraw.lr_y = fy;
    
    if (tabraw.lr_x <= (tabraw.ll_x + 0.0001))
        {
        zz_msg ("Error....Bottom right point must be greater than bottom left.");
        goto do_lr;
        }
    status = tg_calc_reg_xform (&tabraw, wcs_ext, &theta, (MATPTR)REG_XFORM);
    
tb_cancel:
    return status;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:

Prototype:
    static INT zz_get_coord(INT *x,INT *y,INT *b,INT done_btn);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    x               -(INT *)
    y               -(INT *)
    b               -(INT *)
    done_btn        -(INT)

Return Value/Status:
    SUCCESS - Successful completion.
    CANCEL
    
Scope:
    PRIVATE to <tg_tablet.c>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static INT zz_get_coord (INT *x, INT *y, INT *b, INT done_btn)
#else
static INT zz_get_coord (x, y, b, done_btn)
INT *x, *y, *b;
INT done_btn;
#endif
    {
    INT status = SUCCESS;
    INT cnt = 0;
    INT tx, ty, tb;
    
    tb = 0;
    while (cnt == 0 OR (tb != done_btn))
        {
        status = tbh_get_points (1, &tx, &ty, &tb, &cnt);
        
        if (status == TB_NOT_INIT OR (tb == 12))
            break;
        }
    if (tb == 12)
        status = CANCEL;
    else if (status != TB_NOT_INIT)
        {
        *x = tx;
        *y = ty;
        *b = tb;
        status = SUCCESS;
        }
    else
        {
        *x = 0;
        *y = 0;
        *b = 0;
        }
    return status;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:

Prototype:
    static INT zz_msg(CHAR *str);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    str             -(CHAR *)

Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PRIVATE to <tg_tablet.c>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static INT zz_msg (CHAR *str)
#else
static INT zz_msg (str)
CHAR *str;
#endif
    {
    printf ("\n%s\n", str);
    return SUCCESS;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    Given an extents structure describing the WCS coord system extents
    of the users current transform calculate the proper scale factor.

Prototype:
    publicdef INT tg_set_xform(EXTENTS  *t_wcs);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) t_wcs       -(EXTENTS *) Struct describing Curr. WCS space.

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.
    Function returns an xform stored in the global REG_XFORM matrix.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT tg_set_xform (EXTENTS *t_wcs)
#else
publicdef INT tg_set_xform (t_wcs)
EXTENTS *t_wcs;
#endif
    {
    INT status = SUCCESS;
    REG_INT i, j;
    EXTENTS t_ext;
    
    DOUBLE xscale, yscale;
    
    status = tbh_inq_tablet (TB_INQ_EXTENTS, (VOIDPTR)&t_ext);
    
    /* x scale */
    xscale = ((t_wcs->lr_x - t_wcs->ll_x) / (t_ext.lr_x - t_ext.ll_x));
    /* y scale */
    yscale = ((t_wcs->ul_y - t_wcs->ll_y) / (t_ext.ul_y - t_ext.ll_y));
    
    /* clear the  REG matrix */
    tg_zeromat (REG_XFORM);
    
    /* set scale matrix */
    REG_XFORM[MAXIND][MAXIND] = 1.0;
    REG_XFORM[0][0] = xscale;
    REG_XFORM[1][1] = yscale;
    
    glob_xform_init_flag = TRUE;
    
    return status;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    This function is added to get the templates map coordinates (wcs)
    from the templates tablet coordinates (reg).

Prototype:
    publicdef INT tg_reg_to_wcs(DOUBLE  xreg,DOUBLE yreg,DOUBLE *xwcs,DOUBLE *ywcs,
        MATPTR  XFORM);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) xreg        -(DOUBLE)
    (I) yreg        -(DOUBLE)
    (O) xwcs        -(DOUBLE *)
    (O) ywcs        -(DOUBLE *)
    XFORM           -(MATPTR)

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT tg_reg_to_wcs (DOUBLE xreg, DOUBLE yreg, DOUBLE *xwcs, DOUBLE *ywcs,
                             MATPTR XFORM)
#else
publicdef INT tg_reg_to_wcs (xreg, yreg, xwcs, ywcs, XFORM)
DOUBLE xreg, yreg;
DOUBLE *xwcs, *ywcs;
MATPTR XFORM;
#endif
    {
    if (!(glob_xform_init_flag))
        return FAIL;
    
    xreg += ((*XFORM)[2][0]);
    yreg += ((*XFORM)[2][1]);
    
    *xwcs = xreg * ((*XFORM)[0][0]) + yreg * ((*XFORM)[0][1]) + ((*XFORM)[0][2]);
    *ywcs = xreg * ((*XFORM)[1][0]) + yreg * ((*XFORM)[1][1]) + ((*XFORM)[1][2]);
    
    return SUCCESS;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    This function only calls tg_calc_reg_xform with a different matrix,
    depending on the template flag.

Prototype:
    publicdef INT tg_calc_xform(EXTENTS *t_reg,EXTENTS  *t_wcs,FLOAT *pangle,BOOL template);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) t_reg       -(EXTENTS *)
    (I) t_wcs       -(EXTENTS *)
    (I) template    -(BOOL)
    (O) pangle      -(FLOAT *)

Return Value/Status:
    Function returns status from tg_calc_reg_xform.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT tg_calc_xform (EXTENTS *t_reg, EXTENTS *t_wcs, FLOAT *pangle,
                             BOOL template)
#else
publicdef INT tg_calc_xform (t_reg, t_wcs, pangle, template)
EXTENTS *t_reg;
EXTENTS *t_wcs;
FLOAT *pangle;
BOOL template;
#endif
    {
    INT status;
    
    if (template)
        status = tg_calc_reg_xform (t_reg, t_wcs, pangle, (MATPTR)TEM_XFORM);
    
    else
        status = tg_calc_reg_xform (t_reg, t_wcs, pangle, (MATPTR)REG_XFORM);
    
    return status;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    Returns 1 or more digitized points with either their WCS coordinates
    or their position in a template.

Prototype:
    publicdef INT tg_template_locator(INT   nreq,DOUBLE *x_arr,DOUBLE *y_arr,INT    *b_arr,
        BOOL templ_on,INT   *t_arr,INT  *c_arr, INT *nret,INT   *state);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) nreq        -(INT) Max # of points to fetch.
    (I) templ_on    -(BOOL) Templates enabled.
    (O) x_arr       -(DOUBLE *) -\
    (O) y_arr       -(DOUBLE *) Coord buffers.
    (O) b_arr       -(INT *) Assoc. button states.
    (O) t_arr       -(INT *) Template identificator.
    (O) c_arr       -(INT *) Case of the template.
    (O) nret        -(INT *) Number of points returned.
    (O) state       -(INT *) State of operation at exit.

Return Value/Status:
    SUCCESS - Successful completion.
    TB_NO_POINTS
    TB_NOT_INIT
    TB_BUF_OVERFLOW
    TG_BAD_XFORM
    TG_BAD_COUNT 

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT tg_template_locator (INT nreq, DOUBLE *x_arr, DOUBLE *y_arr, INT *b_arr,
                                   BOOL templ_on, INT *t_arr, INT *c_arr, INT *nret,
                                   INT *state)
#else
publicdef INT tg_template_locator (nreq, x_arr, y_arr, b_arr, templ_on, t_arr, c_arr,
                                   nret, state)
INT nreq;
DOUBLE *x_arr;
DOUBLE *y_arr;
INT *b_arr;
BOOL templ_on;
INT *t_arr;
INT *c_arr;
INT *nret;
INT *state;
#endif
    {
    INT status = SUCCESS;
    INT npts;
    INT index, i, j;
    INT ix, iy, ib, retcnt;
    DOUBLE x, y;
    INT t = 0;
    INT c = 0;
    
    if (nreq <= 0)
        {
        *state = TG_BAD_COUNT;
        status = *state;
        }
    else if (glob_xform_init_flag != TRUE)
        {
        *state = TG_BAD_XFORM;
        status = *state;
        }
    else
        {
        for (i = 0; i < nreq; i++)
            {
            ib = 0;
            ix = 0;
            iy = 0;
            status = tbh_get_points (1, &ix, &iy, &ib, &retcnt);
            
            if (status == TB_NO_POINTS OR (retcnt != 1))
                break;
            
            if (templ_on AND ix >= templ.xmin AND ix <= templ.xmax AND iy >=
                templ.ymin AND iy <= templ.ymax)
                {
                if (status = tg_reg_to_wcs ((DOUBLE)ix, (DOUBLE)iy, &x, &y,
                                            (MATPTR)TEM_XFORM))
                    break;
                tg_template_case (x, y, &t, &c);
                if (t)
                    {
                    *t_arr++ = t;
                    *c_arr++ = c;
                    *b_arr++ = ib;
                    continue;
                    }
                }
            if (status = tg_reg_to_wcs ((DOUBLE)ix, (DOUBLE)iy, x_arr++, y_arr++,
                                        (MATPTR)REG_XFORM))
                break;
            *b_arr++ = ib;
            *t_arr++ = 0;
            
            }
        if ((i == 0))
            {
            *nret = 0;
            *state = TB_NO_POINTS;
            }
        else
            {
            *nret = i;
            *state = SUCCESS;
            }
        }
    return status;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    Initialize the static structure templ.

Prototype:
    publicdef INT tg_ini_templ_rect(INT t_xmin,INT  t_ymin,INT  t_xmax,INT  t_ymax);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) t_xmin      -(INT) -|   
    (I) t_ymin      -(INT) -|
    (I) t_xmax      -(INT) -\
    (I) t_ymax      -(INT) Coordinates of the rectangle, including the templates.

Return Value/Status:
    None.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT tg_ini_templ_rect (INT t_xmin, INT t_ymin, INT t_xmax, INT t_ymax)
#else
publicdef INT tg_ini_templ_rect (t_xmin, t_ymin, t_xmax, t_ymax)
INT t_xmin;
INT t_ymin;
INT t_xmax;
INT t_ymax;
#endif
    {
    templ.xmin = t_xmin;
    templ.ymin = t_ymin;
    templ.xmax = t_xmax;
    templ.ymax = t_ymax;
    
    return;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    Initialize the static arrays containing the templates description
    in templates units.

Prototype:
    publicdef INT tg_ini_templates(INT  nt,INT  (*t_box)[MAXTEMPL][2],DOUBLE    (*t_ext)[MAXTEMPL][4]);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) nt          -(INT) Number of templates.
    (I) t_box       -(INT) Array with Nb of rows and columns /template.
    (I) t_ext       -(DOUBLE) Array with X,Y Min & Max /template.

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT tg_ini_templates (INT nt, INT (*t_box)[MAXTEMPL][2],
                                DOUBLE (*t_ext)[MAXTEMPL][4])
#else
publicdef INT tg_ini_templates (nt, t_box, t_ext)
INT nt;
INT (*t_box)
[MAXTEMPL][2];
DOUBLE (*t_ext)
[MAXTEMPL][4];
#endif
    {
    nbtempl = nt;
    hoblockmove (t_box, templ_box, sizeof(INT) * MAXTEMPL * 2);
    hoblockmove (t_ext, templ_ext, sizeof(DOUBLE) * MAXTEMPL * 4);
    
    return;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    Finds the template id and the case (box) picked.

Prototype:
    publicdef INT tg_template_case(DOUBLE   x,DOUBLE    y,INT   *t,INT  *c);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) x           -(DOUBLE) -\
    (I) y           -(DOUBLE) Coordinates of the picked point.
    (O) t           -(INT *) Template id.
    (O) c           -(INT *) Case id.

Return Value/Status:
    None.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT tg_template_case (DOUBLE x, DOUBLE y, INT *t, INT *c)
#else
publicdef INT tg_template_case (x, y, t, c)
DOUBLE x, y;
INT *t;
INT *c;
#endif
    {
    INT i, row, col;
    
    for (i = 0; i < nbtempl; i++)
        {
        if (templ_ext[i][0] <= x AND x <= templ_ext[i][1] AND templ_ext[i][2] <=
            y AND y <= templ_ext[i][3])
            {
            col = (INT)(templ_box[i][1] * (x - templ_ext[i][0]) / (templ_ext[i][1] -
                                                                   templ_ext[i][0]) +
                        1.00);
            if (col < 1 OR col > templ_box[i][1])
                continue;
            row = (INT)(templ_box[i][0] * (y - templ_ext[i][2]) / (templ_ext[i][3] -
                                                                   templ_ext[i][2]) +
                        1.00);
            if (row < 1 OR row > templ_box[i][0])
                continue;
            *t = i + 1;
            *c = col + templ_box[i][1] * (templ_box[i][0] - row);
            return;
            }
        }
    *t = 0;
    *c = 0;
    return;
    }
/* END:     */
