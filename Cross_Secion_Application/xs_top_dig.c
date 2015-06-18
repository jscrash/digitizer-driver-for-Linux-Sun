/* DEC/CMS REPLACEMENT HISTORY, Element XS_TOP_DIG.C*/
/*  18   27-FEB-1992 09:30:31 JESSIE "(SPR 6180) add TVD display of traces and tops"*/
/* *17   18-JUL-1991 17:05:23 JANTINA "(SPR 0) Check for empty tops tcp"*/
/* *16   17-JUL-1991 10:53:35 JESSIE "(SPR 7829) Set input to Upper case# 2127"*/
/* *15    3-MAY-1991 16:32:16 KEE "(SPR -1) Bug fix when porting MOTIF/PRIORGKS, MOTIF/DECGKS, DECW/PRIORGKS, DECW/DECGKS version"*/
/* *14   31-OCT-1990 09:31:51 GILLESPIE "(SPR 37) Sun compile fixes"*/
/* *13   17-AUG-1990 22:44:17 VINCE "(SPR 5644) Code Cleanup"*/
/* *12   30-MAY-1990 18:18:34 SCD "(SPR 5344) Temporarily disable WELL INFO option on pop menu until we have time to fix*/
/*properly."*/
/* *11   20-APR-1990 18:31:28 JULIAN "(SPR 0) fix text leaking"*/
/* *10   18-APR-1990 10:07:22 JULIAN "(SPR 1) fix depth tracks bad aspect ratio"*/
/* *9    14-APR-1990 20:42:32 SCD "(SPR 1) Eliminate redundant screen refreshes."*/
/* *8    18-JAN-1990 16:47:15 GILLESPIE "(SPR 6001) Changes for new well struct"*/
/* *7    20-OCT-1989 11:19:28 GORDON "(SPR 999) remove ts_tcp_free call"*/
/* *6    20-SEP-1989 07:52:04 GORDON "(SPR 100) undo last checkin"*/
/* *5    19-SEP-1989 13:40:53 GILLESPIE "(SPR 100) GULF MODS"*/
/* *4    18-SEP-1989 12:45:53 JULIAN "Gulf mods under SPR 100"*/
/* *3    14-SEP-1989 18:46:24 GORDON "(SPR -1) checkin from gulf"*/
/* *2    14-SEP-1989 16:24:58 GORDON "(SPR 100) Gulf/Sun/Unix mods"*/
/* *1    19-JUN-1989 13:45:45 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element XS_TOP_DIG.C*/
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
#include "esi_xs_gr.h"   /* this includes the rest needed for X-section */
#include "esi_xs_err.h"
#include "esi_xs_msg.h"

#ifndef ESI_AM_H

#include "esi_am.h"

#endif

#ifndef ESI_WI_H

#include "esi_wi.h"

#endif

#ifndef ESI_WM_H

#include "esi_wm.h"

#endif

#ifndef ESI_LL_H

#include "esi_ll.h"

#endif

#ifndef ESI_LU_H

#include "esi_lu.h"

#endif

#ifndef ESI_TC_H

#include "esi_tc.h"

#endif

#ifndef ESI_TS_H

#include "esi_ts.h"

#endif

#ifndef ESI_BT_H

#include "esi_bt.h"

#endif

#ifndef ESI_MG_H

#include "esi_mg.h"

#endif

#ifndef ESI_UG_H

#include "esi_ug.h"

#endif

#include "xs_cdlgs.rh"
#include "xs_menus.rh"

/*** Some local definitions ***/
#define NUM_INFO_ITEMS 5
#define DEBUG       0
#define DEBUG_DISPLAY 0
#define TOP_NAME_LENGTH 9
#define USE_NEW_DISPLAY TRUE
#define CALC_NDC 0

/** PUCK BUTTON DEFINITIONS **/
#define DIGITIZE_KEY        1
#define COLOR_KEY           2
#define WELL_INFO_KEY       3
#define DELETE_TOP_KEY      4
#define COLOR_CANCEL_KEY   -1
static CHAR *dig_menu[10] = 
    {
    "DIGITIZE", "NEW COLOR", "ERASE TOP", "ZOOM IN", "PAN WINDOW", "CANCEL", "DONE",
    "RESET ZOOM", 0
    };

static INT dig_menu_map[10] = 
    {
    DIGITIZE_KEY, COLOR_KEY, DELETE_TOP_KEY, WM_BUTTON_ZOOMIN, WM_BUTTON_PAN,
    WM_BUTTON_CANCEL, WM_BUTTON_DONE, WM_BUTTON_RESET, 0
    };

typedef struct internal_use
    {
    CHAR **tops;
    XS_STRUCT *pxs;
    } INTERNAL_STRUCT;

/* set this flag to true and new tops or tops without a color */
/* will automatically be assigned a unique color */
/* if the flag is FALSE then uninitialized colors are set to NORMAL */
#define AUTO_COLORS      FALSE
static INT use_auto_colors;
static INT init_colors = TRUE;
static INT tdgz_info_cdlg_id = 0;
static TREEHEAD wiw_tree;
static INT use_debug;
static COLOR_INDEX color_background, color_normal, color_blue;

/** temporary location of variables that are looking for a home somewhere */
static COLOR_INDEX curr_top_color;  /* global for whole module */
static INT use_cdlg;
#define DIG_CDLG    1           /* 1 = use dialog, 0 = none */
#define MAX_DEPTH  500000.00
#define WIW_STRUCT  struct well_info_window
WIW_STRUCT
    {
    INT active;
    INT new;
    SEGMENT_ID wiw_seg_id;
    DOUBLE scr_wid;                 /* screen width   */
    DOUBLE scr_ht;                  /* screen height  */
    DOUBLE scr_xmin;                /* wcs x origin of screen  */
    DOUBLE scr_xmax;                /* "   x coord of far right  */
    DOUBLE scr_ymin;                /* wcs y origin   */
    DOUBLE scr_ymax;                /* "   y coord of top of screen  */
    DOUBLE w_wid;                   /* well info window width in same units as screen */
    DOUBLE w_ht;                    /*  "   "       " height */
    DOUBLE w_xmin;                  /*  wcs far left x of well info window (wiw) */
    DOUBLE w_xmax;                  /*  "   "       right x of wiw */
    DOUBLE w_ymin;                  /*  "   bottom or origin y coord */
    DOUBLE w_ymax;                  /*  "   top y coord of wiw */
    DOUBLE puck_x;                  /*  wcs x coord returned by puck reading */
    DOUBLE puck_y;                  /*  "   y  "    "       "       " */
    DOUBLE tb_xmin;                 /*  these are the coords that describe the */
    DOUBLE tb_xmax;                 /*  box in the top left of the wiw that, when */
    DOUBLE tb_ymin;                 /*  pointed to, closes and erases that wiw */
    DOUBLE tb_ymax;                 /*   */
    CHAR **text_tab;                /*  this is a table of character pointers */
    };



/*  to the strings within the wiw */

#if USE_PROTOTYPES
static INT zz_set_attributes (TOP_INFO *ptinfo);
static INT zz_update_bar (TOP_XY_DATA *pxyd);
static INT zz_msg (CHAR *str);
static INT zz_bump_color (TOP_XY_DATA **tarray, COLOR_INDEX new_color);
static INT zz_free_tarray (TOP_XY_DATA **tarray);
static INT zz_draw_bars (TOP_XY_DATA **tarray);
static INT zz_fill_wiw_tcp (WIW_STRUCT *pwiw, LAYOUT_STRUCT *pls, INT *max);
static INT zz_show_wiw (WIW_STRUCT *pwiw, INT max_string);
static  void zz_make_wiw (XS_STRUCT *pxs, DOUBLE y, INT pos, TOP_XY_DATA **tarray,
                              WIW_STRUCT *pwiw);
