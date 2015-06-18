/* DEC/CMS REPLACEMENT HISTORY, Element CO_MAKE_CON.C*/
/*  16   16-JAN-1992 12:03:02 JTM "(SPR 0)  Contour Annotation code from Don V."*/
/* *15   18-SEP-1991 16:17:24 JULIAN "(SPR 4152) removed the line where it always set the contour interval=1"*/
/* *14   30-JUL-1991 09:13:29 MING "(SPR 0) revert the last change"*/
/* *13   29-JUL-1991 13:40:17 MING "(SPR 8546) assign default value for funit"*/
/* *12   15-JUL-1991 11:20:47 MING "(SPR 0) remove check on coord system"*/
/* *11   11-MAR-1991 10:11:15 KEE "(SPR -1) Modify Charlie Purmoy's Trend and Residual base on QA new requirements"*/
/* *10   20-NOV-1990 09:23:04 MING "(SPR 6173) close the .RPT file"*/
/* *9    28-SEP-1990 10:02:26 GILLESPIE "(SPR 1) Merge beta deltas"*/
/*  5B1  24-SEP-1990 15:28:15 GILLESPIE "Beta Deltas"*/
/* *8    19-SEP-1990 09:19:38 CHARLIE "(SPR -1) override 75degree angle limit for contour labels"*/
/* *7    15-SEP-1990 13:12:51 CHARLIE "(SPR -1) enhanced perspective options"*/
/* *6    14-AUG-1990 11:43:17 VINCE "(SPR 5644) Header Standardization"*/
/* *5    25-APR-1990 08:41:00 JULIAN "(SPR 0) remove line_color option from co_overlay"*/
/* *4     6-FEB-1990 10:20:55 MING "(SPR 0) change date/time declaration to data type DATE_TIME"*/
/* *3    17-OCT-1989 16:56:10 JULIAN "(SPR 0) mod param passing on nl_inq_nlist_user_struct"*/
/* *2    28-SEP-1989 21:33:17 GILLESPIE "(SPR 1) Check for grids where min == max"*/
/* *1    19-JUN-1989 12:43:16 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element CO_MAKE_CON.C*/
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

#include "esi_c_lib.h"
#include "esi_co.h"

#ifndef ESI_CO_ERR_H
#include "esi_co_err.h"
#endif

#ifndef ESI_CO_MSG_H
#include "esi_co_msg.h"
#endif

#ifndef ESI_OV_ERR_H
#include "esi_ov_err.h"
#endif

#ifndef ESI_MP_H
#include "esi_mp.h"

#endif

#ifndef ESI_FM_H
#include "esi_fm.h"
#endif

#ifndef ESI_FM_ERR_H
#include "esi_fm_err.h"
#endif

#ifndef ESI_NL_H
#include "esi_nl.h"
#endif

#ifndef ESI_TS_H
#include "esi_ts.h"
#endif

#ifndef ESI_AM_H
#include "esi_am.h"
#endif

#ifndef ESI_MG_H
#include "esi_mg.h"
#endif

#ifndef ESI_QS_H
#include "esi_qs.h"
#endif

#ifndef ESI_DG_DEFS_H
#include "esi_dg_defs.h"
#endif

#ifndef ESI_HO_H
#include "esi_ho.h"
#endif

#define MAP_TYPE 0
#define PERSPECTIVE_TYPE 1

/* Function Description -----------------------------------------------------       */
/*Description:                                                                      */
/*    Function to make the contours for eventual posting.                           */
/*                                                                                  */
/*Prototype:                                                                        */
/*    publicdef INT co_make_contours(CONTOURS *contours);                           */
/*                                                                                  */
/*Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)        */
/*    (I) contours    -(CONTOURS *) Contour overlay information.                    */
/*                                                                                  */
/*Return Value/Status:                                                              */
/*    SUCCESS - Successful completion                                               */
/*    CO_GRID_FILE_ERROR - Error with n-list grid file.                             */
/*    CO_CANT_OPEN_CONTOUR - Error with opening contours.                           */
/*    CO_NO_GRID_DATA - No n-list grid information available.                       */
/*    OV_PROJECTION_MISMATCH - Protection mismatch with grid file.                  */
/*    CO_GRID_FILE_ERROR - Error with n-list grid file.                             */
/*    Function returns status of co_contour (+/-).                                  */
/*                                                                                  */
/*Scope:                                                                            */
/*    PUBLIC                                                                        */
/*                                                                                  */
/*Limitations/Assumptions:                                                          */
/*                                                                                  */
/*-----------------------------------------------------------------------------     */

