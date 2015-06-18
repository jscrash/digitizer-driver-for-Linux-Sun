/* DEC/CMS REPLACEMENT HISTORY, Element LH_NEXT.C*/
/* *1    19-JUN-1989 13:06:12 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element LH_NEXT.C*/

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

INT lh_next(header_pointer, key_pointer, data_pointer, data_length) 
/************************* lh_next *********************************
-FUNCTION:  This routine returns the key and data of the current list
            nodes' successor, and makes that node current.

-PARAMETERS:                                                      */

    lh_listhead *header_pointer;/*  Pointer to the list.          */
    VOIDPTR	key_pointer,	/*  Receives a copy of the key.   */
                data_pointer;	/*  Receives a copy of the data.  */
    INT         *data_length;   /*  Receives the data length.     */
                                                                  /*
-DESCRIPTION:

  This routine returns the key and data of the current nodes successor.
  If the current node field is NULL, then the first list element is made 
  current, and that node is returned.  If the current node is the last list
  node, then an error code is returned.

-STATUS:

  The return code value is set to one of:

    SUCCESS
    LH_NULL_HEADER_POINTER
    LH_NO_SPACE
    LH_EMPTY_LIST
    LH_AT_END
    LH_BAD_LIST_HEADER

-REVISIONS:

  May 26,1987 by Anthony P. Lucido -- Initial version.

************************** lh_next ********************************/
{
/*                    Local Declarations                       */

  lh_listnode *ptr;/*  General work area                       */
  INT          length;/*  Holds the length values, as needed.  */


/*
    Check the parameters.
*/
    if (header_pointer==(lh_listhead *)NULL)/*  Null list ? */
        return LH_NULL_HEADER_POINTER;
  
    if (header_pointer->SECURITY!=lh_secure_value)
        return LH_BAD_LIST_HEADER;

/*
    Copy the data areas.
*/
    ptr = (lh_listnode *)(header_pointer->CURRENT_NODE);
    if (ptr==(lh_listnode *)NULL)
      {
        if (header_pointer->FIRST_NODE==(lh_listnode *)NULL)
            return LH_EMPTY_LIST;

        ptr = (lh_listnode *)(header_pointer->FIRST_NODE);
      }
    else ptr = ptr->NEXT_NODE;

    if (ptr==(lh_listnode *)NULL)
        return LH_AT_END;


    length = (header_pointer->DATA_LENGTH==0) ?
              ptr->DATA_LENGTH                :
              header_pointer->DATA_LENGTH     ;

    if (data_length!=(INT *)NULL) *data_length = length;

     header_pointer->CURRENT_NODE = ptr;/*  Set the current node value. */

    if (data_pointer!=(VOIDPTR)0)
      {
        if (data_length!=(INT *)NULL) *data_length = 0;
       hoblockmove (
                 (VOIDPTR)ptr->DATA_AREA,/*  Move the data from here      */
                 data_pointer,  /*  to here.  The count is       */
                 length      ); /*  given by this parameter.     */
      }

    if (key_pointer!=(VOIDPTR)0)
       hoblockmove (
                 (VOIDPTR)ptr->KEY_AREA, /*  Move the data from here      */
                 key_pointer,   /*  to here.  The count is       */
                 header_pointer->KEY_LENGTH);
                                /*  given by this parameter.     */

    return SUCCESS;
}


