/* DEC/CMS REPLACEMENT HISTORY, Element LE_NLIST_ED.C*/
/* *12    5-JUL-1991 11:22:33 KEE "(SPR 7598) Fix Electronic Drafting Line Editor PuckKey handling error, case 1893"*/
/* *11    3-MAY-1991 16:10:56 KEE "(SPR -1) MOTIF/PRIORGKS, MOTIF/DECWGKS, DECW/PRIORGKS, DECW/DECGKS version"*/
/* *10   16-OCT-1990 18:21:58 GILLESPIE "(SPR 1000) Merge Ernie deltas"*/
/*  7B3  15-OCT-1990 17:04:15 GILLESPIE "Merge Ernie deltas"*/
/*  7B2   3-OCT-1990 12:20:13 GILLESPIE "Merge beta deltas"*/
/*  7B1  24-SEP-1990 16:45:01 GILLESPIE "Beta Deltas"*/
/* *9     4-SEP-1990 08:43:51 VINCE "(SPR 1) Restored after a mysterious disappearance"*/
/* *8    14-AUG-1990 13:34:53 VINCE "(SPR 5644) Header Standardization"*/
/* *7    26-FEB-1990 18:59:26 JULIAN "(SPR 6010) Port to Decstation 3100 (ULTRIX)"*/
/* *6     8-FEB-1990 15:20:29 GORDON "(SPR 0) Fix bad include line, bad reference to zz_dial_release"*/
/* *5    25-JAN-1990 09:50:18 ADEY "(SPR -1) fix keymap_first for use_keymap & int_dialog"*/
/* *4    13-NOV-1989 13:27:49 JULIAN "(SPR 1) Integrate external tablet functions into finder."*/
/* *3    25-SEP-1989 11:58:44 GORDON "(SPR 201) INT becomes UINT for snum & pnum"*/
/* *2    19-JUL-1989 07:36:28 GORDON "(SPR 100) Add mods from Sun port"*/
/* *1    19-JUN-1989 13:03:47 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element LE_NLIST_ED.C*/
/* DEC/CMS REPLACEMENT HISTORY, Element LE_NLIST_ED.C                               */
/* *4    14-OCT-1990 09:15:15 SCD "(SPR 0) Correct error in #if HOST==VMS / #ifdef USE_X"   */
/* *3    30-SEP-1990 10:25:04 SCD "(SPR 0) Clean up DECWindows exposure processing, insulate Raster code."  */
/* *2     9-SEP-1990 22:07:40 SCD "(SPR 0) Remove old MAC dialogs, redraw current line on expose events."   */
/* *1    13-AUG-1990 16:52:03 SYSTEM "Finder 6.1"                                   */
/* DEC/CMS REPLACEMENT HISTORY, Element LE_NLIST_ED.C                               */
/************************************************************************************/
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
#include "esi_math.h"
#include "esi_le_nled.h"

#ifndef ESI_LE_ERR_H
#include "esi_le_err.h"
#endif

#ifndef ESI_LE_MSG_H
#include "esi_le_msg.h"
#endif

#ifndef ESI_AM_H
#include "esi_am.h"
#endif

#ifndef ESI_GKS_H
#include "esi_gks.h"
#endif

#ifndef ESI_HO_H
#include "esi_ho.h"
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

#ifndef ESI_WI_H
#include "esi_wi.h"
#endif

#ifndef ESI_WM_INT_H
#include "esi_wm_int.h"
#endif

#ifndef ESI_WM_H
#include "esi_wm.h"
#endif

#include "le_dialogs.rh"

/* -------------------------------------------                                      */
/* The following header files are only used to                                      */
/* support the DEC GKS hack to redraw the                                           */
/* nlist upon an expose event. NOTE that this                                       */
/* method is NOT needed for Prior GKS                                               */
/* -------------------------------------------                                      */
#ifdef vms
#ifdef USE_X
#include "ansixt.h"
#endif
#endif

/*                                                                                  */
/*   definition of the pointers to array of pointers pointing to current  keymaps   */
/*   the storage for current keymaps is allocated dynamically at run time           */
/*                                                                                  */

/*   number of default stroke keymaps in NUM_DEF_STROKE_KEYM                        */
#define NUM_DEF_STROKE_KEYM   1

/* number of default point keymaps is NUM_DEF_POINT_KEYM                            */
#define NUM_DEF_POINT_KEYM    1

#if 0
/*  INITIALIZATION OF DEFAULT STROKE KEYMAPS                                        */
static LE_KEYMAP def_stroke_keym[] = 
    {
    BUTTON_UNDEFINED, le_undefined, "Undefined", FALSE, 0, 0X0, FALSE,
    BUTTON_START_STROKE, le_start_stroke, "Start New Segment", TRUE, 0, 0X1, TRUE,
    BUTTON_DELETE_STROKE, le_delete_stroke, "Delete Segment", TRUE, 0, 0X2, TRUE,
    BUTTON_REPLACE_STROKE, le_replace_stroke, "Replace Segment", TRUE, 0, 0X3, TRUE,
    BUTTON_EXTEND_STROKE, le_extend_stroke, "Extend Segment", TRUE, 0, 0X4, TRUE,
    BUTTON_CLEAVE_STROKE, le_cleave_stroke, "Cleave Segment", TRUE, 0, 0X5, TRUE,
    BUTTON_JOIN_STROKE, le_join_stroke, "Join Segments", TRUE, 0, 0X6, TRUE,
    BUTTON_PAN, le_pan, "Pan Window", TRUE, 0, 0X7, FALSE, BUTTON_UNDO,
    NULL_FUNCTION_PTR, "Undo", TRUE, 0, 0X8, FALSE, BUTTON_MOVE_POINT, le_move_point,
    "Move Point ", TRUE, 0, 0X9, TRUE, BUTTON_ZOOMIN, le_zoomin, "Zoom In Window",
    TRUE, 0, 0XA, FALSE, BUTTON_ZOOMOUT, le_zoomout, "Zoom Out Window", TRUE, 0, 0XB,
    FALSE, BUTTON_CANCEL, le_cancel, "Cancel", TRUE, 0, 0XC, FALSE, BUTTON_DONE,
    le_done, "Exit", TRUE, 0, 0XD, FALSE, BUTTON_SET_PARAM, le_set_param,
    "Set Preferences", FALSE, 0, 0XE, TRUE, BUTTON_RESET, le_reset, "Reset Window",
    TRUE, 0, 0XF, FALSE
    };



/*   INITIALIZATION OF DEFAULT POINT KEYMAP                                         */
static LE_KEYMAP def_point_keym[] = 
    {
    BUTTON_UNDEFINED, le_undefined, "Undefined", FALSE, 0, 0X0, FALSE,
    BUTTON_ADD_POINT, le_add_point, "Add Point", TRUE, 0, 0X1, FALSE,
    BUTTON_BACK_POINT, le_back_point, "Delete Point", TRUE, 0, 0X2, FALSE,
    BUTTON_ADD_MONO, le_add_point_mono, "Add Monotonic", TRUE, 0, 0X3, FALSE,
    BUTTON_BREAK_STROKE, le_start_stroke_pt_mode, "Start New Segment", TRUE, 0, 0X4,
    TRUE, BUTTON_UNDEFINED, le_undefined, "Undefined", FALSE, 0, 0X5, FALSE,
    BUTTON_UNDEFINED, le_undefined, "Undefined", FALSE, 0, 0X6, FALSE, BUTTON_PAN,
    le_pan, "Pan Window", TRUE, 0, 0X7, FALSE, BUTTON_UNDEFINED, le_undefined,
    "Undefined", FALSE, 0, 0X8, FALSE, BUTTON_UNDEFINED, le_undefined, "Undefined",
    FALSE, 0, 0X9, FALSE, BUTTON_ZOOMIN, le_zoomin, "Zoom In Window", TRUE, 0, 0XA,
    FALSE, BUTTON_ZOOMOUT, le_zoomout, "Zoom Out Window", TRUE, 0, 0XB, FALSE,
    BUTTON_CANCEL, le_cancel, "Cancel", TRUE, 0, 0XC, FALSE, BUTTON_DONE, le_done,
    "Return to Line Mode", TRUE, 0, 0XD, FALSE, BUTTON_SET_PARAM, le_set_param,
    "Set Parameters", FALSE, 0, 0XE, TRUE, BUTTON_RESET, le_reset, "Reset Window",
    TRUE, 0, 0XF, FALSE
    };



#else

/*  INITIALIZATION OF DEFAULT STROKE KEYMAPS                                        */
static LE_KEYMAP def_stroke_keym[] = 
    {
    BUTTON_UNDEFINED, le_undefined, "Undefined", FALSE, 0, 0X0, FALSE,
    BUTTON_START_STROKE, le_start_stroke, "Start New Segment", TRUE, 0, 0X1, TRUE,
    BUTTON_DELETE_STROKE, le_delete_stroke, "Delete Segment", TRUE, 0, 0X2, TRUE,
    BUTTON_REPLACE_STROKE, le_replace_stroke, "Replace Segment", TRUE, 0, 0X3, TRUE,
    BUTTON_EXTEND_STROKE, le_extend_stroke, "Extend Segment", TRUE, 0, 0X4, TRUE,
    BUTTON_CLEAVE_STROKE, le_cleave_stroke, "Cleave Segment", TRUE, 0, 0X5, TRUE,
    BUTTON_JOIN_STROKE, le_join_stroke, "Join Segments", TRUE, 0, 0X6, TRUE,
    BUTTON_UNDEFINED, NULL_FUNCTION_PTR, "Undefined", FALSE, 0, 0X7, FALSE,
    BUTTON_UNDO, NULL_FUNCTION_PTR, "Undo", FALSE, 0, 0X8, FALSE, BUTTON_MOVE_POINT,
    le_move_point, "Move Point ", TRUE, 0, 0X9, TRUE, BUTTON_ZOOMIN, le_zoomin,
    "Zoom In", TRUE, 0, 0XA, FALSE, BUTTON_PAN, le_pan, "Pan", TRUE, 0, 0XB, FALSE,
    BUTTON_CANCEL, le_cancel, "Cancel", TRUE, 0, 0XC, FALSE, BUTTON_DONE, le_done,
    "Exit", TRUE, 0, 0XD, FALSE, BUTTON_RESET, le_reset, "Zoom Out", TRUE, 0, 0XE,
    FALSE, BUTTON_UNDEFINED, NULL_FUNCTION_PTR, "Undefined", FALSE, 0, 0XF, FALSE
    };



/*   INITIALIZATION OF DEFAULT POINT KEYMAP                                         */
static LE_KEYMAP def_point_keym[] = 
    {
    BUTTON_UNDEFINED, le_undefined, "Undefined", FALSE, 0, 0X0, FALSE,
    BUTTON_ADD_POINT, le_add_point, "Add Point", TRUE, 0, 0X1, FALSE,
    BUTTON_BACK_POINT, le_back_point, "Delete Point", TRUE, 0, 0X2, FALSE,
    BUTTON_ADD_MONO, le_add_point_mono, "Add Monotonic", TRUE, 0, 0X3, FALSE,
    BUTTON_BREAK_STROKE, le_start_stroke_pt_mode, "Start New Segment", TRUE, 0, 0X4, TRUE,
    BUTTON_UNDEFINED, le_undefined, "Undefined", FALSE, 0, 0X5, FALSE,
    BUTTON_UNDEFINED, le_undefined, "Undefined", FALSE, 0, 0X6, FALSE,
    BUTTON_UNDEFINED, NULL_FUNCTION_PTR, "Undefined", FALSE, 0, 0X7, FALSE,
    BUTTON_UNDEFINED, le_undefined, "Undefined", FALSE, 0, 0X8, FALSE,
    BUTTON_UNDEFINED, le_undefined, "Undefined", FALSE, 0, 0X9, FALSE,
    BUTTON_ZOOMIN, le_zoomin, "Zoom In", TRUE, 0, 0XA, FALSE,
    BUTTON_PAN, le_pan, "Pan", TRUE, 0, 0XB, FALSE,
    BUTTON_CANCEL, le_cancel, "Cancel", TRUE, 0, 0XC, FALSE,
    BUTTON_DONE, le_done, "Return to Line Mode", TRUE, 0, 0XD, FALSE,
    BUTTON_RESET, le_reset, "Zoom Out", TRUE, 0, 0XE, FALSE,
    BUTTON_UNDEFINED, NULL_FUNCTION_PTR, "Undefined", FALSE, 0, 0XF, FALSE
    };


#endif
static LE_KEYS TheNullKey = 
    {
    BUTTON_UNDEFINED, le_undefined, "Undefined", FALSE, 0, 0, FALSE
    };

#define  DIALOG_FILE "LE_DIALOGS"
#define  NDIM  (UINT)2
static CHAR *NULL_STRING = "";
static UINT return_p;
static INT DataType[NDIM] = 
    {
    FLOAT_TYPE, FLOAT_TYPE
    };

static size_t Width[NDIM] = 
    {
    sizeof(FLOAT), sizeof(FLOAT)
    };

static INT Units[NDIM] = 
    {
    0, 0
    };

static VOIDPTR Var_List[NDIM];
static UINT Col_List[NDIM] = 
    {
    1, 2
    };

static FLOAT ValueList[NDIM] = 
    {
    0.0, 0.0
    };

static INT option_array[] = 
    {
    BUTTON_CANCEL, BUTTON_DONE, BUTTON_PAN, BUTTON_ZOOMIN, BUTTON_ZOOMOUT,
    BUTTON_RESET, BUTTON_REPLACE_STROKE
    };
static INT option_array1[] = 
    {
    BUTTON_CANCEL, BUTTON_DONE, BUTTON_PAN, BUTTON_ZOOMIN, BUTTON_ZOOMOUT,
    BUTTON_RESET, BUTTON_MOVE_POINT
    };
static INT option_array2[] = 
    {
    BUTTON_CANCEL, BUTTON_DONE, BUTTON_PAN, BUTTON_ZOOMIN, BUTTON_ZOOMOUT,
    BUTTON_RESET, BUTTON_JOIN_STROKE
    };

static INT option_array3[] = 
    {
    BUTTON_CANCEL, BUTTON_DONE, BUTTON_PAN, BUTTON_ZOOMIN, BUTTON_ZOOMOUT,
    BUTTON_RESET, BUTTON_CLEAVE_STROKE
    };
	
/*   array containing the id of dialog associated with stroke keymaps               */

#define MAX_KEYMAPS 30
static INT cur_stroke_id[MAX_KEYMAPS];
static INT cur_point_id[MAX_KEYMAPS];

static TCP cur_stroke_menus[MAX_KEYMAPS];    /* For GPX menu choice only.    */
static INT *cur_stroke_maps[MAX_KEYMAPS];

static TCP cur_point_menus[MAX_KEYMAPS]; /* For GPX menu choice only.        */
static INT *cur_point_maps[MAX_KEYMAPS];

/*                                                                                  */
/*   * define the dialog items into an array.  We will then be able to              */
/*   * turn on/off items by function rather than by absolute addressing             */
/*                                                                                  */

static INT dlg_item[MAX_KEYS][2] = 
    {
    0, 0, ITEM_LEPKM_BUTTON_1, ITEM_LEPKM_STATIC_1, ITEM_LEPKM_BUTTON_2,
    ITEM_LEPKM_STATIC_2, ITEM_LEPKM_BUTTON_3, ITEM_LEPKM_STATIC_3, ITEM_LEPKM_BUTTON_4,
    ITEM_LEPKM_STATIC_4, ITEM_LEPKM_BUTTON_5, ITEM_LEPKM_STATIC_5, ITEM_LEPKM_BUTTON_6,
    ITEM_LEPKM_STATIC_6, ITEM_LEPKM_BUTTON_7, ITEM_LEPKM_STATIC_7, ITEM_LEPKM_BUTTON_8,
    ITEM_LEPKM_STATIC_8, ITEM_LEPKM_BUTTON_9, ITEM_LEPKM_STATIC_9, ITEM_LEPKM_BUTTON_A,
    ITEM_LEPKM_STATIC_A, ITEM_LEPKM_BUTTON_B, ITEM_LEPKM_STATIC_B, ITEM_LEPKM_BUTTON_C,
    ITEM_LEPKM_STATIC_C, ITEM_LEPKM_BUTTON_D, ITEM_LEPKM_STATIC_D, ITEM_LEPKM_BUTTON_E,
    ITEM_LEPKM_STATIC_E, ITEM_LEPKM_BUTTON_F, ITEM_LEPKM_STATIC_F
    };

static BOOL if_gpx;             /* We are using real workstation (NOT RASTER)   */
static BOOL if_tablet = FALSE;  /* The picking/digitizing input is tablet.      */
static BOOL if_mouse_menus = FALSE; /* Real workstation, using pop-up or tool pallete   */

static INT curr_err = 0;
static BOOL do_update = 0;

static FLOAT dx = 0.05;         /* used for tolerance on pick window            */
static FLOAT dy = 0.05;         /* used for tolerance on pick window            */

/* -----------------------------------------------                                  */
/* Statics for DEC GKS hack to redraw on exposures                                  */
/* -----------------------------------------------                                  */
static NLIST_HEADER lez_current_nlist;
static LINE_ATTRIBUTE *lez_current_attribute;

/* Macros                                                                           */

#define FREE_LINE( line ) \
    nl_free_nlist( line ),\
    line = (NLIST_HEADER) 0
#define ADD_POINT( line, x, y ) \
    Var_List[0] = (VOIDPTR)&(x),\
    Var_List[1] = (VOIDPTR)&(y),\
    nl_add_points_convert(line, (UINT)1, NDIM, Col_List, Var_List, \
                          Units, Width, DataType)
        
#define DELETE_POINT( line ) \
    nl_delete_points( line, (UINT)1 )
#define COUNT_POINTS( line, count ) \
    nl_inq_nlist_int( line, NL_NLIST_NPOINTS, (INT *)count )

#define GET_CURRENT_XY( line, x, y ) \
    Var_List[0] = (VOIDPTR)(x),\
    Var_List[1] = (VOIDPTR)(y),\
    nl_get_rows_convert( line, (UINT)1, NDIM, Col_List, Var_List,\
                           Units, Width, DataType, &return_p )

#define COPY_STROKE( line, stroke1, stroke2) \
    nl_copy_table( line, (UINT)(stroke1), line, (UINT *)(stroke2))

#define DEL_PART_OF_STROKE( line, stroke, cur_point, nbpoints) \
    nl_set_current_row( line, (UINT)(cur_point), (UINT)(stroke)), \
    nl_delete_rows( line, (UINT)(nbpoints))

#define COUNT_STROKE_POINTS(line, stroke, number) \
    nl_inq_table_int( line, (UINT)(stroke), NL_STROKE_NPOINTS, (INT *)(number))

#define ADD_STROKE(line1, stroke1,start1, line2, stroke2, count, start2)\
    for ((i) =(start1), (j) = (start2);(i) <  (count) + (start1); (i)++, (j)++) \
    {\
        nl_set_current_row(line1, (UINT)(i), (UINT)(stroke1));\
        GET_CURRENT_XY(line1, &x, &y);\
        nl_set_current_row(line2, (UINT)(j), (UINT)(stroke2));\
        ADD_POINT(line2, x, y);\
    }

#define ADD_REVERSE_STROKE(line1, stroke1, start1, line2, stroke2, count, start2) \
    for ((i) = (start1), (j) = (start2); (i) > (start1) - (count); (i)--, (j)++) \
    {\
        nl_set_current_point(line1, (UINT)(i), (UINT)(stroke1));\
        GET_CURRENT_XY(line1, &x, &y);\
        nl_set_current_row(line2, (UINT)(j), (UINT)(stroke2));\
        ADD_POINT(line2, x, y);\
    }

#define MOVE_POINT( line, x, y ) \
    Var_List[0] = (VOIDPTR)&(x),\
    Var_List[1] = (VOIDPTR)&(y),\
    nl_replace_points_convert(line, (UINT)1, (UINT)NDIM, Col_List, Var_List, Units, Width, DataType)


/* Internal routines                                                                */

#if USE_PROTOTYPES
static VOID lez_change_key_label (LE_KEYS **pdone, LE_KEYS **pcancel,
                                      LE_CONTROL *pctrl, INT index_prompt);
static VOID lez_control_draw_vector (NLIST_HEADER line, DOUBLE wx1, DOUBLE wy1,
                                         DOUBLE wx2, DOUBLE wy2,
                                         LINE_ATTRIBUTE *attribute);
static VOID lez_control_draw_point (NLIST_HEADER line, DOUBLE wx, DOUBLE wy,
                                        LINE_ATTRIBUTE *attribute);
static VOID lez_control_nonull (NLIST_HEADER line, DOUBLE wx1, DOUBLE wy1,
                                    DOUBLE wx2, DOUBLE wy2, INT nbp, BOOL *exist_null);
static VOID lez_control_but (INT *stat, DOUBLE lastx, DOUBLE lasty, GKS_REAL *wx,
                                 GKS_REAL *wy, INT *but, INT nbp, LE_CONTROL *pctrl,
                                 INT *option_array, INT nb_option, INT *oper,
                                 UINT stroke_picked, UINT first_point,
                                 UINT second_point);
static VOID lez_del_stroke (NLIST_HEADER line, UINT stroke,
                                LINE_ATTRIBUTE *erase_attribute);
static VOID lez_delete_null_strokes (NLIST_HEADER line);
static VOID lez_draw_line (NLIST_HEADER line, LINE_ATTRIBUTE *attribute);
static VOID lez_draw_point (DOUBLE wx, DOUBLE wy, LINE_ATTRIBUTE *attribute);
static VOID lez_draw_stroke (NLIST_HEADER line, UINT stroke,
                                 LINE_ATTRIBUTE *attribute);
static VOID lez_draw_vector (DOUBLE wx1, DOUBLE wy1, DOUBLE wx2, DOUBLE wy2,
                                 LINE_ATTRIBUTE *attribute);
static VOID lez_draw_pl (GKS_REAL *px, GKS_REAL *py, UINT npts,
                             LINE_ATTRIBUTE *attribute);
static VOID lez_enable_option (LE_CONTROL *pctrl, INT *option_array, INT nb_option,
                                   BOOL enable);
static VOID lez_find_stroke_end (LE_CONTROL *pctrl, DOUBLE wx, DOUBLE wy,
                                     UINT *stroke, UINT *point);
static VOID lez_find_stroke (LE_CONTROL *pctrl, DOUBLE wx, DOUBLE wy, UINT *stroke,
                                 UINT *point);
static VOID lez_hilight_stroke (LE_CONTROL *pctrl);
static VOID lez_hilight_part (NLIST_HEADER line, UINT stroke, UINT first_pt,
                                  UINT second_pt, LINE_ATTRIBUTE *hilight);
static VOID lez_init (LE_PARLIST *ppar, LE_CONTROL *pctrl);
static VOID lez_init_choices (LE_KEYS *pkeymap, INT nmaps, INT *dialog_id_array,
                                  TCP *gpx_menus, INT *gpx_maps[], CHAR *title);
static VOID lez_init_window_mgr (LE_CONTROL *pctrl);
static LE_KEYS *lez_inq_key (LE_CONTROL *pctrl, INT operation);
static VOID lez_prompt (INT lepk_id, CHAR *descrip);
static VOID lez_redraw_line (void);
static VOID lez_restor_default_prompt (LE_KEYS *pcancel, LE_KEYS *pdone,
                                           LE_CONTROL *pctrl);
static VOID lez_rqbutxy (NLIST_HEADER line, INT *stat, DOUBLE lastx, DOUBLE lasty,
                             GKS_REAL *wx, GKS_REAL *wy, INT *but, INT nbp);
static VOID lez_save_list (LE_CONTROL *pctrl, LE_CONTROL *lastpctrl);
static VOID lez_sel_part (UINT *first_point, UINT *second_point,
                              UINT *stroke_picked, BOOL *noexit, UINT *cnt,
                              LE_CONTROL *pctrl, INT cur_oper, INT *option_array,
                              INT nb_optio);
static INT lez_setup_tablet_keypad (void);
static INT lez_set_keymap_cdlg (LE_KEYS *current_keymap, INT lepkm_id);
static VOID lez_swap_but (BOOL stroke_key, INT oper1, INT oper2,
                              LE_CONTROL *pctrl);
static VOID lez_term (LE_CONTROL *pctrl);
static LE_KEYS *lez_translate (INT button, LE_KEYS *current_keymap, INT indx);
static VOID lez_undo (LE_CONTROL **pctrl, LE_CONTROL **lastpctrl);
static VOID lez_update (INT lepk_id, BOOL default_puck_map);
static VOID lez_upd_key_prompt (INT lepk_id, INT button, CHAR *descript);

#else
static VOID lez_change_key_label ();
static VOID lez_control_draw_vector ();
static VOID lez_control_draw_point ();
static VOID lez_control_nonull ();
static VOID lez_control_but ();
static VOID lez_del_stroke ();
static VOID lez_delete_null_strokes ();
static VOID lez_draw_line ();
static VOID lez_draw_point ();
static VOID lez_draw_stroke ();
static VOID lez_draw_vector ();
static VOID lez_draw_pl ();
static VOID lez_enable_option ();
static VOID lez_find_stroke_end ();
static VOID lez_find_stroke ();
static VOID lez_hilight_stroke ();
static VOID lez_hilight_part ();
static VOID lez_init ();
static VOID lez_init_choices ();
static VOID lez_init_window_mgr ();
static LE_KEYS *lez_inq_key ();
static VOID lez_prompt ();
static VOID lez_restor_default_prompt ();
static VOID lez_rqbutxy ();
static VOID lez_save_list ();
static VOID lez_sel_part ();
static INT lez_setup_tablet_keypad ();
static INT lez_set_keymap_cdlg ();
static VOID lez_swap_but ();
static VOID lez_term ();
static LE_KEYS *lez_translate ();
static VOID lez_undo ();
static VOID lez_update ();
static VOID lez_upd_key_prompt ();

