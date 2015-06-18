/* DEC/CMS REPLACEMENT HISTORY, Element NL_REG_NLM.C*/
/* *3    25-OCT-1990 12:26:26 GILLESPIE "(SPR 31) Lint run - correct casting inconsistencies"*/
/* *2    25-JUL-1990 15:04:55 VINCE "(SPR 5644) Header Standardization"*/
/* *1     5-MAR-1990 11:00:56 GILLESPIE "Rename files for Unix compliance (SPR 6015)"*/
/* DEC/CMS REPLACEMENT HISTORY, Element NL_REG_NLM.C*/
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
#include "esi_tc.h"
#include "esi_ts.h"
#include "esi_nl_methods.x"

/* Function Description -----------------------------------------------------
Description:
    Register the function specified by the
    method for the n_list.

    Method must NOT been previously set by a
    previous call to nl_set_nlist_method nor to
    this function.

    This establishes a pool of available
    functions registered by programs outside of
    the n-List system for usage when an n-List
    is read from a file and has been previously.

Prototype:
    publicdef INT nl_register_nlist_method(NLIST_METHOD method_id, NLM_FUNCTION_NAME function_name, 
        INT_FUNCTION_PTR pfunc);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    method_id       -(NLIST_METHOD)
    function_name   -(NLM_FUNCTION_NAME)
    pfunc           -(INT_FUNCTION_PTR)

Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT nl_register_nlist_method (NLIST_METHOD method_id,
                                        NLM_FUNCTION_NAME function_name,
                                        INT_FUNCTION_PTR pfunc)
#else
publicdef INT nl_register_nlist_method (method_id, function_name, pfunc)
NLIST_METHOD method_id;
NLM_FUNCTION_NAME function_name;
INT_FUNCTION_PTR pfunc;
#endif
    {
    NL_METHOD_HEADER *pRegistered;
    
    for (pRegistered = nl_registered_methods;
         pRegistered IS_NOT_EQUAL_TO (NL_METHOD_HEADER *)0;
                                                                          pRegistered =
                                                                          pRegistered->
                                                                          next)
        {
        if (ARE_SAME (pRegistered->function_name, function_name))
            {
            return SUCCESS;
            }
        }
    /*  We have scanned through all registered
        methods, and this aint one of em Make it a
        full-fedged member by addint it to the head
        of the list. */
    
    pRegistered = ALLOCATE (NL_METHOD_HEADER);
    strcpy ((char *)pRegistered->function_name, (char *)function_name);
    pRegistered->method_id = method_id;
    pRegistered->function = pfunc;
    
    /*  Link this function into the registered      */
    /*  function list                   */
    
    pRegistered->next = nl_registered_methods;
    nl_registered_methods = pRegistered;
    
    return SUCCESS;
    }
/* END:     */
