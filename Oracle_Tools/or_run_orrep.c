/* DEC/CMS REPLACEMENT HISTORY, Element OR_RUN_ORREP.C*/
/* *12    4-JAN-1991 10:12:25 MING "(SPR 6396) add file extension .lst to report file"*/
/* *11   31-OCT-1990 11:40:32 GILLESPIE "(SPR 37) Change VMS to vms"*/
/* *10   23-AUG-1990 17:18:45 JESSIE "(SPR 1) fix include files"*/
/* *9    25-JUL-1990 19:57:58 VINCE "(SPR 5644) Header Standardization"*/
/* *8     1-NOV-1989 11:12:37 GILLESPIE "(SPR 30) Add esi_ho_err.h"*/
/* *7    20-SEP-1989 07:51:28 GORDON "(SPR 100) undo last checkin"*/
/* *6    19-SEP-1989 13:38:35 GILLESPIE "(SPR 100) GULF MODS"*/
/* *5    18-SEP-1989 12:43:58 JULIAN "Gulf mods under SPR 100"*/
/* *4     6-SEP-1989 09:09:04 GORDON "(SPR 200) Gulf changes (August trip)"*/
/* *3     8-AUG-1989 13:01:05 VINCE "(SPR -1) changed unix file names to lowercase"*/
/* *2    28-JUL-1989 12:42:00 GILLESPIE "(SPR 1) Fix a minor casting problem"*/
/* *1    19-JUN-1989 13:21:34 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element OR_RUN_ORREP.C*/
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

#ifndef ESI_AM_H

#include "esi_am.h"

#endif

#ifndef ESI_HO_H

#include "esi_ho.h"

#endif

#ifndef ESI_HO_FILES_H

#include "esi_ho_files.h"

#endif

#ifndef ESI_QS_H

#include "esi_qs.h"

#endif

#ifndef ESI_MG_H

#include "esi_mg.h"

#endif

#include "or_reports.h"
#include "esi_ho_err.h"
#include "esi_or_err.h"
#include "esi_or_msg.h"

