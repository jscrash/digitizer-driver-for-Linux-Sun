/* DEC/CMS REPLACEMENT HISTORY, Element WMZ_CR_GKS_WIDGET.C */
/*  32   11-MAR-1992 18:16:09 SANTIAGO "(SPR -1) another fix for the case where no #ifdef USE_X" */
/*  31   11-MAR-1992 15:36:09 SANTIAGO "(SPR -0) added MACINTOSH code back, FINDER doesn't compile with out it" */
/* *30    4-MAR-1992 17:52:51 SANTIAGO "(SPR -1) whoops forgot the wm_update() when I remerged the code" */
/* *29   28-FEB-1992 14:03:28 SANTIAGO "(SPR -1) fixed multiple refresh problem under Prior GKS" */
/* *28    6-SEP-1991 13:51:38 KEE "(SPR -1) Get rid of the work around logic for PriorGKS Window go to upper-left hand corner */
/*problem.  Fix has been made at x11.c of PriorGKS" */
/* *27   23-JUL-1991 16:46:44 SANTIAGO "(SPR -1) pass MotionNotify events to GKS to allow for rubberbanding" */
/* *26   16-JUL-1991 17:44:28 SANTIAGO "(SPR -1) bug fix for dangling expose of region" */
/* *25   15-JUL-1991 13:24:42 KEE "(SPR -1) New work around for PriorGKs bug base on the new info they provide" */
/* *24    8-JUL-1991 16:34:00 KEE "(SPR -1) Work around for PriorGKS bug (vendor bug) - Window go to upper left hand corner with */
/*non-Motif Window Manager" */
/* *23   18-JUN-1991 13:03:22 KEE "(SPR -1) Change Menu creation method from inconvenience way to convenience way with Motif 1.1 */
/*version" */
/* *22    3-MAY-1991 16:13:56 KEE "(SPR -1) MOTIF/PRIORGKS, MOTIF/DECWGKS, DECW/PRIORGKS, DECW/DECGKS version" */
/* *21   30-OCT-1990 11:32:59 KEE "(SPR -1) Fix compile noise at sun" */
/* *20   24-OCT-1990 11:50:48 JULIAN "(SPR 0) fix compiling errors" */
/* *19   23-OCT-1990 13:57:49 KEE "(SPR -1) Motif and DecWindow Merge" */
/* *18    3-OCT-1990 12:15:17 GILLESPIE "(SPR 101) Merge beta deltas" */
/*  16B2  3-OCT-1990 12:14:18 GILLESPIE "Merge beta deltas" */
/*  16B1 24-SEP-1990 17:35:11 GILLESPIE "Beta Deltas" */
/* *17    9-AUG-1990 22:46:24 GILLESPIE "(SPR 2001) Change host dependency switch" */
/* *16   28-JUN-1990 15:43:34 KEE "(SPR -1) Add menu_item handling under Windows menu" */
/* *15   27-JUN-1990 17:33:51 KEE "(SPR -1) New wi data structure using 2-D array" */
/* *14    7-JUN-1990 10:10:14 GILLESPIE "(SPR 1) Change ansixlib to ansiXlib for Unix compatability" */
/* *13   30-MAY-1990 17:21:20 SCD "(SPR 1) wmz_delete_workstation: ensure event queue is synchronized after widget destroy's." */
/* *12   29-MAY-1990 10:51:17 SCD "(SPR 1) Fix workstation context switch bug" */
/* *11   29-MAY-1990 07:43:47 SCD "(SPR 1) Elimnate window manager confused warning, snap window on resize." */
/* *10   21-MAY-1990 20:31:03 SCD "(SPR 1) Set window manager hints for max window size" */
/* *9    21-MAY-1990 16:17:14 KEE "(SPR -1) Put back DestroyWidget " */
/* *8    21-MAY-1990 12:14:21 KEE "(SPR -1) Add testing before deactivate, Change Destroy to UnmanageChild/ManageChild" */
/* *7    21-MAY-1990 10:09:29 SCD "(SPR 1) Ensure workstation context switch is robust." */
/* *6    21-MAY-1990 08:44:36 SCD "(SPR 1) Add zoom any window facility to wmz_gks_button_handler." */
/* *5    14-MAY-1990 14:31:36 SCD "(SPR 1) Complete multi-window graphics shakeout" */
/* *4     9-MAY-1990 10:40:21 SCD "(SPR 1) Fix gks_resize_notify to only process configure_notify" */
/* *3     5-MAY-1990 21:36:49 SCD "(SPR 1) Fix gks_resize_notify to exclude events with bad width/height." */
/* *2     3-MAY-1990 22:10:12 KEE "(SPR -1) multi window support" */
/* *1     3-MAY-1990 22:09:08 KEE "create independant new gks 'workstation' windows" */
/* DEC/CMS REPLACEMENT HISTORY, Element WMZ_CR_GKS_WIDGET.C */
/* DEC/CMS REPLACEMENT HISTORY, Element WMZ_CR_GKS_WIDGET.C                         */
/* *3    19-SEP-1990 10:25:34 SCD "(SPR -1) initialize color index (2-15) to black for b/w workstation (Kee)"   */
/* *2     9-SEP-1990 22:09:27 SCD "(SPR 0) Temporary addition of event handler for line editor expose events."  */
/* *1    13-AUG-1990 17:22:10 SYSTEM "Finder 6.1"                                   */
/* DEC/CMS REPLACEMENT HISTORY, Element WMZ_CR_GKS_WIDGET.C                         */
/******************************************************************************     */
/*                                                                                  */
/*      Copyright Finder Graphics Systems, Inc. 1989                                */
/*          Unpublished -- All rights reserved                                      */
/*                                                                                  */
/*THIS SOFTWARE IS THE PROPRIETARY PROPERTY OF Finder Graphics Systems, Inc.  AND   */
/*MAY CONTAIN CONFIDENTIAL TRADE SECRET INFORMATION. IT IS LICENSED FOR USE ON THE  */
/*DESIGNATED EQUIPMENT ON WHICH IT WAS ORIGINALLY INSTALLED AND  MAY NOT BE         */
/*MODIFIED, DUPLICATED OR COPIED IN ANY FORM WITHOUT PRIOR WRITTEN CONSENT OF       */
/*                                                                                  */
/*      Finder Graphics Systems, Inc.                                               */
/*      201 Tamal Vista Blvd                                                        */
/*      Corte Madera, CA  USA 94925                                                 */
/*      (415) 927-0100                                                              */
/*                                                                                  */
/*(The above notice does not constitute an admission of publication or              */
/*unrestricted dissemination of the work)                                           */
/*                                                                                  */
/******************************************************************************     */
#ifndef ESI_WM_INT_H
#include "esi_wm_int.h"
#endif

#ifndef ESI_TC_H
#include "esi_tc.h"
#endif

#ifndef ESI_CONF_WS_H
#include "esi_conf_ws.h"
#endif

#ifndef ESI_AM_H
#include "esi_am.h"
#endif

#ifndef ESI_GKS_H
#include "esi_gks.h"
#endif

#ifdef USE_X /* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx */

#ifndef ESI_WM_H
#include "esi_wm.h"
#endif

#ifndef ESI_WM_CLS_H
#include "esi_wm_cls.h"
#endif

#ifndef ESI_WI_INT_X
#include "esi_wi_int.x"
#endif

#include <X11/Xatom.h>
#include <X11/Shell.h>
#include <X11/StringDefs.h>
#include <X11/Intrinsic.h>

#ifndef PRIOR_GKS
#define DEC_GKS
#endif

#ifdef PRIOR_GKS /* pppppppppppppppppppppppppppppppppppppppppppppppppppppppppp */
#ifndef SUN_AGKSTYPE_H
#include "sun_agkstype.h"
#endif
#ifndef SUN_AGKSFUNC_H
#include "sun_agksfunc.h"
#endif
#ifndef SUN_ANSILONG_H
#include "sun_ansilong.h"
#endif
#ifndef PRIOR_ESCCONST_H
#include "prior_escconst.h"
#endif
#elif DEC_GKS /* ppppppppppppppppppppppppppppppddddddddddddddddddddddddddddddd */
#ifndef $DESCRIPTOR
#include "descrip.h"
#endif
#include "gksdefs.h"
#endif /* dddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddd */

#ifndef ESI_X_H
#include "esi_x.h"
#endif

#define display_width DisplayWidth(wiv.current_Display,DefaultScreen(wiv.current_Display))
#define display_height DisplayHeight(wiv.current_Display,DefaultScreen(wiv.current_Display))

#ifdef PRIOR_GKS /* pppppppppppppppppppppppppppppppppppppppppppppppppppppppppp */
#define FINDER_HEADER_H ((HeightOfScreen(DefaultScreenOfDisplay(wiv.current_Display))*10)/100)
#define GKS_W_LEN (display_height-FINDER_HEADER_H)
#elif DEC_GKS /* ppppppppppppppppppppppppppppppddddddddddddddddddddddddddddddd */
#define FINDER_HEADER_H 60
#define GKS_W_LEN (display_height - 65)
/*#define GKS$DECW_COLORMAP_BITMASK GKS$K_DECWINDOWS_WIDGET */
#define GKS$DECW_COLORMAP_BITMASK 0x001000D5
#endif /* dddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddd */

/* ******************** STATIC VARIABLES *********************************** */
static GKS_INT num_gks_win_create = 0 ;
static BOOL wmz_no_gks_input_active = FALSE ;
static CHAR *menu_tcp[] = { "Zoom In", "Zoom Out", "Cancel", NULL } ;

