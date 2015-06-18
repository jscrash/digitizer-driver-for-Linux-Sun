/* DEC/CMS REPLACEMENT HISTORY, Element NL_QNL_INT.C */
/* *5    25-OCT-1990 12:25:13 GILLESPIE "(SPR 31) Lint run - correct casting inconsistencies" */
/* *4    25-JUL-1990 15:03:05 VINCE "(SPR 5644) Header Standardization" */
/* *3     9-APR-1990 10:34:07 GILLESPIE "(SPR 1) Remove superflurous checks" */
/* *2     5-MAR-1990 10:48:55 GILLESPIE "(SPR 6015) Change file names to be compliant with Unix" */
/* *1    24-SEP-1989 22:35:52 GILLESPIE "New n-List V4.0" */
/* DEC/CMS REPLACEMENT HISTORY, Element NL_QNL_INT.C */
/* DEC/CMS REPLACEMENT HISTORY, Element NL_INQ_NL_INT.C*/
/* *1    19-JUN-1989 13:18:51 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element NL_INQ_NL_INT.C*/
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

/* Function Description -----------------------------------------------------
Description:
    Inquire total number of tables in n-List.

Prototype:
    publicdef INT nl_inq_nlist_int(NLIST_HEADER nlist, INT opcode, INT *integer);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    nlist           -(NLIST_HEADER)
    opcode          -(INT)
    integer         -(INT *)

Return Value/Status:
    SUCCESS - Successful completion.
    NL_INVALID_CODE
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT nl_inq_nlist_int (NLIST_HEADER nlist, INT opcode, INT *integer)
#else
publicdef INT nl_inq_nlist_int (nlist, opcode, integer)
NLIST_HEADER nlist;
INT opcode, *integer;
#endif
    {
    INIT_TEST (nlist);
    
    switch (opcode)
        {
    case NL_NLIST_BLOCKSIZE:
        *integer = (INT)nlist->blocksize;
        break;
        
    case NL_NLIST_DATA_CLASS:
        *integer = nlist->nlist_data_class;
        break;
        
    case NL_NLIST_NCOL:
        *integer = (INT)nlist->nCol;
        break;
        
    case NL_NLIST_NROWS:
        *integer = (INT)nlist->total_rows;
        break;
        
    case NL_NLIST_NTABLES:
        *integer = (INT)nlist->total_tables;
        break;
        
    case NL_NLIST_USER_STRUCT_LEN:
        *integer = (INT)nlist->user_struct_len;
        break;
        
    default:
        return NL_INVALID_CODE;
        }
    return SUCCESS;
    
    }
/* END:     */
