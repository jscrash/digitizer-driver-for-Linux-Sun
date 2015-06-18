/* DEC/CMS REPLACEMENT HISTORY, Element DB_SQLP_USER.C */
/* *6    11-JUN-1991 13:50:40 JULIAN "(SPR 8318) fix crash on selector box <-->" */
/* *5    14-AUG-1990 13:10:07 VINCE "(SPR 5644) Header Standardization" */
/* *4     1-APR-1990 20:01:03 SCD "(SPR 1) Ensure response is forced to upper case." */
/* *3    20-SEP-1989 14:53:50 GORDON "(SPR 200) Get in sync with Gulf" */
/* *2    11-AUG-1989 20:35:13 CONROY "(SPR 0) Revised global, selects, SQL macros & fixes" */
/* *1    11-AUG-1989 19:40:51 CONROY "SQL Macro Processor" */
/* DEC/CMS REPLACEMENT HISTORY, Element DB_SQLP_USER.C */
/*****************************************************************************/
/******************************************************************************

                Copyright Finder Graphics Systems, Inc. 1990
                    Unpublished -- All rights reserved

THIS SOFTWARE IS THE PROPRIETARY PROPERTY OF Exploration Systems, Inc.  AND MAY
CONTAIN CONFIDENTIAL TRADE SECRET INFORMATION. IT IS LICENSED FOR USE ON THE
DESIGNATED EQUIPMENT ON WHICH IT WAS ORIGINALLY INSTALLED AND  MAY NOT BE
MODIFIED, DUPLICATED OR COPIED IN ANY FORM WITHOUT PRIOR WRITTEN CONSENT OF

                            Finder Graphics Systems
                            201 Tamal Vista Blvd.
                            Corte Madera, CA  94925
                            (415) 927-0100

(The above notice does not constitute an admission of publication or
unrestricted dissemination of the work)

******************************************************************************/

#include "esi_am.h"
#include "esi_db_sql_parms.h"
#include "db_sqlp_cdlg.rh"
#include "esi_qs.h"
#include "esi_tc.h"
#include "esi_ts.h"
#include "esi_wi.h"

#define resource_file "DB_SQLP_CDLG"

