/* DEC/CMS REPLACEMENT HISTORY, Element GB_MAKE_OR_HI.C*/
/*  20   13-FEB-1992 16:30:53 JULIAN "(SPR 5770) took out update_enable_controls function and create a separate file*/
/*gb_upd_enb_ctl.c"*/
/* *19    2-OCT-1991 16:56:13 JULIAN "(SPR 1558) clear message when starting a new retrieval (point,get list,rectangle,polygon)"*/
/* *18   21-AUG-1991 16:13:49 JULIAN "(SPR 3749) added 0 rows msg when clear select list"*/
/* *17   20-AUG-1991 15:16:09 JULIAN "(SPR 3772) fixed false dimming of buttons"*/
/* *16   11-JUN-1991 13:43:27 JULIAN "(SPR 7330) remove FAIL msg when get empty select list"*/
/* *15    6-MAY-1991 16:48:13 JULIAN "(SPR 0) add select list in culture overlay for data mover"*/
/* *14    5-DEC-1990 12:10:24 JULIAN "(SPR 6261) dim lynx and lease data type"*/
/* *13   30-JUL-1990 17:22:16 JULIAN "(SPR 0) add lynx,lease select_list"*/
/* *12   27-JUN-1990 16:40:29 KEE "(SPR -1) Add am_declare(Make List);"*/
/* *11    1-APR-1990 20:03:29 SCD "(SPR 1) Add public function gb_set_hilite_exists to manage static hilite_exists[]."*/
/* *10   26-FEB-1990 18:57:48 JULIAN "(SPR 6010) Port to Decstation 3100 (ULTRIX)"*/
/* *9    12-JAN-1990 16:09:53 GORDON "(SPR 0) Put in kludge for Mac 512's"*/
/* *8    29-DEC-1989 10:58:47 SCD "(SPR 100) Handle CANCEL status return from sl_choose_list properly."*/
/* *7    27-NOV-1989 08:40:10 JULIAN "(SPR 100) Ensure state of dialog controls correct after each event."*/
/* *6    22-NOV-1989 08:59:14 GORDON "(SPR 0) To get the thing converted to stream mode"*/
/* *5    18-NOV-1989 09:48:42 HALL "(SPR 5075) Global overhaul"*/
/* *4    11-AUG-1989 20:37:23 CONROY "(SPR 0) Revised global, selects, SQL macros & fixes"*/
/* *3     8-AUG-1989 15:22:36 JULIAN "(SPR -1) [data_type] --> [(INT)data_type]"*/
/* *2    28-JUN-1989 09:18:30 JRB "(SPR 1) eliminate dead code"*/
/* *1    19-JUN-1989 12:51:21 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element GB_MAKE_OR_HI.C*/
/******************************************************************************

                Copyright Exploration Systems, Inc. 1989
           Unpublished -- All rights reserved

THIS SOFTWARE IS THE PROPRIETARY PROPERTY OF Exploration Systems, Inc.  AND MAY
CONTAIN CONFIDENTIAL TRADE SECRET INFORMATION. IT IS LICENSED FOR USE ON THE
DESIGNATED EQUIPMENT ON WHICH IT WAS ORIGINALLY INSTALLED AND  MAY NOT BE
MODIFIED, DUPLICATED OR COPIED IN ANY FORM WITHOUT PRIOR WRITTEN CONSENT OF

            Exploration Systems, Inc.
            579 Market Street
            San Francisco, CA  USA 94105
            (415) 974-1600

(The above notice does not constitute an admission of publication or
unrestricted dissemination of the work)

******************************************************************************/
/* ************************************************************************
                  PREFACE
   ************************************************************************

    FILE:     gb_make_or_hilite.c

    AUTHOR:   S. Darden, 21-Feb-87

    DESCRIPTION:

       gb_make_or_hilite handles all current list operations of the
       global menu processor


   ********************************************************************** */



/* **********************************************************************
                 INCLUDE FILES
   ********************************************************************** */

