/* DEC/CMS REPLACEMENT HISTORY, Element XS_MAIN.C*/
/*  10    5-MAR-1992 13:17:24 JESSIE "(SPR 3942) ture off menu items if needed"*/
/*  9     3-MAR-1992 15:10:27 JEFF "(SPR 6657) removed preview layout menu item association"*/
/* *8    15-AUG-1991 11:39:15 JULIAN "(SPR 0) fixed compiling error"*/
/* *7     9-AUG-1991 12:42:36 JULIAN "(SPR 3373) added loop when saving wrong name"*/
/* *6     2-AUG-1991 17:24:41 JULIAN "(SPR 3373) removed message when cancel from well retr"*/
/* *5    24-JUL-1991 16:06:08 JULIAN "(SPR 3373) added check when fail on retrieve wells"*/
/* *4    15-OCT-1990 15:19:11 GILLESPIE "(SPR 1000) Merge Ernie deltas"*/
/*  2B1  12-OCT-1990 18:11:29 GILLESPIE "Merge Ernie Deltas"*/
/* *3    17-AUG-1990 22:40:59 VINCE "(SPR 5644) Code Cleanup"*/
/* *2    26-JUN-1989 11:06:44 GILLESPIE "(SPR 20) Fix castings"*/
/* *1    19-JUN-1989 13:44:41 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element XS_MAIN.C*/
/* DEC/CMS REPLACEMENT HISTORY, Element XS_MAIN.C                                   */
/* *2    12-OCT-1990 00:00:28 SCD "(SPR 6075) plot composer checkin"                */
/* *1    13-AUG-1990 17:25:52 SYSTEM "Finder 6.1"                                   */
/* DEC/CMS REPLACEMENT HISTORY, Element XS_MAIN.C                                   */
/******************************************************************************     */
/*                                                                                  */
/*                Copyright Exploration Systems, Inc. 1989                          */
/*         Unpublished -- All rights reserved                                       */
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
#include "esi_xs_defs.h"

#ifndef ESI_AM_H
#include "esi_am.h"
#endif

#ifndef ESI_AS_YESNO_H
#include "esi_as_yesno.h"
#endif

#ifndef ESI_WI_H
#include "esi_wi.h"
#endif

#ifndef ESI_MG_H
#include "esi_mg.h"
#endif

#ifndef ESI_TC_H
#include "esi_tc.h"
#endif

#ifndef ESI_TS_H
#include "esi_ts.h"
#endif

#ifndef ESI_QS_H
#include "esi_qs.h"
#endif

#ifndef ESI_WM_H
#include "esi_wm.h"
#endif

#ifndef ESI_GKS_H
#include "esi_gks.h"
#endif

#ifndef ESI_AS_SAVE_H
#include "esi_as_save.h"
#endif

#ifndef ESI_XS_MSG_H
#include "esi_xs_msg.h"
#endif

#ifndef ESI_CONF_WS_H
#include "esi_conf_ws.h"
#endif

#ifndef XS_MENUS_RH
#include "xs_menus.rh"
#endif

#ifndef XS_CDLGS_RH
#include "xs_cdlgs.rh"
#endif

#ifndef ESI_AM_ERR_H
#include "esi_am_err.h"
#endif

#include "esi_sl_err.h"

#define AS_YESNO_YES   1
#define AS_YESNO_NO    0

#if USE_PROTOTYPES

static INT xs_init (XS_STRUCT *pxs);
static INT xs_layout_menu_server (INT id, INT item, XS_STRUCT *pxs);
static INT xs_edit_menu_server (INT id, INT item, XS_STRUCT *pxs);
static INT xs_wells_menu_server (INT id, INT item, XS_STRUCT *pxs);
static INT xs_tops_menu_server (INT id, INT item, XS_STRUCT *pxs);
static INT xs_msg_cdlg_server (INT id, INT item, XS_STRUCT *pxs);
static INT xs_quit (XS_STRUCT *pxs);

#else       /* no prototypes */

