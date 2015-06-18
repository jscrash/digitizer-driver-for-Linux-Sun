/* DEC/CMS REPLACEMENT HISTORY, Element TM_AM_DUMP.C*/
/* *4    17-AUG-1990 22:28:00 VINCE "(SPR 5644) Code Cleanup"*/
/* *3    11-AUG-1989 15:30:37 JULIAN "(SPR -1) variable data_id data unused"*/
/* *2    26-JUN-1989 11:04:45 GILLESPIE "(SPR 20) Fix castings"*/
/* *1    19-JUN-1989 13:32:55 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element TM_AM_DUMP.C*/
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

#include "esi_gl_defs.h"
#include "esi_am_int.h"
#include "esi_am_tstws.h"
#include "tmd_am_dump.rh"
#include "tmm_am_dump.rh"
#include "esi_mg.h"
#include "esi_tm_msg.h"
#include "esi_wi.h"

#define APPLICATION_MASK    "%5d %-20.20s %-11.11s %5d %-15d"
#define RESOURCES_MASK      "%5d %-30.30s %5d %5d %-6.6s %-15d"
#define WORKSPACES_MASK     "%-30.30s %5d %-15d"
#define DATA_MASK       "%5d %-11.11s %-15d"
#define ACCOUNTING_MASK     "%5d %-20.20s %-11.11s %5d %9.3f %9.3f"
static INT dump_dialog_server ();
static INT dump_menu_server ();
static INT dump_applications ();
static INT dump_resources ();
static INT dump_workspaces ();
static INT dump_data ();
static INT dump_accounting ();

static CHAR p[80];              /* Messages */
static CHAR type[12];           /* Type of entry in structure */
static INT count;               /* Counter */
static INT i;                   /* Counter */

static INT application_id;      /* Current application id */
static INT resource_id;         /* Current Resource id */
static APPLICATION_LIST app;    /* Application data */
static RESOURCE_LIST rsrc;      /* Resource data */
static WORKSPACE_LIST work;     /* Workspace data */

/* File Description ---------------------------------------------------------
Overview:

Prototypes and one line descriptions of each function in the file.
Public_Functions:
    publicdef INT tm_am_dump(BYTE *pointer);

Private_Functions:
    static INT dump_dialog_server(INT id,INT item,BYTE *pointer);
    
    static INT dump_menu_server(INT id,INT item,BYTE *pointer);
    
    static INT dump_applications();
    
    static INT dump_resources();
    
    static INT dump_workspaces();
    
    static INT dump_data();
    
    static INT dump_accounting();

-----------------------------------------------------------------------------*/

