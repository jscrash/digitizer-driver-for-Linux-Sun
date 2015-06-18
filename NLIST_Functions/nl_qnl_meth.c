/* DEC/CMS REPLACEMENT HISTORY, Element NL_QNL_METH.C */
/* *5    25-OCT-1990 12:25:17 GILLESPIE "(SPR 31) Lint run - correct casting inconsistencies" */
/* *4    25-JUL-1990 15:03:31 VINCE "(SPR 5644) Header Standardization" */
/* *3     5-MAR-1990 10:49:04 GILLESPIE "(SPR 6015) Change file names to be compliant with Unix" */
/* *2    12-OCT-1989 09:53:04 GILLESPIE "(SPR 44) Fiz up methods" */
/* *1    24-SEP-1989 22:35:54 GILLESPIE "New n-List V4.0" */
/* DEC/CMS REPLACEMENT HISTORY, Element NL_QNL_METH.C */
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
#include "esi_c_lib.h"
#include "esi_nl_methods.x"

/* Function Description -----------------------------------------------------
Description:
    Query the function specified by the method for the n_list.   
    Method must have been previously set by a call to nl_set_nlist_method.  
    Note that if more than one function has been defined, then the most recently
    defined function is returned.

Prototype:
    publicdef INT nl_inq_nlist_method(NLIST_HEADER nlist, NLIST_METHOD method_id, 
        NLM_FUNCTION_NAME function_name, INT_FUNCTION_PTR *pfunction);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    nlist           -(NLIST_HEADER)
    method_id       -(NLIST_METHOD)
    function_name   -(NLM_FUNCTION_NAME)
    pfunction       -(INT_FUNCTION_PTR *)

Return Value/Status:
    SUCCESS - Successful completion.
    NL_UNDEFINED_NLIST_METHOD
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT nl_inq_nlist_method (NLIST_HEADER nlist, NLIST_METHOD method_id,
                                   NLM_FUNCTION_NAME function_name,
                                   INT_FUNCTION_PTR *pfunction)
#else
publicdef INT nl_inq_nlist_method (nlist, method_id, function_name, pfunction)
NLIST_HEADER nlist;
NLIST_METHOD method_id;
NLM_FUNCTION_NAME function_name;
INT_FUNCTION_PTR *pfunction;
#endif
    {
    NL_METHOD_HEADER *pMethod;
    
    INIT_TEST (nlist);
    
    for (pMethod = nlist->method_list;
         pMethod->next IS_NOT_EQUAL_TO (NL_METHOD_HEADER *)0; pMethod = pMethod->next)
        {
        if (pMethod->method_id EQUALS method_id)
            {
            *pfunction = pMethod->function;
            strcpy ((char *)function_name, (char *)pMethod->function_name);
            return SUCCESS;
            }
        }
    /* Still couldn't find one, so try the default list */
    
    for (pMethod = nl_default_methods;
         pMethod->method_id IS_NOT_EQUAL_TO NL_NULL_METHOD; pMethod++)
        {
        if (pMethod->method_id EQUALS method_id)
            {
            *pfunction = pMethod->function;
            strcpy ((char *)function_name, (char *)pMethod->function_name);
            return SUCCESS;
            }
        }
    return NL_UNDEFINED_NLIST_METHOD;
    }
/* END:     */
