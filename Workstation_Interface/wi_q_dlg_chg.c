/* DEC/CMS REPLACEMENT HISTORY, Element WI_Q_DLG_CHG.C*/
/* *7    23-OCT-1990 13:56:09 KEE "(SPR -1) Motif and DecWindow Merge"*/
/* *6    27-JUN-1990 17:30:45 KEE "(SPR -1) New wi data structure using 2-D array"*/
/* *5    12-APR-1990 21:01:33 JULIAN "(SPR -1) changed ansiDwt to ansidwt"*/
/* *4    15-MAR-1990 13:36:40 KEE "(SPR -1) Change idverify->text stucture to be dynamic alloc "*/
/* *3     2-MAR-1990 11:24:08 WALTERS "(SPR 6012) Fix #ifndef USE_X to #ifdef USE_X"*/
/* *2     5-FEB-1990 23:40:12 JULIAN "(SPR -1) X version of WI Module (Kee)"*/
/* *1    19-JUN-1989 13:38:08 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element WI_Q_DLG_CHG.C*/
/* DEC/CMS REPLACEMENT HISTORY, Element WI_Q_DLG_CHG.C                              */
/* *6    27-JUN-1990 17:30:45 KEE "(SPR -1) New wi data structure using 2-D array"  */
/* *5    12-APR-1990 21:01:33 JULIAN "(SPR -1) changed ansiDwt to ansidwt"          */
/* *4    15-MAR-1990 13:36:40 KEE "(SPR -1) Change idverify->text stucture to be dynamic alloc "    */
/* *3     2-MAR-1990 11:24:08 WALTERS "(SPR 6012) Fix #ifndef USE_X to #ifdef USE_X"*/
/* *2     5-FEB-1990 23:40:12 JULIAN "(SPR -1) X version of WI Module (Kee)"        */
/* *1    19-JUN-1989 13:38:08 SYSTEM ""                                             */
/* DEC/CMS REPLACEMENT HISTORY, Element WI_Q_DLG_CHG.C                              */
/******************************************************************************     */
/*                                                                                  */
/*                Copyright Finder Graphics Systems, Inc. 1989                      */
/*           Unpublished -- All rights reserved                                     */
/*                                                                                  */
/*THIS SOFTWARE IS THE PROPRIETARY PROPERTY OF Finder Graphics Systems, Inc.  AND   */
/*MAY CONTAIN CONFIDENTIAL TRADE SECRET INFORMATION. IT IS LICENSED FOR USE ON THE  */
/*DESIGNATED EQUIPMENT ON WHICH IT WAS ORIGINALLY INSTALLED AND  MAY NOT BE         */
/*MODIFIED, DUPLICATED OR COPIED IN ANY FORM WITHOUT PRIOR WRITTEN CONSENT OF       */
/*                                                                                  */
/*            Finder Graphics Systems, Inc.                                         */
/*            201 Tamal Vista Blvd                                                  */
/*            Corte Madera, CA  USA 94925                                           */
/*            (415) 927-0100                                                        */
/*                                                                                  */
/*(The above notice does not constitute an admission of publication or              */
/*unrestricted dissemination of the work)                                           */
/*                                                                                  */
/******************************************************************************     */


/*************************************************************************          */
/*                                                                                  */
/*NAME:    WI_QUERY_DIALOG_CHANGES                                                  */
/*                                                                                  */
/*DESCRIPTION:                                                                      */
/*                                                                                  */
/*    Ask for a damage report on a custom dialog.  This will tell                   */
/*    which items have been modified but not reported.  These will                  */
/*    usually be edit text items.                                                   */
/*                                                                                  */
/*    Input:                                                                        */
/*                                                                                  */
/*         cdlgid   = (INT) custom dialog ID.                                       */
/*         maxitems = (INT) maximum number of items, room in array.                 */
/*                                                                                  */
/*    OutPut:                                                                       */
/*                                                                                  */
/*         numitems = (INT) number of items damaged in custom dialog.               */
/*         items    = (INT ARRAY) a list of the item ids.                           */
/*                                                                                  */
/*************************************************************************          */

