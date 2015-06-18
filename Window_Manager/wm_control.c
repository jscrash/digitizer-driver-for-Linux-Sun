/* DEC/CMS REPLACEMENT HISTORY, Element WM_CONTROL.C*/
/* *20   16-AUG-1991 16:54:26 MING "(SPR 0) pass window NDC to pf_close_file in zz_plot"*/
/* *19   16-AUG-1991 11:11:15 JTM "(SPR 0) Changed arg to wm_gslwsc() to DOUBLE."*/
/* *18   12-JUL-1991 10:16:51 MING "(SPR 0) put back PLOT button"*/
/* *17    6-JUN-1991 17:29:04 MING "(SPR 0) delete obsolete buttons"*/
/* *16   31-JUL-1990 16:48:09 GILLESPIE "(SPR 1) Add esi_gks.h"*/
/* *15   10-JUL-1990 19:15:50 MING "(SPR -1) TURN OFF PLOT BUTTON IF NO_WCPLOT IS USED"*/
/* *14   20-JUN-1990 15:01:58 GILLESPIE "(SPR 1) Add esi_pf.h and esi_ho.h (wouldn't compile without them...)"*/
/* *13   20-JUN-1990 10:55:46 MING "(SPR 0) add zz_plot"*/
/* *12   28-MAY-1990 12:06:02 SCD "(SPR 1) Eliminate extra redraws for each overlay OFF action."*/
/* *11   14-MAY-1990 14:30:31 SCD "(SPR 1) Fix SPR re: zoom in/out overriding visibility settings"*/
/* *10    3-MAY-1990 21:45:05 KEE "(SPR -1) multi window support."*/
/* *9     3-MAY-1990 21:37:26 KEE "(SPR -1) multi window support."*/
/* *8    30-MAR-1990 10:09:05 KEE "(SPR 7171) Temporary Dim the (Resize Button) "*/
/* *7    19-NOV-1989 13:35:21 GILLESPIE "(SPR 5075) Fixes for event-driven non global interface"*/
/* *6     6-NOV-1989 14:48:08 GILLESPIE "(SPR 1) Remove unreferenced functions"*/
/* *5    21-SEP-1989 15:18:03 JULIAN "(SPR 4321) added plot button to dialog"*/
/* *4    14-SEP-1989 16:21:40 GORDON "(SPR 100) Gulf/Sun/Unix mods"*/
/* *3    11-AUG-1989 16:18:15 JULIAN "(SPR -1) function wmcontrol, wmtselector has return(e), and return;"*/
/* *2    20-JUL-1989 11:05:27 GORDON "(SPR 100) Sun port mods"*/
/* *1    19-JUN-1989 13:40:39 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element WM_CONTROL.C*/

/******************************************************************************

                Copyright Exploration Systems, Inc. 1989
		   Unpublished -- All rights reserved

THIS SOFTWARE IS THE PROPRIETARY PROPERTY OF Exploration Systems, Inc.  AND MAY
CONTAIN CONFIDENTIAL TRADE SECRET INFORMATION. IT IS LICENSED FOR USE ON THE
DESIGNATED EQUIPMENT ON WHICH IT WAS ORIGINALLY INSTALLED AND  MAY NOT BE
MODIFIED, DUPLICATED OR COPIED IN ANY FORM WITHOUT PRIOR WRITTEN CONSENT OF

			Exploration Systems, Inc.
			579 Market Street
			San Francisco, CA  USA 94105
			(415) 974-1600

(The above notice does not constitute an admission of publication or
unrestricted dissemination of the work)

******************************************************************************/

/* WM.CONTROL.CC,  GUY>CC>SOURCE,  GUY H BINNS,  10/31/86
   Allow user control of windows
   Copyright (c) 1985, Exploration Systems Inc, Sausalito, CA. */

/* TITLE : WM - Allow user control of windows */


/* START-CODE : */
#ifndef ESI_C_LIB_H
#include "esi_c_lib.h"
#endif

#ifndef ESI_WM_INT_H
#include "esi_wm_int.h"
#endif

#ifndef ESI_AM_H
#include "esi_am.h"
#endif

#ifndef ESI_HO_H
#include "esi_ho.h"
#endif

#ifndef ESI_FI_H
#include "esi_fi.h"
#endif

#ifndef ESI_GKS_H
#include "esi_gks.h"
#endif

#ifndef ESI_PF_H
#include "esi_pf.h"
#endif

#ifndef ESI_LL_H
#include "esi_ll.h"
#endif

#ifndef ESI_WI_INT_X
#include "esi_wi_int.x"    /* gets journal/playback stuff */
#endif

#ifndef ESI_WM_CLS_H
#include "esi_wm_cls.h"
#endif

#ifndef ESI_TC_H
#include "esi_tc.h"
#endif

#ifndef ESI_TG_H
#include "esi_tg.h"
#endif

#ifndef ESI_PR_H
#include "esi_pr.h"
#endif

#ifndef ESI_WM_MSG_H
#include "esi_wm_msg.h"
#endif

#ifndef ESI_WM_ERR_H
#include "esi_wm_err.h"
#endif

#ifndef ESI_MG_H
#include "esi_mg.h"
#endif

#ifndef WM_DIALOGS_RH
#include "wm_dialogs.rh"
#endif

#ifndef ESI_UG_H
#include "esi_ug.h"
#endif

#ifndef ESI_LU_H
#include "esi_lu.h"
#endif

#ifndef ESI_CONF_WS_H
#include "esi_conf_ws.h"
#endif

#define NO_ICON 0
#define WINDOW_ICON 29
#define FOLDER_ICON 30
#define DOCUMENT_ICON 31

#define CTRL_SIZE 0.10
#define NO_CTRL 0
#define LL_CTRL 1
#define LR_CTRL 2
#define UR_CTRL 3
#define UL_CTRL 4
#define IN_CTRL 5

static CHAR DIALOG_FILE[] = "wm_dialogs";
#define HELP_FILE   ""

#define MAX_OBJS 200
#define MAX_SELECTABLE 16

static   INT wc_id = 0 ;
static   INT  do_update = FALSE;
static   INT  curr_error = 0 ;
static   INT  curr_prompt = 0 ;
static   WM_ID  curr_obj = 0, last_obj = 0 ;
static   INT  curr_typ = 0 ;
static   CHAR curr_str[WM_USR_STR_SIZE+1] = {""};
static   INT  num_objs = 0 ;
static   INT  num_strs = 0 ;
static   WM_ID  obj_ids[MAX_OBJS] ;
static   CHAR *obj_strs[MAX_OBJS] ;
static   APPLICATION_ID appSave;
static   WINDOW_ID winSave;
static   TNR_ID tnrSave;
static   GROUP_ID groSave ;
static   GKS_INT defmod, regmod;

/* External routines */

VOID GKZ_DRAWING_ON();
BOOL GKZ_DRAWING_TEST();
VOID gkz_inq_workstation();

/* Internal routines */

#if USE_PROTOTYPES

static VOID zz_append (LISTHEAD *list, INT icon ) ;
static VOID zz_band (INT *statPtr, DOUBLE wx1, DOUBLE wy1,
		     FLOAT *wx2Ptr, FLOAT *wy2Ptr, INT *butPtr);
static VOID zz_close (void) ;
static VOID zz_delete (void) ;
static VOID zz_drag (INT *statPtr, DOUBLE llx, DOUBLE lly, DOUBLE urx,
		     DOUBLE ury, FLOAT *llxPtr, FLOAT *llyPtr, INT *butPtr) ;
static VOID zz_draw_ctrls ( DOUBLE llx, DOUBLE lly, DOUBLE urx, DOUBLE ury,
			    INT color ) ;
