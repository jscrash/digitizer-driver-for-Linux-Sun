/* DEC/CMS REPLACEMENT HISTORY, Element MO_INIT_TOOLS.C*/
/* *11   17-OCT-1990 17:56:44 GILLESPIE "(SPR 1000) Merge Ernie deltas"*/
/*  9B1  12-OCT-1990 17:12:56 GILLESPIE "Merge Ernie Deltas"*/
/* *10   17-AUG-1990 22:02:04 VINCE "(SPR 5644) Code Cleanup"*/
/* *9    17-JAN-1990 15:36:08 WALTERS "(SPR 0) Add nl_init_nlist_info to name dimensions"*/
/* *8     6-NOV-1989 11:26:30 JULIAN "(SPR 30) add mg_message"*/
/* *7    24-OCT-1989 14:37:09 JULIAN "(SPR 0) resolve compiling errors"*/
/* *6    20-SEP-1989 14:57:23 GORDON "(SPR 200) Get in sync with Gulf"*/
/* *5    19-SEP-1989 13:37:36 GILLESPIE "(SPR 100) GULF MODS"*/
/* *4    14-SEP-1989 18:22:30 GORDON "(SPR -1) checkin from gulf"*/
/* *3    14-SEP-1989 16:18:26 GORDON "(SPR 100) Gulf/Sun/Unix mods"*/
/* *2    26-JUN-1989 10:49:53 GILLESPIE "(SPR 20) Fix castings"*/
/* *1    19-JUN-1989 13:09:26 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element MO_INIT_TOOLS.C*/
/******************************************************************************     */
/*                                                                                  */
/*                Copyright Exploration Systems, Inc. 1989                          */
/*                    Unpublished -- All rights reserved                            */
/*                                                                                  */
/*THIS SOFTWARE IS THE PROPRIETARY PROPERTY OF Exploration Systems, Inc.  AND MAY   */
/*CONTAIN CONFIDENTIAL TRADE SECRET INFORMATION. IT IS LICENSED FOR USE ON THE      */
/*DESIGNATED EQUIPMENT ON WHICH IT WAS ORIGINALLY INSTALLED AND  MAY NOT BE         */
/*MODIFIED, DUPLICATED OR COPIED IN ANY FORM WITHOUT PRIOR WRITTEN CONSENT OF       */
/*                                                                                  */
/*                            Exploration Systems                                   */
/*                            c/o Finder Graphics Systems                           */
/*                            201 Tamal Vista Blvd.                                 */
/*                            Corte Madera, CA  94925                               */
/*                            (415) 927-0100                                        */
/*                                                                                  */
/*(The above notice does not constitute an admission of publication or              */
/*unrestricted dissemination of the work)                                           */
/*                                                                                  */
/******************************************************************************     */

#include "esi_mo.h"
#include "esi_fi.h"
#include "mo_strat.rh"
#include "esi_lu.h"
#include "esi_gks.h"
#include "esi_tc.h"
#include "esi_ts.h"
#include "esi_wi.h"
#include "esi_wm.h"
#include "esi_am.h"
#include "esi_gl_project.h"
#include "esi_mg.h"
#include "esi_mo_msg.h"
#ifndef ESI_XS_DEFS_H
#include "esi_xs_defs.h"
#endif

#define NullString     ""
#define resource_file  "mo_strat"
#define NL_WIDTH 2
#define VNL_WIDTH 4

/* File Description ---------------------------------------------------------       */
/*Overview:                                                                         */
/*                                                                                  */
/*Prototypes and one line descriptions of each function in the file.                */
/*Public_Functions:                                                                 */
/*    publicdef INT mo_setup(STRAT_MODEL_STRUCT    **pModel_return);                */
/*                                                                                  */
/*    publicdef INT mo_init_window(STRAT_MODEL_STRUCT *pModel);                     */
/*                                                                                  */
/*    publicdef INT mo_init_well_tie(STRAT_MODEL_STRUCT *pModel);                   */
/*                                                                                  */
/*    publicdef INT mo_init_empirical(STRAT_MODEL_STRUCT   *pModel);                */
/*                                                                                  */
/*    publicdef INT mo_get_emp_init_param(STRAT_MODEL_STRUCT   *pModel);            */
/*                                                                                  */
/*Private_Functions:                                                                */
/*                                                                                  */
/*-----------------------------------------------------------------------------     */

