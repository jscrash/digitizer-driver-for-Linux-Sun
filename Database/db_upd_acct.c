/* DEC/CMS REPLACEMENT HISTORY, Element DB_UPD_ACCT.C*/
/* *6    14-AUG-1990 13:10:45 VINCE "(SPR 5644) Header Standardization"*/
/* *5    14-OCT-1989 15:57:00 VINCE "(SPR -1) mods for new update schem"*/
/* *4    19-SEP-1989 13:36:06 GILLESPIE "(SPR 100) GULF MODS"*/
/* *3    13-AUG-1989 03:13:33 GILLESPIE "(SPR 5) Add new steps for Oracle V6"*/
/* *2    26-JUN-1989 10:40:01 GILLESPIE "(SPR 20) Fix castings"*/
/* *1    19-JUN-1989 12:45:47 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element DB_UPD_ACCT.C*/
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
#include "esi_db.h"

#ifndef ESI_AM_H

#include "esi_am.h"

#endif

#ifndef ESI_DB_MSG_H

#include "esi_db_msg.h"

#endif

#ifndef ESI_MG_H

#include "esi_mg.h"

#endif

#ifndef ESI_DB_CR_ACCT_H

#include "esi_db_cr_acct.rh"

#endif

#ifndef ESI_WI_H

#include "esi_wi.h"

#endif

/* Function Description -----------------------------------------------------
Description:
    Routine to update existing ORACLE resources.

Prototype:
    publicdef INT dba_update_account(DB_ACCOUNT_STRUCT *db_account, 
                                DB_CREATE_FLAGS *create_flags);
                                
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) db_account  -(DB_ACCOUNT_STRUCT *) Account structure.
    (I) create_flags-(DB_CREATE_FLAGS *) Flags structure.

Return Value/Status:
    SUCCESS - Successful completion
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT dba_update_account (DB_ACCOUNT_STRUCT *db_account,
                                  DB_CREATE_FLAGS *create_flags) 
#else
publicdef INT dba_update_account (db_account, create_flags)
DB_ACCOUNT_STRUCT *db_account;
DB_CREATE_FLAGS *create_flags;
#endif
    {
    INT fetchcode;
    INT status;
    BOOL use_dialog;
    BOOL do_any;
    INT i = 0;
    INT *id = (INT *)0;
    CHAR string[128];
    
    do_any = (create_flags->create_constraints || create_flags->create_indexes ||
              create_flags->create_sequences || create_flags->create_tables ||
              create_flags->create_views || create_flags->create_synonyms ||
              create_flags->grant_privileges);
    
    if (do_any)
        {
        sprintf (string, mg_message (DB_ORACLE_UPDATE_INIT), db_account->scope,
                 db_account->name);
        am_message (AM_DIALOG, string);
        am_message (AM_STATUS, string);
        }
    /* get ID of status dialog */
    am_open_workspace ("DB_STATUS_ID", AM_APPLICATION, (VOIDPTR *) & id);
    
    use_dialog = FALSE;
    if (id != (INT *)0)
        use_dialog = TRUE;
    
    if (create_flags->create_tables)
        {
        if (use_dialog)
            wi_set_default_sel (*id, DB_STATUS_LIST, 1);
        dba_update_tables (db_account);
        }
    if (create_flags->create_indexes)
        {
        if (use_dialog)
            wi_set_default_sel (*id, DB_STATUS_LIST, 2);
        dba_update_indexes (db_account);
        }
    if (create_flags->create_views)
        {
        if (use_dialog)
            wi_set_default_sel (*id, DB_STATUS_LIST, 3);
        dba_update_views (db_account);
        }
    if (create_flags->create_synonyms)
        {
        if (use_dialog)
            wi_set_default_sel (*id, DB_STATUS_LIST, 4);
        dba_update_synonyms (db_account);
        }
    if (create_flags->create_sequences)
        {
        if (use_dialog)
            wi_set_default_sel (*id, DB_STATUS_LIST, 5);
        dba_update_sequences (db_account);
        }
    if (create_flags->create_constraints)
        {
        if (use_dialog)
            wi_set_default_sel (*id, DB_STATUS_LIST, 6);
        dba_update_constraints (db_account);
        }
    if (create_flags->grant_privileges)
        {
        if (use_dialog)
            wi_set_default_sel (*id, DB_STATUS_LIST, 7);
        dba_grant_public_privileges (db_account);
        }
    if (do_any)
        {
        am_message (AM_STATUS, mg_message (DB_ORACLE_UPDATE_COMPLETE));
        am_message (AM_DIALOG, NULL);
        }
    status = SUCCESS;
    return status;
    }
/* END:     */
