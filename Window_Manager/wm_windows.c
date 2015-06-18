/* DEC/CMS REPLACEMENT HISTORY, Element WM_WINDOWS.C*/
/* *23    6-JUN-1991 15:58:09 MING "(SPR 0) add clip transformation"*/
/* *22   23-OCT-1990 14:00:02 KEE "(SPR -1) Motif and DecWindow Merge"*/
/* *21   15-OCT-1990 23:41:59 GILLESPIE "(SPR 1000) Merge Ernie deltas"*/
/*  18A1B1 12-OCT-1990 18:02:42 GILLESPIE "Merge Ernie Deltas"*/
/* *20   29-SEP-1990 11:36:19 GILLESPIE "(SPR 1) Beta Deltas"*/
/*  18A1 29-SEP-1990 11:34:19 GILLESPIE "Beta Deltas"*/
/* *19    9-AUG-1990 22:47:07 GILLESPIE "(SPR 2001) Change host dependency switch"*/
/* *18   30-JUN-1990 23:27:05 GILLESPIE "(SPR 1) Fix uninitialized var. in gpx_wmtlocate; add USE_PROTOTYPES; add castings"*/
/* *17   20-JUN-1990 10:23:01 MING "(SPR 0) add wm_get_window_size"*/
/* *16    7-JUN-1990 10:44:31 GILLESPIE "(SPR 1) Restore variable declarations in wm_init for SUN"*/
/* *15   21-MAY-1990 12:26:31 KEE "(SPR -1) Add testing before deactivate workstation"*/
/* *14   20-MAY-1990 16:42:46 SCD "(SPR 1) Make wmz_inq_wk_wins public; Add new function wm_wkid_to_window()"*/
/* *13   14-MAY-1990 14:27:56 SCD "(SPR 1) WMWDELETE now deletes GKS widgets"*/
/* *12    5-MAY-1990 21:35:24 SCD "(SPR 1) Fix wmwallocate to only use wmz_create_gks_widget when USE_X"*/
/* *11    3-MAY-1990 21:47:14 KEE "(SPR -1) multi window support."*/
/* *10    3-MAY-1990 21:40:35 KEE "(SPR -1) multi window stuff."*/
/* *9     4-MAR-1990 12:05:19 JULIAN "(SPR 6010) Fixx casting on ULTRIX version"*/
/* *8    28-FEB-1990 15:46:59 JULIAN "(SPR -1) Add element to WM_CURRENT struct, change MAIN_WKID & GKS_WISS_WKID def (Kee)"*/
/* *7    29-DEC-1989 18:05:27 JULIAN "(SPR 1) removed SUN conditional from WMTDRAGSEGMENT"*/
/* *6    13-NOV-1989 13:31:34 JULIAN "(SPR 1) Integrate external tablet functions into finder."*/
/* *5    21-SEP-1989 14:57:58 JULIAN "(SPR 1) added window size routine."*/
/* *4    14-SEP-1989 16:22:10 GORDON "(SPR 100) Gulf/Sun/Unix mods"*/
/* *3    18-JUL-1989 14:04:42 GORDON "(SPR 100) Merge in UNIX changes"*/
/* *2     1-JUL-1989 15:55:18 JULIAN "(SPR 5) fixed many enum clashes and also added more tablet support."*/
/* *1    19-JUN-1989 13:41:14 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element WM_WINDOWS.C*/
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
/*            Exploration Systems, Inc.                                             */
/*            579 Market Street                                                     */
/*            San Francisco, CA  USA 94105                                          */
/*            (415) 974-1600                                                        */
/*                                                                                  */
/*(The above notice does not constitute an admission of publication or              */
/*unrestricted dissemination of the work)                                           */
/*                                                                                  */
/******************************************************************************     */
#include "esi_c_lib.h"
#include "esi_math.h"
#include "esi_wm_int.h"

#ifndef ESI_LE_NLED_H

#include "esi_le_nled.h"

#endif

#ifndef ESI_GKS_H

#include "esi_gks.h"

#endif

#ifndef ESI_WM_CLS_H

#include "esi_wm_cls.h"

#endif

#ifndef ESI_AM_H

#include "esi_am.h"

#endif

#ifndef ESI_LL_H

#include "esi_ll.h"

#endif

#ifndef ESI_LU_H

#include "esi_lu.h"

#endif

#ifndef ESI_LL_TYPES_H

#include "esi_ll_types.h"

#endif

#ifndef ESI_BT_H

#include "esi_bt.h"

#endif

#ifndef ESI_BT_TYPES_H

#include "esi_bt_types.h"

#endif

#ifndef ESI_WI_H

#include "esi_wi.h"

#endif

#ifndef ESI_HO_H

#include "esi_ho.h"

#endif

#ifndef ESI_PR_H

#include "esi_pr.h"

#endif

#ifndef ESI_QS_H

#include "esi_qs.h"

#endif

#ifndef ESI_TC_H

#include "esi_tc.h"

#endif

#ifndef ESI_TG_H

#include "esi_tg.h"

#endif

#ifndef ESI_TS_H

#include "esi_ts.h"

#endif

#ifndef ESI_UG_H

#include "esi_ug.h"

#endif

#ifndef ESI_TU_H

#include "esi_tu.h"

#endif

#ifndef ESI_WM_ERR_H

#include "esi_wm_err.h"

#endif

#ifndef ESI_WM_MSG_H

#include "esi_wm_msg.h"

#endif

#ifndef ESI_TB_DEFS_H

#include "esi_tb_defs.h"

#endif

#include "esi_wi_int.x"               /* for journal/playback stuff */
#include "esi_as_yesno.h"
publicdef INT gpx_wmtlocate ();
publicdef INT wmctallocate ();
publicdef INT wmctselect ();

#define WM_MAX_WIN 100
#define WM_MAX_SEG 10000
#define WM_SEG_OFFSET 1000
#define WM_SCREEN_SEG_NUM 1             
#define WM_BACK_SEG_NUM 2
#define WM_BORDER_SEG_NUM 3             
/*  This is added to track the front window.  The routines that use                 */
/*    it are:                                                                       */
/*    WMINIT -- to initialize the value                                             */
/*    WMWOPEN -- to set the value                                                   */
/*    WMWFRONT -- also to set the value                                             */
/*    WM_INQ_FRONT_WINDOW -- to return the id of the front window.                  */
/*                                                                                  */
static WINDOW_ID FRONTED_WINDOW_FLAG;
static BOOL for_gpx;
static BOOL if_init = FALSE;
static BOOL if_interp = TRUE;
static INT reg_mode = WM_REG_MODE_OFF;
static INT num_malloc = 0;
static INT num_free = 0;
static WM_ID last_id = 0;
static TREEHEAD all_tree;
static TREEHEAD app_tree;
static LISTHEAD opn_gro_list;

static WM_CURRENT wmv_current;
static GKS_REAL IDENT_TM[6];
static GKS_REAL DEFAULT_TABXF[6];
static GKS_INT TEMP_TNR;
static SEGMENT_ID TEMP_SEG;
static PICK_ID TEMP_PIC;
static WINDOW_ID win_array[WM_MAX_WIN];
static INT num_windows;

/* Allocation Tree                                                                  */
typedef INT ALL_TREE_KEY;
typedef struct all_tree_data
    {
    INT typ;
    INT cla;
    CHAR dbs[WM_DBS_STR_SIZE];
    CHAR usr[WM_USR_STR_SIZE];
    BYTE *user_structure;
    APPLICATION_ID app;
    WINDOW_ID win;
    TNR_ID tnr;
    GROUP_ID gro;
    SEGMENT_ID seg;
    PICK_ID pic;
    } ALL_TREE_DATA;

/* Application Tree                                                                 */
typedef APPLICATION_ID APP_TREE_KEY;
typedef TREEHEAD *APP_TREE_DATA;

/* Window Tree                                                                      */
typedef WINDOW_ID WIN_TREE_KEY;
typedef struct win_tree_data
    {
    INT bank_number;                /* workstation bank for GKZ_MEMSEL              */
    SEGMENT_ID screen_seg;          /* segment id of the screen select segment      */
    SEGMENT_ID back_seg;
    INT num;                        /* set to current number of windows allocated   */
                                    /*by wmwallocate                                */
    
    INT nsegs;                      /* set to current number of segments allocated  */
    /* by wmwclose                                                                  */
    RECTANGLE ndc;
    GKS_INT color;
    GKS_INT wkid;                   /* workstation id                               */
    TREEHEAD *tnr;
    TREEHEAD *gro;
    TREEHEAD *seg;
    DOUBLE win_width;
    DOUBLE win_height;
    UOM_STRUCTURE win_uom;
    } WIN_TREE_DATA;

/* TNR Tree                                                                         */
typedef TNR_ID TNR_TREE_KEY;
typedef struct tnr_tree_data
    {
    RECTANGLE nwc;
    RECTANGLE wcs;
    GKS_REAL panx;
    GKS_REAL pany;
    GKS_REAL zoomx;
    GKS_REAL zoomy;
    GKS_INT wkid;                   /* workstation id                               */
    REGISTRATION reg;
    } TNR_TREE_DATA;

/* Group Tree                                                                       */
typedef GROUP_ID GRO_TREE_KEY;
typedef struct gro_tree_data
    {
    TREEHEAD *gro;
    TREEHEAD *seg;
    GKS_INT wkid;                   /* workstation id                               */
    } GRO_TREE_DATA;

/* Segment Tree                                                                     */
typedef SEGMENT_ID SEG_TREE_KEY;
typedef struct seg_tree_data
    {
    BOOL known_to_gks;
    TNR_ID tnr;
    INT num;
    GKS_INT wkid;                   /* workstation id                               */
    INT visibility;
    } SEG_TREE_DATA;

/* Open Group List                                                                  */
typedef struct opn_gro_list_data
    {
    GROUP_ID id;
    TREEHEAD *gro;
    TREEHEAD *seg;
    } OPN_GRO_LIST_DATA;

/* Error macros                                                                     */

#define IF_NOT_INIT_RETURN   \
    if (! if_init)   { return WM_ERR_NOT_INIT ; }
#define IF_NOT_APP_OPEN_RETURN    \
    IF_NOT_INIT_RETURN ;      \
    if (! wmv_current.app_id)  { return WM_ERR_NOT_APP_OPEN ; }
        
#define IF_NOT_WIN_OPEN_RETURN    \
    IF_NOT_APP_OPEN_RETURN ;  \
    if (! wmv_current.win_id)  { return WM_ERR_NOT_WIN_OPEN ; }

#define IF_NOT_GRO_OPEN_RETURN    \
    IF_NOT_WIN_OPEN_RETURN ;  \
    if (! wmv_current.gro_id)  { return WM_ERR_NOT_GRO_OPEN ; }

#define IF_NOT_SEG_OPEN_RETURN    \
    IF_NOT_WIN_OPEN_RETURN ;  \
    if (! wmv_current.seg_id)  { return WM_ERR_NOT_SEG_OPEN ; }

#define IF_NOT_PIC_OPEN_RETURN    \
    IF_NOT_SEG_OPEN_RETURN ;  \
    if (! wmv_current.pic_id)  { return WM_ERR_NOT_PIC_OPEN ; }

#define CLOSE_PIC  \
    if (wmv_current.pic_id)     \
    {     \
       returned_status = WM_ERR_PIC_OPEN ;  \
       wmpclose( wmv_current.pic_id ) ;   \
    }

#define CLOSE_SEG  \
    CLOSE_PIC ;     \
    if (wmv_current.seg_id)     \
    {     \
       returned_status = WM_ERR_SEG_OPEN ;  \
       wmsclose( wmv_current.seg_id ) ;   \
    }

#define CLOSE_GRO  \
    CLOSE_SEG ;     \
    while (wmv_current.gro_id)  \
    {     \
       returned_status = WM_ERR_GRO_OPEN ;  \
       wmgclose( wmv_current.gro_id ) ;   \
    }

#define CLOSE_WIN  \
    CLOSE_GRO ;     \
    if (wmv_current.win_id)     \
    {     \
       returned_status = WM_ERR_WIN_OPEN ;  \
       wmwclose( wmv_current.win_id ) ;   \
    }

#define CLOSE_APP  \
    CLOSE_WIN ;     \
    if (wmv_current.app_id)     \
    {     \
       returned_status = WM_ERR_APP_OPEN ;  \
       wmaclose( wmv_current.app_id ) ;   \
    }

#define GKS_PRIOR(w,s) ((1.0 / WM_MAX_WIN) * (w - 1.0)) + \
      (((1.0 / WM_MAX_WIN) / WM_MAX_SEG) * (s - 1.0))


/* Internal Routines                                                                */

#if USE_PROTOTYPES
static VOIDPTR zzz_malloc (size_t num_bytes);
static INT zzs_find (SEGMENT_ID seg_id, TREEHEAD *seg_tree_ptr,
                         TREEHEAD *gro_tree_ptr, SEG_TREE_DATA *seg_data_ptr);
static BOOL zzt_ask_reset (CHAR *text);
static INT zzt_do_puckkey (DOUBLE wx, DOUBLE wy, INT puckkey);
static WM_ID zzz_next_id (INT what_id);
static VOID zzg_clear (TREEHEAD *seg_tree_ptr, TREEHEAD *gro_tree_ptr);
static VOID zzg_delete (TREEHEAD *seg_tree_ptr, TREEHEAD *gro_tree_ptr);
static VOID zzg_pop (void);
static VOID zzg_push (void);
static VOID zzg_set_dtec (TREEHEAD *seg_tree_ptr, TREEHEAD *gro_tree_ptr,
                              INT detectability);
static VOID zzg_set_hlit (TREEHEAD *seg_tree_ptr, TREEHEAD *gro_tree_ptr,
                              INT hilighting);
static VOID zzg_set_vis (TREEHEAD *seg_tree_ptr, TREEHEAD *gro_tree_ptr,
                             INT visibility);
static BOOL zzt_chk_pseudo_key (REGISTRATION *reg, INT x, INT y, INT *button);
static VOID zzt_pan_zoom (TNR_ID tnr_id, TREEHEAD *seg_tree_ptr,
                              TREEHEAD *gro_tree_ptr);
static VOID zzt_select (TNR_ID tnr_id);
static VOID zzct_select (TNR_ID tnr_id);
static VOID zzt_set_registration (void);
static VOID zzw_inq_slist (TREEHEAD *seg_tree_ptr, TREEHEAD *gro_tree_ptr,
                               LISTHEAD *seg_list_ptr);
static VOID zzw_prior (TREEHEAD *seg_tree_ptr, TREEHEAD *gro_tree_ptr,
                           WINDOW_ID win_num);
static VOID zzw_set_ndc (TREEHEAD *seg_tree_ptr, TREEHEAD *gro_tree_ptr,
                             GKS_REAL TM[6]);
static VOID zzz_free (VOIDPTR a_ptr);
static VOID zzz_store_str_n (CHAR *where, CHAR *what, size_t limit);
static VOID zzz_term1 (TREEHEAD *win_tree_ptr);
static VOID zzz_term2 (TREEHEAD *seg_tree_ptr, TREEHEAD *gro_tree_ptr);
static BOOL wmz_seg_found (TREEHEAD *gro_tree_ptr, SEGMENT_ID *seg_id,
                               SEG_TREE_DATA *seg_data);

#else
static VOIDPTR zzz_malloc ();
static INT zzs_find ();
static BOOL zzt_ask_reset ();
static INT zzt_do_puckkey ();
static WM_ID zzz_next_id ();
static VOID zzg_clear ();
static VOID zzg_delete ();
static VOID zzg_pop ();
static VOID zzg_push ();
static VOID zzg_set_dtec ();
static VOID zzg_set_hlit ();
static VOID zzg_set_vis ();
static BOOL zzt_chk_pseudo_key ();
static VOID zzt_pan_zoom ();
static VOID zzt_select ();
static VOID zzct_select ();
static VOID zzt_set_registration ();
static VOID zzw_inq_slist ();
static VOID zzw_prior ();
static VOID zzw_set_ndc ();
static VOID zzz_free ();
static VOID zzz_store_str_n ();
static VOID zzz_term1 ();
static VOID zzz_term2 ();
static BOOL wmz_seg_found ();

#endif

/* Internal routine macros                                                          */
#define next_win_id() (WINDOW_ID)zzz_next_id( WM_CLASS_WINDOW )
#define next_tnr_id() (TNR_ID)zzz_next_id( WM_CLASS_TNR )
#define next_gro_id() (GROUP_ID)zzz_next_id( WM_CLASS_GROUP )
#define next_seg_id() (SEGMENT_ID)zzz_next_id( WM_CLASS_SEGMENT )
#define next_pic_id() (PICK_ID)zzz_next_id( WM_CLASS_PICK )

/* === WMINIT ===                                                                   */

#if USE_PROTOTYPES
publicdef INT wminit (void)
#else
publicdef INT wminit ()
#endif
    {
    
    INT returned_status = SUCCESS;
    INT tree_type, list_type, key_size, data_size, control;
    GKS_REAL XO = 0.0, YO = 0.0, DX = 0.0, DY = 0.0;
    GKS_REAL PHI = 0.0, FX = 1.0, FY = 1.0;
    GKS_INT SW = 1;
    
    if (if_init)
        {
        return WM_ERR_NOT_TERM;
        }
    /* The following change is added to initialize                                  */
    /*the "do segment priority update" flag.                                        */
    
    GKZ_DOITSET ();                 /*  Set the flag, permitting updates.           */
    /*                                                                              */
    /*  The following change is added to initialize the "display while segment open"*/
    /*  flag, such that the initial default is OFF.  The user can control this      */
    /*  drawing mode via the button DRAWING on the window control dialog.           */
    /*                                                                              */
    GKZ_DRAWING_OFF ();             /*Set the flag, permitting dislay while seg open*/
    
    if_init = TRUE;
    num_malloc = 0;
    num_free = 0;
    last_id = 0;
    num_windows = 0;
    if_interp = TRUE;
    reg_mode = WM_REG_MODE_ABS;
    
    wmv_current.app_id = 0;
    wmv_current.win_id = 0;
    wmv_current.tnr_id = 0;
    wmv_current.gro_id = 0;
    wmv_current.seg_id = 0;
    wmv_current.pic_id = 0;
    
    FRONTED_WINDOW_FLAG = 0;
    
    wmv_current.win_tree_ptr = (TREEHEAD *)NULL;
    wmv_current.tnr_tree_ptr = (TREEHEAD *)NULL;
    wmv_current.gro_tree_ptr = (TREEHEAD *)NULL;
    wmv_current.seg_tree_ptr = (TREEHEAD *)NULL;
    
    wmv_current.ndc.min.x = 0.0;
    wmv_current.ndc.max.x = 0.0;
    wmv_current.ndc.min.y = 0.0;
    wmv_current.ndc.max.y = 0.0;
    
    wmv_current.nwc.min.x = 0.0;
    wmv_current.nwc.max.x = 0.0;
    wmv_current.nwc.min.y = 0.0;
    wmv_current.nwc.max.y = 0.0;
    
    wmv_current.wcs.min.x = 0.0;
    wmv_current.wcs.max.x = 0.0;
    wmv_current.wcs.min.y = 0.0;
    wmv_current.wcs.max.y = 0.0;
    
    wmv_current.panx = 0.0;
    wmv_current.pany = 0.0;
    wmv_current.zoomx = 0.0;
    wmv_current.zoomy = 0.0;
    
    tree_type = WM_APPLICATION_TREE;
    key_size = sizeof(APP_TREE_KEY);
    data_size = sizeof(APP_TREE_DATA);
    control = BT_TREE_SYSTEM;
    btinit (&app_tree, tree_type, key_size, data_size, control);
    
    tree_type = WM_ALLOCATION_TREE;
    key_size = sizeof(ALL_TREE_KEY);
    data_size = sizeof(ALL_TREE_DATA);
    control = BT_TREE_SYSTEM;
    btinit (&all_tree, tree_type, key_size, data_size, control);
    
    list_type = WM_OPEN_GROUP_LIST;
    data_size = sizeof(OPN_GRO_LIST_DATA);
    control = LL_LIST_SYSTEM;
    llinit (&opn_gro_list, list_type, data_size, control);
    
    /* init ident transform                                                         */
    wm_gevtm (XO, YO, DX, DY, PHI, FX, FY, SW, IDENT_TM);
    
    /* init default tablet transform                                                */
    wm_gevtm (XO, YO, DX, DY, PHI, FX, FY, SW, DEFAULT_TABXF);
    
    TEMP_TNR = (GKS_INT) next_tnr_id ();
    TEMP_SEG = next_seg_id ();
    TEMP_PIC = next_pic_id ();

#ifdef sun

#ifndef PRIOR_GKS  /* ========== BEGIN SUN GKS ========== */
    /* Go to full wkvp                                                              */
        {
        GKS_REAL dcMinX, dcMaxX, dcMinY, dcMaxY;
        
        wm_inq_wkvp_max (&dcMinX, &dcMaxX, &dcMinY, &dcMaxY);
        wm_set_wkvp (dcMinX, dcMaxX, dcMinY, dcMaxY);
        }
#endif  /* ========== END SUN GKS ========== */
#endif
    if (!(ug_if_gpx ()))
        {
        for_gpx = FALSE;
        }
    else
        {
        for_gpx = TRUE;
        }
    return returned_status;
    }

/* === WMTERM ===                                                                   */

#if USE_PROTOTYPES
publicdef INT wmterm (void)
#else
publicdef INT wmterm ()
#endif
    {
    INT returned_status = SUCCESS;
    INT app_count, count;
    APP_TREE_KEY app_key;
    APP_TREE_DATA app_data;
    
    IF_NOT_INIT_RETURN;
    CLOSE_APP;
    
    app_count = btcount (&app_tree);
    if (app_count > 0)
        {
        btfirst (&app_tree, (VOIDPTR)&app_key, (VOIDPTR)&app_data);
        zzz_term1 ((TREEHEAD *)app_data);
        for (count = 2; count <= app_count; count++)
            {
            btnext (&app_tree, (VOIDPTR)&app_key, (VOIDPTR)&app_data);
            zzz_term1 ((TREEHEAD *)app_data);
            }
        }
    btfree (&app_tree);
    btfree (&all_tree);
    llfree (&opn_gro_list);
    
    if (num_malloc != num_free)
        {
        returned_status = WM_ERR_MALLOC_FREE;
        }
    if_init = FALSE;
    
    return returned_status;
    }

/* === ZZZ_TERM1 ===                                                                */

#if USE_PROTOTYPES
static VOID zzz_term1 (TREEHEAD *win_tree_ptr)
#else
static VOID zzz_term1 (win_tree_ptr)
TREEHEAD *win_tree_ptr;
#endif
    {
    
    INT win_count, count;
    WIN_TREE_KEY win_key;
    WIN_TREE_DATA win_data;
    
    win_count = btcount (win_tree_ptr);
    if (win_count > 0)
        {
        btfirst (win_tree_ptr, (VOIDPTR)&win_key, (VOIDPTR)&win_data);
        btfree (win_data.tnr);
        zzz_free ((VOIDPTR)win_data.tnr);
        zzz_term2 (win_data.seg, win_data.gro);
        for (count = 2; count <= win_count; count++)
            {
            btnext (win_tree_ptr, (VOIDPTR)&win_key, (VOIDPTR)&win_data);
            btfree (win_data.tnr);
            zzz_free ((VOIDPTR)win_data.tnr);
            zzz_term2 (win_data.seg, win_data.gro);
            }
        }
    btfree (win_tree_ptr);
    zzz_free ((VOIDPTR)win_tree_ptr);
    }

/* === ZZZ_TERM2 ===                                                                */

#if USE_PROTOTYPES
static VOID zzz_term2 (TREEHEAD *seg_tree_ptr, TREEHEAD *gro_tree_ptr)
#else
static VOID zzz_term2 (seg_tree_ptr, gro_tree_ptr)
TREEHEAD *seg_tree_ptr, *gro_tree_ptr;
#endif
    {
    
    INT gro_count, count;
    GRO_TREE_KEY gro_key;
    GRO_TREE_DATA gro_data;
    
    btfree (seg_tree_ptr);
    zzz_free ((VOIDPTR)seg_tree_ptr);
    
    gro_count = btcount (gro_tree_ptr);
    if (gro_count > 0)
        {
        btfirst (gro_tree_ptr, (VOIDPTR)&gro_key, (VOIDPTR)&gro_data);
        zzz_term2 (gro_data.seg, gro_data.gro);
        for (count = 2; count <= gro_count; count++)
            {
            btnext (gro_tree_ptr, (VOIDPTR)&gro_key, (VOIDPTR)&gro_data);
            zzz_term2 (gro_data.seg, gro_data.gro);
            }
        }
    btfree (gro_tree_ptr);
    zzz_free ((VOIDPTR)gro_tree_ptr);
    }

/* === WMAOPEN ===                                                                  */

#if USE_PROTOTYPES
publicdef INT wmaopen (APPLICATION_ID app_id)
#else
publicdef INT wmaopen (app_id)
APPLICATION_ID app_id;
#endif
    {
    
    INT returned_status = SUCCESS;
    INT tree_type, key_size, data_size, control;
    APP_TREE_DATA win_tree_ptr;
    
    IF_NOT_INIT_RETURN;
    CLOSE_APP;
    
    if (!(btfind (&app_tree, (VOIDPTR)&app_id, (VOIDPTR)&win_tree_ptr)))
        {
        win_tree_ptr = (APP_TREE_DATA) zzz_malloc (sizeof(TREEHEAD));
        
        tree_type = WM_WINDOW_TREE;
        key_size = sizeof(WIN_TREE_KEY);
        data_size = sizeof(WIN_TREE_DATA);
        control = BT_TREE_SYSTEM;
        btinit (win_tree_ptr, tree_type, key_size, data_size, control);
        
        btadd (&app_tree, (VOIDPTR)&app_id, (VOIDPTR)&win_tree_ptr);
        }
    wmv_current.app_id = app_id;
    wmv_current.win_tree_ptr = win_tree_ptr;
    
    return returned_status;
    }

/* === WMACLOSE ===                                                                 */

#if USE_PROTOTYPES
publicdef INT wmaclose (APPLICATION_ID app_id)
#else
publicdef INT wmaclose (app_id)
APPLICATION_ID app_id;
#endif
    {
    
    INT returned_status = SUCCESS;
    
    IF_NOT_APP_OPEN_RETURN;
    CLOSE_WIN;
    
    if (app_id != wmv_current.app_id)
        {
        returned_status = WM_ERR_WRONG_APP;
        }
    wmv_current.app_id = 0;
    wmv_current.win_tree_ptr = (TREEHEAD *)NULL;
    
    return returned_status;
    }

/* === WMWALLOCATE_SIZED ===                                                        */

#if USE_PROTOTYPES
publicdef INT wmwallocate_sized (WINDOW_ID *win_id_ptr, DOUBLE nx_ll, DOUBLE nx_ur,
                                 DOUBLE ny_ll, DOUBLE ny_ur, GKS_INT win_color,
                                 DOUBLE scale_factor)
#else
publicdef INT wmwallocate_sized (win_id_ptr, nx_ll, nx_ur, ny_ll, ny_ur, win_color,
                                 scale_factor)
