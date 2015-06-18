/* DEC/CMS REPLACEMENT HISTORY, Element LG_LOG_DIG.C*/
/* *12   14-AUG-1991 16:27:14 JTM "(SPR 0) Changed thick to DOUBLE, for call to wm_gslwsc()."*/
/* *11   17-OCT-1990 17:24:52 GILLESPIE "(SPR 1000) Merge Ernie deltas"*/
/*  9B1  12-OCT-1990 17:06:21 GILLESPIE "Merge Ernie Deltas"*/
/* *10   23-AUG-1990 17:55:03 JESSIE "(SPR 1) fix include files"*/
/* *9    17-MAR-1990 13:00:33 JULIAN "(SPR 0) undo last mods"*/
/* *8    16-MAR-1990 11:22:22 JULIAN "(SPR 5135) add nl_strip_points"*/
/* *7    12-FEB-1990 08:07:46 WALTERS "(SPR 0) Change we_ routines to static"*/
/* *6    22-JAN-1990 11:37:24 ADEY "(SPR -1) fix keymap_first to use_keymap & init_dialog"*/
/* *5    17-JAN-1990 15:35:40 WALTERS "(SPR 0) Add nl_init_nlist_info to name dimensions"*/
/* *4     9-NOV-1989 15:04:02 GORDON "(SPR 5058) Use new nlist methods for resampling"*/
/* *3    14-SEP-1989 16:17:27 GORDON "(SPR 100) Gulf/Sun/Unix mods"*/
/* *2    26-JUN-1989 10:48:21 GILLESPIE "(SPR 20) Fix castings"*/
/* *1    19-JUN-1989 13:04:56 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element LG_LOG_DIG.C*/
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

#ifndef ESI_MATH_H
#include "esi_math.h"
#endif

#ifndef ESI_GKS_H
#include "esi_gks.h"
#endif

#ifndef ESI_LG_LDEFS_H
#include "esi_lg_ldefs.h"
#endif

#ifndef ESI_LE_NLED_H
#include "esi_le_nled.h"
#endif

#ifndef ESI_LG_DIG_H
#include "esi_lg_dig.h"
#endif

#ifndef ESI_AM_H
#include "esi_am.h"
#endif

#ifndef ESI_WI_H
#include "esi_wi.h"
#endif

#ifndef ESI_WM_H
#include "esi_wm.h"
#endif

#ifndef ESI_NL_H
#include "esi_nl.h"
#endif

#include "esi_nl_meth_hdr.h"
#include "esi_nl_methods.h"
#include "we_vfy_wl.rh"

#ifndef ESI_WE_VFDEF_H
#include "esi_we_vfdef.h"
#endif

#ifndef ESI_TS_H
#include "esi_ts.h"
#endif

#ifndef ESI_TC_H
#include "esi_tc.h"
#endif

#ifndef ESI_TU_H
#include "esi_tu.h"
#endif

#ifndef ESI_MR_H
#include "esi_mr.h"
#endif

#ifndef ESI_LU_H
#include "esi_lu.h"
#endif

#ifndef ESI_LG_ERR_H
#include "esi_lg_err.h"
#endif

#ifndef ESI_LG_MSG_H
#include "esi_lg_msg.h"
#endif

#ifndef ESI_MG_H
#include "esi_mg.h"
#endif

#include "esi_gl_project.h"
#include "lg_dig_log.rh"

#define FILTERLEN 29
#define MAX_ERRORS 0
#define NUMBER_OF_POINTS 3
#define ONE 1
#define EXP10(x)  exp((x)*log(10.))
#define TOLER(x,y) (fabs((x)-(y)) <= 0.0001)
#define IS_INT(x) (TOLER((x), floor((x)+0.00001)))
#define ResourceFile            "LG_DIG_LOG"
#define NullString              ""
#define ENGLISH                 "ENGLISH"
#define METRIC                  "METRIC"
#define LINEAR                  "LINEAR"
#define METERS                  "METERS"
#define FEET                    "FEET"
#define LOG                     "LOG"
#define DEPTH                   "DEPTH"
#define TIME                    "TIME"
#define TIME_FORMAT             "%.3f"
#define DEPTH_FORMAT            "%.1f"
#define SCALE_FORMAT            "%.3f"
#define LG_CANCEL   (INT) 2
#define EXIT 3
#define LOG_HEADER_STEP 1
#define LOG_PARM_STEP 2
#define LOG_REGISTER_STEP 3
#define LOG_DIGITIZE_STEP 4
#define END_OF_TRACE 1
#define END_OF_SEGMENT 2
#define CANCEL_TRACE 3
#define MTRS_TO_FEET 3.280833333

/*  Define Puck Buttons                                                             */

#define BUTTON_NEW_POINT 101
#define BUTTON_OFFSCALE_UP 102
#define BUTTON_OFFSCALE_DOWN 103
#define BUTTON_NO_DATA_ZONE 105
#define BUTTON_END_SEGMENT 106
#define BUTTON_CANCEL_TRACE 112
#define BUTTON_TRACE_DONE 113

/* Define global variables                                                          */

static UINT col_list[2] = 
    {
    1, 2
    };


static UINT ncol = 2;
static VOIDPTR c_name[2] = 
    {
    "X", "Y"
    };


static INT units[] = 
    {
    0, 0
    };

static size_t data_size[2] = 
    {
    sizeof(FLOAT), sizeof(FLOAT)
    };

static INT data_types[2] = 
    {
    FLOAT_TYPE, FLOAT_TYPE
    };

static INT value_inc[2] = 
    {
    VALUE_LIST, VALUE_LIST
    };

static VOIDPTR value_list[2];
static INT blocksize = 100;
static BOOL trace_canceled = FALSE;
static BOOL is_log_type = FALSE;

static WINDOW_ID window_id = 0;

static TNR_ID log_tnr;
static BOOL replace_mode = FALSE;   /*  Replace mode if TRUE                    */
static DOUBLE current_x, current_y; /*  Rightmost digitized value               */
static UINT current_stroke;     /*  Used for replace mode                       */
static UINT current_point;      /*  Position in nlist                           */
static DOUBLE scale_x;          /*  WCS to NDC scale factor                     */
static DOUBLE scale_y;          /*  WCS to NDC scale factor                     */
static DOUBLE x_origin;         /*  WCS origin                                  */
static DOUBLE y_origin;         /*  WCS origin                                  */
static DOUBLE x_maximum;        /*  WCS maximum                                 */
static DOUBLE y_maximum;        /*  WCS maximum                                 */
static GKS_REAL ndc_min_x, ndc_max_x, ndc_min_y, ndc_max_y;
/* Device Limits                                                                    */
static UOM_NAME log_trace_uom;  /* Area default trace uom                       */
static CHAR log_type[6];
static INT keymap_dialog = 0;   /*  Keymap dialog ID                            */
static NLIST_HEADER temp_nl;    /*  Scratch nlist                               */

/* End globals definition                                                           */

static INT lg_log_cdlg_term ();
static INT lg_get_log_header ();
static INT lg_select_log ();
static INT lg_get_log_parms ();
static INT lg_get_log_registration ();
static INT lg_digitize_control_points ();
static INT lg_digitize_trace ();
static INT lg_init_keymap ();
static INT lg_save_trace ();
static INT lg_offscale_up ();
static INT lg_offscale_down ();
static INT lg_add_point ();
static INT lg_no_data_zone ();
static INT lg_register_segment ();
static INT lg_end_segment ();
static INT lg_trace_done ();
static INT lg_cancel_trace ();
static INT lg_flip_nlist ();
static INT lg_convert_nlist ();
static INT lg_layout_display ();
static INT lg_post_text ();
static INT lg_draw_grid ();
static INT lg_thin_nlist ();
#define WE_VERIFY_WELL_CDLG_FILE "we_vfy_wl"
static INT we_verify_well_cdlg_init ();

/* File Description ---------------------------------------------------------       */
/*Overview:                                                                         */
/*    Log digitizing routines.                                                      */
/*                                                                                  */
/*Prototypes and one line descriptions of each function in the file.                */
/*Public_Functions:                                                                 */
/*    publicdef INT lg_digitize_main(BYTE *junk);                                   */
/*                                                                                  */
/*Private_Functions:                                                                */
/*    static INT we_verify_well_cdlg_init(VERIFY_STRUCT *pverify_struct);       */
/*                                                                                  */
/*    static INT we_verify_well(VERIFY_WELL_STRUCT *pverify_well_struct);       */
/*                                                                                  */
/*    static INT lg_thin_nlist(NLIST_HEADER *log_nl);                           */
/*                                                                                  */
/*    static INT lg_draw_grid(DOUBLE x_left,DOUBLE x_right,DOUBLE y_bottom,DOUBLE y_top,    */
/*                    INT grid_type,DOUBLE x_inc,INT x_bold,INT y_div,              */
/*                    DOUBLE bold_line,DOUBLE medium_line,DOUBLE fine_line,         */
/*                    INT bold_color,INT medium_color,                              */
/*                    INT fine_color,BOOL flip_it,DOUBLE flip_factor,INT num_tracks,*/
/*                    CHAR format[]);                                               */
/*                                                                                  */
/*    static INT lg_post_text(DOUBLE x_ndc,DOUBLE y_ndc,DOUBLE height_ndc,INT gks_align_h,  */
/*                            INT gks_align_v,INT gks_path,DOUBLE gks_upx,DOUBLE gks_upy,   */
/*                            CHAR text[]);                                         */
/*                                                                                  */
/*    static INT lg_layout_display(DIG_LOG_STRUCT *log_header);                 */
/*                                                                                  */
/*    static INT lg_convert_nlist(NLIST_HEADER   *log_nl, DIG_LOG_STRUCT *log_header);  */
/*                                                                                  */
/*    static INT lg_flip_nlist(DIG_LOG_STRUCT *log_header, NLIST_HEADER *log_nl);   */
/*                                                                                  */
/*    static INT lg_cancel_trace(LE_CONTROL *control);                          */
/*                                                                                  */
/*    static INT lg_trace_done(LE_CONTROL *control);                            */
/*                                                                                  */
/*    static INT lg_end_segment(LE_CONTROL *control);                           */
/*                                                                                  */
/*    static INT lg_register_segment(DIG_LOG_STRUCT *log_header);               */
/*                                                                                  */
/*    static INT lg_no_data_zone(LE_CONTROL *control);                          */
/*                                                                                  */
/*    static INT lg_add_point(LE_CONTROL *control);                             */
/*                                                                                  */
/*    static INT lg_offscale_down(LE_CONTROL *control);                         */
/*                                                                                  */
/*    static INT lg_offscale_up(LE_CONTROL *control);                           */
/*                                                                                  */
/*    static INT lg_save_trace(NLIST_HEADER log_nl, DIG_LOG_STRUCT *log_header);*/
/*                                                                                  */
/*    static INT lg_init_keymap(LE_CONTROL *control);                           */
/*                                                                                  */
/*    static INT lg_digitize_trace(NLIST_HEADER *log_nl, DIG_LOG_STRUCT *log_header);   */
/*                                                                                  */
/*    static INT lg_digitize_control_points(FLOAT xy_tablet[][2]);              */
/*                                                                                  */
/*    static INT lg_get_log_registration(FLOAT xy_cntl[NUMBER_OF_POINTS][2]);   */
/*                                                                                  */
/*    static INT lg_get_log_parms(DIG_LOG_STRUCT *log_header);                  */
/*                                                                                  */
/*    static INT lg_log_cdlg_term(INT *ID, INT *Opened);                        */
/*                                                                                  */
/*    static INT lg_select_log(DIG_LOG_STRUCT *log_header);                     */
/*                                                                                  */
/*    static INT lg_get_log_header(DIG_LOG_STRUCT *log_header);                 */
/*                                                                                  */
/*-----------------------------------------------------------------------------     */

/* Function Description -----------------------------------------------------       */
/*Description:                                                                      */
/*    This routine drives the log digitization process.                             */
/*                                                                                  */
/*Prototype:                                                                        */
/*    publicdef INT lg_digitize_main(BYTE *junk);                                   */
/*                                                                                  */
/*Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)        */
/*    (I) junk        -(BYTE *)                                                     */
/*                                                                                  */
/*Return Value/Status:                                                              */
/*    Function returns the status returned by am_quit during exit.                  */
/*                                                                                  */
/*Scope:                                                                            */
/*    PUBLIC                                                                        */
/*                                                                                  */
/*Limitations/Assumptions:                                                          */
/*                                                                                  */
/*-----------------------------------------------------------------------------     */

