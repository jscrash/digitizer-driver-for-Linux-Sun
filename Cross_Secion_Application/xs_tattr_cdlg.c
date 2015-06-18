/* DEC/CMS REPLACEMENT HISTORY, Element XS_TATTR_CDLG.C*/
/* *4    17-AUG-1990 22:44:07 VINCE "(SPR 5644) Code Cleanup"*/
/* *3    19-SEP-1989 13:40:45 GILLESPIE "(SPR 100) GULF MODS"*/
/* *2    14-SEP-1989 16:24:49 GORDON "(SPR 100) Gulf/Sun/Unix mods"*/
/* *1    19-JUN-1989 13:45:42 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element XS_TATTR_CDLG.C*/
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
#include "esi_am.h"
#include "esi_tc.h"
#include "esi_wi.h"
#include "esi_mg.h"
#include "esi_lu.h"

#ifndef ESI_TC_H
#include "esi_tc.h"
#endif

#ifndef ESI_TS_H
#include "esi_ts.h"
#endif

#include "xs_cdlgs.rh"
#include "xs_menus.rh"

#if USE_PROTOTYPES
static INT xs_traceattr_cdlg_server (INT id, INT item, XS_STRUCT *pxs);
static INT xs_trc_attr_dlg_define (XS_STRUCT *pxs);
#else
static INT xs_traceattr_cdlg_server ();
static INT xs_trc_attr_dlg_define ();
#endif

/* File Description ---------------------------------------------------------
Overview:
    Display dialog to accept trace attributes.

Prototypes and one line descriptions of each function in the file.
Public_Functions:
    publicdef INT xs_traceattr_cdlg_init (XS_STRUCT *pxs);
    
    publicdef INT xs_traceattr_cdlg_term(XS_STRUCT *pxs);

Private_Functions:
    static INT xs_traceattr_cdlg_server(INT id, INT item, XS_STRUCT *pxs);
    
    static INT xs_trc_attr_dlg_define(XS_STRUCT *pxs);

-----------------------------------------------------------------------------*/

/* Function Description -----------------------------------------------------
Description:
    Display dialog to accept trace attributes.

Prototype:
    publicdef INT xs_traceattr_cdlg_init (XS_STRUCT *pxs);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    pxs             -(XS_STRUCT *) Pointer to XSEC struct.

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT xs_traceattr_cdlg_init (XS_STRUCT *pxs)
#else
publicdef INT xs_traceattr_cdlg_init (pxs)
XS_STRUCT *pxs;
#endif
    {
    INT status;
    INT i;
    LP_TRACE_STRUCT *ptrc, *ptrc_data;
    INT ntraces;
    INT id;
    INT numitems;
    INT max_selectable = 1;
    
    status = am_define_resource (AM_DIALOG, XS_CDLGS_FILE, XS_TRACEATTR,
                                 xs_traceattr_cdlg_server, &id, pxs);
    pxs->traceattr_cdlg_id = id;
    
    wienmni (pxs->edit_menu_id, CHANGE_ALL_MENU_ITEM, OFF);
    wienmni (pxs->edit_menu_id, TRACE_ATT_MENU_ITEM, OFF);
    
    /* Disable definition of dialogs which can cause
       conflicts in linked list usage.              */
    
    wienmni (pxs->edit_menu_id, TRACE_SEL_MENU_ITEM, OFF);
    wienmni (pxs->edit_menu_id, TRACK_DEF_MENU_ITEM, OFF);
    
    pxs->traceattr_in_use = ON;
    
    status = llfree (&pxs->ptemp->traces);
    status = llinit (&pxs->ptemp->traces, 0, sizeof(LP_TRACE_STRUCT *),
                     LL_LIST_SYSTEM);
    /* this didnt work */
    /*status = llcopy(&pxs->plo->traces, &pxs->ptemp->traces);*/
    
    ntraces = llcount (&pxs->plo->traces);
    for (i = 1; i <= ntraces; i++)
        {
        ptrc = ALLOCATE(LP_TRACE_STRUCT);
        status = (i == 1) ? llfirst (&pxs->plo->traces,
                                     &ptrc_data) : llnext (&pxs->plo->traces,
                                                           &ptrc_data);
        hoblockmove (ptrc_data, ptrc, sizeof(LP_TRACE_STRUCT));
        status = llappnd (&pxs->ptemp->traces, &ptrc);
        }
    status = xs_trc_attr_dlg_define (pxs);
    
    max_selectable = 2;
    numitems = ts_tcp_len (pxs->trace_name_tcp);
    wisselt (id, XS_TRACEATTR_TRACE, XS_TRACEATTR_VSC, numitems, max_selectable,
             pxs->trace_name_tcp);
    wisselt (id, XS_TRACEATTR_TRACK, XS_TRACEATTR_VSC, numitems, max_selectable,
             pxs->track_number_tcp);
    wisselt (id, XS_TRACEATTR_LINETYPE, XS_TRACEATTR_VSC, numitems, max_selectable,
             pxs->type_tcp);
    wisselt (id, XS_TRACEATTR_LINETHICK, XS_TRACEATTR_VSC, numitems, max_selectable,
             pxs->thickness_tcp);
    wisselt (id, XS_TRACEATTR_COLOR, XS_TRACEATTR_VSC, numitems, max_selectable,
             pxs->color_tcp);
    
    if (numitems < 1)
        wiencdi (id, XS_TRACEATTR_CHANGE, 0);
    else
        wiencdi (id, XS_TRACEATTR_CHANGE, 1);
    
    status = wiopcd (id);
    max_selectable = 1;
    wi_set_selection_limit (id, XS_TRACEATTR_TRACE, max_selectable);
    wi_set_selection_limit (id, XS_TRACEATTR_TRACK, max_selectable);
    wi_set_selection_limit (id, XS_TRACEATTR_LINETYPE, max_selectable);
    wi_set_selection_limit (id, XS_TRACEATTR_LINETHICK, max_selectable);
    wi_set_selection_limit (id, XS_TRACEATTR_COLOR, max_selectable);
    
    return SUCCESS;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    Server to accept trace attributes 

