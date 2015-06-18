/* DEC/CMS REPLACEMENT HISTORY, Element LH_ADD.C*/
/* *1    19-JUN-1989 13:05:28 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element LH_ADD.C*/

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

publicdef INT lh_add(header_pointer, key_pointer, data_pointer, data_length)
/************************* lh_add *********************************
-FUNCTION:  This routine adds the given key and data to the indicated 
            list.  If the key duplicates what is already present, and
            this is not permitted, then an error condition is reported,
            and no entry is made.

-PARAMETERS:                                                      */

      lh_listhead *header_pointer;/*  Pointer to the list to be
                                      modified.
                                  */
      VOIDPTR	    key_pointer,   /*  Points to the new entries' 
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
      size_t	    data_length;   /*  This gives the length of the 
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
-DESCRIPTION:

  This routine adds the given node to the named list.  There is a current node
  value associated with each list.  If the current node value is not NULL_LH_NODE, 
  then it gives the starting poINT for determining where to add in the new
  node.  Else, the list head is taken as that starting point.  

  Nodes are added so that the keys are kept in ascending sorting order.  If 
  the keys are taken as signed integers, then this is a numerical sort.  Else,
  this is an ascending order lexical sort.

  The data and key values provided to this routine are copied, in value, to
  dynamically created areas.  

  Once added, this new node becomes the current node.

-STATUS:

  This routine will return one of the following values:
    SUCCESS
    LH_NULL_KEY_POINTER
    LH_BAD_LENGTH
    LH_DUPLICATE_KEY
    LH_NULL_HEADER_POINTER
    LH_BAD_LIST_HEADER
    LH_NO_SPACE
    LH_BAD_KEY

-REVISIONS:

  May 19, 1987 by Anthony P. Lucido -- Initial version.

************************** lh_add ********************************/
{
/*                    Local Declarations                       */

        lh_listnode *node, *ptr, *lhz_makenode();
        INT          keylen, datalen;
        BOOL         signed_keys, dup_keys,c_string_keys;
        BOOL          sys_list;

/*
    Check the incoming parameters.
*/
    if (key_pointer==(VOIDPTR)0) /*  Invalid key pointer?  */
        return(LH_NULL_KEY_POINTER);

    if (header_pointer==NULL_LH_LIST) /*  Invalid list header?  */
        return(LH_NULL_HEADER_POINTER);

     if (header_pointer->SECURITY!=lh_secure_value)/*  Invalid list header?*/
        return  LH_BAD_LIST_HEADER;

    if ((header_pointer->DATA_LENGTH==0) && (data_length<1))
        return  LH_BAD_LENGTH;

/*
    At this point, we know that the node could be added.

    First, create the node to be entered.
*/
    sys_list = header_pointer->SYSTEM_LIST;

    keylen   = header_pointer->KEY_LENGTH;
    datalen  = (header_pointer->DATA_LENGTH == 0) ?
                      data_length :
                      header_pointer->DATA_LENGTH;/*  The data length is
                                                      either the data length
                                                      passed into this code,
                                                      or the length given
                                                      in the header entry.
                                                  */

    if ((node=lhz_makenode(sys_list,key_pointer,/*  Pointer to the key.      */
                          keylen     ,/*  The length of that key.  */
                         data_pointer,/*  Pointer to the data.     */
                         datalen    ))/*  Length of that data area.*/
                == NULL_LH_NODE)
            /*  Out of space.  */
              return  LH_NO_SPACE;

/*
    Let's try to add the new node.
*/
    if ((header_pointer->FIRST_NODE==NULL_LH_NODE) ||
        (header_pointer->LAST_NODE ==NULL_LH_NODE))
      {
        /*  Empty list, so just add the new node:           */

        node->LAST_NODE  = NULL_LH_NODE;/*  Set the node front and  */
        node->NEXT_NODE  = NULL_LH_NODE;/*  back pointers to NULL_LH_NODE,
                                                  indicating a list with but
                                                  one element.            */
        header_pointer->FIRST_NODE = node;/*  Set the list to reference    */
        header_pointer->LAST_NODE  = node;/*  the new node.                */

        header_pointer->NUMBER_OF_NODES = 1;/*  Set the list length to 1. */
        header_pointer->CURRENT_NODE    = node;/*  Set in the current node.*/

        return  SUCCESS;
      }
/*
    The list is not empty, so locate where to place this node, 
                     and make the addition.
*/
    signed_keys = header_pointer->SIGNED_KEYS;    /*  If 1, then the keys
                                                      are signed long 
                                                      integers.
                                                  */
    dup_keys    = header_pointer->DUPLICATE_KEYS; /*  If 1, then duplicate
                                                      keys are permitted.
                                                  */
    c_string_keys = header_pointer->C_STRING_KEYS;/*  C strings used as 
                                                      keys?*/

    ptr = (header_pointer->CURRENT_NODE==NULL_LH_NODE) ? 
                (header_pointer->FIRST_NODE)   :
                (header_pointer->CURRENT_NODE) ;

    switch (lh_cmpkey (
                        key_pointer  ,/*  Points to the new key.      */
                        (BYTE *)ptr->KEY_AREA,/*  Points to the "current" key.*/
                        keylen       ,/*  Key length.                 */
                        signed_keys  ,/*  Set to 1, if signed keys.   */
                        c_string_keys)/*  C strings as keys?          */)
      {
        case LH_FIRST_KEY_LESS:
                   /*  The new key is less than the current key.      */  
                goto Descending_search;/*  We want to find a node that is
                                          greater than or equal to the
                                          new node.                   */

        case LH_EQUAL_KEYS:
                  /*  The keys are equal.  
                          Question:  Are duplicate keys permitted.    */
                if (dup_keys!=1) 
                  {  /*  Can't have two nodes with the same key value.*/
                    lhz_free(sys_list,(BYTE *)node->KEY_AREA); /*  Delete the data */
                    lhz_free(sys_list,(BYTE *)node->DATA_AREA);/*  areas allocated for*/
                    lhz_free(sys_list,(BYTE *)node);           /*  This node.    */

                    return  LH_DUPLICATE_KEY;
                  }
              /*  Enter the node.  */
                  goto Enter_node;

        case LH_FIRST_KEY_GREATER:    
                 /*  The new node is greater than the current node.*/
                goto Ascending_search;/*  We want to find a node that is
                                          less than or equal to the 
                                          new node.                  */

        default:/*  Something is wrong, so issue an error and leave. */
                return  LH_BAD_KEY;
      }

  Ascending_search:

                while (1)
                  {/*  Loop until the comparison:
                        - exhausts the list (return with code 6)
                        - finds an element that has an equal key (if
                            duplicate keys are allowed, then add in the
                            new node.  Else, return with code 3)
                        - finds an element such that the incoming key
                            is less than the key of the current node.
                            Here, add the new node before the current node.
                    */
                      ptr = ptr->NEXT_NODE; /*  Go to next node. */
                      if (ptr==NULL_LH_NODE) /*  Ran out of list.        */            
                        {/*  Need to enter the new node onto the list end.*/

                            node->NEXT_NODE = NULL_LH_NODE; /*  No successor node.*/

                            node->LAST_NODE = header_pointer->LAST_NODE;
                                              /*  Back link the new end node.
                                              */

                  ((lh_listnode *)(header_pointer->LAST_NODE))->NEXT_NODE 
                                                         = node;
                                              /*  Successor to what was the
                                                  last node is the new
                                                  last node.   */

                            header_pointer->LAST_NODE    = node;/*  Link on
                                                                    the new 
                                                                    end 
                                                                    node.*/
                            header_pointer->CURRENT_NODE = node;/*  Make it 
                                                                    the 
                                                                    current
                                                                    node. */
                            (header_pointer->NUMBER_OF_NODES)++;/*  Increment
                                                                    the nodes 
                                                                    count.*/
                            return  SUCCESS;
                        }

                    switch (lh_cmpkey (
                        key_pointer  ,/*  Points to the new key.      */
                        (BYTE *)ptr->KEY_AREA,/*  Points to the "current" key.*/
                        keylen       ,/*  Key length.                 */
                        signed_keys  ,/*  Set to 1, if signed keys.   */
                        c_string_keys))/*  C strings used as keys?    */
                     {
                        case LH_FIRST_KEY_LESS:
                                  /*  The new key is less than the current. 
                                   Here is where the new node is entered. */
                                    goto Enter_node;
                      
                        case LH_EQUAL_KEYS:
                                  /*  Equal keys.                           */
                                if (dup_keys!=1)
                         {
                          lhz_free(sys_list,(BYTE *)node->KEY_AREA); /*  Delete the data     */
                          lhz_free(sys_list,(BYTE *)node->DATA_AREA);/*  areas allocated for */
                          lhz_free(sys_list,(BYTE *)node);           /*  This node.          */

                          return  LH_DUPLICATE_KEY;  
                         }

                                goto Enter_node;/*  If duplicates are
                                                    allowed ...          */

                        case LH_FIRST_KEY_GREATER:
                                  /*  The incoming key is still greater than
                                    the key of this node.  Continue the
                                    search.                              */
                                break;

                        default:/*  Error condition.  */
                                return  LH_BAD_KEY;

                      }
                    }



  Descending_search:
    
        while(1)
          {
            /*
              The search continues until we encounter one of the following
              conditions:
                - error condition in the compare:  return with error code 6.
                - the new and the current node keys are equal:
                    if duplicates are permitted, enter the new node before
                        the current node.  Else, return with error code 3.
                - the new node is greater than the current node.  The new node
                    is entered after the current nodes' successor.
                - the front of the list is encountered.  The new node becomes 
                    the lists' first node.
            */


            ptr = ptr->LAST_NODE;/*  Go back one node.                */
            if (ptr == NULL_LH_NODE)  /*  We've run out of list, so enter the new
                                  node at the head of the list.       */
              {
                node->NEXT_NODE = header_pointer->FIRST_NODE;/*  Successor of
                                                          the new node is the
                                                          old first node.  */

                node->LAST_NODE = NULL_LH_NODE;/*  No predecessor node.*/

                ((lh_listnode *)(header_pointer->FIRST_NODE))->LAST_NODE 
                                              = node;/*  The 
                                                          predecessor of the
                                                          old first node is 
                                                          the new first 
                                                          node.*/
                header_pointer->FIRST_NODE   = node;/*  Set in the first and*/
                header_pointer->CURRENT_NODE = node;/*  current node values.*/

                (header_pointer->NUMBER_OF_NODES)++;/*  Increment the node
                                                        counter.            */
                return  SUCCESS;/*  And return with a normal completion 
                                                  code.*/
              }

              
              switch (lh_cmpkey (
                        key_pointer  ,/*  Points to the new key.      */
                        (BYTE *)ptr->KEY_AREA,/*  Points to the "current" key.*/
                        keylen       ,/*  Key length.                 */
                        signed_keys  ,/*  Set to 1, if signed keys.   */
                        c_string_keys))/*  C strings used as keys?    */
                {
                  case LH_FIRST_KEY_LESS:
                         /*  New node is less than the current one.  */
                          break;/*  Continue the search.             */

                  case LH_EQUAL_KEYS:
                         /*  The new and current nodes are equal.    */
                                if (dup_keys!=1)
                   {
                    lhz_free(sys_list,(BYTE *)node->KEY_AREA); /*  Delete the data     */
                    lhz_free(sys_list,(BYTE *)node->DATA_AREA);/*  areas allocated for */
                    lhz_free(sys_list,(BYTE *)node);           /*  This node.          */

                    return  LH_DUPLICATE_KEY;  
                   }

                                goto Enter_node;/*  If duplicates are
                                                    allowed ...          */

                  case LH_FIRST_KEY_GREATER:
                        /*  The new node is greater than the current one.*/
                          ptr = ptr->NEXT_NODE;/*  The new node is to be 
                                                   entered before the current
                                                   nodes' successor.      */
                          goto Enter_node;

                  default:/*  Something goofed.  Return with an error 
                                  condition.*/  
                          return  LH_BAD_KEY;
                }
          }

  Enter_node:  /*  Enter the new node in BEFORE that referenced by ptr.  */
              node->LAST_NODE              = ptr->LAST_NODE;/*  New node 
                                                                references the
                                                                predecessor of
                                                                the "ptr" 
                                                                node.     */
              node->NEXT_NODE              = ptr;/*  The successor to the new
                                                     node is that referenced
                                                     by "ptr"             */
              if (node->LAST_NODE!=NULL_LH_NODE)
                      (node->LAST_NODE)->NEXT_NODE = node;
                                                 /*  The successor to "ptr"'s  
                                                      predecessor is now the
                                                      new node.           */
              ptr->LAST_NODE               = node;/*  The predecessor of the
                                                     "ptr" node is the new
                                                     node.                */

              (header_pointer->NUMBER_OF_NODES)++;/*  Increment the number of
                                                      nodes value.        */

              header_pointer->CURRENT_NODE = node;/*  Set in the current node.*/

              if (node->LAST_NODE==NULL_LH_NODE) 
                header_pointer->FIRST_NODE = node;
              
              if (node->NEXT_NODE==NULL_LH_NODE) 
                header_pointer->LAST_NODE = node;
              
  return  SUCCESS;  /*  Report success.  */
}
