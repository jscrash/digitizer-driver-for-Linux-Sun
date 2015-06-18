/* DEC/CMS REPLACEMENT HISTORY, Element LD_UKOOA_MAIN.C*/
/* *14   14-AUG-1991 11:59:57 KEE "(SPR -1) Put in File Lock/Unlock logic"*/
/* *13   17-DEC-1990 16:50:05 JESSIE "(SPR 6273) set the analyst logical name to ESI$JOURNALS"*/
/* *12   27-AUG-1990 08:41:36 VINCE "(SPR 1) change old #if host to ifdef... "*/
/* *11   14-AUG-1990 13:33:19 VINCE "(SPR 5644) Header Standardization"*/
/* *10   20-MAR-1990 16:26:22 KEE "(SPR -1) Get Rid of AM_GET_SERVER call"*/
/* *9    15-MAR-1990 13:39:37 KEE "(SPR -1) Change idverify->text stucture to be dynamic alloc"*/
/* *8     6-FEB-1990 11:21:42 MING "(SPR 0) change date/time declaration to new data type DATE_TIME"*/
/* *7     5-SEP-1989 16:13:35 GORDON "(SPR 200) Gulf changes (August trip)"*/
/* *6    10-AUG-1989 11:25:51 ADEY "(SPR -1) INCLUDE A DEFAULT SURVEY NAME OF 'UNKNOWN'"*/
/* *5     9-AUG-1989 16:33:41 ADEY "(SPR -1) -1"*/
/* *4     4-AUG-1989 11:06:47 ADEY "(SPR -1) CHANGE NAMES"*/
/* *3    25-JUL-1989 16:46:24 ADEY "(SPR -1) Improve User Interface & Decode lat/long and survey?line for seismic"*/
/* *2    26-JUN-1989 10:48:08 GILLESPIE "(SPR 20) Fix castings"*/
/* *1    19-JUN-1989 13:03:30 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element LD_UKOOA_MAIN.C*/
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

#ifndef ESI_C_LIB_H

#include "esi_c_lib.h"

#endif

#ifndef ESI_LL_H

#include "esi_ll.h"

#endif

#ifndef UK_WORK_H

#include "uk_work.h"

#endif

#ifndef ESI_AM_H

#include "esi_am.h"

#endif

#ifndef ESI_MG_H

#include "esi_mg.h"

#endif

#ifndef ESI_CT_H

#include "esi_ct.h"

#endif

#ifndef ESI_HO_H

#include "esi_ho.h"

#endif

#ifndef ESI_HO_FILES_H

#include "esi_ho_files.h"

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

#ifndef ESI_QS_H

#include "esi_qs.h"

#endif

#ifndef ESI_LD_MSG_H

#include "esi_ld_msg.h"

#endif

#ifndef ESI_LD_ERR_H

#include "esi_ld_err.h"

#endif

#include "ld_uk_main.rh"
#include "ld_uk_fields.rh"
#include "ld_uk_lines.rh"
#include "ld_uk_header.rh"

/* FUNCTION DECLARATIONS */

publicdef INT ukooa_main_server ();
static INT ukooa_main_open_flat_file ();
static INT ukooa_main_batch_job ();
static INT ukooa_main_write_parameters ();
static INT uk_fire_batch ();

/* 11/23/88 GS -added these publicdefs */

publicdef CHAR *Header_Parameters[] = 
    {
    "Survey",
    "Projection",
    "Crew Number",
    "Client",
    "Contractor",
    "Month",
    "Year",
    "Source",
    "Positioning",
    "Offset",
    "Spheroid",
    "Meridien",
    "Parallel",
    NULL
    };



publicdef CHAR *Fields_Parameters[] = 
    {
    "Line ID",
    "Shotpoint #",
    "Easting",
    "Northing",
    "Latitude",
    "Longitude",
    "Water Depth",
    NULL
    };



