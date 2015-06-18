/* DEC/CMS REPLACEMENT HISTORY, Element NL_DEL_NLM.C*/
/* *3    25-OCT-1990 12:22:02 GILLESPIE "(SPR 31) Lint run - correct casting inconsistencies"*/
/* *2    25-JUL-1990 14:57:46 VINCE "(SPR 5644) Header Standardization"*/
/* *1     5-MAR-1990 10:59:08 GILLESPIE "Rename files for Unix compliance (SPR 6015)"*/
/* DEC/CMS REPLACEMENT HISTORY, Element NL_DEL_NLM.C*/
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

#ifndef ESI_TC_H

#include "esi_tc.h"

#endif

/* Function Description -----------------------------------------------------
Description:
    Delete the (first) function specified by the method for the n_list. 
    Method must have been previously set by a call to nl_set_nlist_method. 
    Note that if more than one function has been defined, then the most recently
    defined function is deleted.

Prototype:
    publicdef INT nl_delete_nlist_method(NLIST_HEADER nlist, NLIST_METHOD method_id);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    nlist           -(NLIST_HEADER)
    method_id       -(NLIST_METHOD)

Return Value/Status:
    SUCCESS - Successful completion.
    NL_UNDEFINED_NLIST_METHOD
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT nl_delete_nlist_method (NLIST_HEADER nlist, NLIST_METHOD method_id)
#else
publicdef INT nl_delete_nlist_method (nlist, method_id)
NLIST_HEADER nlist;
NLIST_METHOD method_id;
#endif
    {
    NL_METHOD_HEADER *pMethod;
    NL_METHOD_HEADER *previous_method = (NL_METHOD_HEADER *)0;
    
    INIT_TEST (nlist);
    
    for (pMethod = nlist->method_list;
         pMethod->next IS_NOT_EQUAL_TO (NL_METHOD_HEADER *)0; pMethod = pMethod->next)
        {
        if (pMethod->method_id EQUALS method_id)
            {
            if (previous_method EQUALS (NL_METHOD_HEADER *)0)
                {
                nlist->method_list = pMethod->next;
                }
            else
                {
                previous_method->next = pMethod->next;
                }
            tc_free ((VOIDPTR)pMethod);
            return SUCCESS;
            }
        previous_method = pMethod;
        }
    return NL_UNDEFINED_NLIST_METHOD;
    }
/* END:     */
