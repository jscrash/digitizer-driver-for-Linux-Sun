/* DEC/CMS REPLACEMENT HISTORY, Element LLZ_FREE.C*/
/* *1    19-JUN-1989 13:06:23 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element LLZ_FREE.C*/

/******************************************************************************

                Copyright Exploration Systems, Inc. 1989
		   Unpublished -- All rights reserved

THIS SOFTWARE IS THE PROPRIETARY PROPERTY OF Exploration Systems, Inc.  AND MAY
CONTAIN CONFIDENTIAL TRADE SECRET INFORMATION. IT IS LICENSED FOR USE ON THE
DESIGNATED EQUIPMENT ON WHICH IT WAS ORIGINALLY INSTALLED AND  MAY NOT BE
MODIFIED, DUPLICATED OR COPIED IN ANY FORM WITHOUT PRIOR WRITTEN CONSENT OF

			Exploration Systems, Inc.
			579 Market Street
			San Francisco, CA  USA 94105
			(415) 974-1600

(The above notice does not constitute an admission of publication or
unrestricted dissemination of the work)

******************************************************************************/

/*************************************************************************

NAME:	LLZ_FREE

AUTHOR:	W. Jaeck, 21-March-1987

DESCRIPTION:

    LLZ_FREE does all memory freeing for the LL module. Presumably, the
    memory which is being freed was allocated using LLZ_ALLOC.

*************************************************************************/

#include "esi_gl_defs.h"
#include "esi_c_lib.h"
#include "esi_ll.h"
#include "esi_am.h"

/************************************************************************/
INT llz_free(flags,pointer)
/************************************************************************/

INT flags;
VOIDPTR pointer;
{
    PROGNAME("llz_free");
    INT status;			/* Completion status */

/************************************************************************/


#if 0
				/*  For system list, dont use AM. */

    if (flags & LL_LIST_SYSTEM)
    	status = tc_free(pointer);

				/*  For non-system list, use AM. */

    else {
    	status = am_free(pointer);
        }
#endif
    status = tc_free(pointer);
    
    return status;
}
