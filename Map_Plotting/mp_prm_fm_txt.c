/*  DEC/CMS REPLACEMENT HISTORY, Element MP_PRM_FM_TXT.C*/
/*  *4    17-AUG-1990 22:08:20 VINCE "(SPR 5644) Code Cleanup"*/
/*  *3    23-OCT-1989 10:35:54 CONROY "(SPR 5043) Mods for new MAPPING UI programmer tools"*/
/*  *2    26-JUN-1989 10:59:07 GILLESPIE "(SPR 20) Fix castings"*/
/*  *1    19-JUN-1989 13:14:18 SYSTEM ""*/
/*  DEC/CMS REPLACEMENT HISTORY, Element MP_PRM_FM_TXT.C*/
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
#include "esi_mg.h"
#include "esi_md_dbdefs.h"
#include "esi_md.h"
#include "esi_wi.h"
#include "md_mapdef.rh"
#include "esi_md_msg.h"
#include "esi_md_err.h"

/* Function Description -----------------------------------------------------
Description:
    Routine to get the parameter input from the input
    text dialog box.

Prototype:
    publicdef INT mp_parm_from_text(BOOL edit_flag,MD_KEY_INFO *key_info,
        CHAR parm_string[MD_MAX_PARM_STRING],MD_PARM_STRUCT **);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    edit_flag       -(BOOL)
    key_info        -(MD_KEY_INFO *)
    parm_string     -(CHAR [MD_MAX_PARM_STRING])
    parmstruc       -(MD_PARM_STRUCT **)

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.
    CANCEL
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT mp_parm_from_text (BOOL edit_flag, MD_KEY_INFO *key_info,
                                 CHAR parm_string[MD_MAX_PARM_STRING],
                                 MD_PARM_STRUCT **parmstruc)
#else
publicdef INT mp_parm_from_text (edit_flag, key_info, parm_string, parmstruc)
BOOL edit_flag;
MD_KEY_INFO *key_info;
CHAR parm_string[MD_MAX_PARM_STRING];
MD_PARM_STRUCT **parmstruc;
#endif
    {
    
    INT status;
    MD_PARM_STRUCT *temp_parm;
    MD_STATE_WORKSPACE *md_state;
    CHAR temp_string[MD_MAX_PARM_STRING];
    CHAR mess[80];
    RESOURCE_ID *cdlg_id;
    INT value;
    INT event_class;
    INT event_id;
    INT event_item;
    
    /* ********************************************************************** */
    
    am_open_workspace ("CDLG", AM_APPLICATION, (VOIDPTR *) & cdlg_id);
    am_open_workspace ("MAPDEF_STATE", AM_GLOBAL, (VOIDPTR *) & md_state);
    mp_set_mapdef_buttons ();
    
    /* * GET THE TEXT INPUT RESOURCE * */
    /* ------------------------------- */
    
    if (cdlg_id->input_text_id EQUALS 0)
        {
        am_define_resource (AM_DIALOG, "MD_MAPDEF", MD_INPUT_TEXT, NULL_FUNCTION_PTR,
                            &(cdlg_id->input_text_id), NULL);
        }
    else
        {
        wi_clear (cdlg_id->input_text_id);
        }
    /* * SET THE TITLE TEXT FOR THE DIALOG * */
    /* ------------------------------------- */
    
    switch (key_info->val_code)
        {
    case UOMS$:
    case UOMD$:
        sprintf (mess, "%s", mg_message (MD_PARM_UOM));
        break;
        
    case NUM$:
        sprintf (mess, "%s", mg_message (MD_PARM_NUM));
        break;
        
    case NLIST$:
        sprintf (mess, "%s %s", mg_message (MD_PARM_NUM_RANGE), key_info->val_arg);
        break;
        
    case SQL$:
        sprintf (mess, "%s", mg_message (MD_PARM_KEY_LIST));
        break;
        
    default:
        sprintf (mess, "%s", mg_message (MD_PARM_TEXT_ENTRY));
        break;
        }
    wi_set_dialog_text (cdlg_id->input_text_id, MD_INPUT_TEXT_MESSAGE, mess);
    
    /* * SET TEXT AND CANCEL BUTTON * */
    /* ------------------------------ */
    
    wi_enable_dialog_item (cdlg_id->input_text_id, MD_INPUT_TEXT_CANCEL, ON);
    if (edit_flag)
        {
        wi_set_dialog_text (cdlg_id->input_text_id, MD_INPUT_TEXT_FIELD, parm_string);
        }
    else
        {
        if (strlen (key_info->key_default) > 0)
            {
            wi_set_dialog_text (cdlg_id->input_text_id, MD_INPUT_TEXT_FIELD,
                                key_info->key_default);
            }
        }
    /* * OPEN THE DIALOG * */
    /* ------------------- */
    
    wi_open_dialog (cdlg_id->input_text_id);
    wi_activate (cdlg_id->input_text_id, MD_INPUT_TEXT_FIELD, ON);
    
    /* * GET THE PARAMETER FROM THE USER * */
    /* ----------------------------------- */
    
    status = FAIL;
    while (status EQUALS FAIL)
        {
        wi_request_event (0, &event_class, &event_id, &event_item);
        if (event_id EQUALS cdlg_id->input_text_id)
            {
            if (event_item EQUALS MD_INPUT_TEXT_OK)
                {
                wi_query_dialog_item (cdlg_id->input_text_id, MD_INPUT_TEXT_FIELD,
                                      &value, temp_string);
                
                /* * VALIDATE PARAMETER * */
                /* ---------------------- */
                
                mp_parm_ok (temp_string, md_state->active_verb_ndx,
                            md_state->active_key_ndx, &temp_parm, &status);
                }
            else if (event_item EQUALS MD_INPUT_TEXT_CANCEL)
                {
                status = CANCEL;
                }
            else
                {
                wi_beep (1);
                }
            }
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
            strcpy (parm_string, temp_string);
            }
        else if (status != CANCEL AND status != FAIL)
            {
            wi_alert (0, "", "PARAMETER ENTRY ERROR:", mg_message (status));
            status = FAIL;
            }
        }
    wi_close_dialog (cdlg_id->input_text_id);
    
    return status;
    }
/* END:     */
