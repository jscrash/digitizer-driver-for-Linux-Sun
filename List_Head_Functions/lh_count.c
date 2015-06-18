/* DEC/CMS REPLACEMENT HISTORY, Element LH_COUNT.C*/
/* *1    19-JUN-1989 13:05:39 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element LH_COUNT.C*/

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
INT lh_count(head_pointer)
/************************* lh_count *********************************
-FUNCTION:  This routine returns the number of elements in the referenced
            list.

-PARAMETERS:                                                      */

  lh_listhead *head_pointer;/*  Pointer to the list.              */
                                                                  /*
-DESCRIPTION:

  This routine returns either the number of elements in the referenced
  list (if the head_pointer value is valid), or the value 0 (if the
  head_pointer does not reference a valid list, or is NULL).

-STATUS:

  None returned.

-REVISIONS:

  May 19,1987 by Anthony P. Lucido -- Initial version.

************************** lh_count ********************************/
{
/*                    Local Declarations                       */

  
  if (head_pointer==(lh_listhead *)NULL) return ((INT)0);/*  Return 0 if the
                                                        list is empty.      */
  if (head_pointer->SECURITY!=lh_secure_value) return ((INT)0);
                                                    /*  Return 0 if this 
                                                        is an invalid list. */
  return (head_pointer->NUMBER_OF_NODES);/*  If all is ready, return the
                                             list size.                     */
}
