/* DEC/CMS REPLACEMENT HISTORY, Element LL_LINKLIST.C*/
/* *4    26-AUG-1990 14:33:22 GILLESPIE "(SPR 1) Add esi_ll_err.h"*/
/* *3    26-FEB-1990 18:59:45 JULIAN "(SPR 6010) Port to Decstation 3100 (ULTRIX)"*/
/* *2    11-AUG-1989 17:41:12 JULIAN "(SPR -1) the_data may be used before set"*/
/* *1    19-JUN-1989 13:06:25 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element LL_LINKLIST.C*/

/******************************************************************************

                Copyright Finder Graphics Systems, Inc. 1990
		   Unpublished -- All rights reserved

THIS SOFTWARE IS THE PROPRIETARY PROPERTY OF Finder Graphics Systems, Inc.  AND
MAY CONTAIN CONFIDENTIAL TRADE SECRET INFORMATION. IT IS LICENSED FOR USE ON THE
DESIGNATED EQUIPMENT ON WHICH IT WAS ORIGINALLY INSTALLED AND  MAY NOT BE
MODIFIED, DUPLICATED OR COPIED IN ANY FORM WITHOUT PRIOR WRITTEN CONSENT OF

			Finder Graphics Systems, Inc.
			201 Tamal Vista Blvd
			Corte Madera, CA  USA 94925
			(415) 927-0100

(The above notice does not constitute an admission of publication or
unrestricted dissemination of the work)

******************************************************************************/

/* %F
      This is a set of routines which perform storage and retrieval on
   data in a list format.  Since this is based on the 'C' allocate
   mechanism, the only data which the user must maintain between calls
   to the various routines is a LISTHEAD structure.  All routines
   belonging to this set begin with the letters LL (for LinkList).

      The user allocates the LISTHEAD to avoid using pointers to pointers
   and overwriting of pointers.

      Storage and retrieval are done by coping data to and from the list
   area of memory and the user area of memory

   Most routines use the current item and/or set the current item.

   llappnd : set
   llcount :
   llcurr  : use
   lldela  : use and set
   lldelb  : use and set
   llfirst : set
   llgetn  : set
   llinsa  : use and set
   llinsb  : use and set
   lllast  : set
   llnext  : use and set
   llprev  : use and set
   llqcur  : use
   llreplc : use
   llcopy  :
   llfree  :
   llinit  :
   llquery :

   Author: Guy H Binns
   Extensive mods (original almost unrecognizable) by J. Gillespie  3 Feb 88
*/

/* %C  Copyright Exploration Systems, Inc. 06/27/85   */

#include "esi_c_lib.h"
#ifndef ESI_LL_INT_H
#include "esi_ll_int.h"
#endif
#ifndef ESI_TC_H
#include "esi_tc.h"
#endif
#ifndef ESI_HO_H
#include "esi_ho.h"
#endif
#ifndef ESI_LL_ERR_H
#include "esi_ll_err.h"
#endif

/* ------ Exportable routines ------ */

publicdef INT llappnd (head_in, data_in)

/* %S Append data to END of list */

/* %P
      Input Parameters:
         HEAD_IN   =   (LISTHEAD *)   pointer to list
         DATA_IN   =   (any type *)   pointer to data
*/

LISTHEAD *head_in ;
VOIDPTR data_in ;

{  
   LL_NODE  *curr_node ;

/* %? check for invalid list */
   if (head_in EQUALS NULL_LIST)
	{
	return(LL_NULL_LIST_PTR);
	}
   if (head_in->pattern IS_NOT_EQUAL_TO LL_STANDARD_TEST_PATTERN)
	{
	return(LL_INVALID_LIST_PTR);
	}

    curr_node = ((LL_NODE *) llz_alloc(head_in->flags,
	sizeof(LL_NODE) + head_in->data_size - 1));
    if (head_in->entries EQUALS 0)/* is the list empty ? */
	{
	head_in->first   = curr_node ;
	}
    else
	{
	curr_node->prev         = head_in->last ;
	head_in->last->next   = curr_node ;
	}

    curr_node->next         = NULL_NODE;
    head_in->entries++ ;
    head_in->index	    = head_in->entries;
    head_in->current	    = curr_node ;
    head_in->last	    = curr_node ;

/* copy data from user area to list area */
    hoblockmove(data_in, (VOIDPTR) curr_node->data, head_in->data_size);

    return (SUCCESS);
}

