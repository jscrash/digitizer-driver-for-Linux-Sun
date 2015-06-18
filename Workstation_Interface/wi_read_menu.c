/* DEC/CMS REPLACEMENT HISTORY, Element WI_READ_MENU.C*/
/* *2     5-FEB-1990 23:43:02 JULIAN "(SPR -1) X version of WI Module (Kee)"*/
/* *1    19-JUN-1989 13:38:38 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element WI_READ_MENU.C*/

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

NAME:    WI_READ_MENU

DESCRIPTION:

    Read a menu resource into the menu structure.

    Input Parameters;

         mrsid = (INT) menu resource ID.
         themenu = (MENU *) menu structure pointer.

*************************************************************************/

#include "esi_c_lib.h"
#ifndef ESI_WI_INT_X
#include "esi_wi_int.x"
#endif
#ifndef ESI_RS_H
#include "esi_rs.h"
#endif

#define READ_RSRC { if(rsrcptr != NULL) \
                    { tempptr = rsgets(rsrcptr, linein); \
                      rsrcptr = tempptr; \
                    } else { \
                      linein[0] = NUL; }}

/************************************************************************/
publicdef INT wi_read_menu(mrsid, themenu)
/************************************************************************/

INT mrsid;
MENU *themenu;
{
    PROGNAME("wi_read_menu");
#define IDENT progname
    INT id;
    INT i;
    FILE *fpr;
    INT ifoundit;
    INT ls;
    CHAR linein[STMAX];
    CHAR *flag;
    CHAR cper;
    CHAR csd;
    CHAR cma;
    CHAR *rsrcptr;
    CHAR *tempptr;

/************************************************************************/

    wiz_jp_name(IDENT);

    E_CHECK_INIT;

/* Scan for menu resource ID. */
    ifoundit = rsfetch(mrsid, "MENU", &rsrcptr);
    if (ifoundit != TRUE)
    {
        RETURN(WI_RESOURCE_NOT_FOUND);
    }

/* Read into the menu data structure. */
    READ_RSRC;  /* Read Title. */
    sscanf(linein, "%s", themenu->mtitle);

/* Get number of menu items. */
/* default = no helpkey defined from rsrc file */
    wiv.current.rsrc_helpkey[0] = NUL;
    READ_RSRC;
    sscanf(linein, "%d %c %c %s",
            &themenu->num_mn_items, &csd, &cma, wiv.current.rsrc_helpkey);
    if (themenu->num_mn_items > MAX_MN_ITEMS)
    {
         RETURN(WI_TOO_MANY_MNI);
    }

/* Get flags for main/auxillary and static/dynamic. */
    switch (csd)   /* Static or Dynamic */
    {
    case 'S':
         themenu->ifstatic = TRUE;
         break;
    case 'D':
         themenu->ifstatic = FALSE;
         break;
    default:
         themenu->ifstatic = TRUE;
         RETURN(WI_BAD_STATIC_DYNAMIC);

    }

    switch (cma)   /* Main or Auxillary. */
    {
    case 'M':
         themenu->ifmain = TRUE;
         break;
    case 'A':
         themenu->ifmain = FALSE;
         break;
    default:
         themenu->ifmain = TRUE;
         RETURN(WI_BAD_MAIN_AUX);
    }

/* Read Items from Resource file. */
    if (themenu->num_mn_items > MAX_MN_ITEMS)
    {

         RETURN(WI_TOO_MANY_MNI);

    }
    for (i=0; i<themenu->num_mn_items; i++)
    {
         READ_RSRC;
         wiz_remove_cr(linein);
         strcpy(themenu->item[i].command, linein);
    }
    RETURN(SUCCESS);
}
