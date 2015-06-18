/* DEC/CMS REPLACEMENT HISTORY, Element WI_RQ_EVENT.C*/
/* *5    23-OCT-1990 13:56:39 KEE "(SPR -1) Motif and DecWindow Merge"*/
/* *4     2-MAR-1990 12:14:57 JULIAN "(SPR 6012) fix if USE_X to be ifdef USE_X"*/
/* *3     6-FEB-1990 00:13:20 JULIAN "(SPR -1) X Version of WI Module"*/
/* *2    18-NOV-1989 10:29:35 GILLESPIE "(SPR 5075) Global overhaul"*/
/* *1    19-JUN-1989 13:38:53 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element WI_RQ_EVENT.C*/
/* DEC/CMS REPLACEMENT HISTORY, Element WI_RQ_EVENT.C                               */
/* *4     2-MAR-1990 12:14:57 JULIAN "(SPR 6012) fix if USE_X to be ifdef USE_X"    */
/* *3     6-FEB-1990 00:13:20 JULIAN "(SPR -1) X Version of WI Module"              */
/* *2    18-NOV-1989 10:29:35 GILLESPIE "(SPR 5075) Global overhaul"                */
/* *1    19-JUN-1989 13:38:53 SYSTEM ""                                             */
/* DEC/CMS REPLACEMENT HISTORY, Element WI_RQ_EVENT.C                               */
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
/*NAME:    WI_REQUEST_EVENT                                                         */
/*                                                                                  */
/*DESCRIPTION:                                                                      */
/*                                                                                  */
/*    Wait for an event from an input device.                                       */
/*                                                                                  */
/*    Input:                                                                        */
/*                                                                                  */
/*         TIMEOUT   = (INT) Time in seconds to wait. (NOT IMPLEMENTED)             */
/*                                                                                  */
/*    Output:                                                                       */
/*                                                                                  */
/*         EVENT_CLASS_PTR = (INT *) Class that of event that ocurred.              */
/*                                                                                  */
/*         OBJECT_ID_PTR   = (INT *) ID of Object associated with event.            */
/*              For a menu, this would be the menu ID.                              */
/*              For a dialog, the dialog ID.                                        */
/*              For a file menu, this is it's attached CD.                          */
/*              For some event classes, there is no object ID.                      */
/*                                                                                  */
/*         ITEM_ID_PTR     = (INT *) Item ID.                                       */
/*              Eg. Menu item ID, or Dialog item ID.                                */
/*              for a file menu, this is the file item.                             */
/*                                                                                  */
/*************************************************************************          */

#include "esi_wi_int.x"
#include "esi_tc.h"

/************************************************************************************/
publicdef INT wi_request_event (timeout, event_class_ptr, object_id_ptr, item_id_ptr)   
/************************************************************************************/

INT timeout;
INT *event_class_ptr;
INT *object_id_ptr;
INT *item_id_ptr;
    {
    PROGNAME ("wi_request_event");
#define IDENT progname
    INT num_active;
    
    INT event_consumed, status;
    CALLBACK_STRUCT *list_ptr;
    /********************************************************************************/
    
    E_CHECK_INIT;
    
    wiz_jp_name (progname);
    
    wiv.playback_nosend = 1;        /* we don't really want this to go out to the mac   */
    
    do
        {
        event_consumed = FALSE;
        
        num_active = 0;
        
        /* Request Dialogs first so that the menu handler will know                 */
        /*    whether or not to put up the "Pick from Menus window" %?              */

#ifdef USE_X
        if (wiv.event[WI_DIALOG_EVENT].mode == WI_EVENT_MODE)
            {
            num_active++;
            }
        if (wiv.event[WI_MENU_EVENT].mode == WI_EVENT_MODE)
            {
            num_active++;
            }
        if (wiv.event[WI_SELECT_EVENT].mode == WI_EVENT_MODE)
            {
            num_active++;
            }
#else        /* ESITERM code */
        if (wiv.event[WI_DIALOG_EVENT].mode == WI_EVENT_MODE)
            {
            num_active++;
            SEND_1_VALUE (MC_REQUEST_CD);
            }
        if (wiv.event[WI_MENU_EVENT].mode == WI_EVENT_MODE)
            {
            num_active++;
            SEND_1_VALUE (MC_REQUEST_MN);
            }
        if (wiv.event[WI_SELECT_EVENT].mode == WI_EVENT_MODE)
            {
            num_active++;
            SEND_1_VALUE (MC_REQUEST_CD_HIT);
            }
#endif
        wiv.playback_nosend = 0;    /* turn it back on                              */
        
        /* Just return if none in event mode.                                       */
        if (num_active EQUALS 0)
            {
            RETURN (WI_NONE_EVENT_MODE);
            }
        /* Wait for an event.                                                       */
        
        wiz_wait_event (event_class_ptr, object_id_ptr, item_id_ptr);
        
        /* For every event, run through the list of event callback functions        */
        /* (that is built by WI_ADD_CALLBACK) and call each one.                    */
        for (list_ptr = wiv.callback_list; list_ptr != (CALLBACK_STRUCT *)NULL;
             list_ptr = list_ptr->next)
            {
            status = (*list_ptr->function)(*object_id_ptr, *item_id_ptr,
                                           list_ptr->pointer);
            /* did that call consume the event ???                                  */
            if (status == SUCCESS)
                {
                event_consumed = TRUE;  /* yep, so go back to waiting               */
                break;
                }
            }
        } while (event_consumed);
    
    return *event_class_ptr;
    
    }

