/* DEC/CMS REPLACEMENT HISTORY, Element WI_SET_CLICK.C*/
/* *4    27-JUN-1990 17:31:52 KEE "(SPR -1) New wi data structure using 2-D array"*/
/* *3     2-MAR-1990 12:15:17 JULIAN "(SPR 6012) fix if USE_X to be ifdef USE_X"*/
/* *2     5-FEB-1990 23:45:26 JULIAN "(SPR -1) X version of WI Module (Kee)"*/
/* *1    19-JUN-1989 13:39:04 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element WI_SET_CLICK.C*/
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

NAME:    WI_SET_CD_CLICKBACK

DESCRIPTION:

    set the clickback state of a particular dialog

    Input:
         cdlgid = (INT) custom dialog ID
         enable_flag  = (INT) flag for item,  0 or 1.

*************************************************************************/

#include "esi_gl_defs.h"
#ifndef ESI_WI_INT_X
#include "esi_wi_int.x"
#endif

/************************************************************************/
#define IDENT progname
publicdef INT wi_set_cd_clickback(cdlgid, enable_flag)
/************************************************************************/

INT cdlgid;
INT enable_flag;
{
    INT status;
    PROGNAME("wi_set_cd_clickback");

#ifdef USE_X
    Widget cdlgwidget;
    WIDGET_ITEM_INFO *cdlgiteminfo;
    wiz_jp_name(IDENT);

    E_CHECK_INIT;
    E_CHECK_ID(cdlgid,0,WI_DIALOG);
    E_CHECK_WIDGET(cdlgid,0);

    /* get widget and its assoc. info. */
    cdlgiteminfo = E_WIDGET_INFO(cdlgid,0);
    cdlgwidget = E_GET_WIDGET(cdlgid,0);

    switch (cdlgiteminfo->type_id)
      {
	  case DIALOG:
	cdlgiteminfo->item_type.dialog.clickback_flag = (BOOL) enable_flag;
	break;
	  default:
	E_WIZ_ERROR("Invalid dialog item type",progname);
	break;
      }

#else        /* ESITERM code */
    
    wiz_jp_name(IDENT);

    E_CHECK_INIT;
    E_CHECK_ID(cdlgid,0,WI_DIALOG);
    SEND_3_VALUES( enable_flag, cdlgid, MC_SET_CLICKBACK );
#endif

    RETURN(SUCCESS);
}



