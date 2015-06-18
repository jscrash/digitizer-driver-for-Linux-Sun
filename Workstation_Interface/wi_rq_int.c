/* DEC/CMS REPLACEMENT HISTORY, Element WI_RQ_INT.C*/
/* *2     5-FEB-1990 23:44:31 JULIAN "(SPR -1) X version of WI Module (Kee)"*/
/* *1    19-JUN-1989 13:38:56 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element WI_RQ_INT.C*/
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

NAME:    WI_REQUEST_INTEGER

DESCRIPTION:

    Query the user for a value within a given range.

    Input:

         STPROMPT = (CST) prompt string.
         HELPKEY   = (CST) key into help tree.
         VALMIN     = (INT) minimum value of integer.
         VALMAX     = (INT) maximum value of integer.
             Set the two above to zero for no range checking.
         VALDEFAULT = (INT) default value for thevalue.

    Output:

         thevalue   = (INT) integer value from user.
         status     = (INT *) completion code.
                      CANCEL returns FAIL.
                      OK + valid integer returns SUCCESS.

*************************************************************************/

#include "esi_gl_defs.h"
#ifndef ESI_WI_INT_X
#include "esi_wi_int.x"
#endif
#ifndef ESI_C_LIB_H
#include "esi_c_lib.h"
#endif

/************************************************************************/
#define IDENT "wi_request_integer"
INT wi_request_integer(stprompt,
              helpkey, valmin, valmax,
              valdefault, thevalue, status)
/************************************************************************/

CHAR stprompt[STMAX], *helpkey;
INT valmin, valmax, valdefault, *thevalue;
INT *status;
{
    INT if_limits, iquit;
    CHAR stresponse[STMAX], stdefault[32], sttemp[ST_MAC_MAX];
    CHAR strange[STMAX];

/************************************************************************/

    wiz_jp_name(IDENT);

    E_CHECK_INIT;
    E_CHECK_SLEN(stprompt);

/* Set default string to default value. */
    sprintf(stdefault,"%d", valdefault);

    strcpy(sttemp, stprompt);

/* Set prompt to reflect limits. */
    if (valmax > valmin)
    {
         if_limits = TRUE;
         if (strlen(stprompt) < (ST_MAC_MAX - 50))
         {
              strcat(sttemp,"\015"); /* Bare CR */
              sprintf(strange,
              "( Value must be in the range %d to %d)",
              valmin, valmax);
              strcat(sttemp, strange);
          }
     }
     else
     {
         if_limits = FALSE;
     }

/* Loop until valid number or cancel is reached. */
    iquit = FALSE;
    while (iquit == FALSE)
    {
         wi_request_string(sttemp, stdefault, helpkey, STMAX, stresponse, status);

/* Check for cancellation. */
         if (*status == FAIL)
         {
              iquit = TRUE;
         }
         else
         {

/* Check range of number. */
              sscanf(stresponse, "%d", thevalue);
              if (if_limits && ((*thevalue < valmin) || (*thevalue > valmax)))
              {
                   wi_beep(1);
                   wi_message("Value is Out of Range.");
              }
              else
              {
                   iquit = TRUE;
              }
          }
      }
      return(*status);
}