static INT xs_init ();
static INT xs_layout_menu_server ();
static INT xs_edit_menu_server ();
static INT xs_wells_menu_server ();
static INT xs_tops_menu_server ();
static INT xs_msg_cdlg_server ();
static INT xs_quit ();

#endif      /* of #if USE_PROTOTYPES */

/* File Description ---------------------------------------------------------       */
/*Overview:                                                                         */
/*    Main entry to the Cross Section System.                                       */
/*                                                                                  */
/*Prototypes and one line descriptions of each function in the file.                */
/*Public_Functions:                                                                 */
/*    extern INT xs_main(VOIDPTR junk);                                             */
/*                                                                                  */
/*    publicdef INT xs_term(XS_STRUCT *pxs );                                       */
/*                                                                                  */
/*Private_Functions:                                                                */
/*    static INT xs_init (XS_STRUCT *pxs);                                      */
/*                                                                                  */
/*    static INT xs_layout_menu_server (INT id, INT item, XS_STRUCT *pxs);      */
/*                                                                                  */
/*    static INT xs_edit_menu_server (INT id, INT item, XS_STRUCT *pxs);        */
/*                                                                                  */
/*    static INT xs_wells_menu_server (INT id, INT item, XS_STRUCT *pxs);       */
/*                                                                                  */
/*    static INT xs_tops_menu_server (INT id, INT item, XS_STRUCT *pxs);        */
/*                                                                                  */
/*    static INT xs_msg_cdlg_server (INT id, INT item, XS_STRUCT *pxs);         */
/*                                                                                  */
/*    static INT xs_quit (XS_STRUCT *pxs);                                      */
/*                                                                                  */
/*                                                                                  */
/*-----------------------------------------------------------------------------     */

/* Function Description -----------------------------------------------------       */
/*Description:                                                                      */
/*    Calls the system initialization routines & checks for success.                */
/*                                                                                  */
/*Prototype:                                                                        */
/*    extern INT xs_main(VOIDPTR junk);                                             */
/*                                                                                  */
/*Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)        */
/*    junk            -(BYTE *)                                                     */
/*                                                                                  */
/*Return Value/Status:                                                              */
/*    SUCCESS - Successful completion.                                              */
/*                                                                                  */
/*Scope:                                                                            */
/*    PUBLIC                                                                        */
/*                                                                                  */
/*Limitations/Assumptions:                                                          */
/*                                                                                  */
/*-----------------------------------------------------------------------------     */

/* BEGIN:                                                                           */
#if USE_PROTOTYPES
extern INT xs_main (VOIDPTR junk)
#else
extern INT xs_main (junk)
VOIDPTR junk;
#endif
    {
    INT status, tag_status;
    XS_STRUCT *pxs;
    
    tag_status = am_declare ("Cross Section");
    
    /*  Try to open the global workspace.                                           */
    /*  If we are successful, continue.                                             */
    /*  If not, allocate & initialize it.                                           */
    
    status = am_open_workspace (XS_WORKSPACE, AM_GLOBAL, (VOIDPTR *)&pxs);
    if (status == SUCCESS)
        {
        /*  If cross section is already active                                      */
        /*  front the message dialog to orient                                      */
        /*  the user and then return.                                               */
        
        if (tag_status == AM_ERR_DUPLICATE_TAG)
            {
            wiopcd (pxs->msg_cdlg_id);
            return SUCCESS;
            }
        else
            {
            status = xs_init (pxs);
            }
        }
    else
        {
        status = am_define_workspace (XS_WORKSPACE, AM_GLOBAL, sizeof(XS_STRUCT),
                                      (VOIDPTR *)&pxs);
        
        if (status == SUCCESS)
            {
            pxs->ptemp = (LAYOUT_STRUCT *) am_allocate (AM_SYSTEM,
                                                        sizeof(LAYOUT_STRUCT));
            pxs->plo = (LAYOUT_STRUCT *) am_allocate (AM_SYSTEM,
                                                      sizeof(LAYOUT_STRUCT));
            
            if (pxs->ptemp == (LAYOUT_STRUCT *) NULL OR pxs->plo ==
                (LAYOUT_STRUCT *)NULL)
                status = FAIL;
            else
                status = SUCCESS;
            }
        if (status == SUCCESS)
            {
            status = xsz_init_struct (pxs); /*  Set up a minimal structure.         */
            status = xs_init (pxs); /*  Open the Menus & msg CDLG.                  */
            }
        else
            {
            am_message (AM_STATUS, mg_message (XS_WORKSPACE_NOT_INIT));
            er_log (mg_message (XS_WORKSPACE_NOT_INIT));
            }
        if (status == SUCCESS)
            {
            /*          status = xs_retv_last_disp(pxs);                            */
            /* Force a layout retrieve.                                             */
            status = xs_retlayout_cdlg_init (pxs);
            /*        status = wiencdi( pxs->ret_layout_cdlg_id, XS_RETLAYOUT_CANCEL, 0 );  */
            }
        else
            {                       /* Init failed, try to clean up.                */
            am_message (AM_ALERT, mg_message (XS_INIT_FAILED));
            status = xs_term (pxs);
            }
        }
    return SUCCESS;
    }
