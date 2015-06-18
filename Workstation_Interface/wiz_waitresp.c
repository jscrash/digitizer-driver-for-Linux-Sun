/* DEC/CMS REPLACEMENT HISTORY, Element WIZ_WAITRESP.C*/
/* *4     2-MAR-1990 11:43:35 JULIAN "(SPR 6012) fix ifdef USE_X == 0"*/
/* *3     2-MAR-1990 11:14:48 JULIAN "(SPR 6012) fix up USE_X logic to be IFDEF only"*/
/* *2     5-FEB-1990 23:30:35 JULIAN "(SPR -1) X version of WI Module (Kee)"*/
/* *1    19-JUN-1989 13:36:25 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element WIZ_WAITRESP.C*/

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

NAME:    WIZ_WAIT_RESP

DESCRIPTION:

    Wait for a desired response code from the MAC.

    Input:

         progname = (CST) name of routine that requested response.
         desired_response = (INT) the response to wait for.

*************************************************************************/

#include "esi_wi_int.x"
#include "esi_ho.h"

/************************************************************************/
publicdef INT wiz_wait_resp(caller_name, desired_response)
/************************************************************************/

CHAR *caller_name;
INT desired_response;
{
    INT resp_code;
    INT i;
    INT result;

/************************************************************************/

/* Try a few times to get it. */
#ifndef USE_X 

    i = 0;

    while (wiz_gnum(&resp_code) != desired_response)
    {
         if(resp_code EQUALS ABORT_RESPONSE)
         {
                wiz_set_echo(TRUE);
                exit(EXIT_FAILURE);
         }
         else
         {
             if(i > MAX_BAD_RESPONSES)
             {
                  result = FALSE;
                  return result;
             }
             i++;
         }
    }
    result = TRUE;

    return result;
#endif	
}