WINDOW_ID *win_id_ptr;
DOUBLE nx_ll, nx_ur, ny_ll, ny_ur;
GKS_INT win_color;
DOUBLE scale_factor;
#endif
    {
    
    INT returned_status = SUCCESS;
    INT tree_type;
    size_t key_size, data_size;
    INT control, screen_number;
    GKS_REAL vxmin = 0.0, vxmax = 1.0, vymin = 0.0, vymax = 1.0;
    GKS_REAL wxmin = 0.0, wxmax = 1.0, wymin = 0.0, wymax = 1.0;
    TREEHEAD *tnr_tree_ptr, *gro_tree_ptr, *seg_tree_ptr;
    GKS_INT NPTS = 4;
    static GKS_REAL PX[] = 
        {
        0.0, 0.0, 1.0, 1.0
        };
    static GKS_REAL PY[] = 
        {
        0.0, 1.0, 1.0, 0.0
        };
    GKS_INT SGNA;
    GKS_REAL PRIOR;
    TNR_ID tnr_id;
    SEGMENT_ID back_seg, trim_seg;
    WIN_TREE_DATA win_data;
    SEG_TREE_DATA seg_data;
    GKS_INT normal, background, line_style, fill_style;
    
    /*------------------------------------------------------------------------------*/
    /* Following is a hack to work around DECwindows bug which causes               */
    /* the right and bottom window borders to be clipped                            */
    /*------------------------------------------------------------------------------*/
    /*                                                                              */
    /* if (ug_if_gpx ())                                                            */
    /*{                                                                             */
    /*PX[2] = 0.9985;                                                               */
    /*PX[3] = 0.9985;                                                               */
    /*PY[0] = 0.005;                                                                */
    /*PY[3] = 0.005;                                                                */
    /*}                                                                             */
    /*                                                                              */
    IF_NOT_APP_OPEN_RETURN;
    CLOSE_WIN;
    
    if (num_windows == WM_MAX_WIN)
        {
        returned_status = WM_ERR_MAX_WIN;
        return returned_status;
        }
    if ((nx_ll < 0.0) || (nx_ur > 1.0) || (nx_ll >= nx_ur) || (ny_ll < 0.0) ||
        (ny_ur > 1.0) || (ny_ll >= ny_ur))
        {
        returned_status = WM_ERR_BAD_NDC;
        nx_ll = 0.0;
        nx_ur = 1.0;
        ny_ll = 0.0;
        ny_ur = 1.0;
        }
#ifdef USE_X
    /* -----------------------------                                                */
    /* create a true X window and do                                                */
    /* workstation context switching                                                */
    /* -----------------------------                                                */
    wmz_create_gks_widget (nx_ur, nx_ll, ny_ur, ny_ll, scale_factor);
#endif
    tnr_tree_ptr = (TREEHEAD *) zzz_malloc (sizeof(TREEHEAD));
    tree_type = WM_TNR_TREE;
    key_size = sizeof(TNR_TREE_KEY);
    data_size = sizeof(TNR_TREE_DATA);
    control = BT_TREE_SYSTEM;
    btinit (tnr_tree_ptr, tree_type, key_size, data_size, control);
    
    gro_tree_ptr = (TREEHEAD *) zzz_malloc (sizeof(TREEHEAD));
    tree_type = WM_GROUP_TREE;
    key_size = sizeof(GRO_TREE_KEY);
    data_size = sizeof(GRO_TREE_DATA);
    control = BT_TREE_SYSTEM;
    btinit (gro_tree_ptr, tree_type, key_size, data_size, control);
    
    seg_tree_ptr = (TREEHEAD *) zzz_malloc (sizeof(TREEHEAD));
    tree_type = WM_SEGMENT_TREE;
    key_size = sizeof(SEG_TREE_KEY);
    data_size = sizeof(SEG_TREE_DATA);
    control = BT_TREE_SYSTEM;
    btinit (seg_tree_ptr, tree_type, key_size, data_size, control);
    
    *win_id_ptr = next_win_id ();
    
    num_windows++;
    win_array[num_windows] = *win_id_ptr;
    
    win_data.bank_number = -1;      /*      indicate no memory bank selected yet    */
    win_data.screen_seg = -1;       /*       indicate no screen segment yet         */
    win_data.num = num_windows;
    win_data.nsegs = 0;
    win_data.color = win_color;
    win_data.ndc.min.x = nx_ll;
    win_data.ndc.max.x = nx_ur;
    win_data.ndc.min.y = ny_ll;
    win_data.ndc.max.y = ny_ur;
    
    win_data.tnr = tnr_tree_ptr;
    win_data.gro = gro_tree_ptr;
    win_data.seg = seg_tree_ptr;
    win_data.wkid = MAIN_WKID;      /* MGKS                                         */
    btadd (wmv_current.win_tree_ptr, (VOIDPTR)win_id_ptr, (VOIDPTR)&win_data);
    
    wmwopen (*win_id_ptr);
    
    wmtallocate (&tnr_id, vxmin, vxmax, vymin, vymax, wxmin, wxmax, wymin, wymax);
    wmoset ((WM_ID)tnr_id, WM_CLASS_NWC, "", WM_CLSTR_NWC);
    
    /*      Establish the binding of this window                                    */
    /*    to the current preferred workstation                                      */
    /*    screen:                                                                   */
    
    returned_status = wm_inq_current_screen (&screen_number);
    if (returned_status == SUCCESS)
        {
        wm_set_window_screen (*win_id_ptr, screen_number);
        }
    wmsallocate (&back_seg);
    if (win_color >= 0)
        {
        lu_text_to_index ("LINE_STYLE", &line_style, "SOLID");
        wm_gsfais (line_style);
        wm_gsfaci ((GKS_INT)win_color);
        PRIOR = GKS_PRIOR (wmv_current.win_num, WM_BACK_SEG_NUM);
        SGNA = back_seg;
        wm_gssgp (SGNA, (DOUBLE)PRIOR);
        wm_gfa (NPTS, PX, PY);
        }
    wmsclose (back_seg);
    wmoset ((WM_ID)back_seg, WM_CLASS_BACKGROUND, "", WM_CLSTR_BACKGROUND);
    btfind (wmv_current.seg_tree_ptr, (VOIDPTR)&back_seg, (VOIDPTR)&seg_data);
    seg_data.num = WM_BACK_SEG_NUM; /* next to lowest prior                         */
    btrepcur (wmv_current.seg_tree_ptr, (VOIDPTR)&back_seg, (VOIDPTR)&seg_data);
    
    return returned_status;
    }

/* === WMWALLOCATE ===                                                              */

#if USE_PROTOTYPES
publicdef INT wmwallocate (WINDOW_ID *win_id_ptr, DOUBLE nx_ll, DOUBLE nx_ur,
                           DOUBLE ny_ll, DOUBLE ny_ur, GKS_INT win_color)
#else
publicdef INT wmwallocate (win_id_ptr, nx_ll, nx_ur, ny_ll, ny_ur, win_color)
WINDOW_ID *win_id_ptr;
DOUBLE nx_ll, nx_ur, ny_ll, ny_ur;
GKS_INT win_color;
#endif
    {
    DOUBLE scale_factor = 1.0;
    return wmwallocate_sized (win_id_ptr, nx_ll, nx_ur, ny_ll, ny_ur, win_color,
                              scale_factor);
    }

/* === WM_SET_WINDOW_SCREEN ===                                                     */
/* Bind a newly allocated window to the request screen, or                          */
/*   change the binding of an existing window to a different screen.                */
/*                                                                                  */
/*   INITIAL CONDITIONS:  1) Window must be open or attached.                       */
/*                                                                                  */
#if USE_PROTOTYPES
INT wm_set_window_screen (WINDOW_ID win_id, INT screen_number)
#else
INT wm_set_window_screen (win_id, screen_number)
WINDOW_ID win_id;
INT screen_number;
#endif
    {
    INT returned_status = SUCCESS;
    GKS_INT SGNA, BANK_NUMBER;
    GKS_REAL PRIOR;
    WIN_TREE_DATA win_data;
    SEG_TREE_DATA seg_data;
    
    if (ug_if_gpx () == TRUE)
        return returned_status;
    
    /* Determine Raster bank number from screen number.                             */
    /*    This approach is probably dependent upon the specific Raster memory       */
    /*    configuration and MUST be audited.                                        */
    /*                                                                              */
    
    BANK_NUMBER = (screen_number == 2 ? 4 : 0);
    
    /* Fetch the win_data window structure associated with win_id, so we            */
    /*    can update it with the seg id of the screen selector segment.             */
    if (btfind (wmv_current.win_tree_ptr, (VOIDPTR)&win_id, (VOIDPTR)&win_data))
        {
        win_data.bank_number = BANK_NUMBER;
        
        /* If this window already allocated & associcated with a screen, then       */
        /*   remove the existing screen segment and rebuild below                   */
        /*                                                                          */
        if (win_data.screen_seg > 0)
            {
            wmsdelete (win_data.screen_seg);
            }
        /*      Allocate first segment of this window to hold the MEMSEL command,   */
        /*    and place it into the window structure.                               */
        /*                                                                          */
        wmsallocate (&win_data.screen_seg);
        
        /*      Send the memory select command                                      */
        
        GKZ_MEMSEL (&BANK_NUMBER);
        
        wmsclose (win_data.screen_seg);
        wmoset ((WM_ID)win_data.screen_seg, WM_CLASS_BACKGROUND, "",
                "Screen Selector");
        
        /*      Change the screen selector segment priority to be the lowest priority   */
        /*    in the window.  The next highest segment priority will be the window  */
        /*    background.                                                           */
        /*                                                                          */
        if (btfind (wmv_current.seg_tree_ptr, (VOIDPTR)&win_data.screen_seg,
                    (VOIDPTR)&seg_data))
            {
            seg_data.num = WM_SCREEN_SEG_NUM;   /* lowest prior                     */
            seg_data.known_to_gks = FALSE;
            btrepcur (wmv_current.seg_tree_ptr, (VOIDPTR)&win_data.screen_seg,
                      (VOIDPTR)&seg_data);
            
            /*      Compute the GKS priority corresponding to seg_data.num,         */
            /*    and inform GKS.                                                   */
            /*                                                                      */
            PRIOR = GKS_PRIOR (win_data.num, seg_data.num);
            SGNA = win_data.screen_seg;
            wm_gssgp (SGNA, (DOUBLE)PRIOR);
            
            /*      Update the window structure, in order to remember the segment id of */
            /*    the screen selector segment for subsequent dynamic reassignments of the   */
            /*    window.                                                           */
            /*                                                                      */
            btrepcur (wmv_current.win_tree_ptr, (VOIDPTR)&win_id, (VOIDPTR)&win_data);
            }
        else
            {
            returned_status = WM_ERR_SEG_NOT_FOUND;
            }
        }
    else
        {
        returned_status = WM_ERR_WIN_NOT_FOUND;
        }
    return returned_status;
    }
/* === WM_ATTACH_WINDOW_SCREEN ===                                                  */
/* Given desired window id, select the associated workstation screen.               */
/*   This function is used to maintain synchronization of window & screen           */
/*   by functions such as wmattach().  No memory select is performed, if the        */
/*   window id is zero (indicates we are positioned at the root node of the         */
/*   workstation tree.                                                              */
/*                                                                                  */

#if USE_PROTOTYPES
publicdef INT wm_attach_window_screen (WINDOW_ID win_id)
#else
publicdef INT wm_attach_window_screen (win_id)
WINDOW_ID win_id;
#endif
    {
    
    INT returned_status = SUCCESS;
    GKS_INT BANK_NUMBER;
    WIN_TREE_DATA win_data;
    
    if (ug_if_gpx () == TRUE)
        return returned_status;
    
    /*      Fetch the win_data window structure associated with win_id, so we       */
    /*    can update it with the seg id of the screen selector segment.             */
    /*                                                                              */
    if (win_id > 0)
        {
        if (btfind (wmv_current.win_tree_ptr, (VOIDPTR)&win_id, (VOIDPTR)&win_data))
            {
            BANK_NUMBER = win_data.bank_number;
            GKZ_MEMSEL (&BANK_NUMBER);
            }
        else
            {
            returned_status = FAIL;
            }
        }
    return returned_status;
    }
/* === WMWOPEN ===                                                                  */

#if USE_PROTOTYPES
publicdef INT wmwopen (WINDOW_ID win_id)
#else
publicdef INT wmwopen (win_id)
WINDOW_ID win_id;
#endif
    {
    INT returned_status = SUCCESS;
    TNR_ID tnr_id;
    WIN_TREE_DATA win_data;
    TNR_TREE_DATA tnr_data;
    GKS_INT BANK_NUMBER;
    IF_NOT_APP_OPEN_RETURN;
    CLOSE_WIN;
    
    if (btfind (wmv_current.win_tree_ptr, (VOIDPTR)&win_id, (VOIDPTR)&win_data))
        {
        if (ug_if_gpx () == FALSE)
            {
            /* Select the associated workstation head (memory bank)                 */
            if (win_data.bank_number >= 0)
                {
                BANK_NUMBER = win_data.bank_number;
                GKZ_MEMSEL (&BANK_NUMBER);
                }
            }
        wm_wkid_update (win_id);    /* MGKS - workstation context switching         */
        wmv_current.win_id = win_id;
        wmv_current.tnr_tree_ptr = win_data.tnr;
        wmv_current.gro_tree_ptr = win_data.gro;
        wmv_current.seg_tree_ptr = win_data.seg;
        
        wmv_current.ndc.min.x = win_data.ndc.min.x;
        wmv_current.ndc.max.x = win_data.ndc.max.x;
        wmv_current.ndc.min.y = win_data.ndc.min.y;
        wmv_current.ndc.max.y = win_data.ndc.max.y;
        wmv_current.win_num = win_data.num;
        wmv_current.nsegs = win_data.nsegs;
        
        if (btcount (wmv_current.tnr_tree_ptr) > 0)
            {
            btfirst (wmv_current.tnr_tree_ptr, (VOIDPTR)&tnr_id, (VOIDPTR)&tnr_data);
            returned_status = wmtselect (tnr_id);
            }
        }
    else
        {
        returned_status = WM_ERR_WIN_NOT_FOUND;
        }
    return returned_status;
    }

/* === WMWCLOSE ===                                                                 */

#if USE_PROTOTYPES
publicdef INT wmwclose (WINDOW_ID win_id)
#else
publicdef INT wmwclose (win_id)
WINDOW_ID win_id;
#endif
    {
    INT returned_status = SUCCESS;
    
    WIN_TREE_DATA win_data;
    
    IF_NOT_WIN_OPEN_RETURN;
    CLOSE_GRO;
    
    if (win_id == wmv_current.win_id)
        {
        FRONTED_WINDOW_FLAG = win_id;   /*  Set the current (front) window id.      */
        btfind (wmv_current.win_tree_ptr, (VOIDPTR)&win_id, (VOIDPTR)&win_data);
        win_data.nsegs = wmv_current.nsegs;
        btrepcur (wmv_current.win_tree_ptr, (VOIDPTR)&win_id, (VOIDPTR)&win_data);
        reg_mode = WM_REG_MODE_REL;
        zzt_set_registration ();
        reg_mode = WM_REG_MODE_ABS;
        }
    else
        {
        returned_status = WM_ERR_WRONG_WIN;
        }
    wmv_current.win_id = 0;
    wmv_current.tnr_tree_ptr = (TREEHEAD *)NULL;
    wmv_current.gro_tree_ptr = (TREEHEAD *)NULL;
    wmv_current.seg_tree_ptr = (TREEHEAD *)NULL;
    wmv_current.ndc.min.x = 0.0;
    wmv_current.ndc.max.x = 0.0;
    wmv_current.ndc.min.y = 0.0;
    wmv_current.ndc.max.y = 0.0;
    wmv_current.win_num = 0;
    wmv_current.nsegs = 0;
    
    wmv_current.tnr_id = 0;
    wmv_current.nwc.min.x = 0.0;
    wmv_current.nwc.max.x = 0.0;
    wmv_current.nwc.min.y = 0.0;
    wmv_current.nwc.max.y = 0.0;
    wmv_current.wcs.min.x = 0.0;
    wmv_current.wcs.max.x = 0.0;
    wmv_current.wcs.min.y = 0.0;
    wmv_current.wcs.max.y = 0.0;
    wmv_current.panx = 0.0;
    wmv_current.pany = 0.0;
    wmv_current.zoomx = 0.0;
    wmv_current.zoomy = 0.0;
    
    return returned_status;
    }

/* === WMWSETNDC ===                                                                */

#if USE_PROTOTYPES
publicdef INT wmwsetndc (WINDOW_ID win_id, DOUBLE x_min, DOUBLE x_max, DOUBLE y_min,
                         DOUBLE y_max)
#else
publicdef INT wmwsetndc (win_id, x_min, x_max, y_min, y_max)
WINDOW_ID win_id;
DOUBLE x_min, x_max, y_min, y_max;
#endif
    {
    INT returned_status = SUCCESS;
    
    GKS_INT SW = 1;
    GKS_REAL XO, YO, DX, DY, PHI, FX, FY, TM[6];
    TREEHEAD *seg_tree_ptr, *gro_tree_ptr;
    WIN_TREE_DATA win_data;
    
    /* This function logic may be wrong - Kee when I implement MGKS.                */
    /*   It is called by wmgwsetndc, wmwband and wmwgrag and these three            */
    /*   functions are never been called                                            */
    
    IF_NOT_APP_OPEN_RETURN;
    CLOSE_WIN;
    
    if (btfind (wmv_current.win_tree_ptr, (VOIDPTR)&win_id, (VOIDPTR)&win_data))
        {
        seg_tree_ptr = win_data.seg;
        gro_tree_ptr = win_data.gro;
        
        XO = win_data.ndc.min.x;
        YO = win_data.ndc.min.y;
        DX = x_min - win_data.ndc.min.x;
        DY = y_min - win_data.ndc.min.y;
        PHI = 0.0;
        FX = (x_max - x_min) / (win_data.ndc.max.x - win_data.ndc.min.x);
        FY = (y_max - y_min) / (win_data.ndc.max.y - win_data.ndc.min.y);
        wm_gevtm (XO, YO, DX, DY, PHI, FX, FY, SW, TM);
        
        win_data.ndc.min.x = x_min;
        win_data.ndc.max.x = x_max;
        win_data.ndc.min.y = y_min;
        win_data.ndc.max.y = y_max;
        btrepcur (wmv_current.win_tree_ptr, (VOIDPTR)&win_id, (VOIDPTR)&win_data);
        
        wmwopen (win_id);
        zzw_set_ndc (seg_tree_ptr, gro_tree_ptr, TM);
        wmwclose (win_id);
        }
    else
        {
        returned_status = WM_ERR_WIN_NOT_FOUND;
        }
    return returned_status;
    }

/* === ZZW_SET_NDC ===                                                              */

#if USE_PROTOTYPES
static VOID zzw_set_ndc (TREEHEAD *seg_tree_ptr, TREEHEAD *gro_tree_ptr,
                             GKS_REAL TM[6])
#else
static VOID zzw_set_ndc (seg_tree_ptr, gro_tree_ptr, TM)
TREEHEAD *seg_tree_ptr, *gro_tree_ptr;
GKS_REAL TM[6];
#endif
    {
    INT num_items, count;
    GKS_INT SGNA;
    GKS_REAL PRIOR;
    GRO_TREE_KEY gro_id;
    GRO_TREE_DATA gro_data;
    SEG_TREE_KEY seg_id;
    SEG_TREE_DATA seg_data;
    
    num_items = btcount (seg_tree_ptr);
    if (num_items > 0)
        {
        btfirst (seg_tree_ptr, (VOIDPTR)&seg_id, (VOIDPTR)&seg_data);
        
        /* Special segments controlled by window manager are not recreated,         */
        /*   as GKS does not know what is in them -- therefore there goodies will   */
        /*   be lost since wm_ginsg() will not copy them into the new segment       */
        /*                                                                          */
        if (seg_data.known_to_gks)
            {
            wmtselect (seg_data.tnr);
            SGNA = (GKS_INT)seg_id;
            wm_gssgt (SGNA, IDENT_TM);
            wm_grensg (SGNA, (GKS_INT)TEMP_SEG);
            wm_gcrsg (SGNA);
            wm_ginsg ((GKS_INT)TEMP_SEG, TM);
            wm_gclsg ();
            wm_gssgt (SGNA, wmv_current.TM);
            wm_gdsg ((GKS_INT)TEMP_SEG);
            PRIOR = GKS_PRIOR (wmv_current.win_num, seg_data.num);
            wm_gssgp (SGNA, (DOUBLE)PRIOR);
            }
        for (count = 1; count < num_items; count++)
            {
            btnext (seg_tree_ptr, (VOIDPTR)&seg_id, (VOIDPTR)&seg_data);
            
            if (seg_data.known_to_gks)
                {
                wmtselect (seg_data.tnr);
                SGNA = (GKS_INT)seg_id;
                wm_gssgt (SGNA, IDENT_TM);
                wm_grensg (SGNA, (GKS_INT)TEMP_SEG);
                wm_gcrsg (SGNA);
                wm_ginsg ((GKS_INT)TEMP_SEG, TM);
                wm_gclsg ();
                wm_gssgt (SGNA, wmv_current.TM);
                wm_gdsg ((GKS_INT)TEMP_SEG);
                PRIOR = GKS_PRIOR (wmv_current.win_num, seg_data.num);
                wm_gssgp (SGNA, (DOUBLE)PRIOR);
                }
            }
        }
    num_items = btcount (gro_tree_ptr);
    if (num_items > 0)
        {
        btfirst (gro_tree_ptr, (VOIDPTR)&gro_id, (VOIDPTR)&gro_data);
        zzw_set_ndc (gro_data.seg, gro_data.gro, TM);
        for (count = 1; count < num_items; count++)
            {
            btnext (gro_tree_ptr, (VOIDPTR)&gro_id, (VOIDPTR)&gro_data);
            zzw_set_ndc (gro_data.seg, gro_data.gro, TM);
            }
        }
    }

/* === WMWVIS ===                                                                   */

#if USE_PROTOTYPES
publicdef INT wmwvis (WINDOW_ID win_id, INT visibility)
#else
publicdef INT wmwvis (win_id, visibility)
WINDOW_ID win_id;
INT visibility;
#endif
    {
    INT returned_status = SUCCESS;
    WIN_TREE_DATA win_data;
    TREEHEAD *seg_tree_ptr, *gro_tree_ptr;
    
    IF_NOT_APP_OPEN_RETURN;
    CLOSE_SEG;
    
    wm_wkid_update (win_id);        /* MGKS - context switching                     */
    
    if (btfind (wmv_current.win_tree_ptr, (VOIDPTR)&win_id, (VOIDPTR)&win_data))
        {
        seg_tree_ptr = win_data.seg;
        gro_tree_ptr = win_data.gro;
        zzg_set_vis (seg_tree_ptr, gro_tree_ptr, visibility);
        }
    else
        {
        returned_status = WM_ERR_WIN_NOT_FOUND;
        }
    return returned_status;
    }

/* === WMWDTEC ===                                                                  */

#if USE_PROTOTYPES
publicdef INT wmwdtec (WINDOW_ID win_id, INT detectability)
#else
publicdef INT wmwdtec (win_id, detectability)
WINDOW_ID win_id;
INT detectability;
#endif
    {
    INT returned_status = SUCCESS;
    WIN_TREE_DATA win_data;
    TREEHEAD *seg_tree_ptr, *gro_tree_ptr;
    
    IF_NOT_APP_OPEN_RETURN;
    CLOSE_SEG;
    
    if (btfind (wmv_current.win_tree_ptr, (VOIDPTR)&win_id, (VOIDPTR)&win_data))
        {
        seg_tree_ptr = win_data.seg;
        gro_tree_ptr = win_data.gro;
        zzg_set_dtec (seg_tree_ptr, gro_tree_ptr, detectability);
        }
    else
        {
        returned_status = WM_ERR_WIN_NOT_FOUND;
        }
    return returned_status;
    }

/* === WMWHLIT ===                                                                  */

#if USE_PROTOTYPES
publicdef INT wmwhlit (WINDOW_ID win_id, INT highlighting)
#else
publicdef INT wmwhlit (win_id, highlighting)
WINDOW_ID win_id;
INT highlighting;
#endif
    {
    INT returned_status = SUCCESS;
    WIN_TREE_DATA win_data;
    TREEHEAD *seg_tree_ptr, *gro_tree_ptr;
    
    IF_NOT_APP_OPEN_RETURN;
    CLOSE_SEG;
    
    if (btfind (wmv_current.win_tree_ptr, (VOIDPTR)&win_id, (VOIDPTR)&win_data))
        {
        seg_tree_ptr = win_data.seg;
        gro_tree_ptr = win_data.gro;
        zzg_set_hlit (seg_tree_ptr, gro_tree_ptr, highlighting);
        }
    else
        {
        returned_status = WM_ERR_WIN_NOT_FOUND;
        }
    return returned_status;
    }

/* === WMWCLEAR ===                                                                 */

#if USE_PROTOTYPES
publicdef INT wmwclear (WINDOW_ID win_id)
#else
publicdef INT wmwclear (win_id)
WINDOW_ID win_id;
#endif
    {
    INT returned_status = SUCCESS;
    WIN_TREE_DATA win_data;
    TREEHEAD *seg_tree_ptr, *gro_tree_ptr, *tnr_tree_ptr;
    TNR_TREE_KEY tnr_key;
    TNR_TREE_DATA tnr_data;
    INT numItems, i;
    
    IF_NOT_APP_OPEN_RETURN;
    CLOSE_WIN;
    
    if (btfind (wmv_current.win_tree_ptr, (VOIDPTR)&win_id, (VOIDPTR)&win_data))
        {
        seg_tree_ptr = win_data.seg;
        gro_tree_ptr = win_data.gro;
        zzg_clear (seg_tree_ptr, gro_tree_ptr);
        
        tnr_tree_ptr = win_data.tnr;
        numItems = btcount (tnr_tree_ptr);
        if (numItems > 0)
            {
            for (i = 0; i < numItems; i++)
                {
                if (i == 0)
                    {
                    btfirst (tnr_tree_ptr, (VOIDPTR)&tnr_key, (VOIDPTR)&tnr_data);
                    }
                else
                    {
                    btnext (tnr_tree_ptr, (VOIDPTR)&tnr_key, (VOIDPTR)&tnr_data);
                    }
                btdel (&all_tree, (VOIDPTR)&tnr_key);
                }
            }
        }
    else
        {
        returned_status = WM_ERR_WIN_NOT_FOUND;
        }
    return returned_status;
    }

/* === WMWDELETE ===                                                                */

