/* DEC/CMS REPLACEMENT HISTORY, Element GB_HIGHLIGHT.C*/
/* *4    14-AUG-1990 13:26:01 VINCE "(SPR 5644) Header Standardization"*/
/* *3    18-OCT-1989 10:21:11 JULIAN "(SPR 0) mod param passing on nl_inq_nlist_user_struct"*/
/* *2    14-SEP-1989 16:15:15 GORDON "(SPR 100) Gulf/Sun/Unix mods"*/
/* *1    19-JUN-1989 12:51:07 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element GB_HIGHLIGHT.C*/
/*****************************************************************************/
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

/* Include required headers */
#ifndef ESI_GL_DEFS_H

#include "esi_gl_defs.h"

#endif

#include "esi_gb.h"

#ifndef ESI_AM_H

#include "esi_am.h"

#endif

#ifndef ESI_C_LIB_H

#include "esi_c_lib.h"

#endif

#ifndef ESI_CS_H

#include "esi_cs.h"

#endif

#ifndef ESI_MP_H

#include "esi_mp.h"

#endif

#ifndef ESI_GKS_H

#include "esi_gks.h"

#endif

#ifndef ESI_WI_H

#include "esi_wi.h"

#endif

#ifndef ESI_QS_H

#include "esi_qs.h"

#endif

#ifndef ESI_TC_H

#include "esi_tc.h"

#endif

#ifndef ESI_SL_H

#include "esi_sl.h"     /* contains def. of SELECT_STRUCT */

#endif

#ifndef ESI_MG_H

#include "esi_mg.h"

#endif

#ifndef ESI_NL_H

#include "esi_nl.h"

#endif

#ifndef ESI_BT_H

#include "esi_bt.h"

#endif

#ifndef ESI_LU_H

#include "esi_lu.h"

#endif

#ifndef ESI_TS_H

#include "esi_ts.h"

#endif

#include "esi_gb_err.h"

/* Resource file include for dialog box */
#include "hl_hi_cdlg.rh"

/* Constants Global within this source module */
#define ERR_CANCEL 1               /* Operator depressed cancel key */
#define ERR_DONE   2               /* Operator depressed DONE key */
#define CONNECT    999             /* Token used to indicate that */
/* current_symbol choice is to connect */
/* points (e.g., wells) */
/* this is a bandaid, awaiting some other approach to */
/* knowing how many items are in your dialog */
#define NUM_HILITE_CDLG_ITEMS 32 

/* file name of resource file */
#define GB_HILITE_RSRC "HL_HI_CDLG"
#define GB_DO_SEISMIC  FALSE

/***********************************************************************
*                            APPLICATION GLOBAL DATA
********************************************************************** */

static CHAR *colorTcp[] = 
    {
    "RED",
    "GREEN",
    "YELLOW",
    "CYAN",
    "MAGENTA",
    "BLUE",
    "WHITE",
    NULL
    };



/* remember dialog attributes across invocations */
static INT cdlg_nitems = 0;
static INT cdlg_id = 0;
static INT enabled_new[MAX_CD_ITEMS], enabled_old[MAX_CD_ITEMS];
static INT current_color;
static INT current_symbol = 1;
static INT current_size = 1;
static INT current_width = 1;
static INT current_connect = 0;

#if USE_PROTOTYPES
static INT gb_highlight_init (void);
static INT gb_highlight_operate (SL_DATA_TYPE, NLIST_HEADER);
static INT gb_zz_update_enable_controls (SL_DATA_TYPE);
static INT gb_zz_enable_colors (void);
static INT gb_zz_enable_symbols (void);
static INT gb_zz_enable_sizes (void);
static INT gb_zz_enable_widths (void);
static INT gb_zz_dialog_events (SL_DATA_TYPE, NLIST_HEADER, INT);
static INT gbzz_do_highlight (SL_DATA_TYPE, NLIST_HEADER);
#else
static INT gb_highlight_init ();
static INT gb_highlight_operate ();
static INT gb_zz_update_enable_controls ();
static INT gb_zz_enable_colors ();
static INT gb_zz_enable_symbols ();
static INT gb_zz_enable_sizes ();
static INT gb_zz_enable_widths ();
static INT gb_zz_dialog_events ();
static INT gbzz_do_highlight ();
#endif

