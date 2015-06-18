/* DEC/CMS REPLACEMENT HISTORY, Element FI_INIT_MENUS.C*/
/* *11    5-AUG-1991 16:58:19 JTM "(SPR 0) Removed special check for Velocity Inter. "*/
/* *10   11-JUN-1991 14:51:23 JESSIE "(SPR 0) check for activating VI menu"*/
/* *9    23-OCT-1990 13:51:48 KEE "(SPR -1) Motif and DecWindow Merge"*/
/* *8    27-JUN-1990 17:24:54 KEE "(SPR -1) New wi data structure using 2-D array"*/
/* *7     2-MAR-1990 11:13:38 JULIAN "(SPR 6012) fix up USE_X logic to be IFDEF only"*/
/* *6    28-FEB-1990 06:13:15 GILLESPIE "(SPR 6010) Change WIMENU to MENU"*/
/* *5    26-FEB-1990 18:57:43 JULIAN "(SPR 6010) Port to Decstation 3100 (ULTRIX)"*/
/* *4     6-FEB-1990 00:17:43 JULIAN "(SPR -1) X Version "*/
/* *3    27-NOV-1989 15:38:54 GORDON "(SPR 0) Use NULL_MENU_ID, not 0"*/
/* *2    31-JUL-1989 14:21:14 GORDON "(SPR 101) add call to fih_init_menus for bozo bus"*/
/* *1    19-JUN-1989 12:50:06 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element FI_INIT_MENUS.C*/
/* DEC/CMS REPLACEMENT HISTORY, Element FI_INIT_MENUS.C                             */
/* *1    13-AUG-1990 16:38:58 SYSTEM "Finder 6.1"                                   */
/* DEC/CMS REPLACEMENT HISTORY, Element FI_INIT_MENUS.C                             */
/* DEC/CMS REPLACEMENT HISTORY, Element FI_INIT_MENUS.C                             */
/* *8    27-JUN-1990 17:24:54 KEE "(SPR -1) New wi data structure using 2-D array"  */
/* *7     2-MAR-1990 11:13:38 JULIAN "(SPR 6012) fix up USE_X logic to be IFDEF only"*/
/* *6    28-FEB-1990 06:13:15 GILLESPIE "(SPR 6010) Change WIMENU to MENU"          */
/* *5    26-FEB-1990 18:57:43 JULIAN "(SPR 6010) Port to Decstation 3100 (ULTRIX)"  */
/* *4     6-FEB-1990 00:17:43 JULIAN "(SPR -1) X Version "                          */
/* *3    27-NOV-1989 15:38:54 GORDON "(SPR 0) Use NULL_MENU_ID, not 0"              */
/* *2    31-JUL-1989 14:21:14 GORDON "(SPR 101) add call to fih_init_menus for bozo bus"    */
/* *1    19-JUN-1989 12:50:06 SYSTEM ""                                             */
/* DEC/CMS REPLACEMENT HISTORY, Element FI_INIT_MENUS.C                             */
/******************************************************************************     */
/*                                                                                  */
/*                Copyright Finder Graphics Systems, Inc. 1990                      */
/*         Unpublished -- All rights reserved                                       */
/*                                                                                  */
/*THIS SOFTWARE IS THE PROPRIETARY PROPERTY OF Finder Graphics Systems, Inc.  AND   */
/*MAY CONTAIN CONFIDENTIAL TRADE SECRET INFORMATION. IT IS LICENSED FOR USE ON THE  */
/*DESIGNATED EQUIPMENT ON WHICH IT WAS ORIGINALLY INSTALLED AND  MAY NOT BE         */
/*MODIFIED, DUPLICATED OR COPIED IN ANY FORM WITHOUT PRIOR WRITTEN CONSENT OF       */
/*                                                                                  */
/*          Finder Graphics Systems, Inc.                                           */
/*          201 Tamal Vista Blvd                                                    */
/*          Corte Madera, CA  USA 94925                                             */
/*          (415) 927-0100                                                          */
/*                                                                                  */
/*(The above notice does not constitute an admission of publication or              */
/*unrestricted dissemination of the work)                                           */
/*                                                                                  */
/******************************************************************************     */

#include "esi_fi_int.h"

#ifndef ESI_AM_INT_H
#include "esi_am_int.h"
#endif