/* Function Description -----------------------------------------------------       */
/*Description:                                                                      */
/*    See if X-section workspace exists ,Initialize modelling                       */
/*    resources, n-lists, GKS defaults, etc.                                        */
/*                                                                                  */
/*Prototype:                                                                        */
/*    publicdef INT mo_setup(STRAT_MODEL_STRUCT    **pModel_return);                */
/*                                                                                  */
/*Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)        */
/*    pModel_return   -(STRAT_MODEL_STRUCT    **) Pointer to model struct.          */
/*                                                                                  */
/*Return Value/Status:                                                              */
/*    SUCCESS - Successful completion.                                              */
/*    FAIL    - Unsuccessful completion.                                            */
/*                                                                                  */
/*Scope:                                                                            */
/*    PUBLIC                                                                        */
/*                                                                                  */
/*Limitations/Assumptions:                                                          */
/*                                                                                  */
/*-----------------------------------------------------------------------------     */

/* BEGIN:                                                                           */
#if USE_PROTOTYPES
publicdef INT mo_setup (STRAT_MODEL_STRUCT **pModel_return)
#else
publicdef INT mo_setup (pModel_return)
STRAT_MODEL_STRUCT **pModel_return;
#endif
    {
    XS_STRUCT *pxsec;               /* ptr to x-sec. structure                      */
    STRAT_MODEL_STRUCT *pModel;
    INT yes, status;
    
    /* **************************************************************************   */
    /* set up N-list dimensions,data types for model horizon, velocity data         */
    
    static INT Units[NL_WIDTH] = 
        {
        0, 0
        };
    static INT DataTypes[NL_WIDTH] = 
        {
        FLOAT_TYPE, FLOAT_TYPE
        };
    static INT Widths[NL_WIDTH] = 
        {
        sizeof(FLOAT), sizeof(FLOAT)
        };
    static INT Incs[NL_WIDTH] = 
        {
        VALUE_LIST, VALUE_LIST
        };
    static CHAR *c_name[NL_WIDTH + 1] = 
        {
        "HORIZON", "VELOCITY", NULL
        };
    static UINT d_list[NL_WIDTH] = 
        {
        1, 2
        };
    
    static INT VUnits[VNL_WIDTH] = 
        {
        0, 0, 0, 0
        };                          /* x,y, velocity, density                       */
    static INT VDataTypes[VNL_WIDTH] = 
        {
        FLOAT_TYPE, FLOAT_TYPE, FLOAT_TYPE, FLOAT_TYPE
        };
    static INT VWidths[VNL_WIDTH] = 
        {
        sizeof(FLOAT), sizeof(FLOAT), sizeof(FLOAT), sizeof(FLOAT)
        };
    static INT VIncs[VNL_WIDTH] = 
        {
        VALUE_LIST, VALUE_LIST, VALUE_LIST, VALUE_LIST
        };
    static CHAR *c_nameV[VNL_WIDTH + 1] = 
        {
        "X", "Y", "VELOCITY", "DENSITY", NULL
        };
    static UINT d_listV[VNL_WIDTH] = 
        {
        1, 2, 3, 4
        };
    /* **************************************************************************   */
    /* if it exists, recall x-section wkspace                                       */
    
    status = am_open_workspace (XS_WORKSPACE, AM_GLOBAL, (VOIDPTR *)&pxsec);
    
    /* allocate model struct                                                        */
    
    *pModel_return = pModel = ALLOCATE (STRAT_MODEL_STRUCT);
    
    /* X section type must be structural, else will not have subsea elevation scale */
    /*   bar to establish coordinates                                               */
    
    if (status == SUCCESS AND pxsec->xs_type == XS_STRUCTURAL)
        {
        pModel->model_type = WELL_TIE;
        }
    else
        {
        /* If user did not make a struct. x section ask him                         */
        /*   if he wants to do an empirical anyway                                  */
        if (status == SUCCESS)
            {
            wiyesno (mg_message (MO_SECTION_TYPE), "X-SECTION/TYPE", &yes, &status);
            if (!yes)
                {
                return FAIL;
                }
            }
        pModel->model_type = PURE_EMPIRICAL;
        }
    /*                                                                              */
    /*   * initialize the n-lists used for the model                                */
    /*                                                                              */
    status = nl_init_nlist (&pModel->model.nlist, NL_WIDTH, Units, (size_t *)Widths,
                            DataTypes, Incs, 0);
    status = nl_set_nlist_info (pModel->model.nlist, NL_NLIST_COLUMN_NAME, NL_WIDTH,
                                d_list, (VOIDPTR)c_name);
    if (status != SUCCESS)
        {
        return status;
        }
    status = nl_init_nlist (&pModel->velocity.nlist, VNL_WIDTH, VUnits,
                            (size_t *)VWidths, VDataTypes, VIncs, 0);
    status = nl_set_nlist_info (pModel->velocity.nlist, NL_NLIST_COLUMN_NAME,
                                VNL_WIDTH, d_listV, (VOIDPTR)c_nameV);
    if (status != SUCCESS)
        {
        return status;
        }
    /*                                                                              */
    /*   * SET UP SOME GOOD GKS DEFAULTS                                            */
    /*                                                                              */
    lu_text_to_index ("COLOR", &pModel->line.color, "RED");
    lu_text_to_index ("COLOR", &pModel->hvels_color, "NORMAL");
    lu_text_to_index ("COLOR", &pModel->hvels_cur_color, "GREEN");
    pModel->line.style = 1;
    pModel->line.width = 1.0;
    
    return SUCCESS;
    
    }