/* BEGIN:                                                                           */
#if USE_PROTOTYPES
publicdef INT lg_digitize_main (BYTE *junk)
#else
publicdef INT lg_digitize_main (junk)
BYTE *junk;
#endif
    {
    static DIG_LOG_STRUCT log_header = 
        {
        0
        };
    INT status;
    CHAR msg[81];
    GKS_REAL nwc_min_x, nwc_min_y, nwc_max_x, nwc_max_y;
    DOUBLE delta_y, x_length;
    DOUBLE log_top_y, log_bottom_y;
    BOOL more_traces;
    BOOL done;
    NLIST_HEADER log_nl, out_list;
    INT job_step;
    UINT dlist[1];
    PROJECT_DEFAULT *project_defaults;
    SEGMENT_ID seg_id = 0;
    LINE_ATTRIBUTE line;
    
    /********************************************************************************/
    
    am_declare ("LOG DIGITIZE");
    
    more_traces = TRUE;
    
    /*  Set system defaults                                                         */
    
    tc_zeroes (&log_header, sizeof log_header);
    /* get area depth units and sampling incr.                                      */
    /* the only oracle routine didnt need to be here. get info from workspace       */
    /*    status = lg_get_area_defaults(&log_header);                               */
    am_open_workspace ("PROJECT_DEFAULTS", AM_GLOBAL, (VOIDPTR *)&project_defaults);
    (void) strcpy (log_header.trace_struct.depth_uom, project_defaults->unit.vertical);
    log_header.trace_struct.dinc = project_defaults->log_digitizing_interval;
    
    if (strlen (log_header.trace_struct.depth_uom) == 0)
        (void) strcpy (log_header.trace_struct.depth_uom, FEET);
    (void) strcpy (log_trace_uom, log_header.trace_struct.depth_uom);
    (void) strcpy (log_header.log_units_system, ENGLISH);
    (void) strcpy (log_header.log_scale_type, LINEAR);
    (void) strcpy (log_header.trace_struct.depth_or_time, DEPTH);
    (void) strcpy (log_type, DEPTH);
    (void) strcpy (log_header.trace_struct.trace_uom, "USEC/FT");
    log_header.current_offscale_position = 0;
    
    /* lookup color codes in oracle table                                           */
    lu_text_to_index ("COLOR", &log_header.draw_color, "YELLOW");
    lu_text_to_index ("COLOR", &log_header.hilight_color, "HIGHLIGHT");
    lu_text_to_index ("COLOR", &log_header.background_color, "BACKGROUND");
    lu_text_to_index ("COLOR", &log_header.erase_color, "BACKGROUND");
    lu_text_to_index ("COLOR", &log_header.text_color, "GREEN");
    lu_text_to_index ("COLOR", &log_header.grid_color, "BLUE");
    
    /* start of digitize trace loop                                                 */
    
    while (more_traces)             /*  Loop for each trace                         */
        {
        done = FALSE;
        job_step = LOG_HEADER_STEP;
        do                          /*  Loop until trace is completed               */
            {
            switch (job_step)
                {
                
                /*  Check to make sure Log Header exists, else cannot digitize traces   */
                
            case LOG_HEADER_STEP:
                log_header.edit_flag = FALSE;
                
                status = lg_get_log_header (&log_header);
                /* cp: 8/23/88 it appears that if we_verify_well fails to be success that   */
                /*   a bad status code could be passed back here, yet ignored       */
                if (status == EXIT)
                    {
                    if (window_id != 0)
                        {
                        wmwclose (window_id);
                        wmwdelete (window_id);
                        window_id = 0;
                        }
                    return am_quit ();
                    }
                if (strlen (log_header.trace_struct.trace_name) == 0)
                    {
                    /* If no trace name entered bring up log selector dialog box    */
                    status = lg_select_log (&log_header);
                    
                    if (status == SUCCESS)  /* log selected                         */
                        {
                        log_header.edit_flag = TRUE;    /* set edit flag            */
                        log_header.log_exists = TRUE;
                        
                        log_header.trace_struct.top = 0.0;  /* reset top,base in case   */
                        log_header.trace_struct.base = 0.0; /* was a previous call  */
                        /* get the existing trace                                   */
                        status = lg_ltdm_get_trace (&log_header.trace_struct);
                        
                        if (status != SUCCESS)
                            {
                            am_message (AM_ALERT, mg_message (LG_NO_LOG_TRACE));
                            job_step = LOG_HEADER_STEP;
                            }
                        else
                            {
                            
                            job_step = LOG_PARM_STEP;
                            }
                        }
                    else            /* not selected go back to                      */
                        {           /* to header step                               */
                        job_step = LOG_HEADER_STEP;
                        }
                    }
                /* trace name entered, check if trace name okay and if trace exists */
                else
                    {
                    am_message (AM_STATUS, mg_message (LG_CHECK_LOG_TRACE));
                    
                    status = lg_ltdm_check_write (log_header.trace_struct.uwi,
                                                  log_header.trace_struct.trace_name);
                    
                    am_message (AM_STATUS, NullString);
                    
                    switch (status)
                        {
                    case SUCCESS:   /* trace does not exist                         */
                        log_header.log_exists = FALSE;
                        job_step = LOG_PARM_STEP;
                        break;
                        
                    case FAIL:      /* trace exists, retrieve and edit              */
                        log_header.log_exists = TRUE;
                        log_header.edit_flag = TRUE;
                        am_message (AM_STATUS, mg_message (LG_TRACE_EXISTS));
                        log_header.trace_struct.top = 0.0;
                        log_header.trace_struct.base = 0.0;
                        
                        status = lg_ltdm_get_trace (&log_header.trace_struct);
                        job_step = LOG_PARM_STEP;
                        break;
                        /* no version specified, assume                             */
                        /*   adding new version                                     */
                    case LG_INVALID_VERSION:
                        job_step = LOG_PARM_STEP;
                        break;
                        
                    case LG_INVALID_BLANK:
                        status = am_message (AM_ALERT, mg_message (LG_INVALID_BLANK));
                        job_step = LOG_HEADER_STEP;
                        break;
                        
                    case LG_INVALID_FORMAT:
                        status = am_message (AM_ALERT,
                                             mg_message (LG_INVALID_TRACE_FMT));
                        job_step = LOG_HEADER_STEP;
                        break;
                        }
                    }
                break;
                
                /*  Get the log digitization parameters                             */
                
            case LOG_PARM_STEP:
                log_header.flipped = FALSE;
                log_header.segment_number = 1;
                status = lg_get_log_parms (&log_header);
                if (status == LG_CANCEL)
                    {
                    job_step = LOG_HEADER_STEP;
                    break;
                    }
                else
                    {
                    job_step = LOG_REGISTER_STEP;
                    }
                /* endif                                                            */
                /* Create the window                                                */
                
                window_id = 0;
                wm_inq_wkwn (&ndc_min_x, &ndc_max_x, &ndc_min_y, &ndc_max_y);
                wmwallocate (&window_id, ndc_min_x, ndc_max_x, ndc_min_y, ndc_max_y,
                             log_header.background_color);
                line.style = 1;
                line.width = 1.0;
                lu_text_to_index ("COLOR", (INDEX *)&line.color, "BLACK");
                wm_draw_window_border (window_id, &line);
                nwc_min_x = 0.0;
                nwc_max_x = 1.0;
                nwc_min_y = 0.0;
                nwc_max_y = 1.0;
                
                /*  Force world coordinates to be quadrant one                      */
                
                if (!is_log_type)
                    /* linear log units                                             */
                    {
                    delta_y = log_header.top_y - log_header.bottom_y;
                    /* calculate y-origin and ymax accord-                          */
                    /* ing to top, bottom values entered                            */
                    if (delta_y >= 0.0)
                        {
                        y_origin = log_header.bottom_y -
                                   (log_header.num_offscales_down + 1) * delta_y -
                                   0.05 * delta_y;
                        y_maximum = log_header.top_y + (log_header.num_offscales_up +
                                                        1) * delta_y + 0.05 * delta_y;
                        }
                    else
                        {           /* top and bottom vice-versa                    */
                        y_origin = log_header.top_y + (log_header.num_offscales_down +
                                                       1) * delta_y + 0.05 * delta_y;
                        y_maximum = log_header.bottom_y -
                                    (log_header.num_offscales_up + 1) * delta_y -
                                    0.05 * delta_y;
                        log_header.flipped = TRUE;
                        log_header.trace_flip_value = log_header.bottom_y +
                            log_header.top_y;
                        }
                    }
                /* logarithmic log units                                            */
                else
                    {
                    log_top_y = log10 (log_header.top_y);
                    log_bottom_y = log10 (log_header.bottom_y);
                    delta_y = log_top_y - log_bottom_y;
                    if (delta_y >= 0.0)
                        {
                        y_origin = log_bottom_y - delta_y - 0.05 * delta_y;
                        y_maximum = log_top_y + delta_y + 0.05 * delta_y;
                        }
                    else
                        {
                        y_origin = log_top_y + delta_y + 0.05 * delta_y;
                        y_maximum = log_bottom_y - delta_y - 0.05 * delta_y;
                        }
                    }
                log_header.trace_struct.min = y_origin;
                log_header.trace_struct.max = y_maximum;
                
                /* Set up window coords., allowing a 5% border on the x-axis        */
                
                x_origin = log_header.trace_struct.top;
                x_maximum = log_header.trace_struct.base;
                x_length = x_maximum - x_origin;
                x_origin = x_origin - 0.05 * x_length;
                x_maximum = x_maximum + 0.05 * x_length;
                
                /* create the transformation                                        */
                wmtallocate (&log_tnr, nwc_min_x, nwc_max_x, nwc_min_y, nwc_max_y,
                             x_origin, x_maximum, y_origin, y_maximum);
                /* allocate the segment                                             */
                seg_id = 0;
                wmsallocate (&seg_id);
                scale_x = (x_maximum - x_origin) / (ndc_max_x - ndc_min_x);
                scale_y = (y_maximum - y_origin) / (ndc_max_y - ndc_min_y);
                
                /*  Layout the display                                              */
                
                status = lg_layout_display (&log_header);
                /*  Close the segment                                               */
                wmsclose (seg_id);
                
                /*  If the log is being edited, draw it, otherwise init the nlist   */
                
                if (log_header.edit_flag)
                    {
                    log_nl = log_header.trace_struct.ptrace;
                    
                    status = lg_flip_nlist (&log_header, &log_nl);
                    if (is_log_type)    /* logarithmic trace type                   */
                        {
                        dlist[0] = 2;
                        /* convert to log values                                    */
                        status = nl_operate_nlist_1 (log_nl, 1, dlist, NL_LOG10, 0, 0,
                                                     &out_list);
                        /* copy and draw                                            */
                        status = nl_copy_nlist (out_list, &log_nl);
                        status = nl_draw_nlist (log_nl, 1, 2);
                        }
                    }
                else                /* initialize the n-list                        */
                    {
                    status = nl_init_nlist (&log_nl, ncol, units, data_size,
                                            data_types, value_inc, blocksize);
                    nl_set_nlist_info (log_nl, NL_NLIST_COLUMN_NAME, ncol, col_list,
                                       c_name);
                    log_header.trace_struct.ptrace = log_nl;
                    }
                break;
                
                /* Get the registration parameters                                  */
                
            case LOG_REGISTER_STEP:
                
                job_step = lg_register_segment (&log_header);
                break;
                
                /* Digitize a log segment                                           */
                
            case LOG_DIGITIZE_STEP:
                
                status = lg_digitize_trace (&log_nl, &log_header);
                
                if (trace_canceled)
                    {
                    job_step = LOG_HEADER_STEP;
                    }
                else                /* Save the digitized trace                     */
                    {
                    job_step = LOG_HEADER_STEP;
                    (void) sprintf (msg, mg_message (LG_SAVING_TRACE),
                             log_header.trace_struct.trace_name);
                    am_message (AM_STATUS, msg);
                    
                    status = lg_save_trace (log_nl, &log_header);
                    if (status != SUCCESS)
                        {
                        am_message (AM_ALERT, mg_message (LG_CANNOT_SAVE_TRACE));
                        }
                    }
                /* Close the window                                                 */
                wmwclose (window_id);
                wmwdelete (window_id);
                break;
            default:
                break;
                }
            /* End of Case                                                          */
            
            } while (!done);        /* End of do...While                            */
        }
    /* End of While                                                                 */
    return am_quit ();
    }
/* END:                                                                             */

/* Function Description -----------------------------------------------------       */
/*Description:                                                                      */
/*    This routine brings up dialog and gets log header info                        */
/*    from the abuser.  The specified UWI is used to check                          */
/*    existence of the well with a WE_VERIFY_WELL call. If no                       */
/*    such well is found a message is printed.                                      */
/*                                                                                  */
/*Prototype:                                                                        */
/*    static INT lg_get_log_header(DIG_LOG_STRUCT *log_header);                 */
/*                                                                                  */
/*Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)        */
/*    log_header      -(DIG_LOG_STRUCT *)                                           */
/*                                                                                  */
/*Return Value/Status:                                                              */
/*    SUCCESS - Successful completion.                                              */
/*    EXIT                                                                          */
/*    LOG_HEADER_STEP                                                               */
/*                                                                                  */
/*Scope:                                                                            */
/*    PRIVATE to <lg_log_dig>                                                       */
/*                                                                                  */
/*Limitations/Assumptions:                                                          */
/*                                                                                  */
/*-----------------------------------------------------------------------------     */

/* BEGIN:                                                                           */
#if USE_PROTOTYPES
static INT lg_get_log_header (DIG_LOG_STRUCT *log_header)
#else
static INT lg_get_log_header (log_header)
DIG_LOG_STRUCT *log_header;
#endif
    {
    static INT Opened = FALSE;
    static INT ID = 0;
    
    INT status;
    CHAR message[81];
    CHAR text[128];
    INT Item, value, done = FALSE;
    
    VERIFY_WELL_STRUCT verify_well;
    
    /* **************************************************************************   */
    
    if (ID == 0)
        status = am_define_resource (AM_DIALOG, ResourceFile, GET_LOG_HEADER,
                                     NULL_FUNCTION_PTR, &ID, NULL);
    
    wi_set_dialog_text (ID, DIG_LOG_STAT_MESSAGE, NullString);
    wi_set_dialog_text (ID, DIG_LOG_WELL_ID, NullString);
    wi_set_dialog_text (ID, DIG_LOG_TRACE_NAME, NullString);
    wi_set_dialog_text (ID, DIG_LOG_COMMENTS, NullString);
    
    /* default uwi if exists                                                        */
    if (strlen (log_header->trace_struct.uwi) != 0)
        {
        wi_set_dialog_text (ID, DIG_LOG_WELL_ID, log_header->trace_struct.uwi);
        }
    if (Opened == FALSE)
        {
        Opened = TRUE;
        wiopcd (ID);                /* open header dialog box                       */
        }
    while (done == FALSE)
        {
        wi_request_dialog_item (&ID, &Item, &value, text);
        
        switch (Item)
            {
        case DIG_LOG_BTN_OK:
            wi_query_dialog_item (ID, DIG_LOG_WELL_ID, &value,
                                  log_header->trace_struct.uwi);
            if (strlen (log_header->trace_struct.uwi) == 0)
                {
                wi_set_dialog_text (ID, DIG_LOG_STAT_MESSAGE,
                                    mg_message (LG_ENTER_UWI));
                wi_beep (1);
                }
            else
                {
                
                /* get log trace name                                               */
                wi_query_dialog_item (ID, DIG_LOG_TRACE_NAME, &value,
                                      log_header->trace_struct.trace_name);
                
                /* get comments                                                     */
                wi_query_dialog_item (ID, DIG_LOG_COMMENTS, &value,
                                      log_header->trace_struct.comments);
                
                /* copy UWI into log_header                                         */
                (void) strcpy (verify_well.uwi, log_header->trace_struct.uwi);
                
                verify_well.show_well = FALSE;
                verify_well.well_exists = FALSE;
                
                am_message (AM_STATUS, mg_message (LG_VERIFYING_WELL));
                status = we_verify_well (&verify_well);
                am_message (AM_STATUS, NullString);
                
                if (!verify_well.well_exists || status != SUCCESS)
                    {
                    (void) sprintf (message, mg_message (LG_CANNOT_LOCATE_WELL),
                             log_header->trace_struct.uwi);
                    wi_set_dialog_text (ID, DIG_LOG_STAT_MESSAGE, message);
                    wi_beep (1);
                    }
                else
                    {
                    wi_set_dialog_text (ID, DIG_LOG_STAT_MESSAGE, NullString);
                    status = LOG_HEADER_STEP;
                    done = TRUE;
                    }
                }
            break;
            
        case DIG_LOG_BTN_EXIT:
            wi_set_dialog_text (ID, DIG_LOG_STAT_MESSAGE, NullString);
            status = EXIT;
            done = TRUE;
            break;
            
        default:
            break;
            }
        }
    (void) lg_log_cdlg_term (&ID, &Opened);
    
    return status;
    }
/* END:                                                                             */

/* Function Description -----------------------------------------------------       */
/*Description:                                                                      */
/*    This routine brings up a select list of log traces for                        */
/*    the current UWI. The user may select a log to edit or if                      */
/*    no log trace is selected he will be returned to the LOG                       */
/*    HEADER dialog to enter a trace name or exit.                                  */
/*                                                                                  */
/*Prototype:                                                                        */
/*    static INT lg_select_log(DIG_LOG_STRUCT *log_header);                     */
/*                                                                                  */
/*Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)        */
/*    log_header      -(DIG_LOG_STRUCT *)                                           */
/*                                                                                  */
/*Return Value/Status:                                                              */
/*    SUCCESS - Successful completion.                                              */
/*    FAIL    - Unsuccessful completion.                                            */
/*                                                                                  */
/*Scope:                                                                            */
/*    PRIVATE to <lg_log_dig>                                                       */
/*                                                                                  */
/*Limitations/Assumptions:                                                          */
/*                                                                                  */
/*-----------------------------------------------------------------------------     */

