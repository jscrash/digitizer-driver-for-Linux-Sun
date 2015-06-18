/* DEC/CMS REPLACEMENT HISTORY, Element LHZ_ALLOCATE.C*/
/* *1    19-JUN-1989 13:05:21 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element LHZ_ALLOCATE.C*/

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


#include "esi_lh.h"
#ifndef ESI_TC_H
#include "esi_tc.h"
#endif

VOIDPTR lhz_allocate(flag, size)
/************************* lhz_allocate *********************************
-FUNCTION:  This routine allocates storage for the lh facility.  It will
            use am_allocate to create either a system or application 
            dynamic area.

-PARAMETERS:                                                      */

  BOOL    flag;  /*  Indicates whether this is a system or 
                    application area allocation.                  */
  INT     size;  /*  Gives the size (in characters) to allocate.   */
                                                                  /*
-DESCRIPTION:

  This routine allocates the given amount of storage, and designates it
  as either system or application storage.  If there are any errors, the
  routine returns the NULL pointer value.  Else, it returns a pointer to
  the allocated area.

  The value of flag is interpreted as:

     = 1:  this is a system area.
    else:  this is an application area.

-STATUS:

    The return value will be set to one of:
      NULL          -- could not perform the allocation,
      valid pointer -- address of the allocated area.

-REVISIONS:

  May 28,1987 by Anthony P. Lucido -- Initial version.

************************** lhz_allocate ********************************/
{
/*                    Local Declarations                       */

/*
    Check parameters.
*/
    if (size<1)
        /*  Bad length.  */
           return (VOIDPTR)0;

/*
    Allocate the storage.
*/

  return (VOIDPTR)tc_alloc(size);                          /*  Area size.*/
}