#if USE_PROTOTYPES
publicdef INT wmwdelete (WINDOW_ID win_id)
#else
publicdef INT wmwdelete (win_id)
WINDOW_ID win_id;
#endif
    {
    INT returned_status = SUCCESS;
    WIN_TREE_DATA win_data;
    TREEHEAD *seg_tree_ptr, *gro_tree_ptr, *tnr_tree_ptr;
    TNR_TREE_KEY tnr_key;
    TNR_TREE_DATA tnr_data;
    INT numItems, i;
    GKS_INT wk_id;
    
    IF_NOT_APP_OPEN_RETURN;
    CLOSE_WIN;
    
    if (btfind (wmv_current.win_tree_ptr, (VOIDPTR)&win_id, (VOIDPTR)&win_data))
        {
        wk_id = wm_obj_wkid ((WM_ID)win_id);
        seg_tree_ptr = win_data.seg;
        gro_tree_ptr = win_data.gro;
        zzg_delete (seg_tree_ptr, gro_tree_ptr);
        
        tnr_tree_ptr = win_data.tnr;
        numItems = btcount (tnr_tree_ptr);
        if (numItems > 0)
            {
            for (i = 0; i < numItems; i++)
                {
                if (i == 0)
                    {
                    btfirst (tnr_tree_ptr, (VOIDPTR)&tnr_key, (VOIDPTR)&tnr_data);
                    }
                else
                    {
                    btnext (tnr_tree_ptr, (VOIDPTR)&tnr_key, (VOIDPTR)&tnr_data);
                    }
                btdel (&all_tree, (VOIDPTR)&tnr_key);
                }
            }
        btfree (tnr_tree_ptr);
        zzz_free ((VOIDPTR)tnr_tree_ptr);
        
        btdel (wmv_current.win_tree_ptr, (VOIDPTR)&win_id);
        btdel (&all_tree, (VOIDPTR)&win_id);
        win_array[win_data.num] = 0;
        /* ------------------------------------------                               */
        /* Now delete the GKS workstation and widgets                               */
        /* NOTE: A better, faster way to do this is                                 */
        /*       to just free the WM memory, and let                                */
        /*       GKS free its state list                                            */
        /* ------------------------------------------                               */
#ifdef USE_X
        wmz_delete_workstation (wk_id);
#endif
        }
    else
        {
        returned_status = WM_ERR_WIN_NOT_FOUND;
        }
    return returned_status;
    }

/* === WMWFRONT ===                                                                 */

#if USE_PROTOTYPES
publicdef INT wmwfront (WINDOW_ID win_id)
#else
publicdef INT wmwfront (win_id)
WINDOW_ID win_id;
#endif
    {
    INT returned_status = SUCCESS;
    WIN_TREE_DATA win_data;
    TREEHEAD *seg_tree_ptr, *gro_tree_ptr;
    INT old_num;
    WINDOW_ID top_win;
    APPLICATION_ID app_id;
    APP_TREE_DATA app_data;
    INT app_count, count;
    INT deferral_changed = FALSE;
    INT defmod, regmod;
    
    INT  class ;
    CHAR db_name[WM_DBS_STR_SIZE];
    CHAR usr_name[WM_USR_STR_SIZE];
    
    IF_NOT_APP_OPEN_RETURN;
    CLOSE_WIN;

#ifndef USE_X
    /* ----------------------------------------------                               */
    /* Do nothing if the desired window is front.                                   */
    /* NOTE: Under X, using multiple GKS workstations                               */
    /*       we cannot reliably know what is the                                    */
    /*       front window, as user can change using                                 */
    /*       the X window manager.                                                  */
    /* ----------------------------------------------                               */
    if (FRONTED_WINDOW_FLAG == win_id)
        goto Leave;
#endif
    wm_inq_deferral (&defmod, &regmod);
    if (defmod != (GKS_INT)GBNIL || (regmod != (GKS_INT)GSUPPD))
        {
        deferral_changed = TRUE;
        wm_set_deferral ((GKS_INT)GBNIL, (GKS_INT)GSUPPD);
        }
    if (btfind (wmv_current.win_tree_ptr, (VOIDPTR)&win_id, (VOIDPTR)&win_data))
        {
        FRONTED_WINDOW_FLAG = win_id;   /* Set the current (front) window id.       */
        
        wm_wkid_update (win_id);    /* MGKS - workstation context switching         */
        if (wmz_inq_wk_wins (wm_obj_wkid ((WM_ID)win_id)) > 1)
            {
            wmoinq (win_id, &class , db_name, usr_name);    /* MGKS                 */
            wmz_update_gks_title (win_id, usr_name);    /* MGKS - retitle           */
            
            seg_tree_ptr = win_data.seg;
            gro_tree_ptr = win_data.gro;
            old_num = win_data.num;
            top_win = win_array[num_windows];
            win_array[old_num] = top_win;
            win_array[num_windows] = win_id;
            win_data.num = num_windows;
            btrepcur (wmv_current.win_tree_ptr, (VOIDPTR)&win_id, (VOIDPTR)&win_data);
            zzw_prior (seg_tree_ptr, gro_tree_ptr, (WINDOW_ID)win_data.num);
            if (top_win != 0)
                {
                app_count = btcount (&app_tree);
                if (app_count > 0)
                    {
                    btfirst (&app_tree, (VOIDPTR)&app_id, (VOIDPTR)&app_data);
                    if (btfind (app_data, (VOIDPTR)&top_win, (VOIDPTR)&win_data))
                        {
                        seg_tree_ptr = win_data.seg;
                        gro_tree_ptr = win_data.gro;
                        win_data.num = old_num;
                        btrepcur (app_data, (VOIDPTR)&top_win, (VOIDPTR)&win_data);
                        zzw_prior (seg_tree_ptr, gro_tree_ptr,
                                   (WINDOW_ID)win_data.num);
                        }
                    for (count = 2; count <= app_count; count++)
                        {
                        btnext (&app_tree, (VOIDPTR)&app_id, (VOIDPTR)&app_data);
                        if (btfind (app_data, (VOIDPTR)&top_win, (VOIDPTR)&win_data))
                            {
                            seg_tree_ptr = win_data.seg;
                            gro_tree_ptr = win_data.gro;
                            win_data.num = old_num;
                            btrepcur (app_data, (VOIDPTR)&top_win, (VOIDPTR)&win_data);
                            zzw_prior (seg_tree_ptr, gro_tree_ptr, win_data.num);
                            }
                        }
                    }
                }
            }
        }
    else
        {
        returned_status = WM_ERR_WIN_NOT_FOUND;
        }
Leave:
    if (deferral_changed == TRUE)
        wm_set_deferral (defmod, regmod);
    
    return returned_status;
    }

/* === ZZW_PRIOR ===                                                                */

#if USE_PROTOTYPES
static VOID zzw_prior (TREEHEAD *seg_tree_ptr, TREEHEAD *gro_tree_ptr,
                           WINDOW_ID win_num)
#else
static VOID zzw_prior (seg_tree_ptr, gro_tree_ptr, win_num)
TREEHEAD *seg_tree_ptr, *gro_tree_ptr;
WINDOW_ID win_num;
#endif
    {
    
    SEG_TREE_KEY seg_id;
    SEG_TREE_DATA seg_data;
    GRO_TREE_KEY gro_id;
    GRO_TREE_DATA gro_data;
    INT num_items, count;
    GKS_REAL PRIOR;
    
    num_items = btcount (seg_tree_ptr);
    if (num_items > 0)
        {
        /*                                                                          */
        /*  This change is done to suspend updating the raster tech segment list    */
        /*  until all priority changes have been made.                              */
        /*                                                                          */
        if (ug_if_gpx () == FALSE)
            {
            GKZ_DOITRESET ();
            }
        btfirst (seg_tree_ptr, (VOIDPTR)&seg_id, (VOIDPTR)&seg_data);
        PRIOR = GKS_PRIOR (win_num, seg_data.num);
        wm_gssgp ((GKS_INT)seg_id, (DOUBLE)PRIOR);
        
        for (count = 1; count < num_items; count++)
            {
            btnext (seg_tree_ptr, (VOIDPTR)&seg_id, (VOIDPTR)&seg_data);
            PRIOR = GKS_PRIOR (win_num, seg_data.num);
            wm_gssgp ((GKS_INT)seg_id, (DOUBLE)PRIOR);
            }
        /*  Permit updating the raster tech.  See GKBSEG for the code change.       */
        if (ug_if_gpx () == FALSE)
            GKZ_DOITSET ();
        
        wm_gssgp (seg_id, (DOUBLE)PRIOR);
        }
    num_items = btcount (gro_tree_ptr);
    if (num_items > 0)
        {
        btfirst (gro_tree_ptr, (VOIDPTR)&gro_id, (VOIDPTR)&gro_data);
        zzw_prior (gro_data.seg, gro_data.gro, win_num);
        for (count = 1; count < num_items; count++)
            {
            btnext (gro_tree_ptr, (VOIDPTR)&gro_id, (VOIDPTR)&gro_data);
            zzw_prior (gro_data.seg, gro_data.gro, win_num);
            }
        }
    }

/* === WMWINQ ===                                                                   */

#if USE_PROTOTYPES
publicdef INT wmwinq (WINDOW_ID win_id, GKS_REAL *nx_ll, GKS_REAL *nx_ur,
                      GKS_REAL *ny_ll, GKS_REAL *ny_ur, GKS_INT *color)
#else
publicdef INT wmwinq (win_id, nx_ll, nx_ur, ny_ll, ny_ur, color)
WINDOW_ID win_id;
GKS_REAL *nx_ll, *nx_ur, *ny_ll, *ny_ur;
GKS_INT *color;
#endif
    {
    
    INT returned_status = SUCCESS;
    
    WIN_TREE_DATA win_data;
    
    *nx_ll = 0.0;
    *nx_ur = 0.0;
    *ny_ll = 0.0;
    *ny_ur = 0.0;
    *color = 0;
    
    IF_NOT_APP_OPEN_RETURN;
    
    if (btfind (wmv_current.win_tree_ptr, (VOIDPTR)&win_id, (VOIDPTR)&win_data))
        {
        *nx_ll = win_data.ndc.min.x;
        *nx_ur = win_data.ndc.max.x;
        *ny_ll = win_data.ndc.min.y;
        *ny_ur = win_data.ndc.max.y;
        *color = win_data.color;
        }
    else
        {
        returned_status = WM_ERR_WIN_NOT_FOUND;
        }
    return returned_status;
    }

/* === WMWINQLIST ===                                                               */

#if USE_PROTOTYPES
publicdef INT wmwinqlist (INT  class , LISTHEAD *win_list)
#else
publicdef INT wmwinqlist (class , win_list)
INT  class ;
LISTHEAD *win_list;
#endif
    {
    
    INT returned_status = SUCCESS;
    WIN_TREE_KEY win_key;
    WIN_TREE_DATA win_data;
    INT count, i;
    ALL_TREE_DATA all_data;
    
    IF_NOT_APP_OPEN_RETURN;
    
    if (llcount (win_list) > 0)
        {
        llfree (win_list);
        }
    count = btcount (wmv_current.win_tree_ptr);
    for (i = 1; count > 0 && i <= count; i++)
        {
        if (i == 1)
            {
            btfirst (wmv_current.win_tree_ptr, (VOIDPTR)&win_key, (VOIDPTR)&win_data);
            }
        else
            {
            btnext (wmv_current.win_tree_ptr, (VOIDPTR)&win_key, (VOIDPTR)&win_data);
            }
        btfind (&all_tree, (VOIDPTR)&win_key, (VOIDPTR)&all_data);
        if ((all_data.cla == class) || (class  == WM_CLASS_WINDOW))
            {
            llappnd (win_list, (VOIDPTR)&win_key);
            }
        }
    return returned_status;
    }

/* === WMTALLOCATE ===                                                              */

#if USE_PROTOTYPES
publicdef INT wmtallocate (TNR_ID *tnr_id_ptr, DOUBLE vx_ll, DOUBLE vx_ur,
                           DOUBLE vy_ll, DOUBLE vy_ur, DOUBLE wx_ll, DOUBLE wx_ur,
                           DOUBLE wy_ll, DOUBLE wy_ur)
#else
publicdef INT wmtallocate (tnr_id_ptr, vx_ll, vx_ur, vy_ll, vy_ur, wx_ll, wx_ur, wy_ll,
                           wy_ur)
TNR_ID *tnr_id_ptr;
DOUBLE vx_ll, vx_ur, vy_ll, vy_ur;
DOUBLE wx_ll, wx_ur, wy_ll, wy_ur;
#endif
    {
    INT returned_status = SUCCESS;
    
    TNR_TREE_DATA tnr_data;
    
    IF_NOT_WIN_OPEN_RETURN;
    
    /* allow multi tnrs in a segment                                                */
    /*   CLOSE_SEG ;                                                                */
    /*                                                                              */
    
    if ((vx_ll < 0.0) || (vx_ur > 1.0) || (vx_ll >= vx_ur) || (vy_ll < 0.0) ||
        (vy_ur > 1.0) || (vy_ll >= vy_ur))
        {
        returned_status = WM_ERR_BAD_NWC;
        vx_ll = 0.0;
        vx_ur = 1.0;
        vy_ll = 0.0;
        vy_ur = 1.0;
        }
    if ((wx_ll >= wx_ur) || (wy_ll >= wy_ur))
        {
        returned_status = WM_ERR_BAD_WCS;
        wx_ll = 0.0;
        wx_ur = 1.0;
        wy_ll = 0.0;
        wy_ur = 1.0;
        }
    *tnr_id_ptr = next_tnr_id ();
    
    tc_zeroes ((VOIDPTR)&tnr_data, sizeof(TNR_TREE_DATA));
    tnr_data.nwc.min.x = vx_ll;
    tnr_data.nwc.max.x = vx_ur;
    tnr_data.nwc.min.y = vy_ll;
    tnr_data.nwc.max.y = vy_ur;
    
    tnr_data.wcs.min.x = wx_ll;
    tnr_data.wcs.max.x = wx_ur;
    tnr_data.wcs.min.y = wy_ll;
    tnr_data.wcs.max.y = wy_ur;
    
    tnr_data.panx = (wx_ll + ((wx_ur - wx_ll) / 2));
    tnr_data.pany = (wy_ll + ((wy_ur - wy_ll) / 2));
    tnr_data.zoomx = 1.0;
    tnr_data.zoomy = 1.0;
    
    tnr_data.reg.ifset = FALSE;
    tnr_data.wkid = MAIN_WKID;      /* MGKS                                         */
    btadd (wmv_current.tnr_tree_ptr, (VOIDPTR)tnr_id_ptr, (VOIDPTR)&tnr_data);
    
    wmtselect (*tnr_id_ptr);
    
    return returned_status;
    }

/* === WMTSELECT ===                                                                */

#if USE_PROTOTYPES
publicdef INT wmtselect (TNR_ID tnr_id)
#else
publicdef INT wmtselect (tnr_id)
TNR_ID tnr_id;
#endif
    {
    
    INT returned_status = SUCCESS;
    TNR_TREE_DATA tnr_data;
    INT status;
    
    IF_NOT_WIN_OPEN_RETURN;
    
    /* allow multi tnrs in a segment                                                */
    /*   CLOSE_SEG ;                                                                */
    /*                                                                              */
    
    if (tnr_id)
        {
        status = btfind (wmv_current.tnr_tree_ptr, (VOIDPTR)&tnr_id,
                         (VOIDPTR)&tnr_data);
        if (status)
            {
            zzt_select (tnr_id);
            }
        else
            {
            returned_status = WM_ERR_TNR_NOT_FOUND;
            }
        }
    else
        {
        btfirst (wmv_current.tnr_tree_ptr, (VOIDPTR)&tnr_id, (VOIDPTR)&tnr_data);
        zzt_select (tnr_id);
        }
    return returned_status;
    }

/* === ZZT_SELECT ===                                                               */

#if USE_PROTOTYPES
static VOID zzt_select (TNR_ID tnr_id)
#else
static VOID zzt_select (tnr_id)
TNR_ID tnr_id;
#endif
    {
    TNR_TREE_DATA tnr_data;
    GKS_REAL VX1, VX2, VY1, VY2;
    GKS_REAL WX1, WX2, WY1, WY2;
    GKS_REAL wcs_dx, wcs_dy;
    GKS_REAL ndc_dx, ndc_dy;
    INT status;
    GKS_REAL XO = 0.0, YO = 0.0, DX = 0.0, DY = 0.0, PHI = 0.0, FX = 1.0, FY = 1.0;
    GKS_INT SW = 1;
    GKS_REAL cx, cy;
    RECTANGLE nwcRect, wcsRect;
    PIPE nwc2wcs;
    
    status = btfind (wmv_current.tnr_tree_ptr, (VOIDPTR)&tnr_id, (VOIDPTR)&tnr_data);
    if (status)
        {
        wmv_current.tnr_id = tnr_id;
        
        wmv_current.nwc.min.x = tnr_data.nwc.min.x;
        wmv_current.nwc.max.x = tnr_data.nwc.max.x;
        wmv_current.nwc.min.y = tnr_data.nwc.min.y;
        wmv_current.nwc.max.y = tnr_data.nwc.max.y;
        
        wmv_current.wcs.min.x = tnr_data.wcs.min.x;
        wmv_current.wcs.max.x = tnr_data.wcs.max.x;
        wmv_current.wcs.min.y = tnr_data.wcs.min.y;
        wmv_current.wcs.max.y = tnr_data.wcs.max.y;
        
        wmv_current.panx = tnr_data.panx;
        wmv_current.pany = tnr_data.pany;
        wmv_current.zoomx = tnr_data.zoomx;
        wmv_current.zoomy = tnr_data.zoomy;
        
        wmv_current.reg = tnr_data.reg;
        
        wcs_dx = wmv_current.wcs.max.x - wmv_current.wcs.min.x;
        wcs_dy = wmv_current.wcs.max.y - wmv_current.wcs.min.y;

        ndc_dx = wmv_current.ndc.max.x - wmv_current.ndc.min.x;
        ndc_dy = wmv_current.ndc.max.y - wmv_current.ndc.min.y;

#if 0
        VX1 = wmv_current.ndc.min.x + (wmv_current.nwc.min.x);
        VX2 = wmv_current.ndc.min.x + (wmv_current.nwc.max.x);
        VY1 = wmv_current.ndc.min.y + (wmv_current.nwc.min.y);
        VY2 = wmv_current.ndc.min.y + (wmv_current.nwc.max.y);

        VX1 = wmv_current.ndc.min.x + (wmv_current.nwc.min.x * ndc_dx);
        VX2 = wmv_current.ndc.min.x + (wmv_current.nwc.max.x * ndc_dx);
        VY1 = wmv_current.ndc.min.y + (wmv_current.nwc.min.y * ndc_dy);
        VY2 = wmv_current.ndc.min.y + (wmv_current.nwc.max.y * ndc_dy);
#endif

        /* use entire window for clipping                                           */
        VX1 = wmv_current.ndc.min.x;
        VX2 = wmv_current.ndc.max.x;
        VY1 = wmv_current.ndc.min.y;
        VY2 = wmv_current.ndc.max.y;
        wm_gsvp (TEMP_TNR, VX1, VX2, VY1, VY2);
        
        cx = wmv_current.wcs.min.x + ((wmv_current.wcs.max.x - wmv_current.wcs.min.x) /
                                      2);
        cy = wmv_current.wcs.min.y + ((wmv_current.wcs.max.y - wmv_current.wcs.min.y) /
                                      2);
        wcsRect.min.x = wmv_current.panx + ((wmv_current.wcs.min.x - cx) /
                                            wmv_current.zoomx);
        wcsRect.max.x = wmv_current.panx + ((wmv_current.wcs.max.x - cx) /
                                            wmv_current.zoomx);
        wcsRect.min.y = wmv_current.pany + ((wmv_current.wcs.min.y - cy) /
                                            wmv_current.zoomy);
        wcsRect.max.y = wmv_current.pany + ((wmv_current.wcs.max.y - cy) /
                                            wmv_current.zoomy);
        PR_SetPipe (&nwc2wcs, &wmv_current.nwc, &wcsRect);
        /* entire window                                                            */
        nwcRect.min.x = 0.0;
        nwcRect.max.x = 1.0;
        nwcRect.min.y = 0.0;
        nwcRect.max.y = 1.0;
        PR_PipeRect (&nwcRect, &nwc2wcs, &wcsRect);
        WX1 = wcsRect.min.x;
        WX2 = wcsRect.max.x;
        WY1 = wcsRect.min.y;
        WY2 = wcsRect.max.y;
        wm_gswn (TEMP_TNR, WX1, WX2, WY1, WY2);
        
        wm_gselnt (TEMP_TNR);
        
        XO = tnr_data.panx;
        YO = tnr_data.pany;
        DX = (wmv_current.wcs.min.x + (wcs_dx / 2)) - tnr_data.panx;
        DY = (wmv_current.wcs.min.y + (wcs_dy / 2)) - tnr_data.pany;
        PHI = 0.0;
        FX = tnr_data.zoomx;
        FY = tnr_data.zoomy;
        SW = 0;
        wm_gevtm (XO, YO, DX, DY, PHI, FX, FY, SW, wmv_current.TM);
        zzt_set_registration ();
        }
    else
        {
        /* returned_status = WM_ERR_TNR_NOT_FOUND ;                                 */
        }
    }

/* === WMTLOCATE ===                                                                */
#if USE_PROTOTYPES
publicdef INT wmtlocate (INT *statPtr, GKS_REAL *wxPtr, GKS_REAL *wyPtr, INT *butPtr)
#else
publicdef INT wmtlocate (statPtr, wxPtr, wyPtr, butPtr)
INT *statPtr, *butPtr;
GKS_REAL *wxPtr, *wyPtr;
#endif
    {
    PROGNAME ("wmtlocate");
    BOOL done;
    
    GKS_INT wkid = MAIN_WKID, locator_device = 1;
    GKS_INT gks_status, Tnr, Button;
    GKS_REAL locx, locy;
    GKS_INT RTNR;
    
    INT returned_status = SUCCESS;
    
    if (ug_if_gpx ())
        {
        return gpx_wmtlocate (statPtr, wxPtr, wyPtr, butPtr);
        }
    wiz_jp_name (progname);
    
    IF_NOT_WIN_OPEN_RETURN;
    
    RTNR = 0;
    wm_gsvpip (TEMP_TNR, RTNR, HIGHER);
    
    done = FALSE;
    do
        {
        
        if (wiv.jpmode == WI_PLAYBACK)
            {
            wiz_jp_read_long (&gks_status);
            wiz_jp_read_long (&Tnr);
            wiz_jp_read_long (&Button);
            wiz_jp_read_float (&locx);
            wiz_jp_read_float (&locy);
            }
        else
            {
            
            /* request x,y position and button number from GKS                      */
            wm_grqlc (wkid, locator_device, &gks_status, &Tnr, &locx, &locy);
            wm_gkzqbtn (&Button);
            
            if (wiv.jpmode == WI_JOURNAL)
                {
                wiz_jp_write_long (gks_status);
                wiz_jp_write_long (Tnr);
                wiz_jp_write_long (Button);
                wiz_jp_write_float (locx);
                wiz_jp_write_float (locy);
                }
            }
        /* if status = success and button not cancel                                */
        if (gks_status == (GKS_INT)GOK && Button != WM_BUTTON_CANCEL)
            {
            if (Tnr != TEMP_TNR)
                {
                tg_tnr2tnr (Tnr, locx, locy, TEMP_TNR, &locx, &locy);
                RTNR = Tnr;
                wm_gsvpip (TEMP_TNR, RTNR, HIGHER);
                }
            done = !(zzt_do_puckkey (locx, locy, Button));
            }
        else
            {
            done = TRUE;
            }
        } while (!(done));
    
    if (gks_status == (GKS_INT)GOK)
        *statPtr = SUCCESS;
    else
        *statPtr = FAIL;
    *wxPtr = locx;
    *wyPtr = locy;
    *butPtr = Button;
    
    RTNR = 0;
    wm_gsvpip (TEMP_TNR, RTNR, (GKS_INT)GLOWER);
    
    return returned_status;
    }

/* === WMTBANDRECTANGLE ===                                                         */

#if USE_PROTOTYPES
publicdef INT wmtbandrectangle (INT *statPtr, DOUBLE wx1, DOUBLE wy1, GKS_REAL *wx2Ptr,
                                GKS_REAL *wy2Ptr, INT *butPtr)
#else
publicdef INT wmtbandrectangle (statPtr, wx1, wy1, wx2Ptr, wy2Ptr, butPtr)
INT *statPtr;
DOUBLE wx1, wy1;
GKS_REAL *wx2Ptr, *wy2Ptr;
INT *butPtr;
#endif
    {
    
    INT returned_status = SUCCESS;
    
    GKS_REAL ugWx1, ugWy1, ugWx2, ugWy2;
    GKS_INT ugStatus, ugTnr, ugButton;
    GKS_INT RTNR;
    BOOL done;
    
    *statPtr = FAIL;
    *wx2Ptr = wx1;
    *wy2Ptr = wy1;
    *butPtr = 0;
    
    IF_NOT_WIN_OPEN_RETURN;
    if (ug_if_gpx ())
        {
        return gpx_wmtbandrectangle (statPtr, wx1, wy1, wx2Ptr, wy2Ptr, butPtr);
        }
    RTNR = 0;
    wm_gsvpip (TEMP_TNR, RTNR, HIGHER);
    
    done = FALSE;
    do
        {
        ugWx1 = wx1;
        ugWy1 = wy1;
        ugStatus = 0;
        ugTnr = 0;
        ugWx2 = wx1;
        ugWy2 = wy1;
        ugButton = 0;
        ugrqband_gc (UGRQ_TYPE_BAND_RECTANGLE, ugWx1, ugWy1, &ugStatus, &ugTnr, &ugWx2,
                     &ugWy2, &ugButton);
        if (ugStatus == SUCCESS)
            {
            if (ugTnr != TEMP_TNR)
                {
                tg_tnr2tnr (ugTnr, ugWx2, ugWy2, TEMP_TNR, &ugWx2, &ugWy2);
                RTNR = ugTnr;
                wm_gsvpip (TEMP_TNR, RTNR, HIGHER);
                }
            done = !(zzt_do_puckkey (ugWx2, ugWy2, ugButton));
            }
        else
            {
            done = TRUE;
            }
        } while (!(done));
    
    *statPtr = ugStatus;
    *wx2Ptr = ugWx2;
    *wy2Ptr = ugWy2;
    *butPtr = ugButton;
    
    RTNR = 0;
    wm_gsvpip (TEMP_TNR, RTNR, (GKS_INT)GLOWER);
    
    return returned_status;
    }

/* === WMTBANDVECTOR ===                                                            */

#if USE_PROTOTYPES
publicdef INT wmtbandvector (INT *statPtr, DOUBLE wx1, DOUBLE wy1, GKS_REAL *wx2Ptr,
                             GKS_REAL *wy2Ptr, INT *butPtr)
