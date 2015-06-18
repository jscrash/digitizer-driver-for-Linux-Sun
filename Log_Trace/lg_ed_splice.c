/* DEC/CMS REPLACEMENT HISTORY, Element LG_ED_SPLICE.C */
/* *2    17-JUL-1991 16:40:04 JANTINA "(SPR 0) ." */
/* *1    17-JUL-1991 16:39:10 JANTINA "Log splicing routine" */
/* DEC/CMS REPLACEMENT HISTORY, Element LG_ED_SPLICE.C */
/******************************************************************************

                Copyright Exploration Systems, Inc. 1989
		   Unpublished -- All rights reserved

THIS SOFTWARE IS THE PROPRIETARY PROPERTY OF Exploration Systems, Inc.  AND MAY
CONTAIN CONFIDENTIAL TRADE SECRET INFORMATION. IT IS LICENSED FOR USE ON THE
DESIGNATED EQUIPMENT ON WHICH IT WAS ORIGINALLY INSTALLED AND  MAY NOT BE
MODIFIED, DUPLICATED OR COPIED IN ANY FORM WITHOUT PRIOR WRITTEN CONSENT OF

			Finder Graphics Systems, Inc.
			201 Tamal Vista Blvd.
			Corte Madera, CA
			(415) 927-0100

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

#ifndef ESI_TC_H
#include "esi_tc.h"
#endif

#ifndef ESI_MG_H
#include "esi_mg.h"
#endif

#ifndef ESI_NL_H
#include "esi_nl.h"
#endif

#include "lg_ed_edit_cdlgs.rh"

static INT lg_ed_splice_conf();
publicdef INT lg_edit_splice_work();
#define Resource_File  "LG_ED_EDIT_CDLGS"

#if USE_PROTOTYPES
static INT lg_edit_splice_server(INT id, INT item, LOG_EDIT_STRUCT * plg);

#else
static INT lg_edit_splice_server();

#endif

publicdef INT lg_edit_splice_cdlg_init(plg)
LOG_EDIT_STRUCT *plg;

{
	INT     status;
	INT     id;

	/* bring up the splice dialog */
	status = am_define_resource(AM_DIALOG, Resource_File, LG_ED_SPLICE,
				    lg_edit_splice_server, &id, plg);

	wiopcd(id);

	return SUCCESS;

}

static INT lg_edit_splice_server(id, item, plg)
INT     id;
INT     item;
LOG_EDIT_STRUCT *plg;