#endif
/* File Description ---------------------------------------------------------       */
/*Overview:                                                                         */
/*    New line editor for nlist stuff.                                              */
/*                                                                                  */
/*    START-HISTORY :                                                               */
/*    12/29/86 JOHN     delete stroke if npoints = 1 in le_backup_point             */
/*    12/29/86 JAMES    testing...                                                  */
/*    12/29/86 JAMES    modify le_infouser().                                       */
/*    12/19/86 JOHN     Make zoom,reset routines call BUTTON_REDRAW                 */
/*    12/11/86 JAMES    the best version till now.                                  */
/*    12/11/86 JAMES    adds LEZ_FIND_STROKE_END() routine.                         */
/*    12/10/86 JAMES    testing...                                                  */
/*    12/08/86 CGG CGG  REMI  :  correct bug bad argument passed to lez_draw_stroke in  */
/*            join_stroke                                                           */
/*    12/01/86 JOHN     made line_attribute structures rather than pointers         */
/*    12/01/86 CGG      add GKS drawing attributes                                  */
/*    12/01/86 JOHN     add dt to le_set_key and le_scan_keymap                     */
/*    11/26/86 CGG      add beep after each input at the puck key                   */
/*    11/21/86 CGG      debug join_stroke, start_stroke_pt_mode + clean code        */
/*    11/20/86 CGG      new release.                                                */
/*    11/19/86 CGG      fixed lez_find_stroke                                       */
/*            changed ctrl structure for new booleans                               */
/*            allow toggle of start stroke/add point button positions               */
/*    11/16/86 JAMES    Adds NULL value after calling nl_free_nlist() and nl_clear_nlist(). */
/*    11/16/86 CGG      fix move                                                    */
/*             change return status from WI_COMPLETE to SUCCESS and FAIL            */
/*    11/15/86 JOHN     added le_zoomin, le_zoomout, le_reset, le_pan               */
/*             disabled window manager button intercept                             */
/*             moved pan to button 7; set parms to button E                         */
/*             (change back when RT fixed)                                          */
/*    11/15/86 CGG      debugging case when no stroke in nlist                      */
/*             +  additionnal error message and prompt                              */
/*             +  Replace function can replace a complete segment                   */
/*             +  message confirm to exit or cancel the editor                      */
/*    11/13/86 CGG      suppress select stroke                                      */
/*             +simplification of colors                                            */
/*             +  optimisation of drawings                                          */
/*             +  corrections of bugs on cases with 1 or 0 point strokes            */
/*    11/11/86 CGG      stroke with less than 2 points not allowed                  */
/*             + changing default labels for keys                                   */
/*             + allows user to delete the first point of a segment                 */
/*    11/10/86 CGG      add application puck map display on mac                     */
/*    11/10/86 JAMES    modify le_extend_stroke().                                  */
/*    11/07/86 JOHN     minor changes to le_infouser                                */
/*    11/06/86 JAMES    Adds error checing for lez_FIND_STROKE().                   */
/*    11/03/86 JOHN     recompile;change le_infouser to be primary attach point from info   */
/*    11/03/86 JOHN     added le_infouser to this file - converted from pointlists  */
/*    11/03/86 JULIAN   Completed construction                                      */
/*    11/03/86 REMI     Template constructed                                        */
/*    END-HISTORY                                                                   */
/*                                                                                  */
/*Prototypes and one line descriptions of each function in the file.                */
/*Public_Functions:                                                                 */
/*    publicdef INT  le_nlist_editor (LE_PARLIST *ppar);                            */
/*                                                                                  */
/*    publicdef INT le_add_point (LE_CONTROL *pctrl);                               */
/*                                                                                  */
/*    publicdef INT le_add_point_mono (LE_CONTROL *pctrl);                          */
/*                                                                                  */
/*    publicdef INT le_back_point (LE_CONTROL *pctrl);                              */
/*                                                                                  */
/*    publicdef INT le_cancel (LE_CONTROL *pctrl);                                  */
/*                                                                                  */
/*    publicdef INT le_cleave_stroke (LE_CONTROL *pctrl);                           */
/*                                                                                  */
/*    publicdef INT le_delete_stroke (LE_CONTROL *pctrl);                           */
/*                                                                                  */
/*    publicdef INT le_done (LE_CONTROL *pctrl);                                    */
/*                                                                                  */
/*    publicdef INT le_extend_stroke (LE_CONTROL *pctrl);                           */
/*                                                                                  */
/*    publicdef INT le_gold_key (LE_CONTROL *pctrl);                                */
/*                                                                                  */
/*    publicdef INT le_hilight_pts(LE_CONTROL *pctrl);                              */
/*                                                                                  */
/*    publicdef INT le_join_stroke (LE_CONTROL *pctrl);                             */
/*                                                                                  */
/*    publicdef INT le_move_point (LE_CONTROL *pctrl);                              */
/*                                                                                  */
/*    publicdef INT le_nooperation ();                                              */
/*                                                                                  */
/*    publicdef INT le_pan(LE_CONTROL *pctrl);                                      */
/*                                                                                  */
/*    publicdef INT le_replace_stroke(LE_CONTROL *pctrl);                           */
/*                                                                                  */
/*    publicdef INT le_reset(LE_CONTROL *pctrl);                                    */
/*                                                                                  */
/*    publicdef INT le_select_stroke(LE_CONTROL *pctrl);                            */
/*                                                                                  */
/*    publicdef INT le_set_param(LE_CONTROL *pctrl);                                */
/*                                                                                  */
/*    publicdef INT le_start_stroke (LE_CONTROL *pctrl);                            */
/*                                                                                  */
/*    publicdef INT le_start_stroke_pt_mode(LE_CONTROL *pctrl);                     */
/*                                                                                  */
/*    publicdef INT le_undefined (LE_CONTROL *pctrl);                               */
/*                                                                                  */
/*    publicdef INT le_zoomin(LE_CONTROL *pctrl);                                   */
/*                                                                                  */
/*    publicdef INT le_zoomout(LE_CONTROL *pctrl);                                  */
/*                                                                                  */
/*    publicdef INT le_allocate_keymap(LE_KEYS  **pkeymap, INT mode, INT nmaps);    */
/*                                                                                  */
/*    publicdef INT le_set_key(LE_KEYS *keymap, INT indx, INT button, INT operation,    */
/*                        INT (*pfunc)(), CHAR text[32], BOOL save_state);          */
/*                                                                                  */
/*    publicdef LE_KEYS * le_scan_keymap(LE_KEYS *keymap, INT nmaps, INT operation);*/
/*                                                                                  */
/*    publicdef INT le_enable_key(LE_KEYS *keymap, INT indx, INT button, BOOL onoff);   */
/*                                                                                  */
/*                                                                                  */
/*Private_Functions:                                                                */
/*    static VOID lez_change_key_label(LE_KEYS **pdone, LE_KEYS **pcancel,      */
/*        LE_CONTROL *pctrl, INT index_prompt);                                     */
/*                                                                                  */
/*    static VOID lez_control_draw_vector(NLIST_HEADER line, DOUBLE wx1,        */
/*        DOUBLE wy1, DOUBLE wx2, DOUBLE wy2, LINE_ATTRIBUTE *attribute);           */
/*                                                                                  */
/*    static VOID lez_control_draw_point(NLIST_HEADER line, DOUBLE wx,          */
/*        DOUBLE wy, LINE_ATTRIBUTE                                                 */

/* Function Description -----------------------------------------------------       */
/*Description:                                                                      */
/*    See above.                                                                    */
/*                                                                                  */
/*Prototype:                                                                        */
/*    publicdef INT  le_nlist_editor (LE_PARLIST *ppar);                            */
/*                                                                                  */
/*Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)        */
/*    (I) ppar        -(LE_PARLIST *) N-List editor data structure.                 */
/*                                                                                  */
/*Return Value/Status:                                                              */
/*    See below.                                                                    */
/*                                                                                  */
/*Scope:                                                                            */
/*    PUBLIC                                                                        */
/*                                                                                  */
/*Limitations/Assumptions:                                                          */
/*    NOTE: zzt_wcs2nwc() is in the window manager internals area.                  */
/*                                                                                  */
/*-----------------------------------------------------------------------------     */

/* BEGIN:                                                                           */
#if USE_PROTOTYPES
publicdef INT le_nlist_editor (LE_PARLIST *ppar)
#else
publicdef INT le_nlist_editor (ppar)
LE_PARLIST *ppar;
#endif
    {
    
    INT oper, count;
    LE_CONTROL *pctrl;
    LE_CONTROL *lastpctrl;
    LE_KEYS *pKey;
    GKS_INT defmod, regmod;

#ifdef vms

#ifdef USE_X
    WM_CURRENT *wmv_current = wm_static ();
    INT num_gks_win_create = wmz_ngks_wk_created ();
    INT i;
#endif  
#endif  
    wm_inq_deferral (&defmod, &regmod);
    wm_set_deferral (GASAP, GSUPPD);
    
    /* disable auto swapping of buttons 1 & 4 for GPX menu                          */
    
    if (ug_if_gpx ())
        ppar->key_swap = FALSE;
    
    /*    initialize control structures and other variables                         */
    
    lastpctrl = (LE_CONTROL *) tc_zalloc (sizeof(LE_CONTROL));
    pctrl = (LE_CONTROL *) tc_zalloc (sizeof(LE_CONTROL));
    
    /* work on a copy of this pointlist, just in case the bozo hits CANCEL          */
    
    nl_copy_nlist (ppar->nlist, &pctrl->nlist);
    
    /*    delete stroke with 0 of 1 points in the nlist passed by the application   */
    
    lez_delete_null_strokes (pctrl->nlist);
    
    /*   initialisations of default dialogs                                         */
    
    lez_init (ppar, pctrl);
    
    /*  if the application has its own dialogs or means to provide functions to     */
    /*   the nlist editor, then initialize this dialogs                             */
    
    if (ppar->init_dialog AND (ppar->zz_init_dial IS_NOT_EQUAL_TO NULL_FUNCTION_PTR))
        {
        (*ppar->zz_init_dial)(pctrl);
        }
    curr_err = LE_NONE;
    do_update = TRUE;
    
    /*  copy of the last state of the nlist in order the UNDO function              */
    
    nl_copy_nlist (pctrl->nlist, &lastpctrl->nlist);
    
    /*   draw   the nlist                                                           */
    
    COUNT_POINTS (pctrl->nlist, &count);
    if (count > 0)
        lez_draw_line (pctrl->nlist, &pctrl->draw_attribute);
    
    /*  initialize  PICK WINDOW                                                     */
    
    Var_List[0] = (VOIDPTR)&dx;
    Var_List[1] = (VOIDPTR)&dy;
    nl_set_nlist_info (pctrl->nlist, NL_NLIST_FIND_TOLERANCE, NDIM, Col_List,
                       Var_List);
    
    wmtbuttoninterp (OFF);          /* turn off window mgr pan/zoom                 */
    
    pctrl->nbp = 0;
    pctrl->end_edit = FALSE;
    
    /*  BEGINNING OF MAIN LOOP OF THE NLIST_EDITOR                                  */
    /* ------------------------------------------------------                       */
    /* DEC GKS HACK: Add the exposure redraw event handler                          */
    /* ------------------------------------------------------                       */

#ifdef vms
#ifdef USE_X
#ifndef PRIOR_GKS  /* ========== BEGIN DEC GKS ==========*/   
    /* ----------------------------------                                           */
    /* Find the output widget associated                                            */
    /* with the current workstation id.                                             */
    /* ----------------------------------                                           */
    for (i = 0; i < num_gks_win_create; i++)
        {
        if (MAIN_WKID == wmv_current->gks_widget_array[i].ws_id)
            {
            break;
            }
        }
    /* ----------------------------------                                           */
    /* Define an event handler to redraw                                            */
    /* the current nlist when an expose                                             */
    /* event occurs.                                                                */
    /* ----------------------------------                                           */
    XtAddEventHandler (wmv_current->gks_widget_array[i].out_widget, ExposureMask, TRUE,
                       (XtEventHandler)lez_redraw_line, (caddr_t)NULL);

#endif  /* ========== END DEC GKS ========== */
#endif
#endif
    do
        {
        /* -------------------------------------------------                        */
        /* DEC GKS HACK: Set Statics for redraw on exposures                        */
        /* -------------------------------------------------                        */
        lez_current_nlist = pctrl->nlist;
        lez_current_attribute = &pctrl->draw_attribute;
        
        if (pctrl->use_keym)        /* *** begin key operation ***                  */
            {
            /* case the application uses keymaps and associated custom dialogs      */
            /* we open the dialogs hilight current stroke and ask the operator      */
            /* to hit a key for providing an operation                              */
            /* if error in the last operation  process the error                    */
            
            if (do_update)
                {
                if (pctrl->stroke_mode)
                    {
                    lez_update (cur_stroke_id[pctrl->index_stroke_keym],
                                pctrl->default_puck_map);
                    }
                else
                    {
                    lez_update (cur_point_id[pctrl->index_point_keym],
                                pctrl->default_puck_map);
                    }
                }
            /* open dialog on mac corresponding to stroke or point puck key         */
            
            if (pctrl->default_puck_map)
                {
                if (pctrl->dial_update)
                    {
                    /* for deleting a previous error message on the                 */
                    /* dialog we open we set a message to noerror and               */
                    /* a blank line will be displayed erasing the old message       */
                    
                    curr_err = LE_NONE;
                    do_update = TRUE;
                    /* ---------------------                                        */
                    /* Currently STROKE MODE                                        */
                    /* ---------------------                                        */
                    if (pctrl->stroke_mode)
                        {
                        if (!if_mouse_menus)
                            {
                            wiopcd (cur_stroke_id[pctrl->index_stroke_keym]);
                            lez_update (cur_stroke_id[pctrl->index_stroke_keym],
                                        pctrl->default_puck_map);
                            lez_prompt (cur_stroke_id[pctrl->index_stroke_keym], NULL_STRING);
                            }
                        else
                            {
                            wm_set_mouse_menu (cur_stroke_menus[pctrl->
                                                                index_stroke_keym],
                                               cur_stroke_maps[pctrl->
                                                               index_stroke_keym]);
                            am_message (AM_DIALOG, NULL_STRING);
                            }
                        }
                    else            /* --------------------                         */
                        /* Currently POINT MODE                                     */
                        /* --------------------                                     */
                        {
                        if (!if_mouse_menus)
                            {
                            wiopcd (cur_point_id[pctrl->index_point_keym]);
                            lez_update (cur_point_id[pctrl->index_point_keym],
                                        pctrl->default_puck_map);
                            lez_prompt (cur_stroke_id[pctrl->index_stroke_keym], NULL_STRING);
                            lez_prompt (cur_point_id[pctrl->index_point_keym], NULL_STRING);
                            }
                        else
                            {
                            wm_set_mouse_menu (cur_point_menus[pctrl->
                                                               index_point_keym],
                                               cur_point_maps[pctrl->
                                                              index_point_keym]);
                            am_message (AM_DIALOG, NULL_STRING);
                            }
                        }
                    pctrl->dial_update = FALSE;
                    }
                }
            pctrl->last_mode = pctrl->stroke_mode;
            
            /* initialisation for next request to the puck key                      */
            
            pctrl->wx = 0.0;
            pctrl->wy = 0.0;
            pctrl->but = 0;
            
            /* request locator or rubberband                                        */
            /* depending on last option processed                                   */
            /* pctrl -> nbp = 0   means we need the locator                         */
            /* = 1   means we need the rubberband                                   */
            
            lez_rqbutxy (pctrl->nlist, &pctrl->stat, pctrl->lastx, pctrl->lasty,
                         &pctrl->wx, &pctrl->wy, &pctrl->but, pctrl->nbp);
            
            /* depending of the current puck key we are looking for the key_function*/
            /* corresponding to the button returned by the window manager           */
            
            if (pctrl->stroke_mode)
                {
                pKey = lez_translate (pctrl->but, pctrl->stroke_keymap,
                                      pctrl->index_stroke_keym);
		while (pKey->operation EQUALS BUTTON_START_STROKE AND
		       wm_is_menu_button() EQUALS TRUE)
		  {
		    lez_rqbutxy (pctrl->nlist, &pctrl->stat, pctrl->lastx, 
				 pctrl->lasty, &pctrl->wx, &pctrl->wy, 
				 &pctrl->but, pctrl->nbp);
		    pKey = lez_translate (pctrl->but, pctrl->stroke_keymap,
					  pctrl->index_stroke_keym);
		  }

                }
            else
                {
                pKey = lez_translate (pctrl->but, pctrl->point_keymap,
                                      pctrl->index_point_keym);
                }
            /* MAKE SURE THIS BUTTON IS VALID                                       */
            
            if ((pKey->validt EQUALS FALSE) AND (pKey->
                                                 operation IS_NOT_EQUAL_TO 
                                                 BUTTON_UNDO))
                {
                oper = BUTTON_NOOP;
                curr_err = LE_BUTTON_NOT_USED;
                }
            else
                {
                oper = pKey->operation;
                pctrl->pKey = pKey;
                }
            }
        /* ***** end of keymap operation *****                                      */
        
        else                        /*  the application has provided a server which updates the */
                                    /*    interface and returns an operation to the editor  */
            {
            if (ppar->zz_dial_oper IS_NOT_EQUAL_TO NULL_FUNCTION_PTR)
                {
                oper = (*ppar->zz_dial_oper)(pctrl);
                }
            else
                {
                oper = BUTTON_NOOP;
                curr_err = LE_BUTTON_NOT_USED;
                }
            }
        /*   END  OF SECTION  WHERE A FUNCTION IS SELECTED USING THE KEYMAP OR ANOTHER  */
        /*  INTERFACE PROVIDED BY THE APPLICATION                                   */
        /* save last stroke operation                                               */
        
        if (pctrl->stroke_mode)
            {
            pctrl->last_operation = pctrl->current_operation;
            pctrl->current_operation = oper;
            }
        /* send the labelling of the function in the PROMPT of the dialog           */
        
        if (pctrl->use_keym AND pctrl->default_puck_map)
            {
            if (pctrl->stroke_mode)
                {
                lez_prompt (cur_stroke_id[pctrl->index_stroke_keym],
                            pKey->description);
                }
            else
                {
                lez_prompt (cur_point_id[pctrl->index_point_keym], pKey->description);
                }
            }
        /*  in order to perform the UNDO option we need to have a copie             */
        /*of the last state of the n_list this is done by function                  */
        /*lez_save_list which is called if the function wanted by the               */
        /*operator is of the mode STROKE_MODE in point_mode we use BACK_POINT to undo.. */
        
        if (pKey->save_state)
            {
            lez_save_list (pctrl, lastpctrl);
            }
        else                        /*  if we are  on the UNDO function we perform it now   */
            {
            if (oper EQUALS BUTTON_UNDO)
                {
                /*  UNDO has been performed and we set a nooperation as next function   */
                /*  to be performed                                                 */
                
                lez_undo (&pctrl, &lastpctrl);
                oper = BUTTON_NOOP;
                }
            }
        /*  INNER LOOP  PROCESSING THE FUNCTION CHOOSEN BY THE OPERATOR             */
        /*  processing the function hit by the operator . this function             */
        /*can return a key_function  of another function which is processed         */
        /*in sequence and so on....  default function of the n_list editor          */
        /*always return a key_function  BUTTON_NOOP which means that there is       */
        /*no other operations in sequence.                                          */
        
        do
            {
            pKey = lez_inq_key (pctrl, oper);
            if (pKey->key_function IS_NOT_EQUAL_TO NULL_FUNCTION_PTR)
                {
                oper = (*pKey->key_function)(pctrl);
                }
            else
                {
                oper = BUTTON_NOOP;
                }
            } while (oper != BUTTON_NOOP);
        
        /* **** END OF INNER LOOP ****                                              */
        
        } while (NOT pctrl->end_edit);
    
    /* ------------------------------------------------------                       */
    /* DEC GKS HACK: Remove the exposure redraw event handler                       */
    /* ------------------------------------------------------                       */

#ifdef vms
#ifdef USE_X
#ifndef PRIOR_GKS  /* ========== BEGIN DEC GKS ========== */    
    /* ----------------------------------                                           */
    /* Remove the event handler to redraw                                           */
    /* the current nlist when an expose                                             */
    /* event occurs.                                                                */
    /* ----------------------------------                                           */
    XtRemoveEventHandler (wmv_current->gks_widget_array[i].out_widget, ExposureMask,
                          TRUE, (XtEventHandler)lez_redraw_line, (caddr_t)NULL);

#endif  /* ========== END DEC GKS ========== */
#endif
#endif
    
    /* END OF MAIN LOOP OF THE EDITOR _ WE EXIT THE LOOP IF KEYS                    */
    /*  CANCEL OR DONE WERE HIT WHILE THE EDITOR WAS IN STROKE MODE                 */
    /*  end  of the n-list editor   we test the status if it was canceled           */
    /*we return the n-list passed as input of the n-list editor else                */
    /*we return the edited line                                                     */
    
    if (pctrl->current_operation IS_NOT_EQUAL_TO BUTTON_CANCEL)
        {
        FREE_LINE (ppar->nlist);
        ppar->nlist = pctrl->nlist;
        pctrl->stat = SUCCESS;
        }
    else                            /*   the line editor has been canceled so we draw in erase colour   */
                                    /*the current nlist -  the initial nlist is passed back to  */
                                    /*the aplication which can redraw it            */
        {
        COUNT_POINTS (pctrl->nlist, &count);
        if (count > 0)
            lez_draw_line (pctrl->nlist, &pctrl->erase_attribute);
        FREE_LINE (pctrl->nlist);
        pctrl->stat = FAIL;
        }
    lez_term (pctrl);
    
    ppar->status = pctrl->stat;
    
    /*  release copy of nlist  kept for UNDO function                               */
    
    FREE_LINE (lastpctrl->nlist);
    
    /*  release  application defined dialogs                                        */
    
    if ((ppar->init_dialog) AND (ppar->zz_dial_release != NULL_FUNCTION_PTR))
        (*ppar->zz_dial_release)(pctrl);
    
    /*  release control structures                                                  */
    
    tc_free ((VOIDPTR)pctrl);
    tc_free ((VOIDPTR)lastpctrl);
    
    wmtbuttoninterp (ON);           /* turn on window mgr pan/zoom                  */
    
    wm_set_deferral (defmod, regmod);
    
    return ppar->status;
    }
/* END:                                                                             */

/* **************************************************************************       */
/*                                                                                  */
/*                           DEFAULT FUNCTIONS                                      */
/*                                                                                  */
/* ************************************************************************ **      */

/* Function Description -----------------------------------------------------       */
/*Description:                                                                      */
/*    Add a point after the last point of the scratch stroke.                       */
/*                                                                                  */
/*Prototype:                                                                        */
/*    publicdef INT le_add_point (LE_CONTROL *pctrl);                               */
/*                                                                                  */
/*Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)        */
/*    pctrl           -(LE_CONTROL *)                                               */
/*                                                                                  */
/*Return Value/Status:                                                              */
/*    BUTTON_NOOP                                                                   */
/*                                                                                  */
/*Scope:                                                                            */
/*    PUBLIC                                                                        */
/*                                                                                  */
/*Limitations/Assumptions:                                                          */
/*                                                                                  */
/*-----------------------------------------------------------------------------     */

/* BEGIN:                                                                           */
#if USE_PROTOTYPES
publicdef INT le_add_point (LE_CONTROL *pctrl)
#else
publicdef INT le_add_point (pctrl)
LE_CONTROL *pctrl;
#endif
    {
    /* set current point at the end of the scratch stroke                           */
    
    COUNT_STROKE_POINTS (pctrl->nlist, pctrl->scratch_str1, &pctrl->curpoint_str1);
    nl_set_current_point (pctrl->nlist, pctrl->curpoint_str1, pctrl->scratch_str1);
    
    /*  add a point after the last point of scratch stroke                          */
    
    ADD_POINT (pctrl->nlist, pctrl->wx, pctrl->wy);
    
    /*  draw segment between last point and current point  if more than one point   */
    /*  in the stroke                                                               */
    
    if (pctrl->curpoint_str1 > 0)
        {
        lez_control_draw_vector (pctrl->nlist, pctrl->lastx, pctrl->lasty, pctrl->wx,
                                 pctrl->wy, &pctrl->draw_attribute);
        }
    pctrl->lastx = pctrl->wx;
    pctrl->lasty = pctrl->wy;
    pctrl->stroke_mode = FALSE;
    pctrl->nbp = 1;
    
    return BUTTON_NOOP;
    }
/* END:                                                                             */

/* Function Description -----------------------------------------------------       */
/*Description:                                                                      */
/*    Add a point to  the scratch stroke in a monotonic mode on first dimension     */
/*    of the nlist.                                                                 */
/*                                                                                  */
/*Prototype:                                                                        */
/*    publicdef INT le_add_point_mono (LE_CONTROL *pctrl);                          */
/*                                                                                  */
/*Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)        */
/*    pctrl           -(LE_CONTROL *)                                               */
/*                                                                                  */
/*Return Value/Status:                                                              */
/*    BUTTON_ADD_POINT                                                              */
/*    BUTTON_NOOP                                                                   */
/*                                                                                  */
/*Scope:                                                                            */
/*    PUBLIC                                                                        */
/*                                                                                  */
/*Limitations/Assumptions:                                                          */
/*                                                                                  */
/*-----------------------------------------------------------------------------     */

