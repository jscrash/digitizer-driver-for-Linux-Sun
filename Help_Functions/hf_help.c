/* DEC/CMS REPLACEMENT HISTORY, Element HF_HELP.C*/
/* *7    25-OCT-1990 17:48:59 GILLESPIE "(SPR 32) Restore what was lost ..."*/
/*  2B1   3-OCT-1990 12:17:04 GILLESPIE "Merge beta deltas"*/
/* *6    21-AUG-1990 17:05:24 KEE "(SPR -1) Fixing compile error at sun"*/
/* *5    20-AUG-1990 18:24:33 KEE "(SPR -1) fixing compile error at sun"*/
/* *4    20-AUG-1990 17:07:28 KEE "(SPR -1) fixing compile error at sun"*/
/* *3    17-AUG-1990 21:48:43 VINCE "(SPR 5644) Code Cleanup"*/
/* *2    27-JUL-1990 16:13:40 GILLESPIE "(SPR 1) Robb Hall's additions restored to their (rightful) place"*/
/* *1    27-JUL-1990 16:06:50 GILLESPIE "The REAL thing"*/
/* DEC/CMS REPLACEMENT HISTORY, Element HF_HELP.C*/
/* DEC/CMS REPLACEMENT HISTORY, Element HF_HELP.C                                   */
/* *6    21-AUG-1990 17:05:24 KEE "(SPR -1) Fixing compile error at sun"            */
/* *5    20-AUG-1990 18:24:33 KEE "(SPR -1) fixing compile error at sun"            */
/* *4    20-AUG-1990 17:07:28 KEE "(SPR -1) fixing compile error at sun"            */
/* *3    17-AUG-1990 21:48:43 VINCE "(SPR 5644) Code Cleanup"                       */
/* *2    27-JUL-1990 16:13:40 GILLESPIE "(SPR 1) Robb Hall's additions restored to their (rightful) place"  */
/* *1    27-JUL-1990 16:06:50 GILLESPIE "The REAL thing"                            */
/* DEC/CMS REPLACEMENT HISTORY, Element HF_HELP.C                                   */
/************************************************************************************/
/******************************************************************************     */
/*                                                                                  */
/*                Copyright Exploration Systems, Inc. 1989                          */
/*                    Unpublished -- All rights reserved                            */
/*                                                                                  */
/*THIS SOFTWARE IS THE PROPRIETARY PROPERTY OF Exploration Systems, Inc.  AND MAY   */
/*CONTAIN CONFIDENTIAL TRADE SECRET INFORMATION. IT IS LICENSED FOR USE ON THE      */
/*DESIGNATED EQUIPMENT ON WHICH IT WAS ORIGINALLY INSTALLED AND  MAY NOT BE         */
/*MODIFIED, DUPLICATED OR COPIED IN ANY FORM WITHOUT PRIOR WRITTEN CONSENT OF       */
/*                                                                                  */
/*                            Exploration Systems                                   */
/*                            c/o Finder Graphics Systems                           */
/*                            201 Tamal Vista Blvd.                                 */
/*                            Corte Madera, CA  94925                               */
/*                            (415) 927-0100                                        */
/*                                                                                  */
/*(The above notice does not constitute an admission of publication or              */
/*unrestricted dissemination of the work)                                           */
/*                                                                                  */
/******************************************************************************     */

#include "esi_c_lib.h"
#include "esi_hf_int.h"

#ifndef ESI_AM_H

#include "esi_am.h"

#endif

#ifndef ESI_AM_STAT_H

#include "esi_am_stat.h"

#endif

#ifndef ESI_FI_H

#include "esi_fi.h"

#endif

#ifndef ESI_MG_H

#include  "esi_mg.h"

#endif

#ifndef ESI_RS_H

#include "esi_rs.h"

#endif

#ifndef ESI_TS_H

#include "esi_ts.h"

#endif

#ifndef ESI_WI_H

#include "esi_wi.h"

#endif

#ifndef ESI_TC_H

#include "esi_tc.h"

