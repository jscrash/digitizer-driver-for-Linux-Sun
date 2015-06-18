/* DEC/CMS REPLACEMENT HISTORY, Element LD_LM2_HRZ_PIPE.C */
/* *2     1-MAY-1991 22:14:44 KEE "(SPR -1) Landmark Import & Export" */
/* *1     1-MAY-1991 21:11:47 KEE "Landmark Import & Export, Zycor Export and Mimic Export" */
/* DEC/CMS REPLACEMENT HISTORY, Element LD_LM2_HRZ_PIPE.C */
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

   NAME: LD_LM2_HRZ_PIPE.

   AUTHOR: Rod Hanks
   DATE:   June 1st, 1990
   DESCRIPTION: Black pipe application to get a list of desired horizons
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
#ifndef ESI_TS_H
#include "esi_ts.h"
#endif
#ifndef ESI_WI_H
#include "esi_wi.h"
#endif
#ifndef ESI_MG_H
#include "esi_mg.h"
#endif
#ifndef LD_LM2_IMP_WORK_H
#include "ld_lm2_imp_work.h"
#endif

#include "ld_lm2_imp_hrz.rh" 

#define maxItems 32

#if USE_PROTOTYPES
publicdef INT ld_lm2_hrz_pipe (LANDMARK_IMPSPACE *workspace)
#else
publicdef INT ld_lm2_hrz_pipe (workspace)
LANDMARK_IMPSPACE *workspace;
#endif
{
/****************************************************************************

	V a r i a b l e   D e c l a r a t i o n s .

******************************************************************************/

    INT     status;
    INT     i;
    INT	    dialogID;
    INT	    items[maxItems];
    INT	    theID;
    INT	    theItem;
    INT     loop;
    TCP     temp_tcp = (TCP)0;
    INT     theValue;
    CHAR    theText[255];
    INT	    l1;
    INT     l2;
    INT     xfer;

/****************************************************************************

	I n i t i a l i z a t i o n .

******************************************************************************/

    fi_push();
/*
					Disable all other windows and menus.
*/
    am_define_resource(AM_DIALOG, "LD_LM2_IMP_HRZ", LANDMARK_HRZFILE,
		       NULL_FUNCTION_PTR, &dialogID, NULL);    
/*
					Display the dialog box without
					bringing up a server for it.
*/
    for (i = 0; i < workspace->horizonCount; i++)
      {
	ts_tcp_append(&temp_tcp,workspace->horizonFiles[i]);
      }

    if (status = wi_set_selections(dialogID, HFL_NAMES,
				HFL_SCROLL, workspace->horizonCount,
				maxItems, temp_tcp))
	wi_alert(0, "", "wi_set_selections", mg_message(status));

    if (status = wi_set_default_button(dialogID, HFL_OKAY, 1))
	wi_alert(0, "", "wi_set_default_button", mg_message(status));

    if (status = wi_open_dialog(dialogID))
	wi_alert(0, "", "wi_open_dialog", mg_message(status));

    wi_request_dialog_item(&theID, &theItem, &theValue, theText);
    if (theItem == HFL_OKAY)
	{
        status = wi_query_selections(dialogID, HFL_NAMES, 
				&workspace->horizonCount,
				 items);
	for (l1 = 0; l1 < workspace->horizonCount; l1++)
	    {
	    for (l2 = 0; l2 < workspace->horizonCount - 1; l2++)
		{
		if (items[l2 + 1] < items[l2])
		    {
		    xfer = items[l2];
		    items[l2] = items[l2 + 1];
		    items[l2 + 1] = xfer;
		    }
		}
	    }
        for (loop = 0; loop < workspace->horizonCount; loop++)
	    {
	    if ((items[loop] - 1) != loop)
	        {
	        strcpy(workspace->horizonFiles[loop],
				workspace->horizonFiles[items[loop] - 1]);
	        strcpy(workspace->horizonNames[loop],
				workspace->horizonNames[items[loop] - 1]);
	        }
	    }
	}
/*
					Change our arrays to match the
					files they have chosen.  If they
					made a mistake, they will have to
					press the apply button again.  The
					loop assumes that the items in the
					array are sorted in ascending order.

					If they press selectall, then the
					list we have is already the list they
					want.
*/
    ts_tcp_free(temp_tcp);
    am_release_resource(dialogID);
    fi_pop();
/*
					Return the workstation back to normal
					mode.
*/
    return SUCCESS;
}