/* File Description ---------------------------------------------------------
Overview:
   Ukooa main server.
    START-HISTORY :
    12/16/86 KIMMAN   creating Com Input file and a Com Output file
    12/16/86 KIMMAN   Print seismic lines only if Selection == TRUE (only following lines)
    12/16/86 KIMMAN   RecLen set to max record length of first 20 records
    12/12/86 KIMMAN   first time checkin
    12/12/86 GEORGE DOBEK/KIMMAN  Template constructed
    END-HISTORY 

Prototypes and one line descriptions of each function in the file.
Public_Functions:
    publicdef INT ukooa_main_server(INT id, INT item, UKOOA_WORKSPACE *pukooa);
    
Private_Functions:
    static INT ukooa_main_open_flat_file(UKOOA_WORKSPACE *pukooa);
    
    static INT ukooa_main_batch_job (UKOOA_WORKSPACE *pukooa);
    
    static INT  ukooa_main_write_parameters(UKOOA_WORKSPACE *pukooa, 
                FILE *comi_fp, FILENAME DAM_file);
                
    static INT  uk_fire_batch (FILENAME comi_file);
-----------------------------------------------------------------------------*/

/* Function Description -----------------------------------------------------
Description:
    Main custom dialog server.

Prototype:
    publicdef INT ukooa_main_server(INT id, INT item, UKOOA_WORKSPACE *pukooa);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) id          -(INT) Dialog id for the UKOOA application driver's
                        custom dialog, assigned by the am_define_resource service.
    (I) item        -(INT) Resource item to process.
    (M) pointer     -(WORKSPACE *) Pointer to the application workspace,
                        which was defined in the calling routine.

Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT ukooa_main_server (INT id, INT item, UKOOA_WORKSPACE *pukooa) 
#else
publicdef INT ukooa_main_server (id, item, pukooa)
INT id;
INT item;
UKOOA_WORKSPACE *pukooa;
#endif
    {
    INT junk;
    CHAR text[STMAX], message[80];
    PROJECTION_ID projection;
    CHAR def_survey[20];
    
    PROGNAME ("ukooa_main_server");
    
    strcpy (def_survey, "UNKNOWN");
    def_survey[7] = '\0';
    
    switch (item)
        {
    case UKOOA_MAIN_Batch_Btn:
        
        /*    put data file name in pukooa  before submitting the batch job  */
        wiqccdi (pukooa->main_dialog_id, UKOOA_MAIN_File_Edit, &junk, text);
        strcpy (pukooa->file.file_name, text);
        
        ukooa_main_batch_job (pukooa);
        break;
        
    case UKOOA_MAIN_Header_Btn:
        wiqccdi (pukooa->main_dialog_id, UKOOA_MAIN_File_Edit, &junk, text);
        if (strcmp (pukooa->file.file_name, text) != 0)
            {
            strcpy (pukooa->file.file_name, text);
            if (ukooa_main_open_flat_file (pukooa) == FAIL)
                break;
            }
        wiscdt (pukooa->header.dialog_id, UKOOA_HEADER_txt, 
"0         1         2         3         4         5         6         7        8\n\
12345678901234567890123456789012345678901234567890123456789012345678901234567890"
                );
        wiscdt (pukooa->header.dialog_id, UKOOA_HEADER_Records, pukooa->Buffer20);
        wiscdr (pukooa->header.dialog_id, UKOOA_HEADER_Records_VScroll, 0, 11);
        strcpy (message, "Select a parameter and highlight text,then select enter.");
        
        wiscdt (pukooa->header.dialog_id, UKOOA_HEADER_Status_Stat, message);
        
        /*        set up the projection to be the project default projection */
        
        if (ct_get_default_project (projection) == SUCCESS)
            {
            pukooa->header.Values[1] = STRING_ALLOCATE (projection);
            strcpy (pukooa->header.Values[1], projection);
            strcpy (pukooa->header.ProjCode, projection);
            }
        /*        set up the default survey name  */
        
        pukooa->header.Values[0] = STRING_ALLOCATE (def_survey);
        strcpy (pukooa->header.Values[0], def_survey);
        strcpy (pukooa->header.Survey, def_survey);
        
        wisselt (pukooa->header.dialog_id, UKOOA_HEADER_Values_Selector,
                 UKOOA_HEADER_Parameters_VScroll, ts_tcp_len (Header_Parameters), 1,
                 pukooa->header.Values);
        
        wiclcd (pukooa->main_dialog_id);
        wiopcd (pukooa->header.dialog_id);
        break;
        
    case UKOOA_MAIN_Fields_Btn:
        strcpy (message, "Select a parameter and highlight text,then select enter.");
        
        wiscdt (pukooa->fields.dialog_id, UKOOA_FIELDS_Status_Stat, message);
        
        wisselt (pukooa->fields.dialog_id, UKOOA_FIELDS_Values_Selector,
                 UKOOA_FIELDS_Parameters_VScroll, NUMBER_OF_DATA_FIELDS, 0,
                 pukooa->fields.Values);
        
        wiclcd (pukooa->main_dialog_id);
        wiopcd (pukooa->fields.dialog_id);
        break;
        
    case UKOOA_MAIN_Lines_Btn:
        wiqccdi (pukooa->main_dialog_id, UKOOA_MAIN_File_Edit, &junk, text);
        if (strcmp (pukooa->lines.file_name, text) != 0)
            {
            strcpy (pukooa->lines.file_name, text);
            if (ukooa_lines_analyze_flat_file (pukooa) != SUCCESS)
                break;
            else
                ukooa_lines_reset_dialog (pukooa);
            }
        else
            {
            if (pukooa->lines.Replacement == FALSE)
                wiscdv (pukooa->lines.dialog_id, UKOOA_LINES_Radio_Honor, ON);
            else
                wiscdv (pukooa->lines.dialog_id, UKOOA_LINES_Radio_Replace, ON);
            }
        wiscdt (pukooa->lines.dialog_id, UKOOA_LINES_LineName_Edit, " ");
        wiscdt (pukooa->lines.dialog_id, UKOOA_LINES_FirstSP_Edit, " ");
        wiscdt (pukooa->lines.dialog_id, UKOOA_LINES_LastSP_Edit, " ");
        wiscdt (pukooa->lines.dialog_id, UKOOA_LINES_Skip_Edit, " ");
        wiscdt (pukooa->lines.dialog_id, UKOOA_LINES_Status_Stat, " ");
        
        wiclcd (pukooa->main_dialog_id);
        wiopcd (pukooa->lines.dialog_id);
        break;
        
    case UKOOA_MAIN_Cancel_Btn:
    case UKOOA_MAIN_Done_Btn:
        am_quit ();
        break;
        
        }
    return SUCCESS;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    Raw UKOOA flat-file Initialization. The flat-file is opened
    and the first 20 records are placed in a buffer for installation
    in the custom dialog viewing window.