/* END:                                                                             */

/* Function Description -----------------------------------------------------       */
/*Description:                                                                      */
/*    Open the menus and the message custom dialog.                                 */
/*                                                                                  */
/*Prototype:                                                                        */
/*    static INT xs_init(XS_STRUCT *pxs);                                       */
/*                                                                                  */
/*Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)        */
/*    pxs             -(XS_STRUCT *)                                                */
/*                                                                                  */
/*Return Value/Status:                                                              */
/*    SUCCESS - Successful completion.                                              */
/*    FAIL    - Unsuccessful completion.                                            */
/*                                                                                  */
/*Scope:                                                                            */
/*    PRIVATE to <xs_main.c>                                                        */
/*                                                                                  */
/*Limitations/Assumptions:                                                          */
/*                                                                                  */
/*-----------------------------------------------------------------------------     */

/* BEGIN:                                                                           */
#if USE_PROTOTYPES
static INT xs_init (XS_STRUCT *pxs)
#else
static INT xs_init (pxs)
XS_STRUCT *pxs;
#endif
    {
    INT status;
    INT nl_status, n_wells;
    INT id;
    INT num;
    CHAR message[128];
    
    /* Load Menus & main CDLG                                                       */
    status = am_define_resource (AM_MENU, XS_MENUS_FILE, XS_LAYOUT_MENU,
                                 xs_layout_menu_server, &pxs->layout_menu_id, pxs)
    OR am_define_resource (AM_MENU, XS_MENUS_FILE, XS_EDIT_MENU, xs_edit_menu_server,
                           &pxs->edit_menu_id, pxs)
    OR am_define_resource (AM_MENU, XS_MENUS_FILE, XS_WELLS_MENU, xs_wells_menu_server,
                           &pxs->wells_menu_id, pxs)
    OR am_define_resource (AM_MENU, XS_MENUS_FILE, XS_TOPS_MENU, xs_tops_menu_server,
                           &pxs->tops_menu_id, pxs)
    OR am_define_resource (AM_DIALOG, XS_CDLGS_FILE, XS_MESSAGE, xs_msg_cdlg_server,
                           &pxs->msg_cdlg_id, pxs);
    
    /* Set menu states.                                                             */
    if (status == SUCCESS)
        status = wienmni (pxs->edit_menu_id, STOP_ALL_MENU_ITEM, OFF)
        OR wienmni (pxs->layout_menu_id, PREVIEW_MENU_ITEM, OFF)
        OR wienmni (pxs->tops_menu_id, RETV_TOPS_MENU_ITEM, ON)
        OR wienmni (pxs->tops_menu_id, DISPLAY_TOPS_MENU_ITEM, OFF)
        OR wienmni (pxs->tops_menu_id, DIG_TOPS_MENU_ITEM, OFF)
        OR wienmni (pxs->tops_menu_id, SAVE_TOPS_MENU_ITEM, OFF);
    
    nl_status = nl_inq_nlist_int (pxs->well_list, NL_NLIST_NPOINTS, &n_wells);
    if (nl_status < 0 OR n_wells <= 0)
        {
        pxs->valid_well_list = FALSE;
        }
    else
        {
        pxs->valid_well_list = TRUE;
        }
    xs_cs_menu_mgr (pxs);
    
    /* Make the menus & msg CDLG visible.                                           */
    if (status == SUCCESS)
        status = wivismn (pxs->layout_menu_id, 1)
        OR wivismn (pxs->edit_menu_id, 1)
        OR wivismn (pxs->wells_menu_id, 1)
        OR wivismn (pxs->tops_menu_id, 1)
        OR wiopcd (pxs->msg_cdlg_id);
    
    if (status == SUCCESS)
        {                           /* Identify layout & prompt for menu pick.      */
        sprintf (message, "%s  %s", mg_message (XS_LAYOUT_IS), pxs->layout_name);
        status = wiscdt (pxs->msg_cdlg_id, XS_MESSAGE2, message);
        
        status = wiscdt (pxs->msg_cdlg_id, XS_MESSAGE1,
                         mg_message (XS_PICK_MENU_ITEM));
        status = SUCCESS;
        }
    return status;
    }
