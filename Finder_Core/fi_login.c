/* DEC/CMS REPLACEMENT HISTORY, Element FI_LOGIN.C */
/* *5    14-AUG-1990 13:23:37 VINCE "(SPR 5644) Header Standardization" */
/* *4     2-JUN-1990 15:47:10 GILLESPIE "(SPR 1) Make last_project a static, dummy" */
/* *3    22-MAY-1990 09:01:36 GILLESPIE "(SPR 1) Return error if project change attempted" */
/* *2    22-FEB-1990 09:56:16 GILLESPIE "(SPR 1) Initial release for IES" */
/* *1    22-FEB-1990 09:55:31 GILLESPIE "External finder login assiatance procedure" */
/* DEC/CMS REPLACEMENT HISTORY, Element FI_LOGIN.C */
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

#include "esi_fi.h"

#ifndef ESI_AM_H

#include "esi_am.h"

#endif

#ifndef ESI_OR_H

#include "esi_or.h"

#endif

#ifndef ESI_QS_H

#include "esi_qs.h"

#endif

#ifndef ESI_TS_H

#include "esi_ts.h"

#endif
static PROJECT_NAME last_project;

#include "esi_fi_err.h"

/* Function Description -----------------------------------------------------
    Log into Finder and connect to a project.             
    This is to be used by stand-alone programs that need a simple way to
    initialize the Finder environment.  To connect to a remote database, the
    user/programmer must build the analyst name in the form name@database,
    where name is the analyst name and database is the connection string to the
    remote database.  The project must already exist.                             

Prototype:
    publicdef fi_login(CHAR analyst_name[64], ORACLE_NAME password,
            PROJECT_NAME project, INT *pstatus);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) analyst_name    -(CHAR [])
    (I) password        -(CHAR [])
    (I) project         -(CHAR [])
    (O) pstatus         -(LONG INT *)

Return Value/Status:
    Function returns same as *pstatus.

    Zero is considered success.  Any negative number is an error.
    Call mg_message(status) to get the translation of the status code
    returned to your program.                                         

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef fi_login (CHAR analyst_name[64], ORACLE_NAME password, PROJECT_NAME project,
                    INT *pstatus) 
#else
publicdef fi_login (analyst_name, password, project, pstatus)
CHAR analyst_name[64];
ORACLE_NAME password;
PROJECT_NAME project;
INT *pstatus;
#endif
    {
    PROGNAME ("fi_login");
    int argc = 2;
    char *argv[2];
    CHAR namepass[128];
    PROJECT_NAME local_copy;
    
    strcpy (local_copy, project);
    ts_sto_upper (local_copy);
    
    if (IS_STRING (last_project) AND ARE_DIFFERENT (local_copy, last_project))
        {
        return *pstatus = FI_CANNOT_CHANGE_PROJ;    /* NOT allowed to change projects */
        }
    sprintf (namepass, "%s/%s", analyst_name, password);
    argv[0] = progname;
    argv[1] = namepass;
    
    *pstatus = or_login (argc, argv);
    if (*pstatus EQUALS SUCCESS)
        {
        am_initialize ();
        qs_set_c (QS_PROJECT_NAME, project, 0);
        *pstatus = finder_config_project (TRUE);
        if (*pstatus EQUALS SUCCESS)
            {
            strcpy (last_project, local_copy);
            }
        }
    return *pstatus;
    }
/* END:     */
