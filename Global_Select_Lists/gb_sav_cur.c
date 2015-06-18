/* DEC/CMS REPLACEMENT HISTORY, Element GB_SAV_CUR.C*/
/* *4    30-JUL-1990 17:22:45 JULIAN "(SPR 0) add lynx,lease select_list"*/
/* *3     1-NOV-1989 10:12:00 GILLESPIE "(SPR 30) Adjust for include file changes; add pragmas to HO routines"*/
/* *2    11-AUG-1989 21:08:57 CONROY "(SPR 0) Global reorganization"*/
/* *1    19-JUN-1989 12:52:08 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element GB_SAV_CUR.C*/

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
#include "esi_gb.h"
#include "esi_nl.h"
#include "esi_qs.h"
#include "esi_am.h"
#include "esi_mg.h"

#include "esi_gb_err.h"

/* *************************************************************************

	ROUTINE: GB_SAVE_CURRENT_LIST

	DESCRIPTION:
		To save the current list.

	INPUT:
		DATA_TYPE =(SL_DATA_TYPE) The data type of current list.

	OUTPUT:
		SUCCESS;

	AUTHOR: James Chang		
   ************************************************************************* */
publicdef INT  gb_save_current_list( data_type )
SL_DATA_TYPE data_type;
{
    INT status, num_bytes;
    INT exists,qs_var;
    SELECT_KEY_STRING list_name;
    NLIST_HEADER current_list;

    status = SUCCESS;
    
    status = sl_inq_dtype_int("QS_LIST_EXISTS",data_type,&qs_var);
    if(status != SUCCESS)
         return GB_BAD_DATA_TYPE;
    qs_inq_i (qs_var, &exists);
    if( ! exists )
        {
	am_message(AM_ALERT, "There is no current list.");
	return status;
        }
    status = sl_inq_dtype_int("QS_CUR_LIST",data_type,&qs_var);
    qs_inq_c (qs_var, list_name, (INT *) 0);

					/* read the current list.	    */
    status = sl_read_list(data_type, list_name, &current_list );
    if( status )
    {   
	am_message( AM_ALERT, mg_message(status) );
        return status;
    }
    status = sl_select_description(current_list );
    if( status != SUCCESS )
    {    
        if ( status != CANCEL )
            {
 	    am_message( AM_ALERT, mg_message(status) );
            }
        return status;
    }

					/* save the current list.          */
    status = sl_save_list(current_list, 1);
    if (status != SUCCESS)
	am_message( AM_ALERT, mg_message(status) );
    status = nl_free_nlist(current_list );
    if (status != SUCCESS)
	am_message( AM_ALERT, mg_message(status) );
    return status;    
}
