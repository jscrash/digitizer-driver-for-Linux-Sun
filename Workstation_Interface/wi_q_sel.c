/* DEC/CMS REPLACEMENT HISTORY, Element WI_Q_SEL.C*/
/* *5    23-OCT-1990 13:56:24 KEE "(SPR -1) Motif and DecWindow Merge"*/
/* *4    27-JUN-1990 17:31:05 KEE "(SPR -1) New wi data structure using 2-D array"*/
/* *3     2-MAR-1990 11:24:22 WALTERS "(SPR 6012) Fix #ifndef USE_X to #ifdef USE_X"*/
/* *2     5-FEB-1990 23:42:03 JULIAN "(SPR -1) X version of WI Module (Kee)"*/
/* *1    19-JUN-1989 13:38:24 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element WI_Q_SEL.C*/
/* DEC/CMS REPLACEMENT HISTORY, Element WI_Q_SEL.C                                  */
/* *4    27-JUN-1990 17:31:05 KEE "(SPR -1) New wi data structure using 2-D array"  */
/* *3     2-MAR-1990 11:24:22 WALTERS "(SPR 6012) Fix #ifndef USE_X to #ifdef USE_X"*/
/* *2     5-FEB-1990 23:42:03 JULIAN "(SPR -1) X version of WI Module (Kee)"        */
/* *1    19-JUN-1989 13:38:24 SYSTEM ""                                             */
/* DEC/CMS REPLACEMENT HISTORY, Element WI_Q_SEL.C                                  */
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
/*NAME:    WI_QUERY_SELECTIONS                                                      */
/*                                                                                  */
/*DESCRIPTION:                                                                      */
/*                                                                                  */
/*    Ask for a list of the selected items from a selector.                         */
/*                                                                                  */
/*    Input:                                                                        */
/*                                                                                  */
/*         cdlgid   = (INT) custom dialog ID.                                       */
/*         cditemid = (INT) item id, 1 -> N.                                        */
/*                                                                                  */
/*    OutPut:                                                                       */
/*                                                                                  */
/*         numitems = (INT) number of items selected.                               */
/*         items    = (INT ARRAY) a list of the lines selected.                     */
/*                                                                                  */
/*   NOTE: 5/5/87 ... since there is no max_selectable limitation anymore           */
/*         in ESITerm (I've made it a dynamically allocated array) the              */
/*         range checking is invalid and has been removed.  MAX_CD_SELECTABLE       */
/*         does not mean anything anymore...selectors can have any number           */
/*         of selections.  MAKE SURE YOUR ARRAY IS BIG ENOUGH!                      */
/*         (Actually, your array COULD have been too small before if                */
/*          it was less than MAX_CD_SELECTABLE elements.)  mdh                      */
/*                                                                                  */
/*************************************************************************          */

#include "esi_wi_int.x"

#ifdef USE_X

#include "ansixt.h"

#endif

/************************************************************************************/
#define IDENT progname
publicdef INT wi_query_selections (cdlgid, cditemid, numitems, items)   /*************************************************************************/

INT cdlgid;
INT cditemid;
INT *numitems;
INT items[];

    {
    PROGNAME ("wi_query_selections");
    INT i;
    INT status;

#ifdef USE_X
    INT num_items;
    Widget itemwidget;
    WIDGET_ITEM_INFO *iteminfo;
    Arg arglist[1];
    
    E_CHECK_INIT;
    E_CHECK_DIALOGITEM (cditemid);
    E_CHECK_ID (cdlgid, cditemid, WI_DIALOG);
    E_CHECK_WIDGET (cdlgid, cditemid);
    
    wiz_jp_name (IDENT);
    
    iteminfo = E_WIDGET_INFO (cdlgid, cditemid);
    itemwidget = E_GET_WIDGET (cdlgid, cditemid);
    
    switch (iteminfo->type_id)
        {
    case SELECTOR:
        /* get number of selected items                                             */
#ifdef MOTIF  /* ---------- BEGIN MOTIF BLOCK ---------- */
        XtSetArg (arglist[0], XmNselectedItemCount, &num_items);
#else  /* ---------- END MOTIF BLOCK - BEGIN DECW BLOCK ---------- */
        XtSetArg (arglist[0], DwtNselectedItemsCount, &num_items);
#endif  /* ---------- END DECW BLOCK ---------- */
        XtGetValues (itemwidget, arglist, 1);
        *numitems = num_items;
        /* num_items should also = iteminfo->item_type.selector.num_sel             */
        for (i = 0; i < num_items; i++)
            {
            items[i] = iteminfo->item_type.selector.sel_list[i];
            }
        break;
    default:
        E_WIZ_ERROR ("Invalid dialog item type", progname);
        break;
        }
#else        /* ESITERM code */
    
    /* Issue request, query selections.                                             */
    E_CHECK_INIT;
    E_CHECK_DIALOGITEM (cditemid);
    E_CHECK_ID (cdlgid, cditemid, WI_DIALOG);
    
    wiz_jp_name (IDENT);
    
    wiv.playback_nosend = 1;
    SEND_3_VALUES (cditemid, cdlgid, MC_QUERY_SELECTIONS);
    wiv.playback_nosend = 0;
    
    status = SUCCESS;
    
    /* Pick up report, count followed by the itemids.                               */
    wiz_set_echo (FALSE);
    if (WAIT_RESPONSE (QSEL_CODE))
        {
        wiz_gnum (numitems);
        
        /* Pick up array elements.                                                  */
        for (i = 0; i < *numitems; i++)
            {
            wiz_gnum (items++);
            }
        wiz_acknowledge ();
        }
    wiz_set_echo (TRUE);
#endif
    RETURN (SUCCESS);
    }
