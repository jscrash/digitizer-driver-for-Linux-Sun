/* DEC/CMS REPLACEMENT HISTORY, Element XS_RTR_LYT.C*/
/* *3    15-OCT-1990 15:19:30 GILLESPIE "(SPR 1000) Merge Ernie deltas"*/
/*  1B1  12-OCT-1990 18:13:32 GILLESPIE "Merge Ernie Deltas"*/
/* *2    17-AUG-1990 22:43:19 VINCE "(SPR 5644) Code Cleanup"*/
/* *1    19-JUN-1989 13:45:24 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element XS_RTR_LYT.C*/
/******************************************************************************     */
/*                                                                                  */
/*                Copyright Exploration Systems, Inc. 1989                          */
/*         Unpublished -- All rights reserved                                       */
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


#include "esi_xs_defs.h"

#ifndef ESI_AM_H
#include "esi_am.h"
#endif

#ifndef ESI_WI_H
#include "esi_wi.h"
#endif

#ifndef ESI_QS_H
#include "esi_qs.h"
#endif

#ifndef ESI_MG_H
#include "esi_mg.h"
#endif

#ifndef ESI_TS_H
#include "esi_ts.h"
#endif

#ifndef ESI_TC_H
#include "esi_tc.h"
#endif

#ifndef ESI_BT_H
#include "esi_bt.h"
#endif

#ifndef ESI_LL_H
#include "esi_ll.h"
#endif

#ifndef XS_CDLGS_RH
#include "xs_cdlgs.rh"
#endif

#ifndef XS_MENUS_RH
#include "xs_menus.rh"
#endif

#if USE_PROTOTYPES
static INT xs_retlayout_cdlg_server (INT id, INT item, XS_STRUCT *pxs);
#else
static INT xs_retlayout_cdlg_server ();
#endif

/* File Description ---------------------------------------------------------       */
/*Overview:                                                                         */
/*                                                                                  */
/*Prototypes and one line descriptions of each function in the file.                */
/*Public_Functions:                                                                 */
/*    publicdef INT xs_retlayout_cdlg_init(XS_STRUCT *pxs);                         */
/*                                                                                  */
/*    publicdef INT xsz_free_top_table(XS_STRUCT *pxs);                             */
/*                                                                                  */
/*Private_Functions:                                                                */
/*    static INT xs_retlayout_cdlg_server(INT id, INT item, XS_STRUCT *pxs);    */
/*                                                                                  */
/*-----------------------------------------------------------------------------     */

/* Function Description -----------------------------------------------------       */
/*Description:                                                                      */
/*    Open dialog allowing user to retrieve a different layout.                     */
/*                                                                                  */
/*Prototype:                                                                        */
/*    publicdef INT xs_retlayout_cdlg_init(XS_STRUCT *pxs);                         */
/*                                                                                  */
/*Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)        */
/*    pxs             -(XS_STRUCT *) Pointer to XS_STRUCT.                          */
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
publicdef INT xs_retlayout_cdlg_init (XS_STRUCT *pxs)
#else
publicdef INT xs_retlayout_cdlg_init (pxs)
XS_STRUCT *pxs;
#endif
    {
    static INT max_selectable = 1;
    INT status;
    INT id;
    INT num_items;
    
    status = am_define_resource (AM_DIALOG, XS_CDLGS_FILE, XS_RETLAYOUT,
                                 xs_retlayout_cdlg_server, &id, pxs);
    
    wiencdi (id, XS_RETLAYOUT_CANCEL, ON);  /* (for second time thru.)              */
    pxs->ret_layout_cdlg_id = id;   /* Main disables cancel 1st time                */
    
    /*  Deactivate conflicting menu items.                                          */
    wienmni (pxs->layout_menu_id, NEW_MENU_ITEM, OFF);
    wienmni (pxs->layout_menu_id, RETRIEVE_MENU_ITEM, OFF);
    wienmni (pxs->layout_menu_id, DELETE_MENU_ITEM, OFF);
    
    wiscdv (id, XS_RETLAYOUT_CURRPROJ, ON);
    wiscdv (id, XS_RETLAYOUT_CURRACNT, ON);
    
    /* get the most restrictive list to start                                       */
    
    pxs->search_project = OFF;
    pxs->search_accounts = OFF;
    
    status = xs_gen_layout_list (pxs);
    
    num_items = ts_tcp_len (pxs->layout_name_tcp);
    /* set up the three tcp's and open dialog                                       */
    
    wisselt (id, XS_RETLAYOUT_NAME, XS_RETLAYOUT_VSCROLL, num_items, max_selectable,
             pxs->layout_name_tcp);
    wisselt (id, XS_RETLAYOUT_PROJECT, XS_RETLAYOUT_VSCROLL, num_items, 0,
             pxs->project_name_tcp);
    wisselt (id, XS_RETLAYOUT_ACCOUNT, XS_RETLAYOUT_VSCROLL, num_items, 0,
             pxs->analyst_name_tcp);
    
    if (num_items < 1)
        {
        wiencdi (id, XS_RETLAYOUT_RETRIEVE, 0);
        wiscdt (id, XS_RETLAYOUT_CHOOSE, mg_message (XS_SET_LIST_LIMITS));
        }
    else
        {
        wiencdi (id, XS_RETLAYOUT_RETRIEVE, 1);
        wiscdt (id, XS_RETLAYOUT_CHOOSE, mg_message (XS_PRESS_RETRIEVE));
        }
    status = wiopcd (id);
    
    return SUCCESS;
    
    }