#else
publicdef INT wmtbandvector (statPtr, wx1, wy1, wx2Ptr, wy2Ptr, butPtr)
INT *statPtr;
DOUBLE wx1, wy1;
GKS_REAL *wx2Ptr, *wy2Ptr;
INT *butPtr;
#endif
    {
    
    INT returned_status = SUCCESS;
    GKS_REAL ugWx1, ugWy1, ugWx2, ugWy2;
    GKS_INT ugStatus, ugTnr, ugButton;
    GKS_INT RTNR;
    BOOL done;
    
    if (ug_if_gpx ())
        {
        return gpx_wmtbandvector (statPtr, wx1, wy1, wx2Ptr, wy2Ptr, butPtr);
        }
    *statPtr = FAIL;
    *wx2Ptr = wx1;
    *wy2Ptr = wy1;
    *butPtr = 0;
    
    IF_NOT_WIN_OPEN_RETURN;
    
    RTNR = 0;
    wm_gsvpip (TEMP_TNR, RTNR, HIGHER);
    
    done = FALSE;
    do
        {
        ugWx1 = wx1;
        ugWy1 = wy1;
        ugStatus = 0;
        ugTnr = 0;
        ugWx2 = wx1;
        ugWy2 = wy1;
        ugButton = 0;
        ugrqband_gc (UGRQ_TYPE_BAND_VECTOR, ugWx1, ugWy1, &ugStatus, &ugTnr, &ugWx2,
                     &ugWy2, &ugButton);
        if (ugStatus == SUCCESS)
            {
            if (ugTnr != TEMP_TNR)
                {
                tg_tnr2tnr (ugTnr, ugWx2, ugWy2, TEMP_TNR, &ugWx2, &ugWy2);
                RTNR = ugTnr;
                wm_gsvpip (TEMP_TNR, RTNR, HIGHER);
                }
            done = !(zzt_do_puckkey (ugWx2, ugWy2, ugButton));
            }
        else
            {
            done = TRUE;
            }
        } while (!(done));
    
    *statPtr = ugStatus;
    *wx2Ptr = ugWx2;
    *wy2Ptr = ugWy2;
    *butPtr = ugButton;
    
    RTNR = 0;
    wm_gsvpip (TEMP_TNR, RTNR, (GKS_INT)GLOWER);
    return returned_status;
    }


#define BTN_INPUT 1
#define XY_INPUT  2
static BOOL menu_active = FALSE;
/********************************************************************               */
/**      Copyright 1988, Exploration Systems Inc. San Francisco                     */
/**                                                                                 */
/** PROGRAM:    VaxStation/VAXGKS version of wmt_locate()                           */
/**                                                                                 */
/** DESCRIPTION:                                                                    */
/**   This program implements a window manager locator input handler.               */
/**                                                                                 */
/**   This version of wmt_locate uses the workstation mouse as an                   */
/**   input locator device and provides button press resolution via                 */
/**   a button menu on the graphics screen.  This is because the                    */
/**   WS mouse only provides 2 or 3 buttons and the current FINDER                  */
/**   implementation depends on the availability of 16.  The number                 */
/**   of buttons on the mouse is assumed to be 3 in this implementation.            */
/**                                                                                 */
/**                                                                                 */
/** ARGUMENTS:                                                                      */
/**   Out:                                                                          */
/**       INT  *istat     - Pointer to a long for returned status.                  */
/**       GKS_REAL    *wx,*wy   - Pointers to FLOAT for returned coords.            */
/**       INT  *button    - Pointer to a long for returned button.                  */
/**                                                                                 */
/** RETURNS:                                                                        */
/**   SUCCESS always.  For status of input check istat arg. on return.              */
/**                                                                                 */
/**   Julian Carlisle      Oct 15, 1988.                                            */
/*********************************************************************              */
#if USE_PROTOTYPES
publicdef INT gpx_wmtlocate (INT *istat, GKS_REAL *wx, GKS_REAL *wy, INT *button)
#else
publicdef INT gpx_wmtlocate (istat, wx, wy, button)
INT *istat;                         /* Out: Status from GKS Out: WCS Out: Button    */
GKS_REAL *wx, *wy;                  /* Code                                         */
INT *button;
#endif
    {
    
    GKS_INT gks_stat = 0;           /*     returned status from locate              */
    INT status = SUCCESS;
    EXTENTS wext, dext;
    
    GKS_REAL x, y;
    INT int_x, int_y;
    DOUBLE double_x, double_y;
    
    INT b, unity = 0, key;
    GKS_REAL wcs_window[4], wcs_vport[4], angle;    /*      saved transformation.   */
    APPLICATION_ID appCur;
    WINDOW_ID winCur;
    TNR_ID tnrCur, tnr, maintnr;
    GROUP_ID groCur;
    SEGMENT_ID segCur;
    PICK_ID pickCur;
    TNR_TREE_DATA tnr_data;
    BOOL done = FALSE;
    INT pet;
    
    status = wmoinqcur (&appCur, &winCur, &tnrCur, &groCur, &segCur, &pickCur);
    if (tnrCur)
        {
        status = btfind (wmv_current.tnr_tree_ptr, (VOIDPTR)&tnrCur,
                         (VOIDPTR)&tnr_data);
        if (status == 0)
            return WM_ERR_TNR_NOT_FOUND;
        }
    else
        {
        btfirst (wmv_current.tnr_tree_ptr, (VOIDPTR)&tnrCur, (VOIDPTR)&tnr_data);
        }
    if (tnr_data.reg.key_type == WM_PD_TABLET)
        {
        do
            {
            
            /* If there is a defined pseudo keypad, try to                          */
            /*interpret the key button and require the                              */
            /*digitized value.                                                      */
            
            status = tbh_request_locator (&int_x, &int_y, &b);
            if (status > 0)
                status = SUCCESS;
            
            if (tnr_data.reg.keypad_reg != (POINT *)NULL)
                {
                while (zzt_chk_pseudo_key (&tnr_data.reg, int_x, int_y, &key))
                    {
                    (wm_static ())->reg.default_button = key;
                    if (key == WM_BUTTON_CANCEL || key == WM_BUTTON_DONE)
                        break;
                    tbh_request_locator (&int_x, &int_y, &b);
                    }
                *button = (wm_static ())->reg.default_button;
                }
            else
                {
                /* else the reyurned button just same as the                        */
                /* user clicked one.                                                */
                *button = b;
                }
            if (key == WM_BUTTON_CANCEL || key == WM_BUTTON_DONE)
                (wm_static ())->reg.default_button = 1;
            if (tnr_data.reg.x_matrix == (MATRIX *)NULL)
                {
                tnr_data.reg.x_matrix = (MATRIX *) tc_zalloc (sizeof(MATRIX));
                if (tnr_data.reg.wcs == (POINT *)NULL)
                    {
                    
                    /* if there is no defined registration, we use'                 */
                    /*   the whole tablet to be the registered area,                */
                    /*and use the whole world coord. in TNR to be                   */
                    /*the registered area.                                          */
                    
                    wext.ll_x = tnr_data.wcs.min.x;
                    wext.ll_y = tnr_data.wcs.min.y;
                    wext.lr_x = tnr_data.wcs.max.x;
                    wext.lr_y = tnr_data.wcs.min.y;
                    wext.ur_x = tnr_data.wcs.max.x;
                    wext.ur_y = tnr_data.wcs.max.y;
                    wext.ul_x = tnr_data.wcs.min.x;
                    wext.ul_y = tnr_data.wcs.max.y;
                    tbh_inq_tablet (10, (BYTE *)&dext); /* QQQQQQ                   */
                    }
                else
                    {
                    wext.ul_x = tnr_data.reg.wcs[0].x;
                    wext.ul_y = tnr_data.reg.wcs[0].y;
                    wext.ll_x = tnr_data.reg.wcs[1].x;
                    wext.ll_y = tnr_data.reg.wcs[1].y;
                    wext.lr_x = tnr_data.reg.wcs[2].x;
                    wext.lr_y = tnr_data.reg.wcs[2].y;
                    wext.ur_x = tnr_data.reg.wcs[3].x;
                    wext.ur_y = tnr_data.reg.wcs[3].y;
                    
                    dext.ul_x = tnr_data.reg.tablet[0].x;
                    dext.ul_y = tnr_data.reg.tablet[0].y;
                    dext.ll_x = tnr_data.reg.tablet[1].x;
                    dext.ll_y = tnr_data.reg.tablet[1].y;
                    dext.lr_x = tnr_data.reg.tablet[2].x;
                    dext.lr_y = tnr_data.reg.tablet[2].y;
                    dext.ur_x = tnr_data.reg.tablet[3].x;
                    dext.ur_y = tnr_data.reg.tablet[3].y;
                    }
                tg_calc_reg_xform (&dext, &wext, &angle, tnr_data.reg.x_matrix);
                
                btrepcur (wmv_current.tnr_tree_ptr, (VOIDPTR)&tnrCur,
                          (VOIDPTR)&tnr_data);
                
                }
            tg_reg_to_wcs ((DOUBLE)int_x, (DOUBLE)int_y, &double_x, &double_y,
                           tnr_data.reg.x_matrix);
            
            if (*button != WM_BUTTON_CANCEL)
                {
                done = !(zzt_do_puckkey (double_x, double_y, *button));
                }
            else
                {
                done = TRUE;
                }
            } while (!done);
        
        *wx = double_x;
        *wy = double_y;
        }
    else
        {
        
        /* Inquire & save the current xform number &                                */
        /* it's components                                                          */
        gks_stat = wm_gqcntn (&status, &maintnr);
        
        gks_stat = wm_gqnt (maintnr, &status, wcs_window, wcs_vport);
        
        /* Make the viewport priority HIGHER than the                               */
        /* unity                                                                    */
        gks_stat = wm_gsvpip (maintnr, unity, HIGHER);
        *button = (wm_static ())->reg.default_button;
        pet = 1;                    /* prompt echo type                             */
        do
            {
            gks_stat = wmz_getxy (&tnr, &x, &y, button, pet);
            if (gks_stat)
                done = TRUE;
            
            /* If the input x,y xform is not the current xform, we must             */
            /* convert the x,y to WCS by applying the current tnr                   */
            if (tnr != maintnr)
                {
                tg_tnr2tnr (tnr, x, y, maintnr, &x, &y);
                gks_stat = wm_gsvpip (maintnr, tnr, HIGHER);
                }
            if (*button != WM_BUTTON_CANCEL)
                {
                done = !(zzt_do_puckkey (x, y, *button));
                }
            else
                {
                done = TRUE;
                }
            } while (!done);
        
        *wx = x;
        *wy = y;
        
        gks_stat = wm_gsvpip (maintnr, unity, (GKS_INT)GLOWER);
        
        if (status == 1)
            status = SUCCESS;
        }
    *istat = status;
    return SUCCESS;
    }

/* === WMTINQ ===                                                                   */

#if USE_PROTOTYPES
publicdef INT wmtinq (TNR_ID tnr_id, GKS_REAL *vx_ll, GKS_REAL *vx_ur, GKS_REAL *vy_ll,
                      GKS_REAL *vy_ur, GKS_REAL *wx_ll, GKS_REAL *wx_ur,
                      GKS_REAL *wy_ll, GKS_REAL *wy_ur)
#else
publicdef INT wmtinq (tnr_id, vx_ll, vx_ur, vy_ll, vy_ur, wx_ll, wx_ur, wy_ll, wy_ur)
TNR_ID tnr_id;
GKS_REAL *vx_ll, *vx_ur, *vy_ll, *vy_ur;
GKS_REAL *wx_ll, *wx_ur, *wy_ll, *wy_ur;
#endif
    {
    
    INT returned_status = SUCCESS;
    TNR_TREE_DATA tnr_data;
    
    *vx_ll = 0.0;
    *vx_ur = 0.0;
    *vy_ll = 0.0;
    *vy_ur = 0.0;
    *wx_ll = 0.0;
    *wx_ur = 0.0;
    *wy_ll = 0.0;
    *wy_ur = 0.0;
    
    IF_NOT_WIN_OPEN_RETURN;
    
    if (btfind (wmv_current.tnr_tree_ptr, (VOIDPTR)&tnr_id, (VOIDPTR)&tnr_data))
        {
        *vx_ll = tnr_data.nwc.min.x;
        *vx_ur = tnr_data.nwc.max.x;
        *vy_ll = tnr_data.nwc.min.y;
        *vy_ur = tnr_data.nwc.max.y;
        *wx_ll = tnr_data.wcs.min.x;
        *wx_ur = tnr_data.wcs.max.x;
        *wy_ll = tnr_data.wcs.min.y;
        *wy_ur = tnr_data.wcs.max.y;
        }
    else
        {
        returned_status = WM_ERR_TNR_NOT_FOUND;
        }
    return returned_status;
    }

/* === WMTINQPANZOOM ===                                                            */

#if USE_PROTOTYPES
publicdef INT wmtinqpanzoom (TNR_ID tnr_id, DOUBLE *xo, DOUBLE *yo, DOUBLE *fx,
                             DOUBLE *fy)
#else
publicdef INT wmtinqpanzoom (tnr_id, xo, yo, fx, fy)
TNR_ID tnr_id;
DOUBLE *xo, *yo, *fx, *fy;
#endif
    {
    
    INT returned_status = SUCCESS;
    
    TNR_TREE_DATA tnr_data;
    
    *xo = 0.0;
    *yo = 0.0;
    *fx = 0.0;
    *fy = 0.0;
    
    IF_NOT_WIN_OPEN_RETURN;
    
    if (btfind (wmv_current.tnr_tree_ptr, (VOIDPTR)&tnr_id, (VOIDPTR)&tnr_data))
        {
        *xo = tnr_data.panx;
        *yo = tnr_data.pany;
        *fx = tnr_data.zoomx;
        *fy = tnr_data.zoomy;
        }
    else
        {
        returned_status = WM_ERR_TNR_NOT_FOUND;
        }
    return returned_status;
    }

/* === WMTINQLIST ===                                                               */

#if USE_PROTOTYPES
publicdef INT wmtinqlist (INT  class , LISTHEAD *tnr_list)
#else
publicdef INT wmtinqlist (class , tnr_list)
INT  class ;
LISTHEAD *tnr_list;
#endif
    {
    
    INT returned_status = SUCCESS;
    TNR_TREE_KEY tnr_key;
    TNR_TREE_DATA tnr_data;
    INT count, i;
    ALL_TREE_DATA all_data;
    
    IF_NOT_WIN_OPEN_RETURN;
    
    if (llcount (tnr_list) > 0)
        {
        llfree (tnr_list);
        }
    count = btcount (wmv_current.tnr_tree_ptr);
    for (i = 1; count > 0 && i <= count; i++)
        {
        if (i == 1)
            {
            btfirst (wmv_current.tnr_tree_ptr, (VOIDPTR)&tnr_key, (VOIDPTR)&tnr_data);
            }
        else
            {
            btnext (wmv_current.tnr_tree_ptr, (VOIDPTR)&tnr_key, (VOIDPTR)&tnr_data);
            }
        btfind (&all_tree, (VOIDPTR)&tnr_key, (VOIDPTR)&all_data);
        if ((all_data.cla == class) || (class  == WM_CLASS_TNR))
            {
            llappnd (tnr_list, (VOIDPTR)&tnr_key);
            }
        }
    return returned_status;
    }

/* === WMTREGISTRATION ===                                                          */

#if USE_PROTOTYPES
publicdef INT wmtregistration (TNR_ID tnr_id, GKS_REAL *xpoints, GKS_REAL *ypoints,
                               DOUBLE x1, DOUBLE y1, DOUBLE x2, DOUBLE y2)
#else
publicdef INT wmtregistration (tnr_id, xpoints, ypoints, x1, y1, x2, y2)
TNR_ID tnr_id;
GKS_REAL *xpoints, *ypoints;
DOUBLE x1, y1, x2, y2;
#endif
    {
    
    INT returned_status = SUCCESS;
    INT i;
    INT npoints;
    TNR_TREE_DATA tnr_data;
    
    IF_NOT_WIN_OPEN_RETURN;
    CLOSE_SEG;
    
    if (btfind (wmv_current.tnr_tree_ptr, (VOIDPTR)&tnr_id, (VOIDPTR)&tnr_data))
        {
        if (tnr_data.reg.wcs != (POINT *)NULL)
            {
            tc_zeroes ((VOIDPTR)tnr_data.reg.wcs, sizeof(POINT) * 4);
            tc_zeroes ((VOIDPTR)tnr_data.reg.tablet, sizeof(POINT) * 4);
            }
        else
            {
            tnr_data.reg.wcs = (POINT *) tc_zalloc (sizeof(POINT) * 4);
            tnr_data.reg.tablet = (POINT *) tc_zalloc (sizeof(POINT) * 4);
            }
        tnr_data.reg.ifset = TRUE;
        
        if (!for_gpx)
            {
            npoints = 3;
            tnr_data.reg.wcs[0].x = x1;
            tnr_data.reg.wcs[0].y = y1;
            tnr_data.reg.wcs[2].x = x2;
            tnr_data.reg.wcs[2].y = y2;
            }
        else
            {
            npoints = 4;
            tnr_data.reg.wcs[0].x = x1;
            tnr_data.reg.wcs[0].y = y1;
            tnr_data.reg.wcs[1].x = x2;
            tnr_data.reg.wcs[1].y = y1;
            tnr_data.reg.wcs[2].x = x2;
            tnr_data.reg.wcs[2].y = y2;
            tnr_data.reg.wcs[3].x = x1;
            tnr_data.reg.wcs[3].y = y2;
            if (tnr_data.reg.x_matrix != (MATRIX *)NULL)
                {
                tc_free ((VOIDPTR)tnr_data.reg.x_matrix);
                tnr_data.reg.x_matrix = (MATRIX *)NULL;
                }
            }
        for (i = 0; i < npoints; i++)
            {
            tnr_data.reg.tablet[i].x = xpoints[i];
            tnr_data.reg.tablet[i].y = ypoints[i];
            }
        btrepcur (wmv_current.tnr_tree_ptr, (VOIDPTR)&tnr_id, (VOIDPTR)&tnr_data);
        if (tnr_id == wmv_current.tnr_id)
            {
            wmv_current.reg = tnr_data.reg;
            zzt_set_registration ();
            }
        }
    else
        {
        returned_status = WM_ERR_TNR_NOT_FOUND;
        }
    return returned_status;
    }

/* === GPX_WMTREGISTRATION ===                                                      */

/*----------------------------------------------------------------------------------*/
/*  For now, assume that wcs_points & tcs_points contain points_in = 3              */
/*  entries in the correct order (as expected by gpx_wmtlocate()):                  */
/*                                                                                  */
/*      [0] lower left  LL                                                          */
/*      [1] lower right LR                                                          */
/*      [2] upper right UR                                                          */
/*      [3] upper left  UL this position is assummed to be undefined                */
/*                                                                                  */
/*  Later we will ensure that points are arranged in the correct order              */
/*----------------------------------------------------------------------------------*/


#if USE_PROTOTYPES
publicdef INT gpx_wmtregistration (TNR_ID tnr_id, POINT wcs_points[4],
                                   POINT tcs_points[4], INT points_in)
#else
publicdef INT gpx_wmtregistration (tnr_id, wcs_points, tcs_points, points_in)
TNR_ID tnr_id;
POINT wcs_points[4];                /* array of wcs registration points             */
POINT tcs_points[4];                /* array of tablet coord (tcs) registration points  */
INT points_in;                      /* number of points actually supplied by caller */
#endif
    {
    
    INT returned_status = SUCCESS;
    INT i;
    TNR_TREE_DATA tnr_data;
    
    IF_NOT_WIN_OPEN_RETURN;
    CLOSE_SEG;
    
    if (btfind (wmv_current.tnr_tree_ptr, (VOIDPTR)&tnr_id, (VOIDPTR)&tnr_data))
        {
        /*--------------------------------------------------------------------------*/
        /* zero (or allocate) the tnr_data.reg arrays                               */
        /*--------------------------------------------------------------------------*/
        
        if (tnr_data.reg.wcs != (POINT *)NULL)
            {
            tc_zeroes ((VOIDPTR)tnr_data.reg.wcs, sizeof(POINT) * 4);
            tc_zeroes ((VOIDPTR)tnr_data.reg.tablet, sizeof(POINT) * 4);
            }
        else
            {
            tnr_data.reg.wcs = (POINT *) tc_zalloc (sizeof(POINT) * 4);
            tnr_data.reg.tablet = (POINT *) tc_zalloc (sizeof(POINT) * 4);
            }
        tnr_data.reg.ifset = TRUE;
        
        /*--------------------------------------------------------------------------*/
        /* plant the wcs & tcs registration points in tnr_data.reg                  */
        /*--------------------------------------------------------------------------*/
        
        for (i = 0; i < points_in; i++)
            {
            tnr_data.reg.wcs[i].x = wcs_points[i].x;
            tnr_data.reg.wcs[i].y = wcs_points[i].y;
            }
        for (i = 0; i < points_in; i++)
            {
            tnr_data.reg.tablet[i].x = tcs_points[i].x;
            tnr_data.reg.tablet[i].y = tcs_points[i].y;
            }
        /*--------------------------------------------------------------------------*/
        /* if a transform matrix is already present, free it                        */
        /*--------------------------------------------------------------------------*/
        
        if (tnr_data.reg.x_matrix != (MATRIX *)NULL)
            {
            tc_free ((VOIDPTR)tnr_data.reg.x_matrix);
            tnr_data.reg.x_matrix = (MATRIX *)NULL;
            }
        /*--------------------------------------------------------------------------*/
        /* automatically select the preferred input                                 */
        /* device to the tablet                                                     */
        /*--------------------------------------------------------------------------*/
        wm_set_pick_device (tnr_id, WM_PD_TABLET);
        tnr_data.reg.key_type = WM_PD_TABLET;
        tnr_data.reg.default_button = 1;
        btrepcur (wmv_current.tnr_tree_ptr, (VOIDPTR)&tnr_id, (VOIDPTR)&tnr_data);
        if (tnr_id == wmv_current.tnr_id)
            {
            wmv_current.reg = tnr_data.reg;
            zzt_set_registration ();
            }
        }
    else
        {
        returned_status = WM_ERR_TNR_NOT_FOUND;
        }
    return returned_status;
    }

/* === WMTREGONOFF ===                                                              */

#if USE_PROTOTYPES
publicdef INT wmtregonoff (BOOL on_off)
#else
publicdef INT wmtregonoff (on_off)
BOOL on_off;
#endif
    {
    
    INT returned_status = SUCCESS;
    
    IF_NOT_INIT_RETURN;
    CLOSE_SEG;
    if (on_off && (!ug_if_gpx ()))
        {
        wmtregmode (WM_REG_MODE_ABS);
        }
    else
        {
        wmtregmode (WM_REG_MODE_OFF);
        }
    return returned_status;
    }

/* === WMTREGMODE ===                                                               */

#if USE_PROTOTYPES
publicdef INT wmtregmode (INT mode)
#else
publicdef INT wmtregmode (mode)
INT mode;
#endif
    {
    INT returned_status;
    
    IF_NOT_INIT_RETURN;
    CLOSE_SEG;
    if (ug_if_gpx ())
        {
        reg_mode = WM_REG_MODE_OFF;
        }
    else
        {
        switch (mode)
            {
        case WM_REG_MODE_ABS:
            reg_mode = WM_REG_MODE_ABS;
            break;
        case WM_REG_MODE_REL:
            reg_mode = WM_REG_MODE_REL;
            break;
        case WM_REG_MODE_OFF:
            reg_mode = WM_REG_MODE_OFF;
            break;
        default:
            reg_mode = WM_REG_MODE_OFF;
            returned_status = WM_ERR_BAD_REG_MODE;
            }
        if (wmv_current.tnr_id != 0)
            {
            zzt_set_registration ();
            }
        }
    return returned_status;
    }

/* === WMTREGDEFAULT ===                                                            */

#if USE_PROTOTYPES
publicdef INT wmtregdefault (GKS_REAL xform[6])
#else
publicdef INT wmtregdefault (xform)
GKS_REAL xform[6];

#endif
    {
    
    INT returned_status = SUCCESS;
    INT i;
    
    IF_NOT_INIT_RETURN;
    
    for (i = 0; i < 6; i++)
        {
        DEFAULT_TABXF[i] = xform[i];
        }
    return returned_status;
    }

/* === ZZT_SET_REGISTRATION ===                                                     */

#if USE_PROTOTYPES
static VOID zzt_set_registration (void)
#else
static VOID zzt_set_registration ()
#endif
    {
    GKS_REAL xpoints[3], ypoints[3], x1, y1, x2, y2;
    GKS_REAL TABXF[6], angle;
    INT i;
    
    if (ug_if_gpx ())
        return;
    
    /* do not send registration while segment open                                  */
    if (wmv_current.seg_id)
        {
        return;
        }
    switch (reg_mode)
        {
    case WM_REG_MODE_ABS:
        if (wmv_current.tnr_id && wmv_current.reg.ifset)
            {
            tg_tnr2tnr (TEMP_TNR, wmv_current.reg.wcs[0].x, wmv_current.reg.wcs[0].y,
                        0, &x1, &y1);
            tg_tnr2tnr (TEMP_TNR, wmv_current.reg.wcs[2].x, wmv_current.reg.wcs[2].y,
                        0, &x2, &y2);
            for (i = 0; i < 3; i++)
                {
                xpoints[i] = wmv_current.reg.tablet[i].x;
                ypoints[i] = wmv_current.reg.tablet[i].y;
                }
            tg_calc_3p_xform (xpoints, ypoints, x1, y1, x2, y2, TABXF, &angle);
            
            ugstabxf_gc (MAIN_WKID, 0, TABXF);
            }
        else
            {
            ugstabxf_gc (MAIN_WKID, 0, DEFAULT_TABXF);
            }
        break;
    case WM_REG_MODE_REL:
        ugstabxf_gc (MAIN_WKID, 0, DEFAULT_TABXF);
        break;
    default:
        ugstabxf_gc (MAIN_WKID, 1, DEFAULT_TABXF);
        }
    }

/* === WMGALLOCATE ===                                                              */

#if USE_PROTOTYPES
publicdef INT wmgallocate (GROUP_ID *gro_id_ptr)
#else
publicdef INT wmgallocate (gro_id_ptr)
GROUP_ID *gro_id_ptr;
#endif
    {
    INT returned_status = SUCCESS;
    INT tree_type, key_size, data_size, control;
    GRO_TREE_DATA gro_data;
    TREEHEAD *gro_tree_ptr, *seg_tree_ptr;
    
    IF_NOT_WIN_OPEN_RETURN;
    CLOSE_SEG;
    
    gro_tree_ptr = (TREEHEAD *) zzz_malloc (sizeof(TREEHEAD));
    tree_type = WM_GROUP_TREE;
    key_size = sizeof(GRO_TREE_KEY);
    data_size = sizeof(GRO_TREE_DATA);
    control = BT_TREE_SYSTEM;
    btinit (gro_tree_ptr, tree_type, key_size, data_size, control);
    
    seg_tree_ptr = (TREEHEAD *) zzz_malloc (sizeof(TREEHEAD));
    tree_type = WM_SEGMENT_TREE;
    key_size = sizeof(SEG_TREE_KEY);
    data_size = sizeof(SEG_TREE_DATA);
    control = BT_TREE_SYSTEM;
    btinit (seg_tree_ptr, tree_type, key_size, data_size, control);
    
    *gro_id_ptr = next_gro_id ();
    gro_data.gro = gro_tree_ptr;
    gro_data.seg = seg_tree_ptr;
    gro_data.wkid = MAIN_WKID;      /* MGKS                                         */
    btadd (wmv_current.gro_tree_ptr, (VOIDPTR)gro_id_ptr, (VOIDPTR)&gro_data);
    
    wmgopen (*gro_id_ptr);
    return returned_status;
    }