#define BUTTON_ZOOMIN 0
#define BUTTON_ZOOMOUT 1
#define BUTTON_CANCEL 2
/* ************************************************************************* */

#if USE_PROTOTYPES
static XtEventHandler wmz_gks_button_handler(Widget w, XtPointer client_data, XEvent *event, Boolean *continue_to_dispatch ) ;
static XtEventHandler wmz_gks_configure (Widget widget, caddr_t data,
                                             XEvent *event);
static VOID wmz_gks_button_release (Widget widget, INT data, XEvent *event);
static XtCallbackProc front_gks_callback( Widget widget, XtPointer data, XtPointer *callback_data);
static XtEventHandler wmz_gks_handle_expose_event( Widget w, XtPointer client_data, XEvent *event, Boolean *continue_to_dispatch ) ;
static XtEventHandler wmz_gks_handle_resize_event( Widget w, XtPointer client_data, XEvent *event, Boolean *continue_to_dispatch ) ;
/*static*/ XtEventHandler wmz_gks_handle_input_event( Widget w, XtPointer client_data, XEvent *event, Boolean *continue_to_dispatch ) ;
#else
static XtEventHandler wmz_gks_button_handler() ;
static VOID wmz_gks_button_release() ;
static XtCallbackProc front_gks_callback() ;

static XtEventHandler wmz_gks_handle_expose_event() ;
static XtEventHandler wmz_gks_handle_resize_event() ;
/*static*/ XtEventHandler wmz_gks_handle_input_event() ;
#endif

/*
#define DEBUG
*/

/*
 *
 * WMZ_CREATE_GKS_WIDGET
 *
 * Given a viewport specification in NDC, create the required dialog widget
 * and children, and open a new associated GKS workstation.
 *
 * RETURNS
 *     Always SUCCESS
 *
 */

