/* DEC/CMS REPLACEMENT HISTORY, Element WI_ENBL_DLG.C*/
/* *4    27-JUN-1990 17:28:26 KEE "(SPR -1) New wi data structure using 2-D array"*/
/* *3     2-MAR-1990 11:28:09 VINCE "(SPR 6012) Changed usage of USE_X symbol"*/
/* *2     5-FEB-1990 23:35:45 JULIAN "(SPR -1) X version of WI Module (Kee)"*/
/* *1    19-JUN-1989 13:37:20 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element WI_ENBL_DLG.C*/
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

NAME:    WI_ENABLE_DIALOG

DESCRIPTION:

    Enable Custom Dialog for Picking. This can be called
    during a dialog interaction.

    Input:

         cdlgid = (INT) custom dialog ID
         enable_flag  = (INT) flag for item,  0 or 1.

*************************************************************************/

#include "esi_wi_int.x"        /* WI publicdefs are referenced here */

#ifdef USE_X
#include "ansixt.h"
#endif

#define IDENT progname

publicdef INT wi_enable_dialog(cdlgid, enable_flag)

INT cdlgid;
INT enable_flag;

    {
    PROGNAME("wi_enable_dialog");
    INT status;

#ifdef USE_X
    Widget cdlgwidget;

    wiz_jp_name(IDENT);

    E_CHECK_INIT;
    E_CHECK_ID(cdlgid,0,WI_DIALOG);
    E_CHECK_WIDGET(cdlgid,0);

/* Enable or disable item. */

    /* get the pop up dialog box and set sensitive */
    cdlgwidget = E_GET_WIDGET(cdlgid,0);
    XtSetSensitive(cdlgwidget, enable_flag);

    /* set the stattext sensitive to be false always */
    if (enable_flag == TRUE)              
      {
	wiz_stattext_sensitive_false(cdlgid);
      }

#else        /* ESITERM code */
    wiz_jp_name(IDENT);

    E_CHECK_INIT;
    E_CHECK_ID(cdlgid,0,WI_DIALOG);
    SEND_3_VALUES( enable_flag, cdlgid, MC_ENABLE_CD);
#endif
    RETURN(SUCCESS);
  }

