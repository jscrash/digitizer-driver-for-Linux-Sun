/* DEC/CMS REPLACEMENT HISTORY, Element LD_TL_TOBIN.C*/
/* *8    14-AUG-1991 11:59:47 KEE "(SPR -1) Put in File Lock/Unlock logic"*/
/* *7    26-JUL-1991 14:55:28 CHEW "(SPR 0) Change default to northwest region."*/
/* *6    19-JUL-1991 10:41:38 CHEW "(SPR 0) Prompt users for longitude and latitude input for tobin loader."*/
/* *5    23-AUG-1990 16:13:28 VINCE "(SPR 1) part of a mass #if HOST to #ifdef conversion"*/
/* *4    14-AUG-1990 13:32:48 VINCE "(SPR 5644) Header Standardization"*/
/* *3     5-SEP-1989 16:10:32 GORDON "(SPR 200) Gulf changes (August trip)"*/
/* *2    11-AUG-1989 11:00:01 JULIAN "(SPR -1) static variable TOBIN_PHANTOM_SERVER unused"*/
/* *1    19-JUN-1989 13:03:24 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element LD_TL_TOBIN.C*/
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
#include "esi_ld_tl_tobin.h"

#ifndef ESI_AM_H

#include "esi_am.h"

#endif

#ifndef ESI_HO_H

#include "esi_ho.h"

#endif

#ifndef ESI_LD_MSG_H

#include "esi_ld_msg.h"

#endif

#ifndef ESI_MG_H

#include "esi_mg.h"

#endif

#ifndef ESI_QS_H

#include "esi_qs.h"

#endif

#ifndef ESI_TS_H

#include "esi_ts.h"

#endif

#ifndef ESI_WI_H

#include "esi_wi.h"

#endif

#ifndef ESI_HO_FILES_H

#include "esi_ho_files.h"

#endif

#include "ld_tl.rh"

#if USE_PROTOTYPES
static INT tl_tobin_loader_cdlg_server (INT cdlgid, INT item, TL_STRUCT *pTLS);
static INT tl_tobin_loader_init (TL_STRUCT *pTLS);
static INT tl_tobin_loader_reset (TL_STRUCT *pTLS);
static INT tl_tobin_loader_display (TL_STRUCT *pTLS);
static INT tl_tobin_loader_read (TL_STRUCT *pTLS);
static INT tl_tobin_loader_selector (TL_STRUCT *pTLS);
static INT tl_tobin_loader_spawn (TL_STRUCT *pTLS);
#else
static INT tl_tobin_loader_cdlg_server ();
static INT tl_tobin_loader_init ();
static INT tl_tobin_loader_reset ();
static INT tl_tobin_loader_display ();
static INT tl_tobin_loader_read ();
static INT tl_tobin_loader_selector ();
static INT tl_tobin_loader_spawn ();
#endif
static CHAR *TOBIN_CDLG_FILE = 
    {
    "LD_TL"
    };


static CHAR *TOBIN_SELECTOR_HELP = 
    {
    ""
    };



static CODE_STRUCT tobin_codes[] = 
    {
    "Annotation Only",
    1,
    "Sections or Abstracts",
    2,
    "Townships",
    3,
    "Counties",
    4,
    "State Borders",
    5,
    "Process Outlines",
    6,
    "Municipalities",
    7,
    "Rivers",
    8,
    "Streams",
    9,
    "Islands",
    10,
    "Lakes",
    11,
    "Old Riverbeds",
    12,
    "Ocean Areas",
    13,
    "Federal (US) Highways",
    14,
    "State Highways",
    15,
    "Other Travelways",
    16,
    "Pipelines",
    17,
    "Railroads",
    18,
    "Monumented Section Corners",
    19,
    "Benchmarks",
    20,
    "Monuments",
    21,
    "Federal Reservations",
    22,
    "State Reservations",
    23,
    "Transmission Lines",
    24,
    "Unsurveyed Areas",
    26,
    "end of list",
    0
    };

/* must be last in list  */