publicdef INT wmz_create_gks_widget( xur, xll, yur, yll, scale_factor )
DOUBLE xur, xll, yur, yll, scale_factor ;
{
    /*
     * Static variables for this function
     */

    static GKS_INT ws_id = 0 ;
    static INT num_wk_create = 0 ;
    
    Arg arglist[10] ;
    INT width = 0 ;
    INT height = 0 ;
    INT i, n, gks_const, status, create_done = 0 ;
    INT last_ws_id = 0, re_used = FALSE ;
    INT x, y, edge = 0 ;
    CHAR st1[80] ;
    WM_CURRENT *wmv_current = wm_static() ;
    GKS_WIDGET_STRUCT *gks_widget_ref ;
    GKS_INT tnr ;
    GKS_REAL aspect_ratio = (yur - yll) / (xur - xll) ;
    XEvent event_return ;
    Window window_widget ;

    XSizeHints hints ;
#ifdef PRIOR_GKS /* pppppppppppppppppppppppppppppppppppppppppppppppppppppppppp */
    struct {
      Display *x_display ;
      Window x_window ;
    } connection ;

    Gescin esc_in ;
    Gescout esc_out ;
    GKS_INT int_data[4] ;
    Gchar *str_data[1] ;
    GKS_REAL ndcMinX = 0.0 ;
    GKS_REAL ndcMaxX = 0.0 ;
    GKS_REAL ndcMinY = 0.0 ;
    GKS_REAL ndcMaxY = 0.0 ;
    GKS_REAL ndcX    = 0.0 ;
    GKS_REAL ndcY    = 0.0 ;
#elif DEC_GKS /* ppppppppppppppppppppppppppppppddddddddddddddddddddddddddddddd */
    GKS_REAL dcMinX  = 0.0 ;
    GKS_REAL dcMaxX  = 0.0 ;
    GKS_REAL dcMinY  = 0.0 ;
    GKS_REAL dcMaxY  = 0.0 ;
    Display *display = (Display *) NULL ;
    Window window, window_child, window_parent, window_out = (Window) NULL ;
#endif /* dddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddd */

#ifdef MOTIF /* mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm */
    XmString labelString = (XmString) NULL ;
    XmFontList fontList  = wiv.Fontlist[ NORMAL_P_TEXT ] ;
#elif DECWINDOWS /* mmmmmmmmmmmmmmmmmmmmmmmmmmmmmwwwwwwwwwwwwwwwwwwwwwwwwwwwww */
    DwtCompString label = (DwtCompString) NULL ;
    DwtCallback callback_arg[2] ;
#endif /* wwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwww */

#ifdef DEBUG
    fprintf( stdout, "wmz_create_gks_widget( %f, %f, %f, %f, %f )\n",
	                  xur, xll, yur, yll, scale_factor ) ; fflush( stdout ) ;
#endif

    wmv_current->wm_gks_button = 0 ; /* reset button */
    for ( i = 0 ; i < wmv_current->max_ws ; i++ ) /* find null widget in gks array */
      {
        if (wmv_current->gks_widget_array[ i ].widget == (Widget) NULL)
	  {
            if (wmv_current->gks_widget_array[ i ].ws_id != 0)
	      {
                last_ws_id = ws_id ; /* store last ws_id and restore later */
                ws_id = wmv_current->gks_widget_array[ i ].ws_id - 1 ;
                /* -1 because in the while(TRUE) loop below would update to correct ws_id */
                re_used = TRUE ;
	      }
            break ;
	  }
      }

    if (i == wmv_current->max_ws)   /* all array elements have been used            */
      {
        num_wk_create++ ;
        wm_gdawk( MAIN_WKID ) ;     /* if MAIN_WKID == 0 testing is unnecessary     */
                                    /* because the gks array elements are filled up */
                                    /* ie. all the gks windows have been used       */
        wmv_current->ws_id =
	  ((num_wk_create % wmv_current->max_ws) == 0) ?
                wmv_current->max_ws :
		(num_wk_create % wmv_current->max_ws) ;
        wm_gacwk( wmv_current->ws_id ) ;
        wmz_front_wk_widget( wmv_current->ws_id ) ;

        return( SUCCESS ) ;
      }
    else
      {
        if (re_used == FALSE)
	  {
            ++num_gks_win_create ;
	  }

        x = display_width - (display_height - FINDER_HEADER_H) ;
        y = FINDER_HEADER_H ;

        edge = GKS_W_LEN ;
        
        width = height = (edge * scale_factor) ;

#ifdef PRIOR_GKS /* pppppppppppppppppppppppppppppppppppppppppppppppppppppppppp */
        if (MAIN_WKID > 0) /* deactivate the current ws */
	  {
            wm_gdawk( MAIN_WKID ) ;
	  }

	if (aspect_ratio < 1.0)         /* horizontal */
	  {
            height *= aspect_ratio ;
	  }
	else if (aspect_ratio > 1.0)    /* vertical   */
	  {
            width /= aspect_ratio ;
	  }
	else if ((xur - xll) < 1.0)     /* or ((yur - yll) < 1.0) */
	  {
            width *= (xur - xll) ;
            height *= (yur - yll) ;
	  }

        ws_id++ ;

        gks_widget_ref = &wmv_current->gks_widget_array[ i ] ;

        if (gks_widget_ref->widget == (Widget) NULL)
            {
            sprintf( st1, "%dx%d+%d+%d", width, height, x, y ) ;
            n = 0 ;
#ifdef MOTIF /* mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm */
            XtSetArg( arglist[n], XmNwaitForWm,        True   ) ; n++ ;
            XtSetArg( arglist[n], XmNallowShellResize, True   ) ; n++ ;
            XtSetArg( arglist[n], XmNgeometry,         st1    ) ; n++ ;
            XtSetArg( arglist[n], XmNx,                x      ) ; n++ ;
            XtSetArg( arglist[n], XmNy,                y      ) ; n++ ;
            XtSetArg( arglist[n], XmNwidth,            width  ) ; n++ ;
            XtSetArg( arglist[n], XmNheight,           height ) ; n++ ;
#elif DECWINDOWS /* mmmmmmmmmmmmmmmmmmmmmmmmmmmmmwwwwwwwwwwwwwwwwwwwwwwwwwwwww */
            XtSetArg( arglist[n], XtNallowShellResize, True   ) ; n++ ;
            XtSetArg( arglist[n], XtNgeometry,         st1    ) ; n++ ;
            XtSetArg( arglist[n], DwtNx,               x      ) ; n++ ;
            XtSetArg( arglist[n], DwtNy,               y      ) ; n++ ;
            XtSetArg( arglist[n], DwtNwidth,           width  ) ; n++ ;
            XtSetArg( arglist[n], DwtNheight,          height ) ; n++ ;
#endif /* wwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwww */
            gks_widget_ref->widget =
	        XtAppCreateShell(
		    st1,
		    NULL,
                    topLevelShellWidgetClass,
                    wiv.current_Display,
		    arglist,
                    n ) ;
            XtRealizeWidget( gks_widget_ref->widget ) ;
#ifdef DEBUG
	    fprintf( stdout, "created %s\n", XtName(gks_widget_ref->widget) ) ;
#endif
            connection.x_display = XtDisplay( gks_widget_ref->widget ) ;
            connection.x_window  = XtWindow ( gks_widget_ref->widget ) ;
/*
 * RJS - this is, in part, the correct way to handle the problem of
 * resizing a scaled drawing. By setting the bit_gravity to ForgetGravity
 * we are guaranteed that an expose event will be generated for the entire
 * area, then we could eliminate the generation of the synthetic expose that
 * we do later on in the code
 *         {
 *	      XSetWindowAttributes attr ;
 *
 *	      attr.bit_gravity = ForgetGravity ;
 *
 *	      XChangeWindowAttributes(
 *	          wiv.current_Display,
 *		  connection.x_window,
 *		  CWBitGravity,
 *		  &attr ) ;
 *	    }
 */
            while ( gopenws(ws_id, &connection, "x11") != SUCCESS &&
		    ws_id < wmv_current->max_ac_ws)
	      {
                ws_id++;
	      }

            if (ws_id >= wmv_current->max_ac_ws)
	      {
                XtDestroyWidget( gks_widget_ref->widget ) ;

                wmv_current->max_ws = num_gks_win_create - 1 ;

                /* reset the maximum activate ws */
                wm_gacwk( MAIN_WKID ) ; /* reactivate current ws since no more */
                                        /* new ws can be created */
                return( FAIL ) ;
	      }

            /* Disable event capturing by the PRIOR GKS X11 workstation handler. */

            esc_in.esc_function = -2601 ;
            esc_in.esc_route    = G_SPECIFIC_WS ;
            esc_in.esc_ws       = ws_id ;
            esc_in.n_ints       = 1 ;
            int_data[0]         = 0 ;
            esc_in.int_array    = int_data ;
            esc_in.n_reals      = 0 ;
            esc_in.n_strings    = 0 ;
            esc_out             = gescape( NULL, &esc_in ) ;

            /* Disable the PRIOR GKS keyboard echo window. */

            esc_in.esc_function = -2603 ;
            esc_out             = gescape( NULL, &esc_in ) ;
            
	    /*
	     * RJS - The nonmaskable parameter should be FALSE for all but
	     * GraphicsExpose, NoExpose, SelectionClear, SelectionRequest,
	     * SelectionNotify, ClientMessage, MappingNotify.
	     */

            XtAddEventHandler( /* handle Expose events */
	        gks_widget_ref->widget,
		ExposureMask,
		FALSE,
                (XtEventHandler) wmz_gks_handle_expose_event,
		(XtPointer) i );

            XtAddEventHandler( /* handle Input events */
	        gks_widget_ref->widget,
                ButtonPressMask|ButtonReleaseMask|KeyPressMask|KeyReleaseMask|PointerMotionMask,
		FALSE,
                (XtEventHandler) wmz_gks_handle_input_event,
		(XtPointer) i );

            XtAddEventHandler( /* handle Resize events */
	        gks_widget_ref->widget,
		StructureNotifyMask,
		FALSE,
                (XtEventHandler) wmz_gks_handle_resize_event,
		(XtPointer) i );
            
            wmv_current->ws_id = ws_id ;
            wm_gacwk( ws_id ) ;
            gks_widget_ref->ws_id        = ws_id ;
            gks_widget_ref->aspect_ratio = aspect_ratio ;
            gks_widget_ref->width        = width ;
            gks_widget_ref->height       = height ;
            
	    /* Add a menu item under the Finder Global Windows menu. */
#ifdef MOTIF /* mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm */
            labelString = XmStringCreateSimple( st1 ) ;
            n = 0 ;
            XtSetArg( arglist[n], XmNlabelString, labelString ) ; n++ ;
            XtSetArg( arglist[n], XmNfontList,    fontList    ) ; n++ ;
	    sprintf( st1, "GKS_MITEM_%d", i + 1 ) ;
            gks_widget_ref->menu_item =
	        XmCreatePushButtonGadget(
		    wiv.main_dialog_menu[0], /* ZEROth pulldown menu */
		    st1,
		    arglist,
		    n ) ;
            XtManageChild( gks_widget_ref->menu_item ) ;
            XmStringFree( labelString ) ;
            
	    XtAddCallback(
	        gks_widget_ref->menu_item,
		XmNactivateCallback,
		(XtCallbackProc) front_gks_callback,
		(XtPointer) i ) ;
#elif DECWINDOWS /* mmmmmmmmmmmmmmmmmmmmmmmmmmmmmwwwwwwwwwwwwwwwwwwwwwwwwwwwww */
	    labelString = DwtLatin1String( st1 ) ;
	    callback_arg[0].proc = (VoidProc) front_gks_callback ;
	    callback_arg[0].tag  = i ;
	    callback_arg[1].proc = (VoidProc) NULL ;
	    XtSetArg( arglist[n], DwtNlabel,            labelString              ) ; n++ ;
	    XtSetArg( arglist[n], DwtNactivateCallback, callback_arg                ) ; n++ ;
	    XtSetArg( arglist[n], DwtNfont,             wiv.Fontlist[NORMAL_P_TEXT] ) ;	n++ ;
            sprintf( st1, "GKS_MITEM_%d", i + 1 ) ;
            gks_widget_ref->menu_item =
	        DwtPushButtonCreate(
		    wiv.main_dialog_menu[0],
		    st1,
		    arglist,
		    n ) ;
            XtManageChild( gks_widget_ref->menu_item ) ;
            XtFree( CompoundString ) ;
#endif /* wwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwww */

            num_wk_create++ ;

#ifdef DEC_GKS
            /* ------------------------------------------------- */
            /* Throw away redundant events on this widget tree   */
            /* ------------------------------------------------- */

            window_widget = XtWindow( gks_widget_ref->widget ) ;

            while( XCheckWindowEvent( wiv.current_Display, window_widget,
                                      ExposureMask | StructureNotifyMask,
                                      &event_return))
	      ;
#endif /* dddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddd */

	  }
      }

    /* adjust ndc window for the first configure */

    ndcMinX = ndcMinY = 0.0 ;
    ndcMaxX = ndcMaxY = 1.0 ;
    
    if (aspect_ratio < 1.0)         /* horizontal */
      {
        ndcY = ndcMaxY - ndcMinY ;
        ndcMinY += (ndcY - ndcY * aspect_ratio) / 2.0 ;
        ndcMaxY -= (ndcY - ndcY * aspect_ratio) / 2.0 ;
      }
    else if (aspect_ratio > 1.0)    /* vertical   */
      {
        ndcX = ndcMaxX - ndcMinX ;
        ndcMinX += (ndcX - ndcX / aspect_ratio) / 2.0 ;
        ndcMaxX -= (ndcX - ndcX / aspect_ratio) / 2.0;
      }
    else if ((xur - xll) < 1.0)     /* or ((yur - yll) < 1.0) */
      {
        ndcMinX = xll ;
        ndcMaxX = xur ;
        ndcMinY = yll ;
        ndcMaxY = yur ;
      }

    wm_gswkwn( ws_id, ndcMinX, ndcMaxX, ndcMinY, ndcMaxY ) ;
    
    /* set window and viewport */

    tnr = 1 ;
    wm_gswn( tnr, 0.0, 1000.0, 0.0, 1000.0 ) ;
    wm_gsvp( tnr, 0.0, 1.0, 0.0, 1.0 ) ;
    wm_gselnt( tnr ) ;

#elif DEC_GKS /* ppppppppppppppppppppppppppppppddddddddddddddddddddddddddddddd */

    while( TRUE )
      {
        /* ------------------------------------------------------------------- */
        /* I assume that if re_used is true, this while loop should be         */
        /* looped thru only once and ws_id is the ws_id of the array[i]        */
        /* ------------------------------------------------------------------- */
        ws_id++ ;
        gks_widget_ref = &wmv_current->gks_widget_array[ i ] ;
        
        if (gks_widget_ref->widget == (Widget) NULL &&
	    gks_widget_ref->child_widget == (Widget) NULL)
	  {
            sprintf( st1, "GKS_%d", i + 1 ) ;
#ifdef MOTIF /* mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm */
	    labelString = XmStringCreateSimple( st1 ) ;
            n = 0 ;
	    XtSetArg( arglist[n], XmNallowOverlap,    FALSE          ) ; n++ ;
	    XtSetArg( arglist[n], XmNautoUnmanage,    FALSE          ) ; n++ ;
	    XtSetArg( arglist[n], XmNdialogTitle,     labelString    ) ; n++ ;
	    XtSetArg( arglist[n], XmNdefaultPosition, FALSE          ) ; n++ ;
	    XtSetArg( arglist[n], XmNx,               x              ) ; n++ ;
	    XtSetArg( arglist[n], XmNy,               y              ) ; n++ ;
            width = height = (edge * scale_factor);
	    XtSetArg( arglist[n], XmNwidth,           width          ) ; n++ ;
	    XtSetArg( arglist[n], XmNheight,          height         ) ; n++ ;
	    XtSetArg( arglist[n], XmNunitType,        XmPIXELS       ) ; n++ ;
            gks_widget_ref->widget =
	        XmCreateBulletinBoardDialog(
		    wiv.mainw,
		    st1,
		    arglist,
                    n ) ;
            XtManageChild( gks_widget_ref->widget ) ;
            XmStringFree( labelString ) ;

            sprintf( st1, "GKS_CHILD_%d", i + 1 ) ;
            n = 0 ;
            XtSetArg( arglist[n], XmNx,                0             ); n++;
            XtSetArg( arglist[n], XmNy,                0             ); n++;
            XtSetArg( arglist[n], XmNwidth,            width         ); n++;
            XtSetArg( arglist[n], XmNheight,           height        ); n++;
	    XtSetArg( arglist[n], XmNnoResize,         FALSE         ); n++;
            XtSetArg( arglist[n], XmNresizePolicy,     XmRESIZE_ANY  ); n++;
            XtSetArg( arglist[n], XmNresizable,        TRUE          ); n++;
	    XtSetArg( arglist[n], XmNunitType,         XmPIXELS      ); n++;
            XtSetArg( arglist[n], XmNbottomAttachment, XmATTACH_FORM ); n++;
            XtSetArg( arglist[n], XmNleftAttachment,   XmATTACH_FORM ); n++;
            XtSetArg( arglist[n], XmNrightAttachment,  XmATTACH_FORM ); n++;
            XtSetArg( arglist[n], XmNtopAttachment,    XmATTACH_FORM ); n++;
            gks_widget_ref->child_widget =
	        XmCreateForm(
		    gks_widget_ref->widget,
                    st1,
		    arglist,
		    n ) ;
#elif DECWINDOWS /* mmmmmmmmmmmmmmmmmmmmmmmmmmmmmwwwwwwwwwwwwwwwwwwwwwwwwwwwww */
            labelString = DwtLatin1String( st1 ) ;
            n = 0 ;
            XtSetArg( arglist[n], DwtNtitle,           labelString   ); n++;
            XtSetArg( arglist[n], DwtNtakeFocus,       TRUE          ); n++;
            XtSetArg( arglist[n], DwtNnoResize,        FALSE         ); n++;
            XtSetArg( arglist[n], DwtNresize,          DwtResizeFixed); n++;
            XtSetArg( arglist[n], DwtNx,               x             ); n++;
            XtSetArg( arglist[n], DwtNy,               y             ); n++;
            width = height = (edge * scale_factor);
            XtSetArg( arglist[n], DwtNwidth,           width         ); n++;
            XtSetArg( arglist[n], DwtNheight,          height        ); n++;
            XtSetArg( arglist[n], DwtNunits,           DwtPixelUnits ); n++;
            gks_widget_ref->widget =
	        DwtDialogBoxPopupCreate(
		    wiv.mainw,
		    st1,
		    arglist,
                    n ) ;
            XtManageChild( gks_widget_ref->widget ) ;
            XtFree( labelString ) ;
            sprintf( st1, "GKS_CHILD_%d", i + 1 ) ;
            n = 0 ;
            XtSetArg( arglist[n], DwtNx,               0             ); n++;
            XtSetArg( arglist[n], DwtNy,               0             ); n++;
            XtSetArg( arglist[n], DwtNwidth,           width         ); n++;
            XtSetArg( arglist[n], DwtNheight,          height        ); n++;
            XtSetArg( arglist[n], DwtNresize,     DwtResizeShrinkWrap); n++;
            XtSetArg( arglist[n], DwtNunits,           DwtPixelUnits ); n++;
            XtSetArg( arglist[n], DwtNadbBottomAttachment, DwtAttachAdb ) ; n++ ;
            XtSetArg( arglist[n], DwtNadbLeftAttachment,   DwtAttachAdb ) ; n++ ;
            XtSetArg( arglist[n], DwtNadbRightAttachment,  DwtAttachAdb ) ; n++ ;
            XtSetArg( arglist[n], DwtNadbTopAttachment,    DwtAttachAdb ) ; n++ ;
            gks_widget_ref->child_widget =
	        DwtAttachedDBCreate(
	            gks_widget_ref->widget,
                    st1,
		    arglist,
		    n ) ;
#endif /* wwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwww */

            XtManageChild( gks_widget_ref->child_widget ) ;
            gks_widget_ref->conid.dsc$w_length  = 0 ;
            gks_widget_ref->conid.dsc$b_dtype   = DSC$K_DTYPE_T ;
            gks_widget_ref->conid.dsc$b_class   = DSC$K_CLASS_S ;
            gks_widget_ref->conid.dsc$a_pointer = gks_widget_ref->conid_buffer ;
            sprintf( gks_widget_ref->conid.dsc$a_pointer, "%d",
		     gks_widget_ref->child_widget ) ;
            gks_widget_ref->conid.dsc$w_length  =
		strlen( gks_widget_ref->conid.dsc$a_pointer ) ;
	  }
        gks_const = GKS$DECW_COLORMAP_BITMASK ;
        /* ------------------------------------------------------------ */
        /* the following while loop should be only loop thru once if    */
        /* re_used condition is true since a good ws_id has been reused */
        /* ------------------------------------------------------------ */
        while( GKS$_SUCCESS != gks$open_ws( &ws_id, &gks_widget_ref->conid,
                                            &gks_const) &&
	      ws_id < wmv_current->max_ac_ws)
	  {
            ws_id++;
	  }
        if (ws_id >= wmv_current->max_ac_ws)
	  {
            XtDestroyWidget( gks_widget_ref->widget ) ;
            XtDestroyWidget( gks_widget_ref->child_widget ) ;
            wmv_current->max_ws = num_gks_win_create - 1 ;
            continue ;
	  }
        if (ws_id > 1 && gks_widget_ref->out_widget == (Widget)NULL)
	  {             /* the 2nd condition is to avoid deactivate more than once  */
            /* ------------------------------------------                           */
            /* Above test does not avoid the deactivation                           */
            /* of nonexistant workstation (e.g., after an                           */
            /* erase map. So for quick fix, we just check                           */
            /* the MAIN_WKID.                                                       */
            /* ------------------------------------------                           */
            if (MAIN_WKID > 0)
                {
                wm_gdawk (MAIN_WKID);
                }
            }
        wmv_current->ws_id = ws_id;
        wm_gacwk (ws_id);
        
        gks_widget_ref->ws_id                    = ws_id ;
        gks_widget_ref->in_esc_size              = sizeof(ESC_DATA_STRUCT) ;
        gks_widget_ref->in_esc_data.num_ints     = 1 ;
        gks_widget_ref->in_esc_data.num_floats   = 0 ;
        gks_widget_ref->in_esc_data.num_strings  = 0 ;
        gks_widget_ref->in_esc_data.ints         = &wmv_current->ws_id ;
        gks_widget_ref->in_esc_data.floats       = (FLOAT *)NULL ;
        gks_widget_ref->in_esc_data.string_sizes = (INT *)NULL ;
        gks_widget_ref->in_esc_data.strings      = (CHAR **)NULL ;
        
        gks_widget_ref->out_esc_size             = sizeof(ESC_DATA_STRUCT) ;
        gks_widget_ref->out_esc_data.num_ints    = 2 ;
        gks_widget_ref->out_esc_data.num_floats  = 0 ;
        gks_widget_ref->out_esc_data.num_strings = 0 ;
        gks_widget_ref->out_esc_data.ints        = gks_widget_ref->int_buffer ;
        gks_widget_ref->out_esc_data.floats      = (FLOAT *)NULL ;
        gks_widget_ref->out_esc_data.string_sizes = (INT *)NULL ;
        gks_widget_ref->out_esc_data.strings     = (CHAR **)NULL ;

        gks_const = GKS$K_ESC_INQ_WINDOW_IDS ;
        gks$escape(
	    &gks_const,
	    &gks_widget_ref->in_esc_data,
            &gks_widget_ref->in_esc_size,
	    &gks_widget_ref->out_esc_data,
            &gks_widget_ref->out_esc_size,
            &gks_widget_ref->esc_data_return_size ) ;
        display = (Display *)gks_widget_ref->int_buffer[0] ;
        window = (Window)gks_widget_ref->int_buffer[1] ;
        gks_widget_ref->out_widget = XtWindowToWidget( display, window ) ;
        if (gks_widget_ref->out_widget != (Widget) NULL)
	  {
            /* create menu item under "Windows" menu */
            n = 0 ;
            sprintf( st1, "GKS_%d", i + 1 ) ;
#ifdef MOTIF /* mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm */
            CompoundString = XmStringCreateSimple( st1 ) ;
            callback_arg[0].callback = (XtCallbackProc)front_gks_callback ;
            callback_arg[0].closure  = (XtPointer)i ;
            callback_arg[1].callback = (XtCallbackProc)NULL ;
            callback_arg[1].closure  = (XtPointer) NULL ;
            XtSetArg( arglist[n], XmNlabelString,      CompoundString      ) ; n++ ;
            XtSetArg( arglist[n], XmNactivateCallback, callback_arg        ) ; n++ ;
            XtSetArg( arglist[n], XmNfontList, wiv.Fontlist[NORMAL_P_TEXT] ) ; n++ ;
            sprintf( st1, "GKS_MITEM_%d", i + 1 ) ;
            gks_widget_ref->menu_item =
                XmCreatePushButtonGadget(
		    wiv.main_dialog_menu[0],
		    st1,
		    arglist,
                    n ) ;
            XtManageChild( gks_widget_ref->menu_item ) ;
            XmStringFree( CompoundString ) ;
#elif DECWINDOWS /* mmmmmmmmmmmmmmmmmmmmmmmmmmmmmwwwwwwwwwwwwwwwwwwwwwwwwwwwww */
	    CompoundString = DwtLatin1String( st1 ) ;
	    callback_arg[0].proc = (VoidProc)front_gks_callback ;
	    callback_arg[0].tag = i ;
            callback_arg[1].proc = (VoidProc)NULL ;
	    XtSetArg( arglist[n], DwtNlabel,            CompoundString ) ; n++ ;
	    XtSetArg( arglist[n], DwtNactivateCallback, callback_arg   ) ; n++ ;
	    XtSetArg( arglist[n], DwtNfont, wiv.Fontlist[NORMAL_P_TEXT]) ; n++ ;
            sprintf( st1, "GKS_MITEM_%d", i + 1 ) ;
            gks_widget_ref->menu_item =
	        DwtPushButtonCreate(
		    wiv.main_dialog_menu[0],
		    st1,
		    arglist,
		    n ) ;
            XtManageChild( gks_widget_ref->menu_item ) ;
            XtFree( CompoundString ) ;
#endif /* wwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwww */

            num_wk_create++;
            
            /* ------------------------------ */
            /* Define the event handler for resizing of the GKS widget */
            /* ------------------------------ */

            XtAddEventHandler(
	        gks_widget_ref->widget,
		StructureNotifyMask,
		TRUE,
	        (XtEventHandler) wmz_gks_configure,
		(caddr_t) i ) ;

            /* ---------------------------------- */
            /* Define the event handler for mouse clicks on the GKS widget */
            /* ---------------------------------- */

            XtAddEventHandler(
	        gks_widget_ref->out_widget,
		ButtonReleaseMask,
		TRUE,
                (XtEventHandler) wmz_gks_button_handler,
		(caddr_t) i ) ;

            /* ---------------------------------------------------- */
            /* Set up the window manager hints for parent of widget */
            /* ---------------------------------------------------- */

            hints.flags = PMaxSize ;
            hints.max_width = width ;
            hints.max_height = height ;
            XSetNormalHints(
	        wiv.current_Display,
		XtWindow( XtParent( gks_widget_ref->widget ) ),
		&hints ) ;

            /* ------------------------------------------------- */
            /* Throw away redundant events on this widget tree   */
            /* ------------------------------------------------- */

            window_parent = XtWindow( XtParent( gks_widget_ref->widget ) ) ;
            window_widget = XtWindow( gks_widget_ref->widget ) ;
            window_child = XtWindow( gks_widget_ref->child_widget ) ;
            window_out = XtWindow( gks_widget_ref->out_widget ) ;
            while( XPending( wiv.current_Display ) > 0 )
	      {
                XNextEvent( wiv.current_Display, &event_return ) ;
                window = event_return.xany.window ;
                if (window == window_parent || window == window_widget ||
                    window == window_child || window == window_out)
		  {
                    continue ;
		  }
                XtDispatchEvent( &event_return ) ;
	      }

            /* ----------------------------------------- */
            /* Finally all done building the GKS widgets */
            /* so we break out of the while loop         */
            /* ----------------------------------------- */

            break ;
	  }
      }
    /* while (TRUE) */
  }   /* else i != wmv_current->max_ws */

  /* set window and viewport */

  tnr = 1 ;
  wm_gswn( tnr, 0.0, 1000.0, 0.0, 1000.0 ) ;
  wm_gsvp( tnr, 0.0, 1.0, 0.0, 1.0 ) ;
  wm_gselnt( tnr ) ;

  /* Go to full wkvp */

  wm_inq_wkvp_max( &dcMinX, &dcMaxX, &dcMinY, &dcMaxY ) ;
  wm_set_wkvp( dcMinX, dcMaxX, dcMinY, dcMaxY ) ;
