/* DEC/CMS REPLACEMENT HISTORY, Element MO_GRID_TOOLS.C*/
/* *8    15-AUG-1991 17:03:42 JTM "(SPR 0) Changed argument to wm_gslwsc() to DOUBLE."*/
/* *7    17-AUG-1990 22:01:24 VINCE "(SPR 5644) Code Cleanup"*/
/* *6    20-APR-1990 18:30:24 JULIAN "(SPR 0) fix text leaking"*/
/* *5    18-APR-1990 11:54:47 JULIAN "(SPR 1) fix tg_fit_string"*/
/* *4    24-OCT-1989 14:32:34 JULIAN "(SPR 0) mod param passing on nl_inq_nlist_user_struct"*/
/* *3    14-SEP-1989 16:18:19 GORDON "(SPR 100) Gulf/Sun/Unix mods"*/
/* *2    28-JUL-1989 11:24:45 GILLESPIE "(SPR 1) fix minor casting problems"*/
/* *1    19-JUN-1989 13:09:20 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element MO_GRID_TOOLS.C*/
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

#include "esi_mo.h"
#include "mo_strat.rh"
#include "esi_wm.h"
#include "esi_le_nled.h"
#include "esi_tg.h"
#include "esi_lu.h"
#include "esi_gks.h"

publicdef INT mo_post_text ();
/* File Description ---------------------------------------------------------
Overview:
    Grid tools.
    
Prototypes and one line descriptions of each function in the file.
Public_Functions:
    publicdef INT mo_graticule(STRAT_MODEL_STRUCT *pModel, DOUBLE *x_grid_int,
        DOUBLE *y_grid_int);
    
    publicdef INT mo_mark_layer_ends(STRAT_MODEL_STRUCT *pModel, NLIST_HEADER nlist);
    
    publicdef INT mo_mark_sed_type( STRAT_MODEL_STRUCT  *pModel, NLIST_HEADER nlist);
    
    publicdef INT mo_post_text(STRAT_MODEL_STRUCT *pModel, DOUBLE x_ndc,DOUBLE y_ndc,
        DOUBLE height_ndc,INT gks_align_h, INT gks_align_v,INT gks_path,DOUBLE gks_upx,
        DOUBLE gks_upy,CHAR text[]);

Private_Functions:

-----------------------------------------------------------------------------*/