/* Function Description -----------------------------------------------------
Description:

Prototype:
    publicdef INT or_run_oracle_report(REPORT_STRUCT *pScript);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    pScript         -(REPORT_STRUCT *)

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT or_run_oracle_report (REPORT_STRUCT *pScript)
#else
publicdef INT or_run_oracle_report (pScript)
REPORT_STRUCT *pScript;
#endif
    {
    INT status = SUCCESS;
    INT i;
    INT rlen;
    PROJECT_NAME project;
    ANALYST_NAME account_name;
    ORACLE_NAME account_password;
    ORACLE_NAME database;
    FILENAME script_name;
    FILENAME report_name;
    FILENAME directory;
    FILENAME driver_procedure;
    FILENAME command;
    CHAR args[1024];
    CHAR disposition[128];
    CHAR tag_buf[14];
    CHAR *pdot;	
    
    INT userno;
    /* lowercase the script name for systems that care  */
    ts_sto_lower (pScript->script_name);
    
    /* make sure the requested script exists        */
    strcpy (directory, "ESI$SCRIPTS");
    status = ho_add_path (directory, pScript->script_name, sizeof(FILENAME),
                          script_name);
    
    if (status EQUALS HO_NO_SUCH_PATH)
        {
        sprintf (args, mg_message (OR_BAD_LOGICAL), directory);
        am_message (AM_ALERT, args);
        return FAIL;
        }
    /* add the standard oracle extension 
       so that translate_filename will work */
    if (strcmp (pScript->script_type, "SQLPLUS") EQUALS 0)
        {
        strcat (script_name, ".sql");
        }
    else if (strcmp (pScript->script_type, "RPT") EQUALS 0)
        {
        strcat (script_name, ".rpt");
        }
    status = ho_translate_filename (script_name, script_name);
    if (status IS_NOT_EQUAL_TO SUCCESS)
        {
        sprintf (args, mg_message (OR_BAD_SCRIPT_NAME), pScript->script_name);
        am_message (AM_ALERT, args);
        return FAIL;
        }
    /* make sure the name of a select list is      */
    /* filled in                                   */
    
    if (pScript->control_list_required AND strlen (pScript->select_list_name) EQUALS 0)
        {
        am_message (AM_ALERT, mg_message (OR_NO_SELECT_LISTS_ACTIVE));
        return FAIL;
        }
    /* generate the full path name of the external */
    /* command procedure                           */
    if ((status = ho_add_path ("ESI$BIN", "or_run_report", sizeof(FILENAME),
                               driver_procedure)) != SUCCESS)
        {
        return FAIL;
        }
    /* make sure the report name is filled in      */
    /* (it is REQUIRED for a batch job)            */
    
    rlen = strlen (pScript->report_name);
    if (pScript->run_interactive)
        {
        /* if this is an interactive job, AND the
        abuser has NOT requested either printing or
        saving, then there is no need to spool      */
        
        if (NOT pScript->print AND NOT pScript->save)
            {
            strcpy (pScript->report_name, "");
            rlen = 0;
            }
        /* If this is an interactive run that the
           individual has professed the desire to run
           interactively and either print or save, and
           yet they have not provided even the
           slightest clue as to what file name this
           should be saved as.  We must therefore do
           the work for this person by inventing an
           output report name.  When ho_phase_of_moon
           becomes available, then this should be
           prepended to the name of the file       */
        
        else if (rlen EQUALS 0)
            {
            sprintf (pScript->report_name, "esi_report_%s", ho_date_tag (tag_buf));
            }
        }
    else
        {
        /* here we have the case of the abuser asking
           a batch job to be run, but no file name is
           supplied to which we are directed to direct
           our indirect output. Insofas as is directly
           possible, we should direct the user to
           remedy this directly.  Lacking the
           direction to do same, we take a more
           indirect approach of conjuring up a
           mysterious and purposely obscure name with
           which to burden the user.  Since this
           section was so hard to program, they should
           bear some of the burden             */
        
        if (rlen EQUALS 0)
            {
            sprintf (pScript->report_name, "esi_report_%s", ho_date_tag (tag_buf));
            }
        /* The case of a batch job which is neither to
        be printed nor saved is effectively a
        non-operation.  Change this to be save only
        under the default file name ESI_REPORT      */
        
        if (NOT pScript->print AND NOT pScript->save)
            {
            pScript->save = TRUE;
            }
        }
    /* add the directory path name for the file    */
    if (strlen (pScript->report_name) > 0)
        {
        /* ------------------------------------------ */		
        /* report file always has the extension .lst  */
        /* if there is no user defined file extension */
        /* ------------------------------------------ */				
        pdot = strrchr (pScript->report_name,'.');
        if (pdot == NULL)
            {
            strcat (pScript->report_name,".lst");
            }
			
        /* analyst level logical; where they what all reports to end up */
        strcpy (directory, "ESI$REPORTS");
        status = ho_add_path (directory, pScript->report_name, sizeof report_name,
                              report_name);
        
        if (status EQUALS HO_NO_SUCH_PATH)
            {
            sprintf (args, mg_message (OR_BAD_LOGICAL), directory);
            am_message (AM_STATUS, args);
            am_message (AM_STATUS, mg_message (OR_SETTING_LOCAL_DIR));
            strcpy (report_name, pScript->report_name);
            }
        }
    else                            /* NO report selected - interactive run only... */
        {
        strcpy (report_name, "");
        }
    /* get the variables filled                    */
    
    qs_inq_c (QS_PROJECT_NAME, project, (INT *)0);
    qs_inq_c (QS_FINDER_ACCOUNT, account_name, (INT *)0);
    qs_inq_c (QS_FINDER_PASSWORD, account_password, (INT *)0);
    qs_inq_c (QS_DATABASE, database + 1, &i);
    /* if database is filled in, set 1st char to @ */
    database[0] = i ? '@' : 0;
    
    /* set up the report disposition */

