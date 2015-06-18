/* DEC/CMS REPLACEMENT HISTORY, Element GB_UPD_STAT.C*/
/* *6    30-JUL-1990 17:22:49 JULIAN "(SPR 0) add lynx,lease select_list"*/
/* *5     1-NOV-1989 10:12:04 GILLESPIE "(SPR 30) Adjust for include file changes; add pragmas to HO routines"*/
/* *4    18-OCT-1989 10:23:45 JULIAN "(SPR 0) mod param passing on nl_inq_nlist_user_struct"*/
/* *3    11-AUG-1989 20:42:20 CONROY "(SPR 0) Revised global, selects, SQL macros & fixes"*/
/* *2    27-JUL-1989 12:33:49 GILLESPIE "(SPR 1) Add castings"*/
/* *1    19-JUN-1989 12:52:19 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element GB_UPD_STAT.C*/
/******************************************************************************

                Copyright Finder Graphics Systems, Inc. 1989
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

/* This routine takes an existing select list and makes it current. 
   If the select list is empty it calls GB_CLEAR_CURRENT_LIST to set a
   state of no-current-list.                                              */

#include "esi_c_lib.h"
#include "esi_gb.h"
#include "esi_nl.h"
#include "esi_qs.h"
#include "esi_gb_err.h"

publicdef INT  gb_update_current_status(select_list)
     NLIST_HEADER select_list;     /* nlist with user structure */

/*  The data_type and nlist name is removed from the structure and the
    nlist name is set into appropriate data type qs variable.
*/    
{
    SELECT_STRUCT temp_struct;
    INT status;
    size_t num_bytes;
    NLIST_ID list_name;
    SELECT_KEY_STRING item_name;
    UINT dim_list[1];
    VOIDPTR value_list[1];
    SL_DATA_TYPE data_type;
    INT nitems;
    INT qs_var;

    status = SUCCESS;
    
    status = nl_inq_nlist_user_struct( select_list, 
	sizeof( SELECT_STRUCT ), (VOIDPTR)&temp_struct, &num_bytes );
    if (status) return (status);
    
    data_type = temp_struct.data_type;
    strcpy( list_name, temp_struct.name );

    if ( nl_inq_nlist_int(select_list, NL_NLIST_NPOINTS, &nitems) 
         OR (nitems < 1) )
       return ( gb_clear_current_list(data_type) );

    dim_list[0] = 1;
    value_list[0] = (VOIDPTR)item_name;
    nl_first_point ( select_list, 1, dim_list, value_list);

    status = sl_inq_dtype_int("QS_LIST_EXISTS",data_type,&qs_var);
    if(status != SUCCESS)
         return ( GB_BAD_DATA_TYPE );
    qs_set_i (qs_var, TRUE);

    status = sl_inq_dtype_int("QS_CUR_COUNT",data_type,&qs_var);
    qs_set_i (qs_var, nitems);

    status = sl_inq_dtype_int("QS_CUR",data_type,&qs_var);
    qs_set_c (qs_var, item_name,0);

    status = sl_inq_dtype_int("QS_CUR_LIST",data_type,&qs_var);
    qs_set_c (qs_var, list_name,0);
    
    return (status);
}