static VOID zz_drawing (void) ;
static VOID zz_front (void) ;
static INT  zz_in_ctrl (DOUBLE llx, DOUBLE lly, DOUBLE urx, DOUBLE ury,
			DOUBLE  wx, DOUBLE wy ) ;
static VOID zz_on_off (INT on_off) ;
static VOID zz_open (void) ;
static VOID zz_resize (void) ;
static VOID zz_screen (INT screen_number) ;
static VOID zz_update (void) ;
static VOID zz_zoom (BOOL if_in) ;
publicdef VOID zz_plot(void);

#else

static VOID zz_append() ;
static VOID zz_band() ;
static VOID zz_close() ;
static VOID zz_delete() ;
static VOID zz_drag() ;
static VOID zz_draw_ctrls() ;
static VOID zz_drawing() ;
static VOID zz_front() ;
static INT  zz_in_ctrl() ;
static VOID zz_on_off() ;
static VOID zz_open() ;
static VOID zz_resize() ;
static VOID zz_screen() ;
static VOID zz_update() ;
static VOID zz_zoom() ;
publicdef VOID zz_plot();
#endif
/******************************************************************************/

static INT wm_control_server(dlog_id,dlog_item, ptr)
INT dlog_id;      /* this argument is not used in this routine */
INT dlog_item;
BYTE *ptr;        /* this argument is not used in this routine */
{
    INT status = SUCCESS;

    wm_hold_updates();      /* Buffer screen changes for efficiency. JSC */

    switch (dlog_item) {
	case ITEM_WC_OK:
	    wmoattach( appSave, winSave, tnrSave, groSave, &status ) ;
	    if (status IS_NOT_EQUAL_TO SUCCESS)
		{
		status = WM_ERR_ATTACH_FAILED ;
		}
	   wm_update();        /* Update screen surface if necessary. JSC */
	   if (ug_if_gpx() == TRUE)   /* Restore original deferral. */
	     wm_gsds(MAIN_WKID, defmod, regmod);
	   fi_pop();
	   am_quit();
	   return SUCCESS;
	case ITEM_WC_OPEN:
	    zz_open() ;
	    break ;
	case ITEM_WC_CLOSE:
	    zz_close() ;
	    break ;
	case ITEM_WC_ON:
	    zz_on_off( GVISI ) ;
	    break ;
	case ITEM_WC_OFF:
	    zz_on_off( GINVIS ) ;
	    break ;
	case ITEM_WC_FRONT:
	    zz_front() ;
	    break ;
	case ITEM_WC_PLOT:
	    zz_plot();
	    break ;

	/* -------------------------------------------------------- */
        /* take off the following buttons since they are not useful */
	/* at this moment. may or may not re-activate in the future */
	/* -------------------------------------------------------- */
#if 0
	case ITEM_WC_RESIZE:
	    if (ug_if_gpx() == FALSE)
		zz_resize() ;
	    break ;
	case ITEM_WC_DELETE:
	    zz_delete() ;
	    break ;
	case ITEM_WC_LEFT:
	    if (ug_if_gpx() == FALSE)
		zz_screen(1) ;
	    break ;
	case ITEM_WC_RIGHT:
	    if (ug_if_gpx() == FALSE)
		zz_screen(2) ;
	    break ;
	case ITEM_WC_ZOOM_IN:
	    zz_zoom(TRUE);
	    break;
	case ITEM_WC_ZOOM_OUT:
	    zz_zoom(FALSE);
	    break;
#endif
        /* ------------------------------------------------------------*/

	default:
	    curr_error = WM_ERROR_UNKNOWN_BUTTON ;
	    do_update = TRUE ;
	}

    if (do_update)
	{
	zz_update() ;
	}

    wm_update();        /* Update screen surface if necessary. JSC */

    return status;
}

/******************************************************************************/

publicdef INT  wm_control(ptr)
VOIDPTR ptr;
{
   INT returned_status = SUCCESS ;     /* status of called routines */
   INT dlog_id, dlog_item, class;
   INT status ;
   GKS_INT  ier, dum1, dum2;
   CONFIGURATION_WORKSPACE *c;

   wmowhere( &appSave, &winSave, &tnrSave, &groSave, &status ) ;
   if (status EQUALS FAIL)
   {
      returned_status =  WM_ERR_WHERE_FAILED ;
      return returned_status ;
   }
   if (appSave EQUALS 0)
   {
      returned_status =  WM_ERR_NOT_APP_OPEN ;
      return returned_status ;
   }

   am_declare("WM Control");
   fi_push();

   returned_status = am_open_workspace ("CONFIGURATION",AM_GLOBAL,(VOIDPTR *) &c);

    if (returned_status != SUCCESS)
	{
	am_quit();
	return returned_status;
	}

   returned_status = am_define_resource(AM_DIALOG,DIALOG_FILE,DIALOG_WC,
	    wm_control_server,&wc_id,NULL);
    if (returned_status != SUCCESS)
	{
	am_quit();
	return returned_status;
	}

	/* -------------------------------------------------------- */
        /* take off the following buttons since they are not useful */
	/* at this moment. may or may not re-activate in the future */
	/* -------------------------------------------------------- */
#if 0
   if (c->wiss == FALSE)
     {
     wi_enable_dialog_item (wc_id, ITEM_WC_PLOT, OFF);
     }
#endif

   wi_open_dialog( wc_id ) ;

   curr_obj = 0 ;
   last_obj = -1 ;
   curr_typ = 0 ;
   curr_str[0] = NUL ;
   curr_error = WM_ERROR_NONE ;
   curr_prompt = WM_PROMPT_DEFAULT ;

   if (winSave OR groSave)
   {
      if (groSave)
      {
	 curr_obj = groSave ;
      }
      else
      {
	 curr_obj = winSave ;
      }
      wmoinqtyp( curr_obj, &curr_typ ) ;
   }

   do_update = TRUE ;
   zz_update() ;

   if (ug_if_gpx() == TRUE)   /* save previous deferral state. */
	wm_gqwkdu(MAIN_WKID, &ier, &defmod, &regmod, &dum1, &dum2);

   return SUCCESS;
}

/* === ZZ_UPDATE === */

static VOID zz_update()