static INT zz_well_info_manager (XS_STRUCT *pxs, DOUBLE y, TOP_XY_DATA **tarray,
                                     INT pos);
static INT zz_term_wiw ();
static INT zz_hide_wiw ();
static INT xsz_read_puck (XS_STRUCT *pxs, FLOAT *x, FLOAT *y, INT *button);
static INT xsz_xy_to_track (XS_STRUCT *pxs, DOUBLE wcs_x, DOUBLE wcs_y,
                                TOP_XY_DATA **tarray, INT *pos, INT get_depth_flag);

#else
static INT zz_set_attributes ();
static INT zz_update_bar ();
static INT zz_msg ();
static INT zz_bump_color ();
static INT zz_free_tarray ();
static INT zz_draw_bars ();
static INT zz_fill_wiw_tcp ();
static INT zz_show_wiw ();
static INT zz_well_info_manager ();
static INT zz_term_wiw ();
static INT zz_hide_wiw ();
static INT xsz_read_puck ();
static INT xsz_xy_to_track ();
static  void zz_make_wiw ();

#endif

/* File Description ---------------------------------------------------------
Overview:
    Cross Section Tops digitizing module; The main entry point to the 
    Tops Digitizer is XSZ_DIGITIZE_TOP.

Prototypes and one line descriptions of each function in the file.
Public_Functions:
    publicdef INT xsz_digitize_top(XS_STRUCT *pxs, CHAR *sel_top);
    
    publicdef INT xs_tdgz_info_server(INT id, INT item, INTERNAL_STRUCT *parms);
    
    publicdef INT xs_gr_fetch_layout(XS_STRUCT  *pxs, CHAR *uwi, LAYOUT_STRUCT  **pls);

Private_Functions:
    static INT zz_set_attributes(TOP_INFO *ptinfo);

    static INT zz_update_bar(TOP_XY_DATA *pxyd);

    static INT zz_msg(CHAR *str);

    static INT zz_bump_color(TOP_XY_DATA **tarray,COLOR_INDEX new_color);

    static INT zz_free_tarray(TOP_XY_DATA **tarray);

    static INT zz_draw_bars(TOP_XY_DATA **tarray);

    static INT zz_fill_wiw_tcp(WIW_STRUCT  *pwiw,
        LAYOUT_STRUCT  *pls, INT *max);

    static INT zz_show_wiw(WIW_STRUCT *pwiw, INT max_string);

    static void zz_make_wiw(XS_STRUCT *pxs, DOUBLE y, INT pos,
        TOP_XY_DATA **tarray, WIW_STRUCT *pwiw);

    static INT zz_well_info_manager(XS_STRUCT  *pxs, DOUBLE y,
        TOP_XY_DATA **tarray, INT pos);

    static INT zz_term_wiw();

    static INT zz_hide_wiw();
    
    static INT xsz_read_puck(XS_STRUCT *pxs, FLOAT *x, FLOAT *y, INT *button);
    
    static INT xsz_xy_to_track(XS_STRUCT *pxs, DOUBLE wcs_x, DOUBLE wcs_y,
        TOP_XY_DATA **tarray, INT *pos, INT get_depth_flag);

-----------------------------------------------------------------------------*/

/* Function Description -----------------------------------------------------
Description:
    This function performs the actual interactive digitizing.

    A XS_STRUCT pointer, the name of the selected top to digitize &
    it's index position (+1!) in the tops table (TCP) are passed in.

Prototype:
    publicdef INT xsz_digitize_top(XS_STRUCT *pxs, CHAR *sel_top);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    pxs             -(XS_STRUCT *)
    sel_top         -(CHAR *)

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.
    CANCEL
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT xsz_digitize_top (XS_STRUCT *pxs, CHAR *sel_top)
#else
publicdef INT xsz_digitize_top (pxs, sel_top)
XS_STRUCT *pxs;
CHAR *sel_top;
#endif
    {
    TOP_STRUCT top;
    LAYOUT_STRUCT *pls;
    TOP_XY_DATA **tarray;
    TOP_INFO *ptinfo;
    INT i, l, st, puck_st, nlayouts, index, pos, value, button, changed = FALSE;
    CHAR number[20];
    FLOAT x, y;
    CHAR color_string[40], string[127];
    INT id;
    COLOR_INDEX color;
    INT tmp, status, new_flag, count, n;
    TRACK_STRUCT *pt;
    WIW_STRUCT *pwiw;
    UINT color_index = 0;
    static TCP color_menu = (TCP)0;
    static INT *color_menu_map = (INT *)0;
    static TCP or_colors;
    static INT num_or_colors;
    
    /* check our entry conditions */
    if (pxs == ((XS_STRUCT *)0))
        {
        return FAIL;                /* got bad copy of object */
        }
    wmtselect (pxs->drawing.tnr_id);    /* restore def. xform */
    
    /* We need to construct a tcp for GPX-type workstations to put up a 
       menu of color options.  We can't use the raw form of the colors tcp
       from the database because a) it has BACKGROUND, an invalid option
       here, and b) we need a CANCEL option. So, we first get the database
       list of colors, then use it to make our own customized menu tcp */
    
    if (ug_if_gpx () && color_menu == (CHAR **)0)   /*  first time only */
        {
        status = lu_get_tcp ("COLOR", &or_colors, &num_or_colors);
        if (status != SUCCESS)
            {
            am_message (AM_ALERT, mg_message (status));
            return FAIL;
            }
        /* We need 1 more than the database, for CANCEL, but
        we're not using BACKGROUND, so just allocate
        num_or_colors. */
        
        color_menu = TCP_ALLOCATE (num_or_colors);
        color_menu_map = (INT *) tc_zalloc (num_or_colors * sizeof(INT));
        
        for (n = index = 0; n < num_or_colors; n++)
            {
            if (ARE_DIFFERENT (or_colors[n], "BACKGROUND"))
                {
                color_menu[index] = tc_zalloc (strlen (or_colors[n]) + 1);
                strcpy (color_menu[index], or_colors[n]);
                lu_text_to_index ("COLOR", &color_menu_map[index], color_menu[index]);
                ++index;
                }
            }
        color_menu[index] = tc_zalloc (strlen ("CANCEL") + 1);
        strcpy (color_menu[index], "CANCEL");
        color_menu_map[index] = COLOR_CANCEL_KEY;
        }
    if (ug_if_gpx ())
        wm_set_mouse_menu (dig_menu, dig_menu_map);
    
    /* switch that either activates or disables dialog */
#if DIG_CDLG == 1
    use_cdlg = TRUE;
#else
    use_cdlg = FALSE;
#endif
    
    /* switch that activates auto color assignment to tops */
#if AUTO_COLORS == TRUE
    use_auto_colors = TRUE;
#else
    use_auto_colors = FALSE;
#endif
    
    /* switch that enables debug messages to be printed out */
#if DEBUG == TRUE
    use_debug = TRUE;
#else
    use_debug = FALSE;
