/* DEC/CMS REPLACEMENT HISTORY, Element MO_DIG_TOOLS.C*/
/* *11   17-AUG-1990 22:00:54 VINCE "(SPR 5644) Code Cleanup"*/
/* *10   22-MAR-1990 01:28:06 GILLESPIE "(SPR 1) Change %f to %lf in sscanf (since we are translating doubles...)"*/
/* *9    22-JAN-1990 11:40:06 ADEY "(SPR -1) fix keymap_first to use_keymap & init_dialog"*/
/* *8     6-NOV-1989 11:26:14 JULIAN "(SPR 30) add mg_message"*/
/* *7    20-SEP-1989 14:56:29 GORDON "(SPR 200) Undo last checkin"*/
/* *6    19-SEP-1989 13:37:26 GILLESPIE "(SPR 100) GULF MODS"*/
/* *5    14-SEP-1989 18:22:00 GORDON "(SPR -1) checkin from gulf"*/
/* *4    14-SEP-1989 16:18:08 GORDON "(SPR 100) Gulf/Sun/Unix mods"*/
/* *3     9-AUG-1989 11:53:46 JULIAN "(SPR -1) fixing minor casting problem"*/
/* *2    26-JUN-1989 10:49:34 GILLESPIE "(SPR 20) Fix castings"*/
/* *1    19-JUN-1989 13:09:09 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element MO_DIG_TOOLS.C*/
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

#ifndef ESI_MO_H

#include "esi_mo.h"

#endif

#ifndef ESI_FI_H

#include "esi_fi.h"

#endif

#ifndef MO_STRAT_RH

#include "mo_strat.rh"

#endif

#ifndef ESI_C_LIB

#include "esi_c_lib.h"

#endif

#ifndef ESI_GKS_H

#include "esi_gks.h"

#endif

#ifndef ESI_LE_NLED_H

#include "esi_le_nled.h"

#endif

#ifndef ESI_LU_H

#include "esi_lu.h"

#endif

#ifndef ESI_MATH_H

#include "esi_math.h"

#endif

#ifndef ESI_TC_H

#include "esi_tc.h"

#endif

#ifndef ESI_TG_H

#include "esi_tg.h"

#endif

#ifndef ESI_TS_H

#include "esi_ts.h"

#endif

#ifndef ESI_WI_H

#include "esi_wi.h"

#endif

#ifndef ESI_WM_H

#include "esi_wm.h"

#endif

#ifndef ESI_AM_H

#include "esi_am.h"

#endif

#ifndef ESI_XS_DEFS_H

#include "esi_xs_defs.h"

#endif

#include "esi_mg.h"
#include "esi_mo_msg.h"

#define NullString     ""
#define resource_file  "mo_strat"

#if USE_PROTOTYPES
static INT mo_add_point (LE_CONTROL *pctrl);
static INT mo_change_grid (LE_CONTROL *pctrl);
static INT mo_get_grid_info (STRAT_MODEL_STRUCT *pModel, DOUBLE *x_int,
                                 DOUBLE *y_int);
static INT mo_get_sed_type (INT *sed_type);
static INT mo_line_intsect (DOUBLE s1x1, DOUBLE s1y1, DOUBLE s1x2, DOUBLE s1y2,
                                DOUBLE s2x1, DOUBLE s2y1, DOUBLE s2x2, DOUBLE s2y2,
                                DOUBLE *xint, DOUBLE *yint);
static INT mo_redraw (LE_CONTROL *pctrl);
static INT mo_set_sed_type (LE_CONTROL *pctrl);
static INT mo_snap_layers (DOUBLE seg_x1, DOUBLE seg_y1, FLOAT *seg_x2,
                               FLOAT *seg_y2, NLIST_HEADER nlist, INT no_strokes);
static INT mo_track_grid_on_off (LE_CONTROL *pctrl);
static INT mo_track_grid_vis (LE_CONTROL *pctrl);
static INT mo_zoom (LE_CONTROL *pctrl);

#else
static INT mo_add_point ();
static INT mo_change_grid ();
static INT mo_get_grid_info ();
static INT mo_get_sed_type ();
static INT mo_line_intsect ();
static INT mo_redraw ();
static INT mo_set_sed_type ();
static INT mo_snap_layers ();
static INT mo_track_grid_on_off ();
static INT mo_track_grid_vis ();
static INT mo_zoom ();

#endif

/* File Description ---------------------------------------------------------
Overview:
    Digitizing tools.
    
Prototypes and one line descriptions of each function in the file.
Public_Functions:
    publicdef INT mo_digitize_model(STRAT_MODEL_STRUCT *pModel);

Private_Functions:
    static INT mo_add_point(LE_CONTROL *pctrl);

    static INT mo_change_grid(LE_CONTROL *pctrl);

    static INT mo_get_grid_info( STRAT_MODEL_STRUCT  *pModel,DOUBLE *x_int,
                             DOUBLE *y_int);

    static INT mo_get_sed_type(INT *sed_type);

    static INT mo_line_intsect(DOUBLE s1x1,DOUBLE s1y1,DOUBLE s1x2,DOUBLE s1y2,
                           DOUBLE s2x1,DOUBLE s2y1,DOUBLE s2x2,DOUBLE s2y2,
                           DOUBLE *xint,DOUBLE *yint);

    static INT mo_redraw(LE_CONTROL *pctrl);

    static INT mo_set_sed_type(LE_CONTROL *pctrl);

    static INT mo_snap_layers(DOUBLE seg_x1,DOUBLE seg_y1, FLOAT *seg_x2,
              FLOAT *seg_y2,NLIST_HEADER nlist,INT  no_strokes);

    static INT mo_track_grid_on_off(LE_CONTROL *pctrl);

    static INT mo_track_grid_vis(LE_CONTROL *pctrl);

    static INT mo_zoom(LE_CONTROL *pctrl);

-----------------------------------------------------------------------------*/

