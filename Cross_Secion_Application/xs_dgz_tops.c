/* DEC/CMS REPLACEMENT HISTORY, Element XS_DGZ_TOPS.C*/
/* *4     3-MAY-1991 16:32:03 KEE "(SPR -1) Bug fix when porting MOTIF/PRIORGKS, MOTIF/DECGKS, DECW/PRIORGKS, DECW/DECGKS version"*/
/* *3    17-AUG-1990 22:38:21 VINCE "(SPR 5644) Code Cleanup"*/
/* *2    18-JAN-1990 08:52:28 GILLESPIE "(SPR 6001) Changes for new wells struct stuff"*/
/* *1    19-JUN-1989 13:43:50 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element XS_DGZ_TOPS.C*/
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

#include "esi_c_lib.h"
#include "esi_xs_defs.h"

#ifndef ESI_AM_H

#include "esi_am.h"

#endif

#ifndef ESI_TS_H

#include "esi_ts.h"

#endif

#ifndef ESI_TC_H

#include "esi_tc.h"

#endif

#ifndef ESI_WI_H

#include "esi_wi.h"

#endif

#ifndef ESI_WM_H

#include "esi_wm.h"

#endif

#ifndef ESI_SL_H

#include "esi_sl.h"

#endif

#ifndef ESI_WI_H

#include "esi_wi.h"

#endif

#ifndef ESI_BT_H

#include "esi_bt.h"

#endif

#include "xs_cdlgs.rh"
#include "xs_menus.rh"
typedef struct internal_use
    {
    CHAR **tops;
    XS_STRUCT *pxs;
    } INTERNAL_STRUCT;

/* File Description ---------------------------------------------------------
Overview:
    This module includes initial and server routines for operating
    tops.

Prototypes and one line descriptions of each function in the file.
Public_Functions:
    publicdef INT xs_dgz_tops_server(INT id, INT item, INTERNAL_STRUCT *parms );

    publicdef INT xs_dgz_tops_init(XS_STRUCT *pxs);
    
    publicdef INT xsz_get_top(XS_STRUCT *pxs, CHAR *top, INT color, INT line_style, 
        DOUBLE thickness);

Private_Functions:

-----------------------------------------------------------------------------*/

/* Function Description -----------------------------------------------------
Description:
    The excutive server routine of xs_ggz_tops.

Prototype:
    publicdef INT xs_dgz_tops_server(INT id, INT item, INTERNAL_STRUCT *parms );

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) id          -(INT) The id of active CDLG.
    (I) item        -(INT) The item picked in the active CDLG.
    (M) pxs         -(XS_STRUCT *) A pointer points to the XS_STRUCT.

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT xs_dgz_tops_server (INT id, INT item, INTERNAL_STRUCT *parms)
#else
publicdef INT xs_dgz_tops_server (id, item, parms)
INTERNAL_STRUCT *parms;
INT id;
INT item;
#endif
    {
    INT status = SUCCESS;
    INT selected_list[3], total_tops = 0, value;
    static CHAR selected_top[64];
    XS_STRUCT *pxs;
    CHAR **tops;
    INT not_found, num_items;
    DOUBLE min_char_height;
    
    /* ************************************************************************* */
    
    /* analyze the picked item.          */
    tops = parms->tops;
    pxs = parms->pxs;
    switch (item)
        {
    case XS_DGZ_BTN_DGZ:
        wiqccdi (id, XS_DGZ_SELECT_TOP, &value, selected_top);
        /* check if the selected top is in the selector list */
        not_found = TRUE;
        while (*tops != (CHAR *)0)
            {
            if (strcmp (*tops, selected_top) == 0)
                {
                not_found = FALSE;
                break;
                }
            tops++;
            }
        if (not_found == TRUE)
            {
            status = xsz_get_top (pxs, selected_top,/* name  */
                    (COLOR_INDEX)1,              /* color */
                    (INDEX)1,              /* style */
                    1.0);           /* thick */
            
            }
        if (strlen (selected_top) > 0)
            {
            wmwopen (pxs->drawing.window_id);
            status = xsz_digitize_top (pxs, selected_top);
            wmwclose (pxs->drawing.window_id);
            if (status != CANCEL AND not_found == TRUE)
                {
                status = xsz_make_tops_tcp (pxs, &(parms->tops));
                status = ts_tcp_sort (parms->tops);
                num_items = ts_tcp_len (parms->tops);
                wisselt (id, XS_DGZ_SELECT_LIST, XS_DGZ_SELECT_BAR, num_items, 1,
                         parms->tops);
                }
            }
        break;
        
    case XS_DGZ_BTN_SELECT:
        /* allocates the selected_list for   */
        /* retruned items.           */
        wiqsel (id, XS_DGZ_SELECT_LIST, &total_tops, selected_list);
        if (total_tops > 0)
            {
            strcpy (selected_top, tops[selected_list[0] - 1]);
            wiscdt (id, XS_DGZ_SELECT_TOP, selected_top);
            }
        break;
        
    case XS_DGZ_BTN_CANCEL:
        am_release_resource (pxs->digitize_top_cdlg_id);
        ts_tcp_free (tops);
	xs_get_hdr_min_height(pxs, &min_char_height);
        xsz_display_tops (pxs, min_char_height);
    default:
        wienmni (pxs->tops_menu_id, DIG_TOPS_MENU_ITEM, ON);
        break;
        }
    return status;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    This routine to define the digitize tops dialog box.