/* END:                                                                             */

/* Function Description -----------------------------------------------------       */
/*Description:                                                                      */
/*    Server will allow user to retrieve a layout from a list.                      */
/*                                                                                  */
/*Prototype:                                                                        */
/*    static INT xs_retlayout_cdlg_server(INT id, INT item, XS_STRUCT *pxs);    */
/*                                                                                  */
/*Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)        */
/*    id              -(INT) Resource id.                                           */
/*    item            -(INT) Event number.                                          */
/*    pxs             -(XS_STRUCT *) Pointer to XSEC struct.                        */
/*                                                                                  */
/*Return Value/Status:                                                              */
/*    SUCCESS - Successful completion.                                              */
/*                                                                                  */
/*Scope:                                                                            */
/*    PRIVATE to <xs_rtr_lyt.c>                                                     */
/*                                                                                  */
/*Limitations/Assumptions:                                                          */
/*                                                                                  */
/*-----------------------------------------------------------------------------     */

/* BEGIN:                                                                           */
#if USE_PROTOTYPES
static INT xs_retlayout_cdlg_server (INT id, INT item, XS_STRUCT *pxs)
#else
static INT xs_retlayout_cdlg_server (id, item, pxs)
INT id;
INT item;
XS_STRUCT *pxs;
#endif
    {
    static INT max_selectable = 1;
    INT status;
    INT num_items;
    INT items[1];
    CHAR buffer[81];
    ANALYST_NAME analyst_name;
    PROJECT_NAME project_name;
    CHAR layout_name[41];
    INT index;
    
    switch (item)
        {
        
    case XS_RETLAYOUT_RETRIEVE:
        /* determine which layout is selected on the panel (if any),                */
        /* pass that name to the retrieve_layout routine                            */
        
        wiqsel (id, XS_RETLAYOUT_NAME, &num_items, items);
        
        if (num_items != 1)
            {
            am_message (AM_ALERT, mg_message (XS_SELECT_LAYOUT_NAME));
            break;
            }
        else
            {
            index = items[0] - 1;
            strcpy (layout_name, pxs->layout_name_tcp[index]);
            strcpy (project_name, pxs->project_name_tcp[index]);
            strcpy (analyst_name, pxs->analyst_name_tcp[index]);
            sprintf (buffer, "%s  %s", mg_message (XS_RETRIEVING_LAYOUT), layout_name);
            wiscdt (id, XS_RETLAYOUT_CHOOSE, buffer);
            
            /* Free the tops table belongs to                                       */
            /*the previous layout                                                   */
            status = xsz_free_top_table (pxs);
            status = xs_get_layout (pxs, layout_name, project_name, analyst_name);
            if (status == SUCCESS)
                {
                strcpy (pxs->layout_name, layout_name);
                sprintf (buffer, "%s  %s", mg_message (XS_LAYOUT_IS), layout_name);
                wiscdt (pxs->msg_cdlg_id, XS_MESSAGE2, buffer);
                pxs->changed_since_save = FALSE;
                
                qs_set_c (QS_CUR_XS_LAYOUT, layout_name, 0);
                }
            else
                {
                /* (status != SUCCESS)                                              */
                /* warn user. shouldnt happen except: 1) user screwed with table    */
                am_message (AM_ALERT, mg_message (XS_RETV_LAYOUT_FAILED));
                }
            /* we are through so free them                                          */
            status = ts_tcp_free (pxs->layout_name_tcp);
            status = ts_tcp_free (pxs->project_name_tcp);
            status = ts_tcp_free (pxs->analyst_name_tcp);
            pxs->layout_name_tcp = (CHAR **)NULL;
            pxs->project_name_tcp = (CHAR **)NULL;
            pxs->analyst_name_tcp = (CHAR **)NULL;
            
            status = am_release_resource (id);
            /*  Activate conflicting menu items.                                    */
            wienmni (pxs->layout_menu_id, NEW_MENU_ITEM, ON);
            wienmni (pxs->layout_menu_id, RETRIEVE_MENU_ITEM, ON);
            wienmni (pxs->layout_menu_id, DELETE_MENU_ITEM, ON);
            pxs->changed_since_display = TRUE;
            status = xs_cs_menu_mgr (pxs);
            }
        break;
        
    case XS_RETLAYOUT_CANCEL:
        /* we are through so free them                                              */
        status = ts_tcp_free (pxs->layout_name_tcp);
        status = ts_tcp_free (pxs->project_name_tcp);
        status = ts_tcp_free (pxs->analyst_name_tcp);
        pxs->layout_name_tcp = (CHAR **)NULL;
        pxs->project_name_tcp = (CHAR **)NULL;
        pxs->analyst_name_tcp = (CHAR **)NULL;
        
        status = am_release_resource (id);
        /*  Activate conflicting menu items.                                        */
        wienmni (pxs->layout_menu_id, NEW_MENU_ITEM, ON);
        wienmni (pxs->layout_menu_id, RETRIEVE_MENU_ITEM, ON);
        wienmni (pxs->layout_menu_id, DELETE_MENU_ITEM, ON);
        break;
        
    case XS_RETLAYOUT_SELECT:
        /* user possibly changed buttons of who to retrieve                         */
        /* now wants us to go get it for display                                    */
        
        status = ts_tcp_free (pxs->layout_name_tcp);
        status = ts_tcp_free (pxs->project_name_tcp);
        status = ts_tcp_free (pxs->analyst_name_tcp);
        status = xs_gen_layout_list (pxs);
        
        /* modify dialog and continue                                               */
        num_items = ts_tcp_len (pxs->layout_name_tcp);
        
        wisselt (id, XS_RETLAYOUT_NAME, XS_RETLAYOUT_VSCROLL, num_items,
                 max_selectable, pxs->layout_name_tcp);
        wisselt (id, XS_RETLAYOUT_PROJECT, XS_RETLAYOUT_VSCROLL, num_items,
                 max_selectable, pxs->project_name_tcp);
        wisselt (id, XS_RETLAYOUT_ACCOUNT, XS_RETLAYOUT_VSCROLL, num_items,
                 max_selectable, pxs->analyst_name_tcp);
        
        if (num_items < 1)
            {
            wiencdi (id, XS_RETLAYOUT_RETRIEVE, 0);
            wiscdt (id, XS_RETLAYOUT_CHOOSE, mg_message (XS_SET_LIST_LIMITS));
            }
        else
            {
            wiencdi (id, XS_RETLAYOUT_RETRIEVE, 1);
            wiscdt (id, XS_RETLAYOUT_CHOOSE, mg_message (XS_PRESS_RETRIEVE));
            }
        break;
        
    case XS_RETLAYOUT_ALLPROJ:
        /* user switched to all projects list.                                      */
        pxs->search_project = ON;
        break;
        
    case XS_RETLAYOUT_CURRPROJ:
        /* user switched to current projects list.                                  */
        pxs->search_project = OFF;
        break;
        
    case XS_RETLAYOUT_ALLACNTS:
        /* user switched to all accounts list.                                      */
        pxs->search_accounts = ON;
        break;
        
    case XS_RETLAYOUT_CURRACNT:
        /* user switched to curr account list.                                      */
        pxs->search_accounts = OFF;
        break;
        
    default:
        break;
        
        }
    return SUCCESS;
    
    }
