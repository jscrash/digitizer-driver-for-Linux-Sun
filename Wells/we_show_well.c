/* DEC/CMS REPLACEMENT HISTORY, Element WE_SHOW_WELL.C*/
/* *5    13-AUG-1991 11:34:21 JESSIE "(SPR 8909) Added else for enabling Display Header btn after 1st instance"*/
/* *4    14-AUG-1990 13:39:37 VINCE "(SPR 5644) Header Standardization"*/
/* *3     6-MAR-1990 16:21:45 WALTERS "(SPR 0) Show well name instead of UWI"*/
/* *2    17-JAN-1990 22:56:21 GILLESPIE "(SPR 6001) Changes for new well struct mods"*/
/* *1    19-JUN-1989 13:35:24 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element WE_SHOW_WELL.C*/
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
#include "esi_we_wells.h"
#include "esi_wi.h"
#include "esi_am.h"
#include "esi_we_msg.h"

#ifndef ESI_TS_H

#include "esi_ts.h"

#endif

#ifndef ESI_MG_H

#include "esi_mg.h"

#endif

#include "we_sho_well.rh"

#define WE_SHOW_WELL_CDLG_FILE "we_sho_well"
static INT we_show_well_cdlg_init ();
static INT we_show_well_cdlg_server ();
static INT we_show_well_bad_cdlg_init ();
static INT we_show_well_bad_cdlg_server ();
static INT we_slt_uwi_server ();

/* File Description ---------------------------------------------------------
Overview:
    Display well header information to the operator.

Prototypes and one line descriptions of each function in the file.
Public_Functions:
    extern INT we_show_wells_server(CHAR **tcp_uwi, CHAR **tcp_name);
    
    publicdef INT we_show_well_server(UWI  uwi);
    
Private_Functions:
    static INT we_slt_uwi_server(INT id, INT item, CHAR **tcp);
    
    static INT we_show_well_bad_cdlg_server(INT  id,INT  item,CHAR *junk);
    
    static INT we_show_well_cdlg_server(INT  id,INT  item,CHAR *junk);

    static INT we_show_well_cdlg_init(WELL_STRUCT *pwell);
    
-----------------------------------------------------------------------------*/

/* Function Description -----------------------------------------------------
Description:
    See above.
    
Prototype:
    publicdef INT we_show_well_server(UWI  uwi);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    uwi             -(UWI)

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT we_show_well_server (UWI uwi) 
#else
publicdef INT we_show_well_server (uwi)
UWI uwi;
#endif
    {
    INT status;
    WELL_STRUCT *pwell;
    
    status = we_get_well_header (uwi, WELL_HDR_1_DETAIL, &pwell);
    if (status IS_NOT_EQUAL_TO SUCCESS)
        status = we_show_well_bad_cdlg_init (uwi);
    else
        status = we_show_well_cdlg_init (pwell);
    return SUCCESS;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    Put up the custom dialog box to display information from the well     
    retrieved by we_get_well_header.                                      

Prototype:
    static INT we_show_well_cdlg_init(WELL_STRUCT *pwell);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    pwell           -(WELL_STRUCT *)

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PRIVATE to <we_show_well_server>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static INT we_show_well_cdlg_init (WELL_STRUCT *pwell) 
#else
static INT we_show_well_cdlg_init (pwell)
WELL_STRUCT *pwell;
#endif
    {
    INT status;
    INT id;
    CHAR text[16];
    CHAR junk[2];
    WELL_HDR_1 *pwell1 = (WELL_HDR_1 *)pwell->detail.ptr;
    
    am_define_resource (AM_DIALOG, WE_SHOW_WELL_CDLG_FILE, WE_SHOW_WELL_SELECTION,
                        we_show_well_cdlg_server, &id, junk);
    
    /* Load the text fields.      */
    
    status = wiscdt (id, WE_SHOW_UWI_FIELD, pwell->uwi);
    status = wiscdt (id, WE_SHOW_OPER_FIELD, pwell1->operator);
    status = wiscdt (id, WE_SHOW_NAME_FIELD, pwell1->name);
    status = wiscdt (id, WE_SHOW_NUMB_FIELD, pwell1->number);
    sprintf (text, "%8.2f %s", pwell1->elevation.value, pwell1->elevation.ref);
    status = wiscdt (id, WE_SHOW_ELEV_FIELD, text);
    
    /* Make the CDLG visible.     */
    
    status = wi_open_dialog (id);
    
    return status;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    Get events from the custom dialog box and invoke the                  
    appropriate functions.                                                

