/* DEC/CMS REPLACEMENT HISTORY, Element XS_PREFS_1.C*/
/*  8    27-FEB-1992 09:28:56 JESSIE "(SPR 6180) add TVD display of traces and tops"*/
/* *7    27-MAR-1991 13:30:03 KEE "(SPR -1) Cross Section bug fixing and enhancement for Canadian Hunter"*/
/* *6     9-NOV-1990 10:28:51 MING "(SPR 6075) take off title block dialog in the change all function"*/
/* *5    16-OCT-1990 15:04:57 GILLESPIE "(SPR 1000) Merge Ernie deltas"*/
/*  3B1  15-OCT-1990 17:27:48 GILLESPIE "Merge Ernie deltas"*/
/* *4    17-AUG-1990 22:41:57 VINCE "(SPR 5644) Code Cleanup"*/
/* *3     6-DEC-1989 15:47:51 GORDON "(SPR 0) don't free hdr_list_tcp, it's not yours"*/
/* *2    14-SEP-1989 16:24:25 GORDON "(SPR 100) Gulf/Sun/Unix mods"*/
/* *1    19-JUN-1989 13:44:59 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element XS_PREFS_1.C*/
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

#ifndef ESI_WI_H
#include "esi_wi.h"
#endif

#ifndef ESI_TC_H
#include "esi_tc.h"
#endif

#ifndef ESI_TS_H
#include "esi_ts.h"
#endif

#ifndef ESI_LU_H
#include "esi_lu.h"
#endif

#ifndef XS_CDLGS_RH
#include "xs_cdlgs.rh"
#endif

#ifndef XS_MENUS_RH
#include "xs_menus.rh"
#endif

#if USE_PROTOTYPES
static INT xs_tblock_cdlg_server (INT id, INT item, XS_STRUCT *pxs);
static INT xs_tblock_cdlg_term (XS_STRUCT *pxs);
static INT xs_type_cdlg_server (INT id, INT item, XS_STRUCT *pxs);
static INT xs_type_cdlg_term (XS_STRUCT *pxs);
static INT xs_horizontal_cdlg_server (INT id, INT item, XS_STRUCT *pxs);
static INT xs_horizontal_cdlg_term (XS_STRUCT *pxs);
static INT xs_hdr_cdlg_server (INT id, INT item, XS_STRUCT *pxs);
static INT xs_hdr_cdlg_term (XS_STRUCT *pxs);
#else
static INT xs_tblock_cdlg_server ();
static INT xs_tblock_cdlg_term ();
static INT xs_type_cdlg_server ();
static INT xs_type_cdlg_term ();
static INT xs_horizontal_cdlg_server ();
static INT xs_horizontal_cdlg_term ();
static INT xs_hdr_cdlg_server ();
static INT xs_hdr_cdlg_term ();
#endif

/* File Description ---------------------------------------------------------       */
/*Overview:                                                                         */
/*                                                                                  */
/*Prototypes and one line descriptions of each function in the file.                */
/*Public_Functions:                                                                 */
/*    publicdef INT xs_start_chg_all(XS_STRUCT *pxs );                              */
/*                                                                                  */
/*    publicdef INT xs_stop_chg_all(XS_STRUCT *pxs );                               */
/*                                                                                  */
/*    publicdef INT xs_tblock_cdlg_init(XS_STRUCT *pxs);                            */
/*                                                                                  */
/*    publicdef INT xs_type_cdlg_init(XS_STRUCT *pxs);                              */
/*                                                                                  */
/*    publicdef INT xs_horizontal_cdlg_init(XS_STRUCT *pxs );                       */
/*                                                                                  */
/*    publicdef INT xs_hdr_cdlg_init(XS_STRUCT *pxs);                               */
/*                                                                                  */
/*Private_Functions:                                                                */
/*    static INT xs_tblock_cdlg_server(INT id, INT item, XS_STRUCT *pxs);       */
/*                                                                                  */
/*    static INT xs_tblock_cdlg_term(XS_STRUCT *pxs);                           */
/*                                                                                  */
/*    static INT xs_type_cdlg_server(INT id, INT item, XS_STRUCT *pxs);         */
/*                                                                                  */
/*    static INT xs_type_cdlg_term(XS_STRUCT *pxs);                             */
/*                                                                                  */
/*    static INT xs_horizontal_cdlg_server(INT id, INT item, XS_STRUCT *pxs);   */
/*                                                                                  */
/*    static INT xs_horizontal_cdlg_term(XS_STRUCT *pxs);                       */
/*                                                                                  */
/*    static INT xs_hdr_cdlg_server(INT id, INT item, XS_STRUCT *pxs);          */
/*                                                                                  */
/*    static INT xs_hdr_cdlg_term(XS_STRUCT *pxs);                              */
/*                                                                                  */
/*-----------------------------------------------------------------------------     */

