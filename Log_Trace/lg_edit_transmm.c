/* *5    12-AUG-1991 12:00:00 JEFF  "Added check for CurrentMin = CurrentMax"*/
/* DEC/CMS REPLACEMENT HISTORY, Element LG_EDIT_TRANS.C*/
/* *4    16-NOV-1989 17:10:13 JULIAN "(SPR 30) add nl_set_current_point before nl_replace_points"*/
/* *3    11-AUG-1989 12:27:19 JULIAN "(SPR -1) degenerate unsigned comparison, and statement not reached"*/
/* *2    27-JUL-1989 23:09:26 GILLESPIE "(SPR 1) Fix minor casting problems"*/
/* *1    19-JUN-1989 13:04:49 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element LG_EDIT_TRANS.C*/

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
#ifndef ESI_ORACLE_H
#include "esi_oracle.h"
#endif
#ifndef ESI_AM_H
#include "esi_am.h"
#endif
#ifndef ESI_MG_H
#include "esi_mg.h"
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
#ifndef ESI_NL_H
#include "esi_nl.h"
#endif
#ifndef ESI_LG_EDIT_H
#include "esi_lg_edit.h"
#endif
#ifndef ESI_LG_MSG_H
#include "esi_lg_msg.h"
#endif
#ifndef ESI_LG_ERR_H
#include "esi_lg_err.h"
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
#include "lg_ed_main_menus.rh"
#include "lg_ed_edit_cdlgs.rh"


#define Resource_File  "LG_ED_EDIT_CDLGS"

#if USE_PROTOTYPES

static INT lg_edit_linear_transform_mm( INT id, LOG_EDIT_STRUCT *plg);
static INT lg_edit_ltrans_cdlg_server_mm(INT id, INT item, LOG_EDIT_STRUCT *plg);

#else

static INT lg_edit_linear_transform_mm();
static INT lg_edit_ltrans_cdlg_server_mm();

#endif

/******************************************************************************/
/*                                                                            */
/*    Routine:        LG_EDIT_LTRANS_CDLG_INIT_MM                                */
/*                                                                            */
/*    Function:       This routine brings up a selector dialog for performing */
/*                    linear transformations (Y=MX+B) on selected traces.     */
/*                                                                            */
/*    Author:         D. Mcleod                                               */
/*                                                                            */
/******************************************************************************/


publicdef INT lg_edit_ltrans_cdlg_init_mm( plg)

  LOG_EDIT_STRUCT  *plg;

{
    INT                status;
    INT			id;
    CHAR		value[10];

/******************************************************************************/

                                           /* set up trace selector dialog res*/

    status = am_define_resource(AM_DIALOG, Resource_File,
                                LG_ED_LIN_TRANS_MIN_MAX, 
                                lg_edit_ltrans_cdlg_server_mm, 
                                &id,               
                                plg);

                                           /* set up selector control         */

    wisselt( id, LG_ED_LIN_MM_SELECTOR,
             LG_ED_LIN_MM_VSCROLL, plg->num_traces,
             1, plg->trace_tcp);

                                           /* set default selec. to 1st item  */
    wi_set_default_sel( id,
                        LG_ED_LIN_MM_SELECTOR, 1);


    /**************************************************************/
    /* set default value for current min&max and new min&max      */
    /**************************************************************/
    strcpy(value,"0.0");
    wi_set_dialog_text(id, LG_ED_LIN_MM_OLDMIN, value);
    wi_set_dialog_text(id, LG_ED_LIN_MM_NEWMIN, value);
    strcpy(value,"1.0");
    wi_set_dialog_text(id, LG_ED_LIN_MM_OLDMAX, value);
    wi_set_dialog_text(id, LG_ED_LIN_MM_NEWMAX, value);

                                           /* bring up dialog box             */
    status = wiopcd(id); 


    return status;
}






/******************************************************************************/
/*                                                                            */
/*    Routine:        LG_EDIT_LTRANS_CDLG_SERVER                              */
/*                                                                            */
/*    Function:       Server for the select trace to transfrom routine.       */
/*                                                                            */
/*    Author:         D. Mcleod                                               */
/*                                                                            */
/******************************************************************************/


static INT lg_edit_ltrans_cdlg_server_mm( id, item, plg)

  INT               id, item;
  LOG_EDIT_STRUCT   *plg;