/* File Description ---------------------------------------------------------
Overview:
    Read a Tobin format tape and load it as a Graphic Object.

    START-HISTORY :
    06/08/87 VINCE    Converted to V5.
    10/16/86 DAVID    Cartographic pipeline implementation
    07/18/86 VINCE    fixed the use of the returned values from inq selector.
    07/18/86 VINCE    Changed Logout.Delete to LogDel
    07/18/86 VINCE    Fixed problem with for loop running off the end of
                     the tobin_codes array.
    07/16/86 VINCE    Forgot to send to_area to the phantom process.
    07/16/86 VINCE    Fixed bug in SSR fprintf line.
    07/09/86 VINCE    Added 'R' in front of phantom server path so that it
                     does not have to be part of the path.
    07/07/86 VINCE    Corrected spelling of prompt
    06/16/86 VINCE    Added ui_close call.
    06/11/86 VINCE    Initial Coding
    END-HISTORY 

Prototypes and one line descriptions of each function in the file.
Public_Functions:
    publicdef   INT  tl_tobin_main (TL_STRUCT *pTLS);

Private_Functions:
    static  INT  tl_tobin_loader_cdlg_server
                (INT cdlgid, INT item, TL_STRUCT *pTLS);
    
    static  INT  tl_tobin_loader_init       (TL_STRUCT *pTLS);
    
    static  INT  tl_tobin_loader_reset      (TL_STRUCT *pTLS);
    
    static  INT  tl_tobin_loader_display    (TL_STRUCT *pTLS);
    
    static  INT  tl_tobin_loader_read       (TL_STRUCT *pTLS);
    
    static  INT  tl_tobin_loader_selector   (TL_STRUCT *pTLS);
    
    static  INT  tl_tobin_loader_spawn      (TL_STRUCT *pTLS);

-----------------------------------------------------------------------------*/

/* Function Description -----------------------------------------------------
Description:

Prototype:
    publicdef   INT  tl_tobin_main (TL_STRUCT *pTLS);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    pTLS            -(TL_STRUCT *)

Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT tl_tobin_main (TL_STRUCT *pTLS) 
#else
publicdef INT tl_tobin_main (pTLS)
TL_STRUCT *pTLS;
#endif
    {
    tl_tobin_loader_init (pTLS);
    return SUCCESS;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    See above.
    
Prototype:
    static  INT  tl_tobin_loader_cdlg_server (INT cdlgid, INT item, TL_STRUCT *pTLS);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    cdlgid          -(INT) 
    item            -(INT)
    pTLS            -(TL_STRUCT *)

Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PRIVATE to <tl_tobin_main>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static INT tl_tobin_loader_cdlg_server (INT cdlgid, INT item, TL_STRUCT *pTLS) 
#else
static INT tl_tobin_loader_cdlg_server (cdlgid, item, pTLS)
INT cdlgid, item;
TL_STRUCT *pTLS;
#endif
    {
    INT status;
    
    status = SUCCESS;
    
    switch (item)
        {
    case TL_BTN_CANCEL:
        status = am_quit ();
        break;
    case TL_BTN_OK:
        tl_tobin_loader_read (pTLS);
        if (pTLS->load_type == TL_LOAD_SELECTED_DATA)
            tl_tobin_loader_selector (pTLS);
        tl_tobin_loader_spawn (pTLS);
        status = am_quit ();
        break;
    case TL_BTN_RESET:
        tl_tobin_loader_reset (pTLS);
        tl_tobin_loader_display (pTLS);
        break;
    default:
        break;
        }
    return status;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    See above.
    
Prototype:
    static  INT  tl_tobin_loader_init (TL_STRUCT *pTLS);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    pTLS            -(TL_STRUCT *)

Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PRIVATE to <tl_tobin_main>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static INT tl_tobin_loader_init (TL_STRUCT *pTLS) 
#else
static INT tl_tobin_loader_init (pTLS)
TL_STRUCT *pTLS;
#endif
    {
    INT cdlgid;
    
    am_declare ("LD_TL_TOBIN");
    pTLS = (TL_STRUCT *)am_allocate (AM_APPLICATION, sizeof(TL_STRUCT));
    
    am_define_resource (AM_DIALOG, TOBIN_CDLG_FILE, TOBIN_CDLG_ID,
                        tl_tobin_loader_cdlg_server, &pTLS->dialog_id, pTLS);
    
    tl_tobin_loader_reset (pTLS);
    tl_tobin_loader_display (pTLS);
    wiencdi (pTLS->dialog_id, TL_RBT_SUBSET, FALSE);    /* cant do subset yet */
    wiscdv  (pTLS->dialog_id, TL_RBT_NOR, TRUE);
    wiscdv  (pTLS->dialog_id, TL_RBT_WES, TRUE);
    
    wiopcd (pTLS->dialog_id);
    
    return SUCCESS;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    See above.
    