/* END:                                                                             */

#ifdef ACTIVATED

/* Function Description -----------------------------------------------------       */
/*Description:                                                                      */
/*    !!!  DE-ACTIVATED for performance  !!!                                        */
/*                                                                                  */
/*    Attempts to retrieve the last displayed layout.                               */
/*                                                                                  */
/*Prototype:                                                                        */
/*    static INT xs_retv_last_disp(XS_STRUCT *pxs);                             */
/*                                                                                  */
/*Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)        */
/*    pxs             -(XS_STRUCT *)                                                */
/*                                                                                  */
/*Return Value/Status:                                                              */
/*    SUCCESS - Successful completion.                                              */
/*    FAIL    - Unsuccessful completion.                                            */
/*                                                                                  */
/*Scope:                                                                            */
/*    PRIVATE to <xs_main.c>                                                        */
/*                                                                                  */
/*Limitations/Assumptions:                                                          */
/*                                                                                  */
/*-----------------------------------------------------------------------------     */

/* BEGIN:                                                                           */
#if USE_PROTOTYPES
static INT xs_retv_last_disp (XS_STRUCT *pxs)
#else
static INT xs_retv_last_disp (pxs)
XS_STRUCT *pxs;
#endif
    {
    INT status;
    INT id;
    INT num;
    CHAR message[128];
    PROJECT_NAME project;
    ANALYST_NAME analyst;
    
    num = 0;
    status = qs_inq_c (QS_PROJECT_NAME, project, &num);
    num = 0;
    status = qs_inq_c (QS_FINDER_ACCOUNT, analyst, &num);
    
    /* Retrieve the last displayed layout.                                          */
    
    if (status == SUCCESS)
        {
        strcpy (message, mg_message (XS_RESTORING));    /* set the msg              */
        status = wiscdt (pxs->msg_cdlg_id, XS_MESSAGE1, message);
        
        status = xs_get_layout (pxs, LAST_DISPLAYED_LAYOUT, project, analyst);
        
        if (status != SUCCESS)
            am_message (AM_ALERT, mg_message (XS_RETV_LAYOUT_FAILED));
        else
            {
            strcpy (pxs->layout_name, LAST_DISPLAYED_LAYOUT);   /* Set the msg      */
            sprintf (message, "%s  %s", mg_message (XS_LAYOUT_IS),/* to show the    */
                    LAST_DISPLAYED_LAYOUT); /* layout name                          */
            wiscdt (pxs->msg_cdlg_id, XS_MESSAGE2, message);
            }
        status = wiscdt (pxs->msg_cdlg_id, XS_MESSAGE1, /* disp prompt              */
                mg_message (XS_PICK_MENU_ITEM));
        
        status = SUCCESS;
        }
    return status;
    }