#endif /* dddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddd */

    if (re_used == TRUE)
        {
	  ws_id = last_ws_id ;
	}

    wmz_init_color() ;

    return( SUCCESS ) ;
}

/*
 *
 * WMZ_GKS_BUTTON_HANDLER
 *
 * The event handler for XButtonReleasedEvent events associated with a GKS widget.
 * This event handler is bound to the widget by wmz_create_gks_widget
 * when the WM window is allocated.
 *
 */

static XtEventHandler wmz_gks_button_handler( w, client_data, event, continue_to_dispatch )
Widget w ;
XtPointer client_data ;
XEvent *event ;
Boolean *continue_to_dispatch ;
{
    wmz_gks_button_release( w, (INT) client_data, event ) ;
}

/*
 *
 * WMZ_GKS_BUTTON_RELEASE
 *
 * the event handler for xbuttonreleasedevent events associated with a gks
 * widget. this event handler is bound to the widget by wmz_create_gks_widget
 * when the wm window is allocated.
 */

static void wmz_gks_button_release( widget, data, event )
Widget widget ;
int data ;
XEvent *event ;
{
    WM_CURRENT *wmv_current = wm_static() ;
    WINDOW_ID window_id = 0 ;
    GKS_INT cur_wk_id, save_wk_id, win_count ;
    int x_root, y_root, button, num_windows ;
    
    /*
     * plant the selected button number in the wm static structure, so we can
     * pick it up in wmz_getxy.
     */

    wmv_current->wm_gks_button = ((XButtonReleasedEvent *)event)->button ;
    
    /*
     * handle zoom-in/out on mouse button 2 so long as we are not seeing an
     * event underneath the gks input code
     */

    if ((wmv_current->wm_gks_button == 2) && !wmz_no_gks_input_active)
      {
        wmz_no_gks_input_active = TRUE ;

        /*
         * context switch workstations if different
         */

        cur_wk_id = wmv_current->gks_widget_array[ data ].ws_id ;
        save_wk_id = MAIN_WKID ;
        if (cur_wk_id != save_wk_id)
	  {
            /* deactivate current workstation id */

            if (save_wk_id > 0)
	      {
                wm_gdawk( save_wk_id ) ;
	      }
            wmv_current->ws_id = cur_wk_id ;

            /* activate the new workstation */

            wm_gacwk( cur_wk_id ) ;
	  }

        /*
         * if more than one wm window associated with this workstation, we
	 * just alert the user that we don't know which window to operate upon
         */

        num_windows = wmz_inq_wk_wins( cur_wk_id ) ;
        if (num_windows > 1)
	  {
            am_message( AM_ALERT, "more than one logical window, cannot zoom!");
            return ;
	  }

        /*
         * look up the associated window_id and allow user to select action
         * from our pop up menu
         */

        window_id = wm_wkid_to_window( cur_wk_id ) ;

        if (window_id > 0)
	  {
            x_root = event->xbutton.x_root ;
            y_root = event->xbutton.y_root ;

            /* --------------- */
            /* Pop up the menu */
            /* --------------- */

            xf_get_choice( x_root, y_root, menu_tcp, &button ) ;
            
            switch( button )
	      {
	        case BUTTON_ZOOMIN :
		    wmwzoom( window_id, TRUE ) ;
#ifdef PRIOR_GKS /* pppppppppppppppppppppppppppppppppppppppppppppppppppppppppp */
		    wm_grsgwk( MAIN_WKID ) ;
#elif DEC_GKS /* ppppppppppppppppppppppppppppppddddddddddddddddddddddddddddddd */
		    wm_update() ;
#endif /* dddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddd */
		break ;
                
		case BUTTON_ZOOMOUT :
		    wmwzoom( window_id, FALSE ) ;
#ifdef PRIOR_GKS /* pppppppppppppppppppppppppppppppppppppppppppppppppppppppppp */
		    wm_grsgwk( MAIN_WKID ) ;
#elif DEC_GKS /* ppppppppppppppppppppppppppppppddddddddddddddddddddddddddddddd */
		    wm_update() ;
#endif /* dddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddd */
		break ;

		case BUTTON_CANCEL :
		default :
		break ;
	      }
	  }

        /* ------------------------------------------- */
        /* Context switch back to original workstation */
        /* ------------------------------------------- */

        if (cur_wk_id != save_wk_id)
	  {
            if (save_wk_id > 0)
	      {
                wm_gdawk( cur_wk_id ) ; /* deactivate current workstation id */
                wmv_current->ws_id = save_wk_id ; /* activate the original workstation */
                wm_gacwk( save_wk_id ) ;
	      }
	  }
        wmz_no_gks_input_active = FALSE ;
      }
}

