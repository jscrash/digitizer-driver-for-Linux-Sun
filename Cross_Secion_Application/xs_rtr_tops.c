/* DEC/CMS REPLACEMENT HISTORY, Element XS_RTR_TOPS.C*/
/*  8     2-MAR-1992 14:44:50 JILL "(SPR 0) thickness for tops line=BOLD"*/
/* *7    27-MAR-1991 13:30:39 KEE "(SPR -1) Cross Section bug fixing and enhancement for Canadian Hunter"*/
/* *6     7-FEB-1991 15:05:45 MING "(SPR 6579) fix parameter passed to lu_text_to_index which should be INT"*/
/* *5     3-DEC-1990 17:43:39 MING "(SPR 5637) take off the check on md"*/
/* *4    28-SEP-1990 11:49:27 GILLESPIE "(SPR 1) Merge beta deltas"*/
/*  1B1  28-SEP-1990 11:11:01 GILLESPIE "Beta Deltas"*/
/* *3    27-AUG-1990 14:10:53 JULIAN "(SPR 0) bug fixes from ernie"*/
/* *2    17-AUG-1990 22:43:27 VINCE "(SPR 5644) Code Cleanup"*/
/* *1    19-JUN-1989 13:45:27 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element XS_RTR_TOPS.C*/
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
#include "esi_xs_defs.h"

#ifndef ESI_WE_WELLS_H
#include "esi_we_wells.h"
#endif

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

#ifndef ESI_SL_H
#include "esi_sl.h"
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

/* File Description ---------------------------------------------------------       */
/*Overview:                                                                         */
/*    This module includes initial and server routines for operating                */
/*    tops.                                                                         */
/*                                                                                  */
/*Prototypes and one line descriptions of each function in the file.                */
/*Public_Functions:                                                                 */
/*    publicdef INT xs_rtr_tops_init(XS_STRUCT *pxs);                               */
/*                                                                                  */
/*    publicdef INT xsz_rtr_wells_tops(XS_STRUCT *pxs, CHAR *top, COLOR_INDEX color, INT line_style,    */
/*        DOUBLE thickness, INT init );                                             */
/*                                                                                  */
/*Private_Functions:                                                                */
/*    static INT xs_rtr_tops_server(INT id, INT item,  INTERNAL_STRUCT *parms );*/
/*                                                                                  */
/*-----------------------------------------------------------------------------     */

/* Function Description -----------------------------------------------------       */
/*Description:                                                                      */
/*    The excutive server routine of xs_rtr_tops.                                   */
/*                                                                                  */
/*Prototype:                                                                        */
/*    static INT xs_rtr_tops_server(INT id, INT item,  INTERNAL_STRUCT *parms );*/
/*                                                                                  */
/*Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)        */
/*    (M) pxs         -(XS_STRUCT *) A pointer points to the XS_STRUCT.             */
/*    (I) id          -(INT) The id of active CDLG.                                 */
/*    (I) item        -(INT) The item picked in the active CDLG.                    */
/*                                                                                  */
/*Return Value/Status:                                                              */
/*    SUCCESS - Successful completion.                                              */
/*    FAIL    - Unsuccessful completion.                                            */
/*                                                                                  */
/*Scope:                                                                            */
/*    PRIVATE to <xs_rtr_tops.c>                                                    */
/*                                                                                  */
/*Limitations/Assumptions:                                                          */
/*                                                                                  */
/*-----------------------------------------------------------------------------     */

