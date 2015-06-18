/* DEC/CMS REPLACEMENT HISTORY, Element WI_SET_DTITL.C*/
/* *9    18-JUN-1991 15:29:26 KEE "(SPR -1) fix do not set pushbutton text under Windows if it is not being created or it is being*/
/*destroyed, case 2983"*/
/* *8     3-MAY-1991 16:13:50 KEE "(SPR -1) MOTIF/PRIORGKS, MOTIF/DECWGKS, DECW/PRIORGKS, DECW/DECGKS version"*/
/* *7    19-NOV-1990 10:19:17 KEE "(SPR -1) fix motif code "*/
/* *6    23-OCT-1990 13:57:22 KEE "(SPR -1) Motif and DecWindow Merge"*/
/* *5    27-JUN-1990 17:32:27 KEE "(SPR -1) New wi data structure using 2-D array"*/
/* *4    12-APR-1990 21:05:32 JULIAN "(SPR -1) changed ansiDwt to ansidwt"*/
/* *3     2-MAR-1990 11:35:33 VINCE "(SPR 6012) Changed usage of USE_X symbol"*/
/* *2     5-FEB-1990 23:46:26 JULIAN "(SPR -1) X version of WI Module (Kee)"*/
/* *1    19-JUN-1989 13:39:14 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element WI_SET_DTITL.C*/
/* DEC/CMS REPLACEMENT HISTORY, Element WI_SET_DTITL.C                              */
/* *5    27-JUN-1990 17:32:27 KEE "(SPR -1) New wi data structure using 2-D array"  */
/* *4    12-APR-1990 21:05:32 JULIAN "(SPR -1) changed ansiDwt to ansidwt"          */
/* *3     2-MAR-1990 11:35:33 VINCE "(SPR 6012) Changed usage of USE_X symbol"      */
/* *2     5-FEB-1990 23:46:26 JULIAN "(SPR -1) X version of WI Module (Kee)"        */
/* *1    19-JUN-1989 13:39:14 SYSTEM ""                                             */
/* DEC/CMS REPLACEMENT HISTORY, Element WI_SET_DTITL.C                              */
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
/*NAME:    WI_SET_DIALOG_TITLE                                                      */
/*                                                                                  */
/*DESCRIPTION:                                                                      */
/*                                                                                  */
/*    Set title on custom dialog window.                                            */
/*                                                                                  */
/*    Input:                                                                        */
/*                                                                                  */
/*         cdlgid = (INT) custom dialog ID                                          */
/*         cdtitle   = (CST) text to be put in item.                                */
/*                                                                                  */
/*************************************************************************          */

#include "esi_wi_int.x"

#ifdef USE_X
#ifndef ESI_X_H
#include "esi_x.h"
#endif

#ifdef MOTIF  /* ---------- BEGIN MOTIF BLOCK ---------- */  
#include <X11/Xatom.h>
#endif  /* ---------- END MOTIF BLOCK ---------- */

#ifndef ESI_TS_H
#include "esi_ts.h"
#endif
#ifndef ESI_TC_H
#include "esi_tc.h"
#endif
#endif

/************************************************************************************/
publicdef INT wi_set_dialog_title (cdlgid, cdtitle) /*************************************************************************/