/*
 * wmz_set_gks_input_active
 *
 * Set our static flag to indicate whether we are currently getting GKS input
 * thru the usual wmz_getxy path.
 * The flag should be set TRUE when wmz_getxy, or any other GKS input function
 * wishes to disable the zoom on mouse button 2
 */

publicdef VOID wmz_set_gks_input_active( active_flag )
BOOL active_flag ;
{
    wmz_no_gks_input_active = active_flag ;
}

/*************************************************************************          */
/*                                                                                  */
/*NAME:    wmz_gks_configure                                                        */
/*                                                                                  */
/*DESCRIPTION:                                                                      */
/*                                                                                  */
/*        The event handler for ConfigureNotify events associated                   */
/*        with a GKS widget.                                                        */
/*                                                                                  */
/*        This event handler is bound to the widget by wmz_create_gks_widget        */
/*        when the WM window is allocated.                                          */
/*                                                                                  */
/*        The GKS workstation viewport is reset to fill the resized widget          */
/*        to the extent possible, preserving aspect ratio.  The widget is           */
/*        then resized ("shrink wrapped") to fit the resulting GKS viewport.        */
/*                                                                                  */
/*        If the affected GKS workstation is different than the current             */
/*        active workstation, we context switch to the resized workstation          */
/*        and back to the current one.                                              */
/*                                                                                  */
/*************************************************************************          */
static XtEventHandler wmz_gks_configure (widget, data, event)
Widget widget;
caddr_t data;
XEvent *event;
{
#ifdef DEC_GKS /* dddddddddddddddddddddddddddddddddddddddddddddddddddddddddddd */
    XConfigureEvent *config = (XConfigureEvent *)event;
    XWindowChanges xwc;
    Window window;
    Window window_parent, window_widget, window_out, window_child;
    DOUBLE gks_aspect;
    Arg arglist[1];
    
    GKS_REAL dcx, dcy;
    GKS_REAL gks_fa, gks_fb;
    GKS_REAL new_width, new_height;
    GKS_INT wktype, regenflag, last_ws_id = MAIN_WKID;
    INT i = (INT)data;
    GKS_INT stat, meters, rsx, rsy;
    WM_CURRENT *wmv_current = wm_static ();
    XEvent event_return;
    INT num_events = 0;
    XSizeHints hints;
    
    window_parent = XtWindow (XtParent (widget));
    window_widget = XtWindow (widget);
    window_child = XtWindow (wmv_current->gks_widget_array[i].child_widget);
    window_out = XtWindow (wmv_current->gks_widget_array[i].out_widget);
    
    /* -------------------------------------------------                            */
    /* If first event received by this event handler is                             */
    /* not the ConfigureNotify event, then throw away                               */
    /* reduntdant events on this widget tree                                        */
    /* until we find the first ConfigureNotify  event                               */
    /* -------------------------------------------------                            */
    if (event->type != ConfigureNotify)
        {
        /* ------------------------------------------                               */
        /* Find and throw away any remaining                                        */
        /* expose events for child or out windows                                   */
        /* ------------------------------------------                               */
        while (TRUE)
            {
            stat = XCheckTypedWindowEvent (wiv.current_Display, window_child, Expose,
                                           &event_return);
            if (!stat)
                break;
            }
        while (TRUE)
            {
            stat = XCheckTypedWindowEvent (wiv.current_Display, window_out, Expose,
                                           &event_return);
            if (!stat)
                break;
            }
        return;
        }
    /* -------------------------------------                                        */
    /* context switch to current workstation                                        */
    /* -------------------------------------                                        */
    if (wmv_current->gks_widget_array[i].ws_id != last_ws_id)
        {
        if (last_ws_id > 0)
            {
            /* deactivate current workstation id                                    */
            wm_gdawk (last_ws_id);
            }
        wmv_current->ws_id = wmv_current->gks_widget_array[i].ws_id;
        /* activate the new one                                                     */
        wm_gacwk (wmv_current->ws_id);
        }
    wktype = GKS$DECW_COLORMAP_BITMASK;
    wm_gqdsp (wktype, &stat, &meters, &dcx, &dcy, &rsx, &rsy);
    
    /* ----------------------------------------------                               */
    /* Now reset the GKS viewport to fit inside the                                 */
    /* resized widget.  We take a conservative                                      */
    /* approach to ensure that the resulting window                                 */
    /* is NEVER larger than the window resized by the                               */
    /* user.                                                                        */
    /* ----------------------------------------------                               */
    if (config->width > 0 && config->height > 0)
        {
        xwc.width = config->width;
        xwc.height = config->height;
        gks_aspect = (DOUBLE)rsy / (DOUBLE)rsx;
        
        /* Wide GKS window snaps to width of X window                               */
        if (gks_aspect < 1.0)
            {
            xwc.width = config->width;
            xwc.height = config->width * gks_aspect;
            }
        /* Tall GKS window snaps to height of X window                              */
        else if (gks_aspect > 1.0)
            {
            xwc.height = config->height;
            xwc.width = config->height / gks_aspect;
            }
        /* Square GKS window snaps to min dimension of X window                     */
        else
            {
            xwc.height = xwc.width = MIN (config->width, config->height);
            }
        /* -----------------------------------------------------                    */
        /* Set up the window manager hints for the parent window                    */
        /* -----------------------------------------------------                    */
        hints.flags = PSize;
        hints.width = xwc.width;
        hints.height = xwc.height;
        XSetNormalHints (wiv.current_Display, XtWindow (XtParent (widget)), &hints);
        XConfigureWindow (wiv.current_Display, XtWindow (XtParent (widget)),
                          CWWidth | CWHeight, &xwc);
        
        new_width = xwc.width * (dcx / rsx);
        new_height = xwc.height * (dcy / rsy);
        gks_fa = 0.0;
        gks_fb = 0.0;
        
        /* ----------------------------------                                       */
        /* Adjust the GKS viewport to conform                                       */
        /* to the new X window size                                                 */
        /* ----------------------------------                                       */
        wm_gswkvp (wmv_current->ws_id, gks_fa, new_width, gks_fb, new_height);
        regenflag = GKS$K_PERFORM_FLAG;
        wm_guwk (wmv_current->ws_id, regenflag);
        }
    /* --------------------------------------                                       */
    /* context switch to original workstation                                       */
    /* --------------------------------------                                       */
    if (wmv_current->ws_id != last_ws_id)
        {
        if (last_ws_id > 0)
            {
            /* deactivate new workstation id                                        */
            wm_gdawk (wmv_current->ws_id);
            wmv_current->ws_id = last_ws_id;
            /* activate the old one                                                 */
            wm_gacwk (last_ws_id);
            }
        }
    /* -------------------------------------------------                            */
    /* Throw away redundant events on this widget tree                              */
    /* -------------------------------------------------                            */
    while (XPending (wiv.current_Display) > 0)
        {
        XNextEvent (wiv.current_Display, &event_return);
        window = event_return.xany.window;
        if (window == window_parent || window == window_widget ||
            window == window_child || window == window_out)
            {
            continue;
            }
        XtDispatchEvent (&event_return);
        }
#endif /* dddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddd */
    return ;
}

