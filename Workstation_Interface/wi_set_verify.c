/* DEC/CMS REPLACEMENT HISTORY, Element WI_SET_VERIFY.C*/
/* *4    27-JUN-1990 17:33:41 KEE "(SPR -1) New wi data structure using 2-D array"*/
/* *3     2-MAR-1990 11:07:34 JULIAN "(SPR 6012) change if USE_X to ifdef USE_X (Kee)"*/
/* *2     5-FEB-1990 23:48:56 JULIAN "(SPR -1) X version of WI Module (Kee)"*/
/* *1    19-JUN-1989 13:39:40 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element WI_SET_VERIFY.C*/
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

NAME:    WI_SET_VERIFY

DESCRIPTION:

    Set the verification mode for an edittext field.


   This word may be invoked as follows:

   wi_set_verify( cdlgid, itemid, VERIFY_FLOAT, 0 ) ...

           Verify a floating point number,  no length limit.

   wi_set_verify( cdlgid, itemid, VERIFY_ALPHANUMERIC, 20 ) ...

           Verify any alphanumeric string, limit length to 20 characters.

   wi_set_verify( cdlgid, itemid, VERIFY_NONE, 0 ) ...

           No Verify

    Input:

             cdlgid = (INT) custom dialog ID
           cditemid = (INT) item ID, 1 -> N.
         verifymode = (INT) may be either VERIFY_FLOAT, VERIFY_INTEGER,
                            VERIFY_ALPHANUMERIC, or VERIFY_NONE.
            length  = (INT) maximum length of response string.

*************************************************************************/
#include "esi_wi_int.x"

/************************************************************************/
publicdef INT wi_set_verify (cdlgid, cditemid, verifymode, length)
/************************************************************************/

INT cdlgid;
INT cditemid;
INT verifymode;
INT length;

/************************************************************************/
    {
    PROGNAME ("wi_set_verify");
#define IDENT progname

#ifdef USE_X
    INT status;
    Widget itemwidget;
    WIDGET_ITEM_INFO *iteminfo;

    wiz_jp_name (IDENT);

    E_CHECK_INIT;
    E_CHECK_WIDGET(cdlgid,cditemid);

    iteminfo = E_WIDGET_INFO(cdlgid,cditemid);
    itemwidget = E_GET_WIDGET(cdlgid,cditemid);

    switch (iteminfo->type_id) 
      {
	  case EDITTEXT:
        iteminfo->item_type.edittext.verifymode = verifymode;
        iteminfo->item_type.edittext.verifylen = length;
	break;
	  default:
	E_WIZ_ERROR("Invalid dialog item type",progname);
	break;
      }
#else        /* ESITERM code */
    wiz_jp_name (IDENT);

    E_CHECK_INIT;

    SEND_5_VALUES (length, verifymode, cditemid, cdlgid, MC_SET_VERIFY);
#endif
    RETURN (SUCCESS);
    }