#endif

#ifndef ESI_HO_FILES_H

#include "esi_ho_files.h"

#endif

#include "hf_cdlgs.rh"
#include "gb_menus.rh"
#include "esi_conf_ws.h"

/* HF_HELP's global data items                                                      */

publicdef HF_STRUCT *hf_struct = (HF_STRUCT *)
0;
/* 11/23/88 gs                                                                      */

/* HF_HELP's function prototypes                                                    */

#if USE_PROTOTYPES
static INT hf_help_check (INT menuid, INT itemid, BYTE *ptr);
static INT hf_display_help_server (INT id, INT item, BYTE *ptr);
static INT hf_display_help (void);
static INT hfz_select_menu_help (INT menuid, INT itemid, INT rsrc_id,
                                     FILENAME rsrc_file);
static INT hfz_get_menu_item_help (INT menuid, INT itemid, INT rsrc_id,
                                       FILENAME rsrc_file, CHAR string[]);
static INT hfz_front_cdlg_help (void);

#else
static INT hf_help_check ();
static INT hf_display_help_server ();
static INT hf_display_help ();
static INT hfz_select_menu_help ();
static INT hfz_get_menu_item_help ();
static INT hfz_front_cdlg_help ();

#endif

/* File Description ---------------------------------------------------------       */
/*Overview:                                                                         */
/*    This HF_HELP module contains the various functions used by the FINDER                 */
/*    system to provide "HELP on a MENU ITEM" and "HELP on a DIALOG BOX",                   */
/*    both of which appear as choices on the GLOBAL menu.                                   */
/*                                                                                  */
/*Prototypes and one line descriptions of each function in the file.                */
/*Public_Functions:                                                                 */
/*    publicdef INT hf_do_help(CHAR *help_key);                                     */
/*                                                                                  */
/*    publicdef INT hf_display_help_init();                                         */
/*                                                                                  */
/*Private_Functions:                                                                */
/*    static INT hf_help_check(INT menuid, INT itemid, BYTE *ptr);              */
/*                                                                                  */
/*    static INT hf_display_help_server(INT id, INT item, BYTE *ptr);           */
/*                                                                                  */
/*    static INT hf_display_help(void);                                         */
/*                                                                                  */
/*    static INT hfz_select_menu_help(INT menuid, INT itemid, INT rsrc_id,      */
/*                                    FILENAME rsrc_file);                          */
/*                                                                                  */
/*    static INT hfz_get_menu_item_help(INT menuid, INT itemid, INT rsrc_id,    */
/*                                      FILENAME rsrc_file, CHAR string[]);         */
/*                                                                                  */
/*    static INT hfz_front_cdlg_help(void);                                     */
/*                                                                                  */
/*-----------------------------------------------------------------------------     */

/* Function Description -----------------------------------------------------       */
/*Description:                                                                      */
/*    This is function is installed into the "callback list" by HF_HELP_SERVER      */
/*    (in response to the (ab)user's selection of a HELP pick), and is called       */
/*    by WI_REQUEST_EVENT for each and every event that occurs.                     */
/*                                                                                  */
/*    It will determine whether or not the user has previously picked a HELP        */
/*    selection from the GLOBAL menu, and if so, it will transfer control to        */
/*    the HELP facility (to provide a help message for the current choice).         */
/*                                                                                  */
/*Prototype:                                                                        */
/*    static INT hf_help_check(INT menuid, INT itemid, BYTE *ptr);              */
/*                                                                                  */
/*Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)        */
/*    (I) menuid      -(INT) Mac resource id of menu.                               */
/*    (I) itemid      -(INT) Number of item on menu.                                */
/*    (I) ptr         _(BYTE *) An unused data pointer.                             */
/*                                                                                  */
/*Return Value/Status:                                                              */
/*    SUCCESS - Successful completion.                                              */
/*    FAIL    - Unsuccessful completion.                                            */
/*                                                                                  */
/*Scope:                                                                            */
/*    PRIVATE to <hf_help.c>                                                        */
/*                                                                                  */
/*Limitations/Assumptions:                                                          */
/*                                                                                  */
/*-----------------------------------------------------------------------------     */

