/* DEC/CMS REPLACEMENT HISTORY, Element LD_LM2_SOURCE_PIPE.C */
/* *2     1-MAY-1991 22:14:56 KEE "(SPR -1) Landmark Import & Export" */
/* *1     1-MAY-1991 21:26:37 KEE "Landmark Import & Export, Zycor Export and Mimic Export" */
/* DEC/CMS REPLACEMENT HISTORY, Element LD_LM2_SOURCE_PIPE.C */
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

   NAME: LD_LM2_SOURCE_PIPE.C

   AUTHOR: Rod Hanks
   DATE:   June 13th, 1990
   DESCRIPTION: Black pipe application to get a list of desired sources
		from the user.

   ************************************************************************ */

#ifndef ESI_C_LIB_H
#include "esi_c_lib.h"
#endif
#ifndef ESI_AM_H
#include "esi_am.h"
#endif
#ifndef ESI_FI_H
#include "esi_fi.h"
#endif
#ifndef ESI_WI_H
#include "esi_wi.h"
#endif
#ifndef ESI_MG_H
#include "esi_mg.h"
#endif
#ifndef LD_LM2_EXP_WORK_H
#include "ld_lm2_exp_work.h" 
#endif

#include "ld_lm2_sources.rh" 


#if USE_PROTOTYPES
publicdef INT ld_lm2_source_pipe (LANDMARK_WORKSPACE *workspace, BOOL *goFlag)
#else
publicdef INT ld_lm2_source_pipe (workspace, goFlag)
LANDMARK_WORKSPACE *workspace;
BOOL *goFlag;
#endif
{
/****************************************************************************

	V a r i a b l e   D e c l a r a t i o n s .

******************************************************************************/

    INT status;
    INT	    dialogID;
    INT	    theID;
    INT	    theItem;
    INT	    theValue;
    CHAR    theText[255];
    BOOL    pleaseContinue;

/****************************************************************************

	I n i t i a l i z a t i o n .

******************************************************************************/

    fi_push();
/*
					Disable all other windows and menus.
*/
    am_define_resource(AM_DIALOG, "LD_LM2_SOURCES", LANDMARK_SOURCE,
               NULL_FUNCTION_PTR, &dialogID, NULL);    
/*
					Display the dialog box without
					bringing up a server for it.
*/
    if (status = wi_set_selections(dialogID, SRC_NAMES, 
				SRC_SCROLL, workspace->nsources,
				1, workspace->tcp_of_sources))
	wi_alert(0, "", "wi_set_selections", mg_message(status));

    if (status = wi_set_default_button(dialogID, SRC_OKAY, 1))
	wi_alert(0, "", "wi_set_default_button", mg_message(status));

    if (status = wi_open_dialog(dialogID))
	wi_alert(0, "", "wi_open_dialog", mg_message(status));

    pleaseContinue = TRUE;
    while (pleaseContinue)
	{
	wi_request_dialog_item(&theID, &theItem, &theValue, theText);
	switch (theItem)
	    {
	    case SRC_CANCEL:
	        am_release_resource(dialogID);
	        *goFlag = FALSE;
	        pleaseContinue = FALSE;
	        break;
	    case SRC_OKAY:
                status = wi_query_selections(dialogID, SRC_NAMES,
					&theValue, 
					&workspace->sourcePicked);
		if (theValue != 1 && workspace->nsources != 0)
		    {
		    wi_beep(1);
		    wi_alert(0, "", "",
			"You must choose the source for the output data.");
		    }
		else
		    {
	    	    am_release_resource(dialogID);
		    pleaseContinue = FALSE;
		    }
	        break;
	    }
	}
    fi_pop();
/*
					Return the workstation back to normal
					mode.
*/
    return SUCCESS;
}

