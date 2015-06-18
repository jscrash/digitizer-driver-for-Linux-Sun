/* DEC/CMS REPLACEMENT HISTORY, Element WI_SET_SEL.C*/
/* *2     5-FEB-1990 23:48:28 JULIAN "(SPR -1) X version of WI Module (Kee)"*/
/* *1    19-JUN-1989 13:39:32 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element WI_SET_SEL.C*/

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

NAME:    WI_SET_SELECTIONS

DESCRIPTION:

    Set list of selections in custom dialog selector.
    Sets range of scroll for associated scroll bar.

    Input:

         cdlgid = (INT) custom dialog ID
         cd_sel_id = (INT) selector item ID, 1 -> N.
         cd_scroll_id = (INT) associated scroll item ID, 1 -> N.
         num_selections = (INT) number of selections in list.
         max_selectable = (INT) maximum number selectable by user.
         select_list = (CHAR ** str) array of pointers to strings.

*************************************************************************/

#ifndef ESI_WI_INT_X
#include "esi_wi_int.x"
#endif

#ifndef ESI_TS_H
#include "esi_ts.h"
#endif

/************************************************************************/
INT wi_set_selections(cdlgid, cd_sel_id, cd_scroll_id, num_selections,
                max_selectable, select_list)
/************************************************************************/

INT cdlgid;
INT cd_sel_id;
INT cd_scroll_id;
INT num_selections;
INT max_selectable;
CHAR **select_list;
{
    INT status;
    PROGNAME("wi_set_selections");
#define IDENT progname
    CHAR *packed_string; /* Point to selections packed in dynamic memory. */

/************************************************************************/

    wiz_jp_name(IDENT);

    E_CHECK_INIT;
    E_CHECK_DIALOGITEM(cd_sel_id);
    E_CHECK_DIALOGITEM(cd_scroll_id);
    E_CHECK_ID(cdlgid,cd_sel_id,WI_DIALOG);
    E_CHECK_ID(cdlgid,cd_scroll_id,0);

#if 0
    if (max_selectable > MAX_CD_SELECTABLE)
    {
         RETURN(WI_TOO_MANY_SELECTIONS);
    }
#endif    

    if (num_selections > 0)
    {
/* Pack text items into long string with <CR> after each item. */
        packed_string = ts_pack_strings(num_selections, select_list);

/* Send text. */
        wi_set_dialog_text(cdlgid, cd_sel_id, packed_string);
        tc_free(packed_string);
    }
    else
    {
        wi_set_dialog_text(cdlgid, cd_sel_id, "<--->");
    }

/* Set range for scroll bar. */
    wi_set_dialog_range(cdlgid, cd_scroll_id, 0, MAX(1,(num_selections-2)));

/* Set maximum number of items selectable. */
    wi_set_selection_limit(cdlgid, cd_sel_id, MIN(max_selectable, num_selections));

    RETURN(SUCCESS);
}
