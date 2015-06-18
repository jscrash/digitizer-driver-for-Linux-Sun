/* DEC/CMS REPLACEMENT HISTORY, Element SX_DLG_SIORED.C*/
/* *3    17-AUG-1990 22:21:22 VINCE "(SPR 5644) Code Cleanup"*/
/* *2    25-JUL-1989 20:11:56 GILLESPIE "(SPR 50) Seismic system revamp"*/
/* *1    19-JUN-1989 13:29:09 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element SX_DLG_SIORED.C*/
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
#include "esi_sx_err.h"
#include "esi_sx_msg.h"

/* Function Description -----------------------------------------------------
Description:
    Reader for shotpoint filter trigger dialog box.

Prototype:
    publicdef INT sx_dialog_sio_read(SX_STRUCT *psx,INT id,INT type);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    psx             -(SX_STRUCT *)
    id              -(INT)
    type            -(INT)

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT sx_dialog_sio_read (SX_STRUCT *psx, INT id, INT type)
#else
publicdef INT sx_dialog_sio_read (psx, id, type)
SX_STRUCT *psx;
INT id;
INT type;
#endif
    {
    INT status;
    INT message;
    INT *ivalue;
    INT min, max;
    INT mode;
    
    /* assume SUCCESS */
    
    status = SUCCESS;
    
    /* if all or none, no read */
    
    if (type == ITEM_LP_SET_SYMTRIG)
        {
        mode = psx->trigger.sio_sym_mode;
        ivalue = &psx->pso->disp_opt.line.shot_symbol_value;
        }
    else if (type == ITEM_LP_SET_LABTRIG)
        {
        mode = psx->trigger.sio_lab_mode;
        ivalue = &psx->pso->disp_opt.line.shot_label_value;
        }
    else if (type == ITEM_LP_SET_SHOTTRIG)
        {
        mode = psx->trigger.sio_zval_mode;
        ivalue = &psx->pso->disp_opt.line.zvalue_value;
        }
    /* if ALL or NONE just return */
    
    if (mode == ITEM_SIO_ALL OR mode == ITEM_SIO_NONE)
        {
        SX_RETURN;
        }
    if (mode == ITEM_SIO_NTH_EDIT)
        {
        min = 1;
        max = 10000;
        message = SX_INV_EVERY;
        }
    else if (mode == ITEM_SIO_ENDING_EDIT)
        {
        min = 0;
        max = 9;
        message = SX_INV_NUMBERS;
        }
    else if (mode == ITEM_SIO_DIVIS_EDIT)
        {
        min = 1;
        max = 10000;
        message = SX_INV_NUMBER2;
        }
    /* check dialog item */
    
    status = sx_check_dialog_item (id, mode, message, INT_TYPE, (BYTE *) & min,
                                   (BYTE *) & max, (BYTE *)ivalue);
    
    SX_RETURN;
    }
/* END:     */