Prototype:
    static INT xs_traceattr_cdlg_server(INT id, INT item, XS_STRUCT *pxs);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    id              -(INT) Resource id.
    item            -(INT) Resource event.
    pxs             -(XS_STRUCT *) Pointer to XSEC struct.

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PRIVATE to <xs_tattr_cdlg.c>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static INT xs_traceattr_cdlg_server (INT id, INT item, XS_STRUCT *pxs)
#else
static INT xs_traceattr_cdlg_server (id, item, pxs)
INT id;
INT item;
XS_STRUCT *pxs;
#endif
    {
    static LP_TRACE_STRUCT *ptraces;
    INT i;
    LP_TRACE_STRUCT *ptrc, *ptrc_data;
    INT ntraces;
    INT count;
    INT unused;
    INT num_selected;
    INT status;
    INT items[1];
    INT numitems;
    INT max_selectable = 1;
    INT in_thickness;
    INDEX out_style;
    INDEX out_thick;
    COLOR_INDEX  out_color;    /* GKS codes from LUP_GET_GR... */
    CHAR str_style[32], str_thick[32], str_color[32];   /* Strings from LUP_GET  */
    
    switch (item)
        {
        
    case XS_TRACEATTR_ACCEPT:
        /* copy temp stuff from temp to permanent in the data structure*/
        ntraces = llcount (&pxs->plo->traces);
        for (i = 1; i <= ntraces; i++)
            {
            status = llgetn (&pxs->plo->traces, &ptrc, i);
            tc_free (ptrc);
            }
        status = llfree (&pxs->plo->traces);
        status = llinit (&pxs->plo->traces, 0, sizeof(LP_TRACE_STRUCT *),
                         LL_LIST_SYSTEM);
        /*  status = llcopy(&pxs->ptemp->traces, &pxs->plo->traces);*/
        
        ntraces = llcount (&pxs->ptemp->traces);
        for (i = 1; i <= ntraces; i++)
            {
            ptrc = ALLOCATE(LP_TRACE_STRUCT);
            status = (i == 1) ? llfirst (&pxs->ptemp->traces,
                                         &ptrc_data) : llnext (&pxs->ptemp->traces,
                                                               &ptrc_data);
            hoblockmove (ptrc_data, ptrc, sizeof(LP_TRACE_STRUCT));
            status = llappnd (&pxs->plo->traces, &ptrc);
            }
        status = xs_traceattr_cdlg_term (pxs);
        break;
        
    case XS_TRACEATTR_IGNORE:
        ntraces = llcount (&pxs->ptemp->traces);
        for (i = 1; i <= ntraces; i++)
            {
            status = llgetn (&pxs->ptemp->traces, &ptrc, i);
            tc_free (ptrc);
            }
        status = llfree (&pxs->ptemp->traces);
        status = xs_traceattr_cdlg_term (pxs);
        break;
        
    case XS_TRACEATTR_CHANGE:
        wiqsel (id, XS_TRACEATTR_TRACE, &num_selected, items);
        if (num_selected != 1)
            {
            am_message (AM_ALERT, mg_message (XS_SELECT_TRACE));
            break;
            }
        if (items[0] > 0)
            {
            status = llgetn (&pxs->ptemp->traces, &ptraces, items[0]);
            in_thickness = (INT)ptraces->line.width;
            status = lup_get_graphic_attributes (ptraces->name, in_thickness,
                                                 ptraces->line.style, 
                                                 ptraces->line.color,
                                                 &out_thick, &out_style, &out_color,
                                                 str_thick, str_style, str_color);
            switch (status)
                {
            case SUCCESS:
                ptraces->line.style = out_style;
                ptraces->line.width = (DOUBLE)out_thick;
                ptraces->line.color = out_color;
                
                status = xs_trc_attr_dlg_define (pxs);
                
                numitems = ts_tcp_len (pxs->trace_name_tcp);
                max_selectable = 2;
                wisselt (id, XS_TRACEATTR_TRACE, XS_TRACEATTR_VSC, numitems,
                         max_selectable, pxs->trace_name_tcp);
                wisselt (id, XS_TRACEATTR_TRACK, XS_TRACEATTR_VSC, numitems,
                         max_selectable, pxs->track_number_tcp);
                wisselt (id, XS_TRACEATTR_LINETYPE, XS_TRACEATTR_VSC, numitems,
                         max_selectable, pxs->type_tcp);
                wisselt (id, XS_TRACEATTR_LINETHICK, XS_TRACEATTR_VSC, numitems,
                         max_selectable, pxs->thickness_tcp);
                wisselt (id, XS_TRACEATTR_COLOR, XS_TRACEATTR_VSC, numitems,
                         max_selectable, pxs->color_tcp);
                
                max_selectable = 1;
                wi_set_selection_limit (id, XS_TRACEATTR_TRACE, max_selectable);
                wi_set_selection_limit (id, XS_TRACEATTR_TRACK, max_selectable);
                wi_set_selection_limit (id, XS_TRACEATTR_LINETYPE, max_selectable);
                wi_set_selection_limit (id, XS_TRACEATTR_LINETHICK, max_selectable);
                wi_set_selection_limit (id, XS_TRACEATTR_COLOR, max_selectable);
                break;
            case FAIL:
                am_message (AM_ALERT, mg_message (XS_BAD_ATTR_LOOKUP));
            case CANCEL:
                break;
            default:
                break;
                }
            }
        break;
        
    default:
        break;
        
        }
    return SUCCESS;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    Finish up trace attribute selection.

Prototype:
    publicdef INT xs_traceattr_cdlg_term(XS_STRUCT *pxs);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    pxs             -(XS_STRUCT *) Pointer to XSEC struct.

Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT xs_traceattr_cdlg_term (XS_STRUCT *pxs)
#else
publicdef INT xs_traceattr_cdlg_term (pxs)
XS_STRUCT *pxs;
#endif
    {
    INT status;
    
    status = ts_tcp_free (pxs->trace_name_tcp);
    status = ts_tcp_free (pxs->track_number_tcp);
    status = ts_tcp_free (pxs->type_tcp);
    status = ts_tcp_free (pxs->thickness_tcp);
    status = ts_tcp_free (pxs->color_tcp);
    pxs->trace_name_tcp = (CHAR **)0;
    pxs->track_number_tcp = (CHAR **)0;
    pxs->type_tcp = (CHAR **)0;
    pxs->thickness_tcp = (CHAR **)0;
    pxs->color_tcp = (CHAR **)0;
    pxs->traceattr_in_use = OFF;
    am_release_resource (pxs->traceattr_cdlg_id);
    /* temp fix up while shading and annotation are out of order */
    if (pxs->review_all_switch == ON)
        /*status = xs_shading_cdlg_init( pxs )*/
            ;
    else
        {
        wienmni (pxs->edit_menu_id, TRACE_ATT_MENU_ITEM, ON);
        wienmni (pxs->edit_menu_id, TRACE_SEL_MENU_ITEM, ON);
        wienmni (pxs->edit_menu_id, TRACK_DEF_MENU_ITEM, ON);
        wienmni (pxs->edit_menu_id, CHANGE_ALL_MENU_ITEM, ON);
        }
    /* for now assume at end of review all...... enable menu item*/
    if (pxs->review_all_switch == ON)
        {
        wienmni (pxs->edit_menu_id, CHANGE_ALL_MENU_ITEM, ON);
        status = xs_stop_chg_all (pxs);
        }
    /* end the *for now* section */
    
    return SUCCESS;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    To retrieve from the trace structure the data requi-  
    red to fill in the trace attribute dialog box.  The   
    trace name, track no., line type, line thickness,     
    color are retrieved from the LP_TRACE structures and  
    converted to strings. The line type, thickness, color 
    are stored as integers in the structure, so the act-  
    ual string descriptors must be retrieved from the OR- 
    ACLE table.                                           

Prototype:
    static INT xs_trc_attr_dlg_define(XS_STRUCT *pxs);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) pxs         -(XS_STRUCT *) Log_plot LISTHEAD structure   (must contain 
                        complete trace information), addresses 
                        of ptrs to tcps.

Return Value / Status :
    
    SUCCESS-Successful completion
    FAIL-Unsuccessful completion

Scope :
    PRIVATE to <xs_tattr_cdlg.c >

Limitations / Assumptions :
-----------------------------------------------------------------------------*/ 

/* BEGIN:   */
#if USE_PROTOTYPES
static INT xs_trc_attr_dlg_define (XS_STRUCT *pxs)
#else
static INT xs_trc_attr_dlg_define (pxs)
XS_STRUCT *pxs;
#endif
    {
    LP_TRACE_STRUCT *p_trace;
    INT j;
    INT no_traces;
    INT string_len = 31;
    INT nchars;
    CHAR text[81];
    CHAR name[21];
    INT status;
    SHORT flag;
    INT int_thickness;
    
    no_traces = llcount (&pxs->ptemp->traces);
    flag = llfirst (&pxs->ptemp->traces, &p_trace);
    
    /* set returned pointers= to start of tcps and allocate memory */
    
    if (pxs->trace_name_tcp != (CHAR **)0)
        status = ts_tcp_free (pxs->trace_name_tcp);
    if (pxs->track_number_tcp != (CHAR **)0)
        status = ts_tcp_free (pxs->track_number_tcp);
    if (pxs->type_tcp != (CHAR **)0)
        status = ts_tcp_free (pxs->type_tcp);
    if (pxs->thickness_tcp != (CHAR **)0)
        status = ts_tcp_free (pxs->thickness_tcp);
    if (pxs->color_tcp != (CHAR **)0)
        status = ts_tcp_free (pxs->color_tcp);
    
    pxs->trace_name_tcp = (CHAR **)ts_tcp_alloc (no_traces);
    pxs->track_number_tcp = (CHAR **)ts_tcp_alloc (no_traces);
    pxs->type_tcp = (CHAR **)ts_tcp_alloc (no_traces);
    pxs->thickness_tcp = (CHAR **)ts_tcp_alloc (no_traces);
    pxs->color_tcp = (CHAR **)ts_tcp_alloc (no_traces);
    
    for (j = 0; j < no_traces; j++) /* loop on traces, get data    */
        {
        /* allocate trace string space */
        pxs->trace_name_tcp[j] = STRING_ALLOCATE(p_trace->name);
        strcpy (pxs->trace_name_tcp[j], p_trace->name);
        
        nchars = sprintf (text, "%d", p_trace->track_num);
        pxs->track_number_tcp[j] = (CHAR *)tc_zalloc (sizeof(CHAR) * (nchars + 1));
        strcpy (pxs->track_number_tcp[j], text);
        
        int_thickness = (INT)p_trace->line.width;
        /* lookup line style in table  */
        status = lu_index_to_text ("LINE_STYLE", name, p_trace->line.style);
        pxs->type_tcp[j] = STRING_ALLOCATE(name);
        strcpy (pxs->type_tcp[j], name);
        /* lookup thick. string in tab.*/
        status = lu_index_to_text ("THICKNESS", name, int_thickness);
        pxs->thickness_tcp[j] = STRING_ALLOCATE(name);
        strcpy (pxs->thickness_tcp[j], name);
        /* lookup color string in table*/
        status = lu_index_to_text ("COLOR", name, p_trace->line.color);
        pxs->color_tcp[j] = STRING_ALLOCATE(name);
        strcpy (pxs->color_tcp[j], name);
        
        llnext (&pxs->ptemp->traces, &p_trace);
        }
    /*end of trak loop             */
    
    return(SUCCESS);
    }
/* END:     */
