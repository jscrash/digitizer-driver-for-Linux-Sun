/* DEC/CMS REPLACEMENT HISTORY, Element WM_GPX_INPUT.C*/
/*  23   11-MAR-1992 18:33:45 SANTIAGO "(SPR -1) more fixes for case where #ifdef USE_X doesn't exist"*/
/* *22   28-FEB-1992 14:04:06 SANTIAGO "(SPR -1) fixed multiple refresh problem under Prior GKS"*/
/* *21   16-AUG-1991 11:16:44 JTM "(SPR 0) Cast args to wm_gslwsc() to DOUBLE."*/
/* *20    5-JUL-1991 11:24:09 KEE "(SPR 7598) Fix Electronic Drafting Line Editor PuckKey handling error, case 1893"*/
/* *19    3-MAY-1991 16:14:58 KEE "(SPR -1) MOTIF/PRIORGKS, MOTIF/DECWGKS, DECW/PRIORGKS, DECW/DECGKS version"*/
/* *18   23-OCT-1990 13:59:23 KEE "(SPR -1) Motif and DecWindow Merge"*/
/* *17   30-AUG-1990 12:54:53 GILLESPIE "(SPR 1) Include correct include files"*/
/* *16   22-AUG-1990 16:42:18 KEE "(SPR -1) fixing compile error"*/
/* *15   21-MAY-1990 08:41:43 SCD "(SPR 1) Format & comment. Change usage of xf_get_choice to remap buttons here."*/
/* *14   14-MAY-1990 14:02:16 JULIAN "(SPR 0) revert changes for aspect ratio lock/unlock"*/
/* *13    3-MAY-1990 22:07:47 KEE "(SPR -1) multi window and aspect menu changes"*/
/* *12    2-MAR-1990 11:08:20 JULIAN "(SPR 6012) change if USE_X to ifdef USE_X (Kee)"*/
/* *11   28-FEB-1990 15:46:35 JULIAN "(SPR -1) Add element to WM_CURRENT struct, change MAIN_WKID & GKS_WISS_WKID def (Kee)"*/
/* *10    6-FEB-1990 15:40:13 WALTERS "(SPR 0) Fix ts_tcp_copy"*/
/* *9     5-FEB-1990 23:50:13 JULIAN "(SPR -1) X version of WI Module (Kee)"*/
/* *8    13-NOV-1989 13:30:48 JULIAN "(SPR 1) Integrate external tablet functions into finder."*/
/* *7    19-SEP-1989 13:39:59 GILLESPIE "(SPR 100) GULF MODS"*/
/* *6    14-SEP-1989 16:21:52 GORDON "(SPR 100) Gulf/Sun/Unix mods"*/
/* *5    10-AUG-1989 08:04:36 GORDON "(SPR 105) added timeout init line for GULF"*/
/* *4    18-JUL-1989 13:34:47 GORDON "(SPR 100) Merge in UNIX changes"*/
/* *3    17-JUL-1989 10:03:41 GORDON "(SPR 0) default menu fix, concept of continue added"*/
/* *2     1-JUL-1989 16:43:25 JULIAN "(SPR 5) newer, stronger, faster, and it works on DECWINDOWS!"*/
/* *1    19-JUN-1989 13:40:58 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element WM_GPX_INPUT.C*/
/*                                                                                  */
/**                                                                                 */
/** This is a module of input routines written for the GPX.  They                   */
/** handle various interactions with the Mouse and the Tablet (external).           */
/**                                                                                 */
/** There are also Menu support routines that use GKS Choice input.                 */
/**                                                                                 */
/** Julian Carlisle  Jan 4, 1989                                                    */
/**                                                                                 */
/*                                                                                  */
#include "esi_wm_int.h"

#ifndef ESI_WI_INT_X
#include "esi_wi_int.x"
#endif

#ifndef ESI_HF_H
#include "esi_hf.h"
#endif

#include "esi_gks.h"
#include "esi_math.h"
#include "esi_mg.h"
#include "esi_tg_mat.h"
#include  "esi_am.h"

#ifndef ESI_WI_H
#include  "esi_wi.h"
#endif

#include "esi_tc.h"
#include "esi_ts.h"
#include  "esi_tb_defs.h"
#include "esi_lu.h"
#include  "pd_dialogs.rh"
publicdef INT wmz_getxy ();
publicdef INT gpx_wmtbandrectangle ();
publicdef INT gpx_wmtbandvector ();
publicdef INT wm_get_choice ();
publicdef INT pd_reg_cdlg ();
publicdef INT wm_set_mouse_menu ();
publicdef INT wm_map_choice_button ();
publicdef BOOL wm_is_menu_button();
static INT pdz_tablet_xy ();
static INT zz_msg ();

#ifdef SUN_GKS_OR_PRIOR_GKS
#ifndef SUN_AGKSTYPE_H
#include "sun_agkstype.h"
#endif
#endif

#define MAX_ITEM_LEN  32
#define MAX_MENU_ITEMS 16

/* define locator echo prompt types                                                 */

#define  LOC_ECHO_RECT   5
#define  LOC_ECHO_VECTOR 4
#define  LOC_ECHO_CROSS  3
#define  LOC_ECHO_PLUS   1
#define  LOC_ECHO_DIGIT  6

/* For sizing choice menus. These are Vaxstation dependant values of course.        */
#define CHOICE_CHAR_HT   0.0052475
#define CHOICE_ASPECT    0.8249258
INT rootx, rooty;

/* This is referenced globally inside this source file                              */
static GKS_INT lastbutton = 1;
static BOOL is_menu_button;

#if UNIX == 0				/* 910319:sjs: changed from sun */
static CHOICE_DATREC choice_rec = 
    {
    0
    };

static CHOICE_DATREC temp_choice_rec = 
    {
    0
    };

#endif

/* This is the currently active menu tcp and its button map                         */
static CHAR *default_menu[] = 
    {
    "Zoom", "Pan", "Cancel", "Done", "Reset", "Continue", NULL
    };

static INT default_button_map[] = 
    {
    WM_BUTTON_ZOOMIN, WM_BUTTON_PAN, WM_BUTTON_CANCEL, WM_BUTTON_DONE, WM_BUTTON_RESET,
    WM_BUTTON_CONTINUE
    };

static TCP cur_menu = default_menu;
static INT *cur_button_map = default_button_map;

static TCP last_menu = (TCP)0;
/*                                                                                  */
/*    Define the active transition for the mouse button.                            */
/*                                                                                  */
#define BTN_RELEASE_TRIG 5  /* Trigger on mouse btn release _- */
#define BTN_PRESS_TRIG   4  /* Trigger on mouse btn press   -_ */
#define BTN_TRIG_MODE   BTN_PRESS_TRIG
#define BTN_INPUT 1
#define XY_INPUT  2
INT xf_gbl_xbtn2 = 0;

extern XtEventHandler wmz_gks_handle_input_event() ;

