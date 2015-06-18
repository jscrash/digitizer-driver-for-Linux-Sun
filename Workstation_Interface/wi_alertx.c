/* DEC/CMS REPLACEMENT HISTORY, Element WI_ALERTX.C*/
/* *5    29-JUN-1990 10:14:21 KEE "(SPR -1) change wi_close_dialog to am_release_resource"*/
/* *4    21-MAY-1990 10:19:33 KEE "(SPR -1) change to all modal using fi_push and wi_request_event"*/
/* *3     2-MAR-1990 11:11:17 MING "(SPR 6012) mod USE_X"*/
/* *2     5-FEB-1990 23:31:22 JULIAN "(SPR -1) X version of WI Module (Kee)"*/
/* *1    19-JUN-1989 13:36:39 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element WI_ALERTX.C*/
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

NAME:    WI_ALERTX

DESCRIPTION:

    Put up a non-modal alert box on the MAC, using the currently
    set ownerid and tag.

    Input:

         LEVEL = (INT) Error severity level (not implemented).
         HELPKEY = (CST) not implemented (should be NULL).
         MESSAGE1 = (CST) short, general class or error.
                    (NULL may be passed for this parameter if only one
                     message is desired...)
         MESSAGE2 = (CST) longer, more descriptive error message.
         OWNER = (INT) Application id.
         TAG = (CST) Application tag.

*************************************************************************/

#include "esi_c_lib.h"
#include "esi_wi_int.x"        /* external definitions needed here */

#ifdef USE_X
#ifndef ESI_AM_H
#include "esi_am.h"
#endif
#include "wi_alertx.rh"
#endif

/************************************************************************/
publicdef INT wi_alertx(level, helpkey, message1, message2, owner, tag)
/************************************************************************/

INT level, owner;
CHAR *helpkey;
CHAR *message1;
CHAR *message2;
CHAR *tag;

    {
#define IDENT progname
#define MGMAX   241

    PROGNAME("wi_alertx");
    static CHAR alert_title[]=" Please Note!";
    CHAR tempbuff[MGMAX];
    INT cdlgid;
    
    INT event_class, event_id, event_item;

/* cat the messages into 1 string... */

    tempbuff[0] = 0;
    if (message1 != NULL)
        {
        strcat(tempbuff, message1);
        strcat(tempbuff,"\r \r");   /* the 'space' is needed! */
        }
    strcat(tempbuff,message2);

#ifdef USE_X

    am_declare(alert_title);
    am_define_resource(AM_DIALOG, "WI_ALERTX" , WI_ALERTX,
               NULL_FUNCTION_PTR, &cdlgid, NULL);
    wi_set_dialog_text(cdlgid,WI_ALERTX_MSG,tempbuff);
    fi_push();
    wi_open_dialog(cdlgid);
    for (event_item = -1; 
         event_item != WI_ALERTX_OKBTN;
	 wi_request_event(0,&event_class,&event_id,&event_item));
    am_release_resource(cdlgid);
    fi_pop();
#else        /* ESITERM code */
    if (wiv.ittype EQUALS TTYPE_ESITERM)
        {
#if 0      

/* now send the message text... */

        SEND_PROMPT(tempbuff);

/* create & send the resulting full_title... */

        tempbuff[0] = 0;
        wiz_prepend_tag_id(alert_title, tag, owner, tempbuff);
        SEND_DEFAULT(tempbuff);

/* and finally, send the integer ownerid and the command... */

        wiv.playback_nosend = 1;
        SEND_2_VALUES(owner, MC_ALERTX);
        wiv.playback_nosend = 0;

#endif
        }
    else   /* is not ESITerminal */
        {
        wi_message(tempbuff);
        }
#endif

    RETURN(SUCCESS);
    }
