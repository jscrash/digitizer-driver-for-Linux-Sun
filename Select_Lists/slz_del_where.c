/* DEC/CMS REPLACEMENT HISTORY, Element SLZ_DEL_WHERE.C*/
/* *2    25-JUL-1990 20:07:54 VINCE "(SPR 5644) Header Standardization"*/
/* *1    19-JUN-1989 13:25:07 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element SLZ_DEL_WHERE.C*/
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
#include "esi_ts.h"
#include "esi_tc.h"

/* Function Description -----------------------------------------------------
Description:
    Function to delete "where" if there is one in the front of 
    select phrase.

Prototype:
    publicdef INT slz_del_where(CHAR **string);

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
publicdef INT slz_del_where (CHAR **string)
#else
publicdef INT slz_del_where (string)
CHAR **string;
#endif
    {
    
    INT status;                     /* RETURN STATUS */
    CHAR *cptr;                     /* TEMP CHAR STRING POINTER */
    CHAR *word;                     /* WORD FROM TS_NEXT_WORD */
    INT slen;                       /* LENGTH OF STRING */
    
    /* ********************************************************************** */
    
    status = SUCCESS;
    
    slen = strlen (*string);
    word = (CHAR *)tc_alloc (sizeof(CHAR) * slen + 1);
    ts_next_word (*string, word);
    ts_sto_upper (word);
    
    if (strcmp (word, "WHERE") EQUALS 0)
        {
        cptr = (CHAR *)tc_alloc (sizeof(CHAR) * (slen - 4));
        strcpy (cptr, (*string + 6));
        tc_free (*string);
        *string = cptr;
        }
    tc_free (word);
    
    return status;
    }
/* END:     */
