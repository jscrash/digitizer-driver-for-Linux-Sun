/* DEC/CMS REPLACEMENT HISTORY, Element XS_TSEL_CDLG.C*/
/* *8    21-JUN-1991 11:08:28 DOUG "(SPR 0) (Jantina) fixed trace unit flag checking"*/
/* *7    21-MAY-1991 15:29:43 KEE "(SPR 2578) fix crash in preferences when add new trace type before clicking on an existing one"*/
/* *6    28-DEC-1990 10:08:36 MING "(SPR 6434) add checking on trace unit"*/
/* *5    26-OCT-1990 15:35:46 GILLESPIE "(SPR 34) Incorporate Van Zandwyk/Gulf trace unit deltas"*/
/* *4    17-AUG-1990 22:46:08 VINCE "(SPR 5644) Code Cleanup"*/
/* *3    25-JUN-1990 18:39:45 MING "(SPR 0) fix bug: change LINE_TYPE to LINE_STYLE when calling lu_text_to_index"*/
/* *2    14-SEP-1989 16:25:20 GORDON "(SPR 100) Gulf/Sun/Unix mods"*/
/* *1    19-JUN-1989 13:46:00 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element XS_TSEL_CDLG.C*/
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

#ifndef ESI_AM_H
#include "esi_am.h"
#endif

#ifndef ESI_WI_H
#include "esi_wi.h"
#endif

#ifndef ESI_HO_H
#include "esi_ho.h"
#endif

#ifndef ESI_LL_H
#include "esi_ll.h"
#endif

#ifndef ESI_TC_H
#include "esi_tc.h"
#endif

#ifndef ESI_TS_H
#include "esi_ts.h"
#endif

#ifndef ESI_TU_H
#include "esi_tu.h"
#endif

#ifndef ESI_MG_H
#include "esi_mg.h"
#endif

#ifndef ESI_LU_H
#include "esi_lu.h"
#endif

#include "xs_cdlgs.rh"
#include "xs_menus.rh"

#if USE_PROTOTYPES
static INT xs_tracesel_cdlg_server (INT id, INT item, XS_STRUCT *pxs);
static INT xs_trc_sel_dlg_define (XS_STRUCT *pxs);
#else
static INT xs_tracesel_cdlg_server ();
static INT xs_trc_sel_dlg_define ();
#endif

/* File Description ---------------------------------------------------------       */
/*Overview:                                                                         */
/*                                                                                  */
/*Prototypes and one line descriptions of each function in the file.                */
/*Public_Functions:                                                                 */
/*    publicdef INT xs_tracesel_cdlg_init(XS_STRUCT *pxs);                          */
/*                                                                                  */
/*    publicdef INT xs_tracesel_cdlg_term(XS_STRUCT *pxs);                          */
/*                                                                                  */
/*    publicdef INT xsz_check_min_max_for_log(LAYOUT_STRUCT *p_layout );            */
/*                                                                                  */
/*Private_Functions:                                                                */
/*    static INT xs_tracesel_cdlg_server(INT id, INT item, XS_STRUCT *pxs);     */
/*                                                                                  */
/*    static INT xs_trc_sel_dlg_define(XS_STRUCT *pxs);                         */
/*                                                                                  */
/*-----------------------------------------------------------------------------     */