/* BEGIN:                                                                           */
#if USE_PROTOTYPES
static INT lg_select_log (DIG_LOG_STRUCT *log_header)
#else
static INT lg_select_log (log_header)
DIG_LOG_STRUCT *log_header;
#endif
    {
    INT status;
    INT num_items;                  /* no traces in select list                     */
    INT num_items_ret;              /* num items returned by select (1)             */
    INT Item, value;
    static INT ID = 0;
    static INT Opened = FALSE;
    INT items_selected[1];          /* array for return item                        */
    TCP table_ptr;                  /* TCP for returned list of traces              */
    CHAR text[128];                 /* dummy array for wirqst                       */
    
    /********************************************************************************/
    /* set up trace selector dialog res                                             */
    if (ID == 0)
        status = am_define_resource (AM_DIALOG, ResourceFile, LG_RETRIEVE_TRACE,/* dialog id    */
                NULL_FUNCTION_PTR,  /*no server (black pipe)                        */
                &ID, NULL);
    
    /* call LTDM for list of traces                                                 */
    status = lg_ltdm_inq_well_trace (log_header->trace_struct.uwi, NullString,/* call with null SQL */
            &table_ptr,             /* list ret. in a TCP                           */
            &num_items);            /* num traces in list                           */
    
    /* check no items in select list                                                */
    if (num_items == 0)
        {
        return(FAIL);
        }
    /* set up selector control                                                      */
    wisselt (ID, LG_RTR_SELECT_LIST, LG_RTR_SELECT_BAR, num_items, 1, table_ptr);
    
    /* set default selec. to 1st item                                               */
    wi_set_default_sel (ID, LG_RTR_SELECT_LIST, 1);
    
    /* bring up dialog box                                                          */
    if (Opened == FALSE)
        {
        Opened = TRUE;
        status = wiopcd (ID);
        }
    /* query Proceed or Cancel buttons                                              */
    wi_request_dialog_item (&ID, &Item, &value, text);
    
    switch (Item)
        {
    case LG_RTR_BTN_CANCEL:         /* Cancel button pressed, exit                  */
        status = FAIL;
        break;
        
    case LG_RTR_BTN_RETRIEVE:       /* Retrieve button pressed                      */
    /* query dialog for 1 selection                                                 */
        wiqsel (ID, LG_RTR_SELECT_LIST, &num_items_ret, items_selected);
        if (num_items_ret == 0)
            {
            status = FAIL;
            }
        else                        /* set sel. trace name in struct.               */
            {
            (void) strcpy (log_header->trace_struct.trace_name,
                    table_ptr[items_selected[0] - 1]);
            status = SUCCESS;
            }
        break;
        }
    /* end of switch construct                                                      */
    
    (void) lg_log_cdlg_term (&ID, &Opened);    /* release dialog resource                  */
    
    /* CP: 8/23/88 this may be ok if the wi routine sets up the tcp with memory     */
    /*   calls similar to ts_tcp_alloc                                              */
    
    ts_tcp_free (table_ptr);        /* free up tcp memory                           */
    return status;
    
    }
/* END:                                                                             */

/* Function Description -----------------------------------------------------       */
/*Description:                                                                      */
/*    This routine releases the dialog resources for a part-                        */
/*    icular dialog with an AM_RELEASE_RESOURCE call.                               */
/*                                                                                  */
/*Prototype:                                                                        */
/*    static INT lg_log_cdlg_term(INT *ID, INT *Opened);                        */
/*                                                                                  */
/*Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)        */
/*    ID              -(INT *)                                                      */
/*    Opened          -(INT *)                                                      */
/*                                                                                  */
/*Return Value/Status:                                                              */
/*    SUCCESS - Successful completion.                                              */
/*                                                                                  */
/*Scope:                                                                            */
/*    PRIVATE to <lg_log_dig>                                                       */
/*                                                                                  */
/*Limitations/Assumptions:                                                          */
/*                                                                                  */
/*-----------------------------------------------------------------------------     */

/* BEGIN:                                                                           */
#if USE_PROTOTYPES
static INT lg_log_cdlg_term (INT *ID, INT *Opened)
#else
static INT lg_log_cdlg_term (ID, Opened)
INT *ID, *Opened;
#endif
    {
    
    am_release_resource (*ID);
    *Opened = FALSE;
    *ID = 0;
    
    return(SUCCESS);
    }

/* END:                                                                             */

/* Function Description -----------------------------------------------------       */
/*Description:                                                                      */
/*    This routine brings up a dialog and gets log parameter                        */
/*    info from the abuser.  Requested parameters include                           */
/*    log scales, units, etc.                                                       */
/*                                                                                  */
/*Prototype:                                                                        */
/*    static INT lg_get_log_parms(DIG_LOG_STRUCT *log_header);                  */
/*                                                                                  */
/*Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)        */
/*    log_header      -(DIG_LOG_STRUCT *)                                           */
/*                                                                                  */
/*Return Value/Status:                                                              */
/*    SUCCESS - Successful completion.                                              */
/*    FAIL    - Unsuccessful completion.                                            */
/*                                                                                  */
/*Scope:                                                                            */
/*    PRIVATE to <lg_log_dig>                                                       */
/*                                                                                  */
/*Limitations/Assumptions:                                                          */
/*                                                                                  */
/*-----------------------------------------------------------------------------     */

/* BEGIN:                                                                           */
#if USE_PROTOTYPES
static INT lg_get_log_parms (DIG_LOG_STRUCT *log_header)
#else
static INT lg_get_log_parms (log_header)
DIG_LOG_STRUCT *log_header;
#endif
    {
    static INT Opened = FALSE;
    static INT ID = 0;
    
    INT status;
    CHAR text[128];
    CHAR message[81];
    CHAR format[6];
    INT Item, value, done = FALSE;
    /********************************************************************************/
    /* define dialog resources                                                      */
    if (ID == 0)
        am_define_resource (AM_DIALOG, ResourceFile, DIG_PARM_PARAMETERS,
                            NULL_FUNCTION_PTR, &ID, NULL);
    
    if (log_header->log_exists)
        {                           /* Default the log format                       */
        if (strcmp (log_header->trace_struct.depth_or_time, DEPTH) == 0)
            (void) strcpy (format, DEPTH_FORMAT);
        else
            (void) strcpy (format, TIME_FORMAT);
        
        /* Check depth units default                                                */
        if (strlen (log_header->trace_struct.depth_uom) == 0)
            (void) strcpy (log_header->trace_struct.depth_uom, log_trace_uom);
        
        /* copy non_null top,base to top,base since                                 */
        /*      ltdm_get_trace does not return these                                */
        log_header->trace_struct.top = log_header->trace_struct.non_null_top;
        log_header->trace_struct.base = log_header->trace_struct.non_null_base;
        
        wi_set_dialog_text (ID, DIG_PARM_STAT_MESSAGE, NullString);
        
        (void) sprintf (message, format, log_header->trace_struct.top);
        wi_set_dialog_text (ID, DIG_PARM_MIN_TD, message);
        
        log_header->top_y = log_header->trace_struct.max;
        log_header->bottom_y = log_header->trace_struct.min;
        
        (void) sprintf (message, format, log_header->trace_struct.base);
        wi_set_dialog_text (ID, DIG_PARM_MAX_TD, message);
        
        (void) sprintf (message, SCALE_FORMAT, log_header->top_y);
        wi_set_dialog_text (ID, DIG_PARM_TOP_SCALE, message);
        
        (void) sprintf (message, SCALE_FORMAT, log_header->bottom_y);
        wi_set_dialog_text (ID, DIG_PARM_BTM_SCALE, message);
        
        (void) sprintf (message, "%d", log_header->num_offscales_up);
        wi_set_dialog_text (ID, DIG_PARM_OFFSCALE_UP, message);
        
        (void) sprintf (message, "%d", log_header->num_offscales_down);
        wi_set_dialog_text (ID, DIG_PARM_OFFSCALE_DOWN, message);
        
        (void) sprintf (message, format, log_header->trace_struct.dinc);
        wi_set_dialog_text (ID, DIG_PARM_SAMPLE, message);
        
        wi_set_dialog_text (ID, DIG_PARM_TRACE_UNITS,
                            log_header->trace_struct.depth_uom);
        wi_set_dialog_text (ID, DIG_PARM_LOG_UNITS,
                            log_header->trace_struct.trace_uom);
        if (strcmp (log_header->log_units_system, ENGLISH) == 0)
            {
            wi_set_dialog_value (ID, DIG_PARM_RADIO_ENGLISH, ON);
            wi_set_dialog_value (ID, DIG_PARM_RADIO_METRIC, OFF);
            }
        else
            {
            wi_set_dialog_value (ID, DIG_PARM_RADIO_ENGLISH, OFF);
            wi_set_dialog_value (ID, DIG_PARM_RADIO_METRIC, ON);
            }
        if (strcmp (log_header->log_scale_type, LINEAR) == 0)
            {
            wi_set_dialog_value (ID, DIG_PARM_RADIO_LINEAR, ON);
            wi_set_dialog_value (ID, DIG_PARM_RADIO_LOG, OFF);
            is_log_type = FALSE;
            }
        else
            {
            wi_set_dialog_value (ID, DIG_PARM_RADIO_LINEAR, OFF);
            wi_set_dialog_value (ID, DIG_PARM_RADIO_LOG, ON);
            is_log_type = TRUE;
            wi_enable_dialog_item (ID, DIG_PARM_OFFSCALE_LBL, OFF);
            wi_enable_dialog_item (ID, DIG_PARM_OFFSCALE_UP, OFF);
            wi_enable_dialog_item (ID, DIG_PARM_OFFSCALE_DOWN, OFF);
            }
        if (strcmp (log_header->trace_struct.depth_or_time, DEPTH) == 0)
            {
            wi_set_dialog_value (ID, DIG_PARM_RADIO_DEPTH, ON);
            wi_set_dialog_value (ID, DIG_PARM_RADIO_TIME, OFF);
            }
        else
            {
            wi_set_dialog_value (ID, DIG_PARM_RADIO_DEPTH, OFF);
            wi_set_dialog_value (ID, DIG_PARM_RADIO_TIME, ON);
            }
        }
    else
        {
        wi_set_dialog_text (ID, DIG_PARM_STAT_MESSAGE, NullString);
        wi_set_dialog_text (ID, DIG_PARM_MIN_TD, NullString);
        wi_set_dialog_text (ID, DIG_PARM_MAX_TD, NullString);
        wi_set_dialog_text (ID, DIG_PARM_TOP_SCALE, NullString);
        wi_set_dialog_text (ID, DIG_PARM_BTM_SCALE, NullString);
        (void) sprintf (message, DEPTH_FORMAT, log_header->trace_struct.dinc);
        wi_set_dialog_text (ID, DIG_PARM_SAMPLE, message);
        wi_set_dialog_text (ID, DIG_PARM_OFFSCALE_UP, "0");
        wi_set_dialog_text (ID, DIG_PARM_OFFSCALE_DOWN, "0");
        wi_set_dialog_text (ID, DIG_PARM_TRACE_UNITS,
                            log_header->trace_struct.depth_uom);
        wi_set_dialog_text (ID, DIG_PARM_LOG_UNITS,
                            log_header->trace_struct.trace_uom);
        wi_set_dialog_value (ID, DIG_PARM_RADIO_ENGLISH, ON);
        wi_set_dialog_value (ID, DIG_PARM_RADIO_METRIC, OFF);
        wi_set_dialog_value (ID, DIG_PARM_RADIO_LINEAR, ON);
        wi_set_dialog_value (ID, DIG_PARM_RADIO_LOG, OFF);
        wi_set_dialog_value (ID, DIG_PARM_RADIO_DEPTH, ON);
        wi_set_dialog_value (ID, DIG_PARM_RADIO_TIME, OFF);
        (void) strcpy (log_header->log_units_system, ENGLISH);
        (void) strcpy (log_header->log_scale_type, LINEAR);
        is_log_type = FALSE;
        (void) strcpy (log_header->trace_struct.depth_or_time, DEPTH);
        (void) strcpy (log_type, DEPTH);
        }
    if (Opened == FALSE)
        {
        Opened = TRUE;
        wiopcd (ID);                /* open header dialog box                       */
        }
    while (done == FALSE)
        {
        wi_request_dialog_item (&ID, &Item, &value, text);
        
        switch (Item)
            {
        case DIG_PARM_BTN_OK:
            wi_query_dialog_item (ID, DIG_PARM_MIN_TD, &value, text);
            if ((ts_isfloat (text) <= 0) || (strlen (text) == 0))
                {
                wi_set_dialog_text (ID, DIG_PARM_STAT_MESSAGE,
                                    mg_message (LG_INVALID_MIN_VALUE));
                wi_beep (1);
                break;
                }
            else
                (void) sscanf (text, "%f", &log_header->trace_struct.top);
            wi_query_dialog_item (ID, DIG_PARM_MAX_TD, &value, text);
            if ((ts_isfloat (text) <= 0) || (strlen (text) == 0))
                {
                wi_set_dialog_text (ID, DIG_PARM_STAT_MESSAGE,
                                    mg_message (LG_INVALID_MAX_VALUE));
                wi_beep (1);
                break;
                }
            else
                (void) sscanf (text, "%f", &log_header->trace_struct.base);
            wi_query_dialog_item (ID, DIG_PARM_TOP_SCALE, &value, text);
            if ((ts_isfloat (text) <= 0) || (strlen (text) == 0))
                {
                wi_set_dialog_text (ID, DIG_PARM_STAT_MESSAGE,
                                    mg_message (LG_INVALID_UPPER_LOG_VALUE));
                wi_beep (1);
                break;
                }
            else
                (void) sscanf (text, "%f", &log_header->top_y);
            
            wi_query_dialog_item (ID, DIG_PARM_BTM_SCALE, &value, text);
            if ((ts_isfloat (text) <= 0) || (strlen (text) == 0))
                {
                wi_set_dialog_text (ID, DIG_PARM_STAT_MESSAGE,
                                    mg_message (LG_INVALID_LOWER_LOG_VALUE));
                wi_beep (1);
                break;
                }
            else
                (void) sscanf (text, "%f", &log_header->bottom_y);
            
            wi_query_dialog_item (ID, DIG_PARM_SAMPLE, &value, text);
            if ((ts_isfloat (text) <= 0) || (strlen (text) == 0))
                {
                wi_set_dialog_text (ID, DIG_LOG_STAT_MESSAGE,
                                    mg_message (LG_INVALID_SAMPLE_INC));
                wi_beep (1);
                break;
                }
            else
                (void) sscanf (text, "%f", &log_header->trace_struct.dinc);
            
            wi_query_dialog_item (ID, DIG_PARM_OFFSCALE_UP, &value, text);
            if (ts_isfloat (text) <= 0)
                {
                wi_set_dialog_text (ID, DIG_LOG_STAT_MESSAGE,
                                    mg_message (LG_INVALID_OFFSCALES));
                wi_beep (1);
                break;
                }
            else
                (void) sscanf (text, "%d", &log_header->num_offscales_up);
            
            wi_query_dialog_item (ID, DIG_PARM_OFFSCALE_DOWN, &value, text);
            if (ts_isfloat (text) <= 0)
                {
                wi_set_dialog_text (ID, DIG_LOG_STAT_MESSAGE,
                                    mg_message (LG_INVALID_OFFSCALES));
                wi_beep (1);
                break;
                }
            else
                (void) sscanf (text, "%d", &log_header->num_offscales_down);
            
            wi_query_dialog_item (ID, DIG_PARM_TRACE_UNITS, &value,
                                  log_header->trace_struct.depth_uom);
            
            wi_query_dialog_item (ID, DIG_PARM_LOG_UNITS, &value,
                                  log_header->trace_struct.trace_uom);
            
            done = TRUE;
            break;
            
        case DIG_PARM_BTN_CANCEL:
            wi_set_dialog_text (ID, DIG_LOG_STAT_MESSAGE, NullString);
            status = LG_CANCEL;
            done = TRUE;
            break;
            
        case DIG_PARM_RADIO_ENGLISH:
            wi_set_dialog_value (ID, DIG_PARM_RADIO_ENGLISH, ON);
            wi_set_dialog_value (ID, DIG_PARM_RADIO_METRIC, OFF);
            (void) strcpy (log_header->log_units_system, ENGLISH);
            break;
            
        case DIG_PARM_RADIO_METRIC:
            wi_set_dialog_value (ID, DIG_PARM_RADIO_ENGLISH, OFF);
            wi_set_dialog_value (ID, DIG_PARM_RADIO_METRIC, ON);
            (void) strcpy (log_header->log_units_system, METRIC);
            break;
            
        case DIG_PARM_RADIO_LOG:
            wi_set_dialog_value (ID, DIG_PARM_RADIO_LINEAR, OFF);
            wi_set_dialog_value (ID, DIG_PARM_RADIO_LOG, ON);
            (void) strcpy (log_header->log_scale_type, LOG);
            wi_enable_dialog_item (ID, DIG_PARM_OFFSCALE_LBL, OFF);
            wi_enable_dialog_item (ID, DIG_PARM_OFFSCALE_UP, OFF);
            wi_enable_dialog_item (ID, DIG_PARM_OFFSCALE_DOWN, OFF);
            is_log_type = TRUE;
            break;
            
        case DIG_PARM_RADIO_LINEAR:
            wi_set_dialog_value (ID, DIG_PARM_RADIO_LINEAR, ON);
            wi_set_dialog_value (ID, DIG_PARM_RADIO_LOG, OFF);
            (void) strcpy (log_header->log_scale_type, LINEAR);
            wi_enable_dialog_item (ID, DIG_PARM_OFFSCALE_LBL, ON);
            wi_enable_dialog_item (ID, DIG_PARM_OFFSCALE_UP, ON);
            wi_enable_dialog_item (ID, DIG_PARM_OFFSCALE_DOWN, ON);
            is_log_type = FALSE;
            break;
            
        case DIG_PARM_RADIO_DEPTH:
            wi_set_dialog_value (ID, DIG_PARM_RADIO_DEPTH, ON);
            wi_set_dialog_value (ID, DIG_PARM_RADIO_TIME, OFF);
            (void) strcpy (log_header->trace_struct.depth_or_time, DEPTH);
            (void) strcpy (log_type, DEPTH);
            break;
            
        case DIG_PARM_RADIO_TIME:
            wi_set_dialog_value (ID, DIG_PARM_RADIO_DEPTH, OFF);
            wi_set_dialog_value (ID, DIG_PARM_RADIO_TIME, ON);
            (void) strcpy (log_header->trace_struct.depth_or_time, TIME);
            (void) strcpy (log_type, TIME);
            break;
            
        default:
            break;
            }
        }
    (void) lg_log_cdlg_term (&ID, &Opened);
    
    return status;
    }