#include "esi_c_lib.h"
#include "esi_gb.h"
#include "esi_sl.h"
#include "esi_wi.h"
#include "esi_wi_rsrc.h"
#include "esi_nl.h"
#include "esi_qs.h"
#include "esi_mp.h"
#include "esi_am.h"
#include "esi_mg.h"
#include "esi_gb_msg.h"

/* **********************************************************************
               RESOURCE INCLUDE FILES
   ********************************************************************** */

#include "gb_mhl_cdlg.rh"

/* **********************************************************************
                 DEFINITIONS
   ********************************************************************** */

#define ERR_CANCEL 1               /* Operator depressed cancel key */
#define ERR_DONE   2               /* Operator depressed DONE key */
#define NULL_SELECT_LIST (NLIST_HEADER)0


/* **********************************************************************
                 DECLARATIONS
   ********************************************************************** */
#if USE_PROTOTYPES
static INT gb_make_or_hilite_operate ();
static VOID update_enable_controls ();
static INT dialog_events (INT id, INT item, BYTE *ptr);
static VOID gb_zz_set_prompt (INT num_selected);
#else
static INT gb_make_or_hilite_operate ();
static VOID update_enable_controls ();
static INT dialog_events ();
static VOID gb_zz_set_prompt ();
#endif

/* **********************************************************************
                APPLICATION GLOBAL DATA
   ********************************************************************** */

/* file name of resource file                                           */
static CHAR GB_MAKE_OR_HILITE_RSRC[] = "gb_mhl_cdlg";
static INT cdlg_1_nitems = 0;   /* remember dialog attributes across invocations    */
static INT cdlg_1_id = 0;
static SL_DATA_TYPE data_type = WELLS_DATA_TYPE;
static INT enabled_new[MAX_CD_ITEMS], enabled_old[MAX_CD_ITEMS];
#define MAX_TYPES 10
static INT hilite_exists[MAX_TYPES] = 
    {
    0
    };


#define MAX_PROMPT 60
static CHAR prompt[MAX_PROMPT];


/* **********************************************************************

    ROUTINE:  gb_make_or_hilite 

    DESCRIPTION:

    PARAMETERS:

     none

    NOTE:  GB_MAKE_OR_HILITE does some small housekeeping, then 
           passes control onto GB_MAKE_OR_HILITE_OPERATE to get the 
           MAKE or HIGHLIGHT dialog box up and running.
           It is called from the software (BOZO) bus, when the (ab)user
           chooses "Make or Highlight" from the GLOBAL menu (Robb 11/07/89).

   ********************************************************************** */

INT gb_make_or_hilite (dummy)
BYTE *dummy;

    {
    INT status;
    INT i;

    /* initialize dialog housekeeping to DISabled */

    for (i = 1; i < MAX_CD_ITEMS; i++)
        {
                    /* Note: the following switch has been
                       added as a temporary fix.  It seems
                       the Mac 512's have trouble enabling
                       Rectitem items.  The switch will prevent
                       any calls to enable or disable the
                       Rectitem items (or StatText items) -GS */
        switch(i)
            {
            case MHI_RT2: case MHI_RT1:
            case MHI_MESSAGE: 
            case MHI_ST1: case MHI_ST2:
            enabled_new[i] = enabled_old[i] = -1;
            break;          
            default:
            enabled_new[i] = 0;
            enabled_old[i] = -1;
            }
        }

    /* Clear the dialog message window */
    strcpy (prompt, "");

    status = gb_make_or_hilite_operate ();

    return status;
    }


/* **********************************************************************

    ROUTINE:  gb_make_or_hilite_operate

    DESCRIPTION:

        gb_make_or_hilite_operate manages the main dialog for
        selecting, hiliting, editing, lists, etc.

    NOTE:  GB_MAKE_OR_HILITE_OPERATE installs DIALOG_EVENTS as the
               "event handler" for the MAKE or HIGHLIGHT dialog box.
               It is called from GB_MAKE_OR_HILITE, in response to a call
               from GB_PROCESS_GLOBALS.         

    ********************************************************************** */