{
   INT class, num_sel ;
   CHAR db_name[WM_DBS_STR_SIZE] ;
   LISTHEAD win_list, gro_list, seg_list ;
   static INT workstation_heads = 0, workstation_type, workstation_planes;
   static INT multihead = OFF;
   CHAR error_msg[240];
   CHAR prompt_msg[240];

    if (ug_if_gpx() == FALSE)
	{
	if (workstation_heads == 0)
	    {
	    gkz_inq_workstation(&workstation_type, &workstation_heads,
			 &workstation_planes);
	    multihead = (workstation_heads > 1 ? ON : OFF);
	    }
	}

   if (curr_obj IS_NOT_EQUAL_TO last_obj)
   {
      last_obj = curr_obj ;
      if (curr_obj IS_NOT_EQUAL_TO 0)
      {
	 wmoinq( curr_obj, &class, db_name, &(curr_str[1]) ) ;
	 switch (curr_typ)
	 {
	    case (WM_CLASS_WINDOW):
	    {
	       curr_str[0] = FOLDER_ICON ;
	       break ;
	    }
	    case (WM_CLASS_GROUP):
	    {
	       curr_str[0] = FOLDER_ICON ;
	       break ;
	    }
	    default:
	    {
	       curr_str[0] = DOCUMENT_ICON ;
	    }
	 }
      }
      else
      {
	 strcpy( curr_str, "Windows:" ) ;
      }
      wi_set_dialog_text( wc_id, ITEM_WC_CURRENT, curr_str ) ;

      num_objs = 0 ;

	/* -------------------------------------------------------- */
        /* take off the following buttons since they are not useful */
	/* at this moment. may or may not re-activate in the future */
	/* -------------------------------------------------------- */
#if 0
/* ADD BY JJC, 02/17/89 Dim the DELETE BUTTON to avoid the system crash. */
      wi_enable_dialog_item(wc_id, ITEM_WC_DELETE, OFF);
#endif

      switch (curr_typ)
      {
	 case ( WM_CLASS_WINDOW ):
	 {
	    llinit( &seg_list, 0, sizeof(INT), 0 ) ;
	    wmsinqlist( WM_CLASS_SEGMENT, &seg_list ) ;
	    zz_append( &seg_list, DOCUMENT_ICON ) ;
	    llfree( &seg_list ) ;

	    llinit( &gro_list, 0, sizeof(INT), 0 ) ;
	    wmginqlist( WM_CLASS_GROUP, &gro_list ) ;
	    zz_append( &gro_list, FOLDER_ICON ) ;
	    llfree( &gro_list ) ;

	    wi_enable_dialog_item( wc_id, ITEM_WC_FRONT, ON ) ;
	/* -------------------------------------------------------- */
        /* take off the following buttons since they are not useful */
	/* at this moment. may or may not re-activate in the future */
	/* -------------------------------------------------------- */
#if 0
	    wi_enable_dialog_item( wc_id, ITEM_WC_CLOSE, ON ) ;
	    wi_enable_dialog_item( wc_id, ITEM_WC_RESIZE, ON ) ;
	    wi_enable_dialog_item( wc_id, ITEM_WC_LEFT, multihead ) ;
	    wi_enable_dialog_item( wc_id, ITEM_WC_RIGHT, multihead );
#endif 

	    break ;
	 }
	 case ( WM_CLASS_GROUP ):
	 {
	    llinit( &seg_list, 0, sizeof(INT), 0 ) ;
	    wmsinqlist( WM_CLASS_SEGMENT, &seg_list ) ;
	    zz_append( &seg_list, DOCUMENT_ICON ) ;
	    llfree( &seg_list ) ;

	    llinit( &gro_list, 0, sizeof(INT), 0 ) ;
	    wmginqlist( WM_CLASS_GROUP, &gro_list ) ;
	    zz_append( &gro_list, FOLDER_ICON ) ;
	    llfree( &gro_list ) ;

	    wi_enable_dialog_item( wc_id, ITEM_WC_FRONT, ON ) ;
	/* -------------------------------------------------------- */
        /* take off the following buttons since they are not useful */
	/* at this moment. may or may not re-activate in the future */
	/* -------------------------------------------------------- */
#if 0
	    wi_enable_dialog_item( wc_id, ITEM_WC_CLOSE, ON ) ;
	    wi_enable_dialog_item( wc_id, ITEM_WC_RESIZE, ON ) ;
	    wi_enable_dialog_item( wc_id, ITEM_WC_LEFT, OFF ) ;
	    wi_enable_dialog_item( wc_id, ITEM_WC_RIGHT, OFF ) ;
	    break ;
#endif
	 }
	 case ( WM_CLASS_SEGMENT ):
	 {
	    wi_enable_dialog_item( wc_id, ITEM_WC_FRONT, ON ) ;
	/* -------------------------------------------------------- */
        /* take off the following buttons since they are not useful */
	/* at this moment. may or may not re-activate in the future */
	/* -------------------------------------------------------- */
#if 0
	    wi_enable_dialog_item( wc_id, ITEM_WC_CLOSE, ON ) ;
	    wi_enable_dialog_item( wc_id, ITEM_WC_RESIZE, ON ) ;
	    wi_enable_dialog_item( wc_id, ITEM_WC_LEFT, OFF ) ;
	    wi_enable_dialog_item( wc_id, ITEM_WC_RIGHT, OFF ) ;
#endif
	    break ;
	 }
	 default:
	 {
	    llinit( &win_list, 0, sizeof(INT), 0 ) ;
	    wmwinqlist( WM_CLASS_WINDOW, &win_list ) ;
	    zz_append( &win_list, FOLDER_ICON ) ;
	    llfree( &win_list ) ;
	    wi_enable_dialog_item( wc_id, ITEM_WC_FRONT, ON ) ;
	/* -------------------------------------------------------- */
        /* take off the following buttons since they are not useful */
	/* at this moment. may or may not re-activate in the future */
	/* -------------------------------------------------------- */
#if 0
	    wi_enable_dialog_item( wc_id, ITEM_WC_CLOSE, OFF ) ;
	    wi_enable_dialog_item( wc_id, ITEM_WC_RESIZE, ON ) ;
	    wi_enable_dialog_item( wc_id, ITEM_WC_LEFT, multihead ) ;
	    wi_enable_dialog_item( wc_id, ITEM_WC_RIGHT, multihead );
#endif
	 }
      }
      if (num_objs EQUALS 0)
      {
	 wi_enable_dialog_item( wc_id, ITEM_WC_OPEN, OFF ) ;
	 wi_enable_dialog_item( wc_id, ITEM_WC_FRONT, OFF ) ;
	 wi_enable_dialog_item( wc_id, ITEM_WC_ON, OFF ) ;
	 wi_enable_dialog_item( wc_id, ITEM_WC_OFF, OFF ) ;
	/* -------------------------------------------------------- */
        /* take off the following buttons since they are not useful */
	/* at this moment. may or may not re-activate in the future */
	/* -------------------------------------------------------- */
#if 0
	 wi_enable_dialog_item( wc_id, ITEM_WC_RESIZE, OFF ) ;
	 wi_enable_dialog_item( wc_id, ITEM_WC_DELETE, OFF ) ;
	 wi_enable_dialog_item( wc_id, ITEM_WC_LEFT, OFF ) ;
	 wi_enable_dialog_item( wc_id, ITEM_WC_RIGHT, OFF ) ;
#endif
	 num_sel = 1 ;
	 strcpy( obj_strs[0], "..." ) ;
      }
      else
      {
	 wi_enable_dialog_item( wc_id, ITEM_WC_OPEN, ON ) ;
	 wi_enable_dialog_item( wc_id, ITEM_WC_ON, ON ) ;
	 wi_enable_dialog_item( wc_id, ITEM_WC_OFF, ON ) ;
/* COM BY JJC 02/21/89. For the time being dim this button.
	 wi_enable_dialog_item( wc_id, ITEM_WC_DELETE, ON ) ;   */
	 num_sel = num_objs ;
      }
      wisselt( wc_id, ITEM_WC_SELECTOR, ITEM_WC_VSCROLL, num_sel,
	       MAX_SELECTABLE, obj_strs ) ;
   }

   switch (curr_prompt)
     {
     case WM_PROMPT_DEFAULT:
       strcpy (prompt_msg,"Please make a selection from the selector box \
and then click on the appropriate Window Control function button.");
       break;
     default:
       strcpy (prompt_msg,"");
     }

   switch (curr_error)
     {
     case WM_ERROR_NOTHING_SELECTED:
       strcpy (error_msg,"Make a selection from the selector box!");
       break;
     case WM_ERROR_MORE_THAN_ONE_SELECTED:
       strcpy (error_msg,"Please make a single selection.");
       break;
     case WM_ERROR_NONE:
       strcpy (error_msg,"");
       break;
     }

   wi_set_dialog_text( wc_id, ITEM_WC_PROMPT, prompt_msg);
   wi_set_dialog_text( wc_id, ITEM_WC_ERROR, error_msg);

   do_update = ((curr_error IS_NOT_EQUAL_TO WM_ERROR_NONE) OR (curr_prompt IS_NOT_EQUAL_TO WM_PROMPT_DEFAULT)) ;
   curr_error = WM_ERROR_NONE ;
   curr_prompt = WM_PROMPT_DEFAULT ;
}

