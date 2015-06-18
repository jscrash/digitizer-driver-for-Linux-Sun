/* DEC/CMS REPLACEMENT HISTORY, Element SE_LINE_INTER.C*/
/* *7    14-AUG-1991 12:01:02 KEE "(SPR -1) Put in File Lock/Unlock logic"*/
/* *6     6-DEC-1990 10:27:41 JULIAN "(SPR 6216) add double quote for command line arguments"*/
/* *5    27-NOV-1990 17:10:10 JULIAN "(SPR 6216) add case for UNIX"*/
/* *4    17-AUG-1990 22:17:14 VINCE "(SPR 5644) Code Cleanup"*/
/* *3    25-JUL-1989 20:03:04 GILLESPIE "(SPR 50) Seismic system revamp"*/
/* *2    26-JUN-1989 11:02:32 GILLESPIE "(SPR 20) Fix castings"*/
/* *1    19-JUN-1989 13:24:39 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element SE_LINE_INTER.C*/
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

#ifndef ESI_QS_H

#include "esi_qs.h"

#endif

#ifndef ESI_TC_H

#include "esi_tc.h"

#endif

#ifndef ESI_TS_H

#include "esi_ts.h"

#endif

#ifndef ESI_WI_H

#include "esi_wi.h"

#endif

#ifndef ESI_HO_H

#include "esi_ho.h"

#endif

#ifndef ESI_HO_FILES_H

#include "esi_ho_files.h"

#endif

#ifndef ESI_MG_H

#include "esi_mg.h"

#endif

/* Specific include files */
#ifndef INTER_WORK_H

#include "inter_work.h"

#endif

#include "intersect_dialog.rh"

/*
   **                            FUNCTION DECLARATIONS
*/
INT inter_main ();
INT inter_main_server ();
static INT inter_main_batch_job ();

/* File Description ---------------------------------------------------------
Overview:
    To compute line intersections digitized with Finder.

Prototypes and one line descriptions of each function in the file.
Public_Functions:
    publicdef INT inter_main_server(INT id, INT item, INTER_WORKSPACE     *pinter);
    
    publicdef INT inter_main (BYTE *pointer);

Private_Functions:
    static INT inter_main_batch_job (INTER_WORKSPACE     *pinter);

-----------------------------------------------------------------------------*/

/* Function Description -----------------------------------------------------
Description:
    Initialize application. The line intersection
    module accesses the necessary
    resource files and loads them into the MAC terminal.
    The application manager is given the names of functions
    for handling resource events.

Prototype:
    publicdef INT inter_main (BYTE *pointer);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    pointer         -(BYTE *) Required Application Manager BYTE pointer.
                        It is not utilized in this application.

Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT inter_main (BYTE *pointer)
#else
publicdef INT inter_main (pointer)
BYTE *pointer;
#endif
    {
    INTER_WORKSPACE *pinter;
    
    am_declare ("INTERSECTIONS");
    
    am_define_workspace ("INTERSECTIONS", AM_APPLICATION, sizeof(INTER_WORKSPACE),
                         (VOIDPTR *) & pinter);
    
    am_define_resource (AM_DIALOG, "INTERSECT_DIALOG", COMPUTE_INTERS,
                        inter_main_server, &pinter->dialog_id, pinter);
    
    
    /* Default to the ALL option on startup */
    wiscdv (pinter->dialog_id, CDLG_155_ALL, ON);
    wiscdv (pinter->dialog_id, CDLG_155_LIST, OFF);
    wiopcd (pinter->dialog_id);
    
    return SUCCESS;
    
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    Main custom dialog server.

