/* DEC/CMS REPLACEMENT HISTORY, Element LG_REN_CDLG.C*/
/* *3    30-AUG-1990 12:20:13 GILLESPIE "(SPR 1) Remove unused, duplicate includes"*/
/* *2    27-JUL-1989 23:47:49 GILLESPIE "(SPR 1) Fix minor casting problems"*/
/* *1    19-JUN-1989 13:05:18 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element LG_REN_CDLG.C*/

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

#include "esi_c_lib.h"
#ifndef ESI_AM_H
#include "esi_am.h"
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
#ifndef ESI_LG_ERR_H
#include "esi_lg_err.h"
#endif
#ifndef ESI_LG_EDIT_H
#include "esi_lg_edit.h"
#endif
#ifndef ESI_TS_H
#include "esi_ts.h"
#endif
#ifndef ESI_MG_H
#include "esi_mg.h"
#endif
#include "lg_ed_db_cdlgs.rh"

#define Resource_File  "LG_ED_DB_CDLGS"

#if USE_PROTOTYPES

static INT lg_db_rename_cdlg_server( INT id, INT item, LOG_EDIT_STRUCT *plg);
static INT lg_db_rename_confirm( LOG_EDIT_STRUCT *plg,
    LOG_TRACE_NAME new_name);

#else

static INT lg_db_rename_cdlg_server();
static INT lg_db_rename_confirm();

#endif


/******************************************************************************/
/*                                                                            */
/*    Routine:        LG_DB_RENAME_CDLG_INIT                                  */
/*                                                                            */
/*    Function:       This routine brings up a select list of log traces for  */
/*                    the current UWI. The user may select a log from the     */
/*                    trace to rename.                                        */
/*                                                                            */
/*    Author:         D. Mcleod                                               */
/*                                                                            */
/******************************************************************************/


publicdef INT lg_db_rename_cdlg_init( plg)

  LOG_EDIT_STRUCT  *plg;

{
    INT                status;

/******************************************************************************/



                                           /* set up trace selector dialog res*/

    status = am_define_resource(AM_DIALOG, Resource_File,
                                LG_ED_RENAME_TRACE_CDLG, 
                                lg_db_rename_cdlg_server, 
                                &plg->db_rename_cdlg_id,               
                                plg);


                                           /* set up selector control         */
    wisselt(plg->db_rename_cdlg_id, LG_ED_REN_TRACE_SELECTOR,
            LG_ED_REN_TRACE_VSCROLL, plg->num_traces, 1, plg->trace_tcp);

                                           /* set default selec. to 1st item  */
    wi_set_default_sel( plg->db_rename_cdlg_id,
                        LG_ED_REN_TRACE_SELECTOR, 1);

                                           /* bring up dialog box             */
    status = wiopcd(plg->db_rename_cdlg_id); 


    return status;

}






/******************************************************************************/
/*                                                                            */
/*    Routine:        LG_DB_RENAME_CDLG_SERVER                                */
/*                                                                            */
/*    Function:       Server for the select trace routine.                    */
/*                                                                            */
/*    Author:         D. Mcleod                                               */
/*                                                                            */
/******************************************************************************/