/* BEGIN:                                                                           */
#if USE_PROTOTYPES
publicdef INT le_add_point_mono (LE_CONTROL *pctrl)
#else
publicdef INT le_add_point_mono (pctrl)
LE_CONTROL *pctrl;
#endif
    {
    UINT number;
    BOOL order;
    GKS_REAL lastx, lasty, x, y;
    
    COUNT_STROKE_POINTS (pctrl->nlist, pctrl->scratch_str1, &number);
    
    if (number >= 2)
        {
        nl_set_current_point (pctrl->nlist, number - 1, pctrl->scratch_str1);
        GET_CURRENT_XY (pctrl->nlist, &lastx, &lasty);
        nl_set_current_point (pctrl->nlist, number, pctrl->scratch_str1);
        GET_CURRENT_XY (pctrl->nlist, &x, &y);
        
        /*  looking for the order (ascending or descending on first dimension       */
        /*  using last two points of the stroke                                     */
        
        order = (lastx > x);
        
        /*   control that new point has the same order than rest of the stroke      */
        
        if (((pctrl->lastx >
              pctrl->wx) AND (order EQUALS FALSE)) OR ((pctrl->lastx <
                                                        pctrl->
                                                        wx) AND (order EQUALS TRUE)))
            {
            
            /*  last input has not the same order so we return an error and the     */
            /*   point will not be added to the stroke(return BUTTON_NOOP)          */
            do_update = TRUE;
            curr_err = LE_POINT_NO_MONO;
            return BUTTON_NOOP;
            }
        }
    /*  control  OK so we add point to the stroke  using default function           */
    /* le_add_point (return BUTTON_ADD_POINT)                                       */
    
    return BUTTON_ADD_POINT;
    }
/* END:                                                                             */

/* Function Description -----------------------------------------------------       */
/*Description:                                                                      */
/*    Delete the last point of the scratch stroke.                                  */
/*                                                                                  */
/*Prototype:                                                                        */
/*    publicdef INT le_back_point (LE_CONTROL *pctrl);                              */
/*                                                                                  */
/*Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)        */
/*    pctrl           -(LE_CONTROL *)                                               */
/*                                                                                  */
/*Return Value/Status:                                                              */
/*    BUTTON_NOOP                                                                   */
/*                                                                                  */
/*Scope:                                                                            */
/*    PUBLIC                                                                        */
/*                                                                                  */
/*Limitations/Assumptions:                                                          */
/*                                                                                  */
/*-----------------------------------------------------------------------------     */

/* BEGIN:                                                                           */
#if USE_PROTOTYPES
publicdef INT le_back_point (LE_CONTROL *pctrl)
#else
publicdef INT le_back_point (pctrl)
LE_CONTROL *pctrl;
#endif
    {
    
    /*  count number of points in the scratch stroke                                */
    COUNT_STROKE_POINTS (pctrl->nlist, pctrl->scratch_str1, &pctrl->curpoint_str1);
    switch (pctrl->curpoint_str1)
        {
    case (0):
        do_update = TRUE;
        curr_err = LE_NO_POINTS;
        pctrl->nbp = 0;
        break;
    case (1):                       /* delete the stroke if there is only one point */
        nl_delete_stroke (pctrl->nlist, (UINT)0);
        pctrl->stroke_mode = TRUE;
        pctrl->dial_update = TRUE;
        pctrl->nbp = 0;
        break;
    default:
        pctrl->wx = pctrl->lastx;
        pctrl->wy = pctrl->lasty;
        nl_delete_points (pctrl->nlist, (UINT)1);
        GET_CURRENT_XY (pctrl->nlist, &pctrl->lastx, &pctrl->lasty);
        lez_control_draw_vector (pctrl->nlist, pctrl->lastx, pctrl->lasty, pctrl->wx,
                                 pctrl->wy, &pctrl->erase_attribute);
        break;
        }
    return BUTTON_NOOP;
    }
/* END:                                                                             */

/* Function Description -----------------------------------------------------       */
/*Description:                                                                      */
/*    If we are i stroke mode cancel the line editor;                               */
/*    If we are in point mode cancel the operation done on the scratch stroke       */
/*    which is deleted.                                                             */
/*                                                                                  */
/*Prototype:                                                                        */
/*    publicdef INT le_cancel (LE_CONTROL *pctrl);                                  */
/*                                                                                  */
/*Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)        */
/*    pctrl           -(LE_CONTROL *)                                               */
/*                                                                                  */
/*Return Value/Status:                                                              */
/*    BUTTON_NOOP                                                                   */
/*                                                                                  */
/*Scope:                                                                            */
/*    PUBLIC                                                                        */
/*                                                                                  */
/*Limitations/Assumptions:                                                          */
/*                                                                                  */
/*-----------------------------------------------------------------------------     */

/* BEGIN:                                                                           */
#if USE_PROTOTYPES
publicdef INT le_cancel (LE_CONTROL *pctrl)
#else
publicdef INT le_cancel (pctrl)
LE_CONTROL *pctrl;
#endif
    {
    
    pctrl->nbp = 0;
    
    if (pctrl->stroke_mode)
        /*  we are in stroke mode cancel means that we cancel the editor            */
        /* the editor will be canceled with two consecutive CANCEL                  */
        /* on the first one we send a message to ask for a confirmation             */
        {
        if (pctrl->last_operation IS_NOT_EQUAL_TO BUTTON_CANCEL)
            {
            if (pctrl->default_puck_map)
                {
                lez_prompt (cur_stroke_id[pctrl->index_stroke_keym],
                            mg_message (LE_PROMPT_CANCEL));
                }
            }
        else
            {
            pctrl->end_edit = TRUE;
            }
        }
    else
        {
        
        /*  we are in point mode  CANCEL means that we stop the digitalisation      */
        /*and cancel the operation on the scratch stroke recovering the             */
        /*current stroke  so we need to :                                           */
        /*    draw scratch stroke with erase color                                  */
        /*    delete scratch stroke in nlist                                        */
        /*    draw current stroke in draw color                                     */
        
        lez_draw_stroke (pctrl->nlist, pctrl->scratch_str1, &pctrl->erase_attribute);
        nl_delete_stroke (pctrl->nlist, pctrl->scratch_str1);
        
        pctrl->scratch_str1 = 0;
        
        /*  redraw current stroke                                                   */
        
        lez_draw_stroke (pctrl->nlist, pctrl->current_stroke, &pctrl->draw_attribute);
        pctrl->stroke_mode = TRUE;
        pctrl->dial_update = TRUE;
        }
    return BUTTON_NOOP;
    }
/* END:                                                                             */

/* Function Description -----------------------------------------------------       */
/*Description:                                                                      */
/*    Divide  stroke into two strokes by deleting part of the                       */
/*    stroke which is selected interactively.                                       */
/*    In this function zoom pan reset are available                                 */
/*        cancel  -cancel the operation                                             */
/*        done    -confirm the operation                                            */
/*                                                                                  */
/*Prototype:                                                                        */
/*    publicdef INT le_cleave_stroke (LE_CONTROL *pctrl);                           */
/*                                                                                  */
/*Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)        */
/*    pctrl           -(LE_CONTROL *)                                               */
/*                                                                                  */
/*Return Value/Status:                                                              */
/*    BUTTON_NOOP                                                                   */
/*                                                                                  */
/*Scope:                                                                            */
/*    PUBLIC                                                                        */
/*                                                                                  */
/*Limitations/Assumptions:                                                          */
/*                                                                                  */
/*-----------------------------------------------------------------------------     */

/* BEGIN:                                                                           */
#if USE_PROTOTYPES
publicdef INT le_cleave_stroke (LE_CONTROL *pctrl)
#else
publicdef INT le_cleave_stroke (pctrl)
LE_CONTROL *pctrl;
#endif
    {
    UINT stroke1, stroke2, stroke_picked, numpoints, minpoint, maxpoint, number, cnt,
        first_point, second_point, numb;
    INT nb_option = 7;
    BOOL noexit;
    LE_KEYS *pdone, *pcancel;
    
    /*  testing first if there are some strokes in the n-list                       */
    
    nl_inq_nlist_int (pctrl->nlist, NL_NLIST_NSTROKES, (INT *)&numb);
    
    if (numb EQUALS 0)
        {
        do_update = TRUE;
        curr_err = LE_NO_STROKES;
        }
    else
        {
        /* update  prompts  for key cancel and done                                 */
        lez_change_key_label (&pdone, &pcancel, pctrl, LE_PROMPT_CLEAVE);
        
        /*  dim operation keys not used in this function                            */
        lez_enable_option (pctrl, option_array3, nb_option, OFF);
        
        /*  select part of stroke to cleave                                         */
        
        lez_sel_part (&first_point, &second_point, &stroke_picked, &noexit, &cnt,
                      pctrl, BUTTON_CLEAVE_STROKE, option_array3, nb_option);
        
        if (noexit)
            {
            minpoint = (first_point > second_point) ? second_point : first_point;
            maxpoint = (first_point > second_point) ? first_point : second_point;
            /* create stroke1 first part of segment cleaved                         */
            
            lez_draw_stroke (pctrl->nlist, stroke_picked, &pctrl->erase_attribute);
            
            COUNT_STROKE_POINTS (pctrl->nlist, stroke_picked, &numpoints);
            
            /*  test if we want to cleave the whole stroke which                    */
            /*is  equivalent to delete the stroke                                   */
            
            if (minpoint > 1 OR maxpoint < numpoints)
                {
                /*  we just keep strokes with at least  two points                  */
                if (minpoint > 1)
                    {
                    COPY_STROKE (pctrl->nlist, stroke_picked, &stroke1);
                    number = numpoints - minpoint;
                    DEL_PART_OF_STROKE (pctrl->nlist, stroke1, minpoint + 1, number);
                    lez_draw_stroke (pctrl->nlist, stroke1, &pctrl->draw_attribute);
                    }
                if (maxpoint < numpoints)
                    {
                    /*create stroke2   second part of segment cleaved               */
                    COPY_STROKE (pctrl->nlist, stroke_picked, &stroke2);
                    DEL_PART_OF_STROKE (pctrl->nlist, stroke2, 1, maxpoint - 1);
                    
                    lez_draw_stroke (pctrl->nlist, stroke2, &pctrl->draw_attribute);
                    }
                }
            /*  delete the original stroke before cleaving                          */
            
            nl_delete_stroke (pctrl->nlist, stroke_picked);
            }
        /*   restore default labels for cancel and done keys                        */
        
        lez_restor_default_prompt (pcancel, pdone, pctrl);
        
        /*  restore operation keys not used in this function                        */
        lez_enable_option (pctrl, option_array3, nb_option, ON);
        
        }
    return BUTTON_NOOP;
    }
/* END:                                                                             */

/* Function Description -----------------------------------------------------       */
/*Description:                                                                      */
/*    Delete the stroke we are pointing at.                                         */
/*                                                                                  */
/*Prototype:                                                                        */
/*    publicdef INT le_delete_stroke (LE_CONTROL *pctrl);                           */
/*                                                                                  */
/*Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)        */
/*    pctrl           -(LE_CONTROL *)                                               */
/*                                                                                  */
/*Return Value/Status:                                                              */
/*    BUTTON_NOOP                                                                   */
/*                                                                                  */
/*Scope:                                                                            */
/*    PUBLIC                                                                        */
/*                                                                                  */
/*Limitations/Assumptions:                                                          */
/*                                                                                  */
/*-----------------------------------------------------------------------------     */

/* BEGIN:                                                                           */
#if USE_PROTOTYPES
publicdef INT le_delete_stroke (LE_CONTROL *pctrl)
#else
publicdef INT le_delete_stroke (pctrl)
LE_CONTROL *pctrl;
#endif
    {
    UINT strokenum;
    UINT pointnum;
    
    lez_find_stroke (pctrl, pctrl->wx, pctrl->wy, &strokenum, &pointnum);
    if (strokenum IS_NOT_EQUAL_TO 0)
        {
        lez_del_stroke (pctrl->nlist, strokenum, &pctrl->erase_attribute);
        pctrl->nbp = 0;
        }
    else
        {
        curr_err = LE_NO_STROKE_FOUND;
        do_update = TRUE;
        }
    return BUTTON_NOOP;
    }
/* END:                                                                             */

/* Function Description -----------------------------------------------------       */
/*Description:                                                                      */
/*    In stroke mode normal exit of the editor.                                     */
/*    In point mode confirm the operation done using the scratch stroke.            */
/*                                                                                  */
/*Prototype:                                                                        */
/*    publicdef INT le_done (LE_CONTROL *pctrl);                                    */
/*                                                                                  */
/*Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)        */
/*    pctrl           -(LE_CONTROL *)                                               */
/*                                                                                  */
/*Return Value/Status:                                                              */
/*    BUTTON_NOOP                                                                   */
/*                                                                                  */
/*Scope:                                                                            */
/*    PUBLIC                                                                        */
/*                                                                                  */
/*Limitations/Assumptions:                                                          */
/*                                                                                  */
/*-----------------------------------------------------------------------------     */

/* BEGIN:                                                                           */
#if USE_PROTOTYPES
publicdef INT le_done (LE_CONTROL *pctrl)
#else
publicdef INT le_done (pctrl)
LE_CONTROL *pctrl;
#endif
    {
    UINT number, minpoint, maxpoint;
    UINT i, j, nbpoints;
    GKS_REAL x, y;
    
    if (pctrl->stroke_mode)
        
        /*  we are in stroke mode   DONE  means that it is the end of the editor    */
        /* the editor will be canceled with two consecutive DONE                    */
        /* on the first one we send a message to ask for a confirmation             */
        {
        if (pctrl->last_operation IS_NOT_EQUAL_TO BUTTON_DONE)
            {
            if (pctrl->default_puck_map)
                {
                lez_prompt (cur_stroke_id[pctrl->index_stroke_keym],
                            mg_message (LE_PROMPT_EXIT));
                }
            }
        else
            {
            /*    delete strokes with 0 or 1 points  before leaving the editor      */
            
            lez_delete_null_strokes (pctrl->nlist);
            
            pctrl->end_edit = TRUE;
            }
        }
    else                            /*   we are in point mode done is used to validate a new digitized stroke   */
                                    /*  or   an extension of an existing stroke     */
                                    /*  or   a replacement of a part of stroke      */
        {
        
        switch (pctrl->current_operation)
            {
        case (BUTTON_START_STROKE):
            /*  case of a new stroke that we finish to digitize                     */
            /* we validate the scratch stroke in current stroke                     */
            /*   we verify first that there are at least two points in the stokr    */
            /* if not we ignore this new stroke                                     */
            COUNT_STROKE_POINTS (pctrl->nlist, pctrl->scratch_str1, &nbpoints);
            if (nbpoints > 1)
                {
                pctrl->last_stroke = pctrl->current_stroke;
                pctrl->last_point = pctrl->current_point;
                }
            else
                {
                nl_delete_stroke (pctrl->nlist, pctrl->scratch_str1);
                }
            break;
            
        case (BUTTON_EXTEND_STROKE):
            /* we have extended a stroke and confirm the extension                  */
            /*  so the scratch stroke created is replacing the current stroke       */
            /*  which is deleted before its replacement                             */
            /*   we verify first that there are at least two points in the stokr    */
            /*if not we ignore this new stroke                                      */
            COUNT_STROKE_POINTS (pctrl->nlist, pctrl->scratch_str1, &nbpoints);
            
            if (nbpoints > 1)
                {
                lez_draw_stroke (pctrl->nlist, pctrl->scratch_str1,
                                 &pctrl->draw_attribute);
                nl_delete_stroke (pctrl->nlist, pctrl->current_stroke);
                }
            else
                {
                /*  given the structure of nlist we need to  delete the higher      */
                /*stroke number before the lower one else we will need to           */
                /*  keep some points and retrieve the stroke number for             */
                /*  deleting the second one                                         */
                if (pctrl->scratch_str1 > pctrl->current_stroke)
                    {
                    nl_delete_stroke (pctrl->nlist, pctrl->scratch_str1);
                    nl_delete_stroke (pctrl->nlist, pctrl->current_stroke);
                    }
                else
                    {
                    nl_delete_stroke (pctrl->nlist, pctrl->current_stroke);
                    nl_delete_stroke (pctrl->nlist, pctrl->scratch_str1);
                    }
                }
            break;
            
        case (BUTTON_REPLACE_STROKE):
            
            /* we have replaced a part stroke we validate this digitalization       */
            /* so we need to join with the rest of the stroke                       */
            /* depending on how you digitized starting closer to the end            */
            /* or to the beginning of the nlist we have to reverse or not tt he     */
            /* rest of the string                                                   */
            
            minpoint = (pctrl->current_point > pctrl->last_point) ?
                pctrl->last_point : pctrl->current_point;
            maxpoint = (pctrl->current_point > pctrl->last_point) ?
                pctrl->current_point : pctrl->last_point;
            
            COUNT_STROKE_POINTS (pctrl->nlist, pctrl->current_stroke, &number);
            
            /* we test if the one of the bound of the part to replace  was a        */
            /*bound of the original stroke - in this case we just                   */
            /*keep the scratch_stroke and don't add the extra part                  */
            /*which in this case is one point  - we assume that the user            */
            /*didn't want to keep the last point                                    */
            
            if (minpoint IS_NOT_EQUAL_TO 1 AND maxpoint IS_NOT_EQUAL_TO number)
                {
                /* count number of points in the scratch stroke                     */
                COUNT_STROKE_POINTS (pctrl->nlist, pctrl->scratch_str1,
                                     &pctrl->curpoint_str1);
                if (pctrl->reverse)
                    {
                    /*   we need to add the beginning of the nlist reversing it     */
                    /*to be the same way than the scratch digitized stroke          */
                    ADD_REVERSE_STROKE (pctrl->nlist, pctrl->current_stroke, minpoint,
                                        pctrl->nlist, pctrl->scratch_str1, minpoint,
                                        pctrl->curpoint_str1);
                    }
                else                /*  we just need to join the scratch stroke with the end    */
                                    /* of the current stroke where we replaced a part   */
                                    /* of stroke                                    */
                    {
                    number -= maxpoint;
                    ADD_STROKE (pctrl->nlist, pctrl->current_stroke, maxpoint,
                                pctrl->nlist, pctrl->scratch_str1, number + 1,
                                pctrl->curpoint_str1);
                    }
                }
            /*   erase current stroke which is replaced by the scratch stroke       */
            
            lez_draw_stroke (pctrl->nlist, pctrl->current_stroke,
                             &pctrl->erase_attribute);
            
            /*   we verify first that there are at least two points in the stroke   */
            /*if not we ignore this new stroke                                      */
            
            COUNT_STROKE_POINTS (pctrl->nlist, pctrl->scratch_str1, &nbpoints);
            
            if (nbpoints > 1)
                {
                /* the scratch stroke has more than 1 point so it validated         */
                /*and  the old stroke is deleted                                    */
                lez_draw_stroke (pctrl->nlist, pctrl->scratch_str1,
                                 &pctrl->draw_attribute);
                
                nl_delete_stroke (pctrl->nlist, pctrl->current_stroke);
                }
            else
                {
                /*  given the structure of nlist we need to  delete the higher      */
                /*stroke number before the lower one else we will need to           */
                /*  keep some points and retrieve the stroke number for             */
                /*  deleting the second one                                         */
                if (pctrl->scratch_str1 > pctrl->current_stroke)
                    {
                    nl_delete_stroke (pctrl->nlist, pctrl->scratch_str1);
                    nl_delete_stroke (pctrl->nlist, pctrl->current_stroke);
                    }
                else
                    {
                    nl_delete_stroke (pctrl->nlist, pctrl->current_stroke);
                    nl_delete_stroke (pctrl->nlist, pctrl->scratch_str1);
                    }
                }
            break;
            }
        pctrl->stroke_mode = TRUE;
        pctrl->dial_update = TRUE;
        pctrl->scratch_str1 = 0;
        pctrl->nbp = 0;
        }
    return BUTTON_NOOP;
    
    }
/* END:                                                                             */

/* Function Description -----------------------------------------------------       */
/*Description:                                                                      */
/*    Switches the editor in point mode to add points at one end                    */
/*    of the stroke we point at.                                                    */
/*                                                                                  */
/*Prototype:                                                                        */
/*    publicdef INT le_extend_stroke (LE_CONTROL *pctrl);                           */
/*                                                                                  */
/*Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)        */
/*    pctrl           -(LE_CONTROL *)                                               */
/*                                                                                  */
/*Return Value/Status:                                                              */
/*    BUTTON_NOOP                                                                   */
/*                                                                                  */
/*Scope:                                                                            */
/*    PUBLIC                                                                        */
/*                                                                                  */
/*Limitations/Assumptions:                                                          */
/*                                                                                  */
/*-----------------------------------------------------------------------------     */

/* BEGIN:                                                                           */
#if USE_PROTOTYPES
publicdef INT le_extend_stroke (LE_CONTROL *pctrl)
#else
publicdef INT le_extend_stroke (pctrl)
LE_CONTROL *pctrl;
#endif
    {
    UINT stroke, point_picked, last_point;
    INT i, j;
    GKS_REAL x, y;
    LE_KEYS *pkey1, *pkey2;
    
    lez_find_stroke_end (pctrl, pctrl->wx, pctrl->wy, &stroke, &point_picked);
    if (stroke EQUALS 0)
        /* no stroke was found where the user picked                                */
        {
        do_update = TRUE;
        curr_err = LE_NO_STROKE_FOUND;
        }
    else
        {
        /*   the stroke picked is copied in the scratch stroke                      */
        /*  to be extended then in point mode;depending on the end                  */
        /*  of stroke we want to extend it is reversed or not                       */
        /*  before being copied                                                     */
        COUNT_STROKE_POINTS (pctrl->nlist, stroke, &last_point);
        if (point_picked IS_NOT_EQUAL_TO 1)
            {
            pctrl->reverse = FALSE;
            COPY_STROKE (pctrl->nlist, stroke, &pctrl->scratch_str1);
            }
        else
            {
            pctrl->reverse = TRUE;
            nl_start_stroke (pctrl->nlist, &pctrl->scratch_str1);
            ADD_REVERSE_STROKE (pctrl->nlist, stroke, last_point, pctrl->nlist,
                                pctrl->scratch_str1, last_point, 0);
            }
        nl_set_current_point (pctrl->nlist, last_point, pctrl->scratch_str1);
        GET_CURRENT_XY (pctrl->nlist, &pctrl->lastx, &pctrl->lasty);
        pctrl->stroke_mode = FALSE;
        pctrl->dial_update = TRUE;
        pctrl->nbp = 1;
        pctrl->current_stroke = stroke;
        
        if (pctrl->key_swap)
            {
            /*  we configure the point keymap so that the add point key is on the same key  */
            /*than the start stroke key                                             */
            /* first look for the button for the start_stroke key                   */
            
            pkey2 = pctrl->stroke_keymap + (pctrl->index_stroke_keym * MAX_KEYS);
            pkey1 = le_scan_keymap (pkey2, (INT)1, BUTTON_EXTEND_STROKE);
            if (pkey1 IS_NOT_EQUAL_TO (LE_KEYS *)0)
                {
                /* pointer to the structure of point keymap corresponding to the same   */
                /* key                                                              */
                pkey2 = pctrl->point_keymap + (pctrl->index_point_keym * MAX_KEYS) +
                    pkey1->button;
                lez_swap_but (FALSE, pkey2->operation, BUTTON_ADD_POINT, pctrl);
                }
            }
        }
    return BUTTON_NOOP;
    }
/* END:                                                                             */

/* Function Description -----------------------------------------------------       */
/*Description:                                                                      */
/*                                                                                  */
/*Prototype:                                                                        */
/*    publicdef INT le_erase_pts(LE_CONTROL *pctrl);                                */
/*                                                                                  */
/*Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)        */
/*    pctrl           -(LE_CONTROL *)                                               */
/*                                                                                  */
/*Return Value/Status:                                                              */
/*    BUTTON_NOOP                                                                   */
/*                                                                                  */
/*Scope:                                                                            */
/*    PUBLIC                                                                        */
/*                                                                                  */
/*Limitations/Assumptions:                                                          */
/*                                                                                  */
/*-----------------------------------------------------------------------------     */

/* BEGIN:                                                                           */
#if USE_PROTOTYPES
publicdef INT le_erase_pts (LE_CONTROL *pctrl)
#else
publicdef INT le_erase_pts (pctrl)
LE_CONTROL *pctrl;
#endif
    {
    UINT cnt1, cnt2, num1, num2;
    GKS_REAL x1, y1;
    
    nl_inq_nlist_int (pctrl->nlist, NL_NLIST_NSTROKES, (INT *)&num1);
    for (cnt1 = 1; cnt1 <= num1; cnt1++)
        {
        nl_inq_stroke_int (pctrl->nlist, cnt1, NL_STROKE_NPOINTS, (INT *)&num2);
        for (cnt2 = 1; cnt2 <= num2; cnt2++)
            {
            nl_set_current_point (pctrl->nlist, cnt2, cnt1);
            GET_CURRENT_XY (pctrl->nlist, &x1, &y1);
            lez_control_draw_point (pctrl->nlist, x1, y1, &pctrl->erase_attribute);
            }
        }
    lez_draw_line (pctrl->nlist, &pctrl->draw_attribute);
    return BUTTON_NOOP;
    }
/* END:                                                                             */

/* Function Description -----------------------------------------------------       */
/*Description:                                                                      */
/*    Function allowing to go through a set of key maps.                            */
/*                                                                                  */
/*Prototype:                                                                        */
/*    publicdef INT le_gold_key (LE_CONTROL *pctrl);                                */
/*                                                                                  */
/*Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)        */
/*    pctrl           -(LE_CONTROL *)                                               */
/*                                                                                  */
/*Return Value/Status:                                                              */
/*    BUTTON_NOOP                                                                   */
/*                                                                                  */
/*Scope:                                                                            */
/*    PUBLIC                                                                        */
/*                                                                                  */
/*Limitations/Assumptions:                                                          */
/*                                                                                  */
/*-----------------------------------------------------------------------------     */