/* END:                                                                             */

/* Function Description -----------------------------------------------------       */
/*Description:                                                                      */
/*    This routine brings up a dialog and gets log registrat-                       */
/*    ion (calibration of the grid) info from the abuser.                           */
/*                                                                                  */
/*Prototype:                                                                        */
/*    static INT lg_get_log_registration(FLOAT xy_cntl[NUMBER_OF_POINTS][2]);   */
/*                                                                                  */
/*Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)        */
/*    xy_cntl         -(FLOAT [][])                                                 */
/*                                                                                  */
/*Return Value/Status:                                                              */
/*    SUCCESS - Successful completion.                                              */
/*    FAIL    - Unsuccessful completion.                                            */
/*                                                                                  */
/*Scope:                                                                            */
/*    PRIVATE to <lg_log_dig>                                                       */
/*                                                                                  */
/*Limitations/Assumptions:                                                          */
/*                                                                                  */
/*-----------------------------------------------------------------------------     */

/* BEGIN:                                                                           */
#if USE_PROTOTYPES
static INT lg_get_log_registration (FLOAT xy_cntl[NUMBER_OF_POINTS][2])
#else
static INT lg_get_log_registration (xy_cntl)
FLOAT xy_cntl[NUMBER_OF_POINTS][2];
#endif
    {
    static INT Opened = FALSE;
    static INT ID = 0;
    INT status;
    CHAR text[128];
    INT Item, value, done = FALSE;
    /********************************************************************************/
    /* define dialog resources                                                      */
    if (ID == 0)
        am_define_resource (AM_DIALOG, ResourceFile, DIG_REG_LOG_SEGMENT,
                            NULL_FUNCTION_PTR, &ID, NULL);
    
    wi_set_dialog_text (ID, DIG_REG_STAT_MESSAGE,
                        mg_message (LG_ENTER_CONTROL_POINTS));
    wi_set_dialog_text (ID, DIG_REG_X_LBL_UNITS, log_type);
    wi_set_dialog_text (ID, DIG_REG_LL_CP_X, NullString);
    wi_set_dialog_text (ID, DIG_REG_LL_CP_Y, NullString);
    wi_set_dialog_text (ID, DIG_REG_LR_CP_X, NullString);
    wi_set_dialog_text (ID, DIG_REG_LR_CP_Y, NullString);
    wi_set_dialog_text (ID, DIG_REG_UL_CP_X, NullString);
    wi_set_dialog_text (ID, DIG_REG_UL_CP_Y, NullString);
    
    if (Opened == FALSE)
        {
        Opened = TRUE;
        wiopcd (ID);
        }
    while (done == FALSE)
        {
        wi_request_dialog_item (&ID, &Item, &value, text);
        
        switch (Item)
            {
        case DIG_REG_BTN_OK:
            wi_query_dialog_item (ID, DIG_REG_LL_CP_X, &value, text);
            if ((ts_isfloat (text) <= 0) || (strlen (text) == 0))
                {
                wi_set_dialog_text (ID, DIG_REG_STAT_MESSAGE,
                                    mg_message (LG_INVALID_LOWER_LEFT_X));
                wi_beep (1);
                break;
                }
            else
                (void) sscanf (text, "%f", &xy_cntl[0][0]);
            
            wi_query_dialog_item (ID, DIG_REG_LL_CP_Y, &value, text);
            if ((ts_isfloat (text) <= 0) || (strlen (text) == 0))
                {
                wi_set_dialog_text (ID, DIG_REG_STAT_MESSAGE,
                                    mg_message (LG_INVALID_LOWER_LEFT_Y));
                wi_beep (1);
                break;
                }
            else
                (void) sscanf (text, "%f", &xy_cntl[0][1]);
            
            wi_query_dialog_item (ID, DIG_REG_UL_CP_X, &value, text);
            if ((ts_isfloat (text) <= 0) || (strlen (text) == 0))
                {
                wi_set_dialog_text (ID, DIG_REG_STAT_MESSAGE,
                                    mg_message (LG_INVALID_UPPER_LEFT_X));
                wi_beep (1);
                break;
                }
            else
                (void) sscanf (text, "%f", &xy_cntl[1][0]);
            
            wi_query_dialog_item (ID, DIG_REG_UL_CP_Y, &value, text);
            if ((ts_isfloat (text) <= 0) || (strlen (text) == 0))
                {
                wi_set_dialog_text (ID, DIG_REG_STAT_MESSAGE,
                                    mg_message (LG_INVALID_UPPER_LEFT_Y));
                wi_beep (1);
                break;
                }
            else
                (void) sscanf (text, "%f", &xy_cntl[1][1]);
            
            wi_query_dialog_item (ID, DIG_REG_LR_CP_X, &value, text);
            if ((ts_isfloat (text) <= 0) || (strlen (text) == 0))
                {
                wi_set_dialog_text (ID, DIG_REG_STAT_MESSAGE,
                                    mg_message (LG_INVALID_LOWER_RIGHT_X));
                wi_beep (1);
                break;
                }
            else
                (void) sscanf (text, "%f", &xy_cntl[2][0]);
            
            wi_query_dialog_item (ID, DIG_REG_LR_CP_Y, &value, text);
            if ((ts_isfloat (text) <= 0) || (strlen (text) == 0))
                {
                wi_set_dialog_text (ID, DIG_REG_STAT_MESSAGE,
                                    mg_message (LG_INVALID_LOWER_RIGHT_Y));
                wi_beep (1);
                break;
                }
            else
                (void) sscanf (text, "%f", &xy_cntl[2][1]);
            /* check for bad limits (left_x == right_x)                             */
            if (xy_cntl[0][0] == xy_cntl[2][0])
                {
                wi_set_dialog_text (ID, DIG_REG_STAT_MESSAGE,
                                    mg_message (LG_INVALID_LIMITS));
                wi_beep (1);
                break;
                }
            if (xy_cntl[0][1] != xy_cntl[2][1])
                {
                wi_set_dialog_text (ID, DIG_REG_STAT_MESSAGE,
                                    mg_message (LG_INVALID_LIMITS));
                wi_beep (1);
                break;
                }
            done = TRUE;
            break;
            
        case DIG_REG_BTN_CANCEL:
            wi_set_dialog_text (ID, DIG_REG_STAT_MESSAGE, NullString);
            status = 2;
            done = TRUE;
            break;
            
        default:
            break;
            }
        }
    (void) lg_log_cdlg_term (&ID, &Opened);
    
    return status;
    }
/* END:                                                                             */

/* Function Description -----------------------------------------------------       */
/*Description:                                                                      */
/*    This routine gets (3) log registration control points                         */
/*    from the user to set up the tablet transformation.                            */
/*    Direct GKS calls are used to return tablet coordin-                           */
/*    ates.                                                                         */
/*                                                                                  */
/*Prototype:                                                                        */
/*    static INT lg_digitize_control_points(FLOAT xy_tablet[][2]);              */
/*                                                                                  */
/*Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)        */
/*    xy_tablet       -(FLOAT [][]) Digitized values.                               */
/*                                                                                  */
/*Return Value/Status:                                                              */
/*    SUCCESS - Successful completion.                                              */
/*                                                                                  */
/*Scope:                                                                            */
/*    PRIVATE to <lg_log_dig>                                                       */
/*                                                                                  */
/*Limitations/Assumptions:                                                          */
/*                                                                                  */
/*-----------------------------------------------------------------------------     */

/* BEGIN:                                                                           */
#if USE_PROTOTYPES
static INT lg_digitize_control_points (FLOAT xy_tablet[][2])
#else
static INT lg_digitize_control_points (xy_tablet)
FLOAT xy_tablet[][2];
#endif
    {
    GKS_REAL x_tablet, y_tablet;
    INT status;
    INT done;
    INT locate_tnr;
    INT button;
    /********************************************************************************/
    
    done = FALSE;
    
    while (done == FALSE)
        {
        /*  Get lower left point                                                    */
        
        am_message (AM_DIALOG, mg_message (LG_DIGITIZE_LOWER_LEFT));
        
        /* Use wi routine to correctly journal or playback...                       */
        wi_request_locate (&status, &locate_tnr, &x_tablet, &y_tablet, &button);
        
        if (status == 0)
            {
            return FAIL;
            }
        xy_tablet[0][0] = x_tablet;
        xy_tablet[0][1] = y_tablet;
        wi_beep (1);
        
        /*  Get the upper left control point                                        */
        
        while (done == FALSE)
            {
            am_message (AM_DIALOG, mg_message (LG_DIGITIZE_UPPER_LEFT));
            
            /* Use wi routine to correctly journal or playback...                   */
            wi_request_locate (&status, &locate_tnr, &x_tablet, &y_tablet, &button);
            
            if (status == 0)
                break;
            xy_tablet[1][0] = x_tablet;
            xy_tablet[1][1] = y_tablet;
            wi_beep (1);
            
            /*  Get the lower right control point                                   */
            
            am_message (AM_DIALOG, mg_message (LG_DIGITIZE_LOWER_RIGHT));
            
            /* Use wi routine to correctly journal or playback...                   */
            wi_request_locate (&status, &locate_tnr, &x_tablet, &y_tablet, &button);
            
            if (status == 0)
                continue;
            xy_tablet[2][0] = x_tablet;
            xy_tablet[2][1] = y_tablet;
            wi_beep (1);
            done = TRUE;
            }
        }
    am_message (AM_DIALOG, NullString);
    
    return SUCCESS;
    }
/* END:                                                                             */

/* Function Description -----------------------------------------------------       */
/*Description:                                                                      */
/*    This routine performs the actual trace digitization.                          */
/*    The N-list editor is called, but with a user defined                          */
/*    keymap and some user defined functions (lg_add_point,                         */
/*    etc.)                                                                         */
/*                                                                                  */
/*Prototype:                                                                        */
/*    static INT lg_digitize_trace(NLIST_HEADER *log_nl, DIG_LOG_STRUCT *log_header);   */
/*                                                                                  */
/*Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)        */
/*    log_nl          -(NLIST_HEADER *) N-List containing the log trace.            */
/*    log_header      -(DIG_LOG_STRUCT *) Current trace info.                       */
/*                                                                                  */
/*Return Value/Status:                                                              */
/*    SUCCESS - Successful completion.                                              */
/*    CANCEL_TRACE                                                                  */
/*    END_OF_TRACE                                                                  */
/*                                                                                  */
/*Scope:                                                                            */
/*    PRIVATE to <lg_log_dig>                                                       */
/*                                                                                  */
/*Limitations/Assumptions:                                                          */
/*                                                                                  */
/*-----------------------------------------------------------------------------     */

/* BEGIN:                                                                           */
#if USE_PROTOTYPES
static INT lg_digitize_trace (NLIST_HEADER *log_nl, DIG_LOG_STRUCT *log_header)
#else
static INT lg_digitize_trace (log_nl, log_header)
NLIST_HEADER *log_nl;
DIG_LOG_STRUCT *log_header;
#endif
    {
    LE_PARLIST le_struct;           /* N-list editor structure                      */
    UINT no_pts;
    INT status;
    LE_KEYS *keymap;
    INDEX width;
    /********************************************************************************/
    /*  Initialize the line editor                                                  */
    
    le_allocate_keymap (&keymap, LE_POINT_MODE, 1);
    
    /*  Set up the keys                                                             */
    
    le_set_key (keymap, 0, 1, BUTTON_NEW_POINT, lg_add_point, NullString, FALSE);
    le_set_key (keymap, 0, 2, BUTTON_OFFSCALE_UP, lg_offscale_up, NullString, FALSE);
    le_set_key (keymap, 0, 3, BUTTON_OFFSCALE_DOWN, lg_offscale_down, NullString,
                FALSE);
    le_set_key (keymap, 0, 4, BUTTON_BACK_POINT, le_back_point, NullString, FALSE);
    le_set_key (keymap, 0, 5, BUTTON_NO_DATA_ZONE, lg_no_data_zone, NullString, FALSE);
    le_set_key (keymap, 0, 6, BUTTON_END_SEGMENT, lg_end_segment, NullString, FALSE);
    le_set_key (keymap, 0, 7, BUTTON_ZOOMIN, le_zoomin, NullString, FALSE);
    le_set_key (keymap, 0, 8, BUTTON_ZOOMOUT, le_zoomout, NullString, FALSE);
    le_set_key (keymap, 0, 9, BUTTON_PAN, le_pan, NullString, FALSE);
    le_set_key (keymap, 0, 10, BUTTON_ADD_POINT, le_add_point, NullString, FALSE);
    le_set_key (keymap, 0, 12, BUTTON_CANCEL_TRACE, lg_cancel_trace, NullString,
                FALSE);
    le_set_key (keymap, 0, 13, BUTTON_TRACE_DONE, lg_trace_done, NullString, FALSE);
    
    /* set up the N-list editor structure                                           */
    
    tc_zeroes (&le_struct, sizeof le_struct);
    le_struct.line.color = log_header->draw_color;
    le_struct.erase_color = log_header->erase_color;
    le_struct.hilight_color = log_header->hilight_color;
    /*    le_struct.line.width = 2.;                                Medium Line     */
    lu_text_to_index ("THICKNESS", (INDEX *)&width, "FINE"); /* Thin line    */
    lu_text_to_index ("LINE_STYLE", &le_struct.line.style, "SOLID");    /*  Solid   */
    le_struct.line.width = width;
    le_struct.data_pointer = (BYTE *)log_header;
    le_struct.initkeymap = TRUE;
    le_struct.init_dialog = FALSE;
    le_struct.use_keymap = TRUE;
    le_struct.only_app_mac_dialog = TRUE;
    le_struct.nlist = *log_nl;
    le_struct.point_keymap = keymap;
    le_struct.num_point_keymaps = 1;
    le_struct.stroke_keymap = ((LE_KEYS *)0);
    le_struct.num_stroke_keymaps = 0;
    (void) strcpy (le_struct.title, NullString);
    le_struct.app_puck_map_disp_init = lg_init_keymap;
    replace_mode = FALSE;
    
    /* Start Line Editor                                                            */
    
    le_nlist_editor (&le_struct);
    
    /*  Process returned nlist                                                      */
    
    status = le_struct.status;
    *log_nl = le_struct.nlist;
    tc_free (le_struct.point_keymap);
    nl_inq_nlist_int (*log_nl, NL_NLIST_NPOINTS, (INT *)&no_pts);
    
    if ((no_pts != 0) && (status == SUCCESS))
        {
        return END_OF_TRACE;
        }
    else
        {
        return CANCEL_TRACE;
        }
    }