/* END:                                                                             */

/* Function Description -----------------------------------------------------       */
/*Description:                                                                      */
/*    Open the window and set the the world and text trans-                         */
/*    formations.                                                                   */
/*                                                                                  */
/*Prototype:                                                                        */
/*    publicdef INT mo_init_window(STRAT_MODEL_STRUCT *pModel);                     */
/*                                                                                  */
/*Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)        */
/*    pModel          -(STRAT_MODEL_STRUCT *)                                       */
/*                                                                                  */
/*Return Value/Status:                                                              */
/*    SUCCESS - Successful completion.                                              */
/*    FAIL    - Unsuccessful completion.                                            */
/*                                                                                  */
/*Scope:                                                                            */
/*    PUBLIC                                                                        */
/*                                                                                  */
/*Limitations/Assumptions:                                                          */
/*                                                                                  */
/*-----------------------------------------------------------------------------     */

/* BEGIN:                                                                           */
#if USE_PROTOTYPES
publicdef INT mo_init_window (STRAT_MODEL_STRUCT *pModel)
#else
publicdef INT mo_init_window (pModel)
STRAT_MODEL_STRUCT *pModel;
#endif
    {
    INT status;
    DOUBLE x_int = 0.0, y_int = 0.0;
    FLOAT ndc_min_x, ndc_max_x;
    FLOAT ndc_min_y, ndc_max_y;
    INT bk_ground_color;
    LINE_ATTRIBUTE line;
    /* **************************************************************************   */
    /* If model is empirical, Set up and allocate a window since model dimensions   */
    /*   can change if recalling an old model, etc.                                 */
    
    if (pModel->model_type == PURE_EMPIRICAL)
        {
        lu_text_to_index ("COLOR", &bk_ground_color, "BLACK");
        
        status = wm_inq_wkwn (&ndc_min_x, &ndc_max_x, &ndc_min_y, &ndc_max_y);
        status = wmwallocate (&pModel->graphics.window_id, ndc_min_x, ndc_max_x,
                              ndc_min_y, ndc_max_y, bk_ground_color);
        line.style = 1;
        line.width = 1.0;
        lu_text_to_index ("COLOR", (INDEX *)&line.color, "BLACK");
        wm_draw_window_border (pModel->graphics.window_id, &line);
        }
    else
        {
        status = wmwopen (pModel->graphics.window_id);  /* Well_tie, open window    */
        status = wmsdelete (pModel->grat_segment_id);
        }
    pModel->graphics.norm_wind.xmin = 0.0;
    pModel->graphics.norm_wind.xmax = 1.0;
    pModel->graphics.norm_wind.ymin = 0.0;
    pModel->graphics.norm_wind.ymax = 1.0;
    
    /* Set up model transformation                                                  */
    
    status = wmtallocate (&pModel->graphics.transform_id,
                          pModel->graphics.norm_wind.xmin,
                          pModel->graphics.norm_wind.xmax,
                          pModel->graphics.norm_wind.ymin,
                          pModel->graphics.norm_wind.ymax, pModel->graphics.world.xmin,
                          pModel->graphics.world.xmax, pModel->graphics.world.ymin,
                          pModel->graphics.world.ymax);
    
    /* Set up the text transformation (TRATX,Y = 10.0,12.0) for hardware text       */
    
    status = wmtallocate (&pModel->graphics.transform_trat,
                          pModel->graphics.norm_wind.xmin,
                          pModel->graphics.norm_wind.xmax,
                          pModel->graphics.norm_wind.ymin,
                          pModel->graphics.norm_wind.ymax, 0.0, TRATX, 0.0, TRATY);
    
    status = wmwvis (pModel->graphics.window_id, GVISI);
    status = wmtselect (pModel->graphics.transform_id);
    
    /* If Empirical, draw the grid and store in a segment                           */
    
    if (pModel->model_type == PURE_EMPIRICAL)
        {
        status = wmsallocate (&pModel->grat_segment_id);
        status = mo_graticule (pModel, &x_int, &y_int);
        status = wmsclose (pModel->grat_segment_id);
        pModel->grid_onoff = ON;
        }
    else
        {
        pModel->grid_onoff = OFF;   /* Default grid to OFF                          */
        pModel->xs_trk_grid_onoff = GVISI;  /* Assume track grids are on            */
        }
    status = wmwclose (pModel->graphics.window_id); /* close the window             */
    wm_update ();
    
    return status;
    }