/*************************************************************************          */
/*                                                                                  */
/*NAME:    WI_ADD_CALLBACK                                                          */
/*                                                                                  */
/*DESCRIPTION:                                                                      */
/*                                                                                  */
/*    Add a function's address to the callback list.                                */
/*                                                                                  */
/*    Input:                                                                        */
/*                                                                                  */
/*         ADDR = pointer to the function that the caller wishes to have            */
/*                control passed to for every event.  This function should          */
/*                have the following prototype ...                                  */
/*                                                                                  */
/*                  INT function (INT menuid, INT itemid, BYTE *ptr)                */
/*                                                                                  */
/*                where, MENUID and ITEMID identify which event occurred, and       */
/*                PTR is an address provided by WI_ADD_CALLBACK's caller.           */
/*                                                                                  */
/*         PTR  = an address value to be passed to the callback function when       */
/*                it is called.  WI_ADD_CALLBACK's caller can use this pointer      */
/*                to communicate data to the callback function.                     */
/*    Output:                                                                       */
/*                                                                                  */
/*         a PASS/FAIL status value is returned.                                    */
/*                                                                                  */
/*************************************************************************          */

publicdef INT wi_add_callback (addr, ptr)
INT (*addr)();
BYTE *ptr;

    {                               /* WI_ADD_CALLBACK                              */
    CALLBACK_STRUCT *list_ptr, *last_ptr, *new_item;
    
    /* locate the last element in the callback list                                 */
    
    for (list_ptr = last_ptr = wiv.callback_list; list_ptr != (CALLBACK_STRUCT *)NULL;
         list_ptr = list_ptr->next)
        last_ptr = list_ptr;
    
    /* allocate and initialize a new element                                        */
    
    new_item = (CALLBACK_STRUCT *) tc_alloc (sizeof(CALLBACK_STRUCT));
    if (new_item == (CALLBACK_STRUCT *)NULL)
        {
        /* handle allocation FAILURES here                                          */
        }
    new_item->function = addr;
    new_item->pointer = ptr;
    new_item->next = (CALLBACK_STRUCT *)NULL;
    
    /* link the new one onto the end of the callback list                           */
    
    if (wiv.callback_list != (CALLBACK_STRUCT *)NULL)
        last_ptr->next = new_item;
    else
        wiv.callback_list = new_item;
    
    return SUCCESS;
    }

/* WI_ADD_CALLBACK                                                                  */

/*************************************************************************          */
/*                                                                                  */
/*NAME:    WI_REMOVE_CALLBACK                                                       */
/*                                                                                  */
/*DESCRIPTION:                                                                      */
/*                                                                                  */
/*    Remove a function's address from the callback list.                           */
/*                                                                                  */
/*    Input:                                                                        */
/*                                                                                  */
/*         ADDR = pointer to the function that the caller wishes to have            */
/*                control passed to for every event.  This function should          */
/*                have been previously added to the callback list using the         */
/*                function WI_ADD_CALLBACK.                                         */
/*                                                                                  */
/*                In the event that WI_REMOVE_CALLBACK can NOT locate ADDR          */
/*                within the callback list, it will take NO action; but             */
/*                will return SUCCESS.                                              */
/*                                                                                  */
/*    Output:                                                                       */
/*                                                                                  */
/*         a PASS/FAIL status value is returned.                                    */
/*                                                                                  */
/*************************************************************************          */

publicdef INT wi_remove_callback (addr)
INT (*addr)();

    {                               /* WI_REMOVE_CALLBACK                           */
    INT status = SUCCESS;
    CALLBACK_STRUCT *list_ptr, *last_ptr;
    
    /* Run through the list of event callback functions, and locate one             */
    /* whose FUNCTION field matches the caller's ADDR.                              */
    
    for (list_ptr = last_ptr = wiv.callback_list; list_ptr != (CALLBACK_STRUCT *)NULL;
         last_ptr = list_ptr, list_ptr = list_ptr->next)
        {
        if (list_ptr->function == addr)
            break;
        }
    /* If a match was found, remove it from the list, and free it.                  */
    
    if (list_ptr != (CALLBACK_STRUCT *)NULL)
        {
        if (last_ptr == list_ptr)   /* head of list ???                             */
            wiv.callback_list = list_ptr->next; /* unchain it ...                   */
        else
            last_ptr->next = list_ptr->next;    /* unchain it ...                   */
        tc_free (list_ptr);         /* then free it                                 */
        }
    return SUCCESS;
    }

/* WI_REMOVE_CALLBACK                                                               */
