/* DEC/CMS REPLACEMENT HISTORY, Element NL_SNL_METH.C */
/* *7    25-OCT-1990 12:27:01 GILLESPIE "(SPR 31) Lint run - correct casting inconsistencies" */
/* *6    25-JUL-1990 15:05:50 VINCE "(SPR 5644) Header Standardization" */
/* *5    17-MAR-1990 17:57:40 GILLESPIE "(SPR 1) Don't bother adding this function if it is already @ top of heap" */
/* *4     5-MAR-1990 10:50:49 GILLESPIE "(SPR 6015) Change file names to be compliant with Unix" */
/* *3     9-NOV-1989 13:56:21 GORDON "(SPR 5058) set nlist->method_list" */
/* *2    12-OCT-1989 09:53:09 GILLESPIE "(SPR 44) Fiz up methods" */
/* *1    24-SEP-1989 22:36:38 GILLESPIE "New n-List V4.0" */
/* DEC/CMS REPLACEMENT HISTORY, Element NL_SNL_METH.C */
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

#ifndef ESI_TC_H

#include "esi_tc.h"

#endif

#ifndef ESI_TS_H

#include "esi_ts.h"

#endif

/* Function Description -----------------------------------------------------
Description:
    Set the function to be used for the specified method for the n_list.   
    Method list grows by placing the new method at the head of the list.  

Prototype:
    publicdef INT nl_set_nlist_method(NLIST_HEADER nlist, NLIST_METHOD method_id, 
        NLM_FUNCTION_NAME function_name, INT_FUNCTION_PTR function);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    nlist           -(NLIST_HEADER)
    method_id       -(NLIST_METHOD)
    function_name   -(NLM_FUNCTION_NAME)
    function        -(INT_FUNCTION_PTR)

Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT nl_set_nlist_method (NLIST_HEADER nlist, NLIST_METHOD method_id,
                                   NLM_FUNCTION_NAME function_name,
                                   INT_FUNCTION_PTR function)
#else
publicdef INT nl_set_nlist_method (nlist, method_id, function_name, function)
NLIST_HEADER nlist;
NLIST_METHOD method_id;
NLM_FUNCTION_NAME function_name;
INT_FUNCTION_PTR function;
#endif
    {
    NL_METHOD_HEADER *pMethod;
    
    INIT_TEST (nlist);
    
    /* See if this function for this method is already at the top of the heap */
    for (pMethod = nlist->method_list; pMethod != (NL_METHOD_HEADER *)0;
                                                                                   pMethod
                                                                                    = 
                                                                                   pMethod
                                                                                   ->next
                                                                                   )
        {
        if (pMethod->method_id EQUALS method_id)
            {
            if (pMethod->function EQUALS function AND ARE_SAME (pMethod->function_name,
                                                                function_name))
                {
                return SUCCESS;
                }
            else
                {
                break;
                }
            }
        }
    /* No such luck, so add it */
    
    pMethod = ALLOCATE (NL_METHOD_HEADER);
    pMethod->next = nlist->method_list;
    strcpy ((char *)pMethod->function_name, (char *)function_name);
    pMethod->method_id = method_id;
    pMethod->function = function;
    nlist->method_list = pMethod;
    
    /* Now make sure this function is known to the nlist method pool */
    
    nl_register_nlist_method (method_id, function_name, function);
    
    return SUCCESS;
    
    }
/* END:     */