/* Function Description -----------------------------------------------------       */
/*Description:                                                                      */
/*    Calls the initialization routine for the first CDLG.                          */
/*    Sets the flag to signal sequential invocation of layout editing cdlgs.        */
/*    Also enables the Stop Review-Change menu item on the Edit menu.               */
/*                                                                                  */
/*Prototype:                                                                        */
/*    publicdef INT xs_start_chg_all(XS_STRUCT *pxs );                              */
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
publicdef INT xs_start_chg_all (XS_STRUCT *pxs)
#else
publicdef INT xs_start_chg_all (pxs)
XS_STRUCT *pxs;
#endif
    {
    INT status;
    
    /* start with title block routine                                               */
    
    status = xs_type_cdlg_init (pxs);
    if (status == SUCCESS)
        {
        /*  turn off all menu items, except                                         */
        /*  stop-review-all                                                         */
        status = wienmni (pxs->edit_menu_id, STOP_ALL_MENU_ITEM, 1);
        pxs->review_all_switch = TRUE;
        wienmni (pxs->edit_menu_id, TYPE_MENU_ITEM, OFF);
        wienmni (pxs->edit_menu_id, HORIZ_SCALE_MENU_ITEM, OFF);
        wienmni (pxs->edit_menu_id, LOG_HEADER_MENU_ITEM, OFF);
        wienmni (pxs->edit_menu_id, DEPTH_SCALE_MENU_ITEM, OFF);
        wienmni (pxs->edit_menu_id, INTERVAL_MENU_ITEM, OFF);
        wienmni (pxs->edit_menu_id, TRACK_DEF_MENU_ITEM, OFF);
        wienmni (pxs->edit_menu_id, TRACE_SEL_MENU_ITEM, OFF);
        wienmni (pxs->edit_menu_id, TRACE_ATT_MENU_ITEM, OFF);
        }
    return SUCCESS;
    }
/* END:                                                                             */

/* Function Description -----------------------------------------------------       */
/*Description:                                                                      */
/*  Sets the "review all" flag to FALSE.                                            */
/*  Determines which CDLGs are open and closes them.                                */
/*  Disables the Stop Review-Change menu item.                                      */
/*    (The Start Review-Change item will be enabled when the last CDLG is terminated.)  */
/*                                                                                  */
/*Prototype : publicdef INT xs_stop_chg_all (XS_STRUCT *pxs);                       */
/*                                                                                  */
/*Parameters : (indicate (I) / (O) / (M)for input / output / input-and-modified)    */
/*    pxs-(XS_STRUCT *)                                                             */
/*                                                                                  */
/*Return Value / Status :                                                           */
/*    SUCCESS-Successful completion                                                 */
/*                                                                                  */
/*Scope :                                                                           */
/*    PUBLIC                                                                        */
/*                                                                                  */
/*Limitations / Assumptions :                                                       */
/*                                                                                  */
/*-----------------------------------------------------------------------------     */

