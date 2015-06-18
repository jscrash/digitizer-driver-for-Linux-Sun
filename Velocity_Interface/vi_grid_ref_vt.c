/* DEC/CMS REPLACEMENT HISTORY, Element VI_GRID_REF_VT.C */
/* *2    11-JUN-1991 14:36:10 JESSIE "(SPR 0) get vel_time pair values" */
/* *1    11-JUN-1991 14:34:57 JESSIE "VI user interface" */
/* DEC/CMS REPLACEMENT HISTORY, Element VI_GRID_REF_VT.C */
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

#ifndef ESI_TU_H
#include "esi_tu.h"
#endif

#ifndef ESI_MG_H
#include "esi_mg.h"
#endif

#ifndef ESI_VI_H
#include "esi_vi.h"
#endif

#include "vi_grid_cdlgs.rh"
#define NullString ""

static CHAR VI_RESOURCE_FILE[] = "VI_GRID_CDLGS";

/* File Description ---------------------------------------------------------       */
/*Overview:                                                                         */
/*                                                                                  */
/*                                                                                  */
/*-----------------------------------------------------------------------------     */

/* Function Description -----------------------------------------------------       */
/*Description:                                                                      */
/*    Display dialog to select v_t pair.                                            */
/*                                                                                  */
/*Prototype:                                                                        */
/*    publicdef INT vi_ref_cdlg_init(VI_STRUCT *pvi);                          */
/*                                                                                  */
/*Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)        */
/*    pvi             -(VI_STRUCT *) Pointer to VI struct.                        */
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
publicdef INT vi_ref_cdlg_init (VI_STRUCT *pvi)
#else
publicdef INT vi_ref_cdlg_init (pvi)
VI_STRUCT *pvi;
#endif
    {
    INT status;
    BOOL done = FALSE;
    INT id;
    INT i,count;
    INT len, index;
    INT num_items,item;
    INT time_item[1];
    INT unused;
    INT event_class, event_id;
    INT max_selectable = 1;
    TCP temp_time = (TCP) 0;
    TCP temp_vel  = (TCP) 0;
    TCP vel_backup  = (TCP) 0;
    INT temp_no = 0;
    CHAR time_value[20];
    CHAR vel_value[20];
    CHAR *time_string = (CHAR *) 0;
    CHAR *vel_string  = (CHAR *) 0;
    
    status = am_define_resource (AM_DIALOG, VI_RESOURCE_FILE, VI_GRID_REF,
                                 NULL_FUNCTION_PTR, &id, NULL);
    
    if (pvi->ref_time != (TCP) 0) 
      {
      ts_tcp_copy (&temp_time,pvi->ref_time);
      ts_tcp_copy (&temp_vel,pvi->ref_vel);
      temp_no = pvi->num_ref;
      }

    fi_push();
    /* put time and vef into selector */

    max_selectable = 1;
    num_items = ts_tcp_len (pvi->ref_time);
    if (num_items != 0)
      {
      /* time_string = ts_pack_strings (num_items, pvi->ref_time);*/
      wisselt (id, VI_GRID_TIME_SELECTOR, VI_GRID_VEL_VSCROLL, num_items, max_selectable,
             	pvi->ref_time);
      }
    num_items = ts_tcp_len (pvi->ref_vel);
    if (num_items != 0)
      {
      /*vel_string = ts_pack_strings (num_items, pvi->ref_vel);*/
      wisselt (id, VI_GRID_VEL_SELECTOR, VI_GRID_VEL_VSCROLL, num_items, max_selectable,
             	pvi->ref_vel);
      }

    wi_activate (id, VI_GRID_TIME_SELECTOR, ON);
    wi_activate (id, VI_GRID_VEL_SELECTOR, ON);

    status = wi_open_dialog(id);

    done = FALSE;
    while (done == FALSE)
      {
      wi_request_event (0, &event_class, &event_id, &item);
      switch (item)
        {
	case VI_GRID_REF_ADD:
		wi_query_dialog_item (id, VI_GRID_TIME_TEXT, &unused, time_value);
		wi_query_dialog_item (id, VI_GRID_VEL_TEXT, &unused, vel_value);
		if (strlen (ts_noblanks (time_value)) == 0)
		  {
		  wi_beep(1);
		  am_message (AM_ALERT, " Please input time value.");
		  }
		else if (strlen (ts_noblanks (vel_value)) == 0)
		  {
		  wi_beep(1);
		  am_message (AM_ALERT, " Please input velocity value.");
		  }
		else /* set both values into selector */
		  {
		  /* check for duplicated time value */
		  if ((i= ts_tcp_len(pvi->ref_time)) == 0)
		    {
		    ts_tcp_append (&pvi->ref_time, time_value);
		    ts_tcp_append (&pvi->ref_vel, vel_value);
		    }
		  else
		    {
		    index = ts_switch (time_value, pvi->ref_time);
		    if (index != -1)
		      {
		      wi_beep(1);
		      am_message (AM_ALERT, "You cannot add time-velocity pair having a duplicate time value. Please either change the time value you are adding or delete the time-velocity pair with the same time value from the velocity function.");
		      }
 		    else
		      {
		      /* append time data and sort in order */

		      ts_tcp_append (&pvi->ref_time, time_value);
		      vi_sort (pvi->ref_time);

		      /* looking for index on time value, in order to set */
		      /* velocity value into the same position            */

		      index = ts_switch (time_value, pvi->ref_time);
		      if (index != -1)
		        {
		        len = ts_tcp_len (pvi->ref_vel);
			if (index == len ) /* the last one? */
			  {
		          ts_tcp_append (&pvi->ref_vel, vel_value);
			  }
			else
			  {
			  vel_backup = (TCP) 0;
		          for ( i = 0; i< index; i++)
		            ts_tcp_append (&vel_backup,pvi->ref_vel[i]);

		          ts_tcp_append (&vel_backup, vel_value);

		          for ( i = index; i< len; i++)
		            ts_tcp_append (&vel_backup,pvi->ref_vel[i]);
		      
		          ts_tcp_copy (&pvi->ref_vel, vel_backup);
			  }
	  	        }
		      }
		    }
		    i = ts_tcp_len (pvi->ref_time);
		    time_string = ts_pack_strings (i, pvi->ref_time);
		    vel_string = ts_pack_strings (i, pvi->ref_vel);
		    wi_set_dialog_text (id, VI_GRID_TIME_SELECTOR, time_string);
		    wi_set_dialog_text (id, VI_GRID_VEL_SELECTOR, vel_string);
		  }
		break;

	case VI_GRID_REF_OK: 
		pvi->num_ref = ts_tcp_len (pvi->ref_time);
		done = TRUE;
		break;

	case VI_GRID_REF_DELETE:
		wi_set_selection_limit (id, VI_GRID_TIME_SELECTOR, 1);
		wi_set_selection_limit (id, VI_GRID_VEL_SELECTOR, 1);
		
		/* assume time and vel is a pair, so addition or deleting item */
		/* always in the same location				       */

		wi_query_selections (id, VI_GRID_TIME_SELECTOR, &num_items, time_item);
		if (num_items != 1)
		  {
		  wi_beep(1);
		  am_message (AM_STATUS, "Please select time and velocity.");
		  }
		else
		  {
		  ts_tcp_delete (pvi->ref_time,time_item[0]);
		  ts_tcp_delete (pvi->ref_vel,time_item[0]);
		  
		  i = ts_tcp_len (pvi->ref_time);
		  if (i == 0)
		    {
		    wi_set_dialog_text (id, VI_GRID_TIME_SELECTOR, NullString);
		    wi_set_dialog_text (id, VI_GRID_VEL_SELECTOR, NullString); 
		    }
		  else
		    {
		    time_string = ts_pack_strings (i, pvi->ref_time);
		    vel_string = ts_pack_strings (i, pvi->ref_vel);

		    wi_set_dialog_text (id, VI_GRID_TIME_SELECTOR, time_string);
		    wi_set_dialog_text (id, VI_GRID_VEL_SELECTOR, vel_string); 
		    }
		  }
		break;

	case VI_GRID_TIME_SELECTOR:
		wi_query_selections (id, VI_GRID_TIME_SELECTOR, &num_items, time_item);
		if (num_items > 0)
		  {
		  wi_set_default_sel(id, VI_GRID_VEL_SELECTOR, time_item[0]);
		  }
		break;

	case VI_GRID_VEL_SELECTOR:
		wi_query_selections (id, VI_GRID_VEL_SELECTOR, &num_items, time_item);
		if (num_items > 0)
		  {
		  wi_set_default_sel(id, VI_GRID_TIME_SELECTOR, time_item[0]);
		  }
		break;

	case VI_GRID_REF_CANCEL:
		if (temp_time != (TCP) 0)
		  {
    		  ts_tcp_copy (&pvi->ref_time,temp_time);
    		  ts_tcp_copy (&pvi->ref_vel,temp_vel);
		  }
		else
		  {
		  pvi->ref_time = (TCP) 0; 
		  pvi->ref_vel  = (TCP) 0;
		  }
		pvi->num_ref = temp_no;
		done = TRUE;
		break;
        }
      }
    if (time_string != (CHAR *) 0)
      tc_free(time_string);
    if (vel_string != (CHAR *) 0)
      tc_free(vel_string);
    if (temp_time != (TCP) 0)
      ts_tcp_free(temp_time);
    if (temp_vel != (TCP) 0)
      ts_tcp_free(temp_vel);
    if (vel_backup != (TCP) 0)
      ts_tcp_free(vel_backup);
    status = am_release_resource (id);
    fi_pop();
    return SUCCESS;
    
    }
/* END:                                                                             */

