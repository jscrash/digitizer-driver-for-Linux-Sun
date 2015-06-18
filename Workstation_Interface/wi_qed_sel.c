/* DEC/CMS REPLACEMENT HISTORY, Element WI_QED_SEL.C*/
/* *7    23-OCT-1990 13:56:03 KEE "(SPR -1) Motif and DecWindow Merge"*/
/* *6    27-JUN-1990 17:30:36 KEE "(SPR -1) New wi data structure using 2-D array"*/
/* *5    12-APR-1990 21:02:49 JULIAN "(SPR -1) changed ansiDwt to ansidwt"*/
/* *4    21-MAR-1990 09:55:07 KEE "(SPR 5142) fixing no selection of edittext bug"*/
/* *3     2-MAR-1990 11:23:55 WALTERS "(SPR 6012) Fix #ifndef USE_X to #ifdef USE_X"*/
/* *2     5-FEB-1990 23:39:31 JULIAN "(SPR -1) X version of WI Module (Kee)"*/
/* *1    19-JUN-1989 13:38:03 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element WI_QED_SEL.C*/
/* DEC/CMS REPLACEMENT HISTORY, Element WI_QED_SEL.C                                */
/* *6    27-JUN-1990 17:30:36 KEE "(SPR -1) New wi data structure using 2-D array"  */
/* *5    12-APR-1990 21:02:49 JULIAN "(SPR -1) changed ansiDwt to ansidwt"          */
/* *4    21-MAR-1990 09:55:07 KEE "(SPR 5142) fixing no selection of edittext bug"  */
/* *3     2-MAR-1990 11:23:55 WALTERS "(SPR 6012) Fix #ifndef USE_X to #ifdef USE_X"*/
/* *2     5-FEB-1990 23:39:31 JULIAN "(SPR -1) X version of WI Module (Kee)"        */
/* *1    19-JUN-1989 13:38:03 SYSTEM ""                                             */
/* DEC/CMS REPLACEMENT HISTORY, Element WI_QED_SEL.C                                */
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
/*NAME:    WI_QEDIT_SELECTION                                                       */
/*                                                                                  */
/*DESCRIPTION:                                                                      */
/*                                                                                  */
/*    Query for the selected text from an edittext rec                              */
/*                                                                                  */
/*    Input:                                                                        */
/*                                                                                  */
/*         cdlgid   = (INT) custom dialog ID.                                       */
/*         cditemid = (INT) item in custom dialog , 1 -> N.                         */
/*                                                                                  */
/*    OutPut:                                                                       */
/*                                                                                  */
/*         cdvalue  = (INT *) value of 1st char pos in editrec                      */
/*         cdtext   = (CHAR *) text currently selected                              */
/*                                                                                  */
/*************************************************************************          */

#include "esi_wi_int.x"

#ifdef USE_X
#ifndef ESI_X_H
#include "esi_x.h"
#endif
#include "ansixt.h"
#endif

/************************************************************************************/
#define IDENT progname
publicdef INT wi_qedit_selection (cdlgid, cditemid, cdvalue, cdtext)    /*************************************************************************/

INT cdlgid;
INT cditemid;
INT *cdvalue;
CHAR *cdtext;
    {
    PROGNAME ("wi_qedit_selection");
    INT status;

#ifdef USE_X
    INT i, j;
    INT match;
    INT ltext;
    INT lcdtext;
    Widget itemwidget;
    WIDGET_ITEM_INFO *iteminfo;
    CHAR *text;
    CHAR *cdtext_ptr;
    
    E_CHECK_INIT;
    E_CHECK_DIALOGITEM (cditemid);
    E_CHECK_ID (cdlgid, cditemid, WI_DIALOG);
    
    wiz_jp_name (IDENT);
    
    /* following is not correctly implemented.  If in edittext field                */
    /*   the highlighted word is '40' and '404040' is the string,                   */
    /*   then the following coding always reports the first match even if           */
    /*   the highlighted word is the last one                                       */
    /*                                                                              */
    /*   In DECW, there's no way to tell the starting position of a hightlighted    */
    /*   word                                                                       */
    /*                                                                              */
    
    E_CHECK_WIDGET (cdlgid, cditemid);
    iteminfo = E_WIDGET_INFO (cdlgid, cditemid);
    itemwidget = E_GET_WIDGET (cdlgid, cditemid);
    
    if (iteminfo->type_id == EDITTEXT)
        {
#ifdef MOTIF  /* ---------- BEGIN MOTIF BLOCK ---------- */
        cdtext_ptr = XmTextGetSelection (itemwidget);
#else  /* ---------- END MOTIF BLOCK - BEGIN DECW BLOCK ---------- */
        cdtext_ptr = DwtSTextGetSelection (itemwidget);
#endif  /* ---------- END DECW BLOCK ---------- */
        if (cdtext_ptr != (CHAR *)0)
            {
            strcpy (cdtext, cdtext_ptr);
            XtFree (cdtext_ptr);
            /* free memory created from DwtSTextGetSelection call                   */
            lcdtext = strlen (cdtext);
#ifdef MOTIF  /* ---------- BEGIN MOTIF BLOCK ---------- */
            text = XmTextGetString (itemwidget);
#else  /* ---------- END MOTIF BLOCK - BEGIN DECW BLOCK ---------- */
            text = DwtSTextGetString (itemwidget);
#endif  /* ---------- END DECW BLOCK ---------- */
            if (text != (CHAR *)0)
                {
                for (i = 0, ltext = strlen (text); i < ltext; i++)
                    {
                    if (text[i] == cdtext[0])
                        {
                        match = TRUE;
                        for (j = 0; j < lcdtext; j++)
                            if (text[i + j] != cdtext[j])
                                {
                                match = FALSE;
                                break;
                                }
                        if (match == TRUE)
                            {
                            *cdvalue = i;
                            XtFree (text);
                            return(SUCCESS);
                            }
                        }
                    }
                XtFree (text);
                /* free memory created from DwtSTextGetString call                  */
                }
            }
        }
    else
        {
        E_WIZ_ERROR ("Invalid dialog item type", progname);
        }
    strcpy (cdtext, "");
    *cdvalue = 0;
    return(SUCCESS);
#else        /* ESITERM code */
    INT cditemid_temp;
    
    /* Issue request, query current state.                                          */
    E_CHECK_INIT;
    E_CHECK_DIALOGITEM (cditemid);
    E_CHECK_ID (cdlgid, cditemid, WI_DIALOG);
    
    wiz_jp_name (IDENT);
    
    wiv.playback_nosend = 1;
    SEND_3_VALUES (cditemid, cdlgid, MC_QSELTEXT);
    wiv.playback_nosend = 0;
    
    /* Pick up report.                                                              */
    wiz_set_echo (FALSE);
    if (WAIT_RESPONSE (RQCD_CODE))
        {
        wiz_gnum (&cditemid_temp);
        wiz_gnum (cdvalue);
        wiz_gstr (8196, cdtext);    /*%Q Need longer strings.                       */
        wiz_acknowledge ();
        }
    wiz_set_echo (TRUE);
    RETURN (SUCCESS);

#endif
    }