/* BEGIN:                                                                           */
#if USE_PROTOTYPES
publicdef INT xs_stop_chg_all (XS_STRUCT *pxs)
#else
publicdef INT xs_stop_chg_all (pxs)
XS_STRUCT *pxs;
#endif
    {
    INT status;
    
    pxs->review_all_switch = FALSE;
    
    status = SUCCESS;
    
    /*  terminate any open dialogs                                                  */
    
    if (pxs->tblock_in_use == TRUE)
        status = xs_tblock_cdlg_term (pxs);
    if (pxs->type_in_use == TRUE)
        status = xs_type_cdlg_term (pxs);
    if (pxs->horiscale_in_use == TRUE)
        status = xs_horizontal_cdlg_term (pxs);
    if (pxs->hdr_in_use == TRUE)
        status = xs_hdr_cdlg_term (pxs);
    if (pxs->dpthscal_in_use == TRUE)
        status = xs_depth_scale_cdlg_term (pxs);
    if (pxs->dataint_in_use == TRUE)
        status = xs_dataint_cdlg_term (pxs);
    if (pxs->depthgrid_in_use == TRUE)
        status = xs_depthgrid_cdlg_term (pxs);
    if (pxs->depthconst_in_use == TRUE)
        status = xs_depthconst_cdlg_term (pxs);
    if (pxs->top2top_in_use == TRUE)
        status = xs_top2top_cdlg_term (pxs);
    if (pxs->zonesel_in_use == TRUE)
        status = xs_zonesel_cdlg_term (pxs);
    if (pxs->trackdef_in_use == TRUE)
        status = xs_trackdef_cdlg_term (pxs);
    if (pxs->tracesel_in_use == TRUE)
        status = xs_tracesel_cdlg_term (pxs);
    if (pxs->traceattr_in_use == TRUE)
        status = xs_traceattr_cdlg_term (pxs);
    
    /*  now enable individual items on menus                                        */
    
    if (status == SUCCESS)
        {
        status = wienmni (pxs->edit_menu_id, STOP_ALL_MENU_ITEM, 0);
        wienmni (pxs->edit_menu_id, TYPE_MENU_ITEM, ON);
        wienmni (pxs->edit_menu_id, HORIZ_SCALE_MENU_ITEM, ON);
        wienmni (pxs->edit_menu_id, LOG_HEADER_MENU_ITEM, ON);
        wienmni (pxs->edit_menu_id, DEPTH_SCALE_MENU_ITEM, ON);
        wienmni (pxs->edit_menu_id, INTERVAL_MENU_ITEM, ON);
        wienmni (pxs->edit_menu_id, TRACK_DEF_MENU_ITEM, ON);
        wienmni (pxs->edit_menu_id, TRACE_SEL_MENU_ITEM, ON);
        wienmni (pxs->edit_menu_id, TRACE_ATT_MENU_ITEM, ON);
        }
    return SUCCESS;
    }
/* END:                                                                             */

/* Function Description -----------------------------------------------------       */
/*Description:                                                                      */
/*    Allow user to specify 6 lines of text for title block.                        */
/*                                                                                  */
/*Prototype:                                                                        */
/*    publicdef INT xs_tblock_cdlg_init(XS_STRUCT *pxs);                            */
/*                                                                                  */
/*Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)        */
/*    pxs             -(XS_STRUCT *) Pointer to XSEC struc.                         */
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
publicdef INT xs_tblock_cdlg_init (XS_STRUCT *pxs)
#else
publicdef INT xs_tblock_cdlg_init (pxs)
XS_STRUCT *pxs;
#endif
    {
    INT status;
    INT id;
    
    status = am_define_resource (AM_DIALOG, XS_CDLGS_FILE, XS_TITLE_BLOCK,
                                 xs_tblock_cdlg_server, &id, pxs);
    
    /*  dont let user start change all while dialog                                 */
    /*  is open.  dont let user repick current                                      */
    /*  menu item                                                                   */
    
    wienmni (pxs->edit_menu_id, CHANGE_ALL_MENU_ITEM, OFF);
    
    /*  save value of dialog id's for later use                                     */
    
    pxs->tblock_in_use = ON;
    pxs->tblock_cdlg_id = id;
    
    /*  display the 6 title lines for changes                                       */
    
    wiscdt (id, XS_TITLE1, pxs->title.line1);
    wiscdt (id, XS_TITLE2, pxs->title.line2);
    wiscdt (id, XS_TITLE3, pxs->title.line3);
    wiscdt (id, XS_TITLE4, pxs->title.line4);
    wiscdt (id, XS_TITLE5, pxs->title.line5);
    wiscdt (id, XS_TITLE6, pxs->title.line6);
    
    status = wi_open_dialog (id);
    
    return SUCCESS;
    
    }
/* END:                                                                             */

/* Function Description -----------------------------------------------------       */
/*Description:                                                                      */
/*    Server allows user to change title lines.                                     */
/*                                                                                  */
/*Prototype:                                                                        */
/*    static INT xs_tblock_cdlg_server(INT id,INT item, XS_STRUCT *pxs);        */
/*                                                                                  */
/*Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)        */
/*    id              -(INT) Resource id.                                           */
/*    item            -(INT) Event number.                                          */
/*    pxs             -(XS_STRUCT *) Pointer to XSEC struct.                        */
/*                                                                                  */
/*Return Value/Status:                                                              */
/*    SUCCESS - Successful completion.                                              */
/*                                                                                  */
/*Scope:                                                                            */
/*    PRIVATE to <xs_prefs_1.c>                                                     */
/*                                                                                  */
/*Limitations/Assumptions:                                                          */
/*                                                                                  */
/*-----------------------------------------------------------------------------     */