/* BEGIN:                                                                           */
#if USE_PROTOTYPES
static INT hf_help_check (INT menuid, INT itemid, BYTE *ptr)
#else
static INT hf_help_check (menuid, itemid, ptr)
INT menuid;
INT itemid;
BYTE *ptr;
#endif
    {
    INT status;
    INT *help_flag;
    INT resource_id;
    FILENAME resource_file;
    
    /* Open workspace for help facility                                             */
    status = am_open_workspace ("MENU_HELP", AM_GLOBAL, (VOIDPTR *)&help_flag);
    
    if (help_flag[0] AND help_flag[4])  /* process "help on menu pick"              */
        {
        help_flag[4] = FALSE;
        wi_remove_callback (hf_help_check); /* remove the callback function         */
        am_inq_rsrc (menuid, itemid, resource_file, &resource_id);
        status = hfz_select_menu_help (menuid, itemid, resource_id, resource_file);
        }
    else if (help_flag[0] AND!help_flag[4])
        {
        status = am_message (AM_ALERT, 
"To get help for another menu item, when the help message dialog is displayed: \
1. Punch the DONE button. 2. Choose Get Help on a menu from the Global menu. \
3. Choose a menu item."
                );
        }
    return status;
    }
/* END:                                                                             */


/*************************************************************************          */
/*                                                                                  */
/*function:  HF_HELP_SERVER                                                         */
/*                                                                                  */
/*description:                                                                      */
/*                                                                                  */
/*   This is function is called by the Software (BOZO) Bus whenever the             */
/*   (ab)user selects either of the "Get help on a menu" or the "Get help           */
/*   on front dialog" choices from the GLOBAL MENU.  It will set the HELP           */
/*   FLAGS to the appropriate values so that the HELP facility will be              */
/*   invoked for the next selection the (ab)user makes.                             */
/*                                                                                  */
/*inputs:                                                                           */
/*                                                                                  */
/*   EVENT = a pointer to the EVENT_ID structure.                                   */
/*                                                                                  */
/*outputs:                                                                          */
/*                                                                                  */
/*   This function returns SUCCESS, if it processed the event; and FAIL,            */
/*   if it did NOT.  It's caller should expect this indicator, and decide           */
/*   whether (or not) to propagate the event further upstream (or to eat            */
/*   it & wait for another).                                                        */
/*                                                                                  */
/*************************************************************************          */

publicdef INT hf_help_server (event)
EVENT_ID_STRUCT *event;

    {                               /* HF_HELP_SERVER                               */
    INT status = SUCCESS;
    INT *help_flag;
    
    /* define the MENU_HELP workspace                                               */
    
    am_define_workspace ("MENU_HELP", AM_GLOBAL, sizeof(INT) * 5,
                         (VOIDPTR *)&help_flag);
    
    /* process whichever event the (ab)user caused                                  */
    
    switch (event->item_id)
        {
    case HELP_ON_MENU_ID:

#ifdef vms
        
        /* install HF_HELP_CHECK as an event callback function                      */
        wi_add_callback (hf_help_check, (BYTE *)NULL);
        
        /* open the HELP workspace, and set the appropriate flags                   */
        am_open_workspace ("MENU_HELP", AM_GLOBAL, (VOIDPTR *)&help_flag);
        help_flag[0] = !help_flag[0];
        help_flag[1] = event->menu_id;
        help_flag[2] = event->item_id;
        help_flag[3] = HELP_ON_MENU_ID;
        help_flag[4] = TRUE;
        
        /* check-off this menu item, and dim the other one                          */
        wichmni (event->menu_id, event->item_id, help_flag[0]);
        wi_enable_menu_item (event->menu_id, HELP_ON_DIALOG_ID, OFF);
        status = SUCCESS;

#else
        wi_enable_menu_item (event->menu_id, HELP_ON_DIALOG_ID, OFF);
        status = FAIL;
        am_quit ();
#endif          /* Help facility disabled unless HOST is VMS. */
        break;
        
    case HELP_ON_DIALOG_ID:

#ifdef vms
        
        /* check-off this item, and dim the other one                               */
        wichmni (event->menu_id, HELP_ON_DIALOG_ID, ON);
        wi_enable_menu_item (event->menu_id, HELP_ON_MENU_ID, OFF);
        
        /* open the HELP workspace, and set the appropriate flags                   */
        am_open_workspace ("MENU_HELP", AM_GLOBAL, (VOIDPTR *)&help_flag);
        help_flag[0] = !help_flag[0];
        help_flag[1] = event->menu_id;
        help_flag[2] = event->item_id;
        help_flag[3] = HELP_ON_DIALOG_ID;
        help_flag[4] = FALSE;
        
        /* give the (ab)user help with the front dialog box                         */
        hfz_front_cdlg_help ();
        
        status = SUCCESS;
#else
        wi_enable_menu_item (event->menu_id, event->item_id, OFF);
        status = FAIL;

#endif               /* Help facility disabled unless HOST is VMS. */
        break;
        }
    return status;
    }   /* HF_HELP_SERVER                                                           */

