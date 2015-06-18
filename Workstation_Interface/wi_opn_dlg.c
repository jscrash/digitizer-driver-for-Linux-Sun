/* DEC/CMS REPLACEMENT HISTORY, Element WI_OPN_DLG.C*/
/* *4    27-JUN-1990 17:29:56 KEE "(SPR -1) New wi data structure using 2-D array"*/
/* *3     2-MAR-1990 11:32:38 VINCE "(SPR 6012) Changed usage of USE_X symbol"*/
/* *2     5-FEB-1990 23:38:14 JULIAN "(SPR -1) X version of WI Module (Kee)"*/
/* *1    19-JUN-1989 13:37:50 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element WI_OPN_DLG.C*/
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

NAME:    WI_OPEN_DIALOG

DESCRIPTION:

    Open a custom dialog box for interaction.

    Input:

         cdlgid = (INT) dialog box id allocated by workstation.

*************************************************************************/

#include "esi_gl_defs.h"
#ifndef ESI_WI_INT_X
#include "esi_wi_int.x"
#endif

/************************************************************************/
#define IDENT progname
publicdef INT wi_open_dialog(cdlgid)
/************************************************************************/

INT cdlgid;
{
/************************************************************************/

    PROGNAME("wi_open_dialog");
    INT status;

#ifdef USE_X

    E_CHECK_INIT;
    E_CHECK_ID(cdlgid,0,WI_DIALOG);

    wiz_jp_name(IDENT);
    wiz_managechild_widget(cdlgid);

#else
    INT id;
    INPUT_RECORD  inp_rec;

    E_CHECK_INIT;
    E_CHECK_ID(cdlgid,0,WI_DIALOG);

    wiz_jp_name(IDENT);

    id = cdlgid;
    if (btfind(&wiv.input_tree, &id, &inp_rec))  {
       wiz_jp_add_text( inp_rec.orig_title );

       SEND_2_VALUES( cdlgid, MC_OPEN_CD);

       RETURN(SUCCESS);
    }
    else  {
    RETURN(WI_NOT_DEFINED);
    }
#endif
    return(SUCCESS);
}