/* BEGIN:                                                                           */
#if USE_PROTOTYPES
static INT xs_tblock_cdlg_server (INT id, INT item, XS_STRUCT *pxs)
#else
static INT xs_tblock_cdlg_server (id, item, pxs)
INT id;
INT item;
XS_STRUCT *pxs;
#endif
    {
    INT status;
    INT unused;
    
    switch (item)
        {
        
    case XS_TITLE_ACCEPT:
        /*  take down any changes and put them in the                               */
        /*  structure                                                               */
        
        wiqccdi (id, XS_TITLE1, &unused, pxs->title.line1);
        wiqccdi (id, XS_TITLE2, &unused, pxs->title.line2);
        wiqccdi (id, XS_TITLE3, &unused, pxs->title.line3);
        wiqccdi (id, XS_TITLE4, &unused, pxs->title.line4);
        wiqccdi (id, XS_TITLE5, &unused, pxs->title.line5);
        wiqccdi (id, XS_TITLE6, &unused, pxs->title.line6);
        
        pxs->changed_since_save = TRUE;
        
        status = xs_tblock_cdlg_term (pxs);
        break;
        
    case XS_TITLE_IGNORE:
        
        status = xs_tblock_cdlg_term (pxs);
        break;
        
    default:
        break;
        
        }
    return SUCCESS;
    
    }
/* END:                                                                             */

/* Function Description -----------------------------------------------------       */
/*Description:                                                                      */
/*    Cleanup after title block changes.                                            */
/*                                                                                  */
/*Prototype:                                                                        */
/*    static INT xs_tblock_cdlg_term(XS_STRUCT *pxs);                           */
/*                                                                                  */
/*Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)        */
/*    pxs             -(XS_STRUCT *) Pointer to XSEC struct.                        */
/*                                                                                  */
/*Return Value/Status:                                                              */
/*    SUCCESS - Successful completion.                                              */
/*                                                                                  */
/*Scope:                                                                            */
/*    PRIVATE to <xs_prefs_1.c>                                                     */
/*                                                                                  */
/*Limitations/Assumptions:                                                          */
/*                                                                                  */
/*-----------------------------------------------------------------------------     */

/* BEGIN:                                                                           */
#if USE_PROTOTYPES
static INT xs_tblock_cdlg_term (XS_STRUCT *pxs)
#else
static INT xs_tblock_cdlg_term (pxs)
XS_STRUCT *pxs;
#endif
    {
    INT status;
    
    /*  show title block no longer in use.  take                                    */
    /*  down dialog reset menus to original state                                   */
    
    pxs->tblock_in_use = OFF;
    status = am_release_resource (pxs->tblock_cdlg_id);
    if (pxs->review_all_switch == ON)
        status = xs_type_cdlg_init (pxs);
    else
        {
        wienmni (pxs->edit_menu_id, CHANGE_ALL_MENU_ITEM, ON);
        }
    return SUCCESS;
    
    }
/* END:                                                                             */

/* Function Description -----------------------------------------------------       */
/*Description:                                                                      */
/*    Identify which cross section type user will create.                           */
/*                                                                                  */
/*Prototype:                                                                        */
/*    publicdef INT xs_type_cdlg_init(XS_STRUCT *pxs);                              */
/*                                                                                  */
/*Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)        */
/*    pxs             -(XS_STRUCT *) Pointer to XSEC struc.                         */
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
publicdef INT xs_type_cdlg_init (XS_STRUCT *pxs)
#else
publicdef INT xs_type_cdlg_init (pxs)
XS_STRUCT *pxs;
#endif
    {
    INT status;
    INT id;
    
    status = am_define_resource (AM_DIALOG, XS_CDLGS_FILE, XS_TYPE,
                                 xs_type_cdlg_server, &id, pxs);
    
    wienmni (pxs->edit_menu_id, CHANGE_ALL_MENU_ITEM, OFF);
    wienmni (pxs->edit_menu_id, TYPE_MENU_ITEM, OFF);
    pxs->type_in_use = ON;
    pxs->type_cdlg_id = id;
    
    /*  copy switches to temporary                                                  */
    
    pxs->xst_type = pxs->xs_type;
    pxs->xst_spacing = pxs->xs_spacing;
    
    /*  set up radio button for well spacing to                                     */
    /*  current state                                                               */
    
    switch (pxs->xst_spacing)
        {
    case XS_CONSTANT_SPACED:
        wiscdv (id, XS_TYPE_CONSTANT, ON);
        break;
        
    case XS_EDGE_SPACED:
        wiscdv (id, XS_TYPE_GAPS, ON);
        break;
        
    case XS_CENTER_SPACED:
        wiscdv (id, XS_TYPE_CENTERS, ON);
        break;
        
    default:
        break;
        
        }
    /*  set the display-type radio buttons                                          */
    /*  according to structure                                                      */
    
    switch (pxs->xst_type)
        {
    case XS_NO_DATUM:
        wiscdv (id, XS_TYPE_MDEPTH, ON);
        wiencdi (id, XS_TYPE_SELECT, OFF);
        break;
        
    case XS_STRUCTURAL:
        wiscdv (id, XS_TYPE_STRUCT, ON);
        wiencdi (id, XS_TYPE_SELECT, OFF);
        break;
        
    case XS_STRATIGRAPHIC:
        wiscdv (id, XS_TYPE_STRAT, ON);
        wiencdi (id, XS_TYPE_SELECT, ON);
        break;
        
    default:
        break;
        
        }
    /*  Copy in the datum selection phrase                                          */
    
    wiscdt (id, XS_TYPE_SELECT, pxs->xs_datum_select);

    /* Turn on/off the suppress vertical scale bar check box */
    wiscdv (id, XS_TYPE_SUPPRESS_VBAR, pxs->xs_suppress_vbar);
    
    /*  Make custom dialog visible.                                                 */
    status = wi_open_dialog (id);
    
    /*  Hilite of top must be done after dialog is                                  */
    /*  visible.                                                                    */
    
    if (pxs->xst_type == XS_STRATIGRAPHIC)
        {
        wi_activate (id, XS_TYPE_SELECT, ON);
        }
    return SUCCESS;
    
    }
