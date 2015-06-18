/* DEC/CMS REPLACEMENT HISTORY, Element DR_SAV_OBJ.C*/
/* *8    14-AUG-1991 18:52:09 KEE "(SPR 8899) Fix case 3640 Duplicate key in index bug"*/
/* *7    11-JUN-1991 11:30:17 KEE "(SPR 7987) Add overwriting a existing object warning message, case 2338"*/
/* *6    28-SEP-1990 15:05:09 GILLESPIE "(SPR 1) Merge beta deltas"*/
/*  4B1  24-SEP-1990 16:15:12 GILLESPIE "Beta Deltas"*/
/* *5    14-AUG-1990 21:56:07 GILLESPIE "(SPR 2001) Use LINE_ATTRIBUTEs"*/
/* *4    19-JUL-1990 17:55:23 GILLESPIE "(SPR 5411) Support for fault traces"*/
/* *3    18-JUL-1990 11:14:06 GILLESPIE "(SPR 5411) Add support for fault traces"*/
/* *2    26-JUN-1989 10:42:03 GILLESPIE "(SPR 20) Fix castings"*/
/* *1    19-JUN-1989 12:48:26 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element DR_SAV_OBJ.C*/
/******************************************************************************     */
/*                                                                                  */
/*NAME:           dr_save_object                                                    */
/*                                                                                  */
/*DESCRIPTION:    Save a graphics object to the database                            */
/*                                                                                  */
/*                                                                                  */
/*    Input:      Standard data structure                                           */
/*                                                                                  */
/*    Output:     None                                                              */
/*                                                                                  */
/*    Status:     SUCCESS;                                                          */
/*                DR_SAVE_ERROR                                                     */
/*                                                                                  */
/*    TODO:       1)  Why reset the object upon save -- better to leave object      */
/*                    as is, so that user can save frequently to protect            */
/*                    his edits.                                                    */
/*                2)  Error handling.                                               */
/*                                                                                  */
/******************************************************************************     */

#include "esi_dr.h"

#ifndef ESI_QS_H
#include "esi_qs.h"
#endif

#ifndef ESI_AM_H
#include "esi_am.h"
#endif

#ifndef ESI_MG_H
#include "esi_mg.h"
#endif

#ifndef ESI_WM_H
#include "esi_wm.h"
#endif

#ifndef ESI_TS_H
#include "esi_ts.h"
#endif

#ifndef ESI_WI_H
#include "esi_wi.h"
#endif

#include "esi_dr_err.h"

