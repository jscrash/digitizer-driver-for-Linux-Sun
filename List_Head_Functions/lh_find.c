/* DEC/CMS REPLACEMENT HISTORY, Element LH_FIND.C*/
/* *1    19-JUN-1989 13:05:54 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element LH_FIND.C*/

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
#include "esi_ho.h"

INT  lh_find(header_pointer, key_pointer, data_pointer, data_length)
/************************* lh_find *********************************
-FUNCTION:  This routine determines if there is a node in the given 
            list having a key equal to that specified.  If not, an
            error code is returned.  Else, the data area is copied
            to a dynamically created area, the lists' CURRENT_NODE
            value is set to reference the located node, and success
            is returned.

-PARAMETERS:                                                      */

    lh_listhead *header_pointer;/*  Points to the list to be searched.      */
    VOIDPTR  key_pointer;    /*  Points to the key for use in the search.*/
    VOIDPTR  data_pointer;   /*  Receives the data.                      */
    INT  *data_length;          /*  Receives the data area length.          */
                                                                  /*
-DESCRIPTION:

  This routine determines if the given list has an element with the supplied 
  key value.  If not, an error code is returned.  Else, a copy of the data
  area for the node is returned, after the value of CURRENT_NODE for the list
  is set to reference the located node.

  Note that, if the data area pointer is given as the NULL pointer value, the
  data is not copied.

-STATUS:

  The value of the function is set to one of:

    SUCCESS
    LH_NULL_HEADER_POINTER
    LH_NULL_KEY_POINTER
    LH_NO_MATCH
    LH_BAD_LIST_HEADER

-REVISIONS:

  May 22,1987 by Anthony P. Lucido -- Initial version.

************************** lh_find ********************************/
{
/*                    Local Declarations                       */

      lh_listnode *ptr;
      INT          datalen;
      BOOL         signed_keys,c_string_keys;
      INT          keylen;
/*
    Check the parameters.
*/
    if (header_pointer==(lh_listhead *)NULL)
        return LH_NULL_HEADER_POINTER;

    if (key_pointer == (VOIDPTR)0)
        return LH_NULL_KEY_POINTER;

    if (header_pointer->SECURITY!=lh_secure_value)
        return LH_BAD_LIST_HEADER;


/*
    Parameters OK.  Start the test.
*/
    if (header_pointer->FIRST_NODE==(lh_listnode *)NULL)
        return LH_NO_MATCH; /*  Cannot search an empty list.  */


    signed_keys = header_pointer->SIGNED_KEYS;

    ptr = ((ptr=(lh_listnode *)header_pointer->CURRENT_NODE)
                              ==(lh_listnode *)NULL) ? 
                (lh_listnode *)header_pointer->FIRST_NODE : ptr;

    keylen = header_pointer->KEY_LENGTH;

    c_string_keys = header_pointer->C_STRING_KEYS;

    switch (lh_cmpkey (
                        key_pointer  ,/*  Incoming test key.    */
                        (VOIDPTR)ptr->KEY_AREA,/*  Key for this node.    */
                        keylen       ,/*  Key length.           */
                        signed_keys  ,/*  Flag.                 */
                        c_string_keys)/*  C strings flag.       */
           )
      {
          case LH_FIRST_KEY_LESS:  
                   /*  The incoming key is less than the nodes' key.  */
                  goto Descending_search;

          case LH_EQUAL_KEYS:  
                   /*  Found it.                                      */
                  goto Copy_data;

          case LH_FIRST_KEY_GREATER:  
                   /*  The incoming key is greater than the nodes' key.*/
                  goto Ascending_search;

          default:  /*  Something happened.  Treat it as a no match.   */
                  return LH_NO_MATCH;
        }

Ascending_search:

  while (1)
   {
    if ((ptr=(lh_listnode *)ptr->NEXT_NODE)==(lh_listnode *)NULL)
        return LH_NO_MATCH;/*  No match.  */

    switch (lh_cmpkey (
                        key_pointer  ,/*  Incoming test key.    */
                        (VOIDPTR)ptr->KEY_AREA,/*  Key for this node.    */
                        keylen       ,/*  Key length.           */
                        signed_keys  ,/*  Flag.                 */
                        c_string_keys)/*  C strings flag.       */
           )
      {
          case LH_FIRST_KEY_LESS:  
                   /*  Ran out of candidate nodes.    */
              return LH_NO_MATCH;

          case LH_EQUAL_KEYS:  
                  /*  Found it.  */
                  goto Copy_data;

          default: ;
      }
     }
Descending_search:

  while (1)
   {
    if ((ptr=(lh_listnode *)ptr->LAST_NODE)==(lh_listnode *)NULL)
        return LH_NO_MATCH;/*  No match.  */

    switch (lh_cmpkey (
                        key_pointer  ,/*  Incoming test key.    */
                        (VOIDPTR)ptr->KEY_AREA,/*  Key for this node.    */
                        keylen       ,/*  Key length.           */
                        signed_keys  ,/*  Flag.                 */
                        c_string_keys)/*  C strings flag.       */
           )
      {
          case LH_EQUAL_KEYS:  
                   /*  Found it.  */
                  goto Copy_data;

          case LH_FIRST_KEY_GREATER:  
                   /*  Ran out of candidate nodes.  The test key went
                       to being greater than the current key.  Can't
                       do that, else we passed over where the key 
                       should have been if one was there, of the test
                       value.    */
              return LH_NO_MATCH;

          default: ;
      }
     }

Copy_data:  /*  Copy the node data to the receiving area.  */
      header_pointer->CURRENT_NODE = ptr;/*  Set the current node value. */

      datalen = (header_pointer->DATA_LENGTH==0) ?
                    ptr->DATA_LENGTH : 
                    header_pointer->DATA_LENGTH;

    if (data_pointer!=(VOIDPTR)0)
      {
        hoblockmove (
                 (VOIDPTR)ptr->DATA_AREA,/*  Move the data from here      */
                 data_pointer,  /*  to here.  The count is       */
                 datalen);      /*  given by this parameter.     */
        if (data_length!=(INT *)NULL) *data_length = datalen;
      }
    else *data_length = 0;
      return SUCCESS;
}
