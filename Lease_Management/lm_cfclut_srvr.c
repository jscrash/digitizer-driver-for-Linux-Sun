/* DEC/CMS REPLACEMENT HISTORY, Element LM_CFCLUT_SRVR.C */
/* *6    11-SEP-1991 13:40:24 JULIAN "(SPR 0) add comments" */
/* *5     9-AUG-1991 12:45:22 JULIAN "(SPR 0) split create/modify button to two button" */
/* *4    12-JUN-1991 14:56:12 JULIAN "(SPR 0) upgrade user interface" */
/* *3     7-MAY-1991 17:48:44 JULIAN "(SPR 1622) init change" */
/* *2    18-MAR-1991 18:08:05 JULIAN "(SPR 0) Lease Mapping Init Checkin" */
/* *1    18-MAR-1991 17:51:55 JULIAN "Lease Mapping" */
/* DEC/CMS REPLACEMENT HISTORY, Element LM_CFCLUT_SRVR.C */
/* include files */
#include "esi_gl_defs.h"
#ifndef ESI_WI_H
#include "esi_wi.h"
#endif
#ifndef ESI_GTYPES_H
#include "esi_gtypes.h"
#endif
#ifndef ESI_AM_H
#include "esi_am.h"
#endif
#ifndef ESI_TS_H
#include "esi_ts.h"
#endif
#ifndef ESI_LM_H
#include "esi_lm.h"
#endif
#ifndef ESI_TC_H
#include "esi_tc.h"
#endif
#ifndef ESI_MG_H
#include "esi_mg.h"
#endif

/* Resource files */

#ifndef LM_CDLGS_RH
#define LM_CDLGS_RSRC "LM_CDLGS"
#include "lm_cdlgs.rh"
#endif

/* LOCAL FUNCTIONS DEFINED HERE */
#if USE_PROTOTYPES
static INT lmz_defs_server (LM_DIALOG_STRUCT *pex);
static INT lmz_entrs_server (LM_DIALOG_STRUCT *pex);
#else
static INT lmz_defs_server ();
static INT lmz_entrs_server ();
#endif


/* Function Description -----------------------------------------------------
Description:
    Routine to define parameter on color/fill def keyword in lease overlay

Prototype:
    publicdef INT lm_color_fill_clut(CHAR *parameter)

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (O) parameter	- (CHAR *) returned value of the COLOR/FILL DEF 
                                   parameter

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/
#if USE_PROTOTYPES
publicdef INT lm_color_fill_clut(CHAR *parameter)
#else
publicdef INT lm_color_fill_clut(parameter)
CHAR *parameter;
#endif
    {
    INT status, tag_status;
    LM_DIALOG_STRUCT *pex;

			     /*	 Try to open the global workspace.  If we   */
			     /*	 are successful, continue.  If not,	    */
			     /*	 allocate & initialize it.		    */
    status = am_open_workspace(LM_DIALOG_WORKSPACE,AM_GLOBAL,(VOIDPTR *)&pex);
    if (status != SUCCESS)
			     /*	 or initialize it			    */
      {
      am_define_workspace(LM_DIALOG_WORKSPACE,AM_GLOBAL,
  			  sizeof(LM_DIALOG_STRUCT),(VOIDPTR *)&pex);
      pex->defs_type = LM_DATE;
      pex->defs_name[0]=0;
      }
			     /*	open dialog and retrieve information for    */
			     /*	specified parameter			    */
    status = lmz_defs_server(pex);

			     /*	append parameter with its data type	    */
    if(status == SUCCESS && IS_EMPTY_STRING(pex->defs_name) == FALSE)
	{
    	strcpy(parameter,pex->defs_name);
	switch(pex->defs_type)
	    {
	    case LM_DATE :
		strcat(parameter,":DATE");
		break;
	    case LM_TEXT :
		strcat(parameter,":TEXT");
		break;
	    case LM_NUMERIC :
		strcat(parameter,":NUM");
		break;
	    }
        }

    return status;
   
    }