/*
 * WMZ_NGKS_WK_CREATED
 *
 * Return the static count of gks windows created so far.
 */

publicdef GKS_INT wmz_ngks_wk_created()
{
  return num_gks_win_create ; /* for ug_term use */
}

#endif /* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx */

/*
 * WMZ_UPDATE_GKS_TITLE
 *
 * Given a window manager object id, and title string:
 * If the object is a window, then set the associated widget title bar to the
 * string.
 *
 * RETURNS
 *     Always SUCCESS
 */

publicdef INT wmz_update_gks_title( obj_id, usr_name )
WM_ID obj_id ;
CHAR *usr_name ;
{
#ifdef USE_X /* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx */
    INT i ;
    GKS_INT obj_typ, wk_id ;
    WM_CURRENT *wmv_current = wm_static () ;
    Arg arglist[1] ;
    CHAR st1[ST_MAC_MAX] ;
#ifdef MOTIF /* mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm */
    XmString labelString = (XmString) NULL  ;
#elif DECWINDOWS /* mmmmmmmmmmmmmmmmmmmmmmmmmmmmmwwwwwwwwwwwwwwwwwwwwwwwwwwwww */
    DwtCompString labelString = (DwtCompString) NULL ;
#endif /* wwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwww */
    wmoinqtyp( obj_id, &obj_typ ) ;
    if (obj_typ == WM_CLASS_WINDOW)
      {
        wk_id = wm_obj_wkid( obj_id ) ;
        for (i = 0 ; i < num_gks_win_create ; i++)
	  {
            if (wmv_current->gks_widget_array[ i ].ws_id == wk_id)
	      {
	        break ;
	      }
	  }
        sprintf( st1, "GKS - %s", usr_name ) ;
#ifdef MOTIF /* mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm */
        XChangeProperty(
	    wiv.current_Display,
	    XtWindow( wmv_current->gks_widget_array[ i ].widget ),
	    XA_WM_NAME,
	    XA_STRING,
	    8,
	    PropModeReplace,
	    st1,
	    strlen( st1 ) ) ;

        XChangeProperty(
	    wiv.current_Display,
	    XtWindow( wmv_current->gks_widget_array[ i ].widget ),
	    XA_WM_ICON_NAME,
	    XA_STRING,
	    8,
	    PropModeReplace,
	    st1,
	    strlen( st1 ) ) ;

        /* update the menu item name under "Window" menu too */

        labelString = XmStringCreateSimple( st1 )  ;
        XtSetArg( arglist[0], XmNlabelString, labelString ) ;
        XtSetValues( wmv_current->gks_widget_array[ i ].menu_item, arglist, 1 ) ;
        XmStringFree( labelString ) ;
#elif DECWINDOWS /* mmmmmmmmmmmmmmmmmmmmmmmmmmmmmwwwwwwwwwwwwwwwwwwwwwwwwwwwww */
        XChangeProperty(
	    wiv.current_Display,
	    XtWindow( wmv_current->gks_widget_array[ i ].widget ),
	    XA_WM_NAME,
	    XA_STRING,
	    8,
	    PropModeReplace,
	    st1,
	    strlen( st1 ) ) ;
        XChangeProperty(
	    wiv.current_Display,
            XtWindow( wmv_current->gks_widget_array[ i ].widget ),
	    XA_WM_ICON_NAME,
	    XA_STRING,
	    8,
	    PropModeReplace,
	    st1,
	    strlen( st1 ) );

        /* update the menu item name under "Window" menu too */

        labelString = DwtLatin1String( st1 ) ;
        XtSetArg( arglist[0], DwtNlabel, labelString ) ;
        XtSetValues( wmv_current->gks_widget_array[ i ].menu_item, arglist, 1 ) ;
        XtFree( CompoundString ) ;
#endif /* wwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwww */
        }
#endif /* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx */
    return( SUCCESS ) ;
}

/*
 * WMZ_FRONT_WK_WIDGET
 *
 *     Given a workstation id, raise the associated widget to the top of
 *     the window stack.
 *
 * RETURNS
 *     Always SUCCESS
 */

publicdef INT wmz_front_wk_widget( wk_id )
GKS_INT wk_id ;
{
#ifdef USE_X /* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx */
    INT i ;
    WM_CURRENT *wmv_current = wm_static() ;
    Window window ;
    XWindowAttributes WMAttributes ;
    
    for (i = 0 ; i < num_gks_win_create ; i++)
      {
        if (wmv_current->gks_widget_array[ i ].ws_id == wk_id)
	  {
            window = XtWindow( wmv_current->gks_widget_array[ i ].widget ) ;

            XRaiseWindow( wiv.current_Display, window ) ;
            XGetWindowAttributes( wiv.current_Display, window, &WMAttributes ) ;

	    /* RJS - can this be replaced with XmProcessTraversal */

            if (WMAttributes.map_state == IsViewable)
	      {
                XSetInputFocus(
		    wiv.current_Display,
		    window,
		    RevertToNone,
		    CurrentTime ) ;
	      }
            return( SUCCESS ) ;
	  }
      }
#endif /* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx */
    return( SUCCESS ) ;
}