/* BEGIN:                                                                           */
#if USE_PROTOTYPES
publicdef INT le_gold_key (LE_CONTROL *pctrl)
#else
publicdef INT le_gold_key (pctrl)
LE_CONTROL *pctrl;
#endif
    {
    
    if (pctrl->stroke_mode)
        {
        pctrl->index_stroke_keym = ((pctrl->num_stroke_keymaps -
                                     1) EQUALS pctrl->index_stroke_keym) ?
                                   0 : pctrl->index_stroke_keym + 1;
        }
    else
        {
        pctrl->index_point_keym = ((pctrl->num_point_keymaps -
                                    1) EQUALS pctrl->index_point_keym) ?
                                  0 : pctrl->index_point_keym + 1;
        }
    pctrl->dial_update = TRUE;
    return BUTTON_NOOP;
    }
/* END:                                                                             */

/* Function Description -----------------------------------------------------       */
/*Description:                                                                      */
/*                                                                                  */
/*Prototype:                                                                        */
/*    publicdef INT le_hilight_pts(LE_CONTROL *pctrl);                              */
/*                                                                                  */
/*Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)        */
/*    pctrl           -(LE_CONTROL *)                                               */
/*                                                                                  */
/*Return Value/Status:                                                              */
/*    BUTTON_NOOP                                                                   */
/*                                                                                  */
/*Scope:                                                                            */
/*    PUBLIC                                                                        */
/*                                                                                  */
/*Limitations/Assumptions:                                                          */
/*                                                                                  */
/*-----------------------------------------------------------------------------     */

/* BEGIN:                                                                           */
#if USE_PROTOTYPES
publicdef INT le_hilight_pts (LE_CONTROL *pctrl)
#else
publicdef INT le_hilight_pts (pctrl)
LE_CONTROL *pctrl;
#endif
    {
    UINT cnt1, cnt2, num1, num2;
    GKS_REAL x1, y1;
    
    nl_inq_nlist_int (pctrl->nlist, NL_NLIST_NSTROKES, (INT *)&num1);
    for (cnt1 = 1; cnt1 <= num1; cnt1++)
        {
        nl_inq_stroke_int (pctrl->nlist, cnt1, NL_STROKE_NPOINTS, (INT *)&num2);
        for (cnt2 = 1; cnt2 <= num2; cnt2++)
            {
            nl_set_current_point (pctrl->nlist, cnt2, cnt1);
            GET_CURRENT_XY (pctrl->nlist, &x1, &y1);
            lez_control_draw_point (pctrl->nlist, x1, y1, &pctrl->hilight_attribute);
            }
        }
    return BUTTON_NOOP;
    }
/* END:                                                                             */

/* Function Description -----------------------------------------------------       */
/*Description:                                                                      */
/*    Join two strokes which are selected interactively.                            */
/*    In this function zoom pan rset are available                                  */
/*        cancel   cancel the operation                                             */
/*        done   confirm the operation                                              */
/*                                                                                  */
/*Prototype:                                                                        */
/*    publicdef INT le_join_stroke (LE_CONTROL *pctrl);                             */
/*                                                                                  */
/*Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)        */
/*    pctrl           -(LE_CONTROL *)                                               */
/*                                                                                  */
/*Return Value/Status:                                                              */
/*    BUTTON_NOOP                                                                   */
/*                                                                                  */
/*Scope:                                                                            */
/*    PUBLIC                                                                        */
/*                                                                                  */
/*Limitations/Assumptions:                                                          */
/*                                                                                  */
/*-----------------------------------------------------------------------------     */

/* BEGIN:                                                                           */
#if USE_PROTOTYPES
publicdef INT le_join_stroke (LE_CONTROL *pctrl)
#else
publicdef INT le_join_stroke (pctrl)
LE_CONTROL *pctrl;
#endif
    {
    UINT stroke1, stroke2, stroke_picked, point_picked, point_picked1, point_picked2,
        last_point1, last_point2;
    UINT i, j;
    INT but, numb, oper;
    UINT cnt, point_scratch, scratch_str1;
    INT status;
    GKS_REAL firstx, firsty, secondx, secondy, wx, wy, x, y, x_scratch, y_scratch;
    BOOL loop, noexit, enter_join;
    LE_KEYS *pdone, *pcancel;
    INT nb_option = 7;
    
    /*  testing first if there are some strokes in the n-list                       */
    
    nl_inq_nlist_int (pctrl->nlist, NL_NLIST_NSTROKES, &numb);
    if (numb EQUALS 0)
        {
        do_update = TRUE;
        curr_err = LE_NO_STROKES;
        }
    else
        {
        /* change prompt for done and cancel keys                                   */
        lez_change_key_label (&pdone, &pcancel, pctrl, LE_PROMPT_JOIN);
        
        /*  dim operation keys not used in this function                            */
        lez_enable_option (pctrl, option_array2, nb_option, OFF);
        
        /*  we have strokes in the nlist so we can try to select                    */
        /*a part of stroke                                                          */
        
        numb = 0;
        pctrl->nbp = 0;
        cnt = 0;
        stroke1 = 0;
        stroke2 = 0;
        enter_join = TRUE;
        
        /*   boolean enter_join is used not to asked for a point                    */
        /* when we enter in join stroke but to take the point                       */
        /* digitized when selecting the function join_stroke                        */
        
        wx = pctrl->wx;
        wy = pctrl->wy;
        
        do
            {
            /* the first time we enter this loop,                                   */
            /*  we don't need to digitize a point;                                  */
            /*  we use the point digitized                                          */
            /*  when choosing the function join_stroke                              */
            
            if (!enter_join)
                {
                lez_control_but (&status, firstx, firsty, &wx, &wy, &but, numb, pctrl,
                                 option_array2, nb_option, &oper, (UINT)0, (UINT)0, (UINT)0);
                }
            else
                {
                oper = BUTTON_JOIN_STROKE;
                enter_join = FALSE;
                }
            /* process the point and function coming from                           */
            /*   rqbutxy                                                            */
            if (oper EQUALS BUTTON_CANCEL)
                {
                loop = FALSE;
                noexit = FALSE;
                }
            else
                {
                noexit = TRUE;
                if ((oper EQUALS BUTTON_DONE) AND (cnt EQUALS 2))
                    {
                    loop = FALSE;
                    }
                else
                    {
                    lez_find_stroke_end (pctrl, wx, wy, &stroke_picked, &point_picked);
                    if (stroke_picked IS_NOT_EQUAL_TO 0)
                        {
                        if ((cnt < 2) AND (stroke_picked IS_NOT_EQUAL_TO stroke1))
                            {
                            cnt += 1;
                            }
                        point_scratch = point_picked;
                        nl_set_current_point (pctrl->nlist, point_scratch,
                                              stroke_picked);
                        GET_CURRENT_XY (pctrl->nlist, &x_scratch, &y_scratch);
                        if (stroke_picked EQUALS stroke1)
                            {
                            point_picked1 = point_scratch;
                            if (cnt > 0)
                                lez_control_draw_point (pctrl->nlist, firstx, firsty,
                                                        &pctrl->erase_attribute);
                            firstx = x_scratch;
                            firsty = y_scratch;
                            }
                        if (stroke_picked EQUALS stroke2)
                            {
                            point_picked2 = point_scratch;
                            if (cnt > 1)
                                lez_control_draw_point (pctrl->nlist, secondx, secondy,
                                                        &pctrl->erase_attribute);
                            secondx = x_scratch;
                            secondy = y_scratch;
                            }
                        if ((
             stroke_picked IS_NOT_EQUAL_TO stroke1) AND (
                                                 stroke_picked IS_NOT_EQUAL_TO 
                                                         stroke2))
                            {
                            if (cnt EQUALS 2)
                                lez_control_draw_point (pctrl->nlist, secondx, secondy,
                                                        &pctrl->erase_attribute);
                            secondx = firstx;
                            secondy = firsty;
                            firstx = x_scratch;
                            firsty = y_scratch;
                            stroke2 = stroke1;
                            stroke1 = stroke_picked;
                            point_picked2 = point_picked1;
                            point_picked1 = point_scratch;
                            }
                        lez_control_draw_point (pctrl->nlist, x_scratch, y_scratch,
                                                &pctrl->hilight_attribute);
                        }
                    else            /*   no stroke found   errro message            */
                        curr_err = LE_NO_STROKE_FOUND;
                    do_update = TRUE;
                    lez_update (cur_stroke_id[pctrl->index_stroke_keym],
                                pctrl->default_puck_map);
                    loop = TRUE;
                    }
                }
            } while (loop);
        
        if (cnt > 0)
            lez_control_draw_point (pctrl->nlist, firstx, firsty,
                                    &pctrl->erase_attribute);
        if (cnt EQUALS 2)
            lez_control_draw_point (pctrl->nlist, secondx, secondy,
                                    &pctrl->erase_attribute);
        
        if (noexit AND cnt EQUALS 2)
            {
            COUNT_STROKE_POINTS (pctrl->nlist, stroke1, &last_point1);
            COUNT_STROKE_POINTS (pctrl->nlist, stroke2, &last_point2);
            
            if ((last_point1 EQUALS point_picked1) AND (point_picked2 EQUALS 1))
                {
                ADD_STROKE (pctrl->nlist, stroke2, 1, pctrl->nlist, stroke1,
                            last_point2, last_point1);
                lez_draw_stroke (pctrl->nlist, stroke1, &pctrl->draw_attribute);
                nl_delete_stroke (pctrl->nlist, stroke2);
                }
            if ((point_picked1 EQUALS 1) AND (last_point2 EQUALS point_picked2))
                {
                ADD_STROKE (pctrl->nlist, stroke1, 1, pctrl->nlist, stroke2,
                            last_point1, last_point2);
                lez_draw_stroke (pctrl->nlist, stroke2, &pctrl->draw_attribute);
                nl_delete_stroke (pctrl->nlist, stroke1);
                }
            if ((last_point1 EQUALS point_picked1) AND (
                                                      last_point2 EQUALS 
                                                        point_picked2))
                {
                ADD_REVERSE_STROKE (pctrl->nlist, stroke1, last_point1, pctrl->nlist,
                                    stroke2, last_point1, last_point2);
                lez_draw_stroke (pctrl->nlist, stroke2, &pctrl->draw_attribute);
                nl_delete_stroke (pctrl->nlist, stroke1);
                }
            if ((point_picked1 EQUALS 1) AND (point_picked2 EQUALS 1))
                {
                nl_start_stroke (pctrl->nlist, &scratch_str1);
                ADD_REVERSE_STROKE (pctrl->nlist, stroke2, last_point2, pctrl->nlist,
                                    scratch_str1, last_point2, 0);
                ADD_STROKE (pctrl->nlist, stroke1, 1, pctrl->nlist, scratch_str1,
                            last_point1, last_point2);
                lez_draw_stroke (pctrl->nlist, scratch_str1, &pctrl->draw_attribute);
                
                /* as we delete two strokes consecutively we need to delete first th e higher   */
                /*   stroke number because of the nlist stroke structure            */
                
                if (stroke1 > stroke2)
                    {
                    nl_delete_stroke (pctrl->nlist, stroke1);
                    nl_delete_stroke (pctrl->nlist, stroke2);
                    }
                else
                    {
                    nl_delete_stroke (pctrl->nlist, stroke2);
                    nl_delete_stroke (pctrl->nlist, stroke1);
                    }
                }
            pctrl->stroke_mode = TRUE;
            pctrl->nbp = 0;
            
            }
        else
            {
            /*   the operation was cancelled we restore the initial stroke          */
            
            lez_draw_stroke (pctrl->nlist, stroke_picked, &pctrl->draw_attribute);
            }
        /*restore default labels on cancel and done keys                            */
        lez_restor_default_prompt (pcancel, pdone, pctrl);
        
        /*  restore operation keys not used in this function                        */
        lez_enable_option (pctrl, option_array2, nb_option, ON);
        }
    return BUTTON_NOOP;
    }
/* END:                                                                             */

/* Function Description -----------------------------------------------------       */
/*Description:                                                                      */
/*    Interactively  move a point coosen in the stroke we are pointing at.          */
/*    In this function zoom pan reset are available                                 */
/*        cancel   cancel the operation                                             */
/*        done   confirm the operation                                              */
/*                                                                                  */
/*Prototype:                                                                        */
/*    publicdef INT le_move_point (LE_CONTROL *pctrl);                              */
/*                                                                                  */
/*Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)        */
/*    pctrl           -(LE_CONTROL *)                                               */
/*                                                                                  */
/*Return Value/Status:                                                              */
/*    BUTTON_NOOP                                                                   */
/*                                                                                  */
/*Scope:                                                                            */
/*    PUBLIC                                                                        */
/*                                                                                  */
/*Limitations/Assumptions:                                                          */
/*                                                                                  */
/*-----------------------------------------------------------------------------     */

/* BEGIN:                                                                           */
#if USE_PROTOTYPES
publicdef INT le_move_point (LE_CONTROL *pctrl)
#else
publicdef INT le_move_point (pctrl)
LE_CONTROL *pctrl;
#endif
    {
    UINT stroke_picked, stroke1, point_picked, last_point;
    INT but, oper, numb, status;
    GKS_REAL wx, wy, firstx, firsty;
    BOOL loop;
    UINT p1, p2;
    INT nb_option = 7;
    LE_KEYS *pdone, *pcancel;
    
    /* --------------------------------------                                       */
    /* Redraw the current line to replace any                                       */
    /* pixels erased by the mouse menu                                              */
    /* --------------------------------------                                       */
    if (if_mouse_menus)
        {
        lez_draw_line (pctrl->nlist, &pctrl->draw_attribute);
        }
    /*  look for the point to move                                                  */
    
    lez_find_stroke (pctrl, pctrl->wx, pctrl->wy, &stroke_picked, &point_picked);
    if (stroke_picked IS_NOT_EQUAL_TO 0)
        {
        /* change prompt for key cancel and done                                    */
        
        lez_change_key_label (&pdone, &pcancel, pctrl, LE_PROMPT_MOVE);
        
        /*  dim operation keys not used in this function                            */
        
        lez_enable_option (pctrl, option_array1, nb_option, OFF);
        numb = 1;
        COPY_STROKE (pctrl->nlist, stroke_picked, &stroke1);
        
        if (point_picked EQUALS 1)
            {
            nl_set_current_point (pctrl->nlist, (UINT)2, stroke1);
            }
        else
            {
            nl_set_current_point (pctrl->nlist, point_picked - 1, stroke1);
            }
        GET_CURRENT_XY (pctrl->nlist, &firstx, &firsty);
        COUNT_STROKE_POINTS (pctrl->nlist, stroke1, &last_point);
        
        /* looking for part to replace                                              */
        
        if (point_picked EQUALS 1)
            {
            p1 = 1;
            p2 = 2;
            }
        else if (point_picked EQUALS last_point)
            {
            p1 = last_point - 1;
            p2 = last_point;
            }
        else
            {
            p1 = point_picked - 1;
            p2 = point_picked + 1;
            }
        do
            {
            lez_control_but (&status, firstx, firsty, &wx, &wy, &but, numb, pctrl,
                             option_array1, nb_option, &oper, stroke1, p1, p2);
            if (oper EQUALS BUTTON_CANCEL)
                {
                loop = FALSE;
                lez_draw_stroke (pctrl->nlist, stroke1, &pctrl->erase_attribute);
                lez_draw_stroke (pctrl->nlist, stroke_picked, &pctrl->draw_attribute);
                nl_delete_stroke (pctrl->nlist, stroke1);
                }
            else
                {
                loop = TRUE;
                lez_hilight_part (pctrl->nlist, stroke1, p1, p2,
                                  &pctrl->erase_attribute);
                nl_set_current_point (pctrl->nlist, point_picked, stroke1);
                MOVE_POINT (pctrl->nlist, wx, wy);
                lez_hilight_part (pctrl->nlist, stroke1, p1, p2,
                                  &pctrl->hilight_attribute);
                if (oper EQUALS BUTTON_DONE)
                    {
                    loop = FALSE;
                    lez_draw_stroke (pctrl->nlist, stroke_picked,
                                     &pctrl->erase_attribute);
                    lez_draw_stroke (pctrl->nlist, stroke1, &pctrl->draw_attribute);
                    nl_delete_stroke (pctrl->nlist, stroke_picked);
                    }
                }
            } while (loop);
        
        /*restore default labels on cancel and done keys                            */
        
        lez_restor_default_prompt (pcancel, pdone, pctrl);
        
        /*  restore operation keys not used in this function                        */
        
        lez_enable_option (pctrl, option_array1, nb_option, ON);
        }
    else
        {
        do_update = TRUE;
        curr_err = LE_NO_STROKE_FOUND;
        }
    return BUTTON_NOOP;
    }
/* END:                                                                             */

/* Function Description -----------------------------------------------------       */
/*Description:                                                                      */
/*                                                                                  */
/*Prototype:                                                                        */
/*    publicdef INT le_nooperation ();                                              */
/*                                                                                  */
/*Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)        */
/*    None.                                                                         */
/*                                                                                  */
/*Return Value/Status:                                                              */
/*    None.                                                                         */
/*                                                                                  */
/*Scope:                                                                            */
/*    PUBLIC                                                                        */
/*                                                                                  */
/*Limitations/Assumptions:                                                          */
/*                                                                                  */
/*-----------------------------------------------------------------------------     */

/* BEGIN:                                                                           */
#if USE_PROTOTYPES
publicdef INT le_nooperation (LE_CONTROL *pctrl)
#else
publicdef INT le_nooperation (pctrl)
LE_CONTROL *pctrl;
#endif
    {
    
    return BUTTON_NOOP;
    }
/* END:                                                                             */

/* Function Description -----------------------------------------------------       */
/*Description:                                                                      */
/*    Pan the current window.                                                       */
/*                                                                                  */
/*Prototype:                                                                        */
/*    publicdef INT le_pan(LE_CONTROL *pctrl);                                      */
/*                                                                                  */
/*Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)        */
/*    pctrl           -(LE_CONTROL *)                                               */
/*                                                                                  */
/*Return Value/Status:                                                              */
/*    BUTTON_NOOP                                                                   */
/*                                                                                  */
/*Scope:                                                                            */
/*    PUBLIC                                                                        */
/*                                                                                  */
/*Limitations/Assumptions:                                                          */
/*                                                                                  */
/*-----------------------------------------------------------------------------     */

/* BEGIN:                                                                           */
#if USE_PROTOTYPES
publicdef INT le_pan (LE_CONTROL *pctrl)
#else
publicdef INT le_pan (pctrl)
LE_CONTROL *pctrl;
#endif
    {
    
    wmtdopuckkey (pctrl->wx, pctrl->wy, WM_BUTTON_PAN);
    wmtselect (pctrl->transform_id);    /* reselect the current transform           */
    lez_draw_line (pctrl->nlist, &pctrl->draw_attribute);
    
    return BUTTON_NOOP;
    }
/* END:                                                                             */

/* Function Description -----------------------------------------------------       */
/*Description:                                                                      */
/*    Interactively choose a part of stroke in the stroke we are pointing at        */
/*    then go to point mode where the operator can use the current functions        */
/*    to digitize the part to replace.                                              */
/*    In this function, zoom pan reset are available                                */
/*        cancel   cancel the operation                                             */
/*        done   confirm the operation                                              */
/*                                                                                  */
/*    REPLACE_STROKE has been hit and the point returned is used to localize the    */
/*    nearest end of the segment to replace.                                        */
/*                                                                                  */
/*Prototype:                                                                        */
/*    publicdef INT le_replace_stroke(LE_CONTROL *pctrl);                           */
/*                                                                                  */
/*Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)        */
/*    pctrl           -(LE_CONTROL *)                                               */
/*                                                                                  */
/*Return Value/Status:                                                              */
/*    BUTTON_NOOP                                                                   */
/*                                                                                  */
/*Scope:                                                                            */
/*    PUBLIC                                                                        */
/*                                                                                  */
/*Limitations/Assumptions:                                                          */
/*                                                                                  */
/*-----------------------------------------------------------------------------     */

/* BEGIN:                                                                           */
#if USE_PROTOTYPES
publicdef INT le_replace_stroke (LE_CONTROL *pctrl)
#else
publicdef INT le_replace_stroke (pctrl)
LE_CONTROL *pctrl;
#endif
    {
    UINT maxi, mini, numpoints, i, j;
    INT numb;
    GKS_REAL x, y;
    INT nb_option = 7;
    UINT stroke_picked, cnt, first_point, second_point;
    BOOL noexit;
    LE_KEYS *pdone, *pcancel;
    
    /*  testing first if there are some strokes in the n-list                       */
    
    nl_inq_nlist_int (pctrl->nlist, NL_NLIST_NSTROKES, &numb);
    if (numb EQUALS 0)
        {
        do_update = TRUE;
        curr_err = LE_NO_STROKES;
        }
    else
        {
        /* change prompt for key cancel and done                                    */
        
        lez_change_key_label (&pdone, &pcancel, pctrl, LE_PROMPT_REPLACE);
        
        /*  dimmed keys not used in this function                                   */
        
        lez_enable_option (pctrl, option_array, nb_option, OFF);
        
        /*  select the part of line to replace                                      */
        
        lez_sel_part (&first_point, &second_point, &stroke_picked, &noexit, &cnt,
                      pctrl, BUTTON_REPLACE_STROKE, option_array, nb_option);
        if (noexit)
            {
            /*   prompt for digitizing the part to replace                          */
            
            if (pctrl->default_puck_map)
                lez_prompt (cur_stroke_id[pctrl->index_stroke_keym],
                            mg_message (LE_PROMPT_DIGIT));
            
            /*  update control structure                                            */
            
            COUNT_STROKE_POINTS (pctrl->nlist, stroke_picked, &numpoints);
            
            /*   choosing the point where the replace will begin                    */
            /*   we don't take an end of stroke  unless the two ends (= the whole stroke)   */
            /*   was selected to be replaced                                        */
            
            if ((
               first_point IS_NOT_EQUAL_TO 1 AND first_point IS_NOT_EQUAL_TO numpoints) 
                OR (second_point EQUALS 1 OR second_point EQUALS numpoints))
                {
                pctrl->current_point = first_point;
                pctrl->last_point = second_point;
                }
            else
                {
                pctrl->current_point = second_point;
                pctrl->last_point = first_point;
                }
            pctrl->current_stroke = stroke_picked;
            
            /*  look for end of segment to replace closest to point picked when     */
            /*hitting replace stroke,the replace will begin from this point         */
            
            ((pctrl->current_point < pctrl->last_point) ? (mini = pctrl->current_point,
                                                           maxi = pctrl->last_point) :
                (mini = pctrl->last_point, maxi = pctrl->current_point));
            
            pctrl->reverse = (pctrl->current_point EQUALS maxi);
            if (pctrl->reverse)
                {
                nl_start_stroke (pctrl->nlist, &pctrl->scratch_str1);
                for (i = numpoints, j = 0; i > maxi - 1; i--, j++)
                    {
                    nl_set_current_point (pctrl->nlist, i, stroke_picked);
                    GET_CURRENT_XY (pctrl->nlist, &x, &y);
                    nl_set_current_point (pctrl->nlist, j, pctrl->scratch_str1);
                    ADD_POINT (pctrl->nlist, x, y);
                    }
                }
            else
                {
                COPY_STROKE (pctrl->nlist, stroke_picked, &pctrl->scratch_str1);
                numpoints -= mini;
                DEL_PART_OF_STROKE (pctrl->nlist, pctrl->scratch_str1, mini + 1,
                                    numpoints);
                }
            COUNT_STROKE_POINTS (pctrl->nlist, pctrl->scratch_str1,
                                 &pctrl->curpoint_str1);
            nl_set_current_point (pctrl->nlist, pctrl->curpoint_str1,
                                  pctrl->scratch_str1);
            GET_CURRENT_XY (pctrl->nlist, &pctrl->lastx, &pctrl->lasty);
            pctrl->stroke_mode = FALSE;
            pctrl->dial_update = TRUE;
            pctrl->nbp = 1;
            }
        /*  restore default prompts for done and cancel keys                        */
        
        lez_restor_default_prompt (pcancel, pdone, pctrl);
        
        /*  restor option dimmed                                                    */
        
        lez_enable_option (pctrl, option_array, nb_option, ON);
        }
    return BUTTON_NOOP;
    }
/* END:                                                                             */

/* Function Description -----------------------------------------------------       */
/*Description:                                                                      */
/*    Reset the current window back to 'normal'.                                    */
/*                                                                                  */
/*Prototype:                                                                        */
/*    publicdef INT le_reset(LE_CONTROL *pctrl);                                    */
/*                                                                                  */
/*Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)        */
/*    pctrl           -(LE_CONTROL *)                                               */
/*                                                                                  */
/*Return Value/Status:                                                              */
/*    BUTTON_NOOP                                                                   */
/*                                                                                  */
/*Scope:                                                                            */
/*    PUBLIC                                                                        */
/*                                                                                  */
/*Limitations/Assumptions:                                                          */
/*                                                                                  */
/*-----------------------------------------------------------------------------     */

/* BEGIN:                                                                           */
#if USE_PROTOTYPES
publicdef INT le_reset (LE_CONTROL *pctrl)
#else
publicdef INT le_reset (pctrl)
LE_CONTROL *pctrl;
#endif
    {
    
    wmtdopuckkey (0.0, 0.0, WM_BUTTON_RESET);
    wmtselect (pctrl->transform_id);    /* reselect the current transform           */
    
    lez_draw_line (pctrl->nlist, &(pctrl->draw_attribute));
    return BUTTON_NOOP;
    }
/* END:                                                                             */

/* Function Description -----------------------------------------------------       */
/*Description:                                                                      */
/*                                                                                  */
/*Prototype:                                                                        */
/*    publicdef INT le_select_stroke(LE_CONTROL *pctrl);                            */
/*                                                                                  */
/*Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)        */
/*    pctrl           -(LE_CONTROL *)                                               */
/*                                                                                  */
/*Return Value/Status:                                                              */
/*    BUTTON_NOOP                                                                   */
/*                                                                                  */
/*Scope:                                                                            */
/*    PUBLIC                                                                        */
/*                                                                                  */
/*Limitations/Assumptions:                                                          */
/*                                                                                  */
/*-----------------------------------------------------------------------------     */