/* Function Description -----------------------------------------------------
Description:
    Color/fill definition name user interface

Prototype:
    static INT lmz_defs_server(LM_DIALOG_STRUCT *pex)

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I/O) pex  - (LM_DIALOG_STRUCT *) lease overlay dialog boxes structure

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PRIVATE
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/
#if USE_PROTOTYPES
static INT lmz_defs_server(LM_DIALOG_STRUCT *pex)
#else
static INT lmz_defs_server(pex)
LM_DIALOG_STRUCT *pex;
#endif

    {
    INT   status=SUCCESS;
    INT	    unused;
    INT item_selected[1];
    INT nsel = 0;
    INT id,event_class,event_id,event_item;
    BOOL done = FALSE;
   
                                   /* allocate dialog1   */
    status = am_define_resource(AM_DIALOG, LM_CDLGS_RSRC, LM_DEFS,
                                NULL_FUNCTION_PTR, &pex->lm_defs, pex);

			     /*	set id to point to dialog 1		    */
    id = pex->lm_defs;	    

			     /*	Enable and disable radio buttons	    */
    lmz_set_defs_button_value(id,pex->defs_type);
    wi_activate( id, LM_DEFS_NAME_SEL, ON);

			     /*	fill selector box			    */ 
    lmz_defs_fill_selector(pex);
    
			     /*	initialize first value and open dialog	    */
    fi_push();
    wi_open_dialog(id);

			     /*	loop for every event until DONE or CANCEL   */
    for (event_item = -1, event_id = id; 
	event_id == id && NOT done;)
	{
			     /*	wait for event				    */
	wi_request_event(0,&event_class,&event_id,&event_item);

			     /*	handle each cases of button press	    */
	switch (event_item)
	    {
	    /*	 DONE	*/
	    case LM_DEFS_DONE_BUTTON :
		if(IS_EMPTY_STRING(pex->defs_name))
		  {
		    am_message(AM_ALERT,"No Color/Fill Definition name has \
been chosen.\nPlease select one or choose Create/Modify or Cancel.");
		    break;
		  }

		am_release_resource(id);
		status = SUCCESS;
		done = TRUE;
		break;

	     /*	CANCEL	*/
	    case LM_DEFS_CANCEL_BUTTON:
		am_release_resource(id);
		if(pex->defs_name_list != (TCP)0)
		    {
		    ts_tcp_free( pex->defs_name_list);
		    pex->defs_name_list = (TCP)0;
		    }
		status = FAIL;
		done = TRUE;
		break;

	     /*	DATE */
	    case LM_DEFS_DATE_R_BUTTON:
		pex->defs_type=LM_DATE;

		pex->defs_name[0]=0;
		if(pex->defs_name_list != (TCP)0)
		    {
		    ts_tcp_free( pex->defs_name_list);
		    pex->defs_name_list = (TCP)0;
		    }
		wi_set_dialog_text (id, LM_DEFS_NAME_SEL,"");
		lmz_defs_fill_selector(pex);	    
		break;

	     /*	TEXT */
	    case LM_DEFS_TEXT_R_BUTTON:
		pex->defs_type=LM_TEXT;

		pex->defs_name[0]=0;
		if(pex->defs_name_list != (TCP)0)
		    {
		    ts_tcp_free( pex->defs_name_list);
		    pex->defs_name_list = (TCP)0;
		    }
		wi_set_dialog_text (id, LM_DEFS_NAME_SEL,"");
		lmz_defs_fill_selector(pex);	    
		break;

	    /* NUMERIC */
	    case LM_DEFS_NUMERIC_R_BUTTON:
		pex->defs_type=LM_NUMERIC;

		pex->defs_name[0]=0;
		if(pex->defs_name_list != (TCP)0)
		    {
		    ts_tcp_free( pex->defs_name_list);
		    pex->defs_name_list = (TCP)0;
		    }
		wi_set_dialog_text (id, LM_DEFS_NAME_SEL,"");
		lmz_defs_fill_selector(pex);	    
		break;

	    /* TABLE SELECTOR BOX */
	    case LM_DEFS_NAME_SEL:
		wi_query_selections (id, LM_DEFS_NAME_SEL, &nsel, item_selected);
		if ( nsel > 0 )
		    {
		    wi_set_default_sel( id, LM_DEFS_NAME_SEL, item_selected[0]);
		    strcpy(pex->defs_name,pex->defs_name_list[item_selected[0]-1]);
		}
		break;

	    /* CREATE */
	    case LM_DEFS_CREATE_BUTTON :
		pex->defs_name[0]=0;
		lmz_entrs_server(pex);
		if(pex->defs_name_list != (TCP)0)
		    {
		    ts_tcp_free( pex->defs_name_list);
		    pex->defs_name_list = (TCP)0;
		    }
		lmz_set_defs_button_value(id,pex->defs_type);
		lmz_defs_fill_selector(pex);		
		break;

	    /* MODIFY */
	    case LM_DEFS_MODIFY_BUTTON :
		lmz_entrs_server(pex);
		if(pex->defs_name_list != (TCP)0)
		    {
		    ts_tcp_free( pex->defs_name_list);
		    pex->defs_name_list = (TCP)0;
		    }
		lmz_set_defs_button_value(id,pex->defs_type);
		lmz_defs_fill_selector(pex);		
		break;
	    }
	}

    fi_pop();
    return status;
}


