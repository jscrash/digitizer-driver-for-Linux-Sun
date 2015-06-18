/* DEC/CMS REPLACEMENT HISTORY, Element WI_CHOWN_TAG.C*/
/* *2     5-FEB-1990 23:32:23 JULIAN "(SPR -1) X version of WI Module (Kee)"*/
/* *1    19-JUN-1989 13:36:57 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element WI_CHOWN_TAG.C*/

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

NAME:    WI_CHANGE_OWNER_TAG

DESCRIPTION:

    Change the owner tag for a Dialog or Menu

*************************************************************************/

#include "esi_wi_int.x"		/* external definitions needed here */

/************************************************************************/
INT wi_change_owner_tag(input_id, new_owner_tag)
/************************************************************************/

INT input_id;   /* %P Menu or Dialog ID from WI_RDF__ */
CHAR *new_owner_tag;   /* %P The owning application tag */

    {
    PROGNAME("wi_change_owner_tag");
#define IDENT progname
    INT id;
    INPUT_RECORD inp_rec;
    INT status;

    wiz_jp_name(IDENT);

    E_CHECK_INIT;

/* Retrieve from Tree */
    id = input_id;
    if (btfind(&wiv.input_tree, &id, &inp_rec))
        {
/* Update entry in tree. */
        strcpy(inp_rec.owner_tag, new_owner_tag);
        btrepcur(&wiv.input_tree, &id, &inp_rec);
/* change the title of the cd on the mac... */
        if  (input_id > 999)
            {
            wi_set_dialog_title(input_id, inp_rec.owner_title);
            }
        status = SUCCESS;
        }
    else
        {
/* Report not defined */
        status = WI_NOT_DEFINED;
        }

    RETURN(status);
    }
