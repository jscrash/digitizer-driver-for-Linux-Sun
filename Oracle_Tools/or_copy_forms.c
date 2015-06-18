/* DEC/CMS REPLACEMENT HISTORY, Element OR_COPY_FORMS.C */
/*  17   21-NOV-1991 10:56:11 LOEBL "(SPR 0) Convert to Sqlforms30" */
/* *16   19-SEP-1991 15:42:43 LOEBL "(SPR 4126) Include sqlforms version number" */
/* *15   14-AUG-1991 12:00:46 KEE "(SPR -1) Put in File Lock/Unlock logic" */
/* *14   22-MAY-1991 15:09:07 LOEBL "(SPR 7060) Warning message if file cannot be chmod'ed" */
/* *13    8-JAN-1991 16:25:47 GILLESPIE "(SPR 6347) Remove or_update_form_pathnames - should use ADDPATH user exit instead" */
/* *12    6-JAN-1991 14:59:47 GILLESPIE "(SPR 6347) Expand path names in ORACLE_APPLICATIONS when forms compiled" */
/* *11    7-DEC-1990 14:51:02 VINCE "(SPR 1) Cleaned up some error messages." */
/* *10    9-NOV-1990 10:33:51 MING "(SPR -1) replace ho_find_files and ho_find_files2 to ho_find_pathnames and ho_find_filenames" */
/* *9    23-AUG-1990 16:15:33 VINCE "(SPR 1) part of a mass #if HOST to #ifdef conversion" */
/* *8    25-JUL-1990 19:56:51 VINCE "(SPR 5644) Header Standardization" */
/* *7    23-JUL-1990 13:54:43 VINCE "(SPR 1) Added flag in the Unix shell script to force execution in the C-Shell" */
/* *6    23-JUL-1990 11:50:33 VINCE "(SPR 1) final fixes for recursive logical names on Unix" */
/* *5    19-JUN-1990 16:44:50 MING "(SPR 0) change ho_find_files2 to ho_find_files" */
/* *4    18-JUN-1990 11:24:40 VINCE "(SPR 1) changed method from IAP tables to INP file copy." */
/* *3    25-MAY-1990 12:48:35 VINCE "(SPR 1) updated UNIX side of code to match VMS" */
/* *2    22-MAY-1990 21:27:30 VINCE "(SPR 1) replaced old method using IAP tables with INP met{od" */
/* *1    22-MAY-1990 18:00:11 VINCE "replacement for or_copy_forms.pc" */
/* DEC/CMS REPLACEMENT HISTORY, Element OR_COPY_FORMS.C */
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
#include "esi_ho.h"
#include "esi_mg.h"
#include "esi_ts.h"
#include "esi_ho_files.h"
#include "esi_or_msg.h"

/* Function Description -----------------------------------------------------
Description:

Prototype:
    publicdef INT or_copy_forms (DB_ACCOUNT_STRUCT *db_account);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    db_account      -(DB_ACCOUNT_STRUCT *)

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT or_copy_forms (DB_ACCOUNT_STRUCT *db_account)
#else
publicdef INT or_copy_forms (db_account)
DB_ACCOUNT_STRUCT *db_account;
#endif
    {
    INT ii, status, nforms;
    TCP file_list, dir_list, pfile;
    CHAR *cp;
    FILENAME project_directory, inp_name, proj_form_path, command_file;
    FILENAME search_path;
    PATHNAME base_directory;
    PROJECT_NAME defproj, project;
    CHAR sep, format[128];
    CHAR line[200];
    BOOL system_forms;
    FILE *in, *out, *com;
    
    strcpy (defproj, "DEFAULT_PROJECT");
    strcpy (project, db_account->name);
    strcpy (command_file, "or_copy_forms"); /* lowercase to be UNIX friendly */
    
    /* check for one of the "special" project names */
    system_forms = FALSE;
    if (ts_strcmpi (project, "ESI") == 0)
        system_forms = TRUE;
    
    if ((ts_strcmpi (project, defproj) == 0) || (ts_strcmpi (project, "CODES") == 0))
        {
        printf ("Finder reserved project name '%s'.\n", project);
        printf ("No forms will be generated.\n");
        return SUCCESS;
        }
    if (system_forms)
        printf ("Starting processing of SYSTEM level forms.\n");
    else
        printf ("Starting processing of PROJECT level forms for %s.\n", project);

    sep = ENTRYSEPERATOR;