/* === WMGOPEN ===                                                                  */

#if USE_PROTOTYPES
publicdef INT wmgopen (GROUP_ID gro_id)
#else
publicdef INT wmgopen (gro_id)
GROUP_ID gro_id;
#endif
    {
    
    INT returned_status = SUCCESS;
    GRO_TREE_DATA gro_data;
    
    IF_NOT_WIN_OPEN_RETURN;
    CLOSE_SEG;
    
    if (btfind (wmv_current.gro_tree_ptr, (VOIDPTR)&gro_id, (VOIDPTR)&gro_data))
        {
        zzg_push ();
        wmv_current.gro_id = gro_id;
        wmv_current.gro_tree_ptr = gro_data.gro;
        wmv_current.seg_tree_ptr = gro_data.seg;
        }
    else
        {
        returned_status = WM_ERR_GRO_NOT_FOUND;
        }
    return returned_status;
    }

/* === WMGCLOSE ===                                                                 */

#if USE_PROTOTYPES
publicdef INT wmgclose (GROUP_ID gro_id)
#else
publicdef INT wmgclose (gro_id)
GROUP_ID gro_id;
#endif
    {
    
    INT returned_status = SUCCESS;
    
    IF_NOT_GRO_OPEN_RETURN;
    CLOSE_SEG;
    
    if (gro_id == wmv_current.gro_id)
        {
        zzg_pop ();
        }
    else
        {
        returned_status = WM_ERR_WRONG_GRO;
        }
    return returned_status;
    }

/* === ZZG_PUSH ===                                                                 */

#if USE_PROTOTYPES
static VOID zzg_push (void)
#else
static VOID zzg_push ()
#endif
    {
    
    OPN_GRO_LIST_DATA opn_data;
    
    opn_data.id = wmv_current.gro_id;
    opn_data.gro = wmv_current.gro_tree_ptr;
    opn_data.seg = wmv_current.seg_tree_ptr;
    
    llappnd (&opn_gro_list, (VOIDPTR)&opn_data);
    
    wmv_current.gro_id = 0;
    wmv_current.gro_tree_ptr = (TREEHEAD *)NULL;
    wmv_current.seg_tree_ptr = (TREEHEAD *)NULL;
    }

/* === ZZG_POP ===                                                                  */

#if USE_PROTOTYPES
static VOID zzg_pop (void)
#else
static VOID zzg_pop ()
#endif
    {
    
    OPN_GRO_LIST_DATA opn_data;
    
    if (llcount (&opn_gro_list) > 0)
        {
        lllast (&opn_gro_list, (VOIDPTR)&opn_data);
        lldelb (&opn_gro_list);
        
        wmv_current.gro_id = opn_data.id;
        wmv_current.gro_tree_ptr = opn_data.gro;
        wmv_current.seg_tree_ptr = opn_data.seg;
        }
    else
        {
        wmv_current.gro_id = 0;
        wmv_current.gro_tree_ptr = (TREEHEAD *)NULL;
        wmv_current.seg_tree_ptr = (TREEHEAD *)NULL;
        }
    }

/* === WMGVIS ===                                                                   */

#if USE_PROTOTYPES
publicdef INT wmgvis (GROUP_ID gro_id, INT visibility)
#else
publicdef INT wmgvis (gro_id, visibility)
GROUP_ID gro_id;
INT visibility;
#endif
    {
    
    INT returned_status = SUCCESS;
    GRO_TREE_DATA gro_data;
    TREEHEAD *seg_tree_ptr, *gro_tree_ptr;
    
    IF_NOT_WIN_OPEN_RETURN;
    CLOSE_SEG;
    
    if (gro_id == wmv_current.gro_id)
        {
        zzg_set_vis (wmv_current.seg_tree_ptr, wmv_current.gro_tree_ptr, visibility);
        }
    else
        {
        if (btfind (wmv_current.gro_tree_ptr, (VOIDPTR)&gro_id, (VOIDPTR)&gro_data))
            {
            seg_tree_ptr = gro_data.seg;
            gro_tree_ptr = gro_data.gro;
            zzg_set_vis (seg_tree_ptr, gro_tree_ptr, visibility);
            }
        else
            {
            returned_status = WM_ERR_GRO_NOT_FOUND;
            }
        }
    return returned_status;
    }

/* === ZZG_SET_VIS ===                                                              */

#if USE_PROTOTYPES
static VOID zzg_set_vis (TREEHEAD *seg_tree_ptr, TREEHEAD *gro_tree_ptr,
                             INT visibility)
#else
static VOID zzg_set_vis (seg_tree_ptr, gro_tree_ptr, visibility)
TREEHEAD *seg_tree_ptr, *gro_tree_ptr;
INT visibility;
#endif
    {
    
    SEG_TREE_KEY seg_id;
    SEG_TREE_DATA seg_data;
    GRO_TREE_KEY gro_id;
    GRO_TREE_DATA gro_data;
    INT num_items, count;
    
    num_items = btcount (seg_tree_ptr);
    if (num_items > 0)
        {
        btfirst (seg_tree_ptr, (VOIDPTR)&seg_id, (VOIDPTR)&seg_data);
        wm_gsvis (seg_id, visibility);
        /*                                                                          */
        /*  The visibility state for each segment is now being retained.            */
        /*                                                                          */
        seg_data.visibility = visibility;
        btrepcur (seg_tree_ptr, (VOIDPTR)&seg_id, (VOIDPTR)&seg_data);
        
        for (count = 1; count < num_items; count++)
            {
            btnext (seg_tree_ptr, (VOIDPTR)&seg_id, (VOIDPTR)&seg_data);
            wm_gsvis (seg_id, visibility);
            seg_data.visibility = visibility;
            btrepcur (seg_tree_ptr, (VOIDPTR)&seg_id, (VOIDPTR)&seg_data);
            }
        }
    num_items = btcount (gro_tree_ptr);
    if (num_items > 0)
        {
        btfirst (gro_tree_ptr, (VOIDPTR)&gro_id, (VOIDPTR)&gro_data);
        zzg_set_vis (gro_data.seg, gro_data.gro, visibility);
        for (count = 1; count < num_items; count++)
            {
            btnext (gro_tree_ptr, (VOIDPTR)&gro_id, (VOIDPTR)&gro_data);
            zzg_set_vis (gro_data.seg, gro_data.gro, visibility);
            }
        }
    }

/* === WMGDTEC ===                                                                  */

#if USE_PROTOTYPES
publicdef INT wmgdtec (GROUP_ID gro_id, INT detectability)
#else
publicdef INT wmgdtec (gro_id, detectability)
GROUP_ID gro_id;
INT detectability;
#endif
    {
    
    INT returned_status = SUCCESS;
    GRO_TREE_DATA gro_data;
    TREEHEAD *seg_tree_ptr, *gro_tree_ptr;
    
    IF_NOT_WIN_OPEN_RETURN;
    CLOSE_SEG;
    
    if (gro_id == wmv_current.gro_id)
        {
        zzg_set_dtec (wmv_current.seg_tree_ptr, wmv_current.gro_tree_ptr,
                      detectability);
        }
    else
        {
        if (btfind (wmv_current.gro_tree_ptr, (VOIDPTR)&gro_id, (VOIDPTR)&gro_data))
            {
            seg_tree_ptr = gro_data.seg;
            gro_tree_ptr = gro_data.gro;
            zzg_set_dtec (seg_tree_ptr, gro_tree_ptr, detectability);
            }
        else
            {
            returned_status = WM_ERR_GRO_NOT_FOUND;
            }
        }
    return returned_status;
    }

/* === ZZG_SET_DTEC ===                                                             */

#if USE_PROTOTYPES
static VOID zzg_set_dtec (TREEHEAD *seg_tree_ptr, TREEHEAD *gro_tree_ptr,
                              INT detectability)
#else
static VOID zzg_set_dtec (seg_tree_ptr, gro_tree_ptr, detectability)
TREEHEAD *seg_tree_ptr, *gro_tree_ptr;
INT detectability;
#endif
    {
    
    SEG_TREE_KEY seg_id;
    SEG_TREE_DATA seg_data;
    GRO_TREE_KEY gro_id;
    GRO_TREE_DATA gro_data;
    INT num_items, count;
    
    num_items = btcount (seg_tree_ptr);
    if (num_items > 0)
        {
        btfirst (seg_tree_ptr, (VOIDPTR)&seg_id, (VOIDPTR)&seg_data);
        wm_gsdtec (seg_id, detectability);
        
        for (count = 1; count < num_items; count++)
            {
            btnext (seg_tree_ptr, (VOIDPTR)&seg_id, (VOIDPTR)&seg_data);
            wm_gsdtec (seg_id, detectability);
            }
        }
    num_items = btcount (gro_tree_ptr);
    if (num_items > 0)
        {
        btfirst (gro_tree_ptr, (VOIDPTR)&gro_id, (VOIDPTR)&gro_data);
        zzg_set_dtec (gro_data.seg, gro_data.gro, detectability);
        for (count = 1; count < num_items; count++)
            {
            btnext (gro_tree_ptr, (VOIDPTR)&gro_id, (VOIDPTR)&gro_data);
            zzg_set_dtec (gro_data.seg, gro_data.gro, detectability);
            }
        }
    }

/* === WMGHLIT ===                                                                  */

#if USE_PROTOTYPES
publicdef INT wmghlit (GROUP_ID gro_id, INT highlighting)
#else
publicdef INT wmghlit (gro_id, highlighting)
GROUP_ID gro_id;
INT highlighting;
#endif
    {
    
    INT returned_status = SUCCESS;
    GRO_TREE_DATA gro_data;
    
    IF_NOT_WIN_OPEN_RETURN;
    CLOSE_SEG;
    if (gro_id == wmv_current.gro_id)
        {
        zzg_set_hlit (wmv_current.seg_tree_ptr, wmv_current.gro_tree_ptr,
                      highlighting);
        }
    else
        {
        if (btfind (wmv_current.gro_tree_ptr, (VOIDPTR)&gro_id, (VOIDPTR)&gro_data))
            {
            zzg_set_hlit (gro_data.seg, gro_data.gro, highlighting);
            }
        else
            {
            returned_status = WM_ERR_GRO_NOT_FOUND;
            }
        }
    return returned_status;
    }

/* === ZZG_SET_HLIT ===                                                             */

#if USE_PROTOTYPES
static VOID zzg_set_hlit (TREEHEAD *seg_tree_ptr, TREEHEAD *gro_tree_ptr,
                              INT highlighting)
#else
static VOID zzg_set_hlit (seg_tree_ptr, gro_tree_ptr, highlighting)
TREEHEAD *seg_tree_ptr, *gro_tree_ptr;
INT highlighting;
#endif
    {
    
    SEG_TREE_KEY seg_id;
    SEG_TREE_DATA seg_data;
    GRO_TREE_KEY gro_id;
    GRO_TREE_DATA gro_data;
    INT num_items, count;
    
    num_items = btcount (seg_tree_ptr);
    if (num_items > 0)
        {
        btfirst (seg_tree_ptr, (VOIDPTR)&seg_id, (VOIDPTR)&seg_data);
        wm_gshlit (seg_id, highlighting);
        
        for (count = 1; count < num_items; count++)
            {
            btnext (seg_tree_ptr, (VOIDPTR)&seg_id, (VOIDPTR)&seg_data);
            wm_gshlit (seg_id, highlighting);
            }
        }
    num_items = btcount (gro_tree_ptr);
    if (num_items > 0)
        {
        btfirst (gro_tree_ptr, (VOIDPTR)&gro_id, (VOIDPTR)&gro_data);
        zzg_set_hlit (gro_data.seg, gro_data.gro, highlighting);
        for (count = 1; count < num_items; count++)
            {
            btnext (gro_tree_ptr, (VOIDPTR)&gro_id, (VOIDPTR)&gro_data);
            zzg_set_hlit (gro_data.seg, gro_data.gro, highlighting);
            }
        }
    }

/* === WMGDELETE ===                                                                */

#if USE_PROTOTYPES
publicdef INT wmgdelete (GROUP_ID gro_id)
#else
publicdef INT wmgdelete (gro_id)
GROUP_ID gro_id;
#endif
    {
    
    INT returned_status = SUCCESS;
    GRO_TREE_DATA gro_data;
    TREEHEAD *seg_tree_ptr, *gro_tree_ptr;
    
    IF_NOT_WIN_OPEN_RETURN;
    CLOSE_SEG;
    
    if (btfind (wmv_current.gro_tree_ptr, (VOIDPTR)&gro_id, (VOIDPTR)&gro_data))
        {
        seg_tree_ptr = gro_data.seg;
        gro_tree_ptr = gro_data.gro;
        zzg_delete (seg_tree_ptr, gro_tree_ptr);
        btdel (wmv_current.gro_tree_ptr, (VOIDPTR)&gro_id);
        btdel (&all_tree, (VOIDPTR)&gro_id);
        }
    else
        {
        returned_status = WM_ERR_GRO_NOT_FOUND;
        }
    return returned_status;
    }

/* === ZZG_CLEAR ===                                                                */

#if USE_PROTOTYPES
static VOID zzg_clear (TREEHEAD *seg_tree_ptr, TREEHEAD *gro_tree_ptr)
#else
static VOID zzg_clear (seg_tree_ptr, gro_tree_ptr)
TREEHEAD *seg_tree_ptr, *gro_tree_ptr;
#endif
    {
    
    SEG_TREE_KEY seg_id;
    SEG_TREE_DATA seg_data;
    GRO_TREE_KEY gro_id;
    GRO_TREE_DATA gro_data;
    INT num_items, count;
    
    num_items = btcount (seg_tree_ptr);
    if (num_items > 0)
        {
        for (count = 0; count < num_items; count++)
            {
            if (count == 0)
                {
                btfirst (seg_tree_ptr, (VOIDPTR)&seg_id, (VOIDPTR)&seg_data);
                }
            else
                {
                btnext (seg_tree_ptr, (VOIDPTR)&seg_id, (VOIDPTR)&seg_data);
                }
            wm_gdsg (seg_id);
            }
        }
    num_items = btcount (gro_tree_ptr);
    if (num_items > 0)
        {
        for (count = 0; count < num_items; count++)
            {
            if (count == 0)
                {
                btfirst (gro_tree_ptr, (VOIDPTR)&gro_id, (VOIDPTR)&gro_data);
                }
            else
                {
                btnext (gro_tree_ptr, (VOIDPTR)&gro_id, (VOIDPTR)&gro_data);
                }
            zzg_clear (gro_data.seg, gro_data.gro);
            }
        }
    }

/* === ZZG_DELETE ===                                                               */

#if USE_PROTOTYPES
static VOID zzg_delete (TREEHEAD *seg_tree_ptr, TREEHEAD *gro_tree_ptr)
#else
static VOID zzg_delete (seg_tree_ptr, gro_tree_ptr)
TREEHEAD *seg_tree_ptr, *gro_tree_ptr;
#endif
    {
    
    SEG_TREE_KEY seg_id;
    SEG_TREE_DATA seg_data;
    GRO_TREE_KEY gro_id;
    GRO_TREE_DATA gro_data;
    INT num_items, count;
    
    num_items = btcount (seg_tree_ptr);
    if (num_items > 0)
        {
        for (count = 0; count < num_items; count++)
            {
            if (count == 0)
                {
                btfirst (seg_tree_ptr, (VOIDPTR)&seg_id, (VOIDPTR)&seg_data);
                }
            else
                {
                btnext (seg_tree_ptr, (VOIDPTR)&seg_id, (VOIDPTR)&seg_data);
                }
            wm_gdsg (seg_id);
            btdel (&all_tree, (VOIDPTR)&seg_id);
            }
        }
    btfree (seg_tree_ptr);
    zzz_free ((VOIDPTR)seg_tree_ptr);
    
    num_items = btcount (gro_tree_ptr);
    if (num_items > 0)
        {
        for (count = 0; count < num_items; count++)
            {
            if (count == 0)
                {
                btfirst (gro_tree_ptr, (VOIDPTR)&gro_id, (VOIDPTR)&gro_data);
                }
            else
                {
                btnext (gro_tree_ptr, (VOIDPTR)&gro_id, (VOIDPTR)&gro_data);
                }
            zzg_delete (gro_data.seg, gro_data.gro);
            btdel (&all_tree, (VOIDPTR)&gro_id);
            }
        }
    btfree (gro_tree_ptr);
    zzz_free ((VOIDPTR)gro_tree_ptr);
    }

/* === WMGINQLIST ===                                                               */

#if USE_PROTOTYPES
publicdef INT wmginqlist (INT  class , LISTHEAD *gro_list)
#else
publicdef INT wmginqlist (class , gro_list)
INT  class ;
LISTHEAD *gro_list;
#endif
    {
    
    INT returned_status = SUCCESS;
    GRO_TREE_KEY gro_key;
    GRO_TREE_DATA gro_data;
    INT count, i;
    ALL_TREE_DATA all_data;
    
    IF_NOT_WIN_OPEN_RETURN;
    
    if (llcount (gro_list) > 0)
        {
        llfree (gro_list);
        }
    count = btcount (wmv_current.gro_tree_ptr);
    for (i = 1; count > 0 && i <= count; i++)
        {
        if (i == 1)
            {
            btfirst (wmv_current.gro_tree_ptr, (VOIDPTR)&gro_key, (VOIDPTR)&gro_data);
            }
        else
            {
            btnext (wmv_current.gro_tree_ptr, (VOIDPTR)&gro_key, (VOIDPTR)&gro_data);
            }
        btfind (&all_tree, (VOIDPTR)&gro_key, (VOIDPTR)&all_data);
        if ((all_data.cla == class) || (class  == WM_CLASS_GROUP))
            {
            llappnd (gro_list, (VOIDPTR)&gro_key);
            }
        }
    return returned_status;
    }

/* === WMSALLOCATE ===                                                              */

#if USE_PROTOTYPES
publicdef INT wmsallocate (SEGMENT_ID *seg_id_ptr)
#else
publicdef INT wmsallocate (seg_id_ptr)
SEGMENT_ID *seg_id_ptr;
#endif
    {
    INT returned_status = SUCCESS;
    GKS_INT SGNA;
    SEG_TREE_DATA seg_data;
    GKS_REAL WX1, WY1, WX2, WY2;
    RECTANGLE nwcRect, wcsRect;
    PIPE nwc2wcs;
    GKS_REAL PRIOR;
    
    IF_NOT_WIN_OPEN_RETURN;
    CLOSE_SEG;
    
    if (wmv_current.nsegs == WM_MAX_SEG)
        {
        returned_status = WM_ERR_MAX_SEG;
        return returned_status;
        }
    *seg_id_ptr = next_seg_id ();
    wmv_current.nsegs++;
    
    /* All segments, as default, are made up of primitives, etc. which are          */
    /*   known to GKS.  Some special segments are created by WM, such as the        */
    /*   screen binding segment, which contain primitives not known to GKS, and     */
    /*   require special handling -- e.g., when performing a window resize, which   */
    /*   invokes wm_ginsg ==> which will cause GKS to loose the special primitives  */
    /*                                                                              */
    seg_data.known_to_gks = TRUE;
    
    seg_data.num = wmv_current.nsegs + WM_SEG_OFFSET;   /* To allow segments of lower prior */
    seg_data.tnr = wmv_current.tnr_id;
    seg_data.visibility = (INT)GVISI;
    seg_data.wkid = MAIN_WKID;      /* MGKS                                         */
    btadd (wmv_current.seg_tree_ptr, (VOIDPTR)seg_id_ptr, (VOIDPTR)&seg_data);
    
    wmv_current.seg_id = *seg_id_ptr;
    
    /*                                                                              */
    /*   WX1 = wmv_current.wcs.min.x ;                                              */
    /*   WX2 = wmv_current.wcs.max.x ;                                              */
    /*   WY1 = wmv_current.wcs.min.y ;                                              */
    /*   WY2 = wmv_current.wcs.max.y ;                                              */
    /*                                                                              */
    PR_SetPipe (&nwc2wcs, &wmv_current.nwc, &wmv_current.wcs);
    /* entire window                                                                */
    nwcRect.min.x = 0.0;
    nwcRect.max.x = 1.0;
    nwcRect.min.y = 0.0;
    nwcRect.max.y = 1.0;
    PR_PipeRect (&nwcRect, &nwc2wcs, &wcsRect);
    WX1 = wcsRect.min.x;
    WX2 = wcsRect.max.x;
    WY1 = wcsRect.min.y;
    WY2 = wcsRect.max.y;
    wm_gswn (TEMP_TNR, WX1, WX2, WY1, WY2);
    wm_gselnt (TEMP_TNR);
    
    SGNA = *seg_id_ptr;
    /* Build an empty segment only known to Raster, so that                         */
    /*   SEGREF's to the real segment do not cause undefined segment errors         */
    /*   when the transformation segment is referenced.  This empty segment is a    */
    /*   place holder for the transformation segment which will be built when       */
    /*   the segment is closed                                                      */
    /*                                                                              */
    if (ug_if_gpx () == FALSE)
        GKZ_DUMMY_SEGMENT (&SGNA, wmv_current.TM);
    /* Create the real segment                                                      */
    /*                                                                              */
    wm_gcrsg (SGNA);
    wm_gssgt (SGNA, wmv_current.TM);    /* What does this do, given seg is open ??  */
    wm_gspkid (TEMP_PIC);
    PRIOR = GKS_PRIOR (wmv_current.win_num, (wmv_current.nsegs + WM_SEG_OFFSET));
    wm_gssgp (SGNA, (DOUBLE)PRIOR);
    return returned_status;
    }

/* === WMSCLOSE ===                                                                 */

#if USE_PROTOTYPES
publicdef INT wmsclose (SEGMENT_ID seg_id)
#else
publicdef INT wmsclose (seg_id)
SEGMENT_ID seg_id;
#endif
    {
    INT returned_status = SUCCESS;
    GKS_REAL cx, cy;
    GKS_REAL WX1, WY1, WX2, WY2;
    RECTANGLE nwcRect, wcsRect;
    PIPE nwc2wcs;
    
    IF_NOT_SEG_OPEN_RETURN;
    CLOSE_PIC;
    
    if (wmv_current.seg_id == seg_id)
        {
        wm_gclsg ();
        wmv_current.seg_id = 0;
        }
    else
        {
        wm_gclsg ();
        returned_status = WM_ERR_WRONG_SEG;
        }
    cx = wmv_current.wcs.min.x + ((wmv_current.wcs.max.x - wmv_current.wcs.min.x) / 2);
    cy = wmv_current.wcs.min.y + ((wmv_current.wcs.max.y - wmv_current.wcs.min.y) / 2);
    wcsRect.min.x = wmv_current.panx + ((wmv_current.wcs.min.x - cx) /
                                        wmv_current.zoomx);
    wcsRect.max.x = wmv_current.panx + ((wmv_current.wcs.max.x - cx) /
                                        wmv_current.zoomx);
    wcsRect.min.y = wmv_current.pany + ((wmv_current.wcs.min.y - cy) /
                                        wmv_current.zoomy);
    wcsRect.max.y = wmv_current.pany + ((wmv_current.wcs.max.y - cy) /
                                        wmv_current.zoomy);
    PR_SetPipe (&nwc2wcs, &wmv_current.nwc, &wcsRect);
    /* entire window                                                                */
    nwcRect.min.x = 0.0;
    nwcRect.max.x = 1.0;
    nwcRect.min.y = 0.0;
    nwcRect.max.y = 1.0;
    PR_PipeRect (&nwcRect, &nwc2wcs, &wcsRect);
    WX1 = wcsRect.min.x;
    WX2 = wcsRect.max.x;
    WY1 = wcsRect.min.y;
    WY2 = wcsRect.max.y;
    wm_gswn (TEMP_TNR, WX1, WX2, WY1, WY2);
    wm_gselnt (TEMP_TNR);
    /* if a tnr was selected while this segment was open,                           */
    /*      registration has to be updated                                          */
    if (ug_if_gpx () == FALSE)
        zzt_set_registration ();
    
    return returned_status;
    }

/* === WMSVIS ===                                                                   */

#if USE_PROTOTYPES
publicdef INT wmsvis (SEGMENT_ID seg_id, INT visibility)
#else
publicdef INT wmsvis (seg_id, visibility)
SEGMENT_ID seg_id;
INT visibility;
#endif
    {
    INT returned_status = SUCCESS;
    SEG_TREE_DATA seg_data;
    
    IF_NOT_WIN_OPEN_RETURN;
    CLOSE_SEG;
    
    if (btfind (wmv_current.seg_tree_ptr, (VOIDPTR)&seg_id, (VOIDPTR)&seg_data))
        {
        wm_gsvis ((GKS_INT)seg_id, (GKS_INT)visibility);
        }
    else
        {
        returned_status = WM_ERR_SEG_NOT_FOUND;
        }
    return returned_status;
    }

/* === WMSDTEC ===                                                                  */

#if USE_PROTOTYPES
publicdef INT wmsdtec (SEGMENT_ID seg_id, INT detectability)
#else
publicdef INT wmsdtec (seg_id, detectability)
SEGMENT_ID seg_id;
INT detectability;
#endif
    {
    
    INT returned_status = SUCCESS;
    SEG_TREE_DATA seg_data;
    
    IF_NOT_WIN_OPEN_RETURN;
    CLOSE_SEG;
    
    if (btfind (wmv_current.seg_tree_ptr, (VOIDPTR)&seg_id, (VOIDPTR)&seg_data))
        {
        wm_gsdtec (seg_id, detectability);
        }
    else
        {
        returned_status = WM_ERR_SEG_NOT_FOUND;
        }
    return returned_status;
    }

/* === WMSHLIT ===                                                                  */

#if USE_PROTOTYPES
publicdef INT wmshlit (SEGMENT_ID seg_id, INT highlighting)
#else
publicdef INT wmshlit (seg_id, highlighting)
SEGMENT_ID seg_id;
INT highlighting;
#endif
    {
    
    INT returned_status = SUCCESS;
    SEG_TREE_DATA seg_data;
    
    IF_NOT_WIN_OPEN_RETURN;
    CLOSE_SEG;
    
    if (btfind (wmv_current.seg_tree_ptr, (VOIDPTR)&seg_id, (VOIDPTR)&seg_data))
        {
        wm_gshlit (seg_id, highlighting);
        }
    else
        {
        returned_status = WM_ERR_SEG_NOT_FOUND;
        }
    return returned_status;
    }

/* === WMSRELPRIOR ===                                                              */