/* BEGIN:                                                                           */
#if USE_PROTOTYPES
publicdef INT le_select_stroke (LE_CONTROL *pctrl)
#else
publicdef INT le_select_stroke (pctrl)
LE_CONTROL *pctrl;
#endif
    {
    
    pctrl->last_stroke = pctrl->current_stroke;
    pctrl->last_point = pctrl->current_point;
    pctrl->current_stroke = 0;
    lez_find_stroke (pctrl, pctrl->wx, pctrl->wy, &pctrl->current_stroke,
                     &pctrl->current_point);
    
    /* if a new stroke was selected we change the color of last current to          */
    /*draw_color                                                                    */
    
    lez_draw_stroke (pctrl->nlist, pctrl->last_stroke, &pctrl->draw_attribute);
    if (pctrl->current_stroke EQUALS 0)
        {
        curr_err = LE_NO_STROKE_FOUND;
        do_update = TRUE;
        }
    else
        {
        GET_CURRENT_XY (pctrl->nlist, &pctrl->wx, &pctrl->wy);
        }
    return BUTTON_NOOP;
    }
/* END:                                                                             */

/* Function Description -----------------------------------------------------       */
/*Description:                                                                      */
/*                                                                                  */
/*Prototype:                                                                        */
/*    publicdef INT le_set_param(LE_CONTROL *pctrl);                                */
/*                                                                                  */
/*Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)        */
/*    pctrl           -(LE_CONTROL *)                                               */
/*                                                                                  */
/*Return Value/Status:                                                              */
/*    BUTTON_NOOP                                                                   */
/*                                                                                  */
/*Scope:                                                                            */
/*    PUBLIC                                                                        */
/*                                                                                  */
/*Limitations/Assumptions:                                                          */
/*                                                                                  */
/*-----------------------------------------------------------------------------     */

/* BEGIN:                                                                           */
#if USE_PROTOTYPES
publicdef INT le_set_param (LE_CONTROL *pctrl)
#else
publicdef INT le_set_param (pctrl)
LE_CONTROL *pctrl;
#endif
    {
    
    do_update = TRUE;
    curr_err = LE_NOT_IMPLEMENTED;
    return BUTTON_NOOP;
    }
/* END:                                                                             */

/* Function Description -----------------------------------------------------       */
/*Description:                                                                      */
/*    Start a new stroke called the scratch stroke which will be used in point      */
/*    mode to store digitized or computed points.                                   */
/*                                                                                  */
/*Prototype:                                                                        */
/*    publicdef INT le_start_stroke (LE_CONTROL *pctrl);                            */
/*                                                                                  */
/*Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)        */
/*    pctrl           -(LE_CONTROL *)                                               */
/*                                                                                  */
/*Return Value/Status:                                                              */
/*    BUTTON_ADD_POINT                                                              */
/*                                                                                  */
/*Scope:                                                                            */
/*    PUBLIC                                                                        */
/*                                                                                  */
/*Limitations/Assumptions:                                                          */
/*                                                                                  */
/*-----------------------------------------------------------------------------     */

/* BEGIN:                                                                           */
#if USE_PROTOTYPES
publicdef INT le_start_stroke (LE_CONTROL *pctrl)
#else
publicdef INT le_start_stroke (pctrl)
LE_CONTROL *pctrl;
#endif
    {
    LE_KEYS *pkey1, *pkey2;
    
    /* start a new stroke which will be the new current user stroke                 */
    /* this stroke will be a scratch stroke until DONE  and will be deleted if      */
    /* CANCELof the operation                                                       */
    
    nl_start_stroke (pctrl->nlist, &pctrl->scratch_str1);
    
    if (pctrl->key_swap AND pctrl->stroke_mode)
        {
        /*  we configure the point keymap so that the add point key is on the same key  */
        /*    than the start stroke key                                             */
        /* first look for the button for the start_stroke key                       */
        
        pkey2 = pctrl->stroke_keymap + (pctrl->index_stroke_keym * MAX_KEYS);
        pkey1 = le_scan_keymap (pkey2, (INT)1, BUTTON_START_STROKE);
        
        if (pkey1 IS_NOT_EQUAL_TO (LE_KEYS *)0)
            {
            /* pointer to the structure of point keymap corresponding to the same   */
            /* key                                                                  */
            
            pkey2 = pctrl->point_keymap + (pctrl->index_point_keym * MAX_KEYS) +
                pkey1->button;
            lez_swap_but (FALSE, pkey2->operation, BUTTON_ADD_POINT, pctrl);
            }
        }
    pctrl->stroke_mode = FALSE;
    pctrl->dial_update = TRUE;
    return BUTTON_ADD_POINT;
    }
/* END:                                                                             */

/* Function Description -----------------------------------------------------       */
/*Description:                                                                      */
/*    %S start a new stroke while in point mode.                                    */
/*                                                                                  */
/*Prototype:                                                                        */
/*    publicdef INT le_start_stroke_pt_mode(LE_CONTROL *pctrl);                     */
/*                                                                                  */
/*Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)        */
/*    pctrl           -(LE_CONTROL *)                                               */
/*                                                                                  */
/*Return Value/Status:                                                              */
/*    Function returns the value of operation.                                      */
/*                                                                                  */
/*Scope:                                                                            */
/*    PUBLIC                                                                        */
/*                                                                                  */
/*Limitations/Assumptions:                                                          */
/*                                                                                  */
/*-----------------------------------------------------------------------------     */

/* BEGIN:                                                                           */
#if USE_PROTOTYPES
publicdef INT le_start_stroke_pt_mode (LE_CONTROL *pctrl)
#else
publicdef INT le_start_stroke_pt_mode (pctrl)
LE_CONTROL *pctrl;
#endif
    {
    INT operation;
    LE_KEYS *pKey;    
    
    le_done (pctrl);
    
    /* we need to reset pctrl -> stroke_mode to FALSE in order not to swap twice    */
    /*the keys on the point mode keymap if bool key_swap is true                    */
    
    pctrl->stroke_mode = FALSE;
    operation = le_start_stroke (pctrl);
    pctrl->dial_update = FALSE;
    
    /*   reset  current stroke operation                                            */
    
    pctrl->last_operation = pctrl->current_operation;
    pctrl->current_operation = BUTTON_START_STROKE;
    if (pctrl->key_swap)
        {
        /*we want to automatically swap the keys break stroke and add point         */
        /*  so that the operator does not have to change                            */
        /*  the key to go on digitizing                                             */
        
        lez_swap_but (FALSE, BUTTON_ADD_POINT, BUTTON_BREAK_STROKE, pctrl);
        }

    do
      {
	lez_rqbutxy (pctrl->nlist, &pctrl->stat, pctrl->lastx, 
		     pctrl->lasty, &pctrl->wx, &pctrl->wy, 
		     &pctrl->but, pctrl->nbp);
	pKey = lez_translate (pctrl->but, pctrl->point_keymap,
			      pctrl->index_point_keym);
      }
    while (pKey->operation EQUALS BUTTON_BREAK_STROKE AND
	   wm_is_menu_button() EQUALS TRUE);

    return operation;
    }
/* END:                                                                             */

/* Function Description -----------------------------------------------------       */
/*Description:                                                                      */
/*                                                                                  */
/*Prototype:                                                                        */
/*    publicdef INT le_undefined (LE_CONTROL *pctrl);                               */
/*                                                                                  */
/*Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)        */
/*    pctrl           -(LE_CONTROL *)                                               */
/*                                                                                  */
/*Return Value/Status:                                                              */
/*    BUTTON_NOOP                                                                   */
/*                                                                                  */
/*Scope:                                                                            */
/*    PUBLIC                                                                        */
/*                                                                                  */
/*Limitations/Assumptions:                                                          */
/*                                                                                  */
/*-----------------------------------------------------------------------------     */

/* BEGIN:                                                                           */
#if USE_PROTOTYPES
publicdef INT le_undefined (LE_CONTROL *pctrl)
#else
publicdef INT le_undefined (pctrl)
LE_CONTROL *pctrl;
#endif
    {
    
    if (pctrl->current_operation IS_NOT_EQUAL_TO BUTTON_UNDO)
        {
        do_update = TRUE;
        curr_err = LE_BUTTON_NOT_USED;
        }
    return BUTTON_NOOP;
    }
/* END:                                                                             */

/* Function Description -----------------------------------------------------       */
/*Description:                                                                      */
/*    Zoom in the current window.                                                   */
/*                                                                                  */
/*Prototype:                                                                        */
/*    publicdef INT le_zoomin(LE_CONTROL *pctrl);                                   */
/*                                                                                  */
/*Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)        */
/*    pctrl           -(LE_CONTROL *)                                               */
/*                                                                                  */
/*Return Value/Status:                                                              */
/*    BUTTON_NOOP                                                                   */
/*                                                                                  */
/*Scope:                                                                            */
/*    PUBLIC                                                                        */
/*                                                                                  */
/*Limitations/Assumptions:                                                          */
/*                                                                                  */
/*-----------------------------------------------------------------------------     */

/* BEGIN:                                                                           */
#if USE_PROTOTYPES
publicdef INT le_zoomin (LE_CONTROL *pctrl)
#else
publicdef INT le_zoomin (pctrl)
LE_CONTROL *pctrl;
#endif
    {
    
    wmtdopuckkey (pctrl->wx, pctrl->wy, WM_BUTTON_ZOOMIN);
    wmtselect (pctrl->transform_id);    /* reselect the current transform           */
    
    lez_draw_line (pctrl->nlist, &pctrl->draw_attribute);
    return BUTTON_NOOP;
    }
/* END:                                                                             */

/* Function Description -----------------------------------------------------       */
/*Description:                                                                      */
/*    Zoom out the current window.                                                  */
/*                                                                                  */
/*Prototype:                                                                        */
/*    publicdef INT le_zoomout(LE_CONTROL *pctrl);                                  */
/*                                                                                  */
/*Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)        */
/*    pctrl           -(LE_CONTROL *)                                               */
/*                                                                                  */
/*Return Value/Status:                                                              */
/*    Function returns the value from le_reset.                                     */
/*                                                                                  */
/*Scope:                                                                            */
/*    PUBLIC                                                                        */
/*                                                                                  */
/*Limitations/Assumptions:                                                          */
/*                                                                                  */
/*-----------------------------------------------------------------------------     */

/* BEGIN:                                                                           */
#if USE_PROTOTYPES
publicdef INT le_zoomout (LE_CONTROL *pctrl)
#else
publicdef INT le_zoomout (pctrl)
LE_CONTROL *pctrl;
#endif
    {
    return le_reset (pctrl);
    }
/* END:                                                                             */

/************************************************************************************/
/*                                                                                  */
/*                           INTERNAL ROUTINES                                      */
/*                                                                                  */
/************************************************************************************/

/* Function Description -----------------------------------------------------       */
/*Description:                                                                      */
/*    Change the labels of the keys corresponding to BUTTON_DONE AND BUTTON_CANCEL. */
/*                                                                                  */
/*Prototype:                                                                        */
/*    static VOID lez_change_key_label (LE_KEYS **pdone, LE_KEYS **pcancel,     */
/*                                        LE_CONTROL *pctrl, INT index_prompt);     */
/*                                                                                  */
/*Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)        */
/*    (O) pdone       -(LE_KEYS **) Address of pointer to the structure defining    */
/*                        BUTTON_DONE.                                              */
/*    (O) pcancel     -(LE_KEYS **) Address of pointer to the structure defining    */
/*                        CANCEL.                                                   */
/*    (O) pctrl       -(LE_CONTROL **) Control structure.                           */
/*    (O) index_prompt-(INT) Index in static key_prompt of the first label to display   */
/*                        (cancel label, done label and a general prompt label).    */
/*                                                                                  */
/*Return Value/Status:                                                              */
/*    None.                                                                         */
/*                                                                                  */
/*Scope:                                                                            */
/*    PRIVATE to <le_nlist_ed.c>                                                    */
/*                                                                                  */
/*Limitations/Assumptions:                                                          */
/*                                                                                  */
/*-----------------------------------------------------------------------------     */

/* BEGIN:                                                                           */
#if USE_PROTOTYPES
static VOID lez_change_key_label (LE_KEYS **pdone, LE_KEYS **pcancel,
                                      LE_CONTROL *pctrl, INT index_prompt)
#else
static VOID lez_change_key_label (pdone, pcancel, pctrl, index_prompt)
LE_CONTROL *pctrl;
LE_KEYS **pdone, **pcancel;
INT index_prompt;
#endif
    {
    LE_KEYS *pKey;
    
    /*  we update the keymaps for key done and cancel only if the                   */
    /*  keymap is used (pctrl -> default_puck_map is TRUE)                          */
    
    if (pctrl->default_puck_map)
        {
        /*  pkey is the pointer to the first structure  of the active stroke keymap */
        
        pKey = pctrl->stroke_keymap + (pctrl->index_stroke_keym * MAX_KEYS);
        
        /* looking for cancel key                                                   */
        
        *pcancel = le_scan_keymap (pKey, (INT)1, BUTTON_CANCEL);
        
        /*  if we find the  button CANCEL  we replace its label                     */
        
        if (*pcancel IS_NOT_EQUAL_TO (LE_KEYS *)0)
            lez_upd_key_prompt (cur_stroke_id[pctrl->index_stroke_keym],
                                (*pcancel)->button, mg_message (index_prompt));
        
        /* looking for done key                                                     */
        
        *pdone = le_scan_keymap (pKey, (INT)1, BUTTON_DONE);
        
        /*  if we find the  button DONE  we replace its label                       */
        
        if (*pdone IS_NOT_EQUAL_TO (LE_KEYS *)0)
            lez_upd_key_prompt (cur_stroke_id[pctrl->index_stroke_keym],
                                (*pdone)->button, mg_message (index_prompt));
        
        /*  update prompt                                                           */
        
        lez_prompt (cur_stroke_id[pctrl->index_stroke_keym],
                    mg_message (index_prompt + 2));
        }
    }
/* END:                                                                             */

/* Function Description -----------------------------------------------------       */
/*Description:                                                                      */
/*    Routine used to ensure that the key is an available key pertaining to the array   */
/*    of available keys  - this function traps the zoom, pan, and reset function.  It   */
/*    executes this function and ask for a new key function.                        */
/*                                                                                  */
/*Prototype:                                                                        */
/*    static VOID lez_control_but (INT *stat, DOUBLE lastx, DOUBLE lasty, GKS_REAL *wx,    */
/*                                GKS_REAL *wy, INT *but, INT nbp, LE_CONTROL *pctrl,  */
/*                                INT option_array [], INT nb_option, INT *oper,    */
/*                                UINT stroke_picked, UINT first_point, UINT second_point); */
/*                                                                                  */
/*Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)        */
/*    (I) option_array    -(INT []) The list of the operations avalible.            */
/*    (I) nb_option       -(INT) The number of options available in the array.      */
/*    (I) nbp             -(INT) If 0/1 use crosshair/rubberbanding.                */
/*    (I) lastx           -(DOUBLE) Coordinates of the point used for rubberbanding.*/
/*    (I) lasty           -(DOUBLE) Coordinates of the point used for rubberbanding.*/
/*    (I) stroke_picked   -(UINT) The current stroke we are pointing at.            */
/*    (I) first_point     -(UINT) If #0 give the first point of a part selected     */
/*                            in stroke picked                                      */
/*    (I) second_point    -(UINT) If #0 give the second point of a part selected    */
/*                            in stroke picked (first and second point are used     */
/*                            for redrawing in zoom, pan, reset).                   */
/*    (I) pctrl           -(LE_CONTROL *) Control structure.                        */
/*    (O) stat            -(INT *) Status of rqbutxy.                               */
/*    (O) wx              -(GKS_REAL *) Coordinates of the point picked when pushing a key.    */
/*    (O) wy              -(GKS_REAL *) Coordinates of the point picked when pushing a key.    */
/*    (O) oper            -(INT *) The operation corresponding to key pushed.       */
/*                                                                                  */
/*Return Value/Status:                                                              */
/*    None.                                                                         */
/*                                                                                  */
/*Scope:                                                                            */
/*    PRIVATE to <le_nlist_ed.c>                                                    */
/*                                                                                  */
/*Limitations/Assumptions:                                                          */
/*                                                                                  */
/*-----------------------------------------------------------------------------     */

/* BEGIN:                                                                           */
#if USE_PROTOTYPES
static VOID lez_control_but (INT *stat, DOUBLE lastx, DOUBLE lasty, GKS_REAL *wx,
                                 GKS_REAL *wy, INT *but, INT nbp, LE_CONTROL *pctrl,
                                 INT option_array[], INT nb_option, INT *oper,
                                 UINT stroke_picked, UINT first_point,
                                 UINT second_point)
#else
static VOID lez_control_but (stat, lastx, lasty, wx, wy, but, nbp, pctrl,
                                 option_array, nb_option, oper, stroke_picked,
                                 first_point, second_point)
LE_CONTROL *pctrl;
INT option_array[];
INT nb_option;
INT nbp;
INT *stat, *but;
DOUBLE lastx, lasty;
GKS_REAL *wx, *wy;
INT *oper;
UINT stroke_picked, first_point, second_point;
#endif
    {
    LE_KEYS *pkey;
    BOOL loop;
    INT i;
    
    do
        {
        loop = TRUE;
        /*   request  of the button + coordinates of the crosshair                  */
        lez_rqbutxy (pctrl->nlist, stat, lastx, lasty, wx, wy, but, nbp);
        /*  look for the operation which was hit                                    */
        pkey = lez_translate (*but, pctrl->stroke_keymap, pctrl->index_stroke_keym);
        *oper = pkey->operation;
        /*   clean the error field on the dialog                                    */
        do_update = TRUE;
        curr_err = LE_NONE;
        lez_update (cur_stroke_id[pctrl->index_stroke_keym], pctrl->default_puck_map);
        /*  controlling that the operation which was hit is in the array of         */
        /*  allowed operations                                                      */
        for (i = 0; i < nb_option; i++)
            {
            if (*oper EQUALS option_array[i])
                {
                loop = FALSE;
                break;
                }
            }
        /*  in case of valid operation we test if the operation is zoom,pan,        */
        /*zoomout or reset window and perform this operations                       */
        if (!loop)
            {
            switch (*oper)
                {
            case BUTTON_ZOOMOUT:
                pctrl->wx = *wx;
                pctrl->wy = *wy;
                le_zoomout (pctrl);
                lez_hilight_part (pctrl->nlist, stroke_picked, first_point,
                                  second_point, &pctrl->hilight_attribute);
                loop = TRUE;
                break;
            case BUTTON_ZOOMIN:
                pctrl->wx = *wx;
                pctrl->wy = *wy;
                le_zoomin (pctrl);
                lez_hilight_part (pctrl->nlist, stroke_picked, first_point,
                                  second_point, &pctrl->hilight_attribute);
                loop = TRUE;
                break;
            case BUTTON_PAN:
                pctrl->wx = *wx;
                pctrl->wy = *wy;
                le_pan (pctrl);
                lez_hilight_part (pctrl->nlist, stroke_picked, first_point,
                                  second_point, &pctrl->hilight_attribute);
                loop = TRUE;
                break;
            case BUTTON_RESET:
                pctrl->wx = *wx;
                pctrl->wy = *wy;
                le_reset (pctrl);
                lez_hilight_part (pctrl->nlist, stroke_picked, first_point,
                                  second_point, &pctrl->hilight_attribute);
                loop = TRUE;
                break;
            default:
                break;
                }
            }
        else
            {
            /*   the operation is not allowed  error message                        */
            do_update = TRUE;
            curr_err = LE_BUTTON_NOT_USED;
            lez_update (cur_stroke_id[pctrl->index_stroke_keym],
                        pctrl->default_puck_map);
            }
        } while (loop);
    }
/* END:                                                                             */

/* Function Description -----------------------------------------------------       */
/*Description:                                                                      */
/*                                                                                  */
/*Prototype:                                                                        */
/*    static VOID lez_control_draw_vector(NLIST_HEADER line, DOUBLE wx1, DOUBLE wy1,    */
/*                                        DOUBLE wx2, DOUBLE wy2,                   */
/*                                        LINE_ATTRIBUTE *attribute);               */
/*                                                                                  */
/*Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)        */
/*    line            -(NLIST_HEADER)                                               */
/*    wx1             -(DOUBLE)                                                     */
/*    wx2             -(DOUBLE)                                                     */
/*    wy1             -(DOUBLE)                                                     */
/*    wy2             _(DOUBLE)                                                     */
/*    attribute       -(LINE_ATTRIBUTE *)                                           */
/*                                                                                  */
/*Return Value/Status:                                                              */
/*    None.                                                                         */
/*                                                                                  */
/*Scope:                                                                            */
/*    PRIVATE to <le_nlist_ed.c>                                                    */
/*                                                                                  */
/*Limitations/Assumptions:                                                          */
/*                                                                                  */
/*-----------------------------------------------------------------------------     */

/* BEGIN:                                                                           */
#if USE_PROTOTYPES
static VOID lez_control_draw_vector (NLIST_HEADER line, DOUBLE wx1, DOUBLE wy1,
                                         DOUBLE wx2, DOUBLE wy2,
                                         LINE_ATTRIBUTE *attribute)
#else
static VOID lez_control_draw_vector (line, wx1, wy1, wx2, wy2, attribute)
NLIST_HEADER line;
DOUBLE wx1, wx2, wy1, wy2;
LINE_ATTRIBUTE *attribute;
#endif
    {
    
    BOOL exist_null;
    INT nbpt = 2;
    
    lez_control_nonull (line, wx1, wy1, wx2, wy2, nbpt, &exist_null);
    
    /* if there is no null values in the coordinates we draw the vector             */
    
    if (!exist_null)
        lez_draw_vector (wx1, wy1, wx2, wy2, attribute);
    }
/* END:                                                                             */

/* Function Description -----------------------------------------------------       */
/*Description:                                                                      */
/*                                                                                  */
/*Prototype:                                                                        */
/*    static VOID lez_control_draw_point(NLIST_HEADER line, DOUBLE wx, DOUBLE wy,   */
/*                                    LINE_ATTRIBUTE *attribute);                   */
/*                                                                                  */
/*Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)        */
/*    line            -(NLIST_HEADER)                                               */
/*    wx              -(DOUBLE)                                                     */
/*    wy              -(DOUBLE)                                                     */
/*    attribute       -(LINE_ATTRIBUTE *)                                           */
/*                                                                                  */
/*Return Value/Status:                                                              */
/*    None.                                                                         */
/*                                                                                  */
/*Scope:                                                                            */
/*    PRIVATE to <le_nlist_ed.c>                                                    */
/*                                                                                  */
/*Limitations/Assumptions:                                                          */
/*                                                                                  */
/*-----------------------------------------------------------------------------     */

/* BEGIN:                                                                           */
#if USE_PROTOTYPES
static VOID lez_control_draw_point (NLIST_HEADER line, DOUBLE wx, DOUBLE wy,
                                        LINE_ATTRIBUTE *attribute)
#else
static VOID lez_control_draw_point (line, wx, wy, attribute)
NLIST_HEADER line;
DOUBLE wx, wy;
LINE_ATTRIBUTE *attribute;
#endif
    {
    BOOL exist_null;
    INT nbpt = 1;
    DOUBLE dumx, dumy;
    
    lez_control_nonull (line, wx, wy, dumx, dumy, nbpt, &exist_null);
    
    /*    we draw thepoint only if there are no null values                         */
    if (!exist_null)
        lez_draw_point (wx, wy, attribute);
    
    }
/* END:                                                                             */

/* Function Description -----------------------------------------------------       */
/*Description:                                                                      */
/*                                                                                  */
/*Prototype:                                                                        */
/*   static VOID lez_delete_null_strokes (NLIST_HEADER line);                   */
/*                                                                                  */
/*Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)        */
/*   line         -(NLIST_HEADER)                                                   */
/*                                                                                  */
/*Return Value/Status:                                                              */
/*   None.                                                                          */
/*                                                                                  */
/*Scope:                                                                            */
/*   PRIVATE to <le_nlist_ed.c>                                                     */
/*                                                                                  */
/*Limitations/Assumptions:                                                          */
/*                                                                                  */
/*-----------------------------------------------------------------------------     */

/* BEGIN:                                                                           */
#if USE_PROTOTYPES
static VOID lez_delete_null_strokes (NLIST_HEADER line)
#else
static VOID lez_delete_null_strokes (line)
NLIST_HEADER line;
#endif
    {
    UINT strokenum, nbstr, nbpoints;
    
    /*  this routine scans the strokes of an nlist and delete those with less than 2 points */
    
    nl_inq_nlist_int (line, NL_NLIST_NSTROKES, (INT *)&nbstr);
    
    for (strokenum = nbstr; strokenum > 0; strokenum--)
        {
        nl_inq_stroke_int (line, strokenum, NL_STROKE_NPOINTS, (INT *)&nbpoints);
        
        if (nbpoints < 2)
            nl_delete_stroke (line, strokenum);
        
        }
    }
/* END:                                                                             */

/* Function Description -----------------------------------------------------       */
/*Description:                                                                      */
/*    Routine used to control that point values are not null values.                */
/*                                                                                  */
/*Prototype:                                                                        */
/*    static VOID lez_control_nonull(NLIST_HEADER line, DOUBLE wx1, DOUBLE wy1, */
/*                                DOUBLE wx2, DOUBLE wy2, INT nbp, BOOL *exist_null);   */
/*                                                                                  */
/*Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)        */
/*    (I) nbp         -(INT) The number of points to control ( = 1 or 2).           */
/*    (I) wx1         -(DOUBLE) The x coordinate of the first point.                */
/*    (I) wy1         -(DOUBLE) The y coordinate of the first point.                */
/*    (I) wx2         -(DOUBLE) The x coordinate of the second point.               */
/*    (I) wy2         -(DOUBLE) The y coordinate of the second point.               */
/*    (O) exist_null  -(BOOL *) The pointer for the status returned.                */
/*                                                                                  */
/*Return Value/Status:                                                              */
/*    None.                                                                         */
/*                                                                                  */
/*Scope:                                                                            */
/*    PRIVATE to <le_nlist_ed.c>                                                    */
/*                                                                                  */
/*Limitations/Assumptions:                                                          */
/*                                                                                  */
/*-----------------------------------------------------------------------------     */