/********************************************************************               */
/**      Copyright 1988, Exploration Systems Inc. San Francisco                     */
/**                                                                                 */
/** PROGRAM:      WMZ_GETXY()                                                       */
/**                                                                                 */
/** DESCRIPTION:                                                                    */
/**   Handles the protocol required to get an X,Y & button input                    */
/**   via the workstation mouse. The mouse is read by sampling a                    */
/**   GKS choice device that is associated with it.  Each of the                    */
/**   the three buttons different function that effects the                         */
/**   button code returned.                                                         */
/**   LEFT        - Point.  Always returns button 1.                                */
/**   MIDDLE      - Sitcky key. returns the previous button code.                   */
/**   RIGHT       - Display a menu of options and return selection index.           */
/**   It is up to the application to set up the desired menu with a call            */
/**   to WM_SET_MOUSE_MENU before calling WMTLOCATE, using either a customized      */
/**   menu or the default menu, defined above.                                      */
/**                                                                                 */
/** ARGUMENTS:                                                                      */
/**   Out:    INT tnr         The address of a variable to recieve the              */
/**                           transformation associated with the input.             */
/**           GKS_REAL x,y    The coordinates of the input location.                */
/**           INT b           The address of an int for the button code.            */
/**                                                                                 */
/**   In:                                                                           */
/**           INT pet         Prompt echo type. (if your not sure, use 1)           */
/**                                                                                 */
/** RETURNS:                                                                        */
/**   SUCCESS or the GKS status if it was unhappy.                                  */
/**                                                                                 */
/**   Julian Carlisle      Oct 1, 1988.                                             */
/*********************************************************************              */
publicdef INT wmz_getxy (tnr, x, y, b, pet)
GKS_INT *tnr;
GKS_REAL *x, *y;
GKS_INT *b;
GKS_INT pet;
    {
    GKS_INT status;
    GKS_INT wks_id = MAIN_WKID,     /* Active workstation id.                       */
    xform = 0,                      /* Tnr id retruned from grqlc                   */
    record_size = 0,                /* gks input set up                             */
    gks_stat = 0,                   /* returned gks  error state                    */
    echo_type = 0,                  /* Set up for gks device                        */
    echo_flag = 0,                  /* Set up for gks device                        */
    err_stat = 0,                   /* Set up for gks device                        */
    initial_status = 0,             /* Set up for gks device                        */
    initial_choice = 0,             /* Set up for gks device                        */
    input_mode = 0,                 /* Set up for gks device                        */
    input_status = 0,               /* Set up for gks device                        */
    record_buflen = 0;              /* sizedata record buf (bytes)                  */
    
    GKS_REAL echo_area[4];
    GKS_REAL world_x = 0.0, world_y = 0.0;
    GKS_INT unity = 0, maintnr;
    GKS_REAL wsxmin, wsxmax, wsymin, wsymax;
    GKS_INT evclass, evdev, evwkid, puckkey = 0;

#ifdef USE_X
    INT i;
    XEvent event;
    Widget widget;
    WM_CURRENT *wmv_current = wm_static ();
#ifdef PRIOR_GKS  /* ========== BEGIN PRIOR GKS ========== */
    Arg arglist[2];
    Dimension widget_width;
    Dimension widget_height;
    GKS_REAL vpxll, vpxur, vpyll, vpyur;
    GKS_REAL wdxll, wdxur, wdyll, wdyur;
    GKS_REAL map_width, map_height;
    XtAppContext AppContext = XtWidgetToApplicationContext (wiv.toplevel);
#endif  /* ========== END PRIOR GKS ========== */
#endif

#ifdef SUN_GKS_OR_PRIOR_GKS   /* ========== BEGIN SUN OR PRIOR GKS ========== */
    GKS_REAL timeout = 0.0;
    Glocrec locate_datrec;
    GKS_INT locate_dev1 = 1;        /* locator device button 1                      */
    GKS_INT locate_dev2 = 2;        /* locator device button 2                      */
    GKS_INT locate_dev3 = 3;        /* locator device button 3                      */
    GKS_INT ev_wks_id;
    GKS_INT ev_icl;
    GKS_INT ev_idnr;
    GKS_INT color;
#else  /* ========== END SUN OR PRIOR GKS = BEGIN DEC GKS ========== */
    /*                                                                              */
    /*  Choice Device prompt is not used so set up null parameters                  */
    /*  for the choice input data record prompt parmater section.                   */
    /*                                                                              */
    static GKS_INT sizes[4] = 
        {
        0, 0, 0, 0
        };
    static CHAR *strings[4] = 
        {
        "    ", "    ", "    ", "    "
        };
    /*                                                                              */
    /*  We are using a mouse with three buttons                                     */
    /*  so set up for 3 choice button return codes.                                 */
    /*                                                                              */
    CHOICE_DATREC choice_datrec;
    /*                                                                              */
    /*  Choice trigger on button RELEASE:  --__                                     */
    /*  Change from 5 to 4 to get PRESS triggering:   __--                          */
    /*                                                                              */
    GKS_INT choice_dev = BTN_PRESS_TRIG;    /* press trig                           */
    LOCATE_DATREC locate_datrec;
    /* Basic locator uses a null GKS data record for                                */
    GKS_INT locate_dev = 1;
#endif /* ========== END DEC GKS ========== */
    
    /* if not GPX then we can't necessarily do this, abort.                         */
    is_menu_button = FALSE;

    if (ug_if_gpx () == FALSE)
        return FAIL;

#ifdef USE_X
    /* -------------------------------------                                        */
    /* Signal that we are starting GKS input                                        */
    /* to the wmz_gks_button_handler()                                              */
    /* -------------------------------------                                        */
    wmz_set_gks_input_active (TRUE);
#endif
    rootx = 0;
    rooty = 0;
    wm_gqcntn (&status, &maintnr);
    
    /* ------------------------------------                                         */
    /* Inquire the current workstation size                                         */
    /* so we can later set the echo area to                                         */
    /* the entire workstation window                                                */
    /* ------------------------------------                                         */
    wm_inq_wkvp_max (&wsxmin, &wsxmax, &wsymin, &wsymax);
    
    /* --------------------------------------------------------------               */
    /* This loop should only repeat if "Continue" is picked from menu               */
    /* --------------------------------------------------------------               */

#ifdef USE_X
    wmv_current->wm_gks_button = 0;
    
    /* -----------------------------------                                          */
    /* Lookup the output widget associated                                          */
    /* with the current workstation id                                              */
    /* -----------------------------------                                          */
    for (i = 0; i < wmv_current->max_ws; i++)
        {
        if (wmv_current->gks_widget_array[i].ws_id == MAIN_WKID)
            break;
        }
#ifdef PRIOR_GKS /* ========== BEGIN PRIOR GKS ========== */
    
    /*--------------------------------------------------------------------------    */
    /*                                                                              */
    /*        To find world_x(x2?<Gwc>) and world_y(y2?<Gwc>)                       */
    /*                                                                              */
    /*        To find y2?<Gwc>, formula:                                            */
    /*             y1<pixel>-y2<pixel>   y2?<Gwc>-y1<Gwc>                           */
    /*             ------------------- = ----------------                           */
    /*              y1<pixel>             h<Gwc>                                    */
    /*                                                                              */
    /*                           h<Gwc> * (y1<pixel> - y2<pixel>)                   */
    /*       =>  y2?<Gwc> = (--------------------------------) + y1<Gwc>            */
    /*                                      y1<pixel>                               */
    /*                                                                              */
    /*            To find x2?<Gwc>, formula:                                        */
    /*                 x2<pixel>          x2?<Gwc>-x1<Gwc>                          */
    /*              ------------------- =  ----------------                         */
    /*                  x1<pixel>               w<Gwc>                              */
    /*                                                                              */
    /*                          w<Gwc> * x2<pixel>)                                 */
    /*           => x2?<Gwc> = (-------------------) + x1<Gwc>                      */
    /*                               x1<pixel>                                      */
    /*                                                                              */
    /*           where y2<pixel> = event.xmotion.y                                  */
    /*                 y1<pixel> = widget_height                                    */
    /*         x2<pixel> = event.xmotion.x                                          */
    /*         x1<pixel> = widget_width                                             */
    /*                                                                              */
    /*         y<Gwc> = map_height                                                  */
    /*         w<Gwc> = map_width                                                   */
    /*         y1<Gwc> = map_miny (wdyll)                                           */
    /*         x1<Gwc> = map_minx (wdxll)                                           */
    /*--------------------------------------------------------------------------    */
    
#ifdef MOTIF  /* ---------- BEGIN MOTIF BLOCK ----------- */
    XtSetArg (arglist[0], XmNwidth, &widget_width);
    XtSetArg (arglist[1], XmNheight, &widget_height);
#else  /* ---------- END MOTIF BLOCK - BEGIN DECW BLOCK ---------- */
    XtSetArg (arglist[0], DwtNwidth, &widget_width);
    XtSetArg (arglist[1], DwtNheight, &widget_height);
#endif  /* ---------- END DECW BLOCK ---------- */
    XtGetValues (wmv_current->gks_widget_array[i].widget, arglist, 2);
    
    wmtinq (wm_wkid_to_tnr (wks_id), &vpxll, &vpxur, &vpyll, &vpyur, &wdxll, &wdxur,
            &wdyll, &wdyur);
    map_width = wdxur - wdxll;
    map_height = wdyur - wdyll;
    
    do
        {
        if (pet == LOC_ECHO_PLUS)
            {
            /* track the first pointer motion event inside gks window               */
            FOREVER
                {
                XtAppNextEvent (AppContext, &event);
                XtDispatchEvent (&event);
                if ((event.type == MotionNotify &&
                     event.xmotion.window ==
                     XtWindow (wmv_current->gks_widget_array[i].widget)) ||
                    (event.type == ButtonPress &&
                     event.xbutton.window ==
                     XtWindow (wmv_current->gks_widget_array[i].widget)))
                    break;
                }
            /* calculation of world_x and world_y using the above formula           */
            if (event.type == MotionNotify)
                {
                world_y = (map_height * (widget_height - event.xmotion.y)) /
                    widget_height + wdyll;
                world_x = (map_width * event.xmotion.x) / widget_width + wdxll;
                }
            else                    /* ButtonPress                                  */
                {
                world_y = (map_height * (widget_height - event.xbutton.y)) /
                    widget_height + wdyll;
                world_x = (map_width * event.xbutton.x) / widget_width + wdxll;
                }
            /* check if location out of graphics object range                       */
            if (world_y > wdyur)
                world_y = wdyur;
            if (world_y < wdyll)
                world_y = wdyll;
            if (world_x > wdxur)
                world_x = wdxur;
            if (world_x < wdxll)
                world_x = wdxll;
            
            gks_stat = wm_gslcm (wks_id, locate_dev1, GREQU, GECHO);
            
            echo_type = pet;
            echo_area[0] = wsxmin;
            echo_area[1] = wsxmax;
            echo_area[2] = wsymin;
            echo_area[3] = wsymax;
            if (XBlackPixel (wiv.current_Display,
			     XDefaultScreen (wiv.current_Display)) == 0) 
	        /* 1 for Sun and 0 for other */
	      { /* use white cursor for other */
		lu_text_to_index ("COLOR", &color, "WHITE");
	      }
            else
	      { /* use black cursor for sun */
		lu_text_to_index ("COLOR", &color, "BLACK");
	      }
            locate_datrec.pet1.data = (Gchar *)&color;
            gks_stat = wm_ginlc (wks_id, locate_dev1, world_x, world_y, xform,
                                 echo_type, echo_area, record_buflen, &locate_datrec);
            
            }
        gks_stat = wm_gslcm (wks_id, locate_dev1, GEVENT, GECHO);
        
        /* -------------------------------------------------                        */
        /* NOTE: There is a major BUG here if the above loop                        */
        /*       fails to locate the workstation                                    */
        /* -------------------------------------------------                        */
        
        while (wmv_current->wm_gks_button == 0)
            {
            XtAppNextEvent (AppContext, &event);
            if (event.type == ButtonPress)
                {
                widget = XtWindowToWidget (event.xbutton.display,
                                           event.xbutton.window);
                if (widget != wmv_current->gks_widget_array[i].widget)
                    {
                    rootx = 0;
                    rooty = 0;
                    XBell (wiv.current_Display, 22);
                    }
                else
                    {
                    rootx = event.xbutton.x_root;
                    rooty = event.xbutton.y_root;
                    XtDispatchEvent (&event);
                    /* the following line is needed becuase at the band             */
                    /* rectangle or vector stage, no button release happen          */
                    /* for Prior GKS                                                */
                    if (pet == LOC_ECHO_VECTOR || pet == LOC_ECHO_RECT)
                        {
                        wmv_current->wm_gks_button = event.xbutton.button;
                        }
                    }
                }
            else
                {
                XtDispatchEvent (&event);
                }
            }
        puckkey = wmv_current->wm_gks_button;
        
        /*                                                                          */
        /* *   read the current measure of the locator device.                      */
        /*                                                                          */
        if (puckkey != 0)
            gks_stat = wm_ggtlc (&xform, &world_x, &world_y);
        
        switch (puckkey)
            {
        case 1:
            *b = 1;
            break;
        case 2:
            wm_get_choice (world_x, world_y, xform, b);
	    is_menu_button = TRUE;
            break;
        case 3:
            *b = WM_BUTTON_DONE;
            break;
            }
        } while (*b == WM_BUTTON_CONTINUE); /* do at very top                       */
    
    /*                                                                              */
    /* *       Reset the locator devices after the sample.                          */
    /*                                                                              */
    gks_stat = wm_gslcm (wks_id, locate_dev1, GREQU, GECHO);
    gks_stat = wm_gflush (wks_id, GLOCAT, locate_dev1);
    
    if (status & 1)
        status = SUCCESS;
    
    /* Something strange about PRIOR GKS : after do band rectangle                  */
    /*   or band vector, the ButtonRelease Event is disabled.                       */
    /*   To add back the ButtonReleaseMask, we have to do remove first              */
    /*   followed by an add.  It doesn't work with just an add                      */

    XtRemoveEventHandler(
        wmv_current->gks_widget_array[i].widget,
        ButtonPressMask | KeyPressMask | ButtonReleaseMask | KeyReleaseMask | PointerMotionMask,
	TRUE,
	(XtEventHandler) wmz_gks_handle_input_event,
	(caddr_t) i ) ;

    XtAddEventHandler(
	wmv_current->gks_widget_array[i].widget,
	ButtonPressMask | KeyPressMask | ButtonReleaseMask | KeyReleaseMask | PointerMotionMask,
	TRUE,
	(XtEventHandler) wmz_gks_handle_input_event,
	(caddr_t) i ) ;

#else /* ========== END PRIOR GKS = BEGIN DEC GKS ========== */
    do
        {
        record_buflen = 4;
        gks_stat = wm_gqlcs (wks_id, locate_dev, GREALI, record_buflen, &err_stat,
                             &input_mode, &echo_flag, &xform, &world_x, &world_y,
                             &echo_type, echo_area, &record_size, &locate_datrec);
        
        gks_stat = wm_gslcm (wks_id, locate_dev, GREQU, GECHO);
        
        if (pet == LOC_ECHO_PLUS)
            {
            /* Set up for a normal cross hair echo prompt.                          */
            /*   Using the entire screen                                            */
            /* Also update state list for the locator device                        */
            echo_type = pet;
            echo_area[0] = wsxmin;
            echo_area[1] = wsxmax;
            echo_area[2] = wsymin;
            echo_area[3] = wsymax;
            record_buflen = 0;
            locate_datrec.fill_flag = 0;
            gks_stat = wm_ginlc (wks_id, locate_dev, world_x, world_y, unity,
                                 echo_type, echo_area, record_buflen, &locate_datrec);
            
            }
        /* ------------------------------                                           */
        /* Start sampling on  the locator                                           */
        /* ------------------------------                                           */
        gks_stat = wm_gslcm (wks_id, locate_dev, GSAMPL, GECHO);
        
        /* -------------------------------------------------                        */
        /* NOTE: There is a major BUG here if the above loop                        */
        /*       fails to locate the workstation                                    */
        /* -------------------------------------------------                        */
        while (wmv_current->wm_gks_button == 0)
            {
            XNextEvent (wiv.current_Display, &event);
            if (event.type == ButtonRelease)
                {
                widget = XtWindowToWidget (event.xbutton.display,
                                           event.xbutton.window);
                if (widget != wmv_current->gks_widget_array[i].out_widget)
                    {
                    rootx = 0;
                    rooty = 0;
                    XBell (wiv.current_Display, 22);
                    }
                else
                    {
                    rootx = event.xbutton.x_root;
                    rooty = event.xbutton.y_root;
                    XtDispatchEvent (&event);
                    }
                }
            else
                XtDispatchEvent (&event);
            }
        puckkey = wmv_current->wm_gks_button;
        
        /*                                                                          */
        /* *   read the current measure of the locator device.                      */
        /*                                                                          */
        if (puckkey != 0)
            gks_stat = wm_gsmlc (wks_id, locate_dev, &xform, &world_x, &world_y);
        switch (puckkey)
            {
        case 1:
            *b = 1;
            break;
        case 2:
            wm_get_choice (world_x, world_y, xform, b);
	    is_menu_button = TRUE;
            break;
        case 3:
            *b = WM_BUTTON_DONE;
            break;
            }
        } while (*b == WM_BUTTON_CONTINUE); /* do at very top                       */

    /*                                                                              */
    /**       Reset the choice & locator devices after the sample.                  */
    /*                                                                              */
    gks_stat = wm_gslcm (wks_id, locate_dev, GREQU, GNECHO);
    
    if (status & 1)
        status = SUCCESS;

#endif  /* ========== END DEC GKS ========== */

#else  /* ifndef USE_X - MAC */

#if UNIX							/* 910319:sjs: changed from sun */
    do
        {
        if (pet == 1 || pet == 2 || pet == 3)
            {
            gks_stat = wm_gslcm (wks_id, locate_dev1, GREQU, GECHO);
            
            echo_type = pet;
            echo_area[0] = wsxmin;
            echo_area[1] = wsxmax;
            echo_area[2] = wsymin;
            echo_area[3] = wsymax;
            lu_text_to_index ("COLOR", &color, "NORMAL");
            locate_datrec.pet1.data = (Gchar *)&color;
            gks_stat = wm_ginlc (wks_id, locate_dev1, world_x, world_y, xform,
                                 echo_type, echo_area, record_buflen, &locate_datrec);
            
            gks_stat = wm_gslcm (wks_id, locate_dev2, GREQU, GECHO);
            
            echo_type = pet;
            echo_area[0] = wsxmin;
            echo_area[1] = wsxmax;
            echo_area[2] = wsymin;
            echo_area[3] = wsymax;
            locate_datrec.pet1.data = (Gchar *)&color;
            gks_stat = wm_ginlc (wks_id, locate_dev2, world_x, world_y, xform,
                                 echo_type, echo_area, record_buflen, &locate_datrec);
            
            gks_stat = wm_gslcm (wks_id, locate_dev3, GREQU, GECHO);
            
            echo_type = pet;
            echo_area[0] = wsxmin;
            echo_area[1] = wsxmax;
            echo_area[2] = wsymin;
            echo_area[3] = wsymax;
            locate_datrec.pet1.data = (Gchar *)&color;
            gks_stat = wm_ginlc (wks_id, locate_dev3, world_x, world_y, xform,
                                 echo_type, echo_area, record_buflen, &locate_datrec);
            }
        gks_stat = wm_gslcm (wks_id, locate_dev1, GEVENT, GECHO);
        gks_stat = wm_gslcm (wks_id, locate_dev2, GEVENT, GECHO);
        gks_stat = wm_gslcm (wks_id, locate_dev3, GEVENT, GECHO);
        
        do
            {
            gks_stat = wm_gwait (timeout, &ev_wks_id, &ev_icl, &ev_idnr);
            if ((ev_wks_id == wks_id) && (ev_icl && GLOCAT) &&
                (ev_idnr == locate_dev1))
                {
                puckkey = locate_dev1;
                gks_stat = wm_ggtlc (&xform, &world_x, &world_y);
                }
            else if ((ev_wks_id == wks_id) && (ev_icl && GLOCAT) &&
                     (ev_idnr == locate_dev2))
                {
                puckkey = locate_dev2;
                gks_stat = wm_ggtlc (&xform, &world_x, &world_y);
                }
            else if ((ev_wks_id == wks_id) && (ev_icl && GLOCAT) &&
                     (ev_idnr == locate_dev3))
                {
                puckkey = locate_dev3;
                gks_stat = wm_ggtlc (&xform, &world_x, &world_y);
                }
            } while (puckkey == 0);
        
        switch (puckkey)
            {
        case 1:
            *b = 1;
            break;
        case 2:
            wm_get_choice (world_x, world_y, xform, b);
	    is_menu_button = TRUE;
            break;
        case 3:
            *b = WM_BUTTON_DONE;
            break;
            }
        } while (*b == WM_BUTTON_CONTINUE); /* do at very top                       */
    
    gks_stat = wm_gflush (wks_id, GLOCAT, locate_dev1);
    gks_stat = wm_gflush (wks_id, GLOCAT, locate_dev2);
    gks_stat = wm_gflush (wks_id, GLOCAT, locate_dev3);
    
    gks_stat = wm_gslcm (wks_id, locate_dev1, GREQU, GECHO);
    gks_stat = wm_gslcm (wks_id, locate_dev2, GREQU, GECHO);
    gks_stat = wm_gslcm (wks_id, locate_dev3, GREQU, GECHO);

#else /* ifndef UNIX - VMS */
    do
        {
        record_buflen = 4;
        gks_stat = wm_gqlcs (wks_id, locate_dev, GREALI, record_buflen, &err_stat,
                             &input_mode, &echo_flag, &xform, &world_x, &world_y,
                             &echo_type, echo_area, &record_size, &locate_datrec);
        
        gks_stat = wm_gslcm (wks_id, locate_dev, GREQU, GECHO);
        
        if (pet == LOC_ECHO_PLUS)
            {
            /* Set up for a normal cross hair echo prompt.                          */
            /*   Using the entire screen                                            */
            /* Also update state list for the locator device                        */
            echo_type = pet;
            echo_area[0] = wsxmin;
            echo_area[1] = wsxmax;
            echo_area[2] = wsymin;
            echo_area[3] = wsymax;
            record_buflen = 0;
            locate_datrec.fill_flag = 0;
            gks_stat = wm_ginlc (wks_id, locate_dev, world_x, world_y, unity,
                                 echo_type, echo_area, record_buflen, &locate_datrec);
            
            }
        /*                                                                          */
        /* *                                                                        */
        /* *       Set up the CHOICE device so that it operates in sample mode      */
        /* *       without issuing any visible prompt/echo strings on the screen.   */
        /* *                                                                        */
        /* *       We are just using the choice device as a trigger & button delivery   */
        /* *       mechanism, once it has triggered we will sample the locator device   */
        /* *       for the x,y associated with the trigger.                         */
        /* *                                                                        */
        /*                                                                          */
        record_buflen = 4;
        gks_stat = wm_gqchs (wks_id, choice_dev, record_buflen, &err_stat, &input_mode,
                             &echo_flag, &initial_status, &initial_choice, &echo_type,
                             echo_area, &record_size, &choice_rec);
        
        /* new state list values for the choice input data record.                  */
        record_buflen = 12;         /* Bytes. Important!.                           */
        choice_rec.numstr = 3;      /* three button mouse.                          */
        choice_rec.sizes = sizes;   /* Not real data.                               */
        choice_rec.strings = strings;   /* Not real data.                           */
        initial_choice = 0;         /* Default to choice 1                          */
        initial_status = GCNONE;    /* Error if nochoice                            */
        echo_type = 3;              /* Echo strings                                 */
        
        /* Start sampling the mouse buttons                                         */
        gks_stat = wm_gschm (wks_id, choice_dev, GREQU, GNECHO);
        
        gks_stat = wm_ginch (wks_id, choice_dev, initial_status, initial_choice,
                             echo_type, echo_area, record_buflen, &choice_rec);
        
        /* Start sampling the mouse buttons                                         */
        gks_stat = wm_gschm (wks_id, choice_dev, GSAMPL, GNECHO);
        
        /* ------------------------------                                           */
        /* Start sampling on  the locator                                           */
        /* ------------------------------                                           */
        gks_stat = wm_gslcm (wks_id, locate_dev, GSAMPL, GECHO);
        
        do
            {
            wm_gsmch (wks_id, choice_dev, &status, &puckkey);
            } while (puckkey == 0 || status == (GKS_INT)GCNONE);
        
        gks_stat = wm_gschm (wks_id, choice_dev, GREQU, GNECHO);
        
        /*                                                                          */
        /* *   read the current measure of the locator device.                      */
        /*                                                                          */
        if (puckkey != 0)
            gks_stat = wm_gsmlc (wks_id, locate_dev, &xform, &world_x, &world_y);
        switch (puckkey)
            {
        case 1:
            *b = 1;
            break;
        case 2:
            wm_get_choice (world_x, world_y, xform, b);
	    is_menu_button = TRUE;
            break;
        case 3:
            *b = WM_BUTTON_DONE;
            break;
            }
        } while (*b == WM_BUTTON_CONTINUE); /* do at very top                       */
    
    /*                                                                              */
    /**       Reset the choice & locator devices after the sample.                  */
    /*                                                                              */
    gks_stat = wm_gslcm (wks_id, locate_dev, GREQU, GNECHO);
    
    if (status & 1)
        status = SUCCESS;

#endif  /* MAC - VMS */
#endif  /* MAC */
    lastbutton = *b;
    *x = world_x;
    *y = world_y;
    *tnr = xform;

#ifdef USE_X
    /* --------------------------------------                                       */
    /* Signal that we are done with GKS input                                       */
    /* --------------------------------------                                       */
    wmz_set_gks_input_active (FALSE);
#endif
    return status;
    }