/* END:                                                                             */

#endif

/* Function Description -----------------------------------------------------       */
/*Description:                                                                      */
/*  Receives events from the layout menu (now "Cross Section" menu) and             */
/*  launches the appropriate process.                                               */
/*                                                                                  */
/*Prototype:                                                                        */
/*    static INT xs_layout_menu_server(INT id,INT item, XS_STRUCT *pxs );       */
/*                                                                                  */
/*Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)        */
/*    id              -(INT)                                                        */
/*    item            -(INT)                                                        */
/*    pxs             -(XS_STRUCT *)                                                */
/*                                                                                  */
/*Return Value/Status:                                                              */
/*    SUCCESS - Successful completion.                                              */
/*    FAIL    - Unsuccessful completion.                                            */
/*                                                                                  */
/*Scope:                                                                            */
/*    PRIVATE to <xs_main.c>                                                        */
/*                                                                                  */
/*Limitations/Assumptions:                                                          */
/*                                                                                  */
/*-----------------------------------------------------------------------------     */

/* BEGIN:                                                                           */
#if USE_PROTOTYPES
static INT xs_layout_menu_server (INT id, INT item, XS_STRUCT *pxs)
#else
static INT xs_layout_menu_server (id, item, pxs)
INT id;
INT item;
XS_STRUCT *pxs;
#endif
    {
    INT status;
    
    switch (item)
        {
    case NEW_MENU_ITEM:
        status = xs_newlayout_cdlg_init (pxs);
        break;
    case RETRIEVE_MENU_ITEM:
        status = xs_retlayout_cdlg_init (pxs);
        break;
    case SAVE_MENU_ITEM:
        pxs->quit_flag = FALSE;
        status = xs_save_init (pxs);
        break;
    case DELETE_MENU_ITEM:
        status = xs_dellayout_cdlg_init (pxs);
        break;
    case PREVIEW_MENU_ITEM:
        status = am_message (AM_ALERT,
                       "We're terribly sorry.  We haven't yet written this function.");
        break;
    case DRAW_MENU_ITEM:
        status = xs_display_cross_section (pxs);
        break;
    case REDRAW_MENU_ITEM:
        status = xs_redraw_cross_section (pxs);
        break;
    case PLOT_MENU_ITEM:
        status = xs_send_to_plotter (pxs);
        break;
    case PLOT_LAYOUT_ITEM:
        status = or_run_oracle_form ("XS_PLOT_COMPOSER");
        break;
    case QUIT_XS_MENU_ITEM:
        status = xs_quit (pxs);
        break;
        }
    return SUCCESS;
    }
/* END:                                                                             */

/* Function Description -----------------------------------------------------       */
/*Description:                                                                      */
/*  Receives events from the edit menu (now "Preferences" menu)     and             */
/*  launches the appropriate process.                                               */
/*                                                                                  */
/*Prototype:                                                                        */
/*    static INT xs_edit_menu_server(INT id,INT item, XS_STRUCT *pxs );         */
/*                                                                                  */
/*Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)        */
/*    id              -(INT)                                                        */
/*    item            -(INT)                                                        */
/*    pxs             -(XS_STRUCT *)                                                */
/*                                                                                  */
/*Return Value/Status:                                                              */
/*    SUCCESS - Successful completion.                                              */
/*                                                                                  */
/*Scope:                                                                            */
/*    PRIVATE to <xs_main.c>                                                        */
/*                                                                                  */
/*Limitations/Assumptions:                                                          */
/*                                                                                  */
/*-----------------------------------------------------------------------------     */