/* END:                                                                             */

/* Function Description -----------------------------------------------------       */
/*Description:                                                                      */
/*    Set up keymap dialog since we are not using the nlist                         */
/*    editor default dialogs. Also start a stroke if a new                          */
/*    log or set current point if editing.                                          */
/*                                                                                  */
/*Prototype:                                                                        */
/*    static INT lg_init_keymap(LE_CONTROL *control);                           */
/*                                                                                  */
/*Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)        */
/*    control         -(LE_CONTROL *)                                               */
/*                                                                                  */
/*Return Value/Status:                                                              */
/*    SUCCESS - Successful completion.                                              */
/*                                                                                  */
/*Scope:                                                                            */
/*    PRIVATE to <lg_log_dig>                                                       */
/*                                                                                  */
/*Limitations/Assumptions:                                                          */
/*                                                                                  */
/*-----------------------------------------------------------------------------     */

/* BEGIN:                                                                           */
#if USE_PROTOTYPES
static INT lg_init_keymap (LE_CONTROL *control)
#else
static INT lg_init_keymap (control)
LE_CONTROL *control;
#endif
    {
    DIG_LOG_STRUCT *log_header = (DIG_LOG_STRUCT *)control->data_pointer;
    NLIST_HEADER nlist = control->nlist;
    DOUBLE lastx, lasty;
    INT status, npoints;
    CHAR text[128];
    /****************************************************************************   */
    
    if (keymap_dialog == 0)
        am_define_resource (AM_DIALOG, ResourceFile, DIG_DIGITIZE, NULL_FUNCTION_PTR,
                            &keymap_dialog, NULL);
    
    wi_set_dialog_text (keymap_dialog, DIG_STAT_MESSAGE, NullString);
    wi_set_dialog_text (keymap_dialog, DIG_SHOW_X_LBL, log_type);
    
    if (strcmp (log_type, DEPTH) == 0)
        {
        (void) sprintf (text, DEPTH_FORMAT, log_header->trace_struct.top);
        }
    else
        {
        (void) sprintf (text, TIME_FORMAT, log_header->trace_struct.base);
        }
    wi_set_dialog_text (keymap_dialog, DIG_SHOW_X, text);
    (void) strcpy (text, "0.0");
    wi_set_dialog_text (keymap_dialog, DIG_SHOW_Y, text);
    
    (void) sprintf (text, SCALE_FORMAT, log_header->top_y);
    wi_set_dialog_text (keymap_dialog, DIG_UPPER_LIMIT, text);
    
    (void) sprintf (text, SCALE_FORMAT, log_header->bottom_y);
    wi_set_dialog_text (keymap_dialog, DIG_LOWER_LIMIT, text);
    
    /* Open keymap dialog                                                           */
    
    wi_open_dialog (keymap_dialog);
    
    control->stroke_mode = FALSE;
    
    /*  Start a stroke, if not editing                                              */
    
    if (log_header->edit_flag != TRUE)
        {
        status = nl_start_stroke (nlist, &control->scratch_str1);
        control->curpoint_str1 = 0;
        }
    else
        {                           /* editing old trace                            */
        status = nl_copy_stroke (nlist, 1, nlist, &control->scratch_str1);
        /* set current pt. to end                                                   */
        /*   of recalled nlist                                                      */
        status = nl_inq_stroke_int (nlist, control->scratch_str1, NL_STROKE_NPOINTS,
                                    &npoints);
        status = nl_set_current_point (nlist, npoints, control->scratch_str1);
        value_list[0] = (VOIDPTR)&lastx;
        value_list[1] = (VOIDPTR)&lasty;
        /* get last point                                                           */
        status = nl_last_point (nlist, ncol, col_list, value_list);
        /* set current pt=last                                                      */
        control->lastx = current_x = lastx;
        control->lasty = current_y = lasty;
        control->current_operation = BUTTON_EXTEND_STROKE;
        control->current_stroke = 1;
        /*--------------------------------------------------------------------------*/
        /* turn off rubberband for                                                  */
        /* first point                                                              */
        /*--------------------------------------------------------------------------*/
        control->nbp = 0;
        
        }
    return SUCCESS;
    }
/* END:                                                                             */

/* Function Description -----------------------------------------------------       */
/*Description:                                                                      */
/*    To store the digitized trace with an LTDM call, after                         */
/*    performing scale conversion, trace inversion and re-                          */
/*    sampling (if required).                                                       */
/*                                                                                  */
/*Prototype:                                                                        */
/*    static INT lg_save_trace(NLIST_HEADER log_nl, DIG_LOG_STRUCT *log_header);*/
/*                                                                                  */
/*Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)        */
/*    log_nl          -(NLIST_HEADER) N-List containing completed trace.            */
/*    log_header      -(DIG_LOG_STRUCT *) Current trace info.                       */
/*                                                                                  */
/*Return Value/Status:                                                              */
/*    SUCCESS - Successful completion.                                              */
/*    FAIL    - Unsuccessful completion.                                            */
/*                                                                                  */
/*Scope:                                                                            */
/*    PRIVATE to <lg_log_dig>                                                       */
/*                                                                                  */
/*Limitations/Assumptions:                                                          */
/*                                                                                  */
/*-----------------------------------------------------------------------------     */

/* BEGIN:                                                                           */
#if USE_PROTOTYPES
static INT lg_save_trace (NLIST_HEADER log_nl, DIG_LOG_STRUCT *log_header)
#else
static INT lg_save_trace (log_nl, log_header)
NLIST_HEADER log_nl;
DIG_LOG_STRUCT *log_header;
#endif
    {
    CHAR *ptr;
    INT status, yes;
    NLM_RESAMPLE_HEADER parms;
    
    /*  Ask user if he wants to save the log trace                                  */
    
    wiyesno ("Do you wish to save this trace?", "LOG/SAVE", &yes, &status);
    if (!yes)
        return FAIL;
    
    /*  If required, convert from log to linear scale and convert uom               */
    
    status = lg_convert_nlist (&log_nl, log_header);
    
    /*  If required, flip the nlist (invert the y trace value coordinates)          */
    
    if (log_header->flipped)
        status = lg_flip_nlist (log_header, &log_nl);
    
    /*  Sample the n-list to the depth increment                                    */
    status = nl_set_nlist_method (log_nl, NL_RESAMPLE_METHOD, "NLM_SPLINE_NLIST",
                                  nlm_spline_nlist);
    parms.ncol = ncol;
    parms.Col_List = col_list;
    parms.resampleInc = log_header->trace_struct.dinc;
    parms.filterLen = FILTERLEN;
    
    status = nl_invoke_nlist_method (log_nl, NL_RESAMPLE_METHOD, &parms);
    
    if (status != SUCCESS)
        {
        am_message (AM_STATUS, mg_message (LG_CANNOT_RESAMPLE));
        return FAIL;
        }
    /* call LTDM to add new trace                                                   */
    
    log_header->trace_struct.ptrace = parms.outNlist;
    
    /* erase the version.                                                           */
    log_header->trace_struct.version = 0;
    ptr = strchr (log_header->trace_struct.trace_name, ';');
    if (ptr IS_NOT_EQUAL_TO NULL)
        *ptr = '\0';
    
    status = lg_ltdm_add_trace (&log_header->trace_struct);
    
    if (status != SUCCESS)
        {
        return FAIL;
        }
    else
        return SUCCESS;
    
    }
/* END:                                                                             */

/* Function Description -----------------------------------------------------       */
/*Description:                                                                      */
/*    To reset the scale limits when the offscale up button                         */
/*    is pressed.  The user should press the offscale up                            */
/*    button at the start of the offscale, continue adding                          */
/*    points, the press the offscale down button when the                           */
/*    data returns to regular scale.  The number of off-                            */
/*    scales refers to the magnitude (in total scale int-                           */
/*    ervals) of the largest value.                                                 */
/*                                                                                  */
/*Prototype:                                                                        */
/*    static INT lg_offscale_up(LE_CONTROL *control);                           */
/*                                                                                  */
/*Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)        */
/*    control         -(LE_CONTROL *)                                               */
/*                                                                                  */
/*Return Value/Status:                                                              */
/*    BUTTON_NOOP                                                                   */
/*                                                                                  */
/*Scope:                                                                            */
/*    PRIVATE to <lg_log_dig>                                                       */
/*                                                                                  */
/*Limitations/Assumptions:                                                          */
/*                                                                                  */
/*-----------------------------------------------------------------------------     */

/* BEGIN:                                                                           */
#if USE_PROTOTYPES
static INT lg_offscale_up (LE_CONTROL *control)
#else
static INT lg_offscale_up (control)
LE_CONTROL *control;
#endif
    {
    DIG_LOG_STRUCT *log_header = (DIG_LOG_STRUCT *)control->data_pointer;
    CHAR text[31];
    DOUBLE delta_scale;
    DOUBLE log_top, log_bottom;
    
    if (log_header->current_offscale_position + 1 > log_header->num_offscales_up)
        {
        wi_beep (1);
        return BUTTON_NOOP;
        }
    else
        log_header->current_offscale_position++;
    
    if (!is_log_type)
        {
        delta_scale = log_header->top_y - log_header->bottom_y;
        log_header->top_y += delta_scale;
        log_header->bottom_y += delta_scale;
        if (log_header->flipped)
            delta_scale = (-delta_scale);
        log_header->trace_offset += delta_scale;
        }
    else
        {
        log_top = log10 (log_header->top_y);
        log_bottom = log10 (log_header->bottom_y);
        delta_scale = log_top - log_bottom;
        log_header->bottom_y = log_header->top_y;
        log_header->top_y = EXP10 (log_top + delta_scale);
        if (log_header->flipped)
            delta_scale = (-delta_scale);
        log_header->trace_offset += delta_scale;
        }
    /*  Display new limits                                                          */
    
    (void) sprintf (text, SCALE_FORMAT, log_header->top_y);
    wi_set_dialog_text (keymap_dialog, DIG_UPPER_LIMIT, text);
    (void) sprintf (text, SCALE_FORMAT, log_header->bottom_y);
    wi_set_dialog_text (keymap_dialog, DIG_LOWER_LIMIT, text);
    control->default_puck_map = FALSE;
    wi_beep (1);
    
    return BUTTON_NOOP;
    }
/* END:                                                                             */

/* Function Description -----------------------------------------------------       */
/*Description:                                                                      */
/*    Routine called from line editor when offscale-down puck key pressed.          */
/*                                                                                  */
/*Prototype:                                                                        */
/*    static INT lg_offscale_down(LE_CONTROL *control);                         */
/*                                                                                  */
/*Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)        */
/*    control         -(LE_CONTROL *)                                               */
/*                                                                                  */
/*Return Value/Status:                                                              */
/*    BUTTON_NOOP                                                                   */
/*                                                                                  */
/*Scope:                                                                            */
/*    PRIVATE to <lg_log_dig>                                                       */
/*                                                                                  */
/*Limitations/Assumptions:                                                          */
/*                                                                                  */
/*-----------------------------------------------------------------------------     */

/* BEGIN:                                                                           */
#if USE_PROTOTYPES
static INT lg_offscale_down (LE_CONTROL *control)
#else
static INT lg_offscale_down (control)
LE_CONTROL *control;
#endif
    {
    DIG_LOG_STRUCT *log_header = (DIG_LOG_STRUCT *)control->data_pointer;
    CHAR text[31];
    DOUBLE delta_scale;
    DOUBLE log_top, log_bottom;
    
    if (log_header->current_offscale_position - 1 < (-log_header->num_offscales_down))
        {
        wi_beep (1);
        return BUTTON_NOOP;
        }
    else
        log_header->current_offscale_position--;
    
    if (!is_log_type)
        {
        delta_scale = log_header->top_y - log_header->bottom_y;
        log_header->top_y -= delta_scale;
        log_header->bottom_y -= delta_scale;
        if (log_header->flipped)
            delta_scale = (-delta_scale);
        log_header->trace_offset -= delta_scale;
        }
    else
        {
        log_top = log10 (log_header->top_y);
        log_bottom = log10 (log_header->bottom_y);
        delta_scale = log_top - log_bottom;
        log_header->top_y = log_header->bottom_y;
        log_header->bottom_y = EXP10 (log_bottom - delta_scale);
        if (log_header->flipped)
            delta_scale = (-delta_scale);
        log_header->trace_offset -= delta_scale;
        }
    /*  Display new limits                                                          */
    
    (void) sprintf (text, SCALE_FORMAT, log_header->top_y);
    wi_set_dialog_text (keymap_dialog, DIG_UPPER_LIMIT, text);
    (void) sprintf (text, SCALE_FORMAT, log_header->bottom_y);
    wi_set_dialog_text (keymap_dialog, DIG_LOWER_LIMIT, text);
    control->default_puck_map = FALSE;
    wi_beep (1);
    
    return BUTTON_NOOP;
    }
/* END:                                                                             */

/* Function Description -----------------------------------------------------       */
/*Description:                                                                      */
/*    This routine performs the point by point digitization.                        */
/*                    It differs from ADD_POINT_MONO in the N-list editor           */
/*                    in that when the user backs up and digitizes a point          */
/*                    the previous point is erased. The N-list editor function      */
/*                    will not allow the user to back up (beep occurs).             */
/*                                                                                  */
/*Prototype:                                                                        */
/*    static INT lg_add_point(LE_CONTROL *control);                             */
/*                                                                                  */
/*Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)        */
/*    control         -(LE_CONTROL *)                                               */
/*                                                                                  */
/*Return Value/Status:                                                              */
/*    BUTTON_ADD_POINT                                                              */
/*                                                                                  */
/*Scope:                                                                            */
/*    PRIVATE to <lg_log_dig>                                                       */
/*                                                                                  */
/*Limitations/Assumptions:                                                          */
/*                                                                                  */
/*-----------------------------------------------------------------------------     */