/* Function Description -----------------------------------------------------       */
/*Description:                                                                      */
/*    Display dialog to select traces.                                              */
/*                                                                                  */
/*Prototype:                                                                        */
/*    publicdef INT xs_tracesel_cdlg_init(XS_STRUCT *pxs);                          */
/*                                                                                  */
/*Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)        */
/*    pxs             -(XS_STRUCT *) Pointer to XSEC struct.                        */
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
publicdef INT xs_tracesel_cdlg_init (XS_STRUCT *pxs)
#else
publicdef INT xs_tracesel_cdlg_init (pxs)
XS_STRUCT *pxs;
#endif
    {
    INT status;
    INT id;
    LP_TRACE_STRUCT *ptrc, *ptrc_data;
    TRACK_STRUCT *ptrk, *ptrk_data;
    INT i;
    INT ntracks, ntraces;
    INT num_items;
    INT max_selectable = 1;
    
    pxs->xst_tracesel_no_redraw = FALSE;
    
    status = am_define_resource (AM_DIALOG, XS_CDLGS_FILE, XS_TRACESEL,
                                 xs_tracesel_cdlg_server, &id, pxs);
    pxs->tracesel_cdlg_id = id;
    
    wienmni (pxs->edit_menu_id, CHANGE_ALL_MENU_ITEM, OFF);
    wienmni (pxs->edit_menu_id, TRACE_SEL_MENU_ITEM, OFF);
    
    /* Disable activation of dialogs which could                                    */
    /*   cause linked list conflicts.                                               */
    
    wienmni (pxs->edit_menu_id, TRACE_ATT_MENU_ITEM, OFF);
    wienmni (pxs->edit_menu_id, TRACK_DEF_MENU_ITEM, OFF);
    
    pxs->tracesel_in_use = ON;
    
    /* copy plo trace and track info to ptemp trace and track info                  */
    status = llfree (&pxs->ptemp->tracks);
    status = llfree (&pxs->ptemp->traces);
    status = llinit (&pxs->ptemp->tracks, 0, sizeof(TRACK_STRUCT *), LL_LIST_SYSTEM);
    status = llinit (&pxs->ptemp->traces, 0, sizeof(LP_TRACE_STRUCT *),
                     LL_LIST_SYSTEM);
    
    ntracks = llcount (&pxs->plo->tracks);
    ntraces = llcount (&pxs->plo->traces);
    
    for (i = 1; i <= ntracks; i++)
        {
        ptrk = ALLOCATE (TRACK_STRUCT);
        status = (i == 1) ? llfirst (&pxs->plo->tracks,
                                     &ptrk_data) : llnext (&pxs->plo->tracks,
                                                           &ptrk_data);
        hoblockmove (ptrk_data, ptrk, sizeof(TRACK_STRUCT));
        status = llappnd (&pxs->ptemp->tracks, &ptrk);
        }
    for (i = 1; i <= ntraces; i++)
        {
        ptrc = ALLOCATE (LP_TRACE_STRUCT);
        status = (i == 1) ? llfirst (&pxs->plo->traces,
                                     &ptrc_data) : llnext (&pxs->plo->traces,
                                                           &ptrc_data);
        hoblockmove (ptrc_data, ptrc, sizeof(LP_TRACE_STRUCT));
        status = llappnd (&pxs->ptemp->traces, &ptrc);
        }
    status = xs_trc_sel_dlg_define (pxs);
    
    max_selectable = 1;
    num_items = ts_tcp_len (pxs->trace_name_tcp);
    wisselt (id, XS_TRACESEL_TRACE, XS_TRACESEL_VSC, num_items, max_selectable,
             pxs->trace_name_tcp);
    wisselt (id, XS_TRACESEL_TRACK, XS_TRACESEL_VSC, num_items, max_selectable,
             pxs->track_number_tcp);
    wisselt (id, XS_TRACESEL_LEFT, XS_TRACESEL_VSC, num_items, max_selectable,
             pxs->left_tcp);
    wisselt (id, XS_TRACESEL_RIGHT, XS_TRACESEL_VSC, num_items, max_selectable,
             pxs->right_tcp);
    wisselt (id, XS_TRACESEL_UNIT, XS_TRACESEL_VSC, num_items, max_selectable,
             pxs->trace_units_tcp);
    wisselt (id, XS_TRACESEL_TYPE, XS_TRACESEL_VSC, num_items, max_selectable,
             pxs->type_tcp);
    
    wiencdi (id, XS_TRACESEL_REPLACE, 0);
    wiencdi (id, XS_TRACESEL_DELETE, 0);
    wiencdi (id, XS_TRACESEL_DEFAULTS, 0);
    
    status = wi_open_dialog(id);
    
    return SUCCESS;
    
    }
/* END:                                                                             */

/* Function Description -----------------------------------------------------       */
/*Description:                                                                      */
/*    Server to select traces from user.                                            */
/*                                                                                  */
/*Prototype:                                                                        */
/*    static INT xs_tracesel_cdlg_server(INT id, INT item, XS_STRUCT *pxs);     */
/*                                                                                  */
/*Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)        */
/*    id              -(INT) Resource id.                                           */
/*    item            -(INT) Resource event.                                        */
/*    pxs             -(XS_STRUCT *) Pointer to XSEC struct.                        */
/*                                                                                  */
/*Return Value/Status:                                                              */
/*    SUCCESS - Successful completion.                                              */
/*    FAIL    - Unsuccessful completion.                                            */
/*                                                                                  */
/*Scope:                                                                            */
/*    PRIVATE to <module_description>                                               */
/*                                                                                  */
/*Limitations/Assumptions:                                                          */
/*                                                                                  */
/*-----------------------------------------------------------------------------     */