INT cdlgid;
CHAR *cdtitle;
    {
    INT status;
    PROGNAME ("wi_set_dialog_title");
#define IDENT progname
    CHAR new_title[STMAX];
    CHAR owner_tag[32];
    INT owner_id, id;
    INPUT_RECORD inp_rec;

#ifdef USE_X
    CHAR *title;
    Widget cdlgwidget;
    WIDGET_ITEM_INFO *cdlginfo;
    Arg arglist[1];
#ifdef MOTIF  /* ---------- BEGIN MOTIF BLOCK ---------- */
    XmString CompoundString;
#else  /* ---------- END MOTIF BLOCK - BEGIN DECW BLOCK ---------- */
    DwtCompString CompoundString;
#endif  /* ---------- END DECW BLOCK ---------- */
    
    /********************************************************************************/
    
    wiz_jp_name (IDENT);
    
    E_CHECK_INIT;
    E_CHECK_ID (cdlgid, 0, WI_DIALOG);
    E_CHECK_WIDGET (cdlgid, 0);
    
    /* Find Tag and ID.                                                             */
    if ((wi_query_input_owner_id (cdlgid,
                            &owner_id) EQUALS SUCCESS) AND (
                                                      wi_query_input_owner_tag (
                                                                     cdlgid,
                                                                     owner_tag)EQUALS 
                                                            SUCCESS))
        {
        /* Prepend Owner ID and Tag to title.                                       */
        wiz_prepend_tag_id (cdtitle, owner_tag, owner_id, new_title);
        title = new_title;
        }
    else
        {
        title = cdtitle;
        }
    /* get the pop up dialog box and set its title                                  */
    cdlginfo = E_WIDGET_INFO (cdlgid, 0);
    cdlgwidget = E_GET_WIDGET (cdlgid, 0);

#ifdef MOTIF  /* ---------- BEGIN MOTIF BLOCK ---------- */
    XChangeProperty (wiv.current_Display, XtWindow(XtParent(cdlgwidget)),
		     XA_WM_NAME, XA_STRING, 8, PropModeReplace, 
		     title, strlen (title));
    if (cdlginfo->item_type.dialog.popup_icon IS_NOT_EQUAL_TO (Widget)0)
      {
	sprintf (title, "%s - %s", owner_tag, cdtitle);    
	CompoundString = XmStringCreate (title, XmSTRING_DEFAULT_CHARSET);
	XtSetArg (arglist[0], XmNlabelString, CompoundString);
	XtSetValues (cdlginfo->item_type.dialog.popup_icon, arglist, 1);
	XmStringFree (CompoundString);
      }
#else  /* ---------- END MOTIF BLOCK - BEGIN DECW BLOCK ---------- */
    CompoundString = DwtLatin1String (title);
    XtSetArg (arglist[0], DwtNtitle, CompoundString);
    XtSetValues (cdlgwidget, arglist, 1);
    XtFree (CompoundString);

    if (cdlginfo->item_type.dialog.popup_icon IS_NOT_EQUAL_TO (Widget)0)
      {
	sprintf (title, "%s - %s", owner_tag, cdtitle);    
	CompoundString = DwtLatin1String (title);
	XtSetArg (arglist[0], DwtNlabel, CompoundString);
	XtSetValues (cdlginfo->item_type.dialog.popup_icon, arglist, 1);
	XtFree (CompoundString);
      }
#endif  /* ---------- END DECW BLOCK ---------- */
    if (IS_EMPTY_STRING (cdlginfo->text) == FALSE)
        tc_free (cdlginfo->text);
    cdlginfo->text = STRING_ALLOCATE (title);
    strcpy (cdlginfo->text, title);
    /* change label of push button under main window menu                           */
    
    wiz_XFlush (wiv.current_Display);

#else        /* ESITERM code */
    wiz_jp_name (IDENT);
    
    E_CHECK_INIT;
    E_CHECK_ID (cdlgid, 0, WI_DIALOG);
    
    /* Find Tag and ID.                                                             */
    if ((wi_query_input_owner_id (cdlgid,
                            &owner_id) EQUALS SUCCESS) AND (
                                                      wi_query_input_owner_tag (
                                                                     cdlgid,
                                                                     owner_tag)EQUALS 
                                                            SUCCESS))
        {
        /* Prepend Owner ID and Tag to title.                                       */
        wiz_prepend_tag_id (cdtitle, owner_tag, owner_id, new_title);
        SEND_PROMPT (new_title);
        }
    else
        {
        SEND_PROMPT (cdtitle);
        }
    SEND_2_VALUES (cdlgid, MC_SET_CD_TITLE);

#endif
    
    /* Now set the 'title' field of the input_record for this cd...                 */
    id = cdlgid;
    if (btfind (&wiv.input_tree, &id, &inp_rec))
        {
        /* Update entry in tree.                                                    */
        strcpy (inp_rec.owner_title, cdtitle);
        btrepcur (&wiv.input_tree, &id, &inp_rec);
        }
    RETURN (SUCCESS);
    }
