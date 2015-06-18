/* DEC/CMS REPLACEMENT HISTORY, Element NLM_DRAW_NL.C*/
/* *2    25-JUL-1990 14:49:45 VINCE "(SPR 5644) Header Standardization"*/
/* *1     5-MAR-1990 13:57:13 GILLESPIE "Rename files (SPR 6015)"*/
/* DEC/CMS REPLACEMENT HISTORY, Element NLM_DRAW_NL.C*/
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

/* Function Description -----------------------------------------------------
Description:
    An n-List methods function to draw the
    n-List.  Param points to an array of two
    unsigned integers that indicate the
    independent and dependent dimensions.

Prototype:
    publicdef INT nlm_draw_nlist(NLIST_HEADER nlist, VOIDPTR param);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    nlist           -(NLIST_HEADER)
    param           -(VOIDPTR)

Return Value/Status:
    Function returns the value of nl_draw_nlist.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:
    NOTE:   THIS IS A STUB - the actual method should be added
    by a program that actually requires drawing.  If a call to
    nl_draw_nlist was made, that any/all routines that use any
    n-List method would have this method linked into their
    application, which would cause unresolved GKS calls to be
    included... 

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT nlm_draw_nlist (NLIST_HEADER nlist, VOIDPTR param) 
#else
publicdef INT nlm_draw_nlist (nlist, param)
NLIST_HEADER nlist;
VOIDPTR param;
#endif
    {
    UINT *pint = (UINT *)param;
    UINT independent = *pint++;
    UINT dependent = *pint;

#if 0
    return nl_draw_nlist (nlist, independent, dependent);
#endif
    return FAIL;
    }
/* END:     */