/*
 * WMZ_DELETE_WORKSTATION
 *
 * Given a workstation id, deactivate and close the GKS workstation, and
 * destroy the associated widgets.
 *
 * NOTE: It does not appear that we need to context switch workstations.
 *       Is this true?
 *
 * RETURNS
 *
 *     SUCCESS     if workstation deleted
 *     FAIL        if cannot find the workstation id
 */

publicdef INT wmz_delete_workstation( wk_id )
GKS_INT wk_id  ;
{
#ifdef USE_X /* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx */
    INT i ;
    GKS_WIDGET_STRUCT *gws ;
    WM_CURRENT *wmv_current = wm_static() ;
    XEvent event_return ;
    Window window, window_widget ;
#ifdef DEC_GKS /* dddddddddddddddddddddddddddddddddddddddddddddddddddddddddddd */
    Window window_parent, window_out, window_child ;
#endif /* dddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddd */

    /* ---------------------------------- */
    /* Look up the widget associated with the workstation wk_id */
    /* ---------------------------------- */

    if (wmz_inq_wk_wins( wk_id ) < 2) /* if only one window in gks_widget */
      {
        for ( i = 0 ; i < num_gks_win_create ; i++ )
	  {
            if (wmv_current->gks_widget_array[ i ].ws_id == wk_id)
	      {
                gws = &wmv_current->gks_widget_array[ i ] ;
                
                /* ----------------------------------- */
                /* If deleting the current workstation then deactivate before
		 * closing, and set the current workstation to zero
		 */
                /* ----------------------------------- */

                if (gws->ws_id == MAIN_WKID)
		  {
                    wm_gdawk( gws->ws_id ) ;
                    wmv_current->ws_id = 0 ;
		  }

                /* ----------------------------------- */
                /* Close the indicated workstation, destroy the associated
		 * widgets, and clear the GKS_WIDGET_STRUCT entry
		 */
                /* ----------------------------------- */

                wm_gclwk( gws->ws_id ) ;
                
                /* ------------------------------------------------- */
                /* Throw away redundant events on this widget tree   */
                /* ------------------------------------------------- */

                window_widget = XtWindow( gws->widget ) ;
#ifdef DEC_GKS /* dddddddddddddddddddddddddddddddddddddddddddddddddddddddddddd */
                window_parent = XtWindow( XtParent( gws->widget ) ) ;
                window_child  = XtWindow( gws->child_widget ) ;
                window_out    = XtWindow( gws->out_widget ) ;
                
                XtDestroyWidget( gws->out_widget   ) ;
                XtDestroyWidget( gws->child_widget ) ;
#endif /* dddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddd */
                XtDestroyWidget( gws->widget ) ;
                XtDestroyWidget( gws->menu_item ) ; /* menu item under */
		                                    /* "Windows"  menu */

                tc_zeroes( gws, sizeof( GKS_WIDGET_STRUCT ) ) ;

                gws->ws_id = wk_id ; /* store back the wk_id for re_use */

#ifdef DEC_GKS /* dddddddddddddddddddddddddddddddddddddddddddddddddddddddddddd */
                while( XPending( wiv.current_Display ) > 0 )
		  {
                    XNextEvent( wiv.current_Display, &event_return ) ;
                    window = event_return.xany.window ;
                    if (window == window_parent || window == window_widget ||
                        window == window_child || window == window_out)
		      {
                        continue ;
		      }
                    XtDispatchEvent( &event_return ) ;
		  }
#endif /* dddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddd */
		break ;
	      }
	  }
      }
#endif /* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx */
    return( SUCCESS ) ;
}

#ifdef USE_X /* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx */
/*
 * FRONT_GKS_CALLBACK
 *
 * When the menu item under "Windows" menu being pushed, callback happen to
 * front the appropriate gks window
 *
 */

static XtCallbackProc front_gks_callback( widget, data, callback_data )
Widget widget ;
XtPointer data ;
XtPointer *callback_data ;
{
    INT i = (INT) data ;
    WM_CURRENT *wmv_current = wm_static() ;
    Window window ;
    GKS_INT cur_wkid = MAIN_WKID ;
    GKS_INT new_wkid = wmv_current->gks_widget_array[ i ].ws_id ;
    XWindowAttributes WMAttributes ;
    
    /* update workstation id if necessary for MGKS */

    if (new_wkid != cur_wkid)
      {
        if (cur_wkid > 0)
	  {
            wm_gdawk( cur_wkid ) ; /* deactivate current workstation id */
	  }
        wmv_current->ws_id = new_wkid ;
        wm_gacwk( new_wkid ) ;     /* activate the new one */
      }
#ifdef PRIOR_GKS /* pppppppppppppppppppppppppppppppppppppppppppppppppppppppppp */
    window = XtWindow( wmv_current->gks_widget_array[ i ].widget ) ;
#elif DEC_GKS /* ppppppppppppppppppppppppppppppddddddddddddddddddddddddddddddd */
    window = XtWindow( XtParent( wmv_current->gks_widget_array[ i ].widget ) ) ;
#endif /* dddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddd */
    XRaiseWindow( wiv.current_Display, window ) ;
    XGetWindowAttributes( wiv.current_Display, window, &WMAttributes ) ;
    if (WMAttributes.map_state == IsViewable)
      {
        XSetInputFocus(
	    wiv.current_Display,
	    window,
	    RevertToNone,
	    CurrentTime ) ;
      }
    else
      {
        XMapWindow( wiv.current_Display, window ) ;
      }
}
#endif /* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx */

#ifdef PRIOR_GKS /* pppppppppppppppppppppppppppppppppppppppppppppppppppppppppp */
static XtEventHandler wmz_gks_handle_expose_event( w, client_data, event, continue_to_dispatch )
Widget w ;
XtPointer client_data ;
XEvent *event ;
Boolean *continue_to_dispatch ;
{
    WM_CURRENT *wmv_current = wm_static() ;

    Gescin  esc_in ;
    Gescout esc_out ;
    Gchar *str_data[1] ;
    INT i = (INT) client_data ;
    GKS_INT dummy_ws, dummy_class, dummy_dev ;
    GKS_REAL aspect_ratio ;
    GKS_REAL timeout = 0.0 ;
#ifdef DEBUG
    fprintf(stdout,"wmz_gks_handle_expose_event == #%d ( %d, %d, %d, %d )\n",
	    event->xexpose.count,
	    event->xexpose.x,
	    event->xexpose.y,
	    event->xexpose.width,
	    event->xexpose.height ) ;
#endif
    /* Prior GKS Escape Function -2602 == Process an event.
     * The Prior GKS Manual says "Allow Prior GKS to process only KeyPress,
     * ButtonPress, Expose, and ConfigureNotify", but also need MotionNotify
     * for rubberbanding.
     */

    if (event->type == Expose)
      {
        esc_in.esc_function = -2602  ;
        esc_in.esc_route    = G_SPECIFIC_WS ;
        esc_in.esc_ws       = ((GKS_INT) wmv_current->gks_widget_array[ i ].ws_id) ;
        esc_in.n_ints       = 0 ;
        esc_in.n_reals      = 0 ;
        esc_in.n_strings    = 1 ;
        str_data[0]         = ((Gchar *) event) ;
        esc_in.string_array = str_data ;
        esc_out             = gescape( NULL, &esc_in )  ;
        wm_gwait( timeout, &dummy_ws, &dummy_class, &dummy_dev )  ;
      }
}