/* Function Description -----------------------------------------------------       */
/*Description:                                                                      */
/*    This is function provides help for a specific "help key".                     */
/*                                                                                  */
/*Prototype:                                                                        */
/*    publicdef INT hf_do_help(CHAR *help_key);                                     */
/*                                                                                  */
/*Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)        */
/*    (I) help_key    -(CHAR *) The specified key.                                  */
/*                                                                                  */
/*Return Value/Status:                                                              */
/*    SUCCESS - Successful completion.                                              */
/*                                                                                  */
/*Scope:                                                                            */
/*    PUBLIC                                                                        */
/*                                                                                  */
/*Limitations/Assumptions:                                                          */
/*                                                                                  */
/*-----------------------------------------------------------------------------     */

/* BEGIN:                                                                           */
#if USE_PROTOTYPES
publicdef INT hf_do_help (CHAR *help_key)
#else
publicdef INT hf_do_help (help_key)
CHAR *help_key;
#endif
    {                               /* HF_DO_HELP                                   */
    INT status = SUCCESS;
    INT id, item = 0, class , *help_flag, len;
    CHAR *end, valid_help_key[256];
    FILENAME lib_name;
    
    /*                                                                              */
    /*   The help facility will not work for anything but VMS.                      */
    /*   For the time being we will simply tell the user that                       */
    /*   help is not available on any other host.                                   */
    /*                                                  JRB, Calgary, 5/4/89        */
    /*                                                                              */
    

#ifdef vms
    status = ho_add_path ("ESI$HELP", "FINDER_HELP.HLB", STMAX, lib_name);
    if (!hoexists (lib_name))
        {
        am_message (AM_ALERT, "The FINDER help facility is not available");
        }
    else
        {
        /* Do our best to validate the help key.                                    */
        if (help_key EQUALS NULL OR help_key EQUALS (CHAR *)NULL)
            {
            strcpy (valid_help_key, "HELP");
            }
        else
            {
            len = strlen (help_key);
            if (len >= 256)
                {
                strcpy (valid_help_key, "HELP");
                printf ("Warning --- The help key is invalid, length = %d\n", len);
                }
            else
                {
                strcpy (valid_help_key, help_key);
                }
            }
        /* reset the workspace for the help facility.                               */
        status = hf_start_help (valid_help_key);
        status = am_open_workspace ("HELP_WORKSPACE", AM_GLOBAL,
                                    (VOIDPTR *)&hf_struct);
        
        /* get help messages from help libaray and fill in display buffer.          */
        
        status = hf_get_help (valid_help_key);
        
        if (status EQUALS SUCCESS)
            {
            /* If the specified key is found,                                       */
            /* display the help messages and                                        */
            /* set the current key to be parent key                                 */
            
            status = hf_display_help ();
            if (hf_struct->parent_key != (CHAR *)NULL)
                am_free ((BYTE *)hf_struct->parent_key);
            hf_struct->parent_key = hf_struct->current_key;
            }
        else
            {
            /* otherwise, displays the not found                                    */
            /* messages and find the last level                                     */
            /* of valid help key.                                                   */
            
            status = hf_display_help ();
            do
                {
                end = strrchr (hf_struct->current_key, ' ');
                if (end != (CHAR *)0)
                    *end = '\0';
                hf_struct->message_ptr =
                    (CHAR *) tc_zeroes ((BYTE *)hf_struct->message_ptr,
                                         hf_struct->total_allocated_chars);
                
                hf_struct->total_used_chars = 0;
                status = hf_get_help (hf_struct->current_key);
                } while (status != SUCCESS AND end != (CHAR *)0);
            
            if (hf_struct->parent_key != (CHAR *)0)
                am_free ((BYTE *)hf_struct->parent_key);
            hf_struct->parent_key = hf_struct->current_key;
            }
        }
#else

#ifndef vms
    am_message (AM_ALERT, "The FINDER help facility is not available");

#endif
#endif
    return SUCCESS;
    }   /* HF_DO_HELP                                                               */
