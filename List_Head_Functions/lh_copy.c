/* DEC/CMS REPLACEMENT HISTORY, Element LH_COPY.C*/
/* *1    19-JUN-1989 13:05:36 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element LH_COPY.C*/

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
INT lh_copy(incoming, outgoing)
/************************* lh_copy *********************************
-FUNCTION:  This routine copies the contents of the incoming tree to
            the outgoing tree area.

-PARAMETERS:                                                      */

    lh_listhead *incoming,/*  Incoming tree.                      */
                *outgoing;/*  Outgoing tree.  Gets the copy.      */
                                                                  /*
-DESCRIPTION:

  This routine copies the contents of the incoming tree to the
  outgoing tree area.  The routine does not delete the contents of 
  the incoming tree.

-STATUS:

  This routine returns one of the following values:

    SUCCESS
    LH_NULL_HEADER_POINTER
    LH_BAD_LIST_HEADER
    LH_BAD_LENGTH
    LH_NO_SPACE

-REVISIONS:

  June 2,1987 by Anthony P. Lucido -- Initial version.

************************** lh_copy ********************************/
{
/*                    Local Declarations                       */
    lh_listnode *ptr1,*ptr2,*node;/*  Work pointers.           */
    BOOL         syslist;
	INT          datalen,keylen;    

/*
    Check the parameters.
*/
    if ((incoming==(lh_listhead *)NULL) || (outgoing==(lh_listhead *)NULL))  
      return LH_NULL_HEADER_POINTER;

    if ((incoming->SECURITY!=lh_secure_value) || 
        (outgoing->SECURITY!=lh_secure_value) ||
        (incoming->SIGNED_KEYS!=outgoing->SIGNED_KEYS) ||
        (incoming->DUPLICATE_KEYS!=outgoing->DUPLICATE_KEYS) ||
        (incoming->SYSTEM_LIST!=outgoing->SYSTEM_LIST))
      return LH_BAD_LIST_HEADER;

    if (((incoming->KEY_LENGTH) != (outgoing->KEY_LENGTH)) ||
        ((incoming->DATA_LENGTH)!=(outgoing->DATA_LENGTH)))
      return LH_BAD_LENGTH;

/*
    Set up the areas.
*/
    datalen = incoming->DATA_LENGTH;
    keylen  = incoming->KEY_LENGTH;
    syslist = incoming->SYSTEM_LIST;

    outgoing->NUMBER_OF_NODES = incoming->NUMBER_OF_NODES;

    if ((incoming->FIRST_NODE)==(lh_listnode *)NULL)
      {/*  Null list, so initialize the outgoing list, and leave.  */
        outgoing->NUMBER_OF_NODES = 0;
        outgoing->FIRST_NODE      = (lh_listnode *)NULL;
        outgoing->LAST_NODE       = (lh_listnode *)NULL;
        outgoing->CURRENT_NODE    = (lh_listnode *)NULL;

        return SUCCESS;
      }

/*
    Nodes are present, so copy them to the outgoing area.
*/

  ptr1 = (lh_listnode *)(incoming->FIRST_NODE);/*  Get started.  */

/*
    Create a new node, and copy in the key and data areas.
*/
      if ((node=lhz_makenode (syslist,/*  System or application? */
                       (BYTE *)ptr1->KEY_AREA,/*  Reference to the key.  */
                               keylen,/*  Length of the keys.    */
                       (BYTE *)ptr1->DATA_AREA,/*  Reference to the data. */
                             datalen))/*  Size of the data area. */
            ==(lh_listnode *)NULL)
         return LH_NO_SPACE;
    outgoing->FIRST_NODE = node;
    outgoing->LAST_NODE  = node;

    ptr2 = node;

      ptr1 = (lh_listnode *)(ptr1->NEXT_NODE);
  while (ptr1!=(lh_listnode *)NULL)
    {
      if ((node=lhz_makenode (syslist,/*  System or application? */
                       (BYTE *)ptr1->KEY_AREA,/*  Reference to the key.  */
                               keylen,/*  Length of the keys.    */
                      (BYTE *)ptr1->DATA_AREA,/*  Reference to the data. */
                             datalen))/*  Size of the data area. */
            ==(lh_listnode *)NULL)
         return LH_NO_SPACE;

/*
    Link on the node.
*/
      node->LAST_NODE = ptr2;/*  Back link.                   */
      node->NEXT_NODE = (lh_listnode *)NULL;/*  Null front link.             */
      ptr2->NEXT_NODE = node;/*  Front link of previous node. */
      ptr2            = node;/*  Reference this node, for next
                                  time.                       */

      outgoing->LAST_NODE      = (lh_listnode *)node;

      ptr1 = (lh_listnode *)(ptr1->NEXT_NODE);
    }
  outgoing->CURRENT_NODE   = outgoing->FIRST_NODE;

  return SUCCESS;
}