#ifdef primos
    strcat (command_file, ".CPL");
#endif

#if UNIX
    /* (no command file extension) */
#endif
    
#ifdef vms
    strcat (command_file, ".COM");
#endif

    /* get all the INP filenames to be copied to the new project.       */
    /* the logical name used below contains a directory path with a     */
    /* wildcard filename i.e. (vms) "ESI$ROOT:[FORMS.PROJECT]*.INP"     */
    
    if (system_forms)
        {
        status = ho_translate_symbol ("ESI$SYSTEM_FORM_TEMPLATES", &dir_list);
        if (status != SUCCESS)
            {
            printf ("The system logical ESI$SYSTEM_FORM_TEMPLATES is not defined.\n");
            return FAIL;
            }
        }
    else
        {
        status = ho_translate_symbol ("ESI$PROJECT_FORM_TEMPLATES", &dir_list);
        if (status != SUCCESS)
            {
            printf ("The system logical ESI$PROJECT_FORM_TEMPLATES is not defined.\n");
            return FAIL;
            }
        }
    strcpy (search_path, dir_list[0]);
    ts_tcp_free (dir_list);
    
    ho_find_pathnames (search_path, &file_list);
    nforms = ts_tcp_len (file_list);
    if (nforms == 0)
        {
        printf ("WARNING: No forms found to process using '%s'.\n", search_path);
        return SUCCESS;
        }
    /* get a scratch pathname for the command file. */
    ho_add_path ("ESI$SCRATCH", command_file, sizeof command_file, command_file);
    com = ho_open (command_file, "w", NULL);
    status = ho_lock (com, ON);
    if (com == NULL_FILE_PTR || status != SUCCESS)
        {
        printf ("Unable to create the command file '%s'.", command_file);
        return FAIL;
        }
    /*  get the pathname for the INP files. */
    if (system_forms)
        {
	strcpy(base_directory, "ESI$SYSTEM_BASE_FORMS");
        }
    else
        {
	strcpy(base_directory, "ESI$PROJECT_BASE_FORMS");
        }
    status = ho_translate_symbol (base_directory, &dir_list);
    if (status != SUCCESS)
	{
	printf ("The logical name ESI$SYSTEM_BASE_FORMS is not defined.\n");
	return status;
	}
    strcpy (project_directory, dir_list[0]);
    ts_tcp_free (dir_list);
    
    /* copy the forms from the delivery directory to the project directory  */
    /* convert DEFAULT_PROJECT to the new project name during the copy      */
    
    for (pfile = file_list, ii = 1; *pfile != NULL; pfile++, ii++)
        {
        cp = strrchr (*pfile, ';');
        if (cp != NULL)
            *cp = NUL;
        
        if ((in = fopen (*pfile, "r")) == NULL_FILE_PTR)
            {
            printf ("Can't open %s for reading.\n", *pfile);
            continue;
            }
        /* --------------------------------------------------- */
        /* set the output filename using the last component of */
        /* the input path.                                     */
        /* --------------------------------------------------- */
        cp = strrchr (*pfile, sep);
        if (cp)
            cp++;
        else
            cp = *pfile;
        
        strcpy (inp_name, cp);
        
        /* use lower case to be UNIX friendly */
        ho_assemble_pathname (proj_form_path, project_directory,
                              ts_sto_lower (inp_name));
    
	out = fopen (proj_form_path, "w");
	status = ho_lock (out, ON);
        if (out == NULL_FILE_PTR || status != SUCCESS)
            {
            printf ("Can't open %s for writing.\n", proj_form_path);
            continue;
            }
        printf ("processing (%2d of %2d) %s\n", ii, nforms, inp_name);
        
        while (fgets (line, sizeof line, in) != NULL)
            fputs (ts_subs (line, defproj, project, TRUE), out);
        
        fclose (in);
	status = ho_lock (out, OFF);
        fclose (out);
        }
    /* create the command procedure to compile the forms */