/* Function Description -----------------------------------------------------
Description:
    This routine is used to overlay a grid on the model while  
    while digitizing. A border is drawn around the model bou-  
    daries in thick line and the grid lines are drawn in thin  
    line.                                                      

Prototype:
    publicdef INT mo_graticule(STRAT_MODEL_STRUCT *pModel, DOUBLE *x_grid_int,
        DOUBLE *y_grid_int);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    pModel          -(STRAT_MODEL_STRUCT *)
    x_grid_int      -(DOUBLE *)
    y_grid_int      -(DOUBLE *)

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT mo_graticule (STRAT_MODEL_STRUCT *pModel, DOUBLE *x_grid_int,
                            DOUBLE *y_grid_int)
#else
publicdef INT mo_graticule (pModel, x_grid_int, y_grid_int)
STRAT_MODEL_STRUCT *pModel;
DOUBLE *x_grid_int, *y_grid_int;
#endif
    {
    
    FLOAT xdraw_buf[5], ydraw_buf[5];   /* GKS line buffers            */
    INT bor_color, grid_color;      /* GKS line attributes         */
    INT label_color;
    DOUBLE bor_line_width = 2.0;
    DOUBLE  grid_line_width = 1.0;
    FLOAT char_height, char_width, chh; /* GKS character variables     */
    FLOAT char_xmin, char_xmax;
    FLOAT char_spacing = 0.1;
    FLOAT char_xpos, char_ypos;
    FLOAT y_pos, x_pos;             /* grid line coordinates       */
    FLOAT model_ymin;
    CHAR text[15];
    CHAR label_text[60];
    DOUBLE scale_length, grid_int;  /* y grid variables            */
    DOUBLE grid_start;
    INT status;
    INT annotation = -10000;
    DOUBLE exp;                     /* GKS text expansion factor   */
    
    /******************************************************************************/
    
    /* Set up border drawing attr. */
    lu_text_to_index ("COLOR", &bor_color, "NORMAL");
    lu_text_to_index ("COLOR", &grid_color, "GREY");
    lu_text_to_index ("COLOR", &label_color, "CYAN");
    
    /* Put a title on top- according to model type, and if Emp. label depth axis  */
    
    strcpy (label_text, "Well Tie Model");
    
    wm_gstxci (label_color);        /* Set text color to green    */
    wm_gstxfp (SIMPLEX, GSTRKP);    /* Set Font, precision         */
    wm_gschsp (char_spacing);
    
    if (pModel->model_type == PURE_EMPIRICAL)
        {
        strcpy (label_text, "Empirical Model"); /* Label the top               */
        status = mo_post_text (pModel, 0.002, 0.52, 0.009, 0, 0, 0, 0.0, 0.1, "Depth");
        if (strcmp (pModel->model_uom, "FEET") == 0)
            {
            status = mo_post_text (pModel, 0.002, 0.47, 0.008, 0, 0, 0, 0.0, 0.1,
                                   "(Feet)");
            }
        else
            {
            status = mo_post_text (pModel, 0.002, 0.47, 0.008, 0, 0, 0, 0.0, 0.1,
                                   "(Mtrs)");
            }
        }
    status = mo_post_text (pModel, 0.35, 0.96, 0.025, 0, 0, 0, 0.0, 0.1, label_text);
    
    /* Set up text crap            */
    wm_gstxci (bor_color);          /* Set text color to white     */
    chh = (pModel->log_start - pModel->graphics.world.ymin) * 0.015;
    char_height = chh;
    char_width = (pModel->well.xmax - pModel->well.xmin) * 0.005;
    
    sprintf (text, "  %d", annotation); /* Calculate expansion factor  */
    char_xmin = pModel->well.xmin;
    char_xmax = pModel->well.xmin + 8 * char_width;
    
    exp = 1.0;
    wm_gschxp (exp);
    
    tg_fit_string (text, char_xmin, 0.0, char_xmax, 0, chh, (DOUBLE *) & char_height);
    wm_gschh (char_height);
    
    /* Set up GKS line drawing attributes                                         */
    
    wm_gsplci (bor_color);
    wm_gsln (1);
    wm_gslwsc (bor_line_width);
    
    /* check ymin                  */
    if (pModel->model_type == PURE_EMPIRICAL)
        {
        model_ymin = pModel->nl_user_struct.model_ymin;
        }
    else                            /* if Well Tie we use WCS ymin */
        {                           /* as the model bottom         */
        model_ymin = pModel->graphics.world.ymin;
        }
    /* Draw the border             */
    
    xdraw_buf[0] = xdraw_buf[3] = xdraw_buf[4] = pModel->well.xmin;
    xdraw_buf[1] = xdraw_buf[2] = pModel->well.xmax;
    
    ydraw_buf[0] = ydraw_buf[1] = ydraw_buf[4] = pModel->log_start;
    ydraw_buf[2] = ydraw_buf[3] = model_ymin;
    
    wm_gpl (5, xdraw_buf, ydraw_buf);
    
    /* Set up the horizontal grid lines                                           */
    
    wm_gsplci (grid_color);
    wm_gslwsc (grid_line_width);
    
    scale_length = pModel->log_start - model_ymin;
    
    /* Decide on grid line interval */
    if (*y_grid_int == 0.0)
        {
        grid_int = 1000;
        if (scale_length <= 5000)
            grid_int = 500;
        if (scale_length <= 1000)
            grid_int = 100;
        if (scale_length <= 500)
            grid_int = 50;
        if (scale_length <= 100)
            grid_int = 10;
        *y_grid_int = grid_int;
        }
    else
        {
        grid_int = *y_grid_int;
        }
    /* Calc. no. lines, start,etc   */
    grid_start = ((INT)(model_ymin / grid_int)) * grid_int;
    
    if (model_ymin < 0)
        grid_start = grid_start - grid_int;
    xdraw_buf[0] = pModel->well.xmin;
    xdraw_buf[1] = pModel->well.xmax;
    
    y_pos = grid_start + grid_int;
    
    /* Loop to draw,annotate x lines*/
    while (y_pos <= pModel->log_start)
        {
        ydraw_buf[0] = ydraw_buf[1] = y_pos;
        
        char_ypos = ydraw_buf[0] - char_height; /* Draw Line           */
        wm_gpl (2, xdraw_buf, ydraw_buf);
        /* Annotate Depth      */
        
        sprintf (text, " %d", (INT)ydraw_buf[0]);
        char_xpos = xdraw_buf[0] - 9 * char_width;
        wm_gtx (char_xpos, char_ypos, text);    /* Left side depth annotate     */
        char_xpos = xdraw_buf[1];
        wm_gtx (char_xpos, char_ypos, text);    /* Right side depth             */
        y_pos = y_pos + grid_int;   /* increment y position         */
        }
    /* Set up the Vertical grid lines                                             */
    
    scale_length = pModel->well.xmax - pModel->well.xmin;
    
    /* If we have not been given a grid
       interval, calculate one             */
    if (*x_grid_int == 0.0)
        {
        grid_int = 10000;
        if (scale_length <= 50000)
            grid_int = 5000;
        if (scale_length <= 10000)
            grid_int = 1000;
        if (scale_length <= 5000)
            grid_int = 500;
        if (scale_length <= 1000)
            grid_int = 100;
        if (scale_length <= 500)
            grid_int = 50;
        if (scale_length <= 100)
            grid_int = 10;
        *x_grid_int = grid_int;
        }
    else
        {
        grid_int = *x_grid_int;
        }
    /* Calc. no. lines, start,etc   */
    grid_start = ((INT)(pModel->well.xmin / grid_int)) * grid_int;
    ydraw_buf[0] = model_ymin;
    ydraw_buf[1] = pModel->log_start;
    x_pos = grid_start + grid_int;
    /* Loop to draw vertical lines  */
    /* Annotate x values on top only*/
    while (x_pos <= pModel->well.xmax)
        {
        xdraw_buf[0] = xdraw_buf[1] = x_pos;    /* grid line                    */
        wm_gpl (2, xdraw_buf, ydraw_buf);
        
        sprintf (text, " %.1f", xdraw_buf[0]);  /* text line interval annotation*/
        char_xpos = xdraw_buf[0];
        char_ypos = ydraw_buf[1] + 0.5 * char_height;
        wm_gtx (char_xpos, char_ypos, text);
        x_pos = x_pos + grid_int;
        }
    return(SUCCESS);
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    This routine is used to mark the ends of the layers with a 
    '*' symbol. This facilitates digitization of the layers.   

