/* DEC/CMS REPLACEMENT HISTORY, Element NL_PRG_NL.C*/
/* *4    25-OCT-1990 12:24:51 GILLESPIE "(SPR 31) Lint run - correct casting inconsistencies"*/
/* *3    25-JUL-1990 15:02:12 VINCE "(SPR 5644) Header Standardization"*/
/* *2    12-MAR-1990 19:08:48 GILLESPIE "(SPR 6016) More error checking on mallocs; re-engineer stats calcs"*/
/* *1     5-MAR-1990 11:00:00 GILLESPIE "Rename files for Unix compliance (SPR 6015)"*/
/* DEC/CMS REPLACEMENT HISTORY, Element NL_PRG_NL.C*/
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

/* Function Description -----------------------------------------------------
Description:
    To free all memoried allocated in tables, whose data is loaded and
    the data has not been updated, and reset the flag to be NL_DATA_UNLOADED.

Prototype:
    publicdef INT nl_purge_nlist (NLIST_HEADER nlist);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) nlist       -(NLIST_HEADER) The pointer of nlist header.

Return Value/Status:
    Function returns status from nl_purge_table.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT nl_purge_nlist (NLIST_HEADER nlist)
#else
publicdef INT nl_purge_nlist (nlist)
NLIST_HEADER nlist;
#endif
    {
    UINT i;
    INT status = SUCCESS;
    
    /* error checking.    */
    INIT_TEST (nlist);
    
    for (i = 1; i <= nlist->total_tables; i++)
        {
        status = nl_purge_table (nlist, i);
        }
    return status;
    }
/* END:     */
