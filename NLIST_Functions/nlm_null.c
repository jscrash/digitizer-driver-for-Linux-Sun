/* DEC/CMS REPLACEMENT HISTORY, Element NLM_NULL.C*/
/* *4    25-OCT-1990 12:17:01 GILLESPIE "(SPR 31) Lint run - correct casting inconsistencies"*/
/* *3    25-JUL-1990 14:49:51 VINCE "(SPR 5644) Header Standardization"*/
/* *2    27-MAR-1990 11:59:12 GILLESPIE "(SPR 1) The Null Method"*/
/* *1    27-MAR-1990 11:53:51 GILLESPIE "The Null Method"*/
/* DEC/CMS REPLACEMENT HISTORY, Element NLM_NULL.C*/
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
#include "esi_nl_msg.h"

/* Function Description -----------------------------------------------------
Description:

Prototype:
    publicdef INT nlm_null(void);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    None.
    
Return Value/Status:
    NL_NULL_METHOD_FUNCTION
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT nlm_null (void) 
#else
publicdef INT nlm_null ()
#endif
    {
    return NL_NULL_METHOD_FUNCTION;
    }
/* END:     */
