/* DEC/CMS REPLACEMENT HISTORY, Element LG_DEL_CDLG.C*/
/* *4    17-JUL-1991 16:23:54 JANTINA "(SPR 0) Removed highlighting of first log in list"*/
/* *3    11-AUG-1989 11:22:57 JULIAN "(SPR -1) j maybe used before set"*/
/* *2    27-JUL-1989 22:25:24 GILLESPIE "(WBS 1.1) Fix minor casting problems"*/
/* *1    19-JUN-1989 13:04:13 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element LG_DEL_CDLG.C*/

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
#ifndef ESI_ER_H
#include "esi_er.h"
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
#ifndef ESI_FI_H
#include "esi_fi.h"
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
#ifndef ESI_MG_H
#include "esi_mg.h"
#endif
#ifndef LG_ED_MAIN_MENUS_RH
#include "lg_ed_main_menus.rh"
#endif
#ifndef LG_ED_DB_CDLGS_RH
#include "lg_ed_db_cdlgs.rh"
#endif

#define Resource_File  "LG_ED_DB_CDLGS"

#if USE_PROTOTYPES

static INT lg_db_delete_cdlg_server(INT id, INT item, LOG_EDIT_STRUCT *plg);
static INT lg_db_delete_conf_cdlg_init(LOG_EDIT_STRUCT *plg);
static INT lg_ed_erase_edit_uwi(LOG_EDIT_STRUCT *plg);

#else

static INT lg_db_delete_cdlg_server();
static INT lg_db_delete_conf_cdlg_init();
static INT lg_ed_erase_edit_uwi();

#endif


/******************************************************************************/
/*                                                                            */
/*    Routine:        LG_DB_DELETE_CDLG_INIT                                  */
/*                                                                            */
/*    Function:       This routine brings up a select list of log traces for  */
/*                    the current UWI. The user may select a trace or a group */
/*                    of traces to delete.                                    */
/*                                                                            */
/*    Author:         D. Mcleod                                               */
/*                                                                            */
/******************************************************************************/


publicdef INT lg_db_delete_cdlg_init( plg)

  LOG_EDIT_STRUCT  *plg;

{
    INT                status;
    INT                max_items = 100;

/******************************************************************************/

                                           /* set up trace selector dialog res*/

    status = am_define_resource(AM_DIALOG, Resource_File,
                                LG_ED_DEL_TRACES, 
                                lg_db_delete_cdlg_server, 
                                &plg->db_delete_cdlg_id,               
                                plg);


                                           /* set up selector control         */
    wisselt(plg->db_delete_cdlg_id, LG_ED_DEL_TRACES_SELECTOR,
            LG_ED_DEL_TRACES_VSCROLL, plg->num_traces,
            max_items, plg->trace_tcp);

                                           /* set default selec. to 1st item  */
/*    wi_set_default_sel( plg->db_delete_cdlg_id,
                        LG_ED_DEL_TRACES_SELECTOR, 1);  */

                                           /* bring up dialog box             */
    status = wiopcd(plg->db_delete_cdlg_id); 


    return status;

}






/******************************************************************************/
/*                                                                            */
/*    Routine:        LG_DB_DELETE_CDLG_SERVER                                */
/*                                                                            */
/*    Function:       Server for the select and delete trace routine.         */
/*                                                                            */
/*    Author:         D. Mcleod                                               */
/*                                                                            */
/******************************************************************************/


static INT lg_db_delete_cdlg_server( id, item, plg)

  INT               id, item;
  LOG_EDIT_STRUCT   *plg;

