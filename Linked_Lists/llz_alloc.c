/* DEC/CMS REPLACEMENT HISTORY, Element LLZ_ALLOC.C*/
/* *1    19-JUN-1989 13:06:21 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element LLZ_ALLOC.C*/

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

NAME:	LLZ_ALLOC

AUTHOR:	W. Jaeck, 21-March-1987

DESCRIPTION:

    LLZ_ALLOC does all memory allocation for the LL module.

    The memory allocation procedure depends on what type of list the
    allocation is done on behalf of.

    The function of LLZ_ALLOC is to determine which type of list
    is being manipulated, and allocate the memory accordingly using
    the am_allocate service.

*************************************************************************/

#include "esi_am.h"
#include "esi_tc.h"

/************************************************************************/
VOIDPTR llz_alloc(flags,size)
/************************************************************************/

INT flags;
INT size;
{
    INT am_type;		/* Application or global */
    VOIDPTR pointer;		/* Pointer to allocated memory */

/************************************************************************/


#if 0
    if (flags & LL_LIST_SYSTEM)
        am_type = AM_SYSTEM;
    else
    	if (flags & LL_LIST_GLOBAL)
            am_type = AM_GLOBAL;
        else
            am_type = AM_APPLICATION;

    pointer = am_allocate(am_type,size);
#endif

    pointer = tc_alloc(size);

    return pointer;
}
