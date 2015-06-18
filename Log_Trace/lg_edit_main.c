/* DEC/CMS REPLACEMENT HISTORY, Element LG_EDIT_MAIN.C*/
/* *5    17-JUL-1991 16:32:09 JANTINA "(SPR 0) Added linear transform(min,max) and splice"*/
/* *4    23-AUG-1990 17:51:25 JESSIE "(SPR 1) fix include files"*/
/* *3    17-OCT-1989 10:10:39 GORDON "(SPR 999) Add am_quit calls when failing, return SUCCESS always"*/
/* *2    21-SEP-1989 11:42:08 GILLESPIE "(SPR 101) Change lg_get_priviledges to lg_get_privileges"*/
/* *1    19-JUN-1989 13:04:45 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element LG_EDIT_MAIN.C*/

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
#ifndef ESI_ERRLEVEL_H
#include "esi_errlevel.h"
#endif
#ifndef ESI_WI_H
#include "esi_wi.h"
#endif
#ifndef LG_ED_MAIN_MENUS_RH
#include "lg_ed_main_menus.rh"
#endif
#ifndef ESI_LG_EDIT_H
#include "esi_lg_edit.h"
#endif
#ifndef ESI_LG_LDEFS_H
#include "esi_lg_ldefs.h"
#endif
#ifndef ESI_LG_MSG_H
#include "esi_lg_msg.h"
#endif
#ifndef ESI_MG_H
#include "esi_mg.h"
#endif
#ifndef ESI_TS_H
#include "esi_ts.h"
#endif

/* Define Entry points for servers, etc.    */

static INT lg_init_main_menus();
static INT lg_edit_menu_server();
static INT lg_db_menu_server();
static INT lg_well_menu_server();
static INT lg_msg_cdlg_server();
publicdef  INT lg_ed_get_log_traces();
static INT lg_edit_quit();

static CHAR Resource_File[] = "LG_ED_MAIN_MENUS";

/* ************************************************************************** */
/*                                                                            */
/*   ROUTINE:      LG_EDIT_MAIN                                               */
/*                                                                            */
/*   FUNCTION:     This routine is the top level routine for log editing.     */
/*                 User authorization checks are performed, and the main      */
/*                 log edit menus are initialized.                            */
/*							                      */
/* ************************************************************************** */

INT  lg_edit_main(param)
BYTE *param;
{
    LOG_EDIT_STRUCT    *plg;
    INT                i, status;
    CHAR               message[120];

/* ************************************************************************** */

/* Allocate Memory for LG_STRUCT (AM_APPLICATION so is freed automatically)   */

    plg = (LOG_EDIT_STRUCT *)am_allocate( AM_APPLICATION, 
        sizeof( LOG_EDIT_STRUCT));

    status = am_declare( "Log Trace Editing");


/* Check User privileges and inform user if not sufficient                   */

    am_message( AM_DIALOG, mg_message(LG_ED_CHECKING_PRIV) );

    status = lg_get_privileges( plg);

    am_message( AM_DIALOG, "");

    if ( status != SUCCESS) 
       {
	 am_quit();
         return SUCCESS;
       }

    if ( plg->select_priv != 1)
       {
         am_message(AM_ALERT, mg_message(LG_ED_NO_SELECT) );
	 am_quit();
         return SUCCESS;
       }

    if ( plg->delete_priv != 1 || plg->update_priv != 1
         || plg->insert_priv != 1)
       {
         strcpy ( message, mg_message(LG_ED_NOT_ALL_PRIV) );
         sprintf( message, "%s  %s", message,  
                    mg_message(LG_ED_MENU_ITEMS_DIMMED));
         am_message( AM_ALERT, message );
       }

                   
/* Initialize the main menus, set visibilities etc.                         */

    status = lg_init_main_menus( plg);
    return SUCCESS;
}

/***************************************************************************/
/*                                                                         */
/*  Routine:   LG_INIT_MAIN_MENUS                                          */
/*                                                                         */
/*  Function:  Define and open the main menus.                             */
/*                                                                         */
/*  Returns:   SUCCESS, FAIL                                               */
/*                                                                         */
/***************************************************************************/