/* --------------------------------- */

publicdef INT llcount(head_in)

/* %S Return number of items in list */

/* %P
      Input Parameters:
         HEAD_IN   =   (LISTHEAD *)   pointer to list
      Output Parameters:
         LLCOUNT   =   (INT)          number of items in list
*/
LISTHEAD *head_in ;

{  
   INT return_var ;

   if (head_in EQUALS NULL_LIST)    
	{
	return ( LL_NULL_LIST_PTR);
	}
   if (head_in->pattern IS_NOT_EQUAL_TO LL_STANDARD_TEST_PATTERN)
	{
	return ( LL_INVALID_LIST_PTR);
	}

   return_var = head_in->entries ;

   return return_var;
}

/* --------------------------------- */

publicdef INT llcurr (head_in, data_in)

/* %S Retrieve data of CURRENT item */

/* %P
      Input Parameters:
         HEAD_IN   =   (LISTHEAD *)   pointer to list
      Output Parameters:
         DATA_IN   =   (any type *)   pointer to data
*/

LISTHEAD *head_in ;
VOIDPTR data_in ;

{  
/* %? check for invalid list */
   if (head_in EQUALS NULL_LIST)
	{
	return ( LL_NULL_LIST_PTR);
	}
   if (head_in->pattern IS_NOT_EQUAL_TO LL_STANDARD_TEST_PATTERN)
	{
	return ( LL_INVALID_LIST_PTR);
	}
   if (head_in->entries EQUALS 0)
	{
	return ( LL_REQUEST_ON_EMPTY_LIST);
	}

/* copy data from list area to user area */
   hoblockmove( (VOIDPTR) head_in->current->data, data_in, head_in->data_size);

   return (SUCCESS);
}

/* --------------------------------- */

publicdef INT lldela (head_in)

/* %S Delete CURRENT item; Make item AFTER, current item */

/* %P
      Input Parameters:
         HEAD_IN   =   (LISTHEAD *)   pointer to list
*/

LISTHEAD *head_in ;

{  
   LL_NODE  *curr_node ;
   
/* %? check for invalid list */
   if (head_in EQUALS NULL_LIST)
	{
	return( LL_NULL_LIST_PTR);
	}
   if (head_in->pattern IS_NOT_EQUAL_TO LL_STANDARD_TEST_PATTERN)
	{
	return (  LL_INVALID_LIST_PTR);
	}
   if (head_in->entries EQUALS 0)
	{
	return( LL_REQUEST_ON_EMPTY_LIST);
	}

   curr_node = head_in->current ;

/* handle special cases of delete */
   if(head_in->entries EQUALS 1)
   {
	 head_in->index   = 0;
         head_in->current = NULL_NODE;
         head_in->first   = NULL_NODE;
         head_in->last    = NULL_NODE;
    }
    else if (head_in->index EQUALS 1)
    {
     head_in->current  = curr_node->next ;
     head_in->first    = curr_node->next ;
     curr_node->next->prev = NULL_NODE;
    }
    else if (head_in->index EQUALS head_in->entries)
    {
     head_in->current  = curr_node->prev ;
     head_in->last     = curr_node->prev ;
     curr_node->prev->next = NULL_NODE;
    }
    else
    {
     head_in->current  = curr_node->next ;
     curr_node->prev->next = curr_node->next ;
     curr_node->next->prev = curr_node->prev ;
    }

   head_in->entries-- ;

/* deallocate memory for node */
   llz_free(head_in->flags, (VOIDPTR)curr_node) ;
   return (SUCCESS);
}

