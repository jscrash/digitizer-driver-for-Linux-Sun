/* DEC/CMS REPLACEMENT HISTORY, Element LG_ED_DB_COPY.C*/
/* *3    23-AUG-1990 17:53:00 JESSIE "(SPR 1) fix include files"*/
/* *2    11-AUG-1989 11:52:03 JULIAN "(SPR -1) statment not reached"*/
/* *1    19-JUN-1989 13:04:53 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element LG_ED_DB_COPY.C*/
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

#ifndef ESI_ORACLE_H

#include "esi_oracle.h"

#endif

#ifndef ESI_AM_H

#include "esi_am.h"

#endif

#ifndef ESI_ER_H

#include "esi_er.h"

#endif

#ifndef ESI_QS_H

#include "esi_qs.h"

#endif

#ifndef ESI_WI_H

#include "esi_wi.h"

#endif

#ifndef ESI_LG_LDEFS_H

#include "esi_lg_ldefs.h"

#endif

#ifndef ESI_LG_MSG_H

#include "esi_lg_msg.h"

#endif

#ifndef ESI_LG_EDIT_H

#include "esi_lg_edit.h"

#endif

#ifndef ESI_TS_H

#include "esi_ts.h"

#endif

#ifndef ESI_TC_H

#include "esi_tc.h"

#endif

#ifndef ESI_MG_H

#include "esi_mg.h"

#endif

#ifndef ESI_ED_DB_CDLGS_RH

#include "lg_ed_db_cdlgs.rh"

#endif

#include "esi_lg_err.h"

#define Resource_File  "LG_ED_DB_CDLGS"

#if USE_PROTOTYPES
static INT lg_db_copy_cdlg_server (INT id, INT item, LOG_EDIT_STRUCT *plg);
static INT lg_db_copy_trace_data (LOG_EDIT_STRUCT *plg);

#else
static INT lg_db_copy_cdlg_server ();
static INT lg_db_copy_trace_data ();

#endif

/* File Description ---------------------------------------------------------
Overview:

Prototypes and one line descriptions of each function in the file.
Public_Functions:
    publicdef INT lg_db_copy_cdlg_init(LOG_EDIT_STRUCT  *plg);
    
Private_Functions:
    static INT lg_db_copy_cdlg_server(INT id, INT item, LOG_EDIT_STRUCT *plg);
    
    static INT lg_db_copy_trace_data( LOG_EDIT_STRUCT *plg);

-----------------------------------------------------------------------------*/

/* Function Description -----------------------------------------------------
Description:
    This routine brings up a select list of log traces for  
    the current UWI. The user may select a trace to be      
    copied ( entering the new trace and optionally the      
    service).                                               

Prototype:
    publicdef INT lg_db_copy_cdlg_init(LOG_EDIT_STRUCT  *plg);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    plg             -(LOG_EDIT_STRUCT  *)

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT lg_db_copy_cdlg_init (LOG_EDIT_STRUCT *plg) 
#else
publicdef INT lg_db_copy_cdlg_init (plg)
LOG_EDIT_STRUCT *plg;
#endif
    {
    INT status;
    
    /******************************************************************************/
    
    /* set up trace selector dialog res*/
    
    status = am_define_resource (AM_DIALOG, Resource_File, LG_ED_COPY_TRACE_CDLG,
                                 lg_db_copy_cdlg_server, &plg->db_copy_cdlg_id, plg);
    
    /* set up selector control         */
    wisselt (plg->db_copy_cdlg_id, LG_ED_COPY_SELECTOR, LG_ED_COPY_VSCROLL,
             plg->num_traces, 1, plg->trace_tcp);
    
    /* set default selec. to 1st item  */
    wi_set_default_sel (plg->db_copy_cdlg_id, LG_ED_COPY_SELECTOR, 1);
    
    /* bring up dialog box             */
    status = wiopcd (plg->db_copy_cdlg_id);
    
    return status;
    
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    Server for the select trace to copy routine.            

