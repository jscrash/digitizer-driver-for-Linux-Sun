/*  DEC/CMS REPLACEMENT HISTORY, Element MP_MAP_SERVER.C*/
/*  *2    17-AUG-1990 22:07:03 VINCE "(SPR 5644) Code Cleanup"*/
/*  *1    19-JUN-1989 13:13:07 SYSTEM ""*/
/*  DEC/CMS REPLACEMENT HISTORY, Element MP_MAP_SERVER.C*/
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

#ifndef ESI_MP_H

#include "esi_mp.h"

#endif

#ifndef ESI_WI_H

#include "esi_wi.h"

#endif

#include "md_mapdef.rh"

/* Function Description -----------------------------------------------------
Description:
    Services events from the map overlay definition dialog.

Prototype:
    publicdef INT mp_map_server(INT id,INT item,BYTE *p);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    id              -(INT)
    item            -(INT)
    p               -(BYTE *)

Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT mp_map_server (INT id, INT item, BYTE *p)
#else
publicdef INT mp_map_server (id, item, p)
INT id;
INT item;
BYTE *p;
#endif
    {
    
    /**********************************************************************/
    
    /* * BLANK THE MESSAGE LINE * */
    /* -------------------------- */
    
    wi_set_dialog_text (id, MD_OVER_MSG, "");
    
    /* * PROCESS EVENTS * */
    /* ------------------ */
    
    switch (item)
        {
        /* OVERLAY ITEMS */
    case MD_OVER_SELECT:
        mp_overlay_selected (id);
        break;
        
    case MD_OVER_ADD_BTN:
        mp_add_overlay (id);
        break;
        
    case MD_OVER_PREFS:
        mp_pref_overlay_selected (id);
        break;
        
    case MD_OVER_DELETE_BTN:
        mp_delete_overlay (id);
        break;
        
        /* KEYWORD ITEMS */
    case MD_KEY_SELECT:
        mp_keyword_selected (id);
        break;
        
    case MD_KEY_ADD_BTN:
        mp_add_keyword (id);
        break;
        
    case MD_KEY_PREFS:
        mp_pref_keyword_selected (id);
        break;
        
    case MD_KEY_DELETE_BTN:
        mp_delete_keyword (id);
        break;
        
        /* PARAMETER ITEMS */
    case MD_PARM_PREFS:
        mp_pref_parm_selected (id);
        break;
        
    case MD_PARM_EXPAND_BTN:
        mp_edit_parameter (id);
        break;
        
        
    default:
        break;
        }
    return SUCCESS;
    }
/* END:     */