{
    INT                status, i;
    INT                items_selected[100];  /*array for return item          */
    CHAR               text[120];

/******************************************************************************/

    switch (item)
     {
                                       /* Cancel button pressed, exit     */
      case LG_ED_DEL_TRACES_CANCEL:                 
           status = SUCCESS;
           break;

      case LG_ED_DEL_TRACES_DELETE:        /* Retrieve button pressed         */
                                           /* query dialog for 1 selection    */

           wiqsel( plg->db_delete_cdlg_id, LG_ED_AT_SELECTOR,
                   &plg->num_traces_delete, items_selected);

           if ( plg->num_traces_delete == 0)
            {
             am_message ( AM_ALERT, mg_message(LG_ED_NO_TRACE_SELECTED) );
             break;
            }
           else                        /* Traces have been selected for 
                                          deletion, get confirmation from user*/
            {

             status = lg_db_delete_conf_cdlg_init( plg);

             if ( status == SUCCESS)
               {
                                       /* If delete confirmed then loop on 
                                          traces to delete                   */

                am_message( AM_DIALOG, mg_message(LG_ED_DELETING_TRACES) );

                for ( i = 0; i < plg->num_traces_delete; i++)
                  {
                    status = lg_ltdm_delete_trace( plg->edit_uwi,
                               plg->trace_tcp[items_selected[i] - 1]);        
                    if (status != SUCCESS)
                      {
                       sprintf( text, mg_message(LG_ED_TRACE_NOT_DELETED),
                                plg->trace_tcp[items_selected[i] - 1]);
                       am_message( AM_ALERT, text);
                       er_log( text );
                      }
                  }

					/* If all traces deleted from th DB,
					    delete the selection of cur_uwi from\
					    uwi_tcp. */

		if( plg -> num_traces EQUALS plg -> num_traces_delete)
		{
		    status = lg_ed_erase_edit_uwi(plg);
		}
		                                       /* Deletes performed okay,release dialog,
                                         reset num_traces, trace tcp         */

                status = SUCCESS;
                plg->num_traces = 0;
		if( plg -> trace_tcp IS_NOT_EQUAL_TO (CHAR **)NULL)
		{
		    ts_tcp_free( plg->trace_tcp);
		    plg -> trace_tcp = (CHAR **)NULL;
		}
			 
               } 
            } 
           break;
     }                                     /* end of switch construct         */
    

    am_message( AM_DIALOG, "");

    if ( status == SUCCESS)
     {
           status = am_release_resource( plg->db_delete_cdlg_id);
           status = lg_ed_db_menu_vis_on_off( plg, ON);
           status = lg_ed_well_menu_vis_on_off( plg, ON);
           status = lg_ed_edit_menu_vis_on_off( plg, ON);
     }

    return SUCCESS;

}






/******************************************************************************/
/*                                                                            */
/*    Routine:        LG_DB_DELETE_CONF_CDLG_INIT                             */
/*                                                                            */
/*    Function:       This routine brings up the delete confirmation dialog.  */
/*                    The use confirms his desire to delete n traces.         */
/*                                                                            */
/*    Author:         D. Mcleod                                               */
/*                                                                            */
/******************************************************************************/


static INT lg_db_delete_conf_cdlg_init( plg)
  LOG_EDIT_STRUCT  *plg;


{
    INT                status;
    INT                ID, item, value;
    CHAR               text[128];          /* dummy array for wirqst          */

/******************************************************************************/

                                       /* set up delete trace(s) dialog       */

    fi_push();

    status = am_define_resource(AM_DIALOG, Resource_File,
                                LG_ED_DEL_CONFIRM, 
                                NULL_FUNCTION_PTR, &ID, NULL);

                                       /* Put number of traces to delete in 
                                          prompt message                     */

    sprintf( text, mg_message(LG_ED_TRACES_FOR_DELETION),
                   plg->num_traces_delete);

    status = wi_set_dialog_text( ID, LG_ED_DEL_CONFIRM_MSG, text);

                                           /* bring up dialog box             */

    status = wiopcd( ID); 


    wi_request_dialog_item( &ID, &item, &value, text);

    switch (item)
      {
       case LG_ED_DEL_CONFIRM_CANCEL:
       status = FAIL;
       break;

       case LG_ED_DEL_CONFIRM_DELETE:
       status = SUCCESS;
       break;
      }



    am_release_resource( ID); 
    fi_pop();

    return status;

}

/* **********************************************************************

    ROUTINE: LG_ED_ERASE_EDIT_UWI

    DECRIPTION:
	When all traces belongs to editing uwi are deleted, the edit uwi
    will be erase from the TCP of UWIs.

    ************************************************************************/
static INT lg_ed_erase_edit_uwi(plg)
LOG_EDIT_STRUCT  *plg;
{
    INT i, j, done;
    
    for(i = 0, done = FALSE; i < plg -> num_uwis AND ! done; i++)
    {
	if(! strcmp(plg -> edit_uwi, plg -> uwi_tcp[i]) )
	{
	    tc_free(plg -> uwi_tcp[i]);
	    plg -> uwi_tcp[i] = (CHAR *)NULL;
	    done = TRUE;
	    for( j = i; j < plg -> num_uwis; j++)
	    {
		plg -> uwi_tcp[j] = plg -> uwi_tcp[j+1];
	    }
	}
    }
    plg -> num_uwis--;
    return SUCCESS;
}