#if USE_PROTOTYPES
publicdef INT wmsrelprior (SEGMENT_ID seg1, SEGMENT_ID seg2, INT relprior)
#else
publicdef INT wmsrelprior (seg1, seg2, relprior)
SEGMENT_ID seg1, seg2;
INT relprior;
#endif
    {
    
    INT returned_status = SUCCESS;
    GKS_INT SGNA;
    GKS_REAL PRIOR;
    SEG_TREE_DATA seg1_data, seg2_data;
    WIN_TREE_KEY win_id;
    WIN_TREE_DATA win_data;
    
    IF_NOT_WIN_OPEN_RETURN;
    CLOSE_SEG;
    
    win_id = wmv_current.win_id;
    btfind (wmv_current.win_tree_ptr, (VOIDPTR)&win_id, (VOIDPTR)&win_data);
    
    /* do find on seg1 last because it will be updated with btrepcur                */
    if (zzs_find (seg2, win_data.seg, win_data.gro, &seg2_data))
        {
        if (btfind (wmv_current.seg_tree_ptr, (VOIDPTR)&seg1, (VOIDPTR)&seg1_data))
            {
            if (relprior == (GKS_INT)GLOWER)
                {
                seg1_data.num = seg2_data.num - 1;
                }
            else
                {
                seg1_data.num = seg2_data.num + 1;
                }
            btrepcur (wmv_current.seg_tree_ptr, (VOIDPTR)&seg1, (VOIDPTR)&seg1_data);
            
            SGNA = seg1;
            PRIOR = GKS_PRIOR (wmv_current.win_num, seg1_data.num);
            wm_gssgp (SGNA, (DOUBLE)PRIOR);
            }
        else
            {
            returned_status = WM_ERR_SEG_NOT_FOUND;
            }
        }
    else
        {
        returned_status = WM_ERR_SEG_NOT_FOUND;
        }
    return returned_status;
    }

/* === WMSDELETE ===                                                                */

#if USE_PROTOTYPES
publicdef INT wmsdelete (SEGMENT_ID seg_id)
#else
publicdef INT wmsdelete (seg_id)
SEGMENT_ID seg_id;
#endif
    {
    
    INT returned_status = SUCCESS;
    SEG_TREE_DATA seg_data;
    
    IF_NOT_WIN_OPEN_RETURN;
    CLOSE_SEG;
    
    if (btfind (wmv_current.seg_tree_ptr, (VOIDPTR)&seg_id, (VOIDPTR)&seg_data))
        {
        btdel (wmv_current.seg_tree_ptr, (VOIDPTR)&seg_id);
        btdel (&all_tree, (VOIDPTR)&seg_id);
        wm_gdsg ((GKS_INT)seg_id);
        }
    else
        {
        returned_status = WM_ERR_SEG_NOT_FOUND;
        }
    return returned_status;
    }

/* === WMSINQLIST ===                                                               */

#if USE_PROTOTYPES
publicdef INT wmsinqlist (INT  class , LISTHEAD *seg_list)
#else
publicdef INT wmsinqlist (class , seg_list)
INT  class ;
LISTHEAD *seg_list;
#endif
    {
    
    INT returned_status = SUCCESS;
    SEG_TREE_KEY seg_key;
    SEG_TREE_DATA seg_data;
    INT count, i;
    ALL_TREE_DATA all_data;
    
    IF_NOT_WIN_OPEN_RETURN;
    
    if (llcount (seg_list) > 0)
        {
        llfree (seg_list);
        }
    count = btcount (wmv_current.seg_tree_ptr);
    for (i = 1; count > 0 && i <= count; i++)
        {
        if (i == 1)
            {
            btfirst (wmv_current.seg_tree_ptr, (VOIDPTR)&seg_key, (VOIDPTR)&seg_data);
            }
        else
            {
            btnext (wmv_current.seg_tree_ptr, (VOIDPTR)&seg_key, (VOIDPTR)&seg_data);
            }
        btfind (&all_tree, (VOIDPTR)&seg_key, (VOIDPTR)&all_data);
        if ((all_data.cla == class) || (class  == WM_CLASS_SEGMENT))
            {
            llappnd (seg_list, (VOIDPTR)&seg_key);
            }
        }
    return returned_status;
    }

/* === ZZS_FIND ===                                                                 */

#if USE_PROTOTYPES
static INT zzs_find (SEGMENT_ID seg_id, TREEHEAD *seg_tree_ptr,
                         TREEHEAD *gro_tree_ptr, SEG_TREE_DATA *seg_data_ptr)
#else
static INT zzs_find (seg_id, seg_tree_ptr, gro_tree_ptr, seg_data_ptr)
SEGMENT_ID seg_id;
TREEHEAD *seg_tree_ptr, *gro_tree_ptr;
SEG_TREE_DATA *seg_data_ptr;
#endif
    {
    
    GRO_TREE_KEY gro_id;
    GRO_TREE_DATA gro_data;
    INT num_items, count;
    BOOL if_found;
    
    if_found = FALSE;
    if (!(btfind (seg_tree_ptr, (VOIDPTR)&seg_id, (VOIDPTR)seg_data_ptr)))
        {
        num_items = btcount (gro_tree_ptr);
        if (num_items > 0)
            {
            for (count = 0; count < num_items; count++)
                {
                if (count == 0)
                    {
                    btfirst (gro_tree_ptr, (VOIDPTR)&gro_id, (VOIDPTR)&gro_data);
                    }
                else
                    {
                    btnext (gro_tree_ptr, (VOIDPTR)&gro_id, (VOIDPTR)&gro_data);
                    }
                if_found = zzs_find (seg_id, gro_data.seg, gro_data.gro, seg_data_ptr);
                if (if_found)
                    break;
                }
            }
        }
    else
        {
        if_found = TRUE;
        }
    return(INT)if_found;
    }

/* === WMPALLOCATE ===                                                              */
#if USE_PROTOTYPES
publicdef INT wmpallocate (PICK_ID *pic_id_ptr)
#else
publicdef INT wmpallocate (pic_id_ptr)
PICK_ID *pic_id_ptr;
#endif
    {
    
    INT returned_status = SUCCESS;
    if (ug_if_gpx () == FALSE)
        return returned_status;
    
    IF_NOT_SEG_OPEN_RETURN;
    CLOSE_PIC;
    
    wmv_current.pic_id = next_pic_id ();
    *pic_id_ptr = wmv_current.pic_id;
    wm_gspkid (wmv_current.pic_id);
    return returned_status;
    }

/* === WMPOPEN ===                                                                  */

#if USE_PROTOTYPES
publicdef INT wmpopen (PICK_ID pic_id)
#else
publicdef INT wmpopen (pic_id)
PICK_ID pic_id;
#endif
    {
    
    INT returned_status = SUCCESS;
    ALL_TREE_DATA all_data;
    if (ug_if_gpx () == FALSE)
        return returned_status;
    
    IF_NOT_SEG_OPEN_RETURN;
    CLOSE_PIC;
    
    if (btfind (&all_tree, (VOIDPTR)&pic_id, (VOIDPTR)&all_data))
        {
        if (all_data.typ == WM_CLASS_PICK)
            {
            wmv_current.pic_id = pic_id;
            wm_gspkid (pic_id);
            }
        else
            {
            returned_status = WM_ERR_PIC_NOT_FOUND;
            }
        }
    else
        {
        returned_status = WM_ERR_PIC_NOT_FOUND;
        }
    return returned_status;
    }
/* === WMPCLOSE ===                                                                 */
#if USE_PROTOTYPES
publicdef INT wmpclose (PICK_ID pic_id)
#else
publicdef INT wmpclose (pic_id)
PICK_ID pic_id;
#endif
    {
    
    INT returned_status = SUCCESS;
    if (ug_if_gpx () == FALSE)
        return returned_status;
    
    IF_NOT_PIC_OPEN_RETURN;
    
    if (pic_id == wmv_current.pic_id)
        {
        wmv_current.pic_id = 0;
        wm_gspkid (TEMP_PIC);
        }
    else
        {
        returned_status = WM_ERR_WRONG_PIC;
        }
    return returned_status;
    }
/* === WMPREQUEST ===                                                               */
#if USE_PROTOTYPES
publicdef INT wmprequest (INT *statPtr, SEGMENT_ID *segPtr, PICK_ID *pickPtr,
                          INT *butPtr)
#else
publicdef INT wmprequest (statPtr, segPtr, pickPtr, butPtr)
INT *statPtr;
SEGMENT_ID *segPtr;
PICK_ID *pickPtr;
INT *butPtr;
#endif
    {
    PROGNAME ("wmprequest");
    
    INT returned_status = SUCCESS;
    BOOL done;
    
    GKS_INT wkid = MAIN_WKID, pk_dev_num = 1;
    GKS_INT locator_device = 1;
    GKS_INT pickStatus, locStatus, Tnr, Seg, Pick, pickButton, locButton;
    GKS_REAL locx, locy;
    GKS_INT RTNR;
    if (ug_if_gpx () == FALSE)
        return returned_status;
    
    wiz_jp_name (progname);
    
    *statPtr = FAIL;
    *segPtr = 0;
    *pickPtr = 0;
    *butPtr = 0;
    IF_NOT_WIN_OPEN_RETURN;
    
    RTNR = 0;
    wm_gsvpip (TEMP_TNR, RTNR, HIGHER);
    
    done = FALSE;
    do
        {
        
        if (wiv.jpmode == WI_PLAYBACK)
            {
            wiz_jp_read_long (&pickStatus);
            wiz_jp_read_long (&Seg);
            wiz_jp_read_long (&Pick);
            wiz_jp_read_long (&pickButton);
            }
        else
            {
            
            /* request and pick from GKS.                                           */
            wm_grqpk (wkid, pk_dev_num, &pickStatus, &Seg, &Pick);
            wm_gkzqbtn (&pickButton);
            
            if (wiv.jpmode == WI_JOURNAL)
                {
                wiz_jp_write_long (pickStatus);
                wiz_jp_write_long (Seg);
                wiz_jp_write_long (Pick);
                wiz_jp_write_long (pickButton);
                }
            }
        /* if button not equal to cancel                                            */
        if (pickButton != WM_BUTTON_CANCEL)
            {
            ugssamp_gc (TRUE);
            
            if (wiv.jpmode == WI_PLAYBACK)
                {
                wiz_jp_read_long (&locStatus);
                wiz_jp_read_long (&Tnr);
                wiz_jp_read_long (&locButton);
                wiz_jp_read_float (&locx);
                wiz_jp_read_float (&locy);
                }
            else
                {
                
                /* request x,y position and button number from GKS                  */
                wm_grqlc (wkid, locator_device, &locStatus, &Tnr, &locx, &locy);
                wm_gkzqbtn (&locButton);
                
                if (wiv.jpmode == WI_JOURNAL)
                    {
                    wiz_jp_write_long (locStatus);
                    wiz_jp_write_long (Tnr);
                    wiz_jp_write_long (locButton);
                    wiz_jp_write_float (locx);
                    wiz_jp_write_float (locy);
                    }
                }
            ugssamp_gc (FALSE);
            
            if (Tnr != TEMP_TNR)
                {
                tg_tnr2tnr (Tnr, locx, locy, TEMP_TNR, &locx, &locy);
                RTNR = Tnr;
                wm_gsvpip (TEMP_TNR, RTNR, HIGHER);
                }
            done = !(zzt_do_puckkey (locx, locy, locButton));
            }
        else
            {
            done = TRUE;
            }
        } while (!(done));
    
    if (pickStatus == (GKS_INT)GOK)
        {
        *statPtr = SUCCESS;
        }
    *segPtr = Seg;
    *pickPtr = Pick;
    *butPtr = pickButton;
    
    RTNR = 0;
    wm_gsvpip (TEMP_TNR, RTNR, (GKS_INT)GLOWER);
    
    return *statPtr;
    }

/* === WMOSET ===                                                                   */

#if USE_PROTOTYPES
publicdef INT wmoset (WM_ID obj_id, INT  class , CHAR *db_name, CHAR *usr_name)
#else
publicdef INT wmoset (obj_id, class , db_name, usr_name)
WM_ID obj_id;
INT  class ;
CHAR *db_name, *usr_name;
#endif
    {
    INT returned_status = SUCCESS;
    ALL_TREE_DATA all_data;
    
    IF_NOT_INIT_RETURN;
    
    if (btfind (&all_tree, (VOIDPTR)&obj_id, (VOIDPTR)&all_data))
        {
        all_data.cla = class ;
        zzz_store_str_n (all_data.dbs, db_name, WM_DBS_STR_LEN);
        zzz_store_str_n (all_data.usr, usr_name, WM_USR_STR_LEN);
        btrepcur (&all_tree, (VOIDPTR)&obj_id, (VOIDPTR)&all_data);
        wmz_update_gks_title (obj_id, usr_name);    /* MGKS                         */
        }
    else
        {
        returned_status = WM_ERR_OBJ_NOT_FOUND;
        }
    return returned_status;
    }

/* === WMOINQ ===                                                                   */

#if USE_PROTOTYPES
publicdef INT wmoinq (WM_ID obj_id, INT * class , CHAR *db_name, CHAR *usr_name)
#else
publicdef INT wmoinq (obj_id, class , db_name, usr_name)
WM_ID obj_id;
INT *class ;
CHAR *db_name, *usr_name;
#endif
    {
    
    INT returned_status = SUCCESS;
    ALL_TREE_DATA all_data;
    
    *class  = 0;
    db_name[0] = 0;
    usr_name[0] = 0;
    
    IF_NOT_INIT_RETURN;
    
    if (btfind (&all_tree, (VOIDPTR)&obj_id, (VOIDPTR)&all_data))
        {
        *class  = all_data.cla;
        strcpy (db_name, all_data.dbs);
        strcpy (usr_name, all_data.usr);
        }
    else
        {
        returned_status = WM_ERR_OBJ_NOT_FOUND;
        }
    return returned_status;
    }

/* === WMOINQCUR ===                                                                */

#if USE_PROTOTYPES
publicdef INT wmoinqcur (APPLICATION_ID *app_id, WINDOW_ID *win_id, TNR_ID *tnr_id,
                         GROUP_ID *gro_id, SEGMENT_ID *seg_id, PICK_ID *pic_id)
#else
publicdef INT wmoinqcur (app_id, win_id, tnr_id, gro_id, seg_id, pic_id)
APPLICATION_ID *app_id;
WINDOW_ID *win_id;
TNR_ID *tnr_id;
GROUP_ID *gro_id;
SEGMENT_ID *seg_id;
PICK_ID *pic_id;
#endif
    {
    
    INT returned_status = SUCCESS;
    
    *app_id = 0;
    *win_id = 0;
    *tnr_id = 0;
    *gro_id = 0;
    *seg_id = 0;
    *pic_id = 0;
    
    IF_NOT_INIT_RETURN;
    
    *app_id = wmv_current.app_id;
    *win_id = wmv_current.win_id;
    *tnr_id = wmv_current.tnr_id;
    *gro_id = wmv_current.gro_id;
    *seg_id = wmv_current.seg_id;
    *pic_id = wmv_current.pic_id;
    
    return returned_status;
    }

/* === WMOINQOLD ===                                                                */

#if USE_PROTOTYPES
publicdef INT wmoinqold (WM_ID obj_id, APPLICATION_ID *app_id, WINDOW_ID *win_id,
                         TNR_ID *tnr_id, GROUP_ID *gro_id, SEGMENT_ID *seg_id,
                         PICK_ID *pic_id)
#else
publicdef INT wmoinqold (obj_id, app_id, win_id, tnr_id, gro_id, seg_id, pic_id)
WM_ID obj_id;
APPLICATION_ID *app_id;
WINDOW_ID *win_id;
TNR_ID *tnr_id;
GROUP_ID *gro_id;
SEGMENT_ID *seg_id;
PICK_ID *pic_id;
#endif
    {
    
    INT returned_status = SUCCESS;
    ALL_TREE_DATA all_data;
    
    *app_id = 0;
    *win_id = 0;
    *tnr_id = 0;
    *gro_id = 0;
    *seg_id = 0;
    *pic_id = 0;
    
    IF_NOT_INIT_RETURN;
    
    if (btfind (&all_tree, (VOIDPTR)&obj_id, (VOIDPTR)&all_data))
        {
        *app_id = all_data.app;
        *win_id = all_data.win;
        *tnr_id = all_data.tnr;
        *gro_id = all_data.gro;
        *seg_id = all_data.seg;
        *pic_id = all_data.pic;
        }
    else
        {
        returned_status = WM_ERR_OBJ_NOT_FOUND;
        }
    return returned_status;
    }

/* === WMOINQTYP ===                                                                */

#if USE_PROTOTYPES
publicdef INT wmoinqtyp (WM_ID obj_id, INT *typ_id)
#else
publicdef INT wmoinqtyp (obj_id, typ_id)
WM_ID obj_id;
INT *typ_id;
#endif
    {
    
    INT returned_status = SUCCESS;
    ALL_TREE_DATA all_data;
    
    *typ_id = 0;
    
    IF_NOT_INIT_RETURN;
    
    if (btfind (&all_tree, (VOIDPTR)&obj_id, (VOIDPTR)&all_data))
        {
        *typ_id = all_data.typ;
        }
    else
        {
        returned_status = WM_ERR_OBJ_NOT_FOUND;
        }
    return returned_status;
    }

/* === ZZZ_NEXT_ID ===                                                              */

#if USE_PROTOTYPES
static WM_ID zzz_next_id (INT what_id)
#else
static WM_ID zzz_next_id (what_id)
INT what_id;
#endif
    {
    
    ALL_TREE_KEY all_key;
    ALL_TREE_DATA all_data;
    INT  class ;
    CHAR *usr_name;
    
    all_key = ++last_id;
    all_data.typ = what_id;
    all_data.app = wmv_current.app_id;
    all_data.win = wmv_current.win_id;
    all_data.tnr = wmv_current.tnr_id;
    all_data.gro = wmv_current.gro_id;
    all_data.seg = wmv_current.seg_id;
    all_data.pic = wmv_current.pic_id;
    all_data.cla = WM_CLASS_BACKGROUND;
    all_data.dbs[0] = 0;
    all_data.usr[0] = 0;
    all_data.user_structure = (BYTE *)NULL;
    
    switch (what_id)
        {
    case WM_CLASS_WINDOW:
        class  = WM_CLASS_WINDOW;
        usr_name = WM_CLSTR_WINDOW;
        break;
    case WM_CLASS_TNR:
        class  = WM_CLASS_TNR;
        usr_name = WM_CLSTR_TNR;
        break;
    case WM_CLASS_GROUP:
        class  = WM_CLASS_GROUP;
        usr_name = WM_CLSTR_GROUP;
        break;
    case WM_CLASS_SEGMENT:
        class  = WM_CLASS_SEGMENT;
        usr_name = WM_CLSTR_SEGMENT;
        break;
    case WM_CLASS_PICK:
        class  = WM_CLASS_PICK;
        usr_name = WM_CLSTR_PICK;
        break;
    default:
        class  = WM_CLASS_BACKGROUND;
        usr_name = WM_CLSTR_BACKGROUND;
        }
    all_data.cla = class ;
    zzz_store_str_n (all_data.usr, usr_name, WM_USR_STR_LEN);
    
    btadd (&all_tree, (VOIDPTR)&all_key, (VOIDPTR)&all_data);
    
    return(WM_ID)all_key;
    }

/* === ZZZ_STORE_STR_N ===                                                          */

#if USE_PROTOTYPES
static VOID zzz_store_str_n (CHAR *where, CHAR *what, size_t limit)
#else
static VOID zzz_store_str_n (where, what, limit)
CHAR *where, *what;
size_t limit;
#endif
    {
    
    if (strlen (what) <= limit)
        {
        strcpy (where, what);
        }
    else
        {
        strncpy ((char *)where, (char *)what, limit);
        where[limit - 3] = '.';
        where[limit - 2] = '.';
        where[limit - 1] = '.';
        where[limit] = 0;
        }
    }

/* === ZZZ_MALLOC ===                                                               */

#if USE_PROTOTYPES
static VOIDPTR zzz_malloc (size_t num_bytes)
#else
static VOIDPTR zzz_malloc (num_bytes)
size_t num_bytes;
#endif
    {
    
    ++num_malloc;
    
    return(VOIDPTR) tc_alloc (num_bytes);
    }

/* === ZZZ_FREE ===                                                                 */

#if USE_PROTOTYPES
static VOID zzz_free (VOIDPTR a_ptr)
#else
static VOID zzz_free (a_ptr)
VOIDPTR a_ptr;
#endif
    {
    
    ++num_free;
    
    tc_free (a_ptr);
    }

/* === WMWINQSLIST ===                                                              */

#if USE_PROTOTYPES
publicdef INT wmwinqslist (WINDOW_ID win_id, LISTHEAD *seg_list_ptr)
#else
publicdef INT wmwinqslist (win_id, seg_list_ptr)
WINDOW_ID win_id;
LISTHEAD *seg_list_ptr;
#endif
    {
    
    INT returned_status = SUCCESS;
    TREEHEAD *seg_tree_ptr, *gro_tree_ptr;
    WIN_TREE_DATA win_data;
    
    if (llcount (seg_list_ptr) > 0)
        {
        llfree (seg_list_ptr);
        }
    IF_NOT_APP_OPEN_RETURN;
    /*   CLOSE_WIN ;  commented out by Kee - if window being closed,                */
    /*                  the caller have to open it again, otherwise;                */
    /*          no window is activate.  I don't think                               */
    /*          CLOSE_WIN is necessary.  Anyway, this routine is                    */
    /*          never being called                                                  */
    
    if (btfind (wmv_current.win_tree_ptr, (VOIDPTR)&win_id, (VOIDPTR)&win_data))
        {
        seg_tree_ptr = win_data.seg;
        gro_tree_ptr = win_data.gro;
        zzw_inq_slist (seg_tree_ptr, gro_tree_ptr, seg_list_ptr);
        }
    else
        {
        returned_status = WM_ERR_WIN_NOT_FOUND;
        }
    return returned_status;
    }

/* === ZZW_INQ_SLIST ===                                                            */

#if USE_PROTOTYPES
static VOID zzw_inq_slist (TREEHEAD *seg_tree_ptr, TREEHEAD *gro_tree_ptr,
                               LISTHEAD *seg_list_ptr)
#else
static VOID zzw_inq_slist (seg_tree_ptr, gro_tree_ptr, seg_list_ptr)
TREEHEAD *seg_tree_ptr, *gro_tree_ptr;
LISTHEAD *seg_list_ptr;
#endif
    {
    
    INT num_items, count;
    GRO_TREE_KEY gro_id;
    GRO_TREE_DATA gro_data;
    SEG_TREE_KEY seg_id;
    SEG_TREE_DATA seg_data;
    
    num_items = btcount (seg_tree_ptr);
    if (num_items > 0)
        {
        btfirst (seg_tree_ptr, (VOIDPTR)&seg_id, (VOIDPTR)&seg_data);
        llappnd (seg_list_ptr, (VOIDPTR)&seg_id);
        for (count = 1; count < num_items; count++)
            {
            btnext (seg_tree_ptr, (VOIDPTR)&seg_id, (VOIDPTR)&seg_data);
            llappnd (seg_list_ptr, (VOIDPTR)&seg_id);
            }
        }
    num_items = btcount (gro_tree_ptr);
    if (num_items > 0)
        {
        btfirst (gro_tree_ptr, (VOIDPTR)&gro_id, (VOIDPTR)&gro_data);
        zzw_inq_slist (gro_data.seg, gro_data.gro, seg_list_ptr);
        for (count = 1; count < num_items; count++)
            {
            btnext (gro_tree_ptr, (VOIDPTR)&gro_id, (VOIDPTR)&gro_data);
            zzw_inq_slist (gro_data.seg, gro_data.gro, seg_list_ptr);
            }
        }
    }

/* === WM_SET_WKVP ===                                                              */

#if USE_PROTOTYPES
publicdef INT wm_set_wkvp (DOUBLE vpMinX, DOUBLE vpMaxX, DOUBLE vpMinY, DOUBLE vpMaxY)
#else
publicdef INT wm_set_wkvp (vpMinX, vpMaxX, vpMinY, vpMaxY)
DOUBLE vpMinX, vpMaxX, vpMinY, vpMaxY;
#endif
    {
    INT returned_status = SUCCESS;
    ALL_TREE_KEY all_key;
    ALL_TREE_DATA all_data;
    GKS_REAL dcDx, dcDy;
    GKS_REAL dcMinX, dcMaxX, dcMinY, dcMaxY;
    GKS_REAL ndcMinX, ndcMaxX, ndcMinY, ndcMaxY;
    INT num_items, i;
    
    IF_NOT_INIT_RETURN;
    CLOSE_WIN;
    
    dcMinX = vpMinX;
    dcMaxX = vpMaxX;
    dcMinY = vpMinY;
    dcMaxY = vpMaxY;
    ndcMinX = 0.0;
    ndcMaxX = 1.0;
    ndcMinY = 0.0;
    ndcMaxY = 1.0;
    dcDx = dcMaxX - dcMinX;
    dcDy = dcMaxY - dcMinY;
    if (dcDx != dcDy)
        {
        if (dcDx > dcDy)
            {
            ndcMaxY = dcDy / dcDx;
            }
        else
            {
            ndcMaxX = dcDx / dcDy;
            }
        }
    wm_gclrwk (MAIN_WKID, 0);
    wm_gswkvp (MAIN_WKID, dcMinX, dcMaxX, dcMinY, dcMaxY);
    wm_gswkwn (MAIN_WKID, ndcMinX, ndcMaxX, ndcMinY, ndcMaxY);
    wm_guwk (MAIN_WKID, 0);

#ifndef USE_X
    num_items = btcount (&all_tree);
    if (num_items > 3)
        {
        /* first segment is temp                                                    */
        btfirst (&all_tree, (VOIDPTR)&all_key, (VOIDPTR)&all_data);
        btnext (&all_tree, (VOIDPTR)&all_key, (VOIDPTR)&all_data);
        btnext (&all_tree, (VOIDPTR)&all_key, (VOIDPTR)&all_data);
        for (i = 3; i < num_items; i++)
            {
            btnext (&all_tree, (VOIDPTR)&all_key, (VOIDPTR)&all_data);
            if (all_data.typ == WM_CLASS_SEGMENT &&
                wm_obj_wkid ((WM_ID)all_data.seg) == MAIN_WKID) /* MGKS             */
                {
                wm_gasgwk (MAIN_WKID, (GKS_INT)all_key);
                }
            }
        }
#endif
    return returned_status;
    }

/* === WM_INQ_WKVP ===                                                              */

#if USE_PROTOTYPES
publicdef INT wm_inq_wkvp (GKS_REAL *dcMinXPtr, GKS_REAL *dcMaxXPtr,
                           GKS_REAL *dcMinYPtr, GKS_REAL *dcMaxYPtr)
#else
publicdef INT wm_inq_wkvp (dcMinXPtr, dcMaxXPtr, dcMinYPtr, dcMaxYPtr)
GKS_REAL *dcMinXPtr, *dcMaxXPtr, *dcMinYPtr, *dcMaxYPtr;
#endif
    {
    INT returned_status = SUCCESS;
    GKS_INT errind, tus;
    GKS_REAL rwindo[4], cwindo[4], rviewp[4], cviewp[4];
    
    *dcMinXPtr = 0.0;
    *dcMaxXPtr = 1.0;
    *dcMinYPtr = 0.0;
    *dcMaxYPtr = 1.0;
    
    IF_NOT_INIT_RETURN;
    
    wm_gqwkt (MAIN_WKID, &errind, &tus, rwindo, cwindo, rviewp, cviewp);
    if (!(errind))
        {
        *dcMinXPtr = cviewp[0];
        *dcMaxXPtr = cviewp[1];
        *dcMinYPtr = cviewp[2];
        *dcMaxYPtr = cviewp[3];
        }
    else
        {
        returned_status = WM_ERR_GQWKT;
        }
    return returned_status;
    }