{
	INT     junk;
	CHAR    text[STMAX];
	LOG_TRACE_NAME trace;
	LOG_TRACE_NAME service;
	INT     status = SUCCESS;
	DOUBLE  conv;

	switch (item)
	{
	    case LG_ED_CANCEL:
		/* we changed our minds */
		status = am_release_resource(id);
		status = lg_ed_db_menu_vis_on_off( plg, ON);
		status = lg_ed_well_menu_vis_on_off( plg, ON);
		status = lg_ed_edit_menu_vis_on_off( plg, ON);
		break;

	    case LG_ED_PROCEED:
		/* allocate trace structures for splicing */
		plg->ptrace = ALLOCATE(LOG_TRACE_STRUCT);
		plg->ptrace_2 = ALLOCATE(LOG_TRACE_STRUCT);
		
		/* get trace name for output */
		status = wiqccdi(id, LG_ED_TRACE_OUT, &junk, text);
		ts_noblanks(text);
		ts_sto_upper(text);
		strcpy(trace, text);
		if(strlen(trace) > 12)
		{
		    sprintf(text,"Trace name %s too long",trace);
		    am_message(AM_ALERT,text);
		    break;
		}

		/* get trace service for output */
		status = wiqccdi(id, LG_ED_SERVICE_OUT, &junk, text);
		ts_noblanks(text);
		ts_sto_upper(text);
		strcpy(service, text);
		if(strlen(service) > 12)
		{
		    sprintf(text,"Service name %s too long",service);
		    am_message(AM_ALERT,text);
		    break;
		}
		
		if (strlen(service) > 0)
		{
			sprintf(plg->edit_trace, "SPLICE:%s.%s", trace, service);
		}
		else
		{
			sprintf(plg->edit_trace, "SPLICE:%s", trace);
		}

		/* this is where we store the name until needed */

		ts_sto_upper(plg->edit_trace);

		/* get first name (higher priority), user gets this from db menu */
		status = wiqccdi(id, LG_ED_TRACE1_IN, &junk, text);
		ts_noblanks(text);
		ts_sto_upper(text);
		strcpy(plg->splice_log_1, text);
		status = lg_ltdm_inq_trace_struct(plg->edit_uwi,
				plg->splice_log_1,"",plg->ptrace);
        	if (status != SUCCESS)
                {
                  sprintf(text,"%s not found",plg->splice_log_1);
                  am_message( AM_ALERT, text);
                  return FAIL;
                }

/* special note. if this is null then we are nulling out log */

		status = wiqccdi(id, LG_ED_TRACE2_IN, &junk, text);
		ts_noblanks(text);
		ts_sto_upper(text);
		strcpy(plg->splice_log_2, text);
		if(strlen(plg->splice_log_2) != 0)
		{
		  status = lg_ltdm_inq_trace_struct(plg->edit_uwi,
				plg->splice_log_2,"",plg->ptrace_2);
        	  if (status != SUCCESS)
                  {
                    sprintf(text,"%s not found",plg->splice_log_2);
                    am_message( AM_ALERT, text);
                    return FAIL;
                  }
          	  if(strlen(plg->ptrace->depth_uom) == 0
		    OR strlen(plg->ptrace_2->depth_uom) == 0
            	    OR tu_unit_conversion(1.0,plg->ptrace->depth_uom,
                	&conv,plg->ptrace_2->depth_uom) != SUCCESS)
          	  {
            	    am_message(AM_ALERT,"Unable to validate depth units for input traces");
            	    return FAIL;
          	  }
          	  else if(conv != 1.0)
          	  {
            	    am_message(AM_ALERT,"Depth units of input traces must be the same");
            	    return FAIL;
          	  }

          	  if(strlen(plg->ptrace->trace_uom) == 0
		    OR strlen(plg->ptrace_2->trace_uom) == 0
            	    OR tu_unit_conversion(1.0,plg->ptrace->trace_uom,
                	&conv,plg->ptrace_2->trace_uom) != SUCCESS)
          	  {
            	    am_message(AM_ALERT,"Unable to validate trace units for input traces");
            	    return FAIL;
          	  }
          	  else if(conv != 1.0)
          	  {
            	    am_message(AM_ALERT,"Trace units of input traces must be the same");
            	    return FAIL;
          	  }

		}

		/* get the interval to splice in/out */
		status = wiqccdi(id, LG_ED_LOW_INTVL, &junk, text);
		plg->splice_2_top = atof(text);

		status = wiqccdi(id, LG_ED_HI_INTVL, &junk, text);
		plg->splice_2_base = atof(text);


		if(strlen(plg->splice_log_2) != 0)

		/* Splicing two curves so .. */
        	/* Make sure splice interval is within trace 2 */

		{
          	  if (plg->splice_2_top < plg->ptrace_2->top)
                  {
                    sprintf(text,"Interval top specified is outside Trace 2 \
			(%.4f - %.4f)",plg->ptrace_2->top,plg->ptrace_2->base);
                    am_message( AM_ALERT, text);
                    return FAIL;
                  }
          	  else if (plg->splice_2_base > plg->ptrace_2->base)
                  {
                    sprintf(text,"Interval base specified is outside Trace 2 \
 			(%.4f - %.4f)",plg->ptrace_2->top,plg->ptrace_2->base);
                    am_message( AM_ALERT, text);
                    return FAIL;
                  }
		}
		else

		/* Placing nulls in one curve so .. */
		/* Make sure splice interval is within trace 1 */

                {
                  if (plg->splice_2_top < plg->ptrace->top)
                  {
                    sprintf(text,"Interval top specified is outside Trace 1 \
                        (%.4f - %.4f)",plg->ptrace->top,plg->ptrace->base);
                    am_message( AM_ALERT, text);
                    return FAIL;
                  }
                  else if (plg->splice_2_base > plg->ptrace->base)
                  {
                    sprintf(text,"Interval base specified is outside Trace 1 \
                        (%.4f - %.4f)",plg->ptrace->top,plg->ptrace->base);
                    am_message( AM_ALERT, text);
                    return FAIL;
                  }
                }


	/* if everybody happy, then... */

		if ((status = lg_ed_splice_conf(plg)) == SUCCESS)
		  status = lg_edit_splice_work(plg);

		if (status == SUCCESS)
			{
			status = am_release_resource(id);
			status = lg_ed_db_menu_vis_on_off( plg, ON);
			status = lg_ed_well_menu_vis_on_off( plg, ON);
			status = lg_ed_edit_menu_vis_on_off( plg, ON);
			}
		
		nl_free_nlist(plg->ptrace->ptrace);
		if (strlen(plg->splice_log_2) > 0)
			nl_free_nlist(plg->ptrace_2->ptrace);
		tc_free( plg->ptrace);
		tc_free( plg->ptrace_2);

		break;

	}

	return SUCCESS;

}

