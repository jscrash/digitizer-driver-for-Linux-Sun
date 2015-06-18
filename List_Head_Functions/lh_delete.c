/* DEC/CMS REPLACEMENT HISTORY, Element LH_DELETE.C*/
/* *1    19-JUN-1989 13:05:45 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element LH_DELETE.C*/

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
INT lh_delete(header_pointer, key_pointer, current_flag)
/************************* lh_delete *********************************
-FUNCTION:  This routine deletes the first located element having 
            the given key value.

-PARAMETERS:                                                      */

  lh_listhead	*header_pointer;/*  Pointer to the list.            */
  VOIDPTR	key_pointer;   /*  Pointer to the key.             */
  INT          current_flag;  /*  Set to one of:
                                    0 -- delete the given item and
                                         do not make any node current;
                                    1 -- delete the given item and 
                                         make the predecessor node
                                         current (if present.  If not,
                                         leave the current node 
                                         undefined).
                                    2 -- as for 1, above, but makes the
                                         next node current.       */
                                                                  /*
-DESCRIPTION:

  This routine deletes the indicated list element.  If the list can
  contain duplicated keys, then the potential exists for just deleting
  one of a set of nodes having the given key value.  To be certain of
  getting all nodes with the given key, repeat the delete operation
  until being notified that no further nodes with that key exist.
  
  The implementation will be simplistic:  
    - use lh_find to locate if there is such a node,
    - if not -- return with an error.
    - else, delete the node referenced by header_pointer->CURRENT_NODE.

-STATUS:

  The routine will return one of the following values:

    SUCCESS
    LH_NULL_HEADER_POINTER
    LH_NULL_KEY_POINTER
    LH_EMPTY_LIST
    LH_FREE_ERROR
    LH_BAD_FLAG

-REVISIONS:

  May 19,1987 by Anthony P. Lucido -- Initial version.

************************** lh_delete ********************************/
{
/*                    Local Declarations                       */
    INT   datalen,
          status;
	BOOL  sys_list;
  
    lh_listnode *ptr,*pred,*succ;
/*
    Check the parameters.
*/
    if (header_pointer==(lh_listhead *)NULL)
        return LH_NULL_HEADER_POINTER;

    if (key_pointer==(VOIDPTR)0)
        return LH_NULL_KEY_POINTER;

    if ((current_flag<0) || (current_flag>2))
        return LH_BAD_FLAG;

    sys_list = header_pointer->SYSTEM_LIST;


/*
    Try to find the node.
*/

  status = lh_find (header_pointer, key_pointer, NULL, &datalen);
    if (status!=0)
        /*  No such node.  */
          return LH_EMPTY_LIST;

/*
    Delete the data and key areas, and then the node, itself.
*/
    ptr     = header_pointer->CURRENT_NODE;/*  Just for the ease of typing.*/

    if ((lhz_free(sys_list,(VOIDPTR)ptr->DATA_AREA)==FAIL) ||
        (lhz_free(sys_list,(VOIDPTR)ptr->KEY_AREA) ==FAIL)) /*  Try to free the node 
                                              data and key areas.  */
        return LH_FREE_ERROR;

/*
    Link around the named node.  Watch out for the end points.
*/
  pred = (lh_listnode *)(ptr->LAST_NODE);
  succ = (lh_listnode *)(ptr->NEXT_NODE);
  if ((ptr->LAST_NODE==NULL_LH_NODE) &&
      (ptr->NEXT_NODE==NULL_LH_NODE))  /*  Only child case.  */
        {
          header_pointer->FIRST_NODE   = NULL_LH_NODE;
          header_pointer->LAST_NODE    = NULL_LH_NODE;
          header_pointer->CURRENT_NODE = NULL_LH_NODE;
        }
  else if (ptr->LAST_NODE==NULL_LH_NODE)  /*  First node.    */
        {
          header_pointer->FIRST_NODE  = ptr->NEXT_NODE;
          succ->LAST_NODE             = NULL_LH_NODE;
          header_pointer->CURRENT_NODE = (current_flag!=2) ?
                                          NULL_LH_NODE : /*  For either make the 
                                                   predecessor node 
                                                   current, or make none
                                                   current */
                                          ptr->NEXT_NODE;
        }
  else if (ptr->NEXT_NODE==NULL_LH_NODE)  /*  Last node.     */
        {
          header_pointer->LAST_NODE   = ptr->LAST_NODE;
          pred->NEXT_NODE             = NULL_LH_NODE;
          header_pointer->CURRENT_NODE = (current_flag!=1) ?
                                          NULL_LH_NODE : /*  For either make the 
                                                   succecessor node 
                                                   current, or make none
                                                   current */
                                          ptr->LAST_NODE;
        }
  else  /*  General case.                            */
        {
          pred->NEXT_NODE              = succ;
          succ->LAST_NODE              = pred;
          header_pointer->CURRENT_NODE = (current_flag==0) ?
                                          NULL_LH_NODE : /*  For make none current */
                                          (current_flag==1) ?
                                            ptr->LAST_NODE: 
                                            ptr->NEXT_NODE;
        }
/*
    Decrement the count of active nodes.
*/
    (header_pointer->NUMBER_OF_NODES)--;

/*  
    Now, free the node area, itself.
*/
    if (lhz_free(sys_list,(VOIDPTR)ptr)==FAIL)
        return LH_FREE_ERROR;

    return SUCCESS;
}