static INT gb_make_or_hilite_operate ()

    {
    INT status;
    INT id;
    INT item;
    INT class;
    INT event_class;
    INT value, len;
    CHAR text[32];
    WI_RSRC_INFO rsrc_struct_ptr;   /* used to query number of items    */

    wi_set_event_mode (WI_MENU_EVENT, WI_EVENT_MODE);
    wi_set_event_mode (WI_DIALOG_EVENT, WI_EVENT_MODE);
    wi_set_event_mode (WI_SELECT_EVENT, WI_EVENT_MODE);

    /* Bind resource file -- server function is DIALOG_EVENTS */
    am_declare("Make List");
    status = am_define_resource (AM_DIALOG, GB_MAKE_OR_HILITE_RSRC,
                    MAKE_OR_HIGHLIGHT, dialog_events,
                    &cdlg_1_id, NULL);
    if (status != SUCCESS)
        {
        am_message (AM_ALERT, "Cannot open resource file!");
        return status;
        }


    /* Find the number of items in the resource                         */
    status = wi_query_info (cdlg_1_id, &rsrc_struct_ptr);
    if (status != SUCCESS)
        return status;
    else
        cdlg_1_nitems = rsrc_struct_ptr.numitems;


    /* open any dialogs that should be up.                              */
    update_enable_controls ();
    wi_open_dialog (cdlg_1_id);

    return SUCCESS;
    }


/* **********************************************************************

     ROUTINE:  update_enable_controls

     DESCRIPTION:

      update_enable_controls ensures that active menus and dialogs
      reflect the current state of the application and data.

      Menu & dialog items are enabled/disabled as appropriate.
      Menu items are checked/unchecked as approriate.
      Dialog text & selectors are updated as approriate.

    ********************************************************************** */

static VOID update_enable_controls ()

    {
    INT i, if_current_list;
    INT num_maps_display = 0;

    /* is there a map currently defined?                                */
    mp_num_displayed_maps (&num_maps_display);

    enabled_new[MHI_WELLS] = TRUE;
    enabled_new[MHI_SEISMIC] = TRUE;
    enabled_new[MHI_LYNX] = TRUE;
    enabled_new[MHI_LEASE] = TRUE;

    enabled_new[MHI_RELATIONAL] = TRUE;
    enabled_new[MHI_GET_LIST] = TRUE;
    enabled_new[MHI_DELETE] = TRUE;
    enabled_new[MHI_CLEAR_CURRENT] = FALSE;

    enabled_new[MHI_DISPLAY_STATUS] = FALSE;    /* not implemented yet  */
    enabled_new[MHI_DONE] = TRUE;

    if_current_list = FALSE;

    switch (data_type)
        {
    case WELLS_DATA_TYPE:
        enabled_new[MHI_POINT] = (num_maps_display > 0) ? TRUE : FALSE;
        enabled_new[MHI_BAND_RECTANGLE] = (num_maps_display > 0) ?
            TRUE : FALSE;
        enabled_new[MHI_POLYGON] = (num_maps_display > 0) ? TRUE : FALSE;
        qs_inq_i (QS_WELL_LIST_EXISTS, &if_current_list);
        break;
    case SEISMIC_DATA_TYPE:
        enabled_new[MHI_POINT] = (num_maps_display > 0) ? TRUE : FALSE;
        enabled_new[MHI_BAND_RECTANGLE] = FALSE;
        enabled_new[MHI_POLYGON] = (num_maps_display > 0) ? TRUE : FALSE;
        qs_inq_i (QS_SEISMIC_LIST_EXISTS, &if_current_list);
        break;
    case LYNX_DATA_TYPE:
        enabled_new[MHI_POINT] = FALSE;
        enabled_new[MHI_BAND_RECTANGLE] = FALSE;
        enabled_new[MHI_POLYGON] = FALSE;
        qs_inq_i (QS_LYNX_LIST_EXISTS, &if_current_list);
        break;
    case LEASE_DATA_TYPE:
        enabled_new[MHI_POINT] = FALSE;
        enabled_new[MHI_BAND_RECTANGLE] = FALSE;
        enabled_new[MHI_POLYGON] = FALSE;
        qs_inq_i (QS_LEASE_LIST_EXISTS, &if_current_list);
        break;
    default:
        break;
        }
    enabled_new[MHI_HIGHLIGHT_CURRENT_LIST] = ((num_maps_display >
                                                0) &&  if_current_list) ?
        TRUE : FALSE;
    enabled_new[MHI_ERASE_HIGHLIGHT] = hilite_exists[(INT)data_type];
    enabled_new[MHI_SAVE_CURRENT_LIST] = if_current_list;
    enabled_new[MHI_CLEAR_CURRENT] = if_current_list;

    /* enable or disable any controls which have changed state          */

    for (i = 1; i < cdlg_1_nitems; i++)
        {
        if (enabled_new[i] != enabled_old[i])
            enabled_old[i] = enabled_new[i];
	wi_enable_dialog_item (cdlg_1_id, i, enabled_new[i]);
        }

    /* set radio button group to reflect current state                  */

    wi_set_dialog_value (cdlg_1_id, MHI_WELLS,
                         (data_type  ==  WELLS_DATA_TYPE));
    wi_set_dialog_value (cdlg_1_id, MHI_SEISMIC,
                         (data_type  ==  SEISMIC_DATA_TYPE));
    wi_set_dialog_value (cdlg_1_id, MHI_LYNX,
                         (data_type  ==  LYNX_DATA_TYPE));
    wi_set_dialog_value (cdlg_1_id, MHI_LEASE,
                         (data_type  ==  LEASE_DATA_TYPE));

    /* display the prompt or selection report in message area           */
    wi_set_dialog_text (cdlg_1_id, MHI_MESSAGE, prompt);

    return;
    }


