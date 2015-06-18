/* DEC/CMS REPLACEMENT HISTORY, Element SXZ_MODE.C*/
/* *3    17-AUG-1990 22:19:30 VINCE "(SPR 5644) Code Cleanup"*/
/* *2    25-JUL-1989 20:09:05 GILLESPIE "(SPR 50) Seismic system revamp"*/
/* *1    19-JUN-1989 13:27:55 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element SXZ_MODE.C*/
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

/* Function Description -----------------------------------------------------
Description:
    Set sio dialog button item mode.

Prototype:
    publicdef INT sxz_mode(SX_STRUCT *psx,INT id,INT item);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    psx             -(SX_STRUCT *)
    id              -(INT)
    item            -(INT)

Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT sxz_mode (SX_STRUCT *psx, INT id, INT item)
#else
publicdef INT sxz_mode (psx, id, item)
SX_STRUCT *psx;
INT id;
INT item;
#endif
    {
    INT status;
    
    /* assume SUCCESS */
    
    status = SUCCESS;
    
    /* determine type from id and set mode */
    
    if (id == psx->cdlg_id.dialog_sio_sym)
        {
        psx->trigger.sio_sym_mode = item;
        }
    else if (id == psx->cdlg_id.dialog_sio_lab)
        {
        psx->trigger.sio_lab_mode = item;
        }
    else if (id == psx->cdlg_id.dialog_sio_zval)
        {
        psx->trigger.sio_zval_mode = item;
        }
    }
/* END:     */