Prototype:
    publicdef INT mo_mark_layer_ends(STRAT_MODEL_STRUCT *pModel, NLIST_HEADER nlist);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    pModel          -(STRAT_MODEL_STRUCT *)
    nlist           -(NLIST_HEADER)

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT mo_mark_layer_ends (STRAT_MODEL_STRUCT *pModel, NLIST_HEADER nlist)
#else
publicdef INT mo_mark_layer_ends (pModel, nlist)
STRAT_MODEL_STRUCT *pModel;
NLIST_HEADER nlist;
#endif
    {
    INT i;
    UINT no_strokes, status;        /* N-list variables           */
    UINT no_points, no_points_ret;
    VOIDPTR Value_List[2];
    VOIDPTR Value_List2[2];
    UINT Dim_List[2];
    FLOAT x_gks[2], y_gks[2];       /* GKS buffers                */
    FLOAT xmin, xmax;               /* Model x-limits             */
    FLOAT marker_size = 0.7;
    FLOAT x_tol = 0.0005;           /* tolerance factor for >,<   */
    FLOAT x1, y1, x2, y2;
    INT marker_color;
    
    /*****************************************************************************/
    
    status = nl_inq_nlist_int (nlist, NL_NLIST_NSTROKES, (INT *) & no_strokes);
    
    if (no_strokes == 0)
        {
        return(SUCCESS);
        }
    /* Get Model x-limits         */
    xmin = pModel->well.xmin;
    xmax = pModel->well.xmax;
    
    lu_text_to_index ("COLOR", &marker_color, "NORMAL");    /* Set up a marker color */
    
    /* Set up N-list Stuff        */
    Dim_List[0] = 1;
    Dim_List[1] = 2;
    
    Value_List[0] = (VOIDPTR) & x1;
    Value_List[1] = (VOIDPTR) & y1;
    Value_List2[0] = (VOIDPTR) & x2;
    Value_List2[1] = (VOIDPTR) & y2;
    
    wm_gsmksc (marker_size);
    wm_gspmci (marker_color);
    /* Loop through Strokes       */
    for (i = 0; i < no_strokes; i++)
        {
        /* Get First, Last points     */
        status = nl_set_current_point (nlist, 1, i + 1);
        status = nl_get_points (nlist, 1, 2, Dim_List, Value_List, &no_points_ret);
        
        status = nl_inq_stroke_int (nlist, i + 1, NL_STROKE_NPOINTS,
                                    (INT *) & no_points);
        status = nl_set_current_point (nlist, no_points, i + 1);
        status = nl_get_points (nlist, 1, 2, Dim_List, Value_List2, &no_points_ret);
        
        /* Sort out min,max x positions        */
        if (x1 < x2)
            {
            x_gks[0] = x1;
            y_gks[0] = y1;
            x_gks[1] = x2;
            y_gks[1] = y2;
            }
        else
            {
            x_gks[0] = x2;
            y_gks[0] = y2;
            x_gks[1] = x1;
            y_gks[1] = y1;
            }
        /* Draw First point            */
        
        wm_gsmk (3);
        if (x_gks[0] > (xmin + x_tol))
            wm_gsmk (4);            /* if not edge chnge mark type*/
        
        wm_gpm (1, x_gks, y_gks);
        
        /* Draw Last Pt.              */
        wm_gsmk (3);
        if (x_gks[1] < (xmax - x_tol))
            wm_gsmk (4);
        
        wm_gpm (1, &x_gks[1], &y_gks[1]);
        }
    return(SUCCESS);
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    This routine is used to mark sedimentation type on the    
    layers where sed. points have been digitized. The type     
    is marked 1, 2, 3 according to the type set.               

Prototype:
    publicdef INT mo_mark_sed_type( STRAT_MODEL_STRUCT  *pModel, NLIST_HEADER nlist);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    pModel          -(STRAT_MODEL_STRUCT  *)
    nlist           -(NLIST_HEADER)

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT mo_mark_sed_type (STRAT_MODEL_STRUCT *pModel, NLIST_HEADER nlist)
#else
publicdef INT mo_mark_sed_type (pModel, nlist)
STRAT_MODEL_STRUCT *pModel;
NLIST_HEADER nlist;
#endif
    {
    SED_DATA_STRUCT sed_struct;
    INT i, no_strokes, status;      /* N-list variables           */
    INT no_bytes;
    INT marker_color;
    CHAR text[2];
    
    /*****************************************************************************/
    
    if (pModel->model_type == PURE_EMPIRICAL)
        {
        return(SUCCESS);
        }
    status = nl_inq_nlist_int (nlist, NL_NLIST_NSTROKES, &no_strokes);
    
    if (no_strokes == 0)
        {
        return(SUCCESS);
        }
    lu_text_to_index ("COLOR", &marker_color, "YELLOW");    /* Set up a marker color      */
    wm_gstxci (marker_color);
    
    status = mo_post_text (pModel, 0.01, 0.01, 0.009, 0, 0, 0, 0.0, 0.1, " ");
    
    /* Loop through Strokes       */
    for (i = 0; i < no_strokes; i++)
        {
        
        status = nl_inq_stroke_user_struct (nlist, i + 1, sizeof(SED_DATA_STRUCT),
                                            &sed_struct, (size_t *) & no_bytes);
        
        if (sed_struct.x != 0.0)
            {
            sprintf (text, "%1d", sed_struct.sed_type);
            wm_gtx (sed_struct.x, sed_struct.y, text);
            }
        }
    return(SUCCESS);
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    To allow the user to post text in NDC coordinates      
    with a single call.  The x,y text position, text       
    height, alignment, up vector, path are all specified.  

Prototype:
    publicdef INT mo_post_text(STRAT_MODEL_STRUCT *pModel, DOUBLE x_ndc,DOUBLE y_ndc,
        DOUBLE height_ndc,INT gks_align_h, INT gks_align_v,INT gks_path,DOUBLE gks_upx,
        DOUBLE gks_upy,CHAR text[]);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    pModel          -(STRAT_MODEL_STRUCT *)
    x_ndc           -(DOUBLE) -\
    y_ndc           -(DOUBLE) NDC coordinates of string origin.
    height_ndc      -(DOUBLE) Text height in NDC units.
    gks_align_h     -(INT) The GKS text alignment - horizontal
                        value = 0 gives right alignment.
    gks_align_v     -(INT) The GKS text alignment - vertical.
    gks_path        -(INT) GKS path vector(rel. to up vector) 
                        value = 0 gives perpendicular path.
    gks_upx         -(DOUBLE) -\
    gks_upy         -(DOUBLE) Char. up vector- ie (0,1) = vert. 
                        (1,0) gives horizontal.
    text            -(CHAR []) The text string to be posted.

Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT mo_post_text (STRAT_MODEL_STRUCT *pModel, DOUBLE x_ndc, DOUBLE y_ndc,
                            DOUBLE height_ndc, INT gks_align_h, INT gks_align_v,
                            INT gks_path, DOUBLE gks_upx, DOUBLE gks_upy, CHAR text[])
#else
publicdef INT mo_post_text (pModel, x_ndc, y_ndc, height_ndc, gks_align_h, gks_align_v,
                            gks_path, gks_upx, gks_upy, text)
STRAT_MODEL_STRUCT *pModel;
DOUBLE x_ndc, y_ndc;
DOUBLE height_ndc;
INT gks_align_h;
INT gks_align_v;
INT gks_path;
DOUBLE gks_upx, gks_upy;
CHAR text[];
#endif
    {
    FLOAT x_wcs, y_wcs;
    FLOAT height_wcs;
    FLOAT expansion;
    FLOAT scale_x, scale_y;         /* WCS scale factors            */
    FLOAT x_origin, y_origin;       /* WCS origins                  */
    FLOAT upx, upy;
    /* ************************************************************************** */
    
    /*  Calculate scale factors and set x,y origins in WCS coordinates          */
    
    scale_x = pModel->graphics.world.xmax - pModel->graphics.world.xmin;
    scale_y = pModel->graphics.world.ymax - pModel->graphics.world.ymin;
    
    x_origin = pModel->graphics.world.xmin;
    y_origin = pModel->graphics.world.ymin;
    
    /*  Convert to requested NDC text coordinates into WCS                      */
    
    x_wcs = (scale_x * x_ndc) + x_origin;
    y_wcs = (scale_y * y_ndc) + y_origin;
    
    /*  Calculate text height and expansion factor in WCS units                 */
    
    height_wcs = scale_y * height_ndc;
    
    expansion = scale_x / scale_y;
    
    /*  Set up text attributes        */
    
    wm_gschxp (expansion);          /*  Text expansion(aspect)  */
    wm_gstxal (gks_align_h, gks_align_v);   /*  Path alignment   */
    wm_gstxp (gks_path);            /*  Path direction   */
    wm_gschh (height_wcs);          /*  Character height  */
    upx = gks_upx;
    upy = gks_upy;
    wm_gschup (upx, upy);           /*  Text up vector    */
    
    /*  Post the text     */
    
    wm_gtx (x_wcs, y_wcs, text);
    
    return SUCCESS;
    
    }
/* END:     */
