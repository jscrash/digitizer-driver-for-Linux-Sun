/* DEC/CMS REPLACEMENT HISTORY, Element WI_RQ_DLGITM.C*/
/* *3     2-MAR-1990 12:14:45 JULIAN "(SPR 6012) fix if USE_X to be ifdef USE_X"*/
/* *2     5-FEB-1990 23:44:14 JULIAN "(SPR -1) X version of WI Module (Kee)"*/
/* *1    19-JUN-1989 13:38:50 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element WI_RQ_DLGITM.C*/
/******************************************************************************

                Copyright Finder Graphics Systems, Inc. 1989
           Unpublished -- All rights reserved

THIS SOFTWARE IS THE PROPRIETARY PROPERTY OF Finder Graphics Systems, Inc.  AND
MAY CONTAIN CONFIDENTIAL TRADE SECRET INFORMATION. IT IS LICENSED FOR USE ON THE
DESIGNATED EQUIPMENT ON WHICH IT WAS ORIGINALLY INSTALLED AND  MAY NOT BE
MODIFIED, DUPLICATED OR COPIED IN ANY FORM WITHOUT PRIOR WRITTEN CONSENT OF

            Finder Graphics Systems, Inc.
            201 Tamal Vista Blvd
            Corte Madera, CA  USA 94925
            (415) 927-0100

(The above notice does not constitute an admission of publication or
unrestricted dissemination of the work)

******************************************************************************/

/*************************************************************************

NAME:    WI_REQUEST_DIALOG_ITEM

DESCRIPTION:

    Request a single item from a custom dialog box.

    Input:

    OutPut:

         cdlgid   = (INT *) custom dialog ID.
         cditemid = (INT *) item in custom dialog , 1 -> N.
         cdvalue  = (INT *) value of custom dialog item, usually 0 or 1.
         cdtext   = (CHAR *) text from custom dialog.

*************************************************************************/

#include "esi_wi_int.x"

/************************************************************************/
publicdef INT wi_request_dialog_item(cdlgid, cditemid, cdvalue, cdtext)
/************************************************************************/

INT *cdlgid;
INT *cditemid;
INT *cdvalue;
CHAR *cdtext;
{
    PROGNAME("wi_request_dialog_item");
#define IDENT progname
    INT event_class;

    INT event_consumed, status;
    CALLBACK_STRUCT *list_ptr;

/************************************************************************/

    E_CHECK_INIT;

    wiz_jp_name(progname);

    do
      {
    event_consumed = FALSE;

#ifndef USE_X
    wiv.playback_nosend = 1;  /* don't send this in wiv.playback */
       SEND_1_VALUE(MC_REQUEST_CD);
       wiv.playback_nosend = 0; /* re-enable output */
#endif

    wiz_wait_event(&event_class, cdlgid, cditemid);

/* Test for unexpected events! */
    if (event_class != WI_DIALOG_EVENT)
      {
        RETURN(WI_UNEXPECTED_EVENT);
      }

    for (list_ptr = wiv.callback_list; 
         list_ptr != (CALLBACK_STRUCT *) NULL; 
         list_ptr = list_ptr->next)
      {
        status = 
          (*list_ptr->function)(*cdlgid, *cditemid,
                    list_ptr->pointer);
        /* did that call consume the event ??? */
        if (status == SUCCESS)
          {
        event_consumed = TRUE;  /* yep, so go back to waiting */
        break;
          }
      }
      }
    while (event_consumed);

/* set info */
    *cdvalue = wiv.hold.cdvalue;
    strcpy(cdtext, wiv.hold.cdtext);
    return(*cdlgid);
  }