Prototype:
    publicdef INT inter_main_server(INT id, INT item, INTER_WORKSPACE     *pinter);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    id              -(INT) Dialog id for the UKOOA application driver's
                        custom dialog, assigned by the am_define_resource service.
    item            -(INT) Resource item to process.
    pointer         -(WORKSPACE *) Pointer to the application workspace,
                        which was defined in the calling routine.

Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PUBLIC

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT inter_main_server (INT id, INT item, INTER_WORKSPACE *pinter)
#else
publicdef INT inter_main_server (id, item, pinter)
INT id;
INT item;
INTER_WORKSPACE *pinter;
#endif
    {
    
    INT option, len;
    INT status;
    CHAR text[STMAX];
    
    switch (item)
        {
        
    case CDLG_155_OK:
        
        status = wiqccdi (pinter->dialog_id, CDLG_155_ALL, &option, text);
        if (option == ON)
            {                       /* process ALL option */
            pinter->Flag_all = 1;
            }
        else if (option == OFF)
            {                       /*  Get current seismic line select list  */
            
            pinter->Flag_all = 0;
            qs_inq_c (QS_CUR_SEISMIC_LIST, pinter->curr_seismic_list, &len);
            
            if (len <= 0)
                {
                
                status = wi_alert (0, " ", "There is no current seismic line select \
list."
                                   , 
"Please create one or click on the \n All Seismic Lines radiobutton in \n \
the Line Intersection Control dialog  box."
                        );
                break;
                
                }
            }
        inter_main_batch_job (pinter);
        am_quit ();
        break;
        
    case CDLG_155_CANCEL:
        am_quit ();
        break;
        
        }
    return SUCCESS;
    
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    Batch job generator. The files necessary for the batch processing
    of the UKOOA data are created. These include:
    
        * a COMINPUT file, and
        * a text file for storing paramters.

    Unique names will be generated for these files and for files
    created during batch processing. These additional files include:

        * a FORTRAN direct access parameters file, and
        * a listing file.

Prototype:
    static INT inter_main_batch_job (INTER_WORKSPACE     *pinter);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    pinter          -(INTER_WORKSPACE *) Pointer to the application workspace.

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PRIVATE to <se_line_inter.c>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static INT inter_main_batch_job (INTER_WORKSPACE *pinter)
#else
static INT inter_main_batch_job (pinter)
INTER_WORKSPACE *pinter;
#endif
    {
    INT code, job_user_no;
    CHAR login_oracle[STMAX], message[STMAX];
    CHAR command_line[255];
    FILENAME batch_loader;
    FILE *comi_fp;
    FILENAME comi_file;
    INT len, status;
    ANALYST_NAME account;
    ORACLE_NAME password;
    ORACLE_NAME database;
    DATE_TIME date_tag;
    
    /*  build the login string = account - password - database  */
    
    qs_inq_c (QS_PROJECT_NAME, pinter->Area, (INT *)0);
    qs_inq_c (QS_FINDER_ACCOUNT, pinter->User, (INT *)0);
    qs_inq_c (QS_FINDER_PASSWORD, password, &len);
    qs_inq_c (QS_DATABASE, database, &len);
    
    if (len > 0)
        {
        sprintf (login_oracle, "%s/%s@%s", pinter->User, password, database);
        }
    else
        {
        sprintf (login_oracle, "%s/%s", pinter->User, password);
        }
    /* Generate file names */
    
    /* get date */
    ho_date_tag (date_tag);

    /*   name of the batch loader   */
    
    ho_add_path ("ESI$BIN", "inters_main", sizeof(batch_loader), batch_loader);
    
#ifdef vms
    /*  name of the DCL file which is submitted to run the loader  */
    
    sprintf (comi_file, "CAL_INTERS_%s.COM", pinter->User);
    
#endif

#if UNIX

    /*    name of the script file which is submitted to run the loader  */
    sprintf (comi_file, "cal_inters_%s", date_tag);

#endif

    /* Create  script file */
    
    comi_fp = ho_open (comi_file, "w", NULL);
    status = ho_lock (comi_fp, ON);

    if (comi_fp EQUALS (FILE *)NULL || status != SUCCESS)
        {
        sprintf (message, "Cominput file cannot be opened!\n");
        am_message (AM_ALERT, message);
        
        return FAIL;
        }
    /* Write commands to cominput file */

#ifdef vms
    fprintf (comi_fp, "$FINT :== $%s\n", batch_loader);
    if (pinter->Flag_all == 1)
        {
        sprintf (command_line, "$FINT \"%s\" \"%s\" ALL", login_oracle, pinter->Area);
        }
    else
        {
        sprintf (command_line, "$FINT \"%s\" \"%s\" \"%s\"", login_oracle, pinter->Area,
                 pinter->curr_seismic_list);
        }
    fprintf (comi_fp, "%s\n", command_line);
    fprintf (comi_fp, "$EXIT \n");
#endif

#if UNIX
    fprintf (comi_fp, "# Temporary script file created by Finder.\n");
    fprintf (comi_fp, "#\n# THIS FILE MAY BE DELETED AT ANY TIME.\n");
    if (pinter->Flag_all == 1)
        {
        sprintf (command_line, "%s \"%s\" \"%s\" ALL", batch_loader, login_oracle, 
		 pinter->Area);
        }
    else
        {
        sprintf (command_line, "%s \"%s\" \"%s\" \"%s\"", batch_loader, login_oracle, 
		 pinter->Area, pinter->curr_seismic_list);
        }
    fprintf (comi_fp, "%s\n", command_line);

#endif
    
    status = ho_lock (comi_fp, OFF);
    fclose (comi_fp);
    
    sprintf (message, " batch job %s ready to submit", comi_file);
    status = wi_alert (0, " ", message, " ");
    
    code = ho_spawn (SPAWN_BATCH, comi_file, "DELETE", &job_user_no);
    
    if (code)
        {
        sprintf (message, "%s\n%s%s%s; in subroutine %s.", 
"System error occurred while attempting to submit \
a line intersection calculation job."
                , "Error: ", mg_message (code), comi_file, "inter_main_batch_job");
        }
    else
        {
        sprintf (message, "%s%s%d.", "Line intersections calculation ",
                 "is being done by job # ", job_user_no);
        am_message (AM_STATUS, message);
        }
    return SUCCESS;
    }
/* END:     */
