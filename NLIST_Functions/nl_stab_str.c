/* DEC/CMS REPLACEMENT HISTORY, Element NL_STAB_STR.C */
/* *6    25-OCT-1990 12:27:39 GILLESPIE "(SPR 31) Lint run - correct casting inconsistencies" */
/* *5    25-JUL-1990 15:07:19 VINCE "(SPR 5644) Header Standardization" */
/* *4     9-APR-1990 10:34:43 GILLESPIE "(SPR 1) Remove superflurous checks" */
/* *3    12-MAR-1990 19:10:10 GILLESPIE "(SPR 6016) More error checking on mallocs; re-engineer stats calcs" */
/* *2     5-MAR-1990 10:51:30 GILLESPIE "(SPR 6015) Change file names to be compliant with Unix" */
/* *1    24-SEP-1989 22:36:58 GILLESPIE "New n-List V4.0" */
/* DEC/CMS REPLACEMENT HISTORY, Element NL_STAB_STR.C */
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
#include "esi_c_lib.h"
#include "esi_tc.h"

/* Function Description -----------------------------------------------------
Description:
    Set the character string of the indicated table;
    this becomes the new current table.

Prototype:
    publicdef INT nl_set_table_string(NLIST_HEADER nlist, UINT table_num, INT opcode, 
        CHAR *string);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    nlist           -(NLIST_HEADER)
    table_num       -(UINT)
    opcode          -(INT)
    string          -(CHAR *)

Return Value/Status:
    SUCCESS - Successful completion.
    NL_INVALID_POINTER
    NL_INVALID_CODE
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT nl_set_table_string (NLIST_HEADER nlist, UINT table_num, INT opcode,
                                   CHAR *string)
#else
publicdef INT nl_set_table_string (nlist, table_num, opcode, string)
NLIST_HEADER nlist;
UINT table_num;
INT opcode;
CHAR *string;
#endif
    {
    TABLE_HEADER *ptable;
    UINT old_table_num, old_row_num;
    INT status = SUCCESS;
    
    /* error checking    */
    INIT_TEST (nlist);
    nl_inq_current_row (nlist, &old_row_num, &old_table_num);
    
    if (table_num EQUALS 0)
        {
        table_num = nlist->current_table;
        }
    if (string EQUALS NULL)
        {
        return NL_INVALID_POINTER;
        }
    ptable = nlz_set_table (nlist, table_num);
    if (ptable EQUALS NULL_TABLE)
        return NL_INVALID_TABLE;
    
    switch (opcode)
        {
    case NL_TABLE_NAME:
        if (ptable->name IS_NOT_EQUAL_TO (CHAR *)NULL)
            {
            tc_free ((VOIDPTR)ptable->name);
            }
        ptable->name = STRING_ALLOCATE (string);
        strcpy ((char *)ptable->name, (char *)string);
        break;
        
    default:
        status = NL_INVALID_CODE;
        }
    /* set back to the old current row.   */
    nl_set_current_row (nlist, old_row_num, old_table_num);
    
    return status;
    }
/* END:     */
