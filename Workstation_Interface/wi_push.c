/* DEC/CMS REPLACEMENT HISTORY, Element WI_PUSH.C*/
/* *6    23-OCT-1990 13:55:52 KEE "(SPR -1) Motif and DecWindow Merge"*/
/* *5    27-JUN-1990 17:30:20 KEE "(SPR -1) New wi data structure using 2-D array"*/
/* *4     3-MAY-1990 21:49:13 KEE "(SPR -1) menu sync fix."*/
/* *3     2-MAR-1990 11:34:05 VINCE "(SPR 6012) Changed usage of USE_X symbol"*/
/* *2     5-FEB-1990 23:39:01 JULIAN "(SPR -1) X version of WI Module (Kee)"*/
/* *1    19-JUN-1989 13:37:58 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element WI_PUSH.C*/
/* DEC/CMS REPLACEMENT HISTORY, Element WI_PUSH.C                                   */
/* *5    27-JUN-1990 17:30:20 KEE "(SPR -1) New wi data structure using 2-D array"  */
/* *4     3-MAY-1990 21:49:13 KEE "(SPR -1) menu sync fix."                         */
/* *3     2-MAR-1990 11:34:05 VINCE "(SPR 6012) Changed usage of USE_X symbol"      */
/* *2     5-FEB-1990 23:39:01 JULIAN "(SPR -1) X version of WI Module (Kee)"        */
/* *1    19-JUN-1989 13:37:58 SYSTEM ""                                             */
/* DEC/CMS REPLACEMENT HISTORY, Element WI_PUSH.C                                   */
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
/*NAME:    WI_PUSH                                                                  */
/*                                                                                  */
/*DESCRIPTION:                                                                      */
/*                                                                                  */
/*    Push interactive environment to nest interactions.                            */
/*    Disables all dialog boxes, removes all rolling menus.                         */
/*                                                                                  */
/*************************************************************************          */

#ifndef ESI_WI_INT_X
#include "esi_wi_int.x"
#endif

#ifdef USE_X
#include "ansixt.h"
#endif

/************************************************************************************/
publicdef INT wi_push ()            /*************************************************************************/
    {
    PROGNAME ("wi_push");
#define IDENT progname
    INT status = SUCCESS;
    INT i;

#ifdef USE_X
    /********************************************************************************/
    
    wiz_jp_name (IDENT);
    
    E_CHECK_INIT;
    
    for (i = 0; i < NUM_EVENT_TYPES; i++)
        {
        status = wiz_push_int (wiv.event[i].mode);
        if (status != SUCCESS)
            {
            break;
            }
        /* Set to default modes.                                                    */
        wiv.event[i].mode = WI_REQUEST_MODE;
        }
    wiv.wi_push_pop_top++;          /* update push/pop pointer                      */
    for (i = 1; i <= wiv.dialog_menu_count; i++)
        {
        E_CHECK_WIDGET (i, 0);
        if ((E_DIALOG_MENU (i))->num_items != 0)    /* not delete                   */
            {
            if (XtIsManaged (E_GET_WIDGET (i, 0)) == TRUE)
                {
                if ((E_WIDGET_INFO (i, 0))->type_id == DIALOG &&
                    (E_DIALOG_MENU (i))->wi_push_pop_num == 0)
                    {
                    XtSetSensitive (E_GET_WIDGET (i, 0), FALSE);
                    (E_DIALOG_MENU (i))->wi_push_pop_num = wiv.wi_push_pop_top;
                    /* update push/pop num                                          */
                    }
                else if ((E_WIDGET_INFO (i, 0))->type_id == DMENU)
                    {
                    /* wiv.dialog_menu[i].wi_push_pop_num is always equal 0         */
                    /*   for menu if it is still on the screen becuase if it is     */
                    /*   begin pushed, it will be wi_enable_menu(i,OFF) and         */
                    /*   no longer on the screen                                    */
                    wiz_unmanagechild_widget (i);   /* close the menu               */
                    (E_DIALOG_MENU (i))->wi_push_pop_num = wiv.wi_push_pop_top;
                    }
                /* update push/pop num                                              */
                }
            }
        }
#else        /* ESITERM code */
    wiz_jp_name (IDENT);
    
    E_CHECK_INIT;
    
    SEND_1OPCODE (MC_PUSH);
    /* Push input mode control onto stack.                                          */
    for (i = 0; i < NUM_EVENT_TYPES; i++)
        {
        status = wiz_push_int (wiv.event[i].mode);
        if (status != SUCCESS)
            {
            break;
            }
        /* Set to default modes.                                                    */
        wiv.event[i].mode = WI_REQUEST_MODE;
        }
#if DEBUG
    printf ("WI_PUSH - stackpointer = %d\n", wi_stack_pointer);
#endif
#endif
    RETURN (status);
    }