/* === ZZ_APPEND === */

static VOID zz_append( list, icon )

LISTHEAD *list ;
INT icon ;

{
   INT count, i, obj_id, class ;
   CHAR db_name[WM_DBS_STR_SIZE] ;

   if (num_strs < 1)
   {
      num_strs++ ;
      obj_strs[num_strs-1] = (CHAR *)tc_alloc(WM_USR_STR_SIZE+1) ;
   }

   count = llcount( list ) ;
   if (count > 0)
   {
      for (i = 0; i < count; i++)
      {
	 if (num_objs < MAX_OBJS)
	 {
	    if (i EQUALS 0)
	    {
	       llfirst( list, &obj_id ) ;
	    }
	    else
	    {
	       llnext( list, &obj_id ) ;
	    }
	    num_objs++ ;
	    obj_ids[num_objs-1] = obj_id ;
	    if (num_objs > num_strs)
	    {
	       num_strs++ ;
	       obj_strs[num_strs-1] = (CHAR *)tc_alloc(WM_USR_STR_SIZE+1) ;
	    }
	    if (icon IS_NOT_EQUAL_TO NO_ICON)
	    {
	       wmoinq( obj_id, &class, db_name, &(obj_strs[num_objs-1][1]) ) ;
	       obj_strs[num_objs-1][0] = icon ;
	    }
	    else
	    {
	       wmoinq( obj_id, &class, db_name, obj_strs[num_objs-1] ) ;
	    }
	 }
	 else
	 {
	    curr_error = WM_ERROR_MAX_OBJECTS ;
	    break ;
	 }
      }
   }
}

/* === ZZ_OPEN === */

static VOID zz_open()

{
   INT num_items, items[MAX_SELECTABLE] ;

   wiqsel( wc_id, ITEM_WC_SELECTOR, &num_items, items ) ;
   if ((num_items EQUALS 1) AND (num_objs > 0))
   {
      curr_obj = obj_ids[items[0]-1] ;
      wmoinqtyp( curr_obj, &curr_typ ) ;
      switch (curr_typ)
      {
	 case ( WM_CLASS_WINDOW ):
	 {
	    wmwopen( curr_obj ) ;
	    break ;
	 }
	 case ( WM_CLASS_GROUP ):
	 {
	    wmgopen( curr_obj ) ;
	    break ;
	 }
	 default:
	 {
	 }
      }
      do_update = TRUE ;
   }
   else
   {
      if ((num_items <= 0) OR (num_objs <= 0))
      {
	 curr_error = WM_ERROR_NOTHING_SELECTED ;
	 do_update = TRUE ;
      }
      else
      {
	 curr_error = WM_ERROR_MORE_THAN_ONE_SELECTED ;
	 do_update = TRUE ;
      }
   }
}

/* === ZZ_CLOSE === */

static VOID zz_close()

{
   APPLICATION_ID app_id;
   WINDOW_ID win_id;
   TNR_ID tnr_id;
   GROUP_ID gro_id;
   SEGMENT_ID seg_id;
   PICK_ID pic_id;

   switch (curr_typ)
   {
      case ( WM_CLASS_WINDOW ):
      {
	 wmwclose( curr_obj ) ;
	 curr_obj = 0 ;
	 curr_typ = 0 ;
	 break ;
      }
      case ( WM_CLASS_GROUP ):
      {
	 wmgclose( curr_obj ) ;
	 /* no break, continue to class segment */
      }
      case ( WM_CLASS_SEGMENT ):
      {
	 wmoinqold( curr_obj, &app_id, &win_id, &tnr_id, &gro_id, &seg_id, &pic_id ) ;
	 if (gro_id IS_NOT_EQUAL_TO 0)
	 {
	    curr_obj = gro_id ;
	    curr_typ = WM_CLASS_GROUP ;
	 }
	 else
	 {
	    curr_obj = win_id ;
	    curr_typ = WM_CLASS_WINDOW ;
	 }
      }
      default:
      {
      }
   }
   do_update = TRUE ;
}

/* === ZZ_ON_OFF === */

static VOID zz_on_off( on_off )

INT on_off ;

{
   INT num_items, items[MAX_SELECTABLE], obj_typ, i ;
   WM_ID obj_id;
   APPLICATION_ID app;
   WINDOW_ID win;
   TNR_ID tnr;
   GROUP_ID gro;
   INT status;

   wiqsel( wc_id, ITEM_WC_SELECTOR, &num_items, items ) ;
/* Reset the selected values... */
   wi_set_defaults( wc_id, ITEM_WC_SELECTOR, num_items, items, ON) ;
   if ((num_items > 0) AND (num_objs > 0))
   {
      wmowhere(&app, &win, &tnr, &gro, &status);    /* find current window, if any */
						    /* so we can assoc screen */
      for (i = 0; i < num_items; i++)
      {
	 obj_id = obj_ids[items[i]-1] ;
	 wmoinqtyp( obj_id, &obj_typ ) ;
	 switch (obj_typ)
	 {
	    case ( WM_CLASS_WINDOW ):
	    {
		if (ug_if_gpx() == FALSE)
		    wm_attach_window_screen( obj_id );      /* associate wk-station screen */
	       wmwvis( obj_id, on_off ) ;
	       break ;
	    }
	    case ( WM_CLASS_GROUP ):
	    {
		if (ug_if_gpx() == FALSE)
		    if (on_off) wm_attach_window_screen( win );  /* associate wk-station screen */
	       wmgvis( obj_id, on_off ) ;
	       break ;
	    }
	    case ( WM_CLASS_SEGMENT ):
	    {
		if (ug_if_gpx() == FALSE)
		    if (on_off) wm_attach_window_screen( win );  /* associate wk-station screen */
	       wmsvis( obj_id, on_off ) ;
	       break ;
	    }
	    default:
	    {
	    }
	 }
#if 0	 
	 wm_grsgwk( MAIN_WKID ) ;
#endif

       }
   }
   else
   {
      curr_error = WM_ERROR_NOTHING_SELECTED ;
      do_update = TRUE ;
   }
}
/* === ZZ_DRAWING === */
/* Enable or disable the Raster Driver option for displaying
   while a segment is open.

   This code is Raster Driver dependent and will generally not be
   used for other workstation drivers
*/
static VOID zz_drawing()

{
    if (ug_if_gpx() == FALSE)
	{
	if ( GKZ_DRAWING_TEST(0) )
	    {
	    GKZ_DRAWING_OFF();
	    curr_prompt = WM_PROMPT_DRAWING_OFF;
	    }
	else
	    {
	    GKZ_DRAWING_ON();
	    curr_prompt = WM_PROMPT_DRAWING_ON;
	    }
	do_update = TRUE;       /* Force zz_update to happen so user can see message */
	}
}

/* === ZZ_FRONT === */

static VOID zz_front()

