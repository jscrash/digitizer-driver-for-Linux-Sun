/* DEC/CMS REPLACEMENT HISTORY, Element LH_REPLACE.C*/
/* *1    19-JUN-1989 13:06:18 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element LH_REPLACE.C*/

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

INT lh_replace(header_pointer, key_pointer, data_pointer, data_length)
/************************* lh_replace *********************************
-FUNCTION:  This routine replaces the data component of the current node.

-PARAMETERS:                                                      */

      lh_listhead *header_pointer;/*  Pointer to the list to be
                                      modified.
                                  */
      VOIDPTR	key_pointer,   /*  Points to the new entries' 
                                      key value.  If this is NULL
                                      then an error condition is
                                      returned.
                                  */
                data_pointer;  /*  Points to the data to
                                      be entered.  If this is NULL
                                      then a node containing all
                                      NULL characters, but of the
                                      proper length, will be created.
                                  */
      INT          data_length;   /*  This gives the length of the 
                                      provided data element.  If the
                                      list is initialized to have
                                      a single data length for all
                                      nodes then this value is ignored.
                                      Else, it provides the length of
                                      the incoming data node.  If that
                                      lenght is negative, then an error
                                      condition is returned.
                                  */

                                                                  /*
-DESCRIPTION

  This routine replaces the data component of the current node.  The
  incoming key must match that of the current node for the replace to 
  be performed.  If the keys do not match, an error is issued and the 
  routine terminates.

-STATUS:

  This routine will return one of the following return code values:

    SUCCESS
    LH_NULL_KEY_POINTER
    LH_BAD_LENGTH
    LH_NULL_HEADER_POINTER
    LH_BAD_LIST_HEADER
    LH_NO_SPACE
    LH_BAD_KEY
    LH_FREE_ERROR

-REVISIONS:

  May 19, 1987 by Anthony P. Lucido -- Initial version.

************************** lh_replace ********************************/
{
/*                    Local Declarations                       */

        lh_listnode *ptr;
        INT          datalen;
/*
    Check the incoming parameters.
*/
    if (key_pointer==(VOIDPTR)0) /*  Invalid key pointer?  */
        return LH_NULL_KEY_POINTER;

    if (header_pointer==(lh_listhead *)NULL) /*  Invalid list header?  */
        return LH_NULL_HEADER_POINTER;

     if (header_pointer->SECURITY!=lh_secure_value)/*  Invalid list header?*/
        return LH_BAD_LIST_HEADER;

    if  ((data_length<=0) && (header_pointer->DATA_LENGTH<=0))
      /*  Lengths mismatch, or the incoming length is zero when the 
           node data lenght must be positive.  */

        return LH_BAD_LENGTH;

    ptr = (lh_listnode *)(header_pointer->CURRENT_NODE);
    if (ptr==(lh_listnode *)NULL)
      {
        if (header_pointer->FIRST_NODE==(lh_listnode *)NULL)
          {
            return LH_EMPTY_LIST;
          }
        header_pointer->CURRENT_NODE = header_pointer->FIRST_NODE;
        ptr = (lh_listnode *)header_pointer->CURRENT_NODE;
      }

    if (lh_cmpkey(
            key_pointer,                /*  Incoming key.           */
            (VOIDPTR)ptr->KEY_AREA,              /*  Current nodes' key.     */
            header_pointer->KEY_LENGTH, /*  Key length.             */
            header_pointer->SIGNED_KEYS,/*  Signed integer search?  */
            header_pointer->C_STRING_KEYS)/* C strings as keys?     */
          !=LH_EQUAL_KEYS)
        /*  Keys do not match.  */

        return LH_BAD_KEY;

  /*  Delete the old data area.  */

    if (header_pointer->DATA_LENGTH == 0)
      datalen = ptr->DATA_LENGTH;
    else
      datalen = header_pointer->DATA_LENGTH;

    if (lhz_free(header_pointer->SYSTEM_LIST,(VOIDPTR)ptr->DATA_AREA)==FAIL)
        return LH_FREE_ERROR;

  /*  Allocate the new area and link it on.  */

    ptr->DATA_LENGTH = datalen;
    
    if ((ptr->DATA_AREA=
	    (lh_listnode *)lhz_allocate(header_pointer->SYSTEM_LIST,datalen))
             ==(lh_listnode *)NULL)   return LH_NO_SPACE;

    hoblockmove (
              data_pointer,  /*  Copy data from here      */
              (VOIDPTR)ptr->DATA_AREA,/*  to here, and             */
              datalen);      /*  this number of bytes.    */

    return SUCCESS;
}
