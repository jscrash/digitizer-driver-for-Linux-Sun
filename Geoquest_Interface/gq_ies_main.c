/* DEC/CMS REPLACEMENT HISTORY, Element GQ_IES_MAIN.C*/
/* *8    23-OCT-1991 13:28:21 JULIAN "(SPR 4068) Improved error messages"*/
/* *7    14-OCT-1991 17:29:25 JULIAN "(SPR 0) user interface fix"*/
/* *6    11-SEP-1991 13:50:26 JULIAN "(SPR 0) fixed crashes when moving wells"*/
/* *5    28-AUG-1991 17:28:51 JULIAN "(SPR 0) fixed crashes when select list is empty"*/
/* *4    23-AUG-1991 16:05:28 JULIAN "(SPR 0) removed comments on function calls"*/
/* *3     1-MAY-1991 22:54:09 KEE "(SPR -1) Landmark Import & Export, Zycor Export and Mimic Export"*/
/* *2    20-JUL-1990 11:52:52 VINCE "(SPR 5644) Header Standardization"*/
/* *1    21-JUN-1990 19:23:37 GILLESPIE "Geoquest Link"*/
/* DEC/CMS REPLACEMENT HISTORY, Element GQ_IES_MAIN.C*/
/* DEC/CMS REPLACEMENT HISTORY, Element GQ_IES_MAIN.C*/
/* *2    20-JUL-1990 11:52:52 VINCE "(SPR 5644) Header Standardization"*/
/* *1    21-JUN-1990 19:23:37 GILLESPIE "Geoquest Link"*/
/* DEC/CMS REPLACEMENT HISTORY, Element GQ_IES_MAIN.C*/
/*****************************************************************************/
/******************************************************************************

                Copyright Exploration Systems, Inc. 1989
                    Unpublished -- All rights reserved

THIS SOFTWARE IS THE PROPRIETARY PROPERTY OF Exploration Systems, Inc.  AND MAY
CONTAIN CONFIDENTIAL TRADE SECRET INFORMATION. IT IS LICENSED FOR USE ON THE
DESIGNATED EQUIPMENT ON WHICH IT WAS ORIGINALLY INSTALLED AND  MAY NOT BE
MODIFIED, DUPLICATED OR COPIED IN ANY FORM WITHOUT PRIOR WRITTEN CONSENT OF

                            Exploration Systems
                            c/o Finder Graphics Systems
                            201 Tamal Vista Blvd.
                            Corte Madera, CA  94925
                            (415) 927-0100

(The above notice does not constitute an admission of publication or
unrestricted dissemination of the work)

******************************************************************************/

#include "fgs_gq_ies.h"
#include "esi_am.h"
#include "esi_mg.h"
#include "esi_nl.h"
#include "esi_tc.h"
#include "esi_ts.h"
#include "esi_sl.h"
#include "esi_wi.h"
#include "gq_ies_cdlg.rh"
#include "interface.h"

#define GQ_IES_FILE "gq_ies_cdlg"
IES_STRUCT *ies;
#if USE_PROTOTYPES
static INT ies_cdlg_init (IES_STRUCT *ies);
static INT ies_cdlg_server (INT id, INT item, IES_STRUCT *ies);
static INT ies_cdlg_term ();
#else
static INT ies_cdlg_init ();
static INT ies_cdlg_server ();
static INT ies_cdlg_term ();
#endif

/* File Description ---------------------------------------------------------
Overview:
    IES main entry point.
    
Prototypes and one line descriptions of each function in the file.
Public_Functions:
    publicdef INT gq_ies_main(VOIDPTR ptr);
        
Private_Functions:
    static INT ies_cdlg_init( IES_STRUCT *ies );

    static INT ies_cdlg_server( INT id, INT item, IES_STRUCT *ies );
    
    static INT ies_cdlg_term();

-----------------------------------------------------------------------------*/

/* Function Description -----------------------------------------------------
Description:
    The main entrypoint for the GeoQuest IES export
    utility.

Prototype:
    publicdef INT gq_ies_main(VOIDPTR ptr);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) ptr         -(VOIDPTR) Dummy ptr passed from AM.

Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT gq_ies_main (VOIDPTR ptr)
#else
publicdef INT gq_ies_main (ptr)
VOIDPTR ptr;
#endif
    {
    INT status;
    
    /*----------------------------*/
    /* Inform application manager */
    /* Get the ies workspace      */
    /*----------------------------*/
    status = am_declare ("IES Export");
    
    status = am_define_workspace ("IES", AM_APPLICATION, sizeof(IES_STRUCT),
                                  (VOIDPTR) & ies);
    status = ies_cdlg_init (ies);   /* Set up the resource */
    return SUCCESS;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    Prepare and open the user interface dialog box.

