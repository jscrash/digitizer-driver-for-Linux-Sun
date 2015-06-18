/* DEC/CMS REPLACEMENT HISTORY, Element HTZ_ZALLOC.C*/
/* *2    17-AUG-1990 21:54:46 VINCE "(SPR 5644) Code Cleanup"*/
/* *1    19-JUN-1989 13:00:10 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element HTZ_ZALLOC.C*/
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

#include "esi_gl_defs.h"
#include "esi_ht.h"
#include "esi_tc.h"

/* Function Description -----------------------------------------------------
Description:
    Allocate and zero memory for the hash table modules.

Prototype:
    publicdef VOIDPTR htz_zalloc (size_t nbytes, INT flag);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) nbytes      -(size_t) The number of bytes to allocate.
    (I) flag        -(INT) One of HT_APPLICATION_TABLE or HT_SYSTEM_TABLE.

Return Value/Status:
    Function returns a pointer to the allocated memory.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef VOIDPTR htz_zalloc (size_t nbytes, INT flag)
#else
publicdef VOIDPTR htz_zalloc (nbytes, flag)
size_t nbytes;
INT flag;
#endif
    {
    return(VOIDPTR)tc_zalloc (nbytes);
    }
/* END:     */