/* BEGIN:                                                                           */
#if USE_PROTOTYPES
static INT lg_add_point (LE_CONTROL *control)
#else
static INT lg_add_point (control)
LE_CONTROL *control;
#endif
    {
    NLIST_HEADER nlist = control->nlist;
    DIG_LOG_STRUCT *log_header = (DIG_LOG_STRUCT *)control->data_pointer;
    CHAR text[31];
    FLOAT x, y;                     /* Scratch point                                */
    INT status;
    UINT npnts;
    UINT temp_stroke;
    static DOUBLE x_end_replace;
    
    /*  Flip trace if necessary                                                     */
    /*  Bias trace value by offscale bias, if any                                   */
    
    control->wy += log_header->trace_offset;
    
    /* Display new point on MAC                                                     */
    
    if (strcmp (log_type, TIME) == 0)
        {
        (void) sprintf (text, TIME_FORMAT, control->wx);
        }
    else
        {
        (void) sprintf (text, DEPTH_FORMAT, control->wx);
        }
    wi_set_dialog_text (keymap_dialog, DIG_SHOW_X, text);
    y = control->wy;
    if (is_log_type)
        y = EXP10 (y);
    if (log_header->flipped)
        y = log_header->trace_flip_value - y;
    
    (void) sprintf (text, SCALE_FORMAT, y);
    wi_set_dialog_text (keymap_dialog, DIG_SHOW_Y, text);
    
    /*  Check for replace mode                                                      */
    
    if (!replace_mode)
        {
        if (control->wx >= current_x)
            {
            /* dig. pt. is to the right of prev.                                    */
            /*   pt. (monotonic) no problem                                         */
            if (log_header->trace_struct.base < control->wx)
                log_header->trace_struct.base = control->wx;
            control->default_puck_map = FALSE;
            control->lastx = current_x;
            control->lasty = current_y;
            current_x = control->wx;
            current_y = control->wy;
            return BUTTON_ADD_POINT;
            }
        else
            {
            /* New pt. is to the left of previous pt., copy pts. in nlist from xmax back to     */
            /*   the new pt. into a temporary nlist.  These pts. will be highlighted and del-   */
            /*   eted from the permanent nlist.                                     */
            
            x = 1.e20;
            status = 0;
            value_list[0] = (VOIDPTR)&x;
            value_list[1] = (VOIDPTR)&y;
            nl_init_nlist (&temp_nl, ncol, units, data_size, data_types, value_inc,
                           blocksize);
            nl_set_nlist_info (temp_nl, NL_NLIST_COLUMN_NAME, ncol, col_list, c_name);
            nl_start_stroke (temp_nl, &temp_stroke);
            x = control->lastx;
            y = control->lasty;
            nl_add_points (temp_nl, 1, ncol, col_list, value_list);
            npnts = 1;
            x_end_replace = x;
            status = 0;
            
            /* loop to copy pts. to be erased                                       */
            while ((x > control->wx) && (status == 0))
                {
                status = nl_previous_point (nlist, ncol, col_list, value_list);
                if (status == 0)
                    {
                    nl_insert_points (temp_nl, 1, ncol, col_list, value_list);
                    nl_set_current_point (temp_nl, 1, 1);
                    control->lastx = x;
                    control->lasty = y;
                    npnts++;
                    }
                }
            if (npnts > 1)
                {
                /* highlight and delete points                                      */
                wm_gsplci (log_header->hilight_color);
                nl_draw_stroke (temp_nl, temp_stroke, 1, 2);
                wm_gsplci (log_header->draw_color);
                nl_inq_current_point (nlist, &current_point, &current_stroke);
                current_point++;
                npnts--;
                nl_set_current_point (nlist, current_point, control->scratch_str1);
                nl_delete_points (nlist, npnts);
                
                nl_set_current_point (nlist, current_point, current_stroke);
                nl_delete_points (nlist, npnts);
                
                }
            /*  Check to see if the backup was to the left of the starting point    */
            
            if (status != 0)        /* Yes                                          */
                {
                control->default_puck_map = FALSE;
                control->lastx = control->wx;
                control->lasty = control->wy;
                control->stroke_mode = FALSE;
                control->nbp = 1;
                return BUTTON_ADD_POINT;
                }
            else                    /* No                                           */
                {
                replace_mode = TRUE;
                current_x = control->wx;
                current_y = control->wy;
                control->default_puck_map = FALSE;
                control->nbp = 1;
                return BUTTON_ADD_POINT;
                }
            }
        }
    else
        {
        if (control->wx > x_end_replace)
            {
            replace_mode = FALSE;   /* Turn-off replace mode                        */
            log_header->trace_struct.base = control->wx;
            wm_gsplci (log_header->erase_color);
            nl_draw_stroke (temp_nl, 1, 1, 2);  /* Erase the old values             */
            wm_gsplci (log_header->draw_color);
            nl_free_nlist (temp_nl);
            control->lastx = current_x;
            control->lasty = current_y;
            current_x = control->wx;
            current_y = control->wy;
            return BUTTON_ADD_POINT;
            }
        else
            {
            current_x = control->wx;    /*  Still in replace mode                   */
            current_y = control->wy;
            control->nbp = 1;
            control->default_puck_map = FALSE;
            return BUTTON_ADD_POINT;
            }
        }
    }
/* END:                                                                             */

/* Function Description -----------------------------------------------------       */
/*Description:                                                                      */
/*    This routine inserts null values into the N-list where                        */
/*    the user hits the No-Data zone button.                                        */
/*                                                                                  */
/*Prototype:                                                                        */
/*    static INT lg_no_data_zone(LE_CONTROL *control);                          */
/*                                                                                  */
/*Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)        */
/*    control         -(LE_CONTROL *)                                               */
/*                                                                                  */
/*Return Value/Status:                                                              */
/*    BUTTON_NOOP                                                                   */
/*                                                                                  */
/*Scope:                                                                            */
/*    PRIVATE to <lg_log_dig>                                                       */
/*                                                                                  */
/*Limitations/Assumptions:                                                          */
/*                                                                                  */
/*-----------------------------------------------------------------------------     */

/* BEGIN:                                                                           */
#if USE_PROTOTYPES
static INT lg_no_data_zone (LE_CONTROL *control)
#else
static INT lg_no_data_zone (control)
LE_CONTROL *control;
#endif
    {
    NLIST_HEADER nlist = control->nlist;
    INT status;
    FLOAT null_x, null_y;
    
    /*  Add the current point                                                       */
    /*  Determine the null value for this pointlist                                 */
    
    value_list[0] = (VOIDPTR)&null_x;
    value_list[1] = (VOIDPTR)&null_y;
    status = nl_inq_nlist_info (nlist, NL_NLIST_NULL_VALUE, ncol, col_list,
                                value_list);
    
    /*  Insert a null point into the pointlist                                      */
    
    status = nl_add_points (nlist, 1, ncol, col_list, value_list);
    control->default_puck_map = FALSE;
    
    return BUTTON_NOOP;
    }
/* END:                                                                             */

/* Function Description -----------------------------------------------------       */
/*Description:                                                                      */
/*    Register a segment - A segment is that part of a log which will               */
/*    fit on the tablet.                                                            */
/*                                                                                  */
/*Prototype:                                                                        */
/*    static INT lg_register_segment(DIG_LOG_STRUCT *log_header);               */
/*                                                                                  */
/*Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)        */
/*    log_header      -(DIG_LOG_STRUCT *)                                           */
/*                                                                                  */
/*Return Value/Status:                                                              */
/*    LOG_DIGITIZE_STEP                                                             */
/*                                                                                  */
/*Scope:                                                                            */
/*    PRIVATE to <lg_log_dig>                                                       */
/*                                                                                  */
/*Limitations/Assumptions:                                                          */
/*                                                                                  */
/*-----------------------------------------------------------------------------     */

/* BEGIN:                                                                           */
#if USE_PROTOTYPES
static INT lg_register_segment (DIG_LOG_STRUCT *log_header)
#else
static INT lg_register_segment (log_header)
DIG_LOG_STRUCT *log_header;
#endif
    {
    INT status;
    FLOAT xy_cntl[NUMBER_OF_POINTS][2];
    FLOAT xy_tablet[NUMBER_OF_POINTS][2];
    FLOAT llx, lly, urx, ury, xpoints[3], ypoints[3];
    DOUBLE rscale_x, rscale_y;
    INT no_pnts;
    INT i;
    DOUBLE x_temp, y_temp;
    DOUBLE angle, wcos, wsin;
    
    wmtregonoff (OFF);
    
    status = lg_get_log_registration (xy_cntl);
    if (status == LG_CANCEL)
        {
        log_header->segment_number--;
        if (log_header->segment_number == 0)
            return LOG_PARM_STEP;
        }
    if (log_header->flipped)
        for (i = 0; i < 3; i++)
            xy_cntl[i][1] = log_header->trace_flip_value - xy_cntl[i][1];
    log_header->segment_number++;
    
    /* Get the control points                                                       */
    
    status = lg_digitize_control_points (xy_tablet);
    if (status == FAIL)
        {
        wmwclose (window_id);
        wmwdelete (window_id);
        window_id = 0;
        return LOG_HEADER_STEP;
        }
    /* Now actually register the transformation                                     */
    
    no_pnts = 3;
    llx = x_origin;
    lly = y_origin;
    
    rscale_x = (xy_cntl[2][0] - xy_cntl[0][0]) / (xy_tablet[2][0] - xy_tablet[0][0]);
    if (is_log_type)
        {
        rscale_y = (log10 (xy_cntl[1][1]) - log10 (xy_cntl[0][1])) /
            (xy_tablet[1][1] - xy_tablet[0][1]);
        }
    else
        {
        rscale_y = (xy_cntl[1][1] - xy_cntl[0][1]) / (xy_tablet[1][1] -
                                                      xy_tablet[0][1]);
        }
    angle = atan2 ((xy_tablet[2][1] - xy_tablet[0][1]),
                   (xy_tablet[2][0] - xy_tablet[0][0]));
    wcos = cos (angle);
    wsin = sin (angle);
    
    /* Calculate a 3 point registration to fake out the window manager's goofyness  */
    
    urx = x_maximum;
    ury = y_maximum;
    
    for (i = 0; i < 3; i++)
        {
        switch (i)
            {
        case 0:
            x_temp = (llx - xy_cntl[0][0]) / rscale_x;
            if (is_log_type)
                y_temp = (lly - log10 (xy_cntl[0][1])) / rscale_y;
            else
                y_temp = (lly - xy_cntl[0][1]) / rscale_y;
            break;
        case 1:
            x_temp = (urx - xy_cntl[0][0]) / rscale_x;
            if (is_log_type)
                y_temp = (lly - log10 (xy_cntl[0][1])) / rscale_y;
            else
                y_temp = (lly - xy_cntl[0][1]) / rscale_y;
            break;
        case 2:
            x_temp = (urx - xy_cntl[0][0]) / rscale_x;
            if (is_log_type)
                y_temp = (ury - log10 (xy_cntl[0][1])) / rscale_y;
            else
                y_temp = (ury - xy_cntl[0][1]) / rscale_y;
            break;
        default:
            break;
            }
        xpoints[i] = wcos * x_temp - wsin * y_temp + xy_tablet[0][0];
        ypoints[i] = wsin * x_temp + wcos * y_temp + xy_tablet[0][1];
        }
    /* Now register the log                                                         */
    
    wmtregistration (log_tnr, xpoints, ypoints, x_origin, y_origin, x_maximum,
                     y_maximum);
    
    /* Renable registration                                                         */
    
    wmtregonoff (ON);
    
    return LOG_DIGITIZE_STEP;
    }
/* END:                                                                             */

/* Function Description -----------------------------------------------------       */
/*Description:                                                                      */
/*    Called from line editor when "End this Segment" button is pressed.            */
/*                                                                                  */
/*Prototype:                                                                        */
/*    static INT lg_end_segment(LE_CONTROL *control);                           */
/*                                                                                  */
/*Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)        */
/*    control         -(LE_CONTROL *)                                               */
/*                                                                                  */
/*Return Value/Status:                                                              */
/*    BUTTON_NOOP                                                                   */
/*                                                                                  */
/*Scope:                                                                            */
/*    PRIVATE to <lg_log_dig>                                                       */
/*                                                                                  */
/*Limitations/Assumptions:                                                          */
/*                                                                                  */
/*-----------------------------------------------------------------------------     */

/* BEGIN:                                                                           */
#if USE_PROTOTYPES
static INT lg_end_segment (LE_CONTROL *control)
#else
static INT lg_end_segment (control)
LE_CONTROL *control;
#endif
    {
    INT status, yes;
    DIG_LOG_STRUCT *log_header = (DIG_LOG_STRUCT *)control->data_pointer;
    
    /* ask user if more is to be digitized. Old code would error out with           */
    /*   arithmetic trap if user cancelled from register new segment                */
    
    wiyesno (mg_message (LG_CONTINUE_PROMPT), "LOG/CANCEL", &yes, &status);
    if (!yes)
        return BUTTON_NOOP;
    
    status = lg_register_segment (log_header);
    if (status != LOG_DIGITIZE_STEP)
        return BUTTON_CANCEL;
    control->default_puck_map = FALSE;
    
    return BUTTON_NOOP;
    }
/* END:                                                                             */

/* Function Description -----------------------------------------------------       */
/*Description:                                                                      */
/*    Called from line editor when "End of trace" button is pressed.                */
/*                                                                                  */
/*Prototype:                                                                        */
/*    static INT lg_trace_done(LE_CONTROL *control);                            */
/*                                                                                  */
/*Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)        */
/*    control         -(LE_CONTROL *)                                               */
/*                                                                                  */
/*Return Value/Status:                                                              */
/*    BUTTON_NOOP                                                                   */
/*                                                                                  */
/*Scope:                                                                            */
/*    PRIVATE to <lg_log_dig>                                                       */
/*                                                                                  */
/*Limitations/Assumptions:                                                          */
/*                                                                                  */
/*-----------------------------------------------------------------------------     */

/* BEGIN:                                                                           */
#if USE_PROTOTYPES
static INT lg_trace_done (LE_CONTROL *control)
#else
static INT lg_trace_done (control)
LE_CONTROL *control;
#endif
    {
    INT open;
    
    le_done (control);
    control->end_edit = TRUE;
    
    /*  Take down the keymap dialog                                                 */
    
    (void) lg_log_cdlg_term (&keymap_dialog, &open);
    
    return BUTTON_NOOP;
    }
/* END:                                                                             */

/* Function Description -----------------------------------------------------       */
/*Description:                                                                      */
/*    Called from line editor when "Cancel" button is pressed.                      */
/*                                                                                  */
/*Prototype:                                                                        */
/*    static INT lg_cancel_trace(LE_CONTROL *control);                          */
/*                                                                                  */
/*Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)        */
/*    control         -(LE_CONTROL *)                                               */
/*                                                                                  */
/*Return Value/Status:                                                              */
/*    BUTTON_NOOP                                                                   */
/*                                                                                  */
/*Scope:                                                                            */
/*    PRIVATE to <lg_log_dig>                                                       */
/*                                                                                  */
/*Limitations/Assumptions:                                                          */
/*                                                                                  */
/*-----------------------------------------------------------------------------     */

/* BEGIN:                                                                           */
#if USE_PROTOTYPES
static INT lg_cancel_trace (LE_CONTROL *control)
#else
static INT lg_cancel_trace (control)
LE_CONTROL *control;
#endif
    {
    INT open;
    
    le_cancel (control);
    control->end_edit = TRUE;
    control->stroke_mode = FALSE;
    control->dial_update = FALSE;
    
    trace_canceled = TRUE;
    
    /*  Take down the keymap dialog                                                 */
    
    (void) lg_log_cdlg_term (&keymap_dialog, &open);
    
    return BUTTON_NOOP;
    }
/* END:                                                                             */

/* Function Description -----------------------------------------------------       */
/*Description:                                                                      */
/*    Flip the pointlist containing a log.                                          */
/*                                                                                  */
/*Prototype:                                                                        */
/*    static INT lg_flip_nlist(DIG_LOG_STRUCT *log_header, NLIST_HEADER *log_nl);   */
/*                                                                                  */
/*Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)        */
/*    log_header      -(DIG_LOG_STRUCT *)                                           */
/*    log_nl          -(NLIST_HEADER *)                                             */
/*                                                                                  */
/*Return Value/Status:                                                              */
/*    SUCCESS - Successful completion.                                              */
/*                                                                                  */
/*Scope:                                                                            */
/*    PRIVATE to <lg_log_dig>                                                       */
/*                                                                                  */
/*Limitations/Assumptions:                                                          */
/*                                                                                  */
/*-----------------------------------------------------------------------------     */