/* BEGIN:                                                                           */
#if USE_PROTOTYPES
publicdef INT co_make_contours (CONTOURS *contours)
#else
publicdef INT co_make_contours (contours)
CONTOURS *contours;
#endif
/* * VARIABLE LIST *                                                                */
/* -----------------                                                                */
    {
    
    INT status;                     /* RETURN STATUS                                */
    FLOAT *grid_z = (FLOAT *)0;     /* GRIDDED Z VALUES                             */
    FLOAT *dummy;                   /* DUMMY FLOAT POINTER                          */
    INT nrows;                      /* NUMBER OF GRID ROWS                          */
    INT ncols;                      /* NUMBER OF GRID COLUMNS                       */
    DOUBLE znull;                   /* NULL VALUE FOR GRID                          */
    DOUBLE contour_min;
    DOUBLE contour_max;
    DOUBLE scale_factor;
    FLOAT temp;
    INT null_flag = 1;              /* FLAG FOR DGI TO RECOGNIZE NULL               */
    DOUBLE tolerance = 0.01;        /* DGI NULL TOLERANCE                           */
    DOUBLE interval;                /* CONTOUR INTERVAL IN MAP UNITS                */
    INT places;                     /* NUMBER OF DECIMAL PLACES IN LABEL            */
    DATE_TIME date;
    FM_LONG tmp_name;
    FM_PATH log_name;
    FM_TYPE fi_type;
    FILENAME os_name;               /* GRID FILE NAME                               */
    FILENAME report_file;
    INT i;                          /* DUMMY INDEX                                  */
    UINT j;                         /* DUMMY INDEX                                  */
    INT con_type;
    NLIST_HEADER contour_nlist;     /* NLIST TO STORE CONTOURS                      */
    CONTOUR_NLIST_USER_STRUCT contour_user; /* NLIST USER STRUCT FOR CONTOURS       */
    NLIST_HEADER grid_nlist = (NLIST_HEADER)0;  /* NLIST TO STORE GRID_Z            */
    GRID_NLIST_USER_STRUCT grid_user;   /* NLIST USER STRUCT FOR CONTOURS           */
    MAP_STRUCTURE *map;             /* MAP INFORMATION STRUCTURE                    */
    DG_WORKSPACE_STRUCT *dg_work;
    DOUBLE ratio;
    DOUBLE wxlen;
    DOUBLE wylen;
    INT funit;
    INT iflrpl = 1;
    static BOOL inited = FALSE;
    INT batch;
    DOUBLE size = 8.0;
    FLOAT min_value;
    FLOAT max_value;
    BOOL perm;
    FM_DESCRIB desc;
    
    /* NLIST INFORMATION                                                            */
    UINT Col_List[1];                 /*      DIM LIST                                */
    VOIDPTR Var_List[1];              /*      VALUE LIST                              */
    
    /* **********************************************************************       */
    
    status = SUCCESS;
    
    mp_get_current_map (&map);
    
    con_type = MAP_TYPE;
    if (contours->view_type != 0)
        con_type = PERSPECTIVE_TYPE;
    
    /* PRE-PROCESSING FOR GRID CONSTRUCTION *                                       */
    /* --------------------------------------                                       */
    
    strcpy (desc, "");
    if ((status = fm_init_file ("ESI$GRIDS", contours->grid_file, "GRID", "r", -1,
                                desc, NULL_FUNCTION_PTR, os_name,
                                "MAPPING")) EQUALS SUCCESS)
        {
        status = (nl_read_nlist (&grid_nlist, os_name,
                                 "GRIDS") >= 0) ? SUCCESS : CO_GRID_FILE_ERROR;
        fm_release_file ("ESI$GRIDS", contours->grid_file, "GRID");
        }
    if (status != SUCCESS)
        {
        goto finished;
        }
    nl_inq_nlist_user_struct (grid_nlist, sizeof(GRID_NLIST_USER_STRUCT),
                              (VOIDPTR)&grid_user, (size_t *)&i);
    if (ARE_DIFFERENT (grid_user.projection, map->map_projection))
        {
        status = OV_PROJECTION_MISMATCH;
        goto finished;
        }
    /* GET ROW AND COL INFORMATION                                                  */
    
    nl_inq_nlist_int (grid_nlist, NL_NLIST_NSTROKES, &nrows);
    nl_inq_stroke_int (grid_nlist, 1, NL_STROKE_NPOINTS, &ncols);
    if (nrows EQUALS 0 OR ncols EQUALS 0)
        {
        status = CO_NO_GRID_DATA;
        goto finished;
        }
    /* Make sure the grid is not a flat surface (min == max)                        */
    Col_List[0] = 1;
    Var_List[0] = (VOIDPTR)&min_value;
    nl_inq_nlist_info (grid_nlist, NL_NLIST_MIN, 1, Col_List, Var_List);
    Var_List[0] = (VOIDPTR)&max_value;
    nl_inq_nlist_info (grid_nlist, NL_NLIST_MAX, 1, Col_List, Var_List);
    if (min_value EQUALS max_value)
        {
        status = CO_NO_GRID_DATA;
        goto finished;
        }
    /* ALLOCATE AND FILL GRID                                                       */
    
    grid_z = (FLOAT *) am_allocate (AM_APPLICATION, (ncols * nrows) * sizeof(FLOAT));
    dummy = grid_z;
    for (i = 1; i < nrows + 1; i++)
        {
        nl_set_current_point (grid_nlist, 1, i);
        Var_List[0] = (VOIDPTR)dummy;
        nl_get_points (grid_nlist, ncols, 1, Col_List, Var_List, &j);
        dummy += ncols;
        }
    /* GET NULL VALUE                                                               */
    
    Var_List[0] = (VOIDPTR)&temp;
    nl_inq_nlist_info (grid_nlist, NL_NLIST_NULL_VALUE, 1, Col_List, Var_List);
    znull = temp;
    
    /* GET NLIST FILE NAME AND SET QS                                               */
    
    if (contours->contour_file EQUALS NULL)
        {
        ho_date_tag (date);
        sprintf (tmp_name, "ZZZ_CONTOUR %s", date);
        contours->contour_file = (CHAR *) am_allocate (AM_APPLICATION,
                                                       strlen (tmp_name) + 1);
        strcpy (contours->contour_file, tmp_name);
        perm = FALSE;
        }
    else
        {
        strcpy (tmp_name, contours->contour_file);
        perm = TRUE;
        }
    /* * INITIALIZE FILE MANAGEMENT *                                               */
    /* ------------------------------                                               */
    
    if (con_type == MAP_TYPE)
        {
        strcpy (log_name, "ESI$CONTOURS");
        strcpy (fi_type, "CONTOUR");
        }
    else
        {
        strcpy (log_name, "ESI$PERSPECTIVES");
        strcpy (fi_type, "PERSPECTIVE");
        }
    if ((status = fm_check_access (log_name, tmp_name, fi_type,
                                   "r")) EQUALS SUCCESS OR status EQUALS 
                  FM_NO_SUCH_FILE)
        {
        status = fm_init_file (log_name, tmp_name, fi_type, "w", perm, desc,
                               fm_contour_term_func, os_name, "MAPPING");
        }
    if (status != SUCCESS)
        {
        status = CO_CANT_OPEN_CONTOUR;
        goto finished;
        }
    co_delete_contour_nlists (os_name);
    
    /* * CONTOUR CONSTRUCTION *                                                     */
    /* ------------------------                                                     */
    
    qs_inq_i (QS_IF_BATCH, &batch);
    if (batch)
        {
        size = 40.0;
        }
    ratio = (grid_user.ymax - grid_user.ymin) / (grid_user.xmax - grid_user.xmin);
    if (ratio <= 1.0)
        {
        scale_factor = size / (grid_user.xmax - grid_user.xmin);
        wxlen = size;
        wylen = size * ratio;
        }
    else
        {
        scale_factor = size / (grid_user.ymax - grid_user.ymin);
        wylen = size;
        wxlen = size / ratio;
        }
    qs_set_f (QS_DGI_XMIN, grid_user.xmin);
    qs_set_f (QS_DGI_YMIN, grid_user.ymin);
    qs_set_f (QS_DGI_SCALE_FACTOR, scale_factor);
    qs_set_c (QS_DGI_ROOTNAME, os_name, strlen (os_name));
    
    /* START DGI CONTOURING                                                         */
    co_bgnplt_gc ();
    
    /* OPEN REPORT FILE                                                             */
    
    qs_inq_c (QS_FINDER_ACCOUNT, report_file, (INT *)0);
    sprintf (report_file, "%s_%03d", report_file, ho_user_number ());
    
    if (con_type EQUALS MAP_TYPE)
        strcat (report_file, "_CONT.RPT");
    else
        strcat (report_file, "_PERS.RPT");
    
    ho_add_path ("ESI$REPORTS", report_file, sizeof(FILENAME), report_file);
    
    co_new_file_gc (report_file, &funit, &status);
    co_setrpl_gc (iflrpl, funit);
    
    /* SET GRID CELL INFORMATION                                                    */
    
    co_setxyg_gc (ncols, 0.0, wxlen, nrows, 0.0, wylen);
    
    /* SET NULL VALUE                                                               */
    
    co_setnul_gc (null_flag, znull, tolerance);
    
    /* SET "PAGE" AND "WINDOW"                                                      */
    
    co_setpag_gc (wxlen, wylen, 0.0, 0.0);
    co_setwin_gc (wxlen, wylen, 0.0, 0.0);
    
    /* SET CONTOUR INTERVALS                                                        */
    
    if (contours->contour_interval EQUALS NULL)
        {
        co_contour_interval (contours, grid_nlist);
        }
    co_get_con_info (contours, &interval, &places);
    co_setcon_gc (0.0, interval, 1, 1, 1, places);
    
    /* SET CONTOUR MIN AND MAX                                                      */
    
    if (contours->isopach OR contours->contour_minimum !=
        NULL OR contours->contour_maximum != NULL)
        {
        co_get_contour_limits (contours, grid_nlist, &contour_min, &contour_max);
        co_setzbd_gc (1, contour_min, contour_max);
        }
    /* SET MINIMUM SEPARATION                                                       */
    
    co_setsep_gc (1, 0.01, 0.01);
    
    /* SET DOWNHILL TICK MARKS                                                      */
    /* co_settik_gc(1,1,0.04,0.1);                                                  */
    /* SET NO SMOOTHING                                                             */
    
    if (contours->suppress_smoothing)
        {
        co_setsmt_gc (1);
        }
    /* SET EDGE                                                                     */
    if (con_type EQUALS MAP_TYPE)
        {
        co_setedg_gc (1);
        }
    else
        {
        co_setedg_gc (0);
        }
    /* SET LABEL PLACEMENT PARAMETERS                                               */
    /********************************************************************************/
    /* THIS CALL CAN BE MODIFIED TO ALLOWW USER SPECIFIED PARAMETERS TO BE          */
    /* PASSED.  CURRENTLY WE USE THE DGI DEFAULTS.  TO ALLOW USER TO                */
    /* SPECIFY THEM, REQUIRES NEW KEYWORDS IN THE CONTOURS OVERLAY                  */
    
    co_setclb_gc ((3.5 / map->wcs_to_inches * scale_factor),
                  (5.0 / map->wcs_to_inches * scale_factor), 90.0, 10.0);
    
    /* SET TEXT CHARACTERISTICS                                                     */
    
    co_settxt (contours, scale_factor);
    
    if (con_type != MAP_TYPE)
        {
        co_setvue_gc (contours->view_distance, contours->view_inclination,
                      contours->view_azimuth);
        co_setprp_gc (2, 1.0, 1.0, contours->vertical_exaggeration);
        co_sethid_gc (2, 1);
        co_setslb_gc (0, 1.0);
        co_setmln_gc (1, 1.0);
        }
    /* GENERATE CONTOURS                                                            */
    
    status = co_contour (contours, grid_z, ncols, nrows);
    
    /* END DGI GRIDDING                                                             */
    
    co_endplt_gc ();
    tfg_close (&funit);
    ho_delete_file (report_file, &funit);
	
    am_release_workspace ("CONTOUR_SPACE", AM_APPLICATION);
    if (status < 0)
        {
        fm_release_file (log_name, tmp_name, fi_type);
        
        sprintf (desc, mg_message (CO_CANT_CREATE), mg_message (status));
        fm_add_history_record (log_name, tmp_name, fi_type, "", "", "", desc);
        goto finished;
        }
    /* * ADD NLIST USER STRUCT TO CONTOUR LINES NLIST *                             */
    /* ------------------------------------------------                             */
    /* READ NLIST                                                                   */
    
    status = nl_read_nlist (&contour_nlist, os_name, "LINES");
    if (status != SUCCESS)
        {
        status = CO_GRID_FILE_ERROR;
        goto finished;
        }
    /* SET NLIST USER STRUCT                                                        */
    
    strcpy (contour_user.projection, map->map_projection);
    strcpy (contour_user.coord_system, map->map_coordinate_system);
    contour_user.bold_line_interval = contours->bold_line_interval;
    contour_user.label_interval = contours->label_interval;
    contour_user.bold_line_color = contours->bold_line_color;
    contour_user.fine_line_color = contours->fine_line_color;
    contour_user.contour_interval = interval;
    strcpy (contour_user.base_surface, grid_user.base_surface);
    strcpy (contour_user.top_surface, grid_user.top_surface);
    if (contours->faulted != NULL)
        {
        strcpy (contour_user.fault, contours->faulted);
        }
    else
        {
        strcpy (contour_user.fault, "");
        }
    contour_user.uniform = TRUE;
    
    nl_set_nlist_user_struct (contour_nlist, &contour_user,
                              sizeof(CONTOUR_NLIST_USER_STRUCT));
    
    /* UPDATE NLIST                                                                 */
    
    nl_update_nlist (contour_nlist);
    nl_free_nlist (contour_nlist);
    fm_release_file (log_name, tmp_name, fi_type);
    
    sprintf (desc, mg_message (CO_CREATE_FROM_GRID), contours->grid_file);
    fm_add_history_record (log_name, tmp_name, fi_type, "ESI$GRIDS",
                           contours->grid_file, "GRID", desc);
    
    /* * FREE UP ALLOCATED MEMORY *                                                 */
    /* ----------------------------                                                 */
    
finished:
    nl_free_nlist (grid_nlist);
    am_free (grid_z);
    
    return status;
    }
/* END:                                                                             */