/********************************************************************               */
/**      Copyright 1988, Exploration Systems Inc. San Francisco                     */
/**                                                                                 */
/** PROGRAM:    VaxStation/VAXGKS version of wmt_bandrectangle()                    */
/**                                                                                 */
/** DESCRIPTION:                                                                    */
/**   This program implements a window manager rectangular region input.            */
/**                                                                                 */
/**   This version of wmt_bandrectanlge uses the workstation mouse as an            */
/**   input locator device.                                                         */
/**                                                                                 */
/** ARGUMENTS:                                                                      */
/**   In: GKS_REAL    wx1,wy1     - Starting location of rect. region.              */
/**   Out:                                                                          */
/**       GKS_INT     *istat      - Pointer to a long for returned status.          */
/**       GKS_REAL    *wx2,*wy2   - Pointers to GKS_REAL for returned coords.       */
/**       GKS_INT     *button     - Pointer to a long for returned button.          */
/**                                                                                 */
/** RETURNS:                                                                        */
/**   SUCCESS always.  For status of input check istat arg. on return.              */
/**                                                                                 */
/**   Julian Carlisle      Oct 1, 1988.                                             */
/*********************************************************************              */
publicdef INT gpx_wmtbandrectangle (istat, wx1, wy1, wx2, wy2, button)
GKS_INT *istat;                     /* Out: Status from GKS                         */
GKS_REAL wx1, wy1;
GKS_REAL *wx2, *wy2;                /* Out: WCS                                     */
GKS_INT *button;                    /* Out: Button Code                             */
    {
    
    GKS_INT status = SUCCESS;       /* returned status from locate                  */
    
    GKS_REAL wcs_window[4],         /* saved transformation.                        */
        wcs_vport[4];
    
    GKS_REAL x, y, world_x, world_y;
    
    GKS_INT b, tnr, unity = 0, maintnr;
    
    GKS_INT wks_id = MAIN_WKID,     /* Active workstation id.                       */
    xform = 0,                      /* Tnr id retruned from grqlc                   */
    record_size = 0,                /* gks input set up                             */
    choice_value = 0,               /* returned value from choice                   */
    gks_stat = 0,                   /* returned gks  error state                    */
    echo_type = 0,                  /* Set up for gks device                        */
    echo_flag = 0,                  /* Set up for gks device                        */
    err_stat = 0,                   /* Set up for gks device                        */
    initial_status = 0,             /* Set up for gks device                        */
    initial_choice = 0,             /* Set up for gks device                        */
    input_mode = 0,                 /* Set up for gks device                        */
    input_status = 0,               /* Set up for gks device                        */
    record_buflen = 0;              /* sizedata record buf (bytes)                  */
    GKS_INT color;
    GKS_INT line_style;
    GKS_REAL save_line_width;
    GKS_INT save_line_style;
    GKS_INT save_line_color;
    /*                                                                              */
    /*    Default to whole screen for locator input sensitive zone.                 */
    /*                                                                              */
    GKS_REAL echo_area[4];
    
    /*                                                                              */
    /*    Choice Device prompt is not used so set up null parameters                */
    /*    for the choice input data record prompt parmater section.                 */
    /*                                                                              */
    
    GKS_INT pet;
    static INT asf_flags[13] = 
        {
        GINDIV, GINDIV, GINDIV, GINDIV, GINDIV, GINDIV, GINDIV, GINDIV, GINDIV, GINDIV,
        GINDIV, GINDIV, GINDIV
        };

    GKS_REAL wsxmin, wsxmax, wsymin, wsymax;
#ifdef SUN_GKS_OR_PRIOR_GKS   /* ========== BEGIN SUN OR PRIOR GKS ========== */
    static Glocrec locate_datrec;
    GKS_INT locate_dev1 = 1;
#else  /* ========== END SUN OR PRIOR GKS = BEGIN DEC GKS ========== */
    static LOCATE_DATREC locate_datrec;
    GKS_INT locate_dev = 1;         /* General locator device. Standard id          */
#endif /* ========== END DEC GKS ========== */

    
    /* Check for gpx host.                                                          */
    if (ug_if_gpx () == FALSE)
        return FAIL;
    
    if (wm_static ()->reg.key_type == WM_PD_TABLET)
        {
        return wmtlocate (istat, wx2, wy2, button);
        }
    /* Inquire & save the current xform number & it's components                    */
    gks_stat = wm_gqcntn (&status, &maintnr);
    wm_inq_wkvp_max (&wsxmin, &wsxmax, &wsymin, &wsymax);

    gks_stat = wm_gqnt (maintnr, &status, wcs_window, wcs_vport);
    
    /* Make the viewport priority HIGHER than the unity                             */
    gks_stat = wm_gsvpip (maintnr, unity, GHIGHR);

#ifndef SUN_GKS_OR_PRIOR_GKS   /* ========== BEGIN DEC GKS ========== */
    record_buflen = 4;
    gks_stat = wm_gqlcs (wks_id, locate_dev, GREALI, record_buflen, &err_stat,
                         &input_mode, &echo_flag, &xform, &world_x, &world_y,
                         &echo_type, echo_area, &record_size, &locate_datrec);
    
    gks_stat = wm_gslcm (wks_id, locate_dev, GREQU, GECHO);
    
    /* ------------------------------------                                         */
    /* All aspect source flags = individual                                         */
    /* NOTE: Setting the flags here will                                            */
    /*       screw up context for any appl                                          */
    /*       which is using bundles, as we                                          */
    /*       do not restore the state later                                         */
    /* ------------------------------------                                         */
    gks_stat = wm_gsasf (asf_flags);
#endif /* ========== END DEC GKS ========== */
    
    /* ---------------------------------------                                      */
    /* Remember polyline attributes, set them                                       */
    /* to NORMAL,SOLID,thin line, then restore                                      */
    /* before we return                                                             */
    /* ---------------------------------------                                      */
    wm_gqplci (&gks_stat, &save_line_color);
    wm_gqln (&gks_stat, &save_line_style);
    wm_gqlwsc (&gks_stat, &save_line_width);
    lu_text_to_index ("COLOR", &color, "NORMAL");
#ifdef USE_X    
#ifdef PRIOR_GKS /* ========== BEGIN PRIOR GKS ========== */
    if (XBlackPixel (wiv.current_Display,
		     XDefaultScreen (wiv.current_Display)) == 0) 
        /* 1 for Sun and 0 for other */
      { /* use white cursor for other */
	lu_text_to_index ("COLOR", &color, "WHITE");
      }
    else
      { /* use black cursor for sun */
	lu_text_to_index ("COLOR", &color, "BLACK");
      }
#endif /* ========== END PRIOR GKS ========== */
#endif
    lu_text_to_index ("LINE_STYLE", &line_style, "SOLID");
    wm_gsln (line_style);
    wm_gsplci (color);
    wm_gslwsc (1.0);

#ifdef SUN_GKS_OR_PRIOR_GKS   /* ========== BEGIN SUN OR PRIOR GKS ========== */
    echo_type = LOC_ECHO_RECT;      /* Rubber Banded rectangle echo prompt.         */
    
    echo_area[0] = wsxmin;
    echo_area[1] = wsxmax;
    echo_area[2] = wsymin;
    echo_area[3] = wsymax;
    
    locate_datrec.pet5.pfcf = GPLINE;   /* polyline rect echo                       */
    locate_datrec.pet5.acf = GSPEC; /* specified  attributes                        */
    
    locate_datrec.pet5.attr.ln.ln_type = GINDIV;    /* polyline type aspect source flag */
    locate_datrec.pet5.attr.ln.ln_width = GINDIV;   /* polyline width aspect source flag    */
    locate_datrec.pet5.attr.ln.ln_colour = GINDIV;  /* polyline color aspect source flag    */
    locate_datrec.pet5.attr.ln.line = 1;    /* polyline index                       */
    locate_datrec.pet5.attr.ln.lnbundl.type = line_style;
    /* polyline type                                                                */
    locate_datrec.pet5.attr.ln.lnbundl.width = 1.0; /* polyline width               */
    locate_datrec.pet5.attr.ln.lnbundl.colour = color;  /* polyline color           */
    
    gks_stat = wm_ginlc (wks_id, locate_dev1, wx1, wy1, maintnr, echo_type, echo_area,
                         record_buflen, &locate_datrec);

#else  /* ========== END SUN OR PRIOR GKS = BEGIN DEC GKS ========== */
    /* ------------------------------------                                         */
    /* Rubber Banded rectangle echo prompt.                                         */
    /* ------------------------------------                                         */
    echo_type = LOC_ECHO_RECT;
    record_buflen = 8;              /* full  input data record                      */
    locate_dev = 1;                 /* Logical device number                        */
    
    echo_area[0] = wsxmin;
    echo_area[1] = wsxmax;
    echo_area[2] = wsymin;
    echo_area[3] = wsymax;
    
    locate_datrec.fill_flag = GPLINE;   /* polyline or fill type rect echo          */
    locate_datrec.acf = GCURNT;         /* current or specified  attributes             */
    locate_datrec.type_asf = GINDIV;    /* polyline type aspect source flag         */
    locate_datrec.width_asf = GINDIV;   /* polyline width aspect source flag        */
    locate_datrec.color_asf = GINDIV;   /* polyline color aspect source flag        */
    locate_datrec.polyline_index = 1;   /* polyline index                           */
    locate_datrec.polyline_type = line_style;   /* polyline type                    */
    locate_datrec.width = 1.0;      /* polyline width                               */
    locate_datrec.color_index = color;  /* polyline color                           */
    
    gks_stat = wm_ginlc (wks_id, locate_dev, wx1, wy1, maintnr, echo_type, echo_area,
                         record_buflen, &locate_datrec);

#endif /* ========== END DEC GKS ========== */
    pet = LOC_ECHO_RECT;
    status = wmz_getxy (&xform, &world_x, &world_y, button, pet);
    /*                                                                              */
    /**       Reset the locator after the sample.                                   */
    /*                                                                              */
    
    *wx2 = world_x;
    *wy2 = world_y;
    
    gks_stat = wm_gsvpip (maintnr, unity, GLOWER);
    
    /* ---------------------------                                                  */
    /* Restore polyline attributes                                                  */
    /* ---------------------------                                                  */
    wm_gslwsc ((DOUBLE)save_line_width);
    wm_gsln (save_line_style);
    wm_gsplci (save_line_color);
    
    *istat = status;
    return SUCCESS;
    }