/* Function Description -----------------------------------------------------
Description:
    Color/fill definition name user interface

Prototype:
    static INT lmz_entrs_server(LM_DIALOG_STRUCT *pex)

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I/O) pex  - (LM_DIALOG_STRUCT *) lease overlay dialog boxes structure

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PRIVATE
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/
static INT lmz_entrs_server(pex)
LM_DIALOG_STRUCT *pex;

    {
    INT   status;
    INT id,event_class,event_id,event_item;
    INT	    unused;
    INT item_selected[1];
    INT nsel = 0;
    CHAR msg[256];
    
                                   /* allocate dialog1   */
    status = am_define_resource(AM_DIALOG, LM_CDLGS_RSRC, LM_ENTRS,
                                NULL_FUNCTION_PTR, &pex->lm_entrs, pex);

			     /*	initialize first value and open dialog	    */
    id = pex->lm_entrs;	    
    pex->save_mth_n_day.save = FALSE;
                   
       /* activate selector boxes */
    wi_activate( id, LM_ENTRS_CONTROL_SEL, ON);
    wi_activate( id, LM_ENTRS_REPRESENT_SEL, ON);
    wi_activate( id, LM_ENTRS_CNTRL_SEL, ON);

       /* set selection limit to the selector boxes */
    wi_set_selection_limit (id, LM_ENTRS_CONTROL_SEL, 1);
    wi_set_selection_limit (id, LM_ENTRS_REPRESENT_SEL,1);

       /* write the definition name on the dialog box */
    wi_set_dialog_text(id,LM_ENTRS_NAME_EDIT,pex->defs_name);

        /* TEMPORARILY disable unused button */
    wi_enable_dialog_item(id,LM_ENTRS_FILL_R_BUTTON,OFF);
    
        /* load all the information on this definition name and
	   store it in the structure */

    lmz_load_name(pex);

        /* set all the radio button value according to the 
	   information stored by the latest update or take
	   the default value */
    switch(pex->entrs_type)
      {
      case LM_DATE:
        wi_set_dialog_value(id,LM_ENTRS_DATE_R_BUTTON,ON);
        break;
      case LM_TEXT:
        wi_set_dialog_value(id,LM_ENTRS_TEXT_R_BUTTON,ON);
        break;
      case LM_NUMERIC:
        wi_set_dialog_value(id,LM_ENTRS_NUMERIC_R_BUTTON,ON);
        break;
      }
   switch(pex->entrs_type)
     {
     case LM_COLOR:
       wi_set_dialog_value(id,LM_ENTRS_COLOR_R_BUTTON,ON);
       break;
     case LM_FILL_STYLE:
       wi_set_dialog_value(id,LM_ENTRS_FILL_R_BUTTON,ON);
       break;
     }

   switch (pex->entrs_date_format)
     {
     case LM_MDY :
       wi_set_dialog_value(id,LM_ENTRS_MDY_R_BUTTON,ON);
       break;
     case LM_DMY :
       wi_set_dialog_value(id,LM_ENTRS_DMY_R_BUTTON,ON);
       break;
     case LM_MY :
       wi_set_dialog_value(id,LM_ENTRS_MY_R_BUTTON,ON);
       break;
     case LM_Y:
       wi_set_dialog_value(id,LM_ENTRS_Y_R_BUTTON,ON);
       break;
     }

        /* open dialog box */
    fi_push();    
    wi_open_dialog(id);

			     /*	loop for every event and exit when DONE or  */
			     /*	CANCEL					    */
    for (event_item = -1, event_id = id; 
	event_id == id && 
	event_item != LM_ENTRS_DONE_BUTTON && 
	event_item != LM_ENTRS_CANCEL_BUTTON;)
	{
	wi_request_event(0,&event_class,&event_id,&event_item);
	switch (event_item)
	    {
	    /* DONE */
	    case LM_ENTRS_DONE_BUTTON :
		if(! pex->entrs_no_changes)
		    {
		    status=lmz_check_save_changes(pex,
					    "Do you want to save any changes?");
		    if(status != SUCCESS) break;
		    }

		   /* reset all the button value  and close dialog */
		wi_clear(id);
		am_release_resource(id);	    

		if(pex->save_mth_n_day.save)
		    {   
		    pex->save_mth_n_day.save = FALSE;
		    tc_free(pex->save_mth_n_day.mth);
		    tc_free(pex->save_mth_n_day.day);
		    }

		/* free memory */
		lmz_entrs_free_memory(pex);
		break;

	    /* SAVE */
	    case LM_ENTRS_SAVE_BUTTON :
		status=lmz_get_edittext(id,LM_ENTRS_NAME_EDIT,pex->defs_name,
					"Color/Fill Definition Name");
		if(status EQUALS SUCCESS)
		    {
		    status=lmz_save_name(pex);
		    if(status EQUALS SUCCESS)
		      {
			lmz_disable_radio_buttons(id);
			pex->entrs_no_changes=TRUE;
			sprintf(msg,"Saving Color/Fill Definition %s was \
completed succesfully.",pex->defs_name);
		      }
		    else
		      {
			sprintf(msg,"Saving Color/Fill Definition %s failed \
with error: %s. Please contact your system administrator.",
			pex->defs_name,mg_message(status));
		      }
		    am_message(AM_ALERT,msg);
		    wi_set_dialog_text(id,LM_ENTRS_NAME_EDIT,pex->defs_name);
		    }
		break;

	    /* LOAD */
	    case LM_ENTRS_LOAD_BUTTON :
		if(! pex->entrs_no_changes)
		    {
		    status=lmz_check_save_changes(pex,
			"Do you want to save any changes?");
		    if(status != SUCCESS) break;
		    wi_clear(id);
		    lmz_entrs_free_memory(pex);
		    lmz_set_button_value(id,pex);
		    lmz_enable_radio_buttons(id);
		    pex->entrs_no_changes=TRUE;
		    }
		if(pex->save_mth_n_day.save)
		    {   
		    pex->save_mth_n_day.save = FALSE;
		    tc_free(pex->save_mth_n_day.mth);
		    tc_free(pex->save_mth_n_day.day);
		    }
		status=lmz_get_edittext(id,LM_ENTRS_NAME_EDIT,pex->defs_name,
					"Color/Fill Definition Name");
		if(status EQUALS SUCCESS)
		    lmz_load_name(pex);
		break;
	    
	    /* CANCEL */
	    case LM_ENTRS_CANCEL_BUTTON:

		wi_clear(id);
		am_release_resource(id);

		if(pex->save_mth_n_day.save)
		    {   
		    pex->save_mth_n_day.save = FALSE;
		    tc_free(pex->save_mth_n_day.mth);
		    tc_free(pex->save_mth_n_day.day);
		    }

		/* free memory */
		lmz_entrs_free_memory(pex);
		break;

	    /* RANGE CHECKITEM */
	    case LM_ENTRS_RANGE_C_ITEM :
	        if(pex->entrs_range)
		  {
		    pex->entrs_range=FALSE;
		    wiscdv(id,LM_ENTRS_RANGE_C_ITEM,0);
		  }
		else
		  {
		    pex->entrs_range=TRUE;
		    wiscdv(id,LM_ENTRS_RANGE_C_ITEM,1);
		  }
		break;
	    
	    /* DATE */
	    case LM_ENTRS_DATE_R_BUTTON:
		status=lmz_adjust_controls(pex->defs_type,LM_DATE,pex);
		if(status == SUCCESS)	    
		    {
		    pex->defs_type = LM_DATE;
		    lmz_enable_date_buttons(id);
		    }
		else
		    lmz_set_button_value(id,pex);
		break;

	    /* TEXT */
	    case LM_ENTRS_TEXT_R_BUTTON:
		lmz_adjust_controls(pex->defs_type,LM_TEXT,pex);
		pex->defs_type = LM_TEXT;
		lmz_disable_date_buttons(id);
		break;

	    /* NUMERIC */
	    case LM_ENTRS_NUMERIC_R_BUTTON:
		lmz_adjust_controls(pex->defs_type,LM_NUMERIC,pex);
		pex->defs_type = LM_NUMERIC;
		lmz_disable_date_buttons(id);
		break;

	    /* CONTROL SELECTOR BOX */
	    case LM_ENTRS_CONTROL_SEL:
		wi_query_selections (id, LM_ENTRS_CONTROL_SEL, &nsel, item_selected);
		if ( nsel > 0 )
		    {
		    wi_set_default_sel( id, LM_ENTRS_CONTROL_SEL,item_selected[0]);
		    strcpy(pex->entrs_typeEntry,
			pex->entrs_typeEntry_list[item_selected[0]-1]);

		    wi_set_default_sel( id, LM_ENTRS_REPRESENT_SEL, item_selected[0]);
		    strcpy(pex->entrs_cfEntry,
			pex->entrs_cfEntry_list[item_selected[0]-1]);
		    }
		break;

	    /* REPRESENT BY SELECTOR BOX */
	    case LM_ENTRS_REPRESENT_SEL:
		wi_query_selections (id, LM_ENTRS_REPRESENT_SEL, &nsel, item_selected);
		if ( nsel > 0 )
		    {
		    wi_set_default_sel( id, LM_ENTRS_CONTROL_SEL,item_selected[0]);
		    strcpy(pex->entrs_typeEntry,
			pex->entrs_typeEntry_list[item_selected[0]-1]);

		    wi_set_default_sel( id, LM_ENTRS_REPRESENT_SEL, item_selected[0]);
		    strcpy(pex->entrs_cfEntry,
			pex->entrs_cfEntry_list[item_selected[0]-1]);
		    }
		break;

	    /* CONTROL SELECTION SELECTOR BOX */
	    case LM_ENTRS_CNTRL_SEL:
		wi_query_selections (id, LM_ENTRS_CNTRL_SEL, &nsel, item_selected);
		if ( nsel > 0 )
		    {
		    wi_set_default_sel( id, LM_ENTRS_CNTRL_SEL, item_selected[0]);
		    strcpy(pex->entrs_cfPick,
			pex->entrs_cfPick_list[item_selected[0]-1]);
		    }
		break;

	    /* COLOR */
	    case LM_ENTRS_COLOR_R_BUTTON:
		if(! pex->entrs_no_changes)
		    {
		    status=lmz_check_save_changes(pex,
			"Do you want to save your current work?");
		    if(status != SUCCESS) break;
		    }		
		pex->entrs_type = LM_COLOR;
		wi_clear(id);
		lmz_entrs_free_memory(pex);
		lmz_set_button_value(id,pex);
		lmz_entrs_fill_selEntry(id,pex);
		break;

	    /* FILL BY */
	    case LM_ENTRS_FILL_R_BUTTON:
		if(! pex->entrs_no_changes)
		    {
		    status=lmz_check_save_changes(pex,
			"Do you want to save your current work?");
		    if(status != SUCCESS) break;
		    }
		pex->entrs_type = LM_FILL_STYLE;
		wi_clear(id);
		lmz_entrs_free_memory(pex);
		lmz_set_button_value(id,pex);
		lmz_entrs_fill_selEntry(id,pex);
		break;

	    /* MM/DD/YY DATE FORMAT */
	    case LM_ENTRS_MDY_R_BUTTON :
		lmz_adjust_controls(pex->entrs_date_format,LM_MDY,pex);
		pex->entrs_date_format = LM_MDY;
		break;

	    /* DD/MM/YY DATE FORMAT */
	    case LM_ENTRS_DMY_R_BUTTON :
		lmz_adjust_controls(pex->entrs_date_format,LM_DMY,pex);
		pex->entrs_date_format = LM_DMY;
		break;

	    /* MM/YY DATE FORMAT */
	    case LM_ENTRS_MY_R_BUTTON :
		lmz_adjust_controls(pex->entrs_date_format,LM_MY,pex);
		pex->entrs_date_format = LM_MY;
		break;

	    /* YY DATE FORMAT */
	    case LM_ENTRS_Y_R_BUTTON :
		lmz_adjust_controls(pex->entrs_date_format,LM_Y,pex);
		pex->entrs_date_format = LM_Y;
		break;

	    /* ADD BUTTON */
	    case LM_ENTRS_ADD_BUTTON :
		status=lmz_get_edittext(id,LM_ENTRS_CNTRL_ENT_EDT,
	    			    pex->entrs_controlEntry,
				    "Color/Fill Control Entry");
		if (status EQUALS SUCCESS)	
		    {
		    pex->entrs_no_changes=FALSE;
		    lmz_add_entry(id,pex);
		    wi_set_dialog_text (id, LM_ENTRS_CNTRL_ENT_EDT,"");
		    }
		break;

	    /* REMOVE */
	    case LM_ENTRS_REMOVE_BUTTON :
	        if(IS_EMPTY_STRING(pex->entrs_typeEntry))
		  {
		    am_message(AM_ALERT,"Please pick the entry you want to \
remove by hiliting one of the Color/Fill Control.");
		   break;
		  }
	        strcpy(pex->entrs_controlEntry,pex->entrs_typeEntry);
		pex->entrs_no_changes=FALSE;
		lmz_remove_entry(pex);
		break;
	    }
	}

    lmz_enable_radio_buttons(id);
    fi_pop();
    return SUCCESS;
}