static INT lg_init_main_menus( plg )

    LOG_EDIT_STRUCT  *plg;
   {
    INT		status;
    INT         id;
    INT         num;

/***************************************************************************/

/* define LOGEDIT resource server                                             */

    status = am_define_resource(AM_MENU,Resource_File, LG_EDIT_MENU,
                                lg_edit_menu_server,
                                &plg->log_edit_menu_id,
                                plg);

                                     /* Temporarily set all but QUIT off since
                                        we have no routines yet            */

    status = lg_ed_edit_menu_vis_on_off( plg, OFF);   


/* define LOGDB resource server                                               */


    status = am_define_resource(AM_MENU, Resource_File, LG_DB_MENU,
                                lg_db_menu_server,
                                &plg->db_menu_id,
                                plg);

                                       /* Leave all items off since no UWI     
                                          has been selected yet             */ 

    status = lg_ed_db_menu_vis_on_off( plg, OFF);
       


/* define WELL resource, server                                               */

    status = am_define_resource(AM_MENU, Resource_File, LG_WELL_MENU,
                                lg_well_menu_server,
                                &plg->well_menu_id,
                                plg);

                                       /* Leave all but enter UWI off for now */

    status = lg_ed_well_menu_vis_on_off( plg, ON);


/* Define Message Window Dialog and copy crap in to it for now              */

    status = am_define_resource(AM_DIALOG, Resource_File,
                                LG_ED_MAIN_CDLG,
                                NULL_FUNCTION_PTR,
                                &plg->msg_cdlg_id,
                                NULL);



/* Set menu visibilities and message dialog ON                              */

    wivismn(plg->log_edit_menu_id, ON);
    wivismn(plg->db_menu_id, ON);
    wivismn(plg->well_menu_id, ON);
    wi_open_dialog( plg->msg_cdlg_id);


    return status;
    }

/*************************************************************************/
/*                                                                       */
/*  Routine:     LG_EDIT_MENU_SERVER                                     */
/*                                                                       */
/*  Function:    Receives events from the LogEdit menu and launches      */
/*               the appropriate process.                                */
/*                                                                       */
/*  Returns:     SUCCESS                                                 */
/*                                                                       */
/*************************************************************************/
    
static INT lg_edit_menu_server( id, item, plg )

    INT              id;
    INT              item;
    LOG_EDIT_STRUCT  *plg;
    {
    INT status;

/*************************************************************************/

/* First check if any traces for this well, if not issue message, set menu items
   off so user cannot select again.                                           */

    status = lg_ed_get_log_traces( plg);

    if (status != SUCCESS && item != LG_EDIT_QUIT )
      {
       am_message( AM_ALERT, mg_message(LG_ED_NO_TRACES_THIS_WELL) );

       status = lg_ed_db_menu_vis_on_off( plg, OFF);
       status = lg_ed_edit_menu_vis_on_off( plg, OFF);
       return SUCCESS;     
      }

/* Turn off other db menu items, Edit menu items and the get UWI items so user
   cannot switch UWI's or change current trace tcp while mucking with dbase  */

    status = lg_ed_well_menu_vis_on_off( plg, OFF);
    status = lg_ed_db_menu_vis_on_off( plg, OFF);
    status = lg_ed_edit_menu_vis_on_off( plg, OFF);


    switch(item) {

          case LG_EDIT_LINEAR:
             status = lg_edit_ltrans_cdlg_init( plg );
             break;                                                    

          case LG_EDIT_LINEAR_MM:
             status = lg_edit_ltrans_cdlg_init_mm( plg );
             break;                                                    

	  case LG_EDIT_SPLICE:
	     status = lg_edit_splice_cdlg_init( plg);
	     break;

/*        case LG_EDIT_DESPIKE:
             status = lg_edit_despike( plg );
             break;                                                    */

/*        case LG_EDIT_FILTERS:
	     	 status = lg_edit_filters( plg );
             break;                                                    */

/*        case LG_EDIT_DEPTH_ADJUST:
             status = lg_edit_depth_adjust( plg );
             break;                                                    */

        case LG_EDIT_QUIT:
             status = lg_edit_quit( plg );
             break;
        }

     return SUCCESS;
   }