/* END:                                                                             */

/* Function Description -----------------------------------------------------       */
/*Description:                                                                      */
/*    This is function handles events in the HELP dialog box.                       */
/*                                                                                  */
/*Prototype:                                                                        */
/*    static INT hf_display_help_server(INT id, INT item, BYTE *ptr);           */
/*                                                                                  */
/*Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)        */
/*    (I) id          -(INT) The id of active CDLG.                                 */
/*    (I) item        -(INT) The item picked in the active CDLG.                    */
/*    (I) ptr         -(BYTE *) The unused data pointer passed to all servers.      */
/*                                                                                  */
/*Return Value/Status:                                                              */
/*    SUCCESS - Successful completion.                                              */
/*    FAIL    - Unsuccessful completion.                                            */
/*                                                                                  */
/*Scope:                                                                            */
/*    PRIVATE to <hf_help.c>                                                        */
/*                                                                                  */
/*Limitations/Assumptions:                                                          */
/*                                                                                  */
/*-----------------------------------------------------------------------------     */

/* BEGIN:                                                                           */
#if USE_PROTOTYPES
static INT hf_display_help_server (INT id, INT item, BYTE *ptr)
#else
static INT hf_display_help_server (id, item, ptr)
INT id;
INT item;
BYTE *ptr;
#endif
    {                               /* HF_DISPLAY_HELP_SERVER                       */
    INT status = SUCCESS;
    INT value;
    INT i, *help_flag;
    BOOL done;
    CHAR *temp_key;
    CHAR subtopic[64];
    
    if (id IS_NOT_EQUAL_TO hf_struct->cdlg_id)
        {
        wibeep (1);
        return SUCCESS;
        }
    switch (item)
        {
    case HF_BUTTON_CONTINUE:
        subtopic[0] = '\0';
        wi_qedit_selection (id, HF_SELECTOR, &value, subtopic);
        if (strlen (subtopic) EQUALS 0)
            {
            wiqccdi (id, HF_EDIT_SUBTOPIC, &value, subtopic);
            }
        else
            {
            wiscdt (id, HF_EDIT_SUBTOPIC, subtopic);
            }
        temp_key = (CHAR *) tc_zalloc (strlen (subtopic) +
                                       strlen (hf_struct->current_key) + 5);
        if (strlen (subtopic) > 0)
            {
            strcpy (temp_key, hf_struct->current_key);
            if (strlen (hf_struct->current_key) > 0)
                {
                strcat (temp_key, " ");
                }
            strcat (temp_key, subtopic);
            }
        else
            {
            for (i = strlen (hf_struct->current_key) - 1, done = FALSE;
                 i >= 0 AND!done; i--)
                {
                if (hf_struct->current_key[i] EQUALS' ')
                    {
                    done = TRUE;
                    hf_struct->current_key[i] = '\0';
                    }
                }
            if (!done)
                hf_struct->current_key[0] = '\0';
            strcpy (temp_key, hf_struct->current_key);
            }
        status = hf_start_help (temp_key);
        status = hf_get_help (temp_key);
        if (status EQUALS SUCCESS)
            {
            status = hf_display_help ();
            if (hf_struct->parent_key != (CHAR *)NULL)
                am_free (hf_struct->parent_key);
            hf_struct->parent_key = hf_struct->current_key;
            }
        else
            {
            status = hf_display_help ();
            if (hf_struct->current_key != (CHAR *)NULL)
                am_free ((BYTE *)hf_struct->current_key);
            hf_struct->current_key = hf_struct->parent_key;
            }
        tc_free ((BYTE *)temp_key);
        break;
        
    case HF_BUTTON_DONE:
        wiclcd (hf_struct->cdlg_id);
        if (hf_struct->parent_key != (CHAR *)NULL)
            am_free ((BYTE *)hf_struct->parent_key);
        if (hf_struct->current_key != (CHAR *) NULL AND hf_struct->current_key !=
            hf_struct->parent_key)
            am_free ((BYTE *)hf_struct->current_key);
        am_release_workspace ("HELP_WORKSPACE", AM_GLOBAL);
        
        am_open_workspace ("MENU_HELP", AM_GLOBAL, (VOIDPTR *)&help_flag);
        help_flag[0] = FALSE;
        wichmni (help_flag[1], HELP_ON_MENU_ID, OFF);
        wichmni (help_flag[1], HELP_ON_DIALOG_ID, OFF);
        wi_enable_menu_item (help_flag[1], HELP_ON_DIALOG_ID, ON);
        wi_enable_menu_item (help_flag[1], HELP_ON_MENU_ID, ON);
        if (help_flag[2] == HELP_ON_DIALOG_ID)
            am_quit ();
        break;
        
    default:
        break;
        }
    return status;
    }   /* HF_DISPLAY_HELP_SERVER                                                   */
