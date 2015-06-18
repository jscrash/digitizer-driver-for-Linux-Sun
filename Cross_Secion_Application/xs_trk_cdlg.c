/* DEC/CMS REPLACEMENT HISTORY, Element XS_TRK_CDLG.C*/
/* *9    27-MAR-1991 13:31:03 KEE "(SPR -1) Cross Section bug fixing and enhancement for Canadian Hunter"*/
/* *8    17-AUG-1990 22:45:51 VINCE "(SPR 5644) Code Cleanup"*/
/* *7     2-MAR-1990 11:24:41 WALTERS "(SPR 6012) Fix #ifndef USE_X to #ifdef USE_X"*/
/* *6    10-FEB-1990 09:28:35 GILLESPIE "(SPR 1) Change USE_X to be #ifdef USE_X"*/
/* *5     8-FEB-1990 17:57:13 JULIAN "(SPR -1) X Version some minor fixing (Kee)"*/
/* *4     6-FEB-1990 00:18:28 JULIAN "(SPR -1) X Version "*/
/* *3     6-DEC-1989 15:49:15 GORDON "(SPR 0) make type_tcp_size & marks_tcp_size static"*/
/* *2    14-SEP-1989 16:25:11 GORDON "(SPR 100) Gulf/Sun/Unix mods"*/
/* *1    19-JUN-1989 13:45:55 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element XS_TRK_CDLG.C*/
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
#include "esi_xs_defs.h"
#include "esi_am.h"
#include "esi_wi.h"
#include "esi_tc.h"
#include "esi_ts.h"
#include "esi_mg.h"
#include "esi_ll.h"
#include "xs_cdlgs.rh"
#include "xs_menus.rh"

#if USE_PROTOTYPES
static INT xs_trackdef_cdlg_server (INT id, INT item, XS_STRUCT *pxs);
static INT xs_trk_dlg_define (XS_STRUCT *pxs);
#else
static INT xs_trackdef_cdlg_server ();
static INT xs_trk_dlg_define ();
#endif
static TCP or_track_tcp = (TCP)
0;
static TCP or_marks_tcp = (TCP)
0;

#ifdef USE_X
static CHAR blank[] = "";
#else 
static CHAR blank[] = " ";
#endif

/* File Description ---------------------------------------------------------
Overview:

Prototypes and one line descriptions of each function in the file.
Public_Functions:
    publicdef INT xs_trackdef_cdlg_init(XS_STRUCT *pxs);
    
    publicdef INT xs_trackdef_cdlg_term(XS_STRUCT *pxs);

Private_Functions:
    static INT xs_trackdef_cdlg_server(INT id, INT item, XS_STRUCT *pxs);

    static INT xs_trk_dlg_define(XS_STRUCT *pxs);

-----------------------------------------------------------------------------*/