/* BEGIN:                                                                           */
#if USE_PROTOTYPES
static INT xs_tracesel_cdlg_server (INT id, INT item, XS_STRUCT *pxs)
#else
static INT xs_tracesel_cdlg_server (id, item, pxs)
INT id;
INT item;
XS_STRUCT *pxs;
#endif
    {
    INT status;
    INT num_items;
    INT max_selectable = 1;
    INT unused;
    INT num_selected;
    INT items[1];
    static INT index;
    INT i;
    INT ithick;
    LP_TRACE_STRUCT *ptrc, *ptrc_data;
    TRACK_STRUCT *ptrk, *ptrk_data;
    INT ntraces, ntracks;
    static LP_TRACE_STRUCT *ptraces;
    CHAR text[81];
    CHAR text_trk_num[8];
    CHAR trace_units[16];
    CHAR copy[81];
    UOM_STRUCTURE uom_data;
    
    switch (item)
        {
        
    case XS_TRACESEL_ACCEPT:
        /* return the temp data to permanent                                        */
        ntracks = llcount (&pxs->plo->tracks);
        ntraces = llcount (&pxs->plo->traces);
        
        for (i = 1; i <= ntracks; i++)
            {
            status = llgetn (&pxs->plo->tracks, &ptrk, i);
            tc_free (ptrk);
            }
        status = llfree (&pxs->plo->tracks);
        
        for (i = 1; i <= ntraces; i++)
            {
            status = llgetn (&pxs->plo->traces, &ptrc, i);
            tc_free (ptrc);
            }
        status = llfree (&pxs->plo->traces);
        status = llinit (&pxs->plo->tracks, 0, sizeof(TRACK_STRUCT *), LL_LIST_SYSTEM);
        status = llinit (&pxs->plo->traces, 0, sizeof(LP_TRACE_STRUCT *),
                         LL_LIST_SYSTEM);
        
        ntracks = llcount (&pxs->ptemp->tracks);
        ntraces = llcount (&pxs->ptemp->traces);
        
        for (i = 1; i <= ntracks; i++)
            {
            ptrk = ALLOCATE (TRACK_STRUCT);
            status = (i == 1) ? llfirst (&pxs->ptemp->tracks,
                                         &ptrk_data) : llnext (&pxs->ptemp->tracks,
                                                               &ptrk_data);
            hoblockmove (ptrk_data, ptrk, sizeof(TRACK_STRUCT));
            status = llappnd (&pxs->plo->tracks, &ptrk);
            }
        for (i = 1; i <= ntraces; i++)
            {
            ptrc = ALLOCATE (LP_TRACE_STRUCT);
            status = (i == 1) ? llfirst (&pxs->ptemp->traces,
                                         &ptrc_data) : llnext (&pxs->ptemp->traces,
                                                               &ptrc_data);
            hoblockmove (ptrc_data, ptrc, sizeof(LP_TRACE_STRUCT));
            status = llappnd (&pxs->plo->traces, &ptrc);
            }
        /* warn user if edit text fields are not blank?                             */
        /* prob should free pxs->ptemp link lists                                   */
        
        if (pxs->xst_tracesel_no_redraw EQUALS TRUE)
            {
            pxs->changed_since_display = TRUE;
            pxs->changed_since_save = TRUE;
            status = xs_cs_menu_mgr (pxs);
            }
        status = xs_tracesel_cdlg_term (pxs);
        break;
        
    case XS_TRACESEL_IGNORE:
        /* free ptemp ll                                                            */
        ntracks = llcount (&pxs->ptemp->tracks);
        for (i = 1; i <= ntracks; i++)
            {
            status = llgetn (&pxs->ptemp->tracks, &ptrk, i);
            tc_free ((VOIDPTR)ptrk);
            }
        status = llfree (&pxs->ptemp->tracks);
        
        ntraces = llcount (&pxs->ptemp->traces);
        for (i = 1; i <= ntraces; i++)
            {
            status = llgetn (&pxs->ptemp->traces, &ptrc, i);
            tc_free ((VOIDPTR)ptrc);
            }
        status = llfree (&pxs->ptemp->traces);
        status = xs_tracesel_cdlg_term (pxs);
        break;
        
    case XS_TRACESEL_SELECT:
        /* user has selected the trace (hopefully), now copy it down into           */
        /*   the edittext boxes                                                     */
        wiqsel (id, XS_TRACESEL_TRACE, &num_selected, items);
        if (num_selected != 1)
            {
            am_message (AM_ALERT, mg_message (XS_SELECT_TRACE));
            break;
            }
        index = items[0] - 1;
        if (index >= 0)
            {
            wiencdi (id, XS_TRACESEL_REPLACE, 1);
            wiencdi (id, XS_TRACESEL_DELETE, 1);
            wiscdt (id, XS_TRACESEL_TRACEEDT, pxs->trace_name_tcp[index]);
            wiscdt (id, XS_TRACESEL_TRACKEDT, pxs->track_number_tcp[index]);
            wiscdt (id, XS_TRACESEL_LEFTEDT, pxs->left_tcp[index]);
            wiscdt (id, XS_TRACESEL_RIGHTEDT, pxs->right_tcp[index]);
            wiscdt (id, XS_TRACESEL_UNITEDT, pxs->trace_units_tcp[index]);
            /* now find current data in linked list                                 */
            status = llgetn (&pxs->ptemp->traces, &ptraces, index + 1);
            /* current is set                                                       */
            wi_set_default_sel (id, XS_TRACESEL_TRACE, index + 1);
            wi_set_default_sel (id, XS_TRACESEL_TRACK, index + 1);
            wi_set_default_sel (id, XS_TRACESEL_LEFT, index + 1);
            wi_set_default_sel (id, XS_TRACESEL_RIGHT, index + 1);
            wi_set_default_sel (id, XS_TRACESEL_UNIT, index + 1);
            wi_set_default_sel (id, XS_TRACESEL_TYPE, index + 1);
            }
        else
            index = -1;
        
        break;
        
    case XS_TRACESEL_REPLACE:
        /* copy the info from the edittext boxes into the temporary structure       */
        /*   and do a replace, then update screen                                   */
        if (index < 0)
            break;
        
        /* Verify entry of a trace name.                                            */
        
        wiqccdi (id, XS_TRACESEL_TRACEEDT, &unused, text);
        if (strlen (ts_noblanks (text)) == 0)
            {
            am_message (AM_ALERT, mg_message (XS_NULL_TRACE_NAME));
            return SUCCESS;
            }

	/* Verify the trace unit flag */

	/* Trace unit flag is either "T" for traditional units or 
	   "M" for metric units or null.
	   The routine xsz_draw_trace will handle trace unit conversions
	   based on this flag for the following units:
				MM <--> IN
			  K/M3 <--> G/C3
			  US/M <--> US/F
    */

        wiqccdi (id, XS_TRACESEL_UNITEDT, &unused, trace_units);
		ts_trim (trace_units);
        ts_sto_upper (trace_units);
		if (IS_STRING (trace_units))
	    {
		if(strcmp(trace_units,"T") != 0 && strcmp(trace_units,"M") != 0
			&& strcmp(trace_units,"") != 0)
			{
			am_message (AM_ALERT,"Invalid trace unit flag.\nUse T to display curve in traditional units or M to display curve in metric units");
			return SUCCESS;
			}
	   	strcpy (ptraces->trace_units, trace_units);
        }
		if(strcmp(trace_units,"") == 0) strcpy(ptraces->trace_units,trace_units);

        /* Verify entry of a track number.                                          */
        
        wiqccdi (id, XS_TRACESEL_TRACKEDT, &unused, text_trk_num);
        if (strlen (ts_noblanks (text_trk_num)) == 0)
            {
            am_message (AM_ALERT, mg_message (XS_NULL_TRACK_NUM));
            return SUCCESS;
            }
        ts_sto_upper (text);
        strcpy (ptraces->name, text);
        
        sscanf (text_trk_num, "%d", &ptraces->track_num);
        
        wiqccdi (id, XS_TRACESEL_LEFTEDT, &unused, text);
        if (strlen (ts_noblanks (text)) == 0)
            {
            ptraces->left = 0.0;
            }
        else
            {
            if (strcmp (text, "MIN") == 0)
                ptraces->left_flag = MIN_SCALE;
            else if (strcmp (text, "MAX") == 0)
                ptraces->left_flag = MAX_SCALE;
            else
                sscanf (text, "%f", &ptraces->left);
            }
        wiqccdi (id, XS_TRACESEL_RIGHTEDT, &unused, text);
        if (strlen (ts_noblanks (text)) == 0)
            {
            ptraces->right = 0.0;
            }
        else
            {
            if (strcmp (text, "MIN") == 0)
                ptraces->right_flag = MIN_SCALE;
            else if (strcmp (text, "MAX") == 0)
                ptraces->right_flag = MAX_SCALE;
            else
                sscanf (text, "%f", &ptraces->right);
            }
        
        /* for now                                                                  */
        
        status = xs_trc_sel_dlg_define (pxs);
        num_items = ts_tcp_len (pxs->trace_name_tcp);
        max_selectable = 2;
        wisselt (id, XS_TRACESEL_TRACE, XS_TRACESEL_VSC, num_items, max_selectable,
                 pxs->trace_name_tcp);
        wiscdt (id, XS_TRACESEL_TRACEEDT, " ");
        wisselt (id, XS_TRACESEL_TRACK, XS_TRACESEL_VSC, num_items, max_selectable,
                 pxs->track_number_tcp);
        wiscdt (id, XS_TRACESEL_TRACKEDT, " ");
        wisselt (id, XS_TRACESEL_LEFT, XS_TRACESEL_VSC, num_items, max_selectable,
                 pxs->left_tcp);
        wiscdt (id, XS_TRACESEL_LEFTEDT, " ");
        wisselt (id, XS_TRACESEL_RIGHT, XS_TRACESEL_VSC, num_items, max_selectable,
                 pxs->right_tcp);
        wiscdt (id, XS_TRACESEL_RIGHTEDT, " ");
        wisselt (id, XS_TRACESEL_UNIT, XS_TRACESEL_VSC, num_items, max_selectable,
                 pxs->trace_units_tcp);
        wiscdt (id, XS_TRACESEL_UNITEDT, " ");
        wisselt (id, XS_TRACESEL_TYPE, XS_TRACESEL_VSC, num_items, max_selectable,
                 pxs->type_tcp);
        
        max_selectable = 1;
        wi_set_selection_limit (id, XS_TRACESEL_TRACE, max_selectable);
        wi_set_selection_limit (id, XS_TRACESEL_TRACK, max_selectable);
        wi_set_selection_limit (id, XS_TRACESEL_LEFT, max_selectable);
        wi_set_selection_limit (id, XS_TRACESEL_RIGHT, max_selectable);
        wi_set_selection_limit (id, XS_TRACESEL_UNIT, max_selectable);
        wi_set_selection_limit (id, XS_TRACESEL_TYPE, max_selectable);
        index = -1;
        wiencdi (id, XS_TRACESEL_REPLACE, 0);
        wiencdi (id, XS_TRACESEL_DELETE, 0);
        xsz_check_min_max_for_log (pxs->ptemp);
        break;
        
    case XS_TRACESEL_ADD:
        
        /* operate same as replace but make sure it is new                          */
        
        wiqccdi (id, XS_TRACESEL_TRACEEDT, &unused, text);
        if (strlen (ts_noblanks (text)) == 0)
            {
            am_message (AM_ALERT, mg_message (XS_NULL_TRACE_NAME));
            return SUCCESS;
            }

	/* Verify the trace unit flag */

	/* Trace unit flag is either "T" for traditional units or 
	   "M" for metric units or null.
	   The routine xsz_draw_trace will handle trace unit conversions
	   based on this flag for the following units:
				MM <--> IN
			  K/M3 <--> G/C3
			  US/M <--> US/F
    */

        wiqccdi (id, XS_TRACESEL_UNITEDT, &unused, trace_units);
		ts_trim (trace_units);
        ts_sto_upper (trace_units);
		if (IS_STRING (trace_units))
	    {
		if(strcmp(trace_units,"T") != 0 && strcmp(trace_units,"M") != 0
			 && strcmp(trace_units,"") != 0)
			{
			am_message (AM_ALERT,"Invalid trace unit flag.\nUse T to display curve in traditional units or M to display curve in metric units");
			return SUCCESS;
			}
	    }

        /* Verify entry of a track number.                                          */
        
        wiqccdi (id, XS_TRACESEL_TRACKEDT, &unused, text_trk_num);
        if (strlen (ts_noblanks (text_trk_num)) == 0)
            {
            am_message (AM_ALERT, mg_message (XS_NULL_TRACK_NUM));
            return SUCCESS;
            }
        pxs->xst_tracesel_no_redraw = TRUE;
        
        ptraces = ALLOCATE (LP_TRACE_STRUCT);
        ts_sto_upper (text);
        strcpy (ptraces->name, text);
	if (IS_STRING(trace_units))
	  {
	    strcpy (ptraces->trace_units, trace_units);
	  }
        sscanf (text_trk_num, "%d", &ptraces->track_num);
        
        wiqccdi (id, XS_TRACESEL_LEFTEDT, &unused, text);
        if (strlen (ts_noblanks (text)) == 0)
            {
            ptraces->left = 0.0;
            }
        else
            {
            if (strcmp (text, "MIN") == 0)
                ptraces->left_flag = MIN_SCALE;
            else if (strcmp (text, "MAX") == 0)
                ptraces->left_flag = MAX_SCALE;
            else
                sscanf (text, "%f", &ptraces->left);
            }
        wiqccdi (id, XS_TRACESEL_RIGHTEDT, &unused, text);
        if (strlen (ts_noblanks (text)) == 0)
            {
            ptraces->right = 0.0;
            }
        else
            {
            if (strcmp (text, "MIN") == 0)
                ptraces->right_flag = MIN_SCALE;
            else if (strcmp (text, "MAX") == 0)
                ptraces->right_flag = MAX_SCALE;
            else
                sscanf (text, "%f", &ptraces->right);
            }
        
        /* know a better way to do this??                                           */
        ptraces->visible = TRUE;
        ptraces->visible_scale_bar = TRUE;
        pxs->plo->last_trace_number++;
        ptraces->number = pxs->plo->last_trace_number;
        lu_text_to_index ("COLOR", (INDEX *)&ptraces->line.color, "RED");
        ptraces->line.width = 1.0;  /* fine                                         */
        lu_text_to_index ("LINE_STYLE", (INDEX *)&ptraces->line.style, "SOLID");
        status = llappnd (&pxs->ptemp->traces, &ptraces);
        /* for now                                                                  */
        
        status = xs_trc_sel_dlg_define (pxs);
        num_items = ts_tcp_len (pxs->trace_name_tcp);
        max_selectable = 2;
        wisselt (id, XS_TRACESEL_TRACE, XS_TRACESEL_VSC, num_items, max_selectable,
                 pxs->trace_name_tcp);
        wiscdt (id, XS_TRACESEL_TRACEEDT, " ");
        wisselt (id, XS_TRACESEL_TRACK, XS_TRACESEL_VSC, num_items, max_selectable,
                 pxs->track_number_tcp);
        wiscdt (id, XS_TRACESEL_TRACKEDT, " ");
        wisselt (id, XS_TRACESEL_LEFT, XS_TRACESEL_VSC, num_items, max_selectable,
                 pxs->left_tcp);
        wiscdt (id, XS_TRACESEL_LEFTEDT, " ");
        wisselt (id, XS_TRACESEL_RIGHT, XS_TRACESEL_VSC, num_items, max_selectable,
                 pxs->right_tcp);
        wiscdt (id, XS_TRACESEL_RIGHTEDT, " ");
        wisselt (id, XS_TRACESEL_UNIT, XS_TRACESEL_VSC, num_items, max_selectable,
                 pxs->trace_units_tcp);
        wiscdt (id, XS_TRACESEL_UNITEDT, " ");
        wisselt (id, XS_TRACESEL_TYPE, XS_TRACESEL_VSC, num_items, max_selectable,
                 pxs->type_tcp);
        max_selectable = 1;
        wi_set_selection_limit (id, XS_TRACESEL_TRACE, max_selectable);
        wi_set_selection_limit (id, XS_TRACESEL_TRACK, max_selectable);
        wi_set_selection_limit (id, XS_TRACESEL_LEFT, max_selectable);
        wi_set_selection_limit (id, XS_TRACESEL_RIGHT, max_selectable);
        wi_set_selection_limit (id, XS_TRACESEL_UNIT, max_selectable);
        wi_set_selection_limit (id, XS_TRACESEL_TYPE, max_selectable);
        index = -1;
        wiencdi (id, XS_TRACESEL_REPLACE, 0);
        wiencdi (id, XS_TRACESEL_DELETE, 0);
        xsz_check_min_max_for_log (pxs->ptemp);
        break;
        
    case XS_TRACESEL_DELETE:
        
        pxs->xst_tracesel_no_redraw = TRUE;
        
        /* delete the selected trace from the list, also from the screen            */
        if (index < 0)
            break;
        /* prob should do llgetn and free the data pointed to                       */
        status = lldela (&pxs->ptemp->traces);
        /* for now                                                                  */
        
        status = xs_trc_sel_dlg_define (pxs);
        num_items = ts_tcp_len (pxs->trace_name_tcp);
        max_selectable = 2;
        wisselt (id, XS_TRACESEL_TRACE, XS_TRACESEL_VSC, num_items, max_selectable,
                 pxs->trace_name_tcp);
        wiscdt (id, XS_TRACESEL_TRACEEDT, " ");
        wisselt (id, XS_TRACESEL_TRACK, XS_TRACESEL_VSC, num_items, max_selectable,
                 pxs->track_number_tcp);
        wiscdt (id, XS_TRACESEL_TRACKEDT, " ");
        wisselt (id, XS_TRACESEL_LEFT, XS_TRACESEL_VSC, num_items, max_selectable,
                 pxs->left_tcp);
        wiscdt (id, XS_TRACESEL_LEFTEDT, " ");
        wisselt (id, XS_TRACESEL_RIGHT, XS_TRACESEL_VSC, num_items, max_selectable,
                 pxs->right_tcp);
        wiscdt (id, XS_TRACESEL_RIGHTEDT, " ");
        wisselt (id, XS_TRACESEL_UNIT, XS_TRACESEL_VSC, num_items, max_selectable,
                 pxs->trace_units_tcp);
        wiscdt (id, XS_TRACESEL_UNITEDT, " ");
        wisselt (id, XS_TRACESEL_TYPE, XS_TRACESEL_VSC, num_items, max_selectable,
                 pxs->type_tcp);
        
        max_selectable = 1;
        wi_set_selection_limit (id, XS_TRACESEL_TRACE, max_selectable);
        wi_set_selection_limit (id, XS_TRACESEL_TRACK, max_selectable);
        wi_set_selection_limit (id, XS_TRACESEL_LEFT, max_selectable);
        wi_set_selection_limit (id, XS_TRACESEL_RIGHT, max_selectable);
        wi_set_selection_limit (id, XS_TRACESEL_UNIT, max_selectable);
        wi_set_selection_limit (id, XS_TRACESEL_TYPE, max_selectable);
        index = -1;
        /* END FOR NOW                                                              */
        wiencdi (id, XS_TRACESEL_REPLACE, 0);
        wiencdi (id, XS_TRACESEL_DELETE, 0);
        
        break;
        
    case XS_TRACESEL_DEFAULTS:
        /* user selected a trace and now wants the trace defaults copied            */
        /*   if (index < 0 ) break;                                                 */
        /*   into the edittext boxes                                                */
        /* first get just the tracename from the dialog box                         */
        wiqccdi (id, XS_TRACESEL_TRACEEDT, &unused, text);
        
        strcpy (copy, text);
        status = strcspn (copy, ":");
        if (status != strlen (copy))
            strcpy (copy, &copy[status + 1]);
        status = strcspn (copy, ".");
        if (status != strlen (copy))
            {
            strncpy (copy, copy, status);
            copy[status] = NUL;
            }
        status = strcspn (copy, ";");
        if (status != strlen (copy))
            {
            strncpy (copy, copy, status);
            copy[status] = NUL;
            }
        ts_sto_upper (copy);
        strcpy (pxs->lg_trace_defs.trace_name, copy);
        status = xs_gen_trc_defs (pxs);
        if (status == SUCCESS)
            {
            status = llgetn (&pxs->ptemp->traces, &ptraces, index + 1);
            strcpy (ptraces->name, text);
            ptraces->track_num = pxs->lg_trace_defs.track_number;
            ptraces->left = pxs->lg_trace_defs.left;
            ptraces->right = pxs->lg_trace_defs.right;
            
            status = lu_text_to_index ("COLOR", (INDEX *)&ptraces->line.color,
                                       pxs->lg_trace_defs.line_color);
            
            status = lu_text_to_index ("THICKNESS", (INDEX *)&ithick,
                                       pxs->lg_trace_defs.line_thickness);
            ptraces->line.width = (DOUBLE)ithick;
            
            status = lu_text_to_index ("LINE_STYLE", (INDEX *)&ptraces->line.style,
                                       pxs->lg_trace_defs.line_style);
            
            wiscdt (id, XS_TRACESEL_TRACEEDT, ptraces->name);
            sprintf (text, "%d", ptraces->track_num);
            wiscdt (id, XS_TRACESEL_TRACKEDT, text);
            sprintf (text, "%.2f", ptraces->left);
            wiscdt (id, XS_TRACESEL_LEFTEDT, text);
            sprintf (text, "%.2f", ptraces->right);
            wiscdt (id, XS_TRACESEL_RIGHTEDT, text);
            
            }
        /* end success                                                              */
        
        break;
        
        }
    return SUCCESS;
    
    }