/* --------------------------------- */

publicdef INT lldelb (head_in)

/* %S Delete CURRENT item; Make item BEFORE, current item */

/* %P
      Input Parameters:
         HEAD_IN   =   (LISTHEAD *) pointer to list
*/

LISTHEAD *head_in ;

{  
   LL_NODE  *curr_node ;

/* %? check for invalid list */
   if (head_in EQUALS NULL_LIST)
	{
	return ( LL_NULL_LIST_PTR);
	}
   if (head_in->pattern IS_NOT_EQUAL_TO LL_STANDARD_TEST_PATTERN)
	{
	return ( LL_INVALID_LIST_PTR);
	}
   if (head_in->entries EQUALS 0)
	{
	return ( LL_REQUEST_ON_EMPTY_LIST);
	}

   curr_node = head_in->current ;

/* handle special cases of delete */
    if (head_in->entries EQUALS 1)
    {
     head_in->index = 0;
     head_in->current = NULL_NODE;
     head_in->first   = NULL_NODE;
     head_in->last    = NULL_NODE;
    }
    else if (head_in->index EQUALS 1)
    {      
     head_in->current  = curr_node->next ;
     head_in->first    = curr_node->next ;
     curr_node->next->prev = NULL_NODE;
    }
    else if (head_in->entries EQUALS head_in->index)
    {
     head_in->index--;
     head_in->current  = curr_node->prev ;
     head_in->last     = curr_node->prev ;
     curr_node->prev->next = NULL_NODE;
    }
    else
    {
     head_in->index--;
     head_in->current  = curr_node->prev ;
     curr_node->prev->next = curr_node->next ;
     curr_node->next->prev = curr_node->prev ;
     }

    head_in->entries--;
    llz_free(head_in->flags, (VOIDPTR)curr_node) ;
    return (SUCCESS);
}

/* --------------------------------- */

publicdef INT llfirst (head_in, data_in)

/* %S Retrieve data of FIRST item */

/* %P
      Input Parameters:
         HEAD_IN   =   (LISTHEAD *)   pointer to list
      Output Parameters:
         DATA_IN   =   (any type *)   pointer to data
*/

LISTHEAD *head_in ;
VOIDPTR  data_in ;

{  

/* %? check for invalid list */
   if (head_in EQUALS NULL_LIST)
	{
	return (  LL_NULL_LIST_PTR);
	}
   if (head_in->pattern IS_NOT_EQUAL_TO LL_STANDARD_TEST_PATTERN)
	{
	return (  LL_INVALID_LIST_PTR);
	}
   if (head_in->entries EQUALS 0)
	{
	return (  LL_REQUEST_ON_EMPTY_LIST);
	}

   head_in->current = head_in->first ;
   head_in->index = 1;

/* copy data from list area to user area */
   hoblockmove((VOIDPTR)head_in->current->data, data_in, head_in->data_size);
   return (SUCCESS);
}

/* --------------------------------- */

publicdef INT llgetn (head_in, data_in, nth)

/* %S Retrieve data of Nth item */

/* %P
      Input Parameters:
         HEAD_IN   =   (LISTHEAD *)   pointer to list
         NTH       =   (INT)          position of item in list
      Output Parameters:
         DATA_IN   =   (any type *)   pointer to data
*/

LISTHEAD *head_in ;
VOIDPTR  data_in ;
INT nth ;

