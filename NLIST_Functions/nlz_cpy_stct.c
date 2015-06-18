/* DEC/CMS REPLACEMENT HISTORY, Element NLZ_CPY_STCT.C */
/* *3     8-JUN-1990 10:15:05 GILLESPIE "(SPR 1) Fix minor syntax errors in non-prototype declaration" */
/* *2    29-MAY-1990 12:14:28 GILLESPIE "(SPR 1) Initial checkin - function allocates and copies user structures" */
/* *1    29-MAY-1990 09:24:55 GILLESPIE "User structure copier for n-Lists" */
/* DEC/CMS REPLACEMENT HISTORY, Element NLZ_CPY_STCT.C */
/******************************************************************************

                Copyright Finder Graphics Systems, Inc. 1990
           Unpublished -- All rights reserved

THIS SOFTWARE IS THE PROPRIETARY PROPERTY OF Finder Graphics Systems, Inc.  AND
MAY CONTAIN CONFIDENTIAL TRADE SECRET INFORMATION. IT IS LICENSED FOR USE ON THE
DESIGNATED EQUIPMENT ON WHICH IT WAS ORIGINALLY INSTALLED AND  MAY NOT BE
MODIFIED, DUPLICATED OR COPIED IN ANY FORM WITHOUT PRIOR WRITTEN CONSENT OF

            Finder Graphics Systems, Inc.
            201 Tamal Vista Blvd
            Corte Madera, CA  USA 94925
            (415) 927-0100

(The above notice does not constitute an admission of publication or
unrestricted dissemination of the work)

******************************************************************************/

#include "esi_nl_int.h"

#ifndef ESI_TC_H
#include "esi_tc.h"
#endif

#ifndef ESI_HO_H
#include "esi_ho.h"
#endif

#if USE_PROTOTYPES
publicdef INT nlz_copy_structure(VOIDPTR ptr_in, size_t len_in,
        VOIDPTR *ptr_out, size_t *len_out)
#else
publicdef INT nlz_copy_structure(ptr_in, len_in, ptr_out, len_out)
VOIDPTR ptr_in;
size_t len_in;
VOIDPTR *ptr_out;
size_t *len_out;
#endif
{
    if (len_in > 0)
        {
        *ptr_out = (VOIDPTR)tc_alloc(len_in);
        if (*ptr_out == (VOIDPTR)0)
            {
            *len_out = 0;
            return NL_OVERFLOW;
            }
        *len_out = len_in;
        hoblockmove (ptr_in, *ptr_out, len_in);
        }
    else
        {
        *ptr_out = (VOIDPTR)0;
        *len_out = 0;
        }
        
    return SUCCESS;
}
    
