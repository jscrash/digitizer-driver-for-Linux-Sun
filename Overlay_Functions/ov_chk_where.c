/* DEC/CMS REPLACEMENT HISTORY, Element OV_CHK_WHERE.C*/
/* *2    25-JUL-1990 19:58:56 VINCE "(SPR 5644) Header Standardization"*/
/* *1    19-JUN-1989 13:21:44 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element OV_CHK_WHERE.C*/
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

#include "esi_c_lib.h"
#include "esi_gl_defs.h"
#include "esi_tc.h"
#include "esi_ts.h"

/* Function Description -----------------------------------------------------
Description:
    Function to check if the supplied string starts with "WHERE"
    and remove it if it does.
                
    Added JGG: ALWAYS enclose statement in parentheses - 
    This means that the passed in string will ALWAYS be internally
    reallocated.  Caller must assure that the passed in string
    is a directly allocated beast (direct result of tc_alloc)

Prototype:
    publicdef INT ov_chk_where(CHAR **string);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    string          -(CHAR **) String to check.

Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT ov_chk_where (CHAR **string)
#else
publicdef INT ov_chk_where (string)
CHAR **string;
#endif
    {
    
    INT status;                     /* RETURN STATUS */
    CHAR *cptr;                     /* TEMP CHAR STRING POINTER */
    CHAR *word;                     /* WORD FROM TS_NEXT_WORD */
    INT slen;                       /* LENGTH OF STRING */
    
    /* ********************************************************************** */
    
    status = SUCCESS;
    
    if (*string EQUALS NULL)
        {
        return SUCCESS;
        }
    slen = strlen (*string) + 3;
    word = (CHAR *)tc_alloc (slen); /* allocate enough space for new string */
    cptr = ts_next_word (*string, word);    /* cstr points to second word */
    ts_sto_upper (word);
    
    if (strcmp (word, "WHERE") IS_NOT_EQUAL_TO 0)
        {
        /*-------------------------------------*/
        /* statement did not start with WHERE; */
        /* make cstr point back to first word  */
        /*-------------------------------------*/
        cptr = *string;
        
        }
    sprintf (word, "(%s)", cptr);   /* add parantheses */
    tc_free (*string);              /* free input string */
    *string = word;                 /* reset string to new address */
    
    return status;
    }
/* END:     */