/* END:                                                                             */

/* Function Description -----------------------------------------------------       */
/*Description:                                                                      */
/*    Wrap up trace selection dialog.                                               */
/*                                                                                  */
/*Prototype:                                                                        */
/*    publicdef INT xs_tracesel_cdlg_term(XS_STRUCT *pxs);                          */
/*                                                                                  */
/*Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)        */
/*    pxs             -(XS_STRUCT *) Pointer to XSEC struct.                        */
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
publicdef INT xs_tracesel_cdlg_term (XS_STRUCT *pxs)
#else
publicdef INT xs_tracesel_cdlg_term (pxs)
XS_STRUCT *pxs;
#endif
    {
    INT status;
    
    status = ts_tcp_free (pxs->trace_name_tcp);
    status = ts_tcp_free (pxs->track_number_tcp);
    status = ts_tcp_free (pxs->left_tcp);
    status = ts_tcp_free (pxs->right_tcp);
    status = ts_tcp_free (pxs->trace_units_tcp);
    status = ts_tcp_free (pxs->type_tcp);
    pxs->trace_name_tcp = (TCP)0;
    pxs->track_number_tcp = (TCP)0;
    pxs->left_tcp = (TCP)0;
    pxs->right_tcp = (TCP)0;
    pxs->trace_units_tcp = (TCP)0;
    pxs->type_tcp = (TCP)0;
    pxs->tracesel_in_use = OFF;
    am_release_resource (pxs->tracesel_cdlg_id);
    if (pxs->review_all_switch == ON)
        status = xs_traceattr_cdlg_init (pxs);
    else
        {
        wienmni (pxs->edit_menu_id, TRACE_SEL_MENU_ITEM, ON);
        wienmni (pxs->edit_menu_id, CHANGE_ALL_MENU_ITEM, ON);
        wienmni (pxs->edit_menu_id, TRACE_ATT_MENU_ITEM, ON);
        wienmni (pxs->edit_menu_id, TRACK_DEF_MENU_ITEM, ON);
        
        }
    return SUCCESS;
    
    }