/* BEGIN:                                                                           */
#if USE_PROTOTYPES
static INT xs_rtr_tops_server (INT id, INT item, INTERNAL_STRUCT *parms)
#else
static INT xs_rtr_tops_server (id, item, parms)
INTERNAL_STRUCT *parms;
INT id;
INT item;
#endif
    {
    INT status = SUCCESS;
    INT initial;
    INT *items_selected;
    INT total_items = 0, total_exist_tops = 0;
    INT i, j;
    TCP ptr, tops;
    TOP_INFO top_info;
    XS_STRUCT *pxs;
    INT color;
    INT line_style;
    INT thickness_code;
    DOUBLE thickness;
    
    pxs = parms->pxs;
    tops = parms->tops;
    /* create the table of returned items                                           */
    
    total_items = ts_tcp_len (tops);
    items_selected = (INT *) tc_zalloc (sizeof(INT) * (total_items + 1));
    
    /* analyze the item.                                                            */
    
    switch (item)
        {
    case XS_RTR_BTN_RETRIEVE:
        wiqsel (id, XS_RTR_SELECT_LIST, &total_items, items_selected);
        
        /*  get the graphic context for the tops lines                              */
        
        lu_text_to_index ("COLOR", &color, "NORMAL");
        lu_text_to_index ("LINE_STYLE", &line_style, "SOLID");
        lu_text_to_index ("THICKNESS", &thickness_code, "FINE");
	thickness = (DOUBLE) thickness_code;
        
        /*  go througth all selected tops to build up                               */
        /*  the UWI B-tree.                                                         */
        total_exist_tops = llcount (&(pxs->drawing.top_table));
        initial = (total_exist_tops > 0) ? FALSE : TRUE;
        for (i = 0; i < total_items; i++)
            {
            status = xsz_rtr_wells_tops (pxs, tops[items_selected[i] - 1], color,
                                         line_style, thickness, initial);
            initial = FALSE;
            }
        /* retrieve the top values for all                                          */
        /* wells.                                                                   */
        
    case XS_RTR_BTN_CANCEL:
        status = am_release_resource (pxs->retrieve_top_cdlg_id);
        ts_tcp_free (tops);
        
    default:
        wienmni (pxs->tops_menu_id, RETV_TOPS_MENU_ITEM, ON);
        break;
        
        }
    return status;
    
    }
/* END:                                                                             */

/* Function Description -----------------------------------------------------       */
/*Description:                                                                      */
/*    This routine to define the retrieve tops dialog box.                          */
/*                                                                                  */
/*Prototype:                                                                        */
/*    publicdef INT xs_rtr_tops_init(XS_STRUCT *pxs);                               */
/*                                                                                  */
/*Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)        */
/*    (M) pxs         -(XS_STRUCT *) A pointer points to the XS_STRUCT.             */
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
publicdef INT xs_rtr_tops_init (XS_STRUCT *pxs)
#else
publicdef INT xs_rtr_tops_init (pxs)
XS_STRUCT *pxs;
#endif
    {
    INT status = SUCCESS;
    INT id;
    INT num_items = 0;
    static INTERNAL_STRUCT parms;
    
    /* **************************************************************************   */
    
    wienmni (pxs->tops_menu_id, RETV_TOPS_MENU_ITEM, OFF);
    
    /* defines the dialog resource.                                                 */
    parms.pxs = pxs;
    status = am_define_resource (AM_DIALOG, XS_CDLGS_FILE, XS_RETRIEVE_TOPS,
                                 xs_rtr_tops_server, &id, &parms);
    
    pxs->retrieve_top_cdlg_id = id;
    /* go to retrieve the existing tops                                             */
    /* back from the PROJECT.LITHOSTRAT_                                            */
    /*              CODES                                                           */
    
    num_items = llcount (&(pxs->drawing.top_table));
    if (num_items < 0)
        llinit (&(pxs->drawing.top_table), 0, sizeof(TOP_INFO *), LL_LIST_SYSTEM);
    status = xsz_rtr_tops (&(parms.tops));
    if (status)
        return status;
    
    /* set the texts to the selecting box.                                          */
    
    num_items = ts_tcp_len (parms.tops);
    wisselt (id, XS_RTR_SELECT_LIST, XS_RTR_SELECT_BAR, num_items, num_items,
             parms.tops);
    status = wiopcd (id);
    
    return status;
    }
/* END:                                                                             */