/* BEGIN:                                                                           */
#if USE_PROTOTYPES
static INT lg_flip_nlist (DIG_LOG_STRUCT *log_header, NLIST_HEADER *log_nl)
#else
static INT lg_flip_nlist (log_header, log_nl)
DIG_LOG_STRUCT *log_header;
NLIST_HEADER *log_nl;
#endif
    {
    UINT no_pnts, cur_point, out_pts, cur_stroke;
    NLIST_HEADER nl_flipped;
    VOIDPTR array[2];
    FLOAT *x, *y;
    
    /* Flip the points, if necessary                                                */
    
    nl_inq_nlist_int (*log_nl, NL_NLIST_NPOINTS, (INT *)&no_pnts);
    nl_init_nlist (&nl_flipped, ncol, units, data_size, data_types, value_inc,
                   blocksize);
    nl_set_nlist_info (nl_flipped, NL_NLIST_COLUMN_NAME, ncol, col_list, c_name);
    nl_start_stroke (nl_flipped, &cur_stroke);
    x = (FLOAT *) tc_alloc (sizeof(*x) * (no_pnts + 1));
    y = (FLOAT *) tc_alloc (sizeof(*y) * (no_pnts + 1));
    array[0] = (VOIDPTR)x;
    array[1] = (VOIDPTR)y;
    nl_set_current_point (*log_nl, 1, 1);
    nl_get_points (*log_nl, no_pnts, ncol, col_list, array, &out_pts);
    for (cur_point = 0; cur_point < out_pts AND (log_header->flipped); cur_point++)
        {
        y[cur_point] = log_header->trace_flip_value - y[cur_point];
        }
    nl_add_points (nl_flipped, out_pts, ncol, col_list, array);
    nl_free_nlist (*log_nl);
    nl_copy_nlist (nl_flipped, log_nl);
    nl_free_nlist (nl_flipped);
    
    tc_free ((VOIDPTR)x);
    tc_free ((VOIDPTR)y);
    
    return SUCCESS;
    }
/* END:                                                                             */

/* Function Description -----------------------------------------------------       */
/*Description:                                                                      */
/*    Convert the pointlist containing a log to linear scale.                       */
/*                                                                                  */
/*Prototype:                                                                        */
/*    static INT lg_convert_nlist(NLIST_HEADER   *log_nl, DIG_LOG_STRUCT *log_header);  */
/*                                                                                  */
/*Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)        */
/*    log_nl          -(NLIST_HEADER *)                                             */
/*    log_header      -(DIG_LOG_STRUCT *)                                           */
/*                                                                                  */
/*Return Value/Status:                                                              */
/*    SUCCESS - Successful completion.                                              */
/*                                                                                  */
/*Scope:                                                                            */
/*    PRIVATE to <lg_log_dig>                                                       */
/*                                                                                  */
/*Limitations/Assumptions:                                                          */
/*                                                                                  */
/*-----------------------------------------------------------------------------     */

/* BEGIN:                                                                           */
#if USE_PROTOTYPES
static INT lg_convert_nlist (NLIST_HEADER *log_nl, DIG_LOG_STRUCT *log_header)
#else
static INT lg_convert_nlist (log_nl, log_header)
NLIST_HEADER *log_nl;
DIG_LOG_STRUCT *log_header;
#endif
    {
    UINT no_pnts, cur_point, one_pt, cur_stroke;
    NLIST_HEADER nl_converted;
    VOIDPTR array[2];
    FLOAT x, y;
    DOUBLE con_factor;
    /* **************************************************************************   */
    /* If the trace depth UOM is not equal to the area depth UOM then must convert  */
    
    if (strcmp (log_header->trace_struct.depth_uom, log_trace_uom) != 0)
        if (strcmp (log_header->trace_struct.depth_uom, METERS) == 0)
            {
            con_factor = MTRS_TO_FEET;
            }
        else
            {
            con_factor = 1.0 / MTRS_TO_FEET;
            }
        else
            con_factor = 1.0;
    
    if ((is_log_type) || (con_factor != 1.0))
        {
        nl_inq_nlist_int (*log_nl, NL_NLIST_NPOINTS, (INT *)&no_pnts);
        nl_init_nlist (&nl_converted, ncol, units, data_size, data_types, value_inc,
                       blocksize);
        nl_set_nlist_info (nl_converted, NL_NLIST_COLUMN_NAME, ncol, col_list, c_name);
        nl_start_stroke (nl_converted, &cur_stroke);
        array[0] = (VOIDPTR)&x;
        array[1] = (VOIDPTR)&y;
        for (cur_point = 1; cur_point <= no_pnts; cur_point++)
            {
            nl_set_current_point (*log_nl, cur_point, 1);
            nl_get_points (*log_nl, 1, ncol, col_list, array, &one_pt);
            if (is_log_type)
                y = EXP10 (y);
            x *= con_factor;
            nl_add_points (nl_converted, 1, ncol, col_list, array);
            }
        nl_free_nlist (*log_nl);
        nl_copy_nlist (nl_converted, log_nl);
        nl_free_nlist (nl_converted);
        }
    log_header->trace_struct.top *= con_factor;
    log_header->trace_struct.base *= con_factor;
    
    return SUCCESS;
    }
/* END:                                                                             */

/* Function Description -----------------------------------------------------       */
/*Description:                                                                      */
/*    This routine draws the titles, grid border and anno-                          */
/*    tates the log limits on the screen, using GKS calls.                          */
/*                                                                                  */
/*Prototype:                                                                        */
/*    static INT lg_layout_display(DIG_LOG_STRUCT *log_header);                 */
/*                                                                                  */
/*Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)        */
/*    log_header      -(DIG_LOG_STRUCT *)                                           */
/*                                                                                  */
/*Return Value/Status:                                                              */
/*    None.                                                                         */
/*                                                                                  */
/*Scope:                                                                            */
/*    PRIVATE to <lg_log_dig>                                                       */
/*                                                                                  */
/*Limitations/Assumptions:                                                          */
/*                                                                                  */
/*-----------------------------------------------------------------------------     */

/* BEGIN:                                                                           */
#if USE_PROTOTYPES
static INT lg_layout_display (DIG_LOG_STRUCT *log_header)
#else
static INT lg_layout_display (log_header)
DIG_LOG_STRUCT *log_header;
#endif
    {
    DOUBLE delta_y;                 /* Track width                                  */
    FLOAT xtext, ytext;             /*  Text location                               */
    CHAR text[81];                  /*  Text buffer                                 */
    INT status;
    INT num_tracks;                 /*  Number of tracks to draw                    */
    GKS_REAL space = 0.0, upx = 0.0, upy = 1.0;
    GKS_INT right = 0, normal = 0;
    DOUBLE log_top_y, log_bottom_y;
    DOUBLE x_inc;
    INT y_div, grid_color;
    CHAR format[5];
    /********************************************************************************/
    
    wm_gstxci (log_header->text_color); /*  Set text color                          */
    wm_gschsp (space);
    wm_gstxfp (SIMPLEX, GSTRP);
    
    /*  Post display header                                                         */
    
    xtext = 0.01;
    ytext = ndc_max_y - 0.03;
    status = lg_post_text (xtext, ytext, 0.015, normal, normal, right, upx, upy,
                           "Finder Graphic Systems, Inc.");
    (void) sprintf (text, ": %s", log_header->trace_struct.uwi);
    ytext -= 0.02;
    status = lg_post_text (xtext, ytext, 0.01, normal, normal, right, upx, upy,
                           "Well");
    xtext += 0.1;
    status = lg_post_text (xtext, ytext, 0.01, normal, normal, right, upx, upy, text);
    xtext = 0.01;
    (void) sprintf (text, ": %s", log_header->trace_struct.trace_name);
    ytext -= 0.02;
    status = lg_post_text (xtext, ytext, 0.01, normal, normal, right, upx, upy,
                           "Trace");
    xtext += 0.1;
    status = lg_post_text (xtext, ytext, 0.01, normal, normal, right, upx, upy, text);
    xtext = 0.01;
    (void) sprintf (text, ": %s", log_header->trace_struct.trace_uom);
    ytext -= 0.02;
    status = lg_post_text (xtext, ytext, 0.01, normal, normal, right, upx, upy,
                           "Trace UOM");
    xtext += 0.1;
    status = lg_post_text (xtext, ytext, 0.01, normal, normal, right, upx, upy, text);
    
    /*  Post the grids                                                              */
    
    if (strcmp (log_header->trace_struct.depth_or_time, DEPTH) == 0)
        (void) strcpy (format, DEPTH_FORMAT);
    else
        (void) strcpy (format, TIME_FORMAT);
    
    x_inc = log_header->trace_struct.base - log_header->trace_struct.top;
    
    if (x_inc <= 10.)
        x_inc = 0.1;
    else if (x_inc <= 100.)
        x_inc = 1.;
    else if (x_inc <= 500.)
        x_inc = 5.;
    else if (x_inc < 1000.)
        x_inc = 10.;
    else if (x_inc < 10000.)
        x_inc = 100.;
    else
        x_inc = 1000.;
    
    grid_color = log_header->grid_color;
    num_tracks = log_header->num_offscales_up + log_header->num_offscales_down + 1;
    
    if (is_log_type)
        {
        delta_y = log10 (log_header->top_y) - log10 (log_header->bottom_y);
        log_top_y = EXP10 (log10 (log_header->top_y) + log_header->num_offscales_up *
                           delta_y);
        log_bottom_y = EXP10 (log10 (log_header->bottom_y) -
                              log_header->num_offscales_down * delta_y);
        status = lg_draw_grid (log_header->trace_struct.top,
                               log_header->trace_struct.base, log_bottom_y, log_top_y,
                               1, x_inc, 5, 0, 3., 2., 1., grid_color, grid_color,
                               grid_color, log_header->flipped,
                               log_header->trace_flip_value, num_tracks, format);
        }
    else
        {
        if (num_tracks <= 3)
            y_div = 10 * num_tracks;
        else
            y_div = 5 * num_tracks;
        delta_y = log_header->top_y - log_header->bottom_y;
        log_top_y = log_header->top_y + log_header->num_offscales_up * delta_y;
        log_bottom_y = log_header->bottom_y - log_header->num_offscales_down * delta_y;
        status = lg_draw_grid (log_header->trace_struct.top,
                               log_header->trace_struct.base, log_bottom_y, log_top_y,
                               0, x_inc, 5, y_div, 3., 2., 1., grid_color, grid_color,
                               grid_color, log_header->flipped,
                               log_header->trace_flip_value, num_tracks, format);
        }

     return( SUCCESS );
    }
/* END:                                                                             */

/* Function Description -----------------------------------------------------       */
/*Description:                                                                      */
/*    Help routine to allow text to be posted in NDC coordinates.                   */
/*                                                                                  */
/*Prototype:                                                                        */
/*    static INT lg_post_text(DOUBLE x_ndc,DOUBLE y_ndc,DOUBLE height_ndc,INT gks_align_h,  */
/*                            INT gks_align_v,INT gks_path,DOUBLE gks_upx,DOUBLE gks_upy,   */
/*                            CHAR text[]);                                         */
/*                                                                                  */
/*Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)        */
/*    x_ndc           -(DOUBLE) NDC of string origin (x).                           */
/*    y_ndc           -(DOUBLE) NDC of string origin (y).                           */
/*    height_ndc      -(DOUBLE) Text height in normalized units.                    */
/*    gks_align_h     -(INT) The GKS alignment - horizontal.                        */
/*    gks_align_v     -(INT) The GKS alignment - vertical.                          */
/*    gks_path        -(INT) The GKS path direction.                                */
/*    gks_upx         -(DOUBLE) Character up vector (x).                            */
/*    gks_upy         -(DOUBLE) CHaracter up vector (y).                            */
/*    text            -(CHAR []) The text string to be posted.                      */
/*                                                                                  */
/*Return Value/Status:                                                              */
/*    SUCCESS - Successful completion.                                              */
/*                                                                                  */
/*Scope:                                                                            */
/*    PRIVATE to <lg_log_dig>                                                       */
/*                                                                                  */
/*Limitations/Assumptions:                                                          */
/*                                                                                  */
/*-----------------------------------------------------------------------------     */

/* BEGIN:                                                                           */
#if USE_PROTOTYPES
static INT lg_post_text (DOUBLE x_ndc, DOUBLE y_ndc, DOUBLE height_ndc,
                             INT gks_align_h, INT gks_align_v, INT gks_path,
                             DOUBLE gks_upx, DOUBLE gks_upy, CHAR text[])
#else
static INT lg_post_text (x_ndc, y_ndc, height_ndc, gks_align_h, gks_align_v,
                             gks_path, gks_upx, gks_upy, text)
DOUBLE x_ndc, y_ndc;
DOUBLE height_ndc;
INT gks_align_h;
INT gks_align_v;
INT gks_path;
DOUBLE gks_upx, gks_upy;
CHAR text[];
#endif
    {
    DOUBLE x_wcs, y_wcs;
    DOUBLE height_wcs;
    DOUBLE expansion;
    FLOAT upx, upy;
    
    /*  Convert to WCS                                                              */
    
    x_wcs = (scale_x * x_ndc) + x_origin;
    y_wcs = (scale_y * y_ndc) + y_origin;
    height_wcs = scale_y * height_ndc;
    expansion = scale_x / scale_y;
    
    /*  Set up text attributes                                                      */
    
    wm_gschxp (expansion);          /*  Text expansion(aspect)                      */
    wm_gstxal (gks_align_h, gks_align_v);   /*  Path alignment                      */
    wm_gstxp (gks_path);            /*  Path direction                              */
    wm_gschh (height_wcs);          /*  Character height                            */
    upx = gks_upx;
    upy = gks_upy;
    wm_gschup (upx, upy);           /*  Text up vector                              */
    
    /*  Post the text                                                               */
    
    wm_gtx (x_wcs, y_wcs, text);
    
    return SUCCESS;
    
    }
/* END:                                                                             */

/* Function Description -----------------------------------------------------       */
/*Description:                                                                      */
/*    Draw a log track.                                                             */
/*                                                                                  */
/*Prototype:                                                                        */
/*    static INT lg_draw_grid(DOUBLE x_left,DOUBLE x_right,DOUBLE y_bottom,DOUBLE y_top,    */
/*                    INT grid_type,DOUBLE x_inc,INT x_bold,INT y_div,              */
/*                    DOUBLE bold_line,DOUBLE medium_line,DOUBLE fine_line,         */
/*                    INT bold_color,INT medium_color,                              */
/*                    INT fine_color,BOOL flip_it,DOUBLE flip_factor,INT num_tracks,*/
/*                    CHAR format[]);                                               */
/*                                                                                  */
/*Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)        */
/*    x_left          -(DOUBLE) -\                                                  */
/*    y_left          -(DOUBLE) Horizontal limits (WCS).                            */
/*    y_bottom        -(DOUBLE) -\                                                  */
/*    y_top           -(DOUBLE) Vertical limits(WCS).                               */
/*    grid_type       -(INT) 0 = Linear, 1 = logarithmic.                           */
/*    x_inc           -(DOUBLE) Increment for drawing verticals.                    */
/*    x_bold          -(INT) Frequency of bold verticals.                           */
/*    ydiv            -(INT) Number of y divisions for linear.                      */
/*    bold_line       -(DOUBLE) -\                                                  */
/*    medium_line     -(DOUBLE)  -\                                                 */
/*    fine_line       -(DOUBLE) GKS line thickness codes.                           */
/*    bold_color      -(INT) -\                                                     */
/*    medium_color    -(INT)  -\                                                    */
/*    fine_color      -(INT) Line colors.                                           */
/*    flip_it         -(BOOL) TRUE to flip horizontally.                            */
/*    flip_factor     -(DOUBLE) Flip offset.                                        */
/*    num_tracks      -(INT) Number of tracks.                                      */
/*    format          -(CHAR []) Format for trace annotation.                       */
/*                                                                                  */
/*Return Value/Status:                                                              */
/*    SUCCESS - Successful completion.                                              */
/*                                                                                  */
/*Scope:                                                                            */
/*    PRIVATE to <lg_log_dig>                                                       */
/*                                                                                  */
/*Limitations/Assumptions:                                                          */
/*                                                                                  */
/*-----------------------------------------------------------------------------     */