Prototype:
    static INT ies_cdlg_init(IES_STRUCT *ies);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (M) ies         -(IES_STRUCT *) The IES data structure.

Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PRIVATE to <gq_ies_main.c>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static INT ies_cdlg_init (IES_STRUCT *ies)
#else
static INT ies_cdlg_init (ies)
IES_STRUCT *ies;
#endif
    {
    INT status;
    INT cnt;
    
    ies->message_field = IES_MSG_TXT;
    /* set up the resource */
    status = am_define_resource (AM_DIALOG, GQ_IES_FILE, IES_CDLG, ies_cdlg_server,
                                 &ies->cdlg_id, (VOIDPTR)ies);
    
    /* make list of seismic select lists */
    status = sl_inq_slist_names ("SEISMIC", &ies->seis_tcp, &cnt);
    
    /* send the list to the dialog box */
    if (cnt > 0 AND status EQUALS SUCCESS)
      {
	wi_set_selections (ies->cdlg_id, IES_SEIS_LIST, IES_SEIS_SCROLL, cnt,
			   1, ies->seis_tcp);
      }
    wi_activate (ies->cdlg_id, IES_SEIS_LIST, ON);

    /* make list of well select lists */
    status = sl_inq_slist_names ("WELLS", &ies->wells_tcp, &cnt);
    
    /* send the list to the dialog box */
    if (cnt > 0 AND status EQUALS SUCCESS)
      {
        wi_set_selections (ies->cdlg_id, IES_WELLS_LIST, IES_WELLS_SCROLL,
			   cnt, 1, ies->wells_tcp);
      }
    wi_activate (ies->cdlg_id, IES_WELLS_LIST, ON);
    
    /* disable edittext that define the current pick of select lists */
    wi_enable_dialog_item (ies->cdlg_id, IES_SEIS_CUR, OFF);
    wi_enable_dialog_item (ies->cdlg_id, IES_WELLS_CUR, OFF);

    wi_open_dialog (ies->cdlg_id);
    return SUCCESS;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    This function is called by the workstation
    interface whenever an event is generated from the dialog box.
    This is the main control function for this application.

Prototype:
    static INT ies_cdlg_server(INT id, INT item, IES_STRUCT *ies);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) id          -(INT) Dialog id from the Mac.
    (I) item        -(INT) Id of item generating event.
    (M) ies         -(IES_STRUCT *) The data structure.

Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PRIVATE to <gq_ies_main.c>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static INT ies_cdlg_server (INT id, INT item, IES_STRUCT *ies)
#else
static INT ies_cdlg_server (id, item, ies)
INT id;
INT item;
IES_STRUCT *ies;
#endif
    {
    INT status;
    INT nsel = -1;
    static INT item_selected = -1;
    INT unused;
    CHAR errmsg[100];
    
    switch (item)
        {
    case IES_QUIT:
        ies_cdlg_term ();
        break;
    case IES_EXPORT:
        wi_query_dialog_item (ies->cdlg_id, IES_PROJECT, &unused, ies->project_name);
        wi_query_dialog_item (ies->cdlg_id, IES_USER, &unused, ies->user_id);
        wi_query_dialog_item (ies->cdlg_id, IES_PASSWORD, &unused, ies->user_password);
        if (IS_EMPTY_STRING (ies->project_name) || IS_EMPTY_STRING (ies->user_id))
            {                       /* Error - must enter at least project_name & user_id */
            wi_beep (1);
            wi_set_dialog_text (ies->cdlg_id, IES_MSG_TXT,
                                "Please enter an IES PROJECT & USER ID");
            break;
            }
        /******************************************************************************/
        /*                           Lock the IES project                             */
        /******************************************************************************/
        
        /* First, initialize the IES system */
        
        wi_set_dialog_text (ies->cdlg_id, IES_MSG_TXT, "Initializing IES transfer");

        c_ies_init (&status);

        if (status != SUCCESS)
            {
	      wi_beep (1);
	      am_message(AM_ALERT,"Error in Initializing IES transfer. \n\
Please set up IES before running Finder.");
	      return SUCCESS;
            }


        /* Next, establish an IES user and project context. */

        ies_plockc (ies->user_id, ies->project_name, ies->user_password, &status);

        if (status != SUCCESS)
            {
	      wi_beep (1);
	      am_message(AM_ALERT,"Error in establishing the IES user and project. \n Please unlock the IES project before transfering data.");
	      return SUCCESS;
            }

        /******************************************************************************/
        /*                         Export the SEISMIC info                            */
        /******************************************************************************/
        /* User says ready to go. Verify critical data. */
        wi_query_dialog_item (ies->cdlg_id, IES_SEIS_CUR, &unused,
                              ies->seis_list_name);
        if (IS_STRING (ies->seis_list_name))
            {
            wi_set_dialog_text (ies->cdlg_id, IES_MSG_TXT,
                                "Start sending seismic line information");

            status = gqz_put_seismic (ies);      

 
            if (status != SUCCESS)
                {
                wi_beep (1);
		sprintf(errmsg,"Error in sending seismic line information.\n %s", 
			mg_message(status));
		am_message(AM_ALERT,errmsg);
                }
            else
                {
                wi_beep (1);
                wi_set_dialog_text (ies->cdlg_id, IES_MSG_TXT, "Export successful");
                }
            }
        /******************************************************************************/
        /*                         Now export the WELLS                               */
        /******************************************************************************/
        
        /* User says ready to go. Verify critical data. */
        wi_query_dialog_item (ies->cdlg_id, IES_WELLS_CUR, &unused,
                              ies->wells_list_name);
        if (IS_STRING (ies->wells_list_name))
            {
            wi_set_dialog_text (ies->cdlg_id, IES_MSG_TXT,
                                "Start retrieving well information");

           status = gqz_put_well (ies); 

            if (status != SUCCESS)
                {
                wi_beep (1);
		sprintf(errmsg,"Error in sending well information. \n%s", 
			mg_message(status));
		am_message(AM_ALERT,errmsg);
                }
            else
                {
                wi_beep (1);
                wi_set_dialog_text (ies->cdlg_id, IES_MSG_TXT, "Export successful");
                }
            }
        /******************************************************************************/
        /*               Finally,  export the FAULT TRACES                            */
        /******************************************************************************/

        gqz_put_ftrace (ies);
        
 
        wi_set_dialog_text (ies->cdlg_id, IES_MSG_TXT, "Unlocking the IES project");

        c_ies_unplock (&status);

        if (status != SUCCESS)
            {
	      wi_beep (1);
	      am_message(AM_ALERT,"Error in trying to unlock IES project.\n\
Please unlock it manually using IES command.");
	      return SUCCESS;
            }
        else
            {
            wi_set_dialog_text (ies->cdlg_id, IES_MSG_TXT,
                                "Data transmission completed");
            }
        break;
    case IES_SEIS_LIST:
        /* User has clicked on a select list. Find out which. */
        wi_query_selections (ies->cdlg_id, IES_SEIS_LIST, &nsel, &item_selected);
        if (nsel > 0)
            {                       /* Highlight the item */
            wi_set_default_sel (ies->cdlg_id, IES_SEIS_LIST, item_selected);
            /* Also put it down below. */
            wi_set_dialog_text (ies->cdlg_id, IES_SEIS_CUR,
                                ies->seis_tcp[item_selected - 1]);
            wi_set_dialog_text (ies->cdlg_id, IES_MSG_TXT, "Seismic list selected");
            }
        break;
    case IES_WELLS_LIST:
        /* User has clicked on a select list. Find out which. */
        wi_query_selections (ies->cdlg_id, IES_WELLS_LIST, &nsel, &item_selected);
        if (nsel > 0)
            {                       /* Highlight the item */
            wi_set_default_sel (ies->cdlg_id, IES_WELLS_LIST, item_selected);
            /* Also put it down below. */
            wi_set_dialog_text (ies->cdlg_id, IES_WELLS_CUR,
                                ies->wells_tcp[item_selected - 1]);
            wi_set_dialog_text (ies->cdlg_id, IES_MSG_TXT, "Well list selected");
            }
        break;
        }
    /* end of switch */
    return SUCCESS;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    Quit the application. 

Prototype:
    static INT ies_cdlg_term();

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    None.
    
Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PRIVATE to <gq_ies_mian.c>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static INT ies_cdlg_term ()
#else
static INT ies_cdlg_term ()
#endif
    {
    am_quit ();
    return SUCCESS;
    }
/* END:     */




