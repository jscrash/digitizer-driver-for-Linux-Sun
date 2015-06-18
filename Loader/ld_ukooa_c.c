/* DEC/CMS REPLACEMENT HISTORY, Element LD_UKOOA_C.C*/
/* *6    14-NOV-1990 15:40:28 MING "(SPR 6193) dim LINES option on main dialog box"*/
/* *5    14-AUG-1990 13:33:10 VINCE "(SPR 5644) Header Standardization"*/
/* *4     4-AUG-1989 11:06:40 ADEY "(SPR -1) CHANGE NAMES"*/
/* *3    25-JUL-1989 16:44:51 ADEY "(SPR -1) Improve User Interface & Decode lat/long and survey/line for seismic"*/
/* *2    26-JUN-1989 10:47:59 GILLESPIE "(SPR 20) Fix castings"*/
/* *1    19-JUN-1989 13:03:28 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element LD_UKOOA_C.C*/
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
#include "esi_am.h"
#include "esi_ll.h"
#include "esi_qs.h"
#include "ld_uk_main.rh"
#include "ld_uk_header.rh"
#include "ld_uk_lines.rh"
#include "ld_uk_fields.rh"
#include "esi_tc.h"
#include "esi_ts.h"
#include "esi_wi.h"
#include "uk_work.x"

INT ukooa_reset_workspace (UKOOA_WORKSPACE *pukooa) ;
/* FUNCTION DECLARATIONS */

extern INT ukooa_main_server ();
extern INT ukooa_header_server ();
extern INT ukooa_fields_server ();
extern INT ukooa_lines_server ();

/* File Description ---------------------------------------------------------
Overview:
    Ukooa loader.

    START-HISTORY :
    12/12/86 KIMMAN   first time
    12/12/86 GEORGE DOBEK/KIMMAN  Template constructed
    END-HISTORY 

Prototypes and one line descriptions of each function in the file.
Public_Functions:
    publicdef INT ukooa_main (BYTE *pointer);
    
    publicdef INT ukooa_reset_workspace(UKOOA_WORKSPACE *pukooa);
    
Private_Functions:

-----------------------------------------------------------------------------*/

/* Function Description -----------------------------------------------------
Description:
    Initialize application. ukooa accesses the necessary
    resource files and loads them into the MAC terminal.
    The application manager is given the names of functions
    for handling resource events.

Prototype:
    publicdef INT ukooa_main (BYTE *pointer);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) pointer     -(BYTE *) Required Application Manager BYTE pointer.
                        It is not utilized in this application.

Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT ukooa_main (BYTE *pointer) 
#else
publicdef INT ukooa_main (pointer)
BYTE *pointer;
#endif
    {
    UKOOA_WORKSPACE *pukooa;
    INT area_len;
    CHAR text[STMAX];
    
    am_declare ("UKOOA");
    
    am_define_workspace ("UKOOA", AM_APPLICATION, sizeof(UKOOA_WORKSPACE),
                         (VOIDPTR *) & pukooa);
    
    am_define_resource (AM_DIALOG, "LD_UK_MAIN", 1, ukooa_main_server,
                        &pukooa->main_dialog_id, pukooa);
    
    am_define_resource (AM_DIALOG, "LD_UK_HEADER", 1, ukooa_header_server,
                        &pukooa->header.dialog_id, pukooa);
    
    am_define_resource (AM_DIALOG, "LD_UK_FIELDS", 1, ukooa_fields_server,
                        &pukooa->fields.dialog_id, pukooa);
    
    am_define_resource (AM_DIALOG, "LD_UK_LINES", 1, ukooa_lines_server,
                        &pukooa->lines.dialog_id, pukooa);
    
    /* Assign default values to workspace variables. */
    ukooa_reset_workspace (pukooa);
    
    /* Initialize the Main custom dialog */
    qs_inq_c (QS_PROJECT_NAME, pukooa->Area, &area_len);

    /* disable LINES option at this moment */
    wi_enable_dialog_item (pukooa->main_dialog_id,UKOOA_MAIN_Lines_Btn,OFF);

    
    wiscdt (pukooa->main_dialog_id, UKOOA_MAIN_Area_Stat, pukooa->Area);
    
    strcpy (text, "Enter UKOOA data file name");
    wiscdt (pukooa->main_dialog_id, UKOOA_MAIN_File_Edit, text);
    
    /* Initialize the Header Decoding custom dialog */
    wisselt (pukooa->header.dialog_id, UKOOA_HEADER_Parameters_Select,
             UKOOA_HEADER_Parameters_VScroll, ts_tcp_len (Header_Parameters), 1,
             Header_Parameters);
    
    wiscdr (pukooa->header.dialog_id, UKOOA_HEADER_Parameters_VScroll, 0,
            ts_tcp_len (Header_Parameters) - 3);
    
    wiscdt (pukooa->header.dialog_id, UKOOA_HEADER_NumHeaderRec_Edit, "4");
    
    /* Initialize the Data Fields Definition custom dialog */
    wisselt (pukooa->fields.dialog_id, UKOOA_FIELDS_Parameters_Select,
             UKOOA_FIELDS_Parameters_VScroll, NUMBER_OF_DATA_FIELDS, 1,
             Fields_Parameters);
    
    wiscdr (pukooa->fields.dialog_id, UKOOA_FIELDS_Parameters_VScroll, 0,
            NUMBER_OF_DATA_FIELDS - 3);
    
    wiopcd (pukooa->main_dialog_id);
    
    return SUCCESS;
    
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    Resets the UKOOA_WORKSPACE structure values to defaults.

