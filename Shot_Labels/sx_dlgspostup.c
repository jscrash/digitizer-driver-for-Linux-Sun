/* DEC/CMS REPLACEMENT HISTORY, Element SX_DLGSPOSTUP.C*/
/* *4    17-AUG-1990 22:21:17 VINCE "(SPR 5644) Code Cleanup"*/
/* *3    20-SEP-1989 16:08:14 GORDON "(SPR 200) get in sync with Gulf"*/
/* *2    25-JUL-1989 20:11:42 GILLESPIE "(SPR 50) Seismic system revamp"*/
/* *1    19-JUN-1989 13:29:00 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element SX_DLGSPOSTUP.C*/
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
#include "esi_wi.h"

/* Function Description -----------------------------------------------------
Description:
    Set up shot point add dialog box.

Prototype:
    publicdef INT sx_dialog_spo_setup(SX_STRUCT *psx);

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
publicdef INT sx_dialog_spo_setup (SX_STRUCT *psx)
#else
publicdef INT sx_dialog_spo_setup (psx)
SX_STRUCT *psx;
#endif
    {
    INT status;
    INT id;                         /* dialog id */
    INT radio_control1;             /* radio button on off settings */
    INT radio_control2;
    INT enable_control;             /* box enable on off settings */
    CHAR text[128];                 /* set up text strings */
    
    /* assume success */
    
    status = SUCCESS;
    
    /* set up dialog box */
    
    id = psx->cdlg_id.dialog_spo;
    
    /* shot label type not implemented */
    
    wiencdi (id, ITEM_SPO_SHOT_LABELS_ARBITRARY, OFF);
    
    /* set up label type */
    
    switch (psx->dialog_spo.shot_label_type)
        {
        
    case SX_NUMERIC:
        radio_control1 = ON;
        radio_control2 = OFF;
        break;
        
    case SX_ARBITRARY:
        radio_control1 = OFF;
        radio_control2 = ON;
        break;
        
    default:
        psx->dialog_spo.shot_label_type = SX_NUMERIC;
        radio_control1 = ON;
        radio_control2 = OFF;
        break;
        }
    wiscdv (id, ITEM_SPO_SHOT_LABELS_NUMERIC, radio_control1);
    wiscdv (id, ITEM_SPO_SHOT_LABELS_ARBITRARY, radio_control2);
    
    /* auto or manual */
    
    switch (psx->dialog_spo.add_type)
        {
        
    case SX_AUTO:
        radio_control1 = ON;
        radio_control2 = OFF;
        enable_control = ON;
        break;
        
    case SX_MANUAL:
        radio_control1 = OFF;
        radio_control2 = ON;
        enable_control = OFF;
        break;
        
    default:
        psx->dialog_spo.add_type = SX_AUTO;
        radio_control1 = ON;
        radio_control2 = OFF;
        enable_control = ON;
        break;
        }
    wiscdv (id, ITEM_SPO_LABEL_ENTRY_AUTO, radio_control1);
    wiscdv (id, ITEM_SPO_LABEL_ENTRY_MANUAL, radio_control2);
    
    /* enable or disenable other items accordingly */
    
    wiencdi (id, ITEM_SPO_START_EDIT, enable_control);
    wiencdi (id, ITEM_SPO_INCREMENT_EDIT, enable_control);
    wiencdi (id, ITEM_SPO_LABEL_ORDER_ASCEND, enable_control);
    wiencdi (id, ITEM_SPO_LABEL_ORDER_DESCEND, enable_control);
    wiencdi (id, ITEM_SPO_SHOT_SPAC_MANUAL, enable_control);
    wiencdi (id, ITEM_SPO_SHOT_SPAC_AUTO, enable_control);
    wiencdi (id, ITEM_SPO_SHOT_SPAC_EDIT, enable_control);
    
    /* set up shot label entry text */
    
    wiscdt (id, ITEM_SPO_START_EDIT, "");
    sprintf (text, "%6.1f", psx->dialog_spo.shot_incr);
    wiscdt (id, ITEM_SPO_INCREMENT_EDIT, text);
    
    /* set ascend, descend buttons */
    
    switch (psx->dialog_spo.shot_incr_factor)
        {
        
    case SX_ASCEND:
        radio_control1 = ON;
        radio_control2 = OFF;
        break;
        
    case SX_DESCEND:
        radio_control1 = OFF;
        radio_control2 = ON;
        break;
        
    default:
        psx->dialog_spo.shot_incr_factor = SX_ASCEND;
        radio_control1 = ON;
        radio_control2 = OFF;
        break;
        }
    wiscdv (id, ITEM_SPO_LABEL_ORDER_ASCEND, radio_control1);
    wiscdv (id, ITEM_SPO_LABEL_ORDER_DESCEND, radio_control2);
    
    /* auto entry shot spacing method */
    
    wiscdt (id, ITEM_SPO_SHOT_SPAC_EDIT, "");
    
    switch (psx->dialog_spo.shot_spacing_method)
        {
        
    case SX_MANUAL:
        radio_control1 = ON;
        radio_control2 = OFF;
        wiencdi (id, ITEM_SPO_SHOT_SPAC_EDIT, OFF);
        break;
        
    case SX_AUTO:
        radio_control1 = OFF;
        radio_control2 = ON;
        break;
        
    default:
        psx->dialog_spo.shot_spacing_method = SX_MANUAL;
        radio_control1 = ON;
        radio_control2 = OFF;
        wiencdi (id, ITEM_SPO_SHOT_SPAC_EDIT, OFF);
        break;
        }
    wiscdv (id, ITEM_SPO_SHOT_SPAC_MANUAL, radio_control1);
    wiscdv (id, ITEM_SPO_SHOT_SPAC_AUTO, radio_control2);
    
    status = SUCCESS;
    
    SX_RETURN;
    }
/* END:     */
