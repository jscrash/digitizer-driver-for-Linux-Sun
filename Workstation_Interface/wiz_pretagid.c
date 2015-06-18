/* DEC/CMS REPLACEMENT HISTORY, Element WIZ_PRETAGID.C*/
/* *2     5-FEB-1990 23:29:03 JULIAN "(SPR -1) X version of WI Module (Kee)"*/
/* *1    19-JUN-1989 13:36:03 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element WIZ_PRETAGID.C*/

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

NAME:    WIZ_PREPEND_TAG_ID

DESCRIPTION:

    Prepend TAG and ID for dialog identification.

*************************************************************************/

#include "esi_c_lib.h"
#include "esi_wi_int.h"

/************************************************************************/
publicdef INT wiz_prepend_tag_id(cdtitle, owner_tag, owner_id, new_title)
/************************************************************************/

CHAR *cdtitle;   /* %P IN: Text of title. */
CHAR *owner_tag; /* %P IN: Identifying tag for owner. */
INT  owner_id;   /* %P IN: ID of owner from Applicatio Manager. */
CHAR *new_title; /* %P OUT: Title with Tag and ID. */
{
    PROGNAME ("wiz_prepend_tag_id");
#define IDENT progname
    CHAR stl[ST_COMMAND_MAX];
/************************************************************************/


/* Prepend if not an orphan. */
    if (owner_id != WI_NO_OWNER_ID)
    {
        strcpy(new_title, owner_tag);
        sprintf(stl, "-(%d) ", owner_id);
        strcat(new_title, stl);
        strcat(new_title, cdtitle);
    }
    else
    {
        strcpy(new_title, cdtitle);
    }

    RETURN(SUCCESS);
}