/* END:                                                                             */

/* Function Description -----------------------------------------------------       */
/*Description:                                                                      */
/*    To retrieve from the trace structure the data requi-                          */
/*    red to fill in the trace selection dialog box.  The                           */
/*    trace name, track no., left, right and type fields                            */
/*    are retrieved from the TRACK and LP_TRACE structures                          */
/*    and converted to strings.  The routine returns a ptr                          */
/*    to a tcp for each field.                                                      */
/*                                                                                  */
/*Prototype:                                                                        */
/*    static INT xs_trc_sel_dlg_define(XS_STRUCT *pxs);                         */
/*                                                                                  */
/*Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)        */
/*    pxs             -(XS_STRUCT *) Log_plot LISTHEAD structure   (must contain complete   */
/*                        track information), ptrs to tcps                          */
/*                                                                                  */
/*Return Value/Status:                                                              */
/*    SUCCESS - Successful completion.                                              */
/*                                                                                  */
/*Scope:                                                                            */
/*    PRIVATE to <module_description>                                               */
/*                                                                                  */
/*Limitations/Assumptions:                                                          */
/*                                                                                  */
/*-----------------------------------------------------------------------------     */

/* BEGIN:                                                                           */
#if USE_PROTOTYPES
static INT xs_trc_sel_dlg_define (XS_STRUCT *pxs)
#else
static INT xs_trc_sel_dlg_define (pxs)
XS_STRUCT *pxs;
#endif
    {
    INT max_len_depth_string = 11;
    LP_TRACE_STRUCT *p_trace;
    TRACK_STRUCT *p_track;
    INT j;
    INT no_traces;
    INT nchars;
    INT status;
    CHAR text[81];
    
    no_traces = llcount (&pxs->ptemp->traces);
    status = llfirst (&pxs->ptemp->traces, &p_trace);
    
    if (pxs->trace_name_tcp != (TCP)0)
        status = ts_tcp_free (pxs->trace_name_tcp);
    if (pxs->track_number_tcp != (TCP)0)
        status = ts_tcp_free (pxs->track_number_tcp);
    if (pxs->left_tcp != (TCP)0)
        status = ts_tcp_free (pxs->left_tcp);
    if (pxs->right_tcp != (TCP)0)
        status = ts_tcp_free (pxs->right_tcp);
    if (pxs->trace_units_tcp != (TCP)0)
        status = ts_tcp_free (pxs->trace_units_tcp);
    if (pxs->type_tcp != (TCP)0)
        status = ts_tcp_free (pxs->type_tcp);
    
    pxs->trace_name_tcp = ts_tcp_alloc (no_traces);
    pxs->track_number_tcp = ts_tcp_alloc (no_traces);
    pxs->left_tcp = ts_tcp_alloc (no_traces);
    pxs->right_tcp = ts_tcp_alloc (no_traces);
    pxs->trace_units_tcp = ts_tcp_alloc (no_traces);
    pxs->type_tcp = ts_tcp_alloc (no_traces);
    
    for (j = 0; j < no_traces; j++) /* loop on traces, get data                     */
        {
        /* allocate string space                                                    */
        pxs->trace_name_tcp[j] = STRING_ALLOCATE (p_trace->name);
        strcpy (pxs->trace_name_tcp[j], p_trace->name);
        nchars = sprintf (text, "%d", p_trace->track_num);
        pxs->track_number_tcp[j] = (CHAR *) tc_alloc (sizeof(CHAR) * nchars + 1);
        strcpy (pxs->track_number_tcp[j], text);
        /* check min and max flag                                                   */
        if (p_trace->left_flag == MIN_SCALE)
            {
            strcpy (text, "MIN");
            nchars = 3;             /* length of MIN                                */
            }
        else if (p_trace->left_flag == MAX_SCALE)
            {
            strcpy (text, "MAX");
            nchars = 3;             /* length of MAX                                */
            }
        else
            {
            nchars = sprintf (text, "%.2lf", p_trace->left);
            }
        pxs->left_tcp[j] = (CHAR *) tc_alloc (sizeof(CHAR) * nchars + 1);
        strcpy (pxs->left_tcp[j], text);
        if (p_trace->right_flag == MIN_SCALE)
            {
            strcpy (text, "MIN");
            nchars = 3;
            }
        else if (p_trace->right_flag == MAX_SCALE)
            {
            strcpy (text, "MAX");
            nchars = 3;
            }
        else
            {
            nchars = sprintf (text, "%.2lf", p_trace->right);
            }
        pxs->right_tcp[j] = (CHAR *) tc_alloc (sizeof(CHAR) * nchars + 1);
        strcpy (pxs->right_tcp[j], text);
        
	if (IS_STRING(p_trace->trace_units))
	    {
	      pxs->trace_units_tcp[j] = (CHAR *) tc_zalloc (sizeof(CHAR) *
							    (strlen (p_trace->trace_units) +
							     1));
	      strcpy (pxs->trace_units_tcp[j], p_trace->trace_units);
	    }
	else
	    {
	      pxs->trace_units_tcp[j] = (CHAR *) tc_zalloc (sizeof(CHAR) * 2);
	      strcpy (pxs->trace_units_tcp[j], " ");
	    }
        
        pxs->type_tcp[j] = (CHAR *) tc_alloc (sizeof(CHAR) * max_len_depth_string);
        
        /* Find the track of this trace                                             */
        
        status = xsz_find_track (pxs->ptemp, p_trace->track_num, &p_track);
        
        if (status == SUCCESS)
            {
            /*fix . get oracle value rather than def                                */
            /* chk track type and create                                            */
            switch (p_track->type)
                {
            case DEPTH_TRACK:
                strcpy (pxs->type_tcp[j], "DEPTH");
                break;
                
            case LINEAR_TRACK:
                strcpy (pxs->type_tcp[j], "LIN");
                break;
                
            case LOG_TRACK:
                strcpy (pxs->type_tcp[j], "LOG");
                break;
                
            case TEXT_TRACK:
                strcpy (pxs->type_tcp[j], "TEXT");
                break;
                
            case TIME_TRACK:
                strcpy (pxs->type_tcp[j], "TIME");
                break;
                }
            }
        else
            strcpy (pxs->type_tcp[j], "<--->");
        
        llnext (&pxs->ptemp->traces, &p_trace);
        }
    if (no_traces < 1)
        {
        wiencdi (pxs->tracesel_cdlg_id, XS_TRACESEL_SELECT, 0);
        }
    else
        {
        wiencdi (pxs->tracesel_cdlg_id, XS_TRACESEL_SELECT, 1);
        }
    return(SUCCESS);
    }