Prototype:
    publicdef INT xs_dgz_tops_init(XS_STRUCT *pxs);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (M) pxs         -(XS_STRUCT *) A pointer points to the XS_STRUCT.

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT xs_dgz_tops_init (XS_STRUCT *pxs)
#else
publicdef INT xs_dgz_tops_init (pxs)
XS_STRUCT *pxs;
#endif
    {
    INT status = SUCCESS;
    INT id;
    INT num_items = 0;
    static INTERNAL_STRUCT parms;
    /* ************************************************************************** */
    
    wienmni (pxs->tops_menu_id, DIG_TOPS_MENU_ITEM, OFF);
    /* defines the dialog resource.       */
    
    parms.pxs = pxs;
    status = xsz_make_tops_tcp (pxs, &(parms.tops));
    status = am_define_resource (AM_DIALOG, XS_CDLGS_FILE, XS_TDGZ_TOP_INFO,
                                 xs_tdgz_info_server, &pxs->digitize_top_cdlg_id,
                                 &parms);
    
    
    /* set the texts to the selecting box.*/
    num_items = ts_tcp_len (parms.tops);
    if (num_items EQUALS 0)
        {
        /*      am_message(AM_ALERT, "There is no tops for selecting.");*/
        llinit (&(pxs->drawing.top_table), 0, sizeof(TOP_INFO *), LL_LIST_SYSTEM);
        
        }
    /*  else
        { */
    status = ts_tcp_sort (parms.tops);
    wisselt (pxs->digitize_top_cdlg_id, XS_TDGZ_SELECT_LIST, XS_TDGZ_SELECT_BAR,
             num_items, 1, parms.tops);
    
    wiencdi (pxs->digitize_top_cdlg_id, XS_TDGZ_CHK_COLORS, 1);
    wiscdv (pxs->digitize_top_cdlg_id, XS_TDGZ_CHK_COLORS, 0);
    
    status = wiopcd (pxs->digitize_top_cdlg_id);
    /*  } */
    return status;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:

Prototype:
    publicdef INT xsz_get_top(XS_STRUCT *pxs, CHAR *top, COLOR_INDEX color,
	INDEX line_style, 
        DOUBLE thickness);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    pxs             -(XS_STRUCT *)
    top             -(CHAR *)
    color           -(COLOR_INDEX)
    line_style      -(INDEX)
    thickness       -(DOUBLE)        

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT xsz_get_top (XS_STRUCT *pxs, CHAR *top, COLOR_INDEX color,
	                   INDEX line_style, DOUBLE thickness)
#else
publicdef INT xsz_get_top (pxs, top, color, line_style, thickness)
XS_STRUCT *pxs;
CHAR *top;
COLOR_INDEX color;
INDEX line_style;
DOUBLE thickness;
#endif
    {
    TOP_STRUCT top_struct;
    INT status = SUCCESS;
    INT total_tops = 0, old_top = FALSE, old_uwi = FALSE;
    INT i;
    CHAR **list_uwis;
    INT new_top = TRUE;
    TOP_INFO *top_info;
    /* ************************************************************************* */
    
    tc_zeroes (&top_struct, sizeof top_struct);
    
    /* convert the select list into TCP. */
    status = sl_make_tcp (&list_uwis, pxs->well_list, 1);
    if (status != SUCCESS)
        return SUCCESS;
    
    top_info = (TOP_INFO *)tc_zalloc (sizeof(TOP_INFO));
    status = btinit (&(top_info->uwis), 0, sizeof(UWI), sizeof(TOP_STRUCT),
                     BT_KEY_IS_C_STRING | BT_TREE_SYSTEM);
    strcpy (top_info->top, top);
    top_info->line.color = color;
    top_info->line.style = line_style;
    top_info->line.width = thickness;
    
    new_top = TRUE;
    for (i = 0; list_uwis[i] != NULL; i++)
        {
        status = ls_get_top (list_uwis[i], top, &top_struct);
        if (status < 0)
            {
            tc_zeroes (&top_struct, sizeof top_struct);
            top_struct.Depth.md = HUGE_DEPTH;
            }
        else
            {
            new_top = FALSE;
            }
        status = btadd (&(top_info->uwis), list_uwis[i], &top_struct);
        }
    if (!new_top)
        status = llappnd (&(pxs->drawing.top_table), &top_info);
    else
        tc_free (top_info);
    tc_free (list_uwis);
    return(new_top) ? FAIL : SUCCESS;
    }
/* END:     */