/* BEGIN:                                                                           */
#if USE_PROTOTYPES
static VOID lez_control_nonull (NLIST_HEADER line, DOUBLE wx1, DOUBLE wy1,
                                    DOUBLE wx2, DOUBLE wy2, INT nbp, BOOL *exist_null)
#else
static VOID lez_control_nonull (line, wx1, wy1, wx2, wy2, nbp, exist_null)
NLIST_HEADER line;
DOUBLE wx1, wx2, wy1, wy2;
INT nbp;
BOOL *exist_null;
#endif
    {
    
    DOUBLE max_xnull, min_xnull, max_ynull, min_ynull;
    GKS_REAL null1, null2;
    
    Var_List[0] = (VOIDPTR)&null1;
    Var_List[1] = (VOIDPTR)&null2;
    /*  inquire for null values on x and y dim                                      */
    
    nl_inq_nlist_info_convert (line, NL_NLIST_NULL_VALUE, NDIM, Col_List, Var_List,
                               Units, Width, DataType);
    
    max_xnull = null1 * 1.01;
    min_xnull = null1 * 0.99;
    max_ynull = null2 * 1.01;
    min_ynull = null2 * 0.99;
    
    if (nbp EQUALS 2)
        /*  we have two points to test                                              */
        *exist_null = ((wx1 > min_xnull AND wx1 < max_xnull) OR (wy1 >
                                                                 min_ynull AND wy1 <
                                                                 max_ynull) 
                       OR (wx2 > min_xnull AND wx2 < max_xnull) OR (wy2 >
                                                                    min_ynull AND wy2 <
                                                                    max_ynull));
    
    else
        *exist_null = ((wx1 > min_xnull AND wx1 < max_xnull) OR (wy1 >
                                                                 min_ynull AND wy1 <
                                                                 max_ynull));
    
    }
/* END:                                                                             */

/* Function Description -----------------------------------------------------       */
/*Description:                                                                      */
/*                                                                                  */
/*Prototype:                                                                        */
/*    static VOID lez_del_stroke(NLIST_HEADER line, INT stroke,                 */
/*                                LINE_ATTRIBUTE *erase_attribute);                 */
/*                                                                                  */
/*Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)        */
/*    line            -(NLIST_HEADER)                                               */
/*    stroke          -(INT)                                                        */
/*    erase_attribute -(LINE_ATTRIBUTE *)                                           */
/*                                                                                  */
/*Return Value/Status:                                                              */
/*    None.                                                                         */
/*                                                                                  */
/*Scope:                                                                            */
/*    PRIVATE to <le_nlist_ed.c>                                                    */
/*                                                                                  */
/*Limitations/Assumptions:                                                          */
/*                                                                                  */
/*-----------------------------------------------------------------------------     */

/* BEGIN:                                                                           */
#if USE_PROTOTYPES
static VOID lez_del_stroke (NLIST_HEADER line, UINT stroke,
                                LINE_ATTRIBUTE *erase_attribute)
#else
static VOID lez_del_stroke (line, stroke, erase_attribute)
NLIST_HEADER line;
UINT stroke;
LINE_ATTRIBUTE *erase_attribute;
#endif
    {
    if (stroke IS_NOT_EQUAL_TO 0)
        {
        lez_draw_stroke (line, stroke, erase_attribute);
        nl_delete_stroke (line, stroke);
        }
    }
/* END:                                                                             */

/* Function Description -----------------------------------------------------       */
/*Description:                                                                      */
/*                                                                                  */
/*Prototype:                                                                        */
/*    static VOID lez_draw_line(NLIST_HEADER line, LINE_ATTRIBUTE *attribute);  */
/*                                                                                  */
/*Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)        */
/*    line            -(NLIST_HEADER)                                               */
/*    attribute       -(LINE_ATTRIBUTE *)                                           */
/*                                                                                  */
/*Return Value/Status:                                                              */
/*    None.                                                                         */
/*                                                                                  */
/*Scope:                                                                            */
/*    PRIVATE to <le_nlist_ed.c>                                                    */
/*                                                                                  */
/*Limitations/Assumptions:                                                          */
/*                                                                                  */
/*-----------------------------------------------------------------------------     */

/* BEGIN:                                                                           */
#if USE_PROTOTYPES
static VOID lez_draw_line (NLIST_HEADER line, LINE_ATTRIBUTE *attribute)
#else
static VOID lez_draw_line (line, attribute)
NLIST_HEADER line;
LINE_ATTRIBUTE *attribute;
#endif
    {
    INT count;
    COUNT_POINTS (line, &count);
    if (count > 1)
        {
        wm_gsln (attribute->style);
        wm_gslwsc (attribute->width);
        wm_gsplci (attribute->color);
        nl_draw_nlist (line, (UINT)1, (UINT)2);
        }
    }
/* END:                                                                             */

/* Function Description -----------------------------------------------------       */
/*Description:                                                                      */
/*                                                                                  */
/*Prototype:                                                                        */
/*    static VOID lez_draw_pl(GKS_REAL *px, GKS_REAL *py, UINT npts, LINE_ATTRIBUTE *attribute); */
/*                                                                                  */
/*Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)        */
/*    px              -(GKS_REAL *)                                                 */
/*    py              -(GKS_REAL *)                                                 */
/*    npts            -(UINT)                                                       */
/*    attribute       -(LINE_ATTRIBUTE *)                                           */
/*                                                                                  */
/*Return Value/Status:                                                              */
/*    None.                                                                         */
/*                                                                                  */
/*Scope:                                                                            */
/*    PRIVATE to <le_nlist_ed.c>                                                    */
/*                                                                                  */
/*Limitations/Assumptions:                                                          */
/*                                                                                  */
/*-----------------------------------------------------------------------------     */

/* BEGIN:                                                                           */
#if USE_PROTOTYPES
static VOID lez_draw_pl (GKS_REAL *px, GKS_REAL *py, UINT npts,
                             LINE_ATTRIBUTE *attribute)
#else
static VOID lez_draw_pl (px, py, npts, attribute)
GKS_REAL *px, *py;
UINT npts;
LINE_ATTRIBUTE *attribute;
#endif
    {
    if (npts > 1)
        {
        wm_gsln (attribute->style);
        wm_gslwsc (attribute->width);
        wm_gsplci (attribute->color);
        
        wm_gpl ((GKS_INT)npts, px, py);
        }
    }
/* END:                                                                             */

/* Function Description -----------------------------------------------------       */
/*Description:                                                                      */
/*                                                                                  */
/*Prototype:                                                                        */
/*    static VOID lez_draw_point(DOUBLE wx, DOUBLE wy, LINE_ATTRIBUTE *attribute);  */
/*                                                                                  */
/*Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)        */
/*    wx              -(DOUBLE)                                                     */
/*    wy              -(DOUBLE)                                                     */
/*    attribute       -(LINE_ATTRIBUTE *)                                           */
/*                                                                                  */
/*Return Value/Status:                                                              */
/*    None.                                                                         */
/*                                                                                  */
/*Scope:                                                                            */
/*    PRIVATE to <le_nlist_ed.c>                                                    */
/*                                                                                  */
/*Limitations/Assumptions:                                                          */
/*                                                                                  */
/*-----------------------------------------------------------------------------     */

/* BEGIN:                                                                           */
#if USE_PROTOTYPES
static VOID lez_draw_point (DOUBLE wx, DOUBLE wy, LINE_ATTRIBUTE *attribute)
#else
static VOID lez_draw_point (wx, wy, attribute)
DOUBLE wx, wy;
LINE_ATTRIBUTE *attribute;
#endif
    {
    GKS_REAL px[1], py[1];
    GKS_INT npts = 1;
    px[0] = (GKS_REAL)wx;
    py[0] = (GKS_REAL)wy;

    wm_gspmci ((GKS_INT)attribute->color);
    wm_gpm (npts, px, py);
    }
/* END:                                                                             */

/* Function Description -----------------------------------------------------       */
/*Description:                                                                      */
/*                                                                                  */
/*Prototype:                                                                        */
/*    static VOID lez_draw_stroke(NLIST_HEADER line, UINT stroke, LINE_ATTRIBUTE *attribute);    */
/*                                                                                  */
/*Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)        */
/*    line            -(NLIST_HEADER)                                               */
/*    stroke          -(UINT)                                                       */
/*    attribute       -(LINE_ATTRIBUTE *)                                           */
/*                                                                                  */
/*Return Value/Status:                                                              */
/*    None.                                                                         */
/*                                                                                  */
/*Scope:                                                                            */
/*    PRIVATE to <le_nlist_ed.c>                                                    */
/*                                                                                  */
/*Limitations/Assumptions:                                                          */
/*                                                                                  */
/*-----------------------------------------------------------------------------     */

/* BEGIN:                                                                           */
#if USE_PROTOTYPES
static VOID lez_draw_stroke (NLIST_HEADER line, UINT stroke,
                                 LINE_ATTRIBUTE *attribute)
#else
static VOID lez_draw_stroke (line, stroke, attribute)
NLIST_HEADER line;
UINT stroke;
LINE_ATTRIBUTE *attribute;
#endif
    {
    
    if (stroke IS_NOT_EQUAL_TO 0)
        {
        wm_gsln (attribute->style);
        wm_gslwsc (attribute->width);
        wm_gsplci (attribute->color);
        nl_draw_stroke (line, stroke, (UINT)1, (UINT)2);
        }
    }
/* END:                                                                             */

/* Function Description -----------------------------------------------------       */
/*Description:                                                                      */
/*                                                                                  */
/*Prototype:                                                                        */
/*    static VOID lez_draw_vector(DOUBLE wx1, DOUBLE wy1, DOUBLE wx2, DOUBLE wy2,   */
/*                                LINE_ATTRIBUTE  *attribute);                      */
/*                                                                                  */
/*Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)        */
/*    wx1             -(DOUBLE)                                                     */
/*    wy1             -(DOUBLE)                                                     */
/*    wx2             -(DOUBLE)                                                     */
/*    wy2             -(DOUBLE)                                                     */
/*    attribute       -(INE_ATTRIBUTE  *)                                           */
/*                                                                                  */
/*Return Value/Status:                                                              */
/*    None.                                                                         */
/*                                                                                  */
/*Scope:                                                                            */
/*    PRIVATE to <le_nlist_ed.c>                                                    */
/*                                                                                  */
/*Limitations/Assumptions:                                                          */
/*                                                                                  */
/*-----------------------------------------------------------------------------     */

/* BEGIN:                                                                           */
#if USE_PROTOTYPES
static VOID lez_draw_vector (DOUBLE wx1, DOUBLE wy1, DOUBLE wx2, DOUBLE wy2,
                                 LINE_ATTRIBUTE *attribute)
#else
static VOID lez_draw_vector (wx1, wy1, wx2, wy2, attribute)
DOUBLE wx1, wy1, wx2, wy2;
LINE_ATTRIBUTE *attribute;
#endif
    {
    GKS_REAL px[2], py[2];
    UINT npts = (UINT)2;
    px[0] = (GKS_REAL)wx1;
    py[0] = (GKS_REAL)wy1;
    px[1] = (GKS_REAL)wx2;
    py[1] = (GKS_REAL)wy2;

    lez_draw_pl (px, py, npts, attribute);
    }
/* END:                                                                             */

/* Function Description -----------------------------------------------------       */
/*Description:                                                                      */
/*    Used to enable or disable depending on value of boolean.  It enables the      */
/*    operation listed in the array option_array.  The button is dimmed on the      */
/*    puck map.                                                                     */
/*                                                                                  */
/*Prototype:                                                                        */
/*    static VOID lez_enable_option(LE_CONTROL *pctrl, INT option_array [],     */
/*                                INT nb_option, BOOL  enable);                     */
/*                                                                                  */
/*Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)        */
/*    (I) pctrl       -(LE_CONTROL *) The control structure.                        */
/*    (I) option_array-(INT []) Array containing the list of operation.             */
/*    (I) nb_option   -(INT) The number of operations in option_array.              */
/*    (I) enable      -(BOOL) T/F the operations will be enabled/disabled.          */
/*                                                                                  */
/*Return Value/Status:                                                              */
/*    None.                                                                         */
/*                                                                                  */
/*Scope:                                                                            */
/*    PRIVATE to <le_nlist_ed.c>                                                    */
/*                                                                                  */
/*Limitations/Assumptions:                                                          */
/*                                                                                  */
/*-----------------------------------------------------------------------------     */

/* BEGIN:                                                                           */
#if USE_PROTOTYPES
static VOID lez_enable_option (LE_CONTROL *pctrl, INT option_array[],
                                   INT nb_option, BOOL enable)
#else
static VOID lez_enable_option (pctrl, option_array, nb_option, enable)
LE_CONTROL *pctrl;
INT option_array[];
INT nb_option;
BOOL enable;
#endif
    {
    INT i, j;
    BOOL absent;
    LE_KEYS *pkey;
    
    /*  we enable or diseable the items not present in the array                    */
    /*option_array  . this is just done if the default mac dialog is used           */
    if (pctrl->default_puck_map)
        {
        /*  pkey is initaialise to the first structure of the active stroke keymap  */
        pkey = pctrl->stroke_keymap + (pctrl->index_stroke_keym * MAX_KEYS);
        /* loop on all the function keys of the active keymap                       */
        for (i = 0; i < MAX_KEYS; i++, pkey++)
            {
            /* loop on the operations listed in the option_array to see if the      */
            /*current operation is present or not                                   */
            absent = TRUE;
            for (j = 0; j < nb_option; j++)
                {
                if (pkey->operation EQUALS option_array[j])
                    {
                    absent = FALSE;
                    break;
                    }
                }
            /* if absent = TRUE  that means that the current operation key is not in the    */
            /*   array and so we need to update the dialog item                     */
            if (absent)
                {
                /*  testing that the item is not null before updating               */
                if (dlg_item[i][0] IS_NOT_EQUAL_TO 0)
                    wiencdi (cur_stroke_id[pctrl->index_stroke_keym], dlg_item[i][0],
                             enable);
                if (dlg_item[i][1] IS_NOT_EQUAL_TO 0)
                    wiencdi (cur_stroke_id[pctrl->index_stroke_keym], dlg_item[i][1],
                             enable);
                }
            }
        }
    }
/* END:                                                                             */

/* Function Description -----------------------------------------------------       */
/*Description:                                                                      */
/*                                                                                  */
/*Prototype:                                                                        */
/*    static VOID lez_find_stroke_end(LE_CONTROL *pctrl, DOUBLE wx, DOUBLE wy,  */
/*                                    UINT *stroke, UINT *point);                   */
/*                                                                                  */
/*Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)        */
/*    pctrl           -(LE_CONTROL *)                                               */
/*    wx              -(DOUBLE)                                                     */
/*    wy              -(DOUBLE)                                                     */
/*    stroke          -(UINT *)                                                     */
/*    point           -(UINT *)                                                     */
/*                                                                                  */
/*Return Value/Status:                                                              */
/*    None.                                                                         */
/*                                                                                  */
/*Scope:                                                                            */
/*    PRIVATE to <le_nlist_ed.c>                                                    */
/*                                                                                  */
/*Limitations/Assumptions:                                                          */
/*                                                                                  */
/*-----------------------------------------------------------------------------     */

/* BEGIN:                                                                           */
#if USE_PROTOTYPES
static VOID lez_find_stroke_end (LE_CONTROL *pctrl, DOUBLE wx, DOUBLE wy,
                                     UINT *stroke, UINT *point)
#else
static VOID lez_find_stroke_end (pctrl, wx, wy, stroke, point)
LE_CONTROL *pctrl;
DOUBLE wx, wy;
UINT *stroke, *point;
#endif
    {
    NLIST_HEADER line;
    UINT i;
    INT status;
    UINT closestStroke, closestPoint;
    UINT count, strokeCount;
    GKS_REAL point1X, point2X, point1Y, point2Y;
    DOUBLE distance1, distance2;
    DOUBLE closestDistance = HUGE_VAL;
    
    *stroke = *point = 0;
    line = pctrl->nlist;
    COUNT_POINTS (line, &count);
    if (count EQUALS 0)
        {
        *stroke = *point = 0;
        return;
        }
    if (count < 2)
        {
        *stroke = *point = 1;
        status = nl_set_current_point (line, *point, *stroke);
        return;
        }
    /*   find the closer point of closest segment.                                  */
    /*   for each stroke, retireve the first and                                    */
    /*   the last point, then calculates the                                        */
    /*   distance, compare with the closet dis-                                     */
    /*   tance.                                                                     */
    
    nl_inq_nlist_int (line, NL_NLIST_NSTROKES, (INT *)&strokeCount);
    for (i = 1; i <= strokeCount; i++)
        {
        nl_inq_stroke_int (line, i, NL_STROKE_NPOINTS, (INT *)&count);
        if (count < 1)
            {
            continue;
            }
        nl_set_current_point (line, (UINT)1, i);
        GET_CURRENT_XY (line, &point1X, &point1Y);
        nl_set_current_point (line, count, i);
        GET_CURRENT_XY (line, &point2X, &point2Y);
        distance1 = hypot ((point1X - wx), (point1Y - wy));
        distance2 = hypot ((point2X - wx), (point2Y - wy));
        if (closestDistance > distance1)
            {
            closestDistance = distance1;
            closestStroke = i;
            closestPoint = 1;
            }
        if (closestDistance > distance2)
            {
            closestDistance = distance2;
            closestStroke = i;
            closestPoint = count;
            }
        }
    nl_set_current_point (line, closestPoint, closestStroke);
    *point = closestPoint;
    *stroke = closestStroke;
    return;
    }
/* END:                                                                             */

/* Function Description -----------------------------------------------------       */
/*Description:                                                                      */
/*                                                                                  */
/*Prototype:                                                                        */
/*    static VOID lez_find_stroke(LE_CONTROL *pctrl, DOUBLE wx, DOUBLE wy,      */
/*                                UINT *stroke, UINT *point);                       */
/*                                                                                  */
/*Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)        */
/*    pctrl           -(LE_CONTROL *)                                               */
/*    wx              -(DOUBLE)                                                     */
/*    wy              -(DOUBLE)                                                     */
/*    stroke          -(UINT *)                                                     */
/*    point           -(UINT *)                                                     */
/*                                                                                  */
/*Return Value/Status:                                                              */
/*    None.                                                                         */
/*                                                                                  */
/*Scope:                                                                            */
/*    PRIVATE to <le_nlist_ed.c>                                                    */
/*                                                                                  */
/*Limitations/Assumptions:                                                          */
/*                                                                                  */
/*-----------------------------------------------------------------------------     */

/* BEGIN:                                                                           */
#if USE_PROTOTYPES
static VOID lez_find_stroke (LE_CONTROL *pctrl, DOUBLE wx, DOUBLE wy, UINT *stroke,
                                 UINT *point)
#else
static VOID lez_find_stroke (pctrl, wx, wy, stroke, point)
LE_CONTROL *pctrl;
DOUBLE wx, wy;
UINT *stroke, *point;
#endif
    {
    NLIST_HEADER line;
    DOUBLE *valueList[2];
    INT count;
    INT status;
    DOUBLE min[2], max[2];
	DOUBLE temp_x = wx, temp_y = wy;
    
    UINT snum, pnum;
    
    *stroke = *point = 0;
    line = pctrl->nlist;
    COUNT_POINTS (line, &count);
    
    if (count EQUALS 0)
        {
        *stroke = *point = 0;
        return;
        }
    if (count < 2)
        {
        *stroke = *point = 1;
        status = nl_set_current_point (line, *point, *stroke);
        return;
        }
    min[0] = pctrl->minx;
    min[1] = pctrl->miny;
    max[0] = pctrl->maxx;
    max[1] = pctrl->maxy;
    
    valueList[0] = &temp_x;             /* contains the current cursor point            */
    valueList[1] = &temp_y;
    
    /* nl_pick_stroke_range finds the closest stroke to a specified                 */
    /*   point (snum). It also returns the closest point within                     */
    /*   that stroke (pnum).                                                        */
    /*   A normalized space is required, hence the min, max                         */
    /*   limits are passed in.                                                      */
    /*   Note: The call to nl_pick_stroke_range replaces a call to                  */
    /*     nl_pick_stroke which did essentially the same thing, but                 */
    /*     in a totally unoptimized manner (Bradshaw 3.29.88)                       */
    
    if (nl_pick_stroke_range (line, (UINT)2, Col_List, valueList, min, max, &snum, &pnum))
        {
        return;
        }
    *stroke = snum;
    *point = pnum;
    nl_set_current_point (line, *point, *stroke);
    
    return;
    }
/* END:                                                                             */

/* Function Description -----------------------------------------------------       */
/*Description:                                                                      */
/*                                                                                  */
/*Prototype:                                                                        */
/*    static VOID lez_hilight_part (NLIST_HEADER line, UINT stroke, UINT first_pt,  */
/*                                    UINT second_pt, LINE_ATTRIBUTE *hilight);     */
/*                                                                                  */
/*Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)        */
/*    line            -(NLIST_HEADER)                                               */
/*    stroke          -(UINT)                                                       */
/*    first_pt        -(UINT)                                                       */
/*    second_pt       -(UINT)                                                       */
/*    hilight         -(LINE_ATTRIBUTE *)                                           */
/*                                                                                  */
/*Return Value/Status:                                                              */
/*    None.                                                                         */
/*                                                                                  */
/*Scope:                                                                            */
/*    PRIVATE to <le_nlist_ed.c>                                                    */
/*                                                                                  */
/*Limitations/Assumptions:                                                          */
/*                                                                                  */
/*-----------------------------------------------------------------------------     */

/* BEGIN:                                                                           */
#if USE_PROTOTYPES
static VOID lez_hilight_part (NLIST_HEADER line, UINT stroke, UINT first_pt,
                                  UINT second_pt, LINE_ATTRIBUTE *hilight)
#else
static VOID lez_hilight_part (line, stroke, first_pt, second_pt, hilight)
NLIST_HEADER line;
UINT stroke, first_pt, second_pt;
LINE_ATTRIBUTE *hilight;
#endif
    {
    GKS_REAL x1, x2, y1, y2;
    UINT mini, maxi;
    UINT i;
    
    if (stroke IS_NOT_EQUAL_TO 0)
        {
        ((first_pt < second_pt) ? (mini = first_pt,
                                   maxi = second_pt) : (mini = second_pt,
                                                        maxi = first_pt));
        nl_set_current_point (line, mini, stroke);
        GET_CURRENT_XY (line, &x1, &y1);
        if (mini IS_NOT_EQUAL_TO maxi)
            {
            for (i = mini + 1; i <= maxi; i++, x1 = x2, y1 = y2)
                {
                nl_set_current_point (line, i, stroke);
                GET_CURRENT_XY (line, &x2, &y2);
                lez_control_draw_vector (line, x1, y1, x2, y2, hilight);
                }
            }
        }
    }
/* END:                                                                             */

/* Function Description -----------------------------------------------------       */
/*Description:                                                                      */
/*                                                                                  */
/*Prototype:                                                                        */
/*    static VOID lez_hilight_stroke (LE_CONTROL *pctrl);                       */
/*                                                                                  */
/*Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)        */
/*    pctrl           -(LE_CONTROL *)                                               */
/*                                                                                  */
/*Return Value/Status:                                                              */
/*    None.                                                                         */
/*                                                                                  */
/*Scope:                                                                            */
/*    PRIVATE to <le_nlist_ed.c>                                                    */
/*                                                                                  */
/*Limitations/Assumptions:                                                          */
/*                                                                                  */
/*-----------------------------------------------------------------------------     */

/* BEGIN:                                                                           */
#if USE_PROTOTYPES
static VOID lez_hilight_stroke (LE_CONTROL *pctrl)
#else
static VOID lez_hilight_stroke (pctrl)
LE_CONTROL *pctrl;
#endif
    {
    /*  if we are in stroke mode we hilight the current stroke or part of the current   */
    /*stoke  -                                                                      */
    /*if we are in point mode we hilight the scratch stroke which is at this time   */
    /*the 'current user stroke'                                                     */
    if (pctrl->stroke_mode)
        /*  we are in stroke mode and control that current stroke is existing       */
        /*   then testing if the user picked twice in the  same stroke to define    */
        /*   a part of stroke                                                       */
        {
        if (pctrl->current_stroke)
            {
            lez_draw_stroke (pctrl->nlist, pctrl->current_stroke,
                             &pctrl->hilight_attribute);
            if (pctrl->current_stroke EQUALS pctrl->last_stroke)
                lez_hilight_part (pctrl->nlist, pctrl->current_stroke,
                                  pctrl->current_point, pctrl->last_point,
                                  &pctrl->hilight_attribute);
            }
        }
    else                            /*  we are in point mode                        */
        lez_draw_stroke (pctrl->nlist, pctrl->scratch_str1, &pctrl->hilight_attribute);
    }
/* END:                                                                             */

/* Function Description -----------------------------------------------------       */
/*Description:                                                                      */
/*    Initialization for the  nlist editor.                                         */
/*                                                                                  */
/*Prototype:                                                                        */
/*    static VOID lez_init(LE_PARLIST *ppar, LE_CONTROL *pctrl);                */
/*                                                                                  */
/*Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)        */
/*    ppar            -(LE_PARLIST *) Pointer to control structure passed by        */
/*                        the application.                                          */
/*    pctrl           -(LE_CONTROL *) Pointer to control structure.                 */
/*                                                                                  */
/*Return Value/Status:                                                              */
/*    None.                                                                         */
/*                                                                                  */
/*Scope:                                                                            */
/*    PRIVATE to <le_nlist_ed.c>                                                    */
/*                                                                                  */
/*Limitations/Assumptions:                                                          */
/*                                                                                  */
/*-----------------------------------------------------------------------------     */