#ifndef ESI_WI_INT_X
#include "esi_wi_int.x"
#endif

#ifndef ESI_BT_H
#include "esi_bt.h"
#endif

#ifndef ESI_LL_H
#include "esi_ll.h"
#endif

#ifndef ESI_TC_H
#include "esi_tc.h"
#endif

#ifndef ESI_HO_H
#include "esi_ho.h"
#endif
typedef struct 
    {
    INT id;
    INT runtime_id;
    LISTHEAD item;                  /*    menu                                      */
    MENU menu;                      /*   WI menu structure list of items within this*/
    CHAR helpkey[256];
    } FIMENUSETUP;

publicdef INT fi_init_menus ()
    {
    FINDER_MENU_STRUCT *pm;
    LISTHEAD list;                  /*    list of struct setups                     */
    INT application_id;
    INT status;
    BOOL found;
    INT i, j;
    INT id;
    INT cnt, cntj;
    APPLICATION_LIST app;           /* application node storage                     */
    RESOURCE_LIST rsrc;             /*    resource node storage                     */
    INPUT_RECORD inp_rec;
    APPLICATION_STATIC *application_static = am_static ();
    WIV_STRUCT *pwiv;
    
    FIMENUSETUP *p;
    
    TCP tcp = (TCP) 0;
    /*------------------------------------------------------------------------------*/
    pwiv = (WIV_STRUCT *) wiz_static ();
    application_id = application_static->application_id;
    btfind (&application_static->application_list, &application_id, &app);
    wi_set_owner_id (application_id);
    wi_set_owner_tag (app.tag);
    
    status = llinit (&list, 0, sizeof(FIMENUSETUP *), 0);
    
    /* construct the final menu struct array from                                   */
    /*the statically linked one and the dynamically                                 */
    /*read one                                                                      */
    status = fih_init_menus ();
    
    /*    loop through each entry in the externally                                 */
    /*    defined menu structure                                                    */
    
    for (pm = fiz_finder_menus (); pm->id IS_NOT_EQUAL_TO NULL_MENU_ID; pm++)
        {
        /*    look for this menu ID - see if it already                             */
        /*    exists                                                                */
        for (llfirst (&list, (VOIDPTR)&p), found = FALSE, i = 0, cnt = llcount (&list);
             i < cnt; llnext (&list, (VOIDPTR)&p), i++)
            {
            if (p->id EQUALS pm->id)
                {
                /*    it does - add item to list Only 1 type 1                      */
                if (pm->Type EQUALS MenuName)   /*    menu allowed                  */
                    {
                    /*    error                                                     */
                    }
                else
                    {
                    i = p->menu.num_mn_items;
                    strcpy (p->menu.item[i].command, pm->Name);
                    /* Put in the help text field                                   */
                    /* If not present, use the string FINDER                        */
                    strcpy (p->helpkey,
                            pm->AppName != (CHAR *)0 ? pm->AppName : "FINDER");
                    p->menu.item[i].ifenab = TRUE;
                    p->menu.item[i].ifcheck = FALSE;
                    llappnd (&p->item, (VOIDPTR)&pm);
                    p->menu.num_mn_items++;
                    }
                found = TRUE;
                break;
                }
            }
        if (NOT found)
            /*    This ID has not yet been found.  Allocate a                       */
            /*    new setup structure, add the appropriate                          */
            /*    menu definitions and add it to the setup                          */
            /*    list                                                              */
            {
            if (pm->Type EQUALS MenuName)
                {
                p = ALLOCATE (FIMENUSETUP);
                p->id = pm->id;
                strcpy (p->menu.mtitle, pm->Name);
                p->menu.ifstatic = TRUE;
                p->menu.ifmain = TRUE;
                p->menu.ifvisibl = FALSE;
                llinit (&p->item, 0, sizeof(FINDER_MENU_STRUCT *), 0);
                llappnd (&p->item, (VOIDPTR)&pm);
                llappnd (&list, (VOIDPTR)&p);
                }
            else
                {
                /*    It is an error to have any type other than                    */
                /*MenuName as the first data type                                   */
                
                }
            }
        }
    /*    at this point, all of the menus have been                                 */
    /*    read.   Now take the link lists for each                                  */
    /*    menu and define the official workstation                                  */
    /*    interface menu structure                                                  */
    
    for (i = 0, cnt = llcount (&list), llfirst (&list, (VOIDPTR)&p); i < cnt;
         i++, llnext (&list, (VOIDPTR)&p))
        {
        
        /*    Now the menu structure has been defined -                             */
        /*    call the workstation interface tools to                               */
        /*    realize this new achievement                                          */
        
        status = wi_define_menu (&p->menu, "", &id);
        
        /*    put this menu in the application manager                              */
        /*    setup                                                                 */
        
        rsrc.type = AM_MENU;
        rsrc.application_id = application_id;
        strcpy (rsrc.file, "SOFTWARE_BUS_MENU");
        p->runtime_id = rsrc.resource_id = id;
        rsrc.server = fi_menu_server;
        rsrc.pointer = (VOIDPTR)0;
        rsrc.language = 0;
        
        /*      Replace RESOURCE_LIST data.                                         */
        btdel (&application_static->resource_list, (VOIDPTR)&id);
        btadd (&application_static->resource_list, (VOIDPTR)&id, (VOIDPTR)&rsrc);
        
        /* do more housekeeping for the workstation interface                       */
        
        inp_rec.type = WI_MENU_EVENT;
        inp_rec.numitems = p->menu.num_mn_items;
        inp_rec.owner_id = pwiv->current.owner_id;
        strcpy (inp_rec.owner_tag, pwiv->current.owner_tag);
        strcpy (inp_rec.helpkey, "FINDER");
        btadd (&pwiv->input_tree, &id, &inp_rec);

#ifdef USE_X
        E_CHECK_WIDGET (id, 0);
        (E_DIALOG_MENU (id))->server = fi_menu_server;
        (E_DIALOG_MENU (id))->pointer = (VOIDPTR)0;
        (E_DIALOG_MENU (id))->app_id = application_id;
#endif
        
        /*    put this run-time id in place of the former                           */
        /*    ID in the structure NOTE: cntj is # menu                              */
        /*    items PLUS the menu title                                             */
        
        for (j = 0, llfirst (&p->item, (VOIDPTR)&pm), cntj = p->menu.num_mn_items + 1;
             j < cntj; j++, llnext (&p->item, (VOIDPTR)&pm))
            {
            pm->id = p->runtime_id;
            if (pm->Type EQUALS MenuName)
                {
                pm->item = cntj;
                }
            else
                {
                pm->item = j;
                
                /*    Disable menu items that perform no action                     */
                
                switch (pm->Type)
                    {
                case NextMenu:
                    pm->flag = ON;
                    break;
                case NoEventsFunction:
                case EventsFunction:
                    
                    /*    Activate only for defined functions                       */
                    
                    pm->flag = pm->Activates.AppEntry != NULL_FUNCTION_PTR ? ON : OFF;
                    break;
                case ProgramName:
                case CPName:
                case CommandName:
                    
                    /*    Activate only for defined names                           */
                    
                    pm->flag = pm->AppName != (CHAR *)0 ? ON : OFF;

                    break;
                default:
                case Undefined:
                    pm->flag = OFF;
                    break;
                    }
                if (pm->flag EQUALS OFF)
                    {
                    wi_enable_menu_item (pm->id, pm->item, pm->flag);
                    }
                }
            }
        }
    /*    Finally, update any 'Next Menu' ids (Record                               */
    /*    type NextMenu) to represent the new                                       */
    /*    run-time menu ids                                                         */
    
    for (cnt = llcount (&list), pm = fiz_finder_menus ();
         pm->id IS_NOT_EQUAL_TO NULL_MENU_ID; pm++)
        {
        if (pm->Type EQUALS NextMenu)
            {
            for (i = 0, llfirst (&list, (VOIDPTR)&p); i < cnt;
                 i++, llnext (&list, (VOIDPTR)&p))
                {
                if (p->id EQUALS pm->Activates.menu)
                    {
                    pm->Activates.menu = p->runtime_id;
                    break;
                    }
                }
            }
        }
    /* free up allocated memory                                                     */
    
    for (i = 0, llfirst (&list, (VOIDPTR)&p); i < cnt;
         i++, llnext (&list, (VOIDPTR)&p))
        {
        llfree (&p->item);
        tc_free ((VOIDPTR)p);
        }
    llfree (&list);
    
    return SUCCESS;
    }
