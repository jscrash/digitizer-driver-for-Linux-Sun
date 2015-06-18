/* DEC/CMS REPLACEMENT HISTORY, Element ER_LOG.C*/
/* *2    23-AUG-1990 16:12:07 VINCE "(SPR 1) part of a mass #if HOST to #ifdef conversion"*/
/* *1    19-JUN-1989 12:49:24 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element ER_LOG.C*/

/******************************************************************************

                Copyright Exploration Systems, Inc. 1989
		   Unpublished -- All rights reserved

THIS SOFTWARE IS THE PROPRIETARY PROPERTY OF Exploration Systems, Inc.  AND MAY
CONTAIN CONFIDENTIAL TRADE SECRET INFORMATION. IT IS LICENSED FOR USE ON THE
DESIGNATED EQUIPMENT ON WHICH IT WAS ORIGINALLY INSTALLED AND  MAY NOT BE
MODIFIED, DUPLICATED OR COPIED IN ANY FORM WITHOUT PRIOR WRITTEN CONSENT OF

			Exploration Systems, Inc.
			579 Market Street
			San Francisco, CA  USA 94105
			(415) 974-1600

(The above notice does not constitute an admission of publication or
unrestricted dissemination of the work)

******************************************************************************/

/* ************************************************************************

   NAME:    ER_LOG

   AUTHOR:  W. Jaeck, 26-Aug-86

   DESCRIPTION:

    ER_LOG logs a text message to the Finder error log file. It does so
    by opening the error log file with write and append access. If the
    error log file does not exist, it is created. If the error log file
    cannot be accessed, probably because it is already opened by er_log
    in another Finder session, then some number of retrys are performed.

    A single error log file is used for all Finder error messages,
    regardless of which terminal, user, or session produces the message.
    The name of the error log file is specified in the include file
    esi_er_err.h by the symbol ER_ERROR_LOG_FILENAME.

    There is no restriction on the length of the message which can be
    logged. The message should be null terminated, like a normal character
    string. The logged message should not contain any line feeds.

    The error log file contains the following specific information.

	 - The date and time of the error,
	 - The terminal line number,
	 - The user's login name,
	 - The source of the error,
	 - The error message string, as specified to er_log.

    The source of the error is specified, in the error log file, by
    the application's tag, as specified in the call to the application
    manager service am_declare. Additionally, the application id is
    noted. Finally, an indication is noted to tell whether to caller was
    an application or an application server.

    In the case of an application server, the source may be listed several
    times. The first source is the caller (to er_log). Additional sources
    indicate the caller's callers, back to the point of the originating
    application.

   ************************************************************************ */

#include "esi_c_lib.h"
#ifndef ESI_HO_H
#include "esi_ho.h"
#endif
#ifndef ESI_ER_H
#include "esi_er.h"
#endif
#ifndef ESI_HO_FILES_H
#include "esi_ho_files.h"
#endif

#include "esi_time.h"

/* ********************************************************************** */
publicdef VOID er_log(string)
/* ********************************************************************** */

CHAR *string;                     /* Error message string */
{
    CHAR text[400];                   /* Text for log file */
    FILE *error_file;             /* Pointer to error log file control block */
    INT application_id;           /* Application id */
    CHAR application_type[20];    /* Application or Server */
    INT retry;                    /* Current retry, opening error log file */
    time_t bin_time;                 /* Current time */
    FILENAME error_log_filename;  /* final error log filename */

/* ********************************************************************** */

				  /*   Construct error log full path name. */

    ho_add_path("ESI$ERRORS",
	    ER_ERROR_LOG_FILENAME,
	    sizeof (FILENAME),
	    error_log_filename);

				  /*   Open the error log file for append.
				       Retry if necessary. Pause 1 second
				       between retries. */

    for (retry = 0; retry <= ER_OPEN_RETRYS; retry++) 
	 {
	 error_file = ho_open( error_log_filename, "a", NULL );
	 if (error_file != (FILE *)0)
	      break;
	 else
	      sleep(1);
	 }

				  /*   If it was not possible to open the
				       error log file, alert the user. */

    if (error_file EQUALS (FILE *)0)
	{
	 printf(
"!!! Can't open error log file %s.\n!!! Please notify system administrator.",
		error_log_filename);
	}

				  /*   If it was possible to open the error
				       log file, get environmental information
				       to accompany logged message. */

    else {

				  /*   Get current time and write to error
				       log file. */

	 time(&bin_time);
	 sprintf(text,"Time: %s",ctime(&bin_time));
	 fputs(text,error_file);

				  /*   Log the terminal line number. */

	 sprintf(text,"Line: %s\n",ho_terminal(NULL));
	 fputs(text,error_file);

				  /*   Log the user's login name. */

	 sprintf(text,"User: %s\n",ho_user_name(NULL));
	 fputs(text,error_file);

#if 0
/* disabled 6 apr 88 j gillespie - this is a bad coupling between the
   lowest system logger and the highest system overseer */

				  /*   Log the source of the error. If the
				       application manager has been initialized,
				       then this is a V5 message, and source
				       information can be got from the am
				       data structures. */

	 if (am_static()->initialized) {
	      application_id = am_static()->application_id;
	      while (TRUE) {
		   btfind(&am_static()->application_list,&application_id,&app);
		   if (app.type == AM_SERVER)
			strcpy(application_type,"Server");
		   else
			strcpy(application_type,"Application");
		   sprintf(text,"Source: %s ID=%d (%s)\n",app.tag,application_id,application_type);
		   fputs(text,error_file);

				  /*   If server, note who parent was, up to
				       the application level. */

		   if (app.type == AM_SERVER)
			application_id = app.parent_id;
		   else
			break;
		   }
	      }
#endif
				  /*   Log the specified error message. */

	 sprintf(text,"Error: %s\n",string);
	 fputs(text,error_file);

				  /*   Write breaker. */

	 fputs("******************************\n",error_file);

				  /*   Deaccess error log file. */

	 fclose(error_file);
	 }

				  /*   Allow world access to file. */
#ifdef vms
    chmod(error_log_filename,(unsigned)0777);
#endif
    return;
}

                                                        
