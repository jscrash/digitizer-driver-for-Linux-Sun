/* DEC/CMS REPLACEMENT HISTORY, Element LD_UK_HEADER.C*/
/* *4    14-AUG-1990 13:33:55 VINCE "(SPR 5644) Header Standardization"*/
/* *3     4-AUG-1989 11:07:08 ADEY "(SPR -1) CHANGE NAMES"*/
/* *2    25-JUL-1989 16:50:17 ADEY "(SPR -1) Improve User Interface & decode lat/long and survey/line for seismic"*/
/* *1    19-JUN-1989 13:03:37 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element LD_UK_HEADER.C*/
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
#include "esi_tc.h"
#include "esi_ts.h"
#include "esi_wi.h"
#include "uk_work.h"
#include "esi_math.h"

#ifndef ESI_LD_MSG_H

#include "esi_ld_msg.h"

#endif

#ifndef ESI_LD_ERR_H

#include "esi_ld_err.h"

#endif

#ifndef ESI_MG_H

#include "esi_mg.h"

#endif

#include "ld_uk_main.rh"
#include "ld_uk_fields.rh"
#include "ld_uk_lines.rh"
#include "ld_uk_header.rh"

/* FUNCTION DECLARATIONS */

publicdef INT ukooa_header_server ();
static INT ukooa_header_parameter_reader ();
static INT ukooa_header_edittext_reader ();

/* File Description ---------------------------------------------------------
Overview:
    Ukooa header server.
    START-HISTORY :
    12/12/86 KIMMAN   first time checkin
    12/12/86 GEORGE DOBEK/KIMMAN  Template constructed
    END-HISTORY 

Prototypes and one line descriptions of each function in the file.
Public_Functions:
    publicdef INT ukooa_header_server(INT id, INT item, UKOOA_WORKSPACE *pukooa);
    
Private_Functions:
    static INT ukooa_header_parameter_reader(UKOOA_WORKSPACE *pukooa);
    
    static INT ukooa_header_edittext_reader(UKOOA_WORKSPACE *pukooa);

-----------------------------------------------------------------------------*/

/* Function Description -----------------------------------------------------
Description:
    Header records decoding custom dialog server.

Prototype:
    publicdef INT ukooa_header_server(INT id, INT item, UKOOA_WORKSPACE *pukooa);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) id          -(INT) Dialog id of the UKOOA header decoding custom dialog,
                        assigned by the am_define_resource service.
    (I) item        -(INT) Resource item to process.
    (M) pukooa      -(UKOOA_WORKSPACE *) Pointer to the application workspace,
                        which was defined in the calling routine.

Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT ukooa_header_server (INT id, INT item, UKOOA_WORKSPACE *pukooa) 
#else
publicdef INT ukooa_header_server (id, item, pukooa)
INT id;
INT item;
UKOOA_WORKSPACE *pukooa;
#endif
    {
    switch (item)
        {
    case UKOOA_HEADER_Enter_Btn:
        ukooa_header_parameter_reader (pukooa);
        break;
        
    case UKOOA_HEADER_Surv_Ln_ChkBx:
        if (pukooa->fields.Choix[0] == 1)
            pukooa->fields.Choix[0] = 2;
        else
            pukooa->fields.Choix[0] = 1;
        break;
        
    case UKOOA_HEADER_Cancel_Btn:
        wiclcd (pukooa->header.dialog_id);
        wiopcd (pukooa->main_dialog_id);
        break;
        
    case UKOOA_HEADER_Done_Btn:
        
        if (ukooa_header_edittext_reader (pukooa) == SUCCESS)
            {
            wiclcd (pukooa->header.dialog_id);
            wiopcd (pukooa->main_dialog_id);
            }
        break;
        }
    return SUCCESS;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    Header parameter definition and validation.

Prototype:
    static INT ukooa_header_parameter_reader(UKOOA_WORKSPACE *pukooa);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (M) pukooa      -(UKOOA_WORKSPACE *) Pointer to the application workspace,
                        which was defined in the calling routine.

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL - Unsuccessful completion.
    
Scope:
    PRIVATE to <ukooa_header_server>
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static INT ukooa_header_parameter_reader (UKOOA_WORKSPACE *pukooa) 
#else
static INT ukooa_header_parameter_reader (pukooa)
UKOOA_WORKSPACE *pukooa;
#endif
    {
    INT NumItems, Parameter_Selected[2], Start;
    CHAR message[STMAX];
    CHAR text[STMAX];
    
    /* Which header parameter is being assign a value? */
    wiqsel (pukooa->header.dialog_id, UKOOA_HEADER_Parameters_Select, &NumItems,
            Parameter_Selected);
    
    if (NumItems EQUALS 0)
        {
        strcpy (message, "Please select a header parameter.");
        wiscdt (pukooa->header.dialog_id, UKOOA_HEADER_Status_Stat, message);
        
        return FAIL;
        }
    /* What text has been painted? */
    wi_qedit_selection (pukooa->header.dialog_id, UKOOA_HEADER_Records, &Start, text);
    
    if (strlen (text) EQUALS 0)
        {
        strcpy (message, "Please paint a value for parameter.");
        wiscdt (pukooa->header.dialog_id, UKOOA_HEADER_Status_Stat, message);
        return FAIL;
        }
    /* Replace selected parameters value with painted text */
    wi_delete_cd_line (pukooa->header.dialog_id, UKOOA_HEADER_Values_Selector,
                       Parameter_Selected[0]);
    wi_insert_cd_line (pukooa->header.dialog_id, UKOOA_HEADER_Values_Selector,
                       Parameter_Selected[0], text);
    
    /* Store value in workspace tcp for header values. */
    pukooa->header.Values[Parameter_Selected[0] - 1] = STRING_ALLOCATE (text);
    strcpy (pukooa->header.Values[Parameter_Selected[0] - 1], text);
    
    /* if Parameter_Selected[0] == 1, then this is the Survey
    if Parameter_Selected[0] == 2, then this is the ProjCode */
    if (Parameter_Selected[0] == 1)
        strcpy (pukooa->header.Survey, text);
    else if (Parameter_Selected[0] == 2)
        {
        strcpy (pukooa->header.ProjCode, text);
        /*
                        IF GEODETIC PROJECTION TURN OFF EASTING/NORTHING
                                               TURN ON  LAT/LONG FOR DECIMAL
        */
        if (strcmp (ts_sto_upper (text), "GEODETIC") == 0)
            {
            pukooa->fields.Choix[2] = 0;
            pukooa->fields.Choix[3] = 0;
            pukooa->fields.Choix[4] = 1;
            pukooa->fields.Choix[5] = 1;
            }
        else
            {
            pukooa->fields.Choix[2] = 1;
            pukooa->fields.Choix[3] = 1;
            pukooa->fields.Choix[4] = 0;
            pukooa->fields.Choix[5] = 0;
            }
        }
    return SUCCESS;
    
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    Determines how many edittext fields were "damaged". These
    fields are read, validated, converted and assigned to
    corresponding workspace variables.

    If an invalid field entry is detected, the field's control
    number is noted in the workspace. An alert box informs the
    operator of the specific problem. Finally, the application
    immediately returns to allow the operator the opportunity
    of correcting the offending field entry.

