/*  DEC/CMS REPLACEMENT HISTORY, Element MP_ST_MPDF_BT.C*/
/*  *3    17-AUG-1990 22:10:12 VINCE "(SPR 5644) Code Cleanup"*/
/*  *2    26-JUN-1989 11:00:59 GILLESPIE "(SPR 20) Fix castings"*/
/*  *1    19-JUN-1989 13:15:02 SYSTEM ""*/
/*  DEC/CMS REPLACEMENT HISTORY, Element MP_ST_MPDF_BT.C*/
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
#include "esi_gl_defs.h"
#include "esi_md_defs.h"
#include "esi_mp.h"
#include "esi_am.h"
#include "md_mapdef.rh"
BOOL saved_status[6];

/* File Description ---------------------------------------------------------
Overview:
    Routine to set the state (on/off) of the button items
    on the map definition dialog.

Prototypes and one line descriptions of each function in the file.
Public_Functions:
    publicdef INT mp_set_mapdef_buttons();

    publicdef INT mp_init_mapdef_buttons();

Private_Functions:

-----------------------------------------------------------------------------*/

/* Function Description -----------------------------------------------------
Description:
    Routine to set the state (on/off) of the button items
    on the map definition dialog.

Prototype:
    publicdef INT mp_set_mapdef_buttons();

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    None.
    
Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT mp_set_mapdef_buttons ()
#else
publicdef INT mp_set_mapdef_buttons ()
#endif
    {
    INT i;
    RESOURCE_ID *cdlg_id;
    MD_STATE_WORKSPACE *md_state;
    
    PROGNAME ("MP_SET_MAPDEF_BUTTONS");
    INT status;
    
    /* ********************************************************************** */
    
    status = SUCCESS;
    
    am_open_workspace ("CDLG", AM_APPLICATION, (VOIDPTR *) & cdlg_id);
    am_open_workspace ("MAPDEF_STATE", AM_GLOBAL, (VOIDPTR *) & md_state);
    
    for (i = 0; i < 6; i++)
        {
        if (md_state->button_status[i] != saved_status[i])
            {
            saved_status[i] = md_state->button_status[i];
            
            switch (i)
                {
            case 0:
                wi_enable_dialog_item (cdlg_id->overlays_id, MD_OVER_ADD_BTN,
                                       md_state->button_status[0]);
                break;
                
            case 1:
                wi_enable_dialog_item (cdlg_id->overlays_id, MD_KEY_ADD_BTN,
                                       md_state->button_status[1]);
                break;
                
            case 2:
                wi_enable_dialog_item (cdlg_id->overlays_id, MD_PARM_ADD_BTN,
                                       md_state->button_status[2]);
                wi_enable_dialog_item (cdlg_id->overlays_id, MD_PARM_CANCEL,
                                       md_state->button_status[2]);
                break;
                
            case 3:
                wi_enable_dialog_item (cdlg_id->overlays_id, MD_OVER_DELETE_BTN,
                                       md_state->button_status[3]);
                break;
                
            case 4:
                wi_enable_dialog_item (cdlg_id->overlays_id, MD_KEY_DELETE_BTN,
                                       md_state->button_status[4]);
                break;
                
            case 5:
                wi_enable_dialog_item (cdlg_id->overlays_id, MD_PARM_EXPAND_BTN,
                                       md_state->button_status[5]);
                break;
                
            default:
                break;
                }
            }
        }
    return status;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    Initialization function.

Prototype:
    publicdef INT mp_init_mapdef_buttons();
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    None.
    
Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT mp_init_mapdef_buttons ()
#else
publicdef INT mp_init_mapdef_buttons ()
#endif
    {
    static BOOL init_array[6] = 
        {
        -1,
        -1,
        -1,
        -1,
        -1,
        -1
        };
    
    hoblockmove (init_array, saved_status, 6 * sizeof(BOOL));
    return SUCCESS;
    }
/* END:     */