/* BEGIN:                                                                           */
#if USE_PROTOTYPES
static INT xs_edit_menu_server (INT id, INT item, XS_STRUCT *pxs)
#else
static INT xs_edit_menu_server (id, item, pxs)
INT id;
INT item;
XS_STRUCT *pxs;
#endif
    {
    INT status;
    
    switch (item)
        {
    case CHANGE_ALL_MENU_ITEM:
        status = xs_start_chg_all (pxs);
        break;
    case STOP_ALL_MENU_ITEM:
        status = xs_stop_chg_all (pxs);
        break;
    case TITLE_BLOCK_ITEM:
        status = or_run_oracle_form ("XS_TITLE_BLOCK");
        break;
    case TYPE_MENU_ITEM:
        status = xs_type_cdlg_init (pxs);
        break;
    case HORIZ_SCALE_MENU_ITEM:
        status = xs_horizontal_cdlg_init (pxs);
        break;
    case LOG_HEADER_MENU_ITEM:
        status = xs_hdr_cdlg_init (pxs);
        break;
    case DEPTH_SCALE_MENU_ITEM:
        status = xs_depth_scale_cdlg_init (pxs);
        break;
    case INTERVAL_MENU_ITEM:
        status = xs_dataint_cdlg_init (pxs);
        break;
    case TRACK_DEF_MENU_ITEM:
        status = xs_trackdef_cdlg_init (pxs);
        break;
    case TRACE_SEL_MENU_ITEM:
        status = xs_tracesel_cdlg_init (pxs);
        break;
    case TRACE_ATT_MENU_ITEM:
        status = xs_traceattr_cdlg_init (pxs);
        break;
#if 0
    case PATTERN_MENU_ITEM:
        status = xs_shading_cdlg_init (pxs);
        break;
    case ANNOTATE_MENU_ITEM:
        status = xs_annotate_cdlg_init (pxs);
        break;
        /* these are currently defunct until code is written                        */
#endif
        }
    return SUCCESS;
    }
/* END:                                                                             */

/* Function Description -----------------------------------------------------       */
/*Description:                                                                      */
/*    Receives events from the wells menu & launches the correct process.           */
/*                                                                                  */
/*Prototype:                                                                        */
/*    static INT xs_wells_menu_server(INT id, INT item, XS_STRUCT *pxs );       */
/*                                                                                  */
/*Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)        */
/*    id              -(INT)                                                        */
/*    item            -(INT)                                                        */
/*    pxs             -(XS_STRUCT *)                                                */
/*                                                                                  */
/*Return Value/Status:                                                              */
/*    SUCCESS - Successful completion.                                              */
/*                                                                                  */
/*Scope:                                                                            */
/*    PRIVATE to <xs_main.c>                                                        */
/*                                                                                  */
/*Limitations/Assumptions:                                                          */
/*                                                                                  */
/*-----------------------------------------------------------------------------     */

/* BEGIN:                                                                           */
#if USE_PROTOTYPES
static INT xs_wells_menu_server (INT id, INT item, XS_STRUCT *pxs)
#else
static INT xs_wells_menu_server (id, item, pxs)
INT id;
INT item;
XS_STRUCT *pxs;
#endif
    {
    INT status;
    
    switch (item)
        {
        /*                               Inactive code for now.                     */
        /*     case CURRENT_WELL_MENU_ITEM:                                         */
        /*          status = xs_we_use_cur_well( pxs );                             */
        /*          if (status == SUCCESS)                                          */
        /*              pxs->valid_well_list = TRUE;                                */
        /*          break;                                                          */
        /*                                                                          */
    case CURRENT_LIST_MENU_ITEM:
        status = xs_we_use_cur_we_list (pxs);
        if (status == SUCCESS)
            pxs->valid_well_list = TRUE;
        break;
    case DIG_TRAVERSE_MENU_ITEM:
        status = xs_we_dgz_traverse (pxs);
        if (status == SUCCESS)
            pxs->valid_well_list = TRUE;
        break;
    case SAVE_TRAVERSE_MENU_ITEM:
	FOREVER
	  {
	    status = xs_we_save_traverse (pxs);
	    if(status != SUCCESS)
	      am_message(AM_ALERT,mg_message(status));
	    if(status != SL_INVALID_NAME) break;
	  }
	break;
    case RETV_TRAVERSE_MENU_ITEM:
        status = xs_we_rtr_traverse (pxs);
        if (status == SUCCESS)
            pxs->valid_well_list = TRUE;
	else if(status != CANCEL)
            am_message (AM_ALERT, mg_message (XS_NO_WELL_IN_TRAVERSE));
        break;
    case SHOW_WELLS_MENU_ITEM:
        status = xs_we_show_well_list (pxs);
        break;
        }
    status = xs_cs_menu_mgr (pxs);
    return SUCCESS;
    }
