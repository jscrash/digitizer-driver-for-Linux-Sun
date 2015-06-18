/* DEC/CMS REPLACEMENT HISTORY, Element WI_RQ_STR.C*/
/* *6    23-OCT-1990 13:56:46 KEE "(SPR -1) Motif and DecWindow Merge"*/
/* *5    24-SEP-1990 17:31:04 GILLESPIE "Beta Deltas"*/
/* *4    27-JUN-1990 17:31:43 KEE "(SPR -1) New wi data structure using 2-D array"*/
/* *3     2-MAR-1990 12:15:06 JULIAN "(SPR 6012) fix if USE_X to be ifdef USE_X"*/
/* *2     5-FEB-1990 23:45:04 JULIAN "(SPR -1) X version of WI Module (Kee)"*/
/* *1    19-JUN-1989 13:39:02 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element WI_RQ_STR.C*/
/* DEC/CMS REPLACEMENT HISTORY, Element WI_RQ_STR.C                                 */
/* *4    27-JUN-1990 17:31:43 KEE "(SPR -1) New wi data structure using 2-D array"  */
/* *3     2-MAR-1990 12:15:06 JULIAN "(SPR 6012) fix if USE_X to be ifdef USE_X"    */
/* *2     5-FEB-1990 23:45:04 JULIAN "(SPR -1) X version of WI Module (Kee)"        */
/* *1    19-JUN-1989 13:39:02 SYSTEM ""                                             */
/* DEC/CMS REPLACEMENT HISTORY, Element WI_RQ_STR.C                                 */
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
/*NAME:    WI_REQUEST_STRING                                                        */
/*                                                                                  */
/*DESCRIPTION:                                                                      */
/*                                                                                  */
/*    Request String from the user.                                                 */
/*                                                                                  */
/*    Input:                                                                        */
/*                                                                                  */
/*         STPROMPT = (CST) prompt string.                                          */
/*         STDEFAULT = (CST) default response from user.                            */
/*         HELPKEY   = (CST) key into help tree.                                    */
/*         LENRESP   = (INT) maximum length of response string.                     */
/*                                                                                  */
/*     Output:                                                                      */
/*                                                                                  */
/*         stresponse = (CST) response from user.                                   */
/*         status    = (INT *) completion code.                                     */
/*                     CANCEL returns FAIL                                          */
/*                     OK returns SUCCESS                                           */
/*                                                                                  */
/*************************************************************************          */

#include "esi_wi_int.x"

#ifndef ESI_HF_H
#include "esi_hf.h"
#endif

#ifdef USE_X
#ifndef ESI_AM_H
#include "esi_am.h"
#endif
#ifndef ESI_FI_H
#include "esi_fi.h"
#endif
#include "wi_rqstr.rh"
#endif

/************************************************************************************/
#define IDENT progname
publicdef INT wi_request_string (stprompt, stdefault, helpkey, lenresp, stresponse,
                                 status)    /*************************************************************************/

CHAR *stprompt;
CHAR *stdefault;
CHAR *helpkey;
CHAR *stresponse;
INT lenresp;
INT *status;
    {
    PROGNAME ("wi_request_string");

#ifdef USE_X
    INT cdlgid, value;
    INT event_class_ptr, cdlg_ptr, item_ptr;
    CHAR st1[ST_MAC_MAX];
    
    E_CHECK_INIT;
    E_CHECK_SLEN (stprompt);
    
    wiz_jp_name (IDENT);
    
    am_declare ("ESITerm-Resident Dialog Box");
    am_define_resource (AM_DIALOG, "WI_RQSTR", WI_RQSTR_CDLG, NULL_FUNCTION_PTR,
                        &cdlgid, NULL);
    
    wi_set_dialog_text (cdlgid, WI_RQSTR_STATTEXT, stprompt);
    wi_set_dialog_text (cdlgid, WI_RQSTR_EDITTEXT, stdefault);
    wi_set_dialog_range (cdlgid, WI_RQSTR_EDITTEXT, 1, lenresp);
    fi_push ();
    wi_open_dialog (cdlgid);
    while (TRUE)
        {
        wi_request_event (0, &event_class_ptr, &cdlg_ptr, &item_ptr);
        /* if dialog item event                                                     */
        if (event_class_ptr == WI_DIALOG_EVENT && cdlgid == cdlg_ptr)
            {
            wi_query_dialog_item (cdlgid, WI_RQSTR_EDITTEXT, &value, stresponse);
            /* get the string                                                       */
            wi_close_dialog (cdlgid);   /* close request dialog                     */
            *status = (item_ptr == WI_RQSTR_OKBTN) ? SUCCESS : FAIL;
            fi_pop ();
            return(*status);
            }
        }
#else        /* ESITERM code */
    INT data_code;
    INT iquit = FALSE;
    
    E_CHECK_INIT;
    E_CHECK_SLEN (stprompt);
    
    wiz_jp_name (IDENT);
    wiz_set_echo (FALSE);
    while (iquit EQUALS FALSE)
        {
        /* Send multiple commands.                                                  */
        
        wiv.playback_nosend = 1;
        SEND_DEFAULT (stdefault);   /* Load up prompt and default strings.s         */
        SEND_PROMPT (stprompt);
        SEND_1_VALUE (MC_REQUEST_ST);
        wiv.playback_nosend = 0;
        
        if (WAIT_RESPONSE (RQST_CODE))
            {
            wiz_gnum (&data_code);  /* Get response from MAC                        */
            
            switch (data_code)
                {
            case CANCEL_CODE:
                *status = FAIL;
                iquit = TRUE;
                break;
                
            case HELP_CODE:
                wiz_acknowledge ();
                hf_do_help (helpkey);
                break;
                
            case OK_CODE:
                *status = SUCCESS;
                iquit = TRUE;
                wiz_gstr (lenresp, stresponse);
                break;
                
            default:
                wiz_set_echo (TRUE);
                RETURN (WI_BAD_RESPONSE);
                }
            wiz_acknowledge ();
            }
        wiz_set_echo (TRUE);
        }
    RETURN (SUCCESS);
#endif 
    }
