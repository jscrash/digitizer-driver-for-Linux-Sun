/* DEC/CMS REPLACEMENT HISTORY, Element SX_DLGCLPSRVR.C*/
/* *6    17-AUG-1990 22:20:43 VINCE "(SPR 5644) Code Cleanup"*/
/* *5     2-APR-1990 10:32:15 KEE "(SPR -1) remove sx_dialog_clp_ok at case ITEM_CLP_SUPPRESS"*/
/* *4    26-MAR-1990 11:59:51 KEE "(SPR -1) Fixing bug of new (sx_cdlgs-131) dialog "*/
/* *3    23-MAR-1990 23:54:01 KEE "(SPR -1) change sx application for new dialog (Select the Best Label Position)"*/
/* *2    25-JUL-1989 20:11:07 GILLESPIE "(SPR 50) Seismic system revamp"*/
/* *1    19-JUN-1989 13:28:44 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element SX_DLGCLPSRVR.C*/
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
#include "sx_cdlgs.rh"
#include "sx_menus.rh"
#include "esi_wi.h"

#ifndef ESI_AM_H

#include "esi_am.h"

#endif

#if USE_PROTOTYPES
static INT sx_dialog_clp_ok (SX_CLP_STRUCT *pclp, INT id);
#else
static INT sx_dialog_clp_ok ();
#endif

/* File Description ---------------------------------------------------------
Overview:

Prototypes and one line descriptions of each function in the file.
Public_Functions:
    publicdef INT sx_dialog_clp_srvr(INT id,INT item,SX_CLP_STRUCT *pclp);

Private_Functions:
    static INT sx_dialog_clp_ok(SX_CLP_STRUCT *pclp, INT id);

-----------------------------------------------------------------------------*/

/* Function Description -----------------------------------------------------
Description:
    Server for label position dialog box.
    Modified by J. R. Breedlove 12/9/87 to accomodate                      
    new design for HELP and CANCEL                                          

Prototype:
    static INT sx_dialog_clp_ok(SX_CLP_STRUCT *pclp,INT id);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    pclp            -(SX_CLP_STRUCT *)
    id              -(INT)

Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PRIVATE to <sx_dlgclpsrvr.c>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static INT sx_dialog_clp_ok (SX_CLP_STRUCT *pclp, INT id)
#else
static INT sx_dialog_clp_ok (pclp, id)
SX_CLP_STRUCT *pclp;
INT id;
#endif
    {
    switch (pclp->menu_item)
        {
        
    case ITEM_LP_SET_DEFPOS:
        sx_dialog_clp_lsdterm (pclp);
        break;
        
    case ITEM_LP_CHANGE_INDIVIDUAL:
        sx_dialog_clp_citerm (pclp);
        break;
        
    case ITEM_LP_CHANGE_LINE:
        sx_dialog_clp_clterm (pclp);
        break;
        
    default:
        break;
        }
    wienmni (pclp->psx->menu_id.lp, pclp->menu_item, ON);
    am_free (pclp);
    am_release_resource (id);
    return(SUCCESS);
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:

Prototype:
    publicdef INT sx_dialog_clp_srvr(INT id,INT item,SX_CLP_STRUCT *pclp);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    id              -(INT)
    item            -(INT)
    pclp            -(SX_CLP_STRUCT *)

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT sx_dialog_clp_srvr (INT id, INT item, SX_CLP_STRUCT *pclp)
#else
publicdef INT sx_dialog_clp_srvr (id, item, pclp)
INT id;
INT item;
SX_CLP_STRUCT *pclp;
#endif
    {
    INT status;
    INT iang;
    
    status = SUCCESS;
    
    /* process dialog box event */
    
    switch (item)
        {
        /*      case ITEM_CLP_HELP:     JRB 12/9/87     */
        /*      break;                  JRB 12/9/87     */
        
    case ITEM_CLP_CANCEL:
        am_release_resource (id);
        wienmni (pclp->psx->menu_id.lp, pclp->menu_item, ON);
        
        /*      status = FAIL;          JRB 12/9/87     */
        break;
        
    case ITEM_CLP_SUPPRESS:
        if (pclp->paraperp == SO_NOPARA)
            {
            pclp->paraperp = 0;
            sx_dialog_clp_setup (pclp, id);
            }
        else
            {
            pclp->paraperp = SO_NOPARA;
            }
        break;
        
    case ITEM_CLP_CENTERED_RIGHT:
        pclp->angle = 0.0;
        sx_dialog_clp_setup (pclp, id);
        sx_dialog_clp_ok (pclp, id);
        break;
        
    case ITEM_CLP_ABOVE_RIGHT:
        pclp->angle = 45.0;
        sx_dialog_clp_setup (pclp, id);
        sx_dialog_clp_ok (pclp, id);
        break;
        
    case ITEM_CLP_ABOVE_CENTERED:
        pclp->angle = 90.0;
        sx_dialog_clp_setup (pclp, id);
        sx_dialog_clp_ok (pclp, id);
        break;
        
    case ITEM_CLP_ABOVE_LEFT:
        pclp->angle = 135.0;
        sx_dialog_clp_setup (pclp, id);
        sx_dialog_clp_ok (pclp, id);
        break;
        
    case ITEM_CLP_CENTERED_LEFT:
        pclp->angle = 180.0;
        sx_dialog_clp_setup (pclp, id);
        sx_dialog_clp_ok (pclp, id);
        break;
        
    case ITEM_CLP_BELOW_LEFT:
        pclp->angle = 225.0;
        sx_dialog_clp_setup (pclp, id);
        sx_dialog_clp_ok (pclp, id);
        break;
        
    case ITEM_CLP_BELOW_CENTERED:
        pclp->angle = 270.0;
        sx_dialog_clp_setup (pclp, id);
        sx_dialog_clp_ok (pclp, id);
        break;
        
    case ITEM_CLP_BELOW_RIGHT:
        pclp->angle = 315.0;
        sx_dialog_clp_setup (pclp, id);
        sx_dialog_clp_ok (pclp, id);
        break;
        
    case ITEM_CLP_VERTICAL:
        iang = pclp->angle + 0.001;
        if (iang == 90 || iang == 270)
            {
            pclp->paraperp = SO_PERP;
            }
        else
            {
            pclp->paraperp = SO_PARA;
            }
        break;
        
    case ITEM_CLP_HORIZONTAL:
        iang = pclp->angle + 0.001;
        if (iang == 0 || iang == 180)
            {
            pclp->paraperp = SO_PERP;
            }
        else
            {
            pclp->paraperp = SO_PARA;
            }
        break;
        
    case ITEM_CLP_ANGLED:
        break;
        
    default:
        break;
        
        }
    SX_RETURN;
    }
/* END:     */