/* END:                                                                             */

/* Function Description -----------------------------------------------------       */
/*Description:                                                                      */
/*    This is function defines the HELP workspace and the HELP dialog box.          */
/*                                                                                  */
/*Prototype:                                                                        */
/*    publicdef INT hf_display_help_init();                                         */
/*                                                                                  */
/*Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)        */
/*    None.                                                                         */
/*                                                                                  */
/*Return Value/Status:                                                              */
/*    SUCCESS - Successful completion.                                              */
/*    FAIL    - Unsuccessful completion.                                            */
/*                                                                                  */
/*Scope:                                                                            */
/*    PUBLIC                                                                        */
/*                                                                                  */
/*Limitations/Assumptions:                                                          */
/*                                                                                  */
/*-----------------------------------------------------------------------------     */

/* BEGIN:                                                                           */
#if USE_PROTOTYPES
publicdef INT hf_display_help_init ()
#else
publicdef INT hf_display_help_init ()
#endif
    {
    INT status = SUCCESS;
    INT id;
    INT num_items = 0;
    
    /* defines the dialog resource.                                                 */
    status = am_open_workspace ("HELP_WORKSPACE", AM_GLOBAL, (VOIDPTR *)&hf_struct);
    if (status)
        return status;
    
    status = am_define_resource (AM_DIALOG, "HF_CDLGS", HELP_MESSAGE,
                                 hf_display_help_server, &id, NULL);
    
    hf_struct->cdlg_id = id;
    
    return status;
    }
/* END:                                                                             */

/* Function Description -----------------------------------------------------       */
/*Description:                                                                      */
/*                                                                                  */
/*Prototype:                                                                        */
/*    static INT hf_display_help();                                             */
/*                                                                                  */
/*Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)        */
/*    None.                                                                         */
/*                                                                                  */
/*Return Value/Status:                                                              */
/*    SUCCESS - Successful completion.                                              */
/*                                                                                  */
/*Scope:                                                                            */
/*    PRIVATE to <hf_help.c>                                                        */
/*                                                                                  */
/*Limitations/Assumptions:                                                          */
/*                                                                                  */
/*-----------------------------------------------------------------------------     */