/* END:                                                                             */

/* Function Description -----------------------------------------------------       */
/*Description:                                                                      */
/*    Set xsec type.                                                                */
/*                                                                                  */
/*Prototype:                                                                        */
/*    static INT xs_type_cdlg_server(INT id,INT item, XS_STRUCT *pxs);          */
/*                                                                                  */
/*Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)        */
/*    id              -(INT) Resource id.                                           */
/*    item            -(INT) Event.                                                 */
/*    pxs             -(XS_STRUCT *) Pointer to XSEC struct.                        */
/*                                                                                  */
/*Return Value/Status:                                                              */
/*    SUCCESS - Successful completion.                                              */
/*                                                                                  */
/*Scope:                                                                            */
/*    PRIVATE to <xs_prefs_1.c>                                                     */
/*                                                                                  */
/*Limitations/Assumptions:                                                          */
/*                                                                                  */
/*-----------------------------------------------------------------------------     */

/* BEGIN:                                                                           */
#if USE_PROTOTYPES
static INT xs_type_cdlg_server (INT id, INT item, XS_STRUCT *pxs)
#else
static INT xs_type_cdlg_server (id, item, pxs)
INT id;
INT item;
XS_STRUCT *pxs;
#endif
    {
    INT unused;
    INT status;
    CHAR text[128];
    
    switch (item)
        {
        
    case XS_TYPE_ACCEPT:
	wiqccdi (id, XS_TYPE_SUPPRESS_VBAR, &pxs->xs_suppress_vbar, text);
        if (pxs->xst_type == XS_STRATIGRAPHIC)
            {
            wiqccdi (id, XS_TYPE_SELECT, &unused, text);
            ts_sto_upper (text);
            strcpy (pxs->xs_datum_select, text);
            }
        pxs->xs_type = pxs->xst_type;
        pxs->xs_spacing = pxs->xst_spacing;
        
        pxs->changed_since_save = TRUE;
        
        status = xs_type_cdlg_term (pxs);
        break;
        
    case XS_TYPE_IGNORE:
        /*  user doesnt want any changes - so dont                                  */
        
        status = xs_type_cdlg_term (pxs);
        break;
        
    case XS_TYPE_CENTERS:
        /*  save state of buttons when changed                                      */
        
        pxs->xst_spacing = XS_CENTER_SPACED;
        break;
        
    case XS_TYPE_GAPS:
        pxs->xst_spacing = XS_EDGE_SPACED;
        break;
        
    case XS_TYPE_CONSTANT:
        pxs->xst_spacing = XS_CONSTANT_SPACED;
        break;
        
    case XS_TYPE_STRUCT:
        pxs->xst_type = XS_STRUCTURAL;
        wiencdi (id, XS_TYPE_SELECT, OFF);
        break;
        
    case XS_TYPE_MDEPTH:
        pxs->xst_type = XS_NO_DATUM;
        wiencdi (id, XS_TYPE_SELECT, OFF);
        break;
        
    case XS_TYPE_STRAT:
        pxs->xst_type = XS_STRATIGRAPHIC;
        wiencdi (id, XS_TYPE_SELECT, ON);
        wi_activate (id, XS_TYPE_SELECT, ON);
        break;
        
    default:
        break;
        
        }
    return SUCCESS;
    
    }