/* END:                                                                             */

/* Function Description -----------------------------------------------------       */
/*Description:                                                                      */
/*    To check the type of tracks, if they are LOGORITHMIC, make sure               */
/*    the minmum and maxmum of traces which are in this track have positive         */
/*    values.                                                                       */
/*                                                                                  */
/*Prototype:                                                                        */
/*    publicdef INT xsz_check_min_max_for_log(LAYOUT_STRUCT *p_layout );            */
/*                                                                                  */
/*Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)        */
/*    p_layout        -(LAYOUT_STRUCT *)                                            */
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
publicdef INT xsz_check_min_max_for_log (LAYOUT_STRUCT *p_layout)
#else
publicdef INT xsz_check_min_max_for_log (p_layout)
LAYOUT_STRUCT *p_layout;
#endif
    {
    LP_TRACE_STRUCT *p_trace;
    TRACK_STRUCT *p_track;
    INT status = SUCCESS;
    INT no_traces = 0, i;
    BOOL warning = FALSE;
    CHAR text[256], *ptr;
    
    no_traces = llcount (&p_layout->traces);
    ptr = text;
    for (i = 0; i < no_traces; i++)
        {
        status = (i EQUALS 0) ? llfirst (&p_layout->traces, &p_trace) :
                                llnext (&p_layout->traces, &p_trace);
        
        status = xsz_find_track (p_layout, p_trace->track_num, &p_track);
        if (status != SUCCESS)
            continue;
        
        if (p_trace->left >= 0 AND p_trace->right >= 0)
            continue;
        
        if (p_track->type EQUALS LOG_TRACK)
            {
            warning = TRUE;
            ptr += sprintf (ptr, mg_message (XS_TRACK_IS_LOG), p_trace->track_num,
                            p_trace->name);
            if (p_trace->left < 0)
                {
                ptr += sprintf (ptr, mg_message (XS_MIN_VALUE_PROMPT), p_trace->left);
                }
            if (p_trace->right < 0)
                {
                ptr += sprintf (ptr, mg_message (XS_MAX_VALUE_PROMPT), p_trace->right);
                }
            ptr += sprintf (ptr, " is illegal\n");
            }
        }
    *ptr = '\0';
    
    if (warning)
        {
        status = am_message (AM_ALERT, text);
        }
    return status;
    }
/* END:                                                                             */