/*************************************************************************/
/*                                                                       */
/*  Routine:   LG_DB_MENU_SERVER                                         */
/*                                                                       */
/*  Function:  Receives events from the LogDB menu and launches the      */
/*             appropriate process.                                      */
/*                                                                       */
/*  Returns:   SUCCESS                                                   */
/*                                                                       */
/*************************************************************************/
    
static INT lg_db_menu_server( id, item, plg )

  INT             id;
  INT             item;
  LOG_EDIT_STRUCT *plg;
 {

    INT status;
/*************************************************************************/


/* First check if any traces for this well, if not issue message, set menu items
   off so user cannot select again.                                           */

    status = lg_ed_get_log_traces( plg);

    if (status != SUCCESS)
      {
       am_message( AM_ALERT, mg_message(LG_ED_NO_TRACES_THIS_WELL) );

       status = lg_ed_db_menu_vis_on_off( plg, OFF);
       status = lg_ed_edit_menu_vis_on_off( plg, OFF);
       return SUCCESS;     
      }


/* Turn off other db menu items and the get UWI items so user cannot switch
   UWI's or trace from other tcp's while mucking with dbase                */

    status = lg_ed_well_menu_vis_on_off( plg, OFF);
    status = lg_ed_db_menu_vis_on_off( plg, OFF);
    status = lg_ed_edit_menu_vis_on_off( plg, OFF);


    switch(item) {

        case LG_DB_MENU_SHOW:
             status = lg_db_info_cdlg_init( plg );
             break;

        case LG_DB_MENU_RENAME:
             status = lg_db_rename_cdlg_init( plg );
	     break;                                  

        case LG_DB_MENU_COPY:
             status = lg_db_copy_cdlg_init( plg);
             break;                                   

        case LG_DB_MENU_DELETE:
             status = lg_db_delete_cdlg_init( plg);
             break;                                                 

        }                                               

     return SUCCESS;
   }

/*************************************************************************/
/*                                                                       */
/*  Routine:   LG_WELL_MENU_SERVER                                       */
/*                                                                       */
/*  Function:  Receives events from the Log wells menu & launches the    */
/*             correct process.                                          */
/*                                                                       */
/*  Returns:   SUCCESS                                                   */
/*                                                                       */
/*************************************************************************/
    
static INT lg_well_menu_server( id, item, plg )

  INT                id;
  INT                item;
  LOG_EDIT_STRUCT    *plg;
 {
    INT              status;

/*************************************************************************/

                                       /* Turn off db menu items temporarily*/

    status = lg_ed_db_menu_vis_on_off( plg, OFF);
    status = lg_ed_edit_menu_vis_on_off( plg, OFF);


    switch(item) {

        case LG_WELL_ENTER_UWI:
	     status = lg_well_get_uwi( plg );                    
	     break;

        }

        if (status == SUCCESS)
         {
                                       /* Copy UWI into  message window     */

          plg->valid_edit_uwi = TRUE;
          wiscdt( plg->msg_cdlg_id, LG_ED_MAIN_UWI, plg->edit_uwi);


         }

                                       /* If there are traces for this UWI
                                         then highlight db menu items       */
     if (plg->num_traces > 0)
       {
        status = lg_ed_db_menu_vis_on_off( plg, ON);
        status = lg_ed_edit_menu_vis_on_off( plg, ON);
       }

     return SUCCESS;
   }

/******************************************************************************/
/*                                                                            */
/*    Routine:        LG_ED_DB_MENU_VIS_ON_OFF()                              */
/*                                                                            */
/*    Function:       This routine turns appropriate db menu items on or off  */
/*                    according to the input flag which is set off if there   */
/*                    are no traces for the current UWI.                      */
/*                                                                            */
/*    Author:         D. Mcleod                                               */
/*                                                                            */
/******************************************************************************/

publicdef INT lg_ed_db_menu_vis_on_off( plg, vis_flag)

  LOG_EDIT_STRUCT  *plg;
  INT              vis_flag;