/* END:                                                                             */

/* Function Description -----------------------------------------------------       */
/*Description:                                                                      */
/*    Clean up after type of xsec selected.                                         */
/*                                                                                  */
/*Prototype:                                                                        */
/*    static INT xs_type_cdlg_term(XS_STRUCT *pxs);                             */
/*                                                                                  */
/*Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)        */
/*    pxs             -(XS_STRUCT *) Pointer to XSEC struct.                        */
/*                                                                                  */
/*Return Value/Status:                                                              */
/*    SUCCESS - Successful completion.                                              */
/*                                                                                  */
/*Scope:                                                                            */
/*    PRIVATE to <xs_prefs_1.c>                                                     */
/*                                                                                  */
/*Limitations/Assumptions:                                                          */
/*                                                                                  */
/*-----------------------------------------------------------------------------     */

/* BEGIN:                                                                           */
#if USE_PROTOTYPES
static INT xs_type_cdlg_term (XS_STRUCT *pxs)
#else
static INT xs_type_cdlg_term (pxs)
XS_STRUCT *pxs;
#endif
    {
    INT status;
    
    /*  reset in use flag, take down dialog, and                                    */
    /*  return menus to normal                                                      */
    
    pxs->type_in_use = OFF;
    status = am_release_resource (pxs->type_cdlg_id);
    if (pxs->review_all_switch == ON)
        status = xs_horizontal_cdlg_init (pxs);
    else
        {
        wienmni (pxs->edit_menu_id, TYPE_MENU_ITEM, ON);
        wienmni (pxs->edit_menu_id, CHANGE_ALL_MENU_ITEM, ON);
        }
    return SUCCESS;
    
    }
/* END:                                                                             */

/* Function Description -----------------------------------------------------       */
/*Description:                                                                      */
/*    Allow user to specify horizontal scale of xsect.                              */
/*                                                                                  */
/*Prototype:                                                                        */
/*    publicdef INT xs_horizontal_cdlg_init(XS_STRUCT *pxs );                       */
/*                                                                                  */
/*Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)        */
/*    pxs             -(XS_STRUCT *) Pointer to XSEC struct.                        */
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
publicdef INT xs_horizontal_cdlg_init (XS_STRUCT *pxs)
#else
publicdef INT xs_horizontal_cdlg_init (pxs)
XS_STRUCT *pxs;
#endif
    {
    INT status;
    INT id;
    CHAR text[128];
    
    status = am_define_resource (AM_DIALOG, XS_CDLGS_FILE, XS_HORIZONTAL,
                                 xs_horizontal_cdlg_server, &id, pxs);
    
    wienmni (pxs->edit_menu_id, CHANGE_ALL_MENU_ITEM, OFF);
    wienmni (pxs->edit_menu_id, HORIZ_SCALE_MENU_ITEM, OFF);
    pxs->horiscale_in_use = ON;
    pxs->horiscale_cdlg_id = id;
    
    /*  copy in the scale string to the dialog                                      */
    wiscdt (id, XS_HORIZ_STR, pxs->xs_scale_str);
    /*  Make the dialog visible.                                                    */
    status = wi_open_dialog (id);
    /*  Hilight the scale string for changes.                                       */
    status = wi_activate (id, XS_HORIZ_STR, 1);
    
    return SUCCESS;
    
    }
/* END:                                                                             */

/* Function Description -----------------------------------------------------       */
/*Description:                                                                      */
/*    Receive horizontal scale from user.                                           */
/*                                                                                  */
/*Prototype:                                                                        */
/*    static INT xs_horizontal_cdlg_server(INT id, INT item, XS_STRUCT *pxs)    */
/*                                                                                  */
/*Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)        */
/*    id              -(INT) Resource id.                                           */
/*    item            -(INT) Event number.                                          */
/*    pxs             -(XS_STRUCT *) Pointer to XSEC struct.                        */
/*                                                                                  */
/*Return Value/Status:                                                              */
/*    SUCCESS - Successful completion.                                              */
/*                                                                                  */
/*Scope:                                                                            */
/*    PRIVATE to <xs_prefs_1.c>                                                     */
/*                                                                                  */
/*Limitations/Assumptions:                                                          */
/*                                                                                  */
/*-----------------------------------------------------------------------------     */

