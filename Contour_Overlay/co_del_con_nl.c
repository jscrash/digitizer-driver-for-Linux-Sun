/* DEC/CMS REPLACEMENT HISTORY, Element CO_DEL_CON_NL.C*/
/* *4    14-AUG-1990 23:37:45 GILLESPIE "(SPR 5644) Fix up last checkin"*/
/* *3    14-AUG-1990 11:42:10 VINCE "(SPR 5644) Header Standardization"*/
/* *2    27-JUL-1989 13:37:08 GILLESPIE "(SPR 1) Fix minor casting problem"*/
/* *1    19-JUN-1989 12:01:54 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element CO_DEL_CON_NL.C*/
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

#include "esi_co.h"

#ifndef ESI_NL_H
#include "esi_nl.h"
#endif

#ifndef ESI_TS_H
#include "esi_ts.h"
#endif

#ifndef ESI_HO_FILES_H
#include "esi_ho_files.h"
#endif

/* Function Description -----------------------------------------------------
Description:
    Routine to delete all n-lists in supplied contour file.
    
Prototype:
    publicdef INT co_delete_contour_nlists(FILENAME filename);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (M) filename    -(FILENAME) file to delete all n-lists from.

Return Value/Status:
    SUCCESS - Successful completion
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT co_delete_contour_nlists (FILENAME filename) 
#else
publicdef INT co_delete_contour_nlists (filename)
FILENAME filename;
#endif
/* * VARIABLE LIST * */
/* ----------------- */
    {
    UINT i, j;
    TCP tcp;
    
    /**************************************************************************/
    
    if (nl_inq_nlist_names (filename, &i, &tcp)EQUALS SUCCESS)
        {
        for (j = 0; j < i; j++)
            {
            nl_delete_nlist (filename, tcp[j]);
            }
        ts_tcp_free (tcp);
        }
    return SUCCESS;
    }

/*END :  */ 
