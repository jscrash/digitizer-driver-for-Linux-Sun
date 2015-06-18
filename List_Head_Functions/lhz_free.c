/* DEC/CMS REPLACEMENT HISTORY, Element LHZ_FREE.C*/
/* *1    19-JUN-1989 13:05:23 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element LHZ_FREE.C*/

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
#include "esi_tc.h"

publicdef INT lhz_free(flag,pointer)
/************************* lhz_free *********************************
-FUNCTION:  This routine frees the area allocated by lhz_allocate.

-PARAMETERS:                                                      */

  BOOL   flag;   /*  Set to 1 if this is a system list.  Any other
                    value indicates an application area.          */
  VOIDPTR pointer;  /*  Reference to the area to deallocate.          */
                                                                  /*
-DESCRIPTION:

  This routine deallocates the indicated area.  If the value of flag
  is 1, then this is a system area.  Else, this is an application area.

-STATUS:

  This routine returns one of the following values:

    SUCCESS
    FAIL
    

-REVISIONS:

  May 28,1987 by Anthony P. Lucido -- Initial version.

************************** lhz_free ********************************/
{
/*                    Local Declarations                       */

  

#if 0
  return (((flag==1) ? tc_free(pointer) /*  system list  */
                     : am_free(pointer)));/*  application list.  */
#endif
    return tc_free(pointer);
}