{
   INT num_items, items[MAX_SELECTABLE], obj_typ ;
   WM_ID obj_id;
   APPLICATION_ID app;
   WINDOW_ID win;
   TNR_ID tnr;
   GROUP_ID gro;
   INT status;

/* if there is a window open, use it */
   wmowhere( &app, &win, &tnr, &gro, &status ) ;
   if (win)
   {
      num_items = 1 ;
      obj_id = win ;
      obj_typ = WM_CLASS_WINDOW ;
      wmoattach( app, 0, 0, 0, &status ) ;
   }
   else
   {
/* otherwise check selector */
      wiqsel( wc_id, ITEM_WC_SELECTOR, &num_items, items ) ;
/* Reset the selected values... */
      wi_set_defaults( wc_id, ITEM_WC_SELECTOR, num_items, items, ON) ;
      if ((num_items EQUALS 1) AND (num_objs > 0))
      {
	 obj_id = obj_ids[items[0]-1] ;
	 wmoinqtyp( obj_id, &obj_typ ) ;
      }
      else
      {
	 obj_id = 0 ;
	 obj_typ = 0 ;
      }
   }
   if ( obj_id AND (obj_typ EQUALS WM_CLASS_WINDOW) )
   {
      wmwfront( obj_id ) ;
      wm_grsgwk( MAIN_WKID ) ;
   }
   else
   {
      if ((num_items <= 0) OR (num_objs <= 0))
      {
	 curr_error = WM_ERROR_NOTHING_SELECTED ;
	 do_update = TRUE ;
      }
      else
      {
	 curr_error = WM_ERROR_MORE_THAN_ONE_SELECTED ;
	 do_update = TRUE ;
      }
   }
   wmoattach( app, win, tnr, gro, &status ) ;
}

/* === ZZ_RESIZE === */

static VOID zz_resize()

{
   INT num_items, items[MAX_SELECTABLE], obj_id, obj_typ ;
   APPLICATION_ID app;
   WINDOW_ID win;
   TNR_ID tnr;
   GROUP_ID gro;
   INT status;

/* if there is a window open, use it */
   wmowhere( &app, &win, &tnr, &gro, &status ) ;
   if (win)
   {
      num_items = 1 ;
      obj_id = win ;
      obj_typ = WM_CLASS_WINDOW ;
      wmoattach( app, 0, 0, 0, &status ) ;
   }
   else
   {
/* otherwise check selector */
      wiqsel( wc_id, ITEM_WC_SELECTOR, &num_items, items ) ;
      if ((num_items EQUALS 1) AND (num_objs > 0))
      {
/* Reset the selected values... */
	 wi_set_defaults( wc_id, ITEM_WC_SELECTOR, num_items, items, ON) ;
	 obj_id = obj_ids[items[0]-1] ;
	 wmoinqtyp( obj_id, &obj_typ ) ;
      }
      else
      {
	 obj_id = 0 ;
	 obj_typ = 0 ;
      }
   }
   if ( obj_id AND (obj_typ EQUALS WM_CLASS_WINDOW) )
   {
      curr_prompt = WM_PROMPT_RESIZE ;
      zz_update() ;
      if (ug_if_gpx() == FALSE)
	 wm_attach_window_screen( obj_id );     /* associate wk-station screen */
      wmwband( obj_id ) ;
      wm_grsgwk( MAIN_WKID ) ;
   }
   else
   {
      if ((num_items <= 0) OR (num_objs <= 0))
      {
	 curr_error = WM_ERROR_NOTHING_SELECTED ;
	 do_update = TRUE ;
      }
      else
      {
	 curr_error = WM_ERROR_MORE_THAN_ONE_SELECTED ;
	 do_update = TRUE ;
      }
   }
   wmoattach( app, win, tnr, gro, &status ) ;
}

/* === ZZ_SCREEN === */

static VOID zz_screen(screen_number)
INT screen_number;

{
   INT num_items, items[MAX_SELECTABLE], obj_id, obj_typ ;
   APPLICATION_ID app;
   WINDOW_ID win;
   TNR_ID tnr;
   GROUP_ID gro;
   INT status;

if (ug_if_gpx() == FALSE)
 {
/* if there is a window open, use it */
   wmowhere( &app, &win, &tnr, &gro, &status ) ;
   if (win)
   {
      num_items = 1 ;
      obj_id = win ;
      obj_typ = WM_CLASS_WINDOW ;
   }
   else
   {
/* otherwise check selector */
      wiqsel( wc_id, ITEM_WC_SELECTOR, &num_items, items ) ;
      if ((num_items EQUALS 1) AND (num_objs > 0))
      {
/* Reset the selected values... */
	 wi_set_defaults( wc_id, ITEM_WC_SELECTOR, num_items, items, ON) ;
	 obj_id = obj_ids[items[0]-1] ;
	 wmoinqtyp( obj_id, &obj_typ ) ;
	 if (obj_typ == WM_CLASS_WINDOW)
	 {
	    wmoattach( app, obj_id, 0, 0, &status );
	 }
      }
      else
      {
	 obj_id = 0 ;
	 obj_typ = 0 ;
      }
   }
   if ( obj_id AND (obj_typ EQUALS WM_CLASS_WINDOW) )
   {
      wm_set_window_screen( obj_id, screen_number ) ;
      wm_grsgwk( MAIN_WKID ) ;
   }
   else
   {
      if ((num_items <= 0) OR (num_objs <= 0))
      {
	 curr_error = WM_ERROR_NOTHING_SELECTED ;
	 do_update = TRUE ;
      }
      else
      {
	 curr_error = WM_ERROR_MORE_THAN_ONE_SELECTED ;
	 do_update = TRUE ;
      }
   }
   wmoattach( app, win, tnr, gro, &status ) ;
 }
}


/* === WMWSELECTOR === */

publicdef INT wmwselector( prompt, class, max_sel, num_sel, win_ids, status )

CHAR *prompt ;
INT class, max_sel ;
INT *num_sel, win_ids[], *status ;

{
   INT returned_status = SUCCESS ;    /* status returned from function */
   INT curr_app, curr_win, curr_tnr, curr_gro, curr_seg, curr_pic ;
   INT i ;
   LISTHEAD win_list ;

   wmoinqcur( &curr_app, &curr_win, &curr_tnr, &curr_gro, &curr_seg, &curr_pic ) ;
   if ((curr_app EQUALS 0) OR (curr_win IS_NOT_EQUAL_TO 0))
   {
      *num_sel = 0 ;
      *status = WM_ERR_WI_CANCEL ;
      returned_status = WM_ERR_WI_CANCEL ;
      return returned_status ;
   }

   num_objs = 0 ;
   llinit( &win_list, 0, sizeof(INT), 0 ) ;
   wmwinqlist( class, &win_list ) ;
   zz_append( &win_list, FOLDER_ICON ) ;
   llfree( &win_list ) ;

   if (num_objs > 0)
   {
      wissst( HELP_FILE, num_objs, max_sel, obj_strs ) ;
      wirqss( prompt, num_sel, win_ids, status ) ;
      if (*status EQUALS SUCCESS)
      {
	 for (i = 0; i < *num_sel; i++)
	 {
	    win_ids[i] = obj_ids[win_ids[i]-1] ;
	 }
      }
   }
   else
   {
      *num_sel = 0 ;
      *status =  WM_ERR_WI_CANCEL ;
      returned_status = WM_ERR_WI_CANCEL ;
   }
   return returned_status ;
}


/* === WMTSELECTOR === */

publicdef INT wmtselector( prompt, class, max_sel, num_sel, tnr_ids, status )

CHAR *prompt ;
INT class, max_sel ;
INT *num_sel, tnr_ids[], *status ;

