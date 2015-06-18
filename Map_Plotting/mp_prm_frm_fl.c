/*  DEC/CMS REPLACEMENT HISTORY, Element MP_PRM_FRM_FL.C*/
/*  *5    14-AUG-1991 14:01:05 JULIAN "(SPR 3655) added check in case defined list is empty"*/
/*  *4    17-AUG-1990 22:08:26 VINCE "(SPR 5644) Code Cleanup"*/
/*  *3    23-OCT-1989 10:36:37 CONROY "(SPR 5043) Mods for new MAPPING UI programmer tools"*/
/*  *2    26-JUN-1989 10:59:23 GILLESPIE "(SPR 20) Fix castings"*/
/*  *1    19-JUN-1989 13:14:25 SYSTEM ""*/
/*  DEC/CMS REPLACEMENT HISTORY, Element MP_PRM_FRM_FL.C*/
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
#include "esi_gl_defs.h"
#include "esi_mp.h"
#include "esi_am.h"
#include "esi_fm.h"
#include "esi_fm_err.h"
#include "esi_mg.h"
#include "esi_md_dbdefs.h"
#include "esi_md.h"
#include "esi_wi.h"
#include "esi_ts.h"
#include "esi_qs.h"
#include "md_mapdef.rh"

/* Function Description -----------------------------------------------------
Description:
    Routine to get the parameter input from a list of
    files of specified type.

Prototype:
    publicdef INT mp_parm_from_file(BOOL edit_flag,MD_KEY_INFO *key_info,FM_LONG parm_string,
        MD_PARM_STRUCT **parmstruc);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    edit_flag       -(BOOL)
    key_info        -(MD_KEY_INFO *)
    parm_string     -(FM_LONG)
    parmstruc       -(MD_PARM_STRUCT **)

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.
    FM_NO_ACCESS
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT mp_parm_from_file (BOOL edit_flag, MD_KEY_INFO *key_info,
                                 FM_LONG parm_string, MD_PARM_STRUCT **parmstruc)
#else
publicdef INT mp_parm_from_file (edit_flag, key_info, parm_string, parmstruc)
BOOL edit_flag;
MD_KEY_INFO *key_info;
FM_LONG parm_string;
MD_PARM_STRUCT **parmstruc;
#endif
    {
    
    INT status;
    MAP_STRUCTURE *map;
    MD_STATE_WORKSPACE *md_state;
    RESOURCE_ID *cdlg_id;
    INT id;
    MP_FILES *file_struct;
    CHAR **tcp;
    INT size;
    MD_PARM_STRUCT *temp_parm;
    INT value;
    INT n;
    INT event_class, event_id, event_item;
    FM_TYPE type;
    INT write;
    ANALYST_NAME analyst;
    ANALYST_NAME creator;
    FM_LONG show_string;
    FM_LONG long_in_file;
    
    /* ********************************************************************** */
    
    am_open_workspace ("CDLG", AM_APPLICATION, (VOIDPTR *) & cdlg_id);
    am_open_workspace ("MAPDEF", AM_APPLICATION, (VOIDPTR *) & map);
    am_open_workspace ("MAPDEF_STATE", AM_GLOBAL, (VOIDPTR *) & md_state);
    
    write = (key_info->val_code EQUALS FILEW$);
    qs_inq_c (QS_FINDER_ACCOUNT, analyst, (INT *)0);
    
    /* * SET UP DIALOG * */
    /* ----------------- */
    
    if (cdlg_id->file_select_id EQUALS 0)
        {
        am_define_resource (AM_DIALOG, "MD_MAPDEF", MP_GET_FILE, NULL_FUNCTION_PTR,
                            &(cdlg_id->file_select_id), NULL);
        }
    else
        {
        wi_clear (cdlg_id->file_select_id);
        }
    id = cdlg_id->file_select_id;
    
    /* * SET FILE TYPE * */
    /* ----------------- */
    
    strcpy (type, key_info->val_arg + 4);
    type[strlen (type) - 1] = 0;
    wi_set_dialog_text (id, MP_FILE_TYPE, type);
    mpz_get_file_struct (type, &file_struct);
    
    /* * SET DEFAULT * */
    /* --------------- */
    
    if (edit_flag)
        {
        wi_set_dialog_text (id, MP_FILE_CHOICE, parm_string);
        fm_long_name (parm_string, long_in_file);
        }
    else
        {
        if (strlen (file_struct->last_used) != 0)
            {
            if (write)
                {
                fm_parse_long_name (file_struct->last_used, creator, show_string);
                if (strcmp (creator, analyst) != 0)
                    {
                    strcpy (show_string, "");
                    }
                }
            else
                {
                strcpy (show_string, file_struct->last_used);
                }
            wi_set_dialog_text (id, MP_FILE_CHOICE, show_string);
            }
        }
    /* * GET DISK FILES * */
    /* ------------------ */
    
    fm_file_list (key_info->val_arg, type, FM_NAME, FM_ASC, write, &tcp, &size);
    wi_set_selections (id, MP_EXIST_SELECT, MP_EXIST_SCROLL, size, 1, tcp);
    wi_activate (id, MP_EXIST_SELECT, ON);
    wi_set_default_sel (id, MP_EXIST_SELECT, 0);
    
    /* * GET DEFINED FILES * */
    /* --------------------- */
    
    wi_set_selections (id, MP_DEFINED_SELECT, MP_DEFINED_SCROLL, file_struct->n_names,
                       1, file_struct->name);
    wi_activate (id, MP_DEFINED_SELECT, ON);
    wi_set_default_sel (id, MP_DEFINED_SELECT, 0);
    
    /* * OPEN THE DIALOG * */
    /* ------------------- */
    
    wi_open_dialog (id);
    
    /* * GET THE PARAMETER FROM THE USER * */
    /* ----------------------------------- */
    
    mp_set_mapdef_buttons ();
    status = FAIL;
    while (status EQUALS FAIL)
        {
        wi_request_event (0, &event_class, &event_id, &event_item);
        if (event_id EQUALS id)
            {
            switch (event_item)
                {
                /* SELECTION FROM EXISTING FILES LIST */
                
            case MP_EXIST_SELECT:
                wi_query_selections (id, MP_EXIST_SELECT, &value, &n);
                wi_set_default_sel (id, MP_EXIST_SELECT, n);
                wi_set_default_sel (id, MP_DEFINED_SELECT, 0);
                wi_set_dialog_text (id, MP_FILE_CHOICE, tcp[n - 1]);
                break;
                
                /* SELECTION FROM DEFINED FILES LIST */
                
            case MP_DEFINED_SELECT:
		/* check if defined files list is empty, if it is then do not
		   do anything */
		if(file_struct->name EQUALS 0) break;

                wi_query_selections (id, MP_DEFINED_SELECT, &value, &n);
                if (write)
                    {
                    fm_parse_long_name (file_struct->name[n - 1], creator,
                                        show_string);
                    if (strcmp (creator, analyst) != 0)
                        {
                        status = FM_NO_ACCESS;
                        break;
                        }
                    }
                else
                    {
                    strcpy (show_string, file_struct->name[n - 1]);
                    }
                wi_set_default_sel (id, MP_DEFINED_SELECT, n);
                wi_set_default_sel (id, MP_EXIST_SELECT, 0);
                wi_set_dialog_text (id, MP_FILE_CHOICE, show_string);
                break;
                
                /* OK BUTTON PUSHED */
                
            case MP_FILE_OK:
                wi_query_dialog_item (id, MP_FILE_CHOICE, &value, show_string);
                ts_sto_upper (show_string);
                
                /* CHECK FOR CREATOR/WRITE COMBO */
                
                if (write)
                    {
                    fm_parse_long_name (show_string, creator, show_string);
                    if (strcmp (analyst, creator) != 0)
                        {
                        status = FM_NO_ACCESS;
                        break;
                        }
                    }
                else
                    {
                    fm_long_name (show_string, show_string);
                    }
                wi_set_dialog_text (id, MP_FILE_CHOICE, show_string);
                
                mp_parm_ok (show_string, md_state->active_verb_ndx,
                            md_state->active_key_ndx, &temp_parm, &status);
                
                /* UPDATE FILE STRUCTURE */
                
                if (status EQUALS SUCCESS)
                    {
                    if (edit_flag)
                        {
                        if (strcmp (long_in_file, temp_parm->parmval.cval) != 0)
                            {
                            mpz_drop_from_file_struct (long_in_file, file_struct);
                            mpz_add_to_file_struct (key_info->val_arg,
                                                    temp_parm->parmval.cval, type,
                                                    file_struct);
                            }
                        }
                    else
                        {
                        mpz_add_to_file_struct (key_info->val_arg,
                                                temp_parm->parmval.cval, type,
                                                file_struct);
                        }
                    }
                break;
                
                /* CANCEL BUTTON PUSHED */
                
            case MP_FILE_CANCEL:
                status = CANCEL;
                break;
                
                /* SOME UNKNOWN EVENT FROM DIALOG */
                
            default:
                wi_beep (1);
                break;
                }
            }
        /* SOME UNKNOWN EVENT FROM OUTSIDE DIALOG */
        
        else
            {
            wi_beep (1);
            }
        /* DETERMINE STATUS AND NEXT ACTION * */
        /* ---------------------------------- */
        
        if (status EQUALS SUCCESS)
            {
            md_free_parms (*parmstruc);
            *parmstruc = temp_parm;
            strcpy (parm_string, show_string);
            }
        else if (status != CANCEL AND status != FAIL)
            {
            wi_alert (0, "", "PARAMETER ENTRY ERROR:", mg_message (status));
            status = FAIL;
            }
        }
    wi_close_dialog (id);
    ts_tcp_free (tcp);
    return status;
    }
/* END:     */