/* Function Description -----------------------------------------------------
Description:
    Set up N-list editing for model horizon digitizing, in-  
    itialize editor structure, etc.                          
                                       
    N-list Button# (pt. map)         Function          Default Function      
    ------------------------         --------          ----------------      
            1                        Add Point                 N            
            2                        Del. Point                Y             
            3                        Add point Mono            Y             
            4                        St. New Stroke            Y            
            5                        Grid Options              N            
            6                        Set Sedimentation Type    N             
            7                        Redraw Model              N          
            8                        Track Grid Off            N             
            9                        Track Grid On             N             
            A                        Zoom in                   N             
            B                        Zoom out                  Y             
            C                        Cancel                    Y             
            D                        Return to Line mode       Y             
            E                        Set parameters?           ?             
            F                        Reset Window              Y             

Prototype:
    publicdef INT mo_digitize_model(STRAT_MODEL_STRUCT *pModel);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    pModel          -(STRAT_MODEL_STRUCT *)

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT mo_digitize_model (STRAT_MODEL_STRUCT *pModel)
#else
publicdef INT mo_digitize_model (pModel)
STRAT_MODEL_STRUCT *pModel;
#endif
    {
    /* %S   Run the line editor to digitize a model */
    /* %P   Input: Pointer to current seismic model */
    
    LE_PARLIST le_struct;           /* Line Editor parameter block */
    LE_KEYS *point_keymap;
    LE_KEYS *stroke_keymap;
    LE_KEYS *pKey;
    INT index;
    INT npts, nstrokes;
    INT status = SUCCESS;
    INT yes;
    /* ************************************************************************** */
    
    /* Define Symbolic Function Identifiers for N-list functions                  */

#define MO_ADD_POINT      101
#define MO_ZOOM           102
#define MO_REDRAW         103
#define MO_TRK_GRID_OFF   104
#define MO_TRK_GRID_ON    105
#define MO_SET_SED_TYPE   106
#define MO_GRID_OPTIONS   107
#define N_POINT_KEYMAPS  2
#define N_STROKE_KEYMAPS 1
    wmwfront (pModel->graphics.window_id);
    wmwopen (pModel->graphics.window_id);   /* open the window   */
                                            /* turn off group containing*/
    if (pModel->corr_group IS_NOT_EQUAL_TO 0)    /* top segments ( V4)       */
        {
        wmgvis (pModel->corr_group, GINVIS);
        wm_grsgwk (MAIN_WKID);      /* Refresh Screen    */
        }
    else
        {
        wmwvis (pModel->graphics.window_id, GVISI);
        }
    /* label vel if EMPIRICAL MODEL */
    
    if (pModel->model_type EQUALS PURE_EMPIRICAL)
        {
        mog_reset_text ();
        mo_draw_vels (pModel);
        }
    wmtselect (pModel->graphics.transform_id);  /* set the transform */
    
    mo_mark_layer_ends (pModel, pModel->model.nlist);
    mo_mark_sed_type (pModel, pModel->model.nlist);
    
    /* Ask the user if paper digitization is required, if so get and register the
       coordinates of model to be digitized                                       */
    
    wiyesno (mg_message (MO_DIGIT_MODEL), "MODEL/SAVE", &yes, &status);
    
    if (yes)
        {
        status = mo_register_segment (pModel);
        if (status == FAIL)
            return FAIL;
        }
    /*
       * set up the programmer-defined keys to intercept line editor functions
    */
    
    /*
     * get the default point keymap
     * and replace the specified operations
     */
    le_allocate_keymap (&stroke_keymap, LE_STROKE_MODE, N_STROKE_KEYMAPS);
    
    /*
     * change zoom stuff in every
     * stroke keymap
     */
    
    for (pKey = le_scan_keymap (stroke_keymap, N_STROKE_KEYMAPS, BUTTON_ZOOMIN);
         pKey IS_NOT_EQUAL_TO (LE_KEYS *)0;
                                                   pKey =
                                                   le_scan_keymap (stroke_keymap,
                                                                   N_STROKE_KEYMAPS,
                                                                   BUTTON_ZOOMIN))
        {
        le_set_key (stroke_keymap, pKey->index, pKey->button, MO_ZOOM, mo_zoom,
                    "Zoom Model", FALSE);
        }
    for (pKey = le_scan_keymap (stroke_keymap, N_STROKE_KEYMAPS, BUTTON_SET_PARAM);
         pKey IS_NOT_EQUAL_TO (LE_KEYS *)0;
                                                   pKey =
                                                   le_scan_keymap (stroke_keymap,
                                                                   N_STROKE_KEYMAPS,
                                                                   BUTTON_SET_PARAM))
        {
        le_set_key (stroke_keymap, pKey->index, pKey->button, MO_REDRAW, mo_redraw,
                    "Redraw Model", FALSE);
        }
    le_allocate_keymap (&point_keymap, LE_POINT_MODE, N_POINT_KEYMAPS);
    index = 0;
    
    /* get the key that is the default add
       move it to the second point keymap */
    pKey = le_scan_keymap (point_keymap, N_POINT_KEYMAPS, BUTTON_ADD_POINT);
    le_set_key (point_keymap, 1, pKey->button, MO_ADD_POINT, pKey->key_function,
                pKey->description, FALSE);
    le_set_key (point_keymap, 0, pKey->button, BUTTON_ADD_POINT, mo_add_point,
                "Add Point", FALSE);
    
    /* replace the zoom button               */
    
    pKey = le_scan_keymap (point_keymap, N_POINT_KEYMAPS, BUTTON_ZOOMIN);
    le_set_key (point_keymap, index, pKey->button, MO_ZOOM, mo_zoom, "Zoom Model",
                FALSE);
    
    /* Set button 5 to be grid options type  */
    
    le_set_key (point_keymap, index, 5, MO_GRID_OPTIONS, mo_change_grid,
                "Grid Options", FALSE);
    
    /* Set button 7 to be redraw             */
    
    le_set_key (point_keymap, index, 7, MO_REDRAW, mo_redraw, "Redraw Model", FALSE);
    
    
    /* If Well Tie model, set sedimentation,
       log track grid visibility buttons     */
    
    if (pModel->model_type == WELL_TIE)
        {
        /* Set button 8 to be sedimentation type */
        
        le_set_key (point_keymap, index, 8, MO_SET_SED_TYPE, mo_set_sed_type,
                    "Set Sed. Type", FALSE);
        
        /* Set btn 9 to turn on/off log trk grid */
        
        le_set_key (point_keymap, index, 9, MO_TRK_GRID_OFF, mo_track_grid_on_off,
                    "Track Grid On/Off", FALSE);
        
        }
    tc_zeroes (&le_struct, sizeof le_struct);
    le_struct.line = pModel->line;
    le_struct.line.style = 1;
    le_struct.erase_color = pModel->background_color;
    le_struct.hilight_color = pModel->line.color + 1;
    le_struct.data_pointer = (BYTE *)pModel;
    le_struct.initkeymap = TRUE;
    le_struct.init_dialog = FALSE;
    le_struct.use_keymap = TRUE;
    le_struct.nlist = pModel->model.nlist;
    le_struct.point_keymap = point_keymap;
    le_struct.num_point_keymaps = N_POINT_KEYMAPS;
    le_struct.stroke_keymap = stroke_keymap;
    le_struct.num_stroke_keymaps = N_STROKE_KEYMAPS;
    sprintf (le_struct.title, "Model: %s", pModel->model.id);
    
    status = le_nlist_editor (&le_struct);  /* call the n-List editor */
    
    pModel->model.nlist = le_struct.nlist;
    
    tc_free (le_struct.point_keymap);   /* free up the keymaps */
    tc_free (le_struct.stroke_keymap);  /* free up the keymaps */
    
    nl_inq_nlist_int (pModel->model.nlist, NL_NLIST_NPOINTS, (INT *) & npts);
    
    /* Do some rudimentary error checking of the layer nlist                      */
    
    if (npts < 4)
        {
        status = am_message (AM_ALERT, mg_message (MO_4_POINT));
        status = FAIL;
        }
    nl_inq_nlist_int (pModel->model.nlist, NL_NLIST_NSTROKES, &nstrokes);
    
    if (nstrokes < 2 && status != FAIL)
        {
        status = am_message (AM_ALERT, mg_message (MO_2_LAYERS));
        status = FAIL;
        }
    if (status EQUALS SUCCESS)
        {
        mo_sort_strokes (&pModel->model.nlist); /* sort all the strokes into ascending order */
        wm_gsplci (pModel->line.color);
        nl_draw_nlist (pModel->model.nlist, 1, 2);
        }
    wmwclose (pModel->graphics.window_id);  /* close the window */
    
    am_message (AM_DIALOG, NULL);
    
    return status;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    Get a digitized layer point- must be added monotonically, 
    so direction is checked. If point is outside x-limits  
    is set to x boundary value.                              