{
   INT curr_app, curr_win, curr_tnr, curr_gro, curr_seg, curr_pic ;
   INT i ;
   LISTHEAD tnr_list ;

   wmoinqcur( &curr_app, &curr_win, &curr_tnr, &curr_gro, &curr_seg, &curr_pic ) ;
   if ((curr_app EQUALS 0) OR (curr_win EQUALS 0))
   {
      *num_sel = 0 ;
      return *status =  WM_ERR_WI_CANCEL ;
   }

    num_objs = 0 ;
    llinit( &tnr_list, 0, sizeof(INT), 0 ) ;
    wmtinqlist( class, &tnr_list ) ;
    zz_append( &tnr_list, NO_ICON ) ;
    llfree( &tnr_list ) ;

    if (num_objs > 0)
	{
/* In the case where there are only 2 transforms, the first one
will be the window to ndc transform, and the second one will be
the application wcs transform.  Take the second one... */

	if (num_objs EQUALS 2)
	    {
	    tnr_ids[0] = obj_ids[1];
	    *num_sel = 1;
	    return *status = SUCCESS;
	    }
	else
	    {
	    wissst( HELP_FILE, num_objs, max_sel, obj_strs ) ;
	    wirqss( prompt, num_sel, tnr_ids, status ) ;
	    if (*status EQUALS SUCCESS)
		{
		for (i = 0; i < *num_sel; i++)
		    {
		    tnr_ids[i] = obj_ids[tnr_ids[i]-1] ;
		    }
		}
	    }
	}
    else
	{
	*num_sel = 0 ;
	return *status = WM_ERR_WI_CANCEL ;
	}

    return *status;

}

/* === WMWBAND === */

publicdef INT wmwband( win_id )

INT win_id ;

{
   INT returned_status = SUCCESS ;
   INT wkid = 1,locator_device = 1;
   INT class, max_sel, num_sel, win_ids[1], status ;
   INT done, button, color, ctrl, tnr ;
   FLOAT oldMinX, oldMinY, oldMaxX, oldMaxY ;
   FLOAT newMinX, newMinY, newMaxX, newMaxY ;
   FLOAT oldDX, oldDY, wx, wy ;
   CHAR prompt[STMAX];

   wiz_jp_name("wmwband");

   if (!(win_id))
   {
      class = WM_CLASS_WINDOW ;
      max_sel = 1 ;
      num_sel = 0 ;
      win_ids[0] = 0 ;
      status = 0 ;
      strcpy(prompt, mg_message(WM_SELECT_BAND_WINDOW));
      wmwselector(prompt , class, max_sel, &num_sel, win_ids, &status ) ;
      if ((status EQUALS SUCCESS) AND (num_sel EQUALS 1))
      {
	 win_id = win_ids[0] ;
      }
   }
   if (win_id)
   {
      wmwinq( win_id, &oldMinX, &oldMaxX, &oldMinY, &oldMaxY, &color ) ;
      newMinX = oldMinX ;
      newMaxX = oldMaxX ;
      newMinY = oldMinY ;
      newMaxY = oldMaxY ;
      done = FALSE ;
      lu_text_to_index( "COLOR", &color, "NORMAL" );
      do
      {
	 wm_gselnt( 0 ) ;
	 oldDX = oldMaxX - oldMinX ;
	 oldDY = oldMaxY - oldMinY ;
	 zz_draw_ctrls( oldMinX, oldMinY, oldMaxX, oldMaxY, color ) ;

	 if (wiv.jpmode == WI_PLAYBACK)  {
	    wiz_jp_read_long(&status);
	    wiz_jp_read_long(&tnr);
	    wiz_jp_read_long(&button);
	    wiz_jp_read_float(&wx);
	    wiz_jp_read_float(&wy);
	 }
	 else {

/* request x,y position and button number from GKS */
	    wm_grqlc( wkid, locator_device, &status, &tnr, &wx, &wy);
	    wm_gkzqbtn( &button ) ;

	    if (wiv.jpmode == WI_JOURNAL)  {
	       wiz_jp_write_long(status);
	       wiz_jp_write_long(tnr);
	       wiz_jp_write_long(button);
	       wiz_jp_write_float(wx);
	       wiz_jp_write_float(wy);
	    }
	 }

	 if ((status EQUALS GOK) AND (button IS_NOT_EQUAL_TO 13))
	 {
	    if (tnr IS_NOT_EQUAL_TO 0)
	    {
	       tg_tnr2tnr( tnr, wx, wy, 0, &wx, &wy ) ;
	    }
	    ctrl = zz_in_ctrl(oldMinX, oldMinY, oldMaxX, oldMaxY, wx, wy ) ;
	    switch (ctrl)
	    {
	       case LL_CTRL:
	       {
		  zz_band( &status, oldMaxX, oldMaxY, &wx, &wy, &button ) ;
		  if (status EQUALS SUCCESS)
		  {
		     newMinX = MIN(oldMaxX, wx) ;
		     newMinY = MIN(oldMaxY, wy) ;
		     newMaxX = MAX(oldMaxX, wx) ;
		     newMaxY = MAX(oldMaxY, wy) ;
		  }
		  break ;
	       }
	       case LR_CTRL:
	       {
		  zz_band( &status, oldMinX, oldMaxY, &wx, &wy, &button ) ;
		  if (status EQUALS SUCCESS)
		  {
		     newMinX = MIN(oldMinX, wx) ;
		     newMinY = MIN(oldMaxY, wy) ;
		     newMaxX = MAX(oldMinX, wx) ;
		     newMaxY = MAX(oldMaxY, wy) ;
		  }
		  break ;
	       }
	       case UR_CTRL:
	       {
		  zz_band( &status, oldMinX, oldMinY, &wx, &wy, &button ) ;
		  if (status EQUALS SUCCESS)
		  {
		     newMinX = MIN(oldMinX, wx) ;
		     newMinY = MIN(oldMinY, wy) ;
		     newMaxX = MAX(oldMinX, wx) ;
		     newMaxY = MAX(oldMinY, wy) ;
		  }
		  break ;
	       }
	       case UL_CTRL:
	       {
		  zz_band( &status, oldMaxX, oldMinY, &wx, &wy, &button ) ;
		  if (status EQUALS SUCCESS)
		  {
		     newMinX = MIN(oldMaxX, wx) ;
		     newMinY = MIN(oldMinY, wy) ;
		     newMaxX = MAX(oldMaxX, wx) ;
		     newMaxY = MAX(oldMinY, wy) ;
		  }
		  break ;
	       }
	       case IN_CTRL:
	       {
		  zz_drag( &status, oldMinX, oldMinY, oldMaxX, oldMaxY, &wx, &wy, &button ) ;
/*
		  wm_grsgwk(MAIN_WKID) ;
*/
		  if (status EQUALS SUCCESS)
		  {
		     newMinX = wx ;
		     newMinY = wy ;
		     newMaxX = newMinX + oldDX ;
		     newMaxY = newMinY + oldDY ;
		     if (newMinX < 0.0)
		     {
			newMinX = 0.0 ; newMaxX = oldDX ;
		     }
		     if (newMaxX > 1.0)
		     {
			newMaxX = 1.0 ; newMinX = 1.0 - oldDX ;
		     }
		     if (newMinY < 0.0)
		     {
			newMinY = 0.0 ; newMaxY = oldDY ;
		     }
		     if (newMaxY > 1.0)
		     {
			newMaxY = 1.0 ; newMinY = 1.0 - oldDY ;
		     }
		  }
		  break ;
	       }
	       default:
	       {
	       }
	    }
 	    lu_text_to_index( "COLOR", &color, "BACKGROUND" );
	    zz_draw_ctrls( oldMinX, oldMinY, oldMaxX, oldMaxY, color ) ;
	    oldMinX = newMinX ;
	    oldMaxX = newMaxX ;
	    oldMinY = newMinY ;
	    oldMaxY = newMaxY ;
	 }
	 else
	 {
	    done = TRUE ;
	 }
      } while (!done) ;

      if( (status EQUALS GOK) AND (button EQUALS 13) )
	  {
	     wmwsetndc( win_id, newMinX, newMaxX, newMinY, newMaxY ) ;
	  }
   }
   return returned_status ;
}

