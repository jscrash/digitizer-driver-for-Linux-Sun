/* DEC/CMS REPLACEMENT HISTORY, Element LD_UK_FIELDS.C*/
/* *5    14-AUG-1990 13:33:37 VINCE "(SPR 5644) Header Standardization"*/
/* *4     8-AUG-1989 10:50:57 ADEY "(SPR -1) fix bug in dddmmss.s processing"*/
/* *3     4-AUG-1989 11:06:58 ADEY "(SPR -1) CHANGE NAMES"*/
/* *2    25-JUL-1989 16:48:16 ADEY "(SPR -1) Improve User Interface & decode lat.long for seismic"*/
/* *1    19-JUN-1989 13:03:34 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element LD_UK_FIELDS.C*/
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
#include "esi_am_err.h"
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
#include "ld_uk_lines.rh"
#include "ld_uk_header.rh"
#include "ld_uk_fields.rh"

/* FUNCTION DECLARATIONS */

publicdef INT ukooa_fields_server ();
static INT ukooa_fields_parameter_definit ();
static INT ukooa_fields_edittext_reader ();
static INT ukooa_fields_update_selector ();

/* File Description ---------------------------------------------------------
Overview:
    Ukooa fields server
    START-HISTORY :
    12/12/86 KIMMAN   first time checkin
    12/12/86 GEORGE DOBEK/KIMMAN  Template constructed
    END-HISTORY
    
Prototypes and one line descriptions of each function in the file.
Public_Functions:
    publicdef INT ukooa_fields_server(INT id, INT item, UKOOA_WORKSPACE *pukooa);
    
Private_Functions:
    static INT ukooa_fields_parameter_definit(UKOOA_WORKSPACE *pukooa);
    
    static INT ukooa_fields_edittext_reader(UKOOA_WORKSPACE *pukooa);
    
    static INT ukooa_fields_update_selector(UKOOA_WORKSPACE *pukooa);
    
-----------------------------------------------------------------------------*/

/* Function Description -----------------------------------------------------
Description:
    Data fields analysis custom dialog server.

Prototype:
    publicdef INT ukooa_fields_server(INT id, INT item, UKOOA_WORKSPACE *pukooa);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) id          -(INT) Dialog id of the UKOOA data format analysis
                        custom dialog assigned, by the am_define_resource service.
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
publicdef INT ukooa_fields_server (INT id, INT item, UKOOA_WORKSPACE *pukooa) 
#else
publicdef INT ukooa_fields_server (id, item, pukooa)
INT id;
INT item;
UKOOA_WORKSPACE *pukooa;
#endif
    {
    
    switch (item)
        {
    case UKOOA_FIELDS_Enter_Btn:
        /* Get parameter from selector box */
        if (ukooa_fields_parameter_definit (pukooa) == FAIL)
            break;
        
        /* Get value(s) from edittext boxes */
        else if (ukooa_fields_edittext_reader (pukooa) == FAIL)
            break;
        
        /* Update Values selector box */
        else if (ukooa_fields_update_selector (pukooa) == FAIL)
            break;
        
        break;
        
    case UKOOA_FIELDS_Cancel_Btn:
        wiclcd (pukooa->fields.dialog_id);
        wiopcd (pukooa->main_dialog_id);
        break;
        
    case UKOOA_FIELDS_Done_Btn:
        wiclcd (pukooa->fields.dialog_id);
        wiopcd (pukooa->main_dialog_id);
        break;
        
    case UKOOA_FIELDS_dddmmss_ChkBx:
        if (pukooa->fields.Choix[4] == 1)
            {
            pukooa->fields.Choix[4] = 2;
            pukooa->fields.Choix[5] = 2;
            }
        else if (pukooa->fields.Choix[4] == 2)
            {
            pukooa->fields.Choix[4] = 1;
            pukooa->fields.Choix[5] = 1;
            }
        break;
        
    default:
        break;
        }
    return SUCCESS;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    Parameters selection definition and validation.

Prototype:
    static INT ukooa_fields_parameter_definit(UKOOA_WORKSPACE *pukooa);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (M) pukooa      -(UKOOA_WORKSPACE *) Pointer to the application workspace,
                        which was defined in the calling routine.

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL - Unsuccessful completion.
    
