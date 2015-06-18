/* DEC/CMS REPLACEMENT HISTORY, Element WI_SET_OWNTG.C*/
/* *2     5-FEB-1990 23:48:15 JULIAN "(SPR -1) X version of WI Module (Kee)"*/
/* *1    19-JUN-1989 13:39:30 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element WI_SET_OWNTG.C*/

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

NAME:    WI_SET_OWNER_TAG

DESCRIPTION:

    Set the Owner of subsequently defined Menus and Dialogs

    Input:
         owner_tag = (CHAR *) owner TAG from Application.

*************************************************************************/

#ifndef ESI_WI_INT_X
#include "esi_wi_int.x"
#endif

/************************************************************************/
INT wi_set_owner_tag(owner_tag)
/************************************************************************/

CHAR *owner_tag;
{
    PROGNAME("wi_set_owner_tag");
#define IDENT progname
/************************************************************************/

    wiz_jp_name(IDENT);

/* Save in global area. */
    strcpy (wiv.current.owner_tag, owner_tag);

    RETURN (SUCCESS);
}
