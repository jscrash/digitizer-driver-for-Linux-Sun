/* DEC/CMS REPLACEMENT HISTORY, Element WI_INS_CDLN.C*/
/* *8    23-OCT-1990 13:55:36 KEE "(SPR -1) Motif and DecWindow Merge"*/
/* *7     2-JUL-1990 15:15:16 KEE "(SPR -1) put back old generation (5)"*/
/* *6     2-JUL-1990 13:48:54 KEE "(SPR -1) Add wi_set_selection_limit "*/
/* *5    27-JUN-1990 17:29:47 KEE "(SPR -1) New wi data structure using 2-D array"*/
/* *4    12-APR-1990 21:00:56 JULIAN "(SPR -1) changed ansiDwt to ansidwt"*/
/* *3     2-MAR-1990 11:31:49 VINCE "(SPR 6012) Changed usage of USE_X symbol"*/
/* *2     5-FEB-1990 23:37:39 JULIAN "(SPR -1) X version of WI Module (Kee)"*/
/* *1    19-JUN-1989 13:37:45 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element WI_INS_CDLN.C*/
/* DEC/CMS REPLACEMENT HISTORY, Element WI_INS_CDLN.C                               */
/* *7     2-JUL-1990 15:15:16 KEE "(SPR -1) put back old generation (5)"            */
/* *6     2-JUL-1990 13:48:54 KEE "(SPR -1) Add wi_set_selection_limit "            */
/* *5    27-JUN-1990 17:29:47 KEE "(SPR -1) New wi data structure using 2-D array"  */
/* *4    12-APR-1990 21:00:56 JULIAN "(SPR -1) changed ansiDwt to ansidwt"          */
/* *3     2-MAR-1990 11:31:49 VINCE "(SPR 6012) Changed usage of USE_X symbol"      */
/* *2     5-FEB-1990 23:37:39 JULIAN "(SPR -1) X version of WI Module (Kee)"        */
/* *1    19-JUN-1989 13:37:45 SYSTEM ""                                             */
/* DEC/CMS REPLACEMENT HISTORY, Element WI_INS_CDLN.C                               */
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
/*NAME:    WI_INSERT_CD_LINE                                                        */
/*                                                                                  */
/*DESCRIPTION:                                                                      */
/*                                                                                  */
/*    Insert a line into an edit text field or a selector.                          */
/*                                                                                  */
/*************************************************************************          */

#include "esi_c_lib.h"

#ifndef ESI_WI_INT_X
#include "esi_wi_int.x"
#endif

#ifdef USE_X
#ifndef ESI_X_H
#include "esi_x.h"
#endif
#include "ansixt.h"
#endif

/************************************************************************************/
#define IDENT progname
publicdef INT wi_insert_cd_line (cdlg_id, item_id, line_num, line_text) /*************************************************************************/

