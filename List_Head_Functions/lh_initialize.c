/* DEC/CMS REPLACEMENT HISTORY, Element LH_INITIALIZE.C*/
/* *1    19-JUN-1989 13:06:06 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element LH_INITIALIZE.C*/

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
INT lh_initialize (header_pointer, list_type_code, 
                    key_length, signed_keys_flag, duplicate_keys_flag,
                    c_string_keys_flag,system_list_flag,data_length)
/************************* lh_initialize *********************************
-FUNCTION:  This routine creates and initializes the header for a doubly 
            linked list.

-PARAMETERS:                                                             */

  lh_listhead   **header_pointer;/*  Receives the list header pointer
                                 value.  
                             */
  INT        list_type_code, /*  Gives what type of list is being created.
                                 The value is one of:
                                   0 -- arbitrary list type,
                                  >0 -- a registered list of a particular
                                        type.
                             */
             key_length;     /*  Gives the key length.  The value must be
                                 greater than 0.
                             */
   BOOL      signed_keys_flag,/*  This area is set to 1 if the key should
                                 be taken as an optionally signed integer
                                 key area.  Any other value indicates 
                                 that a lexical ordering is to be used.
                             */
             duplicate_keys_flag,/*  This flag is set to 1 if there
                                 can be duplicated keys.  Any other
                                 value indicates that the keys are to be
                                 unique.
                             */
             c_string_keys_flag;/*  This flag is set to 1 if the keys are
                                  to be interpreted as being NULL terminated
                                  C strings.
                              */
   BOOL      system_list_flag;/*  This flag is set to one if this is a
                                 system, versus an application list.  Any
                                 value other than 1 is interpreted as this
                                 being an application list.            */
   INT       data_length;    /*  This gives the length of the data area.
                                 If the area has the value 0, then each
                                 node has a length value associated with
                                 it.  Else, if the value is greater than
                                 zero, then this is the length of all
                                 node data areas.
                             */
                                                                         /*
-DESCRIPTION:

  This routine allocates an instance of the following structure, initializes 
  that structure, and returns the instances' pointer value.  The data 
  structure to be allocated is:

  typedef 
    struct
      {
        CHAR *FIRST_NODE,
             *LAST_NODE ,
             *KEY_FIELD, 
             *DATA_FIELD,
             *CURRENT_NODE;
        INT   NUMBER_OF_NODES;
        INT   KEY_LENGTH, 
              DATA_LENGTH;
        BOOL  SIGNED_KEYS,
              DUPLICATE_KEYS,
              C_STRING_KEYS,
              SYSTEM_LIST;
      }
        lh_listhead;

-STATUS:

  The value of the function is one of:

      SUCCESS
      LH_BAD_LENGTH
      LH_NO_SPACE
      LH_BAD_FLAG

-REVISIONS:

  May 18, 1987 by Anthony P. Lucido -- Initial version.

************************** lh_initialize ********************************/
{
/*                    Local Declarations                    */

  lh_listhead *ptr;/*  Temporary pointer.                   */

/*
    Check the incoming parameters.
*/
    if ((key_length<1) || (data_length<0) || 
        ((signed_keys_flag==1) && (key_length!=sizeof(INT))))
              /*  Leave with an error condition.  */
            return(LH_BAD_LENGTH);
    if ((c_string_keys_flag==1) && (signed_keys_flag==1))
           return (LH_BAD_FLAG);
/*
    Parameters are OK, so allocate an area for the header, and enter
    the characterizing information.
*/

    if ((ptr=(lh_listhead *)lhz_allocate(system_list_flag,/*  System or 
                                                      application area.*/
                                         sizeof(lh_listhead)))/*  Size */
                ==(lh_listhead *)NULL)
      return(LH_NO_SPACE);

    ptr->SECURITY = lh_secure_value;/*  Enter in the code that indicates
                                        this is a valid list header.    */

    ptr->FIRST_NODE = (lh_listnode *)NULL; /*  Set the first and last node   */
    ptr->LAST_NODE  = (lh_listnode *)NULL; /*  pointers to NULL (no nodes).  */

    ptr->KEY_LENGTH  = key_length;  /*  Set the key and data length     */
    ptr->DATA_LENGTH = data_length; /*  areas.                          */

    ptr->CURRENT_NODE    = (lh_listnode *)NULL;  /*  As this is an empty list, 
						there is no current node.     */

    ptr->NUMBER_OF_NODES = 0;       /*  Set the node count to zero, 
                                        indicating this to be an empty
                                        list.                           */

    ptr->SIGNED_KEYS     = (signed_keys_flag==1)?1:0;/*  Set the        */
    ptr->DUPLICATE_KEYS  = (duplicate_keys_flag==1)?1:0;/*  four        */
    ptr->C_STRING_KEYS   = (c_string_keys_flag ==1)?1:0;
    ptr->SYSTEM_LIST     = (system_list_flag==1)   ?1:0;/*  flags.      */

/*    Return the header pointer value, and a normal return code value.  */

    *header_pointer = ptr;
    return(SUCCESS);
}