/* BEGIN:                                                                           */
#if USE_PROTOTYPES
static VOID lez_init (LE_PARLIST *ppar, LE_CONTROL *pctrl)
#else
static VOID lez_init (ppar, pctrl)
LE_PARLIST *ppar;
LE_CONTROL *pctrl;
#endif
    {
    CHAR title[80];
    
    /* -----------------------------------                                          */
    /* Inquire the workstation type.                                                */
    /* If it is RASTER, i.e., !ug_if_gpx()                                          */
    /* then we use tablet, not mouse menus                                          */
    /* -----------------------------------                                          */
    if (!(ug_if_gpx ()))
        {
        if_gpx = FALSE;
        if_tablet = TRUE;
        if_mouse_menus = FALSE;
        }
    /* ----------------------------------------------                               */
    /* If have real workstation, we know we are using                               */
    /* the tablet if the current transformation is                                  */
    /* registered (as checked by wm_if_tablet()                                     */
    /* ----------------------------------------------                               */
    else
        {
        if_gpx = TRUE;
        if_tablet = wm_if_tablet ();
        if_mouse_menus = (if_gpx && !if_tablet ? TRUE : FALSE);
        }
    /* --------------------------------------------                                 */
    /* Put up the "Initializing line editor message                                 */
    /* --------------------------------------------                                 */
    am_message (AM_DIALOG, mg_message (LE_INIT_MESSAGE));
    
    /*  initialize index stroke map                                                 */
    
    pctrl->index_stroke_keym = 0;
    
    /*  index for point keymaps                                                     */
    
    pctrl->index_point_keym = 0;
    
    /* move the parameter data into the control structure                           */
    
    pctrl->draw_attribute.color = ppar->line.color;
    pctrl->erase_attribute.color = ppar->erase_color;
    pctrl->hilight_attribute.color = ppar->hilight_color;
    
    /* if line width has not been set by the application we put  a default of 1.0   */
    
    if (ppar->line.width < 0.0001)
        ppar->line.width = 1.0;
    
    /* if line style has not being set by the application we put a default of 1     */
    
    if (ppar->line.style EQUALS 0)
        ppar->line.style = 1;
    
    pctrl->draw_attribute.style = ppar->line.style;
    pctrl->draw_attribute.width = ppar->line.width;
    pctrl->erase_attribute.style = ppar->line.style;
    pctrl->erase_attribute.width = ppar->line.width;
    pctrl->hilight_attribute.style = ppar->line.style;
    pctrl->hilight_attribute.width = ppar->line.width;
    pctrl->stroke_mode = TRUE;
    pctrl->last_mode = TRUE;
    pctrl->use_keym = ppar->use_keymap;
    pctrl->dial_update = TRUE;
    pctrl->data_pointer = ppar->data_pointer;
    pctrl->only_app_mac_dialog = ppar->only_app_mac_dialog;
    pctrl->key_swap = ppar->key_swap;
    
    /*  testing if the application has defined its own puck map                     */
    /*    dialog on the mac                                                         */
    /*        -if yes   only this dialog will be displayed and we set the           */
    /*                  boolean  pctrl -> default_puck_map  to FALSE which          */
    /*                  means that none of the default keymap dilaog will           */
    /*                  be displayed  until the boolean is turned to TRUE           */
    
    if (ppar->app_puck_map_disp_init IS_NOT_EQUAL_TO NULL_FUNCTION_PTR)
        {
        (*ppar->app_puck_map_disp_init)(pctrl);
        pctrl->default_puck_map = FALSE;
        }
    else
        {
        pctrl->default_puck_map = TRUE;
        }
    /* allocate space for current point keymaps pointer array                       */
    
    if (ppar->point_keymap EQUALS (LE_KEYS *)0)
        {
        le_allocate_keymap (&ppar->point_keymap, LE_POINT_MODE, NUM_DEF_POINT_KEYM);
        ppar->num_point_keymaps = NUM_DEF_POINT_KEYM;
        }
    pctrl->point_keymap = ppar->point_keymap;
    pctrl->num_point_keymaps = ppar->num_point_keymaps;
    
    /* initialize the keymap dialog(s): we initialise the dialogs only              */
    /*    if the application doesn't define its own dialog                          */
    
    if (!pctrl->only_app_mac_dialog)
        {
        sprintf ((char *)title, "(Point Mode) %s", ppar->title);
        lez_init_choices (pctrl->point_keymap, pctrl->num_point_keymaps, cur_point_id,
                          cur_point_menus, cur_point_maps, title);
        }
    /*  testing if stroke keymaps have been defined by the application              */
    
    if (ppar->stroke_keymap EQUALS (LE_KEYS *)0)
        {
        le_allocate_keymap (&ppar->stroke_keymap, LE_STROKE_MODE, NUM_DEF_STROKE_KEYM);
        ppar->num_stroke_keymaps = NUM_DEF_STROKE_KEYM;
        }
    pctrl->stroke_keymap = ppar->stroke_keymap;
    pctrl->num_stroke_keymaps = ppar->num_stroke_keymaps;
    
    /* initialize the keymap dialog(s)                                              */
    
    if (!pctrl->only_app_mac_dialog)
        {
        sprintf ((char *)title, "(Line Mode) %s", ppar->title);
        lez_init_choices (pctrl->stroke_keymap, pctrl->num_stroke_keymaps,
                          cur_stroke_id, cur_stroke_menus, cur_stroke_maps, title);
        }
    lez_init_window_mgr (pctrl);
    if (if_gpx)
        {
        if (if_tablet)
            lez_setup_tablet_keypad ();
        else
            wm_set_mouse_menu (cur_stroke_menus[0], cur_stroke_maps[0]);
        }
    /* Clear the "initializing..." message                                          */
    am_message (AM_DIALOG, NULL);
    }
/* END:                                                                             */

/* Function Description -----------------------------------------------------       */
/*Description:                                                                      */
/*                                                                                  */
/*Prototype:                                                                        */
/*    static VOID lez_init_choices(LE_KEYS *pkeymap, INT nmaps, INT *dialog_id_array,   */
/*                                TCP *gpx_menus, INT **gpx_maps, CHAR *title);  */
/*                                                                                  */
/*Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)        */
/*    pkeymap         -(LE_KEYS *)                                                  */
/*    nmaps           -(INT)                                                        */
/*    dialog_id_array -(INT *)                                                      */
/*    gpx_menus       -(TCP *)                                                      */
/*    gpx_maps        -(INT **)                                                     */
/*    title           -(CHAR *)                                                     */
/*                                                                                  */
/*Return Value/Status:                                                              */
/*    None.                                                                         */
/*                                                                                  */
/*Scope:                                                                            */
/*    PRIVATE to <le_nlist_ed.c>                                                    */
/*                                                                                  */
/*Limitations/Assumptions:                                                          */
/*                                                                                  */
/*-----------------------------------------------------------------------------     */

/* BEGIN:                                                                           */
#if USE_PROTOTYPES
static VOID lez_init_choices (LE_KEYS *pkeymap, INT nmaps, INT *dialog_id_array,
                                  TCP *gpx_menus, INT **gpx_maps, CHAR *title)
#else
static VOID lez_init_choices (pkeymap, nmaps, dialog_id_array, gpx_menus, gpx_maps,
                                  title)
LE_KEYS *pkeymap;
INT nmaps;
INT *dialog_id_array;
TCP *gpx_menus;
INT **gpx_maps;
CHAR *title;
#endif
    {
    /* allocate and initialize keymap dialog boxes                                  */
    LE_KEYS *pKey;
    INT i, j;
    INT id;
    TCP ptcp;
    INT num_choices;
    INT *pMap;
    
    /* --------------------------------------------                                 */
    /* Process each keymap, creating the associated                                 */
    /* dialogs and or mouse menus                                                   */
    /* --------------------------------------------                                 */
    for (i = 0; i < nmaps; i++, pkeymap += MAX_KEYS)
        {
        /* -----------------------------                                            */
        /* If the workstation is GPX and                                            */
        /* the input device is MOUSE, we                                            */
        /* will define the pop-up menus.                                            */
        /* -----------------------------                                            */
        if (if_mouse_menus)
            {
            for (num_choices = j = 0, pKey = pkeymap; j < MAX_KEYS; j++, pKey++)
                {
                if (pKey->validt)
                    ++num_choices;
                }
            gpx_menus[i] = TCP_ALLOCATE (num_choices + 1);
            gpx_maps[i] = (INT *) tc_zalloc (num_choices * sizeof(INT));
            ptcp = gpx_menus[i];
            pMap = gpx_maps[i];
            for (j = 0, pKey = pkeymap; j < MAX_KEYS; j++, pKey++)
                {
                if (pKey->validt)
                    {
                    *ptcp = STRING_ALLOCATE (pKey->description);
                    strcpy ((char *)*ptcp, (char *)pKey->description);
                    *pMap++ = pKey->button;
                    ++ptcp;
                    }
                }
            }
        else
            {
            /* ----------------------------                                         */
            /* define dialog box for keymap                                         */
            /* ----------------------------                                         */
            am_define_resource (AM_DIALOG, DIALOG_FILE, DIALOG_LEPKM,
                                NULL_FUNCTION_PTR, &id, NULL);
            /* set title of dialog                                                  */
            wiscdttl (id, title);
            dialog_id_array[i] = id;
            
            /* ---------------------------------------------                        */
            /* Set up the text, enable, disable dialog items                        */
            /* ---------------------------------------------                        */
            lez_set_keymap_cdlg (pkeymap, id);
            }
        }
    return;
    }
/* END:                                                                             */

/* Function Description -----------------------------------------------------       */
/*Description:                                                                      */
/*    Initialize the window manager variables in the LE_CONTROL structure.          */
/*                                                                                  */
/*Prototype:                                                                        */
/*    static VOID lez_init_window_mgr(LE_CONTROL *pctrl);                       */
/*                                                                                  */
/*Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)        */
/*    pctrl           -(LE_CONTROL *)                                               */
/*                                                                                  */
/*Return Value/Status:                                                              */
/*    None.                                                                         */
/*                                                                                  */
/*Scope:                                                                            */
/*    PRIVATE to <le_nlist_ed.c>                                                    */
/*                                                                                  */
/*Limitations/Assumptions:                                                          */
/*                                                                                  */
/*-----------------------------------------------------------------------------     */

/* BEGIN:                                                                           */
#if USE_PROTOTYPES
static VOID lez_init_window_mgr (LE_CONTROL *pctrl)
#else
static VOID lez_init_window_mgr (pctrl)
LE_CONTROL *pctrl;
#endif
    {
    APPLICATION_ID app_id;
    TNR_ID tnr_id;
    GROUP_ID gro_id;
    SEGMENT_ID seg_id;
    PICK_ID pic_id;
    WINDOW_ID win_id;
    GKS_REAL wcsxmin, wcsxmax, wcsymin, wcsymax;   /* world coords                     */
    
    GKS_REAL nwcxmin, nwcxmax, nwcymin, nwcymax;   /* normalized window coords         */
    
    wmoinqcur (&app_id, &win_id, &tnr_id, &gro_id, &seg_id, &pic_id);
    pctrl->window_id = win_id;      /* current window                               */
    
    pctrl->transform_id = tnr_id;   /* current xform                                */
    
    wmtinq (tnr_id, &nwcxmin, &nwcxmax, &nwcymin, &nwcymax, &wcsxmin, &wcsxmax,
            &wcsymin, &wcsymax);
    pctrl->minx = wcsxmin;          /* current window limits                        */
    
    pctrl->maxx = wcsxmax;
    pctrl->miny = wcsymin;
    pctrl->maxy = wcsymax;
    }
/* END:                                                                             */

/* Function Description -----------------------------------------------------       */
/*Description:                                                                      */
/*    Scan all available keymaps for requested operation.                           */
/*                                                                                  */
/*Prototype:                                                                        */
/*    static LE_KEYS * lez_inq_key (LE_CONTROL *pctrl, INT operation);          */
/*                                                                                  */
/*Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)        */
/*    (I) pctrl       -(LE_CONTROL *) Standard line editor control block.           */
/*    (I) operation   -(INT) Operation to be translated.                            */
/*                                                                                  */
/*Return Value/Status:                                                              */
/*    Function returns pointer to appropriate key structure, or NULL if not found.  */
/*                                                                                  */
/*Scope:                                                                            */
/*    PRIVATE to <le_nlist_ed.c>                                                    */
/*                                                                                  */
/*Limitations/Assumptions:                                                          */
/*                                                                                  */
/*-----------------------------------------------------------------------------     */

/* BEGIN:                                                                           */
#if USE_PROTOTYPES
static LE_KEYS *lez_inq_key (LE_CONTROL *pctrl, INT operation)
#else
static LE_KEYS *lez_inq_key (pctrl, operation)
LE_CONTROL *pctrl;
INT operation;
#endif
    {
    LE_KEYS *pKey;
    
    /* start looking in 'current' mode keymap.  If that fails, try the              */
    /*other keymap                                                                  */
    
    if (pctrl->stroke_mode)
        {
        pKey = le_scan_keymap (pctrl->stroke_keymap, pctrl->num_stroke_keymaps,
                               operation);
        if (pKey EQUALS (LE_KEYS *)0)
            {
            pKey = le_scan_keymap (pctrl->point_keymap, pctrl->num_point_keymaps,
                                   operation);
            }
        }
    else
        {
        pKey = le_scan_keymap (pctrl->point_keymap, pctrl->num_point_keymaps,
                               operation);
        if (pKey EQUALS (LE_KEYS *)0)
            {
            pKey = le_scan_keymap (pctrl->stroke_keymap, pctrl->num_stroke_keymaps,
                                   operation);
            }
        }
    /* if we still can't translate, set pKey to a special Null Key                  */
    /*(always defined)                                                              */
    
    if (pKey EQUALS (LE_KEYS *)0)
        {
        pKey = &TheNullKey;
        }
    return pKey;
    }
/* END:                                                                             */

/* Function Description -----------------------------------------------------       */
/*Description:                                                                      */
/*                                                                                  */
/*Prototype:                                                                        */
/*    static VOID lez_prompt (INT lepk_id, CHAR *descript);                     */
/*                                                                                  */
/*Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)        */
/*    lepk_id         -(INT)                                                        */
/*    descript        -(CHAR *)                                                     */
/*                                                                                  */
/*Return Value/Status:                                                              */
/*    None.                                                                         */
/*                                                                                  */
/*Scope:                                                                            */
/*    PRIVATE to <le_nlist_ed.c>                                                    */
/*                                                                                  */
/*Limitations/Assumptions:                                                          */
/*                                                                                  */
/*-----------------------------------------------------------------------------     */

/* BEGIN:                                                                           */
#if USE_PROTOTYPES
static VOID lez_prompt (INT lepk_id, CHAR *descript)
#else
static VOID lez_prompt (lepk_id, descript)
INT lepk_id;
CHAR *descript;
#endif
/* Redirect prompts to FINDER message area                                          */
/* when using pop-up menus                                                          */
    {
    if (if_mouse_menus)
        {
        am_message (AM_DIALOG, descript);
        }
    else
        {
        wiscdt (lepk_id, ITEM_LEPKM_PROMPT, descript);
        }
    }
/* END:                                                                             */

/* Function Description -----------------------------------------------------       */
/*Description:                                                                      */
/*    Restore default prompts on the keys Cancel Done.                              */
/*                                                                                  */
/*Prototype:                                                                        */
/*    static VOID lez_restor_default_prompt (LE_KEYS *pcancel, LE_KEYS *pdone,  */
/*                                            LE_CONTROL *pctrl);                   */
/*                                                                                  */
/*Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)        */
/*    (I) pcancel     -(LE_KEYS *) Pointer to the structure in stroke keymap        */
/*                        corresponding to cancel operation.                        */
/*    (I) pdone       -(LE_KEYS *) Pointer to the structure in stroke keymap        */
/*                        corresponding to cancel operation.                        */
/*    (I) pctrl       -(LE_CONTROL *)                                               */
/*                                                                                  */
/*Return Value/Status:                                                              */
/*    None.                                                                         */
/*                                                                                  */
/*Scope:                                                                            */
/*    PRIVATE to <le_nlist_ed.c>                                                    */
/*                                                                                  */
/*Limitations/Assumptions:                                                          */
/*                                                                                  */
/*-----------------------------------------------------------------------------     */

/* BEGIN:                                                                           */
#if USE_PROTOTYPES
static VOID lez_restor_default_prompt (LE_KEYS *pcancel, LE_KEYS *pdone,
                                           LE_CONTROL *pctrl)
#else
static VOID lez_restor_default_prompt (pcancel, pdone, pctrl)
LE_KEYS *pcancel, *pdone;
LE_CONTROL *pctrl;
#endif
    {
    if (pctrl->default_puck_map)
        {
        /*  restore button cancel label                                             */
        
        if (pcancel IS_NOT_EQUAL_TO (LE_KEYS *)0)
            lez_upd_key_prompt (cur_stroke_id[pctrl->index_stroke_keym],
                                pcancel->button, pcancel->description);
        
        /*we restore button DONE label                                              */
        
        if (pdone IS_NOT_EQUAL_TO (LE_KEYS *)0)
            lez_upd_key_prompt (cur_stroke_id[pctrl->index_stroke_keym], pdone->button,
                                pdone->description);
        
        /*   set prompt field to blank                                              */
        
        lez_prompt (cur_stroke_id[pctrl->index_stroke_keym], NULL_STRING);
        }
    }
/* END:                                                                             */

/* Function Description -----------------------------------------------------       */
/*Description:                                                                      */
/*                                                                                  */
/*Prototype:                                                                        */
/*    static VOID lez_rqbutxy (NLIST_HEADER line, INT *stat, DOUBLE lastx,      */
/*                            DOUBLE lasty, GKS_REAL *wx, GKS_REAL *wy, INT *but,         */
/*                            INT nbp);                                             */
/*                                                                                  */
/*Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)        */
/*    line            -(NLIST_HEADER)                                               */
/*    stat            -(INT *)                                                      */
/*    but             -(INT *)                                                      */
/*    lastx           -(DOUBLE)                                                     */
/*    lasty           -(DOUBLE)                                                     */
/*    wx              -(GKS_REAL *)                                                    */
/*    wy              -(GKS_REAL *)                                                    */
/*    nbp             -(INT)                                                        */
/*                                                                                  */
/*Return Value/Status:                                                              */
/*    None.                                                                         */
/*                                                                                  */
/*Scope:                                                                            */
/*    PRIVATE to <le_nlist_ed.c>                                                    */
/*                                                                                  */
/*Limitations/Assumptions:                                                          */
/*                                                                                  */
/*-----------------------------------------------------------------------------     */

/* BEGIN:                                                                           */
#if USE_PROTOTYPES
static VOID lez_rqbutxy (NLIST_HEADER line, INT *stat, DOUBLE lastx, DOUBLE lasty,
                             GKS_REAL *wx, GKS_REAL *wy, INT *but, INT nbp)
#else
static VOID lez_rqbutxy (line, stat, lastx, lasty, wx, wy, but, nbp)
NLIST_HEADER line;
INT *stat, *but;
DOUBLE lastx, lasty;
GKS_REAL *wx, *wy;
INT nbp;
#endif
    {
    BOOL exist_null;
    INT nbpt = 1;
    DOUBLE dumx, dumy;
    
    if (nbp > 0)
        {
        /*  control that the point origin of the rubberband ( lastx, lasty)         */
        /*has no null values                                                        */
        
        lez_control_nonull (line, lastx, lasty, dumx, dumy, nbpt, &exist_null);
        
        /* if the workstation is GPX and input device is tablet, we will not allow the  */
        /*    banding.                                                              */
        
        if (!exist_null)
            {
            if (if_gpx AND if_tablet)
                {
                wmtlocate (stat, wx, wy, but);
                }
            else
                {
                wmtbandvector (stat, lastx, lasty, wx, wy, but);
                }
            }
        else
            wmtlocate (stat, wx, wy, but);
        
        }
    else
        {
        wmtlocate (stat, wx, wy, but);
        }
    }
/* END:                                                                             */

/* Function Description -----------------------------------------------------       */
/*Description:                                                                      */
/*    Copy current nlist in another nlist to perform undo function.                 */
/*                                                                                  */
/*Prototype:                                                                        */
/*    static VOID lez_save_list (LE_CONTROL *pctrl, LE_CONTROL *lastpctrl);     */
/*                                                                                  */
/*Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)        */
/*    pctrl           -(LE_CONTROL *)                                               */
/*    lastpctrl       -(LE_CONTROL *)                                               */
/*                                                                                  */
/*Return Value/Status:                                                              */
/*    None.                                                                         */
/*                                                                                  */
/*Scope:                                                                            */
/*    PRIVATE to <le_nlist_ed.c>                                                    */
/*                                                                                  */
/*Limitations/Assumptions:                                                          */
/*                                                                                  */
/*-----------------------------------------------------------------------------     */

/* BEGIN:                                                                           */
#if USE_PROTOTYPES
static VOID lez_save_list (LE_CONTROL *pctrl, LE_CONTROL *lastpctrl)
#else
static VOID lez_save_list (pctrl, lastpctrl)
LE_CONTROL *pctrl;
LE_CONTROL *lastpctrl;
#endif
    {
    /*  copy current nlist in last state of nlist                                   */
    /*   after   free space allocated for the last copy of the line                 */
    
    FREE_LINE (lastpctrl->nlist);
    
    /*  copy current control structure  in last state of control structure          */
    /*  we just need to keep the pointers to the control structures the same that   */
    /*  at the beginning so we don't erase this pointer in lastpctrl                */
    
    *lastpctrl = *pctrl;
    
    /*  save a copy of current n_list for UNDO   function                           */
    
    nl_copy_nlist (pctrl->nlist, &lastpctrl->nlist);
    }
/* END:                                                                             */

/* Function Description -----------------------------------------------------       */
/*Description:                                                                      */
/*    Interactive selection of a part of stroke.                                    */
/*                                                                                  */
/*Prototype:                                                                        */
/*    static VOID lez_sel_part(UINT *first_point, UINT *second_point, UINT *stroke_picked,  */
/*                            BOOL *noexit, UINT *cnt, LE_CONTROL *pctrl, INT cur_oper,     */
/*                            INT option_array [], INT nb_option);                  */
/*                                                                                  */
/*Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)        */
/*    (I) option_array    -(INT []) Array containing the options allowed.           */
/*    (I) nb_options      -(INT) Number of options.                                 */
/*    (I) cur_oper        -(INT) Current operation where we are doing the selection.*/
/*    (I) pctrl           -(LE_CONTROL *) Control structure.                        */
/*    (O) cnt             -(UINT *) Number of points selected to define the part.   */
/*    (O) first_point     -(UINT *) First point selected in the stroke.             */
/*    (O) second_point    -(UINT *) Second point selected to define the part.       */
/*    (O) stroke_picked   -(UINT *) Number of the stroke picked.                    */
/*    (O) noexit          -(BOOL *) T/F operation well completed/canceled by the operator.  */
/*                                                                                  */
/*Return Value/Status:                                                              */
/*    None.                                                                         */
/*                                                                                  */
/*Scope:                                                                            */
/*    PRIVATE to <le_nlist_ed.c>                                                    */
/*                                                                                  */
/*Limitations/Assumptions:                                                          */
/*                                                                                  */
/*-----------------------------------------------------------------------------     */

/* BEGIN:                                                                           */
#if USE_PROTOTYPES
static VOID lez_sel_part (UINT *first_point, UINT *second_point,
                              UINT *stroke_picked, BOOL *noexit, UINT *cnt,
                              LE_CONTROL *pctrl, INT cur_oper, INT option_array[],
                              INT nb_option)
#else
static VOID lez_sel_part (first_point, second_point, stroke_picked, noexit, cnt,
                              pctrl, cur_oper, option_array, nb_option)