Scope:
    PRIVATE to <ukooa_fields_server>
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static INT ukooa_fields_parameter_definit (UKOOA_WORKSPACE *pukooa) 
#else
static INT ukooa_fields_parameter_definit (pukooa)
UKOOA_WORKSPACE *pukooa;
#endif
    {
    INT status = SUCCESS, NumItems;
    INT Parameter_Selected[2];
    CHAR message[STMAX];
    
    /* Which field has been selected? */
    wiqsel (pukooa->fields.dialog_id, UKOOA_FIELDS_Parameters_Select, &NumItems,
            Parameter_Selected);
    
    if (NumItems EQUALS 0)
        {
        strcpy (message, "Please select a data field parameter.");
        wiscdt (pukooa->fields.dialog_id, UKOOA_FIELDS_Status_Stat, message);
        status = FAIL;
        }
    else
        pukooa->fields.Selection = Parameter_Selected[0];
    
    return status;
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
    static INT ukooa_fields_edittext_reader(UKOOA_WORKSPACE *pukooa);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (M) pukooa      -(UKOOA_WORKSPACE *) Pointer to the application workspace,
                        which was defined in the calling routine.

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL - Unsuccessfule completion.
    
Scope:
    PRIVATE to <ukooa_fields_server>
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static INT ukooa_fields_edittext_reader (UKOOA_WORKSPACE *pukooa) 
#else
static INT ukooa_fields_edittext_reader (pukooa)
UKOOA_WORKSPACE *pukooa;
#endif
    {
    CHAR text[STMAX], message[STMAX];
    INT value, numrec;
    FLOAT xnumrec;
    
    /* Has UKOOA_FIELDS_StartingColumn_Edit changed value? */
    wiqccdi (pukooa->fields.dialog_id, UKOOA_FIELDS_StartingColumn_Ed, &value, text);
    
    if (ts_isfloat (text) > 0)
        {
        xnumrec = atof (text);
        numrec = xnumrec;
        if ((fabs (numrec - xnumrec) > 0.0001) || (xnumrec < 0.0))
            {
            sprintf (message, mg_message (LD_UK_INVALID_SCOL), text);
            wiscdt (pukooa->fields.dialog_id, UKOOA_FIELDS_Status_Stat, message);
            
            return FAIL;
            }
        else
            {
            pukooa->fields.StartingColumn = numrec;
            sprintf (message, "%d", numrec);
            wiscdt (pukooa->fields.dialog_id, UKOOA_FIELDS_StartingColumn_Ed, message);
            }
        }
    else
        {
        sprintf (message, mg_message (LD_UK_INVALID_SCOL), text);
        wiscdt (pukooa->fields.dialog_id, UKOOA_FIELDS_Status_Stat, message);
        
        return FAIL;
        }
    /* Has UKOOA_FIELDS_EndingColumn_Edit changed value? */
    wiqccdi (pukooa->fields.dialog_id, UKOOA_FIELDS_EndingColumn_Edit, &value, text);
    
    if (ts_isfloat (text) > 0)
        {
        xnumrec = atof (text);
        numrec = xnumrec;
        if ((fabs (numrec - xnumrec) > 0.0001) || (xnumrec < 0.0))
            {
            sprintf (message, mg_message (LD_UK_INVALID_ECOL), text);
            wiscdt (pukooa->fields.dialog_id, UKOOA_FIELDS_Status_Stat, message);
            
            return FAIL;
            }
        else
            {
            pukooa->fields.EndingColumn = numrec;
            sprintf (message, "%d", numrec);
            wiscdt (pukooa->fields.dialog_id, UKOOA_FIELDS_EndingColumn_Edit, message);
            }
        }
    else
        {
        sprintf (message, mg_message (LD_UK_INVALID_ECOL), text);
        wiscdt (pukooa->fields.dialog_id, UKOOA_FIELDS_Status_Stat, message);
        
        return FAIL;
        }
    return SUCCESS;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    Assign validated starting and ending column numbers. Validated
    column numbers are copied to the position in the field column
    numbers selector box corresponding to the field parameter
    selected earlier. The column numbers are assigned to their
    workspace variables, as well.

Prototype:
    static INT ukooa_fields_update_selector(UKOOA_WORKSPACE *pukooa);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (M) pukooa      -(UKOOA_WORKSPACE *) Pointer to the application workspace,
                        which was defined in the calling routine.

Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PRIVATE to <ukooa_fields_server>
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static INT ukooa_fields_update_selector (UKOOA_WORKSPACE *pukooa) 
#else
static INT ukooa_fields_update_selector (pukooa)
UKOOA_WORKSPACE *pukooa;
#endif
    {
    CHAR text[STMAX];
    
    /* Convert starting and ending column numbers into a text string. */
    sprintf (text, "%5d %5d", pukooa->fields.StartingColumn,
             pukooa->fields.EndingColumn);
    
    /* Replace selected parameters value with edittext values */
    wi_delete_cd_line (pukooa->fields.dialog_id, UKOOA_FIELDS_Values_Selector,
                       pukooa->fields.Selection);
    wi_insert_cd_line (pukooa->fields.dialog_id, UKOOA_FIELDS_Values_Selector,
                       pukooa->fields.Selection, text);
    
    /* Store values */
    pukooa->fields.Values[pukooa->fields.Selection - 1] = STRING_ALLOCATE (text);
    strcpy (pukooa->fields.Values[pukooa->fields.Selection - 1], text);
    
    /* Store columns */
    pukooa->fields.Columns[pukooa->fields.Selection -
                                                              1][0] =
                                                              pukooa->fields.
                                                              StartingColumn;
    pukooa->fields.Columns[pukooa->fields.Selection -
                                                              1][1] =
                                                              pukooa->fields.
                                                              EndingColumn;
    
    return SUCCESS;
    }
/* END:     */
