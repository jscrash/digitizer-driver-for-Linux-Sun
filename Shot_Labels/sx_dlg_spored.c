/* DEC/CMS REPLACEMENT HISTORY, Element SX_DLG_SPORED.C*/
/* *4    17-AUG-1990 22:21:27 VINCE "(SPR 5644) Code Cleanup"*/
/* *3    14-SEP-1989 18:34:35 GORDON "(SPR -1) checkin from gulf"*/
/* *2    25-JUL-1989 20:12:02 GILLESPIE "(SPR 50) Seismic system revamp"*/
/* *1    19-JUN-1989 13:29:15 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element SX_DLG_SPORED.C*/
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
#include "esi_sx_err.h"

/* Function Description -----------------------------------------------------
Description:
    Read and verify edit text in shotpoint entry dialog.

Prototype:
    publicdef INT sx_dialog_spo_read(SX_STRUCT *psx);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
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
publicdef INT sx_dialog_spo_read (SX_STRUCT *psx)
#else
publicdef INT sx_dialog_spo_read (psx)
SX_STRUCT *psx;
#endif
    {
    INT status = SUCCESS;
    DOUBLE fmin, fmax;
    
    /* if all manual, return */
    
    if (psx->dialog_spo.add_type == SX_MANUAL)
        {
        SX_RETURN;
        }
    /* check start shotpoint */
    
    status = sx_check_dialog_item (psx->cdlg_id.dialog_spo, ITEM_SPO_START_EDIT,
                                   SX_BAD_START_SHOTPOINT, FLOAT_TYPE, (BYTE *)0,
                                   (BYTE *)0, (BYTE *) & psx->dialog_spo.start_shot);
    if (status == SUCCESS)
        {
        psx->dialog_spo.current_shot = psx->dialog_spo.start_shot;
        }
    /* check shotpoint increment */
    
    fmin = 1.0;
    fmax = 10000.0;
    status = status | sx_check_dialog_item (psx->cdlg_id.dialog_spo,
                                            ITEM_SPO_INCREMENT_EDIT, SX_BAD_INCREMENT,
                                            FLOAT_TYPE, (BYTE *) & fmin,
                                            (BYTE *) & fmax,
                                            (BYTE *) & psx->dialog_spo.shot_incr);
    
    /* check auto spacing type, if necessary */
    
    if (psx->dialog_spo.shot_spacing_method == SX_AUTO)
        {
        status = status | sx_check_dialog_item (psx->cdlg_id.dialog_spo,
                                                ITEM_SPO_SHOT_SPAC_EDIT,
                                                SX_BAD_SHOT_SPAC, FLOAT_TYPE,
                                                (BYTE *) & fmin, (BYTE *) & fmax,
                                                (BYTE *) &
                                                psx->dialog_spo.shot_spacing);
        }
    SX_RETURN;
    }
/* END:     */