/* === WM_INQ_WKWN ===                                                              */

#if USE_PROTOTYPES
publicdef INT wm_inq_wkwn (GKS_REAL *ndcMinXPtr, GKS_REAL *ndcMaxXPtr,
                           GKS_REAL *ndcMinYPtr, GKS_REAL *ndcMaxYPtr)
#else
publicdef INT wm_inq_wkwn (ndcMinXPtr, ndcMaxXPtr, ndcMinYPtr, ndcMaxYPtr)
GKS_REAL *ndcMinXPtr, *ndcMaxXPtr, *ndcMinYPtr, *ndcMaxYPtr;
#endif
    {
    
    INT returned_status = SUCCESS;
    GKS_INT errind, tus;
    GKS_REAL rwindo[4], cwindo[4], rviewp[4], cviewp[4];
    
    IF_NOT_INIT_RETURN;
    
    *ndcMinXPtr = 0.0;
    *ndcMaxXPtr = 1.0;
    *ndcMinYPtr = 0.0;
    *ndcMaxYPtr = 1.0;

    return returned_status;
    }

/* === WM_INQ_WKVP_MAX ===                                                          */

#if USE_PROTOTYPES
publicdef INT wm_inq_wkvp_max (GKS_REAL *dcMinXPtr, GKS_REAL *dcMaxXPtr,
                               GKS_REAL *dcMinYPtr, GKS_REAL *dcMaxYPtr)
#else
publicdef INT wm_inq_wkvp_max (dcMinXPtr, dcMaxXPtr, dcMinYPtr, dcMaxYPtr)
GKS_REAL *dcMinXPtr, *dcMaxXPtr, *dcMinYPtr, *dcMaxYPtr;
#endif
    {
    
    INT returned_status = SUCCESS;
    
    GKS_INT errind, conid, wtype, dcunit, lx, ly;
    GKS_REAL rx, ry;
    
    *dcMinXPtr = 0.0;
    *dcMaxXPtr = 1.0;
    *dcMinYPtr = 0.0;
    *dcMaxYPtr = 1.0;
    
    IF_NOT_INIT_RETURN;

#ifdef mips
    wtype = 213;                    /* 213 = GKS$K_DECWINDOWS_WIDGET                */
    errind = 0;
#else
    wm_gqwkc (MAIN_WKID, &errind, &conid, &wtype);
#endif
    if (!(errind))
        {
        wm_gqdsp (wtype, &errind, &dcunit, &rx, &ry, &lx, &ly);
        if (!(errind))
            {
            *dcMinXPtr = 0.0;
            *dcMaxXPtr = rx;
            *dcMinYPtr = 0.0;
            *dcMaxYPtr = ry;
            }
        else
            {
            returned_status = WM_ERR_GQDSP;
            }
        }
    else
        {
        returned_status = WM_ERR_GQWKC;
        }
    return returned_status;
    }

/* === WMTDRAGSEGMENT ===                                                           */

#if USE_PROTOTYPES
publicdef INT wmtdragsegment (INT *statPtr, SEGMENT_ID seg_id, DOUBLE wx1, DOUBLE wy1,
                              GKS_REAL *wx2Ptr, GKS_REAL *wy2Ptr, INT *butPtr)
#else
publicdef INT wmtdragsegment (statPtr, seg_id, wx1, wy1, wx2Ptr, wy2Ptr, butPtr)
INT *statPtr;
SEGMENT_ID seg_id;
DOUBLE wx1, wy1;
GKS_REAL *wx2Ptr, *wy2Ptr;
INT *butPtr;
#endif
    {
    
    INT returned_status = SUCCESS;
    GKS_INT ugSegment;
    GKS_REAL ugWx1, ugWy1, ugWx2, ugWy2;
    GKS_INT ugStatus, ugTnr, ugButton;
    GKS_INT RTNR;
    BOOL done;
    
    *statPtr = FAIL;
    *wx2Ptr = wx1;
    *wy2Ptr = wy1;
    *butPtr = 0;
    
    IF_NOT_WIN_OPEN_RETURN;
    
    RTNR = 0;
    wm_gsvpip (TEMP_TNR, RTNR, HIGHER);
    
    done = FALSE;
    do
        {
        ugSegment = seg_id;
        ugWx1 = wx1;
        ugWy1 = wy1;
        ugStatus = 0;
        ugTnr = 0;
        ugWx2 = wx1;
        ugWy2 = wy1;
        ugButton = 0;
        
        if (ug_if_gpx () == TRUE)
            ugStatus = wmz_getxy (&ugTnr, &ugWx2, &ugWy2, &ugButton, 2);
        else
            ugrqdrag_gc (ugSegment, ugWx1, ugWy1, &ugStatus, &ugTnr, &ugWx2, &ugWy2,
                         &ugButton);
        
        if (ugStatus == SUCCESS)
            {
            if (ugTnr != TEMP_TNR)
                {
                tg_tnr2tnr (ugTnr, ugWx2, ugWy2, TEMP_TNR, &ugWx2, &ugWy2);
                RTNR = ugTnr;
                wm_gsvpip (TEMP_TNR, RTNR, HIGHER);
                }
            done = !(zzt_do_puckkey (ugWx2, ugWy2, ugButton));
            }
        else
            {
            done = TRUE;
            }
        } while (!(done));
    
    *statPtr = ugStatus;
    *wx2Ptr = ugWx2;
    *wy2Ptr = ugWy2;
    *butPtr = ugButton;
    
    RTNR = 0;
    wm_gsvpip (TEMP_TNR, RTNR, (GKS_INT)GLOWER);
    return returned_status;
    }

/* === WMTDRAGRECTANGLE ===                                                         */

#if USE_PROTOTYPES
publicdef INT wmtdragrectangle (INT *statPtr, DOUBLE llx, DOUBLE lly, DOUBLE urx,
                                DOUBLE ury, GKS_REAL *llxPtr, GKS_REAL *llyPtr,
                                INT *butPtr)
#else
publicdef INT wmtdragrectangle (statPtr, llx, lly, urx, ury, llxPtr, llyPtr, butPtr)
INT *statPtr;
DOUBLE llx, lly;
DOUBLE urx, ury;
GKS_REAL *llxPtr, *llyPtr;
INT *butPtr;
#endif
    {
    INT returned_status = SUCCESS;
    GKS_INT ugSegment;
    GKS_REAL ugWx1, ugWy1, ugWx2, ugWy2;
    GKS_INT ugStatus, ugTnr, ugButton;
    GKS_INT RTNR, npts;
    GKS_REAL px[5], py[5];
    GKS_REAL midX, midY;
    BOOL done;
    
    *statPtr = FAIL;
    *llxPtr = llx;
    *llyPtr = lly;
    *butPtr = 0;
    
    IF_NOT_WIN_OPEN_RETURN;
    
    wm_gcrsg (TEMP_SEG);
    npts = 5;
    px[0] = llx;
    py[0] = lly;
    px[1] = llx;
    py[1] = ury;
    px[2] = urx;
    py[2] = ury;
    px[3] = urx;
    py[3] = lly;
    px[4] = llx;
    py[4] = lly;
    wm_gpl (npts, px, py);
    wm_gclsg ();
    
    RTNR = 0;
    wm_gsvpip (TEMP_TNR, RTNR, HIGHER);
    
    midX = (llx + urx) / 2.0;
    midY = (lly + ury) / 2.0;
    
    done = FALSE;
    do
        {
        ugSegment = TEMP_SEG;
        ugWx1 = midX;
        ugWy1 = midY;
        ugStatus = 0;
        ugTnr = 0;
        ugWx2 = midX;
        ugWy2 = midY;
        ugButton = 0;
        ugrqdrag_gc (ugSegment, ugWx1, ugWy1, &ugStatus, &ugTnr, &ugWx2, &ugWy2,
                     &ugButton);
        if (ugStatus == SUCCESS)
            {
            if (ugTnr != TEMP_TNR)
                {
                tg_tnr2tnr (ugTnr, ugWx2, ugWy2, TEMP_TNR, &ugWx2, &ugWy2);
                RTNR = ugTnr;
                wm_gsvpip (TEMP_TNR, RTNR, HIGHER);
                }
            done = !(zzt_do_puckkey (ugWx2, ugWy2, ugButton));
            }
        else
            {
            done = TRUE;
            }
        } while (!(done));
    
    *statPtr = ugStatus;
    *llxPtr = llx + (ugWx2 - midX);
    *llyPtr = lly + (ugWy2 - midY);
    *butPtr = ugButton;
    
    RTNR = 0;
    wm_gsvpip (TEMP_TNR, RTNR, (GKS_INT)GLOWER);
    
    wm_gdsg ((GKS_INT)TEMP_SEG);
    return returned_status;
    }

/* === WMTINQBUTTONINTERP ===                                                       */

#if USE_PROTOTYPES
publicdef INT wmtinqbuttoninterp (INT *onOff)
#else
publicdef INT wmtinqbuttoninterp (onOff)
INT *onOff;
#endif
    {
    
    IF_NOT_INIT_RETURN;
    
    *onOff = if_interp;
    return SUCCESS;
    }

/* === WMTBUTTONINTERP ===                                                          */

#if USE_PROTOTYPES
publicdef INT wmtbuttoninterp (BOOL onOff)
#else
publicdef INT wmtbuttoninterp (onOff)
BOOL onOff;
#endif
    {
    
    IF_NOT_INIT_RETURN;
    
    if_interp = onOff;
    return SUCCESS;
    }

/************************************************************************************/

/*                                                                                  */
/* * File : WM_USER_STRUCT.C                                                        */
/* *                                                                                */
/* * Routines to manipulate User defined Structure within Window manager.           */
/*                                                                                  */

/*************************************************************                      */
/* * Routine : WM_INQ_USER_STRUCTURE()                                              */
/* *                                                                                */
/* * Description : Inquire for a user defined structure, within the                 */
/* *               window manager based on an object id and an offset.              */
/* *                                                                                */
/* ************************************************************                     */
#if USE_PROTOTYPES
publicdef INT wm_inq_user_structure (WM_ID obj_id, VOIDPTR user_structure_ptr,
                                     size_t nbytes)
#else
publicdef INT wm_inq_user_structure (obj_id, user_structure_ptr, nbytes)
WM_ID obj_id;                       /* window manager object id                     */
VOIDPTR user_structure_ptr;         /* pointer to the user defined structure        */
size_t nbytes;                      /* length in bytes of the users structure       */
#endif
    {
    INT status = SUCCESS;
    VOIDPTR src_ptr;                /* temporary structure pointers                 */
    ALL_TREE_DATA node_data;        /* node data storage                            */
    
    /*                                                                              */
    /* * Find the BTree entry for the given object (node) id                        */
    /* * from the window manager allocation tree                                    */
    /*                                                                              */
    if (btfind (&all_tree, (VOIDPTR)&obj_id, (VOIDPTR)&node_data))
        {
        /* get the pointer to the user defined structure                            */
        src_ptr = (VOIDPTR)node_data.user_structure;
        
        /* move the information into the users structure                            */
        hoblockmove (src_ptr, (VOIDPTR)user_structure_ptr, nbytes);
        }
    else
        {
        status = WM_ERR_OBJ_NOT_FOUND;
        }
    return status;
    }

/*************************************************************                      */
/* * Routine : WM_SET_USER_STRUCTURE()                                              */
/* *                                                                                */
/* * Description : Set a user defined structure, within the window manager          */
/* *               based on an id and an offset.                                    */
/* *                                                                                */
/* ************************************************************                     */
#if USE_PROTOTYPES
publicdef INT wm_set_user_structure (WM_ID obj_id, VOIDPTR user_structure_ptr,
                                     size_t nbytes)
#else
publicdef INT wm_set_user_structure (obj_id, user_structure_ptr, nbytes)
WM_ID obj_id;                       /* window manager object id                     */
VOIDPTR user_structure_ptr;         /* pointer to the user defined structure        */
size_t nbytes;                      /* length in bytes of user structure            */
#endif
    {
    
    INT status = SUCCESS;
    VOIDPTR dest_ptr;               /* temporary structure pointers                 */
    ALL_TREE_DATA node_data;        /* node data storage                            */
    
    /*                                                                              */
    /* * Find the BTree entry for the given object id                               */
    /* * from the window managers allocation tree                                   */
    /*                                                                              */
    if (btfind (&all_tree, (VOIDPTR)&obj_id, (VOIDPTR)&node_data))
        {
        /* get enough space to hold the structure                                   */
        dest_ptr = tc_alloc (nbytes);
        
        /* move the information into the objects 'user structure'                   */
        hoblockmove ((VOIDPTR)user_structure_ptr, dest_ptr, nbytes);
        
        /* if a structure is allready exists free the old structure                 */
        if (node_data.user_structure != (BYTE *)NULL)
            {
            tc_free ((VOIDPTR)node_data.user_structure);
            }
        /* set the pointer to the user defined structure                            */
        node_data.user_structure = (BYTE *)dest_ptr;
        
        /* restore the data to the Btree                                            */
        btrepcur (&all_tree, (VOIDPTR)&obj_id, (VOIDPTR)&node_data);
        }
    else
        {
        status = WM_ERR_OBJ_NOT_FOUND;
        }
    return status;
    }

/*************************************************************                      */
/* * Routine : WM_INQ_FRONT_WINDOW()                                                */
/* *                                                                                */
/* * Description : Returns the id of the current front window                       */
/* *                                                                                */
/* ************************************************************                     */
#if USE_PROTOTYPES
publicdef INT wm_inq_front_window (WINDOW_ID *window_id)
#else
publicdef INT wm_inq_front_window (window_id)
WINDOW_ID *window_id;               /* current front window id                      */
#endif
    {
    
    INT status = SUCCESS;
    
    *window_id = FRONTED_WINDOW_FLAG;
    
    return status;
    }

/* === WMTPANZOOM ===                                                               */

#if USE_PROTOTYPES
publicdef INT wmtpanzoom (TNR_ID tnr_id, DOUBLE xo, DOUBLE yo, DOUBLE fx, DOUBLE fy)
#else
publicdef INT wmtpanzoom (tnr_id, xo, yo, fx, fy)
TNR_ID tnr_id;
DOUBLE xo, yo, fx, fy;
#endif
    {
    
    INT returned_status = SUCCESS;
    TNR_TREE_DATA tnr_data;
    WIN_TREE_DATA win_data;
    TREEHEAD *seg_tree_ptr, *gro_tree_ptr;
    
    IF_NOT_WIN_OPEN_RETURN;
    CLOSE_SEG;
    
    if (btfind (wmv_current.tnr_tree_ptr, (VOIDPTR)&tnr_id, (VOIDPTR)&tnr_data))
        {
        tnr_data.panx = xo;
        tnr_data.pany = yo;
        tnr_data.zoomx = fx;
        tnr_data.zoomy = fy;
        btrepcur (wmv_current.tnr_tree_ptr, (VOIDPTR)&tnr_id, (VOIDPTR)&tnr_data);
        wmtselect (tnr_id);
        
        btfind (wmv_current.win_tree_ptr, (VOIDPTR)&wmv_current.win_id,
                (VOIDPTR)&win_data);
        seg_tree_ptr = win_data.seg;
        gro_tree_ptr = win_data.gro;
        zzt_pan_zoom (tnr_id, seg_tree_ptr, gro_tree_ptr);
        }
    else
        {
        returned_status = WM_ERR_TNR_NOT_FOUND;
        }
    return returned_status;
    }

/* === ZZT_PAN_ZOOM ===                                                             */

#if USE_PROTOTYPES
static VOID zzt_pan_zoom (TNR_ID tnr_id, TREEHEAD *seg_tree_ptr,
                              TREEHEAD *gro_tree_ptr)
#else
static VOID zzt_pan_zoom (tnr_id, seg_tree_ptr, gro_tree_ptr)
TNR_ID tnr_id;
TREEHEAD *seg_tree_ptr;
TREEHEAD *gro_tree_ptr;
#endif
    {
    SEG_TREE_KEY seg_id;
    SEG_TREE_DATA seg_data;
    GRO_TREE_KEY gro_id;
    GRO_TREE_DATA gro_data;
    INT num_items, count;
    
    num_items = btcount (seg_tree_ptr);
    if (num_items > 0)
        {
        btfirst (seg_tree_ptr, (VOIDPTR)&seg_id, (VOIDPTR)&seg_data);
        if (seg_data.tnr == tnr_id)
            {
            wm_gssgt ((GKS_INT)seg_id, wmv_current.TM);
            }
        for (count = 1; count < num_items; count++)
            {
            btnext (seg_tree_ptr, (VOIDPTR)&seg_id, (VOIDPTR)&seg_data);
            if (seg_data.tnr == tnr_id)
                {
                wm_gssgt ((GKS_INT)seg_id, wmv_current.TM);
                }
            }
        }
    num_items = btcount (gro_tree_ptr);
    if (num_items > 0)
        {
        btfirst (gro_tree_ptr, (VOIDPTR)&gro_id, (VOIDPTR)&gro_data);
        zzt_pan_zoom (tnr_id, gro_data.seg, gro_data.gro);
        for (count = 1; count < num_items; count++)
            {
            btnext (gro_tree_ptr, (VOIDPTR)&gro_id, (VOIDPTR)&gro_data);
            zzt_pan_zoom (tnr_id, gro_data.seg, gro_data.gro);
            }
        }
    }

/* === ZZT_WCS2NWC ===                                                              */

#if USE_PROTOTYPES
publicdef VOID zzt_wcs2nwc (DOUBLE wcsX, DOUBLE wcsY, GKS_REAL *nwcXPtr,
                            GKS_REAL *nwcYPtr)
#else
publicdef VOID zzt_wcs2nwc (wcsX, wcsY, nwcXPtr, nwcYPtr)
DOUBLE wcsX, wcsY;
GKS_REAL *nwcXPtr, *nwcYPtr;
#endif
    {
    GKS_REAL cx, cy;
    POINT wcsPt, nwcPt;
    RECTANGLE wcsRect;
    PIPE wcs2nwc;
    
    wcsPt.x = wcsX;
    wcsPt.y = wcsY;
    
    cx = wmv_current.wcs.min.x + ((wmv_current.wcs.max.x - wmv_current.wcs.min.x) / 2);
    cy = wmv_current.wcs.min.y + ((wmv_current.wcs.max.y - wmv_current.wcs.min.y) / 2);
    wcsRect.min.x = wmv_current.panx + ((wmv_current.wcs.min.x - cx) /
                                        wmv_current.zoomx);
    wcsRect.max.x = wmv_current.panx + ((wmv_current.wcs.max.x - cx) /
                                        wmv_current.zoomx);
    wcsRect.min.y = wmv_current.pany + ((wmv_current.wcs.min.y - cy) /
                                        wmv_current.zoomy);
    wcsRect.max.y = wmv_current.pany + ((wmv_current.wcs.max.y - cy) /
                                        wmv_current.zoomy);
    PR_SetPipe (&wcs2nwc, &wcsRect, &wmv_current.nwc);
    
    PR_PipePt (&wcsPt, &wcs2nwc, &nwcPt);
    
    *nwcXPtr = nwcPt.x;
    *nwcYPtr = nwcPt.y;
    }
/* === zzt_do_puckkey ===                                                           */
/*                                                                                  */
/*   This is an internal WM function used to service the A,B,E,F keys               */
/*   for all such interactions as wmtlocate(), wmtbandrectangle(), wmprequest(),... */
/*                                                                                  */
/*   The returned status is:                                                        */
/*                                                                                  */
/*      TRUE  button was trapped and interpreted,                                   */
/*        meaning that a screen refresh was performed.                              */
/*      FALSE button had no meaning to zzt_do_puckkey,                              */
/*        (e.g., a normal digitize button)                                          */
/*                                                                                  */
#if USE_PROTOTYPES
static INT zzt_do_puckkey (DOUBLE wx, DOUBLE wy, INT puckkey)
#else
static INT zzt_do_puckkey (wx, wy, puckkey)
DOUBLE wx, wy;
INT puckkey;
#endif
    {
    INT returnVar = FALSE;
    BOOL if_binterp;
    
    wmtinqbuttoninterp (&if_binterp);
    
    if (if_binterp)
        {
        if (wmtdopuckkey (wx, wy, puckkey) == SUCCESS)
            returnVar = TRUE;
        }
    return returnVar;
    }

/* **********************************************************************           */
/*                                                                                  */
/*    ROUTINE:    WM_DEFINE_TABLET_KEYPAD                                           */
/*                                                                                  */
/*    DESCRIPTION:                                                                  */
/*    This routine is designed to define the pseudo keypad on the tablet            */
/*    for the current window on the GPX workstation.                                */
/*                                                                                  */
/*    INPUT:                                                                        */
/*    TNR_ID  = (INT ) The transformation id.                                       */
/*    ROW     = (INT ) The total number of rows of keypad.                          */
/*    COLUMN  = (INT ) The total number of columns of keypad.                       */
/*    DIRECTION = (INT ) The direction of key number order.                         */
/*                HORIZONAL_FIRST_KEYPAD, VERTICAL_FIRST_KEYPAD.                    */
/*                                                                                  */
/*    **********************************************************************        */
#if USE_PROTOTYPES
publicdef INT wm_define_tablet_keypad (TNR_ID tnr_id, INT rows, INT columns,
                                       INT direction)
#else
publicdef INT wm_define_tablet_keypad (tnr_id, rows, columns, direction)
TNR_ID tnr_id;
INT rows;
INT columns;
INT direction;
#endif
    {
    
    INT returned_status = SUCCESS;
    TNR_TREE_DATA tnr_data;
    EXTENTS extents;
    BOOL reset = TRUE;
    DOUBLE dy10, dx10, angle;
    
    IF_NOT_WIN_OPEN_RETURN;
    
    if (btfind (wmv_current.tnr_tree_ptr, (VOIDPTR)&tnr_id, (VOIDPTR)&tnr_data))
        {
        
        /* bring up the dialog box on Mac                                           */
        /* and request user to register the                                         */
        /* pseudo keypad on the tablet.                                             */
        
        if (tnr_data.reg.keypad_reg != (POINT *)NULL)
            {
            reset = zzt_ask_reset ("Do you want to re-register the psudo keypad?");
            }
        else
            {
            reset = zzt_ask_reset (
"Do you wish to use a function template \
instead the puck buttons to control digitizing?"
                    );
            }
        if (!reset)
            {
            return SUCCESS;
            }
        /* allocate the space to store the                                          */
        /* device coord. value for each                                             */
        /* pseudo key.                                                              */
        
        if (tnr_data.reg.keypad_reg != (POINT *)0)
            {
            tc_zeroes ((VOIDPTR)tnr_data.reg.keypad_reg, sizeof(POINT) * 4);
            }
        else
            {
            tnr_data.reg.keypad_reg = (POINT *) tc_zalloc (sizeof(POINT) * 4);
            }
        /* ask user to register the pseudo keypad.                                  */
        returned_status = pd_reg_cdlg (&extents);
        
        tnr_data.reg.keypad_reg[0].x = extents.ll_x;
        tnr_data.reg.keypad_reg[0].y = extents.ll_y;
        tnr_data.reg.keypad_reg[1].x = extents.lr_x;
        tnr_data.reg.keypad_reg[1].y = extents.lr_y;
        tnr_data.reg.keypad_reg[2].x = extents.ur_x;
        tnr_data.reg.keypad_reg[2].y = extents.ur_y;
        tnr_data.reg.keypad_reg[3].x = extents.ul_x;
        tnr_data.reg.keypad_reg[3].y = extents.ul_y;
        
        /* calculate the rotate transformation                                      */
        /*    for pseudo_key, in case the keypad                                    */
        /*    is not paralleled to the horizon.                                     */
        
        dy10 = extents.lr_y - extents.ll_y;
        dx10 = extents.lr_x - extents.ll_x;
        dy10 = dy10 * dy10;
        dx10 = dx10 * dx10;
        angle = atan2 (sqrt (dy10), sqrt (dx10));
        tnr_data.reg.wcos = cos (angle);
        tnr_data.reg.wsin = sin (angle);
        if (returned_status != SUCCESS)
            return returned_status;
        /* calculate the transformation of                                          */
        /* rotation.                                                                */
        
        tnr_data.reg.num_rows = rows;
        tnr_data.reg.num_columns = columns;
        tnr_data.reg.direction = direction;
        btrepcur (wmv_current.tnr_tree_ptr, (VOIDPTR)&tnr_id, (VOIDPTR)&tnr_data);
        returned_status = SUCCESS;
        }
    else
        {
        returned_status = WM_ERR_TNR_NOT_FOUND;
        }
    return returned_status;
    }

/* **********************************************************************           */
/*                                                                                  */
/*    ROUTINE:    WM_SET_PICK_DEVICE                                                */
/*                                                                                  */
/*    DESCRIPTION:                                                                  */
/*    The routine is designed to set the picking device, it should be               */
/*    called before user call WMTLOCATE() to inquire the picking inputs, when       */
/*.    the workstaion in GPX.                                                       */
/*                                                                                  */
/*    INPUT:                                                                        */
/*    TNR_ID = (TNR_ID )The transformation id;                                      */
/*    PICK_DEVICE = (INT ) The specified picking/digitizing device.                 */
/*                                                                                  */
/*    **********************************************************************        */
#if USE_PROTOTYPES
publicdef INT wm_set_pick_device (TNR_ID tnr_id, INT pick_device)
#else
publicdef INT wm_set_pick_device (tnr_id, pick_device)
TNR_ID tnr_id;
INT pick_device;
#endif
    {
    INT returned_status = SUCCESS;
    TNR_TREE_DATA tnr_data;
#ifdef DEBUG
    CHAR msg[128];
#endif
    IF_NOT_WIN_OPEN_RETURN;
    
    if (btfind (wmv_current.tnr_tree_ptr, (VOIDPTR)&tnr_id, (VOIDPTR)&tnr_data))
        {
        /* If the specified pick_device is WM_PD_TABLET, we must make sure that the user    */
        /*has already registered the pseudo keypad, if not, display the alter message and   */
        /*return.                                                                   */
        
        if (pick_device == WM_PD_TABLET AND tnr_data.reg.keypad_reg == (POINT *)NULL)
            {
#ifdef DEBUG
            sprintf (msg, "%s", 
"There is no registration of pseudo keypad, the system only\
 can support function button as digitizer hardware limits."
                    );
            am_message (AM_ALERT, msg);
#endif
            }
        else if (pick_device == WM_PD_MOUSE AND tnr_data.reg.menu_keys ==
                 (CHAR * *)NULL)
            {
#ifdef DEBUG
            sprintf (msg, "%s", 
"Warning: There are no menus defined for mouse picking, the system \
will use a default menu (1 - 16). "
                    );
            am_message (AM_ALERT, msg);
#endif
            }
        tnr_data.reg.key_type = pick_device;
        btrepcur (wmv_current.tnr_tree_ptr, (VOIDPTR)&tnr_id, (VOIDPTR)&tnr_data);
        tnr_data.reg.menu_updated = TRUE;
        (wm_static ())->reg.default_button = 1;
        }
    else
        {
        returned_status = WM_ERR_TNR_NOT_FOUND;
        }
    return returned_status;
    }