/* BEGIN:                                                                           */
#if USE_PROTOTYPES
static INT xs_horizontal_cdlg_server (INT id, INT item, XS_STRUCT *pxs)
#else
static INT xs_horizontal_cdlg_server (id, item, pxs)
INT id;
INT item;
XS_STRUCT *pxs;
#endif
    {
    CHAR text[128];
    INT status;
    INT unused;
    DOUBLE uom_depth_scale;
    
    switch (item)
        {
        
    case XS_HORIZ_ACCEPT:
        wiqccdi (id, XS_HORIZ_STR, &unused, text);
        ts_sto_upper (text);
        strcpy (pxs->xs_scale_str, text);
        status = xsz_horiz_scale_calc (pxs);
        if (status == SUCCESS)
            {
            pxs->changed_since_save = TRUE;
            status = xs_horizontal_cdlg_term (pxs);
            }
        break;
        
    case XS_HORIZ_IGNORE:
        
        status = xs_horizontal_cdlg_term (pxs);
        break;
        
    default:
        break;
        
        }
    return SUCCESS;
    
    }
/* END:                                                                             */

/* Function Description -----------------------------------------------------       */
/*Description:                                                                      */
/*    Terminate horizontal scale dialog.                                            */
/*                                                                                  */
/*Prototype:                                                                        */
/*    static INT xs_horizontal_cdlg_term(XS_STRUCT *pxs);                       */
/*                                                                                  */
/*Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)        */
/*    pxs             -(XS_STRUCT *) Pointer to XSEC struct.                        */
/*                                                                                  */
/*Return Value/Status:                                                              */
/*    SUCCESS - Successful completion.                                              */
/*                                                                                  */
/*Scope:                                                                            */
/*    PRIVATE to <xs_prefs_1.c>                                                     */
/*                                                                                  */
/*Limitations/Assumptions:                                                          */
/*                                                                                  */
/*-----------------------------------------------------------------------------     */

/* BEGIN:                                                                           */
#if USE_PROTOTYPES
static INT xs_horizontal_cdlg_term (XS_STRUCT *pxs)
#else
static INT xs_horizontal_cdlg_term (pxs)
XS_STRUCT *pxs;
#endif
    {
    INT status;
    
    /*  cleanup in use flag, take down dialog,                                      */
    /*  reset menus                                                                 */
    
    pxs->horiscale_in_use = OFF;
    am_release_resource (pxs->horiscale_cdlg_id);
    if (pxs->review_all_switch == ON)
        status = xs_hdr_cdlg_init (pxs);
    else
        {
        wienmni (pxs->edit_menu_id, HORIZ_SCALE_MENU_ITEM, ON);
        wienmni (pxs->edit_menu_id, CHANGE_ALL_MENU_ITEM, ON);
        }
    return SUCCESS;
    
    }
/* END:                                                                             */

/* Function Description -----------------------------------------------------       */
/*Description:                                                                      */
/*    Init dialog to request header items to be posted.                             */
/*                                                                                  */
/*Prototype:                                                                        */
/*    publicdef INT xs_hdr_cdlg_init(XS_STRUCT *pxs);                               */
/*                                                                                  */
/*Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)        */
/*    pxs             -(XS_STRUCT *) Pointer to XSEC struct.                        */
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
publicdef INT xs_hdr_cdlg_init (XS_STRUCT *pxs)
#else
publicdef INT xs_hdr_cdlg_init (pxs)
XS_STRUCT *pxs;
#endif
    {
    static INT max_selectable = 10;
    INT status;
    INT id;
    INT num_items;
    INT i;
    
    status = am_define_resource (AM_DIALOG, XS_CDLGS_FILE, XS_HDR, xs_hdr_cdlg_server,
                                 &id, pxs);
    
    wienmni (pxs->edit_menu_id, CHANGE_ALL_MENU_ITEM, OFF);
    wienmni (pxs->edit_menu_id, LOG_HEADER_MENU_ITEM, OFF);
    pxs->hdr_in_use = ON;
    pxs->hdr_cdlg_id = id;
    
    /*  go get the header item list from oracle and                                 */
    /*  build the tcp                                                               */
    
    status = lu_get_tcp ("LOGPLOT_HEADER_ITEM", &pxs->hdr_list_tcp, &num_items);
    
    wisselt (id, XS_HDR_SELECT_LIST, XS_HDR_VSCROLL, num_items, max_selectable,
             pxs->hdr_list_tcp);
    
    /*  now go thru and hi-lite the ones which were                                 */
    /*  pre selected                                                                */
    
    for (i = 0; i < pxs->plo->log_hdr.total_items; i++)
        wi_set_default_sel (id, XS_HDR_SELECT_LIST, pxs->plo->log_hdr.items[i]);
    
    status = wi_open_dialog (id);
    
    return SUCCESS;
    
    }
