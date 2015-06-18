/* DEC/CMS REPLACEMENT HISTORY, Element NL_QNL_STR.C*/
/* *6    25-OCT-1990 12:25:31 GILLESPIE "(SPR 31) Lint run - correct casting inconsistencies"*/
/* *5    25-JUL-1990 15:03:54 VINCE "(SPR 5644) Header Standardization"*/
/* *4    12-MAR-1990 19:09:17 GILLESPIE "(SPR 6016) More error checking on mallocs; re-engineer stats calcs"*/
/* *3     5-MAR-1990 10:49:20 GILLESPIE "(SPR 6015) Change file names to be compliant with Unix"*/
/* *2    24-SEP-1989 22:28:20 GILLESPIE "(SPR 200) New n-List 4.0 - Table/Row mods"*/
/* *1    19-JUN-1989 13:20:15 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element NL_QNL_STR.C*/
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

#include "esi_nl_int.h"
#include "esi_tc.h"
#include "esi_c_lib.h"

/* Function Description -----------------------------------------------------
Description:

Prototype:
    publicdef INT nl_inq_nlist_string(NLIST_HEADER nlist, INT opcode, UINT nallocated, 
        CHAR *string, size_t *nbytes);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    nlist           -(NLIST_HEADER)
    string          -(CHAR *)
    opcode          -(INT)
    nallocated      -(size_t)
    nbytes          -(size_t *)

Return Value/Status:
    SUCCESS - Successful completion.
    NL_INVALID_CODE
    NL_INVALID_POINTER
    NL_OVERFLOW
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT nl_inq_nlist_string (NLIST_HEADER nlist, INT opcode, size_t nallocated,
                                   CHAR *string, size_t *nbytes)
#else
publicdef INT nl_inq_nlist_string (nlist, opcode, nallocated, string, nbytes)
NLIST_HEADER nlist;
CHAR *string;
INT opcode;
size_t nallocated;
size_t *nbytes;
#endif
    {
    INT status = SUCCESS;
    
    INIT_TEST (nlist);
    *nbytes = 0;
    
    if (nallocated < 1)
        return NL_OVERFLOW;
    
    switch (opcode)
        {
    case NL_NLIST_FILENAME:
        if (nlist->data_filename IS_NOT_EQUAL_TO NULL)
            {
            if (strlen ((char *)nlist->data_filename) < nallocated)
                {
                strcpy ((char *)string, (char *)nlist->data_filename);
                *nbytes = strlen ((char *)nlist->data_filename);
                status = SUCCESS;
                }
            else
                status = NL_OVERFLOW;
            }
        else
            {
            status = NL_INVALID_POINTER;
            }
        break;
        
    case NL_NLIST_NAME:
        if (nlist->name IS_NOT_EQUAL_TO NULL)
            {
            if (strlen ((char *)nlist->name) < nallocated)
                {
                strcpy ((char *)string, (char *)nlist->name);
                *nbytes = strlen ((char *)nlist->name);
                status = SUCCESS;
                }
            else
                status = NL_OVERFLOW;
            }
        else
            {
            status = NL_INVALID_POINTER;
            }
        break;
        
    case NL_NLIST_NEXT:
        if (nlist->next_nlist IS_NOT_EQUAL_TO NULL)
            {
            if (strlen ((char *)nlist->next_nlist) < nallocated)
                {
                strcpy ((char *)string, (char *)nlist->next_nlist);
                *nbytes = strlen ((char *)nlist->next_nlist);
                status = SUCCESS;
                }
            }
        else
            {
            status = NL_INVALID_POINTER;
            }
        break;
        
    case NL_NLIST_PREVIOUS:
        if (nlist->prev_nlist IS_NOT_EQUAL_TO NULL)
            {
            if (strlen ((char *)nlist->prev_nlist) < nallocated)
                {
                strcpy ((char *)string, (char *)nlist->prev_nlist);
                *nbytes = strlen ((char *)nlist->prev_nlist);
                status = SUCCESS;
                }
            }
        else
            {
            status = NL_INVALID_POINTER;
            }
        break;
        
    default:
        status = NL_INVALID_CODE;
        
        }
    return status;
    
    }
/* END:     */