/* routine to do the work of the splicing after dialog stuff is processed */

publicdef INT lg_edit_splice_work(plg)
LOG_EDIT_STRUCT *plg;

{

	INT     status;
	INT 	i;
	INT 	nrows;		/* number of row to replace (calculated) */
	INT	cur_row;	/* number of rows to skip over (calculated) */
	UINT	col_list[1];
	VOIDPTR var_list[1];
	FLOAT 	null_val;	/* hold null value from log */
	BOOL 	null_out;	/* null-out vs. splice-in flag */
	FLOAT 	*replace_vals;	/* place to load replacement values */
	UINT 	ngot;		/* actual rows retrieved */
	INT     base_row;
	INT     splice_start;
 	INT	nnulls;
	FLOAT   start_value;
	CHAR    text[STMAX];

	/* get first trace into memory */
	strcpy(plg->ptrace->trace_name, plg->splice_log_1);
	strcpy(plg->ptrace->uwi, plg->edit_uwi);
	plg->ptrace->top = plg->splice_1_top = 0.0;	/* always 0.0 unless dialog changed */
	plg->ptrace->base = plg->splice_1_base = 0.0;
	plg->ptrace->multi_strokes = FALSE;	/* this will return a one stroke nlist */
	plg->ptrace->non_append_null = TRUE;	/* this will not have junk on either end */
	sprintf(text,"Retrieving trace %s",plg->ptrace->trace_name);
	am_message(AM_DIALOG,text);
	status = lg_ltdm_get_trace( plg->ptrace);
	if (status != SUCCESS)
		{
		sprintf(text,"%s not found",plg->ptrace->trace_name);
		am_message( AM_ALERT, text);
		return FAIL;
		}

	/* get the second trace into memory */
	plg->ptrace_2->top = 0.0;
	plg->ptrace_2->base = 0.0;
	/* use same increment - resample if necessary */
	plg->ptrace_2->dinc = plg->ptrace->dinc;

	/* if the trace 2 name is null we are only going to be deleting data */

	if (strlen(plg->splice_log_2) == 0)
	{
	  null_out = TRUE;
	}
	else
	{
	  /* get the second log */
	  null_out = FALSE;
	  strcpy(plg->ptrace_2->uwi, plg->edit_uwi);
	  strcpy(plg->ptrace_2->trace_name,plg->splice_log_2);
	  plg->ptrace_2->multi_strokes = FALSE;
	  plg->ptrace_2->non_append_null = TRUE;
	  sprintf(text,"Retrieving trace %s",plg->ptrace_2->trace_name);
	  am_message(AM_DIALOG,text);
	  status = lg_ltdm_get_trace_resamp( plg->ptrace_2);
	  if (status != SUCCESS)
	  {
	    sprintf(text,"%s not found",plg->ptrace_2->trace_name);
	    am_message( AM_ALERT, text);
	    return FAIL;
	  }
	}

	if (null_out)
	{
	    sprintf(text,"Creating %s",plg->edit_trace);
	    am_message(AM_DIALOG,text);

	    /* we simply locate interval and remove points (by setting null) */
		
	    cur_row = ((plg->splice_2_top - plg->ptrace->non_null_top ) /
					plg->ptrace->dinc) + 1.0;
	    if(ABS(plg->ptrace->non_null_top + (cur_row - 1)*plg->ptrace->dinc
		 - plg->splice_2_top) > plg->ptrace->dinc/2)
		cur_row++;
	    base_row = ((plg->splice_2_base - plg->ptrace->non_null_top ) /
					plg->ptrace->dinc) + 1.0; 
	    if(ABS(plg->ptrace->non_null_top + (base_row - 1)*plg->ptrace->dinc
		 - plg->splice_2_base) > plg->ptrace->dinc/2)
		base_row++;

	    /* now figure out how many to null out */

	    nrows = base_row - cur_row + 1;

	    col_list[0] = 2;
	    var_list[0] = (VOIDPTR) &null_val;

	    /* get the null value for the log */

	    status = nl_inq_nlist_info(plg->ptrace->ptrace, NL_NLIST_NULL_VALUE, 1, col_list,
			 var_list);

	    for (i=0; i<nrows; i++, cur_row++)
	    {
		status = nl_set_current_row( plg->ptrace->ptrace, cur_row, 1);
		status = nl_replace_rows( plg->ptrace->ptrace, 1, 1, col_list, var_list);
	    }
	    strcpy(plg->ptrace->trace_name, plg->edit_trace);
	    strcpy(plg->ptrace->uwi, plg->edit_uwi);
	    status = lg_ltdm_add_trace( plg->ptrace);
	}

/* splice trace 2 into trace 1 ( four different cases handled) */

	else 
	{
	    sprintf(text,"Splicing traces to create %s",plg->edit_trace);
	    am_message(AM_DIALOG,text);

	  /* case 1 */

	  if (plg->splice_2_top > plg->ptrace->non_null_top &&
	    plg->splice_2_top < plg->ptrace->non_null_base)

	  {
	    /* trace 1 */

	    cur_row = ((plg->splice_2_top - plg->ptrace->non_null_top ) /
			plg->ptrace->dinc) + 1.0; 
	    if(ABS(plg->ptrace->non_null_top + (cur_row - 1)*plg->ptrace->dinc
		 - plg->splice_2_top) > plg->ptrace->dinc/2)
		cur_row++;
	    base_row = ((plg->splice_2_base - plg->ptrace->non_null_top ) /
		plg->ptrace->dinc) + 1.0; 
	    if(ABS(plg->ptrace->non_null_top + (base_row - 1)*plg->ptrace->dinc
		 - plg->splice_2_base) > plg->ptrace->dinc/2)
		base_row++;
	    nrows = base_row - cur_row + 1;

	    status = nl_set_current_row( plg->ptrace->ptrace, cur_row, 1);

	    /* trace 2 */

	    cur_row = ((plg->splice_2_top - plg->ptrace_2->non_null_top ) / 
			plg->ptrace->dinc) + 1.0;
	    if(ABS(plg->ptrace_2->non_null_top + (cur_row - 1)*plg->ptrace->dinc
		 - plg->splice_2_top) > plg->ptrace->dinc/2)
		cur_row++;

	    status = nl_set_current_row( plg->ptrace_2->ptrace, cur_row, 1);

	    /* splice logs */

	    replace_vals = (FLOAT *) tc_zalloc( (nrows+1) * sizeof (FLOAT) );
	    col_list[0] = 2;
	    var_list[0] = (VOIDPTR) replace_vals;
	    status = nl_get_rows(plg->ptrace_2->ptrace,nrows,1,col_list,
				 var_list,&ngot);
	    status = nl_replace_rows(plg->ptrace->ptrace,ngot,1,col_list,var_list);

	    /* write log */

	    strcpy(plg->ptrace->trace_name, plg->edit_trace);
	    strcpy(plg->ptrace->uwi, plg->edit_uwi);
	    status = lg_ltdm_add_trace( plg->ptrace);
	  }

	  else if (plg->splice_2_top < plg->ptrace->non_null_top &&
		   plg->splice_2_base > plg->ptrace->non_null_top)
 	  {

	    /* case 2 */

	    /* trace 2 */

	    cur_row = ((plg->splice_2_base - plg->ptrace_2->non_null_top ) /
			plg->ptrace->dinc) + 1.0; 
	    if(ABS(plg->ptrace_2->non_null_top + (cur_row - 1)*plg->ptrace->dinc
		 - plg->splice_2_base) > plg->ptrace->dinc/2)
		cur_row++;

	    status = nl_set_current_row( plg->ptrace_2->ptrace, cur_row, 1);

	    /* trace 1 */

	    cur_row = ((plg->splice_2_base - plg->ptrace->non_null_top ) / 
			plg->ptrace->dinc) + 1.0;
	    if(ABS(plg->ptrace->non_null_top + (cur_row - 1)*plg->ptrace->dinc
		 - plg->splice_2_base) > plg->ptrace->dinc/2)
		cur_row++;

	    base_row = ((plg->ptrace->non_null_base - plg->ptrace->non_null_top ) /
			plg->ptrace->dinc) + 1.0; 
	    if(ABS(plg->ptrace->non_null_top + (base_row - 1)*plg->ptrace->dinc
		 - plg->ptrace->non_null_base) > plg->ptrace->dinc/2)
		base_row++;
	    nrows = base_row - cur_row + 1;
	    status = nl_set_current_row( plg->ptrace->ptrace, cur_row, 1);

	    /* splice logs */

	    replace_vals = (FLOAT *) tc_zalloc( (nrows+1) * sizeof (FLOAT) );
	    col_list[0] = 2;
	    var_list[0] = (VOIDPTR) replace_vals;
	    status = nl_get_rows(plg->ptrace->ptrace,nrows,1,col_list,
				var_list,&ngot);
	    status = nl_replace_rows(plg->ptrace_2->ptrace,ngot,1,
				col_list,var_list);

	    /* delete top portion of log */

	    splice_start = ((plg->splice_2_top - plg->ptrace_2->non_null_top ) /
				plg->ptrace->dinc) + 1.0; 
	    if(ABS(plg->ptrace_2->non_null_top +(splice_start - 1)*plg->ptrace->dinc
		 - plg->splice_2_top) > plg->ptrace->dinc/2)
		splice_start++;
	    status = nl_set_current_row(plg->ptrace_2->ptrace,1,1);
	    status = nl_delete_rows(plg->ptrace_2->ptrace,splice_start-1);

	    /* reset start value */
 
	    col_list[0] = 1;
	    start_value = (FLOAT) plg->splice_2_top;
	    var_list[0] = (VOIDPTR) &start_value;
	    status = nl_set_table_info(plg->ptrace_2->ptrace,1,
				NL_TABLE_START_VALUE,1,col_list,var_list);

	    /* write log */

	    strcpy(plg->ptrace_2->trace_name, plg->edit_trace);
	    strcpy(plg->ptrace_2->uwi, plg->edit_uwi);
	    status = lg_ltdm_add_trace( plg->ptrace_2);
	  }

	else if(plg->splice_2_top > plg->ptrace->non_null_base)
	  {
	  /* case 3 */

	    /* determine # of nulls */

	    base_row = ((plg->ptrace->non_null_base - plg->ptrace->non_null_top) /
			 plg->ptrace->dinc) + 1.0;
	    if(ABS(plg->ptrace->non_null_top + (base_row -1)*plg->ptrace->dinc
		- plg->ptrace->non_null_base) > plg->ptrace->dinc/2)
		base_row++;
	    splice_start = ((plg->splice_2_top - plg->ptrace->non_null_top ) /
				plg->ptrace->dinc) + 1.0; 
	    if(ABS(plg->ptrace->non_null_top + (splice_start - 1)*plg->ptrace->dinc
		 - plg->splice_2_top) > plg->ptrace->dinc/2)
		splice_start++;
	    nnulls = splice_start - base_row - 1.0;

	    /* add nulls */

	    if (nnulls > 0)
	      {
	      col_list[0] = 2;
	      var_list[0] = (VOIDPTR) &null_val;
	      status = nl_inq_nlist_info(plg->ptrace->ptrace, NL_NLIST_NULL_VALUE, 1, col_list,
			 var_list);
	      for (i=0,cur_row = base_row; i<nnulls; i++, cur_row++)
	        {
		status = nl_set_current_row( plg->ptrace->ptrace, cur_row, 1);
		status = nl_add_rows( plg->ptrace->ptrace, 1, 1, col_list, var_list);
	        }
	      }

	    /* trace 2 */

	    cur_row = ((plg->splice_2_top - plg->ptrace_2->non_null_top ) / 
			plg->ptrace->dinc) + 1.0;
	    if(ABS(plg->ptrace_2->non_null_top + (cur_row - 1)*plg->ptrace->dinc
		 - plg->splice_2_top) > plg->ptrace->dinc/2)
		cur_row++;

	    base_row = ((plg->splice_2_base - plg->ptrace_2->non_null_top ) /
				plg->ptrace->dinc) + 1.0; 
	    if(ABS(plg->ptrace_2->non_null_top + (base_row - 1)*plg->ptrace->dinc
		 - plg->splice_2_base) > plg->ptrace->dinc/2)
		base_row++;
	    nrows = base_row - cur_row + 1;

	    status = nl_set_current_row( plg->ptrace_2->ptrace, cur_row, 1);

	    /* trace 1 */

	    base_row = ((plg->ptrace->non_null_base - plg->ptrace->non_null_top) /
			 plg->ptrace->dinc) + 1.0;
	    if(ABS(plg->ptrace->non_null_top + (base_row -1)*plg->ptrace->dinc
		- plg->ptrace->non_null_base) > plg->ptrace->dinc/2)
		base_row++;
	    cur_row = base_row + nnulls;
	    status = nl_set_current_row( plg->ptrace->ptrace, cur_row, 1);

	    /* splice logs */

	    replace_vals = (FLOAT *) tc_zalloc( (nrows+1) * sizeof (FLOAT) );
	    col_list[0] = 2;
	    var_list[0] = (VOIDPTR) replace_vals;
	    status = nl_get_rows(plg->ptrace_2->ptrace,nrows,1,col_list,
				var_list,&ngot);
	    status = nl_add_rows(plg->ptrace->ptrace,ngot,1,col_list,var_list);
	
	    /* write log */

	    strcpy(plg->ptrace->trace_name, plg->edit_trace);
	    strcpy(plg->ptrace->uwi, plg->edit_uwi);
	    status = lg_ltdm_add_trace( plg->ptrace);
	  }

	else if(plg->splice_2_base < plg->ptrace->non_null_top)
	  {
	  /* case 4 */

	    /* determine # of nulls */

	    splice_start = ((plg->splice_2_base - plg->ptrace_2->non_null_top ) /
			plg->ptrace->dinc) + 1.0; 
	    if(ABS(plg->ptrace_2->non_null_top + (splice_start-1)*plg->ptrace->dinc
		 - plg->splice_2_base) > plg->ptrace->dinc/2)
		splice_start++;
	    base_row = ((plg->ptrace->non_null_top - plg->ptrace_2->non_null_top ) /
			plg->ptrace->dinc) + 1.0; 
	    if(ABS(plg->ptrace_2->non_null_top + (base_row - 1)*plg->ptrace->dinc
		 - plg->ptrace->non_null_top) > plg->ptrace->dinc/2)
		base_row++;
	    nnulls = base_row - splice_start - 1.0;

	    /* add nulls */

	    col_list[0] = 2;
	    var_list[0] = (VOIDPTR) &null_val;
	    status = nl_inq_nlist_info(plg->ptrace_2->ptrace, NL_NLIST_NULL_VALUE, 1, col_list,
			 var_list);
	    for (i=0,cur_row = splice_start; i<nnulls; i++, cur_row++)
	      {
		status = nl_set_current_row( plg->ptrace_2->ptrace, cur_row, 1);
		status = nl_replace_rows( plg->ptrace_2->ptrace, 1, 1, col_list, var_list);
	      }

	    /* trace 1 */

	    cur_row = ((plg->ptrace->non_null_top - plg->ptrace->non_null_top ) / 
			plg->ptrace->dinc) + 1.0;
	    if(ABS(plg->ptrace->non_null_top + (cur_row - 1)*plg->ptrace->dinc
		 - plg->ptrace->non_null_top) > plg->ptrace->dinc/2)
		cur_row++;
	    base_row = ((plg->ptrace->non_null_base - plg->ptrace->non_null_top ) /
			plg->ptrace->dinc) + 1.0; 
	    if(ABS(plg->ptrace->non_null_top + (base_row - 1)*plg->ptrace->dinc
		 - plg->ptrace->non_null_base) > plg->ptrace->dinc/2)
		base_row++;
	    nrows = base_row - cur_row + 1;

	    status = nl_set_current_row( plg->ptrace->ptrace, cur_row, 1);

	    /* trace 2 */
 
	    splice_start = ((plg->splice_2_base - plg->ptrace_2->non_null_top ) /
			plg->ptrace->dinc) + 1.0; 
	    if(ABS(plg->ptrace_2->non_null_top + (splice_start-1)*plg->ptrace->dinc
		 - plg->splice_2_base) > plg->ptrace->dinc/2)
		splice_start++;
	    cur_row = splice_start + nnulls;
	    status = nl_set_current_row( plg->ptrace_2->ptrace, cur_row, 1);

	    /* splice logs */

	    replace_vals = (FLOAT *) tc_zalloc( (nrows+1) * sizeof (FLOAT) );
	    col_list[0] = 2;
	    var_list[0] = (VOIDPTR) replace_vals;
	    status = nl_get_rows(plg->ptrace->ptrace,nrows,1,col_list,
				var_list,&ngot);
	    status = nl_replace_rows(plg->ptrace_2->ptrace,ngot,1,col_list,
					var_list);
	
	    /* delete top portion of log */

	    splice_start = ((plg->splice_2_top - plg->ptrace_2->non_null_top ) /
			plg->ptrace->dinc) + 1.0; 
	    if(ABS(plg->ptrace_2->non_null_top + (splice_start-1)*plg->ptrace->dinc
		 - plg->splice_2_top) > plg->ptrace->dinc/2)
		splice_start++;
	    status = nl_set_current_row(plg->ptrace_2->ptrace,1,1);
	    status = nl_delete_rows(plg->ptrace_2->ptrace,splice_start-1);

	    /* reset start value */

	    col_list[0] = 1;
	    start_value = (FLOAT) plg->splice_2_top;
	    var_list[0] = (VOIDPTR) &start_value;
	    status = nl_set_table_info(plg->ptrace_2->ptrace,1,
				NL_TABLE_START_VALUE,1,col_list,var_list);

	    /* write log */

	    strcpy(plg->ptrace_2->trace_name, plg->edit_trace);
	    strcpy(plg->ptrace_2->uwi, plg->edit_uwi);
	    status = lg_ltdm_add_trace( plg->ptrace_2);
	  }
	
	}

        switch (status)
          {
          case SUCCESS:
	       am_message(AM_DIALOG,"");
               plg->num_traces = 0;
               status = ts_tcp_free( plg->trace_tcp );
               status = SUCCESS;
               break;
          case LG_INVALID_FORMAT:
          case LG_NO_TRACE_NAME:
          case LG_INVALID_BLANK:
          case LG_NO_TRACE_FIELD:
	       am_message(AM_DIALOG,"");
               am_message(AM_ALERT, mg_message( LG_ED_INVALID_TRACE_NAME ));
               status = FAIL;
               break;
          default:
	       sprintf(text,"Finder");
	       am_message(AM_DIALOG,text);
               am_message(AM_ALERT, mg_message( LG_ED_TRACE_NOT_WRITTEN ) );
               er_log("LG_LTDM_ADD_TRACE failed in LG_EDIT_SPLICE.");
               er_log( mg_message( status ) );
               status = FAIL;
          }

	return status;

}

