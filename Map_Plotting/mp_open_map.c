/*  DEC/CMS REPLACEMENT HISTORY, Element MP_OPEN_MAP.C*/
/*  *4    18-JUL-1991 15:49:32 MING "(SPR 0) add map does not exist message"*/
/*  *3    12-FEB-1991 15:23:09 MING "(SPR -1) Fix from BHP"*/
/*  *2    17-AUG-1990 22:07:42 VINCE "(SPR 5644) Code Cleanup"*/
/*  *1    19-JUN-1989 13:13:45 SYSTEM ""*/
/*  DEC/CMS REPLACEMENT HISTORY, Element MP_OPEN_MAP.C*/
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

#include "esi_c_lib.h"
#include "esi_gl_defs.h"
#include "esi_mp.h"
#include "esi_am.h"
#include "esi_ts.h"
#include "esi_qs.h"
#include "esi_md_msg.h"

#ifndef ESI_MG_H

#include "esi_mg.h"

#endif

/* Function Description -----------------------------------------------------
Description:
    Routine to open a map from definitions stored in the
    data base.

Prototype:
    publicdef INT mp_open_map();

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    None.
    
Return Value/Status:
    SUCCESS - Successful completion.
    CANCEL
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT mp_open_map ()
#else
publicdef INT mp_open_map ()
#endif
    {
    
    PROGNAME ("MP_OPEN_MAP");
    INT status;
    PROJECT_NAME project;
    CHAR **names_tcp = 0;
    MAP_NAME map_name;
    INT n_names;
    INT i;
    CHAR msg[200];
    
    /* ********************************************************************** */
    
    fi_push ();
    
    /* * GET THE LIST OF MAPS AVAILABLE * */
    /* ---------------------------------- */
    
    qs_inq_c (QS_PROJECT_NAME, project, (INT *)0);
    status = mp_map_list (project, &names_tcp, &n_names);
    if (status != SUCCESS)
        {
        goto finished;
        }
    /* * GET USER SELECTION * */
    /* ---------------------- */
    
    wi_ssst ("", n_names, 1, names_tcp);
    wi_request_selection (mg_message (MD_PICK_MAP), &n_names, &i, &status);
    if (status != SUCCESS)
        {
        status = CANCEL;
        goto finished;
        }
    /** Hung Hoang - BHP 11-Feb-1991 **/
    if (n_names != 1)
	    {
        wi_beep (1);		
        am_message (AM_STATUS,"Please select a map.");
        status = CANCEL;
        goto finished;
        }
								
    /* * GET MAP DEF * */
    /* --------------- */
    
    i--;
    status = mp_read_map (names_tcp[i]);
    if (status EQUALS SUCCESS)
        {
        mp_init_map_server ();
        }
    else
	{
        sprintf (msg,"%s does not exist.\n",names_tcp[i]);
        am_message (AM_STATUS,msg);
        }

finished:
    fi_pop ();
    if (status != SUCCESS AND status != CANCEL)
        {
        am_message (AM_ALERT, mg_message (status));
        }
    ts_tcp_free (names_tcp);
    return status;
    }
/* END:     */