/* File Description ---------------------------------------------------------
Overview:
    Gb_highlight handles the user interactions to select highlight
    attributes and perform the well, lease, seismic, ... hiliting.
    
    NOTE:

        (1) For the V5 version we went to nlist user structs
            for select lists from GB_SELECT_LIST on V4.
        (2) In V5, pointers may not be handled through the qs_inq()
            facility.  A new scheme is being used.

Prototypes and one line descriptions of each function in the file.
Public_Functions:
    publicdef INT gb_highlight(SL_DATA_TYPE type);
    
    publicdef INT gb_hilite_temp(SL_DATA_TYPE type, NLIST_HEADER hilite_list );

Private_Functions:
    static INT gb_highlight_init(void);

    static INT gb_highlight_operate(SL_DATA_TYPE, NLIST_HEADER);

    static INT gb_zz_update_enable_controls(SL_DATA_TYPE);

    static INT gb_zz_enable_colors(void);

    static INT gb_zz_enable_symbols(void);

    static INT gb_zz_enable_sizes(void);

    static INT gb_zz_enable_widths(void);

    static INT gb_zz_dialog_events(SL_DATA_TYPE, NLIST_HEADER, INT);

    static INT gbzz_do_highlight(SL_DATA_TYPE, NLIST_HEADER);

-----------------------------------------------------------------------------*/

/* Function Description -----------------------------------------------------
Description:
    Global menu selection server entry point.

Prototype:
    publicdef INT gb_highlight(SL_DATA_TYPE type);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    type            -(SL_DATA_TYPE) Data type code ::= {WELLS_DATA_TYPE, SEISMIC_DATA_TYPE, ...}

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT gb_highlight (SL_DATA_TYPE type)
#else
publicdef INT gb_highlight (type)
SL_DATA_TYPE type;
#endif
    {
    INT status = SUCCESS;
    INT defmod, regmod;
    static BOOL first = TRUE;
    
    if (first)
        {
        lu_text_to_index ("COLOR", &current_color, "RED");
        first = FALSE;
        }
    wm_inq_deferral (&defmod, &regmod);
    
    wm_set_deferral (GASAP, GSUPPD);
    
    gb_highlight_init ();
    status = gb_highlight_operate (type, NULL);
    
    wm_update ();
    wm_set_deferral (defmod, regmod);
    
    return status;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    To highlight a temp. list, which is not a global current list.

Prototype:
    publicdef INT gb_hilite_temp(SL_DATA_TYPE type, NLIST_HEADER hilite_list );

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    hilite_list     -(NLIST_HEADER)
    type            -(SL_DATA_TYPE) Data type code ::= {WELLS_DATA_TYPE, SEISMIC_DATA_TYPE, ...}

Return Value/Status:
    None.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT gb_hilite_temp (SL_DATA_TYPE type, NLIST_HEADER hilite_list)
#else
publicdef INT gb_hilite_temp (type, hilite_list)
NLIST_HEADER hilite_list;
SL_DATA_TYPE type;
#endif
    {
    
    current_symbol = CONNECT;
    
    /* above statment added by Joseph H on 11/02/88 in order to turn on CONNECT 
       button for digitiz traverse */
    
    gb_highlight_init ();
    gb_highlight_operate (type, hilite_list);
    
    wm_update ();
    return;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    Initialize server only on first call.

Prototype:
    static INT gb_highlight_init();

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    None.
    
Return Value/Status:
    None.
    
Scope:
    PRIVATE to <gb_highlight.c>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static INT gb_highlight_init ()