/* END:                                                                             */

/* Function Description -----------------------------------------------------       */
/*Description:                                                                      */
/*    Receives events from the TOPS menu and launches the proper process.           */
/*                                                                                  */
/*Prototype:                                                                        */
/*    static INT xs_tops_menu_server(INT id,INT item, XS_STRUCT *pxs );         */
/*                                                                                  */
/*Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)        */
/*    id              -(INT)                                                        */
/*    item            -(INT)                                                        */
/*    pxs             -(XS_STRUCT *)                                                */
/*                                                                                  */
/*Return Value/Status:                                                              */
/*    SUCCESS - Successful completion.                                              */
/*                                                                                  */
/*Scope:                                                                            */
/*    PRIVATE to <xs_main.c>                                                        */
/*                                                                                  */
/*Limitations/Assumptions:                                                          */
/*                                                                                  */
/*-----------------------------------------------------------------------------     */

/* BEGIN:                                                                           */
#if USE_PROTOTYPES
static INT xs_tops_menu_server (INT id, INT item, XS_STRUCT *pxs)
#else
static INT xs_tops_menu_server (id, item, pxs)
INT id;
INT item;
XS_STRUCT *pxs;
#endif
    {
    INT status;
    INT front_window_id;
    
    switch (item)
        {
    case RETV_TOPS_MENU_ITEM:
        status = xs_rtr_tops_init (pxs);
        break;
    case DISPLAY_TOPS_MENU_ITEM:
        wm_inq_front_window (&front_window_id);
        if (front_window_id != pxs->drawing.window_id)
            {
            wmwfront (pxs->drawing.window_id);
            wm_grsgwk (MAIN_WKID);
            }
        status = xs_dsp_tops_init (pxs);
        break;
    case DIG_TOPS_MENU_ITEM:
        wm_inq_front_window (&front_window_id);
        if (front_window_id != pxs->drawing.window_id)
            {
            wmwfront (pxs->drawing.window_id);
            wm_grsgwk (MAIN_WKID);
            }
        status = xs_dgz_tops_init (pxs);
        break;
    case SAVE_TOPS_MENU_ITEM:
        status = xs_sav_tops_init (pxs);
        break;
        }
    return SUCCESS;
    }
/* END:                                                                             */

/* Function Description -----------------------------------------------------       */
/*Description:                                                                      */
/*    Gets events from the message CDLG and does nothing.                           */
/*                                                                                  */
/*Prototype:                                                                        */
/*    static INT xs_msg_cdlg_server( INT id,INT item, XS_STRUCT *pxs );         */
/*                                                                                  */
/*Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)        */
/*    id              -(INT)                                                        */
/*    item            -(INT)                                                        */
/*    pxs             -(XS_STRUCT *)                                                */
/*                                                                                  */
/*Return Value/Status:                                                              */
/*    SUCCESS - Successful completion.                                              */
/*                                                                                  */
/*Scope:                                                                            */
/*    PRIVATE to <xs_main.c>                                                        */
/*                                                                                  */
/*Limitations/Assumptions:                                                          */
/*                                                                                  */
/*-----------------------------------------------------------------------------     */

