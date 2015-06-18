/* DEC/CMS REPLACEMENT HISTORY, Element NLM_SORT_NL.C*/
/* *2    25-JUL-1990 14:50:12 VINCE "(SPR 5644) Header Standardization"*/
/* *1     5-MAR-1990 13:57:33 GILLESPIE "Rename files (SPR 6015)"*/
/* DEC/CMS REPLACEMENT HISTORY, Element NLM_SORT_NL.C*/
/*****************************************************************************/
/******************************************************************************

                Copyright Finder Graphics Systems, Inc. 1990
                    Unpublished -- All rights reserved

THIS SOFTWARE IS THE PROPRIETARY PROPERTY OF Exploration Systems, Inc.  AND MAY
CONTAIN CONFIDENTIAL TRADE SECRET INFORMATION. IT IS LICENSED FOR USE ON THE
DESIGNATED EQUIPMENT ON WHICH IT WAS ORIGINALLY INSTALLED AND  MAY NOT BE
MODIFIED, DUPLICATED OR COPIED IN ANY FORM WITHOUT PRIOR WRITTEN CONSENT OF

                            Finder Graphics Systems
                            201 Tamal Vista Blvd.
                            Corte Madera, CA  94925
                            (415) 927-0100

(The above notice does not constitute an admission of publication or
unrestricted dissemination of the work)

******************************************************************************/

#include "esi_nl_int.h"
#include "esi_nl_meth_hdr.h"

/* Function Description -----------------------------------------------------
Description:
    An n-List methods function to sort the n-List.
    Param points to a structure which is used for two-way
    communications.

Prototype:
    publicdef INT nlm_sort_nlist(NLIST_HEADER nlist, VOIDPTR param);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    nlist           -(NLIST_HEADER)
    param           -(VOIDPTR)

Return Value/Status:
    Function returns status from nl_sort_nlist.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT nlm_sort_nlist (NLIST_HEADER nlist, VOIDPTR param) 
#else
publicdef INT nlm_sort_nlist (nlist, param)
NLIST_HEADER nlist;
VOIDPTR param;
#endif
    {
    NLM_SORT_HEADER *pSort = (NLM_SORT_HEADER *)param;
    
    return nl_sort_nlist (nlist, pSort->ncol, pSort->Col_List, pSort->sortFlags,
                          &pSort->outNlist);
    }
/* END:     */