#else
static INT gb_highlight_init ()
#endif
    {
    INT junk, i;
    
    /* define dialog. In V4 use the include file esi_am_to_ui.h */
    /* which converts am_ functions to ui_ functions */
    
    am_define_resource (AM_DIALOG, GB_HILITE_RSRC, HILIT_CURR_LIST, NULL_FUNCTION_PTR,
                        &cdlg_id, (BYTE *)0);
    
    /* until there is a wi_inquire */
    cdlg_nitems = NUM_HILITE_CDLG_ITEMS;
    
    /* initialize dialog housekeeping to DISabled */
    
    for (i = 1; i < MAX_CD_ITEMS; i++)
        {
        enabled_new[i] = 0;
        enabled_old[i] = -1;
        }
    /* Clear the dialog message window */
    
    return;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    Gb_highlight_operate manages the single 'Highlight Current List' dialog for
    attribute selection and invocation of the well & seismic hilite modules.

Prototype:
    static INT gb_highlight_operate(SL_DATA_TYPE type, NLIST_HEADER hilite_list );

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    type            -(SL_DATA_TYPE)
    hilite_list     -(NLIST_HEADER)

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PRIVATE to <gb_highlight.c>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static INT gb_highlight_operate (SL_DATA_TYPE type, NLIST_HEADER hilite_list)
#else
static INT gb_highlight_operate (type, hilite_list)
SL_DATA_TYPE type;
NLIST_HEADER hilite_list;
#endif
    {
    INT status = SUCCESS;
    INT id;
    INT item;
    INT class ;
    INT value;
    
    wipush ();
    wismode (WI_MENU_EVENT, WI_EVENT_MODE);
    wismode (WI_DIALOG_EVENT, WI_EVENT_MODE);
    wismode (WI_SELECT_EVENT, WI_EVENT_MODE);
    
    /* open any dialogs that should be up. */
    
    wi_open_dialog (cdlg_id);
    
    /* initialize any event servers that need it. */
    
    /* Ensure that the state of menus and dialogs */
    /* reflects the current state of the data and the */
    /* application.  Enable/disable menu/dialog items as needed. */
    
    while (status == SUCCESS)
        {
        gb_zz_update_enable_controls (type);
        wirqevt (0, &class , &id, &item);
        /*    wi_request_dialog_item (&id, &item, &value, text); */
        status = gb_zz_dialog_events (type, hilite_list, item);
        }
    wi_close_dialog (cdlg_id);
    wipop ();
    return status;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    Gb_zz_update_enable_controls ensures that active menus and dialogs
    reflect the current state of the application and data.

    Menu & dialog items are enabled/disabled as appropriate.
    Menu items are checked/unchecked as approriate.
    Dialog text & selectors are updated as approriate.

Prototype:
    static INT gb_zz_update_enable_controls(SL_DATA_TYPE type );

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    type            -(SL_DATA_TYPE)

Return Value/Status:
    None.
    
Scope:
    PRIVATE to <gb_highlight.c>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static INT gb_zz_update_enable_controls (SL_DATA_TYPE type)
#else
static INT gb_zz_update_enable_controls (type)
SL_DATA_TYPE type;
#endif
    {
    INT i;
    
    /* disable all controls first */
    
    for (i = 1; i < cdlg_nitems; i++)
        {
        enabled_new[i] = FALSE;
        }
    /* cancel and highlight always available */
    
    enabled_new[HI_CANCEL] = TRUE;
    enabled_new[HI_HIGHLIGHT] = TRUE;
    
    /* colors always available */
    gb_zz_enable_colors ();
    
    switch (type)
        {
    case WELLS_DATA_TYPE:
        if (current_symbol == CONNECT)
            gb_zz_enable_widths ();
        else
            gb_zz_enable_sizes ();
        gb_zz_enable_symbols ();
        break;
        
    case SEISMIC_DATA_TYPE:
        current_symbol = CONNECT;
        wi_set_dialog_value (cdlg_id, HI_CONNECT, 1);
        gb_zz_enable_widths ();
        break;
        
        /*  case OTHER_DATA_TYPE: */
        /*      break;          */
        
    default:
        break;
        }
    /* enable or disable any controls which have changed state */
    
    for (i = 1; i < cdlg_nitems; i++)
        {
        if (enabled_new[i] != enabled_old[i])
            {
            enabled_old[i] = enabled_new[i];
            wi_enable_dialog_item (cdlg_id, i, enabled_new[i]);
            }
        }
    return;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:

Prototype:
    static INT gb_zz_enable_colors();

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    None.
    
Return Value/Status:
    None.
    
Scope:
    PRIVATE to <gb_highlight.c>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static INT gb_zz_enable_colors ()
#else
static INT gb_zz_enable_colors ()
#endif
    {
    INT i, offset;
    CHAR colorString[26];
    
    for (i = HI_RED; i <= HI_WHITE; i++)
        {
        enabled_new[i] = TRUE;
        }
    /* set radio button group to reflect current state */
    lu_index_to_text ("COLOR", colorString, current_color);
    
    for (i = 0; colorTcp[i] != NULL; ++i)
        {
        if (ARE_SAME (colorString, colorTcp[i]))
            {
            offset = HI_RED + i;
            break;
            }
        }
    wi_set_dialog_value (cdlg_id, offset, 1);
    return;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:

Prototype:
    static INT gb_zz_enable_symbols();

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    None.
    
Return Value/Status:
    None.
    
Scope:
    PRIVATE to <gb_highlight.c>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static INT gb_zz_enable_symbols ()
#else
static INT gb_zz_enable_symbols ()
#endif
    {
    INT i;
    
    for (i = HI_TRIANGLE; i <= HI_CONNECT; i++)
        {
        enabled_new[i] = TRUE;
        }
    /* set radio button group to reflect current state */
    
    switch (current_symbol)
        {
    case CS_TRIANGLE:
        i = HI_TRIANGLE;
        break;
    case CS_DIAMOND:
        i = HI_DIAMOND;
        break;
    case CS_PENTAGON:
        i = HI_PENTAGON;
        break;
    case CS_HEXAGON:
        i = HI_HEXAGON;
        break;
    case CONNECT:
        i = HI_CONNECT;
        break;
        }
    wi_set_dialog_value (cdlg_id, i, 1);
    
    return;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:

Prototype:
    static INT gb_zz_enable_sizes();

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    None.
    
Return Value/Status:
    None.
    
Scope:
    PRIVATE to <gb_highlight.c>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static INT gb_zz_enable_sizes ()
#else
static INT gb_zz_enable_sizes ()
#endif
    {
    INT i;
    
    for (i = HI_SIZE1; i <= HI_SIZE5; i++)
        {
        enabled_new[i] = TRUE;
        }
    wi_set_dialog_value (cdlg_id, current_size + HI_SIZE1 - 1, 1);
    return;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:

Prototype:
    static INT gb_zz_enable_widths();

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    None.
    
Return Value/Status:
    None.
    
Scope:
    PRIVATE to <gb_highlight.c>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static INT gb_zz_enable_widths ()
#else
static INT gb_zz_enable_widths ()
#endif
    {
    INT i;
    
    for (i = HI_WIDTH1; i <= HI_WIDTH5; i++)
        {
        enabled_new[i] = TRUE;
        }
    wi_set_dialog_value (cdlg_id, current_width + HI_WIDTH1 - 1, 1);
    return;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    Gb_zz_dialog_events services all picks on the main make/hilite dialog.

Prototype:
    static INT gb_zz_dialog_events (SL_DATA_TYPE type, NLIST_HEADER hilite_list, INT item);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    type            -(SL_DATA_TYPE)
    hilite_list     -(NLIST_HEADER)
    item            -(INT) Dialog item picked.

Return Value/Status:
    SUCCESS - Successful completion.
    ERR_CANCEL
    ERR_DONE
    
Scope:
    PRIVATE to <gb_highlight.c>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static INT gb_zz_dialog_events (SL_DATA_TYPE type, NLIST_HEADER hilite_list,
                                    INT item)
#else
static INT gb_zz_dialog_events (type, hilite_list, item)
SL_DATA_TYPE type;
NLIST_HEADER hilite_list;
INT item;
#endif
    {
    INT status;
    
    status = SUCCESS;
    
    switch (item)
        {
        
        /* choose color radio buttons */
        
    case HI_RED:
        lu_text_to_index ("COLOR", &current_color, "RED");
        break;
    case HI_GREEN:
        lu_text_to_index ("COLOR", &current_color, "GREEN");
        break;
    case HI_YELLOW:
        lu_text_to_index ("COLOR", &current_color, "YELLOW");
        break;
    case HI_CYAN:
        lu_text_to_index ("COLOR", &current_color, "CYAN");
        break;
    case HI_MAGENTA:
        lu_text_to_index ("COLOR", &current_color, "MAGENTA");
        break;
    case HI_BLUE:
        lu_text_to_index ("COLOR", &current_color, "BLUE");
        break;
    case HI_WHITE:
        lu_text_to_index ("COLOR", &current_color, "WHITE");
        break;
        
        /* choose symbol radio buttons */
        
    case HI_TRIANGLE:
        current_symbol = CS_TRIANGLE;
        break;
    case HI_DIAMOND:
        current_symbol = CS_DIAMOND;
        break;
    case HI_PENTAGON:
        current_symbol = CS_PENTAGON;
        break;
    case HI_HEXAGON:
        current_symbol = CS_HEXAGON;
        break;
    case HI_CONNECT:
        current_symbol = CONNECT;
        break;
        
        /* choose size radio buttons */
        
    case HI_SIZE1:
        current_size = 1;
        break;
    case HI_SIZE2:
        current_size = 2;
        break;
    case HI_SIZE3:
        current_size = 3;
        break;
    case HI_SIZE4:
        current_size = 4;
        break;
    case HI_SIZE5:
        current_size = 5;
        break;
        
        /* choose width radio buttons */
        
    case HI_WIDTH1:
        current_width = 1;
        break;
    case HI_WIDTH2:
        current_width = 2;
        break;
    case HI_WIDTH3:
        current_width = 3;
        break;
    case HI_WIDTH4:
        current_width = 4;
        break;
    case HI_WIDTH5:
        current_width = 5;
        break;
        
        /* action buttons */
        
    case HI_CANCEL:
        status = ERR_CANCEL;
        break;
    case HI_HIGHLIGHT:
        gbzz_do_highlight (type, hilite_list);
        wm_update ();
        status = ERR_DONE;
        break;
    default:
        break;
        }
    return(status);
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    This function performs the actual highlighting 
    of wells or seismic.

Prototype:
    static INT gbzz_do_highlight (SL_DATA_TYPE type, NLIST_HEADER hilite_list );

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    type            -(SL_DATA_TYPE) The routine is handed 'type' which is an integer
                        flagging either WELLS_DATA_TYPE or SEISMIC_DATA_TYPE.
    hilite_list     -(NLIST_HEADER)

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PRIVATE to <gb_highlight.c>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static INT gbzz_do_highlight (SL_DATA_TYPE type, NLIST_HEADER hilite_list)
#else
static INT gbzz_do_highlight (type, hilite_list)
SL_DATA_TYPE type;
NLIST_HEADER hilite_list;
#endif
    {
    NLIST_HEADER data_nlist;
    SELECT_STRUCT select_list;
    INT len, status;
    INT map_window_id, map_tnr_id;
    CHAR data_list_name[127];
    MAP_STRUCTURE *cur_map_workspace;
    
    status = mp_front_map (&cur_map_workspace);
    if (status < SUCCESS)
        {
        am_message (AM_ALERT, mg_message (GB_NO_MAP));
        return(FAIL);
        }
    map_window_id = cur_map_workspace->map_window_id;
    if (!(map_window_id))
        {
        am_message (AM_ALERT, mg_message (GB_NO_MAP_WINDOW));
        return(FAIL);
        }
    map_tnr_id = cur_map_workspace->map_tnr;
    
    switch (type)
        {
    case WELLS_DATA_TYPE:
        /* use this well list name to read the nlist header for the wells */
        if (hilite_list EQUALS NULL)
            {
            
            /* get the name of the current wells list */
            qs_inq_c (QS_CUR_WELL_LIST, data_list_name, &len);
            if (!(len))
                {
                am_message (AM_ALERT, "No current well list name ");
                return(FAIL);
                }
            status = sl_read_list (type, data_list_name, &data_nlist);
            if (status IS_NOT_EQUAL_TO SUCCESS)
                {
                am_message (AM_ALERT, "No current well nlist ");
                }
            }
        else
            {
            data_nlist = hilite_list;
            }
        /* and now get the user struct (select list) associated with this nlist */
        
        status = nl_inq_nlist_user_struct (data_nlist, sizeof(SELECT_STRUCT),
                                           (VOIDPTR) & select_list, (size_t *) & len);
        if (status IS_NOT_EQUAL_TO SUCCESS)
            {
            am_message (AM_ALERT, "Failed trying to read user struct from nlist");
            return(FAIL);
            }
        /* got it, now fill out the hi-lite info in the struct */
        
        select_list.data_type = type;   /* propagate the type of list */
        
        /* fill in the map window and tnr ids */
        select_list.window_id = map_window_id;
        select_list.transform_id = map_tnr_id;
        
        /* set up and fill the cs group name */
        strcpy (select_list.symbol_set, CS_FILLED_GROUP);   /* name it */
        
        /* set segId in listPtr.hilite.segment.id to background segment */
        status = hl_query_hi_seg_id (&select_list);
        if (status == FAIL)
            {
            status = hl_query_seg_id (&select_list);
            }
        select_list.color = current_color;
        select_list.symbol = current_symbol;
        select_list.size = current_size;
        select_list.width = current_width;
        
        current_connect = (current_symbol == CONNECT) ? TRUE : FALSE;
        select_list.connected_flag = current_connect;
        
        /* finally, do the actual highliting */
        status = hl_hilite_wells (&select_list, data_nlist);
        break;
        
    case SEISMIC_DATA_TYPE:
        
        /* get the name of the current seismic list */
        qs_inq_c (QS_CUR_SEISMIC_LIST, data_list_name, &len);
        if (!(len))
            {
            am_message (AM_ALERT, mg_message (GB_NO_SEIS_LIST));
            return(FAIL);
            }
        status = sl_read_list (type, data_list_name, &data_nlist);
        if (status IS_NOT_EQUAL_TO SUCCESS)
            {
            am_message (AM_ALERT, mg_message (GB_NO_SEIS_NLIST));
            }
        /* and now get the user struct (select list) associated with this nlist */
        
        status = nl_inq_nlist_user_struct (data_nlist, sizeof(SELECT_STRUCT),
                                           (VOIDPTR) & select_list, (size_t *) & len);
        if (status IS_NOT_EQUAL_TO SUCCESS)
            {
            am_message (AM_ALERT, mg_message (GB_READ_USER_STRUCT));
            return(FAIL);
            }
        /* got it, now fill out the hi-lite info in the struct */
        
        select_list.data_type = type;   /* propagate the type of list */
        
        /* fill in the map window and tnr ids */
        select_list.window_id = map_window_id;
        select_list.transform_id = map_tnr_id;
        select_list.color = current_color;
        select_list.width = current_width;
        select_list.connected_flag = TRUE;
        
        status = hl_hilite_seis (&select_list, &data_nlist);
        break;
        
    default:
        break;
        }
    return status;
    }
/* END:     */