/* END:                                                                             */

/* Function Description -----------------------------------------------------       */
/*Description:                                                                      */
/*    Perform model structure initialization for a well tie                         */
/*    model- get the x-section data , initialize well link                          */
/*    lists, etc.                                                                   */
/*                                                                                  */
/*Prototype:                                                                        */
/*    publicdef INT mo_init_well_tie(STRAT_MODEL_STRUCT *pModel);                   */
/*                                                                                  */
/*Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)        */
/*    pModel          -(STRAT_MODEL_STRUCT *)                                       */
/*                                                                                  */
/*Return Value/Status:                                                              */
/*    SUCCESS - Successful completion.                                              */
/*    FAIL    - Unsuccessful completion.                                            */
/*                                                                                  */
/*Scope:                                                                            */
/*    PUBLIC                                                                        */
/*                                                                                  */
/*Limitations/Assumptions:                                                          */
/*                                                                                  */
/*-----------------------------------------------------------------------------     */

/* BEGIN:                                                                           */
#if USE_PROTOTYPES
publicdef INT mo_init_well_tie (STRAT_MODEL_STRUCT *pModel)
#else
publicdef INT mo_init_well_tie (pModel)
STRAT_MODEL_STRUCT *pModel;
#endif
    {
    LAYOUT_STRUCT *p_layout;        /* ptr to x-sec. lay. struct                    */
    XS_STRUCT *pxsec;
    PROJECT_DEFAULT *proj;          /* project default structure                    */
    CSE_WELL *pCSEWell;             /* well structures                              */
    CSE_WELL_FTN CSEuwi;
    INT status, i;
    /* **************************************************************************   */
    /* Open x-section, project default workspaces                                   */
    
    status = am_open_workspace (XS_WORKSPACE, AM_GLOBAL, (VOIDPTR *)&pxsec);
    status = am_open_workspace ("PROJECT_DEFAULTS", AM_GLOBAL, (VOIDPTR *)&proj);
    
    /* retrieve main x-sec. window, transform ids from temp. layout in x-section    */
    
    pModel->graphics.window_id = pxsec->drawing.window_id;
    
    strcpy (pModel->crossdef_name, pxsec->layout_name);
    
    /* now hit the list of wells                                                    */
    /*   * we have included two structures here - the uwi_list and well_list        */
    /*   * the uwi_list is intended for consumption by fortran programs             */
    /*   * and the well_list by C programs.  Programmers are encouraged to          */
    /*   * use the well list ...                                                    */
    /*                                                                              */
    /* get  number of wells                                                         */
    
    pModel->nwells = llcount (&pxsec->drawing.log_plots);
    
    if (pModel->nwells < 1)
        return FAIL;
    
    /* initialize link lists of wells                                               */
    
    llinit (&pModel->well_list, 0, sizeof(CSE_WELL *), LL_LIST_SYSTEM);
    llinit (&pModel->uwi_list, 0, sizeof(CSE_WELL_FTN), LL_LIST_SYSTEM);
    
    /*get WELL_STRUCTS from x-sec. layouts link lists, store in model link lists,   */
    /*   get x origins of layouts (left side ) , and min, max x's                   */
    
    for (i = 0; i < pModel->nwells; i++)
        {
        pCSEWell = ALLOCATE (CSE_WELL); /* get storage                              */
        
        status = (i == 0) ? llfirst (&pxsec->drawing.log_plots,
                                     &p_layout) : llnext (&pxsec->drawing.log_plots,
                                                          &p_layout);
        
        pCSEWell->pWell = p_layout->pwell;  /* copy WELL_STRUCT                     */
        /* get x value from layout pos.                                             */
        /* note is x pos. of middle                                                 */
        CSEuwi.x = pCSEWell->x = (p_layout->xorigin + p_layout->width / 2.0) *
            pxsec->xs_scale;
        
        if (i EQUALS 0)             /*put the leftmost (first) well                 */
            {                       /*   x  position in xmin                        */
            pModel->well.xmin = CSEuwi.x;
            }
        llappnd (&pModel->well_list, &pCSEWell);
        /* copy UWI into WELL FTN                                                   */
        strcpy (CSEuwi.uwi, pCSEWell->pWell->uwi);
        llappnd (&pModel->uwi_list, &CSEuwi);
        }
    /*put the rightmost (last)                                                      */
    pModel->well.xmax = CSEuwi.x;   /*well x position in xmax                       */
    
    /*                                                                              */
    /*   * get the x- and y-scale values, top of logs etc.                          */
    /*                                                                              */
    pModel->xscale = 1.0 / pxsec->xs_scale;
    pModel->yscale = 1.0 / pxsec->depth_scale;
    pModel->log_start = pxsec->drawing.sc_upper_limit;
    
    /* Get the project depth unit, and get the sonic log trace uom                  */
    
    strcpy (pModel->model_uom, proj->unit.vertical);
    
    strcpy (pModel->nl_user_struct.depth_uom, proj->unit.vertical);
    strcpy (pModel->nl_user_struct.trace_uom, proj->unit.vertical);
    
    /* Get the X-section wcs limits (which are in x-section units ie inches,cm)     */
    /* and convert back to whatever the actual units were                           */
    
    pModel->graphics.world.xmin = pxsec->drawing.xorigin * pxsec->xs_scale;
    pModel->graphics.world.xmax = pModel->graphics.world.xmin + pxsec->drawing.width *
        pxsec->xs_scale;
    
    pModel->graphics.world.ymin = pxsec->drawing.sc_lower_limit -
        (pxsec->drawing.sc_bar_yorigin - pxsec->drawing.yorigin) * pxsec->depth_scale;
    /* for a Well Tie model the model y min is                                      */
    /*   the same as the wcs ymin ( the emp.                                        */
    /*   model has a border at the bottom)                                          */
    
    pModel->nl_user_struct.wcs_ymin = pModel->nl_user_struct.model_ymin =
        pModel->graphics.world.ymin;
    pModel->graphics.world.ymax = pModel->graphics.world.ymin + pxsec->drawing.height *
        pxsec->depth_scale;
    
    /* Open the window and set up the transformations                               */
    
    status = mo_init_window (pModel);
    
    return status;                  /* Like, Take OFF, EH                           */
    }