{  
    LL_NODE *curr_node ;
    INT index;
    INT distance;
    BOOL direc;

/* %? check for invalid list */
   if (head_in EQUALS NULL_LIST)
	{
	return (  LL_NULL_LIST_PTR);
	}
   if (head_in->pattern IS_NOT_EQUAL_TO LL_STANDARD_TEST_PATTERN)
	{
	return (  LL_INVALID_LIST_PTR);
	}
   if (head_in->entries EQUALS 0)
	{
	return (  LL_REQUEST_ON_EMPTY_LIST);
	}
   if ( (nth < 1) OR (nth > head_in->entries) )
	{
	return (  LL_INVALID_LIST_POS);
	}

/* figure out the shortest distance to go */

    index = head_in->index;
    distance = ABS(nth - index);
    if (distance > 0)
	{

		    /* see if it is quicker to start at the beginning */
	if (distance > (nth - 1))
	    {
	    distance = nth - 1;
	    index = 1;
	    curr_node = head_in->first;
	    }
		    /* see if it is quicker to start at the end */
	else if (distance > (head_in->entries - nth))
	    {
	    index = head_in->entries;
	    curr_node = head_in->last;
	    }
		    /* if not either of the above, then start where you area */
        else
	    {
	    curr_node = head_in->current;
	    }

		    /* go to the correct node */
	for (direc = index < nth;
	    index IS_NOT_EQUAL_TO nth;
	    )
	    {
	    if (direc)
		{
		curr_node = curr_node->next ;
		index++;
		}
	    else
		{
		curr_node = curr_node -> prev;
		index--;
		}
	    }

       head_in->current = curr_node ;
       head_in->index = index;
       }
       else
       {
	curr_node = head_in -> current;
       }

/* copy data from user area to list area */
   hoblockmove((VOIDPTR)curr_node->data, data_in, head_in->data_size);

   return (SUCCESS);
}

/* --------------------------------- */

publicdef INT llinsa (head_in, data_in)

/* %S Insert data AFTER current item */

/* %P
      Input Parameters:
         HEAD_IN   =   (LISTHEAD *) pointer to list
         DATA_IN   =   (any type *) pointer to data
*/

LISTHEAD *head_in ;
VOIDPTR  data_in ;

{  
   LL_NODE  *curr_node ;

/* %? check for invalid list */
   if (head_in EQUALS NULL_LIST)
	{
	return (  LL_NULL_LIST_PTR);
	}
   if (head_in->pattern IS_NOT_EQUAL_TO LL_STANDARD_TEST_PATTERN)
	{
	return (  LL_INVALID_LIST_PTR);
	}

    curr_node = ((LL_NODE *) llz_alloc(head_in->flags,
	sizeof(LL_NODE) + head_in->data_size - 1));

   if (head_in->entries EQUALS 0)
	{
	curr_node->prev         = NULL_NODE;
	curr_node->next         = NULL_NODE;
	head_in->current = curr_node ;
	head_in->first   = curr_node ;
	head_in->last    = curr_node ;
	}
   else
	{
	curr_node->prev         = head_in->current ;
	curr_node->next         = head_in->current->next ;
	if (head_in->entries EQUALS head_in->index)
	    {
	    head_in->last = curr_node ;
	    }
	else
	    {
	    head_in->current->next->prev = curr_node ;
	    }

	head_in->current->next = curr_node ;
	head_in->current  = curr_node ;
	}

    head_in->entries++ ;
    head_in->index++;

/* copy data from user area to list area */
    hoblockmove(data_in, (VOIDPTR)curr_node->data, head_in->data_size);

    return ( SUCCESS);
}

/* --------------------------------- */

publicdef INT llinsb (head_in, data_in)

/* %S Insert data BEFORE current item */

/* %P
      Input Parameters:
         HEAD_IN   =   (LISTHEAD *)   pointer to list
         DATA_IN   =   (any type *)   pointer to data
*/

LISTHEAD *head_in ;
VOIDPTR  data_in ;

