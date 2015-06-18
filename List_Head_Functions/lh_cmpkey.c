/* DEC/CMS REPLACEMENT HISTORY, Element LH_CMPKEY.C*/
/* *1    19-JUN-1989 13:05:34 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element LH_CMPKEY.C*/

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

#include "esi_c_lib.h"

#ifndef ESI_LH_H
#include "esi_lh.h"
#endif

publicdef INT lh_cmpkey(node_key, test_key, key_length, signed_keys,
                        c_string_keys)
/************************* lh_cmpkey *********************************
-FUNCTION:  This routine compares the node key and the test key,
            and returns a value in the range of 0-3.

-PARAMETERS:                                                      */

  VOIDPTR   node_key,   /*  This is the key value in the node.        */
	    test_key;   /*  This is the "new" key.                    */
  size_t    key_length; /*  Must be a positive value.                 */
  BOOL  signed_keys;/*  Set to 1 if the test is with signed 
                        integer keys.                             */
  BOOL  c_string_keys;/*  Flag to indicate if the keys are to be
                        taken as null terminated C strings.       */
                                                                  /*
-DESCRIPTION:

  This routine compares the two key fields, and determines their 
  relationship.  The return values are:

    LH_INVALID_NODE
    FIRST_KEY_LESS
    EQUAL_KEYS
    FIRST_KEY_GREATER

-STATUS:

    See above.

-REVISIONS:

  May 21,1987 by Anthony P. Lucido -- Initial version.

************************** lh_cmpkey ********************************/
{
/*                    Local Declarations                       */

    INT   ndx,   /*  Loop index variable.      */
          result;
    INT  *p1,*p2;   /*  Parameters 1 and 2 for signed compares.   */
    BYTE *p3,*p4;   /*  As above, for char. compares.             */

/*
    Test the incoming parameters.
*/
    if ((node_key==(VOIDPTR)0) || (test_key==(VOIDPTR)0) || (key_length<1) ||
        ((key_length!=sizeof(INT) && (signed_keys==1)))) 
          return (LH_INVALID_NODE);

    if (signed_keys==1)
      {
        p1 = (INT *)node_key;
        p2 = (INT *)test_key;

        return (( (*p1)<(*p2)? 
                    LH_FIRST_KEY_LESS :
                      (((*p1)==(*p2))?
                          LH_EQUAL_KEYS :
                            LH_FIRST_KEY_GREATER)));
      }

   else if (c_string_keys==1)
     {

       result = strncmp((CHAR *)node_key, (CHAR *)test_key, key_length);
       return ( 
                (result<0)?LH_FIRST_KEY_LESS:
                  (result==0)?LH_EQUAL_KEYS:
                              LH_FIRST_KEY_GREATER
              );
     }

   else
     {
      p3 = (BYTE *)node_key;
      p4 = (BYTE *)test_key;
  
      for (ndx=0; ndx<key_length; ndx++)
        if ( p3[ndx] != p4[ndx])
          return ( (p3[ndx] < p4[ndx])? 
                  LH_FIRST_KEY_LESS:LH_FIRST_KEY_GREATER);
      return (LH_EQUAL_KEYS);
     }
}