Prototype:
    static INT we_show_well_cdlg_server(INT  id,INT  item,CHAR *junk);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    id              -(INT)
    item            -(INT)
    junk            -(CHAR *)

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PRIVATE to <we_show_well_server>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static INT we_show_well_cdlg_server (INT id, INT item, CHAR *junk) 
#else
static INT we_show_well_cdlg_server (id, item, junk)
INT id;
INT item;
CHAR *junk;
#endif
    {
    INT status;
    status = SUCCESS;
    
    switch (item)
        {                           /* Dispatch event to process. */
    case WE_SHOW_OK:
        status = am_release_resource (id);
        break;
    default:
        break;
        }
    return status;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    Put up the custom dialog box to display information from the well     
    retrieved by we_get_well_header.                                      

Prototype:
    static INT we_show_well_bad_cdlg_init(UWI uwi);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    uwi             -(UWI)

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PRIVATE to <we_show_well_server>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static INT we_show_well_bad_cdlg_init (UWI uwi) 
#else
static INT we_show_well_bad_cdlg_init (uwi)
UWI uwi;
#endif
    {
    INT status;
    INT id;
    CHAR text[16];
    CHAR junk[2];
    
    am_define_resource (AM_DIALOG, WE_SHOW_WELL_CDLG_FILE, WE_SHOW_WELL_BAD_SELECTION,
                        we_show_well_bad_cdlg_server, &id, junk);
    
    /* load the text fields.      */
    
    status = wiscdt (id, WE_SHOW_BAD_UWI_FIELD, uwi);
    
    /* make the cdlg visible.     */
    
    status = wi_open_dialog (id);
    
    return status;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    Get events from the custom dialog box and invoke the                  
    appropriate functions.                                                

Prototype:
    static INT we_show_well_bad_cdlg_server(INT  id,INT  item,CHAR *junk);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    id              -(INT)
    item            -(INT)
    junk            -(CHAR *)

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PRIVATE to <we_show_well_server>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static INT we_show_well_bad_cdlg_server (INT id, INT item, CHAR *junk) 
#else
static INT we_show_well_bad_cdlg_server (id, item, junk)
INT id;
INT item;
CHAR *junk;
#endif
    {
    INT status;
    status = SUCCESS;
    
    switch (item)
        {                           /* dispatch event to process. */
    case WE_SHOW_BAD_OK:
        status = am_release_resource (id);
        break;
    default:
        break;
        }
    return status;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    The excutive server routine of selecting UWI.

Prototype:
    static INT we_slt_uwi_server(INT id, INT item, CHAR **tcp);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) id          -(INT) The id of active CDLG.
    (I) item        -(INT) The item picked in the active CDLG.
    (I) tcp         -(CHAR **) The TCP pointer.

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PRIVATE to <we_show_well_server>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static INT we_slt_uwi_server (INT id, INT item, CHAR **tcp) 
#else
static INT we_slt_uwi_server (id, item, tcp)
CHAR **tcp;
INT id;
INT item;
#endif
    {
    INT status = SUCCESS;
    INT item_selected[4];
    INT total_items;
    INT i;
    
    /* create the table of returned items*/
    
    total_items = ts_tcp_len (tcp);
    /* analyze the item.                 */
    
    switch (item)
        {
    case WE_SLT_BTN_INFO:
        wiqsel (id, WE_SLT_SELECT_LIST, &total_items, item_selected);
        if (total_items < 1)
            {
            am_message (AM_ALERT, mg_message (WE_SHO_WELL_SEL_WELL));
            break;
            }
        we_show_well_server (tcp[item_selected[0] - 1]);
        break;
    case WE_SLT_BTN_CANCEL:
        status = am_release_resource (id);
        break;
        
    default:
        break;
        
        }
    return status;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    This routine to allow user to display the information of
    several wells.

Prototype:
    extern INT we_show_wells_server(CHAR **tcp_uwi, CHAR **tcp_name);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) tcp_name    -(CHAR **) A tcp table of well names.
    (I) tcp_uwi     -(CHAR **) A tcp table of UWI.

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PRIVATE to <we_show_well_server>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
extern INT we_show_wells_server (CHAR **tcp_uwi, CHAR **tcp_name) 
#else
extern INT we_show_wells_server (tcp_uwi, tcp_name)
CHAR **tcp_uwi;
CHAR **tcp_name;
#endif
    {
    INT status = SUCCESS;
    INT id;
    INT num_items = 0;
    
    /* ************************************************************************** */
    
    /* defines the dialog resource.       */
    status = am_define_resource (AM_DIALOG, WE_SHOW_WELL_CDLG_FILE, WE_SELECT_UWI,
                                 we_slt_uwi_server, &id, tcp_uwi);
    
    num_items = ts_tcp_len (tcp_name);
    
    wisselt (id, WE_SLT_SELECT_LIST, WE_SLT_SELECT_BAR, num_items, 1, tcp_name);
    /*Added else to turn Display Header Button back on after being off*/
    if (num_items < 1)
        {
        wiencdi (id, WE_SLT_BTN_INFO, OFF);
        wiscdt (id, WE_SLT_PROMPT, mg_message (WE_EMPTY_LIST));
        }
    else
        wiencdi (id, WE_SLT_BTN_INFO, ON);
    status = wi_open_dialog (id);
    
    return status;
    }
/* END:     */
