/* DEC/CMS REPLACEMENT HISTORY, Element WI_SET_OWNID.C*/
/* *4     2-MAR-1990 12:07:45 JULIAN "(SPR 6012) fix if USE_X == 0 to be ifndef USE_X"*/
/* *3     2-MAR-1990 11:37:06 VINCE "(SPR 6012) Changed usage of USE_X symbol"*/
/* *2     5-FEB-1990 23:48:02 JULIAN "(SPR -1) X version of WI Module (Kee)"*/
/* *1    19-JUN-1989 13:39:27 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element WI_SET_OWNID.C*/
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

NAME:    WI_SET_OWNER_ID

DESCRIPTION:

    Set the Owner of subsequently defined Menus and Dialogs

    Input:
         owner_id = (INT) owner ID as assigned by Application Manager

*************************************************************************/

#include "esi_wi_int.x"

/************************************************************************/
publicdef INT wi_set_owner_id(owner_id)
/************************************************************************/

INT owner_id;
{
    PROGNAME("wi_set_owner_id");
#define IDENT progname
/************************************************************************/

    wiz_jp_name(IDENT);

/* Save in global area. */
    wiv.current.owner_id = owner_id;

/* Send to MAC. */
#ifndef USE_X
    SEND_2_VALUES(owner_id, MC_SET_OWNER_ID);
#endif

    RETURN (SUCCESS);
}
