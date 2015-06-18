/* DEC/CMS REPLACEMENT HISTORY, Element NLZ_QROW_FLG.C*/
/* *4    27-JUL-1990 13:55:43 GILLESPIE "(SPR 1) Fix syntax error from last checkin "*/
/* *3    25-JUL-1990 14:54:17 VINCE "(SPR 5644) Header Standardization"*/
/* *2     9-APR-1990 10:32:02 GILLESPIE "(SPR 1) Remove superflurous checks"*/
/* *1     5-MAR-1990 10:56:41 GILLESPIE "Rename files for Unix compliance (SPR 6015)"*/
/* DEC/CMS REPLACEMENT HISTORY, Element NLZ_QROW_FLG.C*/
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
    Local routine to determine pointlist flag setting.

Prototype:
    publicdef INT nlz_inq_row_flag (NLIST_HEADER nlist);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    nlist           -(NLIST_HEADER) Pointer of given NLIST_HEADER.          

Return Value/Status:
    NL_END_OF_LIST
    NL_END_OF_TABLE
    NL_START_OF_LIST
    NL_START_OF_TABLE
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

    The n-List header has been validated by the caller

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT nlz_inq_row_flag (NLIST_HEADER nlist)
#else
publicdef INT nlz_inq_row_flag (nlist)
NLIST_HEADER nlist;
#endif
    {
    register INT flag = 0;          /*   returned flag.                          */
    
    /*  *************************************************************************  */
    /*   append NL_START_OF_TABLE flag.         */
    
    if (nlist->current_row EQUALS 1)
        {
        flag |= NL_START_OF_TABLE;
        if (nlist->current_table EQUALS 1)
            {
            /*   append NL_START_OF_LIST flag.           */
            
            flag |= NL_START_OF_LIST;
            }
        }
    if (nlist->current_row EQUALS nlist->current_table_ptr->nrows)
        {
        
        /*   append NL_END_OF_TABLE flag.           */
        
        flag |= NL_END_OF_TABLE;
        if (nlist->current_table EQUALS nlist->total_tables)
            {
            /*   append NL_END_OF_LIST flag              */
            
            flag |= NL_END_OF_LIST;
            }
        }
    return flag;
    }
/* END:     */