Prototype:
    static INT lg_db_copy_cdlg_server( INT id, INT item, LOG_EDIT_STRUCT   *plg);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    id              -(INT)
    item            -(INT)
    plg             -(LOG_EDIT_STRUCT   *)

Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PRIVATE to <lg_db_copy_cdlg_init>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static INT lg_db_copy_cdlg_server (INT id, INT item, LOG_EDIT_STRUCT *plg) 
#else
static INT lg_db_copy_cdlg_server (id, item, plg)
INT id, item;
LOG_EDIT_STRUCT *plg;
#endif
    {
    INT status;
    
    /******************************************************************************/
    
    switch (item)
        {
        /* Cancel button pressed, exit     */
    case LG_ED_COPY_CANCEL:
        status = SUCCESS;
        break;
        
    case LG_ED_COPY_OK:             /* Branch to copy routine           */
        
        status = lg_db_copy_trace_data (plg);
        break;
        
        }
    /* end of switch construct         */
    
    am_message (AM_DIALOG, "");
    
    if (status == SUCCESS)
        {
        status = lg_ed_db_menu_vis_on_off (plg, ON);
        status = lg_ed_well_menu_vis_on_off (plg, ON);
        status = lg_ed_edit_menu_vis_on_off (plg, ON);
        status = am_release_resource (plg->db_copy_cdlg_id);
        }
    /* Free trace structure      */
    status = lg_ltdm_free_trace (plg->ptrace);
    
    return SUCCESS;
    
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    Performs the actual copying of the trace by retrieving  
    the existing trace, changing the trace name and writing 
    out the new trace.                                      

Prototype:
    static INT lg_db_copy_trace_data(LOG_EDIT_STRUCT   *plg);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    plg             -(LOG_EDIT_STRUCT   *)

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PRIVATE to <lg_db_copy_cdlg_init>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static INT lg_db_copy_trace_data (LOG_EDIT_STRUCT *plg) 
#else
static INT lg_db_copy_trace_data (plg)
LOG_EDIT_STRUCT *plg;
#endif
    {
    INT status;
    INT num_items_ret;
    INT value, done;
    CHAR trace[15];                 /* Entered trace  (DT,RHOB etc.)   */
    CHAR service[15];               /* Optionally entered service      */
    LOG_TRACE_STRUCT ck_trace;      /* Structure for checking name.    */
    LOG_TRACE_STRUCT *pck_trace = &ck_trace;
    static INT items_selected[1];   /* array for return item           */
    
    /******************************************************************************/
    
    /* Clean up the ck_trace structure.   */
    
    tc_zeroes ((BYTE *)pck_trace, sizeof(LOG_TRACE_STRUCT));
    
    /* Allocate space for the trace
      structure of the trace to be copied */
    
    plg->ptrace = (LOG_TRACE_STRUCT *)tc_zalloc (sizeof(LOG_TRACE_STRUCT));
    
    wiqsel (plg->db_copy_cdlg_id, LG_ED_REN_TRACE_SELECTOR, &num_items_ret,
            items_selected);
    
    if (num_items_ret == 0)
        {
        status = am_message (AM_ALERT, mg_message (LG_ED_NO_TRACE_SELECTED));
        return FAIL;
        }
    /* A trace has been picked         */
    
    strcpy (plg->edit_trace, plg->trace_tcp[items_selected[0] - 1]);
    
    /* Get trace structure for the trace   */
    
    status = lg_ltdm_inq_trace_struct (plg->edit_uwi, plg->edit_trace, "",
                                       plg->ptrace);
    
    if (status != SUCCESS)
        {
        status = am_message (AM_ALERT, mg_message (LG_ED_TRACE_NOT_READ));
        er_log ("LG_LTDM_INQ_TRACE_STRUCT failed in LG_DB_COPY_TRACE_DATA.");
        return FAIL;
        }
    /* Now check user entered trace,service*/
    
    wi_query_dialog_item (plg->db_copy_cdlg_id, LG_ED_COPY_TRACE, &value, trace);
    ts_noblanks (trace);
    if (strlen (trace) == 0)
        {
        am_message (AM_ALERT, mg_message (LG_ED_ENTER_TRACE));
        return FAIL;
        }
    wi_query_dialog_item (plg->db_copy_cdlg_id, LG_ED_COPY_SERVICE, &value, service);
    ts_noblanks (service);
    
    /* Create new trace name from items   */
    if (strlen (service) > 0)
        {
        sprintf (pck_trace->trace_name, "COPY:%s.%s;0", trace, service);
        }
    else
        {
        sprintf (pck_trace->trace_name, "COPY:%s;0", trace);
        }
    /* Clean up trace name upcase, etc. */
    
    ts_sto_upper (pck_trace->trace_name);
    
    /* Validate the new trace name.     */
    
    status = lg_ltdm_parser (pck_trace);
    switch (status)
        {
    case SUCCESS:
        break;
    case LG_INVALID_POINTER:
        am_message (AM_ALERT, mg_message (LG_INVALID_POINTER));
        er_log (mg_message (LG_INVALID_POINTER));
        return FAIL;
    case LG_NO_TRACE_NAME:
    case LG_INVALID_BLANK:
    case LG_INVALID_FORMAT:
    case LG_NO_TRACE_FIELD:
    default:
        am_message (AM_ALERT, mg_message (LG_ED_INVALID_TRACE_NAME));
        return FAIL;
        }
    /* Now retrieve the trace itself      */
    
    status = am_message (AM_DIALOG, mg_message (LG_ED_COPYING_TRACE));
    status = lg_ltdm_get_trace (plg->ptrace);
    
    if (status != SUCCESS)
        {
        status = am_message (AM_ALERT, mg_message (LG_ED_TRACE_NOT_READ));
        er_log ("LG_LTDM_GET_TRACE failed in LG_DB_COPY_TRACE_DATA.");
        return FAIL;
        }
    /* Now attempt to add back de trace
       to de database, bro' (honky mother) */
    
    strcpy (plg->ptrace->trace_name, pck_trace->trace_name);
    status = lg_ltdm_add_trace (plg->ptrace);
    
    if (status != SUCCESS)
        {
        am_message (AM_ALERT, mg_message (LG_ED_TRACE_NOT_WRITTEN));
        er_log ("LG_LTDM_ADD_TRACE failed in LG_DB_COPY_TRACE_DATA.");
        status = FAIL;
        }
    else
        {
        /* If copy okay, release resource, reset
          trace tcp so traces will be retrieved
          again by LTDM (we have extra trace) */
        
        plg->num_traces = 0;
        status = ts_tcp_free (plg->trace_tcp);
        status = SUCCESS;
        }
    return status;
    
    }
/* END:     */