/********************************************************************               */
/**      Copyright 1988, Exploration Systems Inc. San Francisco                     */
/**                                                                                 */
/** PROGRAM:    VaxStation/VAXGKS version of wmt_bandvector()                       */
/**                                                                                 */
/** DESCRIPTION:                                                                    */
/**   This program implements a window manager rubber band vector input.            */
/**                                                                                 */
/**   This version of wmt_bandvector uses the workstation mouse as an               */
/**   input locator device.                                                         */
/**                                                                                 */
/** ARGUMENTS:                                                                      */
/**   In: GKS_REAL    wx1,wy1     - Starting location of rubber line region.        */
/**   Out:                                                                          */
/**       GKS_INT     *istat      - Pointer to a long for returned status.          */
/**       GKS_REAL    *wx2,*wy2   - Pointers to GKS_REAL for returned coords.       */
/**       GKS_INT     *button     - Pointer to a long for returned button.          */
/**                                                                                 */
/** RETURNS:                                                                        */
/**   SUCCESS always.  For status of input check istat arg. on return.              */
/**                                                                                 */
/**   Julian Carlisle      Oct 1, 1988.                                             */
/*********************************************************************              */
publicdef INT gpx_wmtbandvector (istat, wx1, wy1, wx2, wy2, button)
GKS_INT *istat;                     /* Out: Status from GKS                         */
GKS_REAL wx1, wy1;
GKS_REAL *wx2, *wy2;                /* Out: WCS                                     */
GKS_INT *button;                    /* Out: Button Code                             */
    {
    
    GKS_INT status = SUCCESS;       /* returned status from locate                  */
    GKS_REAL world_x = 0.0, world_y = 0.0;
    GKS_REAL x, y;
    GKS_INT b, tnr, unity = 0, maintnr;
    GKS_REAL wcs_window[4], wcs_vport[4];   /* saved transformation.                */
    GKS_INT wks_id = MAIN_WKID,     /* Active workstation id.                       */
    xform = 0,                      /* Tnr id retruned from grqlc                   */
    record_size = 0,                /* gks input set up                             */
    choice_value = 0,               /* returned value from choice                   */
    gks_stat = 0,                   /* returned gks  error state                    */
    echo_type = 0,                  /* Set up for gks device                        */
    echo_flag = 0,                  /* Set up for gks device                        */
    err_stat = 0,                   /* Set up for gks device                        */
    initial_status = 0,             /* Set up for gks device                        */
    initial_choice = 0,             /* Set up for gks device                        */
    input_mode = 0,                 /* Set up for gks device                        */
    input_status = 0,               /* Set up for gks device                        */
    record_buflen = 0;              /* sizedata record buf (bytes)                  */
    GKS_INT color;
    GKS_INT line_style;
    GKS_REAL save_line_width;
    GKS_INT save_line_style;
    GKS_INT save_line_color;
    /*                                                                              */
    /*    Default to whole screen for locator input sensitive zone.                 */
    /*                                                                              */
    GKS_REAL echo_area[4];
#ifdef SUN_GKS_OR_PRIOR_GKS   /* ========== BEGIN SUN OR PRIOR GKS ========== */
    static Glocrec locate_datrec;
    GKS_INT locate_dev1 = 1;
#else  /* ========== END SUN OR PRIOR GKS = BEGIN DEC GKS ========== */
    static LOCATE_DATREC locate_datrec;
    GKS_INT locate_dev = 1;         /* General locator device. Standard id          */
#endif /* ========== END DEC GKS ========== */
    GKS_INT pet;
    GKS_REAL wsxmin, wsxmax, wsymin, wsymax;
    
    if (ug_if_gpx () == FALSE)
        return FAIL;
    
    if (wm_static ()->reg.key_type == WM_PD_TABLET)
        {
        return wmtlocate (istat, wx2, wy2, button);
        }
    wm_inq_wkvp_max (&wsxmin, &wsxmax, &wsymin, &wsymax);
    
    /* Inquire & save the current xform number & it's components                    */
    gks_stat = wm_gqcntn (&status, &maintnr);
    
    gks_stat = wm_gqnt (maintnr, &status, wcs_window, wcs_vport);

    /* Make the viewport priority HIGHER than the unity                             */
    gks_stat = wm_gsvpip (maintnr, unity, GHIGHR);
    
    wm_gqplci (&gks_stat, &save_line_color);
    wm_gqln (&gks_stat, &save_line_style);
    wm_gqlwsc (&gks_stat, &save_line_width);
    lu_text_to_index ("COLOR", &color, "NORMAL");
#ifdef USE_X    
#ifdef PRIOR_GKS /* ========== BEGIN PRIOR GKS ========== */
    if (XBlackPixel (wiv.current_Display,
		     XDefaultScreen (wiv.current_Display)) == 0) 
        /* 1 for Sun and 0 for other */
      { /* use white cursor for other */
	lu_text_to_index ("COLOR", &color, "WHITE");
      }
    else
      { /* use black cursor for sun */
	lu_text_to_index ("COLOR", &color, "BLACK");
      }
#endif /* ========== END PRIOR GKS ========== */
#endif
    lu_text_to_index ("LINE_STYLE", &line_style, "SOLID");
    wm_gsln (line_style);
    wm_gsplci (color);
    wm_gslwsc (1.0);

#ifdef SUN_GKS_OR_PRIOR_GKS   /* ========== BEGIN SUN OR PRIOR GKS ========== */
    echo_type = LOC_ECHO_VECTOR;    /* Rubber Banded vector echo prompt.            */
    
    echo_area[0] = wsxmin;
    echo_area[1] = wsxmax;
    echo_area[2] = wsymin;
    echo_area[3] = wsymax;
    
    locate_datrec.pet4.acf = GSPEC; /* specified  attributes                        */
    
    locate_datrec.pet4.ln.ln_type = GINDIV; /* polyline type aspect source flag     */
    locate_datrec.pet4.ln.ln_width = GINDIV;    /* polyline width aspect source flag*/
    locate_datrec.pet4.ln.ln_colour = GINDIV;   /* polyline color aspect source flag*/
    locate_datrec.pet4.ln.line = 1; /* polyline index                               */
    locate_datrec.pet4.ln.lnbundl.type = line_style;
    /* polyline type                                                                */
    locate_datrec.pet4.ln.lnbundl.width = 1.0;  /* polyline width                   */
    locate_datrec.pet4.ln.lnbundl.colour = color;   /* polyline color               */
    
    gks_stat = wm_ginlc (wks_id, locate_dev1, wx1, wy1, maintnr, echo_type, echo_area,
                         record_buflen, &locate_datrec);

#else  /* ========== END SUN OR PRIOR GKS = BEGIN DEC GKS ========== */

    /* Inquire the values of locator device state variables.                        */
    /*   Modify, then replace.                                                      */
    record_buflen = 4;
    gks_stat = wm_gqlcs (wks_id, locate_dev, GREALI, record_buflen, &err_stat,
                         &input_mode, &echo_flag, &xform, &world_x, &world_y,
                         &echo_type, echo_area, &record_size, &locate_datrec);
    
    gks_stat = wm_gslcm (wks_id, locate_dev, GREQU, GNECHO);
    
    echo_type = LOC_ECHO_VECTOR;    /* Rubber Band vector echo prompt.              */
    record_buflen = 32;             /* full  input data record                      */
    
    echo_area[0] = wsxmin;
    echo_area[1] = wsxmax;
    echo_area[2] = wsymin;
    echo_area[3] = wsymax;
    
    locate_datrec.fill_flag = GSPEC;    /* THIS IS THE PATCH!!                      */
    locate_datrec.acf = GSPEC;      /* current or specified  attributes             */
    locate_datrec.type_asf = GINDIV;    /* polyline type aspect source flag         */
    locate_datrec.width_asf = GINDIV;   /* polyline width aspect source flag        */
    locate_datrec.color_asf = GINDIV;   /* polyline color aspect source flag        */
    locate_datrec.polyline_index = 1;   /* polyline index                           */
    locate_datrec.polyline_type = line_style;   /* polyline type                    */
    locate_datrec.width = 1.0;      /* polyline width                               */
    locate_datrec.color_index = color;  /* polyline color                           */
    
    gks_stat = wm_ginlc (wks_id, locate_dev, wx1, wy1, maintnr, echo_type, echo_area,
                         record_buflen, &locate_datrec);
#endif /* ========== END DEC GKS ========== */
    pet = LOC_ECHO_VECTOR;
    status = wmz_getxy (&xform, &world_x, &world_y, button, pet);
    
    *wx2 = world_x;
    *wy2 = world_y;
    
    gks_stat = wm_gsvpip (maintnr, unity, GLOWER);
    
    /* ---------------------------                                                  */
    /* Restore polyline attributes                                                  */
    /* ---------------------------                                                  */
    wm_gslwsc ((DOUBLE)save_line_width);
    wm_gsln (save_line_style);
    wm_gsplci (save_line_color);
    
    *istat = status;
    return SUCCESS;
    }