static VOID zz_band( statPtr, wx1, wy1, wx2Ptr, wy2Ptr, butPtr )

INT *statPtr, *butPtr ;
DOUBLE wx1, wy1 ;
FLOAT *wx2Ptr, *wy2Ptr ;

{
   INT ugStatus, ugTnr, ugButton ;
   FLOAT ugWx1, ugWy1, ugWx2, ugWy2 ;

   wm_gselnt( 0 ) ;

   ugStatus = 0 ;
   ugWx1 = wx1 ;
   ugWy1 = wy1 ;
   ugTnr = 0 ;
   ugWx2 = 0.0 ;
   ugWy2 = 0.0 ;
   ugrqband_gc( UGRQ_TYPE_BAND_RECTANGLE, ugWx1, ugWy1, &ugStatus,
		&ugTnr, &ugWx2, &ugWy2, &ugButton ) ;
   if (ugStatus EQUALS SUCCESS)
   {
      if (ugTnr IS_NOT_EQUAL_TO 0)
      {
	 tg_tnr2tnr( ugTnr, ugWx2, ugWy2, 0, &ugWx2, &ugWy2 ) ;
      }
   }
   *statPtr = ugStatus ;
   *butPtr = ugButton ;
   *wx2Ptr = ugWx2 ;
   *wy2Ptr = ugWy2 ;
}

/* === ZZ_DRAG === */

static VOID zz_drag( statPtr, llx, lly, urx, ury, llxPtr, llyPtr, butPtr )

INT *statPtr, *butPtr ;
DOUBLE llx, lly ;
DOUBLE urx, ury ;
FLOAT *llxPtr, *llyPtr ;

{
   INT ugStatus, ugTnr, ugButton ;
   FLOAT ugWx1, ugWy1, ugWx2, ugWy2 ;
   FLOAT midX, midY ;
   GKS_INT TEMP_SEG, npts, coli, ltype ;
   GKS_REAL px[5], py[5];
   DOUBLE lwidth ;

   wm_gselnt( 0 ) ;

   TEMP_SEG = 2 ;
   wm_gcrsg( TEMP_SEG ) ;
   lu_text_to_index( "COLOR", &coli, "NORMAL" );
   wm_gsplci( coli ) ;
   lu_text_to_index( "LINE_STYLE", &ltype, "SOLID" );
   wm_gsln( ltype ) ;
   lwidth = 1.0   ; wm_gslwsc( lwidth ) ;
   npts = 5 ;
   px[0] = llx ; py[0] = lly ;
   px[1] = llx ; py[1] = ury ;
   px[2] = urx ; py[2] = ury ;
   px[3] = urx ; py[3] = lly ;
   px[4] = llx ; py[4] = lly ;
   wm_gpl( npts, px, py ) ;
   wm_gclsg() ;

   midX = (llx + urx) / 2.0 ;
   midY = (lly + ury) / 2.0 ;

   ugStatus = 0 ;
   ugWx1 = midX ;
   ugWy1 = midY ;
   ugTnr = 0 ;
   ugWx2 = midX ;
   ugWy2 = midY ;
   ugrqdrag_gc( TEMP_SEG, ugWx1, ugWy1, &ugStatus, &ugTnr, &ugWx2, &ugWy2,
		&ugButton ) ;
   if (ugStatus EQUALS SUCCESS)
   {
      if (ugTnr IS_NOT_EQUAL_TO 0)
      {
	 tg_tnr2tnr( ugTnr, ugWx2, ugWy2, 0, &ugWx2, &ugWy2 ) ;
      }
   }
   *statPtr = ugStatus ;
   *butPtr = ugButton ;
   *llxPtr = llx + (ugWx2 - midX) ;
   *llyPtr = lly + (ugWy2 - midY) ;

  wm_gdsg( TEMP_SEG ) ;
}

/* === ZZ_DRAW_CTRLS === */

static VOID zz_draw_ctrls( llx, lly, urx, ury, color )

DOUBLE llx, lly, urx, ury ;
INT color ;

{
   DOUBLE dx, dy, dd ;
   GKS_INT npts, faColor, ints, styli;
   GKS_REAL px[17], py[17] ;

   dx = ABS(urx - llx) ;
   dy = ABS(ury - lly) ;
   dd = CTRL_SIZE * MIN(dx, dy) ;

   npts = 17 ;
   px[0] = llx ; py[0] = lly ;
   px[1] = llx ; py[1] = ury ;
   px[2] = urx ; py[2] = ury ;
   px[3] = urx ; py[3] = lly ;
   px[4] = llx ; py[4] = lly ;

/* lower left */
   px[5] = llx    ; py[5] = lly+dd ;
   px[6] = llx+dd ; py[6] = lly+dd ;
   px[7] = llx+dd ; py[7] = lly ;

/* lower right */
   px[8] = urx-dd ; py[8] = lly ;
   px[9] = urx-dd ; py[9] = lly+dd ;
   px[10] = urx   ; py[10] = lly+dd ;

/* upper right */
   px[11] = urx    ; py[11] = ury-dd ;
   px[12] = urx-dd ; py[12] = ury-dd ;
   px[13] = urx-dd ; py[13] = ury ;

/* upper left */
   px[14] = llx+dd ; py[14] = ury ;
   px[15] = llx+dd ; py[15] = ury-dd ;
   px[16] = llx    ; py[16] = ury-dd ;

   ints    = SOLID ; wm_gsfais( ints ) ;
   styli   = 1     ; wm_gsfasi( styli ) ;
   faColor = color ; wm_gsfaci( faColor ) ;
   wm_gfa( npts, px, py ) ;
}

/* === ZZ_IN_CTRL === */

static INT zz_in_ctrl( llx, lly, urx, ury, wx, wy )

DOUBLE llx, lly, urx, ury, wx, wy ;

{
   DOUBLE dx, dy, dd ;
   POINT pt ;
   RECTANGLE rect ;

   dx = ABS(urx - llx) ;
   dy = ABS(ury - lly) ;
   dd = CTRL_SIZE * MIN(dx, dy) ;

   PR_SetPt( &pt, wx, wy ) ;

   if (PR_PtInRect( &pt, PR_SetRect( &rect, llx, lly, llx+dd, lly+dd ) ))
   {
      return (LL_CTRL) ;
   }

   if (PR_PtInRect( &pt, PR_SetRect( &rect, urx-dd, lly, urx, lly+dd ) ))
   {
      return (LR_CTRL) ;
   }

   if (PR_PtInRect( &pt, PR_SetRect( &rect, urx-dd, ury-dd, urx, ury ) ))
   {
      return (UR_CTRL) ;
   }

   if (PR_PtInRect( &pt, PR_SetRect( &rect, llx, ury-dd, llx+dd, ury ) ))
   {
      return (UL_CTRL) ;
   }

   if (PR_PtInRect( &pt, PR_SetRect( &rect, llx, lly, urx, ury ) ))
   {
      return (IN_CTRL) ;
   }

   return (NO_CTRL) ;
}

/* === ZZ_DELETE === */

static VOID zz_delete()