Prototype:
    static INT ukooa_header_edittext_reader(UKOOA_WORKSPACE *pukooa);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (M) pukooa      -(UKOOA_WORKSPACE *) Pointer to the application workspace,
                        which was defined in the calling routine.

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL - Unsuccessful completion.
    
Scope:
    PRIVATE to <ukooa_header_server>
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static INT ukooa_header_edittext_reader (UKOOA_WORKSPACE *pukooa) 
#else
static INT ukooa_header_edittext_reader (pukooa)
UKOOA_WORKSPACE *pukooa;
#endif
    {
    INT value, numrec;
    FLOAT xnumrec;
    CHAR text[STMAX], message[STMAX];
    
    /* Has UKOOA_HEADER_NumHeaderRec_Edit changed value? */
    wiqccdi (pukooa->header.dialog_id, UKOOA_HEADER_NumHeaderRec_Edit, &value, text);
    
    if (ts_isfloat (text) > 0)
        {
        xnumrec = atof (text);
        numrec = xnumrec;
        if ((fabs (numrec - xnumrec) > 0.0001) || (xnumrec < 0.0))
            {
            sprintf (message, mg_message (LD_UK_INVALID_HEADER), text);
            wiscdt (pukooa->header.dialog_id, UKOOA_HEADER_Status_Stat, message);
            
            return FAIL;
            }
        else
            {
            pukooa->header.NumHeaderRec = numrec;
            sprintf (message, "%d", numrec);
            wiscdt (pukooa->header.dialog_id, UKOOA_HEADER_NumHeaderRec_Edit, message);
            }
        }
    else
        {
        sprintf (message, mg_message (LD_UK_INVALID_HEADER), text);
        wiscdt (pukooa->header.dialog_id, UKOOA_HEADER_Status_Stat, message);
        
        return FAIL;
        }
    /*       Encode Separator Char into Choix[0] parameter        */
    
    if (pukooa->fields.Choix[0] == 2)
        {
        wiqccdi (pukooa->header.dialog_id, UKOOA_HEADER_Surv_Ln_Sep_Edit, &value,
                 text);
        pukooa->fields.Choix[0] = (INT)text[0];
        }
    return SUCCESS;
    }
/* END:     */