{
    INT                status;

/******************************************************************************/

  switch (item)
    {   
      case LG_ED_LIN_MM_CANCEL:        /* Cancel button pressed, exit     */
           status = SUCCESS;
           break;
      case LG_ED_LIN_MM_EXECUTE:       /* Branch to copy routine          */
           status = lg_edit_linear_transform_mm( id, plg);
           break;
      default:                            /* Unrecognized event from AM.     */
           return SUCCESS;
     }                                    /* end of switch construct         */
    
    am_message( AM_DIALOG, "");

    if (status == SUCCESS )
      {
       status = am_release_resource( id);
       status = lg_ed_db_menu_vis_on_off( plg, ON);
       status = lg_ed_well_menu_vis_on_off( plg, ON);
       status = lg_ed_edit_menu_vis_on_off( plg, ON);
 
      }
                                                 /* Free trace structure      */
    status = lg_ltdm_free_trace( plg->ptrace); 


    return SUCCESS;

}

/******************************************************************************/
/*                                                                            */
/*    Routine:        LG_EDIT_LINEAR_TRANS_MM                                 */
/*                                                                            */
/*    Function:       Performs the actual linear transformation of the trace  */
/*                    ( Y = MX + B) by retrieving the existing trace, trans-  */
/*                    -forming the trace values, changing the trace name and  */
/*                    writing out the new trace.                              */
/*                                                                            */
/*    Author:         D. Mcleod                                               */
/*                                                                            */
/******************************************************************************/


static INT lg_edit_linear_transform_mm( id,plg)

  INT id;
  LOG_EDIT_STRUCT   *plg;