/********************************************************************               */
/**      Copyright 1988, Exploration Systems Inc. San Francisco                     */
/**                                                                                 */
/** PROGRAM:      WM_GET_CHOICE()                                                   */
/**                                                                                 */
/** DESCRIPTION:                                                                    */
/**   This will display a menu of variable number of items and allow the            */
/**   user to use the workstation mouse to highlight and select one of the options. */
/**                                                                                 */
/**   The value returned is the corresponding integer from the current              */
/**   mouse button map. See WM_SET_MOUSE_MENU.                                      */
/**                                                                                 */
/** ARGUMENTS:                                                                      */
/**   In:     GKS_REAL x,y    -   WCS location for menu.                            */
/**   In:     GKS_INT tnr     -   TNR of x,y input.                                 */
/**   Out:    GKS_INT b       -   Seletion number                                   */
/**                                                                                 */
/** RETURNS:                                                                        */
/**   SUCCESS or the GKS status from input.                                         */
/**   Julian Carlisle      Oct 1, 1988.                                             */
/*********************************************************************              */
publicdef INT wm_get_choice (x, y, tnr, b)
GKS_REAL x, y;                      /* region to place menu near                    */
GKS_INT tnr;                        /* tnr to translate x,y parms                   */
GKS_INT *b;                         /* output selection number                      */
    {
    
    GKS_INT status = SUCCESS;

#ifdef USE_X
    if (ug_if_gpx () == FALSE)
        {
        return FAIL;
        }
    else
        {
        /* -----------------------------------------------                          */
        /* For X based systems, we put up a 'tool palette'                          */
        /* pop-up based upon the current menu, then remap                           */
        /* the 1...N position in the menu list as per the                           */
        /* current button map.                                                      */
        /* -----------------------------------------------                          */
        status = xf_get_choice (rootx, rooty, cur_menu, b);
        wm_map_choice_button (*b, b);
        return status;
        }
#else
    GKS_INT *sizes, n, maxstr = 0, wks_id = MAIN_WKID,  /* Active workstation id.   */
    xform = 0,                      /* Tnr id retruned from grqlc                   */
    choice_value = 0,               /* returned value from choice                   */
    gks_stat = 0,                   /* returned gks  error state                    */
    record_size = 0,                /* gks input set up                             */
    echo_type = 0,                  /* Set up for gks device                        */
    echo_flag = 0,                  /* Set up for gks device                        */
    err_stat = 0,                   /* Set up for gks device                        */
    initial_status = 0,             /* Set up for gks device                        */
    initial_choice = 0,             /* Set up for gks device                        */
    input_mode = 0,                 /* Set up for gks device                        */
    choice_dev,                     /* GKS logical input dev. ( 1 )                 */
    input_status = 0,               /* Set up for gks device                        */
    record_buflen = 0,              /* sizedata record buf (bytes)                  */
    unity = 0, maintnr;             /* Normalization & WCS Tnr ids                  */
    
    GKS_REAL echo_area[4];          /* Rect Extents. Device Coords.                 */
    GKS_REAL dcx, dcy,              /* Non-normalized locator input                 */
        choice_xmin,                /* Origin (DC) x,y of menu box.                 */
        choice_xmax, choice_ymin, choice_ymax, dxmax, dxmin, 
    /* DEV coords.                                                                  */
    dymax, dymin, h_width, h_height, 
    /* Half height of echo region.                                                  */
    width, height, 
    /* Full height of echo region.                                                  */
    wsxmax, wsymax, wsxmin, wsymin;
    TCP p;
    static TCP local_menu = (TCP)0;

#if UNIX  /* ========== BEGIN SUN GKS ========== */
    Gchoicerec tmp_ws;
    Guchpet1 pet1_choice;
    GKS_INT alts;
    static Gchoicerec choice_rec;
    static Gchoicerec temp_choice_rec;
#else  /* ========== END SUN GKS = BEGIN DEC GKS ========== */
    WM_MENU_WS *tmp_ws = (WM_MENU_WS *)0;
#endif  /* ========== END DEC GKS ========== */
    if (ug_if_gpx () == FALSE)
        return FAIL;
    
    /* make sure we have a current menu                                             */
    if (cur_menu == (TCP)0 || cur_button_map == (INT *)0)
        return FAIL;
    
    if (local_menu != (TCP)0)  /* free old menu tcp                            */
        ts_tcp_free (local_menu);
    
    local_menu = TCP_ALLOCATE (ts_tcp_len (cur_menu));  /* get space for new one    */
    ts_tcp_copy (&local_menu, cur_menu);    /* make local copy                      */
    
    for (p = local_menu; *p != NULL; ++p)   /* upper case the strings               */
        ts_sto_upper (*p);
    
    wm_gslcm (wks_id, 1, GREQU, GECHO);
    
    /* Get the maximum device coordinate range for this workstation.                */
    wm_inq_wkvp_max (&wsxmin, &wsxmax, &wsymin, &wsymax);
    
    /* and for the window                                                           */
    wm_inq_wkvp (&dxmin, &dxmax, &dymin, &dymax);
    
    /*                                                                              */
    /** If menu tcp and length passed in, use it for menu.                          */
    /*                                                                              */

#if UNIX  /* ========== BEGIN SUN GKS ========== */
    tmp_ws.pet1.data = (Gchar *)&pet1_choice;
    alts = ts_tcp_len (local_menu);
    pet1_choice.number = alts;
    pet1_choice.strings = local_menu;
    pet1_choice.alt = &alts;
    pet1_choice.colour = 4;
    echo_area[0] = wsxmin;
    echo_area[1] = wsxmax;
    echo_area[2] = wsymin;
    echo_area[3] = wsymax;
    
    /* new state list values for the choice device                                  */
    record_buflen = 12;             /* Length in Bytes.                             */
    initial_choice = 1;             /* Default to choice 1                          */
    initial_status = GOK;           /* Start with 1 selected                        */
    echo_type = 1;                  /* Highlight selection                          */
    choice_dev = 1;                 /* Choice logical dev.                          */
    
    /*                                                                              */
    /* Initialize the choice device with the new input data record.                 */
    /*                                                                              */
    
    gks_stat = wm_ginch (wks_id, choice_dev, initial_status, initial_choice, echo_type,
                         echo_area, record_buflen, &tmp_ws);

#else  /* ========== END SUN GKS = BEGIN DEC GKS ========== */
    tmp_ws = (WM_MENU_WS *) tc_zalloc (sizeof(WM_MENU_WS));
    tmp_ws->tcp = local_menu;
    tmp_ws->nstr = ts_tcp_len (local_menu);
    tmp_ws->sizes = (GKS_INT *) tc_zalloc (sizeof(INT) * tmp_ws->nstr);
    for (n = 0, maxstr = 0; n < tmp_ws->nstr; n++)
        {
        tmp_ws->sizes[n] = strlen (tmp_ws->tcp[n]);
        maxstr = MAX (maxstr, tmp_ws->sizes[n]);
        }
    /*                                                                              */
    /** If the input x,y are not from UNITY (NDC) tnr, convert them to NDC.         */
    /*                                                                              */
    if (tnr != unity)
        status = tg_tnr2tnr (tnr, x, y, unity, &x, &y);
    
    /* Then convert NDC to Device Coords.                                           */
    dcx = (x * (wsxmax - wsxmin)) + wsxmin;
    dcy = (y * (wsymax - wsymin)) + wsymin;
    
    /*****************************************************************************  */
    /**JSC - 3/31/89:             ---- DEVICE DEPENDANCY ---                        */
    /** So, just what is a pixel?:                                                  */
    /**    The next calculation uses constants derived after digging through device */
    /**   font tables and poking at a calculator... I know that sucks but there     */
    /**   doesn't seem to be any other way of consistantly making neat choice menus.*/
    /**   The GKS uses UIS to make choices and doesn't seem to pass info re the     */
    /**   current fontsize. UIS sizes the choice text based on a ratio of the       */
    /**   rectangle size/length of strings passed to it.                            */
    /**                                                                             */
    /**   If left to its own on this matter, gks will render large bulky choice     */
    /**   menus with enormous (well, over-sized at least) characters.               */
    /**                                                                             */
    /**   Naturally I am interested in a better solution to this dilemma, but       */
    /**   unless you fully grok whats up on the next two lines, please don't        */
    /**   alter these values.  Basically my notion is to always try to make         */
    /**   characters as close to 16 PIXELS high as I can calulate them and then     */
    /**   determine the nearest word (16 bit) boundary that fits nstrings*ch_height */
    /**                                                                             */
    /**   As if things weren't ugly enough, I'm afraid it also counts               */
    /**   on hardware fonts being used.  The assumption is safe with the UIS/GKS    */
    /**   combo.                                                                    */
    /****************************************************************************** */
    
    height = CHOICE_CHAR_HT * (FLOAT)((tmp_ws->nstr + 15) & 0xfff8);
    width = (maxstr) ?
        CHOICE_CHAR_HT * (FLOAT)maxstr * CHOICE_ASPECT : CHOICE_ASPECT * height;
    
    h_height = (height * 0.5);
    h_width = (width * 0.5);
    
    /*                                                                              */
    /** X pos.                                                                      */
    /** If any part of the menu will be positioned off the right/left edge,         */
    /** Re-calculate position to be extreme edge, but completely on screen.         */
    /*                                                                              */
    
    if (dcx + h_width >= dxmax)
        choice_xmin = dxmax - width;
    else if (dcx - h_width <= dxmin)
        choice_xmin = dxmin;
    else
        choice_xmin = dcx - h_width;
    
    choice_xmax = choice_xmin + width;
    
    if (dcy + h_height >= dymax)
        choice_ymin = dymax - height;
    else if (dcy - h_height <= dymin)
        choice_ymin = dymin;
    else
        choice_ymin = dcy - h_height;
    
    choice_ymax = choice_ymin + height;
    
    /* Set the extents of the echo-prompt region.                                   */
    echo_area[0] = choice_xmin;     /* Xmin                                         */
    echo_area[1] = choice_xmax;     /* Xmax                                         */
    echo_area[2] = choice_ymin;     /* Ymin                                         */
    echo_area[3] = choice_ymax;     /* Ymax                                         */
    
    
    /* new state list values for the choice device                                  */
    record_buflen = 12;             /* Length in Bytes.                             */
    initial_choice = 1;             /* Default to choice 1                          */
    initial_status = GOK;           /* Start with 1 selected                        */
    echo_type = 1;                  /* Highlight selection                          */
    choice_dev = 1;                 /* Choice logical dev.                          */
    
    /*                                                                              */
    /* Initialize the choice device with the new input data record.                 */
    /*                                                                              */
    
    gks_stat = wm_ginch (wks_id, choice_dev, initial_status, initial_choice, echo_type,
                         echo_area, record_buflen, tmp_ws);

#endif  /* ========== END DEC GKS ========== */
    
    /*                                                                              */
    /* Set the input mode to request/wait                                           */
    /*                                                                              */
    gks_stat = wm_gschm (wks_id, choice_dev, GREQU, GECHO);
    wm_update ();
    
    do
        {
        choice_value = 0;
        gks_stat = wm_grqch (wks_id, choice_dev, &status, &choice_value);
        
        if (status == GOK)
            break;
        
        } while (choice_value == 0);
    
    /* Dont change the button value if none selected.                               */
    
    if (choice_value)
        *b = cur_button_map[choice_value - 1];  /* map choice to correct button     */
    

#if UNIX == 0  /* ========== BEGIN DEC GKS ========== */
    tc_free (tmp_ws->sizes);
    tc_free (tmp_ws);
#endif    /* ========== END DEC GKS ========== */
    wm_gslcm (wks_id, 1, GREQU, GECHO);
    
    if (status & 1)
        status = SUCCESS;
    
    return status;
#endif  /* MAC */
    }