Prototype:
    static INT mo_add_point(LE_CONTROL *pctrl);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    pctrl           -(LE_CONTROL *)

Return Value/Status:
    SUCCESS - Successful completion.
    MO_ADD_POINT
    BUUTON_NOOP
    
Scope:
    PRIVATE to <mo_dig_tools.c>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static INT mo_add_point (LE_CONTROL *pctrl)
#else
static INT mo_add_point (pctrl)
LE_CONTROL *pctrl;
#endif
    {
    NLIST_HEADER nlist = pctrl->nlist;
    STRAT_MODEL_STRUCT *pModel = (STRAT_MODEL_STRUCT *)pctrl->data_pointer;
    SED_DATA_STRUCT sed_struct;
    FLOAT px, py, p2x, p2y;
    FLOAT pmx[1], pmy[1];
    static FLOAT marker_size = 0.7;
    UINT ns, np, no_strokes;
    INT direction;
    VOIDPTR Value_list[2];
    UINT Dim_list[2];
    UINT nnpts;
    BOOL edge_point = FALSE;
    /* **************************************************************************** */
    
    /*
       * check if digitized point outside model limits, if so beep and return
    */
    
    if (pctrl->wy > pModel->log_start - MINFROMTOP)
        {
        wibeep (1);
        return BUTTON_NOOP;
        }
    if (pctrl->wy < pModel->nl_user_struct.model_ymin)
        {
        wibeep (1);
        return BUTTON_NOOP;
        }
    if (pctrl->wx < pModel->well.xmin)
        {
        wibeep (1);
        pctrl->wx = pModel->well.xmin;
        edge_point = TRUE;
        }
    else if (pctrl->wx > pModel->well.xmax)
        {
        wibeep (1);
        pctrl->wx = pModel->well.xmax;
        edge_point = TRUE;
        }
    /* set GKS marker defs                */
    wm_gsmksc (marker_size);
    wm_gsmk (3);
    
    /* if first point we check if an edge point, 
       initialize the sed. struct and store it in the
       stroke user structure and return               */
    
    nl_inq_current_point (nlist, &np, &ns);
    if (np EQUALS 0)
        {
        if (edge_point == TRUE)
            {
            pmx[0] = pctrl->wx;
            pmy[0] = pctrl->wy;
            wm_gpm (1, pmx, pmy);
            }
        /* Set sed. type = normal, the x,y location
              = 0.0  in the sed structure              */
        
        sed_struct.sed_type = 1;
        sed_struct.x = 0.0;
        sed_struct.y = 0.0;
        
        /* Put sediment structure in a stroke
                                 user structure     */
        
        nl_set_stroke_user_struct (pctrl->nlist, ns, &sed_struct,
                                   sizeof(SED_DATA_STRUCT));
        return MO_ADD_POINT;
        }
    Dim_list[0] = 1;
    Dim_list[1] = 2;
    Value_list[0] = (VOIDPTR) & px;
    Value_list[1] = (VOIDPTR) & py;
    
    nl_get_points (nlist, 1, 2, Dim_list, Value_list, &nnpts);
    
    /* if only one prev. point calc. direction  */
    if (np EQUALS 1)
        direction = (px < pctrl->wx);
    else
        {
        /* more than one prev. point, so get the last
               2 pts. and set the direction         */
        Value_list[0] = (VOIDPTR) & p2x;
        Value_list[1] = (VOIDPTR) & p2y;
        
        nl_previous_point (nlist, 2, Dim_list, Value_list);
        Value_list[0] = (VOIDPTR) & px;
        Value_list[1] = (VOIDPTR) & py;
        
        nl_next_point (nlist, 2, Dim_list, Value_list);
        direction = (p2x < px);
        }
    /* if dig. in wrong direction return fail   */
    if (direction IS_NOT_EQUAL_TO (px < pctrl->wx))
        {
        wibeep (1);
        return BUTTON_NOOP;
        }
    if (px EQUALS pctrl->wx)
        {
        wibeep (1);
        return BUTTON_NOOP;
        }
    /* if on edge put symbol * at dig. point      */
    if (edge_point == TRUE)
        {
        pmx[0] = pctrl->wx;
        pmy[0] = pctrl->wy;
        wm_gpm (1, pmx, pmy);
        }
    /* snap off segment if intersects other layers*/
    
    nl_inq_nlist_int (nlist, NL_NLIST_NSTROKES, (INT *) & no_strokes);
    if (no_strokes > 1 && np > 0)
        {
        mo_snap_layers (px, py, &pctrl->wx, &pctrl->wy, nlist, no_strokes);
        }
    return MO_ADD_POINT;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    This routine is used to set the sedimentation type. The  
    user must digitize a point on the horizon he wishes to   
    set the type. Sediment types are as follows:            
                                                                              
    Type        Type Value        Method of Calculation                    
    -------    ---------------   ------------------------------------------  
    Normal             1         Logs are stretched/sqeezed to fit interval  
    Onlap              2         Logs are built from upper layer on down     
    Offlap             3         Logs are built from lower layer on up       
                                                                              
    A sedimentation structure consisting of the x,y value    
    of the digitized point and the sed. type is attached to  
    to each stroke as a stroke user structure.               

Prototype:
    static INT mo_set_sed_type(LE_CONTROL *pctrl);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    pctrl           -(LE_CONTROL *)

Return Value/Status:
    SUCCESS - Successful completion.
    BUTTON_NOOP
    
Scope:
    PRIVATE to <mo_dig_tools.c>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static INT mo_set_sed_type (LE_CONTROL *pctrl)
#else
static INT mo_set_sed_type (pctrl)
LE_CONTROL *pctrl;
#endif
    {
    NLIST_HEADER str_nlist;
    STRAT_MODEL_STRUCT *pModel = (STRAT_MODEL_STRUCT *)pctrl->data_pointer;
    SED_DATA_STRUCT sed_struct;
    INT sediment;
    UINT no_points, no_strokes;
    
    DOUBLE *Value_list[2];          /* layers nlist             */
    UINT Dim_list[2];
    
    VOIDPTR Value_list2[3];         /* returned nlist from pick */
    UINT Dim_list2[3];              /* with stroke,seg,distance */
    
    DOUBLE mins[2], maxs[2];
    UINT status;
    UINT stroke_num, segment_num;
    DOUBLE distance;
    DOUBLE double_wx, double_wy;
    DOUBLE double_xmin, double_xmax;
    DOUBLE double_ymin, double_ymax;
    
    /* ************************************************************************** */
    
    /*
       * check if digitized point inside X, Y limits of model
    */
    if (pctrl->wy > pModel->log_start - MINFROMTOP)
        {
        wibeep (1);
        return BUTTON_NOOP;
        }
    if (pctrl->wy < pModel->nl_user_struct.model_ymin)
        {
        wibeep (1);
        return BUTTON_NOOP;
        }
    if (pctrl->wx < pModel->well.xmin)
        {
        wibeep (1);
        return BUTTON_NOOP;
        }
    else if (pctrl->wx > pModel->well.xmax)
        {
        wibeep (1);
        return BUTTON_NOOP;
        }
    /* Now check n_list to make sure there is at least one stroke and if so, chk
       that the stroke has at least 2 points                                   */
    
    status = nl_inq_nlist_int (pctrl->nlist, NL_NLIST_NSTROKES, (INT *) & no_strokes);
    status = nl_inq_nlist_int (pctrl->nlist, NL_NLIST_NPOINTS, (INT *) & no_points);
    
    if (no_strokes == 0 || no_points < 2)
        {
        wibeep (1);
        return BUTTON_NOOP;
        }
    /* Find the stroke closest to the point digitized                          */
    
    double_wx = pctrl->wx;
    double_wy = pctrl->wy;
    Dim_list[0] = 1, Value_list[0] = &double_wx;
    Dim_list[1] = 2, Value_list[1] = &double_wy;
    
    double_xmin = pModel->well.xmin;
    double_xmax = pModel->well.xmax;
    double_ymin = pModel->nl_user_struct.model_ymin;
    double_ymax = pModel->log_start;
    
    mins[0] = double_xmin;
    mins[1] = double_ymin;
    maxs[0] = double_xmax;
    maxs[1] = double_ymax;
    
    status = nl_pick_stroke (pctrl->nlist, 2, Dim_list, Value_list, mins, maxs,
                             &str_nlist);
    
    /* Now we have an nlist with a list of stroke numbers, segment numbers and dist-
       ance from the digitized pt. ( ordered by distance) we need only the closest 
       point so get the first point in the nlist- this gives us the stroke number */
    
    Dim_list2[0] = 1, Value_list2[0] = (VOIDPTR) & stroke_num;
    Dim_list2[1] = 2, Value_list2[1] = (VOIDPTR) & segment_num;
    Dim_list2[2] = 3, Value_list2[2] = (VOIDPTR) & distance;
    
    status = nl_first_point (str_nlist, 3, Dim_list2, Value_list2);
    
    /* Call routine to get sediment type from user                                */
    
    status = mo_get_sed_type (&sediment);
    if (status == FAIL)
        {
        return BUTTON_NOOP;
        }
    /* Stuff the sediment type, the x,y location of the point in the sed structure*/
    
    sed_struct.sed_type = sediment;
    sed_struct.x = pctrl->wx;
    sed_struct.y = pctrl->wy;
    
    /* Now set the sediment structure in a stroke user structure (in the stroke that
       we found from the pick stroke call)                                      */
    
    status = nl_set_stroke_user_struct (pctrl->nlist, stroke_num, &sed_struct,
                                        sizeof(SED_DATA_STRUCT));
    
    status = mo_mark_sed_type (pModel, pctrl->nlist);
    
    return BUTTON_NOOP;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    This routine brings up a dialog and gets the sediment-   
    ion type from the user.                                  

Prototype:
    static INT mo_get_sed_type(INT *sed_type);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    sed_type        -(INT *)

Return Value/Status:
    Function returns status from am_release_resource and am_define_resource.
    
Scope:
    PRIVATE to <mo_dig_tools.c>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static INT mo_get_sed_type (INT *sed_type)
#else
static INT mo_get_sed_type (sed_type)
INT *sed_type;
#endif
    {
    static INT ID = 0;
    INT status;
    CHAR text[128];
    INT Item, value, done = FALSE;
    /******************************************************************************/
    
    fi_push ();
    
    *sed_type = 1;
    
    /* define dialog resources     */
    status = am_define_resource (AM_DIALOG, resource_file, MO_GET_SED_TYPE,
                                 NULL_FUNCTION_PTR, &ID, NULL);
    
    wiopcd (ID);
    
    wiscdv (ID, MO_SED_NORMAL, 1);
    
    while (done == FALSE)
        {
        wi_request_dialog_item (&ID, &Item, &value, text);
        
        switch (Item)
            {
        case MO_SED_NORMAL:
            *sed_type = 1;
            break;
            
        case MO_SED_UNCONFORM:
            *sed_type = 2;
            break;
            
        case MO_SED_ONLAP:
            *sed_type = 3;
            break;
            
        case MO_SED_OK:
            done = TRUE;
            break;
            
        case MO_SED_CANCEL:
            status = FAIL;
            done = TRUE;
            break;
            
        default:
            break;
            }
        }
    /* Release resource and restore envir.*/
    status = am_release_resource (ID);
    fi_pop ();
    
    return status;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    Change model grid - ie turn it on/off and change         
    the horizontal and vertical grid line spacing.           

Prototype:
    static INT mo_change_grid(LE_CONTROL *pctrl);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    pctrl           -(LE_CONTROL *)

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.
    BUTTON_NOOP
    
Scope:
    PRIVATE to <mo_dig_tools.c>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static INT mo_change_grid (LE_CONTROL *pctrl)
#else
static INT mo_change_grid (pctrl)
LE_CONTROL *pctrl;
#endif
    {
    static DOUBLE x_int = 0.0, y_int = 0.0; /* x,y grid line interval*/
    DOUBLE x_int1, y_int1;
    INT status;
    STRAT_MODEL_STRUCT *pModel = (STRAT_MODEL_STRUCT *)pctrl->data_pointer;
    /******************************************************************************/
    
    /* Keep track of x,y int to see if they get changed by user - if not we don't
       have to create a new graticule segment, we just turn it on                */
    
    x_int1 = x_int;
    y_int1 = y_int;
    /* Get crap from the dialog       */
    
    status = mo_get_grid_info (pModel, &x_int, &y_int);
    
    if (status == FAIL)             /* User pushed CANCEL so get out  */
        {
        return BUTTON_NOOP;
        }
    if (pModel->grid_onoff == OFF)  /* Grid has been turned off       */
        {
        if (pModel->grat_segment_id == 0)
            return BUTTON_NOOP;
        
        status = wmsvis (pModel->grat_segment_id, GINVIS);
        
        wm_grsgwk (MAIN_WKID);
        status = mo_redraw (pctrl);
        }
    else                            /* Grid turned on                */
        {
        /* If ints are same just turn on */
        if (x_int1 == x_int && y_int1 == y_int && pModel->grat_segment_id != 0)
            {
            status = wmsvis (pModel->grat_segment_id, GVISI);
            wm_grsgwk (MAIN_WKID);
            status = mo_redraw (pctrl);
            }
        else                        /* Ints have changed, or grid 
                                      segment did not exist           */
            {
            am_message (AM_DIALOG, mg_message (MO_DIGIT_GRID));
            status = wmsdelete (pModel->grat_segment_id);
            status = wmsallocate (&pModel->grat_segment_id);
            status = mo_graticule (pModel, &x_int, &y_int);
            status = wmsclose (pModel->grat_segment_id);
            wm_grsgwk (MAIN_WKID);
            status = mo_redraw (pctrl);
            am_message (AM_DIALOG, "");
            }
        }
    return BUTTON_NOOP;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    This routine brings up a dialog and gets user prefer-   
    ences for the grid that is overlayed on the digitizing   
    area- ie. on/off, x and y grid line interval             

Prototype:
    static INT mo_get_grid_info(STRAT_MODEL_STRUCT  *pModel, DOUBLE *x_int, 
        DOUBLE *y_int);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    pModel          -(STRAT_MODEL_STRUCT  *)
    x_int           -(DOUBLE *)
    y_int           -(DOUBLE *)  

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PRIVATE to <mo_dig_tools.c>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static INT mo_get_grid_info (STRAT_MODEL_STRUCT *pModel, DOUBLE *x_int,
                                 DOUBLE *y_int)
#else
static INT mo_get_grid_info (pModel, x_int, y_int)
STRAT_MODEL_STRUCT *pModel;
DOUBLE *x_int, *y_int;
#endif
    {
    static INT ID = 0;
    INT status;
    CHAR text[128];
    CHAR message[128];
    INT Item, value, done = FALSE;
    /******************************************************************************/
    
    fi_push ();
    
    /* define dialog resources     */
    status = am_define_resource (AM_DIALOG, resource_file, MO_GRID_INFO,
                                 NULL_FUNCTION_PTR, &ID, NULL);
    
    /* Set existing ints and open dialog */
    sprintf (text, "%.1f", *x_int);
    
    wi_set_dialog_text (ID, MO_GRID_X_VALUE, text);
    
    sprintf (text, "%.1f", *y_int);
    
    wi_set_dialog_text (ID, MO_GRID_Y_VALUE, text);
    
    wiopcd (ID);
    
    wiscdv (ID, MO_GRID_ON_OFF, pModel->grid_onoff);
    
    
    while (done == FALSE)
        {
        status = wi_request_dialog_item (&ID, &Item, &value, text);
        
        switch (Item)
            {                       /* Switch grid visibility flag on/off   */
        case MO_GRID_ON_OFF:
            if (pModel->grid_onoff == ON)
                {
                pModel->grid_onoff = OFF;
                status = SUCCESS;
                done = TRUE;
                }
            else
                {
                pModel->grid_onoff = ON;
                }
            break;
            
        case MO_GRID_OK:
            /* OK pushed, check x and y grid intervals     */
            
            status = wi_query_dialog_item (ID, MO_GRID_X_VALUE, &value, text);
            if (ts_isfloat (text) < 0 || (strlen (text) == 0))
                {
                sprintf (message, "Invalid X grid interval");
                wi_set_dialog_text (ID, MO_GRID_STAT_MESSAGE, message);
                wi_beep (1);
                break;
                }
            else
                sscanf (text, "%lf", x_int);
            
            status = wi_query_dialog_item (ID, MO_GRID_Y_VALUE, &value, text);
            if (ts_isfloat (text) < 0 || (strlen (text) == 0))
                {
                sprintf (message, "Invalid Y grid interval");
                wi_set_dialog_text (ID, MO_GRID_STAT_MESSAGE, message);
                wi_beep (1);
                break;
                }
            else
                sscanf (text, "%lf", y_int);
            
            done = TRUE;
            break;
            
        case MO_GRID_CANCEL:
            status = FAIL;
            done = TRUE;
            break;
            
        default:
            break;
            }
        }
    /* Release resource and restore envir.*/
    am_release_resource (ID);
    fi_pop ();
    
    return status;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    Redraw model.                                            

Prototype:
    static INT mo_redraw(LE_CONTROL *pctrl);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    pctrl           -(LE_CONTROL *)

Return Value/Status:
    BUTTON_NOOP
    
Scope:
    PRIVATE to <mo_dig_tools.c>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static INT mo_redraw (LE_CONTROL *pctrl)
#else
static INT mo_redraw (pctrl)
LE_CONTROL *pctrl;
#endif
    {
    STRAT_MODEL_STRUCT *pModel = (STRAT_MODEL_STRUCT *)pctrl->data_pointer;
    /******************************************************************************/
    
    wm_gsplci (pctrl->draw_attribute.color);
    wm_gslwsc (pctrl->draw_attribute.width);
    
    nl_draw_nlist (pctrl->nlist, 1, 2);
    if (pModel->model_type EQUALS PURE_EMPIRICAL)
        {
        mo_draw_vels (pModel);
        }
    wmtselect (pModel->graphics.transform_id);
    
    /* Annotate Layer endpoints      */
    
    mo_mark_layer_ends (pModel, pctrl->nlist);
    mo_mark_sed_type (pModel, pctrl->nlist);
    
    return BUTTON_NOOP;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    To turn on/off the track grids for each well (since the 
    screen can get cluttered with the graticule etc).       
    If the track grids are already on they will be turned    
    off and vice-versa.                                      

Prototype:
    static INT mo_track_grid_on_off(LE_CONTROL *pctrl);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    pctrl           -(LE_CONTROL *)

Return Value/Status:
    BUTTON_NOOP
    
Scope:
    PRIVATE to <mo_dig_tools.c>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static INT mo_track_grid_on_off (LE_CONTROL *pctrl)
#else
static INT mo_track_grid_on_off (pctrl)
LE_CONTROL *pctrl;
#endif
    {
    STRAT_MODEL_STRUCT *pModel = (STRAT_MODEL_STRUCT *)pctrl->data_pointer;
    
    /* ************************************************************************** */
    
    /* Set flag to opposite of present value*/
    
    if (pModel->xs_trk_grid_onoff == GVISI)
        {
        pModel->xs_trk_grid_onoff = GINVIS;
        }
    else
        {
        pModel->xs_trk_grid_onoff = GVISI;
        }
    /* Set visibility on or off             */
    
    mo_track_grid_vis (pctrl);
    
    return BUTTON_NOOP;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    To set the track grid visibility on or off.           

Prototype:
    static INT mo_track_grid_vis(LE_CONTROL *pctrl);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    pctrl           -(LE_CONTROL *)

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PRIVATE to <mo_dig_tools.c>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static INT mo_track_grid_vis (LE_CONTROL *pctrl)
#else
static INT mo_track_grid_vis (pctrl)
LE_CONTROL *pctrl;
#endif
    {
    XS_STRUCT *pxsec;
    LAYOUT_STRUCT *p_layout;
    TRACK_STRUCT *p_track;
    STRAT_MODEL_STRUCT *pModel = (STRAT_MODEL_STRUCT *)pctrl->data_pointer;
    INT status, i, j;
    INT no_layouts, no_tracks;
    
    /* ************************************************************************** */
    
    status = am_open_workspace (XS_WORKSPACE, AM_GLOBAL, (VOIDPTR *) & pxsec);
    if (status == FAIL)
        return BUTTON_NOOP;
    
    no_layouts = llcount (&pxsec->drawing.log_plots);
    
    /* Loop on x-section layouts    */
    
    for (i = 0; i < no_layouts; i++)
        {
        status = (i == 0) ? llfirst (&pxsec->drawing.log_plots,
                                     &p_layout) : llnext (&pxsec->drawing.log_plots,
                                                          &p_layout);
        /* Open the Group, dude         */
        status = wmgopen (p_layout->group_id);
        
        no_tracks = llcount (&p_layout->tracks);
        /* Loop on tracks in layout     */
        for (j = 0; j < no_tracks; j++)
            {
            status = (j == 0) ? llfirst (&p_layout->tracks,
                                         &p_track) : llnext (&p_layout->tracks,
                                                             &p_track);
            
            /* Turn on/off track grids      */
            
            status = wmsvis (p_track->seg_id, pModel->xs_trk_grid_onoff);
            }
        status = wmgclose (p_layout->group_id);
        
        }
    wm_grsgwk (MAIN_WKID);          /* Refresh the screen, bud      */
    
    status = mo_redraw (pctrl);
    
    return status;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    Perform zoom on model.                                   

Prototype:
    static INT mo_zoom(LE_CONTROL *pctrl);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    pctrl           -(LE_CONTROL *)

Return Value/Status:
    BUTTON_NOOP
    
Scope:
    PRIVATE to <mo_dig_tools.c>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static INT mo_zoom (LE_CONTROL *pctrl)
#else
static INT mo_zoom (pctrl)
LE_CONTROL *pctrl;
#endif
    {
    NLIST_HEADER nlist = pctrl->nlist;
    STRAT_MODEL_STRUCT *pModel = (STRAT_MODEL_STRUCT *)pctrl->data_pointer;
    
    am_message (AM_DIALOG, "Digitize a rectangle around the area to zoom.");
    
    wmwclose (pModel->graphics.window_id);
    wmwzoom (pModel->graphics.window_id, TRUE);
    wm_grsgwk (MAIN_WKID);
    wmwopen (pModel->graphics.window_id);
    wmtselect (pModel->graphics.transform_id);
    wm_gsplci (pModel->line.color);
    /* Redraw layers                 */
    nl_draw_nlist (nlist, 1, 2);
    
    if (pModel->model_type EQUALS PURE_EMPIRICAL)
        {
        mo_draw_vels (pModel);
        }
    wmtselect (pModel->graphics.transform_id);
    /* Mark layers endpoints         */
    mo_mark_layer_ends (pModel, nlist);
    mo_mark_sed_type (pModel, pctrl->nlist);
    wm_update ();
    
    am_message (AM_DIALOG, NullString);
    return BUTTON_NOOP;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    To check if the segment created by the 2 most recently     
    digitized points intersects a previously digitized layer.  
    If so cut off the segment at the intersection point.       
    Unfortunately the strokes are not sorted in Y so we must   
    search through all segments.                               

Prototype:
    static INT mo_snap_layers(DOUBLE seg_x1, DOUBLE seg_y1, FLOAT *seg_x2, FLOAT *seg_y2, 
        NLIST_HEADER nlist, INT no_strokes);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    seg_x1          -(DOUBLE) -\ 
    seg_y1          -(DOUBLE) Coords. of prev. pt. of segment.
    seg_x2          -(FLOAT *) -\
    seg_y2          -(FLOAT *) Coords. of curr. pt. of segment.
    nlist           -(NLIST_HEADER) Layer nlist header.
    no_strokes      -(INT) Number of layers.

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PRIVATE to <mo_dig_tools.c>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static INT mo_snap_layers (DOUBLE seg_x1, DOUBLE seg_y1, FLOAT *seg_x2,
                               FLOAT *seg_y2, NLIST_HEADER nlist, INT no_strokes)
#else
static INT mo_snap_layers (seg_x1, seg_y1, seg_x2, seg_y2, nlist, no_strokes)
DOUBLE seg_x1, seg_y1;
FLOAT *seg_x2, *seg_y2;
NLIST_HEADER nlist;
INT no_strokes;
#endif
    {
    
    DOUBLE seg_ymin, seg_ymax;      /*  ymin, ymax of segment            */
    FLOAT stroke_ymin, stroke_ymax; /*  ymin, ymax of stroke             */
    FLOAT str_seg_x1, str_seg_x2;   /*  coords. of stroke segment        */
    FLOAT str_seg_y1, str_seg_y2;
    FLOAT dummy;
    DOUBLE x_int, y_int;            /*  coords. of inter. of segments    */
    FLOAT x_gks[1], y_gks[1];       /*  GKS buffer for marker            */
    INT marker_color;
    FLOAT marker_size = 0.7;
    UINT no_points;                 /*  layer nlist info                 */
    INT i, j, status;               /*  loop variables, etc.             */
    VOIDPTR Value_List[2];          /*  nlist setup variables            */
    VOIDPTR Value_List2[2];
    UINT Dim_List[2];
    
    /******************************************************************************/
    
    lu_text_to_index ("COLOR", &marker_color, "NORMAL");
    
    wm_gsmk (5);                    /* set up marker type,color,size      */
    wm_gspmci (marker_color);
    wm_gsmksc (marker_size);
    
    Dim_List[0] = 1;
    Dim_List[1] = 2;
    
    seg_ymin = MIN (seg_y1, *seg_y2);   /* get ymins, ymaxs of segment        */
    seg_ymax = MAX (seg_y1, *seg_y2);
    
    /* Loop through layer strokes, checking ymins, ymaxs against segment          */
    
    for (i = 1; i < no_strokes; i++)
        {
        status = nl_set_current_point (nlist, 1, i);
        /* Get stroke min, max Y's           */
        Value_List[0] = (VOIDPTR) & dummy;
        Value_List[1] = (VOIDPTR) & stroke_ymin;
        status = nl_inq_stroke_info (nlist, i, NL_STROKE_MIN, 2, Dim_List, Value_List);
        
        Value_List[1] = (VOIDPTR) & stroke_ymax;
        status = nl_inq_stroke_info (nlist, i, NL_STROKE_MAX, 2, Dim_List, Value_List);
        
        /* If layer ymin-ymax are inside range of segment ymin-ymax then we must loop
           through the points of the stroke and check each segment against the digit-
           ized segment for inside limits of x and y.                                */
        
        if (!(stroke_ymin >= seg_ymax || stroke_ymax <= seg_ymin))
            {
            status = nl_inq_stroke_int (nlist, i, NL_STROKE_NPOINTS,
                                        (INT *) & no_points);
            
            status = FAIL;
            Value_List[0] = (VOIDPTR) & str_seg_x1;
            Value_List[1] = (VOIDPTR) & str_seg_y1;
            Value_List2[0] = (VOIDPTR) & str_seg_x2;
            Value_List2[1] = (VOIDPTR) & str_seg_y2;
            
            for (j = 1; j < no_points && status == FAIL; j++)
                {
                status = nl_set_current_point (nlist, j, i);
                status = nl_next_point (nlist, 2, Dim_List, Value_List2);
                status = nl_previous_point (nlist, 2, Dim_List, Value_List);
                
                /* now we calculate the segment intersections                                 */
                
                status = mo_line_intsect (str_seg_x1, str_seg_y1, str_seg_x2,
                                          str_seg_y2, seg_x1, seg_y1,
                                          (DOUBLE) * seg_x2, (DOUBLE) * seg_y2, &x_int,
                                          &y_int);
                }
            /*End of for loop         */
            
            /* if the segments did intersect, we snap of the digitized segment at the in-
               tersection point                                                           */
            
            if (status == SUCCESS)
                {
                *seg_x2 = (FLOAT)x_int; /* chop coordinates off    */
                *seg_y2 = (FLOAT)y_int;
                x_gks[0] = x_int;
                y_gks[0] = y_int;
                wm_gpm (1, x_gks, y_gks);
                }
            }
        /*End of If               */
        }
    /*End of outer for loop   */
    
    return(SUCCESS);
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    Given the x,y end coordinates of 2 line segments, calc-    
    late the intersection (if they do intersect).  If they do  
    not intersect, ie. are parallel  or intersect outside      
    the limits of either segment then  return FAIL else        
    return the  x,y coordinates of the intersection.           

Prototype:
    static INT mo_line_intsect(DOUBLE s1x1,DOUBLE s1y1,DOUBLE s1x2,DOUBLE s1y2,
        DOUBLE s2x1,DOUBLE s2y1,DOUBLE s2x2,DOUBLE s2y2,DOUBLE *xint, DOUBLE *yint );

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    s1x1            -(DOUBLE) -|
    s1y1            -(DOUBLE) -|
    s1x2            -(DOUBLE) -\
    s1y2            -(DOUBLE) Coordinates of segment 1.
    s2x1            -(DOUBLE) -|
    s2y1            -(DOUBLE) -|
    s2x2            -(DOUBLE) -\
    s2y2            -(DOUBLE) Coordinates of segment 2.
    xint            -(DOUBLE *) -\
    yint            -(DOUBLE *) Coordinates of intersection.

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PRIVATE to <mo_dig_tools.c>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static INT mo_line_intsect (DOUBLE s1x1, DOUBLE s1y1, DOUBLE s1x2, DOUBLE s1y2,
                                DOUBLE s2x1, DOUBLE s2y1, DOUBLE s2x2, DOUBLE s2y2,
                                DOUBLE *xint, DOUBLE *yint)
#else
static INT mo_line_intsect (s1x1, s1y1, s1x2, s1y2, s2x1, s2y1, s2x2, s2y2, xint, yint)
DOUBLE s1x1, s1y1, s1x2, s1y2;
DOUBLE s2x1, s2y1, s2x2, s2y2;
DOUBLE *xint, *yint;
#endif
    {
    static DOUBLE toler = 0.001;    /*  tolerance factor for compare     */
    DOUBLE slope1, slope2;          /*  slope of line segments           */
    
    /* Calculate the slope of the 2 input line segments                          */
    
    /*  trap divide by zero              */
    if ((s1x2 == s1x1) || (s2x2 == s2x1))
        return FAIL;
    
    slope1 = (s1y2 - s1y1) / (s1x2 - s1x1);
    slope2 = (s2y2 - s2y1) / (s2x2 - s2x1);
    
    if (slope1 == slope2)
        return FAIL;                /* lines are parallel        */
    
    /* Calculate x coordinate of intersection point                              */
    
    *xint = (slope1 * s1x2 - slope2 * s2x2 + s2y2 - s1y2) / (slope1 - slope2);
    
    /* Calculate y coordinate of intersection point                              */
    
    *yint = slope1 * (*xint - s1x2) + s1y2;
    
    /* Now check that the intersection pt. is inside the segment limits          */
    
    if (*xint <= (MIN (s1x1, s1x2) - toler) || *xint >= (MAX (s1x1, s1x2) + toler))
        return FAIL;
    if (*xint <= (MIN (s2x1, s2x2) - toler) || *xint >= (MAX (s2x1, s2x2) + toler))
        return FAIL;
    if (*yint <= (MIN (s1y1, s1y2) - toler) || *yint >= (MAX (s1y1, s1y2) + toler))
        return FAIL;
    if (*yint <= (MIN (s2y1, s2y2) - toler) || *yint >= (MAX (s2y1, s2y2) + toler))
        return FAIL;
    
    return(SUCCESS);
    }
/* END:     */
