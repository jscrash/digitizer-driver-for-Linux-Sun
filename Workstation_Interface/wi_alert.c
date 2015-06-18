/* DEC/CMS REPLACEMENT HISTORY, Element WI_ALERT.C*/
/* *6    23-OCT-1990 13:53:33 KEE "(SPR -1) Motif and DecWindow Merge"*/
/* *5    21-MAY-1990 10:19:23 KEE "(SPR -1) change to all modal using fi_push and wi_request_event"*/
/* *4    12-APR-1990 20:54:52 JULIAN "(SPR -1) changed ansiDwt to ansidwt"*/
/* *3     2-MAR-1990 11:09:07 MING "(SPR 6012) mod USE_X"*/
/* *2     5-FEB-1990 23:31:14 JULIAN "(SPR -1) X version of WI Module (Kee)"*/
/* *1    19-JUN-1989 13:36:37 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element WI_ALERT.C*/
/* DEC/CMS REPLACEMENT HISTORY, Element WI_ALERT.C                                  */
/* *5    21-MAY-1990 10:19:23 KEE "(SPR -1) change to all modal using fi_push and wi_request_event" */
/* *4    12-APR-1990 20:54:52 JULIAN "(SPR -1) changed ansiDwt to ansidwt"          */
/* *3     2-MAR-1990 11:09:07 MING "(SPR 6012) mod USE_X"                           */
/* *2     5-FEB-1990 23:31:14 JULIAN "(SPR -1) X version of WI Module (Kee)"        */
/* *1    19-JUN-1989 13:36:37 SYSTEM ""                                             */
/* DEC/CMS REPLACEMENT HISTORY, Element WI_ALERT.C                                  */
/******************************************************************************     */
/*                                                                                  */
/*                Copyright Finder Graphics Systems, Inc. 1989                      */
/*           Unpublished -- All rights reserved                                     */
/*                                                                                  */
/*THIS SOFTWARE IS THE PROPRIETARY PROPERTY OF Finder Graphics Systems, Inc.  AND   */
/*MAY CONTAIN CONFIDENTIAL TRADE SECRET INFORMATION. IT IS LICENSED FOR USE ON THE  */
/*DESIGNATED EQUIPMENT ON WHICH IT WAS ORIGINALLY INSTALLED AND  MAY NOT BE         */
/*MODIFIED, DUPLICATED OR COPIED IN ANY FORM WITHOUT PRIOR WRITTEN CONSENT OF       */
/*                                                                                  */
/*            Finder Graphics Systems, Inc.                                         */
/*            201 Tamal Vista Blvd                                                  */
/*            Corte Madera, CA  USA 94925                                           */
/*            (415) 927-0100                                                        */
/*                                                                                  */
/*(The above notice does not constitute an admission of publication or              */
/*unrestricted dissemination of the work)                                           */
/*                                                                                  */
/******************************************************************************     */


/*************************************************************************          */
/*                                                                                  */
/*NAME:    WI_ALERT                                                                 */
/*                                                                                  */
/*DESCRIPTION:                                                                      */
/*                                                                                  */
/*    Put up an alert box on the MAC with errors.                                   */
/*                                                                                  */
/*    Input:                                                                        */
/*                                                                                  */
/*         LEVEL = (INT) Error severity level.                                      */
/*         HELPKEY   = (CST) key into help tree.                                    */
/*         MESSAGE1 = (CST) part 1 of the error message.                            */
/*         MESSAGE2 = (CST) part 2 of the error message.                            */
/*                                                                                  */
/*************************************************************************          */

#include "esi_wi_int.x"        /* external definitions needed here */
#include "esi_hf.h"

/************************************************************************************/
publicdef INT wi_alert (level, helpkey, message1, message2) /*************************************************************************/

CHAR *message1;
CHAR *message2;
CHAR *helpkey;
INT level;

    {
#define IDENT progname
    PROGNAME ("wi_alert");

#ifdef USE_X
    wi_alertx (level, helpkey, message1, message2, 0, "");
#else        /* ESITERM code */
    INT status;
    wiz_jp_name (IDENT);
    
    /* Send multiple commands.                                                      */
    
    if (wiv.ittype EQUALS TTYPE_ESITERM)
        {
        
        wiv.playback_nosend = 1;
        SEND_PROMPT (message1);     /* Load up prompt and default strings.s         */
        SEND_DEFAULT (message2);
        SEND_1_VALUE (MC_ALERT);
        wiv.playback_nosend = 0;
        
        wiz_set_echo (FALSE);
        if (WAIT_RESPONSE (ALERT_CODE))
            {
            wiz_gnum (&status);     /* Get response from MAC                        */
            wiz_acknowledge ();
            }
        wiz_set_echo (TRUE);
        /* Interpret status.                                                        */
        switch (status)
            {
        case HELP_CODE:
            hf_do_help (helpkey);
            break;
            
        case OK_CODE:
            break;
            }
        }
    else                            /* is not ESITerminal                           */
        {
        wi_message (message1);
        wi_message (message2);
        }
#endif
    RETURN (SUCCESS);
    }
