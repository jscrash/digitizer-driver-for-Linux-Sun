/* DEC/CMS REPLACEMENT HISTORY, Element NL_FREE_NL.C*/
/* *3    25-OCT-1990 12:22:35 GILLESPIE "(SPR 31) Lint run - correct casting inconsistencies"*/
/* *2    25-JUL-1990 14:59:16 VINCE "(SPR 5644) Header Standardization"*/
/* *1     5-MAR-1990 10:59:25 GILLESPIE "Rename files for Unix compliance (SPR 6015)"*/
/* DEC/CMS REPLACEMENT HISTORY, Element NL_FREE_NL.C*/
/* DEC/CMS REPLACEMENT HISTORY, Element NL_FREE_NLIST.C*/
/* *1    19-JUN-1989 13:18:34 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element NL_FREE_NLIST.C*/
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

#include "esi_nl_int.h"
#include "esi_tc.h"

/* Function Description -----------------------------------------------------
Description:
    Free up everything associated with a pointlist.

Prototype:
    publicdef INT nl_free_nlist(NLIST_HEADER nlist);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    nlist           -(NLIST_HEADER)

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT nl_free_nlist (NLIST_HEADER nlist)
#else
publicdef INT nl_free_nlist (nlist)
NLIST_HEADER nlist;
#endif
    {
    INT status;
    
    INIT_TEST (nlist);
    
    /*    clear the N-list first.                */
    
    status = nl_clear_nlist (nlist);
    if (status)
        return status;
    
    /*   set the initialized flag to be '0' */
    /*   for protecting USER to use memory  */
    /*   before initialize it again.        */
    
    nlist->initialized = 0;
    
    tc_free ((VOIDPTR)nlist);
    
    return SUCCESS;
    }
/* END:     */