/* Function Description -----------------------------------------------------
Description:
    Display dialog to accept track definitions and changes.

Prototype:
    publicdef INT xs_trackdef_cdlg_init(XS_STRUCT *pxs);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    pxs             -(XS_STRUCT *) Pointer to XSEC struct.

Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT xs_trackdef_cdlg_init (XS_STRUCT *pxs)
#else
publicdef INT xs_trackdef_cdlg_init (pxs)
XS_STRUCT *pxs;
#endif
    {
    INT status;
    INT max_selectable = 2;
    INT num_items;
    TRACK_STRUCT *ptrk;
    TRACK_STRUCT *ptrk_data;
    static INT track_tcp_size = 0;
    static INT marks_tcp_size = 0;
    INT ntracks;
    INT id;
    INT i;
    
    pxs->xst_trackdef_no_redraw = FALSE;
    
    status = am_define_resource (AM_DIALOG, XS_CDLGS_FILE, XS_TRACKDEF,
                                 xs_trackdef_cdlg_server, &id, pxs);
    pxs->trackdef_cdlg_id = id;
    
    wienmni (pxs->edit_menu_id, CHANGE_ALL_MENU_ITEM, OFF);
    wienmni (pxs->edit_menu_id, TRACK_DEF_MENU_ITEM, OFF);
    
    /* Disable activation of dialogs which
       could cause linked list conflicts.   */
    
    wienmni (pxs->edit_menu_id, TRACE_SEL_MENU_ITEM, OFF);
    wienmni (pxs->edit_menu_id, TRACE_ATT_MENU_ITEM, OFF);
    
    pxs->trackdef_in_use = ON;
    
    /* first init pxs->ptemp then copy linked list from plo to ptemp */
    /* first free ptemp tracks - should be in error */
    status = llfree (&pxs->ptemp->tracks);
    
    /* init the ptemp track ll - note it contains pointers not real memory */
    status = llinit (&pxs->ptemp->tracks, 0, sizeof(TRACK_STRUCT *), LL_LIST_SYSTEM);
    
    /* find out how many */
    ntracks = llcount (&pxs->plo->tracks);
    /* set current ll to first */
    /*status = llfirst(&pxs->plo->tracks);*/
    /* allocate copy of the plo ll by allocating each piece
       and appending to temp */
    for (i = 1; i <= ntracks; i++)
        {
        /* allocate the TRACK_STRUCT for each item */
        ptrk = (TRACK_STRUCT *)tc_alloc (sizeof(TRACK_STRUCT));
        /* get the next data item from plo ll */
        status = (i == 1) ? llfirst (&pxs->plo->tracks,
                                     &ptrk_data) : llnext (&pxs->plo->tracks,
                                                           &ptrk_data);
        hoblockmove (ptrk_data, ptrk, sizeof(TRACK_STRUCT));
        /* now append ptr to ptemp ll */
        status = llappnd (&pxs->ptemp->tracks, &ptrk);
        }
    /* set up tcp for the dialog (6 items) then get tcp for translation codes */
    status = xs_trk_dlg_define (pxs);
    
    if (or_marks_tcp == (TCP)0 || or_track_tcp == (TCP)0)
        {
        status = lu_get_tcp ("LOGPLOT_MARK", &or_marks_tcp, &marks_tcp_size) ||
                 lu_get_tcp ("LOGPLOT_TRACK", &or_track_tcp, &track_tcp_size);
        if (status != SUCCESS)
            am_message (AM_STATUS, mg_message (status));
        }
    /* count the number of items in tcp */
    num_items = ts_tcp_len (pxs->track_number_tcp);
    max_selectable = 2;
    /* set the selectors for the dialog box */
    wisselt (id, XS_TRACKDEF_TRACK, XS_TRACKDEF_VSC, num_items, max_selectable,
             pxs->track_number_tcp);
    wisselt (id, XS_TRACKDEF_TYPE, XS_TRACKDEF_VSC, num_items, max_selectable,
             pxs->type_tcp);
    wisselt (id, XS_TRACKDEF_MARKS, XS_TRACKDEF_VSC, num_items, max_selectable,
             pxs->marks_tcp);
    wisselt (id, XS_TRACKDEF_NUMBER, XS_TRACKDEF_VSC, num_items, max_selectable,
             pxs->numdiv_tcp);
    wisselt (id, XS_TRACKDEF_WIDTH, XS_TRACKDEF_VSC, num_items, max_selectable,
             pxs->width_tcp);
    wisselt (id, XS_TRACKDEF_LOCATION, XS_TRACKDEF_VSC, num_items, max_selectable,
             pxs->start_loc_tcp);
    
    /* set the units of track width. */
    if (pxs->track_width_scale_units EQUALS 0)
        {
        pxs->track_width_scale_units = pxs->xs_scale_user_units;
        }
    switch (pxs->track_width_scale_units)
        {
    case XS_CENTIMETER_UNITS:
        wiscdv (id, XS_TRACKDEF_CM_UNIT_BTN, 1);
        break;
        
    case XS_INCH_UNITS:
    default:
        wiscdv (id, XS_TRACKDEF_INCH_UNIT_BTN, 1);
        break;
        }
#ifdef USE_X
    /* set up dialog for handling pulldownmenu */
    wi_create_option_menu (id, XS_TRACKDEF_TYPESEL, or_track_tcp);
    wiscdv (id, XS_TRACKDEF_TYPESEL, 1);
    
    wi_create_option_menu (id, XS_TRACKDEF_MARKSEL, or_marks_tcp);
    wiscdv (id, XS_TRACKDEF_MARKSEL, 1);

#else 
    /* set up dialog for handling valuator */
    wiscdt (id, XS_TRACKDEF_TYPESEL, or_track_tcp[0]);
    wiscdr (id, XS_TRACKDEF_TYPEVAL, 1, track_tcp_size);
    wiscdv (id, XS_TRACKDEF_TYPEVAL, 1);
    
    wiscdt (id, XS_TRACKDEF_MARKSEL, or_marks_tcp[0]);
    wiscdr (id, XS_TRACKDEF_MARKVAL, 1, marks_tcp_size);
    wiscdv (id, XS_TRACKDEF_MARKVAL, 1);

#endif
    /* dim replace and delete until select is done */
    
    wiencdi (id, XS_TRACKDEF_REPLACE, 0);
    wiencdi (id, XS_TRACKDEF_DELETE, 0);
    
    status = wiopcd (id);
    max_selectable = 1;
    wi_set_selection_limit (id, XS_TRACKDEF_TRACK, max_selectable);
    wi_set_selection_limit (id, XS_TRACKDEF_TYPE, max_selectable);
    wi_set_selection_limit (id, XS_TRACKDEF_MARKS, max_selectable);
    wi_set_selection_limit (id, XS_TRACKDEF_NUMBER, max_selectable);
    wi_set_selection_limit (id, XS_TRACKDEF_WIDTH, max_selectable);
    wi_set_selection_limit (id, XS_TRACKDEF_LOCATION, max_selectable);
    
    return SUCCESS;
    
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    Server to accept track defs from user.

