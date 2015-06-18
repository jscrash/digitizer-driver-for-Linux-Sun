/* DEC/CMS REPLACEMENT HISTORY, Element WI_Q_FR_DLG.C*/
/* *3     2-MAR-1990 11:24:18 WALTERS "(SPR 6012) Fix #ifndef USE_X to #ifdef USE_X"*/
/* *2     5-FEB-1990 23:40:41 JULIAN "(SPR -1) X version of WI Module (Kee)"*/
/* *1    19-JUN-1989 13:38:13 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element WI_Q_FR_DLG.C*/
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

NAME:    WI_QUERY_FRONT_DIALOG

DESCRIPTION:

    Find out which window is in front.

    OutPut:

         cdlgid   = (INT *) custom dialog ID.

*************************************************************************/

#include "esi_wi_int.x"

/************************************************************************/
#define IDENT progname
INT wi_query_front_dialog (cdlgid_ptr)
/************************************************************************/

INT *cdlgid_ptr;
    {
    PROGNAME ("wi_query_front_dialog");

#ifdef USE_X
    E_CHECK_INIT;

    wiz_jp_name (IDENT);

    *cdlgid_ptr = wiv.frontdialog_id;    /* front dialog id stored in 
                                        wiv.frontdialog_id global variable */

#else        /* ESITERM code */

    E_CHECK_INIT;

    wiz_jp_name (IDENT);


    wiv.playback_nosend = 1;
    SEND_1_VALUE (MC_QUERY_FRONT_CD);
    wiv.playback_nosend = 0;

    /* Pick up report, count followed by the itemids. */
    wiz_set_echo (FALSE);
    if (WAIT_RESPONSE (RQWS_CODE))
        {
        wiz_gnum (cdlgid_ptr);
        wiz_acknowledge ();
        }
    wiz_set_echo (TRUE);
#endif
    RETURN (SUCCESS);
    }

