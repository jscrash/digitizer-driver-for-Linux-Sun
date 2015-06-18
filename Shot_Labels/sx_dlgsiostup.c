/* DEC/CMS REPLACEMENT HISTORY, Element SX_DLGSIOSTUP.C*/
/* *3    17-AUG-1990 22:20:53 VINCE "(SPR 5644) Code Cleanup"*/
/* *2    25-JUL-1989 20:11:28 GILLESPIE "(SPR 50) Seismic system revamp"*/
/* *1    19-JUN-1989 13:28:54 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element SX_DLGSIOSTUP.C*/
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
    Set up filter trigger dialog box.

Prototype:
    publicdef INT sx_dialog_sio_setup(SX_STRUCT *psx,INT id,BOOL updatez);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    psx             -(SX_STRUCT *)
    id              -(INT) Dialog box id.
    updatez         -(BOOL) Whether or not to update z value.

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT sx_dialog_sio_setup (SX_STRUCT *psx, INT id, BOOL updatez)
#else
publicdef INT sx_dialog_sio_setup (psx, id, updatez)
SX_STRUCT *psx;
INT id;
BOOL updatez;
#endif
    {
    INT status;
    INT radio_control1 = OFF;       /* radio button on off settings */
    INT radio_control2 = OFF;
    INT radio_control3 = OFF;
    INT radio_control4 = OFF;
    INT radio_control5 = OFF;
    INT enable_control1 = OFF;      /* box enable on off settings */
    INT enable_control2 = OFF;
    INT enable_control3 = OFF;
    CHAR text[128];                 /* set up text strings */
    CHAR cval;                      /* local trigger values */
    INT ival;
    INT text_item;
    
    /* assume success */
    
    status = SUCCESS;
    
    /* set up dialog box */
    /* which type of trigger */
    
    if (id == psx->cdlg_id.dialog_sio_sym)
        {
        cval = *psx->pso->disp_opt.line.shot_symbol_trigger;
        ival = psx->pso->disp_opt.line.shot_symbol_value;
        }
    else if (id == psx->cdlg_id.dialog_sio_lab)
        {
        cval = *psx->pso->disp_opt.line.shot_label_trigger;
        ival = psx->pso->disp_opt.line.shot_label_value;
        }
    else if (id == psx->cdlg_id.dialog_sio_zval)
        {
        cval = *psx->pso->disp_opt.line.zvalue_trigger;
        ival = psx->pso->disp_opt.line.zvalue_value;
        }
    /* determine type */
    
    if (cval == 'N' AND ival > 1)
        {
        radio_control1 = ON;
        text_item = ITEM_SIO_NTH_EDIT;
        enable_control1 = ON;
        sxz_mode (psx, id, ITEM_SIO_NTH_EDIT);
        }
    else if (cval == 'E')
        {
        radio_control2 = ON;
        text_item = ITEM_SIO_ENDING_EDIT;
        enable_control2 = ON;
        sxz_mode (psx, id, ITEM_SIO_ENDING_EDIT);
        }
    else if (cval == 'M')
        {
        radio_control3 = ON;
        text_item = ITEM_SIO_DIVIS_EDIT;
        enable_control3 = ON;
        sxz_mode (psx, id, ITEM_SIO_DIVIS_EDIT);
        }
    else if (cval == 'N' AND ival == 1)
        {
        radio_control4 = ON;
        sxz_mode (psx, id, ITEM_SIO_ALL);
        }
    else if (cval == 'N' AND ival == 0)
        {
        radio_control5 = ON;
        sxz_mode (psx, id, ITEM_SIO_NONE);
        }
    wiscdv (id, ITEM_SIO_EVERY, radio_control1);
    wiencdi (id, ITEM_SIO_NTH_EDIT, enable_control1);
    wiscdv (id, ITEM_SIO_NUMBERS, radio_control2);
    wiencdi (id, ITEM_SIO_ENDING_EDIT, enable_control2);
    wiscdv (id, ITEM_SIO_NUMBER2, radio_control3);
    wiencdi (id, ITEM_SIO_DIVIS_EDIT, enable_control3);
    wiscdv (id, ITEM_SIO_ALL, radio_control4);
    wiscdv (id, ITEM_SIO_NONE, radio_control5);
    
    /* print trigger value to text */
    
    if (updatez)
        {
        text[0] = '\0';
        wiscdt (id, ITEM_SIO_ENDING_EDIT, text);
        wiscdt (id, ITEM_SIO_NTH_EDIT, text);
        wiscdt (id, ITEM_SIO_DIVIS_EDIT, text);
        sprintf (text, "%d", ival);
        wiscdt (id, text_item, text);
        }
    SX_RETURN;
    }
/* END:     */
