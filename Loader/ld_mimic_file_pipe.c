/* DEC/CMS REPLACEMENT HISTORY, Element LD_MIMIC_FILE_PIPE.C */
/* *2     1-MAY-1991 22:10:19 KEE "(SPR -1) Mimic Export" */
/* *1     1-MAY-1991 21:28:48 KEE "Landmark Import & Export, Zycor Export and Mimic Export" */
/* DEC/CMS REPLACEMENT HISTORY, Element LD_MIMIC_FILE_PIPE.C */
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

   NAME: LD_MIMIC_FILE_PIPE.C

   AUTHOR: Rod Hanks (BHP - Consultant)
   DATE:   June 8th, 1990
   DESCRIPTION: Black pipe application to get a list of file names
		from the user.

   MODIFY: Jim Green (Unocal)
   DATE:   November, 1990

   ************************************************************************ */

#ifndef ESI_FI_H
#include "esi_fi.h"
#endif
#ifndef ESI_AM_H
#include "esi_am.h"
#endif
#ifndef ESI_WI_H
#include "esi_wi.h"
#endif
#ifndef ESI_MG_H
#include "esi_mg.h"
#endif
#ifndef LD_MIMIC_WORK_H
#include "ld_mimic_work.h"
#endif

#include "ld_mimic_files.rh"

#if USE_PROTOTYPES
publicdef INT ld_mimic_file_pipe (MIMIC_WORKSPACE *workspace, 
				  INT mapOrdinal, INT faultVal,
				  INT xyzVal, BOOL *goFlag)
#else
publicdef INT ld_mimic_file_pipe (workspace, mapOrdinal, faultVal, xyzVal, goFlag)
MIMIC_WORKSPACE *workspace;
INT	 	 mapOrdinal;
INT              faultVal;
INT	 	 xyzVal;
BOOL		*goFlag;
#endif
{
/****************************************************************************

	V a r i a b l e   D e c l a r a t i o n s .

******************************************************************************/

    INT     status;
    INT	    dialogID;
    INT	    theID;
    INT	    theItem;
    INT	    theValue;
    CHAR    theText[255];
    INT	    loop;
    BOOL    pleaseContinue;

/****************************************************************************

	I n i t i a l i z a t i o n .

******************************************************************************/

    fi_push();
/*
					Disable all other windows and menus.
*/
    am_define_resource(AM_DIALOG, "LD_MIMIC_FILES", MIMIC_FILES,
               NULL_FUNCTION_PTR, &dialogID, NULL);    
/*
					Display the dialog box without
					bringing up a server for it.
*/
    if (strcmp(workspace->mapName, workspace->tcp_of_maps[mapOrdinal-1]) != 0)
	{
	strcpy(workspace->mapName, workspace->tcp_of_maps[mapOrdinal-1]);
	strcpy(theText, workspace->mapName);
	for (loop = 0; loop < strlen(theText); loop++)
	    {
	    if (theText[loop] < '0'
	    || (theText[loop] > '9' && theText[loop] < 'A')
	    || (theText[loop] > 'Z' && theText[loop] < 'a')
	    ||  theText[loop] > 'z')
		{
		theText[loop] = '_';
		}
	    }
	sprintf(workspace->faultFile, "%s_FALT.DATA", theText);
	sprintf(workspace->xyzFile, "%s_XYZ.DATA", theText);
	}
/*
					Initialize file names if it is the 
					first time thru or they have changed
					the map name.
*/
    if (status = wi_set_default_button(dialogID, MFL_OKAY, TRUE))
	wi_alert(0, "", "wi_set_default_button", mg_message(status));

    status = wi_set_dialog_text(dialogID, MFL_FAULT, workspace->faultFile);
    status = wi_set_dialog_text(dialogID, MFL_XYZ, workspace->xyzFile);

    status = wi_enable_dialog_item(dialogID, MFL_FAULT, faultVal);
    status = wi_enable_dialog_item(dialogID, MFL_XYZ, xyzVal);

    if (status = wi_open_dialog(dialogID))
	wi_alert(0, "", "wi_open_dialog", mg_message(status));

    pleaseContinue = TRUE;
    while (pleaseContinue)
	{
        wi_request_dialog_item(&theID, &theItem, &theValue, theText);
        switch (theItem)
	    {
	    case MFL_OKAY:
	    case MFL_CANCEL:
	        status = wi_query_dialog_item(dialogID, MFL_FAULT, &theValue, 
					  workspace->faultFile);
	        status = wi_query_dialog_item(dialogID, MFL_XYZ, &theValue, 
					  workspace->xyzFile);
	        if (theItem == MFL_OKAY)
		    {
	            if ((faultVal   != 0 && strlen(workspace->faultFile)   == 0)
		    ||  (xyzVal     != 0 && strlen(workspace->xyzFile)    == 0))
		        {
		        wi_beep(1);
		        wi_alert(0, "", "",
		    	    "You must give a file name.");
		        }
		    else
		        {
		        *goFlag = TRUE;
		        am_release_resource(dialogID);
		        pleaseContinue = FALSE;
		        }
		    }
	        else
		    {
	            *goFlag = FALSE;
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








