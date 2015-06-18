/* DEC/CMS REPLACEMENT HISTORY, Element WI_ACTIVATE.C*/
/* *6    23-OCT-1990 13:53:28 KEE "(SPR -1) Motif and DecWindow Merge"*/
/* *5    27-JUN-1990 17:26:09 KEE "(SPR -1) New wi data structure using 2-D array"*/
/* *4    12-APR-1990 20:54:04 JULIAN "(SPR -1) changed ansiDwt to ansidwt"*/
/* *3     2-MAR-1990 11:06:45 MING "(SPR 6012) mod USE_X"*/
/* *2     5-FEB-1990 23:31:05 JULIAN "(SPR -1) X version of WI Module (Kee)"*/
/* *1    19-JUN-1989 13:36:34 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element WI_ACTIVATE.C*/
/* DEC/CMS REPLACEMENT HISTORY, Element WI_ACTIVATE.C                               */
/* *5    27-JUN-1990 17:26:09 KEE "(SPR -1) New wi data structure using 2-D array"  */
/* *4    12-APR-1990 20:54:04 JULIAN "(SPR -1) changed ansiDwt to ansidwt"          */
/* *3     2-MAR-1990 11:06:45 MING "(SPR 6012) mod USE_X"                           */
/* *2     5-FEB-1990 23:31:05 JULIAN "(SPR -1) X version of WI Module (Kee)"        */
/* *1    19-JUN-1989 13:36:34 SYSTEM ""                                             */
/* DEC/CMS REPLACEMENT HISTORY, Element WI_ACTIVATE.C                               */
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
/*NAME:    WI_ACTIVATE                                                              */
/*                                                                                  */
/*DESCRIPTION:                                                                      */
/*                                                                                  */
/*    This function currently applies only to edit text fields and only those       */
/*    which are visible.  Calling this function with a non-zero flag causes the     */
/*    specified edittext field to be completely highlighted and ready to receive    */
/*    user input.  This should be used when opening edit text fields used as        */
/*    'SAVE AS' boxes, for example                                                  */
/*                                                                                  */
/*    Calling with a zero flag unconditionally de-activates the edittext ctl.       */
/*                                                                                  */
/*    Input:                                                                        */
/*                                                                                  */
/*         cdlgid = (INT) custom dialog ID                                          */
/*         cditemid = (INT) item ID, 1 -> N.                                        */
/*         activate_flag  = (INT) flag for item,  0 or 1.                           */
/*                                                                                  */
/*************************************************************************          */


#include "esi_wi_int.x"        /* external definitions needed here */

#ifdef USE_X
#ifndef ESI_X_H
#include "esi_x.h"
#endif
#include "ansixt.h"
#endif

/************************************************************************************/
publicdef INT wi_activate (cdlgid, cditemid, activate_flag) /*************************************************************************/

INT cdlgid;
INT cditemid;
INT activate_flag;

    {
    INT status;
    PROGNAME ("WI_ACTIVATE");
#define IDENT progname

#ifdef USE_X
    Widget itemwidget;
    WIDGET_ITEM_INFO *iteminfo;
    CHAR *str;
    
    wiz_jp_name (IDENT);
    
    E_CHECK_INIT;
    E_CHECK_DIALOGITEM (cditemid);
    E_CHECK_ID (cdlgid, cditemid, WI_DIALOG);
    E_CHECK_WIDGET (cdlgid, cditemid);
    
    iteminfo = E_WIDGET_INFO (cdlgid, cditemid);
    itemwidget = E_GET_WIDGET (cdlgid, cditemid);
    
    switch (iteminfo->type_id)
        {
    case EDITTEXT:
        iteminfo->item_type.edittext.activate_flag = (BOOL)activate_flag;
        if (activate_flag == FALSE) /* clear highlight                              */
            {
#ifdef MOTIF  /* ---------- BEGIN MOTIF BLOCK ---------- */
            XmTextClearSelection (itemwidget, CurrentTime);
#else  /* ---------- END MOTIF BLOCK - BEGIN DECW BLOCK ---------- */
            DwtSTextClearSelection (itemwidget, CurrentTime);
#endif  /* ---------- END DECW BLOCK ---------- */
            }
        else                        /* activate = true, highlight the whole string  */
            {
#ifdef MOTIF  /* ---------- BEGIN MOTIF BLOCK ---------- */
            str = XmTextGetString (itemwidget);
            XmTextSetSelection (itemwidget, 0, strlen (str), CurrentTime);
            /* ---------- END MOTIF BLOCK ----------                                */
#else
            /* ---------- BEGIN DECW BLOCK ----------                               */
            str = DwtSTextGetString (itemwidget);
            DwtSTextSetSelection (itemwidget, 0, strlen (str), CurrentTime);
#endif  /* ---------- END DECW BLOCK ---------- */
            XtFree (str);
            /* should use the timestamp of the event leading to the call            */
            /*   instead of CurrentTime when possible                               */
            }
        break;
    case SELECTOR:
        iteminfo->item_type.selector.activate_flag = (BOOL)activate_flag;
        break;
    default:
        E_WIZ_ERROR ("Invalid dialog item type", progname);
        break;
        }
#else        /* ESITERM code */
    wiz_jp_name (IDENT);
    
    E_CHECK_INIT;
    E_CHECK_DIALOGITEM (cditemid);
    E_CHECK_ID (cdlgid, cditemid, WI_DIALOG);
    
    SEND_4_VALUES (activate_flag, cditemid, cdlgid, MC_ACTIVATE);
#endif
    RETURN (SUCCESS);
    }
