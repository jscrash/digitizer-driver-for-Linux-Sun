/* DEC/CMS REPLACEMENT HISTORY, Element LD_LM2_EDIT_HRZ.C */
/* *2     1-MAY-1991 22:18:22 KEE "(SPR -1) Landmark Import & Export" */
/* *1     1-MAY-1991 22:16:55 KEE "Landmark Import & Export, Zycor Export and Mimic Export" */
/* DEC/CMS REPLACEMENT HISTORY, Element LD_LM2_EDIT_HRZ.C */
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

/* ************************************************************************

   NAME: LD_LM2_EDIT_HRZ.C

   AUTHOR: Rod Hanks
   DATE:   June 1st, 1990
   DESCRIPTION: Black pipe application to allow the user to edit horizon
		names.

   ************************************************************************ */

#ifndef ESI_C_LIB_H
#include "esi_c_lib.h"
#endif
#ifndef ESI_AM_H
#include "esi_am.h"
#endif
#ifndef ESI_TS_H
#include "esi_ts.h"
#endif
#ifndef ESI_WI_H
#include "esi_wi.h"
#endif
#ifndef ESI_MG_H
#include "esi_mg.h"
#endif
#ifndef ESI_FI_H
#include "esi_fi.h"
#endif
#ifndef LD_LM2_IMP_WORK_H
#include "ld_lm2_imp_work.h"
#endif

#include "ld_lm2_imp_edit.rh"
#include "ld_lm2_imp_name.rh"

#if USE_PROTOTYPES
static INT setSelectorBox(LANDMARK_IMPSPACE *workspace, 
			      TCP *temp_tcp, VOIDPTR *temp_workspace, 
			      INT dialogID);
#else
static INT setSelectorBox();
#endif

#if USE_PROTOTYPES
publicdef INT ld_lm2_edit_hrz (LANDMARK_IMPSPACE *workspace)
#else
publicdef INT ld_lm2_edit_hrz (workspace)
LANDMARK_IMPSPACE *workspace;
#endif
{
/****************************************************************************

	V a r i a b l e   D e c l a r a t i o n s .

******************************************************************************/

    VOIDPTR temp_workspace = (VOIDPTR) 0;
    INT     status;
    INT	    dialogID;
    INT	    editID;
    INT	    theID;
    INT	    theItem;
    INT	    loop;
    TCP     temp_tcp = (TCP) 0;
    INT     theValue;
    CHAR    theText[255];
    BOOL    continuation;
    BOOL    subcontinuation;
    INT     chosenCount;
    INT     item[2];

/****************************************************************************

	I n i t i a l i z a t i o n .

******************************************************************************/

    fi_push();
/*
					Disable all other windows and menus.
*/
    am_define_resource(AM_DIALOG, "LD_LM2_IMP_EDIT", LANDMARK_EDITLIST,
               NULL_FUNCTION_PTR, &dialogID, NULL);    
/*
					Display the dialog box without
					bringing up a server for it.
*/
    setSelectorBox(workspace, &temp_tcp, &temp_workspace, dialogID);
    if (status = wi_set_default_button(dialogID, HEL_EDIT, 1))
	wi_alert(0, "", "wi_set_default_button", mg_message(status));

    if (status = wi_open_dialog(dialogID))
	wi_alert(0, "", "wi_open_dialog", mg_message(status));

    continuation = TRUE;
    while (continuation)
	{
        wi_request_dialog_item(&theID, &theItem, &theValue, theText);
        switch (theItem)
	    {
	    case HEL_EDIT:
                status = wi_query_selections(dialogID, HEL_NAMES, 
					     &chosenCount, item);
		if (chosenCount == 0)
		    {
		    wi_alert(0, "", "",
				"Chose a horizon or press 'DONE'");
		    }
		else
		    {
                    fi_push();
                    status = am_define_resource(AM_DIALOG, "LD_LM2_IMP_NAME", 
				LANDMARK_HRZNAME, NULL_FUNCTION_PTR, 
				&editID, NULL);    
		    status = wi_set_dialog_text(editID, HNM_FILE,
					workspace->horizonFiles[item[0] - 1]);
		    status = wi_enable_dialog_item(editID, HNM_FILE, FALSE);
		    status = wi_set_dialog_text(editID, HNM_NAME,
					workspace->horizonNames[item[0] - 1]);
		    status = wi_set_default_button(editID, HNM_OKAY, 1);

		    status = wi_open_dialog(editID);

/*
					Display the dialog box without
					bringing up a server for it.
*/
		    subcontinuation = TRUE;
		    while (subcontinuation)
			{
	                status = wi_request_dialog_item(&theID, &theItem, 
							&theValue, theText);
			switch (theItem)
			  {
			  case HNM_OKAY:
			    status = wi_query_dialog_item(editID, HNM_NAME, 
					&theValue, theText);
			    if (strlen(theText) == 0)
			        {
				wi_alert(0, "", "", 
					"Horizon names cannot be blank");
			        }
			    else
			        {
				strncpy(workspace->horizonNames[item[0]-1],
					theText, 30);
				setSelectorBox(workspace, &temp_tcp, 
					       &temp_workspace, dialogID);
				subcontinuation = FALSE;
			        }
			    break;
			  case HNM_CANCEL:
			    subcontinuation = FALSE;
			  }
			}
/*
					If they press okay and their results
					are okay, squirrel them away.
*/
                    am_release_resource(editID);
                    fi_pop();
		    }
		break;
	    case HEL_DONE:
		continuation = FALSE;
	    }
	}
    ts_tcp_free(temp_tcp);
    am_release_resource(dialogID);
    fi_pop();
/*
					Return the workstation back to normal
					mode.
*/
    return SUCCESS;
}

static INT setSelectorBox(workspace, temp_tcp, temp_workspace, dialogID)
LANDMARK_IMPSPACE   *workspace;
TCP		    *temp_tcp;
VOIDPTR		    *temp_workspace;
INT	             dialogID;
{
    INT     bufspace;
    INT     len1;
    INT	    loop;
    INT     status;
    CHAR    *tempPtr;
    CHAR    *ptr1;

    if (*temp_tcp != (TCP) 0)
	{
    	ts_tcp_free(*temp_tcp);
	}
    if (*temp_workspace != (VOIDPTR) 0)
	{
	am_free(*temp_workspace);
	}
    bufspace = workspace->horizonCount*131;
    *temp_workspace = am_allocate(AM_APPLICATION, bufspace);
    tempPtr = (CHAR *) *temp_workspace;
    for (loop = 0; loop < workspace->horizonCount; loop++)
	{
	ptr1 = (CHAR *)workspace->horizonFiles[loop];
	len1 = strlen(workspace->horizonFiles[loop]);
	while (len1>0)
	    {
	    *tempPtr++ = *ptr1++;
	    len1--;
	    }
	*tempPtr++ = ' ';
	*tempPtr++ = '-';
	*tempPtr++ = ' ';
	ptr1 = (CHAR *)workspace->horizonNames[loop];
	len1 = strlen(workspace->horizonNames[loop]);
	while (len1>0)
	    {
	    *tempPtr++ = *ptr1++;
	    len1--;
	    }
	*tempPtr++ = 0;
	}
    *temp_tcp = ts_make_tcp(*temp_workspace, workspace->horizonCount);
    if (status = wi_set_selections(dialogID, HEL_NAMES,
				HEL_SCROLL, workspace->horizonCount,
				1, *temp_tcp))
	wi_alert(0, "", "wi_set_selections", mg_message(status));
    return SUCCESS;
    }

