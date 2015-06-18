/* DEC/CMS REPLACEMENT HISTORY, Element WI_DO_HELP.C*/
/* *3     5-FEB-1990 23:35:19 JULIAN "(SPR -1) X version of WI Module (Kee)"*/
/* *2    11-AUG-1989 16:09:15 JULIAN "(SPR -1) constant in conditional context"*/
/* *1    19-JUN-1989 13:37:16 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element WI_DO_HELP.C*/

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

/* ************************************************************************

   NAME:    WI_DO_HELP

   DESCRIPTION:

    Perform HELP dialog for an input object.

    Input:
         object_id = (INT) Menu or Dialog ID

   ************************************************************************ */

#include "esi_gl_defs.h"
#include "esi_wi_int.h"
#include "esi_hf.h"

/* ********************************************************************** */
#define IDENT "wi_do_help"
INT wi_do_help(object_id)
/* ********************************************************************** */

INT object_id;
{
   CHAR helpkey[STMAX];

/* ********************************************************************** */

    wiz_jp_name(IDENT);


/* Get help key    MOD BY JJC 12/05/86 temperary, we always go to the top
    of help directory.  QQQQ.

   if(wi_query_helpkey(object_id, helpkey) EQUALS SUCCESS)

*/
/*   Perform Help operation. */
        hf_do_help("HELP");
        RETURN(SUCCESS);
}