/* END:                                                                             */

/* Function Description -----------------------------------------------------       */
/*Description:                                                                      */
/*  Before retrieving the new layout, if there are some tops of                     */
/*    previous layout, we should free them first.                                   */
/*                                                                                  */
/*Prototype:                                                                        */
/*    publicdef INT xsz_free_top_table(XS_STRUCT *pxs);                             */
/*                                                                                  */
/*Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)        */
/*    (I) pxs         -(XS_STRUCT *) The pointer of cross section structure.        */
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
publicdef INT xsz_free_top_table (XS_STRUCT *pxs)
#else
publicdef INT xsz_free_top_table (pxs)
XS_STRUCT *pxs;
#endif
    {
    INT count = 0, i;
    INT status;
    TOP_INFO *ptinfo;               /* contains btree of tops keyed by UWI          */
    
    /* If there is no top table list,                                               */
    /*or the pointer is NULL return.                                                */
    
    count = llcount (&pxs->drawing.top_table);
    
    if (count < 0)
        {
        return SUCCESS;
        }
    /* loop on tops...draw all ones marked visible.                                 */
    for (i = 0; i < count; i++)
        {
        status = (i == 0) ? llfirst (&pxs->drawing.top_table,
                                     &ptinfo) : llnext (&pxs->drawing.top_table,
                                                        &ptinfo);
        
        /* is this list any good?                                                   */
        if (status < 0 OR (status == FAIL))
            {
            return FAIL;
            }
        btfree (&ptinfo->uwis);
        tc_free (ptinfo);
        }
    llfree (&pxs->drawing.top_table);
    return SUCCESS;
    
    }
/* END:                                                                             */