Prototype:
    static INT xs_trackdef_cdlg_server(INT id, INT item, XS_STRUCT *pxs);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    id              -(INT) Resource id.
    item            -(INT) Resource event.
    pxs             -(XS_STRUCT *) Pointer to XSEC struct.

Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PRIVATE to <xs_trk_cdlg.c>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static INT xs_trackdef_cdlg_server (INT id, INT item, XS_STRUCT *pxs)
#else
static INT xs_trackdef_cdlg_server (id, item, pxs)
INT id;
INT item;
XS_STRUCT *pxs;
#endif
    {
    INT status;
    INT i;
    INT max_selectable = 1;
    INT num_selected;
    INT n_trks_left;                /* Number of tracks left before one is deleted. */
    INT unused;
    static INT index;
    INT items[1];
    INT mark_index;                 /* prob not static */
    static INT type_index;
    INT num_items;
    INT ntracks;
    TRACK_STRUCT *ptrk;
    TRACK_STRUCT *ptrk_data;
    CHAR text[81];
    static TRACK_STRUCT *ptracks;
    static TRACK_STRUCT *copytracks;
    
    switch (item)
        {
	  
    case XS_TRACKDEF_ACCEPT:
	wiqccdi(id, XS_TRACKDEF_INCH_UNIT_BTN, &i, text);
        pxs->track_width_scale_units = 
	  (i == ON) ? XS_INCH_UNITS : XS_CENTIMETER_UNITS;
        /* copy ptemp back to plo when through with changes */
        /* free the individual items pointed to by the plo ll of pointers */
        ntracks = llcount (&pxs->plo->tracks);
        for (i = 1; i <= ntracks; i++)
            {
            status = llgetn (&pxs->plo->tracks, &ptrk, i);
            tc_free (ptrk);
            }
        /* free the ll of pointers */
        status = llfree (&pxs->plo->tracks);
        status = llinit (&pxs->plo->tracks, 0, sizeof(TRACK_STRUCT *), LL_LIST_SYSTEM);
        
        /* find out how many */
        ntracks = llcount (&pxs->ptemp->tracks);
        /* set current ll to first */
        /*status = llfirst(&pxs->ptemp->tracks);*/
        /* allocate copy of the ptemp ll by allocating each piece
           and appending to plo */
        for (i = 1; i <= ntracks; i++)
            {
            /* allocate the TRACK_STRUCT for each item */
            ptrk = (TRACK_STRUCT *)tc_alloc (sizeof(TRACK_STRUCT));
            /* get the nth data item from plo ll */
            status = (i == 1) ? llfirst (&pxs->ptemp->tracks,
                                         &ptrk_data) : llnext (&pxs->ptemp->tracks,
                                                               &ptrk_data);
            hoblockmove (ptrk_data, ptrk, sizeof(TRACK_STRUCT));
            /* now append ptr to ptemp ll */
            status = llappnd (&pxs->plo->tracks, &ptrk);
            }
        /*  status = llcopy(&pxs->ptemp->tracks, &pxs->plo->tracks); */
        status = xs_trackdef_cdlg_term (pxs);
        if (pxs->xst_trackdef_no_redraw = TRUE)
            {
            pxs->changed_since_display = TRUE;
            xs_cs_menu_mgr (pxs);
            }
        break;
        
    case XS_TRACKDEF_IGNORE:
        /* we're done so clean up */
        /* free the individual items pointed to by the ptemp ll of pointers */
        ntracks = llcount (&pxs->ptemp->tracks);
        for (i = 1; i <= ntracks; i++)
            {
            status = llgetn (&pxs->ptemp->tracks, &ptrk, i);
            tc_free (ptrk);
            }
        /* free the ll of pointers */
        status = llfree (&pxs->ptemp->tracks);
        status = xs_trackdef_cdlg_term (pxs);
        break;
        
    case XS_TRACKDEF_SELECT:
        /* determine which track was selected, copy the information into
           the edittext fields - setting the valuators, and then return */
        
        wiqsel (id, XS_TRACKDEF_TRACK, &num_selected, items);
        if (num_selected != 1)
            {
            am_message (AM_ALERT, mg_message (XS_SELECT_TRACK_NUM));
            break;
            }
        index = items[0] - 1;
        if (index >= 0)
            {                       /*  Activate the REPLACE button.        */
            
            wiencdi (id, XS_TRACKDEF_REPLACE, 1);
            
            /* Don't let user delete all tracks because it
               screws up copying attributes when she adds one */
            
            n_trks_left = llcount (&pxs->ptemp->tracks);
            if (n_trks_left > 1)
                wiencdi (id, XS_TRACKDEF_DELETE, 1);
            
            /* set edit text fields */
            
            wiscdt (id, XS_TRACKDEF_TRACKEDT, pxs->track_number_tcp[index]);
            /* set valuators to match input */
            
            type_index = ts_switch (pxs->type_tcp[index], or_track_tcp) + 1;
            if (type_index == 0)
                am_message (AM_ALERT, mg_message (XS_BAD_TYPE));
#ifdef USE_X
            wiscdv (id, XS_TRACKDEF_TYPESEL, type_index);
#else
            wiscdv (id, XS_TRACKDEF_TYPEVAL, type_index);
            wiscdt (id, XS_TRACKDEF_TYPESEL, or_track_tcp[type_index - 1]);
#endif
            mark_index = ts_switch (pxs->marks_tcp[index], or_marks_tcp) + 1;
            if (mark_index == 0)
                {
                am_message (AM_ALERT, mg_message (XS_BAD_MARK));
                }
#ifdef USE_X
            wiscdv (id, XS_TRACKDEF_MARKSEL, mark_index);
#else 
            wiscdv (id, XS_TRACKDEF_MARKVAL, mark_index);
            wiscdt (id, XS_TRACKDEF_MARKSEL, or_marks_tcp[mark_index - 1]);
#endif
            wiscdt (id, XS_TRACKDEF_DIVISNEDT, pxs->numdiv_tcp[index]);
            wiscdt (id, XS_TRACKDEF_STARTEDT, pxs->start_loc_tcp[index]);
            wiscdt (id, XS_TRACKDEF_WIDTHEDT, pxs->width_tcp[index]);
            
            /* set ll current to track being displayed */
            
            /*  count = llcount(&pxs->ptemp->tracks);
                for ( i = 0; i < count; i++)
                {
                status = (i == 0) ? llfirst(&pxs->ptemp->tracks, &ptracks) :
                            llnext(&pxs->ptemp->tracks, &ptracks);
                sscanf(pxs->track_number_tcp[index], "%d", &unused);
                if (ptracks->number == unused) break; 
                }
            */
            /* need am_message */
            /*      if (i == count) printf("\ncurrent is not set\n");*/
            
            status = llgetn (&pxs->ptemp->tracks, &ptracks, index + 1);
            /* wi set default to highlight selected item */
            
            wi_set_default_sel (id, XS_TRACKDEF_TRACK, index + 1);
            wi_set_default_sel (id, XS_TRACKDEF_NUMBER, index + 1);
            wi_set_default_sel (id, XS_TRACKDEF_WIDTH, index + 1);
            wi_set_default_sel (id, XS_TRACKDEF_LOCATION, index + 1);
            wi_set_default_sel (id, XS_TRACKDEF_TYPE, index + 1);
            wi_set_default_sel (id, XS_TRACKDEF_MARKS, index + 1);
            
            }
        else
            index = -1;
        
        break;
        
    case XS_TRACKDEF_REPLACE:
        /* copy the info from the edittext and valuator fields back into the
           list of items (temp). redisplay the entire dialog box with updated
           information
        */
        
        /* IF INDEX < 1 THEN CURRENT NOT SET */
        /* should warn user to SELECT before REPLACE then return to AM*/
        if (index < 0)
            break;
        
        /* save stuff away in structure */
        wiqccdi (id, XS_TRACKDEF_TRACKEDT, &unused, text);
        sscanf (text, "%d", &ptracks->number);
        wiqccdi (id, XS_TRACKDEF_DIVISNEDT, &unused, text);
        sscanf (text, "%d", &ptracks->divisions.number);
        wiqccdi (id, XS_TRACKDEF_WIDTHEDT, &unused, text);
        sscanf (text, "%lf", &ptracks->width);
        wiqccdi (id, XS_TRACKDEF_STARTEDT, &unused, text);
        sscanf (text, "%lf", &ptracks->xorigin);
        
        /* based on valuator, find text, convert to code, save it away */
#ifdef USE_X
        wiqccdi (id, XS_TRACKDEF_MARKSEL, &mark_index, text);
        lu_text_to_index ("LOGPLOT_MARK", &ptracks->divisions.type,
                          or_marks_tcp[mark_index - 1]);
        
        wiqccdi (id, XS_TRACKDEF_TYPESEL, &type_index, text);
        lu_text_to_index ("LOGPLOT_TRACK", &ptracks->type,
                          or_track_tcp[type_index - 1]);
#else
        wiqccdi (id, XS_TRACKDEF_MARKVAL, &mark_index, text);
        lu_text_to_index ("LOGPLOT_MARK", &ptracks->divisions.type,
                          or_marks_tcp[mark_index - 1]);
        
        wiqccdi (id, XS_TRACKDEF_TYPEVAL, &type_index, text);
        lu_text_to_index ("LOGPLOT_TRACK", &ptracks->type,
                          or_track_tcp[type_index - 1]);
#endif      
        status = xs_trk_dlg_define (pxs);
        /* set counter of items */
        num_items = ts_tcp_len (pxs->track_number_tcp);
        max_selectable = 2;
        wisselt (id, XS_TRACKDEF_TRACK, XS_TRACKDEF_VSC, num_items, max_selectable,
                 pxs->track_number_tcp);
        wiscdt (id, XS_TRACKDEF_TRACKEDT, blank);
        wisselt (id, XS_TRACKDEF_TYPE, XS_TRACKDEF_VSC, num_items, max_selectable,
                 pxs->type_tcp);
        wisselt (id, XS_TRACKDEF_MARKS, XS_TRACKDEF_VSC, num_items, max_selectable,
                 pxs->marks_tcp);
        wisselt (id, XS_TRACKDEF_NUMBER, XS_TRACKDEF_VSC, num_items, max_selectable,
                 pxs->numdiv_tcp);
        wiscdt (id, XS_TRACKDEF_DIVISNEDT, blank);
        wisselt (id, XS_TRACKDEF_LOCATION, XS_TRACKDEF_VSC, num_items, max_selectable,
                 pxs->start_loc_tcp);
        wiscdt (id, XS_TRACKDEF_STARTEDT, blank);
        wisselt (id, XS_TRACKDEF_WIDTH, XS_TRACKDEF_VSC, num_items, max_selectable,
                 pxs->width_tcp);
        wiscdt (id, XS_TRACKDEF_WIDTHEDT, blank);
        max_selectable = 1;
        wi_set_selection_limit (id, XS_TRACKDEF_TRACK, max_selectable);
        wi_set_selection_limit (id, XS_TRACKDEF_TYPE, max_selectable);
        wi_set_selection_limit (id, XS_TRACKDEF_MARKS, max_selectable);
        wi_set_selection_limit (id, XS_TRACKDEF_WIDTH, max_selectable);
        wi_set_selection_limit (id, XS_TRACKDEF_LOCATION, max_selectable);
        wi_set_selection_limit (id, XS_TRACKDEF_NUMBER, max_selectable);
        index = -1;
        wiencdi (id, XS_TRACKDEF_REPLACE, 0);
        wiencdi (id, XS_TRACKDEF_DELETE, 0);
        xsz_check_min_max_for_log (pxs->ptemp);
        
        break;
        
    case XS_TRACKDEF_ADD:
        pxs->xst_trackdef_no_redraw = TRUE;
        
        /* user has entered into the edittext fields certain values, so now
           operate just like replace except to make sure the data is new */
        /* allocate a new one */
        /* should make sure it IS a new one ie match on track number */
        ptracks = (TRACK_STRUCT *)tc_zalloc (sizeof(TRACK_STRUCT));
        
        wiqccdi (id, XS_TRACKDEF_TRACKEDT, &unused, text);
        sscanf (text, "%d", &ptracks->number);
        wiqccdi (id, XS_TRACKDEF_DIVISNEDT, &unused, text);
        sscanf (text, "%d", &ptracks->divisions.number);
        wiqccdi (id, XS_TRACKDEF_WIDTHEDT, &unused, text);
        sscanf (text, "%lf", &ptracks->width);
        wiqccdi (id, XS_TRACKDEF_STARTEDT, &unused, text);
        sscanf (text, "%lf", &ptracks->xorigin);
        /* get strings for marks, type; look up in codes table and store integer */
        /* values in tracks list */
        /* for now */
        /* based on valuator, find text, convert to code, save it away */
#ifdef USE_X
        wiqccdi (id, XS_TRACKDEF_MARKSEL, &mark_index, text);
        lu_text_to_index ("LOGPLOT_MARK", &ptracks->divisions.type,
                          or_marks_tcp[mark_index - 1]);
        
        wiqccdi (id, XS_TRACKDEF_TYPESEL, &type_index, text);
        lu_text_to_index ("LOGPLOT_TRACK", &ptracks->type,
                          or_track_tcp[type_index - 1]);
#else
        wiqccdi (id, XS_TRACKDEF_MARKVAL, &mark_index, text);
        lu_text_to_index ("LOGPLOT_MARK", &ptracks->divisions.type,
                          or_marks_tcp[mark_index - 1]);
        
        wiqccdi (id, XS_TRACKDEF_TYPEVAL, &type_index, text);
        lu_text_to_index ("LOGPLOT_TRACK", &ptracks->type,
                          or_track_tcp[type_index - 1]);

#endif
        status = llfirst (&pxs->ptemp->tracks, &copytracks);
        ptracks->visible = copytracks->visible;
        ptracks->line.color = copytracks->line.color;
        ptracks->divisions.line = copytracks->divisions.line;
        ptracks->line.width = copytracks->line.width;
        status = xsz_copy_track_marks (copytracks, ptracks);
        
        /* do llappnd with current information */
        status = llappnd (&pxs->ptemp->tracks, &ptracks);
        
        /* update tcp and dialog */
        
        status = xs_trk_dlg_define (pxs);
        /* set item count */
        num_items = ts_tcp_len (pxs->track_number_tcp);
        /* set selectors */
        max_selectable = 2;
        wisselt (id, XS_TRACKDEF_TRACK, XS_TRACKDEF_VSC, num_items, max_selectable,
                 pxs->track_number_tcp);
        wiscdt (id, XS_TRACKDEF_TRACKEDT, blank);
        wisselt (id, XS_TRACKDEF_TYPE, XS_TRACKDEF_VSC, num_items, max_selectable,
                 pxs->type_tcp);
        wisselt (id, XS_TRACKDEF_MARKS, XS_TRACKDEF_VSC, num_items, max_selectable,
                 pxs->marks_tcp);
        wisselt (id, XS_TRACKDEF_NUMBER, XS_TRACKDEF_VSC, num_items, max_selectable,
                 pxs->numdiv_tcp);
        wiscdt (id, XS_TRACKDEF_DIVISNEDT, blank);
        wisselt (id, XS_TRACKDEF_LOCATION, XS_TRACKDEF_VSC, num_items, max_selectable,
                 pxs->start_loc_tcp);
        wiscdt (id, XS_TRACKDEF_STARTEDT, blank);
        wisselt (id, XS_TRACKDEF_WIDTH, XS_TRACKDEF_VSC, num_items, max_selectable,
                 pxs->width_tcp);
        wiscdt (id, XS_TRACKDEF_WIDTHEDT, blank);
        
        max_selectable = 1;
        wi_set_selection_limit (id, XS_TRACKDEF_TRACK, max_selectable);
        wi_set_selection_limit (id, XS_TRACKDEF_TYPE, max_selectable);
        wi_set_selection_limit (id, XS_TRACKDEF_MARKS, max_selectable);
        wi_set_selection_limit (id, XS_TRACKDEF_WIDTH, max_selectable);
        wi_set_selection_limit (id, XS_TRACKDEF_LOCATION, max_selectable);
        wi_set_selection_limit (id, XS_TRACKDEF_NUMBER, max_selectable);
        index = -1;
        wiencdi (id, XS_TRACKDEF_REPLACE, 0);
        wiencdi (id, XS_TRACKDEF_DELETE, 0);
        
        xsz_check_min_max_for_log (pxs->ptemp);
        
        break;
        
    case XS_TRACKDEF_DELETE:
        pxs->xst_trackdef_no_redraw = TRUE;
        
        /* user has selected an item in the list. now delete it from the
           data structure, then update the screen to show the changes.
        */
        /* potential fix needed here - get curr addr and free the stuff */
        status = lldela (&pxs->ptemp->tracks);
        
        /* update tcp and dialog */
        
        status = xs_trk_dlg_define (pxs);
        /* count the items */
        num_items = ts_tcp_len (pxs->track_number_tcp);
        max_selectable = 2;
        wisselt (id, XS_TRACKDEF_TRACK, XS_TRACKDEF_VSC, num_items, max_selectable,
                 pxs->track_number_tcp);
        wiscdt (id, XS_TRACKDEF_TRACKEDT, blank);
        wisselt (id, XS_TRACKDEF_TYPE, XS_TRACKDEF_VSC, num_items, max_selectable,
                 pxs->type_tcp);
        wisselt (id, XS_TRACKDEF_MARKS, XS_TRACKDEF_VSC, num_items, max_selectable,
                 pxs->marks_tcp);
        wisselt (id, XS_TRACKDEF_NUMBER, XS_TRACKDEF_VSC, num_items, max_selectable,
                 pxs->numdiv_tcp);
        wiscdt (id, XS_TRACKDEF_DIVISNEDT, blank);
        wisselt (id, XS_TRACKDEF_LOCATION, XS_TRACKDEF_VSC, num_items, max_selectable,
                 pxs->start_loc_tcp);
        wiscdt (id, XS_TRACKDEF_STARTEDT, blank);
        wisselt (id, XS_TRACKDEF_WIDTH, XS_TRACKDEF_VSC, num_items, max_selectable,
                 pxs->width_tcp);
        wiscdt (id, XS_TRACKDEF_WIDTHEDT, blank);
        
        max_selectable = 1;
        wi_set_selection_limit (id, XS_TRACKDEF_TRACK, max_selectable);
        wi_set_selection_limit (id, XS_TRACKDEF_TYPE, max_selectable);
        wi_set_selection_limit (id, XS_TRACKDEF_MARKS, max_selectable);
        wi_set_selection_limit (id, XS_TRACKDEF_WIDTH, max_selectable);
        wi_set_selection_limit (id, XS_TRACKDEF_LOCATION, max_selectable);
        wi_set_selection_limit (id, XS_TRACKDEF_NUMBER, max_selectable);
        index = -1;
        wiencdi (id, XS_TRACKDEF_REPLACE, 0);
        wiencdi (id, XS_TRACKDEF_DELETE, 0);
        break;
#ifndef USE_X
    case XS_TRACKDEF_TYPEVAL:
        /* user hit valuator so determine up/down then reset the valuator
           text if possible. if as top/bottom of list then either do nothing
           or move box to top/bottom and start over */
        wiqccdi (id, XS_TRACKDEF_TYPEVAL, &type_index, text);
        wiscdt (id, XS_TRACKDEF_TYPESEL, or_track_tcp[type_index - 1]);
        break;
        
    case XS_TRACKDEF_MARKVAL:
        /* same as for typeval except for marks */
        wiqccdi (id, XS_TRACKDEF_MARKVAL, &mark_index, text);
        wiscdt (id, XS_TRACKDEF_MARKSEL, or_marks_tcp[mark_index - 1]);
        break;
#endif
    default:
        break;
        
        }
    return SUCCESS;
    
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    Remove dialog for entering track definitions.

Prototype:
    publicdef INT xs_trackdef_cdlg_term(XS_STRUCT *pxs);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    pxs             -(XS_STRUCT *) Pointer to XSEC struct.

Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT xs_trackdef_cdlg_term (XS_STRUCT *pxs)
#else
publicdef INT xs_trackdef_cdlg_term (pxs)
XS_STRUCT *pxs;
#endif
    {
    INT status;
    
    /* clean up the tcp for later use */
    status = ts_tcp_free (pxs->track_number_tcp);
    status = ts_tcp_free (pxs->type_tcp);
    status = ts_tcp_free (pxs->marks_tcp);
    status = ts_tcp_free (pxs->numdiv_tcp);
    status = ts_tcp_free (pxs->width_tcp);
    status = ts_tcp_free (pxs->start_loc_tcp);
    pxs->track_number_tcp = (TCP)NULL;
    pxs->type_tcp = (TCP)NULL;
    pxs->marks_tcp = (TCP)NULL;
    pxs->numdiv_tcp = (TCP)NULL;
    pxs->width_tcp = (TCP)NULL;
    pxs->start_loc_tcp = (TCP)NULL;
    
    pxs->trackdef_in_use = OFF;
    am_release_resource (pxs->trackdef_cdlg_id);
    if (pxs->review_all_switch == ON)
        status = xs_tracesel_cdlg_init (pxs);
    else
        {
        wienmni (pxs->edit_menu_id, TRACK_DEF_MENU_ITEM, ON);
        wienmni (pxs->edit_menu_id, TRACE_SEL_MENU_ITEM, ON);
        wienmni (pxs->edit_menu_id, TRACE_ATT_MENU_ITEM, ON);
        wienmni (pxs->edit_menu_id, CHANGE_ALL_MENU_ITEM, ON);
        }
    return SUCCESS;
    
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    To retrieve from the track structure the data requi-  
    red to fill in the logplot track definition dialog.   
    The track#, type, width, start loc,type and marks are 
    retrieved and converted to strings. Each dialog col-  
    umn requires a pointer to a table of char. pointers.  
    The routine returns an array of 6 such pointers to    
    tables, one for each dialog column.                   

Prototype:
    static INT xs_trk_dlg_define(XS_STRUCT *pxs);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) pxs         -(XS_STRUCT *) Log_plot LISTHEAD structure   (must contain complete 
                        track information).                                  

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PRIVATE to <xs_trk_cdlg.c>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static INT xs_trk_dlg_define (XS_STRUCT *pxs)
#else
static INT xs_trk_dlg_define (pxs)
XS_STRUCT *pxs;
#endif
    {
    TRACK_STRUCT *p_track;
    INT j;
    INT no_tracks;
    INT len_of_type_fld = 20;       /* define string lengths for   */
    SHORT flag;
    CHAR buffer[81];
    INT nchars;
    INT status;
    
    no_tracks = llcount (&pxs->ptemp->tracks);
    
    flag = llfirst (&pxs->ptemp->tracks, &p_track);
    
    /* allocate tcp storage        */
    if (pxs->track_number_tcp != (TCP)NULL)
        status = ts_tcp_free (pxs->track_number_tcp);
    pxs->track_number_tcp = TCP_ALLOCATE (no_tracks);
    
    if (pxs->marks_tcp != (TCP)NULL)
        status = ts_tcp_free (pxs->marks_tcp);
    pxs->marks_tcp = TCP_ALLOCATE (no_tracks);
    
    if (pxs->type_tcp != (TCP)NULL)
        status = ts_tcp_free (pxs->type_tcp);
    pxs->type_tcp = TCP_ALLOCATE (no_tracks);
    
    if (pxs->numdiv_tcp != (TCP)NULL)
        status = ts_tcp_free (pxs->numdiv_tcp);
    pxs->numdiv_tcp = TCP_ALLOCATE (no_tracks);
    
    if (pxs->width_tcp != (TCP)NULL)
        status = ts_tcp_free (pxs->width_tcp);
    pxs->width_tcp = TCP_ALLOCATE (no_tracks);
    
    if (pxs->start_loc_tcp != (TCP)NULL)
        status = ts_tcp_free (pxs->start_loc_tcp);
    pxs->start_loc_tcp = TCP_ALLOCATE (no_tracks);
    
    for (j = 0; j < no_tracks; j++) /* loop on trks, get data      */
        {
        /* allocate string space       */
        pxs->type_tcp[j] = (CHAR *)tc_zalloc (sizeof(CHAR) * (len_of_type_fld + 1));
        
        /* chk track type and create   */
        /* for now - this code breaks down when table changes */
        switch (p_track->type)      /* the appropriate string      */
            {
        case DEPTH_TRACK:
            strcpy (pxs->type_tcp[j], "DEPTH");
            break;
            
        case LINEAR_TRACK:
            strcpy (pxs->type_tcp[j], "LINEAR");
            break;
            
        case LOG_TRACK:
            strcpy (pxs->type_tcp[j], "LOGARITHMIC");
            break;
            
        case TEXT_TRACK:
            strcpy (pxs->type_tcp[j], "TEXT");
            break;
            
        case TIME_TRACK:
            strcpy (pxs->type_tcp[j], "TIME");
            break;
            
        default:
            /* AM_MESSAGE */
            sprintf (buffer, mg_message (XS_TRACK_OUT_OF_RANGE), p_track->type);
            am_message (AM_ALERT, buffer);
            strcpy (pxs->type_tcp[j], "<--->");
            break;
            
            }
        /* allocate marks string space */
        pxs->marks_tcp[j] = (CHAR *)tc_zalloc (sizeof(CHAR) * (len_of_type_fld + 1));
        
        /* chk marks type and create   */
        /* this code breaks down when the table changes */
        switch (p_track->divisions.type)    /* the appropriate string      */
            {
        case NONE:
            strcpy (pxs->marks_tcp[j], "NONE");
            break;
            
        case TICS:
            strcpy (pxs->marks_tcp[j], "TICS");
            break;
            
        case LINES:
            strcpy (pxs->marks_tcp[j], "LINES");
            break;
            
        default:
            /* am_message */
            sprintf (buffer, mg_message (XS_MARK_OUT_OF_RANGE),
                     p_track->divisions.type);
            am_message (AM_ALERT, buffer);
            strcpy (pxs->marks_tcp[j], "<--->");
            break;
            
            }
        /* convert track# to strings   */
        nchars = sprintf (buffer, "%d", p_track->number);
        /*?? if nchars = 0 or less - indicate problem */
        pxs->track_number_tcp[j] = (CHAR *)tc_zalloc (sizeof(CHAR) * (nchars + 1));
        strcpy (pxs->track_number_tcp[j], buffer);
        
        nchars = sprintf (buffer, "%d", p_track->divisions.number);
        pxs->numdiv_tcp[j] = (CHAR *)tc_zalloc (sizeof(CHAR) * (nchars + 1));
        strcpy (pxs->numdiv_tcp[j], buffer);
        
        nchars = sprintf (buffer, "%.2lf", p_track->width);
        pxs->width_tcp[j] = (CHAR *)tc_zalloc (sizeof(CHAR) * (nchars + 1));
        strcpy (pxs->width_tcp[j], buffer);
        
        nchars = sprintf (buffer, "%.2lf", p_track->xorigin);
        pxs->start_loc_tcp[j] = (CHAR *)tc_zalloc (sizeof(CHAR) * (nchars + 1));
        strcpy (pxs->start_loc_tcp[j], buffer);
        
        llnext (&pxs->ptemp->tracks, &p_track);
        }
    /* Dim SELECT if no items are in selector. */
    
    if (no_tracks < 1)
        {
        wiencdi (pxs->trackdef_cdlg_id, XS_TRACKDEF_SELECT, 0);
        }
    else
        {
        wiencdi (pxs->trackdef_cdlg_id, XS_TRACKDEF_SELECT, 1);
        }
    return(SUCCESS);
    }
/* END:     */
