/* DEC/CMS REPLACEMENT HISTORY, Element WI_BEEP.C*/
/* *3     2-MAR-1990 11:22:45 MING "(SPR 6012) mod USE_X"*/
/* *2     5-FEB-1990 23:31:50 JULIAN "(SPR -1) X version of WI Module (Kee)"*/
/* *1    19-JUN-1989 13:36:47 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element WI_BEEP.C*/
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

NAME:    WI_BEEP

DESCRIPTION:

    Make MAC sound a special beep

*************************************************************************/

#include "esi_wi_int.x"        /* external definitions needed here */

#ifdef USE_X
#include "ansixt.h"
#endif

/************************************************************************/
publicdef INT wi_beep(beepid)
/************************************************************************/

INT beepid;

    {
    PROGNAME("wi_beep");
#define IDENT progname

#ifdef USE_X
    INT i;

    wiz_jp_name(IDENT);

    /* beepid is either 1 or 2 */
    for (i=0; i<beepid; i++)
      XBell(wiv.current_Display,5);              
    /* send a beep sound the second argument of
       XBell range from -100 to 100 */

#else        /* ESITERM code */
    wiz_jp_name(IDENT);
    E_CHECK_INIT;
    SEND_2_VALUES(beepid,MC_BEEP);
#endif 

    RETURN(SUCCESS);
    }