/* BEGIN:                                                                           */
#if USE_PROTOTYPES
static INT hf_display_help ()
#else
static INT hf_display_help ()
#endif
    {
    /* put the data in the CDLG.                                                    */
    wiscdr (hf_struct->cdlg_id, HF_SELECT_BAR, 0, hf_struct->total_lines + 10);
    wiscdt (hf_struct->cdlg_id, HF_SELECTOR, hf_struct->message_ptr);
    wiscdt (hf_struct->cdlg_id, HF_EDIT_SUBTOPIC, "");
    wiopcd (hf_struct->cdlg_id);
    wi_activate (hf_struct->cdlg_id, HF_EDIT_SUBTOPIC, 1);
    
    return SUCCESS;
    }
/* END:                                                                             */

/* Function Description -----------------------------------------------------       */
/*Description:                                                                      */
/*    Help facility internal (Z) support functions.                                 */
/*                                                                                  */
/*Prototype:                                                                        */
/*    static INT hfz_select_menu_help(INT menuid, INT itemid, INT rsrc_id, FILENAME rsrc_file); */
/*                                                                                  */
/*Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)        */
/*    menuid          -(INT)                                                        */
/*    itemid          -(INT)                                                        */
/*    rsrc_id         -(INT)                                                        */
/*    rsrc_file       -(FILENAME)                                                   */
/*                                                                                  */
/*Return Value/Status:                                                              */
/*    SUCCESS - Successful completion.                                              */
/*    FAIL    - Unsuccessful completion.                                            */
/*                                                                                  */
/*Scope:                                                                            */
/*    PRIVATE to <hf_help.c>                                                        */
/*                                                                                  */
/*Limitations/Assumptions:                                                          */
/*                                                                                  */
/*-----------------------------------------------------------------------------     */

/* BEGIN:                                                                           */
#if USE_PROTOTYPES
static INT hfz_select_menu_help (INT menuid, INT itemid, INT rsrc_id,
                                     FILENAME rsrc_file)
#else
static INT hfz_select_menu_help (menuid, itemid, rsrc_id, rsrc_file)
INT menuid, itemid, rsrc_id;
FILENAME rsrc_file;
#endif
    {
    INT status = SUCCESS;
    CHAR help_key[STMAX], item_help[STMAX];
    INT i, len, pre_is_seperator = FALSE;
    
    /* If the menu is generated by software bus,                                    */
    /*    constructs help key by search the MACRO                                   */
    /*    structure.                                                                */
    
    if (!strcmp (rsrc_file, "SOFTWARE_BUS_MENU"))
        {
        fih_query_menu_helpkey (menuid, itemid, help_key);
        }
    else
        {
        status = wi_query_helpkey (menuid, help_key);
        if (strlen (help_key) > 1 OR (help_key[0] != ' '))
            {
            status = hfz_get_menu_item_help (menuid, itemid, rsrc_id, rsrc_file,
                                             item_help);
            ts_sto_upper (item_help);
            strcat (help_key, "/");
            strcat (help_key, item_help);
            }
        len = strlen (help_key);
        /* convert the seperator.                                                   */
        if (len > 0)
            {
            for (i = 0; i < len; i++)
                {
                if (help_key[i] EQUALS'/')
                    {
                    if (!pre_is_seperator)
                        {
                        pre_is_seperator = TRUE;
                        help_key[i] = ' ';
                        }
                    }
                else
                    pre_is_seperator = FALSE;
                }
            }
        }
    status = hf_do_help (help_key);
    return status;
    }
/* END:                                                                             */