/* END:                                                                             */

/* Function Description -----------------------------------------------------       */
/*Description:                                                                      */
/*    Perform model structure initialization for an empirical                       */
/*    model- either retrieve an existing point list or enter                        */
/*    the model x,y limits for a new model.  Then the graphics                      */
/*    window can be initialized.                                                    */
/*                                                                                  */
/*Prototype:                                                                        */
/*    publicdef INT mo_init_empirical(STRAT_MODEL_STRUCT   *pModel);                */
/*                                                                                  */
/*Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)        */
/*    pModel          -(STRAT_MODEL_STRUCT   *)                                     */
/*                                                                                  */
/*Return Value/Status:                                                              */
/*    SUCCESS - Successful completion.                                              */
/*    FAIL    - Unsuccessful completion.                                            */
/*                                                                                  */
/*Scope:                                                                            */
/*    PUBLIC                                                                        */
/*                                                                                  */
/*Limitations/Assumptions:                                                          */
/*                                                                                  */
/*-----------------------------------------------------------------------------     */

/* BEGIN:                                                                           */
#if USE_PROTOTYPES
publicdef INT mo_init_empirical (STRAT_MODEL_STRUCT *pModel)
#else
publicdef INT mo_init_empirical (pModel)
STRAT_MODEL_STRUCT *pModel;
#endif
    {
    INT status;
    
    /********************************************************************************/
    /* Get Model Limits so can define window, scales, etc.                          */
    
    status = mo_get_emp_init_param (pModel);
    
    if (status == FAIL)
        return FAIL;
    
    /* Open the window and set up the transformations                               */
    
    status = mo_init_window (pModel);
    
    return status;
    }