/* Function Description -----------------------------------------------------
Description:
    Manage modal interaction with user to supply values for all
    substitution variables in an sql phrase.
 
    INITIAL CONDITIONS:
        
    1.  well-formed sub_vars structure list
                    
    STRATEGY:
        
    Sequence thru the list of substitution variable structs.
    The first time a user interaction is called for, put up dialog
    and leave it up until all variables are processed.

    Selections from a list (TLU operator) cause the SELECT FROM LIST button
    to be enabled.  The user can still just type in respose if known, or can
    hit SELECT FROM LIST to load the selector and activate it to generate 
    events.
            
    Each time user hits ENTER button, the corresponding response is placed in
    the structure, and the next structure is processed. When user hits ENTER
    on on the last structure, we put the dialog away and return.

Prototype:
    publicdef INT db_sql_parms_user (INT nvars, SUB_VAR_STRUCT *sub_vars);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) sql_phrase  -(CHAR *) The phrase to parse.
    (O) nvars       -(INT *) The number of variables.
    (O) sub_vars    -(SUB_VAR_STRUCT *) Pointer to list of structures.

Return Value/Status:
    SUCCESS - If all variables satisfied.
    FAIL - If trouble.
    CANCEL - If user hit CANCEL button. 

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT db_sql_parms_user (INT nvars, SUB_VAR_STRUCT *sub_vars) 
#else
publicdef INT db_sql_parms_user (nvars, sub_vars)
INT nvars;
SUB_VAR_STRUCT *sub_vars;
#endif
    {
    static INT cdlg_id = 0;     /* remember id allocated by am_resource */
    INT class , id, item, value, nsel;  /* wi_... parameters */
    INT item_selected[1];           /* only space to allow single selections */
    INT no_rows;                    /* number rows returned by ts_tcp_from_table() */
    INT status = SUCCESS, done = FALSE;
    INT cdlg_open = FALSE;
    INT l, num_subs = 0, max_len = 0;
    SUB_VAR_STRUCT *cs;
    CHAR operator_code[5], operator_parms[STMAX], *p;
    CHAR prompt[STMAX], table_name[33], column_name[33], project_name[33];
    CHAR **row_list = 0;
    
    /*-----------------------------------------------------------------*/
    /* handle boundary cases -- if no subsitutions req'd, just return  */
    /*-----------------------------------------------------------------*/
    
    if (nvars <= 0 || sub_vars == (SUB_VAR_STRUCT *)0)
        {
        return SUCCESS;
        }
    /*------------------------------------------*/
    /* initialize dialog upon first invocation  */
    /*------------------------------------------*/
    
    if (cdlg_id == 0)
        {
        status = am_define_resource (AM_DIALOG, resource_file, DB_SQL_PARMS,
                                     NULL_FUNCTION_PTR, &cdlg_id, (VOIDPTR)0);
        if (status != SUCCESS)
            return status;
        }
    /*--------------------------------------*/
    /* sequence thru sub variable structs   */
    /*--------------------------------------*/
    
    status = SUCCESS;
    for (cs = sub_vars; status == SUCCESS && cs != (SUB_VAR_STRUCT *)0; cs = cs->next)
        {
        strcpy (operator_code, "");
        strcpy (operator_parms, "");
        
        /* extract operator_code, if any */
        
        if (strlen (cs->prompt_operator) > 0)
            {
            /* see if there is a parameter list following operator */
            
            p = ts_find_char (cs->prompt_operator, '=');
            if (*p != '\0')
                {
                ts_copy_substring (operator_code, cs->prompt_operator, p - 1);
                p++;
                ts_copy_substring (operator_parms, p, p + strlen (p - 1));
                }
            else
                strcpy (operator_code, cs->prompt_operator);
            }
        /* ----------------------------------------------------*/
        /* handle non-interactive variables PROJ, ANAL, ...    */
        /* ----------------------------------------------------*/
        
        if (strcmp (operator_code, "PROJ") == 0)
            {
            qs_inq_c (QS_PROJECT_NAME, cs->response, (INT *)0);
            }
        else if (strcmp (operator_code, "ANAL") == 0)
            {
            qs_inq_c (QS_FINDER_ACCOUNT, cs->response, (INT *)0);
            }
        else
            {
            
            /*-----------------------------------------------------*/
            /* handle interaction for each type of prompt operator */
            /* <simple prompt> with or without DEFault, TLU, ...   */
            /*-----------------------------------------------------*/
            
            if (!cdlg_open)
                {
                /*----------------------------------*/
                /* initialize and open the dialog   */
                /*----------------------------------*/
                cdlg_open = TRUE;
                wi_push ();
                wi_set_event_mode (WI_DIALOG_EVENT, WI_EVENT_MODE);
                wi_set_selections (cdlg_id, DB_SQLP_SELECTOR, DB_SQLP_VSCROLL, 0, 1,
                                   row_list);
                wi_set_dialog_text (cdlg_id, DB_SQLP_KEY_ENTRY, "");
                wi_enable_dialog_item (cdlg_id, DB_SQLP_SELECT_FROM_LIST, OFF);
                wi_enable_dialog_item (cdlg_id, DB_SQLP_CANCEL, ON);
                wi_enable_dialog_item (cdlg_id, DB_SQLP_ENTER, ON);
                wi_set_default_button (cdlg_id, DB_SQLP_ENTER, ON);
                wi_open_dialog (cdlg_id);
                }
            if (strlen (operator_code) == 0)
                {
                wi_enable_dialog_item (cdlg_id, DB_SQLP_SELECT_FROM_LIST, OFF);
                }
            else if (strcmp (operator_code, "TLU") == 0)
                {
                wi_enable_dialog_item (cdlg_id, DB_SQLP_SELECT_FROM_LIST, ON);
                }
            else
                {
                /*----------------------------------------------------------*/
                /* unrecognized operator -- need new MG entry to report     */
                /*----------------------------------------------------------*/
                }
            sprintf (prompt, "Please enter a value for %s:", cs->prompt);
            wi_set_dialog_text (cdlg_id, DB_SQLP_PROMPT, prompt);
            wi_activate (cdlg_id, DB_SQLP_KEY_ENTRY, ON);
            
            /*--------------*/
            /* event loop   */
            /*--------------*/
            
            done = FALSE, status = SUCCESS;
            
            while (!done && status == SUCCESS)
                {
                wi_request_event (0, &class , &id, &item);
                if (id != cdlg_id)
                    return FAIL;    /* this sucker is modal */
                switch (item)
                    {
                case DB_SQLP_CANCEL:
                    status = CANCEL;
                    break;
                    
                case DB_SQLP_ENTER:
                    wi_query_dialog_item (cdlg_id, DB_SQLP_KEY_ENTRY, &value,
                                          cs->response);
                    ts_sto_upper (cs->response);
                    done = TRUE;
                    break;
                    
                case DB_SQLP_SELECT_FROM_LIST:
                    p = ts_find_char (operator_parms, ',');
                    if (*p != '\0')
                        {
                        ts_copy_substring (table_name, operator_parms, p - 1);
                        p++;
                        ts_copy_substring (column_name, p, p + strlen (p - 1));
                        if (strncmp ("PROJ.", table_name, 5) == 0)
                            {
                            qs_inq_c (QS_PROJECT_NAME, project_name, (INT *)0);
                            ts_sub_substring (table_name, "PROJ", project_name);
                            }
                        }
                    else
                        {
                        /* alert user to invalid TLU table name, column name */
                        }
                    ts_tcp_free (row_list); /* free last tcp, if any */
                    status = ts_tcp_from_table (table_name, column_name, &row_list,
                                                &no_rows);
                    if (status == SUCCESS)
                        {
                        wi_set_selections (cdlg_id, DB_SQLP_SELECTOR, DB_SQLP_VSCROLL,
                                           no_rows, 1, row_list);
                        wi_activate (cdlg_id, DB_SQLP_SELECTOR, ON);
                        }
                    break;
                    
                case DB_SQLP_SELECTOR:
                    wi_query_selections (cdlg_id, DB_SQLP_SELECTOR, &nsel,
                                         item_selected);
		    if(row_list != (CHAR **)0)
		      wi_set_dialog_text (cdlg_id, DB_SQLP_KEY_ENTRY,
					  row_list[item_selected[0] - 1]);
                    break;
                    
                default:
                    return FAIL;    /* unrecognized control id */
                    }
                }
            /*------------------------------*/
            /* free memory: row_list, ...   */
            /*------------------------------*/
            ts_tcp_free (row_list);
            }
        }
    if (cdlg_open)
        {
        wi_close_dialog (cdlg_id);
        wi_pop ();
        }
    return status;
    }
/* END:     */