static XtEventHandler wmz_gks_handle_resize_event( w, client_data, event, continue_to_dispatch )
Widget w ;
XtPointer client_data ;
XEvent *event ;
Boolean *continue_to_dispatch ;
{
    WM_CURRENT *wmv_current = wm_static() ;

    Gescin esc_in ;
    Gescout esc_out ;
    Gchar *str_data[ 1 ] ;
    INT i = (INT) client_data ;
    INT width  = 0 ;
    INT height = 0 ;
    GKS_INT dummy = 0 ;  /* placeholder for value returned by reference */
    GKS_REAL aspect_ratio ;
    GKS_REAL timeout = 0.0 ;
    Arg arglist[ 2 ] ;

    XEvent event_return ;
    Boolean found = FALSE ;
#ifdef DEBUG
    if (event->type == ConfigureNotify)
      {
	fprintf( stdout,"wmz_gks_handle_resize_event == ( %d, %d, %d, %d )\n",
		event->xconfigure.x,
		event->xconfigure.y,
		event->xconfigure.width,
		event->xconfigure.height ) ;
      }
#endif
    if (event->type == ConfigureNotify)
      {
	width = event->xconfigure.width ;
	height = event->xconfigure.height ;
	aspect_ratio = wmv_current->gks_widget_array[ i ].aspect_ratio ;

	/* maintain the aspect ratio */

	if (aspect_ratio < 1.0)
	  {
	    if (height >= width)
	      height = aspect_ratio * width  ;
	    else
	      width = height / aspect_ratio  ;
	  }
	else if (aspect_ratio > 1.0)
	  {
	    if (width >= height)
	      width = height / aspect_ratio  ;
	    else
	      height = aspect_ratio * width  ;
	  }
	else /* aspect_ratio == 0 */
	  {
	    width = height = MIN(width, height)  ;
	  }

	/* adjust resize to our widget size */

	if (width != event->xconfigure.width ||
	    height != event->xconfigure.height)
	  {
#ifdef MOTIF /* mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm */
	    XtSetArg( arglist[0], XmNwidth,  width  ) ;
	    XtSetArg( arglist[1], XmNheight, height ) ;
#elif DECWINDOWS /* mmmmmmmmmmmmmmmmmmmmmmmmmmmmmwwwwwwwwwwwwwwwwwwwwwwwwwwwww */
	    XtSetArg( arglist[0], DwtNwidth, width  ) ;
	    XtSetArg( arglist[1], DwtNheight, height) ;
#endif /* wwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwww */
	    XtSetValues( w, arglist, 2 )  ; /* changes w */

	    /* override configure event width and height */

	    event->xconfigure.width = width ;
	    event->xconfigure.height = height ;
	  }

	/* Prior GKS Escape Function -2602 == Process an event.
	 * The Prior GKS Manual says "Allow Prior GKS to process only KeyPress,
	 * ButtonPress, Expose, and ConfigureNotify", but also need MotionNotify
	 * for rubberbanding.
	 */

        esc_in.esc_function = -2602  ;
        esc_in.esc_route    = G_SPECIFIC_WS ;
        esc_in.esc_ws       = ((GKS_INT) wmv_current->gks_widget_array[ i ].ws_id) ;
        esc_in.n_ints       = 0 ;
        esc_in.n_reals      = 0 ;
        esc_in.n_strings    = 1 ;
        str_data[0]         = ((Gchar *) event) ;
        esc_in.string_array = str_data ;
        esc_out             = gescape( NULL, &esc_in )  ;
        wm_gwait( timeout, &dummy, &dummy, &dummy )  ;

	/* if the width and height are different we definitely got a
	 * resize, but if the width and height are not different we
	 * got a change in x,y and not in width, height, (i.e. a move).
	 */

	if (wmv_current->gks_widget_array[ i ].width  != width ||
	    wmv_current->gks_widget_array[ i ].height != height )
	  {
	    /* we are guaranteed that a configure notify will be followed by
	     * an expose event. and since we have decided to synthesize an
	     * expose event after every configure notify, let us just eat a
	     * series of expose events after every configure notify event.
	     */

	    found = TRUE ;
	    while( found )
	      {
		found =
		  XCheckTypedWindowEvent(
		      wiv.current_Display,
		      XtWindow( w ),
		      Expose,
		      &event_return ) ;
#ifdef DEBUG
		if (found)
		  {
		    fprintf( stdout, "XCheckTypedEvent( %d, %d, %d, %d )\n",
			    event_return.xexpose.x,
			    event_return.xexpose.y,
			    event_return.xexpose.width,
			    event_return.xexpose.height ) ;
		  }
#endif
	      }

	    /* this solves the problem of there being no expose after a resize,
	     * but the correct solution is to have bit_gravity be ForgetGravity,
	     * that way an expose will be generated after every resize.
	     */

	    /* construct an Expose event */

	    event->type = Expose ;
	    event->xexpose.x = 0 ;
	    event->xexpose.y = 0 ;
	    event->xexpose.width = width ;
	    event->xexpose.height = height ;
	    event->xexpose.count = 0 ;

	    XtDispatchEvent( event ) ;

	    wmv_current->gks_widget_array[ i ].width  = width ;
	    wmv_current->gks_widget_array[ i ].height = height ;
	  }
      }
#ifdef DEBUG
    else
      {
	switch( event->type )
	  {
  	    case CirculateNotify :
	      fprintf(stdout,"CirculateNotify\n") ;
	    break ;
	    case DestroyNotify   :
	      fprintf(stdout,"DestroyNotify\n") ;
	    break ;
	    case GravityNotify   :
	      fprintf(stdout,"GravityNotify\n") ;
	    break ;
	    case MapNotify       :
	      fprintf(stdout,"MapNotify\n") ;
	    break ;
	    case ReparentNotify  :
	      fprintf(stdout,"ReparentNotify\n") ;
	    break ;
	    case UnmapNotify     :
	      fprintf(stdout,"UnmapNotify\n") ;
	    break ;
	  }
      }
#endif
}

/*ARGSUSED*/
/*static*/ XtEventHandler wmz_gks_handle_input_event( w, client_data, event, continue_to_dispatch )
Widget w ;
XtPointer client_data ;
XEvent *event ;
Boolean *continue_to_dispatch ;
{
    WM_CURRENT *wmv_current = wm_static() ;
    Gescin esc_in ;
    Gescout esc_out ;
    Gchar *str_data[1] ;
    INT i = (INT) client_data ;
    INT width = 0 ;
    INT height = 0 ;
    GKS_INT dummy = 0 ;  /* placeholder for value returned by reference */
    GKS_REAL aspect_ratio = 0.0 ;
    GKS_REAL timeout = 0.0 ;
    Arg arglist[2] ;
#ifdef DEBUG
    if (event->type != MotionNotify)
      {
	fprintf( stdout, "wmz_gks_handle_input_event\n" ) ;
      }
#endif
    if (event->type == ButtonRelease)
      {
        wmz_gks_button_release( w, (INT) client_data, event )  ;
      }
    else
      {
	/* Prior GKS Escape Function -2602 == Process an event
	 * The Prior GKS Manual says "Allow Prior GKS to process only KeyPress,
	 * ButtonPress, Expose, and ConfigureNotify", but also
	 * need MotionNotify for rubberbanding.
	 */

	if ((event->type == KeyPress) ||
	    (event->type == ButtonPress) ||
	    (event->type == MotionNotify))
	  {
	    esc_in.esc_function = -2602  ;
	    esc_in.esc_route    = G_SPECIFIC_WS ;
	    esc_in.esc_ws       = ((GKS_INT) wmv_current->gks_widget_array[ i ].ws_id) ;
	    esc_in.n_ints       = 0 ;
	    esc_in.n_reals      = 0 ;
	    esc_in.n_strings    = 1 ;
	    str_data[0]         = ((Gchar *) event) ;
	    esc_in.string_array = str_data ;
	    esc_out             = gescape( NULL, &esc_in )  ;
	    wm_gwait( timeout, &dummy, &dummy, &dummy )  ;
	  }
      }
}
#endif /* pppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppp */

publicdef INT wmz_init_color()
{
    register int i = 0  ;

    /* Set up the color representation for VAXSTATION workstation.
     * The GPX uses color 1 as white and color 0 as black.
     */

    wm_gscr( MAIN_WKID,  0, (GKS_REAL)0.0, (GKS_REAL)0.0, (GKS_REAL)0.0 )  ; /* black       */
    wm_gscr( MAIN_WKID,  1, (GKS_REAL)1.0, (GKS_REAL)1.0, (GKS_REAL)1.0 )  ; /* white       */

    if ( DefaultDepth( wiv.current_Display, DefaultScreen( wiv.current_Display ) ) > 1 )
      {
        wm_gscr( MAIN_WKID,  2, (GKS_REAL)1.0,  (GKS_REAL)0.0,  (GKS_REAL)0.0  )  ; /* red         */
        wm_gscr( MAIN_WKID,  3, (GKS_REAL)0.0,  (GKS_REAL)1.0,  (GKS_REAL)0.0  )  ; /* green       */
        wm_gscr( MAIN_WKID,  4, (GKS_REAL)0.0,  (GKS_REAL)0.0,  (GKS_REAL)1.0  )  ; /* blue        */
        wm_gscr( MAIN_WKID,  5, (GKS_REAL)0.0,  (GKS_REAL)1.0,  (GKS_REAL)1.0  )  ; /* cyan        */
        wm_gscr( MAIN_WKID,  6, (GKS_REAL)1.0,  (GKS_REAL)0.0,  (GKS_REAL)1.0  )  ; /* magenta     */
        wm_gscr( MAIN_WKID,  7, (GKS_REAL)1.0,  (GKS_REAL)1.0,  (GKS_REAL)0.0  )  ; /* yellow      */
        wm_gscr( MAIN_WKID,  8, (GKS_REAL)1.0,  (GKS_REAL)0.54, (GKS_REAL)0.0  )  ; /* orange      */
        wm_gscr( MAIN_WKID,  9, (GKS_REAL)0.75, (GKS_REAL)0.75, (GKS_REAL)0.68 )  ; /* light_grey  */
        wm_gscr( MAIN_WKID, 10, (GKS_REAL)0.75, (GKS_REAL)0.85, (GKS_REAL)0.85 )  ; /* light_blue  */
        wm_gscr( MAIN_WKID, 11, (GKS_REAL)0.25, (GKS_REAL)0.45, (GKS_REAL)0.55 )  ; /* blue_grey   */
        wm_gscr( MAIN_WKID, 12, (GKS_REAL)1.0,  (GKS_REAL)0.0,  (GKS_REAL)0.35 )  ; /* red_magenta */
        wm_gscr( MAIN_WKID, 13, (GKS_REAL)0.5,  (GKS_REAL)0.5,  (GKS_REAL)0.5  )  ; /* grey        */
        wm_gscr( MAIN_WKID, 14, (GKS_REAL)0.65, (GKS_REAL)0.16, (GKS_REAL)0.16 )  ; /* brown       */
        wm_gscr( MAIN_WKID, 15, (GKS_REAL)0.80, (GKS_REAL)0.20, (GKS_REAL)0.60 )  ; /* violet      */
      }
#ifdef PRIOR_GKS /* pppppppppppppppppppppppppppppppppppppppppppppppppppppppppp */
    else
      {
        for (i = 2 ; i <= 15 ; i++)
          {
            wm_gscr( MAIN_WKID, i, (GKS_REAL)0.0, (GKS_REAL)0.0, (GKS_REAL)0.0 )  ;
          }
      }
#endif /* pppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppp */
    return( SUCCESS ) ;
}