Prototype:
    static INT ukooa_main_open_flat_file(UKOOA_WORKSPACE *pukooa);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (M) pukooa      -(UKOOA_WORKSPACE *) Pointer to the application workspace.

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PRIVATE to <ukooa_main_server>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static INT ukooa_main_open_flat_file (UKOOA_WORKSPACE *pukooa) 
#else
static INT ukooa_main_open_flat_file (pukooa)
UKOOA_WORKSPACE *pukooa;
#endif
    {
    INT i, rlen, blen;
    CHAR Buffer[4096];
    CHAR message[STMAX], string[STMAX];
    PROGNAME ("ukooa_main_open_flat_file");
    
    pukooa->file.fp = ho_open (pukooa->file.file_name, "r", NULL);
    if (NOT pukooa->file.fp)
        {
        sprintf (message, mg_message (LD_CANT_OPEN_FILE), pukooa->file.file_name);
        am_message (AM_ALERT, message);
        
        return FAIL;
        }
    /* Load 20 records into custom dialog window */
    fgets (Buffer, STMAX, pukooa->file.fp);
    
    /* fix RecLen to max of record length */
    rlen = pukooa->file.RecLen;
    blen = strlen (Buffer);
    pukooa->file.RecLen = (rlen > blen) ? rlen : blen;
    
    for (i = 1; i < 20; ++i)
        {
        fgets (string, STMAX, pukooa->file.fp);
        strcat (Buffer, string);
        }
    /* Allocate a compact, AM-managed string buffer */
    pukooa->Buffer20 = STRING_ALLOCATE (Buffer);
    strcpy (pukooa->Buffer20, Buffer);
    
    /* Close flat-file */
    fclose (pukooa->file.fp);
    
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
    static INT ukooa_main_batch_job (UKOOA_WORKSPACE *pukooa);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) pukooa      -(UKOOA_WORKSPACE *) Pointer to the application workspace.

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PRIVATE to <ukooa_main_server>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static INT ukooa_main_batch_job (UKOOA_WORKSPACE *pukooa) 
#else
static INT ukooa_main_batch_job (pukooa)
UKOOA_WORKSPACE *pukooa;
#endif
    {
    INT status;
    CHAR login_oracle[STMAX], message[STMAX];
    DATE_TIME date_tag;
    FILENAME DAM_maker, batch_loader;
    FILE *comi_fp;
    FILENAME comi_file;
    FILENAME journal_name, journal_file;
    FILENAME DAM_file;
    INT len;
    CHAR directory[200];
    ANALYST_NAME account;
    ORACLE_NAME password;
    ORACLE_NAME database;
    PROGNAME ("ukooa_main_batch_job");
    
    /*  build the login string = account - password - database  */
    
    qs_inq_c (QS_FINDER_ACCOUNT, account, &len);
    qs_inq_c (QS_FINDER_PASSWORD, password, &len);
    qs_inq_c (QS_DATABASE, database, &len);
    
    if (len > 0)
        {
        sprintf (login_oracle, "%s/%s@%s", account, password, database);
        }
    else
        {
        sprintf (login_oracle, "%s/%s", account, password);
        }
    /* Get date */
    
    ho_date_tag (date_tag);
    
    /*   name of the batch loader   */
    ho_add_path ("ESI$BIN", "ld_ukooa", sizeof batch_loader, batch_loader);
    
    /*  name of the executable preparing the input file for the loader  */
    ho_add_path ("ESI$BIN", "ld_odam", sizeof(DAM_maker), DAM_maker);

#ifdef vms
    
    /*  name of the DCL file which is submitted to run the loader  */
    sprintf (comi_file, "UK_LOADER%s.COM", account);
    
    /*    name of the journal file used for this job    */
    sprintf (journal_name, "%s_UKOOA.JOU", account);
    
    /*    name of the temporary file used to store parameters for the ukooa loader */
    sprintf (DAM_file, "DAM_FILE%s_%s.DAT", account, date_tag);

#endif

#if UNIX
    /*    name of the script file which is submitted to run the loader  */
    sprintf (comi_file, "ukload_%s", date_tag);
    
    /*    name of the journal file used for this job    */
    sprintf (journal_name, "ukload_%s.jou", date_tag);
    
    /*    name of the temporary file used to store parameters for the ukooa loader */
    sprintf (DAM_file, "ukload_%s.dat", date_tag);
#endif

#ifdef primos
    
    /*  name of the CPL file which is submitted to run the loader  */
    sprintf (comi_file, "UKLOAD_%s.CPL", date_tag);
    
    /*    name of the journal file used for this job    */
    sprintf (journal_name, "UKLOAD_%s.JOU", date_tag);
    
    /*    name of the temporary file used to store parameters for the ukooa loader */
    sprintf (DAM_file, "UKLOAD_%s.DAT", date_tag);

#endif
    ho_add_path ("ESI$JOURNALS", journal_name, sizeof(journal_file), journal_file);
    
    /* Create command file */
    comi_fp = ho_open (comi_file, "w", NULL);
    status = ho_lock (comi_fp, ON);
    
    if (comi_fp EQUALS NULL_FILE_PTR || status != SUCCESS)
        {
        sprintf (message, mg_message (LD_COMFILE_NOT_OPEN));
        am_message (AM_ALERT, message);
        
        return FAIL;
        }
    /* Write commands to cominput file */
    /*   inquire for the default directory where we are running */
    
    ho_get_current_path (directory);

#ifdef vms
    fprintf (comi_fp, "$SET DEFAULT %s\n", directory);
    fprintf (comi_fp, "$RUN/NODEBUG %s\n", DAM_maker);
    ukooa_main_write_parameters (pukooa, comi_fp, DAM_file);
    fprintf (comi_fp, "$RUN/NODEBUG %s\n", batch_loader);
    fprintf (comi_fp, "%s\n", pukooa->file.file_name);
    fprintf (comi_fp, "%s\n", DAM_file);
    fprintf (comi_fp, "%s\n", journal_file);
    fprintf (comi_fp, "%s\n", login_oracle);
    fprintf (comi_fp, "$DELETE %s;*\n", DAM_file);
    fprintf (comi_fp, "$EXIT \n");
#endif

#if UNIX
    fprintf (comi_fp, "# Temporary script file created by Finder.\n");
    fprintf (comi_fp, "#\n# THIS FILE MAY BE DELETED AT ANY TIME.\n");
    fprintf (comi_fp, "#\n# store the parameters in a DAM file.\n");
    fprintf (comi_fp, "cd %s\n", directory);
    fprintf (comi_fp, "%s << ENDOFDATA\n", DAM_maker);
    ukooa_main_write_parameters (pukooa, comi_fp, DAM_file);
    fprintf (comi_fp, "ENDOFDATA\n");
    fprintf (comi_fp, "#\n# run the actual UKOOA loader.\n");
    fprintf (comi_fp, "%s << ENDOFDATA\n", batch_loader);
    fprintf (comi_fp, "%s\n", pukooa->file.file_name);
    fprintf (comi_fp, "%s\n", DAM_file);
    fprintf (comi_fp, "%s\n", journal_file);
    fprintf (comi_fp, "%s\n", login_oracle);
    fprintf (comi_fp, "ENDOFDATA\n");
    fprintf (comi_fp, "rm %s\n", DAM_file);
#endif

#ifdef primos
    fprintf (comi_fp, "/* Temporary script file created by Finder.\n");
    fprintf (comi_fp, "/*\n/* THIS FILE MAY BE DELETED AT ANY TIME.\n");
    fprintf (comi_fp, "/*\n/* set up environment\n");
    fprintf (comi_fp, "$$ JOB * -NOTIFY\n");
    fprintf (comi_fp, "SSR UNISOURCE*>ENTRY$\n");
    fprintf (comi_fp, "DEFINE_GVAR UNISOURCE*>FINDER_ORACLE.GVAR\n");
    fprintf (comi_fp, "ATTACH %s\n", directory);
    fprintf (comi_fp, "&if [exists %s] &then DELETE %s\n", DAM_file, DAM_file);
    fprintf (comi_fp, "&if [exists %s] &then DELETE %s\n", journal_file, journal_file);
    fprintf (comi_fp, "/*\n/* store the parameters in a DAM file.\n");
    fprintf (comi_fp, "&data R %s\n", DAM_maker);
    ukooa_main_write_parameters (pukooa, comi_fp, DAM_file);
    fprintf (comi_fp, "&end   /* of &data\n");
    fprintf (comi_fp, "/*\n/* run the actual UKOOA loader.\n");
    fprintf (comi_fp, "&data R %s\n", batch_loader);
    fprintf (comi_fp, "%s\n", pukooa->file.file_name);
    fprintf (comi_fp, "%s\n", DAM_file);
    fprintf (comi_fp, "%s\n", journal_file);
    fprintf (comi_fp, "%s\n", login_oracle);
    fprintf (comi_fp, "&end   /* of &data\n");
    fprintf (comi_fp, "/*\n/* clean up after the loading.\n");
    fprintf (comi_fp, "DELETE %s\n", DAM_file);
    fprintf (comi_fp, "&return\n");
#endif
    status = ho_lock (comi_fp, OFF);
    fclose (comi_fp);
    
    sprintf (message, mg_message (LD_SUBMIT_JOURNAL), comi_file, journal_file);
    am_message (AM_ALERT, message);
    
    /*  copy comi_file name   in the ukooa workspace  */
    
    strcpy (pukooa->comi_file, comi_file);
    
    uk_fire_batch (comi_file);
    
    return SUCCESS;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    Parameters writer. The parameters needed for processing the input
    data are writter to the COMINPUT file as data for the DAM
    generating program.

Prototype:
    static INT  ukooa_main_write_parameters(UKOOA_WORKSPACE *pukooa, 
                FILE *comi_fp, FILENAME DAM_file);
                
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) pukooa      -(UKOOA_WORKSPACE *) Pointer to the application workspace.
    (I) comi_fp     -(FILE *) File pointer to the COMINPUT file.
    DAM_file        -(FILENAME)

Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PRIVATE to <ukooa_main_server>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static INT ukooa_main_write_parameters (UKOOA_WORKSPACE *pukooa, FILE *comi_fp,
                                            FILENAME DAM_file) 
