/* DEC/CMS REPLACEMENT HISTORY, Element SX_LP_MN_SRVR.C*/
/* *3    17-AUG-1990 22:21:57 VINCE "(SPR 5644) Code Cleanup"*/
/* *2    25-JUL-1989 20:13:00 GILLESPIE "(SPR 50) Seismic system revamp"*/
/* *1    19-JUN-1989 13:29:38 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element SX_LP_MN_SRVR.C*/
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

#include "esi_sx.h"
#include "sx_menus.rh"

/* Function Description -----------------------------------------------------
Description:
    Label position menu server.

Prototype:
    publicdef INT sx_lp_menu_srvr(INT menuid,INT item,SX_STRUCT *psx);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    menuid          -(INT)
    item            -(INT)
    psx             -(SX_STRUCT *)

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT sx_lp_menu_srvr (INT menuid, INT item, SX_STRUCT *psx)
#else
publicdef INT sx_lp_menu_srvr (menuid, item, psx)
INT menuid;
INT item;
SX_STRUCT *psx;
#endif
    {
    INT status = SUCCESS;
    
    switch (item)
        {
        
    case ITEM_LP_AUTO_ALL_LABELS:
        sx_auto_shot_labels (psx);
        sx_auto_line_labels (psx);
        break;
        
    case ITEM_LP_AUTO_SHOT_LABELS:
        sx_auto_shot_labels (psx);
        break;
        
    case ITEM_LP_AUTO_LINE_LABELS:
        sx_auto_line_labels (psx);
        break;
        
    case ITEM_LP_SET_DEFPOS:
        sx_set_def_label_positions (psx);
        break;
        
    case ITEM_LP_SET_SYMTRIG:
    case ITEM_LP_SET_LABTRIG:
    case ITEM_LP_SET_SHOTTRIG:
        sx_set_trigger (psx, item);
        break;
        
    case ITEM_LP_CHANGE_INDIVIDUAL:
        sx_set_1_label_position (psx);
        break;
        
    case ITEM_LP_CHANGE_LINE:
        sx_set_line_label_positions (psx);
        break;
        
    case ITEM_LP_DISPLAY_DATA_LABELS:
        sx_zzzz (psx);
        break;
        
    default:
        break;
        }
    SX_RETURN;
    }
/* END:     */
