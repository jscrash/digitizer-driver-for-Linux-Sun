/* DEC/CMS REPLACEMENT HISTORY, Element NL_INV_NLM.C*/
/* *5    25-OCT-1990 12:23:10 GILLESPIE "(SPR 31) Lint run - correct casting inconsistencies"*/
/* *4    25-JUL-1990 15:00:00 VINCE "(SPR 5644) Header Standardization"*/
/* *3    27-MAR-1990 14:03:23 GILLESPIE "(SPR 1) Fixes"*/
/* *2    25-MAR-1990 11:58:07 SCD "(SPR 1) Handle methods with associated null function pointer."*/
/* *1     5-MAR-1990 10:59:42 GILLESPIE "Rename files for Unix compliance (SPR 6015)"*/
/* DEC/CMS REPLACEMENT HISTORY, Element NL_INV_NLM.C*/
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
#include "esi_ts.h"
#include "esi_nl_methods.ah"

/* Function Description -----------------------------------------------------
Description:
    Invoke the function specified by the method for the n_list.  
    Method must have been previously set by a call to nl_set_nlist_method.

Prototype:
    publicdef INT nl_invoke_nlist_method (NLIST_HEADER nlist, NLIST_METHOD method_id, 
        VOIDPTR parm);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    nlist           -(NLIST_HEADER)
    method_id       -(NLIST_METHOD)
    parm            -(VOIDPTR)

Return Value/Status:
    Function returns the value of (pMethod->function)(nlist, parm).
    NL_UNDEFINED_NLIST_METHOD
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT nl_invoke_nlist_method (NLIST_HEADER nlist, NLIST_METHOD method_id,
                                      VOIDPTR parm)
#else
publicdef INT nl_invoke_nlist_method (nlist, method_id, parm)
NLIST_HEADER nlist;
NLIST_METHOD method_id;
VOIDPTR parm;
#endif
    {
    NL_METHOD_HEADER *pMethod, *pRegistered;
    
    INIT_TEST (nlist);
    
    /* First check for an over-ride method */
    
    for (pMethod = nlist->method_list; pMethod IS_NOT_EQUAL_TO (NL_METHOD_HEADER *)0;
         pMethod = pMethod->next)
        {
        if (pMethod->method_id EQUALS method_id)
            {
            if (pMethod->function EQUALS (INT_FUNCTION_PTR)0)
                {
                /* This case may arise if the function was read and at that time
                no appropriate function was registered.  Check the registered list
                to see if one is available now... */
                
                for (pRegistered = nl_registered_methods;
                     pRegistered IS_NOT_EQUAL_TO (NL_METHOD_HEADER *)0;
                                                                                      pRegistered
                                                                                       =
                                                                                       pRegistered
                                                                                      ->
                                                                                      next
                                                                                      )
                    {
                    if (ARE_SAME (pMethod->function_name, pRegistered->function_name))
                        {
                        pMethod->function = pRegistered->function;
                        break;
                        }
                    }
                }
            if (pMethod->function EQUALS (INT_FUNCTION_PTR)0)
                {
                break;
                }
            return(pMethod->function)(nlist, parm);
            }
        }
    /* Still couldn't find one, so try the (compile time) default list */
    
    for (pMethod = nl_default_methods;
         pMethod->method_id IS_NOT_EQUAL_TO NL_NULL_METHOD; pMethod++)
        {
        if (pMethod->method_id EQUALS method_id)
            {
            if (pMethod->function EQUALS (INT_FUNCTION_PTR)0)
                {
                break;
                }
            return (pMethod->function)(nlist, parm);
            }
        }
    return NL_UNDEFINED_NLIST_METHOD;
    }
/* END:     */