/* Function Description -----------------------------------------------------       */
/*Description:                                                                      */
/*                                                                                  */
/*Prototype:                                                                        */
/*    static INT hfz_front_cdlg_help();                                         */
/*                                                                                  */
/*Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)        */
/*    None.                                                                         */
/*                                                                                  */
/*Return Value/Status:                                                              */
/*    SUCCESS - Successful completion.                                              */
/*    FAIL    - Unsuccessful completion.                                            */
/*                                                                                  */
/*Scope:                                                                            */
/*    PRIVATE to <hf_help.c>                                                        */
/*                                                                                  */
/*Limitations/Assumptions:                                                          */
/*                                                                                  */
/*-----------------------------------------------------------------------------     */

/* BEGIN:                                                                           */
#if USE_PROTOTYPES
static INT hfz_front_cdlg_help ()
#else
static INT hfz_front_cdlg_help ()
#endif
    {
    INT cdlg_id, status = SUCCESS;
    CHAR help_key[STMAX];
    INT i, len, pre_is_seperator = FALSE;
    
    status = wi_query_front_dialog (&cdlg_id);
    status = wi_query_helpkey (cdlg_id, help_key);
    len = strlen (help_key);
    /* convert the seperator.                                                       */
    if (len > 0)
        {
        for (i = 0; i < len; i++)
            {
            if (help_key[i] EQUALS'/')
                {
                if (!pre_is_seperator)
                    {
                    pre_is_seperator = TRUE;
                    help_key[i] = ' ';
                    }
                }
            else
                pre_is_seperator = FALSE;
            }
        }
    status = hf_do_help (help_key);
    return status;
    }
/* END:                                                                             */

/* Function Description -----------------------------------------------------       */
/*Description:                                                                      */
/*                                                                                  */
/*Prototype:                                                                        */
/*    static INT hfz_get_menu_item_help(INT menuid, INT itemid, INT rsrc_id, FILENAME rsrc_file,    */
/*        CHAR string[]);                                                           */
/*                                                                                  */
/*Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)        */
/*    menuid          -(INT)                                                        */
/*    itemid          -(INT)                                                        */
/*    rsrc_id         -(INT)                                                        */
/*    string          -(CHAR [])                                                    */
/*    rsrc_file       -(FILENAME)                                                   */
/*                                                                                  */
/*Return Value/Status:                                                              */
/*    None.                                                                         */
/*                                                                                  */
/*Scope:                                                                            */
/*    PRIVATE to <hf_help.c>                                                        */
/*                                                                                  */
/*Limitations/Assumptions:                                                          */
/*                                                                                  */
/*-----------------------------------------------------------------------------     */

/* BEGIN:                                                                           */
#if USE_PROTOTYPES
static INT hfz_get_menu_item_help (INT menuid, INT itemid, INT rsrc_id,
                                       FILENAME rsrc_file, CHAR string[])
#else
static INT hfz_get_menu_item_help (menuid, itemid, rsrc_id, rsrc_file, string)
INT menuid, itemid, rsrc_id;
CHAR string[];
FILENAME rsrc_file;
#endif
    {
    INT status;
    BOOL found;
    BOOL done;
    INT junk, len, i;
    INT id = 999;
    CHAR *ptr, s[STMAX];
    
    /* Load the resource.                                                           */
    
    rsmdopen (id);
    rspush (id);
    status = rsopen (rsrc_file);
    found = rsfetch (rsrc_id, "MENU", &ptr);
    if (found == TRUE)
        {
        /* skips the title and status lines.                                        */
        ptr = rsgets (ptr, s);
        ptr = rsgets (ptr, s);
        for (i = 0; i < itemid; i++)
            {
            ptr = rsgets (ptr, s);
            }
        strcpy (string, ts_snowhite (s));
        len = strlen (string);
        for (i = 0, done = FALSE; i < len AND!done; i++)
            {
            if (string[i] EQUALS'/')
                {
                string[i] = '\0';
                done = TRUE;
                continue;
                }
            if (string[i] EQUALS' ')
                string[i] = '_';
            }
        }
    rsclose (rsrc_file);
    rspop (&junk);
    rsmdclos (id);
    }
/* END:                                                                             */