/* Function Description -----------------------------------------------------       */
/*Description:                                                                      */
/*    To retrieve top values for all layouts.  Stored as a linked list in           */
/*    LAYOUT_STRUCT.                                                                */
/*                                                                                  */
/*Prototype:                                                                        */
/*    publicdef INT xsz_rtr_wells_tops(XS_STRUCT *pxs, CHAR *top, COLOR_INDEX color, INT line_style,    */
/*        DOUBLE thickness, INT init );                                             */
/*                                                                                  */
/*Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)        */
/*    (M) pxs         -(XS_STRUCT *) A pointer points to the XS_STRUCT.             */
/*    (I) top         -(CHAR *) The name of appended top.                           */
/*    (I) init        -(INT) A flag to decide whether we should initialize the      */
/*                        TOPS linked list in each layout.                          */
/*    color           -(COLOR_INDEX)                                                */
/*    line_style      -(INDEX)                                                      */
/*    thickness       -(DOUBLE)                                                     */
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
publicdef INT xsz_rtr_wells_tops (XS_STRUCT *pxs, CHAR *top, COLOR_INDEX color,
                                  INDEX line_style, DOUBLE thickness, INT init)
#else
publicdef INT xsz_rtr_wells_tops (pxs, top, color, line_style, thickness, init)
XS_STRUCT *pxs;
CHAR *top;
COLOR_INDEX color;
INDEX line_style;
DOUBLE thickness;
INT init;
#endif
    {
    TOP_STRUCT top_struct;
    INT status = SUCCESS;
    INT total_tops = 0, old_top = FALSE, old_uwi = FALSE;
    INT i;
    TCP list_uwis;
    TOP_INFO *top_info;
    /* *************************************************************************    */
    
    tc_zeroes (&top_struct, sizeof top_struct);
    
    /* If the TOP_TABLE has not been                                                */
    /* initialized, initializes it first                                            */
    /* then go througth all uwis to re-                                             */
    /* trieve the top structure.                                                    */
    /* go througth all layouts to fill                                              */
    /* all retrieved top values.                                                    */
    
    if (init)
        {
        llinit (&(pxs->drawing.top_table), 0, sizeof(TOP_INFO *), LL_LIST_SYSTEM);
        }
    else                            /* if the linked list has already been          */
        /* initialized, search for the top.                                         */
        {
        total_tops = llcount (&(pxs->drawing.top_table));
        if (total_tops EQUALS 0)
            old_top = FALSE;
        else
            {
            for (i = 0, llfirst (&(pxs->drawing.top_table), &top_info);
                 i < total_tops AND (!old_top); i++)
                {
                if (!strcmp (top_info->top, top))
                    old_top = TRUE;
                else
                    llnext (&(pxs->drawing.top_table), &top_info);
                }
            }
        }
    /* if it is a new top, creates the                                              */
    /* node and initialize the btree of                                             */
    /* UWIs.                                                                        */
    
    if (!old_top)
        {
        top_info = (TOP_INFO *) tc_zalloc (sizeof(TOP_INFO));
        status = btinit (&(top_info->uwis), 0, sizeof(UWI), sizeof(TOP_STRUCT),
                         BT_KEY_IS_C_STRING | BT_TREE_SYSTEM);
        strcpy (top_info->top, top);
        top_info->line.color = color;
        top_info->line.style = line_style;
        top_info->line.width = thickness;
        status = llappnd (&(pxs->drawing.top_table), &top_info);
        }
    /* convert the select list into TCP.                                            */
    status = sl_make_tcp (&list_uwis, pxs->well_list, 1);
    if (status != SUCCESS)
        return SUCCESS;
    
    /* go througth the uwis to set the                                              */
    /* UWI.                                                                         */
    
    for (i = 0; list_uwis[i] != NULL; i++)
        {
        old_uwi = btfind (&(top_info->uwis), list_uwis[i], &top_struct);
        if (old_uwi)
            {
            status = ls_get_top (list_uwis[i], top, &top_struct);
            if (status >= 0)
                {
                btrepcur (&(top_info->uwis), list_uwis[i], &top_struct);
                }
            }
        else
            {
            status = ls_get_top (list_uwis[i], top, &top_struct);
            if (status < 0)
                {
                tc_zeroes (&top_struct, sizeof top_struct);
                top_struct.Depth.md = HUGE_DEPTH;
                }
            status = btadd (&(top_info->uwis), list_uwis[i], &top_struct);
            }
        }
    tc_free (list_uwis);
    return SUCCESS;
    }
/* END:                                                                             */