{  
   LL_NODE  *curr_node ;

/* %? check for invalid list */
   if (head_in EQUALS NULL_LIST)
	{
	return ( LL_NULL_LIST_PTR);
	}
   if (head_in->pattern IS_NOT_EQUAL_TO LL_STANDARD_TEST_PATTERN)
	{
	return ( LL_INVALID_LIST_PTR);
	}

    curr_node = ((LL_NODE *) llz_alloc(head_in->flags,
	sizeof(LL_NODE) + head_in->data_size - 1));

   if (head_in->entries EQUALS 0)
	{
	curr_node->prev         = NULL_NODE;
	curr_node->next         = NULL_NODE;
        head_in->index = 1;
	head_in->current = curr_node ;
	head_in->first   = curr_node ;
	head_in->last    = curr_node ;
	}
   else
	{
	curr_node->next         = head_in->current ;
	curr_node->prev         = head_in->current->prev ;
	if (head_in->index EQUALS 1)
	    {
	    head_in->first = curr_node ;
	    }
	else
	    {
	    head_in->current->prev->next = curr_node ;
	    }

	head_in->current->prev = curr_node ;
	head_in->current  = curr_node ;
	}

    head_in->entries++ ;

/* copy data from user area to list area */
    hoblockmove(data_in, (VOIDPTR)curr_node->data, head_in->data_size);
    return ( SUCCESS);
}

/* --------------------------------- */

publicdef INT lllast (head_in, data_in)

/* %S Retrieve data of LAST item */

/* %P
      Input Parameters:
         HEAD_IN   =   (LISTHEAD *)   pointer to list
      Output Parameters:
         DATA_IN   =   (any type *)   pointer to data
*/

LISTHEAD *head_in ;
VOIDPTR  data_in ;

{  
/* %? check for invalid list */
   if (head_in EQUALS NULL_LIST)
	{
	return ( LL_NULL_LIST_PTR);
	}
   if (head_in->pattern IS_NOT_EQUAL_TO LL_STANDARD_TEST_PATTERN)
	{
	return ( LL_INVALID_LIST_PTR);
	}
   if (head_in->entries EQUALS 0)
	{
	return ( LL_REQUEST_ON_EMPTY_LIST);
	}

    head_in->current = head_in->last ;
    head_in->index = head_in->entries;

/* copy data from list area to user area */
    hoblockmove((VOIDPTR)head_in->current->data, data_in, head_in->data_size);
    return ( SUCCESS);
}

/* --------------------------------- */

publicdef INT llnext (head_in, data_in)

/* %S Retrieve data of item AFTER current item */

/* %P
        Input Parameters:
           HEAD_IN   =   (LISTHEAD *)   pointer to list
        Output Parameters:
           DATA_IN   =   (any type *)   pointer to data
           LLNEXT    =   (INT)          TRUE if last item
*/

LISTHEAD *head_in ;
VOIDPTR  data_in ;

{  
   INT return_var ;

/* %? check for invalid list */
   if (head_in EQUALS NULL_LIST)
	{
	return ( LL_NULL_LIST_PTR);
	}
   if (head_in->pattern IS_NOT_EQUAL_TO LL_STANDARD_TEST_PATTERN)
	{
	return ( LL_INVALID_LIST_PTR);
	}
   if (head_in->entries EQUALS 0)
	{
	return ( LL_REQUEST_ON_EMPTY_LIST);
	}

   if (head_in->entries IS_NOT_EQUAL_TO head_in->index)
	{
	head_in->current = head_in->current->next ;
	head_in->index++;
	return_var = FALSE;
	}
    else
	{
	return_var = TRUE;
	}

/* copy data from list area to user area */
    hoblockmove((VOIDPTR)head_in->current->data, data_in, head_in->data_size);


    return return_var;
}

/* --------------------------------- */

publicdef INT llprev (head_in, data_in)

/* %S Retrieve data of item BEFORE current item */

/* %P
      Input Parameters:
         HEAD_IN   =   (LISTHEAD *)   pointer to list
      Output Parameters:
         DATA_IN   =   (any type *)   pointer to data
         LLPREV    =   (INT)          TRUE if item is first
*/