static CHAR PD_DIALOGS[] = "pd_dialogs";
static GKS_INT id;              /* cdlg id                                      */
#define MAX_XY_IBUFLEN  10

/********************************************************************               */
/**      Copyright 1988, Exploration Systems Inc. San Francisco                     */
/**                                                                                 */
/** PROGRAM:  PD_REG_CDLG()   - Register the mapping of a tablet surface.           */
/**                                                                                 */
/** DESCRIPTION:                                                                    */
/**   This routine is a self contained service that guides the user                 */
/**   through a registration procedure for a given paper section. The               */
/**   user is asked to pick three control points on the media to be                 */
/**   digitized that describe the extents of an arbitrary section of                */
/**   the tablet surface to be mapped into the World Coord System that              */
/**   is associated with the section.  A transformation matrix is built             */
/**   and selected that performs the appropriate rotational and scaling             */
/**   corrections for points digitized on the section.  This function is            */
/**   called before requesting tablet coordinates for the map.  The angle           */
/**   of the users media with respect to the tablet surface is returned.            */
/**                                                                                 */
/**   ** NOTE **                                                                    */
/**   This routine uses a black pipe type dialog for user interaction.  This        */
/**   means that once started, the registration must be finished or cancelled       */
/**   before any other FINDER operations can be performed.                          */
/**                                                                                 */
/** ARGUMENTS:                                                                      */
/**   EXTENTS   *regext;  -  Pointer to an extents struct to fillout.               */
/**                                                                                 */
/** RETURN:                                                                         */
/**   SUCCESS or TB_NOT_INIT.                                                       */
/**                                                                                 */
/**   Julian Carlisle      October 28, 1988.                                        */
/*********************************************************************              */
publicdef INT pd_reg_cdlg (regext)
EXTENTS *regext;
    {
    
    GKS_INT status = SUCCESS, i, j, nchanges, value, ival, x[MAX_XY_IBUFLEN],
        y[MAX_XY_IBUFLEN], b[MAX_XY_IBUFLEN], id, cdlg_id = 0, item = 0, normal = 0,
    class  = 0, first_one;
    
    CHAR xstr[32], ystr[32], text[256];
    GKS_INT ul_done = FALSE, ll_done = FALSE, lr_done = FALSE, ur_done = FALSE,
        reg_done = FALSE, reg_cancel = FALSE, index = 0;
    
    unsigned modemask, old_modemask;
    
    
    /* check for valid entry.                                                       */
    if (regext == (EXTENTS *)0)
        return FAIL;
    
    /* disable Finder events/servers. We handle our own                             */
    
    status = fi_push ();
    
    /*                                                                              */
    /*    Get started by bringing up the dialog and pushing the state of the        */
    /*    application manager with FI_PUSH().  This holds off external events       */
    /*    and indicates that this routine will request events locally.              */
    /*                                                                              */
    am_define_resource (AM_DIALOG, PD_DIALOGS, PD_REGISTRATION, NULL_FUNCTION_PTR, &id,
                        NULL);
    status = wiopcd (id);
    first_one = FALSE;
    wiscdv (id, PD_REG_UL_RB, ON);
    if (!status)
        {
        zz_msg (id, "One moment...");
        /*                                                                          */
        /*    We must register the media in POGKS_INT mode for better pick control. */
        /*    Turn on the coord echo alarm if it is not already active.             */
        /*    High resolution mode is best for setting xform extents.               */
        /*                                                                          */
        /*                                                                          */
        /*    If an error is encountered by this operation then something serious is*/
        /*    up with the driver.  Inform the user and abort.                       */
        /*                                                                          */
        if (status)
            {
            sprintf (text, "Tablet Driver Error #%d.", status);
            zz_msg (id, text);
            /*          am_message(AM_ALERT, text);                                 */
            wiencdi (id, PD_REG_DONE_BTN, OFF);
            wiencdi (id, PD_REG_CANCEL_BTN, OFF);
            }
        /*                                                                          */
        /*    Reset the dialog's item states and issue begin message                */
        /*                                                                          */
        
        zz_msg (id, "Ready.");
        
            {
            if (first_one == FALSE)
                goto first_time;
            else
                wirqevt (0, &class , &cdlg_id, &item);
            
            if (cdlg_id != id)
                {
                wibeep (1);
                wiopcd (id);
                }
            else
                {
                
                switch (item)
                    {
                    
                case PD_REG_UL_RB:
                first_time:
                    index = 0;
                    zz_msg (id, "Digitize the upper left corner.");
                    status = pdz_tablet_xy (&x[index], &y[index], &b[index]);
                    first_one = TRUE;
                    if (!status)
                        {
                        sprintf (xstr, "%d", x[index]);
                        sprintf (ystr, "%d", y[index]);
                        wiscdt (id, PD_REG_UL_X_ET, xstr);
                        wiscdt (id, PD_REG_UL_Y_ET, ystr);
                        ul_done = TRUE;
                        }
                    break;
                    
                case PD_REG_LL_RB:
                    index = 1;
                    zz_msg (id, "Digitize the lower left corner.");
                    status = pdz_tablet_xy (&x[index], &y[index], &b[index]);
                    if (!status)
                        {
                        sprintf (xstr, "%d", x[index]);
                        sprintf (ystr, "%d", y[index]);
                        wiscdt (id, PD_REG_LL_Y_ET, ystr);
                        wiscdt (id, PD_REG_LL_X_ET, xstr);
                        ll_done = TRUE;
                        }
                    break;
                    
                case PD_REG_LR_RB:
                    index = 2;
                    zz_msg (id, "Digitize the lower right corner.");
                    status = pdz_tablet_xy (&x[index], &y[index], &b[index]);
                    if (!status)
                        {
                        sprintf (xstr, "%d", x[index]);
                        sprintf (ystr, "%d", y[index]);
                        wiscdt (id, PD_REG_LR_Y_ET, ystr);
                        wiscdt (id, PD_REG_LR_X_ET, xstr);
                        lr_done = TRUE;
                        }
                    break;
                    
                case PD_REG_UR_RB:
                    index = 3;
                    zz_msg (id, "Digitize the upper right corner.");
                    status = pdz_tablet_xy (&x[index], &y[index], &b[index]);
                    if (!status)
                        {
                        sprintf (ystr, "%d", y[index]);
                        sprintf (xstr, "%d", x[index]);
                        wiscdt (id, PD_REG_UR_Y_ET, ystr);
                        wiscdt (id, PD_REG_UR_X_ET, xstr);
                        ur_done = TRUE;
                        }
                    break;
                    
                case PD_REG_DONE_BTN:
                    reg_done = TRUE;
                    break;
                    
                case PD_REG_CANCEL_BTN:
                    reg_cancel = TRUE;
                    break;
                    }
                }
            }
        while (!reg_done && !reg_cancel)
            ;
        if (ul_done && ll_done && lr_done && ur_done)
            {
            regext->ul_x = x[0];
            regext->ul_y = y[0];
            regext->ll_x = x[1];
            regext->ll_y = y[1];
            regext->lr_x = x[2];
            regext->lr_y = y[2];
            regext->ur_x = x[3];
            regext->ur_y = y[3];
            status = SUCCESS;
            }
        else
            {
            zz_msg (id, "Not corners entered.  Registration ignored.");
            status = CANCEL;
            }
        }
    else
        {
        am_message (AM_ALERT, mg_message (status));
        }
    am_release_resource (id);
    
    fi_pop ();
    
    return status;
    }