static INT lg_ed_splice_conf(plg)
LOG_EDIT_STRUCT *plg;

{
	INT status;
	INT ID,item,value;
	CHAR text[128];

	fi_push();
	status = am_define_resource(AM_DIALOG,Resource_File,
				LG_ED_SPLICE_CONFIRM,
				NULL_FUNCTION_PTR, &ID, NULL);

	if(strlen(plg->splice_log_2) == 0)
	{
	sprintf(text,"The new trace %s will be created from %s with nulls \
from %.4f to %.4f",plg->edit_trace,plg->splice_log_1,
		plg->splice_2_top,plg->splice_2_base);
	}
	else
	{
	sprintf(text,"The new trace %s will be created by splicing %s from \
%.4f to %.4f into %s",plg->edit_trace,
		plg->splice_log_2,plg->splice_2_top,plg->splice_2_base,
		plg->splice_log_1);
	}


	status = wi_set_dialog_text(ID,LG_ED_SPLICE_CONFIRM_MSG,text);
	status = wiopcd(ID);
	wi_request_dialog_item(&ID, &item, &value, text);

	switch(item)
	{
	case LG_ED_SPLICE_CONFIRM_CANCEL:
	  status = FAIL;
	  break;

	case LG_ED_SPLICE_CONFIRM_PROCEED:
	  status = SUCCESS;
	  break;
	}

	am_release_resource(ID);
	fi_pop();

	return status;
}
