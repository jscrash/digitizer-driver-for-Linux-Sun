/*  DEC/CMS REPLACEMENT HISTORY, Element MP_ST_TXSZ_BT.C*/
/*  *3    17-AUG-1990 22:10:20 VINCE "(SPR 5644) Code Cleanup"*/
/*  *2    26-JUN-1989 11:01:07 GILLESPIE "(SPR 20) Fix castings"*/
/*  *1    19-JUN-1989 13:15:05 SYSTEM ""*/
/*  DEC/CMS REPLACEMENT HISTORY, Element MP_ST_TXSZ_BT.C*/
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
#include "esi_mp.h"
#include "esi_am.h"
#include "esi_wi.h"
#include "esi_tc.h"
#include "md_mapdef.rh"

/* Function Description -----------------------------------------------------
Description:
    Function to set the text size radio buttons on 
    the TITLE BLOCK dialog.

Prototype:
    publicdef INT mp_set_text_size_btns(INT btn_num);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    btn_num         -(INT)

Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT mp_set_text_size_btns (INT btn_num)
#else
publicdef INT mp_set_text_size_btns (btn_num)
INT btn_num;
#endif
    {
    PROGNAME ("MP_SET_TEXT_SIZE_BTNS");
    RESOURCE_ID *cdlg_id;
    BOOL on_off[5];
    
    /****************************************************************************/
    
    am_open_workspace ("CDLG", AM_APPLICATION, (VOIDPTR *) & cdlg_id);
    
    tc_zeroes (on_off, 5 * sizeof(BOOL));
    on_off[btn_num - 1] = ON;
    
    wi_set_dialog_value (cdlg_id->title_block_id, MD_TITLE_SIZE_1, on_off[0]);
    wi_set_dialog_value (cdlg_id->title_block_id, MD_TITLE_SIZE_2, on_off[1]);
    wi_set_dialog_value (cdlg_id->title_block_id, MD_TITLE_SIZE_3, on_off[2]);
    wi_set_dialog_value (cdlg_id->title_block_id, MD_TITLE_SIZE_4, on_off[3]);
    wi_set_dialog_value (cdlg_id->title_block_id, MD_TITLE_SIZE_5, on_off[4]);
    
    return SUCCESS;
    }
/* END:     */