/* **********************************************************************

     ROUTINE:  dialog_events ( id, item, ptr )

     DESCRIPTION:

      dialog_events services all picks on the main make/hilite dialog.

     PARAMETERS:
      -    item:     (INT) dialog item picked

     NOTE:  DIALOG_EVENTS has become the "event server" for the MAKE or
            HILITE list dialog box (Robb 11/03/89).

    ********************************************************************** */

static INT dialog_events (id, item, ptr)
INT id;
INT item;
BYTE *ptr;

    {
    INT num_selected, status, status2;

    NLIST_HEADER select_list, *select_list_temp;

    select_list = (NLIST_HEADER)0;
    status = SUCCESS;
    num_selected = 0;

    switch (item)
        {

        /* choose data type radio buttons   */

    case MHI_WELLS:
        data_type = WELLS_DATA_TYPE;
        break;

    case MHI_SEISMIC:
        data_type = SEISMIC_DATA_TYPE;
        break;

    case MHI_LYNX:
        data_type = LYNX_DATA_TYPE;
        break;

    case MHI_LEASE:
        data_type = LEASE_DATA_TYPE;
        break;

        /* make current list buttons        */

    case MHI_RELATIONAL:
        select_list = NULL_SELECT_LIST;
        status2 = gb_relational_retrieval (data_type, &select_list);
        if ((status2  ==  SUCCESS) &&  (select_list != NULL_SELECT_LIST))
            {
            /*-------------------------------------------*/
            /* don't need to "make list current"         */
            /* as always done by gb_relational_retrieval */
            /*-------------------------------------------*/
            status2 = nl_inq_nlist_int (select_list, NL_NLIST_NPOINTS,
                                        &num_selected);

            }
        gb_zz_set_prompt (num_selected);
        break;

    case MHI_POINT:
	/* clear message box (case 1558) */
	wi_set_dialog_text (cdlg_1_id, MHI_MESSAGE, "");

        select_list = NULL_SELECT_LIST;
        status2 = gb_point (data_type, &select_list);
        if ((status2  ==  SUCCESS) &&  (select_list != NULL_SELECT_LIST))
            {
            status2 = gb_make_list_current (select_list);
            status2 = nl_inq_nlist_int (select_list, NL_NLIST_NPOINTS,
                                        &num_selected);
            }
        gb_zz_set_prompt (num_selected);
        break;

    case MHI_BAND_RECTANGLE:
	/* clear message box (case 1558) */
	wi_set_dialog_text (cdlg_1_id, MHI_MESSAGE, "");

        select_list = NULL_SELECT_LIST;
        status2 = gb_well_rectangle (&select_list);
        if ((status2  ==  SUCCESS) &&  (select_list != NULL_SELECT_LIST))
            {
            status2 = gb_make_list_current (select_list);
            status2 = nl_inq_nlist_int (select_list, NL_NLIST_NPOINTS,
                                        &num_selected);
            }
        gb_zz_set_prompt (num_selected);
        break;

    case MHI_POLYGON:
	/* clear message box (case 1558) */
	wi_set_dialog_text (cdlg_1_id, MHI_MESSAGE, "");

        select_list = NULL_SELECT_LIST;

	status2 = sl_call_method(data_type,"GB_POLYGON",(VOIDPTR)&select_list);
        if ((status2  ==  SUCCESS) &&  (select_list != NULL_SELECT_LIST))
            {
            status2 = gb_make_list_current (select_list);
            status2 = nl_inq_nlist_int (select_list, NL_NLIST_NPOINTS,
                                        &num_selected);
            }
        gb_zz_set_prompt (num_selected);
        break;


    case MHI_GET_LIST:
	/* clear message box (case 1558) */
	wi_set_dialog_text (cdlg_1_id, MHI_MESSAGE, "");

        select_list = NULL_SELECT_LIST;
        status2 = sl_choose_list (data_type, &select_list);
        if ((status2  ==  SUCCESS) &&  (select_list != NULL_SELECT_LIST))
            {
            status2 = gb_update_current_status (select_list);
            status2 = nl_inq_nlist_int (select_list, NL_NLIST_NPOINTS,
                                        &num_selected);
            gb_zz_set_prompt (num_selected);
            }
        else if (status2 == CANCEL OR status2 == FAIL)
            {
            strcpy (prompt, "");
            }
        else
            {
            strcpy (prompt, mg_message (status2));
            }
        break;

    case MHI_DELETE:
        select_list = NULL_SELECT_LIST;
        status = gb_delete_list (data_type);
        break;

    case MHI_HIGHLIGHT_CURRENT_LIST:
        status2 = gb_highlight (data_type);
        hilite_exists[(INT)data_type] = (status2  ==  ERR_CANCEL) ? FALSE : TRUE;
        break;

    case MHI_ERASE_HIGHLIGHT:
        hl_erase_highlight (data_type);
        hilite_exists[(INT)data_type] = FALSE;
        break;

    case MHI_SAVE_CURRENT_LIST:
        gb_save_current_list (data_type);
        break;

    case MHI_CLEAR_CURRENT:
        gb_clear_current_list (data_type);
        gb_zz_set_prompt (0);
        select_list = NULL_SELECT_LIST;
        break;

    case MHI_DISPLAY_STATUS:
        break;

    case MHI_DONE:
        wi_close_dialog (cdlg_1_id);   /* done with this dialog box */

                am_quit(); /* a mate for AM_START done by BOZO bus */

        status = SUCCESS;
        break;

    default:
        break;
    }
    /*----------------------------------*/
    /* Post current status message, and */
    /* ensure that state of all dialog  */
    /* controls is correct.             */
    /*----------------------------------*/
    update_enable_controls();
    return status;
    }

/* **********************************************************************

     ROUTINE:  gb_zz_set_prompt (num_selected)

     DESCRIPTION:

      format the prompt string to display the number of
      items selected by the last user request.

    ********************************************************************** */

static VOID gb_zz_set_prompt (num_selected)
INT num_selected;

    {
    sprintf (prompt, mg_message (GB_ITEMS_SELECTED), num_selected);
    return;
    }

/* **********************************************************************

     ROUTINE:  gb_set_hilite_exists (data_type)

     DESCRIPTION:   Set the static variable to track which
                    hilite layers exist according to data type

        
    ********************************************************************** */

publicdef VOID gb_set_hilite_exists (data_type)
SL_DATA_TYPE data_type;
    {

    hilite_exists[(INT)data_type] = TRUE;
    }