/* BEGIN:                                                                           */
#if USE_PROTOTYPES
static INT lg_draw_grid (DOUBLE x_left, DOUBLE x_right, DOUBLE y_bottom,
                             DOUBLE y_top, INT grid_type, DOUBLE x_inc, INT x_bold,
                             INT y_div, DOUBLE bold_line, DOUBLE medium_line,
                             DOUBLE fine_line, INT bold_color, INT medium_color,
                             INT fine_color, BOOL flip_it, DOUBLE flip_factor,
                             INT num_tracks, CHAR format[])
#else
static INT lg_draw_grid (x_left, x_right, y_bottom, y_top, grid_type, x_inc,
                             x_bold, y_div, bold_line, medium_line, fine_line,
                             bold_color, medium_color, fine_color, flip_it,
                             flip_factor, num_tracks, format)
DOUBLE x_left, x_right;
DOUBLE y_bottom, y_top;
INT grid_type;
DOUBLE x_inc;
INT x_bold;
INT y_div;
DOUBLE bold_line, medium_line, fine_line;
INT bold_color, medium_color, fine_color;
BOOL flip_it;
DOUBLE flip_factor;
INT num_tracks;
CHAR format[];
#endif
    {
    FLOAT inc;
    FLOAT xmax, xmin, ymin, ymax, y;
    FLOAT index;
    DOUBLE thick;
    INT color, style;
    INT solid = 1, dotted = 2;
    DOUBLE xup, yup, xtext, ytext, height, expansion;
    FLOAT xbuf[2], ybuf[2];
    DOUBLE delta;
    DOUBLE do_bold, bold_inc;
    INT normal = 0, right = 0;
    CHAR text[32];
    
    if (bold_line < 1.0)
        bold_line = 3.0;
    if (medium_line < 1.0)
        medium_line = 2.0;
    if (fine_line < 1.0)
        fine_line = 1.0;
    height = 0.01 * scale_x;
    expansion = scale_y / scale_x;
    wm_gschxp (expansion);
    wm_gschh (height);
    wm_gstxp (right);
    
    switch (grid_type)
        {
        
        /*  Linear grid                                                             */
        
    case 0:
        
        thick = bold_line;
        wm_gsln (solid);            /* Line style                                   */
        wm_gsplci (bold_color);     /* Line color                                   */
        wm_gslwsc (thick);          /* Line thick                                   */
        
        xbuf[0] = x_left;
        xbuf[1] = x_right;
        
        if (flip_it)
            ybuf[0] = ybuf[1] = flip_factor - y_bottom;
        else
            ybuf[0] = ybuf[1] = y_bottom;
        wm_gpl (2, xbuf, ybuf);
        
        if (flip_it)
            ybuf[0] = ybuf[1] = flip_factor - y_top;
        else
            ybuf[0] = ybuf[1] = y_top;
        wm_gpl (2, xbuf, ybuf);
        
        /*  Draw the minor horizontals                                              */
        
        delta = y_top - y_bottom;
        if (!flip_it)
            {
            inc = ceil (delta / y_div);
            ymin = y_bottom;
            ymax = floor (y_top / inc) * inc;
            do_bold = ymin + delta / num_tracks;
            }
        else
            {
            delta = y_bottom - y_top;
            inc = ceil (delta / y_div);
            ymin = y_top;
            ymax = floor (y_bottom / inc) * inc;
            do_bold = ymin + delta / num_tracks;
            }
        for (index = ymin; index <= ymax; index += inc)
            {
            if (TOLER (index, do_bold))
                {
                thick = medium_line;
                wm_gsln (solid);    /* Line style                                   */
                wm_gsplci (medium_color);   /* Line color                           */
                wm_gslwsc (thick);  /* Line thickness                               */
                do_bold += (delta / num_tracks);
                }
            else
                {
                thick = fine_line;
                wm_gsln (dotted);   /* Line style                                   */
                wm_gsplci (fine_color); /* Line color                               */
                wm_gslwsc (thick);  /* Line thickness                               */
                }
            if (flip_it)
                ybuf[0] = ybuf[1] = flip_factor - index;
            else
                ybuf[0] = ybuf[1] = index;
            wm_gpl (2, xbuf, ybuf);
            }
        break;
        
        /*  Logarithmic grid                                                        */
        
    case 1:
        inc = EXP10 (floor (log10 (y_bottom)));
        ymin = y_bottom;
        ymax = y_top;
        index = ymin;
        xbuf[0] = x_left;
        xbuf[1] = x_right;
        flip_factor = log10 (y_bottom) + log10 (y_top);
        while (index <= ymax)
            {
            y = log10 (index);
            if (flip_it)
                y = flip_factor - y;
            if (TOLER (index, ymin) || TOLER (index, ymax))
                {
                thick = bold_line;
                color = bold_color;
                style = solid;
                }
            else
                {
                if (IS_INT (y))
                    {
                    thick = medium_line;
                    color = medium_color;
                    style = solid;
                    }
                else
                    {
                    thick = fine_line;
                    color = fine_color;
                    style = dotted;
                    }
                }
            ybuf[0] = ybuf[1] = y;
            
            wm_gsln (style);        /* Line style                                   */
            wm_gsplci (color);      /* Line color                                   */
            wm_gslwsc (thick);      /* Line width                                   */
            
            wm_gpl (2, xbuf, ybuf); /* Draw horizontal                              */
            
            index += inc;
            inc = EXP10 (floor (log10 (index + 0.001)));
            }
        break;
        
    default:
        break;
        }
    /*  Draw vericals same for both grid types                                      */
    
    xmin = ceil (x_left / x_inc) * x_inc;
    xmax = floor (x_right / x_inc) * x_inc;
    
    ybuf[0] = y_bottom;
    ybuf[1] = y_top;
    
    if (grid_type == 1)
        {
        ybuf[0] = log10 (ybuf[0]);
        ybuf[1] = log10 (ybuf[1]);
        }
    bold_inc = x_inc * x_bold;
    do_bold = xmin;
    wm_gsln (solid);                /* Line style                                   */
    
    for (index = xmin; index <= xmax; index += x_inc)
        {
        if (TOLER (index, do_bold))
            {
            thick = medium_line;
            wm_gsplci (medium_color);   /* Line color                               */
            wm_gslwsc (thick);      /* Line width                                   */
            do_bold += bold_inc;
            }
        else
            {
            thick = fine_line;
            wm_gsplci (fine_color); /* Line color                                   */
            wm_gslwsc (thick);      /* Line width                                   */
            }
        xbuf[0] = xbuf[1] = index;
        wm_gpl (2, xbuf, ybuf);     /* Draw verticals                               */
        }
    /*  Vertical Annotation                                                         */
    
    xtext = x_left - 0.01 * (x_right - x_left);
    ytext = y_bottom;
    if (grid_type == 1)
        ytext = log10 (ytext);
    if (flip_it)
        ytext = flip_factor - ytext;
    wm_gstxal (normal, normal);
    xup = -1.;
    yup = 0.;
    wm_gschup (xup, yup);
    (void) sprintf (text, SCALE_FORMAT, y_bottom);
    wm_gtx (xtext, ytext, text);
    ytext = y_top;
    if (grid_type == 1)
        ytext = log10 (ytext);
    if (flip_it)
        ytext = flip_factor - ytext;
    wm_gstxal (3, normal);
    (void) sprintf (text, SCALE_FORMAT, y_top);
    wm_gtx (xtext, ytext, text);
    
    /*  Horizontal annotation                                                       */
    
    
    if (grid_type == 1)
        {
        ymin = log10 (y_bottom);
        ymax = log10 (y_top);
        ytext = ymin - 0.01 * (ymax - ymin);
        }
    else
        {
        ytext = y_bottom - 0.01 * (y_top - y_bottom);
        }
    if (flip_it)
        ytext = flip_factor - ytext;
    
    for (index = xmin; index <= xmax; index += bold_inc)
        {
        (void) sprintf (text, format, index);
        wm_gtx (index, ytext, text);
        }
    thick = fine_line;
    wm_gslwsc (thick);
    
    return SUCCESS;
    }
/* END:                                                                             */

/* Function Description -----------------------------------------------------       */
/*Description:                                                                      */
/*    Thin the nlist by eliminating consectutive segments with equal slopes.        */
/*                                                                                  */
/*Prototype:                                                                        */
/*    static INT lg_thin_nlist(NLIST_HEADER *log_nl);                           */
/*                                                                                  */
/*Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)        */
/*    log_nl          -(NLIST_HEADER *)                                             */
/*                                                                                  */
/*Return Value/Status:                                                              */
/*    SUCCESS - Successful completion.                                              */
/*                                                                                  */
/*Scope:                                                                            */
/*    PRIVATE to <lg_log_dig>                                                       */
/*                                                                                  */
/*Limitations/Assumptions:                                                          */
/*                                                                                  */
/*-----------------------------------------------------------------------------     */

/* BEGIN:                                                                           */
#if USE_PROTOTYPES
static INT lg_thin_nlist (NLIST_HEADER *log_nl)
#else
static INT lg_thin_nlist (log_nl)
NLIST_HEADER *log_nl;
#endif
    {
    UINT cur_stroke;
    INT status;
    NLIST_HEADER nl_thin;
    VOIDPTR array1[2], array2[2];
    FLOAT x1, y1, x2, y2;
    DOUBLE slope1, slope2, min_diff = 0.002;
    
    nl_init_nlist (&nl_thin, ncol, units, data_size, data_types, value_inc, blocksize);
    nl_set_nlist_info (nl_thin, NL_NLIST_COLUMN_NAME, ncol, col_list, c_name);
    nl_start_stroke (nl_thin, &cur_stroke);
    array1[0] = (VOIDPTR)&x1;
    array1[1] = (VOIDPTR)&y1;
    nl_first_point (*log_nl, ncol, col_list, array1);
    nl_add_points (nl_thin, 1, ncol, col_list, array1);
    array2[0] = (VOIDPTR)&x2;
    array2[1] = (VOIDPTR)&y2;
    nl_next_point (*log_nl, ncol, col_list, array2);
    slope1 = atan2 ((y2 - y1), (x2 - x1));
    x1 = x2;
    y1 = y2;
    status = 0;
    while (status == 0)
        {
        status = nl_next_point (*log_nl, ncol, col_list, array2);
        if (status == 0)
            {
            slope2 = atan2 ((y2 - y1), (x2 - x1));
            if (fabs (slope2 - slope1) > min_diff)
                {
                nl_add_points (nl_thin, 1, ncol, col_list, array1);
                slope1 = slope2;
                }
            x1 = x2;
            y1 = y2;
            }
        }
    nl_add_points (nl_thin, 1, ncol, col_list, array1);
    nl_free_nlist (*log_nl);
    nl_copy_nlist (nl_thin, log_nl);
    nl_free_nlist (nl_thin);
    
    return SUCCESS;
    }

/* **********************************************************************           */
/*                                                                                  */
/*  ROUTINE NAME:  WE_VERIFY_WELL                                                   */
/*                                                                                  */
/*  AUTHOR:        Doug Canfield                                                    */
/*                                                                                  */
/*  PURPOSE:       To verify that a UWI exists in the WELLS db table.               */
/*                                                                                  */
/*  CALLING:       status = we_verify_well(pverify_well_structure)                  */
/*                                                                                  */
/*                 Where verify_well_structure has three data items...              */
/*                                                                                  */
/*                   uwi:         The uwi of the well to be tested.                 */
/*                   show_well:   A flag set to TRUE if a dialog is to              */
/*                                be presented to the operator requesting           */
/*                                acceptance of the well.                           */
/*                   well_exists: Set to TRUE if the correct well exists.           */
/*                                                                                  */
/* **********************************************************************           */

typedef struct 
    {
    VERIFY_WELL_STRUCT *pvws;
    WELL_STRUCT *pwell;
    } VERIFY_STRUCT;

/* **********************************************************************           */
/*                                                                                  */
/*  Beginning of code...ENTRY POINT WE_VERIFY_WELL!                                 */
/*                                                                                  */
/* **********************************************************************           */

static INT we_verify_well (pverify_well_struct)
VERIFY_WELL_STRUCT *pverify_well_struct;

    {
    VERIFY_STRUCT verify_struct;
    VERIFY_STRUCT *pverify_struct;
    INT status;
    INT  class ;
    INT id;
    INT item;
    
    pverify_struct = &verify_struct;
    
    status = we_get_well_header (pverify_well_struct->uwi, WELL_HDR_1_DETAIL,
                                 &pverify_struct->pwell);
    
    /*  If the well does not exist, set                                             */
    /*  well_exists to FALSE and return                                             */
    
    if (status IS_NOT_EQUAL_TO SUCCESS)
        {
        pverify_well_struct->well_exists = FALSE;
        }
    /*  If the well exists, check to                                                */
    /*  see of the program wants the                                                */
    /*  operator to verify that it is                                               */
    /*  indeed the correct well.                                                    */
    else
        {
        if (pverify_well_struct->show_well IS_NOT_EQUAL_TO TRUE)
            pverify_well_struct->well_exists = TRUE;    /* Don't verify             */
        
        else
            {                       /*  Verify!                                     */
            pverify_struct->pvws = pverify_well_struct;
            status = fi_push ();    /* Push FINDER environment.                     */
            /* (Prepare to request events)                                          */
            
            status = we_verify_well_cdlg_init (pverify_struct);
            
            /* "Events can only be generated                                        */
            /*    from resources defined since                                      */
            /*    the last fi_push. Otherwise,                                      */
            /*    there is an application-                                          */
            /*    manager bug.", B. Jaeck                                           */
            /*    March 26, 1987.                                                   */
            
            status = wirqevt (0, &class , &id, &item);
            
            switch (item)
                {                   /* Set the flag & quit                          */
            case WE_VERIFY_ACCEPT:
                pverify_struct->pvws->well_exists = TRUE;
                break;
            case WE_VERIFY_REJECT:
                pverify_struct->pvws->well_exists = FALSE;
                break;
                }
            status = am_release_resource (id);  /* Close the CDLG                   */
            status = fi_pop ();     /* Pop FINDER env                               */
            }
        }
    return status;
    }

/* **********************************************************************           */
/*                                                                                  */
/*  WE_VERIFY_WELL_CDLG_INIT                                                        */
/*  Put up the custom dialog box to display information from the well               */
/*  retrieved by we_get_well_header.                                                */
/*                                                                                  */
/* **********************************************************************           */

static INT we_verify_well_cdlg_init (pverify_struct)
VERIFY_STRUCT *pverify_struct;

    {
    INT status;
    INT id;
    CHAR text[16];
    WELL_STRUCT *pwell = pverify_struct->pwell;
    WELL_HDR_1 *pwell1 = (WELL_HDR_1 *)pwell->detail.ptr;
    
    status = am_define_resource (AM_DIALOG, /* Define the CDLG box.                 */
            WE_VERIFY_WELL_CDLG_FILE, WE_VERIFY_WELL_SELECTION, NULL_FUNCTION_PTR,
                                 (VOIDPTR)pverify_struct, &id);
    
    /* Load the text fields.                                                        */
    
    status = wiscdt (id, WE_VERIFY_UWI_FIELD, pwell->uwi);
    status = wiscdt (id, WE_VERIFY_OPER_FIELD, pwell1->operator);
    status = wiscdt (id, WE_VERIFY_NAME_FIELD, pwell1->name);
    status = wiscdt (id, WE_VERIFY_NUMB_FIELD, pwell1->number);
    (void) sprintf (text, "%8.2f %s", pwell1->elevation.value, pwell1->elevation.ref);
    status = wiscdt (id, WE_VERIFY_ELEV_FIELD, text);
    
    /* Make the CDLG visible.                                                       */
    
    status = wi_open_dialog (id);
    
    return status;
    }