/************************************************************************************/

static INT pdz_tablet_xy (x, y, b)
GKS_INT *x, *y, *b;
    {
    GKS_INT status = SUCCESS, cnt = 0, tx,  /* rAW TABLET X COORD                   */
    ty,                             /* and y                                        */
    tb;                             /* button for this coord pair.                  */
    
    tbh_clear_buffer ();
    /*                                                                              */
    /*    Poll for XY input from tablet.  Wait for a coord unless                   */
    /*    severe problems arise, i.e. driver not initialized.                       */
    /*                                                                              */
    tb = 0;
    while (cnt == 0)
        {
        status = tbh_get_points (1, &tx, &ty, &tb, &cnt);
        
        if (status == TB_NOT_INIT || (tb == 12))
            break;
        }
    if (tb == 12)
        status = CANCEL;
    else if (cnt >= 1)
        {
        *x = tx;
        *y = ty;
        *b = tb;
        }
    else
        {
        *x = 0;
        *y = 0;
        *b = 0;
        }
    return status;
    }

static INT zz_msg (dlgid, str)
GKS_INT dlgid;
CHAR *str;
    {
    wiscdt (dlgid, PD_REG_MSG_ST, str);
    }

/****************************************************************************       */
/**                                                                                 */
/** WM_SET_MOUSE_MENU                                                               */
/**                                                                                 */
/** Arguments:                                                                      */
/**   TCP menutcp;   Input: TCP (null terminated!) of menu choice strings           */
/**   INT *buttonmap;   Input: Array of selections corresponding to menutcp         */
/**                                                                                 */
/** Description: This routine controls what menu will be displayed when             */
/**   WM_GET_CHOICE is called.  Each application calling WMTLOCATE should           */
/**   expect that users using a workstation mouse may request a menu, and           */
/**   therefore each application must provide one BEFORE calling WMTLOCATE.         */
/**   If they do not, WM_GET_CHOICE will just use the most recently set menu.       */
/**   If the application wishes to use the default menu, it should call             */
/**   this routine with null arguments.                                             */
/**                                                                                 */
/************************************************************************           */
publicdef INT wm_set_mouse_menu (menutcp, buttonmap)
TCP menutcp;
INT *buttonmap;
    {
    INT status, curValue;
    
    if (ug_if_gpx () == FALSE)
        return FAIL;
    
    if (menutcp == (TCP)0 || buttonmap == (INT *)0)
        {
        cur_menu = default_menu;
        cur_button_map = default_button_map;
        }
    else
        {
        cur_menu = menutcp;         /* set current menu                             */
        cur_button_map = buttonmap; /* set the current button map which corresponds */
                                    /*                                              */
                                    /*to the current menu                           */
        }
    return SUCCESS;
    }

publicdef INT wm_map_choice_button (inbtn, outbtn)
INT inbtn;
INT *outbtn;
    {
    
    *outbtn = cur_button_map[inbtn];    /* map choice to correct button             */
    
    return SUCCESS;
    }

publicdef BOOL wm_is_menu_button()
    {
    return(is_menu_button);
    }