#if USE_PROTOTYPES
static INT lg_db_rename_cdlg_server( INT id, INT item, LOG_EDIT_STRUCT *plg)
#else
static INT lg_db_rename_cdlg_server( id, item, plg)
INT               id, item;
LOG_EDIT_STRUCT   *plg;
#endif
{
    INT                status;
    INT                num_items_ret;
    INT                value, done;
    CHAR               trace[15];
    CHAR               service[15];
    LOG_TRACE_NAME     new_name;
    LOG_TRACE_STRUCT   trace_struct;
    LOG_TRACE_STRUCT   *ptrace = &trace_struct;
    INT                items_selected[1];  /* array for return item           */

/******************************************************************************/



    switch (item)
     {
                                       /* Cancel button pressed, exit     */
      case LG_ED_REN_TRACE_CANCEL:                 
           status = SUCCESS;
           break;


      case LG_ED_REN_TRACE_RENAME:         /* Retrieve button pressed         */
                                           /* query dialog for 1 selection    */

           wiqsel( plg->db_rename_cdlg_id, LG_ED_REN_TRACE_SELECTOR,
                   &num_items_ret, items_selected);
           if (num_items_ret == 0)
            {
             status = am_message(AM_ALERT,mg_message(LG_ED_NO_TRACE_SELECTED));
             status = FAIL;
             break;
            }
                                        /* A trace has been picked, store in
                                         structure and call routine to set up
                                         header rename dialog .         */

           strcpy( plg->edit_trace,
                   plg->trace_tcp[items_selected[0] -1 ]);

           wi_query_dialog_item( plg->db_rename_cdlg_id,
                                 LG_ED_REN_TRACE_TRACE,
                                 &value, trace );

           ts_noblanks( trace );
           if (strlen (trace) == 0)
            {
             am_message( AM_ALERT, mg_message(LG_ED_ENTER_TRACE) );
             status = FAIL;
             break;
            }


           wi_query_dialog_item( plg->db_rename_cdlg_id,
                                 LG_ED_REN_TRACE_SERVICE,
                                 &value, service );
           ts_noblanks( service );

                                       /* Build the trace name string      */

           sprintf( (char *)new_name, "RENAME:%s", trace);

           if (service != (CHAR *) NULL && strlen(service) > 0)
             {
               sprintf( (char *)new_name, "RENAME:%s.%s", trace, service);
             }

                                       /* Clean up trace name upcase, etc. */

           ts_sto_upper( (CHAR *)new_name );

                                       /* Bring up confirmation dialog and 
                                          perform rename if confirmed      */

           status = lg_db_rename_confirm( plg, new_name);
 
           if (status == SUCCESS)
              {
               am_message (AM_DIALOG, mg_message(LG_ED_RENAMING_TRACE) );
  
               status = lg_ltdm_rename_trace (plg->edit_uwi, plg->edit_trace,
                                           new_name);

               am_message (AM_DIALOG, "");

               switch ( status )
                 {
                   case SUCCESS:
                                       /* Rename worked, release dialog and 
                                       copy in new trace name to tcp       */

                        strcpy( ptrace->uwi, plg->edit_uwi );
                        strcpy( ptrace->trace_name, (char *)new_name );
                        status = lg_ltdm_parser( ptrace );
                        status = lgz_inq_last_version( ptrace, FALSE );
                        if ( status == SUCCESS )
                           {
                            strcpy( (char *)new_name, ptrace->trace_name );
                           }
                        strcpy( plg->trace_tcp[items_selected[0] -1 ], 
                                (char *)new_name);
                        status = SUCCESS;
                        break;

                   case LG_INVALID_POINTER:
                        am_message(AM_ALERT,mg_message(LG_INVALID_POINTER));
                        er_log(LG_INVALID_POINTER);
                        status = FAIL;
                        break;

                   case LG_INVALID_FORMAT:
                   case LG_NO_TRACE_NAME:
                   case LG_INVALID_BLANK:
                   case LG_NO_TRACE_FIELD:
                        am_message( AM_ALERT, 
                                    mg_message(LG_ED_INVALID_TRACE_NAME)); 
                        status = FAIL;
                        break;

                   default:
                        am_message( AM_ALERT, 
                                    mg_message(LG_ED_RENAME_FAILED));
                        er_log(
                    "LG_LTDM_RENAME_TRACE failed in LG_DB_RENAME_CDLG_SERVER");
                        er_log( mg_message(status) );
                        status = FAIL;
                        break;  
                 }
              }
            break;

      default:
            status = FAIL;
            break;
     }                                     /* end of switch construct         */


    if (status == SUCCESS)
      {
       status = am_release_resource( plg->db_rename_cdlg_id);
       status = lg_ed_db_menu_vis_on_off( plg, ON);       
       status = lg_ed_well_menu_vis_on_off( plg, ON);       
       status = lg_ed_edit_menu_vis_on_off( plg, ON);       
      }


    return SUCCESS;

}

/******************************************************************************/
/*                                                                            */
/*    Routine:        LG_DB_RENAME_CONFIRM                                    */
/*                                                                            */
/*    Function:       This routine brings up the confirmation dialog          */
/*                    for renaming the trace.                                 */
/*                                                                            */
/*    Author:         D. Mcleod                                               */
/*                                                                            */
/******************************************************************************/

#if USE_PROTOTYPES
static INT lg_db_rename_confirm( LOG_EDIT_STRUCT *plg, LOG_TRACE_NAME new_name)
#else
static INT lg_db_rename_confirm( plg, new_name)
LOG_EDIT_STRUCT  *plg;
LOG_TRACE_NAME   new_name;
#endif
{
    INT                status;
    INT                ID, item, value;
    INT                event_id;
    CHAR               text[128];          /* dummy array for wirqst          */

/******************************************************************************/


                                       /* set up dialog                       */
    fi_push();

    status = am_define_resource(AM_DIALOG, Resource_File,
                                LG_ED_REN_CON_CDLG, 
                                NULL_FUNCTION_PTR, 
                                &ID,           
                                NULL);

    
    status = wi_set_dialog_text( ID, LG_ED_REN_CON_OLD_NAME,
                                     plg->edit_trace);

    status = wi_set_dialog_text( ID, LG_ED_REN_CON_NEW_NAME, new_name);


                                           /* bring up dialog box             */
    status = wiopcd( ID); 

    for( status = FAIL; status != SUCCESS;   )
       {
        wi_request_dialog_item( &event_id, &item, &value, text);
        if (event_id == ID)
           { status = SUCCESS; }
        else
           { wibeep(1); }
        }

    switch (item)
      {
       case LG_ED_REN_CON_CANCEL:
            status = FAIL;
            break;

       case LG_ED_REN_CON_RENAME:
            status = SUCCESS;
            break;
      }

    am_release_resource( ID); 
    fi_pop();

    return status;
}
