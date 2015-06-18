/* DEC/CMS REPLACEMENT HISTORY, Element MO_STRAT_MOD.C*/
/* *3    17-AUG-1990 22:02:23 VINCE "(SPR 5644) Code Cleanup"*/
/* *2     3-NOV-1989 15:09:13 GILLESPIE "(SPR 1) Changes to keep in sync with mo_struct resource"*/
/* *1    19-JUN-1989 13:09:36 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element MO_STRAT_MOD.C*/
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

#include "esi_mo.h"
#include "mo_strat.rh"
#include "esi_am.h"
#include "esi_c_lib.h"
#include "esi_wi.h"
#include "esi_mg.h"
#include "esi_mo_msg.h"

/* Function Description -----------------------------------------------------
Description:
    This routine is the top level routine for strat modelling.  

Prototype:
    publicdef INT mo_strat_model(BYTE *param);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    param           -(BYTE *)

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT mo_strat_model (BYTE *param)
#else
publicdef INT mo_strat_model (param)
BYTE *param;
#endif
    {
    CHAR text[STMAX];
    STRAT_MODEL_STRUCT *pModel;
    INT status;
    static FILENAME resource_file = "mo_strat";
    /* ************************************************************************** */
    
    
    /* call routine to initialize modelling structure, nlists, etc.               */
    
    status = am_declare ("Strat Model.");
    
    status = mo_setup (&pModel);
    
    if (status != SUCCESS)
        {
        return FAIL;
        }
    /* Set flags in model structure off     */
    pModel->generate_model_onoff = OFF;
    pModel->mo_data_entry_onoff = OFF;
    pModel->mo_digitize_onoff = OFF;
    pModel->mo_tieplot_onoff = OFF;
    pModel->generate_traces_onoff = OFF;
    pModel->mo_traces_onoff = OFF;
    
    am_message (AM_DIALOG, mg_message (MO_DEFINING_RESOURCES));
    
    /* define resource servers */
    
    am_define_resource (AM_MENU, resource_file, MO_MENU, mo_main_menu_server,
                        &pModel->main_menu_id, pModel);
    
    /* if Emp. model can't change type  */
    if (pModel->model_type == PURE_EMPIRICAL)
        {
        wi_enable_menu_item (pModel->main_menu_id, MO_CHANGE_TYPE, OFF);
        }
    else
        {
        wi_enable_menu_item (pModel->main_menu_id, MO_CHANGE_TYPE, ON);
        }
    wi_enable_menu_item (pModel->main_menu_id, GENERATE_MODEL, OFF);
    wi_enable_menu_item (pModel->main_menu_id, MO_DATA_ENTER, OFF);
    wi_enable_menu_item (pModel->main_menu_id, MO_DIGITIZE, OFF);
    wi_enable_menu_item (pModel->main_menu_id, MO_DRAW_TIEPLOT, OFF);
    wi_enable_menu_item (pModel->main_menu_id, GENERATE_TRACES, OFF);
    wi_enable_menu_item (pModel->main_menu_id, MO_TRACES, OFF);
    
    am_define_resource (AM_DIALOG, resource_file, MO_DIALOG, mo_main_dialog_server,
                        &pModel->main_dialog_id, pModel);
    
    wi_open_dialog (pModel->main_dialog_id);
    wivismn (pModel->main_menu_id, ON);
    
    /* Enable File Menu Items          */
    wienfmi (pModel->main_dialog_id, WI_FILE_MENU_NEW, ON);
    wienfmi (pModel->main_dialog_id, WI_FILE_MENU_OPEN, ON);
    wienfmi (pModel->main_dialog_id, WI_FILE_MENU_SAVE, ON);
    wienfmi (pModel->main_dialog_id, WI_FILE_MENU_DELETE, ON);
    
    pModel->need_to_save = FALSE;
    
    wiscdt (pModel->main_dialog_id, MO_TEXT, mg_message (MO_PROGRAM_TITLE));
    
    return SUCCESS;
    }
/* END:     */