{
    INT                status;

/******************************************************************************/

    wienmni(plg->db_menu_id, LG_DB_MENU_SHOW, vis_flag);

    if ( plg->update_priv == 1)
      {
       wienmni(plg->db_menu_id, LG_DB_MENU_RENAME, vis_flag);
      }


    if ( plg->insert_priv == 1)
      {
       wienmni(plg->db_menu_id, LG_DB_MENU_COPY, vis_flag);
      }


    if ( plg->delete_priv == 1)
      {
       wienmni(plg->db_menu_id, LG_DB_MENU_DELETE, vis_flag);
      }

    return SUCCESS;

}

/******************************************************************************/
/*                                                                            */
/*    Routine:        LG_ED_WELL_MENU_VIS_ON_OFF()                            */
/*                                                                            */
/*    Function:       This routine turns appropriate well menu items on or off*/
/*                    according to the input flag which is set off if there   */
/*                    are no traces for the current UWI.                      */
/*                                                                            */
/*    Author:         D. Mcleod                                               */
/*                                                                            */
/******************************************************************************/

publicdef INT lg_ed_well_menu_vis_on_off( plg, vis_flag)

  LOG_EDIT_STRUCT  *plg;
  INT              vis_flag;

{
    INT                status;

/******************************************************************************/

    wienmni(plg->well_menu_id, LG_WELL_ENTER_UWI, vis_flag);

    return SUCCESS;

}

/******************************************************************************/
/*                                                                            */
/*    Routine:        LG_ED_EDIT_MENU_VIS_ON_OFF()                            */
/*                                                                            */
/*    Function:       This routine turns appropriate edit menu items on or off*/
/*                    according to the input flag which is set off if there   */
/*                    are no traces for the current UWI.                      */
/*                                                                            */
/*    Author:         D. Mcleod                                               */
/*                                                                            */
/******************************************************************************/


publicdef INT lg_ed_edit_menu_vis_on_off( plg, vis_flag)

  LOG_EDIT_STRUCT  *plg;
  INT              vis_flag;

{

/******************************************************************************/

    wienmni(plg->log_edit_menu_id, LG_EDIT_LINEAR, vis_flag);
    wienmni(plg->log_edit_menu_id, LG_EDIT_LINEAR_MM, vis_flag);
    wienmni(plg->log_edit_menu_id, LG_EDIT_SPLICE, vis_flag);
    wienmni(plg->log_edit_menu_id, LG_EDIT_QUIT, ON);
/* despike, filter and depth adjust should be here */

    return SUCCESS;

}

/******************************************************************************/
/*                                                                            */
/*    Routine:        LG_ED_GET_LOG_TRACES                                    */
/*                                                                            */
/*    Function:       This routine checks if the number of traces for the     */
/*                    current UWI is = 0 and if so an LTDM call if performed  */
/*                    to get the trace list. If the LTDM call finds no traces */
/*                    then the user is informed.                              */
/*                                                                            */
/*    Author:         D. Mcleod                                               */
/*                                                                            */
/******************************************************************************/


publicdef INT lg_ed_get_log_traces( plg)

  LOG_EDIT_STRUCT  *plg;

{
    INT                status;

/******************************************************************************/

    if (plg->num_traces > 0) return SUCCESS;

                                           /* call LTDM for list of traces    */

    status = lg_ltdm_inq_well_trace( plg->edit_uwi, 
                                     "",               /* call with null SQL  */
                                     &plg->trace_tcp,  /* list ret. in a TCP  */
                                     &plg->num_traces);/* num traces in list  */


                                           /* check no returned traces        */
    if ( plg->num_traces == 0)
      {
       return (FAIL);
      }


    return status;

}

/**************************************************************************/
/*                                                                        */
/*  Routine:  LG_EDIT_QUIT                                                */
/*                                                                        */
/*  Function: Quit Log Editing.  Check if an edited trace exists that has */
/*            not been saved and ask user if saving required.             */
/*                                                                        */
/*  Returns:  SUCCESS.                                                    */
/*                                                                        */
/**************************************************************************/

static INT lg_edit_quit( plg)

   LOG_EDIT_STRUCT  *plg;
   {
    INT      status;

    status = am_quit();

    return SUCCESS;
    }