/* END:                                                                             */

/* Function Description -----------------------------------------------------       */
/*Description:                                                                      */
/*    This routine brings up a dialog and gets model boundary                       */
/*    coordinates for an empirical model from the user- ie the                      */
/*    minimum and maximum depth and x values to define the                          */
/*    model.  The UOM strings for the depth and x scales are                        */
/*    also queried.                                                                 */
/*                                                                                  */
/*Prototype:                                                                        */
/*    publicdef INT mo_get_emp_init_param(STRAT_MODEL_STRUCT   *pModel);            */
/*                                                                                  */
/*Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)        */
/*    pModel          -(STRAT_MODEL_STRUCT   *)                                     */
/*                                                                                  */
/*Return Value/Status:                                                              */
/*    SUCCESS - Successful completion.                                              */
/*    FAIL    - Unsuccessful completion.                                            */
/*                                                                                  */
/*Scope:                                                                            */
/*    PUBLIC                                                                        */
/*                                                                                  */
/*Limitations/Assumptions:                                                          */
/*                                                                                  */
/*-----------------------------------------------------------------------------     */

/* BEGIN:                                                                           */
#if USE_PROTOTYPES
publicdef INT mo_get_emp_init_param (STRAT_MODEL_STRUCT *pModel)
#else
publicdef INT mo_get_emp_init_param (pModel)
STRAT_MODEL_STRUCT *pModel;
#endif
    {
    PROJECT_DEFAULT *proj;          /* project default structure                    */
    INT status, done;
    INT id, Item;
    INT value;
    FLOAT depth_min, depth_max;
    FLOAT x_min, x_max, x_border;
    FLOAT y_border;
    CHAR title1[128];
    CHAR depth_unit[11];
    CHAR text[128];
    CHAR *cp;
    /********************************************************************************/
    
    fi_push ();                     /* save environment                             */
    /* define dialog resources                                                      */
    status = am_define_resource (AM_DIALOG, resource_file, MO_PARM_EMP,
                                 NULL_FUNCTION_PTR, &id, NULL);
    
    /* If not first call then default dialog values to existing parameters          */
    
    if (pModel->well.xmin != pModel->well.xmax)
        {
        wi_set_dialog_text (id, MO_PARM_TITLE, pModel->nl_user_struct.title);
        wi_set_dialog_text (id, MO_PARM_DEPTH_UNITS, pModel->nl_user_struct.depth_uom);
        
        sprintf (text, " %.2f", pModel->log_start);
        wi_set_dialog_text (id, MO_PARM_MIN_DEPTH, text);
        sprintf (text, " %.2f", pModel->nl_user_struct.model_ymin);
        wi_set_dialog_text (id, MO_PARM_MAX_DEPTH, text);
        
        sprintf (text, " %.2f", pModel->well.xmin);
        wi_set_dialog_text (id, MO_PARM_MIN_X, text);
        sprintf (text, " %.2f", pModel->well.xmax);
        wi_set_dialog_text (id, MO_PARM_MAX_X, text);
        }
    else
        {
        /* First call, so get project depth uom                                     */
        /*  from project default tables                                             */
        
        status = am_open_workspace ("PROJECT_DEFAULTS", AM_GLOBAL, (VOIDPTR *)&proj);
        wi_set_dialog_text (id, MO_PARM_DEPTH_UNITS, proj->unit.vertical);
        }
    /* open the dialog                                                              */
    wiopcd (id);
    
    wi_set_dialog_text (id, MO_PARM_STAT_MESSAGE, mg_message (MO_MODEL_INFO));
    
    done = FALSE;
    
    while (done == FALSE)           /* start of while loop                          */
        {
        wi_request_dialog_item (&id, &Item, &value, text);
        
        switch (Item)
            {
        case MO_PARM_BTN_OK:
            /* Get title-comment                                                    */
            wi_query_dialog_item (id, MO_PARM_TITLE, &value, text);
            if (strlen (text) > 0)
                {
                strcpy (title1, text);
                }
            /* Check Minimum Depth                                                  */
            
            wi_query_dialog_item (id, MO_PARM_MIN_DEPTH, &value, text);
            if ((ts_isfloat (text) <= 0) || (strlen (text) == 0))
                {
                wi_set_dialog_text (id, MO_PARM_STAT_MESSAGE,
                                    mg_message (MO_INVALID_DEPT));
                wi_beep (1);
                break;
                }
            else
                sscanf (text, "%f", &depth_min);
            
            /* get, check max. depth                                                */
            
            wi_query_dialog_item (id, MO_PARM_MAX_DEPTH, &value, text);
            if ((ts_isfloat (text) <= 0) || (strlen (text) == 0))
                {
                wi_set_dialog_text (id, MO_PARM_STAT_MESSAGE,
                                    mg_message (MO_INVALID_MXDEPTH));
                wi_beep (1);
                break;
                }
            else
                sscanf (text, "%f", &depth_max);
            /* Check Maximum Depth                                                  */
            /*   against Minimum Depth                                              */
            
            if (depth_max >= depth_min)
                {
                wi_set_dialog_text (id, MO_PARM_STAT_MESSAGE,
                                    mg_message (MO_MAX_LE_MIN));
                wi_beep (1);
                break;
                }
            /* Check Min. X value                                                   */
            
            wi_query_dialog_item (id, MO_PARM_MIN_X, &value, text);
            if ((ts_isfloat (text) <= 0) || (strlen (text) == 0))
                {
                wi_set_dialog_text (id, MO_PARM_STAT_MESSAGE,
                                    mg_message (MO_INVALID_MNVAL));
                wi_beep (1);
                break;
                }
            else
                sscanf (text, "%f", &x_min);
            
            /* Check Max. X value                                                   */
            
            wi_query_dialog_item (id, MO_PARM_MAX_X, &value, text);
            if ((ts_isfloat (text) <= 0) || (strlen (text) == 0))
                {
                wi_set_dialog_text (id, MO_PARM_STAT_MESSAGE,
                                    mg_message (MO_INVALID_MXVAL));
                wi_beep (1);
                break;
                }
            else
                sscanf (text, "%f", &x_max);
            
            wi_query_dialog_item (id, MO_PARM_DEPTH_UNITS, &value, text);
            cp = ts_snowhite (ts_sto_upper (text));
            strcpy (depth_unit, cp);
            
            if (ARE_DIFFERENT (depth_unit,
                               "FEET") AND ARE_DIFFERENT (depth_unit,
                                                         "METERS") AND 
                ARE_DIFFERENT (depth_unit, "METRES"))
                {
                wi_set_dialog_text (id, MO_PARM_STAT_MESSAGE,
                                    mg_message (MO_INVALID_DU));
                wi_beep (1);
                break;
                }
            done = TRUE;
            status = SUCCESS;
            break;
            
        case MO_PARM_BTN_CANCEL:
            wi_set_dialog_text (id, MO_REG_STAT_MESSAGE, NullString);
            done = TRUE;
            status = FAIL;
            break;
            
        default:
            break;
            }
        }
    if (status == SUCCESS)
        {
        /* Get the WCS coordinates, allowing space for a                            */
        /*                 border around the model                                  */
        
        x_border = (x_max - x_min) / 10.0;
        y_border = (depth_max - depth_min) / 10.0;
        
        pModel->graphics.world.xmin = x_min - x_border;
        pModel->graphics.world.xmax = x_max + x_border;
        pModel->graphics.world.ymin = depth_max + y_border;
        pModel->graphics.world.ymax = depth_min - y_border;
        
        /* Set the well xmin,xmax since this is                                     */
        /*   checked as the true model limits                                       */
        
        pModel->well.xmin = x_min;
        pModel->well.xmax = x_max;
        pModel->nl_user_struct.model_ymax = pModel->log_start = depth_min;
        pModel->nl_user_struct.model_ymin = depth_max;
        
        strcpy (pModel->nl_user_struct.title, title1);
        
        strcpy (pModel->nl_user_struct.depth_uom, depth_unit);
        strcpy (pModel->nl_user_struct.trace_uom, depth_unit);
        strcpy (pModel->model_uom, depth_unit);
        
        }
    /* Close Dialog, Bud                                                            */
    am_release_resource (id);
    
    fi_pop ();                      /* Restore environment                          */
    
    return status;                  /* Like, TAKE OFF, EH                           */
    }
/* END:                                                                             */