#else
static INT ukooa_main_write_parameters (pukooa, comi_fp, DAM_file)
UKOOA_WORKSPACE *pukooa;
FILE *comi_fp;
FILENAME DAM_file;
#endif
    {
    INT i;
    INT count;
    LINE_LINK *linelink;
    PROJECTION_ID projection;
    PROGNAME ("ukooa_main_write_parameters");
    
    fprintf (comi_fp, "+++ Start of Data for ODAM.RUN\n");
    fprintf (comi_fp, "%s\n", DAM_file);
    
    /********** Part 1 (General Info) **********/
    fprintf (comi_fp, "### First Part (General File Information)\n");
    
    /* Area Name */
    fprintf (comi_fp, "%s\n", pukooa->Area);
    
    /* Survey Name */
    fprintf (comi_fp, "%s\n", pukooa->header.Survey);
    
    /* Number of Header Records */
    fprintf (comi_fp, "%d\n", pukooa->header.NumHeaderRec);
    
    /* Record Length */
    fprintf (comi_fp, "%d\n", pukooa->file.RecLen);
    
    /* Number of Bogus Points per Seismic Line */
    fprintf (comi_fp, "%d\n", pukooa->lines.NumBogusPts);
    
    /* Projection Code */
    /*   if the projection code has not being filled we filled
  it with the default projection for the project  */
    if (!strcmp (pukooa->header.ProjCode, "\0"))
        {
        
        if (ct_get_default_project (projection) == SUCCESS)
            {
            pukooa->header.Values[1] = STRING_ALLOCATE (projection);
            strcpy (pukooa->header.Values[1], projection);
            strcpy (pukooa->header.ProjCode, projection);
            }
        }
    fprintf (comi_fp, "%s\n", pukooa->header.ProjCode);
    
    /* Columns delimiting data fields */
    for (i = 0; i < NUMBER_OF_DATA_FIELDS; i++)
        {
        fprintf (comi_fp, "%d,%d,%d\n", pukooa->fields.Columns[i][0],
                 pukooa->fields.Columns[i][1], pukooa->fields.Choix[i]);
        }
    /********** Part 2 (Line Selection Information) **********/
    fprintf (comi_fp, "### Second Part (Seismic Lines Selection Information)\n");
    
    /* Selection and Replacement Flags */
    fprintf (comi_fp, "%d\n", pukooa->lines.Selection);
    fprintf (comi_fp, "%d\n", pukooa->lines.Replacement);
    
    /* Selected Line Name, First SP, Last SP, and Skip Number */
    if (pukooa->lines.Selection == TRUE)
        {
        count = llcount (&pukooa->line_ll);
        if (count > 0)
            {
            for (i = 1; i <= count; i++)
                {
                if (i == 1)
                    llfirst (&pukooa->line_ll, &linelink);
                else
                    llnext (&pukooa->line_ll, &linelink);
                
                fprintf (comi_fp, "%s,%s,%s,%s\n", linelink->line, linelink->firstsp,
                         linelink->lastsp, linelink->skip);
                }
            }
        }
    fprintf (comi_fp, "+++ End of Data for ODAM.RUN\n");
    
    return SUCCESS;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    See above.
    
Prototype:
    static INT  uk_fire_batch (FILENAME comi_file);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) comi_file   -(FILENAME) 

Return Value/Status:
    None.
    
Scope:
    PRIVATE to <ukooa_main_server>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static INT uk_fire_batch (FILENAME comi_file) 
#else
static INT uk_fire_batch (comi_file)
FILENAME comi_file;
#endif
    {
    INT code;
    INT job_user_no;
    CHAR message[STMAX];
    INT status;
    
    code = ho_spawn (SPAWN_BATCH, comi_file, "DELETE", &job_user_no);
    
    if (code)
        {
        sprintf (message, mg_message (LD_SYSTEM_ERROR), mg (code), comi_file);
        }
    else
        sprintf (message, mg_message (LD_DATA_LOADING), job_user_no);
    am_message (AM_STATUS, message);
    
    }
/* END:     */
