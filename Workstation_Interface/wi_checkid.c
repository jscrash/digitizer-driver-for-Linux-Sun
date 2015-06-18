/* DEC/CMS REPLACEMENT HISTORY, Element WI_CHECKID.C*/
/* *2     5-FEB-1990 23:31:58 JULIAN "(SPR -1) X version of WI Module (Kee)"*/
/* *1    19-JUN-1989 13:36:49 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element WI_CHECKID.C*/
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

NAME:    WI_CHECKID.C

DESCRIPTION:

    This function accepts three arguments, a dlg or menu id, an item id,
    and a 'type' code.
    The item id is ignored if zero, otherwise it is validated as existing
    for the specified dialog (control) or menu (option).
    The type code is ignored if zero, otherwise is checked to be WI_DIALOG
    or WI_MENU...if one of those, it is also verified that, if defined,
    the specified id is that type of entity.

    This function returns either:
          SUCCESS        = unit presently defined
          WI_NOT_DEFINED = unit not presently defined

*************************************************************************/

#include "esi_wi_int.x"		/* external definitions needed here */

/************************************************************************/
INT wi_checkid(input_id,item_id, typecode)
/************************************************************************/

INT input_id;   /* %P Menu or Dialog ID from WI_RDF__ */
INT item_id;    /* if non-zero, check for validity    */
INT typecode;   /* if WI_DIALOG or WI_MENU, verify that the specified
                   id is that kind.  */
    {
    PROGNAME("wi_checkid");
#define IDENT progname
    INT id;
    INPUT_RECORD inp_rec;

    E_CHECK_INIT;

    id = input_id;

/* Check if we have a record for this thing... */
    if (btfind(&wiv.input_tree, &id, &inp_rec))
        {

/* Are we verifying the KIND of entity? */
        if (((typecode == WI_DIALOG) && (inp_rec.type != WI_DIALOG_EVENT))
                                      ||
           ((typecode == WI_MENU)   && (inp_rec.type != WI_MENU_EVENT)))
            {
            return(WI_NOT_DEFINED);
            }

/* Are we checking the item_id in bounds? */
        if (item_id)
            {
            if (item_id > inp_rec.numitems)
                {
                RETURN(WI_ITEM_NOT_DEFINED);
                }
            }
        RETURN(SUCCESS);
        }
    else
        {
        RETURN(WI_NOT_DEFINED);
        }
    }