#endif
    id = pxs->digitize_top_cdlg_id;
    tdgz_info_cdlg_id = id;
    wiqccdi (id, XS_TDGZ_CHK_COLORS, &value, string);
    use_auto_colors = value;
    
    zz_msg (mg_message (XS_INITIALIZE_DIGITIZER));
    
    /* set up some common color variables */
    lu_text_to_index ("COLOR", (INDEX *)&color_background, "BACKGROUND");
    lu_text_to_index ("COLOR", (INDEX *)&color_normal, "NORMAL");
    lu_text_to_index ("COLOR", (INDEX *)&color_blue, "BLUE");
    
    /* init the puck status to SUCCESS initially */
    puck_st = st = SUCCESS;
    
    /*
        check the given top name with the datastructure and determine if it is
        a new top.... if it is, we must create entries for it in the various
        trees before continueing with business as usual.
    */
    
    new_flag = TRUE;
    count = llcount (&pxs->drawing.top_table);
    if (count < 0)
        {
        return FAIL;                /* list is bad, can't continue */
        }
    for (n = 0; n < count; n++)
        {
        status = (n == 0) ? llfirst (&pxs->drawing.top_table,
                                     (BYTE *)&ptinfo) :
                 llnext (&pxs->drawing.top_table, (BYTE *)&ptinfo);
        
        if (NOT (strcmp (ptinfo->top, sel_top)))
            {
            index = (n& 15);
            if (index == 0)
                index = 1;
            new_flag = FALSE;
            break;
            }
        }
    /* if this is a new top, we create it and set it to a NEW state */
    if (new_flag == TRUE)
        {
        if (use_cdlg == TRUE)       /* are dialogs active? */
            {
            sprintf (string, mg_message (XS_CREATING_TOP), sel_top);
            zz_msg (string);
            }
        index = (n& 15);
        if (index == 0)
            index = 1;
        
        ptinfo = ALLOCATE (TOP_INFO);
        lu_text_to_index ("COLOR", (INDEX *)&ptinfo->line.color, "NORMAL");
        lu_index_to_text ("COLOR", color_string, ptinfo->line.color);
        curr_top_color = ptinfo->line.color;
        
        /* default top style */
        ptinfo->line.style = 1;
        ptinfo->line.width = 1.0;
        strcpy (ptinfo->top, sel_top);
        
        if (status == FAIL)
            {
            return FAIL;            /* couldn't add the new tops */
            }
        /* initialize the btree of tops, each one is set to invalid */
        status = btinit (&ptinfo->uwis, 0, sizeof(UWI), sizeof(TOP_STRUCT),
                         BT_KEY_IS_C_STRING | BT_TREE_SYSTEM);
        
        count = llcount (&pxs->drawing.log_plots);
        if (count <= 0 OR (status != SUCCESS))
            {
            return FAIL;            /* list is bad, can't continue */
            }
        /* loop on layouts, for each, init a new top */
        for (n = 0; n < count; n++)
            {
            status = (n == 0) ? llfirst (&pxs->drawing.log_plots,
                                         (BYTE *)&pls) :
                     llnext (&pxs->drawing.log_plots, (BYTE *)&pls);
            /* if the list is bad, abort */
            if (status < 0)
                {
                return FAIL;        /* list is bad, can't continue */
                }
            top.Depth.md = HUGE_DEPTH / 1.5;
            btadd (&ptinfo->uwis, (BYTE *)pls->pwell->uwi, (BYTE *)&top);
            }
        }
    else                            /* if it is not a new top, just set up the color info */
        {
        if (lu_index_to_text ("COLOR", color_string,
                              (INDEX)ptinfo->line.color) != SUCCESS)
            {
            lu_text_to_index ("COLOR", (INDEX *)&ptinfo->line.color, "NORMAL");
            lu_index_to_text ("COLOR", color_string, (INDEX)ptinfo->line.color);
            }
        }
    curr_top_color = (ptinfo->line.color == color_background) ?
        color_normal : ptinfo->line.color;
    
    nlayouts = llcount (&pxs->drawing.log_plots);
    
    /* build our well info datastructure */
    
    btinit (&wiw_tree, 0, sizeof(UWI), sizeof(WIW_STRUCT *), BT_KEY_IS_C_STRING);
    
    for (l = 0; l < nlayouts; l++)
        {
        st = (l == 0) ? llfirst (&pxs->drawing.log_plots,
                                 (BYTE *)&pls) : llnext (&pxs->drawing.log_plots,
                                                         (BYTE *)&pls);
        
        pwiw = (WIW_STRUCT *) tc_zalloc (sizeof(WIW_STRUCT));
        
        pwiw->text_tab = (CHAR **) tc_zalloc (sizeof(CHAR *)*(NUM_INFO_ITEMS + 1));
        
        for (i = 0; i < NUM_INFO_ITEMS; i++)
            {
            pwiw->text_tab[i] = (CHAR *) tc_zalloc (sizeof(CHAR) * 41);
            }
        pwiw->active = FALSE;
        pwiw->new = TRUE;
        btadd (&wiw_tree, (BYTE *)pls->pwell->uwi, (BYTE *)&pwiw);
        }
    /* Allocate the table of pointers */
    /* to the top_xy_data structures. */
    
    tarray = (TOP_XY_DATA **) tc_zalloc (sizeof(TOP_XY_DATA *)*(nlayouts + 1));
    
    for (l = 0; l < nlayouts; l++)  /* Traverse the well linked list */
        {                           /* & assign top data to each well*/
        
        st = (l == 0) ? llfirst (&pxs->drawing.log_plots,
                                 (BYTE *)&pls) : llnext (&pxs->drawing.log_plots,
                                                         (BYTE *)&pls);
        
        /* Allocate a structure for this */
        /* well in the table of pointers.*/
        
        tarray[l] = (TOP_XY_DATA *) tc_zalloc (sizeof(TOP_XY_DATA));
        tarray[l]->num_tracks = llcount (&pls->tracks);
        tarray[l]->tpt =
            (TRACK_STRUCT **) tc_zalloc (sizeof(TRACK_STRUCT *)*tarray[l]->num_tracks);
        if (tarray[l]->num_tracks <= 0)
            {
            am_message (AM_STATUS, mg_message (XS_NO_TRACKS_FOUND));
            return FAIL;
            }
        /* load in all the track structs for this well just once. */
        for (i = 0; i < tarray[l]->num_tracks; i++)
            {
            st = (i == 0) ? llfirst (&pls->tracks,
                                     (BYTE *)&pt) : llnext (&pls->tracks, (BYTE *)&pt);
            tarray[l]->tpt[i] = pt;
            }
        tarray[l]->color = curr_top_color;
        strcpy (tarray[l]->color_name, color_string);
        tarray[l]->modified = FALSE;
        tarray[l]->valid = FALSE;
        tarray[l]->pls = pls;
        tarray[l]->xmin = pls->xorigin;
        tarray[l]->xmax = pls->xorigin + pls->width;
        
        /* Find the top info for this well */
        /* only do this if we are not creating a new top */
        if (!new_flag AND (btfind (&ptinfo->uwis, (BYTE *)pls->pwell->uwi,
                                   (BYTE *)&top) == TRUE))
            {
            if (top.Depth.md > (FLOAT)MAX_DEPTH)
                {
                /* this top is not valid */
                tarray[l]->depth = HUGE_DEPTH / 1.5;
                tarray[l]->valid = FALSE;
                tarray[l]->old_bar_y = 0.0;
                }
            else
                {
                tarray[l]->depth = top.Depth.md;
                
                /* calculate the Y from depth */
                tarray[l]->old_bar_y = (pt->yorigin + pt->length) -
                    ((top.Depth.md - pls->interval.track_top) *
                                       (tarray[l]->tpt[0]->length /
                                        (pls->interval.track_base -
                                         pls->interval.track_top)));
                
                tarray[l]->valid = TRUE;
                }
            }
        else                        /* Top for this well is not in BT...add it? */
            {
            top.Depth.md = HUGE_DEPTH / 1.5;
            st = btadd (&ptinfo->uwis, (BYTE *)pls->pwell->uwi, (BYTE *)&top);
            tarray[l]->depth = HUGE_DEPTH / 1.5;
            tarray[l]->valid = FALSE;
            tarray[l]->old_bar_y = 0.0;
            }
        }
    /* End of loop on well logplot linked list. */
    
    wm_hold_updates ();
    
    /* draw the top being worked on in BLINK if it exists */
    if (new_flag != TRUE)
        {
        lu_text_to_index ("COLOR", (INDEX *)&ptinfo->line.color, "HIGHLIGHT");
        xs_gr_draw_top (pxs, ptinfo);
        ptinfo->line.color = curr_top_color;
        zz_draw_bars (tarray);
        }
    if (use_cdlg == TRUE)           /* are dialogs active? */
        {
        /* display the selected tops name*/
        
        sprintf (string, mg_message (XS_BEGIN_DIGITIZE), sel_top,
                 tarray[0]->color_name);
        zz_msg (string);
        }
    /* enable the return of buttons pressed */
    wmtbuttoninterp (OFF);
    while (puck_st == SUCCESS)
        {
        puck_st = xsz_read_puck (pxs, &x, &y, &button);
        
        switch (button)
            {
            /**********************************/
            /*                                */
            /* Display, within the selected   */
            /* layout, a window of well info  */
            /*                                */
            /**********************************/
        case WELL_INFO_KEY:
            if (xsz_xy_to_track (pxs, x, y, tarray, &pos, FALSE) == FAIL)
                break;
            
            zz_well_info_manager (pxs, y, tarray, pos);
            color = ptinfo->line.color;
            wm_gsplci (color);
            zz_set_attributes (ptinfo);
            zz_draw_bars (tarray);
            break;
            
            /**********************************/
            /*                                */
            /* Digitize a top on the well and */
            /* at the depth pointed to.       */
            /*                                */
            /**********************************/
        case DIGITIZE_KEY:
            if (xsz_xy_to_track (pxs, x, y, tarray, &pos, TRUE) == FAIL)
                break;
            
            if (use_cdlg == TRUE)   /* are dialogs active? */
                {
                sprintf (number, mg_message (XS_CURR_DEPTH),
                         (FLOAT)tarray[pos]->depth);
                zz_msg (number);
                }
            tarray[pos]->modified = TRUE;
            tarray[pos]->valid = TRUE;
            zz_update_bar (tarray[pos]);
            break;
            
            /**********************************/
            /*                                */
            /* band a rectangle and zoom in   */
            /*                                */
            /**********************************/
        case WM_BUTTON_ZOOMIN:
            wmtdopuckkey (0.0, 0.0, WM_BUTTON_ZOOMIN);
            zz_draw_bars (tarray);
            zz_msg (mg_message (XS_CONTINUE_DIGITIZING));
            break;
            
            /**********************************/
            /*                                */
            /* restore screen to normal zoom  */
            /*                                */
            /**********************************/
        case WM_BUTTON_RESET:
            wmtdopuckkey (0.0, 0.0, WM_BUTTON_RESET);
            zz_draw_bars (tarray);
            break;
            
            /**********************************/
            /*                                */
            /* Pan the screen                 */
            /*                                */
            /**********************************/
        case WM_BUTTON_PAN:
            wmtdopuckkey (x, y, WM_BUTTON_PAN);
            zz_draw_bars (tarray);
            break;
            
            /**********************************/
            /*                                */
            /* exit digitizer with no changes */
            /*                                */
            /**********************************/
        case WM_BUTTON_CANCEL:
            zz_msg (mg_message (XS_CANCEL_INTERP));
            xs_gr_draw_top (pxs, ptinfo);
            
            /*--------------------------------------------*/
            /* and kill them                              */
            /* Do not zoom out and redraw                 */
            /* wmtdopuckkey( 0.0, 0.0, WM_BUTTON_RESET ); */
            /*--------------------------------------------*/
            zz_term_wiw ();
            
            zz_free_tarray (tarray);    /* free the datastructure */
            
            wmtbuttoninterp (ON);
            return CANCEL;
            
            /**********************************/
            /*                                */
            /* Save the digitized tops in the */
            /* data structure and exit        */
            /*                                */
            /**********************************/
        case WM_BUTTON_DONE:
            
            wmsdelete (ptinfo->seg_id);
            /* ----------------------------- */
            /* Hide the old correlation line */
            /* behind background color, so   */
            /* things look OK until a screen */
            /* refresh occurs                */
            /* ----------------------------- */
            ptinfo->line.color = color_background;
            xs_gr_draw_top (pxs, ptinfo);
            /* ----------------------------- */
            /* Reset correlation color to    */
            /* the true color for this top   */
            /* ----------------------------- */
            ptinfo->line.color = curr_top_color;
            ptinfo->visible = TRUE;
            
            changed = FALSE;
            /* update the depth.md in each of the tops */
            for (i = 0; tarray[i] != ((TOP_XY_DATA *)0); i++)
                {
                
                /* Any valid mods signal an altered top. */
                if (tarray[i]->modified == TRUE AND (tarray[i]->valid == TRUE))
                    {
                    changed = TRUE;
                    }
                st = btfind (&ptinfo->uwis, (BYTE *)tarray[i]->pls->pwell->uwi,
                             (BYTE *)&top);
                
                if (st != TRUE)
                    continue;
                
                /* Only update the top if it has both been */
                /* modified and left in a valid state      */
                
                if (tarray[i]->modified == TRUE AND (tarray[i]->valid == TRUE))
                    {
                    top.Depth.md = (FLOAT)tarray[i]->depth;
                    st = btrepcur (&ptinfo->uwis, (BYTE *)tarray[i]->pls->pwell->uwi,
                                   (BYTE *)&top);
                    }
                }
            if (changed == TRUE)
                {
                if (new_flag == TRUE)
                    {
                    /* add our new top to the datastructure */
                    status = llappnd (&pxs->drawing.top_table, (BYTE *)&ptinfo);
                    
                    sprintf (string, mg_message (XS_CREATED_TOP), sel_top);
                    }
                else
                    {
                    sprintf (string, mg_message (XS_UPDATING_TOP), sel_top);
                    }
                }
            else
                {
                /* come here if no changes were made */
                if (new_flag == TRUE)
                    {
                    sprintf (string, mg_message (XS_TOP_NOT_CREATED), sel_top);
                    }
                else
                    {
                    sprintf (string, mg_message (XS_TOP_NOT_CHANGED), sel_top);
                    }
                }
            zz_msg (string);
            
            zz_term_wiw ();
            
            xs_gr_draw_top (pxs, ptinfo);   /* draw the final version */
            
            /* Do not zoom out and redraw */
            /* wmtdopuckkey( 0.0, 0.0, WM_BUTTON_RESET ); */
            zz_free_tarray (tarray);
            wmtbuttoninterp (ON);
            /* MOD BY JJC. The returned status should not been decided by change status.
                Because the user could retrieve the existing tops, but keep their original
                values on the layout.
                        return (changed == TRUE) ? SUCCESS : CANCEL;
             */
            return SUCCESS;
            
            /**********************************/
            /*                                */
            /* 'remove' the top from the well */
            /* pointed to by setting it's     */
            /* depth to a huge number and     */
            /* erasing the top from the screen*/
            /*                                */
            /**********************************/
        case DELETE_TOP_KEY:
            if (xsz_xy_to_track (pxs, x, y, tarray, &pos, FALSE) == FAIL)
                break;
            
            tarray[pos]->depth = HUGE_DEPTH / 1.5;
            tarray[pos]->modified = TRUE;
            tarray[pos]->deleted = TRUE;
            
            /* preserve the top color while we erase in background */
            tmp = tarray[pos]->color;
            tarray[pos]->color = color_background;
            zz_update_bar (tarray[pos]);
            tarray[pos]->color = tmp;
            
            break;
            
            /**********************************/
            /*                                */
            /* increment the tops color       */
            /*                                */
            /**********************************/
        case COLOR_KEY:
            if (ug_if_gpx () == FALSE)
                zz_bump_color (tarray, -1);
            
            else
                {
                /* put up a menu of colors */
                wm_set_mouse_menu (color_menu, color_menu_map);
                
                wm_get_choice (x, y, pxs->drawing.tnr_id, (INT *)&color_index);
                if (color_index != COLOR_CANCEL_KEY)
                    zz_bump_color (tarray, color_index);
                
                wm_set_mouse_menu (dig_menu, dig_menu_map);
                }
            zz_draw_bars (tarray);
            break;
            
        default:
            break;
            }
        }
    /* bad puck st - cleanup and exit */
    return FAIL;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:

Prototype:
    static INT zz_set_attributes(TOP_INFO *ptinfo);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    ptinfo          -(TOP_INFO *)

Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PRIVATE to <xs_top_dig.c>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static INT zz_set_attributes (TOP_INFO *ptinfo)
#else
static INT zz_set_attributes (ptinfo)
TOP_INFO *ptinfo;
#endif
    {
    INT font, prec;
    
    wm_gsln (ptinfo->line.style);
    wm_gslwsc (ptinfo->line.width);
    wm_gsplci (ptinfo->line.color);
    lu_text_to_index ("FONT", &font, "SIMPLEX");
    lu_text_to_index ("TEXT_PRECISION", &prec, "STROKE");
    wm_gstxfp (font, prec);
    
    return SUCCESS;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    Update bar - draw a bar on the picked layout at the depth selected
    and erase the previous pick for this layout if any.

Prototype:
    static INT zz_update_bar(TOP_XY_DATA *pxyd);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    pxyd            -(TOP_XY_DATA *)

Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PRIVATE to <xs_top_dig.c>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static INT zz_update_bar (TOP_XY_DATA *pxyd)
#else
static INT zz_update_bar (pxyd)
TOP_XY_DATA *pxyd;
#endif
    {
    DOUBLE bar_xmin, bar_xmax;
    GKS_REAL xarray[5], yarray[5];
    INT lnpts;
    LINE_ATTRIBUTE line;
    
    bar_xmin = pxyd->pls->xorigin;
    bar_xmax = pxyd->pls->width + bar_xmin;
    
    line.style = 1;
    line.width = 1.0;
    lnpts = 2;
    wm_gsln (line.style);
    wm_gslwsc (line.width);
    
    xarray[0] = bar_xmin;
    xarray[1] = bar_xmax;
    
    pxyd->xmin = bar_xmin;
    pxyd->xmax = bar_xmax;
    
    /* erase the old bar if any */
    if (pxyd->old_bar_y > 0.0)
        {
        /* set the erase color */
        line.color = color_background;
        wm_gsplci (line.color);
        yarray[0] = yarray[1] = pxyd->old_bar_y;
        wm_gpl (lnpts, xarray, yarray);
        }
    /* draw new bar pos */
    line.color = pxyd->color;       /* tempoarily hard coded 4/8/87 */
    wm_gsplci (line.color);
    yarray[0] = yarray[1] = pxyd->wcs_y;
    wm_gpl (lnpts, xarray, yarray);
    
    pxyd->old_bar_y = pxyd->wcs_y;
    
    return SUCCESS;
    
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    Top digitizing control dialog initialize.

Prototype:
    publicdef INT xs_tdgz_info_server(INT id, INT item, INTERNAL_STRUCT *parms);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    id              -(INT)
    item            -(INT)
    parms           -(INTERNAL_STRUCT *)

Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT xs_tdgz_info_server (INT id, INT item, INTERNAL_STRUCT *parms)
#else
publicdef INT xs_tdgz_info_server (id, item, parms)
INT id, item;
INTERNAL_STRUCT *parms;
#endif
    {
    INT status = SUCCESS;
    INT selected_list[3], total_tops = 0, value;
    static CHAR selected_top[64];
    XS_STRUCT *pxs;
    CHAR **tops;
    INT not_found, num_items;
    static BOOL color_mode = 0;
    GKS_INT defmod, regmod;
    DOUBLE min_char_height;
    
    tops = parms->tops;
    pxs = parms->pxs;
    switch (item)
        {
    case XS_TDGZ_BTN_DGZ:
        wiqccdi (id, XS_TDGZ_SELECT_TOP, &value, selected_top);
	ts_sto_upper(selected_top);
        /* check if the selected top is in the selector list */
        if (strlen (selected_top) == 0)
            break;
        if (selected_top[0] == ' ')
            {
            selected_top[0] = '\0';
            break;
            }
        not_found = TRUE;
        while (*tops != (CHAR *)0)
            {
            if (ARE_SAME (*tops, selected_top))
                {
                not_found = FALSE;
                break;
                }
            tops++;
            }
        if (not_found == TRUE)
            {
            status = xsz_get_top (pxs, selected_top,/* name  */
                    (COLOR_INDEX)1, /* color */
                    (INDEX)1,       /* style */
                    1.0);           /* thick */
            
            }
        if (strlen (selected_top) > 0)
            {
            wiencdi (id, XS_TDGZ_CHK_COLORS, OFF);
            wiencdi (id, XS_TDGZ_SELECT_LIST, OFF);
            wiencdi (id, XS_TDGZ_SELECT_TOP, OFF);
            wiencdi (id, XS_TDGZ_BTN_SELECT, OFF);
            wiencdi (id, XS_TDGZ_BTN_CANCEL, OFF);
            wiencdi (id, XS_TDGZ_BTN_DGZ, OFF);
            wmwopen (pxs->drawing.window_id);
            
            /* Save old deferral mode for later restoration. */
            wm_inq_deferral (&defmod, &regmod);
            
            status = xsz_digitize_top (pxs, selected_top);
            wm_set_deferral (defmod, regmod);
            
            wmwclose (pxs->drawing.window_id);
            wiencdi (id, XS_TDGZ_SELECT_LIST, ON);
            wiencdi (id, XS_TDGZ_SELECT_TOP, ON);
            wiencdi (id, XS_TDGZ_BTN_SELECT, ON);
            wiencdi (id, XS_TDGZ_BTN_CANCEL, ON);
            wiencdi (id, XS_TDGZ_BTN_DGZ, ON);
            wiencdi (id, XS_TDGZ_CHK_COLORS, ON);
            zz_msg (mg_message (XS_SELECT_TOP));
            if (status != CANCEL AND not_found == TRUE)
                {
                status = xsz_make_tops_tcp (pxs, &(parms->tops));
                status = ts_tcp_sort ((parms->tops));
                num_items = ts_tcp_len (parms->tops);
                wisselt (id, XS_TDGZ_SELECT_LIST, XS_TDGZ_SELECT_BAR, num_items, 1,
                         parms->tops);
                }
            }
        break;
        
    case XS_TDGZ_BTN_SELECT:
        /* allocates the selected_list for   */
        /* retruned items.                   */
        wiqsel (id, XS_TDGZ_SELECT_LIST, &total_tops, selected_list);
        if (total_tops > 0 && *tops != (CHAR *)0)
            {
            strcpy (selected_top, tops[selected_list[0] - 1]);
            wiscdt (id, XS_TDGZ_SELECT_TOP, selected_top);
            }
        break;
        
    case XS_TDGZ_CHK_COLORS:
        color_mode = !color_mode;
        wiscdv (id, XS_TDGZ_CHK_COLORS, color_mode);
        break;
        
    case XS_TDGZ_BTN_CANCEL:
        am_release_resource (pxs->digitize_top_cdlg_id);
        ts_tcp_free (tops);
	xs_get_hdr_min_height(pxs, &min_char_height);
        xsz_display_tops (pxs, min_char_height);
    default:
        wienmni (pxs->tops_menu_id, DIG_TOPS_MENU_ITEM, ON);
        break;
        }
    return SUCCESS;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    Send a string to the info dialog message window.

Prototype:
    static INT zz_msg(CHAR *str);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    str             -(CHAR *)

Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PRIVATE to <xs_top_dig.c>

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
    wiscdt (tdgz_info_cdlg_id, XS_TDGZ_MSG_BOX, str);
    return SUCCESS;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    Increment top digitizing color.

Prototype:
    static INT zz_bump_color(TOP_XY_DATA  **tarray,COLOR_INDEX  new_color);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    tarray          -(TOP_XY_DATA  **)
    new_color       -(COLOR_INDEX) If -1, increment current color.

Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PRIVATE to <xs_top_dig.c>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static INT zz_bump_color (TOP_XY_DATA **tarray, COLOR_INDEX new_color)
#else
static INT zz_bump_color (tarray, new_color)
TOP_XY_DATA **tarray;
COLOR_INDEX new_color;
#endif
    {
    CHAR color_str[40];
    COLOR_INDEX color;
    INT index;
    CHAR string[35];
    
    /* first, see if we're just incrementing */
    if (new_color == -1)
        color = (tarray[0]->color + 1) & 15;
    else
        color = new_color;
    
    lu_index_to_text ("COLOR", color_str, (INDEX)color);
    
    index = 0;
    while (tarray[index] != ((TOP_XY_DATA *)0))
        {
        tarray[index]->color = color;
        strcpy (tarray[index]->color_name, color_str);
        index++;
        }
    sprintf (string, mg_message (XS_CURRENT_COLOR), color_str);
    zz_msg (string);
    curr_top_color = color;
    
    return SUCCESS;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    Free tarray - a table of pointers to TOP_XY_DATA structures.

Prototype:
    static INT zz_free_tarray(TOP_XY_DATA **tarray);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    taaray          -(TOP_XY_DATA **)

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PRIVATE to <xs_top_dig.c>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static INT zz_free_tarray (TOP_XY_DATA **tarray)
#else
static INT zz_free_tarray (tarray)
TOP_XY_DATA **tarray;
#endif
    {
    TOP_XY_DATA **temp_tarray;
    if (tarray == ((TOP_XY_DATA **)0) OR ((*(tarray)) == ((TOP_XY_DATA *)0)))
        {
        return FAIL;
        }
    temp_tarray = tarray;           /* save the address */
    
    while ((*(tarray)) != ((TOP_XY_DATA *)0))
        {
        tc_free ((BYTE *)(*(tarray++)));
        }
    tc_free ((BYTE *)temp_tarray);
    return SUCCESS;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    Draw bar positions on screen (no segments!).

Prototype:
    static INT zz_draw_bars(TOP_XY_DATA **tarray);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    tarray          -(TOP_XY_DATA **)

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PRIVATE to <xs_top_dig.c>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static INT zz_draw_bars (TOP_XY_DATA **tarray)
#else
static INT zz_draw_bars (tarray)
TOP_XY_DATA **tarray;
#endif
    {
    FLOAT xarray[30], yarray[30];
    INT n;
    LINE_ATTRIBUTE line;
    INT lnpts;
    
    if (tarray == ((TOP_XY_DATA **)0) OR ((*(tarray)) == ((TOP_XY_DATA *)0)))
        {
        return FAIL;
        }
    line.style = 1;
    wm_gsln (line.style);
    
    line.width = 1.0;
    wm_gslwsc (line.width);
    
    line.color = curr_top_color;
    wm_gsplci (line.color);
    
    lnpts = 2;
    for (n = 0; tarray[n] != (TOP_XY_DATA *)0; n++)
        {
        if (tarray[n]->old_bar_y > 0.0 AND tarray[n]->depth < (HUGE_DEPTH / 2.0))
            {
            xarray[0] = tarray[n]->xmin;
            yarray[0] = tarray[n]->old_bar_y;
            xarray[1] = tarray[n]->xmax;
            yarray[1] = tarray[n]->old_bar_y;
            wm_gpl (lnpts, xarray, yarray);
            }
        }
    return SUCCESS;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    Fetch a layout with the uwi given.

Prototype:
    publicdef INT xs_gr_fetch_layout(XS_STRUCT  *pxs, CHAR *uwi, LAYOUT_STRUCT  **pls);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    pxs             -(XS_STRUCT *)
    uwi             -(CHAR *)
    pls             -(LAYOUT_STRUCT **)

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT xs_gr_fetch_layout (XS_STRUCT *pxs, CHAR *uwi, LAYOUT_STRUCT **pls)
#else
publicdef INT xs_gr_fetch_layout (pxs, uwi, pls)
XS_STRUCT *pxs;
CHAR *uwi;
LAYOUT_STRUCT **pls;
#endif
    {
    LAYOUT_STRUCT *playout;
    INT l, st, numlayouts;
    
    if (uwi == ((CHAR *)0) OR (pxs == (XS_STRUCT *)0))
        return FAIL;
    
    *pls = ((LAYOUT_STRUCT *)0);
    
    numlayouts = llcount (&pxs->drawing.log_plots);
    
    if (numlayouts <= 0)
        return FAIL;
    
    for (l = 0; l < numlayouts; l++)
        {
        st = (l == 0) ? llfirst (&pxs->drawing.log_plots,
                                 (BYTE *)&playout) : llnext (&pxs->drawing.log_plots,
                                                             (BYTE *)&playout);
        
        if (st < 0 OR (st == FAIL))
            return FAIL;
        
        if (NOT (strcmp (playout->pwell->uwi, uwi)))
            {
            *pls = playout;
            return SUCCESS;
            }
        }
    return FAIL;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    Display a well info window.

Prototype:
    static void zz_make_wiw(XS_STRUCT *pxs, DOUBLE y, INT pos, TOP_XY_DATA **tarray,
        WIW_STRUCT *pwiw);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    pxs             -(XS_STRUCT *)
    y               -(DOUBLE)
    pos             -(INT)
    tarray          -(TOP_XY_DATA **)
    pwiw            -(WIW_STRUCT *)

Return Value/Status:
    None.
    
Scope:
    PRIVATE to <xs_top_dig.c>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static  void zz_make_wiw (XS_STRUCT *pxs, DOUBLE y, INT pos, TOP_XY_DATA **tarray,
                              WIW_STRUCT *pwiw)
#else
static  void zz_make_wiw (pxs, y, pos, tarray, pwiw)
XS_STRUCT *pxs;
DOUBLE y;
INT pos;
TOP_XY_DATA **tarray;
WIW_STRUCT *pwiw;
#endif
    {
    INT max_string;
    
    pwiw->scr_wid = pxs->drawing.width;
    pwiw->scr_ht = pxs->drawing.height;
    
    pwiw->w_xmin = tarray[pos]->pls->xorigin;
    pwiw->w_xmax = tarray[pos]->pls->width + pwiw->w_xmin;
    
    pwiw->w_ht = pwiw->scr_ht * 0.10;
    pwiw->w_wid = pwiw->w_xmax - pwiw->w_xmin;
    
    pwiw->w_ymin = y - (pwiw->w_ht / 2.0);
    pwiw->w_ymax = y + (pwiw->w_ht / 2.0);
    
    zz_fill_wiw_tcp (pwiw, tarray[pos]->pls, &max_string);
    
    wmsallocate (&pwiw->wiw_seg_id);
    zz_show_wiw (pwiw, max_string);
    wmsclose (pwiw->wiw_seg_id);
    wm_update ();
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:

Prototype:
    static INT zz_fill_wiw_tcp(WIW_STRUCT  *pwiw, LAYOUT_STRUCT  *pls,INT *max);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    pwiw            -(WIW_STRUCT *)
    pls             -(LAYOUT_STRUCT *)
    max             -(INT *)

Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PRIVATE to <xs_top_dig.c>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static INT zz_fill_wiw_tcp (WIW_STRUCT *pwiw, LAYOUT_STRUCT *pls, INT *max)
#else
static INT zz_fill_wiw_tcp (pwiw, pls, max)
WIW_STRUCT *pwiw;
LAYOUT_STRUCT *pls;
INT *max;
#endif
    {
    INT max_len;
    WELL_HDR_1 *pwell1 = (WELL_HDR_1 *)pls->pwell->detail.ptr;
    
    sprintf (pwiw->text_tab[0], "UWI: %s", pls->pwell->uwi);
    sprintf (pwiw->text_tab[1], "Operator: %s", pwell1->operator);
    sprintf (pwiw->text_tab[2], "Well name: %s", pwell1->name);
    sprintf (pwiw->text_tab[3], "Well number: %s", pwell1->number);
    sprintf (pwiw->text_tab[4], "Elev: %.2f,   Ref: %s", pwell1->elevation.value,
             pwell1->elevation.ref);
    
    max_len = 0;
    max_len = MAX (max_len, strlen (pwiw->text_tab[0]));
    max_len = MAX (max_len, strlen (pwiw->text_tab[1]));
    max_len = MAX (max_len, strlen (pwiw->text_tab[2]));
    max_len = MAX (max_len, strlen (pwiw->text_tab[3]));
    max_len = MAX (max_len, strlen (pwiw->text_tab[4]));
    
    *max = max_len;
    return SUCCESS;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:

Prototype:
    static INT zz_show_wiw(WIW_STRUCT  *pwiw,INT max_string);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    pwiw            -(WIW_STRUCT *)
    max_string      -(INT)

Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PRIVATE to <xs_top_dig.c>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static INT zz_show_wiw (WIW_STRUCT *pwiw, INT max_string)
#else
static INT zz_show_wiw (pwiw, max_string)
WIW_STRUCT *pwiw;
INT max_string;
#endif
    {
    INT line_type, faints, fastyle;
    static CHAR test_str[] = 
        {
        "MMaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
        };
    CHAR text[50];
    
    FLOAT xarray[10], yarray[10];
    DOUBLE xmin, xmax, ymin, ymax, dy;
    DOUBLE exp;
    DOUBLE chh, char_size, text_xspace, text_y;
    INT npts, i;
    LINE_ATTRIBUTE line;
    
    line.style = 1;
    wm_gsln (line.style);
    line.width = 2.0;
    wm_gslwsc (line.width);
    faints = 1;
    wm_gsfais (faints);
    fastyle = 1;
    wm_gsfasi (fastyle);
    
    xarray[0] = pwiw->w_xmin;
    xarray[1] = pwiw->w_xmin;
    xarray[2] = pwiw->w_xmax;
    xarray[3] = pwiw->w_xmax;
    xarray[4] = pwiw->w_xmin;
    
    yarray[0] = pwiw->w_ymin;
    yarray[1] = pwiw->w_ymax;
    yarray[2] = pwiw->w_ymax;
    yarray[3] = pwiw->w_ymin;
    yarray[4] = pwiw->w_ymin;
    
    npts = 5;
    
    wm_gsfaci (color_background);
    wm_gsplci (color_blue);
    
    wm_gfa (npts, xarray, yarray);
    wm_gpl (npts, xarray, yarray);
    
    line.style = 3;                 /* Holly gadzucks ! What is a URTIUT(*^%*(%^&(% 3???? */
    wm_gsln (line.style);
    line.width = 1.0;
    wm_gslwsc (line.width);
    wm_gsplci (color_normal);
    wm_gpl (npts, xarray, yarray);
    
    xmin = pwiw->w_xmin + ((pwiw->w_xmax - pwiw->w_xmin) * 0.03);
    xmax = pwiw->w_xmax - ((pwiw->w_xmax - pwiw->w_xmin) * 0.03);
    ymin = pwiw->w_ymin + ((pwiw->w_ymax - pwiw->w_ymin) * 0.05);
    ymax = pwiw->w_ymax - ((pwiw->w_ymax - pwiw->w_ymin) * 0.05);
    
    dy = ymax - ymin;
    
    text_xspace = 0.1;
    wm_gschsp (text_xspace);
    
    chh = dy / (NUM_INFO_ITEMS + 1);
    char_size = chh;
    
    exp = 1.0;
    wm_gschxp (exp);
    
    line.color = color_normal;
    wm_gstxci (line.color);
    
    for (i = 0; i < max_string; i++)
        {
        text[i] = test_str[i];
        }
    text[i] = NUL;
    
    tg_fit_string (text, xmin, ymin, xmax, 0, chh, &char_size);
    wm_gschh (char_size);
    
    text_y = ymax - char_size;
    
    for (i = 0; i < NUM_INFO_ITEMS; i++)
        {
        wm_gtx (xmin, text_y, pwiw->text_tab[i]);
        text_y = text_y - (char_size * 1.2);
        }
    return SUCCESS;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    Well info manager.

Prototype:
    static INT zz_well_info_manager(XS_STRUCT  *pxs, DOUBLE y, TOP_XY_DATA **tarray,
        INT pos);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    pxs             -(XS_STRUCT *)  
    y               -(DOUBLE)
    tarray          -(TOP_XY_DATA **)
    pos             -(INT)

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PRIVATE to <xs_top_dig.c>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static INT zz_well_info_manager (XS_STRUCT *pxs, DOUBLE y, TOP_XY_DATA **tarray,
                                     INT pos)
#else
static INT zz_well_info_manager (pxs, y, tarray, pos)
XS_STRUCT *pxs;
DOUBLE y;
TOP_XY_DATA **tarray;
INT pos;
#endif
    {
    WIW_STRUCT *pwiw;
    INT st;
    
    st = btfind (&wiw_tree, (BYTE *)tarray[pos]->pls->pwell->uwi, (BYTE *)&pwiw);
    if (st != TRUE)
        {
        return FAIL;
        }
    if (pwiw->new == FALSE)
        {
        if (pwiw->active)
            {
            wmsvis (pwiw->wiw_seg_id, GINVIS);
            pwiw->active = FALSE;
            wm_update ();
            return SUCCESS;
            }
        else
            {
            pwiw->active = TRUE;
            wmsvis (pwiw->wiw_seg_id, GVISI);
            return SUCCESS;
            }
        }
    if (pwiw->new == TRUE)
        {
        zz_msg (mg_message (XS_OPEN_WELL_WINDOW));
        
        zz_make_wiw (pxs, y, pos, tarray, pwiw);
        pwiw->active = TRUE;
        pwiw->new = FALSE;
        btadd (&wiw_tree, (BYTE *)tarray[pos]->pls->pwell->uwi, (BYTE *)&pwiw);
        zz_msg (mg_message (XS_CONTINUE_DIGITIZING));
        
        return SUCCESS;
        }
    return FAIL;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    Term wiw - free everything.

Prototype:
    static INT zz_term_wiw(void);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    None.
    
Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PRIVATE to <xs_top_dig.c>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static INT zz_term_wiw (void)
#else
static INT zz_term_wiw ()
#endif
    {
    INT nwiw, n, i, status;
    UWI uwi;
    WIW_STRUCT *pwiw;
    
    nwiw = btcount (&wiw_tree);
    if (nwiw <= 0)
        {
        return SUCCESS;             /* don't free the tree if it is not initialized yet! */
        }
    zz_hide_wiw ();                 /* turn off all the segments */
    for (n = 0; n < nwiw; n++)
        {
        status = (n == 0) ? btfirst (&wiw_tree, (BYTE *)uwi,
                                     (BYTE *)&pwiw) : btnext (&wiw_tree, (BYTE *)uwi,
                                                              (BYTE *)&pwiw);
        i = 0;
        while (pwiw->text_tab[i] != ((CHAR *)0))
            {
            tc_free (pwiw->text_tab[i]);
            i++;
            }
        wmsdelete (pwiw->wiw_seg_id);
        tc_free ((BYTE *)pwiw);
        }
    btfree (&wiw_tree);
    return SUCCESS;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    Hide wiw windows but don't delete them.

Prototype:
    static INT zz_hide_wiw(void);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    None.
    
Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PRIVATE to <xs_top_dig.c>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static INT zz_hide_wiw (void)
#else
static INT zz_hide_wiw ()
#endif
    {
    INT nwiw, n, status;
    UWI uwi;
    WIW_STRUCT *pwiw;
    
    nwiw = btcount (&wiw_tree);
    
    if (nwiw <= 0)
        {
        return FAIL;
        }
    for (n = 0; n < nwiw; n++)
        {
        status = (n == 0) ? btfirst (&wiw_tree, (BYTE *)uwi,
                                     (BYTE *)&pwiw) : btnext (&wiw_tree, (BYTE *)uwi,
                                                              (BYTE *)&pwiw);
        if (status == FAIL)
            am_message (AM_STATUS, mg_message (status));
        wmsvis (pwiw->wiw_seg_id, GINVIS);
        }
    return SUCCESS;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    Read a coordinate from the digitizing puck.

Prototype:
    static INT xsz_read_puck(XS_STRUCT   *pxs, FLOAT *x, FLOAT *y, INT *button);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    pxs             -(XS_STRUCT *)
    x               -(FLOAT *)
    y               -(FLOAT *)
    button          -(INT *)

Return Value/Status:
    Function returns status from wmtlocate.
    
Scope:
    PRIVATE to <xs_top_dig.c>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static INT xsz_read_puck (XS_STRUCT *pxs, FLOAT *x, FLOAT *y, INT *button)
#else
static INT xsz_read_puck (pxs, x, y, button)
XS_STRUCT *pxs;
FLOAT *x, *y;
INT *button;
#endif
    {
    INT wm_st, gks_st, btn;
    GKS_REAL wx, wy;
    
    wmtselect (pxs->drawing.tnr_id);    /* restore def. xform */
    
    gks_st = SUCCESS;
    *x = *y = wx = wy = 0.0;
    
    wm_st = wmtlocate (&gks_st, &wx, &wy, &btn);
    *x = wx;
    *y = wy;
    *button = btn;
    
    return wm_st;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    This function takes a WCS X,Y coordinate and returns a pointer to a
    TOP_XY_DATA structure that contains the following information:

    - A pointer to the layout struct that owns the track pointed to.
    - The # of the actual track that is pointed to.
    - The NWC X,Y equivelents of the WCS X,Y coordinates.
    - The depth pointed to on the selected track.

    If the WCS coordinates did not fall on any particular track, or if
    there is some internally-unresolvable problem with locating the right
    track, the function returns FAIL and a null pointer for the TOP_XY_DATA.
    A SUCCESS return always indicates that the TOP_XY_DATA structure
    contains good data.

Prototype:
    static INT xsz_xy_to_track(XS_STRUCT *pxs,DOUBLE wcs_x,DOUBLE wcs_y, TOP_XY_DATA **tarray, 
        INT *pos,INT get_depth_flag);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    pxs             -(XS_STRUCT *)
    wcs_x           -(DOUBLE)
    wcs_y           -(DOUBLE)
    tarray          -(TOP_XY_DATA **)
    pos             -(INT *)
    get_depth_flag  -(INT)

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PRIVATE to <xs_top_dig.c>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static INT xsz_xy_to_track (XS_STRUCT *pxs, DOUBLE wcs_x, DOUBLE wcs_y,
                                TOP_XY_DATA **tarray, INT *pos, INT get_depth_flag)
#else
static INT xsz_xy_to_track (pxs, wcs_x, wcs_y, tarray, pos, get_depth_flag)
XS_STRUCT *pxs;
DOUBLE wcs_x;
DOUBLE wcs_y;
TOP_XY_DATA **tarray;
INT *pos;
INT get_depth_flag;
#endif
    {
    LAYOUT_STRUCT *pls;
    TRACK_STRUCT *pt;
    
    INT l = 0;
    INT t = 0, found = FALSE, nlayouts = 0;
    
    INT status = SUCCESS;
    CHAR text[240];
    
    nlayouts = llcount (&pxs->drawing.log_plots);
    
    if (nlayouts > 0 AND (tarray != (TOP_XY_DATA **)0))
        {
        /* loop through all layouts: for each layout check all the tracks */
        /* against the coords that we have.  Determine if the point sits  */
        /* within any of the tracks rectangular region. */
        
        for (l = 0; l < nlayouts; l++)
            {
            pls = tarray[l]->pls;
            
            for (found = FALSE, t = 0; t < tarray[l]->num_tracks; t++)
                {
                pt = tarray[l]->tpt[t];
                
                if (wcs_x < pt->xorigin OR (wcs_x > (pt->xorigin + pt->width)))
                    continue;
                
                if (wcs_y < pt->yorigin OR (wcs_y > (pt->yorigin + pt->length)))
                    continue;
                
                /* we've passed through the sieve, it must be right */
                found = TRUE;
                break;
                }
            /* Got the layout if we exit found==true */
            if (found EQUALS TRUE)
                break;
            }
        if (!found)
            {
            status = FAIL;
            }
        *pos = l;                   /* pass back the logplot number indicated. */
        
        /* did we leave the loop for good reasons? */
        if (found AND (pt != (TRACK_STRUCT *)0))
            {
            /* GREAT! at this point we have located the track that is pointed
                to by the coordinate wcs_x, wcs_y */
            
            tarray[l]->wcs_x = wcs_x;
            tarray[l]->wcs_y = wcs_y;
            
            /* if the get_depth_flag is true then record the current depth. */
            if (get_depth_flag == TRUE)
                {
                tarray[l]->depth = ((pt->yorigin + pt->length) - wcs_y) *
                    ((pls->interval.track_base - pls->interval.track_top) /
                                   pt->length) + pls->interval.track_top;
                }
            }
        }
    else
        {
        if (nlayouts > 0)
            strcpy (text, mg_message (XS_INVALID_TOPS_ARRAY));
        else
            strcpy (text, mg_message (XS_LOGPLOT_ERR));
        
        am_message (AM_STATUS, text);
        
        status = FAIL;
        }
    return status;
    }
/* END:     */