INT cdlg_id;                        /* %P ID of dialog                              */
INT item_id;                        /* %P ID of individual control item.            */
INT line_num;                       /* %P Line number to insert before, (1->N).     */
CHAR *line_text;                    /* %P Text of line to insert.                   */
    {
    PROGNAME ("wi_insert_cd_line");
    CHAR tempbuffer[STMAX + 1];
    INT char_ctr;
    INT status;
#ifdef USE_X
    Dimension selector_width;
    Widget itemwidget;
    WIDGET_ITEM_INFO *iteminfo;
    Arg arglist[3];
#ifdef MOTIF  /* ---------- BEGIN MOTIF BLOCK ---------- */
    INT pos, shown, inc, page_inc;
    Dimension selector_height;
    Widget selectorscrollbar;
    XmString CompoundString;
    INT minvalue, maxvalue;
    BOOL end_flag;
#else  /* ---------- END MOTIF BLOCK - BEGIN DECW BLOCK ---------- */
    DwtCompString CompoundString;
#endif  /* ---------- END DECW BLOCK ---------- */
#endif
    wiz_jp_name (IDENT);
    
    E_CHECK_INIT;
    E_CHECK_DIALOGITEM (item_id);
    E_CHECK_ID (cdlg_id, item_id, WI_DIALOG);

#ifdef USE_X
    E_CHECK_WIDGET (cdlg_id, item_id);
    iteminfo = E_WIDGET_INFO (cdlg_id, item_id);
    itemwidget = E_GET_WIDGET (cdlg_id, item_id);

#ifdef MOTIF  /* ---------- BEGIN MOTIF BLOCK ---------- */
    if (strlen (line_text) > 84)
        {
        strncpy (tempbuffer, line_text, 84);
        tempbuffer[84] = '\0';
        }
    else
        {
        strcpy (tempbuffer, line_text);
        char_ctr = strlen (tempbuffer);
        if (tempbuffer[char_ctr] == '\r' || tempbuffer[char_ctr] == '\n')
            {
            tempbuffer[char_ctr] == '\0';
            }
        }
    switch (iteminfo->type_id)
        {
    case SELECTOR:
        XtSetArg (arglist[0], XmNwidth, &selector_width);
        XtSetArg (arglist[1], XmNheight, &selector_height);
        XtSetArg (arglist[2], XmNverticalScrollBar, &selectorscrollbar);
        XtGetValues (itemwidget, arglist, 3);
        XmScrollBarGetValues (selectorscrollbar, &pos, &shown, &inc, &page_inc);
        
        /* before insert, test if list items are at the end of list box             */
        XtSetArg (arglist[0], XmNminimum, &minvalue);
        XtSetArg (arglist[1], XmNmaximum, &maxvalue);
        XtGetValues (selectorscrollbar, arglist, 2);
        end_flag = (pos > minvalue &&
                    pos + shown == maxvalue - minvalue) ? TRUE : FALSE;
        
        CompoundString = XmStringCreate (tempbuffer, XmSTRING_DEFAULT_CHARSET);
        XmListAddItem (itemwidget, CompoundString, line_num);   /* add item         */
        XmStringFree (CompoundString);
        wi_set_default_sel (cdlg_id, item_id, 0);
        
        /* after insert, if list items are not at teh end of list box,              */
        /*   then scroll them down                                                  */
        if (end_flag == TRUE)
            {
            XmScrollBarGetValues (selectorscrollbar, &pos, &shown, &inc, &page_inc);
            XtSetArg (arglist[0], XmNminimum, &minvalue);
            XtSetArg (arglist[1], XmNmaximum, &maxvalue);
            XtGetValues (selectorscrollbar, arglist, 2);
            if (pos + shown + 1 == maxvalue - minvalue)
                {
                pos++;
                XtSetArg (arglist[0], XmNvalue, pos);
                XtSetValues (selectorscrollbar, arglist, 1);
                XmScrollBarSetValues (selectorscrollbar, pos, shown, inc, page_inc,
                                      FALSE);
                XmListSetPos (itemwidget, pos + 1);
                }
            }
        wiz_set_scrollbar (cdlg_id, item_id);   /* set our own scrollbar            */
        /* following is to prevent selector resize                                  */
        XtSetArg (arglist[0], XmNwidth, selector_width);
        XtSetArg (arglist[1], XmNheight, selector_height);
        XtSetValues (itemwidget, arglist, 2);
        break;
    default:
        E_WIZ_ERROR ("Invalid dialog item type", progname);
        break;
        }
#else  /* ---------- END MOTIF BLOCK - BEGIN DECW BLOCK ---------- */
    
    /* append a 'cr' to the text, if necessary...                                   */
    if (strlen (line_text) > 84)
        {
        strncpy (tempbuffer, line_text, 84);
        tempbuffer[84] = '\015';
        
        tempbuffer[85] = '\0';
        }
    else
        {
        strcpy (tempbuffer, line_text);
        char_ctr = strlen (tempbuffer);
        if (tempbuffer[char_ctr] != '\r')
            {
            strcat (tempbuffer, "\015");    /* insert a 'cr'                        */
            }
        }
    switch (iteminfo->type_id)
        {
    case SELECTOR:
        XtSetArg (arglist[0], DwtNwidth, &selector_width);
        XtGetValues (itemwidget, arglist, 1);
        CompoundString = wiz_DwtLatin1StringSpecial (tempbuffer, selector_width);
        DwtListBoxAddItem (itemwidget, CompoundString, line_num);   /* add item     */
        XtFree (CompoundString);
        wi_set_default_sel (cdlg_id, item_id, 0);
        wiz_set_scrollbar (cdlg_id, item_id);
        break;
    default:
        E_WIZ_ERROR ("Invalid dialog item type", progname);
        break;
        }
#endif  /* ---------- END DECW BLOCK ---------- */   
    wiz_XFlush (wiv.current_Display);

#else        /* ESITERM code */
    /* append a 'cr' to the text, if necessary...                                   */
    if (strlen (line_text) > 84)
        {
        strncpy (tempbuffer, line_text, 84);
        tempbuffer[84] = '\015';
        
        tempbuffer[85] = '\0';
        }
    else
        {
        strcpy (tempbuffer, line_text);
        char_ctr = strlen (tempbuffer);
        if (tempbuffer[char_ctr] != '\r')
            {
            strcat (tempbuffer, "\015");    /* insert a 'cr'                        */
            }
        }
    SEND_PROMPT (tempbuffer);
    SEND_4_VALUES (line_num, item_id, cdlg_id, MC_INSERT_LINE);
#endif
    
    return(SUCCESS);
    }
