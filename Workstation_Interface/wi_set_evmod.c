/* DEC/CMS REPLACEMENT HISTORY, Element WI_SET_EVMOD.C*/
/* *3     5-FEB-1990 23:47:36 JULIAN "(SPR -1) X version of WI Module (Kee)"*/
/* *2    11-AUG-1989 16:10:17 JULIAN "(SPR -1) redeclaration of wi_set_event_mode"*/
/* *1    19-JUN-1989 13:39:22 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element WI_SET_EVMOD.C*/

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

NAME:    WI_SET_EVENT_MODE

DESCRIPTION:

    Set input mode for a given class of event.

    Input:

         EVENT_CLASS = (INT) class of event to modify.
              This is typically a number between 0 & 15.

         IN_MODE     = (INT) Request, sample or event mode.

    Sample call:     wi_set_event_mode(WI_MENU_EVENT, WI_EVENT_MODE);

*************************************************************************/

#ifndef ESI_WI_INT_X
#include "esi_wi_int.x"
#endif

/************************************************************************/
INT wi_set_event_mode(event_class, in_mode)
/************************************************************************/

INT event_class;
INT in_mode;
{
    PROGNAME("wi_set_event_mode");
#define IDENT progname
/************************************************************************/

    wiz_jp_name(IDENT);

    E_CHECK_INIT;

    if ((event_class < 0) OR (event_class > NUM_EVENT_TYPES))
    {
#if 0
         erpi1(ERR_WI_BAD_EVENT_CLASS, progname, event_class);
         return(FAIL);
#endif
         RETURN(WI_BAD_EVENT_CLASS);
    }
    if((in_mode < WI_REQUEST_MODE) OR (in_mode > WI_EVENT_MODE))
    {
#if 0
         erpi1(ERR_WI_BAD_INPUT_MODE, in_mode);
           return(FAIL);
#endif
         RETURN(WI_BAD_INPUT_MODE);
    }

/* Set mode in event control structure. */
    wiv.event[event_class].mode = in_mode;

    RETURN(SUCCESS);
}
