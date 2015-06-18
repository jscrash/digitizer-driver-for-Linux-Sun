/* DEC/CMS REPLACEMENT HISTORY, Element NL_SNL_STCT.C*/
/* *4    25-JUL-1990 15:05:57 VINCE "(SPR 5644) Header Standardization"*/
/* *3     1-JUN-1990 22:28:21 GILLESPIE "(SPR 1) Use nlz_copy_structure"*/
/* *2    12-MAR-1990 19:09:43 GILLESPIE "(SPR 6016) More error checking on mallocs; re-engineer stats calcs"*/
/* *1     5-MAR-1990 11:01:23 GILLESPIE "Rename files for Unix compliance (SPR 6015)"*/
/* DEC/CMS REPLACEMENT HISTORY, Element NL_SNL_STCT.C*/
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

/* Function Description -----------------------------------------------------
Description:

Prototype:
    publicdef INT nl_set_nlist_user_struct(NLIST_HEADER nlist,
        VOIDPTR user_struct_ptr, size_t nbyte);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    nlist           -(NLIST_HEADER)
    user_struct_ptr -(VOIDPTR)
    nbyte           -(size_t)

Return Value/Status:
    Function returns status from nlz_copy_structure.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT nl_set_nlist_user_struct (NLIST_HEADER nlist, VOIDPTR user_struct_ptr,
                                        size_t nbyte)
#else
publicdef INT nl_set_nlist_user_struct (nlist, user_struct_ptr, nbyte)
NLIST_HEADER nlist;
VOIDPTR user_struct_ptr;
size_t nbyte;
#endif
    {
    INT status;
    
    /* error checking   */
    INIT_TEST (nlist);
    
    if (nlist->user_struct != (VOIDPTR)0)
        tc_free ((VOIDPTR)nlist->user_struct);
    status = nlz_copy_structure (user_struct_ptr, nbyte, &nlist->user_struct,
                                 &nlist->user_struct_len);
    return status;
    }
/* END:     */
