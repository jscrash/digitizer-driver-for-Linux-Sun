/* DEC/CMS REPLACEMENT HISTORY, Element DR_SET_NAME.C*/
/* *3    24-SEP-1990 16:15:59 GILLESPIE "Beta Deltas"*/
/* *2    18-JUL-1990 08:48:47 GILLESPIE "(SPR 1) Add esi_dr_msg.h"*/
/* *1    19-JUN-1989 12:48:40 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element DR_SET_NAME.C*/
/******************************************************************************

                Copyright Exploration Systems, Inc. 1989
		   Unpublished -- All rights reserved

THIS SOFTWARE IS THE PROPRIETARY PROPERTY OF Exploration Systems, Inc.  AND MAY
CONTAIN CONFIDENTIAL TRADE SECRET INFORMATION. IT IS LICENSED FOR USE ON THE
DESIGNATED EQUIPMENT ON WHICH IT WAS ORIGINALLY INSTALLED AND  MAY NOT BE
MODIFIED, DUPLICATED OR COPIED IN ANY FORM WITHOUT PRIOR WRITTEN CONSENT OF

			Exploration Systems, Inc.
                        c/o Finder Graphics Systems
			201 Tamal Vista Blvd.
			Corte Madera, CA  94925
			(415) 927-0100

(The above notice does not constitute an admission of publication or
unrestricted dissemination of the work)

******************************************************************************/

#ifndef ESI_C_LIB_H
#include "esi_c_lib.h"
#endif

#ifndef ESI_DR_H
#include "esi_dr.h"
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

#ifndef ESI_DR_MSG_H
#include "esi_dr_msg.h"
#endif

/* Function Description -----------------------------------------------------
Description:
    Request a name from the user for the current line.
    If the type is FAULTS_TYPE then the name is required.

Prototype:
    publicdef INT dr_set_name (DR_DATA *dr_data);

Parameters:	    (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (M) dr_data - (DR_DATA *) The standard Electronic Drafting data structure.

Return Value/Status:
    SUCCESS	- Always returns success!

Scope:
    PUBLIC

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT dr_set_name (DR_DATA *dr_data)
#else
publicdef INT dr_set_name (dr_data)
DR_DATA *dr_data;
#endif
{
    INT  status;
    BOOL name_required;
    CHAR line_name[41];
    CHAR default_name[41];
    CHAR helpkey[41];

    helpkey[0]              = NUL;
    default_name[0]         = NUL;
    name_required           = FALSE;
    dr_data->object_changed = TRUE;
    
    strcpy( default_name, dr_data->current_line->line_name );
    if ( strlen(default_name) == 0 )
	{
	strcpy( default_name, "UNKNOWN");
	}

    if (strcmp(dr_data->current_object_type, FAULTS_TYPE) == 0)
	{
	name_required = TRUE;
	}

    FOREVER
        {
        wi_request_string(mg_message (DR_LINE_NAME), default_name, helpkey, 32,
                          line_name, &status);

        /* if a name is not required or one was entered leave the FOREVER */
        if (!name_required || line_name[0] != NUL) break;

        /* let the user know that a name is required. */
	wi_alert (0, "", "", mg(DR_NAME_REQUIRED));
        }

    if (status == SUCCESS) 
        {
	strcpy(dr_data->current_line->line_name, ts_sto_upper(line_name) );
        }

    return SUCCESS;
    }
/* END:	    */
