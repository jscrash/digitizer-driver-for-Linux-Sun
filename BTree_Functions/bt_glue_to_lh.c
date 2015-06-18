/* DEC/CMS REPLACEMENT HISTORY, Element BT_GLUE_TO_LH.C*/
/* *1    19-JUN-1989 12:01:02 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element BT_GLUE_TO_LH.C*/

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

/*
 *  File : BT_GLUE_TO_LH.C
 *
 *  Routines :
 *              btinit()
 *
 *    Description : This set of routines is a mapping or glueing from
 *        existing calls to BT routines to replacement LH (list handling)
 *        routines.  These functions are intended to act exactly like
 *        the BT routines to prevent the calling routines from having to
 *        be reprogramed for the new structures.
 */

/* include files */
#ifndef ESI_GL_DEFS_H
#include "esi_gl_defs.h"
#endif

#ifndef ESI_BT_H
#include "esi_bt.h"
#endif

#ifndef ESI_LH_H
#include "esi_lh.h"
#endif

/*************************************************************

    Routine : btinit

    Description : call the routine lh_initialize, after interpreting
     the passed in flags.

*************************************************************/
publicdef INT btinit( header_pointer, tree_type, key_length, data_length,
		    flags )

TREEHEAD *header_pointer;
INT      tree_type;
size_t   key_length;
size_t   data_length;
INT      flags;

{
   INT status;
   INT list_type_code;
   BOOL signed_keys_flag;
   BOOL duplicate_keys_flag;
   BOOL c_string_keys;
   BOOL system_list_flag;

   list_type_code = 0;         /* not registered     */
   signed_keys_flag = (flags & BT_KEY_HAS_SIGN)>0?1:0;/* signed keys? */
   duplicate_keys_flag = (flags & BT_DUPLICATE_KEYS_ALLOWED)?1:0;
					  /* duplicate keys? */
   c_string_keys    = (flags & BT_KEY_IS_C_STRING)? 1:0;
					  /*  Are the keys to be
					    taken as null terminated C
					    strings?*/
   system_list_flag = (flags & BT_TREE_SYSTEM)?1:0;/* use system memory?  */



   status = lh_initialize( (lh_listhead **)header_pointer, list_type_code,
		   key_length, signed_keys_flag, duplicate_keys_flag,
		   c_string_keys,system_list_flag, data_length );

   return( status );
}



/************************************************************

   Function : BTADD

************************************************************/

publicdef INT btadd( header_pointer, key_pointer, data_pointer )

TREEHEAD *header_pointer;
VOIDPTR  key_pointer;
VOIDPTR  data_pointer;

{
   INT status;

   status = lh_add( *(lh_listhead **)header_pointer,key_pointer, data_pointer,
	0);

   return( status );
}

/************************************************************

   Function : BTCOUNT

************************************************************/

publicdef INT btcount( header_pointer )

TREEHEAD *header_pointer;

{
   INT status;

   status = lh_count( *(lh_listhead **)header_pointer );

   return( status );
}

/************************************************************

   Function : BTCOPY

************************************************************/

publicdef INT btcopy( src_header_pointer, dest_header_pointer )

TREEHEAD *src_header_pointer;
TREEHEAD *dest_header_pointer;

{
   INT status;

   status = lh_copy( (lh_listhead *)*(src_header_pointer),
   	(lh_listhead *)*(dest_header_pointer) );

   return( status );
}

/************************************************************

   Function : BTCURR

************************************************************/

publicdef INT btcurr( header_pointer, key_pointer, data_pointer )

TREEHEAD *header_pointer;
VOIDPTR  key_pointer;
VOIDPTR  data_pointer;

{
   INT status;

   status = lh_current( *(lh_listhead **)header_pointer, key_pointer, data_pointer,
	(INT *)0);

   return( status );
}

/************************************************************

   Function : BTDEL

************************************************************/

publicdef INT btdel( header_pointer, key_pointer )

TREEHEAD *header_pointer;
VOIDPTR  key_pointer;

   {
   INT status;

/* LH_DELETE expects an INT flag as the last argument, so pass it an */
/* (INT) 0, instead of an (INT *) 0.  This problem caused a "stuck"  */
/* loop in AM_TERMINATE, since nodes weren't being deleted properly. */
   status =
      (lh_delete (*(lh_listhead **) header_pointer, key_pointer, (INT) 0)
	 == SUCCESS);

   return status;
   }

/************************************************************

   Function : BTFIND

************************************************************/

publicdef INT btfind( header_pointer, key_pointer, data_pointer )

TREEHEAD *header_pointer;
VOIDPTR  key_pointer;
VOIDPTR  data_pointer;

{
   INT status;

   status = (lh_find( *(lh_listhead **)header_pointer,key_pointer,
	data_pointer,(INT *)0) == SUCCESS);

   return( status );
}

/************************************************************

   Function : BTFIRST

************************************************************/

publicdef INT btfirst( header_pointer, key_pointer, data_pointer )

TREEHEAD *header_pointer;
VOIDPTR  key_pointer;
VOIDPTR  data_pointer;

{
   INT status;

   status = lh_first( *(lh_listhead **)header_pointer,key_pointer,
	data_pointer,(INT *)0);

   return( status );
}


/************************************************************

   Function : BTFREE

************************************************************/

publicdef INT btfree( header_pointer )

TREEHEAD *header_pointer;

{
   INT status;

   status = lh_free( *(lh_listhead **)header_pointer );

   return( status );
}


/************************************************************

   Function : BTLAST

************************************************************/

publicdef INT btlast( header_pointer, key_pointer, data_pointer )

TREEHEAD *header_pointer;
VOIDPTR  key_pointer;
VOIDPTR  data_pointer;

{
   INT status;

   status = lh_last( *(lh_listhead **)header_pointer,key_pointer,
	data_pointer,(INT *)0);

   return( status );
}

/************************************************************

   Function : BTNEXT

************************************************************/

publicdef INT btnext( header_pointer, key_pointer, data_pointer )

TREEHEAD *header_pointer;
VOIDPTR  key_pointer;
VOIDPTR  data_pointer;

{
   INT status;

   status = (lh_next( *(lh_listhead **)header_pointer,key_pointer,
	data_pointer,(INT *)0) == SUCCESS);

   return( status );
}

/************************************************************

   Function : BTPREV

************************************************************/

publicdef INT btprev( header_pointer, key_pointer, data_pointer )

TREEHEAD *header_pointer;
VOIDPTR  key_pointer;
VOIDPTR  data_pointer;

{
   INT status;

   status = (lh_previous( *(lh_listhead **)header_pointer,
		       key_pointer,data_pointer,(INT *)0)==SUCCESS);

   return( status );
}

/************************************************************

   Function : BTREPLACE

************************************************************/

publicdef INT btrepcur( header_pointer, key_pointer, data_pointer )

TREEHEAD *header_pointer;
VOIDPTR  key_pointer;
VOIDPTR  data_pointer;

{
   INT status;

   status = lh_replace( *(lh_listhead **)header_pointer,key_pointer,
		data_pointer,0);

   return( status );
}

                                                                                         
