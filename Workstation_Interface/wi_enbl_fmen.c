/* DEC/CMS REPLACEMENT HISTORY, Element WI_ENBL_FMEN.C*/
/* *4     2-MAR-1990 12:07:19 JULIAN "(SPR 6012) fix if USE_X == 0 to be ifndef USE_X"*/
/* *3     2-MAR-1990 11:28:26 VINCE "(SPR 6012) Changed usage of USE_X symbol"*/
/* *2     5-FEB-1990 23:35:58 JULIAN "(SPR -1) X version of WI Module (Kee)"*/
/* *1    19-JUN-1989 13:37:23 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element WI_ENBL_FMEN.C*/

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

NAME:    WI_ENABLE_FILE_MENU_ITEM

DESCRIPTION:

    Enable 'file menu' Item for Picking.

    Input:

         cdlgid = (INT) custom dialog ID associated w/ file menu
         fmitemid = (INT) file menu item ID, 1 -> N.
         enable_flag  = (INT) flag for item,  0 or 1.

*************************************************************************/

#include "esi_wi_int.x"		/* external definitions required here */

/************************************************************************/
INT wi_enable_file_menu_item(cdlgid, fmitemid, enable_flag)
/************************************************************************/

INT cdlgid;
INT fmitemid;
INT enable_flag;

    {
    PROGNAME("wi_enable_file_menu_item");
#define IDENT progname
    INPUT_RECORD inp_rec;
    INT id;
    INT thisbit;

    wiz_jp_name(IDENT);

    E_CHECK_INIT;

    if (!((9001 <= fmitemid) && (fmitemid <= 9007)))
        {
        RETURN(WI_BAD_FMI);
        }
/* Now calc which bit & state, accumulate it, & update Mac... */
#ifndef USE_X
    id = cdlgid;
    if (btfind(&wiv.input_tree, &id, &inp_rec))
        {
        thisbit = (1 << (fmitemid-9001));
        if (enable_flag == 0)
            {
            thisbit = thisbit ^ -1;  /* complement */
            inp_rec.file_menu_mask = inp_rec.file_menu_mask & thisbit;
            }
        else
            {
            inp_rec.file_menu_mask = inp_rec.file_menu_mask | thisbit;
            }
        btrepcur(&wiv.input_tree, &id, &inp_rec);
        SEND_3_VALUES(inp_rec.file_menu_mask, cdlgid, MC_SET_FILE_MENU);
        RETURN(SUCCESS);
        }
    else
        {
        RETURN(WI_NOT_DEFINED);
		}
#endif		
    }
