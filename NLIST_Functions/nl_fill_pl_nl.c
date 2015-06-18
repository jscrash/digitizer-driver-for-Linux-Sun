/* DEC/CMS REPLACEMENT HISTORY, Element NL_FILL_PL_NL.C */
/* *2    18-MAR-1991 18:09:52 JULIAN "(SPR 0) Lease Mapping Init Checkin" */
/* *1    18-MAR-1991 17:59:07 JULIAN "0" */
/* DEC/CMS REPLACEMENT HISTORY, Element NL_FILL_PL_NL.C */
/* DEC/CMS REPLACEMENT HISTORY, Element NL_DRAW_NL.C*/
/* *1    13-AUG-1990 17:05:05 SYSTEM "Finder 6.1"*/
/* DEC/CMS REPLACEMENT HISTORY, Element NL_DRAW_NL.C*/
/* DEC/CMS REPLACEMENT HISTORY, Element NL_DRAW_NL.C*/
/* *2    25-JUL-1990 14:58:19 VINCE "(SPR 5644) Header Standardization"*/
/* *1     5-MAR-1990 10:59:16 GILLESPIE "Rename files for Unix compliance (SPR 6015)"*/
/* DEC/CMS REPLACEMENT HISTORY, Element NL_DRAW_NL.C*/
/* DEC/CMS REPLACEMENT HISTORY, Element NL_DRAW_NLIST.C*/
/* *2    24-SEP-1989 22:27:51 GILLESPIE "(SPR 200) New n-List 4.0 - Table/Row mods"*/
/* *1    19-JUN-1989 13:18:24 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element NL_DRAW_NLIST.C*/
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
    Draws a N-list in the current GKS environment.  The two columns
    must be columns that can be converted ti real number( all except
    BOOL and CHAR).  This call will create table breaks at table bound-
    aries and null values. No external non-GKS clilpping will be done in
    this call.

Prototype:
    publicdef INT nl_draw_nlist(NLIST_HEADER nlist, UINT independent, UINT dependent);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    nlist           -(NLIST_HEADER) The given N-list pointer.
    independent     -(UINT) The independent axis column.
    dependent       -(UINT) The dependent axis diemnsion.    

Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT nl_fill_polygon_nlist (NLIST_HEADER nlist, UINT independent, UINT dependent)
#else
publicdef INT nl_fill_polygon_nlist (nlist, independent, dependent)
NLIST_HEADER nlist;
UINT independent;
UINT dependent;
#endif
    {
    INT i;                          /* index. */
                                    /* error checking for the initialization of */
                                    /* N-list and the available column.      */
    INIT_TEST (nlist);
    
    COLUMN_TEST (nlist, independent);
    
    COLUMN_TEST (nlist, dependent);
    
    EMPTY_TEST (nlist);
    for (i = 0; i < nlist->total_tables; i++)
        {
        /* draw each column.                 */
        
        nl_fill_polygon_table (nlist, i + 1, independent, dependent);
        }
    return SUCCESS;
    
    }
/* END:     */
