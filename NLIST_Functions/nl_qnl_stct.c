/* DEC/CMS REPLACEMENT HISTORY, Element NL_QNL_STCT.C*/
/* *4    25-OCT-1990 12:25:26 GILLESPIE "(SPR 31) Lint run - correct casting inconsistencies"*/
/* *3    25-JUL-1990 15:03:46 VINCE "(SPR 5644) Header Standardization"*/
/* *2    27-MAR-1990 10:01:26 GILLESPIE "(SPR 1) Return more intelligent status codes"*/
/* *1     5-MAR-1990 11:00:34 GILLESPIE "Rename files for Unix compliance (SPR 6015)"*/
/* DEC/CMS REPLACEMENT HISTORY, Element NL_QNL_STCT.C*/

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
#include "esi_nl_msg.h"
#include "esi_nl_err.h"
#include "esi_ho.h"

/* **************************************************************************** */

#if USE_PROTOTYPES
publicdef INT nl_inq_nlist_user_struct (NLIST_HEADER nlist, size_t nallocated,
                                        VOIDPTR user_struct_ptr, size_t *nbytes)
#else
publicdef INT nl_inq_nlist_user_struct (nlist, nallocated, user_struct_ptr, nbytes)
NLIST_HEADER nlist;
size_t nallocated;
VOIDPTR user_struct_ptr;
size_t *nbytes;
#endif
/* %S inquire the user structure associated with n_list */
    {
    /* error checking.    */
    INIT_TEST (nlist);
    
    if (nlist->user_struct_len EQUALS 0)
        {
        return NL_NULL_VALUES_RETURNED;
        }
    if (user_struct_ptr EQUALS (VOIDPTR)0)
        {
        return NL_INVALID_POINTER;
        }
    if (nallocated EQUALS 0)
        {
        return NL_INVALID_ARRAY_SIZE;
        }
    *nbytes = (nlist->user_struct_len > nallocated) ?
              nallocated : nlist->user_struct_len;
    hoblockmove (nlist->user_struct, user_struct_ptr, *nbytes);
    return(nlist->user_struct_len > nallocated) ? NL_OVERFLOW : SUCCESS;
    }