/* **********************************************************************           */
#if USE_PROTOTYPES
static BOOL zzt_ask_reset (CHAR *text)
#else
static BOOL zzt_ask_reset (text)
CHAR *text;
#endif
    {
    AS_YESNO_WORKSPACE *pyn;
    
    /* **************************************************************************   */
    
    
    pyn = (AS_YESNO_WORKSPACE *) am_allocate (AM_APPLICATION,
                                              sizeof(AS_YESNO_WORKSPACE));
    pyn->cancel = FALSE;
    
    sprintf (pyn->question, "%s", text);
    
    asp_yesno (pyn);
    
    if (pyn->answer == TRUE)
        return TRUE;
    else
        return FALSE;
    }

/************************************************************************************/

#if USE_PROTOTYPES
static BOOL zzt_chk_pseudo_key (REGISTRATION *reg, INT x, INT y, INT *button)
#else
static BOOL zzt_chk_pseudo_key (reg, x, y, button)
REGISTRATION *reg;
INT x, y, *button;
#endif
    {
    INT rotate_x, rotate_y;
    INT temp_x, temp_y;
    INT i, j;
    DOUBLE row_inc, column_inc;
    DOUBLE top;
    BOOL done;
    
    *button = 0;
    /* rotate the digitized point to be the parallral value to the horizonal.       */
    
    temp_x = (INT)(x - reg->keypad_reg[0].x);
    temp_y = (INT)(y - reg->keypad_reg[0].y);
    
    rotate_x = temp_x * reg->wcos - temp_y * reg->wsin + reg->keypad_reg[0].x;
    rotate_y = temp_x * reg->wsin + temp_y * reg->wcos + reg->keypad_reg[0].y;
    
    /* The point is not part of pseudo key.                                         */
    
    if (rotate_x < reg->keypad_reg[0].x OR rotate_x >
        reg->keypad_reg[1].x OR rotate_y < reg->keypad_reg[0].y OR rotate_y >
        reg->keypad_reg[2].y)
        {
        return FALSE;
        }
    /* calcultes the increments of keys.                                            */
    row_inc = (reg->keypad_reg[2].y - reg->keypad_reg[1].y) / (reg->num_rows);
    column_inc = (reg->keypad_reg[1].x - reg->keypad_reg[0].x) / (reg->num_columns);
    
    for (i = 1, top = reg->keypad_reg[2].y, done = FALSE; i <= reg->num_rows && !done;
         i++)
        {
        done = (rotate_y >= top - (row_inc * i));
        }
    i--;
    
    for (j = 1, top = reg->keypad_reg[0].x, done = FALSE;
         j <= reg->num_columns && !done; j++)
        {
        done = (rotate_x <= top + (column_inc * j));
        }
    j--;
    /* decide the number of keypad.                                                 */
    
    if (reg->direction == WM_HORI_KEYPAD)
        {
        *button = (i - 1) * reg->num_columns + j - 1;
        }
    else
        {
        *button = (j - 1) * reg->num_rows + i - 1;
        }
    return TRUE;
    
    }

/*****************************************************************************      */
/*      Copyright 1989, Exploration Systems Inc.  San Francisco, CA.                */
/*                                                                                  */
/*WM_CURRENT *wm_static                                                             */
/*                                                                                  */
/*-   Return a pointer to the 'current' structure in wm_windows.c                   */
/*                                                                                  */
/*Julian Carlisle  Dec 27, 1988                                                     */
/*****************************************************************************      */


#if USE_PROTOTYPES
publicdef WM_CURRENT *wm_static (void)
#else
publicdef WM_CURRENT *wm_static ()
#endif
    {
    return &wmv_current;
    }

/* === WM_INQ_SEG_VIS ===                                                           */

#if USE_PROTOTYPES
publicdef INT wm_inq_seg_vis (SEGMENT_ID seg_id)
#else
publicdef INT wm_inq_seg_vis (seg_id)
SEGMENT_ID seg_id;
#endif
    {
    SEG_TREE_DATA seg_data;
    
    /* * FIRST SEARCH WINDOW SEGMENT TREE *                                         */
    /* ------------------------------------                                         */
    
    if (btfind (wmv_current.seg_tree_ptr, (VOIDPTR)&seg_id, (VOIDPTR)&seg_data))
        {
        return seg_data.visibility;
        }
    /* * NOW SEARCH GROUP TREES FOR SEGMENT *                                       */
    /* --------------------------------------                                       */
    
    else if (wmz_seg_found (wmv_current.gro_tree_ptr, &seg_id, &seg_data))
        {
        return seg_data.visibility;
        }
    else
        {
        return FAIL;
        }
    }

/* === WMZ_SEG_FOUND ===                                                            */

#if USE_PROTOTYPES
static BOOL wmz_seg_found (TREEHEAD *gro_tree_ptr, SEGMENT_ID *seg_id,
                               SEG_TREE_DATA *seg_data)
#else
static BOOL wmz_seg_found (gro_tree_ptr, seg_id, seg_data)
TREEHEAD *gro_tree_ptr;
SEGMENT_ID *seg_id;
SEG_TREE_DATA *seg_data;
#endif
    {
    INT status;
    INT gro_id;
    GRO_TREE_DATA gro_data;
    
    status = btfirst (gro_tree_ptr, (VOIDPTR)&gro_id, (VOIDPTR)&gro_data);
    while (status == SUCCESS)
        {
        
        /* * SEARCH GROUP'S SEGMENT TREE *                                          */
        /* -------------------------------                                          */
        
        if (btfind (gro_data.seg, (VOIDPTR)seg_id, (VOIDPTR)seg_data))
            {
            return TRUE;
            }
        /* * SEARCH GROUP'S SUBGROUPS *                                             */
        /* ----------------------------                                             */
        
        if (wmz_seg_found (gro_data.gro, seg_id, seg_data))
            {
            return TRUE;
            }
        /* * NOT FOUND, GET NEXT GROUP TO SEARCH *                                  */
        /* ---------------------------------------                                  */
        
        status = (NOT btnext (gro_tree_ptr, (VOIDPTR)&gro_id, (VOIDPTR)&gro_data));
        }
    return FALSE;
    }

/************************************************************************************/

#if USE_PROTOTYPES
publicdef INT wm_set_window_size (WINDOW_ID win_id, CHAR *units, DOUBLE width,
                                  DOUBLE height)
#else
publicdef INT wm_set_window_size (win_id, units, width, height)
WINDOW_ID win_id;
CHAR *units;
DOUBLE width, height;
#endif
    {
    WIN_TREE_DATA win_data;
    INT status = SUCCESS;
    
    if (btfind (wmv_current.win_tree_ptr, (VOIDPTR)&win_id, (VOIDPTR)&win_data))
        {
        win_data.win_width = width;
        win_data.win_height = height;
        strcpy ((char *)win_data.win_uom.uom_name, (char *)units);
        btrepcur (wmv_current.win_tree_ptr, (VOIDPTR)&win_id, (VOIDPTR)&win_data);
        }
    else
        {
        status = WM_ERR_WIN_NOT_FOUND;
        }
    return status;
    }

/************************************************************************************/

#if USE_PROTOTYPES
publicdef INT wm_get_window_size (WINDOW_ID win_id, CHAR *units, DOUBLE *width,
                                  DOUBLE *height)
#else
publicdef INT wm_get_window_size (win_id, units, width, height)
WINDOW_ID win_id;
CHAR *units;
DOUBLE *width, *height;
#endif
    {
    WIN_TREE_DATA win_data;
    INT status = SUCCESS;
    
    if (btfind (wmv_current.win_tree_ptr, (VOIDPTR)&win_id, (VOIDPTR)&win_data))
        {
        *width = win_data.win_width;
        *height = win_data.win_height;
        strcpy (units, win_data.win_uom.uom_name);
        }
    else
        {
        status = WM_ERR_WIN_NOT_FOUND;
        }
    return status;
    }


#if USE_PROTOTYPES
publicdef GKS_INT wm_mainwkid (void)
#else
publicdef GKS_INT wm_mainwkid ()
#endif
    {
    return wmv_current.ws_id;
    }


#if USE_PROTOTYPES
publicdef GKS_INT wm_maxwknum (void)
#else
publicdef GKS_INT wm_maxwknum ()
#endif
    {
    return wmv_current.max_ws;
    }


#if USE_PROTOTYPES
publicdef INT wm_wkid_update (WINDOW_ID obj_id)
#else
publicdef INT wm_wkid_update (obj_id)
WINDOW_ID obj_id;
#endif
    {
    GKS_INT cur_wkid = MAIN_WKID;
    GKS_INT new_wkid = wm_obj_wkid ((WM_ID)obj_id);
    
    /* update workstation id if necessary for MGKS                                  */
    if (new_wkid != cur_wkid)
        {
        if (cur_wkid > 0)
            {
            wm_gdawk (cur_wkid);    /* deactivate current workstation id            */
            }
        wmv_current.ws_id = new_wkid;
        wm_gacwk (new_wkid);        /* activate the new one                         */
        }
    wmz_front_wk_widget (new_wkid); /* MGKS - front workstation widget              */
    return(SUCCESS);
    }


#if USE_PROTOTYPES
publicdef GKS_INT wm_obj_wkid (WM_ID obj_id)
#else
publicdef GKS_INT wm_obj_wkid (obj_id)
WM_ID obj_id;
#endif
    {
    /* MGKS  - return the workstation id of the object                              */
    INT obj_typ;
    WIN_TREE_DATA win_data;
    GRO_TREE_DATA gro_data;
    SEG_TREE_DATA seg_data;
    TNR_TREE_DATA tnr_data;
    
    wmoinqtyp (obj_id, &obj_typ);
    switch (obj_typ)
        {
    case WM_CLASS_WINDOW:
        if (btfind (wmv_current.win_tree_ptr, (VOIDPTR)&obj_id, (VOIDPTR)&win_data))
            {
            return(win_data.wkid);
            }
        break;
    case WM_CLASS_GROUP:
        if (btfind (wmv_current.gro_tree_ptr, (VOIDPTR)&obj_id, (VOIDPTR)&gro_data))
            {
            return(gro_data.wkid);
            }
        break;
    case WM_CLASS_SEGMENT:
        if (btfind (wmv_current.seg_tree_ptr, (VOIDPTR)&obj_id, (VOIDPTR)&seg_data))
            {
            return(seg_data.wkid);
            }
        break;
    case WM_CLASS_TNR:
        if (btfind (wmv_current.tnr_tree_ptr, (VOIDPTR)&obj_id, (VOIDPTR)&tnr_data))
            {
            return(tnr_data.wkid);
            }
        break;
    default:
        return SUCCESS;
        }
    }


#if USE_PROTOTYPES
publicdef INT wmz_inq_wk_wins (GKS_INT wk_id)
#else
publicdef INT wmz_inq_wk_wins (wk_id)
GKS_INT wk_id;
#endif
    {                               /* MGKS - return number of wins corresponding to certain workstation    */
    WIN_TREE_KEY win_key;
    WIN_TREE_DATA win_data;
    INT count, i, num_win = 0;
    
    count = btcount (wmv_current.win_tree_ptr);
    for (i = 1; count > 0 && i <= count; i++)
        {
        if (i == 1)
            {
            btfirst (wmv_current.win_tree_ptr, (VOIDPTR)&win_key, (VOIDPTR)&win_data);
            }
        else
            {
            btnext (wmv_current.win_tree_ptr, (VOIDPTR)&win_key, (VOIDPTR)&win_data);
            }
        if (win_data.wkid == wk_id)
            {
            num_win++;
            }
        }
    return(num_win);
    }

/* === WM_WKID_TO_WINDOW ===                                                        */

/* ---------------------------------------                                          */
/* Inquire the FIRST window id associated                                           */
/* with the specified GKS workstation id                                            */
/* Returns: window_id if we find the wk_id                                          */
/*          zero, otherwise                                                         */
/* ---------------------------------------                                          */

#if USE_PROTOTYPES
publicdef WINDOW_ID wm_wkid_to_window (GKS_INT wk_id)
#else
publicdef WINDOW_ID wm_wkid_to_window (wk_id)
GKS_INT wk_id;
#endif
    {
    WINDOW_ID win_key;
    WIN_TREE_DATA win_data;
    INT count, i;
    
    count = btcount (wmv_current.win_tree_ptr);
    for (i = 1; count > 0 && i <= count; i++)
        {
        if (i == 1)
            {
            btfirst (wmv_current.win_tree_ptr, (VOIDPTR)&win_key, (VOIDPTR)&win_data);
            }
        else
            {
            btnext (wmv_current.win_tree_ptr, (VOIDPTR)&win_key, (VOIDPTR)&win_data);
            }
        if (win_data.wkid == wk_id)
            {
            return win_key;
            }
        }
    return (WINDOW_ID)0;
    }

/* === WM_WKID_TO_TNR ===                                                           */

/* ---------------------------------------                                          */
/* Inquire the FIRST tnr id associated                                              */
/* with the specified GKS workstation id                                            */
/* Returns: tnr_id if we find the wk_id                                             */
/*          zero, otherwise                                                         */
/* ---------------------------------------                                          */

#if USE_PROTOTYPES
publicdef TNR_ID wm_wkid_to_tnr (GKS_INT wk_id)
#else
publicdef TNR_ID wm_wkid_to_tnr (wk_id)
GKS_INT wk_id;
#endif
    {
    TNR_ID tnr_key;
    TNR_TREE_DATA tnr_data;
    INT count, i;
    
    count = btcount (wmv_current.tnr_tree_ptr);
    for (i = 1; count > 0 && i <= count; i++)
        {
        if (i == 1)
            {
            btfirst (wmv_current.tnr_tree_ptr, (VOIDPTR)&tnr_key, (VOIDPTR)&tnr_data);
            }
        else
            {
            btnext (wmv_current.tnr_tree_ptr, (VOIDPTR)&tnr_key, (VOIDPTR)&tnr_data);
            }
        if (tnr_data.wkid == wk_id)
            {
            return(tnr_key);
            }
        }
    return (TNR_ID)0;
    }


#if USE_PROTOTYPES
publicdef INT wm_draw_window_border (WINDOW_ID win_id, LINE_ATTRIBUTE *line)
#else
publicdef INT wm_draw_window_border (win_id, line)
WINDOW_ID win_id;
LINE_ATTRIBUTE *line;
#endif
    {
    WIN_TREE_DATA win_data;
    INT status = SUCCESS;
    INT npoints;
    TNR_ID tnr_id;
    INT trim_seg;
    GKS_INT SGNA;
    GKS_REAL PRIOR;
    static GKS_REAL px[] = 
        {
        0.0, 1.0, 1.0, 0.0, 0.0
        };
    static GKS_REAL py[] = 
        {
        0.0, 0.0, 1.0, 1.0, 0.0
        };
    SEG_TREE_DATA seg_data;
    
    wmwopen (win_id);
    wmtallocate (&tnr_id, 0.0, 1.0, 0.0, 1.0, 0.0, 1.0, 0.0, 1.0);
    wmsallocate (&trim_seg);
    wm_gsln (line->style);
    wm_gsplci (line->color);
    wm_gslwsc (line->width);
    
    PRIOR = GKS_PRIOR (wmv_current.win_num, WM_BORDER_SEG_NUM);
    SGNA = trim_seg;
    wm_gssgp (SGNA, (DOUBLE)PRIOR);
    npoints = 5;
    wm_gpl (npoints, px, py);
    wmsclose (trim_seg);
    wmoset ((WM_ID)trim_seg, WM_CLASS_BORDER, "", WM_CLSTR_BORDER);
    btfind (wmv_current.seg_tree_ptr, (VOIDPTR)&trim_seg, (VOIDPTR)&seg_data);
    seg_data.num = WM_BORDER_SEG_NUM;
    btrepcur (wmv_current.seg_tree_ptr, (VOIDPTR)&trim_seg, (VOIDPTR)&seg_data);
    
    return SUCCESS;
    }

/* Function Description -----------------------------------------------------       */
/*Description:                                                                      */
/*                                                                                  */
/*    Inquiry current workstation pointing device to determine if                   */
/*    it is registered to a tablet puck.                                            */
/*                                                                                  */
/*Prototype:                                                                        */
/*    publicdef BOOL wm_if_tablet ();                                               */
/*                                                                                  */
/*Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)        */
/*    None                                                                          */
/*                                                                                  */
/*Return Value/Status:                                                              */
/*    TRUE    if tablet registered for the current transformation                   */
/*    FALSE   if input is from the workstation mouse                                */
/*                                                                                  */
/*Scope:                                                                            */
/*    PUBLIC                                                                        */
/*                                                                                  */
/*Limitations/Assumptions:                                                          */
/*                                                                                  */
/*-----------------------------------------------------------------------------     */

/* BEGIN:                                                                           */

publicdef BOOL wm_if_tablet ()
    {
    INT status = SUCCESS;
    BOOL return_value = FALSE;
    APPLICATION_ID appCur;
    WINDOW_ID winCur;
    TNR_ID tnrCur, tnr, maintnr;
    GROUP_ID groCur;
    SEGMENT_ID segCur;
    PICK_ID pickCur;
    TNR_TREE_DATA tnr_data;
    
    wmoinqcur (&appCur, &winCur, &tnrCur, &groCur, &segCur, &pickCur);
    if (tnrCur)
        {
        if (btfind (wm_static ()->tnr_tree_ptr, (VOIDPTR)&tnrCur, (VOIDPTR)&tnr_data))
            {
            return_value = ((tnr_data.reg.key_type == WM_PD_TABLET) ? TRUE : FALSE);
            }
        }
    return return_value;
    }

/* === WMCTALLOCATE ===                                                              */

#if USE_PROTOTYPES
publicdef INT wmctallocate (TNR_ID *tnr_id_ptr, DOUBLE vx_ll, DOUBLE vx_ur,
                           DOUBLE vy_ll, DOUBLE vy_ur, DOUBLE wx_ll, DOUBLE wx_ur,
                           DOUBLE wy_ll, DOUBLE wy_ur)
#else
publicdef INT wmctallocate (tnr_id_ptr, vx_ll, vx_ur, vy_ll, vy_ur, wx_ll, wx_ur, wy_ll,
                           wy_ur)
TNR_ID *tnr_id_ptr;
DOUBLE vx_ll, vx_ur, vy_ll, vy_ur;
DOUBLE wx_ll, wx_ur, wy_ll, wy_ur;
#endif
    {
    INT returned_status = SUCCESS;
    
    TNR_TREE_DATA tnr_data;
    
    IF_NOT_WIN_OPEN_RETURN;
    
    /* allow multi tnrs in a segment                                                */
    /*   CLOSE_SEG ;                                                                */
    /*                                                                              */
    
    if ((vx_ll < 0.0) || (vx_ur > 1.0) || (vx_ll >= vx_ur) || (vy_ll < 0.0) ||
        (vy_ur > 1.0) || (vy_ll >= vy_ur))
        {
        returned_status = WM_ERR_BAD_NWC;
        vx_ll = 0.0;
        vx_ur = 1.0;
        vy_ll = 0.0;
        vy_ur = 1.0;
        }
    if ((wx_ll >= wx_ur) || (wy_ll >= wy_ur))
        {
        returned_status = WM_ERR_BAD_WCS;
        wx_ll = 0.0;
        wx_ur = 1.0;
        wy_ll = 0.0;
        wy_ur = 1.0;
        }
    *tnr_id_ptr = next_tnr_id ();
    
    tc_zeroes ((VOIDPTR)&tnr_data, sizeof(TNR_TREE_DATA));
    tnr_data.nwc.min.x = vx_ll;
    tnr_data.nwc.max.x = vx_ur;
    tnr_data.nwc.min.y = vy_ll;
    tnr_data.nwc.max.y = vy_ur;
    
    tnr_data.wcs.min.x = wx_ll;
    tnr_data.wcs.max.x = wx_ur;
    tnr_data.wcs.min.y = wy_ll;
    tnr_data.wcs.max.y = wy_ur;
    
    tnr_data.panx = (wx_ll + ((wx_ur - wx_ll) / 2));
    tnr_data.pany = (wy_ll + ((wy_ur - wy_ll) / 2));
    tnr_data.zoomx = 1.0;
    tnr_data.zoomy = 1.0;
    
    tnr_data.reg.ifset = FALSE;
    tnr_data.wkid = MAIN_WKID;      /* MGKS                                         */
    btadd (wmv_current.tnr_tree_ptr, (VOIDPTR)tnr_id_ptr, (VOIDPTR)&tnr_data);
    
    wmctselect (*tnr_id_ptr);
    
    return returned_status;
    }

/* === WMCTSELECT ===                                                                */

#if USE_PROTOTYPES
publicdef INT wmctselect (TNR_ID tnr_id)
#else
publicdef INT wmctselect (tnr_id)
TNR_ID tnr_id;
#endif
    {
    
    INT returned_status = SUCCESS;
    TNR_TREE_DATA tnr_data;
    INT status;
    
    IF_NOT_WIN_OPEN_RETURN;
    
    /* allow multi tnrs in a segment                                                */
    /*   CLOSE_SEG ;                                                                */
    /*                                                                              */
    
    if (tnr_id)
        {
        status = btfind (wmv_current.tnr_tree_ptr, (VOIDPTR)&tnr_id,
                         (VOIDPTR)&tnr_data);
        if (status)
            {
            zzct_select (tnr_id);
            }
        else
            {
            returned_status = WM_ERR_TNR_NOT_FOUND;
            }
        }
    else
        {
        btfirst (wmv_current.tnr_tree_ptr, (VOIDPTR)&tnr_id, (VOIDPTR)&tnr_data);
        zzct_select (tnr_id);
        }
    return returned_status;
    }

/* === ZZT_SELECT ===                                                               */

#if USE_PROTOTYPES
static VOID zzct_select (TNR_ID tnr_id)
#else
static VOID zzct_select (tnr_id)
TNR_ID tnr_id;
#endif
    {
    TNR_TREE_DATA tnr_data;
    GKS_REAL VX1, VX2, VY1, VY2;
    GKS_REAL WX1, WX2, WY1, WY2;
    GKS_REAL wcs_dx, wcs_dy;
    GKS_REAL ndc_dx, ndc_dy;
    INT status;
    GKS_REAL XO = 0.0, YO = 0.0, DX = 0.0, DY = 0.0, PHI = 0.0, FX = 1.0, FY = 1.0;
    GKS_INT SW = 1;
    GKS_REAL cx, cy;
    RECTANGLE nwcRect, wcsRect;
    PIPE nwc2wcs;
    
    status = btfind (wmv_current.tnr_tree_ptr, (VOIDPTR)&tnr_id, (VOIDPTR)&tnr_data);
    if (status)
        {
        wmv_current.tnr_id = tnr_id;
        
        wmv_current.nwc.min.x = tnr_data.nwc.min.x;
        wmv_current.nwc.max.x = tnr_data.nwc.max.x;
        wmv_current.nwc.min.y = tnr_data.nwc.min.y;
        wmv_current.nwc.max.y = tnr_data.nwc.max.y;
        
        wmv_current.wcs.min.x = tnr_data.wcs.min.x;
        wmv_current.wcs.max.x = tnr_data.wcs.max.x;
        wmv_current.wcs.min.y = tnr_data.wcs.min.y;
        wmv_current.wcs.max.y = tnr_data.wcs.max.y;
        
        wmv_current.panx = tnr_data.panx;
        wmv_current.pany = tnr_data.pany;
        wmv_current.zoomx = tnr_data.zoomx;
        wmv_current.zoomy = tnr_data.zoomy;
        
        wmv_current.reg = tnr_data.reg;
        
        wcs_dx = wmv_current.wcs.max.x - wmv_current.wcs.min.x;
        wcs_dy = wmv_current.wcs.max.y - wmv_current.wcs.min.y;

        ndc_dx = wmv_current.ndc.max.x - wmv_current.ndc.min.x;
        ndc_dy = wmv_current.ndc.max.y - wmv_current.ndc.min.y;

#if 0
        VX1 = wmv_current.ndc.min.x + (wmv_current.nwc.min.x);
        VX2 = wmv_current.ndc.min.x + (wmv_current.nwc.max.x);
        VY1 = wmv_current.ndc.min.y + (wmv_current.nwc.min.y);
        VY2 = wmv_current.ndc.min.y + (wmv_current.nwc.max.y);
#endif

        VX1 = wmv_current.ndc.min.x + (wmv_current.nwc.min.x * ndc_dx);
        VX2 = wmv_current.ndc.min.x + (wmv_current.nwc.max.x * ndc_dx);
        VY1 = wmv_current.ndc.min.y + (wmv_current.nwc.min.y * ndc_dy);
        VY2 = wmv_current.ndc.min.y + (wmv_current.nwc.max.y * ndc_dy);

#if 0 
        /* use entire window for clipping                                           */
        VX1 = wmv_current.ndc.min.x;
        VX2 = wmv_current.ndc.max.x;
        VY1 = wmv_current.ndc.min.y;
        VY2 = wmv_current.ndc.max.y;
#endif

        wm_gsvp (TEMP_TNR, VX1, VX2, VY1, VY2);
        cx = wmv_current.wcs.min.x + ((wmv_current.wcs.max.x - wmv_current.wcs.min.x) /
                                      2);
        cy = wmv_current.wcs.min.y + ((wmv_current.wcs.max.y - wmv_current.wcs.min.y) /
                                      2);
        wcsRect.min.x = wmv_current.panx + ((wmv_current.wcs.min.x - cx) /
                                            wmv_current.zoomx);
        wcsRect.max.x = wmv_current.panx + ((wmv_current.wcs.max.x - cx) /
                                            wmv_current.zoomx);
        wcsRect.min.y = wmv_current.pany + ((wmv_current.wcs.min.y - cy) /
                                            wmv_current.zoomy);
        wcsRect.max.y = wmv_current.pany + ((wmv_current.wcs.max.y - cy) /
                                            wmv_current.zoomy);
        PR_SetPipe (&nwc2wcs, &wmv_current.nwc, &wcsRect);
        /* entire window                                                            */
        nwcRect.min.x = 0.0;
        nwcRect.max.x = 1.0;
        nwcRect.min.y = 0.0;
        nwcRect.max.y = 1.0;
        PR_PipeRect (&nwcRect, &nwc2wcs, &wcsRect);
        WX1 = wcsRect.min.x;
        WX2 = wcsRect.max.x;
        WY1 = wcsRect.min.y;
        WY2 = wcsRect.max.y;
        wm_gswn (TEMP_TNR, WX1, WX2, WY1, WY2);
        
        wm_gselnt (TEMP_TNR);
        
        XO = tnr_data.panx;
        YO = tnr_data.pany;
        DX = (wmv_current.wcs.min.x + (wcs_dx / 2)) - tnr_data.panx;
        DY = (wmv_current.wcs.min.y + (wcs_dy / 2)) - tnr_data.pany;
        PHI = 0.0;
        FX = tnr_data.zoomx;
        FY = tnr_data.zoomy;
        SW = 0;
        wm_gevtm (XO, YO, DX, DY, PHI, FX, FY, SW, wmv_current.TM);
        zzt_set_registration ();
        }
    else
        {
        /* returned_status = WM_ERR_TNR_NOT_FOUND ;                                 */
        }
    }

