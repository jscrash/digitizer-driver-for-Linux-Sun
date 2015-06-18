/*  DEC/CMS REPLACEMENT HISTORY, Element MP_G_TTLBLK.C*/
/*  *2    17-AUG-1990 22:06:01 VINCE "(SPR 5644) Code Cleanup"*/
/*  *1    19-JUN-1989 13:12:14 SYSTEM ""*/
/*  DEC/CMS REPLACEMENT HISTORY, Element MP_G_TTLBLK.C*/
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
#include "esi_ts.h"
#include "esi_md_msg.h"

/* Function Description -----------------------------------------------------
Description:
    Routine to return a title block name, chosen from those
    currently in the data base.

Prototype:
    publicdef INT mp_get_ttlblk_name(CHAR *ttlblk_name);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    ttlblk_name     -(CHAR *)

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.
    CANCEL
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT mp_get_ttlblk_name (CHAR *ttlblk_name)
#else
publicdef INT mp_get_ttlblk_name (ttlblk_name)
CHAR *ttlblk_name;
#endif
    {
    
    PROGNAME ("MP_GET_TTLBLK_NAME");
    INT status;
    CHAR **names_tcp = 0;
    INT n_names;
    INT i;
    
    /* ********************************************************************** */
    
    /* * GET THE LIST OF TITLE BLOCKS AVAILABLE * */
    /* ------------------------------------------ */
    
    status = mp_ttlblk_list (&names_tcp, &n_names);
    if (status EQUALS SUCCESS)
        {
        
        /* * GET USER SELECTION * */
        /* ---------------------- */
        
        if (n_names > 0)
            {
            wi_ssst ("", n_names, 1, names_tcp);
            wi_request_selection ("Please select a Title Block", &n_names, &i,
                                  &status);
            if (status EQUALS SUCCESS)
                {
                strcpy (ttlblk_name, names_tcp[--i]);
                }
            else
                {
                status = CANCEL;
                }
            }
        else
            {
            status = FAIL;
            }
        }
    ts_tcp_free (names_tcp);
    return status;
    }
/* END:     */