LISTHEAD *head_in ;
VOIDPTR  data_in ;

{  
   INT return_var ;

/* %? check for invalid list */
   if (head_in EQUALS NULL_LIST)
	{
	return ( LL_NULL_LIST_PTR);
	}
   if (head_in->pattern IS_NOT_EQUAL_TO LL_STANDARD_TEST_PATTERN)
	{
	return ( LL_INVALID_LIST_PTR);
	}
   if (head_in->entries EQUALS 0)
	{
	return ( LL_REQUEST_ON_EMPTY_LIST);
	}

   if (head_in->index IS_NOT_EQUAL_TO 1)
	{
	head_in->current = head_in->current->prev ;
	head_in->index--;
	return_var = FALSE;
	}
    else
	{
	return_var = TRUE;
	}

/* copy data from list area to user area */
   hoblockmove((VOIDPTR)head_in->current->data, data_in, head_in->data_size);

   return return_var;
}

/* --------------------------------- */

publicdef INT llqcur(head_in)

/* %S Return position of CURRENT item */

/* %P
      Input Parameters:
         HEAD_IN   =   (LISTHEAD *)   pointer to list
      Output Parameters:
         LLQCUR    =   (INT)          position of current item
*/

LISTHEAD *head_in ;

{  
   INT return_var ;
    
/* %? check for invalid list */
   if (head_in EQUALS NULL_LIST)
	{
	return ( LL_NULL_LIST_PTR);
	}
   if (head_in->pattern IS_NOT_EQUAL_TO LL_STANDARD_TEST_PATTERN)
	{
	return ( LL_INVALID_LIST_PTR);
	}
   if (head_in->entries EQUALS 0)
	{
	return ( LL_REQUEST_ON_EMPTY_LIST);
	}

/* return  position of current item */
    return_var = head_in->index;

    return return_var;
}

/* --------------------------------- */

publicdef INT llreplc (head_in, data_in)

/* %S Replace data of CURRENT item */

/* %P
      Input Parameters:
         HEAD_IN   =   (LISTHEAD *)   pointer to list
         DATA_IN   =   (any typr *)   pointer to data
*/
LISTHEAD *head_in ;
VOIDPTR  data_in ;

{  
/* %? check for invalid list */
   if (head_in EQUALS NULL_LIST)
	{
	return ( LL_NULL_LIST_PTR);
	}
   if (head_in->pattern IS_NOT_EQUAL_TO LL_STANDARD_TEST_PATTERN)
	{
	return ( LL_INVALID_LIST_PTR);
	}
   if (head_in->entries EQUALS 0)
	{
	return ( LL_REQUEST_ON_EMPTY_LIST);
	}

/* copy from user area to list area */
    hoblockmove(data_in, (VOIDPTR)head_in->current->data, head_in->data_size);
    return ( SUCCESS);
}

/* --------------------------------- */

publicdef INT llcopy (source, destin)

/* %S Make a list the duplicate of another */

/* %P
      Input Parameters:
         SOURCE   =   (LISTHEAD *)   pointer to source list
      Output Parameters:
         DESTIN   =   (LISTHEAD *)   pointer to copy of list
*/

LISTHEAD *source, *destin ;

{  
   VOIDPTR the_data = NULL;
   INT count ;
   
/* %? check for invalid lists */
   if ( source EQUALS NULL_LIST OR
        destin EQUALS NULL_LIST )
	{
	return ( LL_NULL_LIST_PTR);
	}
   if ( source->pattern IS_NOT_EQUAL_TO LL_STANDARD_TEST_PATTERN OR
        destin->pattern IS_NOT_EQUAL_TO LL_STANDARD_TEST_PATTERN )
	{
	return ( LL_INVALID_LIST_PTR);
	}
   if (source->data_size IS_NOT_EQUAL_TO destin->data_size)
	{
	return ( LL_DATA_SIZES_DONT_MATCH);
	}

/* make sure the copy is empty first */
   if (destin->entries IS_NOT_EQUAL_TO 0)
	{
	llfree(destin) ;
	}

   count = source->entries ;
   if (count > 0)
   {
      llfirst(source, the_data) ;
      llappnd(destin, the_data) ;
      count-- ;
      while(count)
      {
         llnext(source, the_data) ;
         llappnd(destin, the_data) ;
         count-- ;
      }
   }
  return ( SUCCESS);
}