{
   INT num_items, items[MAX_SELECTABLE], obj_id, obj_typ, i ;
   INT ifyes, status ;
   CHAR prompt[STMAX] ;

   wiqsel( wc_id, ITEM_WC_SELECTOR, &num_items, items ) ;
   if ((num_items > 0) AND (num_objs > 0))
   {
      sprintf( prompt, "Delete %d item(s) from \"%s\"", num_items, curr_str ) ;
      wiyesno( prompt, HELP_FILE, &ifyes, &status ) ;
      if (!(ifyes))
      {
	 num_items = 0 ;
      }
   }

   if ((num_items > 0) AND (num_objs > 0))
   {
      for (i = 0; i < num_items; i++)
      {
	 obj_id = obj_ids[items[i]-1] ;
	 wmoinqtyp( obj_id, &obj_typ ) ;
	 switch (obj_typ)
	 {
	    case ( WM_CLASS_WINDOW ):
	    {
	       wmwdelete( obj_id ) ;
	       break ;
	    }
	    case ( WM_CLASS_GROUP ):
	    {
	       wmgdelete( obj_id ) ;
	       break ;
	    }
	    case ( WM_CLASS_SEGMENT ):
	    {
	       wmsdelete( obj_id ) ;
	       break ;
	    }
	    default:
	    {
	    }
	 }
      wm_grsgwk( MAIN_WKID ) ;
      }
      do_update = TRUE ;
      last_obj = -1 ;        /* force update of selector */
   }
   else
   {
      curr_error = WM_ERROR_NOTHING_SELECTED ;
      do_update = TRUE ;
   }
}

/* === ZZ_ZOOM === */

static VOID zz_zoom( if_in)

INT if_in ;

{
   INT num_items, items[MAX_SELECTABLE], obj_id, obj_typ ;
   INT app, win, tnr, gro, status ;

/* if there is a window open, use it */
   wmowhere( &app, &win, &tnr, &gro, &status ) ;
   if (win)
   {
      num_items = 1 ;
      obj_id = win ;
      obj_typ = WM_CLASS_WINDOW ;
      wmoattach( app, 0, 0, 0, &status ) ;
   }
   else
   {
/* otherwise check selector */
      wiqsel( wc_id, ITEM_WC_SELECTOR, &num_items, items ) ;
/* Reset the selected values... */
      wi_set_defaults( wc_id, ITEM_WC_SELECTOR, num_items, items, ON) ;
      if ((num_items EQUALS 1) AND (num_objs > 0))
      {
	 obj_id = obj_ids[items[0]-1] ;
	 wmoinqtyp( obj_id, &obj_typ ) ;
      }
      else
      {
	 obj_id = 0 ;
	 obj_typ = 0 ;
      }
   }
   if ( obj_id AND (obj_typ EQUALS WM_CLASS_WINDOW) )
   {
      wmwfront(obj_id);      /* front this window */
      /* NO NEED to make it is visible   */
      wmowhere( &app, &win, &tnr, &gro, &status ) ;/* change 'current' context              */
      if (if_in)
      {
	 /* NO NEED to Redraw workstation screen */
	 curr_prompt = WM_PROMPT_ZOOM_IN ;

	if (ug_if_gpx() == FALSE)
	    wm_attach_window_screen( obj_id );  /* associate wk-station screen */
	 zz_update() ;
      }
      wmwzoom( obj_id, if_in ) ;
      wm_grsgwk( MAIN_WKID ) ;
   }
   else
   {
      if ((num_items <= 0) OR (num_objs <= 0))
      {
	 curr_error = WM_ERROR_NOTHING_SELECTED ;
	 do_update = TRUE ;
      }
      else
      {
	 curr_error = WM_ERROR_MORE_THAN_ONE_SELECTED ;
	 do_update = TRUE ;
      }
   }
   wmoattach( app, win, tnr, gro, &status ) ;
}

static VOID zz_plot()
{
   INT num_items, items[MAX_SELECTABLE], obj_id, obj_typ, i ;
   INT ifyes, status ;
   CHAR prompt[STMAX] ;

   INT         meta_ws;         /* metafile workstation */
   WINDOW_ID   win_id;
   INT         app_id;
   TNR_ID      tnr_id;
   GROUP_ID    group_id;
   SEGMENT_ID  seg_id;
   INT         pic_id;
   DOUBLE      width;
   DOUBLE      height;
   CHAR        units[8];
   PATHNAME    metafile;   /* metafile name */
   PATHNAME    paramfile;   
   PDS         *pinfo = (PDS *) 0;
   CHAR        **tcp;
   FILENAME    selected_plotter;
   PATHNAME    cmd_file;
   CHAR        arg_list[512];
   GKS_REAL    ndc_min_x,ndc_min_y,ndc_max_x,ndc_max_y;
   DOUBLE      ndc_width,ndc_height;
   GKS_INT     color;
   
   wiqsel( wc_id, ITEM_WC_SELECTOR, &num_items, items ) ;
   if ((num_items > 0) AND (num_objs > 0))
     {
     sprintf( prompt, "Plot %d item(s) from \"%s\"", num_items, curr_str ) ;
     wiyesno( prompt, HELP_FILE, &ifyes, &status ) ;
     if (!(ifyes))
       {
       num_items = 0 ;
       }
     else 
       {
       pinfo = (PDS *)tc_zalloc(sizeof(PDS));

       if (ho_translate_symbol("ESI$SELECTED_PLOTTER",&tcp) EQUALS SUCCESS)
         {
         strcpy(selected_plotter,tcp[0]);
         status = pf_inq_plotter(selected_plotter, pinfo);
         ts_tcp_free(tcp);
         }
       else
         {
         status = pf_get_plotter(pinfo);
         }
       }	  
     }

   if ((num_items > 0) AND (num_objs > 0) AND status == SUCCESS) 
     {
     for (i = 0; i < num_items; i++) 
       {
       obj_id = obj_ids[items[i]-1];
       wmoinqtyp( obj_id, &obj_typ );
       if (obj_typ == WM_CLASS_WINDOW) 
	 {
         printf ("Working on %s\n",obj_strs[items[i]-1]);
         ho_get_unique_filename(metafile);
         strcpy (paramfile,metafile);
         strcat (paramfile,".PAR");
         strcat (metafile,".");
	 strcat (metafile,pinfo->meta_type);
	 ho_add_path ("ESI$PLOTS",metafile,sizeof(PATHNAME),metafile);
         status = pf_file_init(&meta_ws,metafile);        /* initialize metafile */
	 if (status == SUCCESS)
           {
           pf_win_to_meta(meta_ws,obj_id);
           wm_get_window_size (obj_id,units,&width,&height);
	   /* ming: 8/16/91 */
	   /* pass the exact NDC window size to pf_close_file to put into */
	   /* parameter file                                              */
	   /* ----------------------------------------------------------- */
	   wmwinq (obj_id,&ndc_min_x,&ndc_max_x,&ndc_min_y,&ndc_max_y,&color);
	   ndc_width = ndc_max_x - ndc_min_x;
	   ndc_height = ndc_max_y - ndc_min_y;
           status = pf_file_close(meta_ws,metafile,units,width,height,ndc_width,ndc_height);
	   if (status == SUCCESS)
             {
             sprintf (arg_list, "1 %s %s %s",metafile,pinfo->driver,
					     pinfo->queue);
             ho_add_path ("ESI$BIN","pf_launch_plots",sizeof(FILENAME),cmd_file);
             ho_spawn(SPAWN_BATCH,cmd_file,arg_list,(INT *) 0);
	     }
           }			 
	 }
       }
/*   wm_grsgwk(MAIN_WKID); */
     do_update = TRUE ;
     last_obj = -1 ;        /* force update of selector */
     }
   else
     {
     curr_error = WM_ERROR_NOTHING_SELECTED ;
     do_update = TRUE ;
     }
	 
  if (pinfo != (PDS *)0)
    {
    free(pinfo);
    }

}