#if USE_PROTOTYPES
publicdef INT dr_save_object (DR_DATA *dr_data)
#else
publicdef INT dr_save_object (dr_data)
DR_DATA *dr_data;
#endif
    {
    INT status = SUCCESS;
    INT corr_status = SUCCESS;
    INT line_status = SUCCESS;
    INT text_status = SUCCESS;
    INT object_status = SUCCESS;
    BOOL isfault = FALSE;
    BOOL exist = FALSE;
    BOOL this_object_fail;
    OBJECT_STRUCTURE *this_object = (OBJECT_STRUCTURE *)0;
    OBJECT_STRUCTURE *last_object;
    PROJECT_NAME project;
    CHAR msg_buff[128];
    INT nlen, i;
    FILENAME nlist_file;
    GO_WORKSPACE *w, **global_w;
    INT ifyes;

    /*  Get the project                                                             */
    
    qs_inq_c (QS_PROJECT_NAME, project, &nlen);
    
    /* since the user wants to save the current object, we                          */
    /* can delete all old modified segments.                                        */
    
    for (i = 0; i < dr_data->mod_obj_segments.nused; i++)
        {
        wmsdelete (dr_data->mod_obj_segments.old_seg_array[i]);
        }
    /*  Close the group for this object                                             */
    
    dr_data->mod_obj_segments.nused = 0;
    wmgclose (dr_data->current_object);
    
    /* -----------------------------                                                */
    /* Init extents if there are any                                                */
    /* lines or text to be saved.                                                   */
    /* The extents will be recomputed                                               */
    /* by the functions which update                                                */
    /* the database for each element                                                */
    /* E.g., dr_write_nlist,                                                        */
    /*       dr_update_text, etc.                                                   */
    /* -----------------------------                                                */
    if (dr_data->line_ptr != (OBJECT_STRUCTURE *)NULL ||
        dr_data->text_ptr != (OBJECT_STRUCTURE *)NULL)
        {
    dr_data->lower_left_extent.x = 1.0e30;
    dr_data->lower_left_extent.y = 1.0e30;
    dr_data->upper_right_extent.x = -1.0e30;
    dr_data->upper_right_extent.y = -1.0e30;
        }
    else
        {
        /* --------------------------------                                         */
        /* The object is EMPTY -- it really                                         */
        /* should be deleted                                                        */
        /* --------------------------------                                         */
        }
    /*  Save each line element                                                      */
    
    for (this_object_fail = FALSE, this_object = dr_data->line_ptr; 
	 this_object != (OBJECT_STRUCTURE *)NULL;
	 last_object = this_object,
         this_object = this_object->next_object)
        {
        isfault = (strcmp (dr_data->current_object_type, FAULTS_TYPE) == 0);
        
        if (this_object->new_object)
            {
            corr_status = dr_save_correlation (dr_data->current_object_key,
                                               this_object);
            if (corr_status IS_NOT_EQUAL_TO SUCCESS)
                {
		this_object_fail = TRUE;
		break;
                }
            if (!isfault)
                {
                ho_add_path ("ESI$CULTURE", dr_data->current_object_type,
                             sizeof(FILENAME), nlist_file);
                }
            line_status = dr_put_line (this_object->object_ptr.current_line,
                                       nlist_file, dr_data, isfault,
                                       dr_data->current_marker);
            if (line_status != SUCCESS)
                {
		  this_object_fail = TRUE;
		  break;
                }
                }
            else
                {
            if (!isfault)
                {
                ho_add_path ("ESI$CULTURE", dr_data->current_object_type,
                             sizeof(FILENAME), nlist_file);
                }
            exist = TRUE;
            line_status = dr_update_line (this_object->object_ptr.current_line,
                                          nlist_file, dr_data, isfault,
                                          dr_data->current_marker);
            if (line_status != SUCCESS)
                {
		this_object_fail = TRUE;
		break;
                }
            }
        }
    if (this_object_fail == TRUE)
      {
	if (this_object == dr_data->line_ptr)
	  {
	    dr_data->line_ptr = this_object->next_object;
	  }
	else
	  {
	    last_object->next_object = this_object->next_object;
	  }
	tc_free(this_object);
      }
    /*  Save each text element                                                      */
    
    for (this_object_fail = FALSE,
	 this_object = dr_data->text_ptr; 
	 this_object != (OBJECT_STRUCTURE *)NULL;
	 last_object = this_object,
         this_object = this_object->next_object)
        {
        if (this_object->new_object)
            {
            corr_status = dr_save_correlation (dr_data->current_object_key,
                                               this_object);
            if (corr_status IS_NOT_EQUAL_TO SUCCESS)
                {
		this_object_fail = TRUE;
                break;
                }
            text_status = dr_put_text (this_object->object_ptr.current_text, dr_data);
            
            if (text_status != SUCCESS)
                {
		this_object_fail = TRUE;
                break;
                }
            }
        else
            {
            exist = TRUE;
            text_status = dr_update_text (this_object->object_ptr.current_text,
                                          dr_data);
            if (text_status != SUCCESS)
                {
		this_object_fail = TRUE;
                break;
            }
        }
        }
    if (this_object_fail == TRUE)
      {
	if (this_object == dr_data->text_ptr)
	  {
	    dr_data->text_ptr = this_object->next_object;
	  }
	else
	  {
	    last_object->next_object = this_object->next_object;
	  }
	tc_free(this_object);
      }

    /*  Save the object                                                             */
    
    if (corr_status + line_status + text_status == SUCCESS)
        {
        object_status = dr_put_object (dr_data);
        }
    if (corr_status + line_status + text_status + object_status != SUCCESS)
        {
        dr_commit (FALSE);
        if (corr_status != SUCCESS)
            {
            am_message (AM_ALERT, mg_message (corr_status));
            }
        if (line_status != SUCCESS)
            {
            am_message (AM_ALERT, mg_message (line_status));
            }
        if (text_status != SUCCESS)
            {
            am_message (AM_ALERT, mg_message (text_status));
            }
        if (object_status != SUCCESS)
            {
            am_message (AM_ALERT, mg_message (object_status));
            }
        sprintf (msg_buff, "Object %d Was NOT Saved",
                 dr_data->current_object_key);
        am_message (AM_DIALOG, msg_buff);
        return DR_SAVE_ERROR;
        }
    else
        {
        if (exist EQUALS TRUE)
	  {
	    sprintf(msg_buff, "Object %d already existed, do you want to overwrite it?", 
		    dr_data->current_object_key);
	    wiyesno(msg_buff,"",&ifyes,&status);
	  }
	else
	  {
	    ifyes = TRUE;
	  }

	if (ifyes)
	  {
	    dr_commit (TRUE);
	    
	    sprintf (msg_buff, "Object %d Was Saved", dr_data->current_object_key);
	    am_message (AM_DIALOG, msg_buff);
        
	    dr_data->overlay_exists = TRUE; /* Enable Pick Object menu item             */
	    
	    /*  Reset the object                                                        */
	    
	    dr_reset_object (dr_data);
	    
	    /* update static nlist for picking                                          */
	    status = am_open_workspace ("GO_STATIC_SPACE", AM_GLOBAL,
					(VOIDPTR *)&global_w);
	    w = *global_w;
	    if (w->master_nlist != (NLIST_HEADER)0)
	      status = nl_free_nlist (w->master_nlist);
	    status = nl_copy_nlist (w->temp_nlist, &w->master_nlist);
	    return status;
	  }
	else
	  {
	    dr_commit (FALSE);
	    sprintf (msg_buff, "Object %d Was NOT Saved",
		     dr_data->current_object_key);
	    am_message (AM_DIALOG, msg_buff);
	    return(SUCCESS);
	  }
        }
    }

