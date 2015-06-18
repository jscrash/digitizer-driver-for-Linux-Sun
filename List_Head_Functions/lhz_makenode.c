/* DEC/CMS REPLACEMENT HISTORY, Element LHZ_MAKENODE.C*/
/* *1    19-JUN-1989 13:05:26 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element LHZ_MAKENODE.C*/

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
#include "esi_tc.h"

lh_listnode *lhz_makenode(system_list_flag, key_pointer, key_length, 
                         data_pointer, data_length)
/************************* lhz_makenode *********************************
-FUNCTION:  This routine allocates a list node element, and space for
            both the key field and the data field, and returns the
            conjoin of these.

-PARAMETERS:                                                      */
  
  BOOL   system_list_flag;/*  Set to 1 if this is a system (versus
                         an application) list.                    */
  VOIDPTR key_pointer;	/*  Pointer to the key field.                */
  size_t  key_length;	/*  Length of the key field (bytes).         */
  VOIDPTR data_pointer;	/*  Pointer to the data field.               */
  size_t  data_length;	/*  Length of the data field (bytes).        */
                                                                  /*
-DESCRIPTION:

  This routine creates the data areas for the list node, the key field, and
  the data field, sets up the pointer and length fields, as needed, and 
  returns the node pointer.

-STATUS:

    returns the value NULL if any of the allocations failed, or a 
    bad parameter is passed.

-REVISIONS:

  May 21,1987 by Anthony P. Lucido -- Initial version.

************************** lhz_makenode ********************************/
{
/*                    Local Declarations                       */
    lh_listnode *node_ptr;
    VOIDPTR	key_ptr, data_ptr;

/*
    Check the parameters.
*/
    if ((key_pointer==(VOIDPTR)0) || (data_pointer==(VOIDPTR)0) ||
        (key_length<1)      || (data_length<1))
      return ((lh_listnode *)NULL);

/*
    The parameters are OK, so allocate the areas, and continue.
*/
    if ( ((key_ptr=lhz_allocate(system_list_flag,key_length))==(VOIDPTR)0)   ||
         ((data_ptr=lhz_allocate(system_list_flag,data_length))==(VOIDPTR)0) ||
         ((node_ptr=(lh_listnode *)lhz_allocate(system_list_flag,
                    sizeof(lh_listnode)))==(lh_listnode *)0))

      {/*  At least one allocation did not succeed.  */

        if (key_ptr!=(VOIDPTR)0)  lhz_free(system_list_flag,key_ptr);
        if (data_ptr!=(VOIDPTR)0) lhz_free(system_list_flag,data_ptr);
        if (node_ptr!=(lh_listnode *)0) tc_free ((VOIDPTR)node_ptr);

        return((lh_listnode *)0);
      }

/*  Stuff in the data ...    */
    node_ptr->KEY_AREA  = (lh_listnode *)key_ptr;
	node_ptr->DATA_AREA = (lh_listnode *)data_ptr;

    hoblockmove (key_pointer,  /*  Move the key data from here  */
                 key_ptr,      /*  to here.  The count is       */
                 key_length)   /*  given by this parameter.     */;

    hoblockmove (data_pointer, /*  Move the data from here      */
                 data_ptr,     /*  to here.  The count is       */
                 data_length)  /*  given by this parameter.     */;

    node_ptr->DATA_LENGTH = data_length;
    
    return (node_ptr);
}