#ifdef vms
    fprintf (com, "$ SET DEFAULT %s\n", project_directory);
    fprintf (com, "$ SET VERIFY\n");
    for (pfile = file_list; *pfile != NULL; pfile++)
        {
        strcpy (inp_name, strrchr (*pfile, sep) + 1);
        cp = strrchr (inp_name, '.');
        if (cp != NULL)
            *cp = NUL;
#if SQLFORMS30
        fprintf (com, "$ IAG30 -TON %s\n", inp_name);
#else
        fprintf (com, "$ IAG23 -TO %s\n", inp_name);
#endif
        }
    fprintf (com, "$ SET NOVERIFY\n");
    fprintf (com, "$ PURGE/NOLOG\n");
    sprintf (format, "$ WRITE SYS$OUTPUT \"%s\"\n",
             mg_message (OR_FORM_GEN_COMPLETED));
    fprintf (com, format, db_account->name);
#endif

#ifdef primos
    fprintf (com, "ATTACH %s\n", project_directory);
    fprintf (com, "COMO WATCH_FORM_GENERATION.COMO\n");
    fprintf (com, "SSR FINDER*>ENTRY$\n");
    fprintf (com, "DEFINE_GVAR FINDER*>FINDER_ORACLE.GVAR\n");
    fprintf (com, "&DEBUG &ECHO COMMAND\n");
    for (pfile = file_list; *pfile; pfile++)
        {
        strcpy (inp_name, strrchr (*pfile, sep) + 1);
        cp = strrchr (inp_name, '.');
        if (cp != NULL)
            *cp = NUL;
#if SQLFORMS30
        fprintf (com, "$ IAG30 -TON %s\n", inp_name);
#else
        fprintf (com, "$ IAG23 -TO %s\n", inp_name);
#endif
        }
    fprintf (com, "&DEBUG &NOECHO COMMAND\n");
    sprintf (format, "TYPE %s\n", mg_message (OR_FORM_GEN_COMPLETED));
    fprintf (com, format, db_account->name);
#endif

#if UNIX
    fprintf (com, "cd %s\n", project_directory);
    fprintf (com, "set ECHO\n");
    for (pfile = file_list; *pfile != NULL; pfile++)
        {
        strcpy (inp_name, strrchr (*pfile, sep) + 1);
        cp = strrchr (inp_name, '.');
        if (cp != NULL)
            *cp = NUL;
#if SQLFORMS30
        fprintf (com, "$ IAG30 -TON %s\n", ts_sto_lower (inp_name) );
#else
        fprintf (com, "$ IAG23 -TO %s\n", ts_sto_lower (inp_name) );
#endif
        }
    fprintf (com, "unset ECHO\n");
    sprintf (format, "echo %s\n", mg_message (OR_FORM_GEN_COMPLETED));
    fprintf (com, format, db_account->name);
#endif
    
    status = ho_lock(com, OFF);
    fclose (com);

#if UNIX
    chmod (command_file, (int)0x777);  /* make it executable */
    if (status != SUCCESS)
	{
        printf ("\nThe file: %s could not be marked for WORLD deletion.\n",
                  command_file);
        printf ("It should be manually deleted after this program completes.\n"
               );
	}
#endif
    printf ("\n%s\n", mg_message (OR_START_FORM_GEN));
    
    status = ho_spawn (SPAWN_PHANTOM, command_file, NULL, (INT *)0);
    
    if (status == SUCCESS)
	{
        printf ("\n%d forms processed.\n", nforms);
	}
    else
        printf ("\n%s\n", mg_message (status));
    
    ts_tcp_free (file_list);
    return SUCCESS;
    }
/* END:     */