/* --------------------------------- */

publicdef INT llfree (head_in)

/* %S Delete all items in list */

/* %P
      Input Parameters:
         HEAD_IN   =   (LISTHEAD *)   pointer to list
*/

LISTHEAD *head_in ;

{  
   LL_NODE  *curr_node, *next_node ;
   INT flags;

/* %? check for invalid list */
   if (head_in EQUALS NULL_LIST)
	{
	return ( LL_NULL_LIST_PTR);
	}
   if (head_in->pattern IS_NOT_EQUAL_TO LL_STANDARD_TEST_PATTERN)
	{
	return ( LL_INVALID_LIST_PTR);
	}

/* free all memory allocated to the nodes */

   for (flags = head_in->flags, curr_node = head_in->first;
	curr_node IS_NOT_EQUAL_TO NULL_NODE;
	curr_node = next_node )
	{
	next_node = curr_node->next ;
	llz_free(flags, (VOIDPTR)curr_node) ;
	}

  tc_zeroes((VOIDPTR)head_in, sizeof (*head_in) );
  return ( SUCCESS);

}

/* --------------------------------- */

publicdef INT llinit ( head_in, type_in, d_siz_in, flags_in )

/* %S Initialize a list */

/* %P
      Input Parameters:
         HEAD_IN   =   (LISTHEAD *)   pointer to list
         TYPE_IN   =   (INT)          list type
         D_SIZ_IN  =   (INT)          size of data in bytes
         FLAGS_IN  =   (INT)          list control flags
*/

LISTHEAD *head_in ;
INT      type_in,  d_siz_in, flags_in ;

{      
   if (d_siz_in <= 0)
   {
     return ( LL_ILLEGAL_DATA_SIZE);
   }

   head_in->pattern   = LL_STANDARD_TEST_PATTERN ;
   head_in->flags     = flags_in;
   head_in->index     = 0;
   head_in->entries   = 0 ;
   head_in->data_size = d_siz_in ;
   head_in->current   = NULL_NODE;
   head_in->first     = NULL_NODE;
   head_in->last      = NULL_NODE;
  return ( SUCCESS);
}

/* --------------------------------- */

publicdef INT llquery(head_in, type_in, d_siz_in, flags_in, num_in)

/* %S Return status of list */

/* %P
      Input Parameters:
         HEAD_IN   =   (LISTHEAD *)   pointer to list
      Output Parameters:
         TYPE_IN   =   (INT *)      pointer to list type
         FLAGS_IN  =   (INT *)      pointer to list flags
         D_SIZ_IN  =   (INT *)        pointer to data size
         NUM_IN    =   (INT *)        pointer to list count
*/

LISTHEAD *head_in ;
INT *type_in, *flags_in ;
INT *d_siz_in, *num_in ;

{  
/* %? check for invalid list */
   if (head_in EQUALS NULL_LIST)
	{
	return ( LL_NULL_LIST_PTR);
	}
   if (head_in->pattern IS_NOT_EQUAL_TO LL_STANDARD_TEST_PATTERN)
	{
	return ( LL_INVALID_LIST_PTR);
	}

   *type_in  = 0;
   *d_siz_in = head_in->data_size ;
   *flags_in = head_in->flags ;
   *num_in   = head_in->entries ;

  return ( SUCCESS);
}
