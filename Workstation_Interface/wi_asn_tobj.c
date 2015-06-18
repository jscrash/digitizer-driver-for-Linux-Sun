/* DEC/CMS REPLACEMENT HISTORY, Element WI_ASN_TOBJ.C*/
/* *7    23-OCT-1990 13:53:38 KEE "(SPR -1) Motif and DecWindow Merge"*/
/* *6    27-JUN-1990 17:26:17 KEE "(SPR -1) New wi data structure using 2-D array"*/
/* *5    12-APR-1990 20:55:32 JULIAN "(SPR -1) changed ansiDwt to ansidwt"*/
/* *4    15-MAR-1990 13:36:02 KEE "(SPR -1) Change idverify->text stucture to be dynamic alloc "*/
/* *3     2-MAR-1990 11:20:46 MING "(SPR 6012) mod USE_X"*/
/* *2     5-FEB-1990 23:31:30 JULIAN "(SPR -1) X version of WI Module (Kee)"*/
/* *1    19-JUN-1989 13:36:42 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element WI_ASN_TOBJ.C*/
/* DEC/CMS REPLACEMENT HISTORY, Element WI_ASN_TOBJ.C                               */
/* *6    27-JUN-1990 17:26:17 KEE "(SPR -1) New wi data structure using 2-D array"  */
/* *5    12-APR-1990 20:55:32 JULIAN "(SPR -1) changed ansiDwt to ansidwt"          */
/* *4    15-MAR-1990 13:36:02 KEE "(SPR -1) Change idverify->text stucture to be dynamic alloc "    */
/* *3     2-MAR-1990 11:20:46 MING "(SPR 6012) mod USE_X"                           */
/* *2     5-FEB-1990 23:31:30 JULIAN "(SPR -1) X version of WI Module (Kee)"        */
/* *1    19-JUN-1989 13:36:42 SYSTEM ""                                             */
/* DEC/CMS REPLACEMENT HISTORY, Element WI_ASN_TOBJ.C                               */
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

/* ************************************************************************         */
/*                                                                                  */
/*   NAME:    WI_ASSIGN_TEXT_OBJECT                                                 */
/*                                                                                  */
/*   DESCRIPTION:                                                                   */
/*                                                                                  */
/*        This functions 'assigns' a defined text object to a defined text          */
/*     control, and makes it become visible if the dialog is visible.               */
/*                                                                                  */
/*    INPUT:  'dialog' ....... ID of the dialog holding the text control            */
/*        'control' ...... Specifies which text control on the dialog               */
/*            'textid' ....... ID of the defined text object.                       */
/*                                                                                  */
/*************************************************************************          */

#include "esi_wi_int.x"        /* external definitions needed here */

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

#define MC_ASSIGN_TEXT 80     /* ESITerm opcode for this command  */

/* **********************************************************************           */
publicdef INT wi_assign_text_object (dialog, control, textid)   /* **********************************************************************   */

INT dialog, control, textid;

    {
    PROGNAME ("wi_assign_text_object");
#define IDENT progname

#ifdef USE_X
    INT status;
    CHAR *strings;
    Widget itemwidget;
    WIDGET_ITEM_INFO *iteminfo;
    
    E_CHECK_INIT;
    E_CHECK_WIDGET (dialog, control);
    
    wiz_jp_name (progname);
    
    /* get item widget and its associate info                                       */
    iteminfo = E_WIDGET_INFO (dialog, control);
    itemwidget = iteminfo->widget;
    
    if (1 <= textid && textid <= wiv.text_id_count)
        {
        strings = wiv.text_list[textid - 1];
        }
    else
        {
        E_WIZ_ERROR ("Invalid text id", progname);
        }
    switch (iteminfo->type_id)
        {
    case STATTEXT:
#ifdef MOTIF  /* ---------- BEGIN MOTIF BLOCK ---------- */
        XmTextSetString (itemwidget, strings);
#else  /* ---------- END MOTIF BLOCK - BEGIN DECW BLOCK ---------- */
        DwtSTextSetString (itemwidget, strings);
#endif  /* ---------- END DECW BLOCK ---------- */
        break;
    case EDITTEXT:
#ifdef MOTIF  /* ---------- BEGIN MOTIF BLOCK ---------- */
        XmTextSetString (itemwidget, strings);
#else  /* ---------- END MOTIF BLOCK - BEGIN DECW BLOCK ---------- */
        DwtSTextSetString (itemwidget, strings);
#endif  /* ---------- END DECW BLOCK ---------- */
        if (IS_EMPTY_STRING (iteminfo->text) == FALSE)
            tc_free (iteminfo->text);
        iteminfo->text = STRING_ALLOCATE (strings);
        strcpy (iteminfo->text, strings);
        /* store the text for wi_query_dialog_change checking                       */
        break;
    case SELECTOR:
        wiz_set_selector_text (dialog, control, strings);
        break;
    default:
        E_WIZ_ERROR ("Invalid dialog item type", progname);
        break;
        }
    wiz_XFlush (wiv.current_Display);
#else    /* ESITERM Code */
    E_CHECK_INIT;
    
    wiz_jp_name (progname);
    
    SEND_4_VALUES (textid, control, dialog, MC_ASSIGN_TEXT);
#endif
    RETURN (SUCCESS);
    }
