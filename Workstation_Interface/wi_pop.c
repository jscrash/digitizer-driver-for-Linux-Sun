/* DEC/CMS REPLACEMENT HISTORY, Element WI_POP.C*/
/* *8    15-JUL-1991 12:10:11 KEE "(SPR -1) Add Busy Cursor when execute Finder application code"*/
/* *7     8-NOV-1990 17:03:00 MING "(SPR 6158) reset the frontdialog_id in wiv"*/
/* *6    23-OCT-1990 13:55:42 KEE "(SPR -1) Motif and DecWindow Merge"*/
/* *5    27-JUN-1990 17:30:04 KEE "(SPR -1) New wi data structure using 2-D array"*/
/* *4     3-MAY-1990 21:49:37 KEE "(SPR -1) menu sync fix."*/
/* *3     2-MAR-1990 11:33:00 VINCE "(SPR 6012) Changed usage of USE_X symbol"*/
/* *2     5-FEB-1990 23:38:28 JULIAN "(SPR -1) X version of WI Module (Kee)"*/
/* *1    19-JUN-1989 13:37:53 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element WI_POP.C*/
/* DEC/CMS REPLACEMENT HISTORY, Element WI_POP.C                                    */
/* *5    27-JUN-1990 17:30:04 KEE "(SPR -1) New wi data structure using 2-D array"  */
/* *4     3-MAY-1990 21:49:37 KEE "(SPR -1) menu sync fix."                         */
/* *3     2-MAR-1990 11:33:00 VINCE "(SPR 6012) Changed usage of USE_X symbol"      */
/* *2     5-FEB-1990 23:38:28 JULIAN "(SPR -1) X version of WI Module (Kee)"        */
/* *1    19-JUN-1989 13:37:53 SYSTEM ""                                             */
/* DEC/CMS REPLACEMENT HISTORY, Element WI_POP.C                                    */
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
/*NAME:    WI_POP                                                                   */
/*                                                                                  */
/*DESCRIPTION:                                                                      */
/*                                                                                  */
/*    Pop previous interactive environment to unnest interactions.                  */
/*    Undoes previous push.                                                         */
/*                                                                                  */
/*************************************************************************          */

#include "esi_wi_int.x"

#ifdef USE_X

#include "ansixt.h"

#endif

/************************************************************************************/
publicdef INT wi_pop ()             /*************************************************************************/
    {
    PROGNAME ("wi_pop");
#define IDENT progname
    INT i;

#ifdef USE_X
    INT status;
    INT front_app_id, menu_pop_app_id;
    
    wiz_jp_name (IDENT);
    
    E_CHECK_INIT;
    
    /* Pop input mode control from stack in reverse order.                          */
    wiz_busy_cursor(FALSE);
    for (i = 0; i < NUM_EVENT_TYPES; i++)
        {
        wiv.event[NUM_EVENT_TYPES - i - 1].mode = wiz_pop_int ();
        }
    if (wiv.wi_push_pop_top > 0)    /* pop dialog and menu if top poiner > 0        */
        {
        for (i = 1; i <= wiv.dialog_menu_count; i++)
            {
            E_CHECK_WIDGET (i, 0);
            if ((E_DIALOG_MENU (i))->num_items != 0)    /* not delete               */
                {
                if ((E_DIALOG_MENU (i))->wi_push_pop_num == wiv.wi_push_pop_top)
                    {
                    (E_DIALOG_MENU (i))->wi_push_pop_num = 0;   /* reset push/pop num   */
                    if ((E_WIDGET_INFO (i, 0))->type_id == DIALOG)
                        {
						/* ------------------------- */
						/* reset the front dialog id */
						/* ------------------------- */
						wiv.frontdialog_id = i;
                        XtSetSensitive (E_GET_WIDGET (i, 0), TRUE);
                        wiz_stattext_sensitive_false (i);
                        /* turn sensitive of dialog on except stattext              */
                        }
                    else            /* menu_type                                    */
                        {
                        wiz_managechild_widget (i); /* open menu                    */
                        menu_pop_app_id = (E_DIALOG_MENU (i))->app_id;
                        }
                    }
                }
            }
        wiv.wi_push_pop_top--;      /* update top push/pop pointer                  */
        if (wiv.wi_push_pop_top == 0)
            {
            E_CHECK_WIDGET (wiv.frontdialog_id, 0);
            if ((E_DIALOG_MENU (wiv.frontdialog_id))->num_items != 0)   /* not delete   */
                {
                
                front_app_id = (E_DIALOG_MENU (wiv.frontdialog_id))->app_id;
                /* front application id                                             */
                if (front_app_id != menu_pop_app_id)
                    {               /* if menu being pop doesn't belong to the same application     */
                                    /* id of the front dialog, then we need to syn menu */
                    wiz_menu_syn (front_app_id);
                    }
                }
            }
        }
#else        /* ESITERM code */
    wiz_jp_name (IDENT);
    
    E_CHECK_INIT;
    /* Pop input mode control from stack in reverse order.                          */
    for (i = 0; i < NUM_EVENT_TYPES; i++)
        {
        wiv.event[NUM_EVENT_TYPES - i - 1].mode = wiz_pop_int ();
        }
#if DEBUG
    printf ("WI_POP - stackpointer = %d\n", wi_stack_pointer);
#endif
    SEND_1OPCODE (MC_POP);
#endif
    RETURN (SUCCESS);
    }
