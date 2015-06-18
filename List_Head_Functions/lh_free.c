/* DEC/CMS REPLACEMENT HISTORY, Element LH_FREE.C*/
/* *1    19-JUN-1989 13:06:04 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element LH_FREE.C*/

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
INT lh_free(header_pointer)
/************************* lh_free *********************************
-FUNCTION:  This routine will totally deallocate a list, including all
            nodes, and the list header.  It does not change the value
            of the header_pointer variable.

-PARAMETERS:                                                      */

  lh_listhead *header_pointer;/*  Pointer to the head of the list.*/
                                                                  /*
-DESCRIPTION:

  This routine deallocates a complete tree.  The header structure and the 
  set of nodes are all deallocated.

-STATUS:

  This routine returns one of the following values:

    SUCCESS
    LH_BAD_LIST_HEADER
    LH_FREE_ERROR
    LH_NULL_HEADER_POINTER

-REVISIONS:

  May 29,1987 by Anthony P. Lucido -- Initial version.

************************** lh_free ********************************/
{
/*                    Local Declarations                       */

  lh_listnode *ptr,/*  Work pointer.                           */
              *tmp;/*  Temporary pointer.                      */
  BOOL         sys;/*  Set to 1 if this is a system list.      */

/*
    Check the parameters.
*/
    if (header_pointer==(lh_listhead *)NULL)
      return LH_NULL_HEADER_POINTER;

    if (header_pointer->SECURITY!=lh_secure_value)
      return LH_BAD_LIST_HEADER;

/*
    So, get the starting address and get rid of the node areas.
*/
  ptr = (lh_listnode *)(header_pointer->FIRST_NODE);

  sys = header_pointer->SYSTEM_LIST;

  header_pointer->SECURITY = lh_unsecure_value;/*  Just in case.      */

  while (ptr!=(lh_listnode *)NULL)
    {
      /*  Go through and delete the nodes, so long as there are nodes. */
      tmp = ptr;
      ptr = (lh_listnode *)(ptr->NEXT_NODE); 
      if ((lhz_free(sys,(BYTE *)(tmp->KEY_AREA))==FAIL) ||
          (lhz_free(sys,(BYTE *)(tmp->DATA_AREA))==FAIL) ||
          (lhz_free(sys,(BYTE *)(tmp))==FAIL))
            return LH_FREE_ERROR;
    }
/*
    Now, get the header, itself.
*/
  if (lhz_free(sys,(BYTE *)header_pointer)==FAIL)
            return LH_FREE_ERROR;
  return SUCCESS;
}
