/* DEC/CMS REPLACEMENT HISTORY, Element XS_SAV_TOPS.C*/
/* *5    17-JUL-1991 13:44:03 JESSIE "(SPR 7811) fix save tops fail, case# 2110"*/
/* *4    28-SEP-1990 11:57:39 GILLESPIE "(SPR 1) Merge ernie deltas"*/
/*  1B1  28-SEP-1990 11:12:01 GILLESPIE "Beta Deltas"*/
/* *3    27-AUG-1990 14:11:00 JULIAN "(SPR 0) bug fixes from ernie"*/
/* *2    17-AUG-1990 22:43:54 VINCE "(SPR 5644) Code Cleanup"*/
/* *1    19-JUN-1989 13:45:36 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element XS_SAV_TOPS.C*/
/* DEC/CMS REPLACEMENT HISTORY, Element XS_SAV_TOPS.C                               */
/* *3    27-AUG-1990 14:11:00 JULIAN "(SPR 0) bug fixes from ernie"                  */
/* *2    17-AUG-1990 22:43:54 VINCE "(SPR 5644) Code Cleanup"                       */
/* *1    19-JUN-1989 13:45:36 SYSTEM ""                                             */
/* DEC/CMS REPLACEMENT HISTORY, Element XS_SAV_TOPS.C                               */
/************************************************************************************/
/******************************************************************************     */
/*                                                                                  */
/*                Copyright Exploration Systems, Inc. 1989                          */
/*                    Unpublished -- All rights reserved                            */
/*                                                                                  */
/*THIS SOFTWARE IS THE PROPRIETARY PROPERTY OF Exploration Systems, Inc.  AND MAY   */
/*CONTAIN CONFIDENTIAL TRADE SECRET INFORMATION. IT IS LICENSED FOR USE ON THE      */
/*DESIGNATED EQUIPMENT ON WHICH IT WAS ORIGINALLY INSTALLED AND  MAY NOT BE         */
/*MODIFIED, DUPLICATED OR COPIED IN ANY FORM WITHOUT PRIOR WRITTEN CONSENT OF       */
/*                                                                                  */
/*                            Exploration Systems                                   */
/*                            c/o Finder Graphics Systems                           */
/*                            201 Tamal Vista Blvd.                                 */
/*                            Corte Madera, CA  94925                               */
/*                            (415) 927-0100                                        */
/*                                                                                  */
/*(The above notice does not constitute an admission of publication or              */
/*unrestricted dissemination of the work)                                           */
/*                                                                                  */
/******************************************************************************     */

#include "esi_gl_defs.h"
#include "esi_xs_defs.h"

#ifndef ESI_AM_H
#include "esi_am.h"
#endif

#ifndef ESI_MG_H
#include "esi_mg.h"
#endif

#ifndef ESI_TS_H
#include "esi_ts.h"
#endif

#ifndef ESI_TC_H
#include "esi_tc.h"
#endif

#ifndef ESI_WI_H
#include "esi_wi.h"
#endif

#ifndef ESI_FI_H
#include "esi_fi.h"
#endif

#include "xs_cdlgs.rh"
#include "xs_menus.rh"

/* Function Description -----------------------------------------------------       */
/*Description:                                                                      */
/*    This module includes initial and server routines for operating                */
/*    tops.                                                                         */
/*                                                                                  */
/*    This routine to define the save tops dialog box.                              */
/*                                                                                  */
/*Prototype:                                                                        */
/*    publicdef INT xs_sav_tops_init(XS_STRUCT *pxs);                               */
/*                                                                                  */
/*Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)        */
/*    (M) pxs         -(XS_STRUCT *) A pointer points to the XS_STRUCT.             */
/*                                                                                  */
/*Return Value/Status:                                                              */
/*    SUCCESS - Successful completion.                                              */
/*    FAIL    - Unsuccessful completion.                                            */
/*                                                                                  */
/*Scope:                                                                            */
/*    PUBLIC                                                                        */
/*                                                                                  */
/*Limitations/Assumptions:                                                          */
/*                                                                                  */
/*-----------------------------------------------------------------------------     */

/* BEGIN:                                                                           */
#if USE_PROTOTYPES
publicdef INT xs_sav_tops_init (XS_STRUCT *pxs)
#else
publicdef INT xs_sav_tops_init (pxs)
XS_STRUCT *pxs;
#endif
    {
    INT status = SUCCESS;
    INT id = 0, item = 0, class  = 0;
    INT num_items = 0;
    INT *selected_list;
    static TCP tops;
    /* **************************************************************************   */
    /* defines the dialog resource.                                                 */
    wienmni (pxs->tops_menu_id, SAVE_TOPS_MENU_ITEM, OFF);
    status = xsz_make_tops_tcp (pxs, &tops);
    status = fi_push ();
    status = am_define_resource (AM_DIALOG, XS_CDLGS_FILE, XS_SAVE_TOPS,
                                 NULL_FUNCTION_PTR, &id, NULL);
    
    pxs->save_top_cdlg_id = id;
    /* set the texts to the selecting box.                                          */
    
    num_items = ts_tcp_len (tops);
    wi_set_dialog_text (id, XS_SAV_MESSAGE,
                        "Select one or more tops to save and click 'Save'");
    wisselt (id, XS_SAV_SELECT_LIST, XS_SAV_SELECT_BAR, num_items, num_items, tops);
    
    status = wiopcd (id);
    if (status == SUCCESS)          /* Did the dialog open correctly?               */
        {
        for (status = FAIL; status != SUCCESS; )
            {                       /* Request events until a valid one.            */
            status = wirqevt (0, &class , &id, &item);
            if (id == pxs->save_top_cdlg_id)
                {
                status = SUCCESS;   /* Got one from the correct dialog.             */
                switch (item)
                    {
                case XS_SAV_BTN_SAVE:
                    /* allocates the selected_list for                              */
                    /* retruned items.                                              */
                    
                    selected_list = (INT *) tc_alloc (sizeof(INT) * (num_items + 1));
                    wiqsel (id, XS_SAV_SELECT_LIST, &num_items, selected_list);
                    if (num_items < 1)
                        {
                        wi_set_dialog_text (id, XS_SAV_MESSAGE, "No top is selected");
			status = FAIL;
                        break;
                        }
                    status = xsz_save_tops (pxs, num_items, selected_list);
                    if (status != SUCCESS)
                        {
                        am_message (AM_ALERT, mg_message (status));
                        break;
                        }
                    tc_free (selected_list);
                    ts_tcp_free (tops);
                    
                    pxs->changed_since_display = TRUE;
                    pxs->tops_changed_since_save = FALSE;
                    /* Fall on thru to close dialog.                                */
                case XS_SAV_BTN_CANCEL:
                    am_release_resource (pxs->save_top_cdlg_id);
                    xs_cs_menu_mgr (pxs);
                    wienmni (pxs->tops_menu_id, SAVE_TOPS_MENU_ITEM, ON);
                    status = SUCCESS;
                    break;
                    
                default:
                    status = FAIL;  /* Unknown event, try again.                    */
                    break;
                    }
                }
            else
                {
                wibeep (1);
                }
            }
        }
    status = fi_pop ();
    return status;
    }
/* END:                                                                             */
