/* DEC/CMS REPLACEMENT HISTORY, Element WIZ_WAIT_EV.C*/
/* *6    15-JUL-1991 12:09:41 KEE "(SPR -1) Add Busy Cursor when execute Finder application code"*/
/* *5    23-OCT-1990 13:52:46 KEE "(SPR -1) Motif and DecWindow Merge"*/
/* *4    27-JUN-1990 17:25:28 KEE "(SPR -1) New wi data structure using 2-D array"*/
/* *3     2-MAR-1990 11:00:14 MING "(SPR 6012) mod USE_X"*/
/* *2     5-FEB-1990 23:30:42 JULIAN "(SPR -1) X version of WI Module (Kee)"*/
/* *1    19-JUN-1989 13:36:30 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element WIZ_WAIT_EV.C*/
/* DEC/CMS REPLACEMENT HISTORY, Element WIZ_WAIT_EV.C                               */
/* *4    27-JUN-1990 17:25:28 KEE "(SPR -1) New wi data structure using 2-D array"  */
/* *3     2-MAR-1990 11:00:14 MING "(SPR 6012) mod USE_X"                           */
/* *2     5-FEB-1990 23:30:42 JULIAN "(SPR -1) X version of WI Module (Kee)"        */
/* *1    19-JUN-1989 13:36:30 SYSTEM ""                                             */
/* DEC/CMS REPLACEMENT HISTORY, Element WIZ_WAIT_EV.C                               */
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

/* ************************************************************************         */
/*                                                                                  */
/*   NAME:    WIZ_WAIT_EVENT                                                        */
/*                                                                                  */
/*   DESCRIPTION:                                                                   */
/*                                                                                  */
/*   ************************************************************************       */

#include "esi_wi_int.x"        /* external definitions needed here */

#ifdef USE_X

#include "ansixt.h"

#endif

/* **********************************************************************           */
publicdef INT wiz_wait_event (event_class_ptr, object_id_ptr, item_id_ptr)  /* **********************************************************************   */

INT *event_class_ptr;
INT *object_id_ptr;
INT *item_id_ptr;

    {
    PROGNAME ("wiz_wait_event");

#ifdef USE_X
    INT cdlgid, itemid, eventclass;
#ifdef MOTIF  /* ---------- BEGIN MOTIF BLOCK ---------- */
    XtAppContext AppContext = XtWidgetToApplicationContext (wiv.toplevel);
#else  /* ---------- END MOTIF BLOCK - BEGIN DECW BLOCK ---------- */
    INT event_happened;
    XEvent event;
#endif  /* ---------- END DECW BLOCK ---------- */
    
    /********************************************************************************/
    
    wiz_clear_event_info ();        /* clear event info                             */
    wiz_busy_cursor(FALSE);
#ifdef MOTIF  /* ---------- BEGIN MOTIF BLOCK ---------- */
    FOREVER
        {
        XtAppProcessEvent (AppContext, ALL_EVENT_MASK);
        /* get event info                                                           */
        wiz_get_event_info (&cdlgid, &itemid, &eventclass);
        if (cdlgid != 0 && itemid != 0 && eventclass != 0)
            {
            *object_id_ptr = cdlgid;
            *item_id_ptr = itemid;
            *event_class_ptr = eventclass;
            wiz_clear_event_info ();
	    wiz_busy_cursor(TRUE);
            return(SUCCESS);
            }
        }
#else  /* ---------- END MOTIF BLOCK - BEGIN DECW BLOCK ---------- */
    FOREVER
        {
        XNextEvent (wiv.current_Display, &event);
        XtDispatchEvent (&event);
        if (event.type == ButtonRelease ||  /* 189 == <CR>                          */
            (event.type == KeyPress && event.xkey.keycode == 189))
            {
            /* get event info                                                       */
            wiz_get_event_info (&cdlgid, &itemid, &eventclass);
            if (cdlgid != 0 && itemid != 0 && eventclass != 0)
                {
                *object_id_ptr = cdlgid;
                *item_id_ptr = itemid;
                *event_class_ptr = eventclass;
                wiz_clear_event_info ();
		wiz_busy_cursor(TRUE);
                return(SUCCESS);
                }
            }
        }
#endif  /* ---------- END DECW BLOCK ---------- */

#else        /* ESITERM code */
    INT resp_code;
    INT iquit = FALSE;
    INT i;
    INT event_class;
    
    /* Get response from MAC.                                                       */
    wiz_set_echo (FALSE);
    while (iquit EQUALS FALSE)
        {
        wiz_gnum (&resp_code);
        
        /* Check for abort!                                                         */
        if (resp_code EQUALS ABORT_RESPONSE)
            {
            wiz_set_echo (TRUE);
            printf ("wiz_wait_ev: ABORT_RESPONSE.... Exiting!\n");
            exit (EXIT_FAILURE);
            }
        /* Scan for validity.                                                       */
#define BAD_EVENT -1
        event_class = BAD_EVENT;
        for (i = 0; i < NUM_EVENT_TYPES; i++)
            {
            if (wiv.event[i].event_code EQUALS resp_code)
                {
                event_class = i;
                i = NUM_EVENT_TYPES + 1;
                }
            }
        if (event_class EQUALS BAD_EVENT)
            {
            wiz_set_echo (TRUE);
            return WI_BAD_RESPONSE;
            }
        else
            {
            iquit = TRUE;
            }
        }
    /* Process valid event.                                                         */
    switch (event_class)
        {
    case WI_MENU_EVENT:
        wiz_gnum (object_id_ptr);
        wiz_gnum (item_id_ptr);
        break;
        
    case WI_FILE_MENU_EVENT:
        wiz_gnum (object_id_ptr);
        wiz_gnum (item_id_ptr);
        break;
        
    case WI_DIALOG_EVENT:
        wiz_gnum (object_id_ptr);
        wiv.hold.cdlgid = *object_id_ptr;
        wiz_gnum (item_id_ptr);
        wiv.hold.cditemid = *item_id_ptr;
        wiz_gnum (&wiv.hold.cdvalue);
        wiz_gstr (STMAX, wiv.hold.cdtext);
        break;
        
    case WI_SELECT_EVENT:
        wiz_gnum (object_id_ptr);
        *item_id_ptr = WI_SELECTED;
        }
    wiz_acknowledge ();
    wiz_set_echo (TRUE);
    *event_class_ptr = event_class;
    
    return event_class;
#endif
    }