/* Function Description -----------------------------------------------------
Description:
    TM_AM_DUMP is an application which allows the user to view the internal
    structures used by the application manager.

    TM_AM_DUMP presents a File menu so that the user can "Quit".

    TM_AM_DUMP presents a Dump menu which allows the user to select which
    structure she wishes to view. The selected structure data is
    displayed in the Message window. The following structures may be
    viewed by making a selection from the Dump menu.

         Applications
         Resources
         Workspaces
         Data
         Accounting

Prototype:
    publicdef INT tm_am_dump(BYTE *pointer);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    pointer         -(BYTE *) Not used.

Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT tm_am_dump (BYTE *pointer)
#else
publicdef INT tm_am_dump (pointer)
BYTE *pointer;                      /* Not used */
#endif
    {
    PROGNAME ("TM_AM_DUMP");
    AM_TEST_WORKSPACE *w;           /* Pointer to local workspace */
    
    /* ********************************************************************** */
    
    am_declare ("AM_DUMP");
    
    /*   Define local workspace. */
    
    am_define_workspace ("AM_DUMP", AM_APPLICATION, sizeof(AM_TEST_WORKSPACE),
                         (VOIDPTR *) & w);
    
    /*   Define dump menu, make visible. */
    
    am_define_resource (AM_MENU, "TMM_AM_DUMP", TMM_AM_DUMP, dump_menu_server,
                        &w->dump_menu_id, NULL);
    wivismn (w->dump_menu_id, 1);
    
    /*   Define dialog, erase title, enable QUIT,
         and open dialog. */
    
    am_define_resource (AM_DIALOG, "TMD_AM_DUMP", TMD_AM_DUMP, dump_dialog_server,
                        &w->dialog_id, NULL);
    wiopcd (w->dialog_id);
    
    return SUCCESS;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:

Prototype:
    static INT dump_dialog_server(INT id,INT item,BYTE *pointer);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    id              -(INT) Dialog id.
    item            -(INT) Dialog item.
    pointer         -(BYTE *) Not used.

Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PRIVATE to <tm_am_dump.c>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static INT dump_dialog_server (INT id, INT item, BYTE *pointer)
#else
static INT dump_dialog_server (id, item, pointer)
INT id;
INT item;
BYTE *pointer;
#endif
    {
    PROGNAME ("DUMP_DIALOG_SERVER");
    /* ********************************************************************** */
    
    /*   The only item that can come from the
         dialog is File Menu Quit. */
    
    am_quit ();
    
    return SUCCESS;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:

Prototype:
    static INT dump_menu_server(INT id,INT item,BYTE *pointer);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    id              -(INT) Menu id.
    item            -(INT) Menu command.
    pointer         -(BYTE *) Not used.

Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PRIVATE to <tm_am_dump.c>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static INT dump_menu_server (INT id, INT item, BYTE *pointer)
#else
static INT dump_menu_server (id, item, pointer)
INT id;
INT item;
BYTE *pointer;
#endif
    {
    PROGNAME ("DUMP_MENU_SERVER");
    /* ********************************************************************** */
    
    switch (item)
        {
        
    case AM_DUMP_MENU_APPLICATIONS:
        dump_applications ();
        break;
        
    case AM_DUMP_MENU_RESOURCES:
        dump_resources ();
        break;
        
    case AM_DUMP_MENU_WORKSPACES:
        dump_workspaces ();
        break;
        
    case AM_DUMP_MENU_DATA:
        dump_data ();
        break;
        
    case AM_DUMP_MENU_ACCOUNTING:
        dump_accounting ();
        break;
        
    case AM_DUMP_MENU_ALL:
        dump_applications ();
        dump_resources ();
        dump_workspaces ();
        dump_data ();
        dump_accounting ();
        break;
        }
    return SUCCESS;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:

Prototype:
    static INT dump_applications();

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    None.
    
Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PRIVATE to <tm_am_dump.c>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static INT dump_applications ()
#else
static INT dump_applications ()
#endif
    {
    PROGNAME ("DUMP_APPLICATIONS");
    APPLICATION_STATIC *a;          /* Pointer to AM internals */
    
    /* ********************************************************************** */
    
    /*   Get pointer to AM internals. */
    
    a = am_static ();
    
    /*   Put up headings. */
    
    am_message (AM_REPORT, mg (TM_AM_DUMP_APPLICATIONS));
    am_message (AM_REPORT, mg (TM_AM_DUMP_APP_HDR));
    am_message (AM_REPORT,
                "----- -------------------- ----------- ----- ---------------");
    
    /*   Get number of applications. */
    
    count = btcount (&a->application_list);
    
    /*   For each application, print data. */
    
    for (btfirst (&a->application_list, &application_id, &app), i = 0; i < count;
         i++, btnext (&a->application_list, &application_id, &app))
        {
        
        /*   Get type of application, which is
             either APPLICATION or SERVER. */
        
        if (app.type == AM_APPLICATION)
            strcpy (type, mg (TM_AM_DUMP_APPLICATION));
        else
            strcpy (type, mg (TM_AM_DUMP_SERVER));
        
        /*   Format data for output. */
        
        sprintf (p, APPLICATION_MASK, application_id, app.tag, type, app.parent_id,
                 app.server);
        
        /*   Display application data. */
        
        am_message (AM_REPORT, p);
        }
    return SUCCESS;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:

Prototype:
    static INT dump_resources();

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    None.
    
Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PRIVATE to <tm_am_dump.c>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static INT dump_resources ()
#else
static INT dump_resources ()
#endif
    {
    PROGNAME ("DUMP_RESOURCES");
    APPLICATION_STATIC *a;          /* Pointer to AM internals */
    
    /* ********************************************************************** */
    
    /*   Get pointer to AM internals. */
    
    a = am_static ();
    
    /*   Put up headings. */
    
    am_message (AM_REPORT, mg (TM_AM_DUMP_RESOURCES));
    am_message (AM_REPORT, mg (TM_AM_DUMP_RSRC_HDR));
    am_message (AM_REPORT,
            "----- ------------------------------ ----- ----- ------ ---------------");
    
    /*   Get number of resources. */
    
    count = btcount (&a->resource_list);
    
    /*   For each resource, print data. */
    
    for (btfirst (&a->resource_list, &resource_id, &rsrc), i = 0; i < count;
         i++, btnext (&a->resource_list, &resource_id, &rsrc))
        {
        
        /*   Get type of resource, which is
             either MENU or DIALOG. */
        
        if (rsrc.type == AM_DIALOG)
            strcpy (type, mg (TM_AM_DUMP_DIALOG));
        else
            strcpy (type, mg (TM_AM_DUMP_MENU));
        
        /*   Format data for output. */
        
        sprintf (p, RESOURCES_MASK, resource_id, rsrc.file, rsrc.resource_id,
                 rsrc.application_id, type, rsrc.pointer);
        
        /*   Display resource data. */
        
        am_message (AM_REPORT, p);
        }
    return SUCCESS;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:

Prototype:
    static INT dump_workspaces();

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    None.
    
Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PRIVATE to <tm_am_dump.c>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static INT dump_workspaces ()
#else
static INT dump_workspaces ()
#endif
    {
    PROGNAME ("DUMP_WORKSPACES");
    APPLICATION_STATIC *a;          /* Pointer to AM internals */
    
    /* ********************************************************************** */
    
    /*   Get pointer to AM internals. */
    
    a = am_static ();
    
    /*   Put up headings. */
    
    am_message (AM_REPORT, mg (TM_AM_DUMP_WORKSPACES));
    am_message (AM_REPORT, mg (TM_AM_DUMP_WKSP_HDR));
    am_message (AM_REPORT, "------------------------------ ----- ---------------");
    
    /*   Get number of workspaces. */
    
    count = llcount (&a->workspace_list);
    
    /*   For each workspace, print data. */
    
    if (count)
        {
        for (llfirst (&a->workspace_list, &work), i = 0; i < count;
             i++, llnext (&a->workspace_list, &work))
            {
            
            /*   Format data for output. */
            
            sprintf (p, WORKSPACES_MASK, work.name, work.application_id, work.data);
            
            /*   Display workspace data. */
            
            am_message (AM_REPORT, p);
            }
        }
    else
        am_message (AM_REPORT, mg (TM_AM_DUMP_NONE));
    
    return SUCCESS;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:

Prototype:
    static INT dump_data();

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    None.
    
Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PRIVATE to <tm_am_dump.c>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static INT dump_data ()
#else
static INT dump_data ()
#endif
    {
    PROGNAME ("DUMP_DATA");
    APPLICATION_STATIC *a;          /* Pointer to AM internals */
    
    typedef DATA_LIST *DATA_LIST_PTR;
    typedef BYTE *BYTE_PTR;
    
    UINT nkeys;                     /* number of entries in data_list table */
    /*-----------------------------------------*/
    /* pointer to the buffer of keys returned  */
    /* by call to ht_get_all                   */
    /*-----------------------------------------*/
    BYTE_PTR key_buff;
    
    /*-------------------------------------*/
    /* pointer to the buffer of data items */
    /* returned by call to ht_get_all      */
    /*-------------------------------------*/
    BYTE_PTR data_buff;
    
    /*--------------------------------------*/
    /* pointer to a single data item in the */
    /* buffer of data items                 */
    /*--------------------------------------*/
    DATA_LIST_PTR curr_data;
    
    /*---------------------------------------*/
    /* pointer to a single key in the buffer */
    /* of keys                               */
    /*---------------------------------------*/
    BYTE_PTR *curr_key;
    
    INT status;
    
    /* ********************************************************************** */
    
    /*   Get pointer to AM internals. */
    
    a = am_static ();
    
    /*   Put up headings. */
    
    am_message (AM_REPORT, mg (TM_AM_DUMP_DATA));
    am_message (AM_REPORT, mg (TM_AM_DUMP_DATA_HDR));
    am_message (AM_REPORT, "----- ----------- ---------------");
    
    /*   retrieve all entries from the 
         data_list structure         */
    
    status = ht_get_all (a->data_list, &nkeys, (VOIDPTR *) & key_buff,
                         (VOIDPTR *) & data_buff);
    
    curr_data = (DATA_LIST_PTR)data_buff;
    curr_key = (BYTE_PTR *)key_buff;
    
    /*   traverse the list and print the data */
    
    if (nkeys)
        {
        for (i = 0; i < nkeys; i++)
            {
            
            /*    construct type of data, which is    
                  APPLICATION, GLOBAL, or CURSOR. */
            
            switch (curr_data->type)
                {
            case AM_GLOBAL:
                strcpy (type, mg (TM_AM_DUMP_GLOBAL));
                break;
            case AM_APPLICATION:
                strcpy (type, mg (TM_AM_DUMP_APPLICATION));
                break;
                }
            /*   Format data for output. */
            
            sprintf (p, DATA_MASK, curr_data->application_id, type, *curr_key);
            
            /*   Display data data. */
            
            am_message (AM_REPORT, p);
            
            curr_key++;
            curr_data++;
            
            }
        }
    else
        am_message (AM_REPORT, mg (TM_AM_DUMP_NONE));
    
    return SUCCESS;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:

Prototype:
    static INT dump_accounting();

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    None.
    
Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PRIVATE to <tm_am_dump.c>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static INT dump_accounting ()
#else
static INT dump_accounting ()
#endif
    {
    PROGNAME ("DUMP_ACCOUNTING");
    APPLICATION_STATIC *a;          /* Pointer to AM internals */
    CHAR enabled[9];                /* Accounting enabled/disabled string */
    
    /* ********************************************************************** */
    
    /*   Get pointer to AM internals. */
    
    a = am_static ();
    
    /*   Construct indicator to say whether
         accounting is currently enabled or not. */
    
    if (a->accounting)
        strcpy (enabled, mg (TM_AM_DUMP_ENABLED));
    else
        strcpy (enabled, mg (TM_AM_DUMP_DISABLED));
    sprintf (p, mg (TM_AM_DUMP_ACCOUNTING), enabled);
    
    /*   Put up headings. */
    
    am_message (AM_REPORT, p);
    am_message (AM_REPORT, mg (TM_AM_DUMP_ACC_HDR));
    am_message (AM_REPORT,
                "----- -------------------- ----------- ----- --------- ---------");
    
    /*   Get number of applications. */
    
    count = btcount (&a->application_list);
    
    /*   For each application which has
         accounting enabled, print data. */
    
    for (btfirst (&a->application_list, &application_id, &app), i = 0; i < count;
         i++, btnext (&a->application_list, &application_id, &app))
        {
        if (app.accounting)
            {
            
            /*   Construct application type. */
            
            if (app.type == AM_APPLICATION)
                strcpy (type, mg (TM_AM_DUMP_APPLICATION));
            else
                strcpy (type, mg (TM_AM_DUMP_SERVER));
            
            /*   Format data for output. */
            
            sprintf (p, ACCOUNTING_MASK, application_id, app.tag, type, app.parent_id,
                     app.cpu_time, app.io_time);
            
            /*   Display accounting data for application. */
            
            am_message (AM_REPORT, p);
            }
        }
    return SUCCESS;
    }
/* END:     */