/* END:                                                                             */

/* Function Description -----------------------------------------------------       */
/*Description:                                                                      */
/*    Allow user to select header items.                                            */
/*                                                                                  */
/*Prototype:                                                                        */
/*    static INT xs_hdr_cdlg_server(INT id, INT item, XS_STRUCT *pxs);          */
/*                                                                                  */
/*Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)        */
/*    id              -(INT) Resource id.                                           */
/*    item            -(INT) Resource event.                                        */
/*    pxs             -(XS_STRUCT *) Pointer to XSEC struct.                        */
/*                                                                                  */
/*Return Value/Status:                                                              */
/*    SUCCESS - Successful completion.                                              */
/*                                                                                  */
/*Scope:                                                                            */
/*    PRIVATE to <xs_prefs_1.c>                                                     */
/*                                                                                  */
/*Limitations/Assumptions:                                                          */
/*                                                                                  */
/*-----------------------------------------------------------------------------     */

/* BEGIN:                                                                           */
#if USE_PROTOTYPES
static INT xs_hdr_cdlg_server (INT id, INT item, XS_STRUCT *pxs)
#else
static INT xs_hdr_cdlg_server (id, item, pxs)
INT id;
INT item;
XS_STRUCT *pxs;
#endif
    {
    INT status;
    INT num_items;
    INT i;
    INT *ptr;
    static INT items_selected[10];  /*  max selectable for now                  */
    
    switch (item)
        {
        
    case XS_HDR_ACCEPT:
        /*  user has selected list of items                                         */
        /*  determine 1) number of item & 2) list of                                */
        /*  items                                                                   */
        
        wiqsel (pxs->hdr_cdlg_id, XS_HDR_SELECT_LIST, &num_items, items_selected);
        pxs->plo->log_hdr.total_items = num_items;
        
        pxs->plo->log_hdr.items = ptr =
            (INT *) tc_zalloc (sizeof(INT *)*(num_items + 1));
        for (i = 0; i < num_items; i++)
            ptr[i] = items_selected[i];
        
        /*  Since the header list has changed, force a                              */
        /*  re-retrieve of the data.                                                */
        
        pxs->changed_since_display = TRUE;
        pxs->changed_since_save = TRUE;
        
        status = xs_hdr_cdlg_term (pxs);
        break;
        
    case XS_HDR_IGNORE:
        
        status = xs_hdr_cdlg_term (pxs);
        break;

    default:
        break;
        
        }
    xs_cs_menu_mgr (pxs);
    
    return SUCCESS;
    
    }
/* END:                                                                             */

/* Function Description -----------------------------------------------------       */
/*Description:                                                                      */
/*    Finish header stuff.                                                          */
/*                                                                                  */
/*Prototype:                                                                        */
/*    static INT xs_hdr_cdlg_term(XS_STRUCT *pxs);                              */
/*                                                                                  */
/*Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)        */
/*    pxs             -(XS_STRUCT *) Pointer to XSEC struct.                        */
/*                                                                                  */
/*Return Value/Status:                                                              */
/*    SUCCESS - Successful completion.                                              */
/*                                                                                  */
/*Scope:                                                                            */
/*    PRIVATE to <xs_prefs_1.c>                                                     */
/*                                                                                  */
/*Limitations/Assumptions:                                                          */
/*                                                                                  */
/*-----------------------------------------------------------------------------     */

/* BEGIN:                                                                           */
#if USE_PROTOTYPES
static INT xs_hdr_cdlg_term (XS_STRUCT *pxs)
#else
static INT xs_hdr_cdlg_term (pxs)
XS_STRUCT *pxs;
#endif
    {
    INT status;
    
    /*  free the header list tcp, turn off in use                                   */
    /*  flag, take down dialog, reset menu items                                    */
    
    pxs->hdr_list_tcp = (CHAR **)0;
    pxs->hdr_in_use = OFF;
    status = am_release_resource (pxs->hdr_cdlg_id);
    if (pxs->review_all_switch == ON)
        status = xs_depth_scale_cdlg_init (pxs);
    else
        {
        wienmni (pxs->edit_menu_id, LOG_HEADER_MENU_ITEM, ON);
        wienmni (pxs->edit_menu_id, CHANGE_ALL_MENU_ITEM, ON);
        }
    return SUCCESS;
    
    }
/* END:                                                                             */
