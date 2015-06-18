/* DEC/CMS REPLACEMENT HISTORY, Element WI_CLS_DLG.C*/
/* *4    27-JUN-1990 17:27:10 KEE "(SPR -1) New wi data structure using 2-D array"*/
/* *3     2-MAR-1990 11:35:49 MING "(SPR 6012) mod USE_X"*/
/* *2     5-FEB-1990 23:33:22 JULIAN "(SPR -1) X version of WI Module (Kee)"*/
/* *1    19-JUN-1989 13:37:03 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element WI_CLS_DLG.C*/
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

NAME:    WI_CLOSE_DIALOG

DESCRIPTION:

    Close a custom dialog box.

    Input:

         cdlgid = (INT) dialog box id allocated by workstation.

*************************************************************************/

#include "esi_wi_int.x"        /* external definitions required here */

/************************************************************************/
#define IDENT progname
publicdef INT wi_close_dialog(cdlgid)
/************************************************************************/

INT cdlgid;

    {
    PROGNAME("wi_close_dialog");

#ifdef USE_X

    wiz_jp_name(IDENT);

    wiz_unmanagechild_widget(cdlgid);
    /* unmanage all children under specific widget */

#else        /* ESITERM code */
    INT status;
    
    E_CHECK_INIT;
    E_CHECK_ID(cdlgid,0,WI_DIALOG);

    wiz_jp_name(IDENT);
    SEND_2_VALUES( cdlgid, MC_CLOSE_CD);
#endif
    RETURN(SUCCESS);
    }