UINT *first_point, *second_point, *stroke_picked, *cnt;
INT cur_oper;
BOOL *noexit;
LE_CONTROL *pctrl;
INT option_array[];
INT nb_option;
#endif
    {
    UINT point_picked, last_stroke = 0, number;
    INT numb;
    GKS_REAL wx, wy;
    GKS_REAL firstx, firsty;
    INT status, oper, but;
    BOOL loop, enter_cleave;
    
    enter_cleave = TRUE;
    
    /*   boolean enter_cleave is used not to asked for a point                      */
    /* when we enter in cleave stroke but to take the point                         */
    /* digitized when selecting the function cleave_stroke                          */
    
    wx = pctrl->wx;
    wy = pctrl->wy;
    firstx = wx;
    firsty = wy;
    numb = 1;
    *cnt = 0;
    *first_point = 0;
    *second_point = 0;
    *stroke_picked = 0;
    
    /*  testing first if there are some strokes in the n-list                       */
    
    nl_inq_nlist_int (pctrl->nlist, NL_NLIST_NSTROKES, (INT *)&number);
    if (number EQUALS 0)
        {
        do_update = TRUE;
        curr_err = LE_NO_STROKES;
        *noexit = FALSE;
        }
    else
        {
        /*  we have strokes in the nlist so we can try to select                    */
        /*a part of stroke                                                          */
        
        do
            {
            if (!enter_cleave)
                {
                lez_control_but (&status, firstx, firsty, &wx, &wy, &but, numb, pctrl,
                                 option_array, nb_option, &oper, *stroke_picked,
                                 *first_point, *second_point);
                }
            else
                {
                oper = cur_oper;
                enter_cleave = FALSE;
                }
            if (oper EQUALS BUTTON_CANCEL)
                {
                loop = FALSE;
                *noexit = FALSE;
                if (*stroke_picked IS_NOT_EQUAL_TO 0)
                    {
                    lez_draw_stroke (pctrl->nlist, *stroke_picked,
                                     &pctrl->draw_attribute);
                    }
                }
            else
                {
                *noexit = TRUE;
                if (oper EQUALS BUTTON_DONE)
                    {
                    /*  we control that the two points used for determining a part are  */
                    /*different - if they are the same an error message is displayed*/
                    
                    if ((*first_point EQUALS * second_point AND *
                         cnt EQUALS 2) OR (*cnt EQUALS 1))
                        {
                        do_update = TRUE;
                        curr_err = LE_TWO_POINTS;
                        lez_update (cur_stroke_id[pctrl->index_stroke_keym],
                                    pctrl->default_puck_map);
                        }
                    else
                        {
                        loop = FALSE;
                        }
                    }
                else
                    {
                    loop = TRUE;
                    lez_find_stroke (pctrl, wx, wy, stroke_picked, &point_picked);
                    if (*stroke_picked IS_NOT_EQUAL_TO 0)
                        {
                        if (*cnt < 2)
                            *cnt += 1;
                        numb = 1;
                        *second_point = *first_point;
                        
                        nl_set_current_point (pctrl->nlist, point_picked,
                                              *stroke_picked);
                        
                        GET_CURRENT_XY (pctrl->nlist, &firstx, &firsty);
                        
                        *first_point = point_picked;
                        
                        /*  we verify that the new stroke picked is the same that in the previous pick  */
                        /*if not we consider that the user is wanting to pick another stroke and this is    */
                        /*considered as the first point choosen on this new stroke  */
                        
                        if (*stroke_picked IS_NOT_EQUAL_TO last_stroke AND * cnt > 0)
                            {
                            *cnt = 1;
                            lez_draw_stroke (pctrl->nlist, last_stroke,
                                             &pctrl->draw_attribute);
                            }
                        last_stroke = *stroke_picked;
                        
                        if (*cnt EQUALS 2)
                            {
                            lez_draw_stroke (pctrl->nlist, *stroke_picked,
                                             &pctrl->draw_attribute);
                            lez_hilight_part (pctrl->nlist, *stroke_picked,
                                              *first_point, *second_point,
                                              &pctrl->hilight_attribute);
                            }
                        }
                    else
                        {
                        /* no stroke was found                                      */
                        
                        do_update = TRUE;
                        curr_err = LE_NO_STROKE_FOUND;
                        lez_update (cur_stroke_id[pctrl->index_stroke_keym],
                                    pctrl->default_puck_map);
                        }
                    }
                }
            } while (loop);
        
        if (*cnt < 2)
            *noexit = FALSE;
        
        }
    }
/* END:                                                                             */

/* Function Description -----------------------------------------------------       */
/*Description:                                                                      */
/*    This %S routine will set the text and enable/disable items according to       */
/*    the specified keymap in the indicated dialog.                                 */
/*                                                                                  */
/*Prototype:                                                                        */
/*    static INT lez_set_keymap_cdlg(LE_KEYS *current_keymap, INT lepkm_id);    */
/*                                                                                  */
/*Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)        */
/*    (I) current_keymap  -(LE_KEYS *) Keymap to be used to set text.               */
/*    (I) lepkm_id        -(INT) Previously defined dialog id.                      */
/*                                                                                  */
/*Return Value/Status:                                                              */
/*    SUCCESS - Successful completion.                                              */
/*                                                                                  */
/*Scope:                                                                            */
/*    PRIVATE to <le_nlist_ed.c>                                                    */
/*                                                                                  */
/*Limitations/Assumptions:                                                          */
/*                                                                                  */
/*-----------------------------------------------------------------------------     */

/* BEGIN:                                                                           */
#if USE_PROTOTYPES
static INT lez_set_keymap_cdlg (LE_KEYS *current_keymap, INT lepkm_id)
#else
static INT lez_set_keymap_cdlg (current_keymap, lepkm_id)
LE_KEYS *current_keymap;
INT lepkm_id;
#endif
    {
    LE_KEYS *pKey;
    INT i;
    /* --------------------------------------------                                 */
    /* Only fool with dialogs when there are there!                                 */
    /* --------------------------------------------                                 */
    if (!if_mouse_menus)
        {
        for (i = 0, pKey = current_keymap; i < MAX_KEYS; i++, pKey++)
            {
            if (dlg_item[i][1])
                {
                wiscdt (lepkm_id, dlg_item[i][1], pKey->description);
                /* set enable/disable on the items                                  */
                wiencdi (lepkm_id, dlg_item[i][0], pKey->validt);
                wiencdi (lepkm_id, dlg_item[i][1], pKey->validt);
                }
            }
        }
    return SUCCESS;
    }
/* END:                                                                             */

/* Function Description -----------------------------------------------------       */
/*Description:                                                                      */
/*    Used to swap to operation in a given keymap.                                  */
/*                                                                                  */
/*Prototype:                                                                        */
/*    static VOID lez_swap_but (BOOL stroke_key, INT oper1, INT oper2, LE_CONTROL *pctrl);  */
/*                                                                                  */
/*Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)        */
/*    stroke_key      -(BOOL) True if we are on the current stroke key.             */
/*                            False  if we swap on the current point key.           */
/*    oper1           -(INT) First operation to swap.                               */
/*    oper2           -(INT) Second opertaion to swap.                              */
/*    pctrl           -(LE_CONTROL *) Control structure.                            */
/*                                                                                  */
/*Return Value/Status:                                                              */
/*    None.                                                                         */
/*                                                                                  */
/*Scope:                                                                            */
/*    PRIVATE to <le_nlist_ed.c>                                                    */
/*                                                                                  */
/*Limitations/Assumptions:                                                          */
/*                                                                                  */
/*-----------------------------------------------------------------------------     */

/* BEGIN:                                                                           */
#if USE_PROTOTYPES
static VOID lez_swap_but (BOOL stroke_key, INT oper1, INT oper2, LE_CONTROL *pctrl)
#else
static VOID lez_swap_but (stroke_key, oper1, oper2, pctrl)
BOOL stroke_key;
INT oper1, oper2;
LE_CONTROL *pctrl;
#endif
    {
    LE_KEYS *pkey1, *pkey2, *pkeymap;
    
    /*  pkeymap is the pointer to the first structure of the keymap where we        */
    /*  swap two keys . it is the current stroke or current point keymap depending  */
    /*   on the boolean stroke_key                                                  */
    
    if (stroke_key)
        {
        pkeymap = pctrl->stroke_keymap + (pctrl->index_stroke_keym * MAX_KEYS);
        }
    else
        {
        pkeymap = pctrl->point_keymap + (pctrl->index_point_keym * MAX_KEYS);
        }
    /*  looking for the operation in the keymap                                     */
    
    pkey1 = le_scan_keymap (pkeymap, (INT)1, oper1);
    pkey2 = le_scan_keymap (pkeymap, (INT)1, oper2);
    
    /*  if we found the two operations we can swap them                             */
    
    if (pkey1 IS_NOT_EQUAL_TO (
                          LE_KEYS *)0 AND pkey2 IS_NOT_EQUAL_TO (
                                                 LE_KEYS *)0 AND pkey1 IS_NOT_EQUAL_TO 
        pkey2)
        {
        /* allocate memory for temporary structure LE_KEYS                          */
        
        pkeymap = (LE_KEYS *) tc_zalloc (sizeof(LE_KEYS));
        *pkeymap = *pkey1;
        *pkey1 = *pkey2;
        pkey1->button = pkeymap->button;
        pkeymap->button = pkey2->button;
        *pkey2 = *pkeymap;
        
        /* release memory used for temporary structure                              */
        
        tc_free ((VOIDPTR)pkeymap);
        
        /* change the prompts if we have the puck image on the mac                  */
        
        if (pctrl->default_puck_map)
            {
            if (stroke_key)
                {
                lez_upd_key_prompt (cur_stroke_id[pctrl->index_stroke_keym],
                                    pkey1->button, pkey1->description);
                lez_upd_key_prompt (cur_stroke_id[pctrl->index_stroke_keym],
                                    pkey2->button, pkey2->description);
                }
            else
                {
                lez_upd_key_prompt (cur_point_id[pctrl->index_point_keym],
                                    pkey1->button, pkey1->description);
                lez_upd_key_prompt (cur_point_id[pctrl->index_point_keym],
                                    pkey2->button, pkey2->description);
                }
            }
        }
    return;
    }
/* END:                                                                             */

/* Function Description -----------------------------------------------------       */
/*Description:                                                                      */
/*                                                                                  */
/*Prototype:                                                                        */
/*    static VOID lez_term(LE_CONTROL *pctrl);                                  */
/*                                                                                  */
/*Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)        */
/*    (I) arg - description                                                         */
/*                                                                                  */
/*Return Value/Status:                                                              */
/*    None.                                                                         */
/*                                                                                  */
/*Scope:                                                                            */
/*    PRIVATE to <le_nlist_ed.c>                                                    */
/*                                                                                  */
/*Limitations/Assumptions:                                                          */
/*                                                                                  */
/*-----------------------------------------------------------------------------     */

/* BEGIN:                                                                           */
#if USE_PROTOTYPES
static VOID lez_term (LE_CONTROL *pctrl)
#else
static VOID lez_term (pctrl)
LE_CONTROL *pctrl;
#endif
    {
    INT i;
    
    for (i = 0; i < pctrl->num_stroke_keymaps; i++)
        {
        am_release_resource (cur_stroke_id[i]);
        cur_stroke_id[i] = 0;
        }
    for (i = 0; i < pctrl->num_point_keymaps; i++)
        {
        am_release_resource (cur_point_id[i]);
        cur_point_id[i] = 0;
        }
    return;
    }
/* END:                                                                             */

/* Function Description -----------------------------------------------------       */
/*Description:                                                                      */
/*                                                                                  */
/*Prototype:                                                                        */
/*    static LE_KEYS * lez_translate(INT button, LE_KEYS *current_keymap, INT indx);    */
/*                                                                                  */
/*Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)        */
/*    current_keymap  -(LE_KEYS *) Ptr to first key structure.                      */
/*    button          -(INT) Button pressed.                                        */
/*    indx            -(INT) Keymap index number.                                   */
/*                                                                                  */
/*Return Value/Status:                                                              */
/*    Function returns current_keymap + button + indx * MAX_KEYS.                   */
/*                                                                                  */
/*Scope:                                                                            */
/*    PRIVATE to <le_nlist_ed.c>                                                    */
/*                                                                                  */
/*Limitations/Assumptions:                                                          */
/*                                                                                  */
/*-----------------------------------------------------------------------------     */

/* BEGIN:                                                                           */
#if USE_PROTOTYPES
static LE_KEYS *lez_translate (INT button, LE_KEYS *current_keymap, INT indx)
#else
static LE_KEYS *lez_translate (button, current_keymap, indx)
LE_KEYS *current_keymap;
INT button;
INT indx;
#endif
    {
    return current_keymap + button + indx *MAX_KEYS;
    }
/* END:                                                                             */

/* Function Description -----------------------------------------------------       */
/*Description:                                                                      */
/*    Function used to toggle beetween last state of nlist and current state.       */
/*                                                                                  */
/*Prototype:                                                                        */
/*    static VOID lez_undo (LE_CONTROL **pctrl, LE_CONTROL **lastpctrl);        */
/*                                                                                  */
/*Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)        */
/*    pctrl           -(LE_CONTROL **)                                              */
/*    lastpctrl       -(LE_CONTROL **)                                              */
/*                                                                                  */
/*Return Value/Status:                                                              */
/*    None.                                                                         */
/*                                                                                  */
/*Scope:                                                                            */
/*    PRIVATE to <le_nlist_ed.c>                                                    */
/*                                                                                  */
/*Limitations/Assumptions:                                                          */
/*                                                                                  */
/*-----------------------------------------------------------------------------     */

/* BEGIN:                                                                           */
#if USE_PROTOTYPES
static VOID lez_undo (LE_CONTROL **pctrl, LE_CONTROL **lastpctrl)
#else
static VOID lez_undo (pctrl, lastpctrl)
LE_CONTROL **pctrl, **lastpctrl;
#endif
    {
    LE_CONTROL *pointer;
    
    lez_draw_line ((*pctrl)->nlist, &(*pctrl)->erase_attribute);
    /* draw last current state in draw- color                                       */
    /* toggle lastpctrl and pctrl                                                   */
    
    pointer = *pctrl;
    *pctrl = *lastpctrl;
    *lastpctrl = pointer;
    (*pctrl)->current_operation = BUTTON_UNDO;
    (*pctrl)->nbp = 0;
    lez_draw_line ((*pctrl)->nlist, &(*pctrl)->draw_attribute);
    
    return;
    }
/* END:                                                                             */

/* Function Description -----------------------------------------------------       */
/*Description:                                                                      */
/*                                                                                  */
/*Prototype:                                                                        */
/*    static VOID lez_update(INT lepk_id, BOOL default_puck_map);               */
/*                                                                                  */
/*Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)        */
/*    lepk_id         -(INT)                                                        */
/*    default_puck_map-(BOOL)                                                       */
/*                                                                                  */
/*Return Value/Status:                                                              */
/*    None.                                                                         */
/*                                                                                  */
/*Scope:                                                                            */
/*    PRIVATE to <le-nlist_ed.c>                                                    */
/*                                                                                  */
/*Limitations/Assumptions:                                                          */
/*                                                                                  */
/*-----------------------------------------------------------------------------     */

/* BEGIN:                                                                           */
#if USE_PROTOTYPES
static VOID lez_update (INT lepk_id, BOOL default_puck_map)
#else
static VOID lez_update (lepk_id, default_puck_map)
INT lepk_id;
BOOL default_puck_map;
#endif
    {
    
    if (default_puck_map)
        {
        if (if_mouse_menus && curr_err != LE_NONE)
            {
            am_message (AM_DIALOG, mg_message (curr_err));
            }
        else
            {
            wiscdt (lepk_id, ITEM_LEPKM_ERROR, mg_message (curr_err));
            }
        }
    if (curr_err IS_NOT_EQUAL_TO LE_NONE)
        {
        curr_err = LE_NONE;
        do_update = TRUE;
        }
    else
        {
        do_update = FALSE;
        }
    return;
    
    }
/* END:                                                                             */

/* Function Description -----------------------------------------------------       */
/*Description:                                                                      */
/*                                                                                  */
/*Prototype:                                                                        */
/*    static VOID lez_upd_key_prompt (INT lepk_id, INT button, CHAR *descript); */
/*                                                                                  */
/*Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)        */
/*    lepk_id         -(INT)                                                        */
/*    button          -(INT)                                                        */
/*    descript        -(CHAR *)                                                     */
/*                                                                                  */
/*Return Value/Status:                                                              */
/*    None.                                                                         */
/*                                                                                  */
/*Scope:                                                                            */
/*    PRIVATE to <le_nlist_ed.c>                                                    */
/*                                                                                  */
/*Limitations/Assumptions:                                                          */
/*                                                                                  */
/*-----------------------------------------------------------------------------     */

/* BEGIN:                                                                           */
#if USE_PROTOTYPES
static VOID lez_upd_key_prompt (INT lepk_id, INT button, CHAR *descript)
#else
static VOID lez_upd_key_prompt (lepk_id, button, descript)
INT lepk_id;
INT button;
CHAR *descript;
#endif
    {
    if (!if_mouse_menus)
        {
        wiscdt (lepk_id, dlg_item[button][1], descript);
        }
    return;
    }
/* END:                                                                             */

/* ***************************************************************************      */
/*                                                                                  */
/*                             KEYMAP ROUTINES                                      */
/*                                                                                  */
/* ***************************************************************************      */

/* Function Description -----------------------------------------------------       */
/*Description:                                                                      */
/*    Function %S Allocate new keymaps and fill them with the defaults provided     */
/*    for the indicated mode.                                                       */
/*                                                                                  */
/*Prototype:                                                                        */
/*    publicdef INT le_allocate_keymap(LE_KEYS  **pkeymap, INT mode, INT nmaps);    */
/*                                                                                  */
/*Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)        */
/*    pkeymap         -(LE_KEYS  **) Pointer to a keymap variable that will receive */
/*                        the address of the allocated keymap(s).                   */
/*    mode            -(INT) One of LE_POINT_MODE or LE_STROKE_MODE.                */
/*    nmaps           -(INT) Number of maps to allocate:                            */
/*                        for LE_POINT_MODE, nmaps >= 1                             */
/*                        for LE_STROKE_MODE, nmaps >= 2                            */
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
publicdef INT le_allocate_keymap (LE_KEYS **pkeymap, INT mode, INT nmaps)
#else
publicdef INT le_allocate_keymap (pkeymap, mode, nmaps)
LE_KEYS **pkeymap;
INT mode;
INT nmaps;
#endif
    {
    *pkeymap = (LE_KEYS *)0;        /* plan on failure                              */
    
    switch (mode)
        {                           /* validate the nmaps and mode parameters       */
        
    case LE_POINT_MODE:
        if (nmaps < NUM_DEF_POINT_KEYM)
            return FAIL;
        
        /* allocate the number of keymaps requested                                 */
        
        *pkeymap = (LE_KEYS *) tc_zalloc (nmaps * sizeof(LE_KEYMAP));
        
        /* move the default point keymap in                                         */
        
        hoblockmove ((VOIDPTR)def_point_keym, (VOIDPTR)*pkeymap, sizeof(LE_KEYMAP) * NUM_DEF_POINT_KEYM);
        break;
    case LE_STROKE_MODE:
        if (nmaps < NUM_DEF_STROKE_KEYM)
            return FAIL;
        
        /* allocate the number of keymaps requested                                 */
        
        *pkeymap = (LE_KEYS *) tc_zalloc (nmaps * sizeof(LE_KEYMAP));
        hoblockmove ((VOIDPTR)def_stroke_keym, (VOIDPTR)*pkeymap,
                     sizeof(LE_KEYMAP) * NUM_DEF_STROKE_KEYM);
        break;
    default:
        return FAIL;
        }
    return SUCCESS;
    }
/* END:                                                                             */

/* Function Description -----------------------------------------------------       */
/*Description:                                                                      */
/*                                                                                  */
/*Prototype:                                                                        */
/*    publicdef INT le_set_key(LE_KEYS *keymap, INT indx, INT button, INT operation,    */
/*                        INT (*pfunc)(), CHAR text[32], BOOL save_state);          */
/*                                                                                  */
/*Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)        */
/*    keymap          -(LE_KEYS *) Pointer to a keymap.                             */
/*    indx            -(INT) Keymap index number (starting at zero).                */
/*    button          -(INT) Button number to assign this function to.              */
/*    operation       -(INT) Symbolic function identifier.                          */
/*    pfunc           -(INT (*)()) Pointer to function to be executed when this button  */
/*                        is pressed.                                               */
/*    text            -(CHAR [32]) String to show to operator describing button function.   */
/*    save_state      -(BOOL) TRUE if this function can be undone.                  */
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
publicdef INT le_set_key (LE_KEYS *keymap, INT indx, INT button, INT operation,
                          INT_FUNCTION_PTR pfunc, CHAR text[32], BOOL save_state)
#else
publicdef INT le_set_key (keymap, indx, button, operation, pfunc, text, save_state)
LE_KEYS *keymap;
INT indx;
INT button;
INT operation;
INT_FUNCTION_PTR pfunc;
CHAR text[32];
BOOL save_state;
#endif
    {
    LE_KEYS *pKey;
    INT status;
    
    if ((button < 0) OR (button >= MAX_KEYS))
        {
        status = FAIL;
        }
    else
        {
        pKey = keymap + button + indx * MAX_KEYS;
        pKey->operation = operation;
        pKey->key_function = pfunc;
        strcpy ((char *)pKey->description, (char *)text);
        pKey->validt =
            (operation IS_NOT_EQUAL_TO BUTTON_NOOP) AND (
                                            operation IS_NOT_EQUAL_TO BUTTON_UNDEFINED) 
                       AND (pfunc IS_NOT_EQUAL_TO NULL_FUNCTION_PTR);
        pKey->index = indx;
        pKey->button = button;
        pKey->save_state = save_state;
        status = SUCCESS;
        }
    return status;
    }
/* END:                                                                             */

/* Function Description -----------------------------------------------------       */
/*Description:                                                                      */
/*    Function %S Scan the keymap array - return first key structure that matches   */
/*    the specified operation.                                                      */
/*                                                                                  */
/*Prototype:                                                                        */
/*    publicdef LE_KEYS * le_scan_keymap(LE_KEYS *keymap, INT nmaps, INT operation);*/
/*                                                                                  */
/*Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)        */
/*    keymap          -(LE_KEYS *)                                                  */
/*    nmaps           -(INT)                                                        */
/*    operation       -(INT)                                                        */
/*                                                                                  */
/*Return Value/Status:                                                              */
/*    Function returns the value of pKey.                                           */
/*                                                                                  */
/*Scope:                                                                            */
/*    PUBLIC                                                                        */
/*                                                                                  */
/*Limitations/Assumptions:                                                          */
/*                                                                                  */
/*-----------------------------------------------------------------------------     */

/* BEGIN:                                                                           */
#if USE_PROTOTYPES
publicdef LE_KEYS *le_scan_keymap (LE_KEYS *keymap, INT nmaps, INT operation)
#else
publicdef LE_KEYS *le_scan_keymap (keymap, nmaps, operation)
LE_KEYS *keymap;
INT nmaps;
INT operation;
#endif
    {
    INT i;
    INT nkeys = nmaps * MAX_KEYS;
    LE_KEYS *pKey;
    
    for (i = 0, pKey = keymap; i < nkeys; i++, pKey++)
        {
        if (pKey->operation EQUALS operation)
            {
            return pKey;
            }
        }
    pKey = (LE_KEYS *)0;
    return pKey;
    }
/* END:                                                                             */

/* Function Description -----------------------------------------------------       */
/*Description:                                                                      */
/*                                                                                  */
/*Prototype:                                                                        */
/*    publicdef INT le_enable_key(LE_KEYS *keymap, INT indx, INT button, BOOL onoff);   */
/*                                                                                  */
/*Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)        */
/*    keymap          -(LE_KEYS *) Pointer to keymap array.                         */
/*    indx            -(INT) Keymap index number (starting at zero).                */
/*    button          -(INT) Button number to have available state altered.         */
/*    onoff           -(BOOL) ON (1) or OFF (0).                                    */
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
publicdef INT le_enable_key (LE_KEYS *keymap, INT indx, INT button, BOOL onoff)
#else
publicdef INT le_enable_key (keymap, indx, button, onoff)
LE_KEYS *keymap;
INT indx;
INT button;
BOOL onoff;
#endif
    {
    
    /* change the enabled state of a button in a keymap                             */
    /* error checking                                                               */
    
    if ((keymap EQUALS (LE_KEYS *)0) OR (button < 0) OR (button >= MAX_KEYS))
        {
        return FAIL;
        }
    (keymap + button + indx * MAX_KEYS)->validt = onoff;
    return SUCCESS;
    }
/* END:                                                                             */

/* Function Description -----------------------------------------------------       */
/*Description:                                                                      */
/*                                                                                  */
/*Prototype:                                                                        */
/*    static INT lez_setup_tablet_keypad();                                     */
/*                                                                                  */
/*Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)        */
/*    None.                                                                         */
/*                                                                                  */
/*Return Value/Status:                                                              */
/*    See below.                                                                    */
/*                                                                                  */
/*Scope:                                                                            */
/*    PRIVATE to <le_nlist_ed.c>                                                    */
/*                                                                                  */
/*Limitations/Assumptions:                                                          */
/*                                                                                  */
/*-----------------------------------------------------------------------------     */

/* BEGIN:                                                                           */
#if USE_PROTOTYPES
static INT lez_setup_tablet_keypad ()
#else
static INT lez_setup_tablet_keypad ()
#endif
    {
    INT status = SUCCESS;
#if 0
    APPLICATION_ID  appCur;
    WINDOW_ID winCur;
    TNR_ID tnrCur;
    GROUP_ID groCur;
    SEGMENT_ID segCur;
    PICK_ID pickCur
    status = wmoinqcur (&appCur, &winCur, &tnrCur, &groCur, &segCur, &pickCur);
    status = wm_define_tablet_keypad (tnrCur, 4, 4, WM_HORI_KEYPAD);
    status = wm_set_pick_device (tnrCur, WM_PD_TABLET);
    status = wmtselect (tnrCur);
#endif
    return status;
    
    }
/* END:                                                                             */
/* Function Description -----------------------------------------------------       */
/*Description:                                                                      */
/*    This function is a prototype of an exposure event handler                     */
/*    for the nlist editor.  Its purpose is to refresh the current                  */
/*    working polyline whenever an exposure occurs on the gks window                */
/*    (e.g., from the pop-up menu, or from a resize of the window)                  */
/*                                                                                  */
/*    THIS METHOD IS NEEDED ONLY FOR DEC GKS!!                                      */
/*    PRIOR GKS DOES THIS CORRECTLY ON ITS OWN!!                                    */
/*                                                                                  */
/*Prototype:                                                                        */
/*    static VOID lez_redraw_line();                                            */
/*                                                                                  */
/*Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)        */
/*    None.                                                                         */
/*                                                                                  */
/*Return Value/Status:                                                              */
/*    See below.                                                                    */
/*                                                                                  */
/*Scope:                                                                            */
/*    PUBLIC                                                                        */
/*                                                                                  */
/*Limitations/Assumptions:                                                          */
/*                                                                                  */
/*-----------------------------------------------------------------------------     */

/* BEGIN:                                                                           */
#if USE_PROTOTYPES
static VOID lez_redraw_line (void)
#else
static VOID lez_redraw_line ()
#endif
    {
    lez_draw_line (lez_current_nlist, lez_current_attribute);
    }
/* END:                                                                             */