Prototype:
    static  INT  tl_tobin_loader_reset (TL_STRUCT *pTLS);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    pTLS            -(TL_STRUCT *)

Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PRIVATE to <tl_tobin_main>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static INT tl_tobin_loader_reset (TL_STRUCT *pTLS) 
#else
static INT tl_tobin_loader_reset (pTLS)
TL_STRUCT *pTLS;
#endif
    {
    /*    Reset the Tobin Loader Structure back to the default values.          */
    
    PROJECT_NAME project;
    INT project_len;

#ifndef vms
    CHAR **tcp;
#endif
    
    /* get the current project name and use it as the default TO project.             */
    qs_inq_c (QS_PROJECT_NAME, project, &project_len);
    strcpy (pTLS->to_area, project);
    
    /* set a default FROM directory and load ALL DATA TYPES from it.            */
#ifdef vms
    strcpy (pTLS->disk_name, "ESI$PROJECT:[RAW_TOBIN_DATA]*.*");
#endif
#ifdef primos
    ho_translate_symbol ("ESI$PROJECT", &tcp);
    sprintf (pTLS->disk_name, "%s>RAW_TOBIN_DATA>@@", tcp[0]);
#endif
#if UNIX
    ho_translate_symbol ("ESI$PROJECT", &tcp);
    sprintf (pTLS->disk_name, "%s/raw_tobin_data/*", tcp[0]);
#endif

    pTLS->load_type = TL_LOAD_ALL_DATA;
    
    return SUCCESS;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    See above.
    
Prototype:
    static  INT  tl_tobin_loader_display (TL_STRUCT *pTLS);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    pTLS            -(TL_STRUCT *)

Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PRIVATE to <tl_tobin_main>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static INT tl_tobin_loader_display (TL_STRUCT *pTLS) 
#else
static INT tl_tobin_loader_display (pTLS)
TL_STRUCT *pTLS;
#endif
    {
    wiscdt (pTLS->dialog_id, TL_ETX_DIR_NAME, pTLS->disk_name);
    wiscdt (pTLS->dialog_id, TL_ETX_AREA_NAME, pTLS->to_area);
    
    if (pTLS->load_type == TL_LOAD_ALL_DATA)
        {
        wiscdv (pTLS->dialog_id, TL_RBT_ALL, TRUE);
        wiscdv (pTLS->dialog_id, TL_RBT_SUBSET, FALSE);
        }
    else
        {
        wiscdv (pTLS->dialog_id, TL_RBT_ALL, FALSE);
        wiscdv (pTLS->dialog_id, TL_RBT_SUBSET, TRUE);
        }
    return SUCCESS;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    See above.
    
Prototype:
    static  INT  tl_tobin_loader_read (TL_STRUCT *pTLS);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    pTLS            -(TL_STRUCT *)

Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PRIVATE to <tl_tobin_main>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static INT tl_tobin_loader_read (TL_STRUCT *pTLS) 
#else
static INT tl_tobin_loader_read (pTLS)
TL_STRUCT *pTLS;
#endif
    {
    INT all_on, dummy_int, north, west;
    CHAR text[81];
    
    wiqccdi (pTLS->dialog_id, TL_ETX_DIR_NAME, &dummy_int, pTLS->disk_name);
    wiqccdi (pTLS->dialog_id, TL_ETX_AREA_NAME, &dummy_int, pTLS->to_area);
    
    wiqccdi (pTLS->dialog_id, TL_RBT_ALL, &all_on, text);
    wiqccdi (pTLS->dialog_id, TL_RBT_NOR, &north, text);
    wiqccdi (pTLS->dialog_id, TL_RBT_WES, &west, text);
    if (north)
        strcpy (pTLS->lat, TL_DATA_LOC_NORTH);
    else
        strcpy (pTLS->lat, TL_DATA_LOC_SOUTH);

    if (west)
        strcpy (pTLS->lon, TL_DATA_LOC_WEST);
    else
        strcpy (pTLS->lon, TL_DATA_LOC_EAST);
    
    pTLS->load_type = TL_LOAD_SELECTED_DATA;
    if (all_on)
        pTLS->load_type = TL_LOAD_ALL_DATA;
    
    return SUCCESS;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    See above.
    
Prototype:
    static  INT  tl_tobin_loader_selector (TL_STRUCT *pTLS);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    pTLS            -(TL_STRUCT *)

Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PRIVATE to <tl_tobin_main>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static INT tl_tobin_loader_selector (TL_STRUCT *pTLS) 
#else
static INT tl_tobin_loader_selector (pTLS)
TL_STRUCT *pTLS;
#endif
    {
    CHAR *selections[TOBIN_CODES_DEFINED];
    INT selected[TOBIN_CODES_DEFINED];
    INT i, n_sent, n_codes, status, codes[TOBIN_CODES_DEFINED];
    
    for (i = 0, n_sent = 0; tobin_codes[i].code_code; i++)
        {
        selections[i] = tobin_codes[i].code_name;
        n_sent++;
        }
    wissst (TOBIN_SELECTOR_HELP, n_sent, n_sent, selections);
    wirqss ("Select the data types to be loaded.", &n_codes, codes, &status);
    
    if (n_codes && status)
        {
        pTLS->load_type = TL_LOAD_SELECTED_DATA;
        pTLS->n_codes_to_load = n_codes;
        
        for (i = 0; i < n_codes; i++)
            pTLS->load_codes[i] = tobin_codes[codes[i] - 1].code_code;
        }
    else
        {
        pTLS->load_type = TL_LOAD_ALL_DATA;
        }
    return SUCCESS;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    See above.
    
Prototype:
    static  INT  tl_tobin_loader_spawn (TL_STRUCT *pTLS);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    pTLS            -(TL_STRUCT *)

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PRIVATE to <tl_tobin_main>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static INT tl_tobin_loader_spawn (TL_STRUCT *pTLS) 
#else
static INT tl_tobin_loader_spawn (pTLS)
TL_STRUCT *pTLS;
#endif
    {
    INT i, status;
    FILE *fp;
    FILENAME subset_file;
    FILENAME command_file;
    CHAR oracle_account[33], oracle_password[33], oracle_login[129];
    CHAR database[31];
    CHAR command[512], submit_com[512], err[512];
    
    status = SUCCESS;
    
    qs_inq_c (QS_FINDER_ACCOUNT, oracle_account, (INT *)0);
    qs_inq_c (QS_FINDER_PASSWORD, oracle_password, (INT *)0);
    qs_inq_c (QS_DATABASE, database, &i);
    
    if (i > 0)
        {
        sprintf (oracle_login, "%s/%s@%s", oracle_account, oracle_password, database);
        }
    else
        {
        sprintf (oracle_login, "%s/%s", oracle_account, oracle_password);
        }
    /* each argument pair is enclosed in double quotes and separated by a comma */
    /* if it is a single argument, then the double quotes are not required */

#ifndef primos
    sprintf (command,
        "\"%s\",\"-PROJECT %s\",\"-INPUT %s\",\"-LON %s\",\"-LAT %s\"",
        oracle_login, pTLS->to_area, pTLS->disk_name, pTLS->lon, pTLS->lat);
    
    if (pTLS->load_type == TL_LOAD_ALL_DATA)
        {
        strcat (command, ",-ALL");
        }
    else
        {
        ho_add_path ("ESI$SCRATCH", "tobin_subset_file.dat", sizeof subset_file,
                     subset_file);
        fp = ho_open (subset_file, "w", NULL);
	status = ho_lock (fp, ON);
        if (fp == NULL_FILE_PTR || status != SUCCESS)
            {
            sprintf (err, "%s [%s].", mg (LD_TL_CANT_LOAD_SUBSET), subset_file);
            wi_alert (0, "", mg (LD_TL_CHNG_LOAD_ALL), err);
            strcat (command, ",-ALL");
            }
        else
            {
            for (i = 0; i < pTLS->n_codes_to_load; i++)
                fprintf (fp, "%s\n", tobin_codes[pTLS->load_codes[i]].code_name);
	    status = ho_lock (fp, OFF);	    
            fclose (fp);
            strcat (command, ",\"-SUBSET ");
            strcat (command, subset_file);
            strcat (command, "\"");
            }
        }
#else

    sprintf (command, "%s '-PROJECT' %s '-INPUT' %s '-LON' %s '-LAT' %s", 
        oracle_login, pTLS->to_area, pTLS->disk_name, pTLS->lon, pTLS->lat);
    
    if (pTLS->load_type == TL_LOAD_ALL_DATA)
        {
        strcat (command, " '-ALL'");
        }
    else
        {
        ho_add_path ("ESI$SCRATCH", "TOBIN_SUBSET_FILE.DAT", sizeof subset_file,
                     subset_file);
        fp = ho_open (subset_file, "w", NULL);
	status = ho_lock (fp, ON);
        if (fp == NULL_FILE_PTR || status != SUCCESS)
            {
            sprintf (err, "%s [%s].", mg (LD_TL_CANT_LOAD_SUBSET), subset_file);
            wi_alert (0, "", mg (LD_TL_CHNG_LOAD_ALL), err);
            strcat (command, " '-ALL'");
            }
        else
            {
            for (i = 0; i < pTLS->n_codes_to_load; i++)
                fprintf (fp, "%s\n", tobin_codes[pTLS->load_codes[i]].code_name);
	    status = ho_lock (fp, OFF);	    
            fclose (fp);
            strcat (command, " '-SUBSET' ");
            strcat (command, subset_file);
            }
        }
#endif
    
    /* submit the job to the batch queue */
    
    ho_add_path ("ESI$BIN", "ld_run_tobin_loader", sizeof command_file, command_file);
    status = ho_spawn (SPAWN_BATCH, command_file, command, (INT *)0);
    
    return status;
    }
/* END:     */