Prototype:
    publicdef INT ukooa_reset_workspace(UKOOA_WORKSPACE *pukooa);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (M) pukooa          -(UKOOA_WORKSPACE *) Pointer to application workspace.

Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT ukooa_reset_workspace (UKOOA_WORKSPACE *pukooa) 
#else
publicdef INT ukooa_reset_workspace (pukooa)
UKOOA_WORKSPACE *pukooa;
#endif
    {
    INT i;
    CHAR string[81];
    
    pukooa->file.fp = (FILE *)0;
    strcpy (pukooa->file.file_name, "");
    
    pukooa->ControlCheck = 0;
    pukooa->header.NumHeaderRec = 4;
    
    /* Initialize header values tcp with blank strings */
    for (i = 0; i < ts_tcp_len (Header_Parameters); i++)
        {
        pukooa->header.Values[i] = STRING_ALLOCATE (" ");
        strcpy (pukooa->header.Values[i], " ");
        }
    /* Standard UKOOA Format Fields */
    pukooa->fields.Columns[0][0] = 1;
    pukooa->fields.Columns[0][1] = 16;
    pukooa->fields.Columns[1][0] = 17;
    pukooa->fields.Columns[1][1] = 24;
    pukooa->fields.Columns[2][0] = 45;
    pukooa->fields.Columns[2][1] = 52;
    pukooa->fields.Columns[3][0] = 53;
    pukooa->fields.Columns[3][1] = 60;
    
    pukooa->fields.Columns[4][0] = 25;
    pukooa->fields.Columns[4][1] = 34;
    pukooa->fields.Columns[5][0] = 35;
    pukooa->fields.Columns[5][1] = 44;
    pukooa->fields.Columns[6][0] = 61;
    pukooa->fields.Columns[6][1] = 66;
    
    /* Init the Choix array for Decimal or DDDMMSS.SH lat/long format */
    pukooa->fields.Choix[0] = 1;
    pukooa->fields.Choix[1] = 1;
    pukooa->fields.Choix[2] = 1;
    pukooa->fields.Choix[3] = 1;
    pukooa->fields.Choix[4] = 0;
    pukooa->fields.Choix[5] = 0;
    pukooa->fields.Choix[6] = 0;
    
    for (i = 0; i < NUMBER_OF_DATA_FIELDS; i++)
        {
        sprintf (string, "%5d%5d", pukooa->fields.Columns[i][0],
                 pukooa->fields.Columns[i][1]);
        pukooa->fields.Values[i] = STRING_ALLOCATE (string);
        strcpy (pukooa->fields.Values[i], string);
        }
    pukooa->lines.Selection = FALSE;
    pukooa->lines.Replacement = FALSE;
    pukooa->lines.NumBogusPts = 10;
    
    /* initialize link list for seismic information */
    llinit (&pukooa->line_ll, 0, sizeof(LINE_LINK *), LL_LIST_SYSTEM);
    
    pukooa->file.RecLen = 80;
    
    return SUCCESS;
    }
/* END:     */