#ifdef vms
    if (!pScript->print)
        strcpy (disposition, "");
    else
        {
        strcpy (disposition, "PRINT");
        if (!pScript->save)
            strcat (disposition, "/DELETE");
        }
    if (pScript->run_interactive)
        {
        sprintf (args, "\"%s/%s%s\" \"%s\" \"%s\" \"%s\" \"%s\" \"%s\" \"%s\" \"%s\"",
                 account_name, account_password, database, project, report_name,
                 script_name, pScript->select_list_name, pScript->data_type,
                 pScript->script_type, disposition);
        
        status = ho_spawn (SPAWN_PHANTOM, driver_procedure, args, (INT *)0);
        if (status != SUCCESS)
            {
            am_message (AM_ALERT, mg_message (OR_BAD_SUBMIT));
            }
        }
    else
        {
        sprintf (args, "\"%s/%s%s\",\"%s\",\"%s\",\"%s\",\"%s\",\"%s\",\"%s\",\"%s\"",
                 account_name, account_password, database, project, report_name,
                 script_name, pScript->select_list_name, pScript->data_type,
                 pScript->script_type, disposition);
        
        /* submit the job to the batch queue */
        status = ho_spawn (SPAWN_BATCH, driver_procedure, args, (INT *)0);
        if (status != SUCCESS)
            {
            am_message (AM_ALERT, mg_message (OR_BAD_SUBMIT));
            }
        }
    return status;
#endif

#ifdef PRIMOS
    /* the report name must be filled in and unique */
    if (strlen (report_name) == 0)
        {
        sprintf (report_name, "ESI_REPORT_%s", ho_date_tag (tag_buf));
        }
    if (!pScript->print)
        strcpy (disposition, "");
    else
        {
        strcpy (disposition, "-PRINT");
        if (!pScript->save)
            strcat (disposition, " -DELETE");
        }
    sprintf (args, "'%s/%s%s' %s %s %s %s %s %s %s", account_name, account_password,
             database, project, report_name, script_name, pScript->select_list_name,
             pScript->data_type, pScript->script_type, disposition);
    
    if (pScript->run_interactive)
        {
        status = ho_spawn (SPAWN_PHANTOM, driver_procedure, args, &userno);
        if (status != SUCCESS)
            am_message (AM_ALERT, mg_message (OR_BAD_SUBMIT));
        }
    else
        {
        /* submit the job to the batch queue */
        status = ho_spawn (SPAWN_BATCH, driver_procedure, args, &userno);
        if (status != SUCCESS)
            am_message (AM_ALERT, mg_message (OR_BAD_SUBMIT));
        }
    return status;
#endif

#if UNIX
    /* the report name must not be blank, use NULL as a filler */
    if (strlen (report_name) == 0)
        {
        strcpy (report_name, "NULL");
        }
    if (!pScript->print)
        {
        strcpy (disposition, "noprint keep");
        }
    else
        {
        strcpy (disposition, "print");
        if (pScript->save)
            strcat (disposition, " keep");
        else
            strcat (disposition, " delete");
        }
    sprintf (args, "'%s/%s%s' %s %s %s '%s' %s %s %s", account_name, account_password,
             database, project, report_name, script_name, pScript->select_list_name,
             pScript->data_type, pScript->script_type, disposition);
    
    /* execute the script in the C shell */
    sprintf (command, "csh %s", driver_procedure);
    
    if (pScript->run_interactive)
        {
        status = ho_spawn (SPAWN_PHANTOM, command, args, &userno);
        if (status != SUCCESS)
            am_message (AM_ALERT, mg_message (OR_BAD_SUBMIT));
        }
    else
        {
        /* submit the job to the batch queue */
        status = ho_spawn (SPAWN_BATCH, command, args, &userno);
        if (status != SUCCESS)
            am_message (AM_ALERT, mg_message (OR_BAD_SUBMIT));
        }
    return status;
#endif
    }
/* END:     */