#include "esi_wi_int.x"

#ifdef USE_X
#ifndef ESI_X_H
#include "esi_x.h"
#endif
#include "ansixt.h"
#ifndef ESI_TS_H
#include "esi_ts.h"
#endif
#ifndef ESI_TC_H
#include "esi_tc.h"
#endif
#endif

/************************************************************************************/
#define IDENT progname
publicdef INT wi_query_dialog_changes (cdlgid, maxitems, numitems, items)   /*************************************************************************/

INT cdlgid;
INT maxitems;
INT *numitems;
INT items[MAX_CD_ITEMS];
    {
    PROGNAME ("wi_query_dialog_changes");
    INT i;
    INT status;

#ifdef USE_X
    INT items_count = 0;
    CHAR *str;
    Widget itemwidget;
    WIDGET_ITEM_INFO *iteminfo;
    
    /********************************************************************************/
    
    E_CHECK_INIT;
    E_CHECK_ID (cdlgid, 0, WI_DIALOG);
    E_CHECK_WIDGET (cdlgid, 0);
    
    wiz_jp_name (IDENT);
    
    /* I check all the funcions that call wi_query_dialog_change,                   */
    /*   only the content of the edittext is being referenced - Kee                 */
    /* wi_query_dialog_change should be only referenced by edittext                 */
    
    for (i = 1; i <= (E_DIALOG_MENU (cdlgid))->num_items; i++)
        {
        E_CHECK_WIDGET (cdlgid, i);
        iteminfo = E_WIDGET_INFO (cdlgid, i);
        itemwidget = E_GET_WIDGET (cdlgid, i);
        
        switch (iteminfo->type_id)
            {
        case EDITTEXT:
#ifdef MOTIF  /* ---------- BEGIN MOTIF BLOCK ---------- */
            str = XmTextGetString (itemwidget);
#else  /* ---------- END MOTIF BLOCK - BEGIN DECW BLOCK ---------- */
            str = DwtSTextGetString (itemwidget);
#endif  /* ---------- END DECW BLOCK ---------- */
            if (ARE_DIFFERENT (str, iteminfo->text))
                /* compare original text with text content in edittext field        */
                {
                items[items_count++] = i;
                if (IS_EMPTY_STRING (iteminfo->text) == FALSE)
                    tc_free (iteminfo->text);
                iteminfo->text = STRING_ALLOCATE (str);
                strcpy (iteminfo->text, str);
                }
            XtFree (str);
            break;
        default:
            break;
            }
        }
    *numitems = items_count;
    
    if (*numitems > maxitems)
        {
        return(WI_INSUFFICIENT_SIZE);
        }
    else
        {
        return(SUCCESS);
        }
#else        /* ESITERM code */
    E_CHECK_INIT;
    E_CHECK_ID (cdlgid, 0, WI_DIALOG);
    
    wiz_jp_name (IDENT);
    
    /* Issue request, query current state.                                          */
    
    wiv.playback_nosend = 1;
    SEND_2_VALUES (cdlgid, MC_DAMAGE_REPORT_CD);
    wiv.playback_nosend = 0;
    
    status = SUCCESS;
    
    /* Pick up report, count followed by the itemids.                               */
    wiz_set_echo (FALSE);
    if (WAIT_RESPONSE (DRCD_CODE))
        {
        wiz_gnum (numitems);
        
        /* Range check number of items.                                             */
        if (*numitems > maxitems)
            {
            status = WI_INSUFFICIENT_SIZE;
            }
        /* Pick up array elements.                                                  */
        for (i = 0; i < *numitems; i++)
            {
            wiz_gnum (items++);
            }
        wiz_acknowledge ();
        }
    wiz_set_echo (TRUE);
#endif 
    }