{
    INT                i, j, status;
    INT                num_items_ret;
    INT                value;
    CHAR               text[128];          /* Text array for dialog items     */
    LOG_TRACE          trace;              /* Entered trace  (DT,RHOB etc.)   */
    LOG_SERVICE        service;            /* Optionally entered service      */
    LOG_UOM            output_uom;         /* UOM of transformed trace        */
    DOUBLE             new_min, new_max, old_min, old_max; /* of trace        */
    DOUBLE             slope, intercept;   /* slope, intercept in transform   */
    static INT         items_selected[1];  /* array for return item           */
    UINT                ndim;
    UINT                Dim_List[2];        /* Log N-List crap                 */
    VOIDPTR            Value_List[2];
    UINT                npts, max_pts;
    UINT                nstrokes;
    FLOAT              null_value, null_hi, null_lo;
    FLOAT              *parray, *plog_value;

/******************************************************************************/

                                       /* Allocate space for the trace
                                         structure of the trace to be copied */

  plg->ptrace = ALLOCATE(LOG_TRACE_STRUCT);


  wiqsel( id, LG_ED_LIN_MM_SELECTOR,
                   &num_items_ret, items_selected);

  if ( num_items_ret < 1 )
     {
      status = am_message( AM_ALERT, mg_message(LG_ED_NO_TRACE_SELECTED) );
      return FAIL;
     }
                                       /* A trace has been picked             */
            
  strcpy( plg->edit_trace, plg->trace_tcp[items_selected[0] -1 ]);
  strcpy( plg->ptrace->trace_name, plg->edit_trace );
  strcpy( plg->ptrace->uwi, plg->edit_uwi );


                                       /* Fetch output trace from dialog.    */

  wi_query_dialog_item( id, LG_ED_LIN_MM_TRACE,
                        &value, text );
  ts_noblanks(text);
  if ( strlen(text) == 0 )
    {
     am_message( AM_ALERT, mg_message( LG_ED_ENTER_TRACE ) );
     return FAIL;
    }
  if ( strlen(text) >= sizeof( LOG_TRACE ) )
    {
     am_message( AM_ALERT, mg_message( LG_ED_TRACE_TOO_LONG ) );
     return FAIL;
    }
  strcpy( trace, text );

                                 /* Fetch 'service' from dialog.           */

  wi_query_dialog_item( id, LG_ED_LIN_MM_SERVICE,
                        &value, text );
  ts_noblanks(text);
  if ( strlen(text) >= sizeof( LOG_SERVICE ) )
    {
     am_message( AM_ALERT, mg_message( LG_ED_SERVICE_TOO_LONG ) );
     return FAIL;
    }
  strcpy( service, text );

                                 /* Fetch 'UOM' from dialog.                 */

  wi_query_dialog_item( id, LG_ED_LIN_MM_UNITS,
                        &value, text );
  if ( strlen(text) >= sizeof( LOG_UOM ) )
    {
     am_message( AM_ALERT, mg_message( LG_ED_UOM_TOO_LONG ) );
     return FAIL;
    }
  strcpy( output_uom, text );


                                       /* Now chk slope, intercept values    */

  /**********************************************************************/
  /* Error checking for blank input value				*/
  /**********************************************************************/
  wi_query_dialog_item( id, LG_ED_LIN_MM_OLDMIN,
                        &value, text );
  if (strlen(ts_noblanks(text)) == 0)
    {
    wi_beep(1);
    /*am_message (AM_ALERT, "Please enter Current minimum value.");*/
    /* move message to Data Base */
    am_message( AM_ALERT, mg_message( LG_ED_CURRENT_MIN ) );
    return (FAIL);
    }
  sscanf( text, "%lf", &old_min);    /* old values are double */

  wi_query_dialog_item( id, LG_ED_LIN_MM_OLDMAX,
                        &value, text );
  if (strlen(ts_noblanks(text)) == 0)
    {
    wi_beep(1);
    /*am_message (AM_ALERT, "Please enter Current maximum value.");*/
    /* move message to Data Base */
    am_message( AM_ALERT, mg_message( LG_ED_CURRENT_MAX ) );
    return (FAIL);
    }
  sscanf( text, "%lf", &old_max);    /* old values are double */

  wi_query_dialog_item( id, LG_ED_LIN_MM_NEWMIN,
                        &value, text );
  if (strlen(ts_noblanks(text)) == 0)
    {
    wi_beep(1);
    /*am_message (AM_ALERT, "Please enter New mimimum value.");*/
    /* move message to Data Base */
    am_message( AM_ALERT, mg_message( LG_ED_NEW_MIN ) );
    return (FAIL);
    }
  sscanf( text, "%lf", &new_min);

  wi_query_dialog_item( id, LG_ED_LIN_MM_NEWMAX,
                        &value, text );
  if (strlen(ts_noblanks(text)) == 0)
    {
    wi_beep(1);
    /*am_message (AM_ALERT, "Please enter New maximum value.");*/
    /* move message to Data Base */
    am_message( AM_ALERT, mg_message( LG_ED_NEW_MAX ) );
    return (FAIL);
    }
  sscanf( text, "%lf", &new_max);
  
  /*Don't let CurrentMin = CurrentMax to avoid divide by zero*/
  if ( old_min == old_max )
     {
    wi_beep(1);
    am_message( AM_ALERT, mg_message( LG_ED_OLDMIN_EQ_OLDMAX ) );
    return (FAIL);
    }

  slope = (new_min-new_max) / (old_min-old_max);

  intercept = new_min - slope * old_min;

                                      /* Get trace structure for the trace   */

  status = am_message( AM_DIALOG, mg_message( LG_ED_TRANSFORMING_TRACE ) );

  plg->ptrace->top =  0.0;
  plg->ptrace->base = 0.0;
  status = lg_ltdm_get_trace( plg->ptrace); 

  if ( status != SUCCESS)
     {
      status = am_message(AM_ALERT, mg_message(LG_ED_TRACE_NOT_READ) );
      return  FAIL;
     }


                                       /* Create new trace name from items    */

  if ( strlen( service ) > 0)
    {
     sprintf( plg->ptrace->trace_name, "LINTRANS:%s.%s",trace, service);
    }
  else
    {
     sprintf( plg->ptrace->trace_name, "LINTRANS:%s", trace);
    }
                                       /* Clean up trace name upcase, etc.    */

  ts_sto_upper( ts_noblanks( plg->ptrace->trace_name));
          

                                /* Determine the size of the array which      */
                                /* must be allocated to hold the largest      */
                                /* stroke.                                    */

  status = nl_inq_nlist_int(plg->ptrace->ptrace,NL_NLIST_NSTROKES,(INT *)&nstrokes);
  if (status < SUCCESS)
     {
      er_log("NL_INQ_NLIST_INT failed in LG_EDIT_LINEAR_TRANS.");
      er_log( mg_message( status ) );
      am_message( AM_ALERT, mg_message(LG_ED_BAD_INPUT_TRACE) );
      return FAIL;
     }

  max_pts = 0;
  for (i=1; i<=nstrokes; i++)
     {
      status = nl_inq_stroke_int(plg->ptrace->ptrace,i,NL_STROKE_NPOINTS,
	    (INT *)&npts);
      if (status < SUCCESS)
         {
          er_log("NL_INQ_STROKE_INT failed in LG_EDIT_LINEAR_TRANS.");
          er_log( mg_message( status ) );
          am_message( AM_ALERT, mg_message(LG_ED_BAD_INPUT_TRACE) );
          return FAIL;
         }
      if ( npts > max_pts )
         { max_pts = npts; }
     }

                                    /* We now know how big an array to      */
                                    /* allocate, so let's do it!            */

  parray = (FLOAT *)am_allocate(AM_APPLICATION, max_pts * sizeof(FLOAT) );
  if ( parray == (FLOAT *)NULL )
     {
      er_log("AM_ALLOCATE returned a null pointer in LG_EDIT_LINEAR_TRANS.");
      am_message(AM_ALERT, mg_message( LG_ED_OUT_OF_MEMORY ) );
      return FAIL;
     }



                                       /* Get null value & set test window.   */

  ndim = 1;
  Dim_List[0] = 2;
  Value_List[0] = ( VOIDPTR) &null_value;

  status = nl_inq_nlist_info( plg->ptrace->ptrace, NL_NLIST_NULL_VALUE,
                              ndim, Dim_List, Value_List );
  if ( null_value < 0.0 )
     {
      null_hi = null_value * 0.9999;
      null_lo = null_value * 1.0001;
     }
  else
     {
      null_hi = null_value * 1.0001;
      null_lo = null_value * 0.9999;
     }


                                        /*  Load the array, one stroke at a  */
                                        /*  time.  Then step thru the array  */
                                        /*  converting non null values.      */
                                        /*  Finally, replace the array into  */
                                        /*  the original stroke and go on    */
                                        /*  to the next stroke.              */

  Value_List[0] = ( VOIDPTR ) parray;
  for ( i=1; i<=nstrokes; i++ )
     {                                  /*  Grab a stroke, put it in array   */

      status = nl_set_current_point( plg->ptrace->ptrace, 1, i );
      status = nl_get_points( plg->ptrace->ptrace, max_pts, ndim,
                              Dim_List, Value_List, &npts );

                                        /*  Linear transform if not null.    */

      for ( plog_value=parray, j=0; j<npts; plog_value++, j++ )
         {
          if ( *plog_value > null_hi OR *plog_value < null_lo )
             {  *plog_value = *plog_value * slope + intercept;  }
         }

                                        /*  Put array back into stroke.      */
      status = nl_set_current_point( plg->ptrace->ptrace, 1, i );
      status = nl_replace_points( plg->ptrace->ptrace, npts, ndim,
                                  Dim_List, Value_List );
     }

  am_free( parray );

                                       /* Correct the Trace UOM.              */

  strcpy( plg->ptrace->trace_uom, output_uom );


                                       /* Identify how this trace came about  */
                                       /* in the comments field.              */

  sprintf(plg->ptrace->comments,"%s * %lf + %lf", 
                          plg->edit_trace, slope, intercept );  


                                       /* Now attempt to add back de trace
                                          to de database, bro' (honky mother) */

  status = lg_ltdm_add_trace( plg->ptrace );

  switch (status)
    {
    case SUCCESS:
         plg->num_traces = 0;
         status = ts_tcp_free( plg->trace_tcp );
         status = SUCCESS;
         break;
    case LG_INVALID_FORMAT:
    case LG_NO_TRACE_NAME:
    case LG_INVALID_BLANK:
    case LG_NO_TRACE_FIELD:
         am_message( AM_ALERT, mg_message( LG_ED_INVALID_TRACE_NAME ) );
         status = FAIL;
         break;
    default:
         am_message( AM_ALERT, mg_message( LG_ED_TRACE_NOT_WRITTEN ) );
         er_log("LG_LTDM_ADD_TRACE failed in LG_EDIT_LINEAR_TRANS.");
         er_log( mg_message( status ) );
         status = FAIL;
    }

  return status;

}
