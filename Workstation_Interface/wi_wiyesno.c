/* DEC/CMS REPLACEMENT HISTORY, Element WI_WIYESNO.C*/
/* *3     5-FEB-1990 23:49:58 JULIAN "(SPR -1) X version of WI Module (Kee)"*/
/* *2    22-JUN-1989 16:13:12 GILLESPIE "(SPR 20) change NULL to NULL_FUNCTION_PTR in am_define_resource"*/
/* *1    19-JUN-1989 13:39:51 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element WI_WIYESNO.C*/

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

/* WIYESNO.CC,  JOHN>CC>SOURCE,  J GILLESPIE,  08/27/86
   prompt user for a yes/no response
   Copyright (c) 1986, Exploration Systems Inc, Sausalito, CA. */

/* TITLE : WIYESNO - prompt user for a yes/no response */

/* START-HISTORY :
   08/27/86 CATHEY   no changes
   03/20/86 BILLJ    Remove all the extraneous buttons
   01/03/86 JOHN     Initial coding
   01/03/86 J GILLESPIE Template constructed
   END-HISTORY */

/* START-DESCRIPTION :
    description to be inserted
   END-DESCRIPTION */

/* START-DESIGN :
   WIYESNO:
   BEGIN
    design to be inserted here
   END
   END-DESIGN */

/* START-CODE : */

#include "esi_am.h"
#include "wi_yesno.rh"
#include "esi_wi.h"

INT wiyesno(prompt, helpfil, ifyes, status)
CHAR *prompt;
CHAR *helpfil;
BOOL *ifyes;
INT *status;
{
    INT cdlgid;
    INT id;
    INT item;
    INT value;
    CHAR text[30];

#define IDENT "wiyesno"

    wiz_jp_name(IDENT);

    wipush();
    am_define_resource(AM_DIALOG, "WI_YESNO", WIYESNO_CDLG,
               NULL_FUNCTION_PTR, &cdlgid, NULL);    /* read the dialog box */

    wiscdt(cdlgid, YESNO_QUESTION, prompt);    /* set the text */
    wiopcd(cdlgid);
    wirqcdi(&id,&item,&value,text);
    switch (item)
       {
       case YESNO_YES:
          *ifyes = 1;
          break;
       case YESNO_NO:
          *ifyes = 0;
          break;
       }
    am_release_resource(cdlgid);
    *status = SUCCESS;
    wipop();
    return *status;
}
/* END-CODE */