/* BEGIN:                                                                           */
#if USE_PROTOTYPES
static INT xs_msg_cdlg_server (INT id, INT item, XS_STRUCT *pxs)
#else
static INT xs_msg_cdlg_server (id, item, pxs)
INT id;
INT item;
XS_STRUCT *pxs;
#endif
    {
    return SUCCESS;
    }
/* END:                                                                             */

/* Function Description -----------------------------------------------------       */
/*Description:                                                                      */
/*  Inquire if the operator REALLY wants to quit & provide opportunity to           */
/*  save any changes to tops, layouts, faults etc.                                  */
/*                                                                                  */
/*Prototype:                                                                        */
/*    static INT xs_quit(XS_STRUCT *pxs);                                       */
/*                                                                                  */
/*Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)        */
/*    pxs             -(XS_STRUCT *)                                                */
/*                                                                                  */
/*Return Value/Status:                                                              */
/*    SUCCESS - Successful completion.                                              */
/*                                                                                  */
/*Scope:                                                                            */
/*    PRIVATE to <xs_main.c>                                                        */
/*                                                                                  */
/*Limitations/Assumptions:                                                          */
/*                                                                                  */
/*-----------------------------------------------------------------------------     */

/* BEGIN:                                                                           */
#if USE_PROTOTYPES
static INT xs_quit (XS_STRUCT *pxs)
#else
static INT xs_quit (pxs)
XS_STRUCT *pxs;
#endif
    {
    INT status;
    AS_YESNO_WORKSPACE *pyn;
    
    pyn = (AS_YESNO_WORKSPACE *) am_allocate (AM_APPLICATION,
                                              sizeof(AS_YESNO_WORKSPACE));
    pyn->cancel = FALSE;
    
    /*  Do you really want to quit?                                                 */
    
    strcpy (pyn->question, mg_message (XS_INQ_QUIT));
    status = asp_yesno (pyn);
    if (pyn->answer == AS_YESNO_NO)
        {
        return SUCCESS;
        }
    /*  Save top picks?                                                             */
#if 0
    if (pxs->tops_changed_since_save == TRUE)
        {
        strcpy (pyn->question, mg_message (XS_INQ_SAVE_TOPS));
        status = asp_yesno (pyn);
        if (pyn->answer == AS_YESNO_YES)
            {
            status = xs_sav_tops_init (pxs);
            }
        }
#endif
    /*  Save the layout changes?                                                    */
#if 0
    if (pxs->changed_since_save == TRUE)
        {
        strcpy (pyn->question, mg_message (XS_INQ_SAVE));
        status = asp_yesno (pyn);
        if (pyn->answer == AS_YESNO_YES)
            {
            pxs->quit_flag = TRUE;
            status = xs_save_init (pxs);
            }
        }
#endif
    status = xs_term (pxs);
    
    return SUCCESS;
    }
/* END:                                                                             */

/* Function Description -----------------------------------------------------       */
/*Description:                                                                      */
/*  Free the memory associated with the application and call am_quit.               */
/*  This may be called by xs_quit if verified or if startup fails.                  */
/*                                                                                  */
/*Prototype:                                                                        */
/*    publicdef INT xs_term(XS_STRUCT *pxs );                                       */
/*                                                                                  */
/*Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)        */
/*    pxs             -(XS_STRUCT *)                                                */
/*                                                                                  */
/*Return Value/Status:                                                              */
/*    SUCCESS - Successful completion.                                              */
/*                                                                                  */
/*Scope:                                                                            */
/*    PUBLIC                                                                        */
/*                                                                                  */
/*Limitations/Assumptions:                                                          */
/*                                                                                  */
/*-----------------------------------------------------------------------------     */

/* BEGIN:                                                                           */
#if USE_PROTOTYPES
publicdef INT xs_term (XS_STRUCT *pxs)
#else
publicdef INT xs_term (pxs)
XS_STRUCT *pxs;
#endif
    {
    INT status;
        
    /*  Enable the FINDER menu item.                                                */
    
    status = am_quit ();
    
    return SUCCESS;
    }
/* END:                                                                             */
